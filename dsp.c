#include "dsp.h"
#include <avr/io.h>
#include <avr/interrupt.h>

/* Output the current detected audio level. */
volatile uint8_t dsp_audio_level = 0;

/* See filters.py for generation.
 * Low pass filter with cutoff around 1/16 nyquist,
 * which is around 150Hz for 4800Hz sampling.
 */
static const int8_t lpf_h[31] = {
    0, 1, 1, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6, 7, 7, 7,
    7, 7, 6, 6, 6, 5, 5, 4, 4, 3, 3, 2, 1, 1, 0
};

/* Store low pass filter shift register */
static volatile int8_t lpf_z[31];
static volatile int8_t* lpf_z_p = lpf_z;

/* Set up the ADC */
static void adc_init()
{
    /* VCC as reference, left-aligned, select channel 1 (PB2) */
    ADMUX = (1<<ADLAR) | (1<<MUX0);

    /* Enable ADC, auto trigger, enable interrupt, prescaler=/128 */
    ADCSRA = (1<<ADEN) | (1<<ADATE) | (1<<ADIE)
           | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);

    /* Set auto trigger source to ADC interrupt, enabling free-running */
    ADCSRB = 0;

    /* Start continuous conversion */
    ADCSRA |= (1<<ADSC);
}

/*
 * Call to start up the DSP systems */
void dsp_init()
{
    /* Initialise the LPF filter */
    uint8_t i;
    for(i=0; i<31; i++) {
        lpf_z[i] = 0;
    }

    /* Initialise the ADC */
    adc_init();
}

/* ADC interrupt is where the magic happens */
ISR(ADC_vect)
{
    /* Just do a real basic current-volume thing */
    int8_t x = ADCH - 127;
    if(x < 0) {
        dsp_audio_level = 2*(uint8_t)(-x);
    } else {
        dsp_audio_level = 2*(uint8_t)(x);
    }

    /* Insert ADC reading into LPF shift reg */
    /**lpf_z_p = ADCH - 127;*/

    /* Process filter */
    /*int16_t acc = 0;*/
    /*int8_t i, j = 0;*/

    /* Current position to start inclusive */
    /*for(i=(lpf_z_p - lpf_z); i>=0; i--) {*/
        /*acc += lpf_h[j++] * lpf_z[i];*/
    /*}*/

    /* End to current position exclusive */
    /*for(i=30; i>(lpf_z_p - lpf_z); i--) {*/
        /*acc += lpf_h[j++] * lpf_z[i];*/
    /*}*/

    /* Output 2*abs(y) */
    /*int8_t y = acc >> 8;*/
    /*if(y >= 0) {*/
        /*dsp_audio_level = 2 * (uint8_t)y;*/
    /*} else {*/
        /*dsp_audio_level = 2 * (uint8_t)(-y);*/
    /*}*/

    /* Update LPF pointer */
    /*lpf_z_p++;*/
    /*if(lpf_z_p > lpf_z + 30) {*/
        /*lpf_z_p = lpf_z;*/
    /*}*/
}
