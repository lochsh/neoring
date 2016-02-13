/*
 * colours.h
 * lots of colours
 */
#ifndef COLOURS_H
#define COLOURS_H

#include <avr/pgmspace.h>
#include "leds.h"

extern const uint8_t colours_saturated[3][64] PROGMEM;

/* Set LED to a colour from cmap */
void colours_set(volatile LED* led, const uint8_t cmap[3][64], const uint8_t idx);

/* Set LED to off */
void colours_black(volatile LED* led);

#endif
