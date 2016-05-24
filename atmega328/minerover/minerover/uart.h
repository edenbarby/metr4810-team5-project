#ifndef UART_H
#define UART_H


/* includes ******************************************************************/

#include "config.h"

/* external typedef **********************************************************/
/* external define ***********************************************************/
/* external macro ************************************************************/
/* external variables ********************************************************/

extern FILE uart;

/* external function prototypes **********************************************/

extern void uart_init(void);
extern uint8_t uart_input_available(void);
extern uint8_t uart_putc(uint8_t c);
extern uint8_t uart_getc(void);


#endif // UART_H