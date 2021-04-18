/*
 * TMU.h
 *
 *  Created on: Jan 11, 2020
 *      Author: memo1
 */
#ifndef SERVICELAYER_TMU_TMU_H_
#define SERVICELAYER_TMU_TMU_H_
/************************************************************************/
/*				 INCLUDES			                                    */
/************************************************************************/

#include "std_types.h"
#include "SOS_Lcfg.h"
#include "SOS_Cfg.h"
#include "Timer1.h"
#include "common_retval.h"
#include "sleep.h"
/************************************************************************/
/*		                      DEFINES                                          */
/************************************************************************/

#define TMU_BUFFER_INDEX_BEGIN (-1)

#define NOTINITIALIZED   0
#define INITIALIZED      1

#define STARTED          2
#define START_ERROR      3

#define BUFFER_IS_FULL   4

#define STOPPED          5
#define STOP_ERROR       6

#define DISP_SUCESS      7
#define DISP_ERROR       8

#define MAX_RES          16

#define MAX_TIMER_COUNTS  256


/************************************************************************/
/*		         DATA TYPES	                                            */
/************************************************************************/

typedef void (*Task_Cbk_ptr)(void);




typedef enum {Idle,READY, WAIT,  DELETE}State_t;

/************************************************************************/
/*		         TMU FUNCTIONS' PROTOTYPES		                        */
/************************************************************************/


/**
 * Input: Pointer to a structure contains the information needed to initialize the TMU.
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: Initiates the module, it sets the resolution of the TMU and chooses the timer to work on.
 *
 */
ERR_STATUS Sos_Init(Sos_Cfg_s_t * Sos_Cfg);
/**
 * Input:
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: DeInitiates the module, returns initial settings to default
 *
 */
//ERR_STATUS Tmu_Deinit(void);
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
ERR_STATUS Sos_Create( Task_Cbk_ptr Ptr_ToFunc,uint8 priority,uint16 periodicity);
/**
 * Input:
 * 	TMU_CBF_Ptr: The callback function that should no longer be called.
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function stops the call of the given function.
 *
 */
ERR_STATUS Sos_Delete (Task_Cbk_ptr ptr_Task);
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
ERR_STATUS Sos_Run(void);




#endif /* SERVICELAYER_TMU_TMU_H_ */
