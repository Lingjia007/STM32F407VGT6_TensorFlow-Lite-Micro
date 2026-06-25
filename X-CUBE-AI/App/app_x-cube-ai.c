
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
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#if defined ( __ICCARM__ )
#elif defined ( __CC_ARM ) || ( __GNUC__ )
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

/* VOFA+ fdata format structure */
#define VOFA_CH_COUNT 4

  /* Debug output control: uncomment to enable printf debug output */
  /* #define DEBUG_PRINTF_ENABLED */

  typedef struct
  {
    float fdata[VOFA_CH_COUNT];
    const unsigned char tail[4];
  } VOFA_Send_Handle_t;

  static VOFA_Send_Handle_t VOFA_Handle = {
      .tail = {0x00, 0x00, 0x80, 0x7f}};

  static void Vofa_Send(void)
  {
    HAL_UART_Transmit(&huart4, (uint8_t *)&VOFA_Handle, sizeof(VOFA_Handle), HAL_MAX_DELAY);
  }
/* USER CODE END includes */

/* IO buffers ----------------------------------------------------------------*/

#if !defined(AI_HELLO_WORLD_INPUTS_IN_ACTIVATIONS)
AI_ALIGNED(4) ai_i8 data_in_1[AI_HELLO_WORLD_IN_1_SIZE_BYTES];
ai_i8* data_ins[AI_HELLO_WORLD_IN_NUM] = {
data_in_1
};
#else
ai_i8* data_ins[AI_HELLO_WORLD_IN_NUM] = {
NULL
};
#endif

#if !defined(AI_HELLO_WORLD_OUTPUTS_IN_ACTIVATIONS)
AI_ALIGNED(4) ai_i8 data_out_1[AI_HELLO_WORLD_OUT_1_SIZE_BYTES];
ai_i8* data_outs[AI_HELLO_WORLD_OUT_NUM] = {
data_out_1
};
#else
ai_i8* data_outs[AI_HELLO_WORLD_OUT_NUM] = {
NULL
};
#endif

/* Activations buffers -------------------------------------------------------*/

AI_ALIGNED(32)
static uint8_t pool0[AI_MICRO_SPEECH_QUANTIZED_DATA_ACTIVATION_1_SIZE];

ai_handle data_activations0[] = {pool0};
ai_handle data_activations1[] = {pool0};

/* AI objects ----------------------------------------------------------------*/

static ai_handle hello_world = AI_HANDLE_NULL;

static ai_buffer* ai_input;
static ai_buffer* ai_output;

static void ai_log_err(const ai_error err, const char *fct)
{
  /* USER CODE BEGIN log */
#ifdef DEBUG_PRINTF_ENABLED
    if (fct)
      printf("TEMPLATE - Error (%s) - type=0x%02x code=0x%02x\r\n", fct,
             err.type, err.code);
    else
      printf("TEMPLATE - Error - type=0x%02x code=0x%02x\r\n", err.type, err.code);
#endif

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
  if (err.type != AI_ERROR_NONE) {
    ai_log_err(err, "ai_hello_world_create_and_init");
    return -1;
  }

  ai_input = ai_hello_world_inputs_get(hello_world, NULL);
  ai_output = ai_hello_world_outputs_get(hello_world, NULL);

#if defined(AI_HELLO_WORLD_INPUTS_IN_ACTIVATIONS)
  /*  In the case where "--allocate-inputs" option is used, memory buffer can be
   *  used from the activations buffer. This is not mandatory.
   */
  for (int idx=0; idx < AI_HELLO_WORLD_IN_NUM; idx++) {
	data_ins[idx] = ai_input[idx].data;
  }
#else
  for (int idx=0; idx < AI_HELLO_WORLD_IN_NUM; idx++) {
	  ai_input[idx].data = data_ins[idx];
  }
#endif

#if defined(AI_HELLO_WORLD_OUTPUTS_IN_ACTIVATIONS)
  /*  In the case where "--allocate-outputs" option is used, memory buffer can be
   *  used from the activations buffer. This is no mandatory.
   */
  for (int idx=0; idx < AI_HELLO_WORLD_OUT_NUM; idx++) {
	data_outs[idx] = ai_output[idx].data;
  }
#else
  for (int idx=0; idx < AI_HELLO_WORLD_OUT_NUM; idx++) {
	ai_output[idx].data = data_outs[idx];
  }
#endif

  return 0;
}

static int ai_run(void)
{
  ai_i32 batch;

  batch = ai_hello_world_run(hello_world, ai_input, ai_output);
  if (batch != 1) {
    ai_log_err(ai_hello_world_get_error(hello_world),
        "ai_hello_world_run");
    return -1;
  }

  return 0;
}

/* USER CODE BEGIN 2 */
  /* Model selection: uncomment to use SIN model, default is COS model */
  // #define USE_SIN_MODEL

#ifdef USE_SIN_MODEL
/* SIN model quantization parameters (from hello_world_int8_sin.tflite) */
#define INPUT_SCALE 0.024615485f
#define INPUT_ZEROPOINT -128
#define OUTPUT_SCALE 0.007883940f
#define OUTPUT_ZEROPOINT 0
#else
/* COS model quantization parameters (from hello_world_int8.tflite) */
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

  /* Quantize float value to int8 */
  static ai_i8 quantize_input(float value)
  {
    int quantized = (int)round(value / INPUT_SCALE) + INPUT_ZEROPOINT;
    if (quantized > 127)
      quantized = 127;
    if (quantized < -128)
      quantized = -128;
    return (ai_i8)quantized;
  }

  /* Dequantize int8 to float */
  static float dequantize_output(ai_i8 value)
  {
    return (float)(value - OUTPUT_ZEROPOINT) * OUTPUT_SCALE;
  }

  int acquire_and_process_data(ai_i8 *data[])
  {
    if (test_complete)
      return -1;

    /* Calculate angle: 0 to 2PI over TEST_POINTS */
    float angle = (float)test_counter * TWO_PI / (float)TEST_POINTS;

    /* Quantize input */
    ai_i8 quantized_input = quantize_input(angle);
    data[0][0] = quantized_input;

#ifdef DEBUG_PRINTF_ENABLED
    printf("[%03d] Angle: %.4f, Q_in: %d\r\n", test_counter, angle, quantized_input);
#endif

    return 0;
  }

  int post_process(ai_i8 *data[])
  {
    /* Calculate expected value using ARM DSP library */
    float angle = (float)test_counter * TWO_PI / (float)TEST_POINTS;
#ifdef USE_SIN_MODEL
    float expected_value = arm_sin_f32(angle);
#else
  float expected_value = arm_cos_f32(angle);
#endif

    /* Dequantize output */
    ai_i8 quantized_output = data[0][0];
    float predicted_value = dequantize_output(quantized_output);

    /* Calculate error */
    float error = predicted_value - expected_value;
    total_error += fabsf(error);

    /* Send data via VOFA+ fdata format */
    VOFA_Handle.fdata[0] = angle;           /* Channel 0: Input angle */
    VOFA_Handle.fdata[1] = predicted_value; /* Channel 1: Predicted value */
    VOFA_Handle.fdata[2] = expected_value;  /* Channel 2: True value */
    VOFA_Handle.fdata[3] = error;           /* Channel 3: Error */
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
      printf("Total points: %d\r\n", TEST_POINTS);
      printf("Average error: %.6f\r\n", total_error / (float)TEST_POINTS);
      printf("Total error: %.6f\r\n", total_error);
#endif
    }

    return 0;
  }
/* USER CODE END 2 */

/* Entry points --------------------------------------------------------------*/

void MX_X_CUBE_AI_Init(void)
{
    /* USER CODE BEGIN 5 */
#ifdef DEBUG_PRINTF_ENABLED
    printf("\r\nTEMPLATE - initialization\r\n");
#endif

    ai_boostrap(data_activations0);
    /* USER CODE END 5 */
}

void MX_X_CUBE_AI_Process(void)
{
    /* USER CODE BEGIN 6 */
    int res = -1;

    if (hello_world && !test_complete)
    {
      /* 1 - acquire and pre-process input data */
      res = acquire_and_process_data(data_ins);
      /* 2 - process the data - call inference engine */
      if (res == 0)
        res = ai_run();
      /* 3- post-process the predictions */
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
    /* USER CODE END 6 */
}
/* Multiple network support --------------------------------------------------*/

#include <string.h>
#include "ai_datatypes_defines.h"

static const ai_network_entry_t networks[AI_MNETWORK_NUMBER] = {
    {
        .name = (const char *)AI_HELLO_WORLD_MODEL_NAME,
        .config = AI_HELLO_WORLD_DATA_CONFIG,
        .ai_get_report = ai_hello_world_get_report,
        .ai_create = ai_hello_world_create,
        .ai_destroy = ai_hello_world_destroy,
        .ai_get_error = ai_hello_world_get_error,
        .ai_init = ai_hello_world_init,
        .ai_run = ai_hello_world_run,
        .ai_forward = ai_hello_world_forward,
        .ai_data_params_get = ai_hello_world_data_params_get,
        .activations = data_activations0
    },
    {
        .name = (const char *)AI_MICRO_SPEECH_QUANTIZED_MODEL_NAME,
        .config = AI_MICRO_SPEECH_QUANTIZED_DATA_CONFIG,
        .ai_get_report = ai_micro_speech_quantized_get_report,
        .ai_create = ai_micro_speech_quantized_create,
        .ai_destroy = ai_micro_speech_quantized_destroy,
        .ai_get_error = ai_micro_speech_quantized_get_error,
        .ai_init = ai_micro_speech_quantized_init,
        .ai_run = ai_micro_speech_quantized_run,
        .ai_forward = ai_micro_speech_quantized_forward,
        .ai_data_params_get = ai_micro_speech_quantized_data_params_get,
        .activations = data_activations1
    },
};

struct network_instance {
     const ai_network_entry_t *entry;
     ai_handle handle;
     ai_network_params params;
};

/* Number of instance is aligned on the number of network */
AI_STATIC struct network_instance gnetworks[AI_MNETWORK_NUMBER] = {0};

AI_DECLARE_STATIC
ai_bool ai_mnetwork_is_valid(const char* name,
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
    for (int i=0; i<AI_MNETWORK_NUMBER; i++) {
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
    for (int i=0; i<AI_MNETWORK_NUMBER; i++) {
        if ((inst) && (&gnetworks[i] == inst)) {
            gnetworks[i].entry = NULL;
            return;
        }
    }
}

AI_API_ENTRY
const char* ai_mnetwork_find(const char *name, ai_int idx)
{
    const ai_network_entry_t *entry;

    for (int i=0; i<AI_MNETWORK_NUMBER; i++) {
        entry = &networks[i];
        if (ai_mnetwork_is_valid(name, entry))
            return entry->name;
        else {
            if (!idx--)
                return entry->name;
        }
    }
    return NULL;
}

AI_API_ENTRY
ai_error ai_mnetwork_create(const char *name, ai_handle* network,
        const ai_buffer* network_config)
{
    const ai_network_entry_t *entry;
    const ai_network_entry_t *found = NULL;
    ai_error err;
    struct network_instance *inst = ai_mnetwork_handle(NULL);

    if (!inst) {
        err.type = AI_ERROR_ALLOCATION_FAILED;
        err.code = AI_ERROR_CODE_NETWORK;
        return err;
    }

    for (int i=0; i<AI_MNETWORK_NUMBER; i++) {
        entry = &networks[i];
        if (ai_mnetwork_is_valid(name, entry)) {
            found = entry;
            break;
        }
    }

    if (!found) {
        err.type = AI_ERROR_INVALID_PARAM;
        err.code = AI_ERROR_CODE_NETWORK;
        return err;
    }

    if (network_config == NULL)
        err = found->ai_create(network, found->config);
    else
        err = found->ai_create(network, network_config);
    if ((err.code == AI_ERROR_CODE_NONE) && (err.type == AI_ERROR_NONE)) {
        inst->entry = found;
        inst->handle = *network;
        *network = (ai_handle*)inst;
    }

    return err;
}

AI_API_ENTRY
ai_handle ai_mnetwork_destroy(ai_handle network)
{
    struct network_instance *inn;
    inn =  ai_mnetwork_handle((struct network_instance *)network);
    if (inn) {
        ai_handle hdl = inn->entry->ai_destroy(inn->handle);
        if (hdl != inn->handle) {
            ai_mnetwork_release_handle(inn);
            network = AI_HANDLE_NULL;
        }
    }
    return network;
}

AI_API_ENTRY
ai_bool ai_mnetwork_get_report(ai_handle network, ai_network_report* report)
{
    struct network_instance *inn;
    inn =  ai_mnetwork_handle((struct network_instance *)network);
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

    inn =  ai_mnetwork_handle((struct network_instance *)network);
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

    inn =  ai_mnetwork_handle((struct network_instance *)network);
    if (inn) {
        inn->entry->ai_data_params_get(&par);
        for (int idx=0; idx < par.map_activations.size; idx++)
          AI_BUFFER_ARRAY_ITEM_SET_ADDRESS(&par.map_activations, idx, inn->entry->activations[idx]);
        return inn->entry->ai_init(inn->handle, &par);
    }
    else
        return false;
}

AI_API_ENTRY
ai_i32 ai_mnetwork_run(ai_handle network, const ai_buffer* input,
        ai_buffer* output)
{
    struct network_instance* inn;
    inn =  ai_mnetwork_handle((struct network_instance *)network);
    if (inn)
        return inn->entry->ai_run(inn->handle, input, output);
    else
        return 0;
}

AI_API_ENTRY
ai_i32 ai_mnetwork_forward(ai_handle network, const ai_buffer* input)
{
    struct network_instance *inn;
    inn =  ai_mnetwork_handle((struct network_instance *)network);
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
     struct network_instance* inn;
     inn =  ai_mnetwork_handle((struct network_instance *)network);
     if (inn && phandle && pparams) {
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
