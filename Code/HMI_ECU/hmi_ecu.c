/******************************************************************************
 *
 * [File Name]: hmi_ecu.c
 *
 * [Description]: Source file for HMI-ECU
 *
 * [Created on]: Jul 12, 2022
 *
 * [Micro-controller]: Atmega16
 *
 * [Author]: Mahmoud Khaled
 *
*******************************************************************************/

/*-----------------------------------INCLUDES---------------------------------*/
#include "lcd.h"
#include "keypad.h"
#include "uart.h"
#include "timer.h"
#include "hmi_ecu.h"
#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	uint8 receivedByte, key;

	SREG |= (1<<7); /* Enable I-Bit for Interrupts*/

	LCD_init();		/* Initialize LCD driver */

	/* Create configuration structure for UART driver */
	UART_ConfigType UART_Config = {EIGHT_BIT,DISABLED,ONT_BIT,9600};
	UART_init(&UART_Config);		/* Initialize UART driver */

	/* Create configuration structure for Timer driver */
	Timer_ConfigType TIMER_Config = {TIMER1, COMPARE,0,7812,CLK_1024};
	Timer_init(&TIMER_Config);		/* Initialize Timer driver */
	Timer_setCallBack(Timer_CallBackFunction,TIMER1);

	HMI_displayWelcomeScreen();		/* Display welcome screen when starting the system */

	HMI_takeFirstPassword();		/* Take the password and confirmation password from the
	 	 	 	 	 	 	 	 	   the user */

	HMI_mainOptions();				/* Display the main options after taking the password */


	while(1)
	{
		key = KEYPAD_getPressedKey();		/* Get the pressed button from keypad */
		_delay_ms(4000); /* Delay for switch debouncing */
		switch(key)
		{
		/* Case the user wants to open the door */
		case DOOR_OPEN_OPTION:
			/*
			 * 1.Clear the LCD
			 * 2.Display the required Message
			 * 3.Move the cursor to the location where we type the password
			 */
			LCD_clearScreen();
			LCD_displayStringRowColumn(0, 0, "Enter Password:");
			LCD_displayStringRowColumn(1, 0, "= : To submit");
			LCD_moveCursor(3,12);

			HMI_getPassword(g_userPassword); /* Get the password from the user */


			UART_sendByte(READY_TO_SEND);	/* Inform Control ECU to start sending */
			while(UART_recieveByte() != READY_TO_RECEIVE);
			HMI_sendPasswordByUART(g_userPassword);

			UART_sendByte(DOOR_OPEN_OPTION);

			while(UART_recieveByte() != READY_TO_RECEIVE); /* Wait until the Control ECU checking the received password
			 	 	 	 	 	 	 	 	 	 	 	 	  and receive a byte that determine the password correct or not */
			receivedByte = UART_recieveByte(); /* - Receive open door in case Control ECU check the password and its identical
			 	 	 	 	 	 	 	 	 	  - In case the checked password wrong the HMI receive that password is wrong */
			if(receivedByte == OPEN_DOOR)
			{
				HMI_openingDoor();
			}
			else if(receivedByte == WRONG_PASSWORD)
			{
				++g_trialNumber; /* Increment the number of failed trails from the user */
				LCD_clearScreen();
				LCD_displayString("Wrong Password");
				_delay_ms(3000);
				if(g_trialNumber == MAX_ALLOWED_TRIALS)
				{
					/* Display message on LCD when the user enter wrong password three times */
					HMI_countBuzzerRunTime();
					g_trialNumber = 0; /* Reset the counting of wrong trials */
				}
			}
			receivedByte = 0;
			HMI_mainOptions();
			break;

		/* Case the user wants to change his password */
		case CHANGE_PASSWORD_OPTION:
			/*
			 * 1.Clear the LCD
			 * 2.Display the required Message
			 * 3.Move the cursor to the location where we type the password
			 */
			LCD_clearScreen();
			LCD_displayStringRowColumn(0, 0, "Enter old Password:");
			LCD_displayStringRowColumn(1, 0, "= : To submit");
			LCD_moveCursor(3,12);

			HMI_getPassword(g_userPassword); /* Get the old password from the user */


			UART_sendByte(READY_TO_SEND);	/* Inform Control ECU to start sending */
			while(UART_recieveByte() != READY_TO_RECEIVE); /* Inform HMI that Control ready to receive */
			HMI_sendPasswordByUART(g_userPassword);	/* Send the old password to Control ECU to check it */

			UART_sendByte(CHANGE_PASSWORD_OPTION); /* Send the option that the user wants to do (Change password option)*/

			while(UART_recieveByte() != READY_TO_RECEIVE); /* Wait until the Control ECU inform HMI ECU that the entered
			 	 	 	 	 	 	 	 	 	 	 	 	  password correct or not
			 	 	 	 	 	 	 	 	 	 	 	 	  if the entered password correct: HMI receives Change password byte
			 	 	 	 	 	 	 	 	 	 	 	 	  else the HMI receive that password wrong */

			receivedByte = UART_recieveByte();

			if(receivedByte == CHANGING_PASSWORD)
			{
				HMI_takeFirstPassword();
			}
			else if( receivedByte == WRONG_PASSWORD )
			{
				LCD_clearScreen();
				LCD_displayStringRowColumn(0,0,"Wrong Password");
				LCD_displayStringRowColumn(1,0,"Try again!!");
				_delay_ms(3000);		/*Delay to display the message for 3 seconds */
			}
			receivedByte = 0;
			HMI_mainOptions();
			break;
		}
	}
}


/********************************************************************************************
 * 									Function Definitions									*
 ********************************************************************************************/

/********************************************************************************************
 *
 * [Function Name]: HMI_displayWelcomeScreen
 *
 * [Description]:This function is responsible for displaying the welcome message on LCD for the
 * 				 person who use this system for the first time.
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
void HMI_displayWelcomeScreen(void)
{
	/*
	 * 1. Clear the LCD
	 * 2. Display Message "Welcome to Security Door Lock System"
	 */
	LCD_clearScreen();
	LCD_displayString("Welcome to Security Door Lock System");
	_delay_ms(3000);	/* Delay to display the message for 3 seconds */
}


/********************************************************************************************
 *
 * [Function Name]: HMI_takeFirstPassword
 *
 * [Description]:This function is responsible for taking the password form the user and
 * 				 store it in global array (g_userPassword) in order to send the password
 * 				 to the other micro-controller.
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
void HMI_takeFirstPassword(void)
{
	while(g_passwordStatus == PASSWORD_UNMATCHED)
	{
		/*------------------ Get First Password from the user ----------------------------*/
		/*
		 * 1. Clear the LCD
		 * 2. Display Message "Enter New Password:"
		 * 3. Display Message "= : To submit"
		 * 4. Move the cursor to the location where the user can type the password
		 */
		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 0, "Enter New Password:");
		LCD_displayStringRowColumn(1, 0, "= : To submit");
		LCD_moveCursor(3,12);

		/* Function to get the First password entered and store it in Array */
		HMI_getPassword(g_userPassword);

		UART_sendByte(READY_TO_SEND);	/* Inform Control ECU to be ready to receive the first password */
		while (UART_recieveByte() != READY_TO_RECEIVE);	/* Wait until the Control ECU responds that it is ready to receive
														   first password */

		/* Call function that responsible for sending password to the Control ECU */
		HMI_sendPasswordByUART(g_userPassword); /* Send First Password to Control ECU */
		_delay_ms(1000);

		/*-------------- Get Second (confirmation) Password from the user ------------------*/
		/*
		 * 1. Clear the LCD
		 * 2. Display Message "Re-enter Password:"
		 * 3. Display Message "= : To submit"
		 * 3. Move the cursor to the location where the user can type the password
		 */
		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 0, "Re-enter Password:");
		LCD_displayStringRowColumn(1, 0, "= : To submit");
		LCD_moveCursor(3,12);

		/* Function to get the Second password (confirmation password) entered and store it in Array */
		HMI_getPassword(g_userPassword);

		UART_sendByte(READY_TO_SEND);	/* Inform Control ECU to be ready to receive the second password */
		while(UART_recieveByte() != READY_TO_RECEIVE);	/* Wait until the Control ECU responds that it is ready to receive
														   second password */

		/* Call function that responsible for sending password to the Control ECU */
		HMI_sendPasswordByUART(g_userPassword);	/* Send Second Password to Control ECU */
		_delay_ms(1000);

		/*
		 * Wait until the control ECU make response about the
		 * password and confirmation password if they are matching
		 * or not
		 */
		while(UART_recieveByte() != READY_TO_SEND); 	/* Wait until Control ECU send the response of checking the password */
		g_passwordStatus = UART_recieveByte(); /* Get the response from Control ECU that the password matched or not */

		/* After Control ECU check the two passwords, If two passwords are identical display message "Password Saved" */
		if(g_passwordStatus == PASSWORD_MATCHED)
		{
			LCD_clearScreen();
			LCD_displayStringRowColumn(0,0,"Password Saved");
			_delay_ms(3000);		/*Delay to display the message for 3 seconds */
			g_passwordStatus = PASSWORD_UNMATCHED;	/* Return the variable to its initial state in order to
													   be able use this function again */
			break;
		}
		/* If two passwords are not identical display message "Password Unmatched" and "Try again!!" */
		else if(g_passwordStatus == PASSWORD_UNMATCHED)
		{
			LCD_clearScreen();
			LCD_displayStringRowColumn(0,0,"Password Unmatched");
			LCD_displayStringRowColumn(1,0,"Try again!!");
			HMI_clearArray(g_userPassword); /* Clear the received password from the user to be able to
			 	 	 	 	 	 	 	 	   receive new one
			 	 	 	 	 	 	 	 	 */
			_delay_ms(3000);		/*Delay to display the message for 3 seconds */
		}
	}
}

/********************************************************************************************
 *
 * [Function Name]: HMI_getPassword
 *
 * [Description]:This function is responsible for displaying the main options in LCD.
 *
 * [Arguments]:
 *
 * [in]: *a_password_Ptr: pointer to unsigned character
 *
 * [out]: void
 *
 * [Returns]: void
 *
 ********************************************************************************************/
void HMI_getPassword(uint8 *a_password_Ptr)
{
	uint8 i = 0; 	/* variable used as counter for for-Loop */
	uint8 key;		/* variable used to get the pressed button */

	for(i=0;i<PASSWORD_LENGTH;i++)
	{
		key = KEYPAD_getPressedKey();	/* Get the pressed button from keypad */

		_delay_ms(4000); /* Delay for switch debouncing */

		if((key >= 0) && (key <= 9))
		{
			LCD_displayCharacter ('*');		/* Display '*' instead of the pressed key for security */
			a_password_Ptr[i] = key; 		/* Store the pressed button in the array */
		}
		else if (key == '=')
		{
			break; /* Exit from for-loop in case the user finishes entering the password */
		}
	}
}

/********************************************************************************************
 *
 * [Function Name]: HMI_mainOptions
 *
 * [Description]:This function is responsible for displaying the main options on LCD.
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
void HMI_mainOptions(void)
{
	/*
	 * 1. Clear the LCD
	 * 2. Display main options of the system
	 *    -> "+: Open door"
	 *    -> "-: Change password"
	 */
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "+: Open door");
	LCD_displayStringRowColumn(1, 0, "-: Change password");
}



/********************************************************************************************
 * [Function Name]: HMI_sendPasswordByUART
 *
 * [Description]:This function is responsible for sending password to other micro-controller
 * 				 through UART serial communication.
 *
 * [Arguments]:
 *
 * [in]: *password_Ptr: pointer to unsigned character
 *
 * [out]: void
 *
 * [Returns]: void
 *
 ********************************************************************************************/
void HMI_sendPasswordByUART(uint8 *password_Ptr)
{
	uint8 counter; /* Variable to be used as a counter for for-Loop */

	for(counter = 0; counter<PASSWORD_LENGTH; counter++)
	{
		while(UART_recieveByte() != READY_TO_RECEIVE);	/* Wait until Control ECU be ready to receive data */

		UART_sendByte(password_Ptr[counter]);	/* Send 1 byte from the password to Control ECU */
	}
}



/********************************************************************************************
 *
 * [Function Name]: HMI_openingDoor
 *
 * [Description]:This function is responsible for displaying the Door state on LCD.
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
void HMI_openingDoor(void)
{
	/* Display message on screen while rotating the motor clockwise for 15 seconds */
	g_seconds = 0;
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "Opening The Door...");
	while(g_seconds != DOOR_UNLOCKED_PERIOD);

	/* Display message on screen while holding the motor for 3 seconds */
	g_seconds = 0;
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "Holding The Door");
	while(g_seconds != DOOR_LEFT_OPEN_PERIOD);

	/* Display message on screen while rotating the motor anti-clockwise for 15 seconds */
	g_seconds = 0;

	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "Closing The Door...");
	while(g_seconds != DOOR_UNLOCKED_PERIOD);

	_delay_ms(1000);
}



/********************************************************************************************
 *
 * [Function Name]: HMI_countBuzzerRunTime
 *
 * [Description]:This function is responsible for displaying a warning message on the LCD
 * 				 when a thief tries to type wrong password for many times.
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
void HMI_countBuzzerRunTime(void)
{
	/* Display message on screen while rotating the motor clockwise for 15 seconds */
	g_seconds = 0;
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"System Closed");
	LCD_displayStringRowColumn(1,0,"Catch The Thief!!");
	while(g_seconds != BUZZER_ACTIVE_PERIOD);
}



/********************************************************************************************
 *
 * [Function Name]: HMI_clearArray
 *
 * [Description]:This function is responsible for clearing all the elements of the array.
 *
 * [Arguments]: uint8 *a_Ptr
 *
 * [in]: a_Ptr: pointer to unsigned character
 *
 * [out]: void
 *
 * [Returns]: void
 *
 ********************************************************************************************/
void HMI_clearArray(uint8 *a_Ptr)
{
	uint8 counter = 0;
	while(counter != PASSWORD_LENGTH)
	{
		a_Ptr[counter] = 0;
		counter++;
	}
	a_Ptr[counter] = 0;
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
