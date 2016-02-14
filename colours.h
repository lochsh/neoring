/*
 * colours.h
 * Deal with LED colours
 */
#ifndef COLOURS_H
#define COLOURS_H

#include <stdint.h>

/* Set an LED (0-15) to a certain hue (0-31) and value (0-31). */
void colours_set(const uint8_t led, const uint8_t hue, const uint8_t val);

#endif
