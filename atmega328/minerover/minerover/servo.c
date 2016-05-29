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


/* includes ******************************************************************/

#include "servo.h"

/* private typedef ***********************************************************/
/* private define ************************************************************/

#define TIMER1_PRESCALER (8)
#define TIMER1_TICK      (F_CPU / (2 * TIMER1_PRESCALER))
#define TIMER1_OVERFLOW  (50)

// SERVO_TOP defines the desired PWM period register value.
#define SERVO_TOP     (TIMER1_TICK / TIMER1_OVERFLOW - 1)
// SERVO_MAX and SERVO_MIN define the maximum and minimum compare register
// values.
#define SERVO_MAX     (SERVO_TOP / 10)
#define SERVO_MIN     (SERVO_TOP / 20)
#define SERVO_RANGE   (SERVO_MAX - SERVO_MIN)
#define SERVO_DEFAULT ((SERVO_MAX + SERVO_MIN) / 2)

/* private macro *************************************************************/
/* private variables *********************************************************/
/* private function prototypes ***********************************************/

#ifdef SERVO1_EN
static void servo_one_angle(uint8_t angle);
#endif // SERVO1_EN

#ifdef SERVO2_EN
static void servo_two_angle(uint8_t angle);
#endif // SERVO2_EN


/*
** @brief  Initialises the servo driver.
** @param  none
** @retval none
*/
extern void servo_init(void)
{
    ICR1 = SERVO_TOP;
    
    TCCR1A = 0;
    TCCR1B = 0;
    
#ifdef SERVO1_EN
    DDRB |= (1 << PORTB2);
    
    OCR1B = SERVO_DEFAULT;
    
    // Set timer 1 b to clear on compare match.
    TCCR1A |= (1 << COM1B1) | (0 << COM1B1);
#endif // SERVO1_EN

#ifdef SERVO2_EN
    DDRB |= (1 << PORTB1);
    
    OCR1A = SERVO_DEFAULT;
    
    // Set timer 1 a to clear on compare match.
    TCCR1A |= (1 << COM1A1) | (0 << COM1A1);
#endif // SERVO2_EN


    // Set timer 1 to generate a phase and frequency correct PWM signal with
    // ICR1 as TOP.
    TCCR1A |= (0 << WGM11) | (0 << WGM10);
    TCCR1B |= (1 << WGM13) | (0 << WGM12);

    // Set the timer 1 clock source to have a prescaler of 8.
    TCCR1B |= (0 << CS12) | (1 << CS11) | (0 << CS10);
}

/*
** @brief  Sets the servo angle.
** @param  angle The desired servo angle as a number from 0 to 100.
** @retval none
*/
extern void servo_claw_angle(uint8_t angle)
{
    if(angle > 100) angle = 100;

#ifdef SERVO1_EN
    servo_one_angle(angle);
#endif // SERVO1_EN

#ifdef SERVO2_EN
    servo_two_angle(angle);
#endif // SERVO2_EN
}


#ifdef SERVO1_EN
/*
** @brief  Sets servo 1 angle.
** @param  angle The desired servo angle as a number from 0 to 100.
** @retval none
*/
static void servo_one_angle(uint8_t angle)
{
    OCR1B = (uint16_t)(((SERVO_MAX - SERVO_MIN) * angle) / 100 + SERVO_MIN);
}
#endif // SERVO1_EN

#ifdef SERVO2_EN
/*
** @brief  Sets servo 2 angle.
** @param  angle The desired servo angle as a number from 0 to 100.
** @retval none
*/
static void servo_two_angle(uint8_t angle)
{
    OCR1A = (uint16_t)(((SERVO_MAX - SERVO_MIN) * angle) / 100 + SERVO_MIN);
}
#endif // SERVO2_EN