/*
 * TMU.c
 *
 *  Created on: Jan 11, 2020
 *      Author: memo1
 */
#include "Tmu.h"





/************************************************************************/
/*				            BUFFER STRUCT			                    */
/************************************************************************/
typedef struct Tmu_Tasks_Buffer_s_t
{
	uint8 Task_Time;
	uint8 Task_Repetition;
	uint8 Task_Counter;
	Task_Cbk_ptr Task_Ptr;
}Tmu_Tasks_Buffer_s_t;

/************************************************************************/
/*				            Global Variables			                */
/************************************************************************/
uint8 Init_Error_State ;
uint8 Start_Error_State ;
uint8 Stop_Error_State ;
uint8 Dispatcher_Error_State ;
uint8 Deinit_Error_State ;
uint8 gTimer_Resolution ;
uint8 g_Resolution_Value;
static uint8 g_Task_Tracer ;
static uint8 Timer_Start_Flag ;
static uint8 CallFuncFlag;
static Tmu_Tasks_Buffer_s_t g_Tasks_Buffer[TASKS_MAX_SIZE];

/* Timer CallBackFunction */
void TMU_TimerCheckFlag_CBK (void)
{
	CallFuncFlag = TRUE;
	//Timer_Set(str_Tmu_Cfg.Timer_Id,g_Resolution_Value);

}

/**
 * Input: Pointer to a structure contains the information needed to initialize the TMU.
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: Initiates the module, it sets the resolution of the TMU and chooses the timer to work on.
 *
 */
ERR_STATUS Tmu_Init(Tmu_Cfg_s_t * Tmu_Cfg)
{
	ERR_STATUS retval = TMU_BASE + ERR_SUCCESS;
	Init_Error_State = INITIALIZED;
	Start_Error_State = STARTED;
	Stop_Error_State = STOPPED;
	Dispatcher_Error_State = DISP_SUCESS;
	Deinit_Error_State = E_OK;
	gTimer_Resolution = LOW;
	g_Task_Tracer = TMU_BUFFER_INDEX_BEGIN;
	Timer_Start_Flag = LOW;
	CallFuncFlag=FALSE;

	if(NULL == Tmu_Cfg)
	{
		Init_Error_State = NOTINITIALIZED;
		retval = TMU_BASE + ERR_NULL_PTR;
	}
	else
	{

		Timer_cfg_s str_Timer_Cfg;
		str_Timer_Cfg.Timer_CH_NO = Tmu_Cfg->Timer_Id;
		str_Timer_Cfg.Timer_Polling_Or_Interrupt = TIMER_INTERRUPT_MODE;
		str_Timer_Cfg.Timer_Mode = TIMER_MODE;
		str_Timer_Cfg.Timer_Prescaler = TIMER_PRESCALER_1024;
		str_Timer_Cfg.Timer_Cbk_ptr = TMU_TimerCheckFlag_CBK;
		Timer_Init(&str_Timer_Cfg);
		gTimer_Resolution = Tmu_Cfg->Timer_Resolution;
		g_Resolution_Value = (gTimer_Resolution * MAX_TIMER_COUNTS)/ MAX_RES;



	}

	return retval;
}

/**
 * Input:
 * 	TMU_CBF_Ptr: The callback function to be requested upon end of delay period.
 *	TMU_Count_Period: The delay time as a count of timer resolution period unit.
 *	Repetition: Determines the number of repetitions, whether the function is called once or periodic
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function starts the delay period required by an application and calls a function upon
 *				 the end of the delay period.It can repeat this task indefinitely
 */
ERR_STATUS Tmu_Start( Task_Cbk_ptr Ptr_ToFunc,uint16 delay_time,uint8 periodicity)
{
	ERR_STATUS retval = TMU_BASE + ERR_SUCCESS;
	if(Init_Error_State==INITIALIZED)
	{
		if(g_Task_Tracer < TASKS_MAX_SIZE)
		{
			if(NULL==Ptr_ToFunc)
			{
				Start_Error_State = START_ERROR;
				retval = TMU_BASE + ERR_NULL_PTR;

			}
			else
			{
				/* store task data into buffer */
				g_Task_Tracer++;
				g_Tasks_Buffer[g_Task_Tracer].Task_Ptr = Ptr_ToFunc;
				g_Tasks_Buffer[g_Task_Tracer].Task_Time = delay_time;
				g_Tasks_Buffer[g_Task_Tracer].Task_Repetition = periodicity;
				g_Tasks_Buffer[g_Task_Tracer].Task_Counter = LOW;

				if(Timer_Start_Flag==LOW)
				{
					/* start timer0 */
					Timer_Start(str_Tmu_Cfg.Timer_Id,g_Resolution_Value);
					Timer_Start_Flag = HIGH;
				}


			}
		}
		else
		{
			Start_Error_State = BUFFER_IS_FULL;
			retval = TMU_BASE + ERR_BUFFER_FULL;
		}
	}
	else
	{
		Start_Error_State =START_ERROR;
		retval = TMU_BASE + ERR_NOTINITIALIZED;

	}

	return retval;
}
/**
 * Input:
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: The main Function in the super loop, it continuously checks the flag assigned
 *				by the timer ISR to update the counter of the currently serviced delays. It continuously
 *				compares each counter with their respective TMU_Count_Period as well. When the counter
 *				reaches its respective TMU_Count_Period value, it calls its callback function
 *
 */
ERR_STATUS Tmu_Dispatcher(void)
{
	ERR_STATUS retval = TMU_BASE + ERR_SUCCESS;
	if(Init_Error_State == INITIALIZED)
	{
		/*This flag is to prevent the called function to be called more than one time during the reselution*/
		if(CallFuncFlag == TRUE )
		{
			for(uint8 Counter =0 ; Counter < g_Task_Tracer;Counter++)


			{
				g_Tasks_Buffer[Counter].Task_Counter++;

				/*Check if it is the time to execute the function or not*/
				if((g_Tasks_Buffer[Counter].Task_Counter) == (g_Tasks_Buffer[Counter].Task_Time))

				{
					if(g_Tasks_Buffer[Counter].Task_Repetition==SINGLE_SHOT)
					{
						/* call consumer */
						g_Tasks_Buffer[Counter].Task_Ptr();
						Tmu_Stop(g_Tasks_Buffer[Counter].Task_Ptr);
					}
					else if(g_Tasks_Buffer[Counter].Task_Repetition==PERIODIC)
					{
						g_Tasks_Buffer[Counter].Task_Ptr();
						g_Tasks_Buffer[Counter].Task_Counter = LOW;

					}
					else
					{
						Dispatcher_Error_State = DISP_ERROR;
						retval = TMU_BASE + ERR_INVALID_ARGU;

					}


				}
			}
			/* set calling flag with zero */
			CallFuncFlag= FALSE;
		}
		else
		{
			/* DO NOTHING */
		}
	}
	else
	{
		/* return error */
		Dispatcher_Error_State = DISP_ERROR;
		retval = TMU_BASE + ERR_NOTINITIALIZED;

	}
	return retval;
}
/**
 * Input:
 * 	TMU_CBF_Ptr: The callback function that should no longer be called.
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function stops the call of the given function.
 *
 */
ERR_STATUS Tmu_Stop(Task_Cbk_ptr ptr_Task_CBK)
{
	ERR_STATUS retval = TMU_BASE +ERR_SUCCESS;
	if(g_Task_Tracer==0)
	{
		g_Tasks_Buffer[g_Task_Tracer].Task_Counter=LOW;
		g_Tasks_Buffer[g_Task_Tracer].Task_Ptr=NULL;
		g_Tasks_Buffer[g_Task_Tracer].Task_Repetition=LOW;
		g_Tasks_Buffer[g_Task_Tracer].Task_Time=LOW;
		g_Task_Tracer--;
		Timer_Stop(str_Tmu_Cfg.Timer_Id);
	}
	else if (g_Task_Tracer > 0)
	{
		for(uint8 Counter=0;Counter<g_Task_Tracer;Counter++)
		{
			if(g_Tasks_Buffer[Counter].Task_Ptr == ptr_Task_CBK)
			{
				g_Tasks_Buffer[Counter].Task_Counter = g_Tasks_Buffer[g_Task_Tracer].Task_Counter;
				g_Tasks_Buffer[Counter].Task_Ptr = g_Tasks_Buffer[g_Task_Tracer].Task_Ptr;
				g_Tasks_Buffer[Counter].Task_Repetition = g_Tasks_Buffer[g_Task_Tracer].Task_Repetition;
				g_Tasks_Buffer[Counter].Task_Time = g_Tasks_Buffer[g_Task_Tracer].Task_Time;
				g_Task_Tracer--;
				break;

			}
			else
			{
				/* DO NOTHING */
			}

		}



	}
	else
	{
		retval = TMU_BASE + ERR_INVALID_ARGU;
	}

	return retval;
}

/**
 * Input:
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: DeInitiates the module, returns initial settings to default
 *
 */
ERR_STATUS Tmu_Deinit(void)
{
	ERR_STATUS retval = TMU_BASE + ERR_SUCCESS;
	g_Task_Tracer = TMU_BUFFER_INDEX_BEGIN;
	Init_Error_State = NOTINITIALIZED;
	Timer_Start_Flag = LOW;
	retval = Timer_Stop(TIMER_CH0);


	return retval;



}
