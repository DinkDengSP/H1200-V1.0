/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-22 17:36:26 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-04 11:23:02 +0800
************************************************************************************************/ 
#include "H360SysActModPort.h"

//模块读取指定输入
ERROR_SUB H360SysActModInputRead(uint8_t index,uint8_t indexMax,MODULE_INPUT_MAP_UNIT const* arrayPtr,PIN_STATE* state)
{
    ERROR_SUB errorCode = ERROR_SUB_OK;
    GPIO_PinState inputState;
    //序号错误
    if(index >= indexMax)
    {
        return ERROR_SUB_MODULE_READ_PORT_IN_INDEX;
    }
    //预留参数无效
    if(arrayPtr[index].inputIndex == MODULE_BASE_INPUT_PIN_RESERVE)
    {
        return ERROR_SUB_SYS_READ_PORT_IN_INDEX;
    }
    //读取输入
    errorCode = H360SysActPortInRead((H360_SYS_PORT_IN)arrayPtr[index].inputIndex,&inputState);
    if(errorCode != ERROR_SUB_OK)
    {
        return errorCode;
    }
    //根据读取到的结果转换成有效无效变量
    (inputState == arrayPtr[index].validState)?(*state = PIN_STATE_VALID):(*state = PIN_STATE_INVALID);
    return ERROR_SUB_OK;
}

//模块写入指定输出
ERROR_SUB H360SysActModOutputWrite(uint8_t index,PIN_STATE state,uint8_t indexMax,MODULE_OUTPUT_MAP_UNIT const* arrayPtr)
{
    ERROR_SUB errorCode = ERROR_SUB_OK;
    GPIO_PinState outputState;
    //序号错误
    if(index >= indexMax)
    {
        return ERROR_SUB_MODULE_WRITE_PORT_OUT_INDEX;
    }
    //预留参数无效
    if(arrayPtr[index].outputIndex == MODULE_BASE_OUTPUT_PIN_RESERVE)
    {
        return ERROR_SUB_SYS_WRITE_PORT_OUT_INDEX;
    }
    //根据配置文件转换出真正要写入的状态
    (state == PIN_STATE_VALID)?(outputState = arrayPtr[index].validState):(outputState = (GPIO_PinState)(!(arrayPtr[index].validState)));
    //写入
    errorCode = H360SysActPortOutWrite((H360_SYS_PORT_OUT)arrayPtr[index].outputIndex,outputState);
    if(errorCode != ERROR_SUB_OK)
    {
        return errorCode;
    }
    return ERROR_SUB_OK;
}






















