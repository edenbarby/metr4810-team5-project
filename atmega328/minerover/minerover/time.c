/*
*******************************************************************************
** @file  time.c
** @brief Provides time tracking functionality.
*******************************************************************************
** external functions
*******************************************************************************
** time_init     Initialises the timer.
** time_ms       Returns the number of milliseconds since intialisation.
** time_delay_ms Delays for a specified number of milliseconds.
*******************************************************************************
*/


/* includes ******************************************************************/

#include "time.h"

/* private typedef ***********************************************************/
/* private define ************************************************************/

#define TIMER2_PRESCALER (8)
#define TIMER2_COMPARE (10000L)
#define TIMER2_TICK (F_CPU / (TIMER2_PRESCALER))

/* private macro *************************************************************/
/* private variables *********************************************************/

uint64_t tick;

/* private function prototypes ***********************************************/


/*
** @brief  Initialises the timer.
** @param  none
** @retval none
*/
extern void time_init(void)
{
    tick = 0;
    
    TCCR2A = 0;
    TCCR2B = 0;
    TIMSK2 = 0;
    
    OCR2A = (uint8_t)(TIMER2_TICK / TIMER2_COMPARE - 1);
    
    // Set to clear on compare match.
    TCCR2A |= (1 << WGM21) | (0 << WGM20);
    TCCR2B |= (0 << WGM22);
    
    // Set prescaler to 8.
    TCCR2B |= (0 << CS22) | (1 << CS21) | (0 << CS20);
    
    // Enable timer compare match A interrupt.
    TIMSK2 |= (0 << OCIE2B) | (1 << OCIE2A) | (0 << TOIE2);
}

/*
** @brief  Returns the number of milliseconds since intialisation.
** @param  none
** @retval Milliseconds since intialisation.
*/
extern float time_ms(void)
{
    return ((float)(1000 * tick) / TIMER2_COMPARE);
}

/*
** @brief  Delays for a specified number of milliseconds.
** @param  delay Delay in milliseconds.
** @retval none
*/
extern void time_delay_ms(float delay)
{
    float start = time_ms();
    while((time_ms() - start) < delay) _delay_us(50);
}

ISR(TIMER2_COMPA_vect) {
    tick++;
}