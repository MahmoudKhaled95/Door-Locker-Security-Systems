/******************************************************************************
 *
 * [Module]: Buzzer
 *
 * [File Name]: buzzer.h
 *
 * [Description]: header file for Buzzer driver
 *
 * [Author]: Mahmoud Khaled
 *
*******************************************************************************/


#ifndef BUZZER_H_
#define BUZZER_H_


#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define BUZZER_PORT_ID				PORTA_ID
#define BUZZER_PIN_ID				PIN0_ID


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/********************************************************************************************
 * [Function Name]: BUZZER_init
 *
 * [Description]:The function is used for setup the direction of the buzzer
 * 				 Stop the buzzer at the beginning.
 *
 * [Arguments]: None
 *
 * [in]: void
 *
 * [Returns]: void
 *
 ********************************************************************************************/
void BUZZER_init(void);


/********************************************************************************************
 * [Function Name]: BUZZER_on
 *
 * [Description]:The function used to turn on the buzzer by writing one on its pin.
 *
 * [Arguments]: None
 *
 * [in]: void
 *
 * [Returns]: void
 *
 ********************************************************************************************/
void BUZZER_on(void);


/********************************************************************************************
 * [Function Name]: BUZZER_off
 *
 * [Description]:The function used to turn off the buzzer by writing zero on its pin.
 *
 * [Arguments]: None
 *
 * [in]: void
 *
 * [Returns]: void
 *
 ********************************************************************************************/
void BUZZER_off(void);


#endif /* BUZZER_H_ */
