/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-24 13:00:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2024-01-10 09:36:33 +0800
************************************************************************************************/ 
#include "CanMasterMeasureModule.h"
#include "DriverConfigSystem.h"
#include SYS_LOG_INCLUDE_FILE

//通道的模块号和板号定义
#define CAN_MODULE_NO_MEASURE_MODULE        CAN_MODULE_NO_BOARD3
#define CAN_BOARD_ID_MEASURE_MODULE         CAN_BOARD_ID_BOARD3

//通道配置数组
//参数分别是 指令  是否需要结果  超时时间  返回最小参数长度 附带参数长度
//附带参数长度为不定长的时候,指示参数长度为0,由上级调用者手动申请内存
static const CAN_PROT_CMD_CONFIG canMasterMeasureModuleCmdConfigArray[] = {
    {CAN_MASTER_CMD_H360_MEASURE_MODULE_PORT_IN            , ENABLE, 3000 ,  2,  1 },//模块内输入读取
    {CAN_MASTER_CMD_H360_MEASURE_MODULE_PORT_OUT           , ENABLE, 3000 ,  1,  2 },//模块内输出写入
    {CAN_MASTER_CMD_H360_MEASURE_MODULE_MOTOR_RESET        , ENABLE, 60000,  5,  1 },//模块内电机复位
    {CAN_MASTER_CMD_H360_MEASURE_MODULE_MOTOR_RUN_STEPS    , ENABLE, 60000,  5,  5 },//模块内电机走步数
    {CAN_MASTER_CMD_H360_MEASURE_MODULE_RESET              , ENABLE, 60000,  0,  1 },//测量模块复位
    {CAN_MASTER_CMD_H360_MEASURE_MODULE_DOOR_OPEN_FULL     , ENABLE, 5000 ,  0,  0 },//测量上门全开
    {CAN_MASTER_CMD_H360_MEASURE_MODULE_DOOR_OPEN_HALF     , ENABLE, 5000 ,  0,  0 },//测量上门半开
    {CAN_MASTER_CMD_H360_MEASURE_MODULE_DOOR_CLOSE         , ENABLE, 5000 ,  0,  0 },//测量上门关闭
    {CAN_MASTER_CMD_H360_MEASURE_MODULE_WINDOW_OPEN        , ENABLE, 5000 ,  0,  0 },//测量侧门打开
    {CAN_MASTER_CMD_H360_MEASURE_MODULE_WINDOW_CLOSE       , ENABLE, 5000 ,  0,  0 },//测量侧门关闭
    {CAN_MASTER_CMD_H360_MEASURE_MODULE_PRIME_ONCE         , ENABLE, 60000,  0,  1 },//激发液灌注
    {CAN_MASTER_CMD_H360_MEASURE_MODULE_INJECT_ONCE        , ENABLE, 5000 ,  0,  1 },//激发液注液
    {CAN_MASTER_CMD_H360_MEASURE_MODULE_INJECT_MAINTAIN    , ENABLE, 5000 ,  0,  1 },//测量模块清洗维护
    {CAN_MASTER_CMD_H360_MEASURE_MODULE_READ_DARK          , ENABLE, 10000,  4,  0 },//测量模块暗计数
    {CAN_MASTER_CMD_H360_MEASURE_MODULE_BACK_GROUND        , ENABLE, 10000,  4,  0 },//测量模块读本底
    {CAN_MASTER_CMD_H360_MEASURE_MODULE_READ_LIGHT_INJECT  , ENABLE, 10000,  4,  1 },//测量模块注液测光
    {CAN_MASTER_CMD_H360_MEASURE_MODULE_READ_LIGHT_COMPLEX , ENABLE, 30000,  8,  2 },//测量模块集成单次测量
    {CAN_MASTER_CMD_H360_MEASURE_MODULE_READ_LIGHT_RATE    , ENABLE, 30000,  1,  2 },//测量模块读光子变化率
};

/*************************************发送指令等待ACK************************************************/
//模块内输入读取
void CanMasterMeasureModuleReadSingleWhileAck(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_MEASURE_MODULE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_MEASURE_MODULE_PORT_IN;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterMeasureModuleCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterMeasureModuleCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)modInIdx;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//模块内输出写入
void CanMasterMeasureModuleWriteSingleWhileAck(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_MEASURE_MODULE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_MEASURE_MODULE_PORT_OUT;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterMeasureModuleCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterMeasureModuleCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)modOutIdx;
    canCmdSendPtr->cmdBufferPtr[1] = (uint8_t)pinState;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//模块内电机复位
void CanMasterMeasureModuleResetMotorWhileAck(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_MEASURE_MODULE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_MEASURE_MODULE_MOTOR_RESET;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterMeasureModuleCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterMeasureModuleCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)modMotorIdx;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//模块内电机走位
void CanMasterMeasureModuleRunStepsMotorWhileAck(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_MEASURE_MODULE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_MEASURE_MODULE_MOTOR_RUN_STEPS;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterMeasureModuleCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterMeasureModuleCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)modMotorIdx;
    ProtUtilWriteInt32ToBuffer(steps,canCmdSendPtr->cmdBufferPtr+1);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}


//测量模块复位
void CanMasterMeasureModuleResetWhileAck(BOTTLE_SELECT_MEASURE_MODULE bottle,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_MEASURE_MODULE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_MEASURE_MODULE_RESET;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterMeasureModuleCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterMeasureModuleCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)bottle;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//测量上门全开
void CanMasterMeasureModuleDoorOpenFullWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_MEASURE_MODULE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_MEASURE_MODULE_DOOR_OPEN_FULL;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterMeasureModuleCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterMeasureModuleCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//测量上门半开
void CanMasterMeasureModuleDoorOpenHalfWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_MEASURE_MODULE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_MEASURE_MODULE_DOOR_OPEN_HALF;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterMeasureModuleCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterMeasureModuleCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//测量上门关闭
void CanMasterMeasureModuleDoorCloseWhileAck(SYS_RESULT_PACK* resultPackPtr)
{   
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_MEASURE_MODULE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_MEASURE_MODULE_DOOR_CLOSE;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterMeasureModuleCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterMeasureModuleCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//测量侧门打开
void CanMasterMeasureModuleWindowOpenWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_MEASURE_MODULE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_MEASURE_MODULE_WINDOW_OPEN;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterMeasureModuleCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterMeasureModuleCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//测量侧门关闭
void CanMasterMeasureModuleWindowCloseWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_MEASURE_MODULE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_MEASURE_MODULE_WINDOW_CLOSE;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterMeasureModuleCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterMeasureModuleCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//激发液灌注
void CanMasterMeasureModulePrimeOnceWhileAck(BOTTLE_SELECT_MEASURE_MODULE bottle,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_MEASURE_MODULE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_MEASURE_MODULE_PRIME_ONCE;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterMeasureModuleCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterMeasureModuleCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)bottle;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//激发液注液
void CanMasterMeasureModuleInjectOnceWhileAck(BOTTLE_SELECT_MEASURE_MODULE bottle,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_MEASURE_MODULE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_MEASURE_MODULE_INJECT_ONCE;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterMeasureModuleCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterMeasureModuleCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)bottle;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//测量模块清洗维护
void CanMasterMeasureModuleInjectMaintainWhileAck(BOTTLE_SELECT_MEASURE_MODULE bottle,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_MEASURE_MODULE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_MEASURE_MODULE_INJECT_MAINTAIN;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterMeasureModuleCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterMeasureModuleCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)bottle;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//测量模块暗计数
void CanMasterMeasureModuleReadDarkWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_MEASURE_MODULE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_MEASURE_MODULE_READ_DARK;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterMeasureModuleCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterMeasureModuleCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//测量模块读本底
void CanMasterMeasureModuleReadBackGroundWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_MEASURE_MODULE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_MEASURE_MODULE_BACK_GROUND;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterMeasureModuleCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterMeasureModuleCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//测量模块注液测光
void CanMasterMeasureModuleReadLightWithInjectWhileAck(BOTTLE_SELECT_MEASURE_MODULE bottle,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_MEASURE_MODULE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_MEASURE_MODULE_READ_LIGHT_INJECT;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterMeasureModuleCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterMeasureModuleCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)bottle;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//测量模块集成单次测量
void CanMasterMeasureModuleReadLightComplexWhileAck(BOTTLE_SELECT_MEASURE_MODULE bottle,FunctionalState enableInject,
                                                        SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_MEASURE_MODULE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_MEASURE_MODULE_READ_LIGHT_COMPLEX;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterMeasureModuleCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterMeasureModuleCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)bottle;
    canCmdSendPtr->cmdBufferPtr[1] = (uint8_t)enableInject;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//测量模块读光子变化率
void CanMasterMeasureModuleReadLightChangeRateWhileAck(BOTTLE_SELECT_MEASURE_MODULE bottle,uint8_t setChangeCount,
                                                        SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_MEASURE_MODULE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_MEASURE_MODULE_READ_LIGHT_RATE;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterMeasureModuleCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterMeasureModuleCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)bottle;
    canCmdSendPtr->cmdBufferPtr[1] = (uint8_t)setChangeCount;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}


/*************************************等待指定指令完成返回************************************************/
//模块内输入读取
void CanMasterMeasureModuleWaitReadSingleResult(SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_MEASURE_MODULE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_MEASURE_MODULE_PORT_IN;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(pinStatePtr != NULL)
    {
        //结果解析
        *pinStatePtr = PIN_STATE_INVALID;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *pinStatePtr = (PIN_STATE)(returnDataBufferPtr[1]);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//模块内输出写入
void CanMasterMeasureModuleWaitWriteSingleResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_MEASURE_MODULE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_MEASURE_MODULE_PORT_OUT;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//模块内电机复位
void CanMasterMeasureModuleWaitResetMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_MEASURE_MODULE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_MEASURE_MODULE_MOTOR_RESET;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(motorCurPosPtr != NULL)
    {
        //结果解析
        *motorCurPosPtr = 0;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *motorCurPosPtr = ProtUtilReadInt32Buffer(returnDataBufferPtr+1);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//模块内电机走位
void CanMasterMeasureModuleWaitRunStepsMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_MEASURE_MODULE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_MEASURE_MODULE_MOTOR_RUN_STEPS;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(motorCurPosPtr != NULL)
    {
        //结果解析
        *motorCurPosPtr = 0;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *motorCurPosPtr = ProtUtilReadInt32Buffer(returnDataBufferPtr+1);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//测量模块复位
void CanMasterMeasureModuleWaitResetResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_MEASURE_MODULE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_MEASURE_MODULE_RESET;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//测量上门全开
void CanMasterMeasureModuleWaitDoorOpenFullResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_MEASURE_MODULE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_MEASURE_MODULE_DOOR_OPEN_FULL;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//测量上门半开
void CanMasterMeasureModuleWaitDoorOpenHalfResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_MEASURE_MODULE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_MEASURE_MODULE_DOOR_OPEN_HALF;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//测量上门关闭
void CanMasterMeasureModuleWaitDoorCloseResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_MEASURE_MODULE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_MEASURE_MODULE_DOOR_CLOSE;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//测量侧门打开
void CanMasterMeasureModuleWaitWindowOpenResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_MEASURE_MODULE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_MEASURE_MODULE_WINDOW_OPEN;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//测量侧门关闭
void CanMasterMeasureModuleWaitWindowCloseResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_MEASURE_MODULE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_MEASURE_MODULE_WINDOW_CLOSE;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//激发液灌注
void CanMasterMeasureModuleWaitPrimeOnceResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_MEASURE_MODULE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_MEASURE_MODULE_PRIME_ONCE;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//激发液注液
void CanMasterMeasureModuleWaitInjectOnceResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_MEASURE_MODULE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_MEASURE_MODULE_INJECT_ONCE;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//测量模块清洗维护
void CanMasterMeasureModuleWaitInjectMaintainResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_MEASURE_MODULE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_MEASURE_MODULE_INJECT_MAINTAIN;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//测量模块暗计数
void CanMasterMeasureModuleWaitReadDarkResult(SYS_RESULT_PACK* resultPackPtr,uint32_t* darkValPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_MEASURE_MODULE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_MEASURE_MODULE_READ_DARK;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(darkValPtr != NULL)
    {
        //结果解析
        *darkValPtr = 0;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *darkValPtr = ProtUtilReadUint32Buffer(returnDataBufferPtr);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//测量模块读本底
void CanMasterMeasureModuleWaitReadBackGroundResult(SYS_RESULT_PACK* resultPackPtr,uint32_t* backGroundValPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_MEASURE_MODULE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_MEASURE_MODULE_BACK_GROUND;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(backGroundValPtr != NULL)
    {
        //结果解析
        *backGroundValPtr = 0;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *backGroundValPtr = ProtUtilReadUint32Buffer(returnDataBufferPtr);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//测量模块注液测光
void CanMasterMeasureModuleWaitReadLightWithInjectResult(SYS_RESULT_PACK* resultPackPtr,uint32_t* lightValPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_MEASURE_MODULE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_MEASURE_MODULE_READ_LIGHT_INJECT;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(lightValPtr != NULL)
    {
        //结果解析
        *lightValPtr = 0;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *lightValPtr = ProtUtilReadUint32Buffer(returnDataBufferPtr);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//测量模块集成单次测量
void CanMasterMeasureModuleWaitReadLightComplexResult(SYS_RESULT_PACK* resultPackPtr,uint32_t* backGroundValPtr,uint32_t* lightValPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_MEASURE_MODULE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_MEASURE_MODULE_READ_LIGHT_COMPLEX;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(backGroundValPtr != NULL)
    {
        //结果解析
        *backGroundValPtr = 0;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *backGroundValPtr = ProtUtilReadUint32Buffer(returnDataBufferPtr);
        }
    }
    if(lightValPtr != NULL)
    {
        //结果解析
        *lightValPtr = 0;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *lightValPtr = ProtUtilReadUint32Buffer(returnDataBufferPtr+4);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//测量模块读光子变化率
void CanMasterMeasureModuleWaitReadLightChangeRateResult(SYS_RESULT_PACK* resultPackPtr,uint8_t* pointCountPtr,uint32_t** lightArrayPtrPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_MEASURE_MODULE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_MEASURE_MODULE_READ_LIGHT_RATE;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(pointCountPtr != NULL)
    {
        //结果解析
        *pointCountPtr = 0;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *pointCountPtr = returnDataBufferPtr[0];
        }
    }
    uint8_t indexUtil = 0;
    if(lightArrayPtrPtr != NULL)
    {
        *lightArrayPtrPtr = NULL;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            //申请内存
            *lightArrayPtrPtr = (uint32_t*)CanProtMemMalloc(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,returnDataBufferLength -1);
            for(indexUtil = 0; indexUtil < returnDataBufferPtr[0];indexUtil++)
            {
                //解析获取数据
                (*lightArrayPtrPtr)[indexUtil] = ProtUtilReadUint32Buffer(returnDataBufferPtr+(4*indexUtil)+1);
            }
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//释放注液测光点集合
void CanMasterMeasureModuleReleaseMem(void* ptr)
{
    if(ptr != NULL)
    {
        CanProtMemFree(CAN_MODULE_NO_MEASURE_MODULE,CAN_BOARD_ID_MEASURE_MODULE,ptr);
    }
}

/*************************************发送指令等待完成返回************************************************/
//模块内输入读取
void CanMasterMeasureModuleReadSingleWhileReturn(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr)
{
    //发送指令等待ACK
    CanMasterMeasureModuleReadSingleWhileAck(modInIdx,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterMeasureModuleReadSingleWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterMeasureModuleWaitReadSingleResult(resultPackPtr,pinStatePtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterMeasureModuleWaitReadSingleResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//模块内输出写入
void CanMasterMeasureModuleWriteSingleWhileReturn(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterMeasureModuleWriteSingleWhileAck(modOutIdx,pinState,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterMeasureModuleWriteSingleWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterMeasureModuleWaitWriteSingleResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterMeasureModuleWaitWriteSingleResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//模块内电机复位
void CanMasterMeasureModuleResetMotorWhileReturn(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr)
{
    //发送指令等待ACK
    CanMasterMeasureModuleResetMotorWhileAck(modMotorIdx,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterMeasureModuleResetMotorWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterMeasureModuleWaitResetMotorResult(resultPackPtr,motorCurPosPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterMeasureModuleWaitResetMotorResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//模块内电机走位
void CanMasterMeasureModuleRunStepsMotorWhileReturn(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr)
{
    //发送指令等待ACK
    CanMasterMeasureModuleRunStepsMotorWhileAck(modMotorIdx,steps,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterMeasureModuleRunStepsMotorWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterMeasureModuleWaitRunStepsMotorResult(resultPackPtr,motorCurPosPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterMeasureModuleWaitRunStepsMotorResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//测量模块复位
void CanMasterMeasureModuleResetWhileReturn(BOTTLE_SELECT_MEASURE_MODULE bottle,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterMeasureModuleResetWhileAck(bottle,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterMeasureModuleResetWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterMeasureModuleWaitResetResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterMeasureModuleWaitResetResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//测量上门全开
void CanMasterMeasureModuleDoorOpenFullWhileReturn(SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterMeasureModuleDoorOpenFullWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterMeasureModuleDoorOpenFullWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterMeasureModuleWaitDoorOpenFullResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterMeasureModuleWaitDoorOpenFullResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//测量上门半开
void CanMasterMeasureModuleDoorOpenHalfWhileReturn(SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterMeasureModuleDoorOpenHalfWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterMeasureModuleDoorOpenHalfWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterMeasureModuleWaitDoorOpenHalfResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterMeasureModuleWaitDoorOpenHalfResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//测量上门关闭
void CanMasterMeasureModuleDoorCloseWhileReturn(SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterMeasureModuleDoorCloseWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterMeasureModuleDoorCloseWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterMeasureModuleWaitDoorCloseResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterMeasureModuleWaitDoorCloseResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//测量侧门打开
void CanMasterMeasureModuleWindowOpenWhileReturn(SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterMeasureModuleWindowOpenWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterMeasureModuleWindowOpenWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterMeasureModuleWaitWindowOpenResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterMeasureModuleWaitWindowOpenResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//测量侧门关闭
void CanMasterMeasureModuleWindowCloseWhileReturn(SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterMeasureModuleWindowCloseWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterMeasureModuleWindowCloseWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterMeasureModuleWaitWindowCloseResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterMeasureModuleWaitWindowCloseResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//激发液灌注
void CanMasterMeasureModulePrimeOnceWhileReturn(BOTTLE_SELECT_MEASURE_MODULE bottle,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterMeasureModulePrimeOnceWhileAck(bottle,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterMeasureModulePrimeOnceWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterMeasureModuleWaitPrimeOnceResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterMeasureModuleWaitPrimeOnceResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//激发液注液
void CanMasterMeasureModuleInjectOnceWhileReturn(BOTTLE_SELECT_MEASURE_MODULE bottle,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterMeasureModuleInjectOnceWhileAck(bottle,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterMeasureModuleInjectOnceWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterMeasureModuleWaitInjectOnceResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterMeasureModuleWaitInjectOnceResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//测量模块清洗维护
void CanMasterMeasureModuleInjectMaintainWhileReturn(BOTTLE_SELECT_MEASURE_MODULE bottle,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterMeasureModuleInjectMaintainWhileAck(bottle,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterMeasureModuleInjectMaintainWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterMeasureModuleWaitInjectMaintainResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterMeasureModuleWaitInjectMaintainResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//测量模块暗计数
void CanMasterMeasureModuleReadDarkWhileReturn(SYS_RESULT_PACK* resultPackPtr,uint32_t* darkValPtr)
{
    //发送指令等待ACK
    CanMasterMeasureModuleReadDarkWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterMeasureModuleReadDarkWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterMeasureModuleWaitReadDarkResult(resultPackPtr,darkValPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterMeasureModuleWaitReadDarkResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//测量模块读本底
void CanMasterMeasureModuleReadBackGroundWhileReturn(SYS_RESULT_PACK* resultPackPtr,uint32_t* backGroundValPtr)
{
    //发送指令等待ACK
    CanMasterMeasureModuleReadBackGroundWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterMeasureModuleReadBackGroundWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterMeasureModuleWaitReadBackGroundResult(resultPackPtr,backGroundValPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterMeasureModuleWaitReadBackGroundResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//测量模块注液测光
void CanMasterMeasureModuleReadLightWithInjectWhileReturn(BOTTLE_SELECT_MEASURE_MODULE bottle,SYS_RESULT_PACK* resultPackPtr,
                                                            uint32_t* lightValPtr)
{
    //发送指令等待ACK
    CanMasterMeasureModuleReadLightWithInjectWhileAck(bottle,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterMeasureModuleReadLightWithInjectWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterMeasureModuleWaitReadLightWithInjectResult(resultPackPtr,lightValPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterMeasureModuleWaitReadLightWithInjectResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//测量模块集成单次测量
void CanMasterMeasureModuleReadLightComplexWhileReturn(BOTTLE_SELECT_MEASURE_MODULE bottle,FunctionalState enableInject,
                                                        SYS_RESULT_PACK* resultPackPtr,uint32_t* backGroundValPtr,
                                                        uint32_t* lightValPtr)
{
    //发送指令等待ACK
    CanMasterMeasureModuleReadLightComplexWhileAck(bottle,enableInject,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterMeasureModuleReadLightComplexWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterMeasureModuleWaitReadLightComplexResult(resultPackPtr,backGroundValPtr,lightValPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterMeasureModuleWaitReadLightComplexResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//测量模块读光子变化率
void CanMasterMeasureModuleReadLightChangeRateWhileReturn(BOTTLE_SELECT_MEASURE_MODULE bottle,uint8_t setChangeCount,
                                                            SYS_RESULT_PACK* resultPackPtr,uint8_t* pointCountPtr,
                                                            uint32_t** lightArrayPtrPtr)
{
    //发送指令等待ACK
    CanMasterMeasureModuleReadLightChangeRateWhileAck(bottle,setChangeCount,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterMeasureModuleReadLightChangeRateWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterMeasureModuleWaitReadLightChangeRateResult(resultPackPtr,pointCountPtr,lightArrayPtrPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterMeasureModuleWaitReadLightChangeRateResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}






















