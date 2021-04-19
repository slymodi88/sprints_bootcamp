/*
 * Us.c
 *
 *  Created on: Dec 25, 2019
 *      Author: memo1
 */
#include "Us.h"

#define TRIGGER_BIT  1
#define ECHO_PIN  2

#define LOW   0

/****************************************************************************************************
 * Function Name   : Us_Init.                                                                       *
 * Input Parameters : None.                                                                         *                                                                   *                                                                                                *
 * Output Parameters: None.                                                                         *
 * In_Out Parameters: None.                                                                         *
 * Return           : ERROR_STATUS.                                                                 *
 * Description      : This function initiate the UltraSonic by set the value of the Dio_Dirction of *
 *                    the pin trigger as an output then initiate the ICU to use it to calculate the *
 *                    On time of the ECHO signal.                                                   *
 *                                                                                                  *
 *                                                                                                  *
 ***************************************************************************************************/
ERROR_STATUS Us_Init(void)
{
	Icu_cfg_s Icu_Cfg;
	Icu_Cfg.ICU_Ch_No = ICU_CH2;
	Icu_Cfg.ICU_Ch_Timer = ICU_TIMER_CH0;
	Icu_Init(&Icu_Cfg);
	SET_BIT(PORTB_DIR,TRIGGER_BIT); //TRIGER PIN
	CLR_BIT(PORTB_DIR,ECHO_PIN); //ECHO PIN
	return E_OK;
}

/**************************************************************************************************
 * Function Name    : Us_Trigger.                                                                 *
 * Input Parameters : None.                                                                       *                                                                              *                                                                                                *
 * Output Parameters: None.                                                                       *
 * In_Out Parameters: None.                                                                       *
 * Return           : ERROR_STATUS                                                                *
 * Description      : This function send the  pulse, to trigger pin of the UltraSonic to enhance  *
 *                    the  ECUO signal                                                            *
 *************************************************************************************************/


ERROR_STATUS Us_Trigger(void)
{
	SET_BIT(PORTB_DATA,TRIGGER_BIT);
	for(uint8 Counter=0;Counter<10;Counter++);
	CLR_BIT(PORTB_DATA,TRIGGER_BIT);
	return E_OK;
}





/**************************************************************************************************
 * Function Name    : Us_GetDistance.                                                             *
 * Input Parameters : pointer to uint16 that will return the distance on it.                      *                                                                                              *                                                                                                *
 * Output Parameters: Distance.                                                                   *
 * In_Out Parameters: None.                                                                       *
 * Return           : ERROR_STATUS.                                                               *
 * Description      : This function get the time of high signal of ECUO pulse, then               *
 *                    use it to calculate the distance, then return the value in the              *
 *                    Distance parameter.                                                         *
 *************************************************************************************************/


ERROR_STATUS Us_GetDistance(uint16 *Distance)
{
	uint32 Icu_Time =LOW;
	Us_Trigger();
//	Icu_ReadTime(ICU_CH2,ICU_RISE_TO_FALE,&Icu_Time);
	Icu_RiseToFall(&Icu_Time);
	*Distance = (Icu_Time /58);
	return E_OK;
}


