/*
 * main.c

 *
 *  Created on: Dec 16, 2019
 *      Author: TOSHIBA.SXO11
 *      Description :
 */

#include <stdio.h>
#include "MotorDC.h"
#include "HwPWM.h"
#include "SwICU.h"
#include "registers.h"
#include "gpio.h"

volatile uint32 gu32_EchoPulse; /*The value of the echo gu32_EchoPulse*/
volatile uint8 gu8_EdgeFlag = 0; /*flag to indicate rising or falling edge of the ultrasonic sensor*/

/*External interrupt ISR to measure the echo gu32_EchoPulse*/
ISR(INT2_vect){
	/*If it is at the rising edge, start timer0 and make TCNT0=0 initially
	 * make the interrupt wait for the falling edge*/
	if((gu8_EdgeFlag == 0)){
		gu8_EdgeFlag = 1;
		TCNT0 = 0;
		TCCR0 = 0x05;
		SwICU_SetCfgEdge(SwICU_EdgeFalling);
		/*If the falling edge, stop timer0 and return the value of TCNT0 to calculate the
		 * echo time in micro second and make the interrupt wait for the next rising edge
		 * Tick time = 128 us*/
	}else if(gu8_EdgeFlag == 1){
		gu8_EdgeFlag = 0;
		TCCR0 = 0;
		gu32_EchoPulse = TCNT0 * 128;
		SwICU_SetCfgEdge(SwICU_EdgeRisiging);
	}
}

int main()
{
	uint16 gu16_ObstacleDistance;	/*the gu16_ObstacleDistance measured by the ultrasonic sensor*/
	gpioPinDirection(GPIOB,BIT3,OUTPUT); /*Enabling Trigger Bit of ultrasonic*/
	SwICU_Init(SwICU_EdgeRisiging); /*Initialize the external interrupt*/
	/*Initialize PWM*/
	HwPWM_Init();
	/*Initialize motors*/
	MotorDC_Init(MOT_1);
	MotorDC_Init(MOT_2);

	while(1){
		/*Give the trigger gu32_EchoPulse*/
		gpioPinWrite(GPIOB,BIT3,HIGH);
		_delay_us(10);
		gpioPinWrite(GPIOB,BIT3,LOW);
		_delay_us(100);
		gu16_ObstacleDistance = gu32_EchoPulse / 58;

		/*Controlling the speed at different gu16_ObstacleDistances for obstacle*/
		if(gu16_ObstacleDistance > 50){
			// Ordinary speed
			MotorDC_Dir(MOT_1, FORWARD);
			MotorDC_Dir(MOT_2, FORWARD);
			MotorDC_Speed_HwPWM(60);

		} else if((gu16_ObstacleDistance > 30)){
			// Decreasing Speed
			MotorDC_Speed_HwPWM(30);
			MotorDC_Dir(MOT_1, FORWARD);
			MotorDC_Dir(MOT_2, FORWARD);

		} else if((gu16_ObstacleDistance == 30)){
			//Rotating
			MotorDC_Dir(MOT_2, BACKWARD );
			MotorDC_Dir(MOT_1, FORWARD );
			HwPWM_ChangeWidth(80,1000);
			_delay_ms(550);

		} else {
			MotorDC_Dir(MOT_1, BACKWARD);
			MotorDC_Dir(MOT_2, BACKWARD);
			HwPWM_ChangeWidth(50,1000);
			_delay_ms(100);
		}

	}
	return 0;
}

