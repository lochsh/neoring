/*
 * leds.c
 * LED writing functions
 */

#include "leds.h"

/* Store the LED state */
volatile LED LEDS[16];

/* Send out the LED data */
void leds_draw()
{
    /* Let the compiler pick registers for a, b, and c */
    uint8_t a = 0, b = 0, c = 0;

    __asm__ __volatile__ (
        /*
         * %0 - big counter, reset timer / number of bytes sent
         * %1 - small counter, number of bits sent in byte
         * %2 - value, current value to send
         */
        /* Disable all interrupts */
        "    cli                \n\t"

        /* Send RESET signal for ~55us */
        "    cbi  24, 0         \n\t" /* 2   cycles */
        "    ldi  %0, 140       \n\t" /* 1   cycles */
        "l0: dec  %0            \n\t" /* 1   cycles */
        "    brne l0            \n\t" /* 1/2 cycles */

        /* We'll read out 16 LEDs with 3 bytes each */
        "    ldi %0,48          \n\t" /* 1   cycles */

        /* Read the current byte to send into %2, incrementing pointer */
        "l1: ld  %2, %a[LEDS]+  \n\t" /* 2   cycles */
        "    ldi %1, 8          \n\t" /* 1   cycles */

        /* Start sending the HIGH pulse */
        "l2: sbi 24, 0          \n\t" /* 2   cycles */
        /* Read the next bit of this byte, and branch if it's 1 */
        "    lsl  %2            \n\t" /* 1   cycles */
        "    brcs l3            \n\t" /* 1/2 cycles */

        /* Send a 0: immediately go low, then jump past the '1' stuff */
        "    cbi 24, 0          \n\t" /* 2   cycles */
        "    rjmp l4            \n\t" /* 2   cycles */

        /* Send a 1: a little extra delay (plus branch delay), then go low */
        "l3: nop                \n\t" /* 1   cycles */
        "    cbi 24, 0          \n\t" /* 2   cycles */

        /* Next bit: decrement counter, go around if bits left to send */
        "l4: dec %1             \n\t" /* 1   cycles */
        "    brne l2            \n\t" /* 1/2 cycles */

        /* Check if we've done all 48 bytes */
        "l5: dec %0             \n\t" /* 1   cycles */
        "    brne l1            \n\t" /* 1/2 cycles */

        /* Re-enable interrupts */
        "    sei                \n\t"
    : "+a"(a), "+a"(b), "+a"(c)
    : [LEDS] "e" (LEDS)
    :
    );
}
