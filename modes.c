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

static void modes_spin(void);
static void modes_fill_rotate(void);
static void modes_fade(void);
static void modes_audio_vu(void);
static void modes_audio_fade(void);

/* Array of function pointers - the "mode list" */
void (*modes_list[])(void) = {
    modes_spin,
    modes_fill_rotate,
    modes_fade,
    modes_audio_vu,
    modes_audio_fade,
};

uint8_t modes_count = sizeof(modes_list)/sizeof(modes_list[0]);

/* Spin around the circle */
static void modes_spin()
{
    uint8_t i;
    while(1) {
        for(i=0; i<16; i++) {
            colours_set(i, 30, 31);
            leds_draw();
            _delay_ms(40);
            colours_set(i, 30, 0);
        }
    }
}

/* Fill up the circle while moving start point around */
static void modes_fill_rotate()
{
    uint8_t start, i;
    /* For each starting point */
    for(start=0; start<16; start++) {

        /* Fill up LEDs around the circle */
        for(i=start; i<start+16; i++) {
            colours_set(i & 0xF, 2, 31);
            leds_draw();
            _delay_ms(40);
        }

        /* Unfill LEDs around the circle */
        for(i=start+1; i<start+16; i++) {
            colours_set(i & 0xF, 2, 0);
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
                colours_set(j, colour, 31);
                leds_draw();
                _delay_ms(40);
            }

            colour = (colour + 1) & 0x1F;
        }
    }
}

static void modes_audio_vu()
{
    uint8_t i;

    /* Store the intensity we'll set for LEDs on each side */
    uint8_t intensities[8] = {0};

    /* Variable step size provides very rough AGC */
    /* Default step is 32 for full-range (256/8), smaller amplifies signal. */
    uint8_t step = 16;

    /* Clear the LEDS initially */
    for(i=0; i<16; i++) {
        LEDS[i].R = LEDS[i].G = LEDS[i].B = 0;
    }

    /* Start up the DSP engine */
    dsp_init();

    while(1) {
        /* Compute appropriate step_gain */
        uint8_t step_gain;
        if(step > 16) {
            step_gain = 3;
        } else if(step > 8) {
            step_gain = 4;
        } else if(step > 4) {
            step_gain = 5;
        } else {
            step_gain = 6;
        }

        /* Assign intensities to each LED */
        uint8_t level = dsp_audio_level;
        for(i=0; i<8; i++) {
            if(level >= step) {
                intensities[i] = (step-1) << step_gain;
                level -= step;
            } else {
                intensities[i] = level << step_gain;
                level = 0;
            }
        }
        /* If red is lit, really light it up */
        if(intensities[7]) intensities[7] = 255;

        /* Set relevant LED colours
         * LEDs 1-8 are the right hand side,
         * LEDs 9-15 and 0 are the left hand side.
         * The first four LEDs are green, next three yellow, last one red.
         */
        LEDS[1].G = LEDS[ 9].G = intensities[0];
        LEDS[2].G = LEDS[10].G = intensities[1];
        LEDS[3].G = LEDS[11].G = intensities[2];
        LEDS[4].G = LEDS[12].G = intensities[3];
        LEDS[5].G = LEDS[13].G = intensities[4] >> 1;
        LEDS[5].R = LEDS[13].R = intensities[4];
        LEDS[6].G = LEDS[14].G = intensities[5] >> 1;
        LEDS[6].R = LEDS[14].R = intensities[5];
        LEDS[7].G = LEDS[15].G = intensities[6] >> 1;
        LEDS[7].R = LEDS[15].R = intensities[6];
        LEDS[8].R = LEDS[ 0].R = intensities[7];

        leds_draw();
    }
}

static void modes_audio_fade()
{
    uint8_t i;
    uint16_t hue = 0;
    uint8_t val = 0;

    for(i=0; i<16; i++) {
        LEDS[i].R = LEDS[i].G = LEDS[i].B = 0;
    }

    dsp_init();

    while(1) {
        val = dsp_audio_level >> 1;
        if(val > 31) val = 31;
        hue += val;

        for(i=0; i<16; i++) {
            colours_set(i, hue>>11, val);
        }

        leds_draw();
    }
}
