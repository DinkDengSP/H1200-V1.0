/*******************************************************************
 *Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
 *Author: DengXiaoJun
 *Date: 2021-03-31 11:12:34
 *LastEditors: DengXiaoJun
 *LastEditTime: 2021-06-09 14:32:55
 *ModifyRecord1:    
 *ModifyRecord2:    
 *ModifyRecord3:    
 *ModifyRecord4:    
 *ModifyRecord5:    
******************************************************************/
#ifndef __STEP_MOTOR_MACRO_H_
#define __STEP_MOTOR_MACRO_H_
#include "StepMotorConfig.h"


//S曲线加速的单个加速段最大长度
//加速段过程
//0 - SM_SPEED_TABLE_S_SINGLE_LENGTH_MAX
//匀加速过程
//SM_SPEED_TABLE_S_SINGLE_LENGTH_MAX - 2*SM_SPEED_TABLE_S_SINGLE_LENGTH_MAX
//减加速过程
//2*SM_SPEED_TABLE_S_SINGLE_LENGTH_MAX - 3*SM_SPEED_TABLE_S_SINGLE_LENGTH_MAX -1
//剩下来就完全对称
//多余步数的点放在3*SM_SPEED_TABLE_S_SINGLE_LENGTH_MAX上
#define SM_SPEED_TABLE_S_SINGLE_LENGTH_MAX          (SM_SPEED_TABLE_LENGTH_MAX/6)




#endif




