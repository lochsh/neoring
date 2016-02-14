/*
 * colours.c
 * Deal with LED colours
 */
#include <avr/pgmspace.h>
#include "colours.h"
#include "leds.h"
#include "luts.h"

void colours_set(const uint8_t led, const uint8_t hue, const uint8_t val)
{
    LEDS[led].R = pgm_read_byte(&(lut_colour[hue][val][0]));
    LEDS[led].G = pgm_read_byte(&(lut_colour[hue][val][1]));
    LEDS[led].B = pgm_read_byte(&(lut_colour[hue][val][2]));
}
