/*
 * BCM_Cfg.h
 *
 *  Created on: Jan 15, 2020
 *      Author: memo1
 */

#ifndef SERVICELAYER_BCM_BCM_CFG_H_
#define SERVICELAYER_BCM_BCM_CFG_H_

#include "std_types.h"
#include "common_retval.h"

typedef enum { UART=0,SPI,I2C}Comm_ID_t;


typedef enum { IDLE=0, SENDING_BYTE, SENDING_BYTE_COMPLETE , FRAME_COMPLETE} Dispatcher_Status_t;

typedef enum { Rx_IDLE=0, RECEIVING_BYTE, RECEIVING_BYTE_COMPLETE , RX_FRAME_COMPLETE} Rx_Dispatcher_Status_t;

typedef struct
{
	Comm_ID_t Comm_ID;
} BCM_ConfigType  ;


extern BCM_ConfigType BCM_cnfg ;

#endif /* SERVICELAYER_BCM_BCM_CFG_H_ */
