/*
 * Steering.c
 *
 *  Created on: Dec 24, 2019
 *      Author: memo1
 */
#include "Steering.h"
#include "motor.h"
#include "ICU.h"


#define OFF  0
/**
 * Input:
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: Initiates the module.
 *
 */

ERROR_STATUS Steering_Init(void)
{

	//INIT MOTOR1
	Motor_Init(MOTOR_1);
	//INIT MOTOR2
	Motor_Init(MOTOR_2);

	Motor_Start( MOTOR_1, OFF);
	Motor_Start( MOTOR_2, OFF);

	return E_OK;
}
/**
 * Input:
 * 	carDirection: The direction of the car from the predefined states of the module.
 * 	speed: The speed of the car from 0% to 100%.
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function controls the direction and the speed of the car.
 *
 */
ERROR_STATUS Steering_SteerCar(uint8 carDirection, uint8 speed)
{
	switch (carDirection)
	{
	case CAR_STOP :
		Motor_Stop(MOTOR_1);
		Motor_Stop(MOTOR_2);
		break;

	case CAR_FORWARD :
		Motor_Direction(MOTOR_1,MOTOR_FORWARD);
		Motor_Direction(MOTOR_2,MOTOR_FORWARD);
		Motor_SpeedUpdate(MOTOR_1,speed);
		Motor_SpeedUpdate(MOTOR_2,speed);
		break;
	case CAR_BACKWARD :
		Motor_Direction(MOTOR_1,MOTOR_BACKWORD);
		Motor_Direction(MOTOR_2,MOTOR_BACKWORD);
		Motor_SpeedUpdate(MOTOR_1,speed);
		Motor_SpeedUpdate(MOTOR_2,speed);
		break;
	case CAR_RIGHT :
		Motor_Direction(MOTOR_1,MOTOR_FORWARD);
		Motor_Stop(MOTOR_2);
		Motor_SpeedUpdate(MOTOR_1,speed);
		Motor_SpeedUpdate(MOTOR_2,speed);
		break;
	case CAR_LEFT :
		Motor_Direction(MOTOR_2,MOTOR_FORWARD);
		Motor_Stop(MOTOR_1);
		Motor_SpeedUpdate(MOTOR_1,speed);
		Motor_SpeedUpdate(MOTOR_2,speed);
		break;
	default :
		return E_NOK;
		break;
	}

	return E_OK;
}



