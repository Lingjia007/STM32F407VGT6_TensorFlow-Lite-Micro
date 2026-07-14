#ifndef AUDIO_FRONTEND_H
#define AUDIO_FRONTEND_H

#include <stdint.h>
#include "arm_math.h"
#include "audio_params.h"

typedef struct {
    arm_rfft_instance_q15 rfft_inst;
    uint32_t noise_estimate[NUM_CHANNELS];
    int16_t fft_buffer[FFT_LENGTH];
    int16_t rfft_output[2 * FFT_LENGTH];
} AudioFrontend;

int audio_frontend_init(AudioFrontend* fe);
void audio_frontend_reset(AudioFrontend* fe);
void audio_frontend_process_frame(AudioFrontend* fe, const int16_t* audio_frame, int8_t* features);
void audio_frontend_process_clip(AudioFrontend* fe, const int16_t* audio, int audio_len, int8_t* features_2d);

#endif // AUDIO_FRONTEND_H
