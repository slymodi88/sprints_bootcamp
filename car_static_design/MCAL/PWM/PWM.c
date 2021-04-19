/*
 * PWM.c
 *
 *  Created on: Dec 23, 2019
 *      Author: memo1
 */
#include "std_types.h"
#include "registers.h"
#include "PWM.h"
#include "common_macros.h"


#define BIT0     0
#define BIT1     1
#define BIT2     2
#define BIT3     3
#define BIT4     4
#define BIT5     5
#define BIT6     6
#define BIT7     7

#define OFF      0

#define		PRESCALER_NO				1
#define		PRESCALER_8					8
#define		PRESCALER_64				64
#define		PRESCALER_256				256
#define		PRESCALER_1024				1024

#define DUTYFACTOR  100
#define FREQFACTOR   2

#define FREQUENCY (16000000)
static uint8 gu8_PWM1_PreScaller=LOW;
static uint32 gu8_PWM1_PreScaller_Value=LOW;
static uint32 gu8_PWM1_CH1A_DutyCycle = LOW;
static uint32 gu8_PWM1_CH1B_DutyCycle = LOW;
static uint32 gu8_PWM1_CH1A_Frequency = LOW;
static uint32 gu8_PWM1_CH1B_Frequency = LOW;
static uint32 gu8_PWM1_CHAB_DutyCycle = LOW;
static uint32 gu8_PWM1_CHAB_Frequency = LOW;

/*********************************************************************************/
/* Function: Error_State Pwm_Init(Pwm_Cfg_s_t *Pwm_Cfg);                         */
/* Type: public                                                                  */
/* Input parameters: Pwm_Cfg Structure (channel number, Prescaller)              */
/* Return type : void                                                            */
/*                                                                               */
/* Description: initialize the PWM configuration                                 */
/*********************************************************************************/
ERROR_STATUS Pwm_Init(Pwm_Cfg_s_t *Pwm_Cfg)
{

	TCCR1A = LOW; //initialize the value of TCCR1A REG by Zero
	TCCR1B = LOW; //initialize the value of TCCR1B REG by Zero
	ICR1 = LOW;
	//select Timer1 Phase Correct PWM
	CLR_BIT(TCCR1A,BIT0); //WGM10 = 0;
	SET_BIT(TCCR1A,BIT1); //WGM11 = 1;
	CLR_BIT(TCCR1B,BIT3); //WGM12 = 0;
	SET_BIT(TCCR1B,BIT4); //WGH13 =1;

	//Clear OC1A and OC1B when match occurs (non inverted mode)
	CLR_BIT(TCCR1A,BIT6); //COM1A0 = 0;
	SET_BIT(TCCR1A,BIT7); //COM1A1 = 1;
	CLR_BIT(TCCR1A,BIT4); //COM1B0 = 0;
	SET_BIT(TCCR1A,BIT5); //COM1B1 = 1;


	switch(Pwm_Cfg->channel)
	{
	case PWM_CH1A:

		//CLR_BIT(PORTD_DIR,BIT4); // SWITCH OFF OC1B
		SET_BIT(PORTD_DIR,BIT5);  // set OC1A as OUTPUT
		break;

	case PWM_CH1B:
		//CLR_BIT(PORTD_DIR,BIT5); // SWITCH OFF OC1A
		SET_BIT(PORTD_DIR,BIT4); // set OC1B as OUTPUT
		break;
	case PWM_CHAB:
		SET_BIT(PORTD_DIR,BIT4);
		SET_BIT(PORTD_DIR,BIT5);
		break;
	default :
		return E_NOK;
		break;

	}
	switch(Pwm_Cfg->prescaler)
	{
	case PWM_PRESCALER_NO:
		gu8_PWM1_PreScaller = PWM_PRESCALER_NO;
		gu8_PWM1_PreScaller_Value =PRESCALER_NO;
		break;
	case PWM_PRESCALER_8:
		gu8_PWM1_PreScaller = PWM_PRESCALER_8;
		gu8_PWM1_PreScaller_Value =PRESCALER_8;
		break;
	case PWM_PRESCALER_64:
		gu8_PWM1_PreScaller = PWM_PRESCALER_64;
		gu8_PWM1_PreScaller_Value=PRESCALER_64;

		break;
	case PWM_PRESCALER_256:
		gu8_PWM1_PreScaller = PWM_PRESCALER_256;
		gu8_PWM1_PreScaller_Value =PRESCALER_256;

		break;
	case PWM_PRESCALER_1024:
		gu8_PWM1_PreScaller = PWM_PRESCALER_1024;
		gu8_PWM1_PreScaller_Value =PRESCALER_1024;

		break;
	default:
		return E_NOK;
		break;
	}
	return E_OK;
}
/*********************************************************************************/
/* Function: Error_State Pwm_Start(uint8_t channel,uint8_t Duty);                */
/* Type: public                                                                  */
/* Input parameters: Channel ID, Duty cycle(0:100)                               */
/* Return type : Error State                                                     */
/*                                                                               */
/* Description: starts the PWM on the dedicated channel with the required duty   */
/*				cycle and frequency                                              */
/*********************************************************************************/
ERROR_STATUS Pwm_Start(uint8 channel,uint8 Duty,uint32 Frequncy)
{
	switch(channel)
	{
	case PWM_CH1A:
		//calculate the top value to be loaded in ICR1
		gu8_PWM1_CH1A_Frequency =  ((FREQUENCY)/(FREQFACTOR*gu8_PWM1_PreScaller_Value*Frequncy));
		ICR1 = gu8_PWM1_CH1A_Frequency;
		//calculate the value to be loaded in OCR1A
		gu8_PWM1_CH1A_DutyCycle = ((Duty*gu8_PWM1_CH1A_Frequency)/DUTYFACTOR);
		OCR1A = gu8_PWM1_CH1A_DutyCycle;
		//OCR1B = LOW;
		break;
	case PWM_CH1B:
		//calculate the top value to be loaded in ICR1
		gu8_PWM1_CH1B_Frequency = ((FREQUENCY)/(FREQFACTOR*gu8_PWM1_PreScaller_Value*Frequncy));
		ICR1 =gu8_PWM1_CH1B_Frequency;
		//calculate the value to be loaded in OCR1A
		gu8_PWM1_CH1B_DutyCycle = ((Duty*gu8_PWM1_CH1B_Frequency)/DUTYFACTOR);
		OCR1B = gu8_PWM1_CH1B_DutyCycle;
		//OCR1A = LOW;
		break;
	case PWM_CHAB:
		//calculate the top value to be loaded in ICR1
		gu8_PWM1_CHAB_Frequency = ((FREQUENCY)/(FREQFACTOR*gu8_PWM1_PreScaller_Value*Frequncy));
		ICR1 =gu8_PWM1_CHAB_Frequency;
		//calculate the value to be loaded in OCR1A
		gu8_PWM1_CHAB_DutyCycle = ((Duty*gu8_PWM1_CHAB_Frequency)/DUTYFACTOR);
		OCR1A = gu8_PWM1_CHAB_DutyCycle;
		OCR1B = gu8_PWM1_CHAB_DutyCycle;
		break;
	default:
		return E_NOK;
		break;
	}
	TCCR1B |= gu8_PWM1_PreScaller;


	return E_OK;
}
/*********************************************************************************/
/* Function: Error_State Pwm_Start(uint8_t channel,uint8_t Duty);                */
/* Type: public                                                                  */
/* Input parameters: Channel ID, Duty cycle(0:100)                               */
/* Return type : Error State                                                     */
/*                                                                               */
/* Description: updates the duty cycle and frequency of the dedicated channel    */
/*********************************************************************************/
ERROR_STATUS Pwm_Update(uint8 channel,uint8 Duty,uint32 Frequncy)
{
	switch(channel)
	{
	case PWM_CH1A:
		//calculate the top value to be loaded in ICR1
		gu8_PWM1_CH1A_Frequency = ((FREQUENCY)/(FREQFACTOR*gu8_PWM1_PreScaller*Frequncy));
		ICR1 = gu8_PWM1_CH1A_Frequency;
		//calculate the value to be loaded in OCR1A
		gu8_PWM1_CH1A_DutyCycle = ((Duty*gu8_PWM1_CH1A_Frequency)/DUTYFACTOR);
		OCR1A = gu8_PWM1_CH1A_DutyCycle;
		break;
	case PWM_CH1B:
		//calculate the top value to be loaded in ICR1
		gu8_PWM1_CH1B_Frequency = ((FREQUENCY)/(FREQFACTOR*gu8_PWM1_PreScaller*Frequncy));
		ICR1 = gu8_PWM1_CH1B_Frequency;
		//calculate the value to be loaded in OCR1A
		gu8_PWM1_CH1B_DutyCycle = ((Duty*gu8_PWM1_CH1B_Frequency)/DUTYFACTOR);
		OCR1B = gu8_PWM1_CH1B_DutyCycle;
		break;
	case PWM_CHAB:
		//calculate the top value to be loaded in ICR1
		gu8_PWM1_CHAB_Frequency = ((FREQUENCY)/(FREQFACTOR*gu8_PWM1_PreScaller_Value*Frequncy));
		ICR1 =gu8_PWM1_CHAB_Frequency;
		//calculate the value to be loaded in OCR1A
		gu8_PWM1_CHAB_DutyCycle = ((Duty*gu8_PWM1_CHAB_Frequency)/DUTYFACTOR);
		OCR1A = gu8_PWM1_CHAB_DutyCycle;
		OCR1B = gu8_PWM1_CHAB_DutyCycle;
		break;
	default:
		return E_NOK;
		break;
	}

	return E_OK;
}

/*********************************************************************************/
/* Function: Error_State Pwm_Stop(uint8_t channel);                              */
/* Type: public                                                                  */
/* Input parameters: channel ID                                                  */
/* Return type : Error state                                                     */
/*                                                                               */
/* Description: responsible of Stopping the PWM by clearing the prescaler		 */
/*				of the corresponding channel                                     */
/*********************************************************************************/
ERROR_STATUS Pwm_Stop(uint8 channel)
{
	TCCR1B = OFF;
	return E_OK;
}


