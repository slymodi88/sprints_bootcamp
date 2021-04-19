/*
 * ICU.c
 *
 *  Created on: Dec 24, 2019
 *      Author: memo1
 */
#include "ICU.h"
#include "Timer1.h"
#include "ExternalInterrupt.h"
#include "registers.h"


#define IS_SET    1
#define IS_CLEAR  0

#define TIMER_FACTOR  256
#define TCNT_VALUE     255

static uint16 gOVF_Counts = IS_CLEAR;
static uint8 gEdge_Flag =RISING_EDGE;
static uint16 gTimer_Counts = IS_CLEAR ;
static uint8 Ready_To_Send_Flag = IS_CLEAR ;



/***************************************************************************
 * Function		: Icu_TimerOverFlows_Callback_Func						   *
 * Output 		:														   *
 * Return 		:														   *
 * Description	: Called inside the timer overflow ISR by its address	   *
 * to calculate and deliver the timer overflows to the Icu module		   *
 ***************************************************************************/
void Icu_TimerOverFlows_Callback_Func(void)
{

	gOVF_Counts++;


}

/***************************************************************************
 * Function		: Icu_EdgeToEdge_Callback_Func					    	   *
 * Output 		:														   *
 * Return 		:														   *
 * Description	: Called inside the external interrupt ISR by its address  *
 * to calculate and deliver the timer counts to the Icu module		       *
 ***************************************************************************/
void Icu_EdgeToEdge_Callback_Func(void)
{


	if(RISING_EDGE==gEdge_Flag )
	{

		gOVF_Counts =IS_CLEAR;
		Timer_Start(TIMER_CH0,TCNT_VALUE);

		ExternInt_SetEvent(EXTRN_INT_2, FALLING_EDGE);
		gEdge_Flag = FALLING_EDGE;

	}
	else if( FALLING_EDGE == gEdge_Flag){


		Timer_Stop(TIMER_CH0);

		Timer_GetValue(TIMER_CH0, &gTimer_Counts);
		gTimer_Counts += (gOVF_Counts * TIMER_FACTOR);

		ExternInt_SetEvent(EXTRN_INT_2, RISING_EDGE);
		gEdge_Flag = RISING_EDGE;

	}
	Ready_To_Send_Flag = IS_SET;

}

/**************************************************************************
 * Function 	: Icu_Init                                                *
 * Input 		: Icu_cfg : pointer to structure of type Icu_cfg_s        *
 * Return 		: value of type ERROR_STATUS							  *
 * 				  which is one of the following values:					  *
 * 				  - E_OK  : initialized successfully					  *
 *				  - E_NOK : not initialized successfully				  *
 * Description  : Initializes the ICU by initializing the timer			  *
 * 				  and enabling the global interrupt						  *
 **************************************************************************/

ERROR_STATUS Icu_Init(Icu_cfg_s * Icu_Cfg)
{

	ExternInt_Cfg_s ExtInt_Cfg;
	ExtInt_Cfg.ExternInt_No = ICU_CH2 ;
	ExtInt_Cfg.ExternInt_Event = RISING_EDGE;
	ExtInt_Cfg.ExternInt_CBF_Ptr = Icu_EdgeToEdge_Callback_Func;
	ExternInt_Init(&ExtInt_Cfg);
	ExternInt_Enable(ICU_CH2);
	Timer_cfg_s Timer_Cfg;
	Timer_Cfg.Timer_CH_NO = TIMER_CH0;
	Timer_Cfg.Timer_Mode = TIMER_MODE;
	Timer_Cfg.Timer_Polling_Or_Interrupt = TIMER_INTERRUPT_MODE;
	Timer_Cfg.Timer_Prescaler = TIMER_PRESCALER_1024;
	Timer_Cfg.Timer_Cbk_ptr= Icu_TimerOverFlows_Callback_Func;
	Timer_Init(&Timer_Cfg);

	if (Icu_Cfg->ICU_Ch_No == ICU_CH2)
	{
		ExternInt_Init(&ExtInt_Cfg);


	}
	else
	{
		return E_NOK;
	}
	return E_OK;
}



/***************************************************************************
 * Function		: Icu_RiseToFall										   *
 * Output 		: Icu_Time : pointer to uint32 variable to give the time   *
 * 				  from rising edge to falling edge						   *
 * Return 		: value of type ERROR_STATUS							   *
 * 				  which is one of the following values:					   *
 * 				  - E_OK  : successful									   *
 *				  - E_NOK : not successful								   *
 * Description	: calculates the time between the rising and the falling   *
 * 				  edges												 	   *
 ***************************************************************************/

ERROR_STATUS Icu_RiseToFall(uint16 * Icu_Time)
{
//	while (!Ready_To_Send_Flag );
	*Icu_Time = gTimer_Counts;

	Ready_To_Send_Flag = IS_CLEAR;
	//gTimer_Counts = 0;
	return E_OK;
}

/***************************************************************************
 * Function		: Icu_FallToRise										   *
 * Output 		: Icu_Time : pointer to uint32 variable to give the time   *
 * 				  from falling edge to rising edge						   *
 * Return 		: value of type ERROR_STATUS							   *
 * 				  which is one of the following values:					   *
 * 				  - E_OK : successful									   *
 *				  - E_NOK : not successful								   *
 * Description	: calculates the time between the falling and the rising   *
 * 				  edges													   *
 ***************************************************************************/
ERROR_STATUS Icu_FallToRise(uint32 * Icu_Time)
{
	return E_OK;
}

/***************************************************************************
 * Function		: Icu_RiseToRise										   *
 * Output 		: Icu_Time : pointer to uint32 variable to give the time   *
 * 				  from rising edge to next rising edge					   *
 * Return 		: value of type ERROR_STATUS							   *
 * 				  which is one of the following values:					   *
 * 				  - E_OK : successful									   *
 *				  - E_NOK : not successful								   *
 * Description	: calculates the time between the rising edge and the next *
 * 				  rising edge											   *
 ***************************************************************************/
ERROR_STATUS Icu_RiseToRise(uint32 * Icu_Time)
{
	return E_OK;
}

