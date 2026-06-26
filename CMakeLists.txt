cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME STM32F407VGT6_TensorFlow-Lite-Micro)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM, C and C++ languages
enable_language(C CXX ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# ---- TFLM base path ----
set(TFLM_BASE ${CMAKE_SOURCE_DIR}/stm32f4_tflm_project)

# ---- CMSIS-NN source files (collect all .c files) ----
file(GLOB CMSIS_NN_SOURCES
    ${TFLM_BASE}/third_party/cmsis_nn/Source/ActivationFunctions/*.c
    ${TFLM_BASE}/third_party/cmsis_nn/Source/BasicMathFunctions/*.c
    ${TFLM_BASE}/third_party/cmsis_nn/Source/ConcatenationFunctions/*.c
    ${TFLM_BASE}/third_party/cmsis_nn/Source/ConvolutionFunctions/*.c
    ${TFLM_BASE}/third_party/cmsis_nn/Source/FullyConnectedFunctions/*.c
    ${TFLM_BASE}/third_party/cmsis_nn/Source/LSTMFunctions/*.c
    ${TFLM_BASE}/third_party/cmsis_nn/Source/NNSupportFunctions/*.c
    ${TFLM_BASE}/third_party/cmsis_nn/Source/PadFunctions/*.c
    ${TFLM_BASE}/third_party/cmsis_nn/Source/PoolingFunctions/*.c
    ${TFLM_BASE}/third_party/cmsis_nn/Source/ReshapeFunctions/*.c
    ${TFLM_BASE}/third_party/cmsis_nn/Source/SVDFunctions/*.c
    ${TFLM_BASE}/third_party/cmsis_nn/Source/SoftmaxFunctions/*.c
    ${TFLM_BASE}/third_party/cmsis_nn/Source/TransposeFunctions/*.c
)

# ---- TFLM source files ----
set(TFLM_SOURCES
    # TFLM core
    ${TFLM_BASE}/tensorflow/lite/micro/micro_allocator.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/micro_allocation_info.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/micro_context.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/micro_interpreter.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/micro_interpreter_context.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/micro_interpreter_graph.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/micro_op_resolver.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/micro_log.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/micro_utils.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/micro_time.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/micro_profiler.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/micro_resource_variable.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/memory_helpers.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/flatbuffer_utils.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/debug_log.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/hexdump.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/fake_micro_context.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/system_setup.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/mock_micro_graph.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/recording_micro_allocator.cpp

    # TFLM arena allocator
    ${TFLM_BASE}/tensorflow/lite/micro/arena_allocator/single_arena_buffer_allocator.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/arena_allocator/recording_single_arena_buffer_allocator.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/arena_allocator/persistent_arena_buffer_allocator.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/arena_allocator/non_persistent_arena_buffer_allocator.cpp

    # TFLM memory planner
    ${TFLM_BASE}/tensorflow/lite/micro/memory_planner/linear_memory_planner.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/memory_planner/greedy_memory_planner.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/memory_planner/non_persistent_buffer_planner_shim.cpp

    # TFLM tflite_bridge
    ${TFLM_BASE}/tensorflow/lite/micro/tflite_bridge/micro_error_reporter.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/tflite_bridge/flatbuffer_conversions_bridge.cpp

    # TFLM kernels (standard ops needed by audio_preprocessor)
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/reshape_common.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/cast.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/strided_slice.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/strided_slice_common.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/concatenation.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/mul_common.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/add_common.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/div.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/kernel_runner.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/kernel_util.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/activations.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/activations_common.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/reshape.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/softmax_common.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/prelu.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/prelu_common.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/quantize.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/quantize_common.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/dequantize.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/dequantize_common.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/comparisons.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/logical.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/logical_common.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/reduce.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/reduce_common.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/pad_common.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/conv_common.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/depthwise_conv_common.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/fully_connected_common.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/pooling_common.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/leaky_relu.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/leaky_relu_common.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/logistic.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/logistic_common.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/l2norm.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/sub.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/sub_common.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/svdf_common.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/circular_buffer.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/circular_buffer_common.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/shape.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/slice.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/expand_dims.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/transpose_common.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/fill.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/ceil.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/neg.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/round.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/reverse.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/select.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/squeeze.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/squared_difference.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/pack.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/unpack.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/space_to_batch_nd.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/batch_to_space_nd.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/arg_min_max.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/elu.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/exp.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/floor.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/floor_div.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/floor_mod.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/mirror_pad.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/hard_swish.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/hard_swish_common.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/depth_to_space.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/read_variable.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/assign_variable.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/var_handle.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/call_once.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/if.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/while.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/gather.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/gather_nd.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/resize_bilinear.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/resize_nearest_neighbor.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/split.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/split_v.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/embedding_lookup.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/l2_pool_2d.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/add_n.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/broadcast_args.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/broadcast_to.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/cumsum.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/dynamic_update_slice.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/elementwise.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/decode.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/decode_state.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/decode_state_huffman.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/decode_state_lut.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/decode_state_prune.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/decompress.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/decompress_common.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/ethosu.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/detection_postprocess.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/lstm_eval.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/lstm_eval_common.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/micro_tensor_utils.cpp

    # TFLM cmsis_nn kernels
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/cmsis_nn/add.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/cmsis_nn/conv.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/cmsis_nn/depthwise_conv.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/cmsis_nn/fully_connected.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/cmsis_nn/mul.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/cmsis_nn/pooling.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/cmsis_nn/softmax.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/cmsis_nn/svdf.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/cmsis_nn/batch_matmul.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/cmsis_nn/maximum_minimum.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/cmsis_nn/pad.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/cmsis_nn/transpose.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/cmsis_nn/transpose_conv.cpp
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/cmsis_nn/unidirectional_sequence_lstm.cpp

    # TFLite core
    ${TFLM_BASE}/tensorflow/lite/array.cc
    ${TFLM_BASE}/tensorflow/lite/core/c/common.cpp
    ${TFLM_BASE}/tensorflow/lite/core/api/tensor_utils.cpp
    ${TFLM_BASE}/tensorflow/lite/core/api/flatbuffer_conversions.cpp

    # TFLite kernels internal
    ${TFLM_BASE}/tensorflow/lite/kernels/kernel_util.cpp
    ${TFLM_BASE}/tensorflow/lite/kernels/internal/tensor_ctypes.cpp
    ${TFLM_BASE}/tensorflow/lite/kernels/internal/quantization_util.cpp
    ${TFLM_BASE}/tensorflow/lite/kernels/internal/portable_tensor_utils.cpp
    ${TFLM_BASE}/tensorflow/lite/kernels/internal/tensor_utils.cpp
    ${TFLM_BASE}/tensorflow/lite/kernels/internal/reference/portable_tensor_utils.cpp
    ${TFLM_BASE}/tensorflow/lite/kernels/internal/reference/comparisons.cpp
    ${TFLM_BASE}/tensorflow/lite/kernels/internal/common.cpp
    ${TFLM_BASE}/tensorflow/lite/kernels/internal/runtime_shape.cpp

    # TFLite schema
    ${TFLM_BASE}/tensorflow/compiler/mlir/lite/schema/schema_utils.cpp
    ${TFLM_BASE}/tensorflow/compiler/mlir/lite/core/api/error_reporter.cpp

    # Signal processing (for audio_preprocessor custom ops)
    ${TFLM_BASE}/signal/micro/kernels/window.cpp
    ${TFLM_BASE}/signal/micro/kernels/fft_auto_scale_kernel.cpp
    ${TFLM_BASE}/signal/micro/kernels/fft_auto_scale_common.cpp
    ${TFLM_BASE}/signal/micro/kernels/rfft.cpp
    ${TFLM_BASE}/signal/micro/kernels/energy.cpp
    ${TFLM_BASE}/signal/micro/kernels/filter_bank.cpp
    ${TFLM_BASE}/signal/micro/kernels/filter_bank_square_root.cpp
    ${TFLM_BASE}/signal/micro/kernels/filter_bank_square_root_common.cpp
    ${TFLM_BASE}/signal/micro/kernels/filter_bank_spectral_subtraction.cpp
    ${TFLM_BASE}/signal/micro/kernels/filter_bank_log.cpp
    ${TFLM_BASE}/signal/micro/kernels/pcan.cpp
    ${TFLM_BASE}/signal/micro/kernels/delay.cpp
    ${TFLM_BASE}/signal/micro/kernels/framer.cpp
    ${TFLM_BASE}/signal/micro/kernels/overlap_add.cpp
    ${TFLM_BASE}/signal/micro/kernels/stacker.cpp
    ${TFLM_BASE}/signal/micro/kernels/irfft.cpp

    # Signal processing source implementations
    ${TFLM_BASE}/signal/src/window.cpp
    ${TFLM_BASE}/signal/src/fft_auto_scale.cpp
    ${TFLM_BASE}/signal/src/rfft_int16.cpp
    ${TFLM_BASE}/signal/src/rfft_float.cpp
    ${TFLM_BASE}/signal/src/rfft_int32.cpp
    ${TFLM_BASE}/signal/src/energy.cpp
    ${TFLM_BASE}/signal/src/filter_bank.cpp
    ${TFLM_BASE}/signal/src/filter_bank_square_root.cpp
    ${TFLM_BASE}/signal/src/filter_bank_spectral_subtraction.cpp
    ${TFLM_BASE}/signal/src/filter_bank_log.cpp
    ${TFLM_BASE}/signal/src/pcan_argc_fixed.cpp
    ${TFLM_BASE}/signal/src/circular_buffer.cpp
    ${TFLM_BASE}/signal/src/overlap_add.cpp
    ${TFLM_BASE}/signal/src/max_abs.cpp
    ${TFLM_BASE}/signal/src/log.cpp
    ${TFLM_BASE}/signal/src/msb_32.cpp
    ${TFLM_BASE}/signal/src/msb_64.cpp
    ${TFLM_BASE}/signal/src/square_root_32.cpp
    ${TFLM_BASE}/signal/src/square_root_64.cpp
    ${TFLM_BASE}/signal/src/irfft_float.cpp
    ${TFLM_BASE}/signal/src/irfft_int16.cpp
    ${TFLM_BASE}/signal/src/irfft_int32.cpp
    ${TFLM_BASE}/signal/src/kiss_fft_wrappers/kiss_fft_float.cpp
    ${TFLM_BASE}/signal/src/kiss_fft_wrappers/kiss_fft_int16.cpp
    ${TFLM_BASE}/signal/src/kiss_fft_wrappers/kiss_fft_int32.cpp

    # KissFFT library (required by signal FFT wrappers)
    ${TFLM_BASE}/third_party/kissfft/kiss_fft.c
    ${TFLM_BASE}/third_party/kissfft/tools/kiss_fftr.c

    # Audio preprocessor model data
    ${TFLM_BASE}/examples/micro_speech/models/audio_preprocessor_int8_model_data.cpp

    # Test audio data
    ${TFLM_BASE}/examples/micro_speech/testdata/yes_1000ms_audio_data.cpp
    ${TFLM_BASE}/examples/micro_speech/testdata/no_1000ms_audio_data.cpp
    ${TFLM_BASE}/examples/micro_speech/testdata/silence_1000ms_audio_data.cpp
    ${TFLM_BASE}/examples/micro_speech/testdata/noise_1000ms_audio_data.cpp

    # Micro speech TFLM preprocess wrapper
    ${CMAKE_SOURCE_DIR}/X-CUBE-AI/App/micro_speech_tflm_preprocess.cpp
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # CMSIS DSP sources for arm_cos_f32 and arm_sin_f32
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_cos_f32.c
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_sin_f32.c
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/DSP/Source/CommonTables/arm_common_tables.c
    # CMSIS-NN sources
    ${CMSIS_NN_SOURCES}
    # TFLM sources
    ${TFLM_SOURCES}
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/DSP/Include
    # TFLM include paths
    ${TFLM_BASE}
    ${TFLM_BASE}/signal/micro/kernels
    ${TFLM_BASE}/signal/src
    ${TFLM_BASE}/signal/src/kiss_fft_wrappers
    ${TFLM_BASE}/tensorflow
    ${TFLM_BASE}/tensorflow/lite/micro/kernels
    ${TFLM_BASE}/tensorflow/lite/micro/kernels/cmsis_nn
    ${TFLM_BASE}/examples/micro_speech
    ${TFLM_BASE}/examples/micro_speech/models
    ${TFLM_BASE}/examples/micro_speech/testdata
    # Flatbuffers
    ${TFLM_BASE}/third_party/flatbuffers/include
    # Gemmlowp (fixedpoint)
    ${TFLM_BASE}/third_party/gemmlowp
    # Ruy (profiler instrumentation)
    ${TFLM_BASE}/third_party/ruy
    # CMSIS-NN (include path for "Include/arm_nnfunctions.h" style references)
    ${TFLM_BASE}/third_party/cmsis_nn
    # CMSIS-NN headers direct
    ${TFLM_BASE}/third_party/cmsis_nn/Include
    # KissFFT (for signal processing FFT)
    ${TFLM_BASE}/third_party/kissfft
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # TFLM definitions
    TF_LITE_STRIP_ERROR_STRINGS
    CMSIS_NN
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)

# Enable float printf support for newlib-nano
target_link_options(${CMAKE_PROJECT_NAME} PRIVATE "-Wl,-u,_printf_float")

# Generate hex and bin files after build
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_PROJECT_NAME}.bin
    COMMENT "Generating HEX and BIN files"
)
