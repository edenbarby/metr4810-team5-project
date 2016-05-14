/*
 * FILE: driveMotor.c
 *
 * Written by William Rigby.
 * 
 * Motor Control Functions
 *
 */

#include <avr/io.h>
#include <stdint.h>

void motor_init(){
	DDRB |= (1<<PORTB4)|(1<<PORTB5); //PB4 & PB5 as outputs
	DDRD |= (1<<PORTD2)|(1<<PORTD5)|(1<<PORTD6)|(1<<PORTD7); //Set PD2, PD5, PD6 & PD7 as outputs
	//Configure TIMER0 for Fast PWM for motors, no clock prescaling, fastpwm (mode 3), non-inverting
	TCCR0A = (1<<COM0A1)|(1<<COM0B1)|(1<<WGM01)|(1<<WGM00);
	TCCR0B = (1<<CS00);
}

void motor1(char x){
	/*
	Takes a char a->e and uses this to set the motor to 1 of 5 speeds:
	Fast Forward, Forward, Stop, Reverse, Fast Reverse
	*/
	#define motor1a 4 //motor 1a pin
	#define motor1b 5 //motor 1b pin
	#define motor1en 5//motor 1 enable pin
	//PORTD |= (1<<motor1en);
	if(x == 'a'){ //Motor 1 100%
		PORTB |= (1<<motor1a);
		PORTB &= ~(1<<motor1b);
		OCR0A = 255;
	}
	else if(x == 'b'){//Motor 1 50% (Implement PWM)
		PORTB |= (1<<motor1a);
		PORTB &= ~(1<<motor1b);
		OCR0A = 125;
	}
	else if(x == 'c'){//Motor 1 0%
		PORTB &= ~(1<<motor1a);
		PORTB &= ~(1<<motor1b);
		OCR0A = 0;
	}
	else if(x == 'd'){//Motor 1 -50%
		PORTB &= ~(1<<motor1a);
		PORTB |= (1<<motor1b);
		OCR0A = 255;
	}
	else if(x == 'e'){//Motor 1 -100%
		PORTB &= ~(1<<motor1a);
		PORTB |= (1<<motor1b);
		OCR0A = 125;
	}
}

void motor2(char x){
	#define motor2a 2 //motor 2a pin
	#define motor2b 7 //motor 2b pin
	#define motor2en 6//motor 2 enable pin
	//PORTD |= (1<<motor2en);
	if(x == 'f'){ //Motor 2 100%
		PORTD |= (1<<motor2a);
		PORTD &= ~(1<<motor2b);
		OCR0B = 255;
	}
	else if(x == 'g'){//Motor 2 50% (Implement PWM)
		PORTD |= (1<<motor2a);
		PORTD &= ~(1<<motor2b);
		OCR0B = 125;
	}
	else if(x == 'h'){//Motor 2 0%
		PORTD &= ~(1<<motor2a);
		PORTD &= ~(1<<motor2b);
		OCR0B = 0;
	}
	else if(x == 'i'){//Motor 2 -50%
		PORTD &= ~(1<<motor2a);
		PORTD |= (1<<motor2b);
		OCR0B = 125;
	}
	else if(x == 'j'){//Motor 2 -100%
		PORTD &= ~(1<<motor2a);
		PORTD |= (1<<motor2b);
		OCR0B = 255;
	}
}