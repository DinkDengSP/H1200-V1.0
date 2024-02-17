/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-29 14:07:30 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-22 15:11:23 +0800
************************************************************************************************/ 
#include "SysParam.h"
#include "DriverConfigSystem.h"
#include "DriverHeaderSystem.h"
#include "SrvTaskHeader.h"

SYS_PARAM* sysParamPtr = NULL;

//App写入单个参数的指针
typedef ERROR_SUB (*SysParamWriteParamSingleFuncPtr)(uint8_t subIndex,int32_t writeParam);
//App读取单个参数的指针
typedef ERROR_SUB (*SysParamReadParamSingleFuncPtr)(uint8_t subIndex,int32_t* readParam);

//初始化系统参数为默认参数
static void SysParamSetDefault(void);

//初始化系统参数
void SysParamInit(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //首先申请内存
    sysParamPtr = NULL;
    //申请内存
    sysParamPtr = UserMemMallocWhileSuccess(MEM_REGION_SYS_PARAM,LENGTH_SYS_PARAM);
    //从Flash中加载参数
    errorSub = BOARD_PARAM_READ(ADDR_BOARD_SYS_PARAM_START,(uint8_t*)sysParamPtr,LENGTH_SYS_PARAM);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,BOARD_PARAM_READ Failed: 0X%08X\r\n",__func__,errorSub);
        while(1);
    }
    //数据校验
    if(sysParamPtr->paramFlag == SYS_PARAM_DEFAULT_FLAG)
    {
        //校验通过
        return;
    }
    //校验不通过,设定为默认配置
    SysParamSetDefault();
}


//初始化系统参数为默认参数
static void SysParamSetDefault(void)
{
    //设定为默认配置
    sysParamPtr->paramFlag = SYS_PARAM_DEFAULT_FLAG;
    //试剂盘参数
    UserMemCopy((void*)(&(sysParamPtr->plateReagentParam)),(void*)(&plateReagentParamDefault),LENGTH_SYS_PARAM_PLATE_REAGENT);
    //杯栈管理参数
    UserMemCopy((void*)(&(sysParamPtr->cupTrayManageParam)),(void*)(&cupTrayManageParamDefault),LENGTH_SYS_PARAM_CUP_TRAY_MANAGE);
    //磁珠针参数
    UserMemCopy((void*)(&(sysParamPtr->needleBeadParam)),(void*)(&needleBeadParamDefault),LENGTH_SYS_PARAM_NEEDLE_BEAD);
    //辅助参数
    UserMemCopy((void*)(&(sysParamPtr->otherUtilParam)),(void*)(&otherUtilParamDefault),LENGTH_SYS_PARAM_OTHER);
    //将设定好的值写回外置Flash
    BOARD_PARAM_WRITE(ADDR_BOARD_SYS_PARAM_START,(uint8_t*)sysParamPtr,LENGTH_SYS_PARAM);
}

//通用的写入Flash的函数
static void SysParamWriteSingleCommon(int32_t* targetParamPtr,int32_t writeParam)
{
    *targetParamPtr = writeParam;
    int32_t targetAddr = (int32_t)((char*)targetParamPtr);
    int32_t baseAddr = (int32_t)((char*)(sysParamPtr));
    //计算偏移
    int32_t offset = targetAddr - baseAddr;
    if(offset < 0)
    {
        //偏移必须是正数
        offset = 0 - offset;
    }
    //写入参数
    BOARD_PARAM_WRITE(ADDR_BOARD_SYS_PARAM_START+offset,(uint8_t*)targetParamPtr,4);
}

/*----------------------------------------------------------------------------------------------------*/
//读写试剂盘参数集合
ERROR_SUB SysParamReadParamPlateReagent(uint8_t subIndex,int32_t* readParam)
{
    switch(subIndex)
    {
        case INDEX_SUB_PARAM_PLATE_REAGENT_ROTATE_RESET_CORRECT:
            *readParam = sysParamPtr->plateReagentParam.rotateResetCorrect;
            break;
        case INDEX_SUB_PARAM_PLATE_REAGENT_OFFSET_NEEDLE_REAGENT_WITH_R1:
            *readParam = sysParamPtr->plateReagentParam.offsetNeedleReagentWithR1;
            break;
        case INDEX_SUB_PARAM_PLATE_REAGENT_OFFSET_NEEDLE_REAGENT_WITH_R2:
            *readParam = sysParamPtr->plateReagentParam.offsetNeedleReagentWithR2;
            break;
        case INDEX_SUB_PARAM_PLATE_REAGENT_OFFSET_NEEDLE_BEAD_WITH_R1:
            *readParam = sysParamPtr->plateReagentParam.offsetNeedleBeadWithR1;
            break;
        case INDEX_SUB_PARAM_PLATE_REAGENT_OFFSET_NEEDLE_BEAD_WITH_R2:
            *readParam = sysParamPtr->plateReagentParam.offsetNeedleBeadWithR2;
            break;
        case INDEX_SUB_PARAM_PLATE_REAGENT_OFFSET_NEEDLE_BEAD_WITH_BEAD:
            *readParam = sysParamPtr->plateReagentParam.offsetNeedleBeadWithBead;
            break;
        case INDEX_SUB_PARAM_PLATE_REAGENT_OFFSET_WITH_QR_SCAN:
            *readParam = sysParamPtr->plateReagentParam.offsetWithQRScan;
            break;
        default://不支持的参数
            *readParam = 0;
            return ERROR_SUB_SYS_PARAM_READ_SUB_INDEX;
    }
    //返回结果
    return ERROR_SUB_OK;
}

ERROR_SUB SysParamWriteParamPlateReagent(uint8_t subIndex,int32_t writeParam)
{
    int32_t* targetParamPtr = NULL;
    switch(subIndex)
    {
        case INDEX_SUB_PARAM_PLATE_REAGENT_ROTATE_RESET_CORRECT:
            targetParamPtr = &(sysParamPtr->plateReagentParam.rotateResetCorrect);
            break;
        case INDEX_SUB_PARAM_PLATE_REAGENT_OFFSET_NEEDLE_REAGENT_WITH_R1:
            targetParamPtr = &(sysParamPtr->plateReagentParam.offsetNeedleReagentWithR1);
            break;
        case INDEX_SUB_PARAM_PLATE_REAGENT_OFFSET_NEEDLE_REAGENT_WITH_R2:
            targetParamPtr = &(sysParamPtr->plateReagentParam.offsetNeedleReagentWithR2);
            break;
        case INDEX_SUB_PARAM_PLATE_REAGENT_OFFSET_NEEDLE_BEAD_WITH_R1:
            targetParamPtr = &(sysParamPtr->plateReagentParam.offsetNeedleBeadWithR1);
            break;
        case INDEX_SUB_PARAM_PLATE_REAGENT_OFFSET_NEEDLE_BEAD_WITH_R2:
            targetParamPtr = &(sysParamPtr->plateReagentParam.offsetNeedleBeadWithR2);
            break;
        case INDEX_SUB_PARAM_PLATE_REAGENT_OFFSET_NEEDLE_BEAD_WITH_BEAD:
            targetParamPtr = &(sysParamPtr->plateReagentParam.offsetNeedleBeadWithBead);
            break;
        case INDEX_SUB_PARAM_PLATE_REAGENT_OFFSET_WITH_QR_SCAN:
            targetParamPtr = &(sysParamPtr->plateReagentParam.offsetWithQRScan);
            break;
        default:
            return ERROR_SUB_SYS_PARAM_WRITE_SUB_INDEX;//不支持的参数
    }
    //参数写入Flash
    SysParamWriteSingleCommon(targetParamPtr,writeParam);
    //返回结果
    return ERROR_SUB_OK;
}
/*----------------------------------------------------------------------------------------------------*/
//读写杯栈管理参数集合
ERROR_SUB SysParamReadParamCupTrayManage(uint8_t subIndex,int32_t* readParam)
{
    switch(subIndex)
    {
        case INDEX_SUB_PARAM_CUP_TRAY_MANAGE_NEW_CUP_TRAY_RESET_CORRECT:
            *readParam = sysParamPtr->cupTrayManageParam.newCupTrayStackResetCorrect;
            break;
        case INDEX_SUB_PARAM_CUP_TRAY_MANAGE_EMPTY_CUP_TRAY_RESET_CORRECT:
            *readParam = sysParamPtr->cupTrayManageParam.emptyCupTrayStackResetCorrect;
            break;
        case INDEX_SUB_PARAM_CUP_TRAY_MANAGE_CLAMP_RESET_CORRECT:
            *readParam = sysParamPtr->cupTrayManageParam.clampResetCorrect;
            break;
        case INDEX_SUB_PARAM_CUP_TRAY_MANAGE_PUSH_RESET_CORRECT:
            *readParam = sysParamPtr->cupTrayManageParam.pushResetCorrect;
            break;
        case INDEX_SUB_PARAM_CUP_TRAY_MANAGE_POS_CLAMP_OPEN:
            *readParam = sysParamPtr->cupTrayManageParam.posClampOpen;
            break;
        case INDEX_SUB_PARAM_CUP_TRAY_MANAGE_POS_CLAMP_CLOSE:
            *readParam = sysParamPtr->cupTrayManageParam.posClampClose;
            break;
        case INDEX_SUB_PARAM_CUP_TRAY_MANAGE_POS_PUSH_TRAY_TO_TEST:
            *readParam = sysParamPtr->cupTrayManageParam.posPushTrayToTest;
            break;
        case INDEX_SUB_PARAM_CUP_TRAY_MANAGE_OFFSET_TOP_TO_SENSOR_WHEN_NEW_CUP_TRAY_FULL:
            *readParam = sysParamPtr->cupTrayManageParam.offsetTopToSensorWhenNewCupTrayFull;
            break;
        case INDEX_SUB_PARAM_CUP_TRAY_MANAGE_OFFSET_TOP_TO_PALTFORM_WHEN_NEW_CUP_TRAY_FULL:
            *readParam = sysParamPtr->cupTrayManageParam.offsetTopToPaltformWhenNewCupTrayFull;
            break;
        case INDEX_SUB_PARAM_CUP_TRAY_MANAGE_SPACING_LAYER_NEW_CUP_TRAY:
            *readParam = sysParamPtr->cupTrayManageParam.spacingLayerNewCupTray;
            break;
        case INDEX_SUB_PARAM_CUP_TRAY_MANAGE_OFFSET_TOP_TO_SENSOR_WHEN_EMPTY_CUP_TRAY_FULL:
            *readParam = sysParamPtr->cupTrayManageParam.offsetTopToSensorWhenEmptyCupTrayFull;
            break;
        case INDEX_SUB_PARAM_CUP_TRAY_MANAGE_OFFSET_TOP_TO_PALTFORM_WHEN_EMPTY_CUP_TRAY_FULL:
            *readParam = sysParamPtr->cupTrayManageParam.offsetTopToPaltformWhenEmptyCupTrayFull;
            break;
        case INDEX_SUB_PARAM_CUP_TRAY_MANAGE_SPACING_LAYER_EMPTY_CUP_TRAY:
            *readParam = sysParamPtr->cupTrayManageParam.spacingLayerEmptyCupTray;
            break;
        default://不支持的参数
            *readParam = 0;
            return ERROR_SUB_SYS_PARAM_READ_SUB_INDEX;
    }
    //返回结果
    return ERROR_SUB_OK;
}

ERROR_SUB SysParamWriteParamCupTrayManage(uint8_t subIndex,int32_t writeParam)
{
    int32_t* targetParamPtr = NULL;
    switch(subIndex)
    {
        case INDEX_SUB_PARAM_CUP_TRAY_MANAGE_NEW_CUP_TRAY_RESET_CORRECT:
            targetParamPtr = &(sysParamPtr->cupTrayManageParam.newCupTrayStackResetCorrect);
            break;
        case INDEX_SUB_PARAM_CUP_TRAY_MANAGE_EMPTY_CUP_TRAY_RESET_CORRECT:
            targetParamPtr = &(sysParamPtr->cupTrayManageParam.emptyCupTrayStackResetCorrect);
            break;
        case INDEX_SUB_PARAM_CUP_TRAY_MANAGE_CLAMP_RESET_CORRECT:
            targetParamPtr = &(sysParamPtr->cupTrayManageParam.clampResetCorrect);
            break;
        case INDEX_SUB_PARAM_CUP_TRAY_MANAGE_PUSH_RESET_CORRECT:
            targetParamPtr = &(sysParamPtr->cupTrayManageParam.pushResetCorrect);
            break;
        case INDEX_SUB_PARAM_CUP_TRAY_MANAGE_POS_CLAMP_OPEN:
            targetParamPtr = &(sysParamPtr->cupTrayManageParam.posClampOpen);
            break;
        case INDEX_SUB_PARAM_CUP_TRAY_MANAGE_POS_CLAMP_CLOSE:
            targetParamPtr = &(sysParamPtr->cupTrayManageParam.posClampClose);
            break;
        case INDEX_SUB_PARAM_CUP_TRAY_MANAGE_POS_PUSH_TRAY_TO_TEST:
            targetParamPtr = &(sysParamPtr->cupTrayManageParam.posPushTrayToTest);
            break;
        case INDEX_SUB_PARAM_CUP_TRAY_MANAGE_OFFSET_TOP_TO_SENSOR_WHEN_NEW_CUP_TRAY_FULL:
            targetParamPtr = &(sysParamPtr->cupTrayManageParam.offsetTopToSensorWhenNewCupTrayFull);
            break;
        case INDEX_SUB_PARAM_CUP_TRAY_MANAGE_OFFSET_TOP_TO_PALTFORM_WHEN_NEW_CUP_TRAY_FULL:
            targetParamPtr = &(sysParamPtr->cupTrayManageParam.offsetTopToPaltformWhenNewCupTrayFull);
            break;
        case INDEX_SUB_PARAM_CUP_TRAY_MANAGE_SPACING_LAYER_NEW_CUP_TRAY:
            targetParamPtr = &(sysParamPtr->cupTrayManageParam.spacingLayerNewCupTray);
            break;
        case INDEX_SUB_PARAM_CUP_TRAY_MANAGE_OFFSET_TOP_TO_SENSOR_WHEN_EMPTY_CUP_TRAY_FULL:
            targetParamPtr = &(sysParamPtr->cupTrayManageParam.offsetTopToSensorWhenEmptyCupTrayFull);
            break;
        case INDEX_SUB_PARAM_CUP_TRAY_MANAGE_OFFSET_TOP_TO_PALTFORM_WHEN_EMPTY_CUP_TRAY_FULL:
            targetParamPtr = &(sysParamPtr->cupTrayManageParam.offsetTopToPaltformWhenEmptyCupTrayFull);
            break;
        case INDEX_SUB_PARAM_CUP_TRAY_MANAGE_SPACING_LAYER_EMPTY_CUP_TRAY:
            targetParamPtr = &(sysParamPtr->cupTrayManageParam.spacingLayerEmptyCupTray);
            break;
        default:
            return ERROR_SUB_SYS_PARAM_WRITE_SUB_INDEX;//不支持的参数
    }
    //参数写入Flash
    SysParamWriteSingleCommon(targetParamPtr,writeParam);
    //返回结果
    return ERROR_SUB_OK;
}

/*----------------------------------------------------------------------------------------------------*/
//读写磁珠针参数集合
ERROR_SUB SysParamReadParamNeedleBead(uint8_t subIndex,int32_t* readParam)
{
    switch(subIndex)
    {
        case INDEX_SUB_PARAM_NEEDLE_BEAD_ROTATE_RESET_CORRECT:
            *readParam = sysParamPtr->needleBeadParam.rotateResetCorrect;
            break;
        case INDEX_SUB_PARAM_NEEDLE_BEAD_UPDOWN_RESET_CORRECT:
            *readParam = sysParamPtr->needleBeadParam.upDownResetCorrect;
            break;
        case INDEX_SUB_PARAM_NEEDLE_BEAD_POS_ROTATE_TO_CLEAN:
            *readParam = sysParamPtr->needleBeadParam.posRotateToClean;
            break;
        case INDEX_SUB_PARAM_NEEDLE_BEAD_POS_UPDOWN_CLEAN:
            *readParam = sysParamPtr->needleBeadParam.posUpDownClean;
            break;
        case INDEX_SUB_PARAM_NEEDLE_BEAD_POS_ROTATE_TO_R1:
            *readParam = sysParamPtr->needleBeadParam.posRotateToR1;
            break;
        case INDEX_SUB_PARAM_NEEDLE_BEAD_POS_UPDOWN_R1_LIQUID_DETECT_MAX_LIMIT:
            *readParam = sysParamPtr->needleBeadParam.posUpDownR1LiquidDetectMaxLimit;
            break;
        case INDEX_SUB_PARAM_NEEDLE_BEAD_POS_ROTATE_TO_R2:
            *readParam = sysParamPtr->needleBeadParam.posRotateToR2;
            break;
        case INDEX_SUB_PARAM_NEEDLE_BEAD_POS_UPDOWN_R2_LIQUID_DETECT_MAX_LIMIT:
            *readParam = sysParamPtr->needleBeadParam.posUpDownR2LiquidDetectMaxLimit;
            break;
        case INDEX_SUB_PARAM_NEEDLE_BEAD_POS_ROTATE_TO_BEAD:
            *readParam = sysParamPtr->needleBeadParam.posRotateToBead;
            break;
        case INDEX_SUB_PARAM_NEEDLE_BEAD_POS_UPDOWN_BEAD_LIQUID_DETECT_MAX_LIMIT:
            *readParam = sysParamPtr->needleBeadParam.posUpDownBeadLiquidDetectMaxLimit;
            break;
        case INDEX_SUB_PARAM_NEEDLE_BEAD_POS_ROTATE_TO_PLATE_REACTION:
            *readParam = sysParamPtr->needleBeadParam.posRotateToPlateReaction;
            break;
        case INDEX_SUB_PARAM_NEEDLE_BEAD_POS_UPDOWN_INJECT_AT_BOTTOM:
            *readParam = sysParamPtr->needleBeadParam.posUpDownInjectAtButtom;
            break;
        case INDEX_SUB_PARAM_NEEDLE_BEAD_POS_UPDOWN_INJECT_CORRECT:
            *readParam = sysParamPtr->needleBeadParam.posUpDownInjectCorrect;
            break;
        case INDEX_SUB_PARAM_NEEDLE_BEAD_UL_ABSORB_AIR_AFTER_CLEAN:
            *readParam = sysParamPtr->needleBeadParam.ulAbsorbAirAfterClean;
            break;
        case INDEX_SUB_PARAM_NEEDLE_BEAD_UL_ABSORB_AIR_AFTER_LIQUID_ABSORB:
            *readParam = sysParamPtr->needleBeadParam.ulAbsorbAirAfterLiquidAbsorb;
            break;
        case INDEX_SUB_PARAM_NEEDLE_BEAD_UL_RESERVE_AIR_AFTER_INJECT:
            *readParam = sysParamPtr->needleBeadParam.ulReserveAirAfterInject;
            break;
        case INDEX_SUB_PARAM_NEEDLE_BEAD_LIQUID_DETECT_REAL_TIME_COMM:
            *readParam = sysParamPtr->needleBeadParam.liquidDetectRealTimeComm;
            break;
        case INDEX_SUB_PARAM_NEEDLE_BEAD_UPDOWN_LITTLE_BEAD_MIX:
            *readParam = sysParamPtr->needleBeadParam.posUpDownLittleBeadMix;
            break;
        default://不支持的参数
            *readParam = 0;
            return ERROR_SUB_SYS_PARAM_READ_SUB_INDEX;
    }
    //返回结果
    return ERROR_SUB_OK;
}

ERROR_SUB SysParamWriteParamNeedleBead(uint8_t subIndex,int32_t writeParam)
{
    int32_t* targetParamPtr = NULL;
    switch(subIndex)
    {
        case INDEX_SUB_PARAM_NEEDLE_BEAD_ROTATE_RESET_CORRECT:
            targetParamPtr = &(sysParamPtr->needleBeadParam.rotateResetCorrect);
            break;
        case INDEX_SUB_PARAM_NEEDLE_BEAD_UPDOWN_RESET_CORRECT:
            targetParamPtr = &(sysParamPtr->needleBeadParam.upDownResetCorrect);
            break;
        case INDEX_SUB_PARAM_NEEDLE_BEAD_POS_ROTATE_TO_CLEAN:
            targetParamPtr = &(sysParamPtr->needleBeadParam.posRotateToClean);
            break;
        case INDEX_SUB_PARAM_NEEDLE_BEAD_POS_UPDOWN_CLEAN:
            targetParamPtr = &(sysParamPtr->needleBeadParam.posUpDownClean);
            break;
        case INDEX_SUB_PARAM_NEEDLE_BEAD_POS_ROTATE_TO_R1:
            targetParamPtr = &(sysParamPtr->needleBeadParam.posRotateToR1);
            break;
        case INDEX_SUB_PARAM_NEEDLE_BEAD_POS_UPDOWN_R1_LIQUID_DETECT_MAX_LIMIT:
            targetParamPtr = &(sysParamPtr->needleBeadParam.posUpDownR1LiquidDetectMaxLimit);
            break;
        case INDEX_SUB_PARAM_NEEDLE_BEAD_POS_ROTATE_TO_R2:
            targetParamPtr = &(sysParamPtr->needleBeadParam.posRotateToR2);
            break;
        case INDEX_SUB_PARAM_NEEDLE_BEAD_POS_UPDOWN_R2_LIQUID_DETECT_MAX_LIMIT:
            targetParamPtr = &(sysParamPtr->needleBeadParam.posUpDownR2LiquidDetectMaxLimit);
            break;
        case INDEX_SUB_PARAM_NEEDLE_BEAD_POS_ROTATE_TO_BEAD:
            targetParamPtr = &(sysParamPtr->needleBeadParam.posRotateToBead);
            break;
        case INDEX_SUB_PARAM_NEEDLE_BEAD_POS_UPDOWN_BEAD_LIQUID_DETECT_MAX_LIMIT:
            targetParamPtr = &(sysParamPtr->needleBeadParam.posUpDownBeadLiquidDetectMaxLimit);
            break;
        case INDEX_SUB_PARAM_NEEDLE_BEAD_POS_ROTATE_TO_PLATE_REACTION:
            targetParamPtr = &(sysParamPtr->needleBeadParam.posRotateToPlateReaction);
            break;
        case INDEX_SUB_PARAM_NEEDLE_BEAD_POS_UPDOWN_INJECT_AT_BOTTOM:
            targetParamPtr = &(sysParamPtr->needleBeadParam.posUpDownInjectAtButtom);
            break;
        case INDEX_SUB_PARAM_NEEDLE_BEAD_POS_UPDOWN_INJECT_CORRECT:
            targetParamPtr = &(sysParamPtr->needleBeadParam.posUpDownInjectCorrect);
            break;
        case INDEX_SUB_PARAM_NEEDLE_BEAD_UL_ABSORB_AIR_AFTER_CLEAN:
            targetParamPtr = &(sysParamPtr->needleBeadParam.ulAbsorbAirAfterClean);
            break;
        case INDEX_SUB_PARAM_NEEDLE_BEAD_UL_ABSORB_AIR_AFTER_LIQUID_ABSORB:
            targetParamPtr = &(sysParamPtr->needleBeadParam.ulAbsorbAirAfterLiquidAbsorb);
            break;
        case INDEX_SUB_PARAM_NEEDLE_BEAD_UL_RESERVE_AIR_AFTER_INJECT:
            targetParamPtr = &(sysParamPtr->needleBeadParam.ulReserveAirAfterInject);
            break;
        case INDEX_SUB_PARAM_NEEDLE_BEAD_LIQUID_DETECT_REAL_TIME_COMM:
            targetParamPtr = &(sysParamPtr->needleBeadParam.liquidDetectRealTimeComm);
            break;
        case INDEX_SUB_PARAM_NEEDLE_BEAD_UPDOWN_LITTLE_BEAD_MIX:
            targetParamPtr = &(sysParamPtr->needleBeadParam.posUpDownLittleBeadMix);
            break;
        default:
            return ERROR_SUB_SYS_PARAM_WRITE_SUB_INDEX;//不支持的参数
    }
    //参数写入Flash
    SysParamWriteSingleCommon(targetParamPtr,writeParam);
    //返回结果
    return ERROR_SUB_OK;
}

/*----------------------------------------------------------------------------------------------------*/
//读写辅助参数集合
ERROR_SUB SysParamReadParamOther(uint8_t subIndex,int32_t* readParam)
{
    switch(subIndex)
    {
        case INDEX_SUB_PARAM_OTHER_UTIL:
            *readParam = sysParamPtr->otherUtilParam.util;
            break;
        default://不支持的参数
            *readParam = 0;
            return ERROR_SUB_SYS_PARAM_READ_SUB_INDEX;
    }
    //返回结果
    return ERROR_SUB_OK;
}

ERROR_SUB SysParamWriteParamOther(uint8_t subIndex,int32_t writeParam)
{
    int32_t* targetParamPtr = NULL;
    switch(subIndex)
    {
        case INDEX_SUB_PARAM_OTHER_UTIL:
            targetParamPtr = &(sysParamPtr->otherUtilParam.util);
            break;
        default:
            return ERROR_SUB_SYS_PARAM_WRITE_SUB_INDEX;//不支持的参数
    }
    //参数写入Flash
    SysParamWriteSingleCommon(targetParamPtr,writeParam);
    //返回结果
    return ERROR_SUB_OK;
}
/*----------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------*/
//读写 探液板 集合
ERROR_SUB SysParamReadParamLiquidDetectBoard(uint8_t subIndex,int32_t* readParam)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    BOARD_LIQUID_DETECT_PARAM liquidDetectParam;
    BOARD_LIQUID_DETECT_STATE liquidDetectState;
    uint16_t res = 0;
    switch(subIndex)
    {
        case INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG1_THRESHOLD:
            errorSub = BoardLiquidDetectGetParam((uint8_t)LIQUID_DETECT_CONFIG_1,&liquidDetectParam);
            if(errorSub == ERROR_SUB_OK)
            {
                *readParam = liquidDetectParam.thresholdVal;
            }
            break;
        case INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG1_SLOPE:
            errorSub = BoardLiquidDetectGetParam((uint8_t)LIQUID_DETECT_CONFIG_1,&liquidDetectParam);
            if(errorSub == ERROR_SUB_OK)
            {
                *readParam = liquidDetectParam.slopeVal;
            }
            break;
        case INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG2_THRESHOLD:
            errorSub = BoardLiquidDetectGetParam((uint8_t)LIQUID_DETECT_CONFIG_2,&liquidDetectParam);
            if(errorSub == ERROR_SUB_OK)
            {
                *readParam = liquidDetectParam.thresholdVal;
            }
            break;
        case INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG2_SLOPE:
            errorSub = BoardLiquidDetectGetParam((uint8_t)LIQUID_DETECT_CONFIG_2,&liquidDetectParam);
            if(errorSub == ERROR_SUB_OK)
            {
                *readParam = liquidDetectParam.slopeVal;
            }
            break;
        case INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG3_THRESHOLD:
            errorSub = BoardLiquidDetectGetParam((uint8_t)LIQUID_DETECT_CONFIG_3,&liquidDetectParam);
            if(errorSub == ERROR_SUB_OK)
            {
                *readParam = liquidDetectParam.thresholdVal;
            }
            break;
        case INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG3_SLOPE:
            errorSub = BoardLiquidDetectGetParam((uint8_t)LIQUID_DETECT_CONFIG_3,&liquidDetectParam);
            if(errorSub == ERROR_SUB_OK)
            {
                *readParam = liquidDetectParam.slopeVal;
            }
            break;
        case INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG4_THRESHOLD:
            errorSub = BoardLiquidDetectGetParam((uint8_t)LIQUID_DETECT_CONFIG_4,&liquidDetectParam);
            if(errorSub == ERROR_SUB_OK)
            {
                *readParam = liquidDetectParam.thresholdVal;
            }
            break;
        case INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG4_SLOPE:
            errorSub = BoardLiquidDetectGetParam((uint8_t)LIQUID_DETECT_CONFIG_4,&liquidDetectParam);
            if(errorSub == ERROR_SUB_OK)
            {
                *readParam = liquidDetectParam.slopeVal;
            }
            break;
        case INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG5_THRESHOLD:
            errorSub = BoardLiquidDetectGetParam((uint8_t)LIQUID_DETECT_CONFIG_5,&liquidDetectParam);
            if(errorSub == ERROR_SUB_OK)
            {
                *readParam = liquidDetectParam.thresholdVal;
            }
            break;
        case INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG5_SLOPE:
            errorSub = BoardLiquidDetectGetParam((uint8_t)LIQUID_DETECT_CONFIG_5,&liquidDetectParam);
            if(errorSub == ERROR_SUB_OK)
            {
                *readParam = liquidDetectParam.slopeVal;
            }
            break;
        case INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG6_THRESHOLD:
            errorSub = BoardLiquidDetectGetParam((uint8_t)LIQUID_DETECT_CONFIG_6,&liquidDetectParam);
            if(errorSub == ERROR_SUB_OK)
            {
                *readParam = liquidDetectParam.thresholdVal;
            }
            break;
        case INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG6_SLOPE:
            errorSub = BoardLiquidDetectGetParam((uint8_t)LIQUID_DETECT_CONFIG_6,&liquidDetectParam);
            if(errorSub == ERROR_SUB_OK)
            {
                *readParam = liquidDetectParam.slopeVal;
            }
            break;
        case INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG7_THRESHOLD:
            errorSub = BoardLiquidDetectGetParam((uint8_t)LIQUID_DETECT_CONFIG_7,&liquidDetectParam);
            if(errorSub == ERROR_SUB_OK)
            {
                *readParam = liquidDetectParam.thresholdVal;
            }
            break;
        case INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG7_SLOPE:
            errorSub = BoardLiquidDetectGetParam((uint8_t)LIQUID_DETECT_CONFIG_7,&liquidDetectParam);
            if(errorSub == ERROR_SUB_OK)
            {
                *readParam = liquidDetectParam.slopeVal;
            }
            break;
        case INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG8_THRESHOLD:
            errorSub = BoardLiquidDetectGetParam((uint8_t)LIQUID_DETECT_CONFIG_8,&liquidDetectParam);
            if(errorSub == ERROR_SUB_OK)
            {
                *readParam = liquidDetectParam.thresholdVal;
            }
            break;
        case INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG8_SLOPE:
            errorSub = BoardLiquidDetectGetParam((uint8_t)LIQUID_DETECT_CONFIG_8,&liquidDetectParam);
            if(errorSub == ERROR_SUB_OK)
            {
                *readParam = liquidDetectParam.slopeVal;
            }
            break;
        case INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG9_THRESHOLD:
            errorSub = BoardLiquidDetectGetParam((uint8_t)LIQUID_DETECT_CONFIG_9,&liquidDetectParam);
            if(errorSub == ERROR_SUB_OK)
            {
                *readParam = liquidDetectParam.thresholdVal;
            }
            break;
        case INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG9_SLOPE:
            errorSub = BoardLiquidDetectGetParam((uint8_t)LIQUID_DETECT_CONFIG_9,&liquidDetectParam);
            if(errorSub == ERROR_SUB_OK)
            {
                *readParam = liquidDetectParam.slopeVal;
            }
            break;
        case INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG10_THRESHOLD:
            errorSub = BoardLiquidDetectGetParam((uint8_t)LIQUID_DETECT_CONFIG_10,&liquidDetectParam);
            if(errorSub == ERROR_SUB_OK)
            {
                *readParam = liquidDetectParam.thresholdVal;
            }
            break;
        case INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG10_SLOPE:
            errorSub = BoardLiquidDetectGetParam((uint8_t)LIQUID_DETECT_CONFIG_10,&liquidDetectParam);
            if(errorSub == ERROR_SUB_OK)
            {
                *readParam = liquidDetectParam.slopeVal;
            }
            break;
        case INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_VOL_REALTIME:
            errorSub = BoardLiquidDetectGetState(&liquidDetectState);
            if(errorSub == ERROR_SUB_OK)
            {
                *readParam = liquidDetectState.needleTipVol;
            }
            break;
        case INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_BASE_RES:
            errorSub = BoardLiquidDetectReadBaseRes(&res);
            if(errorSub == ERROR_SUB_OK)
            {
                *readParam = res;
            }
            break;
        case INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_VOL_BASE:
            errorSub = BoardLiquidDetectGetState(&liquidDetectState);
            if(errorSub == ERROR_SUB_OK)
            {
                *readParam = liquidDetectState.baseVol;
            }
            break;
        default://不支持的参数
            return ERROR_SUB_SYS_PARAM_READ_SUB_INDEX;
    }
    //返回结果
    return errorSub;
}

ERROR_SUB SysParamWriteParamLiquidDetectBoard(uint8_t subIndex,int32_t writeParam)
{
    switch(subIndex)
    {
        case INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG1_THRESHOLD:
            //不支持写入
            return ERROR_SUB_SYS_PARAM_WRITE_SUB_UNSUPPORT;
        case INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG1_SLOPE:
            //不支持写入
            return ERROR_SUB_SYS_PARAM_WRITE_SUB_UNSUPPORT;
        case INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG2_THRESHOLD:
            //不支持写入
            return ERROR_SUB_SYS_PARAM_WRITE_SUB_UNSUPPORT;
        case INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG2_SLOPE:
            //不支持写入
            return ERROR_SUB_SYS_PARAM_WRITE_SUB_UNSUPPORT;
        case INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG3_THRESHOLD:
            //不支持写入
            return ERROR_SUB_SYS_PARAM_WRITE_SUB_UNSUPPORT;
        case INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG3_SLOPE:
            //不支持写入
            return ERROR_SUB_SYS_PARAM_WRITE_SUB_UNSUPPORT;
        case INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG4_THRESHOLD:
            //不支持写入
            return ERROR_SUB_SYS_PARAM_WRITE_SUB_UNSUPPORT;
        case INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG4_SLOPE:
            //不支持写入
            return ERROR_SUB_SYS_PARAM_WRITE_SUB_UNSUPPORT;
        case INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG5_THRESHOLD:
            //不支持写入
            return ERROR_SUB_SYS_PARAM_WRITE_SUB_UNSUPPORT;
        case INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG5_SLOPE:
            //不支持写入
            return ERROR_SUB_SYS_PARAM_WRITE_SUB_UNSUPPORT;
        case INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG6_THRESHOLD:
            //不支持写入
            return ERROR_SUB_SYS_PARAM_WRITE_SUB_UNSUPPORT;
        case INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG6_SLOPE:
            //不支持写入
            return ERROR_SUB_SYS_PARAM_WRITE_SUB_UNSUPPORT;
        case INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG7_THRESHOLD:
            //不支持写入
            return ERROR_SUB_SYS_PARAM_WRITE_SUB_UNSUPPORT;
        case INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG7_SLOPE:
            //不支持写入
            return ERROR_SUB_SYS_PARAM_WRITE_SUB_UNSUPPORT;
        case INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG8_THRESHOLD:
            //不支持写入
            return ERROR_SUB_SYS_PARAM_WRITE_SUB_UNSUPPORT;
        case INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG8_SLOPE:
            //不支持写入
            return ERROR_SUB_SYS_PARAM_WRITE_SUB_UNSUPPORT;
        case INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG9_THRESHOLD:
            //不支持写入
            return ERROR_SUB_SYS_PARAM_WRITE_SUB_UNSUPPORT;
        case INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG9_SLOPE:
            //不支持写入
            return ERROR_SUB_SYS_PARAM_WRITE_SUB_UNSUPPORT;
        case INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG10_THRESHOLD:
            //不支持写入
            return ERROR_SUB_SYS_PARAM_WRITE_SUB_UNSUPPORT;
        case INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_CONFIG10_SLOPE:
            //不支持写入
            return ERROR_SUB_SYS_PARAM_WRITE_SUB_UNSUPPORT;
        case INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_VOL_REALTIME:
            //不支持写入
            return ERROR_SUB_SYS_PARAM_WRITE_SUB_UNSUPPORT;
        case INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_BASE_RES:
            //写入基准电阻
            return BoardLiquidDetectWriteBaseRes((uint16_t)writeParam);
        case INDEX_SUB_PARAM_LIQUID_DETECT_BOARD_VOL_BASE:
            //不支持写入
            return ERROR_SUB_SYS_PARAM_WRITE_SUB_UNSUPPORT;
        default:
            return ERROR_SUB_SYS_PARAM_WRITE_SUB_INDEX;//不支持的参数
    }
}
/*----------------------------------------------------------------------------------------------------*/

/****************************************参数读取函数指针数组**************************************************/
static const SysParamReadParamSingleFuncPtr SysParamReadParamSingleArray[] = {
    SysParamReadParamPlateReagent,
    SysParamReadParamCupTrayManage,
    SysParamReadParamNeedleBead,
    SysParamReadParamLiquidDetectBoard,
    SysParamReadParamOther,
};

/****************************************参数写入函数指针数组**************************************************/
static const SysParamWriteParamSingleFuncPtr SysParamWriteParamSingleArray[] = {
    SysParamWriteParamPlateReagent,
    SysParamWriteParamCupTrayManage,
    SysParamWriteParamNeedleBead,
    SysParamWriteParamLiquidDetectBoard,
    SysParamWriteParamOther,
};


//读取参数
ERROR_SUB SysParamReadSingle(INDEX_MAIN_SYS_PARAM mainIndex,uint8_t subIndex,int32_t* paramRead)
{
    if(mainIndex >= DIM_ARRAY_ELEMENT_COUNT(SysParamReadParamSingleArray))
    {
        return ERROR_SUB_SYS_PARAM_READ_MAIN_INDEX;
    }
    return SysParamReadParamSingleArray[mainIndex](subIndex,paramRead);
}

//写入参数
ERROR_SUB SysParamWriteSingle(INDEX_MAIN_SYS_PARAM mainIndex,uint8_t subIndex,int32_t paramWrite)
{
    if(mainIndex >= DIM_ARRAY_ELEMENT_COUNT(SysParamWriteParamSingleArray))
    {
        return ERROR_SUB_SYS_PARAM_WRITE_MAIN_INDEX;
    }
    return SysParamWriteParamSingleArray[mainIndex](subIndex,paramWrite);
}



