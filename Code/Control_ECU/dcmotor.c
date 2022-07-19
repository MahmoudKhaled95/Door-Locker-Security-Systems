/******************************************************************************
 *
 * [Module]: DCMOTOR
 *
 * [File Name]: dcmotor.c
 *
 * [Description]: Source file for the DC Motor driver
 *
 * [Author]: Mahmoud Khaled
 *
*******************************************************************************/



#include "gpio.h"
#include "dcmotor.h"



/********************************************************************************************
 *
 * [Function Name]: DcMotor_Init
 *
 * [Description]:- The Function responsible for setup the direction for the two motor pins
 * 				   through the GPIO driver.
 * 				 - Stop at the DC-Motor at the beginning through the GPIO driver.
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
void DcMotor_Init(void)
{
	/* configure pin of motors as output pins */
	GPIO_setupPinDirection(MOTOR_PORT_ID,MOTOR_FIRST_PIN_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(MOTOR_PORT_ID,MOTOR_SECOND_PIN_ID,PIN_OUTPUT);
	/* Motor is stop at the beginning */
	GPIO_writePin(MOTOR_PORT_ID, MOTOR_FIRST_PIN_ID,LOGIC_LOW);
	GPIO_writePin(MOTOR_PORT_ID, MOTOR_SECOND_PIN_ID,LOGIC_LOW);
}




/********************************************************************************************
 *
 * [Function Name]: DcMotor_Rotate
 *
 * [Description]: The function responsible for rotate the DC Motor CW/ or A-CW or stop the
 * 				  motor based on the state input state value.
 *
 * [Arguments]: DcMotor_State state
 *
 * [in]: Enum to Dc-Motor State
 *
 * [out]: void
 *
 * [Returns]: void
 *
 ********************************************************************************************/
void DcMotor_Rotate(DcMotor_State state)
{
	switch (state)
	{
	default:
		GPIO_writePin(MOTOR_PORT_ID, MOTOR_FIRST_PIN_ID,LOGIC_LOW);
		GPIO_writePin(MOTOR_PORT_ID, MOTOR_SECOND_PIN_ID,LOGIC_LOW);
		break;
	case STOP:
		GPIO_writePin(MOTOR_PORT_ID, MOTOR_FIRST_PIN_ID,LOGIC_LOW);
		GPIO_writePin(MOTOR_PORT_ID, MOTOR_SECOND_PIN_ID,LOGIC_LOW);
		break;
	case CW:
		GPIO_writePin(MOTOR_PORT_ID, MOTOR_FIRST_PIN_ID,LOGIC_HIGH);
		GPIO_writePin(MOTOR_PORT_ID, MOTOR_SECOND_PIN_ID,LOGIC_LOW);
		break;
	case ACW:
		GPIO_writePin(MOTOR_PORT_ID, MOTOR_FIRST_PIN_ID,LOGIC_LOW);
		GPIO_writePin(MOTOR_PORT_ID, MOTOR_SECOND_PIN_ID,LOGIC_HIGH);
		break;
	}
}
