/*
 * serialio.h
 *
 * Created: 2/09/2013 5:07:26 PM
 *  Author: Eden Barby
 */


#ifndef SERIALIO_H_
#define SERIALIO_H_

/* INCLUDES *****************************************************************/

#include "cpu_clock.h"

#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

/* DEFINES ******************************************************************/
/* FUNCTION PROTOTYPES ******************************************************/

/* Initializes serial I/O for use with PC. Input of 1 enables echo and 0 disables.
 */
void init_serial(int8_t echo);

/* 
 *
 */
int8_t serial_input_available(void);

/* 
 *
 */
void clear_serial_input_buffer(void);

#endif /* SERIALIO_H_ */