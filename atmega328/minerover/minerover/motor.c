/* Port D 6 -> Timer 0 B -> 1 -> LEFT
** Port B 4 -> 1 -> LEFT
** Port B 5 -> 1 -> LEFT
** B4 == 1 && B5 == 0 -> Forward
**
** Port D 5 -> Timer 0 A -> 2 -> RIGHT
** Port D 2 -> 2 -> RIGHT
** Port D 7 -> 2 -> RIGHT
** D2 == 1 && D7 == 0 -> Forward
*/

/* includes ******************************************************************/

#include "motor.h"

/* private typedef ***********************************************************/
/* private define ************************************************************/

#define TIMER0_PRESCALER 1

/* private macro *************************************************************/
/* private variables *********************************************************/
/* private function prototypes ***********************************************/


extern void motor_init(void)
{
    DDRB |= (1 << PORTB5) | (1 << PORTB4);
    DDRD |= (1 << PORTD6) | (1 << PORTD7) | (1 << PORTD5) | (1 << PORTD2);

    PORTB &= ~((1 << PORTB4) | (1 << PORTB5));
    PORTD &= ~((1 << PORTD2) | (1 << PORTD7));

    OCR0A = 0;
    OCR0B = 0;
    
    // Clear timer control registers.
    TCCR0A = 0;
    TCCR0B = 0;
    
    // Set timer 0 A and B to clear on compare match.
    TCCR0A |= (1 << COM0A1) | (0 << COM0A0);
    TCCR0A |= (1 << COM0B1) | (0 << COM0B0);
    
    // Set timer 0 to fast PWM with a TOP value of 0xFF.
    TCCR0A |= (1 << WGM01) | (1 << WGM00);
    TCCR0B |= (0 << WGM02);
    
    // No clock prescaler.
    TCCR0B |= (0 << CS02) | (0 << CS01) | (1 << CS00);
}

extern void motor_left(uint8_t speed, int8_t direction)
{
    if(speed < 0) speed = 0;
    if(speed > 100) speed = 100;
    
    if(direction > 0) {
        // Forward
        PORTB |=  (1 << PORTB4);
        PORTB &= ~(1 << PORTB5);
    } else if(direction == 0) {
        // Stationary
        PORTB &= ~(1 << PORTB4);
        PORTB &= ~(1 << PORTB5);
    } else if(direction < 0) {
        // Backward
        PORTB &= ~(1 << PORTB4);
        PORTB |=  (1 << PORTB5);
    }
    
    OCR0B = (uint8_t)((speed * 255) / 100);
}

extern void motor_right(uint8_t speed, int8_t direction)
{
    if(speed < 0) speed = 0;
    if(speed > 100) speed = 100;
    
    if(direction > 0) {
        // Forward
        PORTD |=  (1 << PORTD2);
        PORTD &= ~(1 << PORTD7);
        } else if(direction == 0) {
        // Stationary
        PORTD &= ~(1 << PORTD2);
        PORTD &= ~(1 << PORTD7);
        } else if(direction < 0) {
        // Backward
        PORTD &= ~(1 << PORTD2);
        PORTD |=  (1 << PORTD7);
    }
    
    OCR0A = (uint8_t)((speed * 255) / 100);
}