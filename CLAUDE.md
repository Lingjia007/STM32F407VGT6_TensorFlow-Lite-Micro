# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

STM32F407VGT6 (Cortex-M4, 1024KB Flash, 192KB RAM) bare-metal embedded AI project. Originated from STM32CubeMX, converted to a CMake + Ninja build. Uses ST X-CUBE-AI to run quantized int8 TFLite models on-device, and also bundles a port of TensorFlow Lite Micro for reference/signal-processing code.

Two AI models are embedded:
- **hello_world**: Single-neuron sin/cos benchmark (1×int8 input → 1×int8 output). Used for validation with 3000 test points, streaming results via VOFA+ protocol over UART4.
- **micro_speech_quantized**: Keyword spotting (4 categories: silence, unknown, yes, no) with an audio preprocessor frontend.

## Build

Requires GNU Arm Embedded Toolchain at `C:/Tools/arm-gnu-toolchain-15.2.rel1-mingw-w64-i686-arm-none-eabi/bin`. An alternative starm-clang toolchain is also defined in `cmake/starm-clang.cmake` but the default preset uses GCC.

```bash
# Configure (Debug)
cmake --preset Debug

# Build
cmake --build build/Debug

# The build produces .elf, .hex, and .bin in build/Debug/
```

The CMake project is defined by:
- `CMakeLists.txt` (root) — executable target, extra sources (CMSIS-DSP sin/cos), link options
- `cmake/stm32cubemx/CMakeLists.txt` — all STM32CubeMX-generated sources, HAL drivers, X-CUBE-AI generated model code, and ST AI runtime library
- `cmake/gcc-arm-none-eabi.cmake` — ARM GCC toolchain definition
- `CMakePresets.json` — `Debug` and `Release` presets, both using Ninja generator

### Flash to device

Uses SEGGER J-Link (expects JLink.exe at `C:\Program Files\SEGGER\JLink_V798a\JLink.exe`):

```bash
flash.bat
```

This runs `flash.jlink` which loads the Debug elf, resets, and starts execution. The device connects via SWD.

## High-Level Architecture

### Application flow

```
main.c (main loop)
  ├── HAL_Init() → SystemClock_Config() (168 MHz, HSE+PLL)
  ├── MX_GPIO_Init() / MX_UART4_Init()
  ├── MX_X_CUBE_AI_Init()         # Bootstraps the hello_world model
  └── while(1):
        MX_X_CUBE_AI_Process()     # acquire → infer → post-process → VOFA+ send
```

### X-CUBE-AI layer (`X-CUBE-AI/App/`)

This is the heart of on-device inference. ST's X-CUBE-AI toolchain converts `.tflite` files into C source:

| File | Role |
|---|---|
| `app_x-cube-ai.c/h` | Multi-network manager — init, run loop, model selection, input acquisition, output post-processing |
| `hello_world.c/h` | Auto-generated C code for the hello_world model (layers, weights, inference) |
| `hello_world_data.c/h` | Auto-generated model weights as C arrays |
| `hello_world_data_params.c/h` | Model parameter getter stubs |
| `micro_speech_quantized.c/h` | Auto-generated C code for the micro_speech model |
| `micro_speech_quantized_data*.c/h` | Auto-generated weights and params for micro_speech |

The `networks[]` array in `app_x-cube-ai.c` registers both models, but the current `MX_X_CUBE_AI_Process()` only runs the hello_world model in a validation loop (3000 test points, 0→2π).

**Model selection**: Edit `USE_SIN_MODEL` in `app_x-cube-ai.c` to switch between sin (`hello_world_int8_sin.tflite`) and cos (`hello_world_int8_cos.tflite`) models. Quantization parameters (input/output scale and zero point) change accordingly.

**VOFA+ debugging**: The app sends 4-channel float data over UART4 using the VOFA+ fdata protocol (tail bytes `00 00 80 7f`). Channels: input angle, predicted value, true value, error. Define `DEBUG_PRINTF_ENABLED` to also get printf output over UART.

### ST AI runtime (`Middlewares/ST/AI/`)

Proprietary ST library `NetworkRuntime1020_CM4_GCC.a` (linked from `Middlewares/ST/AI/Lib/`) provides the AI platform API (`ai_platform.h`, `ai_datatypes_defines.h`). This is a black-box runtime — the generated C model code calls into it.

### TFLM reference project (`stm32f4_tflm_project/`)

A port of upstream TensorFlow Lite Micro for STM32F4, NOT compiled into the current firmware binary. It serves as reference code and provides the signal processing pipeline for the micro_speech example:

```
stm32f4_tflm_project/
├── tensorflow/lite/        # TFLM headers: kernels, micro interpreter, op resolver, schema
├── signal/src/             # Audio preprocessing: FFT, filter banks, windowing, energy, etc.
├── signal/micro/kernels/   # Micro-optimized signal kernels
├── third_party/            # flatbuffers, kissfft, gemmlowp, ruy, cmsis_nn
└── examples/micro_speech/
    ├── micro_speech_test.cpp         # Offline test harness (compares model output against expected labels)
    ├── micro_model_settings.h        # 16kHz, 40 features × 49 frames, 4 categories
    ├── models/                       # .tflite files + model_data.cpp/h (weights as C arrays)
    └── testdata/                     # Audio samples: yes, no, silence, noise (30ms + 1000ms)
```

### HAL & CMSIS (`Drivers/`)

- `STM32F4xx_HAL_Driver/` — STM32 HAL for GPIO, UART, RCC, DMA, Cortex, etc.
- `CMSIS/Core/` — CMSIS-Core (Cortex-M4 intrinsics)
- `CMSIS/DSP/` — CMSIS-DSP library. The root `CMakeLists.txt` explicitly compiles `arm_cos_f32.c`, `arm_sin_f32.c`, and `arm_common_tables.c` from here.
- `CMSIS/NN/` — CMSIS-NN kernels (used by TFLM reference, not compiled into firmware)

### Application code (`Core/`)

STM32CubeMX-generated HAL glue:
- `main.c` — entry point, clock config (HSE 8MHz → PLL → 168MHz SYSCLK)
- `gpio.c` — PB2 configured as LED output
- `usart.c` — UART4 (PA0=TX, PA1=RX) at 115200 baud for VOFA+ communication
- `stm32f4xx_it.c` — interrupt handlers (SysTick, UART4)
- `syscalls.c` — newlib/picolibc syscall stubs (`_write`, `_read`, etc.) routing through `__io_putchar`/`__io_getchar`
- `sysmem.c` — heap definition for `_sbrk`

### .tflite model files (repo root)

- `hello_world_int8_sin.tflite` — sin model (1.86 KiB weights, 448 B activations)
- `hello_world_int8_cos.tflite` — cos model
- `model_info.txt` — output of `stedgeai analyze` showing model structure, memory, and quantization parameters

### Key configuration files

- `STM32F407VGT6_TensorFlow-Lite-Micro.ioc` — STM32CubeMX project file (v6.17.0). Open this in CubeMX/CubeIDE to modify pinout, clock tree, or add peripherals. Changes require regenerating code and may need manual merge with user code sections (marked by `USER CODE BEGIN/END` comments).
- `STM32F407XX_FLASH.ld` — Linker script (1024K Flash, 192K RAM)

## Important notes

- All user code lives between `/* USER CODE BEGIN ... */` and `/* USER CODE END ... */` markers. CubeMX regeneration preserves only these blocks.
- X-CUBE-AI generated code in `X-CUBE-AI/App/` is auto-generated by `stedgeai` tool — edit models via CubeMX's AI configuration, not by hand.
- The `.ai/` directory contains cached model analysis JSON from `stedgeai analyze` — used by CubeMX to track model metadata.
- The current firmware only runs the **hello_world** model in the main loop. The micro_speech model is registered in the multi-network table but not called — its C code is compiled in and reserved activation memory (`pool0`) is shared.
- The GCC toolchain path is hardcoded in `cmake/gcc-arm-none-eabi.cmake` — adjust `ARM_TOOLCHAIN_PATH` for your machine.
- J-Link path is hardcoded in `flash.bat` — adjust if using a different J-Link version.
