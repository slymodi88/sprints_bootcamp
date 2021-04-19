/*
 * car_sm.c
 *
 *  Created on: Dec 25, 2019
 *      Author: memo1
 */
#include "car_sm.h"

#define		CAR_STOP		0
#define		CAR_FORWARD		1
#define		CAR_BACKWARD    2
#define		CAR_RIGHT		3
#define		CAR_LEFT		4
#define     CAR_FORWARD_80  5
#define     CAR_FORWARD_30  6

static uint8 g_Car_State = CAR_STOP ;
static uint16 g_Us_Reading = 0;
/*
 * Fun----------: ERROR_STATUS Car_SM_Init(void);
 * Input--------: Nothing
 * Output-------: Nothing
 * Return-------: ERROR_STATUES
 * -------------- #E_Ok	->function operation is okay, no error
 * -------------- #E_Nok->function operation failed, error exists
 * Description--: Initiate the car state machine with state"forward at speed 80%",
 * -------------- And initiate steering and ultrasonic functions
 */
ERROR_STATUS Car_SM_Init(void)
{
	ERROR_STATUS Check_US_Init=E_NOK;
	ERROR_STATUS Check_Steering_Init=E_NOK;

	Check_US_Init = Us_Init();
	Check_Steering_Init	= Steering_Init();
	if(E_OK == Check_US_Init && E_OK ==  Check_Steering_Init)
	{
		return E_OK;
	}
	else
	{
		return E_NOK;

	}


}

/*
 * Fun----------: ERROR_STATUS Car_SM_Update(void);
 * Input--------: Nothing
 * Output-------: Nothing
 * Return-------: ERROR_STATUES
 * -------------- #E_Ok	->function operation is okay, no error
 * -------------- #E_Nok->function operation faild, error exists
 * Description--: changes car state according to the ultrasonic input
 * -------------- And represents the motors output for each state
 */
ERROR_STATUS Car_SM_Update(void)
{

	Us_GetDistance(&g_Us_Reading);
	switch(g_Car_State)
	{
	case CAR_STOP:
		Steering_SteerCar(CAR_STOP, 0);
		break;
	case CAR_FORWARD_80:
		Steering_SteerCar(CAR_FORWARD, 80);
		break;
	case CAR_FORWARD_30:
		Steering_SteerCar(CAR_FORWARD, 30);
		break;
	case CAR_RIGHT:
		Steering_SteerCar(CAR_RIGHT, 30);
		break;
	case CAR_BACKWARD:
		Steering_SteerCar(CAR_BACKWARD, 30);
		break;
	default:
		return E_NOK;
		break;
	}
	if(g_Us_Reading >= 50){
		g_Car_State = CAR_FORWARD_80;
		}
		else if((g_Us_Reading > 30 )){
			g_Car_State = CAR_FORWARD_30;
		}
		else if((g_Us_Reading == 30 )){
			g_Car_State = CAR_RIGHT;
		}
		else{
			g_Car_State = CAR_BACKWARD;
		}

	return E_OK;

}


