 /******************************************************************************
 *
 * [Module]: UART
 *
 * [File Name]: uart.h
 *
 * [Description]: Header file for the UART AVR driver
 *
 * [Author]: Mahmoud Khaled
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "std_types.h"


/*******************************************************************************
 *                      Type Declaration                                   *
 *******************************************************************************/

typedef enum{
	FIVE_BIT, SIX_BIT, SEVEN_BIT, EIGHT_BIT
}UART_DataSize;

typedef enum{
	DISABLED, EVEN = 2, ODD = 3
}UART_ParityMode;

typedef enum{
	ONT_BIT, TWO_BIT
}UART_StopBitNumber;


typedef struct{
	UART_DataSize data_size;
	UART_ParityMode parity_mode;
	UART_StopBitNumber stop_bit;
	uint32 baud_rate;
}UART_ConfigType;



/************************************************************************
 *                           Global variables                           *
 ************************************************************************/
extern volatile uint8 g_receivedValue;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/********************************************************************************************
 *
 * [Function Name]: UART_init
 *
 * [Description]: Functional responsible for Initialize the UART device by:
 * 					1. Setup the Frame format like number of data bits, parity bit type
 * 					   and number of stop bits.
 * 					2. Enable the UART.
 * 					3. Setup the UART baud rate.
 *
 * [Arguments]: const UART_ConfigType * Config_Ptr
 *
 * [in]: *Config_Ptr: Pointer to UART Configuration structure
 *
 * [out]: void
 *
 * [Returns]: void
 *
 ********************************************************************************************/
void UART_init(const UART_ConfigType * Config_Ptr);




/********************************************************************************************
 *
 * [Function Name]: UART_sendByte
 *
 * [Description]: Functional responsible for send byte to another UART device.
 *
 * [Arguments]: const uint8 data
 *
 * [in]: data: unsigned character
 *
 * [out]: void
 *
 * [Returns]: void
 *
 ********************************************************************************************/
void UART_sendByte(const uint8 data);




/********************************************************************************************
 *
 * [Function Name]: UART_recieveByte
 *
 * [Description]: Functional responsible for receive byte from another UART device.
 *
 * [Arguments]: None
 *
 * [in]: void
 *
 * [out]: unsigned character
 *
 * [Returns]: Return the received byte
 *
 ********************************************************************************************/
uint8 UART_recieveByte(void);




/****************************************************************************************
 *
 * [Function Name]: UART_sendString
 *
 * [Description]: Send the required string through UART to the other UART device.
 *
 * [Arguments]: const uint8 *Str
 *
 * [in]: *Str: Pointer to Unsigned Character
 *
 * [out]: None
 *
 * [Returns]: None
 *
 ****************************************************************************************/
void UART_sendString(const uint8 *Str);




/********************************************************************************************
 *
 * [Function Name]: UART_receiveString
 *
 * [Description]: Receive the required string until the '#' symbol through UART from the
 * 				  other UART device.
 *
 * [Arguments]: uint8 *Str
 *
 * [in]: *Str: Pointer to Unsigned Character
 *
 * [out]: void
 *
 * [Returns]: void
 *
 ********************************************************************************************/
void UART_receiveString(uint8 *Str); // Receive until #

#endif /* UART_H_ */
