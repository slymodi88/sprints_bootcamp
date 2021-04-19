#include "DIO1.h"
#include "common_macros.h"
#include "interrupt.h"
#include "ExternalInterrupt.h"
#include "car_sm.h"
#include "motor.h"
#include "Steering.h"
#include "ICU.h"
#include "std_types.h"
#include "PWM.h"
#include "char_lcd.h"
#include "util/delay.h"
#include "Timer1.h"
#include "registers.h"

uint16 xx=0;
uint8 au8_lcdBuffer[16];


int main()
{

	sei();
	Icu_cfg_s Icu_Cfg;
	Icu_Cfg.ICU_Ch_No = ICU_CH2;
	Icu_Cfg.ICU_Ch_Timer = ICU_TIMER_CH0;
	Icu_Init(&Icu_Cfg);

	Pwm_Cfg_s_t pwm;
	pwm.channel = PWM_CH1A;
	pwm.prescaler = PWM_PRESCALER_64;
	Pwm_Init(&pwm);
	Pwm_Start(PWM_CH1A,50,500);
	LCD_init();
	PORTD_DIR |= 0X01;
	//Timer_Start(TIMER_CH0,0);


/*	Timer_cfg_s Timer_Cfg;
	Timer_Cfg.Timer_CH_NO = TIMER_CH0;
		Timer_Cfg.Timer_Mode = TIMER_MODE;
		Timer_Cfg.Timer_Polling_Or_Interrupt = TIMER_INTERRUPT_MODE;
		Timer_Cfg.Timer_Prescaler = TIMER_PRESCALER_1024;
	  // Timer_Cfg.CBK_Ptr = toggle;
		Timer_Init(&Timer_Cfg);
        Timer_Start(0,255);*/

	while(1)
	{

		sprintf(au8_lcdBuffer, "ss  ");
		Icu_RiseToFall(&xx);
		sprintf(au8_lcdBuffer, "%d CM  ", xx);
		LCD_goto_xy(1, 0);
		LCD_send_string(au8_lcdBuffer);
		_delay_ms(100);


		//	Car_SM_Update();

	}


	return 0;
}
