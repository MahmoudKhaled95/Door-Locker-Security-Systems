/******************************************************************************
 *
 * [File Name]: control_ecu.c
 *
 * [Description]: Source file for Control-ECU
 *
 * [Created on]: Jul 12, 2022
 *
 * [Micro-controller]: Atmega16
 *
 * [Author]: Mahmoud Khaled
 *
*******************************************************************************/

/*-----------------------------------INCLUDES---------------------------------*/
#include "buzzer.h"
#include "timer.h"
#include "dcmotor.h"
#include "external_eeprom.h"
#include "twi.h"
#include "uart.h"
#include "control_ecu.h"
#include <avr/io.h>
#include <avr/delay.h>


int main(void)
{
	uint8 receivedByte = 0;

	SREG |= (1<<7); /* Enable I-Bit for Interrupts*/

	/* Create configuration structure for UART driver */
	UART_ConfigType UART_Config = {EIGHT_BIT,DISABLED,ONT_BIT,9600};
	UART_init(&UART_Config);		/* Initialize UART driver */

	/* Create configuration structure for Timer driver */
	Timer_ConfigType TIMER_Config = {TIMER1, COMPARE,0,7812,CLK_1024};
	Timer_init(&TIMER_Config);		/* Initialize Timer driver */
	Timer_setCallBack(Timer_CallBackFunction,TIMER1);

	/* Create configuration structure for I2C driver */
	TWI_ConfigType TWI_Config = {0x02,FAST_MODE};
	TWI_init(&TWI_Config);			/* Initialize I2C driver */

	BUZZER_init();					/* Initialize BUZZER driver */

	DcMotor_Init();					/*Initialize the DcMotor */

	CTRL_takeFirstPassword();		/* Receive the password and confirmation password from the
	 	 	 	 	 	 	 	 	   the HMI ECU */

	while(1)
	{
		while(UART_recieveByte() != READY_TO_SEND); /* Receive from HMI to be ready to receive */
		UART_sendByte(READY_TO_RECEIVE); /* Inform HMI to start sending */
		CTRL_receivePasswordByUART(g_receivedPassword); /* Receive the password from HMI ECU */

		receivedByte = UART_recieveByte(); /* Receive the selected option (select to open the door
											  or to change the password) */

		switch(receivedByte)
		{
		case DOOR_OPEN_OPTION:
			/* Checking if the received password and stored password in EEPROM identical or not */
			if(CTRL_verifyPassword(g_storedPassword, g_receivedPassword) == SUCCESS)
			{
				UART_sendByte(READY_TO_RECEIVE); /* Inform HMI ECU to be ready to receive */
				UART_sendByte(OPEN_DOOR);		/* Sending to HMI ECU to open the door */
				CTRL_openingDoor();	/* Start opening the door */
			}
			else
			{
				UART_sendByte(READY_TO_RECEIVE); /* Inform HMI ECU to be ready to receive */
				UART_sendByte(WRONG_PASSWORD);	/* Sending to HMI ECU that the password wrong */
				g_wrongTrial++;		/* Increment the counter for wrong attempts */
				if(g_wrongTrial == MAX_ALLOWED_TRIALS)
				{
					BUZZER_on(); /* Turn on the Buzzer */
					g_seconds = 0;
					while(g_seconds != BUZZER_ACTIVE_PERIOD);
					BUZZER_off();
					g_wrongTrial = 0;		/* reset the number of trials */
				}
			}
			break;

		case CHANGE_PASSWORD_OPTION:
			/* Checking if the received password and stored password in EEPROM identical or not */
			if(CTRL_verifyPassword(g_storedPassword, g_receivedPassword) == SUCCESS)
			{
				UART_sendByte(READY_TO_RECEIVE); /* Inform the HMI ECU to be ready to receive */

				UART_sendByte(CHANGING_PASSWORD); /* Send to HMI that password correct and allow the
				 	 	 	 	 	 	 	 	     the user to change the password */
				CTRL_takeFirstPassword(); /* Receive the new password from the user */
			}
			else
			{
				UART_sendByte(READY_TO_RECEIVE); /* Inform the HMI ECU to be ready to receive */

				UART_sendByte(WRONG_PASSWORD); /* Send to HMI that password incorrect */
			}
			break;
		}
	}

}


/********************************************************************************************
 * 									Function Definitions									*
 ********************************************************************************************/

/********************************************************************************************
 *
 * [Function Name]: CTRL_takeFirstPassword
 *
 * [Description]:- This function is responsible for receiving the passwords form the HMI micro-
 * 				   controller and store it in global array (g_receivedPassword).
 * 				 - Also This function check the passwords received from the HMI micro-controller
 * 				   if both password identical it send Password match to HMI micro-controller to
 * 				   inform that the password saved.
 *
 * [Arguments]: None
 *
 * [in]: void
 *
 * [Returns]: void
 *
 ********************************************************************************************/
void CTRL_takeFirstPassword(void)
{
	uint8 verify;

	uint8 confirmationPassword[PASSWORD_LENGTH]; /* To store second received password */
	while(1)
	{
		while (UART_recieveByte() != READY_TO_SEND);
		UART_sendByte(READY_TO_RECEIVE);		/* Tell the other HMI controller than control micro-controller
													ready to receive the password */
		CTRL_receivePasswordByUART(g_receivedPassword);		/*Receive the first password from HMI micro-controller*/

		while (UART_recieveByte() != READY_TO_SEND);
		UART_sendByte(READY_TO_RECEIVE);		/* Tell the other HMI controller than control micro-controller
															ready to receive the password */
		CTRL_receivePasswordByUART(confirmationPassword);		/*Receive the second password (confirmation password) from HMI ECU*/

		verify = CTRL_verifyPassword(g_receivedPassword,confirmationPassword); /* check if the received passwords are identical */

		if(verify == SUCCESS)
		{
			UART_sendByte(READY_TO_SEND);
			UART_sendByte(PASSWORD_MATCHED); /* Send password matched to HMI ECU */
			CTRL_storePassword(); /* Store the password in EEPROM */
			break; /* Exit from while loop */
		}
		else
		{
			UART_sendByte(READY_TO_SEND);
			UART_sendByte(PASSWORD_UNMATCHED);	/* Send password unmatched to HMI ECU */
		}
	}
}



/********************************************************************************************
 *
 * [Function Name]: CTRL_verifyPassword
 *
 * [Description]: This function is responsible for checking whether the two password identical
 * 				  or not.
 *
 * [Arguments]: uint8 *a_firstPassword_Ptr, uint8 *a_secondPassword_Ptr
 *
 * [in]: - *a_firstPassword_Ptr: Pointer to character
 *		 - a_secondPassword_Ptr: Pointer to character
 *
 * [out]: Unsigned Character
 *
 * [Returns]: If the checked password true it returns SUCCCES other returns FAILED
 *
 ********************************************************************************************/
uint8 CTRL_verifyPassword(uint8 *a_firstPassword_Ptr, uint8 *a_secondPassword_Ptr)
{
	uint8 correctPassword = SUCCESS;
	uint8 i=0; /* Counter for for-loop */

	CTRL_readStoredPassword(); /* To update the password with last stored password in EEPROM */

	for(i = 0; i<PASSWORD_LENGTH; i++)
	{
		if(a_firstPassword_Ptr[i] != a_secondPassword_Ptr[i])
		{
			correctPassword = FAILED;
			break; /* break from the loop when arrays elements not identical */
		}
	}
	return correctPassword;
}



/********************************************************************************************
 *
 * [Function Name]: CTRL_receivePasswordByUART
 *
 * [Description]: This function is responsible for receiving the password form the HMI micro-
 * 				  controller through UART serial communication.
 *
 * [Arguments]: uint8 *password_Ptr
 *
 * [in]: *password_Ptr: pointer to character
 *
 * [out]: void
 *
 * [Returns]: void
 *
 ********************************************************************************************/
void CTRL_receivePasswordByUART(uint8 *password_Ptr)
{
	uint8 counter; /* Counter to used in for loop */
	for(counter = 0; counter<PASSWORD_LENGTH; counter++)
	{
		UART_sendByte(READY_TO_RECEIVE);	/* Tell HMI ECU that control ECU ready to receive data */
		password_Ptr[counter] = UART_recieveByte(); 	/* Receive 1 Byte of the password from the HMI */
	}
}



/********************************************************************************************
 *
 * [Function Name]: CTRL_storePassword
 *
 * [Description]: This function is responsible for storing the password in EEPROM
 *
 * [Arguments]: None
 *
 * [in]: void
 *
 * [out]: void
 *
 * [Returns]: void
 *
 ********************************************************************************************/
void CTRL_storePassword(void)
{
	uint8 counter;
	uint16 address = 0x0311;
	for(counter = 0; counter<PASSWORD_LENGTH; counter++)
	{
		/*
		 * Store 1 byte from g_receivedPassword array in EEPROM
		 * In order to store array in EEPROM the address must be increment
		 * each time.
		 * In order to increment the address, making the address + counter
		 * to increment it each time by one location.
		 */
		EEPROM_writeByte(address + counter, g_receivedPassword[counter]);
		/* delay time as the EEPROM take 10ms to make a write*/
		_delay_ms(100);		/* Give the EEPROM some time to store 1 Byte */
	}
}


/********************************************************************************************
 *
 * [Function Name]: CTRL_readStoredPassword
 *
 * [Description]: This function is responsible for reading the stored password from EEPROM
 *
 * [Arguments]: None
 *
 * [in]: void
 *
 * [out]: void
 *
 * [Returns]: void
 *
 ********************************************************************************************/
void CTRL_readStoredPassword(void)
{
	uint8 counter;
	uint16 address = 0x0311;
	for(counter = 0; counter<PASSWORD_LENGTH; counter++)
	{
		/* Read 1 byte of the stored password from EEPROM and store it in global array
		 * g_storedPassword.
		 * In order to increment the address, making the address + counter
		 * to increment it each time by one location.
		 */
		EEPROM_readByte(address + counter, &g_storedPassword[counter]);
	}
}



/********************************************************************************************
 *
 * [Function Name]: CTRL_openingDoor
 *
 * [Description]: - This function is responsible for opening and closing the Door.
 * 				  - Make the motor that responsible for opening and closing the door,
 * 				    rotates clockwise (in opening the door) and rotates anti-clockwise
 * 				    (in closing the door).
 *
 * [Arguments]: None
 *
 * [in]: void
 *
 * [out]: void
 *
 * [Returns]: void
 *
 ********************************************************************************************/
void CTRL_openingDoor(void)
{
	/* Opening the Door:
	 * Rotate the motor Clockwise for 15 seconds
	 */
	g_seconds = 0;
	DcMotor_Rotate(CW);
	while(g_seconds != DOOR_UNLOCKED_PERIOD);


	/* Hold the Door:
	 * Holding the motor clockwise for 3 seconds
	 */
	g_seconds = 0;
	DcMotor_Rotate(STOP);
	while(g_seconds != DOOR_LEFT_OPEN_PERIOD);

	/* Closing the Door:
	 * Rotate the motor Anti-Clockwise for 15 seconds
	 */
	g_seconds = 0;
	DcMotor_Rotate(ACW);
	while(g_seconds != DOOR_UNLOCKED_PERIOD);

	DcMotor_Rotate(STOP);
}



/********************************************************************************************
 *
 * [Function Name]: Timer_CallBackFunction
 *
 * [Description]:This function is responsible for incrementing global variable (g_seconds)
 * 				 that indicates the number of counted seconds.
 *
 * [Arguments]: None
 *
 * [in]: void
 *
 * [out]: void
 *
 * [Returns]: void
 *
 ********************************************************************************************/
void Timer_CallBackFunction(void)
{
	/* Call back function for the timer (when timer count 1 second)
	 * the timer increment the global variable g_seconds every second */
	g_seconds++; /* Increment global second variable each time the interrupt fired */
}
