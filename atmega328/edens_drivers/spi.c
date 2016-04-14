/*
 * spi.c
 *
 * Created: 23/08/2013 2:57:38 PM
 *  Author: Eden Barby
 */


/* INCLUDES *****************************************************************/

#include <avr/io.h>
#include <stdint.h>

/* PRIVATE TYPEDEFS *********************************************************/
/* PRIVATE DEFINES **********************************************************/

/* Pin mapping. */
#define SCK		PORTB5
#define MISO	PORTB4
#define MOSI	PORTB3

/* PRIVATE MACROS ***********************************************************/
/* PRIVATE VARIABLES ********************************************************/
/* PRIVATE FUNCTION PROTOTYPES **********************************************/


void init_spi(void) {
	DDRB |= (1 << SCK) | (1 << MOSI);
	
	/* PORTB2 is the predefined SS pin of the ATmega328. If this pin is driven
	 * low, for any reason, the chip changes from master to slave mode. */
	DDRB |= (1 << PORTB2);
	PORTB |= (1 << PORTB2);
	
	/* Leave interrupts disabled, use default endianness, set ATmega as
	 * master, default polarity and phase and set clock to F_CPU/4.
	 */
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1) | (1 << SPR0);
}

uint8_t spi(uint8_t data) {
	SPDR = data;
	while(!(SPSR & (1 << SPIF)));
	return SPDR;
}