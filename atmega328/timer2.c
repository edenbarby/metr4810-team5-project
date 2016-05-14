/*
 * timer2.c
 *
 * Created: 8/05/2016 11:46:39 AM
 *  Author: s4318884
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "driveMotor.h"

void init_timer2(){
	
	TCNT2 = 0; //Set Timer Counter to 0
	OCR2A = 100; //Roughly 10Hz (100ms)
	TCCR2A = (1<<WGM21);//Clear timer counter on match
	TCCR2B = (1<<CS22)|(1<<CS21)|(1<<CS20);//Divide by 1024 prescaler
	TIMSK2 |= (1<<OCIE2A);//Enable interrupt on Output compare match
	TIFR2 &= (1<<OCF2A);//Clear Interrupt Flag
}

ISR(TIMER2_COMPA_vect){
	//Stop the motors
	motor1('c');
	motor2('h');
}
