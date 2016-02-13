/*
 * leds.h
 * LED writing functions
 */

#ifndef LEDS_H
#define LEDS_H

#include <stdint.h>

typedef struct {
    uint8_t G, R, B;
} LED;

extern volatile LED LEDS[16];

void leds_draw(void);

#endif
