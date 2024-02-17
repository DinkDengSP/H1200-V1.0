/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-30 11:40:24 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-30 11:40:42 +0800
************************************************************************************************/ 
#ifndef __STEP_MOTOR_MACRO_H_
#define __STEP_MOTOR_MACRO_H_

//电机驱动的速度表的最大长度
#define SM_SPEED_TABLE_LENGTH_MAX                   1024
//S曲线加速的单个加速段最大长度
#define SM_SPEED_TABLE_S_SINGLE_LENGTH_MAX          (SM_SPEED_TABLE_LENGTH_MAX/6)
//加速段过程
//0 - SM_SPEED_TABLE_S_SINGLE_LENGTH_MAX
//匀加速过程
//SM_SPEED_TABLE_S_SINGLE_LENGTH_MAX - 2*SM_SPEED_TABLE_S_SINGLE_LENGTH_MAX
//减加速过程
//2*SM_SPEED_TABLE_S_SINGLE_LENGTH_MAX - 3*SM_SPEED_TABLE_S_SINGLE_LENGTH_MAX -1
//剩下来就完全对称
//多余步数的点放在3*SM_SPEED_TABLE_S_SINGLE_LENGTH_MAX上
//设定系统可配置加速曲线的数量(每个电机的曲线数量)
#define SM_SPEED_TABLE_COUNT_EACH_ONE               21
//步进电机参数配置标识,初始化的时候检测不到这个值,将会初始化一遍参数列表
#define SM_CONFIG_FLAG                              0XA5A5A5A5
//当下面这个参数和SM_CONFIG_FLAG相等的时候,自装载一次参数,否则,每次装载参数
#define SM_CONFIG_FLAG_LOAD                         SM_CONFIG_FLAG+10
//系统内部的步进电机数量
#define SYSTEM_STEP_MOTOR_COUNT                     12





#endif
