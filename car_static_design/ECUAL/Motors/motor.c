/*
 * motor.c
 *
 *  Created on: Dec 23, 2019
 *      Author: memo1
 */
#include "motor.h"

#include "DIO1.h"
#include "PWM.h"
#define MOTOR_EN_1_GPIO		(GPIOD)
#define MOTOR_EN_1_BIT		(BIT4)

#define MOTOR_OUT_1A_GPIO	(GPIOD)
#define MOTOR_OUT_1B_GPIO	(GPIOD)
#define MOTOR_OUT_1A_BIT	(BIT2)
#define MOTOR_OUT_1B_BIT	(BIT3)


#define MOTOR_EN_2_GPIO		(GPIOD)
#define MOTOR_EN_2_BIT		(BIT5)

#define MOTOR_OUT_2A_GPIO	(GPIOD)
#define MOTOR_OUT_2B_GPIO	(GPIOD)
#define MOTOR_OUT_2A_BIT	(BIT6)
#define MOTOR_OUT_2B_BIT	(BIT7)
#define INITIALIZED            (1)
#define NOT_INITIALIZED            (0)

#define FREQUNCY             1000


static uint8 gu8_MOTOR1_CheckFor_Init =0;
static uint8 gu8_MOTOR2_CheckFor_Init =0;
DIO_Cfg_s MOTOR_init;

/*********************************************
 *	  	   Function prototypes				 *
 ********************************************/

/**
 * Func			: Motor_Init
 * Input		: motorNumber
 *	  	  		  #def
 *	  	  		  number of motor (MOTOR_1, MOTOR_2)
 * output		: none
 * In out		: none
 * Return		: ERROR_STATUS
 * Description	: Initialize (Enable, IN1, IN2) pins as digital outputs.
 */
ERROR_STATUS Motor_Init(uint8 Motor_Number)
{

	Pwm_Cfg_s_t pwm;
	pwm.channel = PWM_CHAB;
	pwm.prescaler = PWM_PRESCALER_64;
	Pwm_Init(&pwm);
	switch(Motor_Number){
	case MOTOR_1:
		MOTOR_init.GPIO = MOTOR_EN_1_GPIO;
		MOTOR_init.dir = HIGH;
		MOTOR_init.pins = (MOTOR_EN_1_BIT|MOTOR_OUT_1A_BIT|MOTOR_OUT_1B_BIT);
		DIO_init(& MOTOR_init);
		gu8_MOTOR1_CheckFor_Init = INITIALIZED;
		break;
	case MOTOR_2:
		MOTOR_init.GPIO = MOTOR_EN_2_GPIO;
		MOTOR_init.dir = HIGH;
		MOTOR_init.pins = (MOTOR_EN_2_BIT|MOTOR_OUT_2A_BIT|MOTOR_OUT_2B_BIT);
		DIO_init(& MOTOR_init);
		gu8_MOTOR1_CheckFor_Init = INITIALIZED;
		break;
	default :
		return E_NOK;
		break;
	}
	return E_OK;
}
/**
 * Fun		 : Motor_Direction
 * input	 : @param Motor_Number
			   #define
		       MOTOR_1, MOTOR_2

			   @param Motor_Direction
			   #define
			   Stop, Forward, Backword
 * output  	 : no output
 * return 	 : ERROR_STATUS
 * Descriptio: Controls the motor direction from getting the motor number and the direction.
 */
ERROR_STATUS Motor_Direction(uint8 Motor_Number, uint8 Motor_Direction)
{
	switch(Motor_Number){
	case MOTOR_1:
		switch(Motor_Direction){
		case MOTOR_FORWARD:
			DIO_Write (MOTOR_OUT_1A_GPIO, MOTOR_OUT_1A_BIT, HIGH);
			DIO_Write (MOTOR_OUT_1B_GPIO, MOTOR_OUT_1B_BIT, LOW);
			break;
		case MOTOR_BACKWORD:
			DIO_Write (MOTOR_OUT_1A_GPIO, MOTOR_OUT_1A_BIT, LOW);
			DIO_Write (MOTOR_OUT_1B_GPIO, MOTOR_OUT_1B_BIT, HIGH);
			break;
		default :
			return E_NOK;
			break;
		}
		break;
		case MOTOR_2:
			switch(Motor_Direction){
			case MOTOR_FORWARD:
				DIO_Write (MOTOR_OUT_2A_GPIO, MOTOR_OUT_2A_BIT, HIGH);
				DIO_Write (MOTOR_OUT_2B_GPIO, MOTOR_OUT_2B_BIT, LOW);
				break;
			case MOTOR_BACKWORD:
				DIO_Write (MOTOR_OUT_2A_GPIO, MOTOR_OUT_2A_BIT, LOW);
				DIO_Write (MOTOR_OUT_2B_GPIO, MOTOR_OUT_2B_BIT, HIGH);
				break;
			default :
				return E_NOK;
				break;
			}
			break;
			default :
				return E_NOK;
				break;
	}
	return E_OK;

}
/**
 * Fun		 : Motor_Start
 * input	 : @param Motor_Number
			   #define
		       MOTOR_1, MOTOR_2
			   @param Mot_Speed
			   set the motor speed
 * output  	 : no output
 * return 	 : ERROR_STATUS
 * Descriptio: Start the motor.
 */
ERROR_STATUS Motor_Start(uint8 Motor_Number, uint8 Mot_Speed)
{
	switch(Motor_Number)
	{
	case MOTOR_1:
		Pwm_Start(PWM_CH1A,Mot_Speed,FREQUNCY);
		break;
	case MOTOR_2:
		Pwm_Start(PWM_CH1B,Mot_Speed,FREQUNCY);
		break;
	default:
		return E_NOK;
		break;
	}
	return E_OK;

}
/**
 * Fun		 : Motor_SpeedUpdate
 * input	 : @param Motor_Number
			   #define
		       MOTOR_1, MOTOR_2
			   @param Mot_Speed
			   set the motor speed
 * output  	 : no output
 * return 	 : ERROR_STATUS
 * Descriptio: Controls the motor speed from getting the motor number and the speed.
 */
ERROR_STATUS Motor_SpeedUpdate(uint8 Motor_Number, uint8 Mot_Speed)
{
	switch(Motor_Number)
		{
		case MOTOR_1:
			Pwm_Update(PWM_CH1A,Mot_Speed,FREQUNCY);
			break;
		case MOTOR_2:
			Pwm_Update(PWM_CH1B,Mot_Speed,FREQUNCY);
			break;
		default:
			return E_NOK;
			break;
		}
	return E_OK;

}

/**
 * Fun		 : Motor_Stop
 * input	 : @param Motor_Number
			   #define
		       MOTOR_1, MOTOR_2
 * output  	 : no output
 * return 	 : ERROR_STATUS
 * Descriptio: stop the motor.
 */
ERROR_STATUS Motor_Stop(uint8 Motor_Number)
{
	switch(Motor_Number){
	case MOTOR_1:
		DIO_Write (MOTOR_OUT_1A_GPIO, MOTOR_OUT_1A_BIT, LOW);
		DIO_Write (MOTOR_OUT_1B_GPIO, MOTOR_OUT_1B_BIT, LOW);
		break;
	case MOTOR_2:
		DIO_Write (MOTOR_OUT_2A_GPIO, MOTOR_OUT_2A_BIT, LOW);
		DIO_Write (MOTOR_OUT_2B_GPIO, MOTOR_OUT_2B_BIT, LOW);
		break;
	default :
		return E_NOK;
		break;
	}
	return E_OK;

}

/**
 * Func			: Motor_GetStatus
 * Input		: Motor_Number
 *	  	  		 #def
 *	  	  		 number of motor (MOTOR_1, MOTOR_2)
 * output		: none
 * In out		: none
 * Return		: Initialization_STATUS
 * Description	: Returns status of the motor whether initialized or not
 */
uint8 Motor_GetStatus(uint8 Motor_Number)
{
	uint8 Status = NOT_INITIALIZED;
	switch(Motor_Number){
	case MOTOR_1:
		if(INITIALIZED == gu8_MOTOR1_CheckFor_Init)
		{
			Status = INITIALIZED;

		}
		else
		{
			Status = NOT_INITIALIZED;

		}
		break;
	case MOTOR_2:
		if(INITIALIZED == gu8_MOTOR2_CheckFor_Init)
		{
			Status = INITIALIZED;

		}
		else
		{
			Status = NOT_INITIALIZED;

		}
		break;
	}

	return Status;
}

