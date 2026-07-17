/**
 ******************************************************************************
 * @file    audio_stream.h
 * @brief   Real-time audio capture via I2S2 DMA ping-pong for INMP441.
 *
 * Hardware assumptions (matching CubeMX-generated i2s.c):
 *   - I2S2 master RX, Philips, 16B_EXTENDED (32-bit slot, 16-bit data in MSB)
 *   - AudioFreq = 16 kHz, stereo
 *   - DMA1_Stream3 / Channel0, circular, halfword alignment
 *
 * INMP441 wiring: L/R pin -> GND selects the LEFT channel (WS=LOW in I2S
 * Philips). On STM32F4, 16B_EXTENDED transfers 1 halfword per channel (16-bit
 * DR read once per 32-bit slot), so each stereo frame is [L, R] = 2 halfwords.
 * If your mic is wired for the right channel instead (L/R=VDD), switch
 * AUDIO_CHANNEL_INDEX below to 1.
 *
 * Pipeline: DMA half/full IRQ -> deinterleave left channel -> per-block DC
 * removal -> ring buffer. Main loop consumes from the ring.
 ******************************************************************************
 */
#ifndef AUDIO_STREAM_H
#define AUDIO_STREAM_H

#include <stdint.h>
#include "audio_capture_ring_buff.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* ---- Audio geometry ------------------------------------------------------ */
#define AS_SAMPLE_RATE_HZ 16000

/* 10 ms per DMA half-buffer = 160 mono samples */
#define AS_DMA_HALF_FRAMES 160
#define AS_DMA_HALFWORDS (AS_DMA_HALF_FRAMES * 2)   /* 320 halfwords: [L,R] per frame */
#define AS_DMA_BUF_HALFWORDS (AS_DMA_HALFWORDS * 2) /* 640 halfwords: double buffer */

/* INMP441 with L/R=GND outputs on the LEFT channel (I2S Philips: WS=LOW=Left).
 * On STM32F4, 16B_EXTENDED transfers 1 halfword per channel per stereo frame,
 * so the layout is [L, R] = 2 halfwords. The 16-bit sample lives at offset 0.
 * If your mic is wired for the right channel instead (L/R=VDD), switch
 * AUDIO_CHANNEL_INDEX to 1. */
#define AUDIO_CHANNEL_INDEX 0

/* Ring buffer geometry: 4 frames x 3200 samples = 12800 samples (~800 ms) */
#define AS_RING_NB_SAMPLES 3200
#define AS_RING_NB_FRAMES 4

  /* Initialize the ring buffer and backing storage. */
  void AudioStream_init(void);

  /* Start I2S2 DMA ping-pong capture. */
  void AudioStream_start(void);

  /* Access the ring buffer handle (consumer side). */
  AudioCaptureRingBuff_t *AudioStream_get_ring(void);

#ifdef __cplusplus
}
#endif
#endif /* AUDIO_STREAM_H */
