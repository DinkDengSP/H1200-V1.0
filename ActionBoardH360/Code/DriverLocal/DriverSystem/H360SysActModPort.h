/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-22 17:36:26 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-23 08:53:37 +0800
************************************************************************************************/ 
#ifndef __H360_SYS_MOD_PORT_H_
#define __H360_SYS_MOD_PORT_H_
#include "SystemBase.h"
#include "H360SysActPortIn.h"
#include "H360SysActPortOut.h"


//模块读取指定输入
ERROR_SUB H360SysActModInputRead(uint8_t index,uint8_t indexMax,MODULE_INPUT_MAP_UNIT const* arrayPtr,PIN_STATE* state);

//模块写入指定输出
ERROR_SUB H360SysActModOutputWrite(uint8_t index,PIN_STATE state,uint8_t indexMax,MODULE_OUTPUT_MAP_UNIT const* arrayPtr);




#endif





















