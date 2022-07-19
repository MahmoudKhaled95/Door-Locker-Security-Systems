/******************************************************************************
 *
 * [File Name]: control_ecu.h
 *
 * [Description]: header file for Control-ECU
 *
 * [Created on]: Jul 12, 2022
 *
 * [Micro-controller]: Atmega16
 *
 * [Author]: Mahmoud Khaled
 *
*******************************************************************************/

#ifndef CONTROL_ECU_H_
#define CONTROL_ECU_H_

#ifndef F_CPU
#define F_CPU 8000000UL /*8MHz Clock frequency*/
#endif

/********************************************************************************************
 * 									Preprocessor Macros										*
 *******************************************************************************************/
#define PASSWORD_LENGTH				5

#define	DOOR_UNLOCKED_PERIOD		15
#define DOOR_LEFT_OPEN_PERIOD		5
#define BUZZER_ACTIVE_PERIOD		3

#define MAX_ALLOWED_TRIALS			3

#define SUCCESS						1
#define FAILED						0

#define PASSWORD_MATCHED			1
#define PASSWORD_UNMATCHED			0

#define READY_TO_SEND				0x10
#define READY_TO_RECEIVE			0x20

#define OPEN_DOOR					0x25

#define WRONG_PASSWORD				0x30
#define CHANGING_PASSWORD			0X31
#define THIEF_IS_DETECTED			0x32

#define CHANGE_PASSWORD_OPTION		45 		/* ACII Code for '+' */
#define DOOR_OPEN_OPTION			43		/* ACII Code for '-' */


/********************************************************************************************
 * 									Global Variables										*
 ********************************************************************************************/
/* Global array to receive the password from HMI ECU */
uint8 g_receivedPassword[PASSWORD_LENGTH];

/* Global array to get the stored password from EEPROM */
uint8 g_storedPassword[PASSWORD_LENGTH];

/* Global variable to store the number of wrong attempts */
uint8 g_wrongTrial=0;

/* Global variable to be incremented every second */
uint8 g_seconds = 0;

/********************************************************************************************
 * 									Function Prototype										*
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
 * [out]: void
 *
 * [Returns]: void
 *
 ********************************************************************************************/
void CTRL_takeFirstPassword(void);



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
uint8 CTRL_verifyPassword(uint8 *a_firstPassword_Ptr, uint8 *a_secondPassword_Ptr);



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
void CTRL_receivePasswordByUART(uint8 *password_Ptr);



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
void CTRL_storePassword(void);



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
void CTRL_readStoredPassword(void);



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
void CTRL_openingDoor(void);



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
void Timer_CallBackFunction(void);

#endif /* CONTROL_ECU_H_ */
