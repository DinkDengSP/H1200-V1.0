/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-24 13:00:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-01 14:44:54 +0800
************************************************************************************************/ 
#ifndef __CAN_MASTER_NEEDLE_SAMPLE_H_
#define __CAN_MASTER_NEEDLE_SAMPLE_H_
#include "CanMasterH360Base.h"

//样本针序号
typedef enum INDEX_MOTOR_NEEDLE_SAMPLE
{
    INDEX_MOTOR_NEEDLE_SAMPLE_UPDOWN  = 0X00,//升降
    INDEX_MOTOR_NEEDLE_SAMPLE_ROTATE  = 0X01,//旋转
    INDEX_MOTOR_NEEDLE_SAMPLE_PUMP    = 0X02,//注射泵        
}INDEX_MOTOR_NEEDLE_SAMPLE;

//样本针位置
typedef enum NEEDLE_SAMPLE_POS
{
    NEEDLE_SAMPLE_POS_CLEAN                 = 0X00,//样本针旋转到清洗位
    NEEDLE_SAMPLE_POS_FORCE_CLEAN           = 0X01,//样本针旋转到强力清洗位
    NEEDLE_SAMPLE_POS_TRACK_NORMAL          = 0X02,//样本针旋转到常规轨道吸样位
    NEEDLE_SAMPLE_POS_PLATE_REACTION_ABSORB = 0X03,//样本针旋转到反应盘吸稀释样本位
    NEEDLE_SAMPLE_POS_PLATE_REACTION_INJECT = 0X04,//样本针旋转到反应盘排样位
    NEEDLE_SAMPLE_POS_TRACK_EM              = 0X05,//样本针旋转到急诊轨道吸样位
}NEEDLE_SAMPLE_POS;

//试管类型
typedef enum TUBE_TYPE
{
    TUBE_TYPE_1                 = 0X00,//试管类型1
    TUBE_TYPE_BLOOD_COLLECT_5ML = 0X00,//5ML采血管
    TUBE_TYPE_2                 = 0X01,//试管类型2
    TUBE_TYPE_BLOOD_COLLECT_2ML = 0X01,//2ML采血管
    TUBE_TYPE_3                 = 0X02,//试管类型3
    TUBE_TYPE_CALIBRATOR        = 0X02,//校准品管
    TUBE_TYPE_4                 = 0X03,//试管类型4
    TUBE_TYPE_BULLET_CUP        = 0X03,//子弹头管
    TUBE_TYPE_5                 = 0X04,//试管类型5
    TUBE_TYPE_HITACHI_CUP       = 0X04,//日立杯管
    TUBE_TYPE_6                 = 0X05,//试管类型6
    TUBE_TYPE_FREEZE_THAW       = 0X05,//冻融管
    TUBE_TYPE_7                 = 0X06,//试管类型7
    TUBE_TYPE_8                 = 0X07,//试管类型8
    TUBE_TYPE_MAX               = TUBE_TYPE_8,
    TUBE_TYPE_NONE              = 0XFF,//未指定
}TUBE_TYPE;

//样本针试管吸液结果
typedef struct NEEDLE_SAMPLE_ABSORB_TUBE_MSG
{
    int32_t currentLiquidDetectPos;
    uint32_t pumpLiquidUnitCount;
    LIQUID_FUNC_RESULT liquidResultRerurn;
    LIQUID_NEEDLE_LIQUID_DETECT_RESULT needleLiquidDetectResult;
    uint16_t liquidDetectBaseVol;
    uint16_t liquidDetectNeedleVol;
}NEEDLE_SAMPLE_ABSORB_TUBE_MSG;

//样本针反应盘吸样结果
typedef struct NEEDLE_SAMPLE_ABSORB_PLATE_MSG
{
    uint32_t pumpLiquidUnitCount;
    int32_t needleDownPos;
}NEEDLE_SAMPLE_ABSORB_PLATE_MSG;



/*************************************发送指令等待ACK************************************************/
//模块内输入读取
void CanMasterNeedleSampleReadSingleWhileAck(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr);
//模块内输出写入
void CanMasterNeedleSampleWriteSingleWhileAck(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterNeedleSampleResetMotorWhileAck(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr);
//模块内电机走位
void CanMasterNeedleSampleRunStepsMotorWhileAck(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr);
//样本针复位
void CanMasterNeedleSampleResetWhileAck(SYS_RESULT_PACK* resultPackPtr);
//样本针清洗液灌注
void CanMasterNeedleSampleNormalPrimeWhileAck(NEEDLE_PRIME_OPT opt,SYS_RESULT_PACK* resultPackPtr);
//样本针强力清洗液灌注
void CanMasterNeedleSamplePowerPrimeWhileAck(SYS_RESULT_PACK* resultPackPtr);
//样本针清洗
void CanMasterNeedleSampleNormalCleanWhileAck(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time,SYS_RESULT_PACK* resultPackPtr);
//样本针强力清洗
void CanMasterNeedleSamplePowerCleanWhileAck(uint8_t count,SYS_RESULT_PACK* resultPackPtr);
//样本针清洗维护
void CanMasterNeedleSampleMaintainCleanWhileAck(NEEDLE_MAINTAIN_OPT opt,NEEDLE_CLEAN_TIME time,SYS_RESULT_PACK* resultPackPtr);
//样本针旋转指定位置
void CanMasterNeedleSampleRotateWhileAck(NEEDLE_SAMPLE_POS targetPos,SYS_RESULT_PACK* resultPackPtr);
//样本针吸试管样本
void CanMasterNeedleSampleLiquidAbsorbSampleTubeWhileAck(uint32_t unitCount,int32_t lastLiquidDetectPos,TUBE_TYPE tubeType,
                                                            SYS_RESULT_PACK* resultPackPtr);
//样本针吸反应盘样本
void CanMasterNeedleSampleLiquidAbsorbPlateWhileAck(uint32_t unitCount,uint32_t cupLiquidResidueAmp1000,SYS_RESULT_PACK* resultPackPtr);
//样本针注液
void CanMasterNeedleSampleLiquidInjectWhileAck(uint32_t cupLiquidUlAmp1000,uint8_t mixCount,SYS_RESULT_PACK* resultPackPtr);
//样本针探液校准
void CanMasterNeedleSampleLiquidDetectAdjustWhileAck(SYS_RESULT_PACK* resultPackPtr);
//读样本针探液板状态
void CanMasterNeedleSampleLiquidDetectReadStateWhileAck(LIQUID_DETECT_CONFIG config,SYS_RESULT_PACK* resultPackPtr);
//读取电阻和针尖电压
void CanMasterNeedleSampleLiquidDetectReadResVolWhileAck(SYS_RESULT_PACK* resultPackPtr);
//写入电阻值
void CanMasterNeedleSampleLiquidDetectWriteResWhileAck(uint16_t resValue,SYS_RESULT_PACK* resultPackPtr);
//读取针尖基准电压
void CanMasterNeedleSampleLiquidDetectReadNeedleBaseVolWhileAck(SYS_RESULT_PACK* resultPackPtr);
//读样本针升降急停高度
void CanMasterNeedleSampleReadNeedleUpDownEmStopPosWhileAck(SYS_RESULT_PACK* resultPackPtr);
//读探液板程序版本
void CanMasterNeedleSampleReadLiquidBoardVersionWhileAck(SYS_RESULT_PACK* resultPackPtr);

/*************************************等待指定指令完成返回************************************************/
//模块内输入读取
void CanMasterNeedleSampleWaitReadSingleResult(SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr);
//模块内输出写入
void CanMasterNeedleSampleWaitWriteSingleResult(SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterNeedleSampleWaitResetMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//模块内电机走位
void CanMasterNeedleSampleWaitRunStepsMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//样本针复位
void CanMasterNeedleSampleWaitResetResult(SYS_RESULT_PACK* resultPackPtr);
//样本针清洗液灌注
void CanMasterNeedleSampleWaitNormalPrimeResult(SYS_RESULT_PACK* resultPackPtr);
//样本针强力清洗液灌注
void CanMasterNeedleSampleWaitPowerPrimeResult(SYS_RESULT_PACK* resultPackPtr);
//样本针清洗
void CanMasterNeedleSampleWaitNormalCleanResult(SYS_RESULT_PACK* resultPackPtr);
//样本针强力清洗
void CanMasterNeedleSampleWaitPowerCleanResult(SYS_RESULT_PACK* resultPackPtr);
//样本针清洗维护
void CanMasterNeedleSampleWaitMaintainCleanResult(SYS_RESULT_PACK* resultPackPtr);
//样本针旋转指定位置
void CanMasterNeedleSampleWaitRotateResult(SYS_RESULT_PACK* resultPackPtr);
//样本针吸试管样本
void CanMasterNeedleSampleWaitLiquidAbsorbSampleTubeResult(SYS_RESULT_PACK* resultPackPtr,NEEDLE_SAMPLE_ABSORB_TUBE_MSG* absorbMsgPtr);
//样本针吸反应盘样本
void CanMasterNeedleSampleWaitLiquidAbsorbPlateResult(SYS_RESULT_PACK* resultPackPtr,NEEDLE_SAMPLE_ABSORB_PLATE_MSG* absorbMsgPtr);
//样本针注液
void CanMasterNeedleSampleWaitLiquidInjectResult(SYS_RESULT_PACK* resultPackPtr);
//样本针探液校准
void CanMasterNeedleSampleWaitLiquidDetectAdjustResult(SYS_RESULT_PACK* resultPackPtr);
//读样本针探液板状态
void CanMasterNeedleSampleWaitLiquidDetectReadStateResult(SYS_RESULT_PACK* resultPackPtr,uint16_t* needleVol,uint16_t* thresholdValue,
                                                            uint16_t* slopeValue);
//读取电阻和针尖电压
void CanMasterNeedleSampleWaitLiquidDetectReadResVolResult(SYS_RESULT_PACK* resultPackPtr,uint16_t* resValue,uint16_t* volValue);
//写入电阻值
void CanMasterNeedleSampleWaitLiquidDetectWriteResResult(SYS_RESULT_PACK* resultPackPtr);
//读取针尖基准电压
void CanMasterNeedleSampleWaitLiquidDetectReadNeedleBaseVolResult(SYS_RESULT_PACK* resultPackPtr,uint16_t* needleBaseVol);
//读样本针升降急停高度
void CanMasterNeedleSampleWaitReadNeedleUpDownEmStopPosResult(SYS_RESULT_PACK* resultPackPtr,int32_t* needleEmStopPosPtr);
//读探液板程序版本
void CanMasterNeedleSampleWaitReadLiquidBoardVersionResult(SYS_RESULT_PACK* resultPackPtr,uint8_t* versionStrLengthPtr,uint8_t** versionCodeBufPtrPtr);

/*************************************发送指令等待完成返回************************************************/
//模块内输入读取
void CanMasterNeedleSampleReadSingleWhileReturn(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr);
//模块内输出写入
void CanMasterNeedleSampleWriteSingleWhileReturn(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr);
//模块内电机复位
void CanMasterNeedleSampleResetMotorWhileReturn(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//模块内电机走位
void CanMasterNeedleSampleRunStepsMotorWhileReturn(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr);
//样本针复位
void CanMasterNeedleSampleResetWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//样本针清洗液灌注
void CanMasterNeedleSampleNormalPrimeWhileReturn(NEEDLE_PRIME_OPT opt,SYS_RESULT_PACK* resultPackPtr);
//样本针强力清洗液灌注
void CanMasterNeedleSamplePowerPrimeWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//样本针清洗
void CanMasterNeedleSampleNormalCleanWhileReturn(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time,SYS_RESULT_PACK* resultPackPtr);
//样本针强力清洗
void CanMasterNeedleSamplePowerCleanWhileReturn(uint8_t count,SYS_RESULT_PACK* resultPackPtr);
//样本针清洗维护
void CanMasterNeedleSampleMaintainCleanWhileReturn(NEEDLE_MAINTAIN_OPT opt,NEEDLE_CLEAN_TIME time,SYS_RESULT_PACK* resultPackPtr);
//样本针旋转指定位置
void CanMasterNeedleSampleRotateWhileReturn(NEEDLE_SAMPLE_POS targetPos,SYS_RESULT_PACK* resultPackPtr);
//样本针吸试管样本
void CanMasterNeedleSampleLiquidAbsorbSampleTubeWhileReturn(uint32_t unitCount,int32_t lastLiquidDetectPos,TUBE_TYPE tubeType,
                                                            SYS_RESULT_PACK* resultPackPtr,NEEDLE_SAMPLE_ABSORB_TUBE_MSG* absorbMsgPtr);
//样本针吸反应盘样本
void CanMasterNeedleSampleLiquidAbsorbPlateWhileReturn(uint32_t unitCount,uint32_t cupLiquidResidueAmp1000,SYS_RESULT_PACK* resultPackPtr,
                                                            NEEDLE_SAMPLE_ABSORB_PLATE_MSG* absorbMsgPtr);
//样本针注液
void CanMasterNeedleSampleLiquidInjectWhileReturn(uint32_t cupLiquidUlAmp1000,uint8_t mixCount,SYS_RESULT_PACK* resultPackPtr);
//样本针探液校准
void CanMasterNeedleSampleLiquidDetectAdjustWhileReturn(SYS_RESULT_PACK* resultPackPtr);
//读样本针探液板状态
void CanMasterNeedleSampleLiquidDetectReadStateWhileReturn(LIQUID_DETECT_CONFIG config,SYS_RESULT_PACK* resultPackPtr,uint16_t* needleVol,
                                                            uint16_t* thresholdValue,uint16_t* slopeValue);
//读取电阻和针尖电压
void CanMasterNeedleSampleLiquidDetectReadResVolWhileReturn(SYS_RESULT_PACK* resultPackPtr,uint16_t* resValue,uint16_t* volValue);
//写入电阻值
void CanMasterNeedleSampleLiquidDetectWriteResWhileReturn(uint16_t resValue,SYS_RESULT_PACK* resultPackPtr);
//读取针尖基准电压
void CanMasterNeedleSampleLiquidDetectReadNeedleBaseVolWhileReturn(SYS_RESULT_PACK* resultPackPtr,uint16_t* needleBaseVol);
//读样本针升降急停高度
void CanMasterNeedleSampleReadNeedleUpDownEmStopPosWhileReturn(SYS_RESULT_PACK* resultPackPtr,int32_t* needleEmStopPosPtr);
//读探液板程序版本
void CanMasterNeedleSampleReadLiquidBoardVersionWhileReturn(SYS_RESULT_PACK* resultPackPtr,uint8_t* versionStrLengthPtr,uint8_t** versionCodeBufPtrPtr);


//释放内存
void CanMasterNeedleSampleFreeBuffer(void* bufferPtr);



#endif

















