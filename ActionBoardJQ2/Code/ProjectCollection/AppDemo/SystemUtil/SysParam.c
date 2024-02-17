/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-29 14:07:30 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-29 14:59:01 +0800
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
    //电机修正
    UserMemCopy((void*)(&(sysParamPtr->stepMotorCorrect)),(void*)(&stepMotorCorrectDefault),LENGTH_SYS_PARAM_STEP_MOTOR_CORRECT);
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
//读写电机修正参数
ERROR_SUB SysParamReadParamStepMotorCorrect(uint8_t subIndex,int32_t* readParam)
{
    switch(subIndex)
    {
        case INDEX_SUB_PARAM_STEP_MOTOR_CORRECT_1_RESET:
            *readParam = sysParamPtr->stepMotorCorrect.stepMotor1ResetCorrect;
            break;
        case INDEX_SUB_PARAM_STEP_MOTOR_CORRECT_1_DIR_SWITCH:
            *readParam = sysParamPtr->stepMotorCorrect.stepMotor1DirSwitchCorrect;
            break;
        case INDEX_SUB_PARAM_STEP_MOTOR_CORRECT_2_RESET:
            *readParam = sysParamPtr->stepMotorCorrect.stepMotor2ResetCorrect;
            break;
        case INDEX_SUB_PARAM_STEP_MOTOR_CORRECT_2_DIR_SWITCH:
            *readParam = sysParamPtr->stepMotorCorrect.stepMotor2DirSwitchCorrect;
            break;
        default://不支持的参数
            *readParam = 0;
            return ERROR_SUB_SYS_PARAM_READ_SUB_INDEX;
    }
    //返回结果
    return ERROR_SUB_OK;
}

ERROR_SUB SysParamWriteParamStepMotorCorrect(uint8_t subIndex,int32_t writeParam)
{
    int32_t* targetParamPtr = NULL;
    switch(subIndex)
    {
        case INDEX_SUB_PARAM_STEP_MOTOR_CORRECT_1_RESET:
            targetParamPtr = &(sysParamPtr->stepMotorCorrect.stepMotor1ResetCorrect);
            break;
        case INDEX_SUB_PARAM_STEP_MOTOR_CORRECT_1_DIR_SWITCH:
            targetParamPtr = &(sysParamPtr->stepMotorCorrect.stepMotor1DirSwitchCorrect);
            break;
        case INDEX_SUB_PARAM_STEP_MOTOR_CORRECT_2_RESET:
            targetParamPtr = &(sysParamPtr->stepMotorCorrect.stepMotor2ResetCorrect);
            break;
        case INDEX_SUB_PARAM_STEP_MOTOR_CORRECT_2_DIR_SWITCH:
            targetParamPtr = &(sysParamPtr->stepMotorCorrect.stepMotor2DirSwitchCorrect);
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
    SysParamReadParamStepMotorCorrect,
};

/****************************************参数写入函数指针数组**************************************************/
static const SysParamWriteParamSingleFuncPtr SysParamWriteParamSingleArray[] = {
    SysParamWriteParamStepMotorCorrect,
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



