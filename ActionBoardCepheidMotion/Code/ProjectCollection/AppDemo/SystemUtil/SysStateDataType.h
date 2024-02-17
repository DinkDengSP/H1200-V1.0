/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-31 16:20:36 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-31 16:40:26 +0800
************************************************************************************************/ 
#ifndef __SYS_STATE_DATA_TYPE_
#define __SYS_STATE_DATA_TYPE_
#include "DriverHeaderBoard.h"

//应用配置的标志位,初始化检测该标志位,如果不对,需要设置为默认参数
#define SYS_STATE_INIT_FLAG              0XA55A5AA5

//步进电机是否忙状态
typedef struct SYS_STATE_STEP_MOTOR
{
    ENTITY_STA stepMotor1EntityState;
    ENTITY_STA stepMotor2EntityState;
    ENTITY_STA stepMotor3EntityState;
    ENTITY_STA stepMotor4EntityState;
    ENTITY_STA stepMotor5EntityState;
    ENTITY_STA stepMotor6EntityState;
    ENTITY_STA stepMotor7EntityState;
    ENTITY_STA stepMotor8EntityState;
    ENTITY_STA stepMotor9EntityState;
    ENTITY_STA stepMotor10EntityState;
    ENTITY_STA stepMotor11EntityState;
    ENTITY_STA stepMotor12EntityState;
}SYS_STATE_STEP_MOTOR;
#define LENGTH_SYS_STATE_STEP_MOTOR        DIM_STRUCT_LENGTH_BYTE(SYS_STATE_STEP_MOTOR)


/*---------------------------------------------------------------------------------------------------------------*/
//总的系统参数
typedef struct SYS_STATE
{
    uint32_t stateInitFlag;//状态初始化标识
    SYS_STATE_STEP_MOTOR stepMotorState;//步进电机是否忙状态
}SYS_STATE;
//参数长度
#define LENGTH_SYS_STATE     DIM_STRUCT_LENGTH_BYTE(SYS_STATE)



#endif

















