/*
*******************************************************************************
** @file  servo.c
** @brief ATmega328 SG90 9 gram servo driver. Servo line 1 is controlled with
**        timer 1B and line 2 with timer 1A.
*******************************************************************************
** external functions
*******************************************************************************
** servo_init       Initialises the servo driver.
** servo_claw_angle Sets the servo angle.
*******************************************************************************
*/


#ifndef SERVO_H
#define SERVO_H


/* includes ******************************************************************/

#include "config.h"

/* external typedef **********************************************************/
/* external define ***********************************************************/

//#define SERVO1_EN
#define SERVO2_EN

/* external macro ************************************************************/
/* external variables ********************************************************/
/* external function prototypes **********************************************/

extern void servo_init(void);
extern void servo_claw_angle(uint8_t angle);


#endif // SERVO_H