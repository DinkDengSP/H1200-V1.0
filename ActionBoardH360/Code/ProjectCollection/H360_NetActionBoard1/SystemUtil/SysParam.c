/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-29 14:07:30 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-04 17:57:44 +0800
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
    UserMemCopy((void*)(&(sysParamPtr->paramPlateWash)),(void*)(&paramPlateWashDefault),LENGTH_SYS_PARAM_PLATE_WASH);
    UserMemCopy((void*)(&(sysParamPtr->paramMixWash)),(void*)(&paramMixWashDefault),LENGTH_SYS_PARAM_MIX_WASH);
    UserMemCopy((void*)(&(sysParamPtr->paramNeedleWash)),(void*)(&paramNeedleWashDefault),LENGTH_SYS_PARAM_NEEDLE_WASH);
    UserMemCopy((void*)(&(sysParamPtr->paramBufferInject)),(void*)(&paramBufferInjectDefault),LENGTH_SYS_PARAM_BUFFER_INJECT);
    UserMemCopy((void*)(&(sysParamPtr->paramLiquidAuto)),(void*)(&paramLiquidAutoDefault),LENGTH_SYS_PARAM_LIQUID_AUTO);
    UserMemCopy((void*)(&(sysParamPtr->paramOther)),(void*)(&paramOtherDefault),LENGTH_SYS_PARAM_OTHER);
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
//读写 清洗盘 参数集合
ERROR_SUB SysParamReadParamPlateWash(uint8_t subIndex,int32_t* readParam)
{
    switch(subIndex)
    {
        case INDEX_SUB_PARAM_PLATE_WASH_RESET_CORRECT:
            *readParam = sysParamPtr->paramPlateWash.resetCorrect;
            break;
        default://不支持的参数
            *readParam = 0;
            return ERROR_SUB_SYS_PARAM_READ_SUB_INDEX;
    }
    //返回结果
    return ERROR_SUB_OK;
}

ERROR_SUB SysParamWriteParamPlateWash(uint8_t subIndex,int32_t writeParam)
{
    int32_t* targetParamPtr = NULL;
    switch(subIndex)
    {
        case INDEX_SUB_PARAM_PLATE_WASH_RESET_CORRECT:
            targetParamPtr = &(sysParamPtr->paramPlateWash.resetCorrect);
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
//读写 清洗混匀 参数集合
ERROR_SUB SysParamReadParamMixWash(uint8_t subIndex,int32_t* readParam)
{
    switch(subIndex)
    {
        case INDEX_SUB_PARAM_MIX_WASH_RESET_CORRECT_UPDOWN:
            *readParam = sysParamPtr->paramMixWash.resetCorrectUpDown;
            break;
        case INDEX_SUB_PARAM_MIX_WASH_RESET_CORRECT_ROTATE:
            *readParam = sysParamPtr->paramMixWash.resetCorrectRotate;
            break;
        case INDEX_SUB_PARAM_MIX_WASH_UP_POS_MODE1:
            *readParam = sysParamPtr->paramMixWash.upPosMode1;
            break;
        case INDEX_SUB_PARAM_MIX_WASH_ROTATE_TIME_MS_MODE1:
            *readParam = sysParamPtr->paramMixWash.rotateTimeMsMode1;
            break;
        case INDEX_SUB_PARAM_MIX_WASH_UP_POS_MODE2:
            *readParam = sysParamPtr->paramMixWash.upPosMode2;
            break;
        case INDEX_SUB_PARAM_MIX_WASH_ROTATE_TIME_MS_MODE2:
            *readParam = sysParamPtr->paramMixWash.rotateTimeMsMode2;
            break;
        case INDEX_SUB_PARAM_MIX_WASH_UP_POS_MODE3:
            *readParam = sysParamPtr->paramMixWash.upPosMode3;
            break;
        case INDEX_SUB_PARAM_MIX_WASH_ROTATE_TIME_MS_MODE3:
            *readParam = sysParamPtr->paramMixWash.rotateTimeMsMode3;
            break;
        case INDEX_SUB_PARAM_MIX_WASH_UP_POS_MODE4:
            *readParam = sysParamPtr->paramMixWash.upPosMode4;
            break;
        case INDEX_SUB_PARAM_MIX_WASH_ROTATE_TIME_MS_MODE4:
            *readParam = sysParamPtr->paramMixWash.rotateTimeMsMode4;
            break;
        case INDEX_SUB_PARAM_MIX_WASH_UP_POS_MODE5:
            *readParam = sysParamPtr->paramMixWash.upPosMode5;
            break;
        case INDEX_SUB_PARAM_MIX_WASH_ROTATE_TIME_MS_MODE5:
            *readParam = sysParamPtr->paramMixWash.rotateTimeMsMode5;
            break;
        case INDEX_SUB_PARAM_MIX_WASH_UP_POS_MODE6:
            *readParam = sysParamPtr->paramMixWash.upPosMode6;
            break;
        case INDEX_SUB_PARAM_MIX_WASH_ROTATE_TIME_MS_MODE6:
            *readParam = sysParamPtr->paramMixWash.rotateTimeMsMode6;
            break;
        case INDEX_SUB_PARAM_MIX_WASH_UP_POS_MODE7:
            *readParam = sysParamPtr->paramMixWash.upPosMode7;
            break;
        case INDEX_SUB_PARAM_MIX_WASH_ROTATE_TIME_MS_MODE7:
            *readParam = sysParamPtr->paramMixWash.rotateTimeMsMode7;
            break;
        case INDEX_SUB_PARAM_MIX_WASH_UP_POS_MODE8:
            *readParam = sysParamPtr->paramMixWash.upPosMode8;
            break;
        case INDEX_SUB_PARAM_MIX_WASH_ROTATE_TIME_MS_MODE8:
            *readParam = sysParamPtr->paramMixWash.rotateTimeMsMode8;
            break;
        case INDEX_SUB_PARAM_MIX_WASH_UP_POS_MODE9:
            *readParam = sysParamPtr->paramMixWash.upPosMode9;
            break;
        case INDEX_SUB_PARAM_MIX_WASH_ROTATE_TIME_MS_MODE9:
            *readParam = sysParamPtr->paramMixWash.rotateTimeMsMode9;
            break;
        case INDEX_SUB_PARAM_MIX_WASH_UP_POS_MODE10:
            *readParam = sysParamPtr->paramMixWash.upPosMode10;
            break;
        case INDEX_SUB_PARAM_MIX_WASH_ROTATE_TIME_MS_MODE10:
            *readParam = sysParamPtr->paramMixWash.rotateTimeMsMode10;
            break;
        default://不支持的参数
            *readParam = 0;
            return ERROR_SUB_SYS_PARAM_READ_SUB_INDEX;
    }
    //返回结果
    return ERROR_SUB_OK;
}

ERROR_SUB SysParamWriteParamMixWash(uint8_t subIndex,int32_t writeParam)
{
    int32_t* targetParamPtr = NULL;
    switch(subIndex)
    {
        case INDEX_SUB_PARAM_MIX_WASH_RESET_CORRECT_UPDOWN:
            targetParamPtr = &(sysParamPtr->paramMixWash.resetCorrectUpDown);
            break;
        case INDEX_SUB_PARAM_MIX_WASH_RESET_CORRECT_ROTATE:
            targetParamPtr = &(sysParamPtr->paramMixWash.resetCorrectRotate);
            break;
        case INDEX_SUB_PARAM_MIX_WASH_UP_POS_MODE1:
            targetParamPtr = &(sysParamPtr->paramMixWash.upPosMode1);
            break;
        case INDEX_SUB_PARAM_MIX_WASH_ROTATE_TIME_MS_MODE1:
            targetParamPtr = &(sysParamPtr->paramMixWash.rotateTimeMsMode1);
            break;
        case INDEX_SUB_PARAM_MIX_WASH_UP_POS_MODE2:
            targetParamPtr = &(sysParamPtr->paramMixWash.upPosMode2);
            break;
        case INDEX_SUB_PARAM_MIX_WASH_ROTATE_TIME_MS_MODE2:
            targetParamPtr = &(sysParamPtr->paramMixWash.rotateTimeMsMode2);
            break;
        case INDEX_SUB_PARAM_MIX_WASH_UP_POS_MODE3:
            targetParamPtr = &(sysParamPtr->paramMixWash.upPosMode3);
            break;
        case INDEX_SUB_PARAM_MIX_WASH_ROTATE_TIME_MS_MODE3:
            targetParamPtr = &(sysParamPtr->paramMixWash.rotateTimeMsMode3);
            break;
        case INDEX_SUB_PARAM_MIX_WASH_UP_POS_MODE4:
            targetParamPtr = &(sysParamPtr->paramMixWash.upPosMode4);
            break;
        case INDEX_SUB_PARAM_MIX_WASH_ROTATE_TIME_MS_MODE4:
            targetParamPtr = &(sysParamPtr->paramMixWash.rotateTimeMsMode4);
            break;
        case INDEX_SUB_PARAM_MIX_WASH_UP_POS_MODE5:
            targetParamPtr = &(sysParamPtr->paramMixWash.upPosMode5);
            break;
        case INDEX_SUB_PARAM_MIX_WASH_ROTATE_TIME_MS_MODE5:
            targetParamPtr = &(sysParamPtr->paramMixWash.rotateTimeMsMode5);
            break;
        case INDEX_SUB_PARAM_MIX_WASH_UP_POS_MODE6:
            targetParamPtr = &(sysParamPtr->paramMixWash.upPosMode6);
            break;
        case INDEX_SUB_PARAM_MIX_WASH_ROTATE_TIME_MS_MODE6:
            targetParamPtr = &(sysParamPtr->paramMixWash.rotateTimeMsMode6);
            break;
        case INDEX_SUB_PARAM_MIX_WASH_UP_POS_MODE7:
            targetParamPtr = &(sysParamPtr->paramMixWash.upPosMode7);
            break;
        case INDEX_SUB_PARAM_MIX_WASH_ROTATE_TIME_MS_MODE7:
            targetParamPtr = &(sysParamPtr->paramMixWash.rotateTimeMsMode7);
            break;
        case INDEX_SUB_PARAM_MIX_WASH_UP_POS_MODE8:
            targetParamPtr = &(sysParamPtr->paramMixWash.upPosMode8);
            break;
        case INDEX_SUB_PARAM_MIX_WASH_ROTATE_TIME_MS_MODE8:
            targetParamPtr = &(sysParamPtr->paramMixWash.rotateTimeMsMode8);
            break;
        case INDEX_SUB_PARAM_MIX_WASH_UP_POS_MODE9:
            targetParamPtr = &(sysParamPtr->paramMixWash.upPosMode9);
            break;
        case INDEX_SUB_PARAM_MIX_WASH_ROTATE_TIME_MS_MODE9:
            targetParamPtr = &(sysParamPtr->paramMixWash.rotateTimeMsMode9);
            break;
        case INDEX_SUB_PARAM_MIX_WASH_UP_POS_MODE10:
            targetParamPtr = &(sysParamPtr->paramMixWash.upPosMode10);
            break;
        case INDEX_SUB_PARAM_MIX_WASH_ROTATE_TIME_MS_MODE10:
            targetParamPtr = &(sysParamPtr->paramMixWash.rotateTimeMsMode10);
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
//读写 清洗针 参数集合
ERROR_SUB SysParamReadParamNeedleWash(uint8_t subIndex,int32_t* readParam)
{
    switch(subIndex)
    {
        case INDEX_SUB_PARAM_NEEDLE_WASH_RESET_CORRECT_UPDOWN:
            *readParam = sysParamPtr->paramNeedleWash.resetCorrectUpDown;
            break;
        case INDEX_SUB_PARAM_NEEDLE_WASH_POS_CUP_TOP_NEEDLE_UPDOWN:
            *readParam = sysParamPtr->paramNeedleWash.posCupTopNeedleUpDown;
            break;
        case INDEX_SUB_PARAM_NEEDLE_WASH_POS_CUP_BOTTOM_NEEDLE_UPDOWN:
            *readParam = sysParamPtr->paramNeedleWash.posCupBottomNeedleUpDown;
            break;
        case INDEX_SUB_PARAM_NEEDLE_WASH_UL_ABSORB_WASH_PUMP1:
            *readParam = sysParamPtr->paramNeedleWash.ulAbsorbWashPump1;
            break;
        case INDEX_SUB_PARAM_NEEDLE_WASH_UL_ABSORB_WASH_PUMP2:
            *readParam = sysParamPtr->paramNeedleWash.ulAbsorbWashPump2;
            break;
        case INDEX_SUB_PARAM_NEEDLE_WASH_UL_ABSORB_WASH_PUMP3:
            *readParam = sysParamPtr->paramNeedleWash.ulAbsorbWashPump3;
            break;
        case INDEX_SUB_PARAM_NEEDLE_WASH_UL_ABSORB_WASH_PUMP4:
            *readParam = sysParamPtr->paramNeedleWash.ulAbsorbWashPump4;
            break;
        case INDEX_SUB_PARAM_NEEDLE_WASH_UL_ABSORB_WASH_PUMP5:
            *readParam = sysParamPtr->paramNeedleWash.ulAbsorbWashPump5;
            break;
        case INDEX_SUB_PARAM_NEEDLE_WASH_TIME_MS_WAIT_WASTE_LIQUID_AT_CUP_BOTTOM:
            *readParam = sysParamPtr->paramNeedleWash.timeMsWaitWasteLiquidAtCupBottom;
            break;
        case INDEX_SUB_PARAM_NEEDLE_WASH_TIME_MS_WAIT_INJECT_STABLE_AT_CUP_TOP:
            *readParam = sysParamPtr->paramNeedleWash.timeMsWaitInjectStableAtCupTop;
            break;
        case INDEX_SUB_PARAM_NEEDLE_WASH_TIME_MS_WAIT_WASTE_LIQUID_AT_PRIME:
            *readParam = sysParamPtr->paramNeedleWash.timeMsWaitWasteLiquidAtPrime;
            break;
        case INDEX_SUB_PARAM_NEEDLE_WASH_POS_UPDOWN_TO_SOAK:
            *readParam = sysParamPtr->paramNeedleWash.posUpdownToSoak;
            break;
        case INDEX_SUB_PARAM_NEEDLA_WASH_TIME_MS_MAINTAIN_CLEAN:
            *readParam = sysParamPtr->paramNeedleWash.maintainCleanTime;
            break;
        case INDEX_SUB_PARAM_NEEDLE_WASH_THREE_HOLE_NEEDLE_2_5_CUP_BUTTOM_INJECT_DELAY_TIME:
            *readParam = sysParamPtr->paramNeedleWash.threeHoleNeedle2_5InjectDelayMs;
            break;
        case INDEX_SUB_PARAM_NEEDLE_WASH_THREE_HOLE_NEEDLE_1_INJECT_WAIT_NEEDLE_UP_TIME:
            *readParam = sysParamPtr->paramNeedleWash.threeHoleNeedle1InjectWaitUpMs;
            break;
        case INDEX_SUB_PARAM_NEEDLE_WASH_BLOCK_UP_THREHOLD:
            *readParam = sysParamPtr->paramNeedleWash.blockUpThrehold;
            break;
        default://不支持的参数
            *readParam = 0;
            return ERROR_SUB_SYS_PARAM_READ_SUB_INDEX;
    }
    //返回结果
    return ERROR_SUB_OK;
}

ERROR_SUB SysParamWriteParamNeedleWash(uint8_t subIndex,int32_t writeParam)
{
    int32_t* targetParamPtr = NULL;
    switch(subIndex)
    {
        case INDEX_SUB_PARAM_NEEDLE_WASH_RESET_CORRECT_UPDOWN:
            targetParamPtr = &(sysParamPtr->paramNeedleWash.resetCorrectUpDown);
            break;
        case INDEX_SUB_PARAM_NEEDLE_WASH_POS_CUP_TOP_NEEDLE_UPDOWN:
            targetParamPtr = &(sysParamPtr->paramNeedleWash.posCupTopNeedleUpDown);
            break;
        case INDEX_SUB_PARAM_NEEDLE_WASH_POS_CUP_BOTTOM_NEEDLE_UPDOWN:
            targetParamPtr = &(sysParamPtr->paramNeedleWash.posCupBottomNeedleUpDown);
            break;
        case INDEX_SUB_PARAM_NEEDLE_WASH_UL_ABSORB_WASH_PUMP1:
            targetParamPtr = &(sysParamPtr->paramNeedleWash.ulAbsorbWashPump1);
            break;
        case INDEX_SUB_PARAM_NEEDLE_WASH_UL_ABSORB_WASH_PUMP2:
            targetParamPtr = &(sysParamPtr->paramNeedleWash.ulAbsorbWashPump2);
            break;
        case INDEX_SUB_PARAM_NEEDLE_WASH_UL_ABSORB_WASH_PUMP3:
            targetParamPtr = &(sysParamPtr->paramNeedleWash.ulAbsorbWashPump3);
            break;
        case INDEX_SUB_PARAM_NEEDLE_WASH_UL_ABSORB_WASH_PUMP4:
            targetParamPtr = &(sysParamPtr->paramNeedleWash.ulAbsorbWashPump4);
            break;
        case INDEX_SUB_PARAM_NEEDLE_WASH_UL_ABSORB_WASH_PUMP5:
            targetParamPtr = &(sysParamPtr->paramNeedleWash.ulAbsorbWashPump5);
            break;
        case INDEX_SUB_PARAM_NEEDLE_WASH_TIME_MS_WAIT_WASTE_LIQUID_AT_CUP_BOTTOM:
            targetParamPtr = &(sysParamPtr->paramNeedleWash.timeMsWaitWasteLiquidAtCupBottom);
            break;
        case INDEX_SUB_PARAM_NEEDLE_WASH_TIME_MS_WAIT_INJECT_STABLE_AT_CUP_TOP:
            targetParamPtr = &(sysParamPtr->paramNeedleWash.timeMsWaitInjectStableAtCupTop);
            break;
        case INDEX_SUB_PARAM_NEEDLE_WASH_TIME_MS_WAIT_WASTE_LIQUID_AT_PRIME:
            targetParamPtr = &(sysParamPtr->paramNeedleWash.timeMsWaitWasteLiquidAtPrime);
            break;
        case INDEX_SUB_PARAM_NEEDLE_WASH_POS_UPDOWN_TO_SOAK:
            targetParamPtr = &(sysParamPtr->paramNeedleWash.posUpdownToSoak);
            break;
        case INDEX_SUB_PARAM_NEEDLA_WASH_TIME_MS_MAINTAIN_CLEAN:
            targetParamPtr = &(sysParamPtr->paramNeedleWash.maintainCleanTime);
            break;
        case INDEX_SUB_PARAM_NEEDLE_WASH_THREE_HOLE_NEEDLE_2_5_CUP_BUTTOM_INJECT_DELAY_TIME:
            targetParamPtr = &(sysParamPtr->paramNeedleWash.threeHoleNeedle2_5InjectDelayMs);
            break;
        case INDEX_SUB_PARAM_NEEDLE_WASH_THREE_HOLE_NEEDLE_1_INJECT_WAIT_NEEDLE_UP_TIME:
            targetParamPtr = &(sysParamPtr->paramNeedleWash.threeHoleNeedle1InjectWaitUpMs);
            break;
        case INDEX_SUB_PARAM_NEEDLE_WASH_BLOCK_UP_THREHOLD:
            targetParamPtr = &(sysParamPtr->paramNeedleWash.blockUpThrehold);
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
//读写 缓冲液注液 参数集合
ERROR_SUB SysParamReadParamBufferInject(uint8_t subIndex,int32_t* readParam)
{
    switch(subIndex)
    {
        case INDEX_SUB_PARAM_BUFFER_INJECT_RESET_CORRECT_PUMP:
            *readParam = sysParamPtr->paramBufferInject.resetCorrectPump;
            break;
        default://不支持的参数
            *readParam = 0;
            return ERROR_SUB_SYS_PARAM_READ_SUB_INDEX;
    }
    //返回结果
    return ERROR_SUB_OK;
}

ERROR_SUB SysParamWriteParamBufferInject(uint8_t subIndex,int32_t writeParam)
{
    int32_t* targetParamPtr = NULL;
    switch(subIndex)
    {
        case INDEX_SUB_PARAM_BUFFER_INJECT_RESET_CORRECT_PUMP:
            targetParamPtr = &(sysParamPtr->paramBufferInject.resetCorrectPump);
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
//读写 液路自动化 参数集合
ERROR_SUB SysParamReadParamLiquidAuto(uint8_t subIndex,int32_t* readParam)
{
    switch(subIndex)
    {
        case INDEX_SUB_PARAM_LIQUID_AUTO_RESERVE:
            *readParam = sysParamPtr->paramLiquidAuto.paramReserve;
            break;
        default://不支持的参数
            *readParam = 0;
            return ERROR_SUB_SYS_PARAM_READ_SUB_INDEX;
    }
    //返回结果
    return ERROR_SUB_OK;
}

ERROR_SUB SysParamWriteParamLiquidAuto(uint8_t subIndex,int32_t writeParam)
{
    int32_t* targetParamPtr = NULL;
    switch(subIndex)
    {
        case INDEX_SUB_PARAM_LIQUID_AUTO_RESERVE:
            targetParamPtr = &(sysParamPtr->paramLiquidAuto.paramReserve);
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
//读写 其他参数 参数集合
ERROR_SUB SysParamReadParamOther(uint8_t subIndex,int32_t* readParam)
{
    switch(subIndex)
    {
        case INDEX_SUB_PARAM_OTHER_BASE:
            *readParam = sysParamPtr->paramOther.baseReserve;
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
        case INDEX_SUB_PARAM_OTHER_BASE:
            targetParamPtr = &(sysParamPtr->paramOther.baseReserve);
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
    SysParamReadParamPlateWash,
    SysParamReadParamMixWash,
    SysParamReadParamNeedleWash,
    SysParamReadParamBufferInject,
    SysParamReadParamLiquidAuto,
    SysParamReadParamOther,
};

/****************************************参数写入函数指针数组**************************************************/
static const SysParamWriteParamSingleFuncPtr SysParamWriteParamSingleArray[] = {
    SysParamWriteParamPlateWash,
    SysParamWriteParamMixWash,
    SysParamWriteParamNeedleWash,
    SysParamWriteParamBufferInject,
    SysParamWriteParamLiquidAuto,
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



