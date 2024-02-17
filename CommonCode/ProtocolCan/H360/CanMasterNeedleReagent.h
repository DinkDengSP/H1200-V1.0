/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-24 13:00:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-01 16:04:53 +0800
************************************************************************************************/ 
#ifndef __CAN_MASTER_NEEDLE_REAGENT_H_
#define __CAN_MASTER_NEEDLE_REAGENT_H_
#include "CanMasterH360Base.h"

//试剂针模块电机
typedef enum INDEX_MOTOR_NEEDLE_REAGENT
{
    INDEX_MOTOR_NEEDLE_REAGENT_UPDOWN  = 0X00,//升降
    INDEX_MOTOR_NEEDLE_REAGENT_ROTATE  = 0X01,//旋转
    INDEX_MOTOR_NEEDLE_REAGENT_PUMP    = 0X02,//注射泵        
}INDEX_MOTOR_NEEDLE_REAGENT;

//试剂针位置
typedef enum NEEDLE_REAGENT_POS
{
    NEEDLE_REAGENT_POS_CLEAN            = 0X00,//试剂针旋转到清洗位
    NEEDLE_REAGENT_POS_R1               = 0X01,//试剂针旋转到R1
    NEEDLE_REAGENT_POS_R2               = 0X02,//试剂针旋转到R2
    NEEDLE_REAGENT_POS_PLATE_REACTION   = 0X03,//试剂针旋转到反应盘排样位
}NEEDLE_REAGENT_POS;

//试剂针吸液结果
typedef struct NEEDLE_REAGENT_ABSORB_MSG
{
    int32_t currentLiquidDetectPos;
    uint32_t pumpLiquidUnitCount;
    LIQUID_FUNC_RESULT liquidResultRerurn;
    LIQUID_NEEDLE_LIQUID_DETECT_RESULT needleLiquidDetectResult;
    LIQUID_DETECT_STAGE liquidDetectStageReturn;
    uint16_t liquidDetectBaseVol;
    uint16_t liquidDetectNeedleVol;
}NEEDLE_REAGENT_ABSORB_MSG;

//试剂针余量检测结果
typedef struct NEEDLE_REAGENT_RESIDUE_MSG
{
    int32_t currentLiquidDetectPos;
    LIQUID_FUNC_RESULT liquidResultRerurn;
    LIQUID_NEEDLE_LIQUID_DETECT_RESULT needleLiquidDetectResult;
}NEEDLE_REAGENT_RESIDUE_MSG;

/*************************************发送指令等待ACK************************************************/
//模块内输入读取
void CanMasterNeedleReagentReadSingleWhileAck(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr);
//模块内输出写入
void CanMasterNeedleReagentWriteSingleWhileAck(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterNeedleReagentResetMotorWhileAck(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr);
//模块内电机走位
void CanMasterNeedleReagentRunStepsMotorWhileAck(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr);
//试剂针复位,带零位偏移
void CanMasterNeedleReagentResetWhileAck(SYS_RESULT_PACK* resultPackPtr);
//试剂针清洗
void CanMasterNeedleReagentCleanWhileAck(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time,SYS_RESULT_PACK* resultPackPtr);
//试剂针灌注
void CanMasterNeedleReagentPrimeWhileAck(NEEDLE_PRIME_OPT opt,SYS_RESULT_PACK* resultPackPtr);
//试剂针清洗维护
void CanMasterNeedleReagentMaintainCleanWhileAck(NEEDLE_MAINTAIN_OPT opt,NEEDLE_CLEAN_TIME time,SYS_RESULT_PACK* resultPackPtr);
//试剂针旋转指定位置
void CanMasterNeedleReagentRotateWhileAck(NEEDLE_REAGENT_POS targetPos,SYS_RESULT_PACK* resultPackPtr);
//试剂针吸液
void CanMasterNeedleReagentLiquidAbsorbWhileAck(uint32_t unitCount,int32_t lastLiquidDetectPos,LIQUID_DETECT_STAGE liquidDetectStage,
                                                SYS_RESULT_PACK* resultPackPtr);
//试剂针注液
void CanMasterNeedleReagentLiquidInjectWhileAck(uint32_t cupLiquidUlAmp1000,uint8_t mixCount,SYS_RESULT_PACK* resultPackPtr);
//试剂针探液校准
void CanMasterNeedleReagentLiquidDetectAdjustWhileAck(SYS_RESULT_PACK* resultPackPtr);
//读试剂针探液板状态
void CanMasterNeedleReagentLiquidDetectReadStateWhileAck(LIQUID_DETECT_CONFIG config,SYS_RESULT_PACK* resultPackPtr);
//试剂针读取针尖电阻和电压
void CanMasterNeedleReagentLiquidDetectReadResVolWhileAck(SYS_RESULT_PACK* resultPackPtr);
//试剂针写入探液板电阻
void CanMasterNeedleReagentLiquidDetectWriteResWhileAck(uint16_t resValue,SYS_RESULT_PACK* resultPackPtr);
//试剂针读取针尖基准电压
void CanMasterNeedleReagentLiquidDetectReadNeedleBaseVolWhileAck(SYS_RESULT_PACK* resultPackPtr);
//试剂针液量余量检查
void CanMasterNeedleReagentLiquidResidueCheckWhileAck(SYS_RESULT_PACK* resultPackPtr);
//试剂针升降急停高度读取
void CanMasterNeedleReagentReadNeedleUpDownEmStopPosWhileAck(SYS_RESULT_PACK* resultPackPtr);
//读取探液板版本号
void CanMasterNeedleReagentReadLiquidBoardVersionWhileAck(SYS_RESULT_PACK* resultPackPtr);

/*************************************等待指定指令完成返回************************************************/
//模块内输入读取
void CanMasterNeedleReagentWaitReadSingleResult(SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr);
//模块内输出写入
void CanMasterNeedleReagentWaitWriteSingleResult(SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterNeedleReagentWaitResetMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//模块内电机走位
void CanMasterNeedleReagentWaitRunStepsMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//试剂针复位,带零位偏移
void CanMasterNeedleReagentWaitResetResult(SYS_RESULT_PACK* resultPackPtr);
//试剂针清洗
void CanMasterNeedleReagentWaitCleanResult(SYS_RESULT_PACK* resultPackPtr);
//试剂针灌注
void CanMasterNeedleReagentWaitPrimeResult(SYS_RESULT_PACK* resultPackPtr);
//试剂针清洗维护
void CanMasterNeedleReagentWaitMaintainCleanResult(SYS_RESULT_PACK* resultPackPtr);
//试剂针旋转指定位置
void CanMasterNeedleReagentWaitRotateResult(SYS_RESULT_PACK* resultPackPtr);
//试剂针吸液
void CanMasterNeedleReagentWaitLiquidAbsorbResult(SYS_RESULT_PACK* resultPackPtr,NEEDLE_REAGENT_ABSORB_MSG* absorbMsgPtr);
//试剂针注液
void CanMasterNeedleReagentWaitLiquidInjectResult(SYS_RESULT_PACK* resultPackPtr);
//试剂针探液校准
void CanMasterNeedleReagentWaitLiquidDetectAdjustResult(SYS_RESULT_PACK* resultPackPtr);
//读试剂针探液板状态
void CanMasterNeedleReagentWaitLiquidDetectReadStateResult(SYS_RESULT_PACK* resultPackPtr,uint16_t* needleVol,uint16_t* thresholdValue,
                                                            uint16_t* slopeValue);
//试剂针读取针尖电阻和电压
void CanMasterNeedleReagentWaitLiquidDetectReadResVolResult(SYS_RESULT_PACK* resultPackPtr,uint16_t* resValue,uint16_t* volValue);
//试剂针写入探液板电阻
void CanMasterNeedleReagentWaitLiquidDetectWriteResResult(SYS_RESULT_PACK* resultPackPtr);
//试剂针读取针尖基准电压
void CanMasterNeedleReagentWaitLiquidDetectReadNeedleBaseVolResult(SYS_RESULT_PACK* resultPackPtr,uint16_t* needleBaseVol);
//试剂针液量余量检查
void CanMasterNeedleReagentWaitLiquidResidueCheckResult(SYS_RESULT_PACK* resultPackPtr,NEEDLE_REAGENT_RESIDUE_MSG* residueMsgPtr);
//试剂针升降急停高度读取
void CanMasterNeedleReagentWaitReadNeedleUpDownEmStopPosResult(SYS_RESULT_PACK* resultPackPtr,int32_t* needleEmStopPosPtr);
//读取探液板版本号
void CanMasterNeedleReagentWaitReadLiquidBoardVersionResult(SYS_RESULT_PACK* resultPackPtr,uint8_t* versionStrLengthPtr,uint8_t** versionCodeBufPtrPtr);

/*************************************发送指令等待完成返回************************************************/
//模块内输入读取
void CanMasterNeedleReagentReadSingleWhileReturn(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr);
//模块内输出写入
void CanMasterNeedleReagentWriteSingleWhileReturn(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterNeedleReagentResetMotorWhileReturn(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//模块内电机走位
void CanMasterNeedleReagentRunStepsMotorWhileReturn(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//试剂针复位,带零位偏移
void CanMasterNeedleReagentResetWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//试剂针清洗
void CanMasterNeedleReagentCleanWhileReturn(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time,SYS_RESULT_PACK* resultPackPtr);
//试剂针灌注
void CanMasterNeedleReagentPrimeWhileReturn(NEEDLE_PRIME_OPT opt,SYS_RESULT_PACK* resultPackPtr);
//试剂针清洗维护
void CanMasterNeedleReagentMaintainCleanWhileReturn(NEEDLE_MAINTAIN_OPT opt,NEEDLE_CLEAN_TIME time,SYS_RESULT_PACK* resultPackPtr);
//试剂针旋转指定位置
void CanMasterNeedleReagentRotateWhileReturn(NEEDLE_REAGENT_POS targetPos,SYS_RESULT_PACK* resultPackPtr);
//试剂针吸液
void CanMasterNeedleReagentLiquidAbsorbWhileReturn(uint32_t unitCount,int32_t lastLiquidDetectPos,LIQUID_DETECT_STAGE liquidDetectStage,
                                                    SYS_RESULT_PACK* resultPackPtr,NEEDLE_REAGENT_ABSORB_MSG* absorbMsgPtr);
//试剂针注液
void CanMasterNeedleReagentLiquidInjectWhileReturn(uint32_t cupLiquidUlAmp1000,uint8_t mixCount,SYS_RESULT_PACK* resultPackPtr);
//试剂针探液校准
void CanMasterNeedleReagentLiquidDetectAdjustWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//读试剂针探液板状态
void CanMasterNeedleReagentLiquidDetectReadStateWhileReturn(LIQUID_DETECT_CONFIG config,SYS_RESULT_PACK* resultPackPtr,uint16_t* needleVol,
                                                                uint16_t* thresholdValue,uint16_t* slopeValue);
//试剂针读取针尖电阻和电压
void CanMasterNeedleReagentLiquidDetectReadResVolWhileReturn(SYS_RESULT_PACK* resultPackPtr,uint16_t* resValue,uint16_t* volValue);
//试剂针写入探液板电阻
void CanMasterNeedleReagentLiquidDetectWriteResWhileReturn(uint16_t resValue,SYS_RESULT_PACK* resultPackPtr);
//试剂针读取针尖基准电压
void CanMasterNeedleReagentLiquidDetectReadNeedleBaseVolWhileReturn(SYS_RESULT_PACK* resultPackPtr,uint16_t* needleBaseVol);
//试剂针液量余量检查
void CanMasterNeedleReagentLiquidResidueCheckWhileReturn(SYS_RESULT_PACK* resultPackPtr,NEEDLE_REAGENT_RESIDUE_MSG* residueMsgPtr);
//试剂针升降急停高度读取
void CanMasterNeedleReagentReadNeedleUpDownEmStopPosWhileReturn(SYS_RESULT_PACK* resultPackPtr,int32_t* needleEmStopPosPtr);
//读取探液板版本号
void CanMasterNeedleReagentReadLiquidBoardVersionWhileReturn(SYS_RESULT_PACK* resultPackPtr,uint8_t* versionStrLengthPtr,uint8_t** versionCodeBufPtrPtr);


//释放内存
void CanMasterNeedleReagentFreeBuffer(void* bufferPtr);

#endif





















