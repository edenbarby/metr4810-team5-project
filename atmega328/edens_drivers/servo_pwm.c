/*
 * servo_pwm.c
 *
 * Created: 30/08/2013 5:07:12 PM
 *  Author: Eden Barby
 */


/* INCLUDES *****************************************************************/

#include "cpu_clock.h"

#include <avr/io.h>
#include <stdint.h>

/* PRIVATE TYPEDEFS *********************************************************/
/* PRIVATE DEFINES **********************************************************/

#define EN_SERV0 1	// P8 Header - Timer 0B	
#define EN_SERV1 1	// P10 Header - Timer 1B
#define EN_SERV2 0	// P11 Header - Timer 2B

//#define PWM_FREQ 50L

#define T0_TOP 78 /* Value chosen assuming a timer clock prescaler of 256. */
#define T0_MAX 10 /* Servo pulse width at maximum angle is 12.25% of period. */
#define T0_DEF 6 /* Servo pulse width at centre angle is 7.25% of period. */
#define T0_MIN 2 /* Servo pulse width at minimum angle is 2.25% of period. */

#define T1_TOP 10000 /* Value chosen assuming a timer clock prescaler of 8. */
#define T1_MAX 1150
#define T1_DEF 725
#define T1_MIN 300

#define T2_TOP 78 /* Value chosen assuming a timer clock prescaler of 256. */
#define T2_MAX 10
#define T2_DEF 6
#define T2_MIN 2

/* PRIVATE MACROS ***********************************************************/
/* PRIVATE VARIABLES ********************************************************/
/* PRIVATE FUNCTION PROTOTYPES **********************************************/


void init_servo(void) {
	/* Timer Counter 0B */
	if(EN_SERV0) {
		/* Set PWM to clear on up count and set on down count. */
		TCCR0A |= (1 << COM0B1) | (0 << COM0B0);
		
		/* Set PWM mode to phase correct. */
		TCCR0A |= (0 << WGM01) | (1 << WGM00);
		TCCR0B |= (1 << WGM02);
		
		/* Set PWM prescaler to 8. */
		TCCR0B |= (1 << CS02) | (0 << CS01) | (1 << CS00);
		
		/* Set servo to default position. */
		OCR0A = (uint8_t)(T0_TOP);
		OCR0B = (uint8_t)(T0_DEF);
		
		DDRD |= (1 << PORTD5);
	}
	
	/* Timer Counter 1B */
	if(EN_SERV1) {
		/* Set PWM to clear on up count and set on down count. */
		TCCR1A |= (1 << COM1B1) | (0 << COM1B0);
		
		/* Set PWM mode to phase and frequency correct. */
		TCCR1A |= (0 << WGM11) | (1 << WGM10);
		TCCR1B |= (1 << WGM13) | (0 << WGM12);
		
		 /* Set PWM prescaler to 8. */
		TCCR1B |= (0 << CS12) | (1 << CS11) | (0 << CS10);
		
		/* Set servo to default position. */
		OCR1AH = (uint8_t)(T1_TOP >> 8);
		OCR1AL = (uint8_t)(T1_TOP);
		OCR1BH = (uint8_t)(T1_DEF >> 8);
		OCR1BL = (uint8_t)(T1_DEF);
		
		DDRB |= (1 << PORTB2);
	}
	
	// Timer Counter 2B
	if(EN_SERV2) {
		
	}
}

uint8_t set_angle(uint16_t angle, uint8_t servo) {
	if(servo == 0) {
		if(!EN_SERV0) return 1;
		OCR0B = (uint8_t)(angle);
	}
		
	if(servo == 1) {
		if(!EN_SERV1) return 1;
		OCR1BH = (uint8_t)(angle >> 8);
		OCR1BL = (uint8_t)(angle);
	}
	
	if(servo == 2) {
		if(!EN_SERV2) return 1;
	}
	
	return 0;
}

uint8_t inc_angle(uint16_t angle, uint8_t servo) {
	if(servo == 0) {
		if(!EN_SERV0) return 1;
	}
	
	if(servo == 1) {
		if(!EN_SERV1) return 1;
	}
	
	if(servo == 2) {
		if(!EN_SERV2) return 1;
	}
	
	return 0;
}