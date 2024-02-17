/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-24 13:00:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-06 09:18:39 +0800
************************************************************************************************/ 
#ifndef __CAN_MASTER_LIQUID_EXCITA_H_
#define __CAN_MASTER_LIQUID_EXCITA_H_
#include "CanMasterH360Base.h"

//激发液排废液状态
typedef enum LIQUID_EXCITE_WASTE_STATE
{
    LIQUID_EXCITE_WASTE_STATE_OFF,
    LIQUID_EXCITE_WASTE_STATE_ON,
}LIQUID_EXCITE_WASTE_STATE;

//激发液注液端口选择
typedef enum BOTTLE_LIQUID_EXCITE
{
    BOTTLE_LIQUID_EXCITE_A = 0X00,//激发液A瓶
    BOTTLE_LIQUID_EXCITE_B = 0X01,//激发液B瓶
}BOTTLE_LIQUID_EXCITE;

/*************************************发送指令等待ACK************************************************/
//模块内输入读取
void CanMasterLiquidExcitaReadSingleWhileAck(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr);
//模块内输出写入
void CanMasterLiquidExcitaWriteSingleWhileAck(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterLiquidExcitaResetMotorWhileAck(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr);
//模块内电机走位
void CanMasterLiquidExcitaRunStepsMotorWhileAck(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr);
//激发液控制复位
void CanMasterLiquidExcitaResetWhileAck(SYS_RESULT_PACK* resultPackPtr);
//打开常规吸液通道
void CanMasterLiquidExcitaOpenNormalAbsorbWhileAck(BOTTLE_LIQUID_EXCITE bottle,SYS_RESULT_PACK* resultPackPtr);
//打开维护吸液通道
void CanMasterLiquidExcitaOpenMaintainAbsorbWhileAck(SYS_RESULT_PACK* resultPackPtr);
//关闭液路通道
void CanMasterLiquidExcitaCloseWhileAck(SYS_RESULT_PACK* resultPackPtr);
//打开注液通道
void CanMasterLiquidExcitaOpenInject(SYS_RESULT_PACK* resultPackPtr);
//设置排放废液
void CanMasterLiquidExcitaSetWasteLiquidWhileAck(LIQUID_EXCITE_WASTE_STATE state,SYS_RESULT_PACK* resultPackPtr);

/*************************************等待指定指令完成返回************************************************/
//模块内输入读取
void CanMasterLiquidExcitaWaitReadSingleResult(SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr);
//模块内输出写入
void CanMasterLiquidExcitaWaitWriteSingleResult(SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterLiquidExcitaWaitResetMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//模块内电机走位
void CanMasterLiquidExcitaWaitRunStepsMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//激发液控制复位
void CanMasterLiquidExcitaWaitResetResult(SYS_RESULT_PACK* resultPackPtr);
//打开常规吸液通道
void CanMasterLiquidExcitaWaitOpenSelectInjectResult(SYS_RESULT_PACK* resultPackPtr);
//打开维护吸液通道
void CanMasterLiquidExcitaWaitOpenSelectMaintainResult(SYS_RESULT_PACK* resultPackPtr);
//关闭液路通道
void CanMasterLiquidExcitaWaitCloseResult(SYS_RESULT_PACK* resultPackPtr);
//打开注液通道
void CanMasterLiquidExcitaWaitOpenInjectResult(SYS_RESULT_PACK* resultPackPtr);
//设置排放废液
void CanMasterLiquidExcitaWaitSetWasteLiquidResult(SYS_RESULT_PACK* resultPackPtr);

/*************************************发送指令等待完成返回************************************************/
//模块内输入读取
void CanMasterLiquidExcitaReadSingleWhileReturn(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr);
//模块内输出写入
void CanMasterLiquidExcitaWriteSingleWhileReturn(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterLiquidExcitaResetMotorWhileReturn(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//模块内电机走位
void CanMasterLiquidExcitaRunStepsMotorWhileReturn(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//激发液控制复位
void CanMasterLiquidExcitaResetWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//打开常规吸液通道
void CanMasterLiquidExcitaOpenNormalAbsorbWhileReturn(BOTTLE_LIQUID_EXCITE bottle,SYS_RESULT_PACK* resultPackPtr);
//打开维护吸液通道
void CanMasterLiquidExcitaOpenMaintainAbsorbWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//关闭液路通道
void CanMasterLiquidExcitaCloseWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//打开注液通道
void CanMasterLiquidExcitaOpenInjectWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//设置排放废液
void CanMasterLiquidExcitaSetWasteLiquidWhileReturn(LIQUID_EXCITE_WASTE_STATE state,SYS_RESULT_PACK* resultPackPtr);























#endif





















