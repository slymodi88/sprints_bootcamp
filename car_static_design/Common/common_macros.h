/*
 * common_macros.h
 *
 *  Created on: Dec 22, 2019
 *      Author: Khalaf & Zayed
 */

#ifndef SPRINTS_ZAYED_AVR_STATICDESIGN_SRC_MCAL_COMMON_MACROS_H_
#define SPRINTS_ZAYED_AVR_STATICDESIGN_SRC_MCAL_COMMON_MACROS_H_


/************************************************************************/
/*			       			BIT_MATH defines       	                    */
/************************************************************************/

/* Set VARiable's Certain BIT High */

#define SET_BIT(var,bitNum)       var |= (1<<bitNum)

/* Set VARiable's Certin BIT Low */

#define CLR_BIT(var,bitNum)       var &= ~(1<<bitNum)

/* Toggle VARiable's Certin BIT */

#define TOGGLE_BIT(var,bitNum)       var ^= (1<<bitNum)

/* Assign VARiable's Certin BIT by a Certin VALue */

#define ASSIGN_BIT(VAR,BIT_NB,VAL)  (VAR)=((VAR&(~(BIT_NB)))|(VAL&BIT_NB))

/* Get VARiable's Certin BIT Value */

#define GET_BIT(var,bitNum)       (var &(1<<bitNum)) >> bitNum
/* Check if a specific bit is set in any register and return true if yes */
#define BIT_IS_SET(REG,BIT) ( REG & (1<<BIT) )

/* Check if a specific bit is cleared in any register and return true if yes */
#define BIT_IS_CLEAR(REG,BIT) ( !(REG & (1<<BIT)) )





#endif /* SPRINTS_ZAYED_AVR_STATICDESIGN_SRC_MCAL_COMMON_MACROS_H_ */
