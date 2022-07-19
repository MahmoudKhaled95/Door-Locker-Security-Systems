 /******************************************************************************
 *
 * [Module]: TWI(I2C)
 *
 * [File Name]: twi.h
 *
 * [Description]: Source file for the TWI(I2C) AVR driver
 *
 * [Author]: Mahmoud Khaled
 *
 *******************************************************************************/
 
#include "twi.h"

#include "common_macros.h"
#include <avr/io.h>


/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description:
 * - The function is responsible for Initializing the I2C Driver
 * - Adjusting the value of the TWBR register
 * - Select the required prescaler for the I2C
 * - Making The address of the MCU in case Master wants to communicate with it.
 * - Enable TWI
 */
void TWI_init(const TWI_ConfigType *Config_Ptr)
{
	/* Adjusting the value of the TWBR register */
    TWBR = Config_Ptr->bit_rate;

    TWSR = 0x00;
	
	/* Two Wire Bus address my address if any master device want to call (The microcontroller's Address is inserted in TWAR register) */
    TWAR = ((Config_Ptr->slaveAddress)<<1);
	
    /* enable TWI */
    TWCR = (1<<TWEN);
}


/*
 * Description:
 * - The function is responsible for Sending the start bit
 */
void TWI_start(void)
{
    /* 
	 * Clear the TWINT flag before sending the start bit TWINT=1
	 * send the start bit by TWSTA=1
	 * Enable TWI Module TWEN=1 
	 */
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    
    /* Wait for TWINT flag set in TWCR Register (start bit is send successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
}


/*
 * Description:
 * - The function is responsible for Sending the stop bit
 */
void TWI_stop(void)
{
    /* 
	 * Clear the TWINT flag before sending the stop bit TWINT=1
	 * send the stop bit by TWSTO=1
	 * Enable TWI Module TWEN=1 
	 */
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}


void TWI_writeByte(uint8 data)
{
    /* Put data On TWI data Register */
    TWDR = data;
    /* 
	 * Clear the TWINT flag before sending the data TWINT=1
	 * Enable TWI Module TWEN=1 
	 */ 
    TWCR = (1 << TWINT) | (1 << TWEN);
    /* Wait for TWINT flag set in TWCR Register(data is send successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
}



uint8 TWI_readByteWithACK(void)
{
	/* 
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable sending ACK after reading or receiving data TWEA=1
	 * Enable TWI Module TWEN=1 
	 */ 
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    /* Wait for TWINT flag set in TWCR Register (data received successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
    /* Read Data */
    return TWDR;
}


uint8 TWI_readByteWithNACK(void)
{
	/* 
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable TWI Module TWEN=1 
	 */
    TWCR = (1 << TWINT) | (1 << TWEN);
    /* Wait for TWINT flag set in TWCR Register (data received successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
    /* Read Data */
    return TWDR;
}

uint8 TWI_getStatus(void)
{
    uint8 status;
    /* masking to eliminate first 3 bits and get the last 5 bits (status bits) */
    status = TWSR & 0xF8;
    return status;
}
