
/*
 * serialio.c
 *
 * Created: 2/09/2013 5:07:26 PM
 *  Author: Eden Barby
 */


/* INCLUDES *****************************************************************/



#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

/* PRIVATE TYPEDEFS *********************************************************/
/* PRIVATE DEFINES **********************************************************/

/* Note that U2X0 is set. */
//#define BAUD 38400
/* Bit rate register value for a bit rate of 38.4kHz. */
#define MYUBRR 25//((F_CPU / (8 * BAUD)) - 1)

#define OUTPUT_BUFFER_SIZE 255

#define INPUT_BUFFER_SIZE 255

/* PRIVATE MACROS ***********************************************************/
/* PRIVATE VARIABLES ********************************************************/

volatile char out_buffer[OUTPUT_BUFFER_SIZE];
volatile uint8_t out_insert_pos;
volatile uint8_t bytes_in_out_buffer;


volatile char input_buffer[INPUT_BUFFER_SIZE];
volatile uint8_t input_insert_pos;
volatile uint8_t bytes_in_input_buffer;
volatile uint8_t input_overrun;

/* Variable to keep track of whether incoming characters are to be echoed
 * back or not.
 */
static int8_t do_echo;

/* PRIVATE FUNCTION PROTOTYPES **********************************************/

static int uart_put_char(char, FILE*);
static int uart_get_char(FILE*);


/* Setup a stream that uses the uart get and put functions. We will
 * make standard input and output use this stream below.
 */
static FILE myStream = FDEV_SETUP_STREAM(uart_put_char, uart_get_char, _FDEV_SETUP_RW);


void init_serial(int8_t echo) {
	/* Initialize our buffers
	 */
	out_insert_pos = 0;
	bytes_in_out_buffer = 0;
	input_insert_pos = 0;
	bytes_in_input_buffer = 0;
	input_overrun = 0;
	
	/*
	 * Record whether we're going to echo characters or not
	 */
	do_echo = echo;
	
	/* Configure the serial port baud rate.
	 */
	UBRR0H = (unsigned char)(MYUBRR >> 8);
	UBRR0L = (unsigned char)(MYUBRR);
	
	/* Receive complete, transmit complete and USART data register empty interrupts enabled.
	 * USART transmit and receive enabled. 2 stop bits. 2 X system clock. Asynchronous
	 * mode enabled.
	 */
	UCSR0A = (1 << U2X0);
	UCSR0B = (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (1 << USBS0) | (1 << UCSZ01) | (1 << UCSZ00);
	
	/* Set up our stream so the put and get functions below are used 
	 * to write/read characters via the serial port when we use
	 * stdio functions.
	 */
	stdout = &myStream;
	stdin = &myStream;
}

int8_t serial_input_available(void) {
	return (bytes_in_input_buffer != 0);
}

void clear_serial_input_buffer(void) {
	input_insert_pos = 0;
	bytes_in_input_buffer = 0;
}

/*
 *
 */
static int uart_put_char(char c, FILE* stream) {
	uint8_t interrupts_enabled;
	
	//if(c == '\n') {
		//uart_put_char('\r', stream);
	//}

	interrupts_enabled = bit_is_set(SREG, SREG_I);
	
	while(bytes_in_out_buffer >= OUTPUT_BUFFER_SIZE) {
		if(!interrupts_enabled) {
			return 1;
		}
	}
	
	cli();
	out_buffer[out_insert_pos++] = c;
	bytes_in_out_buffer++;
	if(out_insert_pos == OUTPUT_BUFFER_SIZE) {
		/* Wrap around buffer pointer if necessary */
		out_insert_pos = 0;
	}
	
	/* Reenable interrupts (UDR Empty interrupt may have been
	 * disabled)
	 */
	UCSR0B |= (1 << UDRIE0);
	if(interrupts_enabled) {
		sei();
	}
	
	return 0;
}

int uart_get_char(FILE* stream) {
	/* Wait until we've received a character */
	while(bytes_in_input_buffer == 0) {
		/* do nothing */
	}
	
	/*
	 * Turn interrupts off and remove a character from the input
	 * buffer. We reenable interrupts if they were on.
	 * The pending character is the one which is byte_in_input_buffer
	 * characters before the insert position (taking into account
	 * that we may need to wrap around).
	 */
	uint8_t interrupts_enabled = bit_is_set(SREG, SREG_I);
	cli();
	char c;
	if(input_insert_pos - bytes_in_input_buffer < 0) {
		/* Need to wrap around */
		c = input_buffer[input_insert_pos - bytes_in_input_buffer
				+ INPUT_BUFFER_SIZE];
	} else {
		c = input_buffer[input_insert_pos - bytes_in_input_buffer];
	}
	
	/* Decrement our count of bytes in the input buffer */
	bytes_in_input_buffer--;
	if(interrupts_enabled) {
		sei();
	}	
	return c;
}

/* USART Rx Complete, place received data into input buffer.
 */
ISR(USART_RX_vect) {
	char c;
	c = UDR0;
	
	if(do_echo && bytes_in_out_buffer < OUTPUT_BUFFER_SIZE) {
		/* If echoing is enabled and there is output buffer
		 * space, echo the received character back to the UART.
		 * (If there is no output buffer space, characters
		 * will be lost.)
		 */
		uart_put_char(c, 0);
	}
	
	/* If the character is a carriage return, turn it into a
	 * linefeed 
	 */
	//if (c == '\r') {
		//c = '\n';
	//}
	
	input_buffer[input_insert_pos++] = c;
	bytes_in_input_buffer++;
	
	if(input_insert_pos == INPUT_BUFFER_SIZE) {
		/* Wrap around buffer pointer if necessary */
		input_insert_pos = 0;
	}
}

/* USART data register empty, move more data from out buffer into data register.
 * Note that this is taking the data place in first hence why
 * 'out_insert_pos - bytes_in_out_buffer' is used.
 */
ISR(USART_UDRE_vect) {
	if(bytes_in_out_buffer > 0) {
		char c;
		
		if(out_insert_pos - bytes_in_out_buffer < 0) {
			/* Wrap around */
			c = out_buffer[out_insert_pos - bytes_in_out_buffer + OUTPUT_BUFFER_SIZE];
		} else {
			c = out_buffer[out_insert_pos - bytes_in_out_buffer];
		}
		/* Decrement count */
		bytes_in_out_buffer--;
		
		UDR0 = c;
	} else {
		/* Clear data register empty interrupt bit. */
		UCSR0B &= ~(1 << UDRIE0);
	}
}
