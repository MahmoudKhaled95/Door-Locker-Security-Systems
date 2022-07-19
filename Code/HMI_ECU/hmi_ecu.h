/******************************************************************************
 *
 * [File Name]: hmi_ecu.h
 *
 * [Description]: header file for HMI-ECU
 *
 * [Created on]: Jul 12, 2022
 *
 * [Micro-controller]: Atmega16
 *
 * [Author]: Mahmoud Khaled
 *
*******************************************************************************/

#ifndef HMI_ECU_H_
#define HMI_ECU_H_

#ifndef F_CPU
#define F_CPU 8000000UL /*8MHz Clock frequency*/
#endif

/********************************************************************************************
 * 									Preprocessor Macros										*
 *******************************************************************************************/
#define PASSWORD_LENGTH				5

#define	DOOR_UNLOCKED_PERIOD		15		/* 15 seconds */
#define DOOR_LEFT_OPEN_PERIOD		5		/* 3 seconds */
#define BUZZER_ACTIVE_PERIOD		3		/* 3 seconds */

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
/* Global array to store the password */
uint8 g_userPassword[PASSWORD_LENGTH];

/* Global variable for password status */
uint8 g_passwordStatus = PASSWORD_UNMATCHED;

/* Global variable to be incremented every second */
uint8 g_seconds = 0;

/* Global variable to store the number of wrong attempts */
uint8 g_trialNumber = 0;

/********************************************************************************************
 * 									Function Prototype										*
 ********************************************************************************************/

/********************************************************************************************
 * [Function Name]: HMI_displayWelcomeScreen
 *
 * [Description]:This function is responsible for displaying the welcome message in LCD for
 * 				 the person who use this system for the first time.
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
void HMI_displayWelcomeScreen(void);



/********************************************************************************************
 * [Function Name]: HMI_takeFirstPassword
 *
 * [Description]:This function is responsible for taking the password for the user and
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
void HMI_takeFirstPassword(void);



/********************************************************************************************
 * [Function Name]: HMI_getPassword
 *
 * [Description]:This function is responsible for displaying the main options in LCD.
 *
 * [Arguments]: uint8 *a_password_Ptr
 *
 * [in]: *a_password_Ptr: pointer to unsigned character
 *
 * [out]: void
 *
 * [Returns]: void
 *
 ********************************************************************************************/
void HMI_getPassword(uint8 *a_password_Ptr);



/********************************************************************************************
 * [Function Name]: HMI_mainOptions
 *
 * [Description]:This function is responsible for displaying the main options in LCD.
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
void HMI_mainOptions(void);



/********************************************************************************************
 * [Function Name]: HMI_sendPasswordByUART
 *
 * [Description]:This function is responsible for sending password to other micro-controller.
 *
 * [Arguments]: uint8 *password_Ptr
 *
 * [in]: *password_Ptr: pointer to unsigned character
 *
 * [out]: void
 *
 * [Returns]: void
 *
 ********************************************************************************************/
void HMI_sendPasswordByUART(uint8 *password_Ptr);



/********************************************************************************************
 * [Function Name]: HMI_openingDoor
 *
 * [Description]:This function is responsible for displaying the Door state in LCD.
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
void HMI_openingDoor(void);



/********************************************************************************************
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
void HMI_countBuzzerRunTime(void);



/********************************************************************************************
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
void HMI_clearArray(uint8 *a_Ptr);



/********************************************************************************************
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

#endif /* HMI_ECU_H_ */
