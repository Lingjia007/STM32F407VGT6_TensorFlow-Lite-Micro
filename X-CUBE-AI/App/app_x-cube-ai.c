
/**
 ******************************************************************************
 * @file    app_x-cube-ai.c
 * @author  X-CUBE-AI C code generator
 * @brief   AI program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

/*
 * Description
 *   v1.0 - Minimum template to show how to use the Embedded Client API
 *          model. Only one input and one output is supported. All
 *          memory resources are allocated statically (AI_NETWORK_XX, defines
 *          are used).
 *          Re-target of the printf function is out-of-scope.
 *   v2.0 - add multiple IO and/or multiple heap support
 *
 *   For more information, see the embeded documentation:
 *
 *       [1] %X_CUBE_AI_DIR%/Documentation/index.html
 *
 *   X_CUBE_AI_DIR indicates the location where the X-CUBE-AI pack is installed
 *   typical : C:\Users\[user_name]\STM32Cube\Repository\STMicroelectronics\X-CUBE-AI\7.1.0
 */

#ifdef __cplusplus
extern "C"
{
#endif

  /* Includes ------------------------------------------------------------------*/

#if defined(__ICCARM__)
#elif defined(__CC_ARM) || (__GNUC__)
#endif

/* System headers */
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>

#include "app_x-cube-ai.h"
#include "main.h"
#include "ai_datatypes_defines.h"
#include "hello_world.h"
#include "hello_world_data.h"
#include "micro_speech_quantized.h"
#include "micro_speech_quantized_data.h"

/* USER CODE BEGIN includes */
#include <math.h>
#include "arm_math.h"
#include "usart.h"
#include "micro_speech_tflm_preprocess.h"
#include "vofa_debug.h"

/* Debug output control: uncomment to enable printf debug output */
/* #define DEBUG_PRINTF_ENABLED */

/* ===== Application mode selection =====
 * 0 = hello_world (cos/sin inference)
 * 1 = micro_speech (TFLM preprocess + X-CUBE-AI classify)
 */
#define APP_MODE_MICRO_SPEECH 1
#define APP_MODE_HELLO_WORLD 0

#define APP_MODE APP_MODE_MICRO_SPEECH

  /* USER CODE END includes */

  /* IO buffers ----------------------------------------------------------------*/

#if !defined(AI_HELLO_WORLD_INPUTS_IN_ACTIVATIONS)
  AI_ALIGNED(4)
  ai_i8 data_in_1[AI_HELLO_WORLD_IN_1_SIZE_BYTES];
  ai_i8 *data_ins[AI_HELLO_WORLD_IN_NUM] = {
      data_in_1};
#else
ai_i8 *data_ins[AI_HELLO_WORLD_IN_NUM] = {
    NULL};
#endif

#if !defined(AI_HELLO_WORLD_OUTPUTS_IN_ACTIVATIONS)
  AI_ALIGNED(4)
  ai_i8 data_out_1[AI_HELLO_WORLD_OUT_1_SIZE_BYTES];
  ai_i8 *data_outs[AI_HELLO_WORLD_OUT_NUM] = {
      data_out_1};
#else
ai_i8 *data_outs[AI_HELLO_WORLD_OUT_NUM] = {
    NULL};
#endif

  /* Activations buffers -------------------------------------------------------*/

  AI_ALIGNED(32)
  static uint8_t pool0[AI_MICRO_SPEECH_QUANTIZED_DATA_ACTIVATION_1_SIZE];

  ai_handle data_activations0[] = {pool0};
  ai_handle data_activations1[] = {pool0};

  /* AI objects ----------------------------------------------------------------*/

  static ai_handle hello_world = AI_HANDLE_NULL;

  static ai_buffer *ai_input;
  static ai_buffer *ai_output;

  static void ai_log_err(const ai_error err, const char *fct)
  {
    /* USER CODE BEGIN log */
    if (fct)
      printf("TEMPLATE - Error (%s) - type=0x%02x code=0x%02x\r\n", fct,
             err.type, err.code);
    else
      printf("TEMPLATE - Error - type=0x%02x code=0x%02x\r\n", err.type, err.code);

    do
    {
    } while (1);
    /* USER CODE END log */
  }

  static int ai_boostrap(ai_handle *act_addr)
  {
    ai_error err;

    /* Create and initialize an instance of the model */
    err = ai_hello_world_create_and_init(&hello_world, act_addr, NULL);
    if (err.type != AI_ERROR_NONE)
    {
      ai_log_err(err, "ai_hello_world_create_and_init");
      return -1;
    }

    ai_input = ai_hello_world_inputs_get(hello_world, NULL);
    ai_output = ai_hello_world_outputs_get(hello_world, NULL);

#if defined(AI_HELLO_WORLD_INPUTS_IN_ACTIVATIONS)
    /*  In the case where "--allocate-inputs" option is used, memory buffer can be
     *  used from the activations buffer. This is not mandatory.
     */
    for (int idx = 0; idx < AI_HELLO_WORLD_IN_NUM; idx++)
    {
      data_ins[idx] = ai_input[idx].data;
    }
#else
  for (int idx = 0; idx < AI_HELLO_WORLD_IN_NUM; idx++)
  {
    ai_input[idx].data = data_ins[idx];
  }
#endif

#if defined(AI_HELLO_WORLD_OUTPUTS_IN_ACTIVATIONS)
    /*  In the case where "--allocate-outputs" option is used, memory buffer can be
     *  used from the activations buffer. This is no mandatory.
     */
    for (int idx = 0; idx < AI_HELLO_WORLD_OUT_NUM; idx++)
    {
      data_outs[idx] = ai_output[idx].data;
    }
#else
  for (int idx = 0; idx < AI_HELLO_WORLD_OUT_NUM; idx++)
  {
    ai_output[idx].data = data_outs[idx];
  }
#endif

    return 0;
  }

  static int ai_run(void)
  {
    ai_i32 batch;

    batch = ai_hello_world_run(hello_world, ai_input, ai_output);
    if (batch != 1)
    {
      ai_log_err(ai_hello_world_get_error(hello_world),
                 "ai_hello_world_run");
      return -1;
    }

    return 0;
  }

  /* USER CODE BEGIN 2 */

  /* --- hello_world mode implementation --- */
#if APP_MODE == APP_MODE_HELLO_WORLD
  /* Model selection: uncomment to use SIN model, default is COS model */
  // #define USE_SIN_MODEL

#ifdef USE_SIN_MODEL
#define INPUT_SCALE 0.024615485f
#define INPUT_ZEROPOINT -128
#define OUTPUT_SCALE 0.007883940f
#define OUTPUT_ZEROPOINT 0
#else
#define INPUT_SCALE 0.024624892f
#define INPUT_ZEROPOINT -128
#define OUTPUT_SCALE 0.007895931f
#define OUTPUT_ZEROPOINT -1
#endif

#define TEST_POINTS 3000
#define TWO_PI 6.28318530717958647692f

  static int test_counter = 0;
  static float total_error = 0.0f;
  static int test_complete = 0;

  static ai_i8 quantize_input(float value)
  {
    int quantized = (int)round(value / INPUT_SCALE) + INPUT_ZEROPOINT;
    if (quantized > 127)
      quantized = 127;
    if (quantized < -128)
      quantized = -128;
    return (ai_i8)quantized;
  }

  static float dequantize_output(ai_i8 value)
  {
    return (float)(value - OUTPUT_ZEROPOINT) * OUTPUT_SCALE;
  }

  int acquire_and_process_data(ai_i8 *data[])
  {
    if (test_complete)
      return -1;
    float angle = (float)test_counter * TWO_PI / (float)TEST_POINTS;
    ai_i8 quantized_input = quantize_input(angle);
    data[0][0] = quantized_input;
#ifdef DEBUG_PRINTF_ENABLED
    printf("[%03d] Angle: %.4f, Q_in: %d\r\n", test_counter, angle, quantized_input);
#endif
    return 0;
  }

  int post_process(ai_i8 *data[])
  {
    float angle = (float)test_counter * TWO_PI / (float)TEST_POINTS;
#ifdef USE_SIN_MODEL
    float expected_value = arm_sin_f32(angle);
#else
    float expected_value = arm_cos_f32(angle);
#endif
    ai_i8 quantized_output = data[0][0];
    float predicted_value = dequantize_output(quantized_output);
    float error = predicted_value - expected_value;
    total_error += fabsf(error);

    VOFA_Handle.fdata[0] = angle;
    VOFA_Handle.fdata[1] = predicted_value;
    VOFA_Handle.fdata[2] = expected_value;
    VOFA_Handle.fdata[3] = error;
    Vofa_Send();

#ifdef DEBUG_PRINTF_ENABLED
    printf("[%03d] Q_out: %d, Pred: %.4f, True: %.4f, Err: %.4f\r\n",
           test_counter, quantized_output, predicted_value, expected_value, error);
#endif
    test_counter++;
    if (test_counter >= TEST_POINTS)
    {
      test_complete = 1;
#ifdef DEBUG_PRINTF_ENABLED
      printf("\r\n=== Test Complete ===\r\n");
      printf("Average error: %.6f\r\n", total_error / (float)TEST_POINTS);
#endif
    }
    return 0;
  }
#endif /* APP_MODE_HELLO_WORLD */

  /* --- micro_speech mode implementation --- */
#if APP_MODE == APP_MODE_MICRO_SPEECH
  /* Category labels (12-class model, order from model output) */
#define MS_CATEGORY_COUNT 12
  static const char *kCategoryLabels[MS_CATEGORY_COUNT] = {
      "_silence_", "_unknown_", "yes", "no",
      "up", "down", "left", "right",
      "on", "off", "stop", "go"};

  /* X-CUBE-AI micro_speech_quantized model handles */
  static ai_handle micro_speech_net = AI_HANDLE_NULL;
  static ai_buffer *ms_ai_input = NULL;
  static ai_buffer *ms_ai_output = NULL;

  /* Output quantization parameters (from micro_speech_quantized model) */
#define MS_OUTPUT_SCALE 0.00390625f
#define MS_OUTPUT_ZEROPOINT (-128)

  /* Feature buffer: 1960 int8 = 49 frames x 40 features */
  static int8_t ms_features[TFLM_FEATURE_ELEMENT_COUNT];

  /* IO buffers for micro_speech_quantized */
  AI_ALIGNED(4)
  static ai_i8 ms_data_in[AI_MICRO_SPEECH_QUANTIZED_IN_1_SIZE_BYTES];
  AI_ALIGNED(4)
  static ai_i8 ms_data_out[AI_MICRO_SPEECH_QUANTIZED_OUT_1_SIZE_BYTES];

  /* Test state */
  typedef struct
  {
    const char *label;
    const int16_t *audio_data;
    size_t audio_data_size;
  } ms_test_sample_t;

  static ms_test_sample_t ms_test_samples[12];
#define MS_TEST_SAMPLE_COUNT 12

  static int ms_test_index = 0;
  static int ms_test_complete = 0;
  static int ms_pass_count = 0;
  static int ms_fail_count = 0;

  static void ms_init_test_samples(void)
  {
    ms_test_samples[0] = (ms_test_sample_t){"_silence_", tflm_test_audio_silence(), tflm_test_audio_silence_size()};
    ms_test_samples[1] = (ms_test_sample_t){"noise", tflm_test_audio_noise(), tflm_test_audio_noise_size()};
    ms_test_samples[2] = (ms_test_sample_t){"yes", tflm_test_audio_yes(), tflm_test_audio_yes_size()};
    ms_test_samples[3] = (ms_test_sample_t){"no", tflm_test_audio_no(), tflm_test_audio_no_size()};
    ms_test_samples[4] = (ms_test_sample_t){"up", tflm_test_audio_up(), tflm_test_audio_up_size()};
    ms_test_samples[5] = (ms_test_sample_t){"down", tflm_test_audio_down(), tflm_test_audio_down_size()};
    ms_test_samples[6] = (ms_test_sample_t){"left", tflm_test_audio_left(), tflm_test_audio_left_size()};
    ms_test_samples[7] = (ms_test_sample_t){"right", tflm_test_audio_rigth(), tflm_test_audio_rigth_size()};
    ms_test_samples[8] = (ms_test_sample_t){"on", tflm_test_audio_on(), tflm_test_audio_on_size()};
    ms_test_samples[9] = (ms_test_sample_t){"off", tflm_test_audio_off(), tflm_test_audio_off_size()};
    ms_test_samples[10] = (ms_test_sample_t){"stop", tflm_test_audio_stop(), tflm_test_audio_stop_size()};
    ms_test_samples[11] = (ms_test_sample_t){"go", tflm_test_audio_go(), tflm_test_audio_go_size()};
  }

  static int ms_bootstrap(ai_handle *act_addr)
  {
    ai_error err;
    err = ai_micro_speech_quantized_create_and_init(&micro_speech_net, act_addr, NULL);
    if (err.type != AI_ERROR_NONE)
    {
      printf("ms_bootstrap error: type=0x%02x code=0x%02x\r\n", err.type, err.code);
      return -1;
    }
    ms_ai_input = ai_micro_speech_quantized_inputs_get(micro_speech_net, NULL);
    ms_ai_output = ai_micro_speech_quantized_outputs_get(micro_speech_net, NULL);
    ms_ai_input[0].data = ms_data_in;
    ms_ai_output[0].data = ms_data_out;
    return 0;
  }

  static int ms_run_inference(void)
  {
    ai_i32 batch = ai_micro_speech_quantized_run(micro_speech_net, ms_ai_input, ms_ai_output);
    if (batch != 1)
    {
      ai_error err = ai_micro_speech_quantized_get_error(micro_speech_net);
      printf("ms_run error: type=0x%02x code=0x%02x\r\n", err.type, err.code);
      return -1;
    }
    return 0;
  }

  static int ms_test_sample(const ms_test_sample_t *sample)
  {
    printf("\r\n--- Testing: %s ---\r\n", sample->label);

    /* Step 1: TFLM audio preprocessing */
    if (micro_speech_tflm_generate_features(sample->audio_data,
                                            sample->audio_data_size,
                                            ms_features) != 0)
    {
      printf("Feature generation failed!\r\n");
      return -1;
    }

    /* Step 2: Copy features to X-CUBE-AI input buffer */
    memcpy(ms_data_in, ms_features, AI_MICRO_SPEECH_QUANTIZED_IN_1_SIZE_BYTES);

    /* Step 3: Run X-CUBE-AI inference */
    if (ms_run_inference() != 0)
    {
      printf("Inference failed!\r\n");
      return -2;
    }

    /* Step 4: Dequantize and print results */
    float category_predictions[MS_CATEGORY_COUNT];
    int max_idx = 0;
    float max_val = -999.0f;
    printf("Predictions for <%s>:\r\n", sample->label);
    for (int i = 0; i < MS_CATEGORY_COUNT; i++)
    {
      category_predictions[i] = (float)((int8_t)ms_data_out[i] - MS_OUTPUT_ZEROPOINT) * MS_OUTPUT_SCALE;
      printf("  %.4f %s\r\n", (double)category_predictions[i], kCategoryLabels[i]);
      if (category_predictions[i] > max_val)
      {
        max_val = category_predictions[i];
        max_idx = i;
      }
    }
    /* Determine if result is acceptable */
    int is_ok = 0;
    if (strcmp(sample->label, kCategoryLabels[max_idx]) == 0)
    {
      is_ok = 1; /* exact match */
    }
    else if (strcmp(sample->label, "noise") == 0 && strcmp(kCategoryLabels[max_idx], "_silence_") == 0)
    {
      is_ok = 1; /* noise -> silence is acceptable */
    }
    printf("Result: %s (%s)\r\n", kCategoryLabels[max_idx],
           is_ok ? "OK" : "MISMATCH");
    return is_ok ? 1 : 0;
  }
#endif /* APP_MODE_MICRO_SPEECH */

  /* USER CODE END 2 */

  /* Entry points --------------------------------------------------------------*/

  void MX_X_CUBE_AI_Init(void)
  {
    /* USER CODE BEGIN 5 */
    printf("\r\n=== AI Init ===\r\n");

#if APP_MODE == APP_MODE_HELLO_WORLD
    ai_boostrap(data_activations0);
#endif

#if APP_MODE == APP_MODE_MICRO_SPEECH
    /* Initialize TFLM audio preprocessor */
    int tflm_res = micro_speech_tflm_init();
    if (tflm_res != 0)
    {
      printf("TFLM init failed: %d\r\n", tflm_res);
    }
    /* Initialize X-CUBE-AI micro_speech_quantized model */
    ms_bootstrap(data_activations1);
    /* Initialize test samples */
    ms_init_test_samples();
#endif
    /* USER CODE END 5 */
  }

  void MX_X_CUBE_AI_Process(void)
  {
    /* USER CODE BEGIN 6 */
#if APP_MODE == APP_MODE_HELLO_WORLD
    int res = -1;
    if (hello_world && !test_complete)
    {
      res = acquire_and_process_data(data_ins);
      if (res == 0)
        res = ai_run();
      if (res == 0)
        res = post_process(data_outs);
      if (res && !test_complete)
      {
#ifdef DEBUG_PRINTF_ENABLED
        ai_error err = ai_hello_world_get_error(hello_world);
        printf("AI inference failed - type=0x%02x code=0x%02x\r\n", err.type, err.code);
#endif
      }
    }
#endif

#if APP_MODE == APP_MODE_MICRO_SPEECH
    if (!ms_test_complete && ms_test_index < (int)MS_TEST_SAMPLE_COUNT)
    {
      int result = ms_test_sample(&ms_test_samples[ms_test_index]);
      if (result > 0)
        ms_pass_count++;
      else
        ms_fail_count++;
      ms_test_index++;
      if (ms_test_index >= (int)MS_TEST_SAMPLE_COUNT)
      {
        ms_test_complete = 1;
        printf("\r\n=== All micro_speech tests done ===\r\n");
        printf("PASSED: %d/%d\r\n", ms_pass_count, (int)MS_TEST_SAMPLE_COUNT);
        printf("FAILED: %d/%d\r\n", ms_fail_count, (int)MS_TEST_SAMPLE_COUNT);
      }
    }
#endif
    /* USER CODE END 6 */
  }
  /* Multiple network support --------------------------------------------------*/

#include <string.h>
#include "ai_datatypes_defines.h"

  static const ai_network_entry_t networks[AI_MNETWORK_NUMBER] = {
      {.name = (const char *)AI_HELLO_WORLD_MODEL_NAME,
       .config = AI_HELLO_WORLD_DATA_CONFIG,
       .ai_get_report = ai_hello_world_get_report,
       .ai_create = ai_hello_world_create,
       .ai_destroy = ai_hello_world_destroy,
       .ai_get_error = ai_hello_world_get_error,
       .ai_init = ai_hello_world_init,
       .ai_run = ai_hello_world_run,
       .ai_forward = ai_hello_world_forward,
       .ai_data_params_get = ai_hello_world_data_params_get,
       .activations = data_activations0},
      {.name = (const char *)AI_MICRO_SPEECH_QUANTIZED_MODEL_NAME,
       .config = AI_MICRO_SPEECH_QUANTIZED_DATA_CONFIG,
       .ai_get_report = ai_micro_speech_quantized_get_report,
       .ai_create = ai_micro_speech_quantized_create,
       .ai_destroy = ai_micro_speech_quantized_destroy,
       .ai_get_error = ai_micro_speech_quantized_get_error,
       .ai_init = ai_micro_speech_quantized_init,
       .ai_run = ai_micro_speech_quantized_run,
       .ai_forward = ai_micro_speech_quantized_forward,
       .ai_data_params_get = ai_micro_speech_quantized_data_params_get,
       .activations = data_activations1},
  };

  struct network_instance
  {
    const ai_network_entry_t *entry;
    ai_handle handle;
    ai_network_params params;
  };

  /* Number of instance is aligned on the number of network */
  AI_STATIC struct network_instance gnetworks[AI_MNETWORK_NUMBER] = {0};

  AI_DECLARE_STATIC
  ai_bool ai_mnetwork_is_valid(const char *name,
                               const ai_network_entry_t *entry)
  {
    if (name && (strlen(entry->name) == strlen(name)) &&
        (strncmp(entry->name, name, strlen(entry->name)) == 0))
      return true;
    return false;
  }

  AI_DECLARE_STATIC
  struct network_instance *ai_mnetwork_handle(struct network_instance *inst)
  {
    for (int i = 0; i < AI_MNETWORK_NUMBER; i++)
    {
      if ((inst) && (&gnetworks[i] == inst))
        return inst;
      else if ((!inst) && (gnetworks[i].entry == NULL))
        return &gnetworks[i];
    }
    return NULL;
  }

  AI_DECLARE_STATIC
  void ai_mnetwork_release_handle(struct network_instance *inst)
  {
    for (int i = 0; i < AI_MNETWORK_NUMBER; i++)
    {
      if ((inst) && (&gnetworks[i] == inst))
      {
        gnetworks[i].entry = NULL;
        return;
      }
    }
  }

  AI_API_ENTRY
  const char *ai_mnetwork_find(const char *name, ai_int idx)
  {
    const ai_network_entry_t *entry;

    for (int i = 0; i < AI_MNETWORK_NUMBER; i++)
    {
      entry = &networks[i];
      if (ai_mnetwork_is_valid(name, entry))
        return entry->name;
      else
      {
        if (!idx--)
          return entry->name;
      }
    }
    return NULL;
  }

  AI_API_ENTRY
  ai_error ai_mnetwork_create(const char *name, ai_handle *network,
                              const ai_buffer *network_config)
  {
    const ai_network_entry_t *entry;
    const ai_network_entry_t *found = NULL;
    ai_error err;
    struct network_instance *inst = ai_mnetwork_handle(NULL);

    if (!inst)
    {
      err.type = AI_ERROR_ALLOCATION_FAILED;
      err.code = AI_ERROR_CODE_NETWORK;
      return err;
    }

    for (int i = 0; i < AI_MNETWORK_NUMBER; i++)
    {
      entry = &networks[i];
      if (ai_mnetwork_is_valid(name, entry))
      {
        found = entry;
        break;
      }
    }

    if (!found)
    {
      err.type = AI_ERROR_INVALID_PARAM;
      err.code = AI_ERROR_CODE_NETWORK;
      return err;
    }

    if (network_config == NULL)
      err = found->ai_create(network, found->config);
    else
      err = found->ai_create(network, network_config);
    if ((err.code == AI_ERROR_CODE_NONE) && (err.type == AI_ERROR_NONE))
    {
      inst->entry = found;
      inst->handle = *network;
      *network = (ai_handle *)inst;
    }

    return err;
  }

  AI_API_ENTRY
  ai_handle ai_mnetwork_destroy(ai_handle network)
  {
    struct network_instance *inn;
    inn = ai_mnetwork_handle((struct network_instance *)network);
    if (inn)
    {
      ai_handle hdl = inn->entry->ai_destroy(inn->handle);
      if (hdl != inn->handle)
      {
        ai_mnetwork_release_handle(inn);
        network = AI_HANDLE_NULL;
      }
    }
    return network;
  }

  AI_API_ENTRY
  ai_bool ai_mnetwork_get_report(ai_handle network, ai_network_report *report)
  {
    struct network_instance *inn;
    inn = ai_mnetwork_handle((struct network_instance *)network);
    if (inn)
      return inn->entry->ai_get_report(inn->handle, report);
    else
      return false;
  }

  AI_API_ENTRY
  ai_error ai_mnetwork_get_error(ai_handle network)
  {
    struct network_instance *inn;
    ai_error err;
    err.type = AI_ERROR_INVALID_PARAM;
    err.code = AI_ERROR_CODE_NETWORK;

    inn = ai_mnetwork_handle((struct network_instance *)network);
    if (inn)
      return inn->entry->ai_get_error(inn->handle);
    else
      return err;
  }

  AI_API_ENTRY
  ai_bool ai_mnetwork_init(ai_handle network)
  {
    struct network_instance *inn;
    ai_network_params par;

    inn = ai_mnetwork_handle((struct network_instance *)network);
    if (inn)
    {
      inn->entry->ai_data_params_get(&par);
      for (int idx = 0; idx < par.map_activations.size; idx++)
        AI_BUFFER_ARRAY_ITEM_SET_ADDRESS(&par.map_activations, idx, inn->entry->activations[idx]);
      return inn->entry->ai_init(inn->handle, &par);
    }
    else
      return false;
  }

  AI_API_ENTRY
  ai_i32 ai_mnetwork_run(ai_handle network, const ai_buffer *input,
                         ai_buffer *output)
  {
    struct network_instance *inn;
    inn = ai_mnetwork_handle((struct network_instance *)network);
    if (inn)
      return inn->entry->ai_run(inn->handle, input, output);
    else
      return 0;
  }

  AI_API_ENTRY
  ai_i32 ai_mnetwork_forward(ai_handle network, const ai_buffer *input)
  {
    struct network_instance *inn;
    inn = ai_mnetwork_handle((struct network_instance *)network);
    if (inn)
      return inn->entry->ai_forward(inn->handle, input);
    else
      return 0;
  }

  AI_API_ENTRY
  int ai_mnetwork_get_private_handle(ai_handle network,
                                     ai_handle *phandle,
                                     ai_network_params *pparams)
  {
    struct network_instance *inn;
    inn = ai_mnetwork_handle((struct network_instance *)network);
    if (inn && phandle && pparams)
    {
      *phandle = inn->handle;
      *pparams = inn->params;
      return 0;
    }
    else
      return -1;
  }

#ifdef __cplusplus
}
#endif
