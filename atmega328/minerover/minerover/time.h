/*
*******************************************************************************
** @file  time.h
** @brief Provides time tracking functionality.
*******************************************************************************
** external functions
*******************************************************************************
** time_init     Initialises the timer.
** time_ms       Returns the number of milliseconds since intialisation.
** time_delay_ms Delays for a specified number of milliseconds.
*******************************************************************************
*/


#ifndef TIME_H
#define TIME_H


/* includes ******************************************************************/

#include "config.h"

/* external typedef **********************************************************/
/* external define ***********************************************************/
/* external macro ************************************************************/
/* external variables ********************************************************/
/* external function prototypes **********************************************/

extern void time_init(void);
extern float time_ms(void);
extern void time_delay_ms(float delay);


#endif // TIME_H