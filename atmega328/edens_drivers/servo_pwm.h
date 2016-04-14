/*
 * servo_pwm.h
 *
 * Created: 30/08/2013 5:07:26 PM
 *  Author: Eden Barby
 */

#ifndef SERVO_PWM_H_
#define SERVO_PWM_H_

/* INCLUDES *****************************************************************/

#include <stdint.h>

/* DEFINES ******************************************************************/
/* FUNCTION PROTOTYPES ******************************************************/

/*
 *
 */
void init_servo(void);

/*
 *
 */
uint8_t set_angle(uint16_t angle, uint8_t servo);

/*
 *
 */
uint8_t inc_angle(uint16_t angle, uint8_t servo);

#endif /* SERVO_PWM_H_ */