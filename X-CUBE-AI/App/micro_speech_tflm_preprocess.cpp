#include "micro_speech_tflm_preprocess.h"

#include <algorithm>
#include <cstdint>

#include "tensorflow/lite/core/c/common.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_log.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"

/* Audio preprocessor int8 model data */
#include "audio_preprocessor_int8_model_data.h"

/* Test audio data (C++ headers) */
#include "yes_1000ms_audio_data.h"
#include "no_1000ms_audio_data.h"
#include "silence_1000ms_audio_data.h"
#include "noise_1000ms_audio_data.h"

namespace {

constexpr int kAudioSampleFrequency = TFLM_AUDIO_SAMPLE_FREQUENCY;
constexpr int kFeatureSize = TFLM_FEATURE_SIZE;
constexpr int kFeatureCount = TFLM_FEATURE_COUNT;
constexpr int kFeatureStrideMs = TFLM_FEATURE_STRIDE_MS;
constexpr int kFeatureDurationMs = TFLM_FEATURE_DURATION_MS;

constexpr int kAudioSampleDurationCount =
    kFeatureDurationMs * kAudioSampleFrequency / 1000;
constexpr int kAudioSampleStrideCount =
    kFeatureStrideMs * kAudioSampleFrequency / 1000;

/* Arena size for audio_preprocessor model (determined empirically) */
constexpr size_t kArenaSize = 28584;
alignas(16) uint8_t g_arena[kArenaSize];

using AudioPreprocessorOpResolver = tflite::MicroMutableOpResolver<18>;

tflite::MicroInterpreter *g_interpreter = nullptr;

TfLiteStatus RegisterOps(AudioPreprocessorOpResolver &op_resolver) {
  TF_LITE_ENSURE_STATUS(op_resolver.AddReshape());
  TF_LITE_ENSURE_STATUS(op_resolver.AddCast());
  TF_LITE_ENSURE_STATUS(op_resolver.AddStridedSlice());
  TF_LITE_ENSURE_STATUS(op_resolver.AddConcatenation());
  TF_LITE_ENSURE_STATUS(op_resolver.AddMul());
  TF_LITE_ENSURE_STATUS(op_resolver.AddAdd());
  TF_LITE_ENSURE_STATUS(op_resolver.AddDiv());
  TF_LITE_ENSURE_STATUS(op_resolver.AddMinimum());
  TF_LITE_ENSURE_STATUS(op_resolver.AddMaximum());
  TF_LITE_ENSURE_STATUS(op_resolver.AddWindow());
  TF_LITE_ENSURE_STATUS(op_resolver.AddFftAutoScale());
  TF_LITE_ENSURE_STATUS(op_resolver.AddRfft());
  TF_LITE_ENSURE_STATUS(op_resolver.AddEnergy());
  TF_LITE_ENSURE_STATUS(op_resolver.AddFilterBank());
  TF_LITE_ENSURE_STATUS(op_resolver.AddFilterBankSquareRoot());
  TF_LITE_ENSURE_STATUS(op_resolver.AddFilterBankSpectralSubtraction());
  TF_LITE_ENSURE_STATUS(op_resolver.AddPCAN());
  TF_LITE_ENSURE_STATUS(op_resolver.AddFilterBankLog());
  return kTfLiteOk;
}

TfLiteStatus GenerateSingleFeature(const int16_t *audio_data,
                                   const int audio_data_size,
                                   int8_t *feature_output,
                                   tflite::MicroInterpreter *interpreter) {
  TfLiteTensor *input = interpreter->input(0);
  if (input == nullptr) {
    MicroPrintf("Input tensor is null!");
    return kTfLiteError;
  }
  if (kAudioSampleDurationCount != audio_data_size) {
    MicroPrintf("Audio data size mismatch!");
    return kTfLiteError;
  }
  if (kAudioSampleDurationCount != input->dims->data[input->dims->size - 1]) {
    MicroPrintf("Input dims mismatch!");
    return kTfLiteError;
  }

  TfLiteTensor *output = interpreter->output(0);
  if (output == nullptr) {
    MicroPrintf("Output tensor is null!");
    return kTfLiteError;
  }
  if (kFeatureSize != output->dims->data[output->dims->size - 1]) {
    MicroPrintf("Feature size mismatch!");
    return kTfLiteError;
  }

  std::copy_n(audio_data, audio_data_size,
              tflite::GetTensorData<int16_t>(input));
  TF_LITE_ENSURE_STATUS(interpreter->Invoke());
  std::copy_n(tflite::GetTensorData<int8_t>(output), kFeatureSize,
              feature_output);

  return kTfLiteOk;
}

} // namespace

int micro_speech_tflm_init(void) {
  const tflite::Model *model =
      tflite::GetModel(g_audio_preprocessor_int8_model_data);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    MicroPrintf("Audio preprocessor model version mismatch!");
    return -1;
  }

  static AudioPreprocessorOpResolver op_resolver;
  if (RegisterOps(op_resolver) != kTfLiteOk) {
    MicroPrintf("Failed to register audio preprocessor ops!");
    return -2;
  }

  static tflite::MicroInterpreter interpreter(model, op_resolver, g_arena,
                                              kArenaSize);

  if (interpreter.AllocateTensors() != kTfLiteOk) {
    MicroPrintf("AllocateTensors failed!");
    return -3;
  }

  MicroPrintf("AudioPreprocessor arena used = %u",
              interpreter.arena_used_bytes());

  g_interpreter = &interpreter;
  return 0;
}

int micro_speech_tflm_generate_features(const int16_t *audio_data,
                                         size_t audio_data_size,
                                         int8_t *features_out) {
  if (g_interpreter == nullptr) {
    MicroPrintf("TFLM not initialized!");
    return -1;
  }

  size_t remaining_samples = audio_data_size;
  size_t feature_index = 0;
  while (remaining_samples >= (size_t)kAudioSampleDurationCount &&
         feature_index < (size_t)kFeatureCount) {
    if (GenerateSingleFeature(audio_data, kAudioSampleDurationCount,
                              features_out + feature_index * kFeatureSize,
                              g_interpreter) != kTfLiteOk) {
      MicroPrintf("GenerateSingleFeature failed at index %u",
                  (unsigned)feature_index);
      return -2;
    }
    feature_index++;
    audio_data += kAudioSampleStrideCount;
    remaining_samples -= kAudioSampleStrideCount;
  }

  return 0;
}

/* C-callable test data accessors */
extern "C" {

const int16_t *tflm_test_audio_yes(void) { return g_yes_1000ms_audio_data; }
size_t tflm_test_audio_yes_size(void) { return g_yes_1000ms_audio_data_size; }

const int16_t *tflm_test_audio_no(void) { return g_no_1000ms_audio_data; }
size_t tflm_test_audio_no_size(void) { return g_no_1000ms_audio_data_size; }

const int16_t *tflm_test_audio_silence(void) { return g_silence_1000ms_audio_data; }
size_t tflm_test_audio_silence_size(void) { return g_silence_1000ms_audio_data_size; }

const int16_t *tflm_test_audio_noise(void) { return g_noise_1000ms_audio_data; }
size_t tflm_test_audio_noise_size(void) { return g_noise_1000ms_audio_data_size; }

} /* extern "C" */
