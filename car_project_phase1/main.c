/*
 * main.c
 *
 *  Created on: Dec 3, 2019
 *      Author: memo1
 */
#include "gpio.h"
#include "softwareDelay.h"
#include "led.h"
#include "req_led.h"
#include "pushButton.h"
#include "interrupt.h"
#include "timers.h"
#include "MotorDC.h"
#include  "interrupt.h"
#include "registers.h"
#include "HwPWM.h"
/*
extern uint16 tcntmax;
extern uint16 vlaue_ocra1;
int main(){



	timer1Init(T1_NORMAL_MODE,T1_OC1_DIS,T1_PRESCALER_NO,0,0,0,0,(T1_INTERRUPT_NORMAL|T1_INTERRUPT_CMP_1A));


	MotorDC_Init(MOT_1);
	MotorDC_Init(MOT_2);
	MotorDC_Dir(MOT_1, FORWARD);
	MotorDC_Dir(MOT_2, FORWARD);
	timer1Start();
	sei();
	while(1)
	{

		//State_led();

		uint8 i=0;
		for(i=1;i<99;i++){
			timer1SwPWM(i,250);
			for( uint32  TimeCounter=0;TimeCounter<=10000;TimeCounter++){
				asm("nop");
			}
		}

		for(i=99;i>1;i--){

			timer1SwPWM(i,250);
			for( uint32  TimeCounter=0;TimeCounter<=10000;TimeCounter++){
				asm("nop");
			}
		}
		MotorDC_Dir(MOT_1, FORWARD);
		MotorDC_Dir(MOT_2, STOP);
		for(i=1;i<99;i++){

			timer1SwPWM(i,250);
			for( uint32  TimeCounter=0;TimeCounter<=2000;TimeCounter++){
				asm("nop");
			}

		}
		MotorDC_Dir(MOT_1, STOP);
		MotorDC_Dir(MOT_2, STOP);
	    timer1Stop();

	}
	return 0;
}
ISR(TIMER1_OVF_vect){
	gpioPinWrite(MOTOR_EN_1_GPIO, MOTOR_EN_1_BIT|MOTOR_EN_2_BIT, HIGH);
	TCNT1=tcntmax;

}
ISR(TIMER1_COMPA_vect){
	gpioPinWrite(MOTOR_EN_2_GPIO, MOTOR_EN_1_BIT|MOTOR_EN_2_BIT, LOW);
	OCR1A = vlaue_ocra1;
} */

int main()
{
	HwPWM_Init();
	MotorDC_Init(MOT_1);
	MotorDC_Init(MOT_2);
	MotorDC_Dir(MOT_1, FORWARD);
	MotorDC_Dir(MOT_2, FORWARD);
	PORTD_DATA = PORTD_DATA | ((1<<4));
	PORTD_DATA = PORTD_DATA |((1<<5));
	while(1){
		uint8 i=0;
		MotorDC_Dir(MOT_1, FORWARD);
		MotorDC_Dir(MOT_2, FORWARD);
		for( i=1;i<99;i++){
			HwPWM_ChangeWidth(i,1000);
			for( uint32  TimeCounter=0;TimeCounter<=10000;TimeCounter++){
				asm("nop");
			}
		}
		for( i=99;i>1;i--){

			HwPWM_ChangeWidth(i,1000);
			for( uint32  TimeCounter=0;TimeCounter<=10000;TimeCounter++){
				asm("nop");
			}
		}
		MotorDC_Dir(MOT_1, FORWARD );
	    MotorDC_Dir(MOT_2, STOP);
		for( i=1;i<99;i++){

			HwPWM_ChangeWidth(i,1000);
			for( uint32  TimeCounter=0;TimeCounter<=2000;TimeCounter++){
				asm("nop");
			}

		}


	}
}

