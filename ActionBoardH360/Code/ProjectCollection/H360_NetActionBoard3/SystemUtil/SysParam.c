/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-29 14:07:30 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-13 13:34:37 +0800
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
    //系统测试参数集合
    UserMemCopy((void*)(&(sysParamPtr->handWashParam)),(void*)(&handWashParamDefault),LENGTH_SYS_PARAM_HAND_WASH);
    UserMemCopy((void*)(&(sysParamPtr->measureModuleParam)),(void*)(&measureModuleParamDefault),LENGTH_SYS_PARAM_MEASURE_MODULE);
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
//读写 清洗机械手参数 参数集合
ERROR_SUB SysParamReadParamHandWash(uint8_t subIndex,int32_t* readParam)
{
    switch(subIndex)
    {
        case INDEX_SUB_PARAM_HAND_WASH_UPDOWN_RESET_CORRECT:
            *readParam = sysParamPtr->handWashParam.updownResetCorrect;
            break;
        case INDEX_SUB_PARAM_HAND_WASH_ROTATE_RESET_CORRECT:
            *readParam = sysParamPtr->handWashParam.rotateResetCorrect;
            break;
        case INDEX_SUB_PARAM_HAND_WASH_POS_ROTATE_PLATE_REACTION:
            *readParam = sysParamPtr->handWashParam.posRotatePlateReaction;
            break;
        case INDEX_SUB_PARAM_HAND_WASH_POS_UPDOWN_CATCH_CUP_PLATE_REACTION:
            *readParam = sysParamPtr->handWashParam.posUpdownCatchCupPlateReaction;
            break;
        case INDEX_SUB_PARAM_HAND_WASH_POS_UPDOWN_PUT_CUP_PLATE_REACTION:
            *readParam = sysParamPtr->handWashParam.posUpdownPutCupPlateReaction;
            break;
        case INDEX_SUB_PARAM_HAND_WASH_POS_ROTATE_PLATE_WASH:
            *readParam = sysParamPtr->handWashParam.posRotatePlateWash;
            break;
        case INDEX_SUB_PARAM_HAND_WASH_POS_UPDOWN_CATCH_CUP_PLATE_WASH:
            *readParam = sysParamPtr->handWashParam.posUpdownCatchCupPlateWash;
            break;
        case INDEX_SUB_PARAM_HAND_WASH_POS_UPDOWN_PUT_CUP_PLATE_WASH:
            *readParam = sysParamPtr->handWashParam.posUpdownPutCupPlateWash;
            break;
        case INDEX_SUB_PARAM_HAND_WASH_POS_ROTATE_MEASURE_UINT:
            *readParam = sysParamPtr->handWashParam.posRotateMeasureUnit;
            break;
        case INDEX_SUB_PARAM_HAND_WASH_POS_UPDOWN_CATCH_CUP_MEASURE_UNIT:
            *readParam = sysParamPtr->handWashParam.posUpdownCatchCupOnMeasureUnitUnit;
            break;
        case INDEX_SUB_PARAM_HAND_WASH_POS_UPDOWN_PUT_CUP_MEASURE_UNIT:
            *readParam = sysParamPtr->handWashParam.posUpdownPutCupOnMeasureUnitUnit;
            break;
        case INDEX_SUB_PARAM_HAND_WASH_UPDOWN_CATCH_MEA_POS_CHECK_ENABLE:
            *readParam = sysParamPtr->handWashParam.updownCatchMeaPosCheckEnable;
            break;
        case INDEX_SUB_PARAM_HAND_WASH_UPDOWN_CATCH_MEA_CORRECT:
            *readParam = sysParamPtr->handWashParam.updownCatchMeaCorrect;
            break;
        default://不支持的参数
            *readParam = 0;
            return ERROR_SUB_SYS_PARAM_READ_SUB_INDEX;
    }
    //返回结果
    return ERROR_SUB_OK;
}

ERROR_SUB SysParamWriteParamHandWash(uint8_t subIndex,int32_t writeParam)
{
    int32_t* targetParamPtr = NULL;
    switch(subIndex)
    {
        case INDEX_SUB_PARAM_HAND_WASH_UPDOWN_RESET_CORRECT:
            targetParamPtr = &(sysParamPtr->handWashParam.updownResetCorrect);
            break;
        case INDEX_SUB_PARAM_HAND_WASH_ROTATE_RESET_CORRECT:
            targetParamPtr = &(sysParamPtr->handWashParam.rotateResetCorrect);
            break;
        case INDEX_SUB_PARAM_HAND_WASH_POS_ROTATE_PLATE_REACTION:
            targetParamPtr = &(sysParamPtr->handWashParam.posRotatePlateReaction);
            break;
        case INDEX_SUB_PARAM_HAND_WASH_POS_UPDOWN_CATCH_CUP_PLATE_REACTION:
            targetParamPtr = &(sysParamPtr->handWashParam.posUpdownCatchCupPlateReaction);
            break;
        case INDEX_SUB_PARAM_HAND_WASH_POS_UPDOWN_PUT_CUP_PLATE_REACTION:
            targetParamPtr = &(sysParamPtr->handWashParam.posUpdownPutCupPlateReaction);
            break;
        case INDEX_SUB_PARAM_HAND_WASH_POS_ROTATE_PLATE_WASH:
            targetParamPtr = &(sysParamPtr->handWashParam.posRotatePlateWash);
            break;
        case INDEX_SUB_PARAM_HAND_WASH_POS_UPDOWN_CATCH_CUP_PLATE_WASH:
            targetParamPtr = &(sysParamPtr->handWashParam.posUpdownCatchCupPlateWash);
            break;
        case INDEX_SUB_PARAM_HAND_WASH_POS_UPDOWN_PUT_CUP_PLATE_WASH:
            targetParamPtr = &(sysParamPtr->handWashParam.posUpdownPutCupPlateWash);
            break;
        case INDEX_SUB_PARAM_HAND_WASH_POS_ROTATE_MEASURE_UINT:
            targetParamPtr = &(sysParamPtr->handWashParam.posRotateMeasureUnit);
            break;
        case INDEX_SUB_PARAM_HAND_WASH_POS_UPDOWN_CATCH_CUP_MEASURE_UNIT:
            targetParamPtr = &(sysParamPtr->handWashParam.posUpdownCatchCupOnMeasureUnitUnit);
            break;
        case INDEX_SUB_PARAM_HAND_WASH_POS_UPDOWN_PUT_CUP_MEASURE_UNIT:
            targetParamPtr = &(sysParamPtr->handWashParam.posUpdownPutCupOnMeasureUnitUnit);
            break;
        case INDEX_SUB_PARAM_HAND_WASH_UPDOWN_CATCH_MEA_POS_CHECK_ENABLE:
            targetParamPtr = &(sysParamPtr->handWashParam.updownCatchMeaPosCheckEnable);
            break;
        case INDEX_SUB_PARAM_HAND_WASH_UPDOWN_CATCH_MEA_CORRECT:
            targetParamPtr = &(sysParamPtr->handWashParam.updownCatchMeaCorrect);
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
//读写 测量室参数 参数集合
ERROR_SUB SysParamReadParamMeasureModule(uint8_t subIndex,int32_t* readParam)
{
    switch(subIndex)
    {
        case INDEX_SUB_PARAM_MEASURE_MODULE_UPDOOR_RESET_CORRECT:
            *readParam = sysParamPtr->measureModuleParam.doorResetCorrect;
            break;
        case INDEX_SUB_PARAM_MEASURE_MODULE_WINDOW_RESET_CORRECT:
            *readParam = sysParamPtr->measureModuleParam.windowResetCorrect;
            break;
        case INDEX_SUB_PARAM_MEASURE_MODULE_PUMP_RESET_CORRECT:
            *readParam = sysParamPtr->measureModuleParam.pumpResetCorrect;
            break;
        case INDEX_SUB_PARAM_MEASURE_MODULE_POS_DOOR_CLOSE:
            *readParam = sysParamPtr->measureModuleParam.posDoorClose;
            break;
        case INDEX_SUB_PARAM_MEASURE_MODULE_POS_DOOR_OPEN_HALF:
            *readParam = sysParamPtr->measureModuleParam.posDoorOpenHalf;
            break;
        case INDEX_SUB_PARAM_MEASURE_MODULE_POS_DOOR_OPEN_FULL:
            *readParam = sysParamPtr->measureModuleParam.posDoorOpenFull;
            break;
        case INDEX_SUB_PARAM_MEASURE_MODULE_POS_WINDOW_CLOSE:
            *readParam = sysParamPtr->measureModuleParam.posWindowClose;
            break;
        case INDEX_SUB_PARAM_MEASURE_MODULE_POS_WINDOW_OPEN:
            *readParam = sysParamPtr->measureModuleParam.posWindowOpen;
            break;
        case INDEX_SUB_PARAM_MEASURE_MODULE_LIGHT_LINEAR_CORRECT_FACTOR_A:
            *readParam = sysParamPtr->measureModuleParam.lightLinearCorrectFactorA;
            break;
        case INDEX_SUB_PARAM_MEASURE_MODULE_LIGHT_LINEAR_CORRECT_FACTOR_B:
            *readParam = sysParamPtr->measureModuleParam.lightLinearCorrectFactorB;
            break;
        default://不支持的参数
            *readParam = 0;
            return ERROR_SUB_SYS_PARAM_READ_SUB_INDEX;
    }
    //返回结果
    return ERROR_SUB_OK;
}

ERROR_SUB SysParamWriteParamMeasureModule(uint8_t subIndex,int32_t writeParam)
{
    int32_t* targetParamPtr = NULL;
    switch(subIndex)
    {
        case INDEX_SUB_PARAM_MEASURE_MODULE_UPDOOR_RESET_CORRECT:
            targetParamPtr = &(sysParamPtr->measureModuleParam.doorResetCorrect);
            break;
        case INDEX_SUB_PARAM_MEASURE_MODULE_WINDOW_RESET_CORRECT:
            targetParamPtr = &(sysParamPtr->measureModuleParam.windowResetCorrect);
            break;
        case INDEX_SUB_PARAM_MEASURE_MODULE_PUMP_RESET_CORRECT:
            targetParamPtr = &(sysParamPtr->measureModuleParam.pumpResetCorrect);
            break;
        case INDEX_SUB_PARAM_MEASURE_MODULE_POS_DOOR_CLOSE:
            targetParamPtr = &(sysParamPtr->measureModuleParam.posDoorClose);
            break;
        case INDEX_SUB_PARAM_MEASURE_MODULE_POS_DOOR_OPEN_HALF:
            targetParamPtr = &(sysParamPtr->measureModuleParam.posDoorOpenHalf);
            break;
        case INDEX_SUB_PARAM_MEASURE_MODULE_POS_DOOR_OPEN_FULL:
            targetParamPtr = &(sysParamPtr->measureModuleParam.posDoorOpenFull);
            break;
        case INDEX_SUB_PARAM_MEASURE_MODULE_POS_WINDOW_CLOSE:
            targetParamPtr = &(sysParamPtr->measureModuleParam.posWindowClose);
            break;
        case INDEX_SUB_PARAM_MEASURE_MODULE_POS_WINDOW_OPEN:
            targetParamPtr = &(sysParamPtr->measureModuleParam.posWindowOpen);
            break;
        case INDEX_SUB_PARAM_MEASURE_MODULE_LIGHT_LINEAR_CORRECT_FACTOR_A:
            targetParamPtr = &(sysParamPtr->measureModuleParam.lightLinearCorrectFactorA);
            break;
        case INDEX_SUB_PARAM_MEASURE_MODULE_LIGHT_LINEAR_CORRECT_FACTOR_B:
            targetParamPtr = &(sysParamPtr->measureModuleParam.lightLinearCorrectFactorB);
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
//读写 辅助参数 参数集合
ERROR_SUB SysParamReadParamOtherUtil(uint8_t subIndex,int32_t* readParam)
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

ERROR_SUB SysParamWriteParamOtherUtil(uint8_t subIndex,int32_t writeParam)
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

/****************************************参数读取函数指针数组**************************************************/
static const SysParamReadParamSingleFuncPtr SysParamReadParamSingleArray[] = {
    SysParamReadParamHandWash,
    SysParamReadParamMeasureModule,
    SysParamReadParamOtherUtil,
};

/****************************************参数写入函数指针数组**************************************************/
static const SysParamWriteParamSingleFuncPtr SysParamWriteParamSingleArray[] = {
    SysParamWriteParamHandWash,
    SysParamWriteParamMeasureModule,
    SysParamWriteParamOtherUtil,
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



