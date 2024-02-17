/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-22 09:01:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-11 13:30:54 +0800
************************************************************************************************/ 
#ifndef __MOD_IMPL_BASE_CUP_TRAY_MANAGE_H_
#define __MOD_IMPL_BASE_CUP_TRAY_MANAGE_H_
#include "ModImplCupTrayManageConfig.h"

//读取模块输入
void ModImplBaseCupTrayManageReadPortIn(uint8_t portIdx,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//写入模块输出
void ModImplBaseCupTrayManageWritePortOut(uint8_t portIdx,PIN_STATE setState,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//步进电机复位
void ModImplBaseCupTrayManageStepMotorReset(uint8_t stepMotorIdx,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//步进电机走位
void ModImplBaseCupTrayManageStepMotorRunSteps(uint8_t stepMotorIdx,int32_t runSteps,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//杯栈门管理状态机初始化
void ModImplBaseCupTrayManageFSM_Init(void);

//杯栈门管理状态机周期运转
void ModImplBaseCupTrayManageFSM_RunPeriod(void);

//设置杯栈门状态
void ModImplBaseCupTrayManageSetDoorState(CUP_TRAY_MANAGE_DOOR_STATE doorState,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//新杯盘栈初始化
void ModImplBaseCupTrayManageNewStackInit(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//空杯盘栈初始化
void ModImplBaseCupTrayManageEmptyStackInit(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//推手初始化
void ModImplBaseCupTrayManagePushInit(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//夹手初始化
void ModImplBaseCupTrayManageClampInit(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//垃圾桶下降
void ModImplBaseCupTrayManageGarbageDownUnlock(GARBAGE_SELECT garbageNo,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//垃圾桶上升
void ModImplBaseCupTrayManageGarbegeUpLock(GARBAGE_SELECT garbageNo,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//杯栈模块整体初始化
void ModImplBaseCupTrayManageInitAll(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//夹手释放
void ModImplBaseCupTrayManageClampOpen(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//夹手夹紧
void ModImplBaseCupTrayManageClampClose(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//新杯盘栈上传一个新盘
void ModImplBaseCupTrayManageUploadNewTray(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//空杯盘栈从测试区回收一个空盘
void ModImplBaseCupTrayManageRecycleEmptyTray(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//推手推一个新盘到测试位
void ModImplBaseCupTrayManagePushNewTrayToTest(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//新杯盘栈上升指定层(门开到位状态)
void ModImplBaseCupTrayManageNewStackUpSpecialLayerWithDoorOpenState(uint8_t layers,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//新杯盘栈回零(门开到位状态)
void ModImplBaseCupTrayManageNewStackReturnZero(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//空杯盘上升指定层(门开到位状态)
void ModImplBaseCupTrayManageEmptyStackUpSpecialLayerWithDoorOpenState(uint8_t layers,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//空杯盘栈回零(门开到位状态)
void ModImplBaseCupTrayManageEmptyStackReturnZero(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//获取模块信息集合
void ModImplBaseCupTrayManageGetMsgSet(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);





#endif













