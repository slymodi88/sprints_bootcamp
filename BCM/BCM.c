/*
 * BCM.c
 *
 *  Created on: Jan 15, 2020
 *      Author: memo1
 */
#include "BCM.h"

/*------------------------------ Section of Macros Constant  -------------------------------------*/
#define BCM_ID      0xA5

#define LOCKED		1
#define UNLOCKED	0


#define NO_REQUEST			0
#define REQUEST				1
#define FINISHED_REQUEST	2

#define BUSY		0
#define NOT_BUSY	1

/*-----------------------------------------Global Variables---------------------------------------------------------*/
static uint8 gTX_Lock_Flag = UNLOCKED;
static Dispatcher_Status_t gTX_Dispatcher_Status;
static Rx_Dispatcher_Status_t gRX_Dispatcher_Status = Rx_IDLE;
static uint8 gTX_BCM_Request = NO_REQUEST ;
static uint8 gRX_Buffer_Index = 0;
static uint8 gBCM_state_flag = 0;


static sint16 gTX_Dispatcher_Counter = 0;
static uint8 gTX_Dispatcher_Busy_Flag = NOT_BUSY;
static uint8 gRx_Buffer_Lock = UNLOCKED;
static uint8 BCM_Received_Data = 0;
static uint8 gRx_Receive_Counter = 0;
static uint8 gRx_Received_CheckSum = 0;
static uint16 Rx_Data_Size =0;
static uint8 gRX_CheckSum=0;

BCM_Buffer_t BCM_Info_str;
BCM_RxBuffer_t BCM_RxInfo_str;


ERR_STATUS BCM_Init ( BCM_ConfigType * ConfigPtr)
{
	ERR_STATUS retval = BCM_BASE_ERROR + ERR_SUCCESS;
	if(NULL == ConfigPtr )
	{
		retval = BCM_BASE_ERROR + ERR_NULL_PTR;

	}
	else
	{
		switch(ConfigPtr->Comm_ID)
		{
		case UART :
			//UART_Init();
			gTX_Dispatcher_Status = IDLE;
			break;
		case SPI :
			/* To call the spi function init*/
			break;
		case I2C :
			/* To call the i2c function init*/
			break;
		default:
			retval = BCM_BASE_ERROR + ERR_RESOURCE_NOT_FOUND;
			break;

		}

	}
	return retval;

}
ERR_STATUS BCM_DeInit(void)
{
	ERR_STATUS retval = BCM_BASE_ERROR + ERR_SUCCESS;
	//UART_DeInit();

	return retval;

}
ERR_STATUS BCM_send (uint8 * data_ptr, uint16 Bytes_Num)
{
	ERR_STATUS retval = BCM_BASE_ERROR + ERR_SUCCESS;
	if(gTX_Lock_Flag == UNLOCKED)
	{
		BCM_Info_str.BCM_Id = UART;
		BCM_Info_str.Data_Size = Bytes_Num;
		BCM_Info_str.data_ptr = data_ptr;
		BCM_Info_str.Check_Sum_Error = 0;
		gTX_BCM_Request = REQUEST;

	}
	else
	{
		retval = BCM_BASE_ERROR + ERR_INVALID_ARGU;

	}


	return retval;



}
ERR_STATUS BCM_TX_Dispatcher(void)
{
	static uint16 TX_Buffer_Index = 0;
	ERR_STATUS retval = BCM_BASE_ERROR + ERR_SUCCESS;
	switch (gTX_Dispatcher_Status)
	{
	case IDLE:
		TX_Buffer_Index = 0;
		gTX_Dispatcher_Counter=0;
		gTX_Dispatcher_Busy_Flag = NOT_BUSY;
		if(gTX_BCM_Request == REQUEST)
		{
			gTX_Dispatcher_Status = SENDING_BYTE;
			gTX_Lock_Flag = LOCKED;
		}
		break;
	case SENDING_BYTE:
		if(gTX_Dispatcher_Busy_Flag == NOT_BUSY)
		{
			if(gTX_Dispatcher_Counter==0)
			{
				gTX_Dispatcher_Busy_Flag = BUSY;
				UART_SendByte(BCM_Info_str.BCM_Id);
			}
			else if(gTX_Dispatcher_Counter == 1)
			{
				gTX_Dispatcher_Busy_Flag = BUSY;
				uint8 Least_Nibble_Data_Size = (uint8)BCM_Info_str.Data_Size;
				UART_SendByte (Least_Nibble_Data_Size);
			}
			else if(gTX_Dispatcher_Counter == 2)
			{
				gTX_Dispatcher_Busy_Flag = BUSY;
				uint8 MOST_Nibble_Data_Size = (uint8)(BCM_Info_str.Data_Size>>8);
				UART_SendByte (MOST_Nibble_Data_Size);
			}
			else
			{
				gTX_Dispatcher_Busy_Flag = BUSY;
				if(TX_Buffer_Index < BCM_Info_str.Data_Size )
				{
					UART_SendByte(BCM_Info_str.data_ptr[TX_Buffer_Index]);
					BCM_Info_str.Check_Sum_Error = BCM_Info_str.Check_Sum_Error  + BCM_Info_str.data_ptr[TX_Buffer_Index];
					TX_Buffer_Index++;
				}
				else if(TX_Buffer_Index == BCM_Info_str.Data_Size)
				{
					UART_SendByte(BCM_Info_str.Check_Sum_Error);
				}

			}
		}
		break;
	case SENDING_BYTE_COMPLETE:
		if (TX_Buffer_Index ==((BCM_Info_str.Data_Size)) )
		{
			gTX_Dispatcher_Status = FRAME_COMPLETE;

		}

		else
		{
			gTX_Dispatcher_Status = SENDING_BYTE;

			if( (gTX_Dispatcher_Counter >= 0) && (gTX_Dispatcher_Counter <= 2 )	)
			{
				gTX_Dispatcher_Counter++;
			}
			else
			{

			}

		}

		gTX_Dispatcher_Busy_Flag = NOT_BUSY;

		break;
	case FRAME_COMPLETE :
		gTX_Dispatcher_Status = IDLE;
		gTX_BCM_Request = FINISHED_REQUEST;
		gTX_Lock_Flag = UNLOCKED ;
		break;
	default:
		break;

	}

	return retval;

}
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
ERR_STATUS BCM_Setup_Receive( uint16 Data_Size, uint8 *ptr_Data)
{
	uint8 retval = BCM_BASE_ERROR + ERR_SUCCESS;
	if(gRx_Buffer_Lock == UNLOCKED)
	{
		if(NULL != ptr_Data)
		{
			BCM_RxInfo_str.Data_Size = Data_Size;
			BCM_RxInfo_str.data_ptr = ptr_Data;
			BCM_RxInfo_str.Check_Sum_Error = 0;

		}
		else
		{
			retval = BCM_BASE_ERROR + ERR_NULL_PTR;
		}
	}
	else
	{
		retval = BCM_BASE_ERROR + ERR_RESOURCE_NOT_FOUND;

	}


	return retval;
}
/**
 * Input:
 *		 u8_BCM_ID: the channel id.
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function unlocks the receiving ability for the desired channel.
 *
 */
ERR_STATUS BCM_RxUnlock(void)
{
	uint8 retval = BCM_BASE_ERROR + ERR_SUCCESS;

	return retval;
}
/**
 * Input:
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function controls the state machine for the Rx.
 *
 */
ERR_STATUS BCM_Rx_Dispatcher(void)
{
	uint8 retval = BCM_BASE_ERROR + ERR_SUCCESS;

	switch (gRX_Dispatcher_Status)
	{
	case Rx_IDLE :
		gRX_Buffer_Index = 0;
		gRx_Receive_Counter=0;
		if(gRx_Receive_Counter==0)
		{
			if(BCM_Received_Data == BCM_ID)
			{
                /* lock Buffer */
				gRx_Buffer_Lock = LOCKED;
				gRX_Dispatcher_Status = RECEIVING_BYTE;
				gRx_Receive_Counter ++;
				gBCM_state_flag = 1;
			}
			else
			{
				gRx_Buffer_Lock = UNLOCKED;
			}
			break;
	case RECEIVING_BYTE:
		if(gRx_Receive_Counter==1)
		{
			Rx_Data_Size |=BCM_Received_Data;
			gRx_Receive_Counter ++;
		}
		else if(gRx_Receive_Counter==2)
		{
			Rx_Data_Size |=(BCM_Received_Data << 8);

		}
		else
		{

		}
		break;
	case RECEIVING_BYTE_COMPLETE:
		if (gRx_Receive_Counter > 2 )
		{
			if(gRx_Receive_Counter == BCM_RxInfo_str.Data_Size  + 3)
			{
				/*check if the received check sum is equal to the calculated one */
				gRx_Received_CheckSum = BCM_Received_Data;
				if(gRx_Received_CheckSum == gRX_CheckSum )
				{
					/*change module state */
					gRX_Dispatcher_Status = RX_FRAME_COMPLETE;
				}


			}
			else
			{
				/* calculate Check Sum*/
				gRX_CheckSum += BCM_Received_Data;
				gRx_Receive_Counter ++;



			}


		}
		break;
	case RX_FRAME_COMPLETE:
		//UART_Deinit();
		break;
	default:
		break;

		}
	}

	return retval;

}

void BCM_Tx_CBK (void)
{
	gTX_Dispatcher_Status = SENDING_BYTE_COMPLETE;
}

void BCM_Rx_CBK(void)
{
	UART_ReceiveByte(&BCM_Received_Data);
	if(gBCM_state_flag==1)
	{
		gRX_Dispatcher_Status = RECEIVING_BYTE_COMPLETE;

	}
	else
	{

	}

}

