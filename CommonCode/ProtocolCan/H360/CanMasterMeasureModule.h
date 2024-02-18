/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-24 13:00:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-30 20:13:48 +0800
************************************************************************************************/ 
#ifndef __CAN_MASTER_MEASURE_MODULE_H_
#define __CAN_MASTER_MEASURE_MODULE_H_
#include "CanMasterH360Base.h"

//测量模块电机
typedef enum INDEX_MOTOR_MEASURE_MODULE
{
    INDEX_MOTOR_MEASURE_MODULE_DOOR     = 0X00,//上门
    INDEX_MOTOR_MEASURE_MODULE_WINDOW   = 0X01,//侧门
    INDEX_MOTOR_MEASURE_MODULE_PUMP     = 0X02,//注射泵
}INDEX_MOTOR_MEASURE_MODULE;

//瓶号选择
typedef enum BOTTLE_SELECT_MEASURE_MODULE
{
    BOTTLE_SELECT_MEASURE_MODULE_A = 0X00,//A瓶
    BOTTLE_SELECT_MEASURE_MODULE_B = 0X01,//B瓶
}BOTTLE_SELECT_MEASURE_MODULE;

/*************************************发送指令等待ACK************************************************/
//模块内输入读取
void CanMasterMeasureModuleReadSingleWhileAck(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr);
//模块内输出写入
void CanMasterMeasureModuleWriteSingleWhileAck(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterMeasureModuleResetMotorWhileAck(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr);
//模块内电机走位
void CanMasterMeasureModuleRunStepsMotorWhileAck(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr);
//测量模块复位
void CanMasterMeasureModuleResetWhileAck(BOTTLE_SELECT_MEASURE_MODULE bottle,SYS_RESULT_PACK* resultPackPtr);
//测量上门全开
void CanMasterMeasureModuleDoorOpenFullWhileAck(SYS_RESULT_PACK* resultPackPtr);
//测量上门半开
void CanMasterMeasureModuleDoorOpenHalfWhileAck(SYS_RESULT_PACK* resultPackPtr);
//测量上门关闭
void CanMasterMeasureModuleDoorCloseWhileAck(SYS_RESULT_PACK* resultPackPtr);
//测量侧门打开
void CanMasterMeasureModuleWindowOpenWhileAck(SYS_RESULT_PACK* resultPackPtr);
//测量侧门关闭
void CanMasterMeasureModuleWindowCloseWhileAck(SYS_RESULT_PACK* resultPackPtr);
//激发液灌注
void CanMasterMeasureModulePrimeOnceWhileAck(BOTTLE_SELECT_MEASURE_MODULE bottle,SYS_RESULT_PACK* resultPackPtr);
//激发液注液
void CanMasterMeasureModuleInjectOnceWhileAck(BOTTLE_SELECT_MEASURE_MODULE bottle,SYS_RESULT_PACK* resultPackPtr);
//测量模块清洗维护
void CanMasterMeasureModuleInjectMaintainWhileAck(BOTTLE_SELECT_MEASURE_MODULE bottle,SYS_RESULT_PACK* resultPackPtr);
//测量模块暗计数
void CanMasterMeasureModuleReadDarkWhileAck(SYS_RESULT_PACK* resultPackPtr);
//测量模块读本底
void CanMasterMeasureModuleReadBackGroundWhileAck(SYS_RESULT_PACK* resultPackPtr);
//测量模块注液测光
void CanMasterMeasureModuleReadLightWithInjectWhileAck(BOTTLE_SELECT_MEASURE_MODULE bottle,SYS_RESULT_PACK* resultPackPtr);
//测量模块集成单次测量
void CanMasterMeasureModuleReadLightComplexWhileAck(BOTTLE_SELECT_MEASURE_MODULE bottle,FunctionalState enableInject,
                                                        SYS_RESULT_PACK* resultPackPtr);
//测量模块读光子变化率
void CanMasterMeasureModuleReadLightChangeRateWhileAck(BOTTLE_SELECT_MEASURE_MODULE bottle,uint8_t setChangeCount,
                                                        SYS_RESULT_PACK* resultPackPtr);

/*************************************等待指定指令完成返回************************************************/
//模块内输入读取
void CanMasterMeasureModuleWaitReadSingleResult(SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr);
//模块内输出写入
void CanMasterMeasureModuleWaitWriteSingleResult(SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterMeasureModuleWaitResetMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//模块内电机走位
void CanMasterMeasureModuleWaitRunStepsMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//测量模块复位
void CanMasterMeasureModuleWaitResetResult(SYS_RESULT_PACK* resultPackPtr);
//测量上门全开
void CanMasterMeasureModuleWaitDoorOpenFullResult(SYS_RESULT_PACK* resultPackPtr);
//测量上门半开
void CanMasterMeasureModuleWaitDoorOpenHalfResult(SYS_RESULT_PACK* resultPackPtr);
//测量上门关闭
void CanMasterMeasureModuleWaitDoorCloseResult(SYS_RESULT_PACK* resultPackPtr);
//测量侧门打开
void CanMasterMeasureModuleWaitWindowOpenResult(SYS_RESULT_PACK* resultPackPtr);
//测量侧门关闭
void CanMasterMeasureModuleWaitWindowCloseResult(SYS_RESULT_PACK* resultPackPtr);
//激发液灌注
void CanMasterMeasureModuleWaitPrimeOnceResult(SYS_RESULT_PACK* resultPackPtr);
//激发液注液
void CanMasterMeasureModuleWaitInjectOnceResult(SYS_RESULT_PACK* resultPackPtr);
//测量模块清洗维护
void CanMasterMeasureModuleWaitInjectMaintainResult(SYS_RESULT_PACK* resultPackPtr);
//测量模块暗计数
void CanMasterMeasureModuleWaitReadDarkResult(SYS_RESULT_PACK* resultPackPtr,uint32_t* darkValPtr);
//测量模块读本底
void CanMasterMeasureModuleWaitReadBackGroundResult(SYS_RESULT_PACK* resultPackPtr,uint32_t* backGroundValPtr);
//测量模块注液测光
void CanMasterMeasureModuleWaitReadLightWithInjectResult(SYS_RESULT_PACK* resultPackPtr,uint32_t* lightValPtr);
//测量模块集成单次测量
void CanMasterMeasureModuleWaitReadLightComplexResult(SYS_RESULT_PACK* resultPackPtr,uint32_t* backGroundValPtr,uint32_t* lightValPtr);
//测量模块读光子变化率
void CanMasterMeasureModuleWaitReadLightChangeRateResult(SYS_RESULT_PACK* resultPackPtr,uint8_t* pointCountPtr,uint32_t** lightArrayPtrPtr);

/*************************************发送指令等待完成返回************************************************/
//模块内输入读取
void CanMasterMeasureModuleReadSingleWhileReturn(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr);
//模块内输出写入
void CanMasterMeasureModuleWriteSingleWhileReturn(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterMeasureModuleResetMotorWhileReturn(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//模块内电机走位
void CanMasterMeasureModuleRunStepsMotorWhileReturn(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//测量模块复位
void CanMasterMeasureModuleResetWhileReturn(BOTTLE_SELECT_MEASURE_MODULE bottle,SYS_RESULT_PACK* resultPackPtr);
//测量上门全开
void CanMasterMeasureModuleDoorOpenFullWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//测量上门半开
void CanMasterMeasureModuleDoorOpenHalfWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//测量上门关闭
void CanMasterMeasureModuleDoorCloseWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//测量侧门打开
void CanMasterMeasureModuleWindowOpenWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//测量侧门关闭
void CanMasterMeasureModuleWindowCloseWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//激发液灌注
void CanMasterMeasureModulePrimeOnceWhileReturn(BOTTLE_SELECT_MEASURE_MODULE bottle,SYS_RESULT_PACK* resultPackPtr);
//激发液注液
void CanMasterMeasureModuleInjectOnceWhileReturn(BOTTLE_SELECT_MEASURE_MODULE bottle,SYS_RESULT_PACK* resultPackPtr);
//测量模块清洗维护
void CanMasterMeasureModuleInjectMaintainWhileReturn(BOTTLE_SELECT_MEASURE_MODULE bottle,SYS_RESULT_PACK* resultPackPtr);
//测量模块暗计数
void CanMasterMeasureModuleReadDarkWhileReturn(SYS_RESULT_PACK* resultPackPtr,uint32_t* darkValPtr);
//测量模块读本底
void CanMasterMeasureModuleReadBackGroundWhileReturn(SYS_RESULT_PACK* resultPackPtr,uint32_t* backGroundValPtr);
//测量模块注液测光
void CanMasterMeasureModuleReadLightWithInjectWhileReturn(BOTTLE_SELECT_MEASURE_MODULE bottle,SYS_RESULT_PACK* resultPackPtr,
                                                            uint32_t* lightValPtr);
//测量模块集成单次测量
void CanMasterMeasureModuleReadLightComplexWhileReturn(BOTTLE_SELECT_MEASURE_MODULE bottle,FunctionalState enableInject,
                                                        SYS_RESULT_PACK* resultPackPtr,uint32_t* backGroundValPtr,
                                                        uint32_t* lightValPtr);
//测量模块读光子变化率
void CanMasterMeasureModuleReadLightChangeRateWhileReturn(BOTTLE_SELECT_MEASURE_MODULE bottle,uint8_t setChangeCount,
                                                            SYS_RESULT_PACK* resultPackPtr,uint8_t* pointCountPtr,
                                                            uint32_t** lightArrayPtrPtr);

//释放注液测光点集合
void CanMasterMeasureModuleReleaseMem(void* ptr);


#endif


















