/*
 * twi.c
 *
 * Created: 25/09/2013 6:05:40 PM
 *  Author: Eden Barby
 */


/* INCLUDES *****************************************************************/

#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>

/* PRIVATE TYPEDEFS *********************************************************/
/* PRIVATE DEFINES **********************************************************/

/* Bit Rate Register = ((CPU Frequency) / (SCL Frequency) - 16) / (2 * Prescaler)
 * CPU Frequency = 8MHz
 * SCL Frequency = 100kHz
 * Prescaler     = 1
 */
#define BRR (uint8_t)(32)
#define IMU_ADDR 0b1101001 /* MPU6050 I2C address. */

/* PRIVATE MACROS ***********************************************************/

#define STATUS() (TWSR & 0xF8)

/* PRIVATE VARIABLES ********************************************************/
/* PRIVATE FUNCTION PROTOTYPES **********************************************/

static uint8_t twi_start(void);
static uint8_t twi_start_r(void);
static uint8_t twi_sla_w(uint8_t address);
static uint8_t twi_sla_r(uint8_t address);
static uint8_t twi_write(uint8_t data);
static uint8_t twi_read_ack(uint8_t *data);
static uint8_t twi_read_nack(uint8_t *data);
static void twi_stop(void);
static uint8_t handle_error(uint8_t status);


/* Initialize two wire interface.
 * 
 */
void init_twi(void) {
	/* Set the bit rate register. Prescaler of 1 is default. */
	TWBR = BRR;
}

/* Read num bytes starting from regAddrStart. Each subsequent byte will be from the register above (newRegPtr = oldRegPtr + 1).
 * 
 * Input:	regAddrStart	The register address to start reading bytes from
 *			data			Location to store read data
 *			num				The number of registers to be read
 * Return:	0				Succeeded
 *			>0				Failed, see handle_error() for relavant error codes
 */
uint8_t twi_read_bytes(uint8_t regAddrStart, uint8_t *data, uint8_t num) {
	uint8_t i, error;
	
	/* Transmit start condition. */
	if((error = twi_start())) return error;
	
	/* Transmit slave address and write bit. */
	if((error = twi_sla_w(IMU_ADDR))) return error;
	
	/* Transmit to slave the register to be read. */
	if((error = twi_write(regAddrStart))) return error;
	
	/* Transmit repeated start condition. */
	if((error = twi_start_r())) return error;
	
	/* Transmit slave address and read bit. */
	if((error = twi_sla_r(IMU_ADDR))) return error;
	
	/* Read data from slave. */
	for(i = 0; i < num; i++) {
		if(i == num - 1) {
			/* Last byte, return NACK. */
			if((error = twi_read_nack(&data[i]))) return error;
		} else {
			if((error = twi_read_ack(&data[i]))) return error;
		}
	}
	
	/* Transmit stop condition. */
	twi_stop();
	
	return 0;
}

/* Writes num bytes starting from regAddrStart. Each subsequent byte will be written to the register above (newRegPtr = oldRegPtr + 1).
 * 
 * Input:	regAddrStart	The register address to start reading bytes from
 *			data			Pointer to data to be written
 *			num				The number of registers to be read
 * Return:	0				Succeeded
 *			>0				Failed, see handle_error() for relavant error codes
 */
uint8_t twi_write_bytes(uint8_t regAddrStart, uint8_t *data, uint8_t num) {
	uint8_t i, error;
	
	/* Transmit start condition. */
	if((error = twi_start())) return error;
	
	/* Transmit slave address and write bit. */
	if((error = twi_sla_w(IMU_ADDR))) return error;
	
	/* Transmit to slave the register to write to. */
	if((error = twi_write(regAddrStart))) return error;
	
	/* Write data to slave. */
	for(i = 0; i < num; i++) {
		if((error = twi_write(data[i]))) return error;
	}
	
	/* Transmit stop condition. */
	twi_stop();
	
	return 0;
}

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
uint8_t twi_write_bits(uint8_t regAddrStart, uint8_t *data, uint8_t *regMasks, uint8_t num) {
	uint8_t i;
	uint8_t buffer[num];
	
	twi_read_bytes(regAddrStart, buffer, num);
	
	for(i = 0; i < num; i++) {
		data[i] &= regMasks[i];
		buffer[i] &= ~regMasks[i];
		buffer[i] |= data[i];
	}
	
	twi_write_bytes(regAddrStart, buffer, num);
	
	return 0;
}

/* Transmits start condition.
 * 
 * Return:	0			Succeeded
 *			>0			Failed, see handle_error() for relavant error codes
 */
static uint8_t twi_start(void) {
	/* Transmit start condition. */
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	
	/* Wait for transmission to complete. */
	while(!(TWCR & (1 << TWINT)));
	
	/* If status register indicates an error, handle it and return the appropriate error code. */
	if(STATUS() != 0x08) return handle_error(STATUS());
	
	return 0;
}

/* Transmits repeated start condition.
 * 
 * Return:	0			Succeeded
 *			>0			Failed, see handle_error() for relavant error codes
 */
static uint8_t twi_start_r(void) {
	/* Transmit repeated start condition. */
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	
	while(!(TWCR & (1 << TWINT)));
	
	if(STATUS() != 0x10) return handle_error(STATUS());
	
	return 0;
}

/* Transmits slave address and write bit condition.
 * 
 * Input:	address		Slave address
 * Return:	0			Succeeded
 *			>0			Failed, see handle_error() for relavant error codes
 */
static uint8_t twi_sla_w(uint8_t address) {
	/* Transmit slave address and write bit. */
	TWDR = (address << 1);
	TWCR = (1 << TWINT) | (1 << TWEN);
	
	while(!(TWCR & (1 << TWINT)));
	
	if(STATUS() != 0x18) return handle_error(STATUS());
	
	return 0;
}

/* Transmits slave address and read bit condition.
 * 
 * Input:	address		Slave address
 * Return:	0			Succeeded
 *			>0			Failed, see handle_error() for relavant error codes
 */
static uint8_t twi_sla_r(uint8_t address) {
	/* Transmit slave address and read bit. */
	TWDR = (address << 1) | 0x01;
	TWCR = (1 << TWINT) | (1 << TWEN);
	
	while(!(TWCR & (1 << TWINT)));
	
	if(STATUS() != 0x40) return handle_error(STATUS());
	
	return 0;
}

/* Transmits data to slave.
 * 
 * Input:	data		Data to be transfered
 * Return:	0			Succeeded
 *			>0			Failed, see handle_error() for relavant error codes
 */
static uint8_t twi_write(uint8_t data) {
	/* Write data to slave. */
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN);
	
	while(!(TWCR & (1 << TWINT)));
	
	if(STATUS() != 0x28) return handle_error(STATUS());
	
	return 0;
}

/* Receives data from slave. Returns ACK to continue transfer.
 * 
 * Input:	data		Location to store read data
 * Return:	0			Succeeded
 *			>0			Failed, see handle_error() for relavant error codes
 */
static uint8_t twi_read_ack(uint8_t *data) {
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	
	while(!(TWCR & (1 << TWINT)));
	
	if(STATUS() != 0x50) return handle_error(STATUS());
	
	(*data) = TWDR;
	
	return 0;
}

/* Receives data from slave. Returns NACK to stop transfer.
 * 
 * Input:	data		Location to store read data
 * Return:	0			Succeeded
 *			>0			Failed, see handle_error() for relavant error codes
 */
static uint8_t twi_read_nack(uint8_t *data) {
	TWCR = (1 << TWINT) | (1 << TWEN);
	
	while(!(TWCR & (1 << TWINT)));
	
	if(STATUS() != 0x58) return handle_error(STATUS());
	
	(*data) = TWDR;
	
	return 0;
}

/* Transmits stop condition.
 * 
 */
static void twi_stop(void) {
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
	
	while(!(TWCR & (1 << TWSTO)));
}

/* Handles errors and returns relevant error code.
 * 
 *	Input:	status		TWI status register
 *	Return:	1			Bus error due to an illegal START or STOP condition
 *	Return:	2			SLA+W has been transmitted, NOT ACK has been received
 *	Return:	3			Data byte has been transmitted, NOT ACK has been received
 *	Return:	4			Control of TWI line lost
 *	Return:	5			SLA+R has been transmitted, NOT ACK has been received
 *	Return:	10			Unexpected status
 */
static uint8_t handle_error(uint8_t status) {
	switch(status) {
		case 0x00:
			TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
			return 1;
		case 0x20:
			TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
			return 2;
		case 0x30:
			TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
			return 3;
		case 0x38:
			TWCR = (1 << TWINT) | (1 << TWEN);
			return 4;
		case 0x48:
			TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
			return 5;
		default:
			TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
			return 10;
	}
}