/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-29 14:07:30 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-29 14:18:37 +0800
************************************************************************************************/ 
#ifndef __SYS_PARAM_H_
#define __SYS_PARAM_H_
#include "SysParamDefault.h"

//主序号
typedef enum INDEX_MAIN_SYS_PARAM
{
    INDEX_MAIN_SYS_PARAM_STEP_MOTOR_CORRECT        = 0X00,//电机修正参数
}INDEX_MAIN_SYS_PARAM;

//入口节点子参数
typedef enum INDEX_SUB_PARAM_STEP_MOTOR_CORRECT
{
    INDEX_SUB_PARAM_STEP_MOTOR_CORRECT_1_RESET      = 0X00,//复位修正参数 
    INDEX_SUB_PARAM_STEP_MOTOR_CORRECT_1_DIR_SWITCH = 0X01,//换向修正参数 
    INDEX_SUB_PARAM_STEP_MOTOR_CORRECT_2_RESET      = 0X02,//复位修正参数 
    INDEX_SUB_PARAM_STEP_MOTOR_CORRECT_2_DIR_SWITCH = 0X03,//换向修正参数 
}INDEX_SUB_APP_RAIL_NODE_ENTRY_PARAM;


//初始化系统参数
void SysParamInit(void);

//写入参数
ERROR_SUB SysParamWriteSingle(INDEX_MAIN_SYS_PARAM mainIndex,uint8_t subIndex,int32_t paramWrite);

//读取参数
ERROR_SUB SysParamReadSingle(INDEX_MAIN_SYS_PARAM mainIndex,uint8_t subIndex,int32_t* paramRead);


#endif








