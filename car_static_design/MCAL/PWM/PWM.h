#ifndef PWM_H_
#define PWM_H_
/***************************************************/
/* Layer: MCAL                                     */

/* Module: PWM                                     */
/* File: PWM.h                                     */
/* Author: Team A                                  */
/* Description: PWM header file                    */
/***************************************************/

/*********************************/
/*            MACROS             */
/*********************************/
#include "std_types.h"
//#define PWM_CH0   0
#define PWM_CH1A  1
#define PWM_CH1B  2
#define PWM_CHAB  4
//#define PWM_CH2   3

#define     PWM_NO_CLOCK                    (0x00)
#define		PWM_PRESCALER_NO				(0x01)
#define		PWM_PRESCALER_8					(0x02)
#define		PWM_PRESCALER_64				(0x03)
#define		PWM_PRESCALER_256				(0x04)
#define		PWM_PRESCALER_1024				(0x05)



typedef struct Pwm_Cfg_s_t 
{
	uint8 channel;
	uint8 prescaler;
}Pwm_Cfg_s_t;

/*********************************/
/*    FUNCTIONS DECLARATION      */
/*********************************/

/*********************************************************************************/
/* Function: Error_State Pwm_Init(Pwm_Cfg_s_t *Pwm_Cfg);                         */
/* Type: public                                                                  */
/* Input parameters: Pwm_Cfg Structure (channel number, Prescaler)               */
/* Return type : void                                                            */
/*                                                                               */
/* Description: initialize the PWM configuration                                 */
/*********************************************************************************/
extern ERROR_STATUS Pwm_Init(/*Pwm_Cfg_s_t *Pwm_Cfg*/);

/*********************************************************************************/
/* Function: Error_State Pwm_Start(uint8_t channel,uint8_t Duty);                */
/* Type: public                                                                  */
/* Input parameters: Channel ID, Duty cycle(0:100)                               */
/* Return type : Error State                                                     */
/*                                                                               */
/* Description: starts the PWM on the dedicated channel with the required duty   */
/*				cycle and frequency                                              */
/*********************************************************************************/
extern ERROR_STATUS Pwm_Start(uint8 channel,uint8 Duty,uint32 Frequncy);

/*********************************************************************************/
/* Function: Error_State Pwm_Start(uint8_t channel,uint8_t Duty);                */
/* Type: public                                                                  */
/* Input parameters: Channel ID, Duty cycle(0:100)                               */
/* Return type : Error State                                                     */
/*                                                                               */
/* Description: updates the duty cycle and frequency of the dedicated channel    */
/*********************************************************************************/
extern ERROR_STATUS Pwm_Update(uint8 channel,uint8 Duty,uint32 Frequncy);

/*********************************************************************************/
/* Function: Error_State Pwm_Stop(uint8_t channel);                              */
/* Type: public                                                                  */
/* Input parameters: channel ID                                                  */
/* Return type : Error state                                                     */
/*                                                                               */
/* Description: responsible of Stopping the PWM by clearing the prescaler		 */
/*				of the corresponding channel                                     */
/*********************************************************************************/
extern ERROR_STATUS Pwm_Stop(uint8 channel);


#endif /* PWM_H_ */
