/*
 * SwICU.c
 *
 *  Created on: Dec 15, 2019
 *      Author: TOSHIBA.SXO11
 */
#include "SwICU.h"

/*ICU with external interrupt 2*/

/*
 * Initializing ICU by enabling ICU interrupt
 * params : inputCaptureEdge : The edge of the ultrasonic
 * 			-SwICU_EdgeFalling
 * 			-SwICU_EdgeRisiging
 */

void SwICU_Init(SwICU_Edge_TypeDef inputCaptureEdge){
	timer0Init(T0_NORMAL_MODE,T0_OC0_DIS,T0_PRESCALER_1024,0,0,T0_POLLING);
	PORTB_DIR &= ~(1<<2);
	GICR |= (1<<5);
	switch(inputCaptureEdge){
		case SwICU_EdgeFalling :
			MCUCSR &= ~(1<<6);
			break;
		case SwICU_EdgeRisiging :
			MCUCSR |= (1<<6);
			break;
	}
	sei();
}
SwICU_Edge_TypeDef SwICU_GetCfgEdge(void){
	SwICU_Edge_TypeDef au8_edge;
	if( (MCUCSR & (1<<6)) != 0 ){
		au8_edge = SwICU_EdgeRisiging;
	}else{
		au8_edge = SwICU_EdgeFalling;
	}
	return au8_edge;
}
void SwICU_SetCfgEdge(SwICU_Edge_TypeDef edge){
	switch(edge){
		case SwICU_EdgeFalling :
			MCUCSR &= ~(1<<6);
			break;
		case SwICU_EdgeRisiging :
			MCUCSR |= (1<<6);
			break;
	}
}
void SwICU_Read(volatile uint8 * capt){
	gpioPinWrite(GPIOB,BIT3,HIGH);
	_delay_us(10);
	gpioPinWrite(GPIOB,BIT3,LOW);
	*capt = TCNT0;
}
void SwICU_Stop(void){
	TCCR0 = 0;
}
void SwICU_Start(void){
	TCNT0 = 0;
	TCCR0 = 0x05;
}
void SwICU_Enable(void){
	GICR |= (1<<5);
}
void SwICU_Disable(void){
	GICR &= ~(1<<5);
}
