/*
 * SPI_Pb.h
 *
 * Created: 12/20/2019 3:25:15 PM
 *  Author: Wave7
 */ 



#ifndef SPI_H_
#define SPI_H_

#include "std_types.h"
#include "interrupt.h"
#include "DIO1.h"
#include "common_macros.h"


typedef struct {
uint8 u8_SPIMode;
uint8 u8_DataMode;
uint8 u8_DataOrder;
uint8 u8_InterruptMode;
uint8 u8_Prescaler;
}SPI_Cfg_s;


/* NORMAL SPEED */
#define SPI_PRESCALER_2     (0)
#define SPI_PRESCALER_4     (1)
#define SPI_PRESCALER_8     (2)
#define SPI_PRESCALER_16    (3)
#define SPI_PRESCALER_32    (4)
#define SPI_PRESCALER_64    (5)
#define SPI_PRESCALER_128   (6)

/* YOU CAN USE PRE_SCALER_F_64 FROM NORMAL SPEED ! */


/* Polling or Interrupt */
#define POLLING         (0)
#define INTERRUPT       (1)

/* Master or Slave */
#define SLAVE           (0)
#define MASTER          (1)

/* Most SB or Least SB */
#define MSB             (0)
#define LSB             (1)

/* Data modes */
#define MODE_0			(0)
#define MODE_1			(1)
#define MODE_2			(2)
#define MODE_3			(3)

/* Description : SPI Initialization                                              */
/* Input       : SPI_Cfg_s* "Pointer to Struct of SPI Configration" */
/* Output      : Error Checking                                                  */
ERROR_STATUS SPI_Init(SPI_Cfg_s* pstr_SPI_Confg);


/* Description : SPI Send Byte                                                    */
/* Input       :                                                                  */
/* Output      : Error Checking                                                   */
/* INOUT       : u8_Data 					                                      */
ERROR_STATUS SPI_SendByte(uint8 u8_Data);

/* Description : SPI Receive Byte                                                 */
/* Input       :                                                                  */
/* Output      : Error Checking                                                   */
/* INOUT       : u8_Data "pointer to u8 Data"                                     */
ERROR_STATUS SPI_ReceiveByte(uint8 *ptru8_Data);


#endif /* SPI_H_ */
