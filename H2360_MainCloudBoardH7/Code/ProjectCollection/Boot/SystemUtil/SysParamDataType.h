/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-29 14:07:30 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-31 16:37:04 +0800
************************************************************************************************/ 
#ifndef __SYS_PARAM_DATA_TYPE_H_
#define __SYS_PARAM_DATA_TYPE_H_
#include "DriverHeaderBoard.h"

//应用配置的标志位,初始化检测该标志位,如果不对,需要设置为默认参数
#define SYS_PARAM_DEFAULT_FLAG              0XAA5555AA

/*---------------------------------------------------------------------------------------------------------------*/
//总的系统参数
typedef struct SYS_PARAM
{
    uint32_t paramFlag;//参数已经配置过标识
}SYS_PARAM;
//参数长度
#define LENGTH_SYS_PARAM     DIM_STRUCT_LENGTH_BYTE(SYS_PARAM)


#endif


