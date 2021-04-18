/*
 * softwareDelay.c
 *
 *  Created on: Dec 3, 2019
 *      Author: TOSHIBA.SXO11
 */
#include "registers.h"
#include "softwareDelay.h"

/**
 * Description: this delay function operate in a polling manner
 * 				don't use it with RTOSs
 * @param n: the milli-seconds
 */
void SwDelay_ms(uint32 n){
	uint32 timeCounter; /*the counter of the for loop*/
	for(timeCounter=0;timeCounter<=n*900;timeCounter++);
}
