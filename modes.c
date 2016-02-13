/*
 * modes.c
 * LED display modes
 */

#include <stdint.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "modes.h"
#include "dsp.h"
#include "colours.h"
#include "leds.h"

static void modes_circle(void);
static void modes_fill(void);
static void modes_fade(void);
static void modes_audio(void);

/* Array of function pointers - the "mode list" */
void (*modes_list[])(void) = {
    /*modes_circle,*/
    /*modes_fill,*/
    /*modes_fade,*/
    modes_audio,
};

uint8_t modes_count = sizeof(modes_list)/sizeof(modes_list[0]);

/* Spin around the circle */
static void modes_circle()
{
    uint8_t i;
    while(1) {
        for(i=0; i<16; i++) {
            colours_set(&LEDS[i], colours_saturated, 60);
            leds_draw();
            _delay_ms(40);
            colours_black(&LEDS[i]);
        }
    }
}

/* Fill up the circle while moving start point around */
static void modes_fill()
{
    uint8_t start, i;
    /* For each starting point */
    for(start=0; start<16; start++) {

        /* Fill up LEDs around the circle */
        for(i=start; i<start+16; i++) {
            colours_set(&LEDS[i & 0xF], colours_saturated, 3);
            leds_draw();
            _delay_ms(40);
        }

        /* Unfill LEDs around the circle */
        for(i=start+1; i<start+16; i++) {
            colours_black(&LEDS[i & 0xF]);
            leds_draw();
            _delay_ms(40);
        }
    }
}

/* Fade between different full-circle colours */
static void modes_fade()
{
    uint8_t start, i, j, colour=0;
    while(1) {
        /* For each starting point */
        for(start=0; start<16; start++) {

            /* Fill up LEDs around the circle */
            for(i=start; i<start+16; i++) {
                if(i < 16) {
                    j = i;
                } else {
                    j = i - 16;
                }
                colours_set(&LEDS[j], colours_saturated, colour);
                leds_draw();
                _delay_ms(40);
            }

            colour = (colour + 1) & 0x3F;
        }
    }
}

static void modes_audio()
{
    uint8_t colour=0;
    dsp_init();
    while(1) {
        uint8_t i;
        uint8_t num = dsp_audio_level >> 4;

        for(i=0; i<num; i++) {
            colours_set(&LEDS[i], colours_saturated, colour);
        }
        for(; i<16; i++) {
            colours_black(&LEDS[i]);
        }

        leds_draw();
    }
}
