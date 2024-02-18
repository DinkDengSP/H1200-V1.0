/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-24 13:00:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-04 18:10:06 +0800
************************************************************************************************/ 
#ifndef __CAN_MASTER_LIQUID_AUTO_H_
#define __CAN_MASTER_LIQUID_AUTO_H_
#include "CanMasterH360Base.h"

//液路自动化泵阀开放状态
typedef enum DILUTION_CH
{
    DILUTION_CH_TRANSFER_BOTTLE_INJECT_PURE_WATER              = 0X00,//中转瓶注纯水
    DILUTION_CH_TRANSFER_BOTTLE_INJECT_CONCENTRATE_BOTTLE_A    = 0X01,//中转瓶注浓缩液A
    DILUTION_CH_TRANSFER_BOTTLE_INJECT_CONCENTRATE_BOTTLE_B    = 0x02,//中转瓶注浓缩液B
    DILUTION_CH_TRANSFER_BOTTLE_TO_TARGET_BOTTLE               = 0X03,//从中转瓶往目标瓶转移
}DILUTION_CH;

//液路自动化状态
typedef enum LIQUID_AUTOMATIC_STATE
{
    LIQUID_AUTOMATIC_STATE_OFF  = 0X00,//关闭液路自动化功能
    LIQUID_AUTOMATIC_STATE_ON   = 0X01,//打开液路自动化功能
}LIQUID_AUTOMATIC_STATE;

//当前抽废液状态
typedef enum STAGE_LIQUID_WASTE
{
    STAGE_LIQUID_WASTE_OPEN_NORMAL      = 0x01,//抽废液正常打开状态
    STAGE_LIQUID_WASTE_OPEN_WARNING     = 0x02,//抽废液紧急打开状态
    STAGE_LIQUID_WASTE_CLOSE            = 0x03,//抽废液关闭状态
}STAGE_LIQUID_WASTE;

/*************************************发送指令等待ACK************************************************/
//模块内输入读取
void CanMasterLiquidAutoReadSingleWhileAck(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr);
//模块内输出写入
void CanMasterLiquidAutoWriteSingleWhileAck(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterLiquidAutoResetMotorWhileAck(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr);
//模块内电机走位
void CanMasterLiquidAutoRunStepsMotorWhileAck(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr);
//初始化
void CanMasterLiquidAutoInitWhileAck(SYS_RESULT_PACK* resultPackPtr);
//强制排一次废液
void CanMasterLiquidAutoForceTrigWasteLiquidWhileAck(SYS_RESULT_PACK* resultPackPtr);
//自动排废液打开
void CanMasterLiquidAutoEnableWasteLiquidAutomaticWhileAck(SYS_RESULT_PACK* resultPackPtr);
//自动排废液关闭
void CanMasterLiquidAutoDisableWasteLiquidAutomaticWhileAck(SYS_RESULT_PACK* resultPackPtr);
//查看排废液状态
void CanMasterLiquidAutoCheckStateWasteLiquidWhileAck(SYS_RESULT_PACK* resultPackPtr);
//关闭稀释
void CanMasterLiquidAutoCloseLiquidDilutionThreeNeedleCleanWhileAck(SYS_RESULT_PACK* resultPackPtr);
//打开稀释
void CanMasterLiquidAutoOpenLiquidDilutionThreeNeedleCleanWhileAck(DILUTION_CH channel,SYS_RESULT_PACK* resultPackPtr);
//读取自动稀释配置瓶纯水脉冲数
void CanMasterLiquidAutoGetLiquidDilutionPureWaterPulseWhileAck(SYS_RESULT_PACK* resultPackPtr);
//清除自动稀释配置瓶纯水脉冲数
void CanMasterLiquidAutoResetLiquidDilutionPureWaterPulseWhileAck(SYS_RESULT_PACK* resultPackPtr);
//读取自动稀释配置瓶纯水触发标记
void CanMasterLiquidAutoGetLiquidDilutionPureWaterTrigFlagWhileAck(SYS_RESULT_PACK* resultPackPtr);
//注册自动稀释配置瓶纯水脉冲数
void CanMasterLiquidAutoRegisterLiquidDilutionPureWaterPulseWithAtuoOffWhileAck(uint32_t trigCount,SYS_RESULT_PACK* resultPackPtr);

/*************************************等待指定指令完成返回************************************************/
//模块内输入读取
void CanMasterLiquidAutoWaitReadSingleResult(SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr);
//模块内输出写入
void CanMasterLiquidAutoWaitWriteSingleResult(SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterLiquidAutoWaitResetMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//模块内电机走位
void CanMasterLiquidAutoWaitRunStepsMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//初始化
void CanMasterLiquidAutoWaitInitResult(SYS_RESULT_PACK* resultPackPtr);
//强制排一次废液
void CanMasterLiquidAutoWaitForceTrigWasteLiquidResult(SYS_RESULT_PACK* resultPackPtr);
//自动排废液打开
void CanMasterLiquidAutoWaitEnableWasteLiquidAutomaticResult(SYS_RESULT_PACK* resultPackPtr);
//自动排废液关闭
void CanMasterLiquidAutoWaitDisableWasteLiquidAutomaticResult(SYS_RESULT_PACK* resultPackPtr);
//查看排废液状态
void CanMasterLiquidAutoWaitCheckStateWasteLiquidResult(SYS_RESULT_PACK* resultPackPtr,LIQUID_AUTOMATIC_STATE* statePtr);
//关闭稀释
void CanMasterLiquidAutoWaitCloseLiquidDilutionThreeNeedleCleanResult(SYS_RESULT_PACK* resultPackPtr);
//打开稀释
void CanMasterLiquidAutoWaitOpenLiquidDilutionThreeNeedleCleanResult(SYS_RESULT_PACK* resultPackPtr);
//读取自动稀释配置瓶纯水脉冲数
void CanMasterLiquidAutoGetLiquidDilutionPureWaterPulseResult(SYS_RESULT_PACK* resultPackPtr,uint32_t* liquidCount);
//清除自动稀释配置瓶纯水脉冲数
void CanMasterLiquidAutoResetLiquidDilutionPureWaterPulseResult(SYS_RESULT_PACK* resultPackPtr);
//读取自动稀释配置瓶纯水触发标记
void CanMasterLiquidAutoGetLiquidDilutionPureWaterTrigFlagResult(SYS_RESULT_PACK* resultPackPtr,uint8_t* trigFlag);
//注册自动稀释配置瓶纯水脉冲数
void CanMasterLiquidAutoRegisterLiquidDilutionPureWaterPulseWithAtuoOffResult(SYS_RESULT_PACK* resultPackPtr);

/*************************************发送指令等待完成返回************************************************/
//模块内输入读取
void CanMasterLiquidAutoReadSingleWhileReturn(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr);
//模块内输出写入
void CanMasterLiquidAutoWriteSingleWhileReturn(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterLiquidAutoResetMotorWhileReturn(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//模块内电机走位
void CanMasterLiquidAutoRunStepsMotorWhileReturn(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//初始化
void CanMasterLiquidAutoInitWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//强制排一次废液
void CanMasterLiquidAutoForceTrigWasteLiquidWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//自动排废液打开
void CanMasterLiquidAutoEnableWasteLiquidAutomaticWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//自动排废液关闭
void CanMasterLiquidAutoDisableWasteLiquidAutomaticWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//查看排废液状态
void CanMasterLiquidAutoCheckStateWasteLiquidWhileReturn(SYS_RESULT_PACK* resultPackPtr,LIQUID_AUTOMATIC_STATE* statePtr);
//关闭稀释
void CanMasterLiquidAutoCloseLiquidDilutionThreeNeedleCleanWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//打开稀释
void CanMasterLiquidAutoOpenLiquidDilutionThreeNeedleCleanWhileReturn(DILUTION_CH channel,SYS_RESULT_PACK* resultPackPtr);
//读取自动稀释配置瓶纯水脉冲数
void CanMasterLiquidAutoGetLiquidDilutionPureWaterPulseWhileReturn(SYS_RESULT_PACK* resultPackPtr,uint32_t* liquidCount);
//清除自动稀释配置瓶纯水脉冲数
void CanMasterLiquidAutoResetLiquidDilutionPureWaterPulseWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//读取自动稀释配置瓶纯水触发标记
void CanMasterLiquidAutoGetLiquidDilutionPureWaterTrigFlagWhileReturn(SYS_RESULT_PACK* resultPackPtr,uint8_t* trigFlag);
//注册自动稀释配置瓶纯水脉冲数
void CanMasterLiquidAutoRegisterLiquidDilutionPureWaterPulseWithAtuoOffWhileReturn(uint32_t trigCount,SYS_RESULT_PACK* resultPackPtr);


#endif
















