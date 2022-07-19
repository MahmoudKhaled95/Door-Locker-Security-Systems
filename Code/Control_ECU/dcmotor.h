/******************************************************************************
 *
 * [Module]: DCMOTOR
 *
 * [File Name]: dcmotor.h
 *
 * [Description]: Header file for DC Motor driver
 *
 * [Author]: Mahmoud Khaled
 *
*******************************************************************************/
#ifndef DCMOTOR_H_
#define DCMOTOR_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define MOTOR_PORT_ID				PORTB_ID
#define MOTOR_FIRST_PIN_ID			PIN0_ID
#define MOTOR_SECOND_PIN_ID			PIN1_ID


/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum{
	STOP, CW, ACW
}DcMotor_State;


/********************************************************************************************
 *                       		  Functions Prototypes                        				*
 ********************************************************************************************/

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
void DcMotor_Init(void);



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
void DcMotor_Rotate(DcMotor_State state);



#endif /* DCMOTOR_H_ */
