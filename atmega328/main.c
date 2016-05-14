/*
 * VehicleCode.c
 *
 * Created: 22/04/2016 10:06:38 PM
 * Author : William
 */

#define F_CPU  8000000UL// Clock Speed
#define FOSC 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdlib.h>
#include "driveMotor.h"
#include "servoControl.h"
#include "serialio.h"
#include "timer2.h"



int main(void){
	CLKPR = (1<<CLKPCE);//Enable changing the clock pre-scaler
	CLKPR = 0;//Turn off the clock pre-scaler
	motor_init(); //Initialize Motors
	servo_init(); //Initialize Servos
	sei();//enable interrupts
	init_serial(1);//Initialize Serial Communication - (25 = 9600 baud at 8MHz)
	init_timer2();//Initialize Timer 2 Interrupt Code
	while (1) {
		//The interrupts for serial are on, when ever commands are sent the vehicle will do shit
		servo_angle(0);
		
	}
}
