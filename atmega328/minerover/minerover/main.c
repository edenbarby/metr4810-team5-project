/*
*******************************************************************************
** @file  main.c
** @brief METR4810 mine rescue system project. Programmed for the ATmega328.
*******************************************************************************
*/


/* includes ******************************************************************/

#include "config.h"
#include "motor.h"
#include "servo.h"
#include "time.h"
#include "uart.h"

/* private typedef ***********************************************************/
/* private define ************************************************************/

#define START 0x12
#define STOP 0x89
#define ACK 0xF8
#define NACK 0x08

#define SERIAL_TIMEOUT 100 // (ms)

/* private macro *************************************************************/
/* private variables *********************************************************/
/* private function prototypes ***********************************************/


int main(void)
{
    DDRB |= (1 << PORTB2);
    
    uint8_t i;
    int8_t speed;
    uint8_t input[5];
    float time_packet_last;

    motor_init();
    servo_init();
    time_init();
    uart_init();

    sei();

    time_packet_last = time_ms();
    for(EVER) {
        // The serial packet will consists of 5 unsigned bytes as per the
        // following:
        // [START, left motor, right motor, servo angle, STOP]
        // Left motor and right motor will be speed values ranging from -100 to
        // 100 centered around 100 (i.e. 0 - 200). The servo angle will be a
        // a value from 0 to 100. 0 is the servo fully open and 100 is the
        // servo fully closed.
        if(uart_input_available()) {
            input[0] = uart_getc();

            if(input[0] == START) {
                for(i = 1; i < 5; i++) input[i] = uart_getc();

                if(input[4] == STOP) {
                    speed = input[1] - 100;
                    if(speed > 0) motor_left(speed, 1);
                    else if(speed == 0) motor_left(0, 0);
                    else if(speed < 0) motor_left(-speed, -1);
                
                    speed = input[2] - 100;
                    if(speed > 0) motor_right(speed, 1);
                    else if(speed == 0) motor_right(0, 0);
                    else if(speed < 0) motor_right(-speed, -1);
                
                    servo_claw_angle(input[3]);
                
                    time_packet_last = time_ms();
                    uart_putc(ACK);
                } else {
                    uart_putc(NACK);
                }
            }
        }
        
        if(time_ms() - time_packet_last > SERIAL_TIMEOUT) {
            motor_left(0, 0);
            motor_right(0, 0);
        }
    }
}