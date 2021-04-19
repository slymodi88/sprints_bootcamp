/*
 * timer.c
 *
 *  Created on: Dec 21, 2019
 *      Author: eng-m
 */



/************************************************************************/
/*		                        INCLUDES                 		        */
/************************************************************************/
#include "registers.h"
#include "Timer1.h"
#include "common_macros.h"
#include "interrupt.h"
/************************************************************************/
/*		                        DEFINITIONS                 		        */
/************************************************************************/

/*TIFR BITS NAME*/
#define TOV0  (0x01)
#define TOV1  (0x04)
#define TOV2  (0x40)

/*TIMSK BITS Define*/
#define TOIE0 (0x01)
#define TOIE1 (0x04)
#define TOIE2 (0x40)

#define TOIE(CH) (CH) ? (TOIE1):(TOIE0)

#define COUNTER_OFFSET  (0x08)
#define STOP_TIMER (0xf8)
#define MAX_MODES 3

/************************************************************************/
/*		                 TIMER STATIC VARIABLES          		        */
/************************************************************************/
uint8 gau8_TimerInitialized[3] = {0};
void (*TimersOverflow_CBF[MAX_CH])() = {NULL,NULL,NULL};
static uint8 gau8_TimerPreScaler[MAX_CH] = {0};
static uint8 gau8_CounterType[2] = {0};

/************************************************************************/
/*		                		TIMER ISR                  		        */
/************************************************************************/

ISR(TIMER0_OVF_vect)
{

	if(	TimersOverflow_CBF[TIMER_CH0] != NULL)
	{
		TimersOverflow_CBF[TIMER_CH0]();

	}
}
ISR(TIMER1_OVF_vect)
{
	if(	TimersOverflow_CBF[TIMER_CH1] != NULL)
	{
		TimersOverflow_CBF[TIMER_CH1]();
	}

}
ISR(TIMER2_OVF_vect)
{

	if(	TimersOverflow_CBF[TIMER_CH2] != NULL)
	{
		TimersOverflow_CBF[TIMER_CH2]();
	}

}

/************************************************************************/
/*		                TIMER FUNCTIONS' DEFINITION     		        */
/************************************************************************/


/**
 * Input: Pointer to a structure contains the information needed to initialize the timer.
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: Initiates the module.
 *
 */
ERROR_STATUS Timer_Init(Timer_cfg_s* Timer_cfg){
	/* check in the parameters of Timer_cfg are in their range or not.*/
	if(Timer_cfg != NULL && gau8_TimerInitialized[Timer_cfg->Timer_CH_NO] == 0  \
	    && Timer_cfg->Timer_Mode < MAX_MODES && Timer_cfg->Timer_Polling_Or_Interrupt < 2\
		&& Timer_cfg->Timer_Prescaler <= TIMER_PRESCALER_1024)
	{
		/*Timer0 and Timer 1 have the same value of prescal and counter so work initiate them are similar */
		if(Timer_cfg->Timer_CH_NO == TIMER_CH0 || Timer_cfg->Timer_CH_NO == TIMER_CH1)
		{
				if(Timer_cfg->Timer_CH_NO == TIMER_CH0)
				{
					/* Overflow mode
						 * TCCR0:WGM 1:0 = 0
						 * 		 COM 1:0 = 0
						 * 		 FOC0 = 0
						 * 		 set clk 0 at first. then apply it at start function.
						 * */
					TCCR0 = LOW;
				}
				else
				{
					TCCR1 = LOW;
				}
				/*
				 * Set the mask if interrupt required */
				if(Timer_cfg->Timer_Polling_Or_Interrupt == TIMER_INTERRUPT_MODE)
				{
					TIMSK |= TOIE(Timer_cfg->Timer_CH_NO);
				}
				else
				{

				}

				if(Timer_cfg->Timer_Mode != TIMER_MODE)
				{
					if(Timer_cfg->Timer_Prescaler != TIMER_PRESCALER_NO)
					{
						/* there is no prescaler applicable on counter modes*/
						return E_NOK;
					}
					else
					{
						/* at falling edge need -> 110 = 6  8 - #define of falling
						 * at Rising edge need 	-> 111 = 7  8 - #define of rising
						 * */
						gau8_CounterType[Timer_cfg->Timer_CH_NO] = COUNTER_OFFSET - Timer_cfg->Timer_Mode;
					}
				}
				else
				{
					/*for Timer mode at timer 0:1 there is no 32 or 128 precaler. So map the coming value and set it at
					 * global variable to use at start function.
					 * */
					switch(Timer_cfg->Timer_Prescaler){
						case TIMER_PRESCALER_NO:
						case TIMER_PRESCALER_8:
							gau8_TimerPreScaler[Timer_cfg->Timer_CH_NO] = Timer_cfg->Timer_Prescaler;
							break;
						case TIMER_PRESCALER_64:
							gau8_TimerPreScaler[Timer_cfg->Timer_CH_NO] = Timer_cfg->Timer_Prescaler - 1;
							break;
						case TIMER_PRESCALER_256:
						case TIMER_PRESCALER_1024:
							gau8_TimerPreScaler[Timer_cfg->Timer_CH_NO] = Timer_cfg->Timer_Prescaler - 2;
							break;

						default:
							return E_NOK;
					}

				}
		}
		/*	Timer 2 Initialization */
		else if(Timer_cfg->Timer_CH_NO == TIMER_CH2)
		{
			/* Overflow mode
					*  TCCR0:WGM2 1:0 = 0
					* 		 COM2 1:0 = 0
					* 		 FOC2 = 0
					* 		 set clk 0 at first. then apply it at start function.
				    */
				TCCR2 = LOW;
				/*set mask of interrupt if required */
				if(Timer_cfg->Timer_Polling_Or_Interrupt == TIMER_INTERRUPT_MODE){
					SET_BIT(TIMSK, TOIE2);
				}
				else{
					CLR_BIT(TIMSK, TOIE2);
				}
				if(Timer_cfg->Timer_Mode != TIMER_MODE){
					/* counter mode not implemented at TIMER 2*/
					return E_NOK;
				}
				else{
					/* assign the value of prescaler to global variable to use it at start function. */
					gau8_TimerPreScaler[Timer_cfg->Timer_CH_NO] = Timer_cfg->Timer_Prescaler;
				}
		}
		else
		{
			/* if timer channel not exist */
			return E_NOK;
		}

	}
	else{
		/* if any of initial check are false return error*/
		return E_NOK;

	}
	/*
	 * set flag to timer to indicate that it's initialized successfully
	 */
	gau8_TimerInitialized[Timer_cfg->Timer_CH_NO] = 1;
	TimersOverflow_CBF[Timer_cfg->Timer_CH_NO] = Timer_cfg->Timer_Cbk_ptr;
	return E_OK;
}


/**
 * Input:
 * 	Timer_CH_NO: The channel number of the timer needed to be started.
 *	Timer_Count: The start value of the timer.
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function starts the selected timer.
 *
 */
ERROR_STATUS Timer_Start(uint8 Timer_CH_NO, uint16 Timer_Count){
	/*	check if timer is already initialized */
	if(Timer_CH_NO < MAX_CH && gau8_TimerInitialized[Timer_CH_NO] == 1){
		if( ((Timer_CH_NO == TIMER_CH2) || (Timer_CH_NO == TIMER_CH0)) &&  Timer_Count>256 ){
			/*check if counts are in range */
			return E_NOK;
		}
		else{
			switch(Timer_CH_NO){
			/*for the selected timer load the TCNT wit initial value
			 * set clk to TCCR0 by assigned prescaler.
			 * if counter selected choose if counter up or down*/
				case TIMER_CH0:
					TCNT0 = TIMER_8BITS_OVF - Timer_Count;
					if(gau8_CounterType[Timer_CH_NO] == 0){
						TCCR0 |=  gau8_TimerPreScaler[TIMER_CH0];
					}
					else
					{
						TCCR0 |=  gau8_CounterType[TIMER_CH0];
					}
					break;

				case TIMER_CH1:
					TCNT1 = TIMER_16BITS_OVF - Timer_Count;
					if(gau8_CounterType[TIMER_CH1] == 0){
						TCCR1 |=  gau8_TimerPreScaler[TIMER_CH1];
					}
					else
					{
						TCCR1 |=  gau8_CounterType[TIMER_CH1];
					}
					break;

				case TIMER_CH2:
					TCNT2 = TIMER_8BITS_OVF - Timer_Count;
					TCCR2 |=  gau8_TimerPreScaler[TIMER_CH2];
					break;

				default:
					/* if channel not exist selected*/
					return E_NOK;
			}

		}

	}
	else{
		/* initialization check in */
		return E_NOK;
	}

	return E_OK;
}

/**
 * Input:
 * 	Timer_CH_NO: The channel number of the timer needed to be stopped.
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function stops the needed timer.
 *
 */
ERROR_STATUS Timer_Stop(uint8 Timer_CH_NO)
{
	switch(Timer_CH_NO)
	{
		case TIMER_CH0:
			TCNT0 = 0;
			TCCR0 &= STOP_TIMER ;
			break;

		case TIMER_CH1:
			TCNT1 = 0;
			TCCR1 &= STOP_TIMER;
			break;

		case TIMER_CH2:
			TCNT2 = 0;
			TCCR2 &=  STOP_TIMER;
			break;

		default:
			return E_NOK;
	}

	gau8_CounterType[Timer_CH_NO] = 0;
	gau8_TimerInitialized[Timer_CH_NO] = 0;
	gau8_TimerPreScaler[Timer_CH_NO] = 0;
	return E_OK;
}


/*
 * Input:
 * 	Timer_CH_NO: The channel number of the timer needed to get its status.
 * Output:
 * 	Data: A variable of type bool returns if the flag of the timer is raised or not.
 * In/Out:
 * Return: The error status of the function.
 * Description: This function is used to return if the flag of the timer is raised or not.
 */
ERROR_STATUS Timer_GetStatus(uint8 Timer_CH_NO, bool* Data){
	/*
	 * return the value of TOV if set or not
	 * */
	uint8 u8_overflowFlag = 0;
	switch(Timer_CH_NO)
	{
		case TIMER_CH0:
			*Data = GET_BIT(TIFR,TOV0);
			u8_overflowFlag = TOV0;
			break;

		case TIMER_CH1:
			*Data = GET_BIT(TIFR,TOV1);
			u8_overflowFlag = TOV1;
			break;

		case TIMER_CH2:
		    *Data = GET_BIT(TIFR,TOV2);
		    u8_overflowFlag = TOV2;
		    break;
		default:
			return E_NOK;
	}
	if(*Data == TRUE)
	{
		CLR_BIT(TIFR,u8_overflowFlag);
	}
	else
	{

	}

	return E_OK;
}


/**
 * Input:
 * 	Timer_CH_NO: The channel number of the timer needed to get its value.
 * Output:
 * 	Data: This is the output variable of the function which holds the value of the timer.
 * In/Out:
 * Return: The error status of the function.
 * Description: This function is used to return the value of the TCNT REG.
 *
 */
ERROR_STATUS Timer_GetValue(uint8 Timer_CH_NO, uint16* Data){
	switch(Timer_CH_NO)
		{
			case TIMER_CH0:
				*Data = TCNT0;
				break;

			case TIMER_CH1:
				*Data = TCNT1;
				break;

			case TIMER_CH2:
			    *Data = TCNT2;
			    break;
			default:
				return E_NOK;
		}

	return E_OK;

}

