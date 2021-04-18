/*

 * HwPWM.c
 *
 *  Created on: Dec 15, 2019
 *      Author: memo1
 */
#include "HwPWM.h"
#include"registers.h"
static uint16 gTopValue;
static uint16 gDutyCycle;
void HwPWM_Init(void)
{
	//select Timer1 Phase Correct PWM
	TCCR1A &= ~(1<<0); // WGM10 = 0;
	TCCR1A |= (1<<1);  //WGM11 = 1;
	TCCR1B &= ~(1<<3); //WGM12 = 0;
	TCCR1B |= (1<<4);  //WGM13 = 1;
	//Clear OC1A and OC1B when match occurs (non inverted mode)
	TCCR1A &= ~(1<<6); //COM1A0 =0
	TCCR1A |= (1<<7);  //COM1A1 =1
	TCCR1A &= ~(1<<4); //COM1B0 =0
	TCCR1A |= (1<<5);  //COM1B1 =1;
	//64 PRESCALER
	TCCR1B |= (1<<0); //CS10=1
	TCCR1B |= (1<<1); //CS11=1
	TCCR1B &= ~(1<<2); //CS12=0
	//set OC1A and OC1B as output
	PORTD_DIR = PORTD_DIR |(1<<4);
	PORTD_DIR = PORTD_DIR |(1<<5);

}

void HwPWM_ChangeWidth(uint8 duty, uint32 frequency)
{
	//calculate the top value to be loaded in ICR1
	gTopValue = ((8000000)/(2*64*frequency));
	ICR1 = gTopValue;
	//calculate the value to be loaded in OCR1A
	gDutyCycle = ((duty*gTopValue)/100);
	OCR1A = gDutyCycle;
	OCR1B = gDutyCycle;
}
