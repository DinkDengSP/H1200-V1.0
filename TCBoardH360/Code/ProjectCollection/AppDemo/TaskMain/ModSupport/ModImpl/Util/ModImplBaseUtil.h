/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-13 10:20:55 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-19 00:02:19 +0800
************************************************************************************************/ 
#ifndef __MOD_IMPL_BASE_UTIL_H_
#define __MOD_IMPL_BASE_UTIL_H_
#include "ModImplBase.h"

//初始化状态机
void ModImplBaseUtilInitTempControlMachine(void);

//环境温度控温状态LED
void ModImplBaseUtilEnvirLedTempControlMachine(void);

//反应盘控温状态LED
void ModImplBaseUtilPlateReactionLedTempControlMachine(void);

//清洗盘控温状态LED
void ModImplBaseUtilPlateWashLedTempControlMachine(void);

//试剂盘控温状态LED
void ModImplBaseUtilPlateReagentLedTempControlMachine(void);

//初始化状态机
void ModImplBaseUtilInitErrorShowMachine(void);

//板上温度通道错误状态LED
void ModImplBaseUtilBoardLedErrorShowMachine(void);

//环境温度通道错误状态LED
void ModImplBaseUtilEnvirLedErrorShowMachine(void);

//反应盘通道错误状态LED
void ModImplBaseUtilPlateReactionLedErrorShowMachine(void);

//清洗盘通道错误状态LED
void ModImplBaseUtilPlateWashLedErrorShowMachine(void);

//试剂盘通道错误状态LED
void ModImplBaseUtilPlateReagentLedErrorShowMachine(void);


//设置温控上报
void ModImplBaseUtilReportFlagSet(TC_BASE_CHANNEL tcBaseChannel,FunctionalState reportFlag,uint32_t intervalTimeMs,
                                        uint8_t reportTargetModuleNo,uint8_t reportTargetBoardID,SYS_RESULT_PACK* resultPackPtr,
                                        uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//获取板上控温状态
void ModImplBaseUtilGetBoardTempControl(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//获取环境控温状态
void ModImplBaseUtilGetEnvirTempControl(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//获取反应盘控温状态
void ModImplBaseUtilGetPlateReactionTempControl(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//获取清洗盘控温状态
void ModImplBaseUtilGetPlateWashTempControl(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//获取试剂盘控温状态
void ModImplBaseUtilGetPlateReagentTempControl(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//获取试剂盘浮球状态
void ModImplBaseUtilGetPlateReagentFloatSensor(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//获取反应盘电流检测值
void ModImplBaseUtilGetPlateReactionCurrent(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//获取清洗盘电流检测值
void ModImplBaseUtilGetPlateWashCurrent(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//获取试剂盘电流检测值
void ModImplBaseUtilGetPlateReagentCurrent(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);


#endif














