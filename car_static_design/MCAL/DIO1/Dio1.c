/*
 * Dio1.c
 *
 *  Created on: Dec 22, 2019
 *      Author: memo1
 */
#include "../DIO1/DIO1.h"
#include "std_types.h"
#include "registers.h"
/*
 *Input: DIO_Cfg_s -> to get PORT name, pins to be initiated and the required direction
 *Output: No output
 *In/Out:
 *Description: This function can set the direction of a full port, a nibble
 * 			  or even one pin.
 */
ERROR_STATUS DIO_init (DIO_Cfg_s *DIO_info)
{
	if(DIO_info -> dir == HIGH){
		switch(DIO_info -> GPIO){
		case GPIOA:
			PORTA_DIR |= (DIO_info -> pins);
			break;
		case GPIOB:
			PORTB_DIR |= (DIO_info -> pins);
			break;
		case GPIOC:
			PORTC_DIR |= (DIO_info -> pins);
			break;
		case GPIOD:
			PORTD_DIR |= (DIO_info -> pins);
			break;
		default:
			return E_NOK;
			break;
		}
	} else if(DIO_info -> dir == LOW){
		switch(DIO_info -> GPIO){
		case GPIOA:
			PORTA_DIR &= ~(DIO_info -> pins);
			break;
		case GPIOB:
			PORTB_DIR &= ~(DIO_info -> pins);
			break;
		case GPIOC:
			PORTC_DIR &= ~(DIO_info -> pins);
			break;
		case GPIOD:
			PORTD_DIR &= ~(DIO_info -> pins);
			break;
		default:
			return E_NOK;
			break;
		}
	}

	return E_OK;
}
/*
 *Input: GPIO -> to get PORT name
 *					- GPIOA
 *					- GPIOB
 *					- GPIOC
 *					- GPIOD
 *		pins -> pins to be written at.
 *					- PIN0
 *					- PIN1
 *					- PIN2
 *					- PIN3
 *					- PIN4
 *					- PIN5
 *					- PIN6
 *					- PIN7
 *					- UPPER_NIBBLE
 *					- LOWER_NIBBLE
 *					- FULL_PORT
 *		value 	-> The desired value
 *Output: No output
 *In/Out: No In/Out
 *Description: This function can set the value of a full port, a nibble
 * 			  or even one pin.
 */
ERROR_STATUS DIO_Write (uint8 GPIO, uint8 pins, uint8 value)
{
	if(value == HIGH){
		switch(GPIO){
		case GPIOA:
			PORTA_DATA |= (pins);
			break;
		case GPIOB:
			PORTB_DATA |= (pins);
			break;
		case GPIOC:
			PORTC_DATA |= (pins);
			break;
		case GPIOD:
			PORTD_DATA |= (pins);
			break;
		default:
			return E_NOK;
			break;
		}
	}else if(value==LOW){
		switch(GPIO){
		case GPIOA:
			PORTA_DATA &= ~(pins);
			break;
		case GPIOB:
			PORTB_DATA &= ~(pins);
			break;
		case GPIOC:
			PORTC_DATA &= ~(pins);
			break;
		case GPIOD:
			PORTD_DATA &= ~(pins);
			break;
		default:
			return E_NOK;
			break;
		}
	}

	return E_OK;
}
/*
*Input: GPIO -> to get PORT name
*					- GPIOA
*					- GPIOB
*					- GPIOC
*					- GPIOD
*		pins -> pins to be written at.
*					- PIN0
*					- PIN1
*					- PIN2
*					- PIN3
*					- PIN4
*					- PIN5
*					- PIN6
*					- PIN7
*					- UPPER_NIBBLE
*					- LOWER_NIBBLE
*					- FULL_PORT
*Output: data -> the acquired data wether it was PORT data or pins data
*In/Out:
*Description: This function gets the value of a full port, a nibble
* 			  or even one pin.
*/
ERROR_STATUS DIO_Read (uint8 GPIO,uint8 pins, uint8 *data)
{
	switch(GPIO){
			case GPIOA:
				*data = PORTA_PIN & pins;
				break;
			case GPIOB:
				*data = PORTB_PIN & pins;
				break;
			case GPIOC:
				*data = PORTC_PIN & pins;
				break;
			case GPIOD:
				*data = PORTD_PIN & pins;
				break;
			default :
				return E_NOK;
				break;
		}
	return E_OK;
}
/*
*Input: GPIO -> to get PORT name
*					- GPIOA
*					- GPIOB
*					- GPIOC
*					- GPIOD
*		pins -> pins to be written at.
*					- PIN0
*					- PIN1
*					- PIN2
*					- PIN3
*					- PIN4
*					- PIN5
*					- PIN6
*					- PIN7
*					- UPPER_NIBBLE
*					- LOWER_NIBBLE
*					- FULL_PORT
*Output: data -> No output
*In/Out:
*Description: This function toggles the value of a full port, a nibble
* 			  or even one pin.
*/
ERROR_STATUS DIO_Toggle (uint8 GPIO, uint8 pins)
{
	switch(GPIO){
			case GPIOA:
				PORTA_DATA ^= pins;
				break;
			case GPIOB:
				PORTB_DATA ^= pins;
				break;
			case GPIOC:
				PORTC_DATA ^= pins;
				break;
			case GPIOD:
				PORTD_DATA ^= pins;
				break;
		}
	return E_OK;
}


