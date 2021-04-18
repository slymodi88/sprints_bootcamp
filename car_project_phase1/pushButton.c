/*
 * pushButton.c
 *
 *  Created on: Dec 6, 2019
 *      Author: TOSHIBA.SXO11
 */

#include "pushButton.h"

static En_buttonStatus_t States[4];
/**
 * Description: Initialize the BTN_x Pin state (where x 0, 1, 2, 3) to Input
 * @param btn_id: The btn to be initialized and it takes
 * 				  one of the enum (En_buttonId) parameters
 *
 */
void pushButton_Init(En_buttonId btn_id){
	switch(btn_id){
		case BTN_0:
			gpioPinDirection(BTN_0_GPIO,BTN_0_BIT,INPUT);
			break;
		case BTN_1:
			gpioPinDirection(BTN_1_GPIO,BTN_1_BIT,INPUT);
			break;
		case BTN_2:
			gpioPinDirection(BTN_2_GPIO,BTN_2_BIT,INPUT);
			break;
		case BTN_3:
			gpioPinDirection(BTN_3_GPIO,BTN_3_BIT,INPUT);
			break;
	}
}
/**
 * Description: read all BTN_x (where x 0, 1, 2, 3) states and store it in the program
 *
 * @note : this function must be called in the system tick hundler or in the super loop hundler
 */
void pushButton_Update(void){
	if(gpioPinRead(BTN_0_GPIO,BTN_0_BIT)!= 0){
		SwDelay_ms(10);
		States[BTN_0] = Pressed;
	}else{
		States[BTN_0] = Released;
	}
	if(gpioPinRead(BTN_1_GPIO,BTN_1_BIT)!= 0){
		SwDelay_ms(10);
		States[BTN_1] = Pressed;
	}else{
		States[BTN_1] = Released;
	}
	if(gpioPinRead(BTN_2_GPIO,BTN_2_BIT)!= 0){
		SwDelay_ms(10);
		States[BTN_2] = Pressed;
	}else{
		States[BTN_2] = Released;
	}
	if(gpioPinRead(BTN_3_GPIO,BTN_3_BIT)!= 0){
		SwDelay_ms(10);
		States[BTN_3] = Pressed;
	}else{
		States[BTN_3] = Released;
	}
}
/**
 * Description: read BTN_x (where x 0, 1, 2, 3) state which is stored in the program
 * 				in the update function
 *
 */
En_buttonStatus_t pushButton_GetStatus(En_buttonId btn_id){
	pushButton_Update();
	return States[btn_id];
}
