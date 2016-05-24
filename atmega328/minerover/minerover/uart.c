/* includes ******************************************************************/

#include "uart.h"

/* private define ************************************************************/

#define ECHO 0

#define UART_BAUD  9600L
#define UART_SPEED 1
#define UART_UBRR  ((F_CPU * UART_SPEED) / (16 * UART_BAUD) - 1)

#define UART_BUFFER_SIZE 100
#define UART_BUFFER_TYPE uint16_t

/* private typedef ***********************************************************/

struct BufferUART {
    uint8_t          buffer[UART_BUFFER_SIZE];
    UART_BUFFER_TYPE index;
    UART_BUFFER_TYPE bytes;
    uint8_t          overrun;
};

/* private macro *************************************************************/
/* private variables *********************************************************/

static volatile struct BufferUART uart_buffer_output;
static volatile struct BufferUART uart_buffer_input;

/* private function prototypes ***********************************************/


extern void uart_init(void)
{
    // Setup the buffers by clearing them.
    memset((void *)uart_buffer_output.buffer, 0, UART_BUFFER_SIZE);
    uart_buffer_output.index   = 0;
    uart_buffer_output.bytes   = 0;
    uart_buffer_output.overrun = 0;

    memset((void *)uart_buffer_input.buffer, 0, UART_BUFFER_SIZE);
    uart_buffer_input.index   = 0;
    uart_buffer_input.bytes   = 0;
    uart_buffer_input.overrun = 0;

    UCSR0A = 0;
    UCSR0B = 0;
    UCSR0C = 0;

    // Set the BAUD rate register.
    UBRR0H = (uint8_t)(UART_UBRR >> 8);
    UBRR0L = (uint8_t)(UART_UBRR >> 0);

    // Disable the UART double transmission speed mode.
    UCSR0A |= (0 << U2X0);

    // Disable multi-processor communication mode.
    UCSR0A |= (0 << MPCM0);

    // Enable the receive complete and the data register empty interrupt and
    // disable the transmit complete interrupt.
    UCSR0B |= (1 << RXCIE0) | (0 << TXCIE0) | (1 << UDRIE0);

    // Enable the receiver and transmitter.
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);

    // Use 8 bit characters.
    UCSR0B |= (0 << UCSZ02);
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);

    // Enable the asynchronous mode.
    UCSR0C |= (0 << UMSEL01) | (0 << UMSEL00);

    // Disable parity.
    UCSR0C |= (0 << UPM01) | (0 << UPM00);

    // Use 1 stop bits.
    UCSR0C |= (0 << USBS0);

    // Write zero to the clock polarity bit because we are using asynchronous mode.
    UCSR0C |= (0 << UCPOL0);
}

extern uint8_t uart_input_available(void)
{
    if(uart_buffer_input.bytes == 0) return 0;
    return 1;
}

extern uint8_t uart_putc(uint8_t c)
{
    uint8_t interrupts_enabled;

    // If the output buffer is full return 1 and set the buffer overrun flag.
    if(uart_buffer_output.bytes >= UART_BUFFER_SIZE) {
        uart_buffer_output.overrun = 1;
        return 1;
    }

    // Copy the status register and disable interrupts before adding to the
    // buffer. This will stop the ISR from modifying the buffer at the same time.
    interrupts_enabled = (SREG >> SREG_I) & 0x1;
    cli();

    uart_buffer_output.buffer[uart_buffer_output.index] = c;
    uart_buffer_output.bytes++;

    // If the index has reached the end, wrap around.
    if(uart_buffer_output.index + 1 == UART_BUFFER_SIZE) {
        uart_buffer_output.index = 0;
    } else uart_buffer_output.index++;

    // Ensure that the data register empty interrupt is enabled to handle transmission of
    // the new data.
    UCSR0B |= (1 << UDRIE0);

    // Return the status register to its original state. Re-enabling interrupts
    // if they were disabled.
    if(interrupts_enabled) sei();

    return 0;
}

extern uint8_t uart_getc(void)
{
    uint8_t c;
    uint8_t interrupts_enabled;

    // Wait for input
    while(uart_buffer_input.bytes == 0);

    interrupts_enabled = (SREG >> SREG_I) & 0x1;
    cli();

    if(uart_buffer_input.index - uart_buffer_input.bytes < 0) {
        c = uart_buffer_input.buffer[uart_buffer_input.index - uart_buffer_input.bytes + UART_BUFFER_SIZE];
    } else {
        c = uart_buffer_input.buffer[uart_buffer_input.index - uart_buffer_input.bytes];
    }

    uart_buffer_input.bytes--;

    if(interrupts_enabled) sei();

    return c;
}

ISR(USART_RX_vect) {
    char c;

    c = UDR0;

    if(ECHO) uart_putc(c);
    
    if(uart_buffer_input.bytes >= UART_BUFFER_SIZE) {
        uart_buffer_input.overrun = 1;
        return;
    }

    uart_buffer_input.buffer[uart_buffer_input.index] = c;
    uart_buffer_input.bytes++;

    if(uart_buffer_input.index + 1 == UART_BUFFER_SIZE) {
        uart_buffer_input.index = 0;
    } else uart_buffer_input.index++;
}

ISR(USART_UDRE_vect) {
    if(uart_buffer_output.bytes > 0) {
        if(uart_buffer_output.index - uart_buffer_output.bytes < 0) {
            UDR0 = uart_buffer_output.buffer[uart_buffer_output.index -
                                             uart_buffer_output.bytes + UART_BUFFER_SIZE];
        } else {
            UDR0 = uart_buffer_output.buffer[uart_buffer_output.index -
                                             uart_buffer_output.bytes];
        }

        uart_buffer_output.bytes--;
    } else {
        // If there is no more bytes to transmit clear the data register empty interrupt.
        UCSR0B &= ~(1 << UDRIE0);
    }
}