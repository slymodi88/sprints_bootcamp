/*
 * timers.c
 *
 *  Created on: Dec 8, 2019
 *      Author: memo1
 */
#include "timers.h"
#include "gpio.h"
#include "registers.h"
#include "std_Types.h"
#include "MotorDC.h"
static uint8 T1_Prescaler;
static uint8 T1_mode;
static uint8 T0_mode;
static uint8 T0_Prescaler;
uint16 tcntmax;
uint16 vlaue_ocra1;
void timer0Init(En_timer0Mode_t mode,En_timer0OC_t OC0,En_timer0perscaler_t prescal, uint8 initialValue, uint8 outputCompare, En_timer1Interrupt_t interruptMask)
{
	T0_mode = mode;
	TCCR0 |= (mode|OC0);

		T0_Prescaler = prescal;
		TCNT0 = initialValue;
		TIMSK|= interruptMask;
		OCR0 = outputCompare;


}
void timer0Set(uint8 value)
{
	TCNT0=value;
}
uint8 timer0Read(void)
{
	return TCNT0;

}
void timer0Start(void)
{
	TCCR0 |=T0_Prescaler;

}
void timer0Stop(void)
{
	TCCR0 &= ~((1<<2)|(1<<1)|(1<<0));

}
void timer0Delay_ms(uint16 delay)
{
	uint16 i;
	switch(T0_mode)
			{
			case T0_NORMAL_MODE :
				timer0Start();
				for( i=0;i<delay;i++)
				{
					timer0Set(230);
					while(! (TIFR & (1<<0)));
					TIFR|=(1<<0);
				}
				timer0Stop();
				break;
			case T0_COMP_MODE :
				OCR0 = 26;
				timer0Start();
				for(uint16 i=0;i<delay;i++)
					{
						while(!(TIFR&(1<<1)));
						TIFR|=(1<<1);
					}
					timer0Stop();
					break;
			default:
				break;

			}


}
void timer0SwPWM(uint8 dutyCycle,uint8 freq)
{

}

void timer1Init(En_timer1Mode_t mode,En_timer1OC_t OC,En_timer1perscaler_t prescal, uint16 initialValue, uint16 outputCompareA, uint16 outputCompareB,uint16 inputCapture, En_timer2Interrupt_t interruptMask)
{
	T1_mode = mode;
	switch(mode)
	{
	case T1_NORMAL_MODE :
		TCCR1A|=T1_NORMAL_MODE;
		break;
	case T1_COMP_MODE_OCR1A_TOP :
		TCCR1B|=T1_COMP_MODE_OCR1A_TOP;
			break;
	case T1_COMP_MODE_ICR1_TOP :
		TCCR1|=T1_COMP_MODE_ICR1_TOP;
			break;
	default:
		break;

	}
	T1_Prescaler = prescal;
	TCNT1 = initialValue;
	OCR1A =outputCompareA;
	OCR1B =outputCompareB;
	ICR1 = inputCapture;
	TIMSK|= interruptMask;

}
void timer1Set(uint16 value)
{
	TCNT1 = value;

}
uint16 timer1Read(void)
{
	return TCNT1;
}
void timer1Start(void)
{
	TCCR1 |= T1_Prescaler;

}
void timer1Stop(void)
{
	TCCR1 &= ~((1<<2)|(1<<1)|(1<<0));

}
void timer1Delay_ms(uint16 delay)
{
	switch(T1_mode)
				{
				case T1_NORMAL_MODE :
					timer1Start();
					for( uint16 i=0;i<delay;i++)
					{
						timer1Set(65508);
						while(! (TIFR & (1<<2)));
						TIFR|=(1<<2);
					}
					timer1Stop();
					break;
				case T1_COMP_MODE_OCR1A_TOP :
					OCR1A = 27;
					timer1Start();
					for(uint16 i=0;i<delay;i++)
						{
							while(!(TIFR&(1<<4)));
							TIFR|=(1<<4);
						}
						timer1Stop();
						break;
				case T1_COMP_MODE_ICR1_TOP :
									OCR1A = 27;
									timer1Start();
									for(uint16 i=0;i<delay;i++)
										{
											while(!(TIFR&(1<<5)));
											TIFR|=(1<<5);
										}
										timer1Stop();
										break;
				default:
					break;

				}




}
void timer1SwPWM(uint8 dutyCycle,uint8 freq)
{
	uint16 tcnt = ((8000000)/(freq));
    tcntmax = (65535-tcnt);
	vlaue_ocra1 = (tcnt*(float)dutyCycle/100.0)+tcntmax;


	//while(!(TIFR&(1<<4)));
   // TIFR|=(1<<4);
   // gpioPinWrite(MOTOR_EN_1_GPIO, MOTOR_EN_1_BIT|MOTOR_EN_2_BIT, LOW);
	//while(!(TIFR&(1<<2)));
    // TIFR|=(1<<2);
   //  gpioPinWrite(MOTOR_EN_2_GPIO, MOTOR_EN_1_BIT|MOTOR_EN_2_BIT, HIGH);

}

