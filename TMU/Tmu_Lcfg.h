/*
 * TMU_LCfg.h
 *
 *  Created on: Jan 11, 2020
 *      Author: memo1
 */

#ifndef SERVICELAYER_TMU_TMU_LCFG_H_
#define SERVICELAYER_TMU_TMU_LCFG_H_

#include "std_types.h"


#define TIMER_0  0
#define TIMER_1  1
#define TIMER_2  2

#define RESOLUTION  1



typedef struct Tmu_Cfg_s_t
{
	uint8 Timer_Id;
	uint8 Timer_Resolution;
}Tmu_Cfg_s_t;

extern Tmu_Cfg_s_t str_Tmu_Cfg;

#endif /* SERVICELAYER_TMU_TMU_LCFG_H_ */
