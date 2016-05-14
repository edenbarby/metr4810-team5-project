/*
 * UART.c
 *
 * Created: 6/05/2016 9:58:51 PM
 *  Author: William
 */ 

#include <avr/io.h>
#include "driveMotor.h"
#include <avr/interrupt.h>
#include "servoControl.h"

void UART_init(unsigned int ubrr){
	//Set baud rate registers
	UBRR0 = ubrr;
	
	//Enable Data Transmission and Receiving - 8bit word, Asynchronous Operation, Enable UART Receive Interrupt
	UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
	UCSR0C =(1<<UCSZ00)|(1<<UCSZ01);
	
	
}

unsigned char UART_Receive(){
	//Wait for data to be received
	while(!(UCSR0A&(1<<RXC0)));
	return UDR0;
}

ISR(USART_RX_vect){
	char data = UART_Receive();//Read Serial
	motor1(data);//Write Motor 1 Value
	motor2(data);//Write Motor 2 Value
	servo_control(data);//Write Servo Angle
	TCNT2 = 0;
}