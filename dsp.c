#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "dsp.h"
#include "luts.h"

/* Output the current detected audio level. */
volatile uint8_t dsp_audio_level = 0;

/* Store CIC filter related variables. */
static volatile int16_t cic_i_reg = 0;
static volatile int16_t cic_c_reg = 0;
static volatile uint8_t cic_decimator = 0;

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

/* Call to start up the DSP systems */
void dsp_init()
{
    adc_init();
}

/*
 * ADC interrupt routine called at 4800Hz.
 * Reads ADC, preprocesses to remove DC bias and square using a lookup table
 * that returns the higher result bits in an int8_t,
 * then runs through a 1st-order R=D=256 CIC filter to low-pass and decimate,
 * outputs uint8_t result 0-255 at 4800Hz/256=18.75Hz.
 */
ISR(ADC_vect)
{
    int8_t x = pgm_read_byte(&(lut_dsp_input[ADCH]));
    cic_i_reg += x;
    if(++cic_decimator == 0) {
        dsp_audio_level = (uint16_t)(cic_i_reg - cic_c_reg) >> 7;
        cic_c_reg = cic_i_reg;
    }
}
