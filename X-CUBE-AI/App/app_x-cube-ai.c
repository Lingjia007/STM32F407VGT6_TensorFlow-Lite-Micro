
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
#include "micro_speech.h"
#include "micro_speech_data.h"

/* USER CODE BEGIN includes */
#include "audio_frontend.h"
#include "audio_stream.h"
#include "audio_params.h"
#include <math.h>
  /* USER CODE END includes */

  /* IO buffers ----------------------------------------------------------------*/

#if !defined(AI_MICRO_SPEECH_INPUTS_IN_ACTIVATIONS)
  AI_ALIGNED(4)
  ai_i8 data_in_1[AI_MICRO_SPEECH_IN_1_SIZE_BYTES];
  ai_i8 *data_ins[AI_MICRO_SPEECH_IN_NUM] = {
      data_in_1};
#else
ai_i8 *data_ins[AI_MICRO_SPEECH_IN_NUM] = {
    NULL};
#endif

#if !defined(AI_MICRO_SPEECH_OUTPUTS_IN_ACTIVATIONS)
  AI_ALIGNED(4)
  ai_i8 data_out_1[AI_MICRO_SPEECH_OUT_1_SIZE_BYTES];
  ai_i8 *data_outs[AI_MICRO_SPEECH_OUT_NUM] = {
      data_out_1};
#else
ai_i8 *data_outs[AI_MICRO_SPEECH_OUT_NUM] = {
    NULL};
#endif

  /* Activations buffers -------------------------------------------------------*/

  AI_ALIGNED(32)
  static uint8_t pool0[AI_MICRO_SPEECH_DATA_ACTIVATION_1_SIZE];

  ai_handle data_activations0[] = {pool0};

  /* AI objects ----------------------------------------------------------------*/

  static ai_handle micro_speech = AI_HANDLE_NULL;

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
    err = ai_micro_speech_create_and_init(&micro_speech, act_addr, NULL);
    if (err.type != AI_ERROR_NONE)
    {
      ai_log_err(err, "ai_micro_speech_create_and_init");
      return -1;
    }

    ai_input = ai_micro_speech_inputs_get(micro_speech, NULL);
    ai_output = ai_micro_speech_outputs_get(micro_speech, NULL);

#if defined(AI_MICRO_SPEECH_INPUTS_IN_ACTIVATIONS)
    /*  In the case where "--allocate-inputs" option is used, memory buffer can be
     *  used from the activations buffer. This is not mandatory.
     */
    for (int idx = 0; idx < AI_MICRO_SPEECH_IN_NUM; idx++)
    {
      data_ins[idx] = ai_input[idx].data;
    }
#else
  for (int idx = 0; idx < AI_MICRO_SPEECH_IN_NUM; idx++)
  {
    ai_input[idx].data = data_ins[idx];
  }
#endif

#if defined(AI_MICRO_SPEECH_OUTPUTS_IN_ACTIVATIONS)
    /*  In the case where "--allocate-outputs" option is used, memory buffer can be
     *  used from the activations buffer. This is no mandatory.
     */
    for (int idx = 0; idx < AI_MICRO_SPEECH_OUT_NUM; idx++)
    {
      data_outs[idx] = ai_output[idx].data;
    }
#else
  for (int idx = 0; idx < AI_MICRO_SPEECH_OUT_NUM; idx++)
  {
    ai_output[idx].data = data_outs[idx];
  }
#endif

    return 0;
  }

  static int ai_run(void)
  {
    ai_i32 batch;

    batch = ai_micro_speech_run(micro_speech, ai_input, ai_output);
    if (batch != 1)
    {
      ai_log_err(ai_micro_speech_get_error(micro_speech),
                 "ai_micro_speech_run");
      return -1;
    }

    return 0;
  }

  /* USER CODE BEGIN 2 */

#define NUM_LABELS 36

  static const char *const kLabelNames[NUM_LABELS] = {
      "_silence_", "_unknown_",
      "yes", "no", "up", "down", "left", "right", "stop", "go",
      "one", "two", "three", "four", "five", "six",
      "seven", "eight", "nine", "zero", "forward", "backward",
      "bed", "bird", "cat", "dog", "happy", "house", "learn",
      "marvin", "sheila", "tree", "visual", "wow", "follow", "off"};

  /* Streaming frontend context (1-sec window + features). Place in CCMRAM
   * to preserve main RAM for stack/heap/model activations. */
  static AudioFrontendStream g_stream __attribute__((section(".ccmram")));

  /* Scratch buffer for one slide worth of samples consumed from the ring. */
  static int16_t g_slide_buf[SLIDE_SAMPLES];

  /* ---- TIM5 32-bit timer for cycle-accurate timing (us) ---- */
  /* DWT CYCCNT is unreliable: audio_frontend/CMSIS-DSP internally disables it. */
  /* TIM5 is on APB1 (42MHz), timer clock = 84MHz when APB1 prescaler != 1.     */
// #define ENABLE_TIMING
#define TIM5_TIMER_CLOCK 84000000U

#ifdef ENABLE_TIMING
  static void timing_init(void)
  {
    __HAL_RCC_TIM5_CLK_ENABLE();
    TIM5->PSC = 0;           /* prescaler = 0, count at 84 MHz */
    TIM5->ARR = 0xFFFFFFFFU; /* max period (32-bit) */
    TIM5->CNT = 0;
    TIM5->CR1 = TIM_CR1_CEN; /* start counting, up-count mode */
  }

  /* timer ticks -> microseconds (TIM5 clock = 84 MHz) */
  static inline float ticks_to_us(uint32_t ticks)
  {
    return (float)ticks * 1000000.0f / (float)TIM5_TIMER_CLOCK;
  }

  static uint32_t g_pre_us_max = 0;
  static uint32_t g_inf_us_max = 0;
#endif /* ENABLE_TIMING */
  static uint32_t g_inference_cnt = 0;

  /* Returns 0 when features are ready for inference, -2 when the ring does
   * not yet have enough samples or the window is still filling. */
  int acquire_and_process_data(ai_i8 *data[])
  {
    AudioCaptureRingBuff_t *ring = AudioStream_get_ring();
    if ((int32_t)ring->availableSamples < (int32_t)SLIDE_SAMPLES)
      return -2;

    AudioCaptureRingBuff_consume((uint8_t *)g_slide_buf, ring, SLIDE_SAMPLES);

    int ready = audio_frontend_stream_push(&g_stream, g_slide_buf, SLIDE_SAMPLES);
    if (!ready)
      return -2; /* initial 1-second window not yet filled */

    memcpy(data[0], audio_frontend_stream_get_features(&g_stream),
           AI_MICRO_SPEECH_IN_1_SIZE_BYTES);
    return 0;
  }

  int post_process(ai_i8 *data[])
  {
    const int8_t *out = (const int8_t *)data[0];

    int max_idx = 0;
    int8_t max_val = out[0];
    for (int i = 1; i < NUM_LABELS; i++)
    {
      if (out[i] > max_val)
      {
        max_val = out[i];
        max_idx = i;
      }
    }

#ifdef ENABLE_TIMING
    g_inference_cnt++;
    if ((g_inference_cnt % 50) == 0)
    {
      printf("pre_max=%lu us, inf_max=%lu us, cnt=%lu\r\n",
             (unsigned long)g_pre_us_max, (unsigned long)g_inf_us_max,
             (unsigned long)g_inference_cnt);
    }
#else
  /* Softmax on raw int8 scores to get the winning probability. */
  float max_f = (float)max_val;
  float sum = 0.0f;
  float exp_vals[NUM_LABELS];
  for (int i = 0; i < NUM_LABELS; i++)
  {
    exp_vals[i] = expf((float)out[i] - max_f);
    sum += exp_vals[i];
  }
  float prob = exp_vals[max_idx] / sum;
  printf("[%lu] %s (%.1f%%, score=%d)\r\n",
         (unsigned long)g_inference_cnt, kLabelNames[max_idx],
         prob * 100.0f, max_val);
  g_inference_cnt++;
#endif /* ENABLE_TIMING */

    return 0;
  }
  /* USER CODE END 2 */

  /* Entry points --------------------------------------------------------------*/

  void MX_X_CUBE_AI_Init(void)
  {
    /* USER CODE BEGIN 5 */
    printf("\r\n=== micro_speech streaming ===\r\n");

    ai_boostrap(data_activations0);

    /* Initialize streaming audio frontend (1-sec sliding window) */
    audio_frontend_stream_init(&g_stream);
#ifdef ENABLE_TIMING
    timing_init();
#endif
    /* USER CODE END 5 */
  }

  void MX_X_CUBE_AI_Process(void)
  {
    /* USER CODE BEGIN 6 */
    if (!micro_speech)
      return;

    int res;
    do
    {
      /* 1 - acquire from ring + slide window + preprocess */
#ifdef ENABLE_TIMING
      uint32_t t0 = TIM5->CNT;
#endif
      res = acquire_and_process_data(data_ins);
      if (res == -2)
        break; /* ring empty or window still filling */
#ifdef ENABLE_TIMING
      uint32_t t1 = TIM5->CNT;
      uint32_t pre_us = (uint32_t)ticks_to_us(t1 - t0);
      if (pre_us > g_pre_us_max)
        g_pre_us_max = pre_us;
#endif
      /* 2 - run inference */
      if (res == 0)
      {
#ifdef ENABLE_TIMING
        uint32_t t2 = TIM5->CNT;
#endif
        res = ai_run();
#ifdef ENABLE_TIMING
        uint32_t t3 = TIM5->CNT;
        uint32_t inf_us = (uint32_t)ticks_to_us(t3 - t2);
        if (inf_us > g_inf_us_max)
          g_inf_us_max = inf_us;
#endif
      }
      /* 3 - post-process predictions */
      if (res == 0)
        res = post_process(data_outs);
    } while (res == 0);
    /* res == -2: not enough samples yet; return to main loop and retry. */
    /* USER CODE END 6 */
  }
#ifdef __cplusplus
}
#endif
