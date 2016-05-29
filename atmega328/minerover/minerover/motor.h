/*
*******************************************************************************
** @file  motor.h
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


#ifndef MOTOR_H
#define MOTOR_H


/* includes ******************************************************************/

#include "config.h"

/* external typedef **********************************************************/
/* external define ***********************************************************/
/* external macro ************************************************************/
/* external variables ********************************************************/
/* external function prototypes **********************************************/

extern void motor_init(void);
extern void motor_left(uint8_t speed, int8_t direction);
extern void motor_right(uint8_t speed, int8_t direction);


#endif // MOTOR_H