/* includes ******************************************************************/

#include "config.h"
#include "motor.h"
#include "servo.h"
#include "time.h"
#include "uart.h"

/* private typedef ***********************************************************/
/* private define ************************************************************/

#define START_BYTE 0x12
#define STOP_BYTE 0x21
#define ACK 0xF8

/* private macro *************************************************************/
/* private variables *********************************************************/
/* private function prototypes ***********************************************/


int main(void)
{
    uint8_t i;
    int8_t speed;
    int8_t input[5];

    motor_init();
    servo_init();
    time_init();
    uart_init();

    sei();

    for(EVER) {
        // The serial packet will consist of a START byte, a value for the
        // left, right motor and servo and a STOP byte. The motor values will
        // be centered on 100 (i.e. a value of 100 is a motor speed of 0 with 0
        // direction). The servo angles will go from 0 to 180.
        input[0] = uart_getc();
        
        if(input[0] == START_BYTE) {
            for(i = 1; i < 5; i++) input[i] = uart_getc();

            if(input[4] == STOP_BYTE) {
                speed = input[1] - 100;
                if(speed > 0) motor_left(speed, 1);
                else if(speed == 0) motor_left(0, 0);
                else if(speed < 0) motor_left(-speed, -1);
                
                speed = input[2] - 100;
                if(speed > 0) motor_right(speed, 1);
                else if(speed == 0) motor_right(0, 0);
                else if(speed < 0) motor_right(-speed, -1);
                
                servo_claw_angle(input[3]);
                
                uart_putc(ACK);
            }
        }
    }
}