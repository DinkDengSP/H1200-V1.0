/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-22 09:20:50 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-11 14:01:37 +0800
************************************************************************************************/ 
#ifndef __CAN_MASTER_NEEDLE_BEAD_H_
#define __CAN_MASTER_NEEDLE_BEAD_H_
#include "CanMasterH360Base.h"

//磁珠针模块电机
typedef enum INDEX_MOTOR_NEEDLE_BEAD
{
    INDEX_MOTOR_NEEDLE_BEAD_UPDOWN  = 0X00,
    INDEX_MOTOR_NEEDLE_BEAD_ROTATE  = 0X01,
    INDEX_MOTOR_NEEDLE_BEAD_PUMP    = 0X02,        
}INDEX_MOTOR_NEEDLE_BEAD;

//磁珠针位置
typedef enum NEEDLE_BEAD_POS
{
    NEEDLE_BEAD_POS_CLEAN           = 0X00,//磁珠针旋转到清洗位
    NEEDLE_BEAD_POS_R1              = 0X01,//磁珠针旋转到R1
    NEEDLE_BEAD_POS_R2              = 0X02,//磁珠针旋转到R2
    NEEDLE_BEAD_POS_BEAD            = 0X03,//磁珠针旋转到BEAD
    NEEDLE_BEAD_POS_PLATE_REACTION  = 0X04,//磁珠针旋转到反应盘排样位
}NEEDLE_BEAD_POS;

//磁珠针吸液结果
typedef struct NEEDLE_BEAD_ABSORB_MSG
{
    int32_t currentLiquidDetectPos;
    uint32_t pumpLiquidUnitCount;
    LIQUID_FUNC_RESULT liquidResultRerurn;
    LIQUID_NEEDLE_LIQUID_DETECT_RESULT needleLiquidDetectResult;
    LIQUID_DETECT_STAGE liquidDetectStageReturn;
    uint16_t liquidDetectBaseVol;
    uint16_t liquidDetectNeedleVol;
}NEEDLE_BEAD_ABSORB_MSG;

//磁珠针余量检测结果
typedef struct NEEDLE_BEAD_RESIDUE_MSG
{
    int32_t currentLiquidDetectPos;
    LIQUID_FUNC_RESULT liquidResultRerurn;
    LIQUID_NEEDLE_LIQUID_DETECT_RESULT needleLiquidDetectResult;
}NEEDLE_BEAD_RESIDUE_MSG;

/*************************************发送指令等待ACK************************************************/
//模块内输入读取
void CanMasterNeedleBeadReadSingleWhileAck(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr);
//模块内输出写入
void CanMasterNeedleBeadWriteSingleWhileAck(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterNeedleBeadResetMotorWhileAck(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr);
//模块内电机走位
void CanMasterNeedleBeadRunStepsMotorWhileAck(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr);
//磁珠针复位,带零位偏移
void CanMasterNeedleBeadResetWhileAck(SYS_RESULT_PACK* resultPackPtr);
//磁珠针清洗
void CanMasterNeedleBeadCleanWhileAck(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time,SYS_RESULT_PACK* resultPackPtr);
//磁珠针灌注
void CanMasterNeedleBeadPrimeWhileAck(NEEDLE_PRIME_OPT opt,SYS_RESULT_PACK* resultPackPtr);
//磁珠针清洗维护
void CanMasterNeedleBeadMaintainCleanWhileAck(NEEDLE_MAINTAIN_OPT opt,NEEDLE_CLEAN_TIME time,SYS_RESULT_PACK* resultPackPtr);
//磁珠针旋转指定位置
void CanMasterNeedleBeadRotateWhileAck(NEEDLE_BEAD_POS targetPos,SYS_RESULT_PACK* resultPackPtr);
//磁珠针吸液
void CanMasterNeedleBeadLiquidAbsorbWhileAck(uint32_t unitCount,int32_t lastLiquidDetectPos,LIQUID_DETECT_STAGE liquidDetectStage,
                                                SYS_RESULT_PACK* resultPackPtr);
//磁珠针注液
void CanMasterNeedleBeadLiquidInjectWhileAck(uint32_t cupLiquidUlAmp1000,uint8_t mixCount,SYS_RESULT_PACK* resultPackPtr);
//磁珠针探液校准
void CanMasterNeedleBeadLiquidDetectAdjustWhileAck(SYS_RESULT_PACK* resultPackPtr);
//读磁珠针探液板状态
void CanMasterNeedleBeadLiquidDetectReadStateWhileAck(LIQUID_DETECT_CONFIG config,SYS_RESULT_PACK* resultPackPtr);
//磁珠针读取针尖电阻和电压
void CanMasterNeedleBeadLiquidDetectReadResVolWhileAck(SYS_RESULT_PACK* resultPackPtr);
//磁珠针写入探液板电阻
void CanMasterNeedleBeadLiquidDetectWriteResWhileAck(uint16_t resValue,SYS_RESULT_PACK* resultPackPtr);
//磁珠针读取针尖基准电压
void CanMasterNeedleBeadLiquidDetectReadNeedleBaseVolWhileAck(SYS_RESULT_PACK* resultPackPtr);
//磁珠针液量余量检查
void CanMasterNeedleBeadLiquidResidueCheckWhileAck(SYS_RESULT_PACK* resultPackPtr);
//磁珠针升降急停高度读取
void CanMasterNeedleBeadReadNeedleUpDownEmStopPosWhileAck(SYS_RESULT_PACK* resultPackPtr);
//磁珠针小磁珠吸吐混匀
void CanMasterNeedleBeadLittleBeadPumpMixWhileAck(uint32_t mixLiquidUnit,uint8_t mixCount,SYS_RESULT_PACK* resultPackPtr);
//读取探液板版本号
void CanMasterNeedleBeadReadLiquidBoardVersionWhileAck(SYS_RESULT_PACK* resultPackPtr);

/*************************************等待指定指令完成返回************************************************/
//模块内输入读取
void CanMasterNeedleBeadWaitReadSingleResult(SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr);
//模块内输出写入
void CanMasterNeedleBeadWaitWriteSingleResult(SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterNeedleBeadWaitResetMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//模块内电机走位
void CanMasterNeedleBeadWaitRunStepsMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//磁珠针复位,带零位偏移
void CanMasterNeedleBeadWaitResetResult(SYS_RESULT_PACK* resultPackPtr);
//磁珠针清洗
void CanMasterNeedleBeadWaitCleanResult(SYS_RESULT_PACK* resultPackPtr);
//磁珠针灌注
void CanMasterNeedleBeadWaitPrimeResult(SYS_RESULT_PACK* resultPackPtr);
//磁珠针清洗维护
void CanMasterNeedleBeadWaitMaintainCleanResult(SYS_RESULT_PACK* resultPackPtr);
//磁珠针旋转指定位置
void CanMasterNeedleBeadWaitRotateResult(SYS_RESULT_PACK* resultPackPtr);
//磁珠针吸液
void CanMasterNeedleBeadWaitLiquidAbsorbResult(SYS_RESULT_PACK* resultPackPtr,NEEDLE_BEAD_ABSORB_MSG* absorbMsgPtr);
//磁珠针注液
void CanMasterNeedleBeadWaitLiquidInjectResult(SYS_RESULT_PACK* resultPackPtr);
//磁珠针探液校准
void CanMasterNeedleBeadWaitLiquidDetectAdjustResult(SYS_RESULT_PACK* resultPackPtr);
//读磁珠针探液板状态
void CanMasterNeedleBeadWaitLiquidDetectReadStateResult(SYS_RESULT_PACK* resultPackPtr,uint16_t* needleVol,uint16_t* thresholdValue,
                                                            uint16_t* slopeValue);
//磁珠针读取针尖电阻和电压
void CanMasterNeedleBeadWaitLiquidDetectReadResVolResult(SYS_RESULT_PACK* resultPackPtr,uint16_t* resValue,uint16_t* volValue);
//磁珠针写入探液板电阻
void CanMasterNeedleBeadWaitLiquidDetectWriteResResult(SYS_RESULT_PACK* resultPackPtr);
//磁珠针读取针尖基准电压
void CanMasterNeedleBeadWaitLiquidDetectReadNeedleBaseVolResult(SYS_RESULT_PACK* resultPackPtr,uint16_t* needleBaseVol);
//磁珠针液量余量检查
void CanMasterNeedleBeadWaitLiquidResidueCheckResult(SYS_RESULT_PACK* resultPackPtr,NEEDLE_BEAD_RESIDUE_MSG* residueMsgPtr);
//磁珠针升降急停高度读取
void CanMasterNeedleBeadWaitReadNeedleUpDownEmStopPosResult(SYS_RESULT_PACK* resultPackPtr,int32_t* needleEmStopPosPtr);
//磁珠针小磁珠吸吐混匀
void CanMasterNeedleBeadWaitLittleBeadPumpMixResult(SYS_RESULT_PACK* resultPackPtr);
//读取探液板版本号
void CanMasterNeedleBeadWaitReadLiquidBoardVersionResult(SYS_RESULT_PACK* resultPackPtr,uint8_t* versionStrLengthPtr,uint8_t** versionCodeBufPtrPtr);

/*************************************发送指令等待完成返回************************************************/
//模块内输入读取
void CanMasterNeedleBeadReadSingleWhileReturn(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr);
//模块内输出写入
void CanMasterNeedleBeadWriteSingleWhileReturn(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterNeedleBeadResetMotorWhileReturn(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//模块内电机走位
void CanMasterNeedleBeadRunStepsMotorWhileReturn(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//磁珠针复位,带零位偏移
void CanMasterNeedleBeadResetWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//磁珠针清洗
void CanMasterNeedleBeadCleanWhileReturn(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time,SYS_RESULT_PACK* resultPackPtr);
//磁珠针灌注
void CanMasterNeedleBeadPrimeWhileReturn(NEEDLE_PRIME_OPT opt,SYS_RESULT_PACK* resultPackPtr);
//磁珠针清洗维护
void CanMasterNeedleBeadMaintainCleanWhileReturn(NEEDLE_MAINTAIN_OPT opt,NEEDLE_CLEAN_TIME time,SYS_RESULT_PACK* resultPackPtr);
//磁珠针旋转指定位置
void CanMasterNeedleBeadRotateWhileReturn(NEEDLE_BEAD_POS targetPos,SYS_RESULT_PACK* resultPackPtr);
//磁珠针吸液
void CanMasterNeedleBeadLiquidAbsorbWhileReturn(uint32_t unitCount,int32_t lastLiquidDetectPos,LIQUID_DETECT_STAGE liquidDetectStage,
                                                SYS_RESULT_PACK* resultPackPtr,NEEDLE_BEAD_ABSORB_MSG* absorbMsgPtr);
//磁珠针注液
void CanMasterNeedleBeadLiquidInjectWhileReturn(uint32_t cupLiquidUlAmp1000,uint8_t mixCount,SYS_RESULT_PACK* resultPackPtr);
//磁珠针探液校准
void CanMasterNeedleBeadLiquidDetectAdjustWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//读磁珠针探液板状态
void CanMasterNeedleBeadLiquidDetectReadStateWhileReturn(LIQUID_DETECT_CONFIG config,SYS_RESULT_PACK* resultPackPtr,uint16_t* needleVol,
                                                            uint16_t* thresholdValue,uint16_t* slopeValue);
//磁珠针读取针尖电阻和电压
void CanMasterNeedleBeadLiquidDetectReadResVolWhileReturn(SYS_RESULT_PACK* resultPackPtr,uint16_t* resValue,uint16_t* volValue);
//磁珠针写入探液板电阻
void CanMasterNeedleBeadLiquidDetectWriteResWhileReturn(uint16_t resValue,SYS_RESULT_PACK* resultPackPtr);
//磁珠针读取针尖基准电压
void CanMasterNeedleBeadLiquidDetectReadNeedleBaseVolWhileReturn(SYS_RESULT_PACK* resultPackPtr,uint16_t* needleBaseVol);
//磁珠针液量余量检查
void CanMasterNeedleBeadLiquidResidueCheckWhileReturn(SYS_RESULT_PACK* resultPackPtr,NEEDLE_BEAD_RESIDUE_MSG* residueMsgPtr);
//磁珠针升降急停高度读取
void CanMasterNeedleBeadReadNeedleUpDownEmStopPosWhileReturn(SYS_RESULT_PACK* resultPackPtr,int32_t* needleEmStopPosPtr);
//磁珠针小磁珠吸吐混匀
void CanMasterNeedleBeadLittleBeadPumpMixWhileReturn(uint32_t mixLiquidUnit,uint8_t mixCount,SYS_RESULT_PACK* resultPackPtr);
//读取探液板版本号
void CanMasterNeedleBeadReadLiquidBoardVersionWhileReturn(SYS_RESULT_PACK* resultPackPtr,uint8_t* versionStrLengthPtr,uint8_t** versionCodeBufPtrPtr);

//释放内存
void CanMasterNeedleBeadFreeBuffer(void* bufferPtr);

#endif













