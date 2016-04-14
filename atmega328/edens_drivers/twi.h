/*
 * twi.h
 *
 * Created: 25/09/2013 6:05:58 PM
 *  Author: Eden Barby
 */


#ifndef I2C_H_
#define I2C_H_


/* INCLUDES *****************************************************************/

#include <stdint.h>

/* DEFINES ******************************************************************/
/* FUNCTION PROTOTYPES ******************************************************/

/* Initialize two wire interface.
 * 
 */
void init_twi(void);

/* Read num bytes starting from regAddrStart. Each subsequent byte will be
 * from the register above (newRegPtr = oldRegPtr + 1).
 * 
 * Input:	regAddrStart	The register address to start reading bytes from
 *			data			Location to store read data
 *			num				The number of registers to be read
 * Return:	0				Succeeded
 *			>0				Failed, see handle_error() for relavant error codes
 */
uint8_t twi_read_bytes(uint8_t regAddrStart, uint8_t *data, uint8_t num);

/* Writes num bytes starting from regAddrStart. Each subsequent byte will be written to the register above (newRegPtr = oldRegPtr + 1).
 * 
 * Input:	regAddrStart	The register address to start reading bytes from
 *			data			Pointer to data to be written
 *			num				The number of registers to be read
 * Return:	0				Succeeded
 *			>0				Failed, see handle_error() for relavant error codes
 */
uint8_t twi_write_bytes(uint8_t regAddrStart, uint8_t *data, uint8_t num);

/* Writes num bytes starting from regAddrStart. Each subsequent byte will be written to the register above (newRegPtr = oldRegPtr + 1). Respects regMasks.
 * 
 * 76543210
 * 00010100 value[n] to write
 * 00011100 mask[n] byte
 * 01101101 original register[regAddrStart + n]
 * 01100001 original & ~mask
 * 01110101 masked | value
 * 
 * Input:	regAddrStart	The register address to start reading bytes from
 *			data			Pointer to data to be written
 *			regMasks		Pointer to byte masks for each register
 *			num				The number of registers to be read
 * Return:	0				Succeeded
 *			>0				Failed, see handle_error() for relavant error codes
 */
uint8_t twi_write_bits(uint8_t regAddr, uint8_t *data, uint8_t *regMasks, uint8_t num);


#endif /* I2C_H_ */