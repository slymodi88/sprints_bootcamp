
/*
 * ExternalInterrupt.c
 *
 *  Created on: Dec 24, 2019
 *      Author: memo1
 */
#include "ExternalInterrupt.h"
#include "registers.h"
#include "interrupt.h"

#define ISC2  6

#define INT1  7
#define INT0  6
#define INT2  5

static void (*ExtInt_CBK_Ptr[3])(void) = {NULL,NULL,NULL};

/*
 * Function: ExternInt_Init
 * Inputs:	ExternIntCfg -> pointer of type ExternInt_Cfg_s which points to structure that contain the initialized data.
 * Outputs:
 * In Out:
 * Return: ERROR_STATUS -> check if there any error occurs, would return E_NOK else it would return E_OK.
 * Description: Initiate the required external interrupt configuration as it specify
 *	event of interrupt.
 *	Enable the required external interrupt from GICR but to allow the interrupt global interrupt must be set
 */
ERROR_STATUS ExternInt_Init(ExternInt_Cfg_s *ExternIntCfg)
{
		if(ExternIntCfg->ExternInt_No == EXTRN_INT_0 ||ExternIntCfg->ExternInt_No == EXTRN_INT_1)
	{
		MCUCR |= ((ExternIntCfg->ExternInt_Event)<<(2*ExternIntCfg->ExternInt_No));

	}
	else if (ExternIntCfg->ExternInt_No == EXTRN_INT_2)
	{


		if (ExternIntCfg->ExternInt_Event == FALLING_EDGE)
		{
			MCUCSR &= ~(1<<ISC2);
		}
		else if(ExternIntCfg->ExternInt_Event == RISING_EDGE)
		{

			MCUCSR |= (1<<ISC2);

		}
		else
		{
			return E_NOK;
		}

	}
	else {
		return E_NOK;
	}

	ExtInt_CBK_Ptr[ExternIntCfg->ExternInt_No] = ExternIntCfg->ExternInt_CBF_Ptr;
	return E_OK;

}

/*
 * Function: ExternInt_SetEvent
 * Inputs: ExternInt_No -> Specify one of the external interrupt (INT_0, INT_1, INT2).
 * 	       InterruptEvent -> indicate required event for INT0 and INT1 there are 4 events to check (RISING_EDGE,FALLING_EDGE,LOW_LEVEL,LOGICAL_CHANGE).
							  But for Interrupt 2 there are only Two cases (Rising,Falling)
 * Outputs:
 * In Out:
 * Return: ERROR_STATUS -> check if there any error occurs, would return E_NOK else it would return E_OK.
 * Description: function is used to set event of required external interrupt.
 * 				note: usually used to poll on the flag.
 */
ERROR_STATUS ExternInt_SetEvent(uint8 ExternInt_No,uint8 InterruptEvent)
{
	if(ExternInt_No == EXTRN_INT_0 ||ExternInt_No == EXTRN_INT_1)
	{
		MCUCR |= ((InterruptEvent)<<(2*InterruptEvent));

	}
	else if (ExternInt_No == EXTRN_INT_2)
	{
		if (InterruptEvent == FALLING_EDGE)
		{
			MCUCSR &= ~(1<<ISC2);
		}
		else if(InterruptEvent == RISING_EDGE)
		{
			MCUCSR |= (1<<ISC2);

		}
		else
		{
			return E_NOK;
		}

	}
	else {
		return E_NOK;
	}
	return E_OK;


}


/*
 * Function: ExternInt_GetStatus
 * Inputs: ExternInt_No -> Specify one of the external interrupt (INT_0, INT_1, INT2).
 * Outputs: *status -> points to the value of selected interrupt flag.
 * In Out:
 * Return: ERROR_STATUS -> check if there any error occurs, would return E_NOK else it would return E_OK.
 * Description: function is used to get flag of required interrupt status.
 * 				note: usually used to poll on the flag.
 */
ERROR_STATUS ExternInt_GetStatus(uint8 ExternInt_No,uint8 *Status)
{
	if(ExternInt_No == EXTRN_INT_0)
	{
		 *Status = ((GIFR >> INT0)&1);


	}
	else if(ExternInt_No == EXTRN_INT_1)
	{
		 *Status = ((GIFR >> INT1)&1);


	}
	else if (ExternInt_No == EXTRN_INT_2)
	{
		 *Status = ((GIFR >> INT2)&1);

	}
	else
	{
		return E_NOK;
	}
	return E_OK;

}

/*
 * Function: ExternInt_Disable
 * Inputs: ExternInt_No -> Specify one of the external interrupt (INT_0, INT_1, INT2)
 * Outputs:
 * In Out:
 * Return: ERROR_STATUS -> check if there any error occurs, would return E_NOK else it would return E_OK.
 * Description: Disable the required external interrupt.
 */
ERROR_STATUS ExternInt_Enable(uint8 ExternInt_No)
{
	if(ExternInt_No == EXTRN_INT_0)
	{
		GICR |=(1<<INT0);


	}
	else if(ExternInt_No == EXTRN_INT_1)
	{
		GICR |=(1<<INT1);


	}
	else if (ExternInt_No == EXTRN_INT_2)
	{
		GICR |=(1<<INT2);

	}
	else
	{
		return E_NOK;
	}
	return E_OK;

}

/*
 * Function: ExternInt_Disable
 * Inputs: ExternInt_No -> Specify one of the external interrupt (INT_0, INT_1, INT2)
 * Outputs:
 * In Out:
 * Return: ERROR_STATUS -> check if there any error occurs, would return E_NOK else it would return E_OK.
 * Description: Disable the required external interrupt.
 */
ERROR_STATUS ExternInt_Disable(uint8 ExternInt_No)
{
	if(ExternInt_No == EXTRN_INT_0)
	{
		GICR &= ~(1<<INT0);


	}
	else if(ExternInt_No == EXTRN_INT_1)
	{
		GICR &= ~(1<<INT1);


	}
	else if (ExternInt_No == EXTRN_INT_2)
	{
		GICR &= ~(1<<INT2);

	}
	else
	{
		return E_NOK;
	}
	return E_OK;
}
ISR(INT0_vect)
{
	if(NULL!= ExtInt_CBK_Ptr[EXTRN_INT_0])
	{
		ExtInt_CBK_Ptr[EXTRN_INT_0]();

	}

}
ISR(INT1_vect)
{

	if(NULL!= ExtInt_CBK_Ptr[EXTRN_INT_1])
	{
		ExtInt_CBK_Ptr[EXTRN_INT_1]();

	}

}
ISR(INT2_vect)
{


	if(NULL!= ExtInt_CBK_Ptr[EXTRN_INT_2])
	{
		ExtInt_CBK_Ptr[EXTRN_INT_2]();

	}

}

