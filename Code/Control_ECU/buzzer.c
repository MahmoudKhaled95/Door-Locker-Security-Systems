/******************************************************************************
 *
 * [Module]: Buzzer
 *
 * [File Name]: buzzer.c
 *
 * [Description]: Source file for Buzzer driver
 *
 * [Author]: Mahmoud Khaled
 *
*******************************************************************************/

#include "buzzer.h"
#include "gpio.h"



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
void BUZZER_init(void)
{
	GPIO_setupPinDirection(BUZZER_PORT_ID, BUZZER_PIN_ID, PIN_OUTPUT);
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_LOW);
}


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
void BUZZER_on(void)
{
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_HIGH);
}



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
void BUZZER_off(void)
{
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_LOW);
}
