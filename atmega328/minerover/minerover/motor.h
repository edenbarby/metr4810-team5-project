#ifndef MOTOR_H
#define MOTOR_H


/* includes ******************************************************************/

#include "config.h"

/* external typedef **********************************************************/
/* external define ***********************************************************/
/* external macro ************************************************************/
/* external variables ********************************************************/
/* external function prototypes **********************************************/

extern void motor_init(void);
extern void motor_left(uint8_t speed, int8_t direction);
extern void motor_right(uint8_t speed, int8_t direction);

#endif // MOTOR_H