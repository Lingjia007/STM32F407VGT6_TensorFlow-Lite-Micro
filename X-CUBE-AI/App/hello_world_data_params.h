/**
  ******************************************************************************
  * @file    hello_world_data_params.h
  * @author  AST Embedded Analytics Research Platform
  * @date    2026-06-26T20:16:06+0800
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */

#ifndef HELLO_WORLD_DATA_PARAMS_H
#define HELLO_WORLD_DATA_PARAMS_H

#include "ai_platform.h"

/*
#define AI_HELLO_WORLD_DATA_WEIGHTS_PARAMS \
  (AI_HANDLE_PTR(&ai_hello_world_data_weights_params[1]))
*/

#define AI_HELLO_WORLD_DATA_CONFIG               (NULL)


#define AI_HELLO_WORLD_DATA_ACTIVATIONS_SIZES \
  { 448, }
#define AI_HELLO_WORLD_DATA_ACTIVATIONS_SIZE     (448)
#define AI_HELLO_WORLD_DATA_ACTIVATIONS_COUNT    (1)
#define AI_HELLO_WORLD_DATA_ACTIVATION_1_SIZE    (448)



#define AI_HELLO_WORLD_DATA_WEIGHTS_SIZES \
  { 1908, }
#define AI_HELLO_WORLD_DATA_WEIGHTS_SIZE         (1908)
#define AI_HELLO_WORLD_DATA_WEIGHTS_COUNT        (1)
#define AI_HELLO_WORLD_DATA_WEIGHT_1_SIZE        (1908)



#define AI_HELLO_WORLD_DATA_ACTIVATIONS_TABLE_GET() \
  (&g_hello_world_activations_table[1])

extern ai_handle g_hello_world_activations_table[1 + 2];



#define AI_HELLO_WORLD_DATA_WEIGHTS_TABLE_GET() \
  (&g_hello_world_weights_table[1])

extern ai_handle g_hello_world_weights_table[1 + 2];


#endif    /* HELLO_WORLD_DATA_PARAMS_H */
