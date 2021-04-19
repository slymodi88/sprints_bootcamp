/*
 * spi.c
 *
 *  Created on: Dec 28, 2019
 *      Author: memo1
 */
#include "spi.h"

#define SPIE  7
#define SPE   6
#define DORD  5
#define MSTR  4
#define CPOL  3
#define CPHA  2
#define SPR1  1
#define SPR0  0

#define PB0   0
#define PB1   1
#define PB2   2
#define PB3   3
#define PB4   4
#define PB5   5
#define PB6   6
#define PB7   7

#define SPI2X 0

#define SPIF 7


static ERROR_STATUS SPI_INIT_Status = E_OK;
static ERROR_STATUS SPI_Send_Status = E_OK;

/* Description : SPI Initialization                                              */
/* Input       : SPI_Cfg_s* "Pointer to Struct of SPI Configration" */
/* Output      : Error Checking                                                  */
ERROR_STATUS SPI_Init(SPI_Cfg_s* pstr_SPI_Confg)
{
	/*
	 * select SPI MODE MASTER or SLAVE
	 */
	switch (pstr_SPI_Confg->u8_SPIMode)
	{
	case SLAVE :
		/******** Configure SPI Slave Pins *********
		 * SS(PB4)   --> Input
		 * MOSI(PB5) --> Input
		 * MISO(PB6) --> Output
		 * SCK(PB7) --> Input
		 ********************************************/

		CLR_BIT(PORTB_DIR,PB4);
		CLR_BIT(PORTB_DIR,PB5);
		SET_BIT(PORTB_DIR,PB6);
		CLR_BIT(PORTB_DIR,PB7);
		/************ SELECT SLAVE MODE AND ENABLE SPI ************/
		CLR_BIT(SPCR,MSTR);
		SET_BIT(SPCR,SPE);
		break;
	case MASTER :
		/******** Configure SPI Master Pins *********
		 * SS(PB4)   --> Output
		 * MOSI(PB5) --> Output
		 * MISO(PB6) --> Input
		 * SCK(PB7) --> Output
		 ********************************************/

		SET_BIT(PORTB_DIR,PB4);
		SET_BIT(PORTB_DIR,PB5);
		CLR_BIT(PORTB_DIR,PB6);
		SET_BIT(PORTB_DIR,PB7);
		/************ SELECT MASTER MODE AND ENABLE SPI ************/
		SET_BIT(SPCR,MSTR);
		SET_BIT(SPCR,SPE);
		break;
	default :
		SPI_INIT_Status = E_NOK;
		break;

	}
	/*
	 * select SPI CLOCK PHASE AND POLARITY
	 */
	switch (pstr_SPI_Confg->u8_DataMode)
	{
	case MODE_0 :
		CLR_BIT(SPCR,CPHA);
		CLR_BIT(SPCR,CPOL);
		break;
	case MODE_1 :
		SET_BIT(SPCR,CPHA);
		CLR_BIT(SPCR,CPOL);
		break;
	case MODE_2 :
		CLR_BIT(SPCR,CPHA);
		SET_BIT(SPCR,CPOL);
		break;
	case MODE_3 :
		SET_BIT(SPCR,CPHA);
		SET_BIT(SPCR,CPOL);
		break;

	default :
		SPI_INIT_Status = E_NOK;
		break;

	}
	/*
	 * SELECT INTERRUPT MODE OR POLLING MODE
	 */
	switch (pstr_SPI_Confg->u8_InterruptMode)
	{
	case POLLING :
		CLR_BIT(SPCR,SPIE);
		break;
	case INTERRUPT :
		SET_BIT(SPCR,SPIE);
		break;
	default :
		SPI_INIT_Status = E_NOK;
		break;

	}
	/*
	 * select SPI DATA ORDER MSB or LSB
	 */
	switch (pstr_SPI_Confg->u8_DataOrder)
	{
	case MSB :
		CLR_BIT(SPCR,DORD);
		break;
	case LSB :
		SET_BIT(SPCR,DORD);
		break;
	default :
		SPI_INIT_Status = E_NOK;
		break;

	}
	/*
	 * select SPI PRESCALER
	 */
	switch (pstr_SPI_Confg->u8_Prescaler)
	{
	/*
	 * (Fosc/2)
	 */
	case SPI_PRESCALER_2 :
		CLR_BIT(SPCR,SPR0);
		CLR_BIT(SPCR,SPR1);
		SET_BIT(SPCR,SPI2X);
		break;
		/*
		 * (Fosc/4)
		 */
	case SPI_PRESCALER_4 :
		CLR_BIT(SPCR,SPR0);
		CLR_BIT(SPCR,SPR1);
		CLR_BIT(SPCR,SPI2X);
		break;
		/*
		 * (Fosc/8)
		 */
	case SPI_PRESCALER_8 :
		SET_BIT(SPCR,SPR0);
		CLR_BIT(SPCR,SPR1);
		SET_BIT(SPCR,SPI2X);
		break;
		/*
		 * (Fosc/16)
		 */
	case SPI_PRESCALER_16 :
		SET_BIT(SPCR,SPR0);
		CLR_BIT(SPCR,SPR1);
		CLR_BIT(SPCR,SPI2X);
		break;
		/*
		 * (Fosc/32)
		 */
	case SPI_PRESCALER_32 :
		CLR_BIT(SPCR,SPR0);
		SET_BIT(SPCR,SPR1);
		SET_BIT(SPCR,SPI2X);
		break;
		/*
		 * (Fosc/64)
		 */
	case SPI_PRESCALER_64 :
		CLR_BIT(SPCR,SPR0);
		SET_BIT(SPCR,SPR1);
		CLR_BIT(SPCR,SPI2X);
		break;
		/*
		 * (Fosc/128)
		 */
	case SPI_PRESCALER_128 :
		SET_BIT(SPCR,SPR0);
		SET_BIT(SPCR,SPR1);
		CLR_BIT(SPCR,SPI2X);
		break;
	default :
		SPI_INIT_Status = E_NOK;
		break;

	}


	return SPI_INIT_Status;
}


/* Description : SPI Send Byte                                                    */
/* Input       :                                                                  */
/* Output      : Error Checking                                                   */
/* INOUT       : u8_Data 					                                      */
ERROR_STATUS SPI_SendByte(uint8 u8_Data)
{

	if(E_OK == SPI_INIT_Status)
	{

	SPDR = u8_Data ;
	while(BIT_IS_CLEAR(SPSR,SPIF)){}

	}
	else {
		SPI_Send_Status = E_NOK;
	}

	return SPI_Send_Status;

}

/* Description : SPI Receive Byte                                                 */
/* Input       :                                                                  */
/* Output      : Error Checking                                                   */
/* INOUT       : u8_Data "pointer to u8 Data"                                     */
ERROR_STATUS SPI_ReceiveByte(uint8 *ptru8_Data)
{
	if(E_OK == SPI_INIT_Status)
		{
	while(BIT_IS_CLEAR(SPSR,SPIF)){}
	*ptru8_Data = SPDR;
		}
	else {
		SPI_Send_Status = E_NOK;
	}
	return SPI_Send_Status;
}

