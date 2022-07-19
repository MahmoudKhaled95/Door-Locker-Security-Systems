/******************************************************************************
 *
 * [FILE NAME]: timer.c
 *
 * [MODULE]: Timer
 *
 * [DATE CREATED]: July,2022
 *
 * [Description]: Source file for the AVR Timer driver
 *
 * [AUTHOR]: Mahmoud Khaled
 *
 *******************************************************************************/


#include "timer.h"
#include "common_macros.h"	/* To use the macros like SET_BIT */
#include <avr/interrupt.h> /* For ICU ISR */
#include <avr/io.h> /* To use Timer0, Timer1 and Timer2 Registers */

/****************************************************************************************
 *                           		Global Variables                                    *
 ****************************************************************************************/
/* Global variables to hold the address of the call back function in the application
 * for Timer0
 */
static volatile void (*g_callBackPtrTimer0)(void) = NULL_PTR;

/* Global variables to hold the address of the call back function in the application
 * for Timer1
 */
static volatile void (*g_callBackPtrTimer1)(void) = NULL_PTR;

/* Global variables to hold the address of the call back function in the application
 * for Timer2
 */
static volatile void (*g_callBackPtrTimer2)(void) = NULL_PTR;



/****************************************************************************************
 *                       		Interrupt Service Routines                              *
 ****************************************************************************************/

/*------------------------------------ Timer 0 ISR -------------------------------------*/
ISR(TIMER0_OVF_vect)
{
	if(g_callBackPtrTimer0 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the overflow has been done */
		(*g_callBackPtrTimer0)();
	}
}

ISR(TIMER0_COMP_vect)
{
	if(g_callBackPtrTimer0 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the compare match has been occurred */
		(*g_callBackPtrTimer0)();
	}
}


/*------------------------------------ Timer 1 ISR -------------------------------------*/

ISR(TIMER1_OVF_vect)
{
	if(g_callBackPtrTimer1 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the overflow has been done */
		(g_callBackPtrTimer1)();
	}
}

ISR(TIMER1_COMPA_vect)
{
	if(g_callBackPtrTimer1 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the compare match has been occurred */
		(g_callBackPtrTimer1)();
	}
}


/*------------------------------------ Timer 2 ISR -------------------------------------*/

ISR(TIMER2_OVF_vect)
{
	if(g_callBackPtrTimer2 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the overflow has been done */
		(g_callBackPtrTimer2)();
	}
}

ISR(TIMER2_COMP_vect)
{
	if(g_callBackPtrTimer2 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the compare match has been occurred */
		(g_callBackPtrTimer2)();
	}
}



/****************************************************************************************
 *                       		Functions Definitions                              		*
 ****************************************************************************************/

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
void Timer_init(const Timer_ConfigType *Config_Ptr)
{
	switch(Config_Ptr -> Timer_ID)
	{
	/*---------------------------------------Timer 0 --------------------------------------*/
	case TIMER0:
		/* Timer 0 Normal mode */
		if(Config_Ptr->timer_mode == NORMAL)
		{
			/*
			 * Configure the Timer 0 control registers (Normal Mode)
			 * => TCCR0 Register:
			 * 1. Non PWM mode FOC0=1.
			 * 2. Normal Mode WGM01=0 & WGM00=0.
			 * 3. Normal Mode COM00=0 & COM01=0.
			 * 4. Insert the required clock in (CS02, CS01, CS00) bits.
			 * => TCNT0 register:
			 * 5. Insert the initial value that you want the timer to start from it.
			 * => TIMSK Register:
			 * 6. Enable Overflow interrupt (TOIE0)
			 */
			TCCR0 = (1<<FOC0);
			TCCR0 = (TCCR0 & 0xF8) | (Config_Ptr->timer_Prescaler);
			TCNT0 = (Config_Ptr->intialValue) & 0xFF;	/* In order to ensure that the register not exceeding his maximum value (255) */
			TIMSK |= (1<<TOIE0);
		}	/* End of Timer 0 Normal Mode */

		else if(Config_Ptr->timer_mode == COMPARE)
		{
			/* Configure Timer 0 control register (Compare Mode)
			 * => TCCR0 Register
			 * 1. Non PWM mode FOC0=1
			 * 2. CTC Mode WGM01=1 & WGM00=0
			 * 3. Insert the required clock in (CS02, CS01, CS00) bits.
			 * => TCNT0 register:
			 * 4. Insert the initial value that you want the timer to start from it.
			 * => TIMSK Register:
			 * 5. Enable Compare match interrupt (OCIE0) in TIMSK register
			 */
			TCCR0 = (1<<FOC0) | (1<<WGM01);
			TCCR0 = (TCCR0 & 0xF8) | (Config_Ptr->timer_Prescaler);
			TCNT0 = (Config_Ptr->intialValue) & 0xFF;	/* In order to ensure that the register not exceeding his maximum value (255) */
			OCR0 = (Config_Ptr->compareValue) & 0xFF;
			TIMSK |= (1<<OCIE0);
		}	/* End of Timer 0 Compare Mode */
		break;			/* End of Timer 0 */

		/*---------------------------------------Timer 1 --------------------------------------*/
	case TIMER1:
		/* Timer 1 Modes */
		if(Config_Ptr->timer_mode == NORMAL)
		{
			/*
			 * Configure the Timer 1 control registers (Normal Mode)
			 * => TCCR1A Register:
			 * 1. COM1A1=0, COM1A0=0, COM1B1=0, COM1B0=0 for Normal Mode
			 * 2. Non PWM mode FOC1A = 1, FOC1B = 1
			 * 3. Normal Mode WGM11=0, WGM10=0
			 * => TCCR1B Register:
			 * 4. Normal Mode WGM13=0, WGM12=0
			 * 5. Insert the required clock in (CS12, CS11, CS10) bits.
			 * => TCNT1 Register:
			 * 6. Insert the initial value that you want the timer to start from it.
			 * => TIMSK Register:
			 * 7. Enable Overflow interrupt (TOIE1)
			 */
			TCCR1A = (1<<FOC1A) | (1<<FOC1B);
			TCCR1B = (TCCR1B & 0xF8) | (Config_Ptr->timer_Prescaler);
			TCNT1 = (Config_Ptr->intialValue) & 0xFFFF;	/* In order to ensure that the register not exceeding his maximum value (65,535) */
			TIMSK |= (1<<TOIE1);
		}	/* End of Timer 1 Normal Mode */
		else if(Config_Ptr->timer_mode == COMPARE)
		{
			/* Configure Timer 1 control register (Compare Mode)
			 * => TCCR1A Register:
			 * 1. Non PWM mode FOC1A = 1, FOC1B = 1
			 * 2. CTC Mode WGM11=0, WGM10=0
			 * => TCCR1B Register:
			 * 3. CTC Mode WGM13=0, WGM12=1
			 * 4. Insert the required clock in (CS12, CS11, CS10) bits.
			 * => TIMSK Register:
			 * 5. Enable Compare match interrupt (OCIE1A) in TIMSK register
			 */
			TCCR1A = (1<<FOC1A) | (1<<FOC1B);
			TCCR1B |= (1<<WGM12);
			TCCR1B = (TCCR1B & 0xF8) | (Config_Ptr->timer_Prescaler);
			TCNT1 = (Config_Ptr->intialValue) & 0xFFFF;	/* In order to ensure that the register not exceeding his maximum value (255) */
			OCR1A = (Config_Ptr->compareValue) & 0xFFFF;
			TIMSK |= (1<<OCIE1A);
		}	/* End of Timer 1 Compare Mode */
		break;		/* End of Timer 1 */

		/*---------------------------------------Timer 2 --------------------------------------*/
	case TIMER2:
		/* Timer 2 Normal mode */
		if(Config_Ptr->timer_mode == NORMAL)
		{
			/*
			 * Configure the Timer 2 control registers (Normal Mode)
			 * => TCCR2 Register:
			 * 1. Non PWM mode FOC2=1.
			 * 2. Normal Mode WGM21=0 & WGM20=0.
			 * 3. Insert the required clock in (CS22, CS21, CS20) bits.
			 * => TCNT2 Register:
			 * 4. Insert the initial value that you want the timer to start from it.
			 * => TIMSK Register:
			 * 5. Enable Overflow interrupt (TOIE2)
			 */
			TCCR2 = (1<<FOC2);
			TCCR2 = (TCCR2 & 0xF8) | (Config_Ptr->timer_Prescaler);
			TCNT2 = (Config_Ptr->intialValue) & 0xFF;	/* In order to ensure that the register not exceeding his maximum value (255) */
			TIMSK |= (1<<TOIE2);
		}	/* End of Timer 2 Normal Mode */
		else if(Config_Ptr->timer_mode == COMPARE)
		{
			/* Configure Timer 2 control register (Compare Mode)
			 * => TCCR2 Register:
			 * 1. Non PWM mode FOC2=1
			 * 2. CTC Mode WGM21=1 & WGM20=0
			 * 3. Insert the required clock in (CS22, CS21, CS20) bits.
			 * => TCNT2 Register:
			 * 4. Insert the initial value that you want the timer to start from it.
			 * => TIMSK Register:
			 * 5. Enable Compare match interrupt (OCIE2) in TIMSK register
			 */
			TCCR2 = (1<<FOC2) | (1<<WGM21);
			TCCR2 = (TCCR2 & 0xF8) | (Config_Ptr->timer_Prescaler);
			TCNT2 = (Config_Ptr->intialValue) & 0xFF;	/* In order to ensure that the register not exceeding his maximum value (255) */
			OCR2 = (Config_Ptr->compareValue) & 0xFF;
			TIMSK |= (1<<OCIE2);
		}	/* End of Timer 2 Compare Mode */
		break;
	}
}




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
void Timer_setCallBack(void(*a_ptr)(void), TIMER_ID a_timerID)
{
	switch(a_timerID)
	{
	/*---------------------------------------Timer 0--------------------------------------*/
	case TIMER0:
		/* Save the address of the Call back function in a global variable */
		g_callBackPtrTimer0 = a_ptr;
		break;
	/*---------------------------------------Timer 1--------------------------------------*/
	case TIMER1:
		/* Save the address of the Call back function in a global variable */
		g_callBackPtrTimer1 = a_ptr;
		break;
	/*---------------------------------------Timer 2--------------------------------------*/
	case TIMER2:
		/* Save the address of the Call back function in a global variable */
		g_callBackPtrTimer2 = a_ptr;
		break;
	}
}



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
void Timer_stop(const TIMER_ID a_timerID)
{
	switch(a_timerID)
	{
	/*---------------------------------------Timer 0--------------------------------------*/
	case TIMER0:
		/* Clear the bits of the clock in TIMER0 */
		TCCR0 = TCCR0 & 0xF8;
		break;
	/*---------------------------------------Timer 1--------------------------------------*/
	case TIMER1:
		/* Clear the bits of the clock in TIMER1 */
		TCCR1B = TCCR0 & 0xF8;
		break;
	/*---------------------------------------Timer 2--------------------------------------*/
	case TIMER2:
		/* Clear the bits of the clock in TIMER2 */
		TCCR2 = TCCR0 & 0xF8;
		break;
	}
}



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
void Timer_DeInit(const TIMER_ID a_timerID)
{
	switch(a_timerID)
	{
	/*---------------------------------------Timer 0--------------------------------------*/
	case TIMER0:
		/* Clear All Timer0 Registers */
		TCNT0 = 0;
		OCR0 = 0;
		TCCR0 = 0;

		/* Disable the Timer 0 interrupt */
		TIMSK &= ~(1<<OCIE0) & ~(1<<TOIE0);

		/* Clear the Flag */
		TIFR |= (1<<OCF0) | (1<<TOV0);
		break;
	/*---------------------------------------Timer 1--------------------------------------*/
	case TIMER1:
		/* Clear All Timer 1 Registers */
		TCNT1 = 0;
		OCR1A = 0;
		TCCR1A = 0;
		TCCR1B = 0;

		/* Disable the Timer 1 interrupt */
		TIMSK &= ~(1<<OCIE1A) & ~(1<<TOIE1);

		/* Clear the Flag */
		TIFR |= (1<<OCF1A) | (1<<TOV1);
		break;
	/*---------------------------------------Timer 2--------------------------------------*/
	case TIMER2:
		/* Clear All Timer 2 Registers */
		TCNT1 = 0;
		OCR1A = 0;
		TCCR1A = 0;
		TCCR1B = 0;

		/* Disable the Timer 2 interrupt */
		TIMSK &= ~(1<<OCIE2) & ~(1<<TOIE2);

		/* Clear the Flag */
		TIFR |= (1<<OCF2) | (1<<TOV2);
		break;
	}
}
