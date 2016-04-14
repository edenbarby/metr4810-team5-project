/*
 * spi.h
 *
 * Created: 26/08/2013 8:46:06 PM
 *  Author: Eden Barby
 */


#ifndef SPI_H_
#define SPI_H_

/* INCLUDES *****************************************************************/

#include <stdint.h>

/* DEFINES ******************************************************************/
/* FUNCTION PROTOTYPES ******************************************************/

/* 
 * 
 */
void init_spi(void);

/* 
 * 
 */
uint8_t spi(uint8_t data);

#endif /* SPI_H_ */