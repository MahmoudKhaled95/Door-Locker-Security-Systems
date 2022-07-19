/******************************************************************************
 *
 * [FILE NAME]: timer.h
 *
 * [MODULE]: Timer
 *
 * [DATE CREATED]: July,2022
 *
 * [Description]: Header file for the Timer AVR driver
 *
 * [AUTHOR]: Mahmoud Khaled
 *
 *******************************************************************************/


#ifndef TIMER_H_
#define TIMER_H_

#include "std_types.h"

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum{
	TIMER0, TIMER1, TIMER2
}TIMER_ID;

typedef enum{
	NORMAL, COMPARE
}Timer_Mode;

typedef enum{
	NO_CLK, CLK_1, CLK_8, CLK_64, CLK_256, CLK_1024
}Timer_Prescaler;

typedef struct{
	TIMER_ID Timer_ID;
	Timer_Mode timer_mode;
	uint8 intialValue;
	uint16 compareValue;
	Timer_Prescaler timer_Prescaler;
}Timer_ConfigType;



/*******************************************************************************
 *                         Functions Prototypes                                *
 *******************************************************************************/

/********************************************************************************************
 * [Function Name]: Timer_init
 *
 * [Description]: This Function to initialize the Timer driver:
 * 	 					1. Initialize Timer Registers
 * 	 					2. Selecting the required Mode ( Normal, Compare(CTC) )
 * 	 					3. Set the required clock.
 * 	 					4. Enable the Timer Interrupt.
 * 	 					5. Insert the initial value
 * 	 					6. Insert the compare value (in case CTC mode)
 *
 * [Arguments]:
 *
 * [in]: *Config_Ptr: Pointer to Timer Configuration structure
 *
 * [out]: void
 *
 * [Returns]: void
 *
 ********************************************************************************************/
void Timer_init(const Timer_ConfigType *Config_Ptr);



/********************************************************************************************
 * [Function Name]: Timer_setCallBack
 *
 * [Description]: Function to set the Call Back function address.
 *
 * [Arguments]:
 *
 * [in]: *a_ptr: Pointer to Function
 * 		  a_timerID: Enum to Timer ID
 *
 * [out]: void
 *
 * [Returns]: void
 *
 ********************************************************************************************/
void Timer_setCallBack(void(*a_ptr)(void), TIMER_ID a_timerID);




/********************************************************************************************
 * [Function Name]: Timer_stop
 *
 * [Description]: This Function responsible for stopping the Timer from counting.
 *
 * [Arguments]:
 *
 * [in]: a_timerID: Enum to Timer ID
 *
 * [out]: void
 *
 * [Returns]: void
 *
 ********************************************************************************************/
void Timer_stop(const TIMER_ID a_timerID);




/********************************************************************************************
 * [Function Name]: Timer_DeInit
 *
 * [Description]: This Function responsible for disabling the Timer.
 *
 * [Arguments]:
 *
 * [in]: a_timerID: Enum to Timer ID
 *
 * [out]: void
 *
 * [Returns]: void
 *
 ********************************************************************************************/
void Timer_DeInit(const TIMER_ID a_timerID);


#endif /* TIMER_H_ */
