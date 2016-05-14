/*
 * servoControl.c
 *
 * Created: 6/05/2016 9:49:24 PM
 *  Author: William
 */ 

#include <avr/io.h>
#include <stdint.h>
#define F_CPU 8000000UL
void servo_init(){
	OCR1A = 1500;
	DDRB |= (1<<PORTB1);//Set PORTB to output
	//Configure TIMER1 to FAST PWM Mode 14, clk/8 from Prescaler
	TCCR1A = (1<<WGM11)|(1<<COM1A1);
	TCCR1B = (1<<WGM13)|(1<<WGM12)|(1<<CS11);
	ICR1 = 19999; //20ms Duty Cycle
	OCR1A = 1500;
}

void servo_angle(float angle){
	uint16_t val = 1000+((1000*angle)/180);
	OCR1A = val;
}

void servo_control(char val){
	if(val == '0'){
		servo_angle(0);
	}
	else if(val == '1'){
		servo_angle(20);
	}
	else if(val == '2'){
		servo_angle(40);
	}
	else if(val == '3'){
		servo_angle(60);
	}
	else if(val == '4'){
		servo_angle(80);
	}
	else if(val == '5'){
		servo_angle(100);
	}
	else if(val == '6'){
		servo_angle(120);
	}
	else if(val == '7'){
		servo_angle(140);
	}
	else if(val == '8'){
		servo_angle(160);
	}
	else if(val == '9'){
		servo_angle(180);
	}
	
}