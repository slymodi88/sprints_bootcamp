/*
 * BCM.h
 *
 *  Created on: Jan 15, 2020
 *      Author: memo1
 */

#ifndef SERVICELAYER_BCM_BCM_H_
#define SERVICELAYER_BCM_BCM_H_

#include "BCM_Cfg.h"
#include "COMM_UART.h"
/******************************************************************************************
*                                                                                         *
*                               TYPEDEF                                                   *
*																						  *
*																						  *
*******************************************************************************************/
typedef struct
{
	uint8 BCM_Id;
	uint8 Check_Sum_Error;
	uint16 Data_Size;
	uint8 * data_ptr;
} BCM_Buffer_t;

typedef struct
{
	uint8 BCM_Id;
	uint8 Check_Sum_Error;
	uint16 Data_Size;
	uint8 * data_ptr;
} BCM_RxBuffer_t;


/******************************************************************************************
*                                                                                         *
*                               STATIC VARIABLES                                          *
*																						  *
*																						  *
*******************************************************************************************/





/******************************************************************************************
*                                                                                         *
*                                 PROTOTYPES                                              *
*																						  *
*																						  *
*******************************************************************************************/

/**
 * Input:
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function initializes the BCM module.
 *
 */
ERR_STATUS BCM_Init ( BCM_ConfigType * ConfigPtr);
/**
 * Input:
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function deinitializes the BCM module.
 *
 */
ERR_STATUS BCM_DeInit(void);
/**
 * Input:
 *		 u8_BCM_ID: the channel id.
 *		 u8_BCM_Size: the buffer size.
 *		 u8ptr_BCM_Data: pointer to desired data buffer.
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function starts sending data through desired channel.
 *
 */
ERR_STATUS BCM_send (uint8 * data_ptr, uint16 Bytes_Num);
/**
 * Input:
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function controls the state machine for the Tx.
 *
 */
ERR_STATUS BCM_TX_Dispatcher(void);
/**
 * Input:
 *		 u8_BCM_ID: the channel id.
 *		 u8_BCM_Size: the buffer size.
 *		 u8ptr_BCM_Data: pointer to desired data buffer.
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function starts receiving data through desired channel.
 *
 */
ERR_STATUS BCM_Setup_Receive( uint16 Data_Size, uint8 *ptr_Data);
/**
 * Input:
 *		 u8_BCM_ID: the channel id.
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function unlocks the receiving ability for the desired channel.
 *
 */
ERR_STATUS BCM_RxUnlock(void);
/**
 * Input:
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function controls the state machine for the Rx.
 *
 */
ERR_STATUS BCM_Rx_Dispatcher(void);


#endif /* SERVICELAYER_BCM_BCM_H_ */
