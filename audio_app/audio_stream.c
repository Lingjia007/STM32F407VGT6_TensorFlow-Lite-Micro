/**
 ******************************************************************************
 * @file    audio_stream.c
 * @brief   I2S2 DMA ping-pong capture for INMP441, right-channel deinterleave
 *          and per-block DC removal, feeding a ring buffer.
 ******************************************************************************
 */
#include "audio_stream.h"
#include "i2s.h"
#include "main.h"
#include <string.h>
#include <stdio.h>

/* DMA double buffer. MUST live in regular RAM (DMA cannot reach CCMRAM). */
static uint16_t s_dma_buf[AS_DMA_BUF_HALFWORDS];

/* Ring buffer backing store (CPU-only access). Kept in main RAM to balance
 * the 64 KB CCMRAM budget, which is already consumed by the 1-sec frontend
 * window placed there. */
static int16_t s_ring_backing[AS_RING_NB_SAMPLES * AS_RING_NB_FRAMES];

static AudioCaptureRingBuff_t s_ring;

/* Scratch buffer for one DMA half-block of mono samples (160 x int16_t). */
static int16_t s_mono[AS_DMA_HALF_FRAMES] __attribute__((section(".ccmram")));

AudioCaptureRingBuff_t *AudioStream_get_ring(void) { return &s_ring; }

void AudioStream_init(void)
{
  AudioCaptureRingBuff_reset(&s_ring,
                             (uint8_t *)s_ring_backing,
                             AS_RING_NB_SAMPLES,
                             AS_RING_NB_FRAMES,
                             sizeof(int16_t));
}

void AudioStream_start(void)
{
  /* HAL_I2S_Receive_DMA on circular DMA gives half + full callbacks. */
  HAL_StatusTypeDef st = HAL_I2S_Receive_DMA(&hi2s2, (uint16_t *)s_dma_buf, AS_DMA_BUF_HALFWORDS);
  if (st != HAL_OK)
  {
    printf("I2S DMA FAIL=%d\r\n", (int)st);
  }
}

/* Deinterleave one DMA half-block into s_mono, remove DC offset, push to ring.
 * half_id = 0 -> first half of s_dma_buf, 1 -> second half. */
static void AudioStream_process_half(uint8_t half_id)
{
  const uint16_t *src = s_dma_buf + half_id * AS_DMA_HALFWORDS;

  /* Extract left (or right) channel 16-bit sample from each stereo frame.
   * STM32F4 16B_EXTENDED layout: [L, R] = 2 halfwords per frame. */
  int32_t sum = 0;
  for (uint32_t i = 0; i < AS_DMA_HALF_FRAMES; i++)
  {
    int16_t v = (int16_t)src[i * 2 + AUDIO_CHANNEL_INDEX];
    s_mono[i] = v;
    sum += v;
  }

  /* DC removal: subtract block mean. */
  int16_t dc = (int16_t)(sum / (int32_t)AS_DMA_HALF_FRAMES);
  for (uint32_t i = 0; i < AS_DMA_HALF_FRAMES; i++)
    s_mono[i] = (int16_t)(s_mono[i] - dc);

  AudioCaptureRingBuff_feed(&s_ring, (const uint8_t *)s_mono,
                            (uint16_t)AS_DMA_HALF_FRAMES);
}

/* HAL callbacks (weak symbols overridden). Only handle I2S2. */
void HAL_I2S_RxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
  if (hi2s->Instance == SPI2)
    AudioStream_process_half(0);
}

void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s)
{
  if (hi2s->Instance == SPI2)
    AudioStream_process_half(1);
}
