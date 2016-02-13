/*
 * dsp.h
 * digital signal processing
 */

#ifndef DSP_H
#define DSP_H

#include <stdint.h>

extern volatile uint8_t dsp_audio_level;

void dsp_init(void);

#endif
