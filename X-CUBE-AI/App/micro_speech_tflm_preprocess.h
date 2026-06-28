#ifndef MICRO_SPEECH_TFLM_PREPROCESS_H
#define MICRO_SPEECH_TFLM_PREPROCESS_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Model settings from micro_model_settings.h */
#define TFLM_AUDIO_SAMPLE_FREQUENCY  16000
#define TFLM_FEATURE_SIZE            40
#define TFLM_FEATURE_COUNT           49
#define TFLM_FEATURE_ELEMENT_COUNT   (TFLM_FEATURE_SIZE * TFLM_FEATURE_COUNT)
#define TFLM_FEATURE_STRIDE_MS       20
#define TFLM_FEATURE_DURATION_MS     30

#define TFLM_CATEGORY_COUNT          4

/* Initialize TFLM audio preprocessor. Returns 0 on success. */
int micro_speech_tflm_init(void);

/* Generate features from 1000ms audio data.
 * audio_data: 16000 int16 samples
 * features_out: must be at least TFLM_FEATURE_ELEMENT_COUNT (1960) int8
 * Returns 0 on success. */
int micro_speech_tflm_generate_features(const int16_t *audio_data,
                                         size_t audio_data_size,
                                         int8_t *features_out);

/* Test data accessors (C-callable wrappers around C++ test data) */
extern const int16_t *tflm_test_audio_yes(void);
extern size_t tflm_test_audio_yes_size(void);
extern const int16_t *tflm_test_audio_no(void);
extern size_t tflm_test_audio_no_size(void);
extern const int16_t *tflm_test_audio_silence(void);
extern size_t tflm_test_audio_silence_size(void);
extern const int16_t *tflm_test_audio_noise(void);
extern size_t tflm_test_audio_noise_size(void);
extern const int16_t *tflm_test_audio_down(void);
extern size_t tflm_test_audio_down_size(void);
extern const int16_t *tflm_test_audio_go(void);
extern size_t tflm_test_audio_go_size(void);
extern const int16_t *tflm_test_audio_left(void);
extern size_t tflm_test_audio_left_size(void);
extern const int16_t *tflm_test_audio_off(void);
extern size_t tflm_test_audio_off_size(void);
extern const int16_t *tflm_test_audio_on(void);
extern size_t tflm_test_audio_on_size(void);
extern const int16_t *tflm_test_audio_rigth(void);
extern size_t tflm_test_audio_rigth_size(void);
extern const int16_t *tflm_test_audio_stop(void);
extern size_t tflm_test_audio_stop_size(void);
extern const int16_t *tflm_test_audio_up(void);
extern size_t tflm_test_audio_up_size(void);

#ifdef __cplusplus
}
#endif

#endif /* MICRO_SPEECH_TFLM_PREPROCESS_H */
