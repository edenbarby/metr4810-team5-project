/*
*******************************************************************************
** @file  motor.c
** @brief ATmega328 H-bridge motor driver. The left motor H-bridge enable pin
**        is driven by the timer 0B output compare pin and the directional
**        control is handled by pin B4 and B5. The right motor enable pin is
**        driven by the timer 0A output compare pin and the directional control
**        is handled by pin D2 and D7.
*******************************************************************************
** external functions
*******************************************************************************
** motor_init  Initialises the motor driver.
** motor_left  Controls the left motor speed and direction.
** motor_right Controls the right motor speed and direction.
*******************************************************************************
*/


/* includes ******************************************************************/

#include "motor.h"

/* private typedef ***********************************************************/
/* private define ************************************************************/
/* private macro *************************************************************/
/* private variables *********************************************************/
/* private function prototypes ***********************************************/

/*
** @brief  Initialises the motor driver.
** @param  none
** @retval none
*/
extern void motor_init(void)
{
    // Set all the H-bridge control pins to outputs.
    DDRB |= (1 << PORTB5) | (1 << PORTB4);
    DDRD |= (1 << PORTD6) | (1 << PORTD7) | (1 << PORTD5) | (1 << PORTD2);

    // Set all the directional control pin outputs to 0.
    PORTB &= ~((1 << PORTB4) | (1 << PORTB5));
    PORTD &= ~((1 << PORTD2) | (1 << PORTD7));

    // Clear the output compare registers for the timer.
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

/*
** @brief  Controls the left motor speed and direction.
** @param  speed     The motor speed from 0 to 100.
** @param  direction The motor driection. A positive value for forward, 0 for
**                   stationary and negative for backward.
** @retval none
*/
extern void motor_left(uint8_t speed, int8_t direction)
{
    // Ensure that the given speed is within the bounds.
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

/*
** @brief  Controls the right motor speed and direction.
** @param  speed     The motor speed from 0 to 100.
** @param  direction The motor driection. A positive value for forward, 0 for
**                   stationary and negative for backward.
** @retval none
*/
extern void motor_right(uint8_t speed, int8_t direction)
{
    // Ensure that the given speed is within the bounds.
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