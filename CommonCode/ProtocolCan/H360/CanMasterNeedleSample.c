/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-24 13:00:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-20 15:04:31 +0800
************************************************************************************************/ 
#include "CanMasterNeedleSample.h"
#include "DriverConfigSystem.h"
#include SYS_LOG_INCLUDE_FILE

//通道的模块号和板号定义
#define CAN_MODULE_NO_NEEDLE_SAMPLE        CAN_MODULE_NO_BOARD5
#define CAN_BOARD_ID_NEEDLE_SAMPLE         CAN_BOARD_ID_BOARD5

//通道配置数组
//参数分别是 指令  是否需要结果  超时时间  返回最小参数长度 附带参数长度
//附带参数长度为不定长的时候,指示参数长度为0,由上级调用者手动申请内存
static const CAN_PROT_CMD_CONFIG canMasterNeedleSampleCmdConfigArray[] = {
    {CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_PORT_IN                         , ENABLE, 3000 ,  2,  1 },//模块内输入读取
    {CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_PORT_OUT                        , ENABLE, 3000 ,  1,  2 },//模块内输出写入
    {CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_MOTOR_RESET                     , ENABLE, 60000,  5,  1 },//模块内电机复位
    {CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_MOTOR_RUN_STEPS                 , ENABLE, 60000,  5,  5 },//模块内电机走步数
    {CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_RESET                           , ENABLE, 60000,  0,  0 },//样本针复位
    {CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_PRIME_NORMAL                    , ENABLE, 60000,  0,  1 },//样本针清洗液灌注
    {CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_PRIME_POWER                     , ENABLE, 60000,  0,  0 },//样本针强力清洗液灌注
    {CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_CLEAN                           , ENABLE, 60000,  0,  2 },//样本针清洗
    {CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_CLEAN_POWER                     , ENABLE, 60000,  0,  1 },//样本针强力清洗
    {CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_MAINTAIN                        , ENABLE, 60000,  0,  2 },//样本针清洗维护
    {CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_ROTATE                          , ENABLE, 60000,  0,  1 },//样本针旋转指定位置
    {CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_LIQUID_ABSORB_TUBE              , ENABLE, 60000,  14, 9 },//样本针吸试管样本
    {CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_LIQUID_ABSORB_PLATE             , ENABLE, 60000,  8,  8 },//样本针吸反应盘样本
    {CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_LIQUID_INJECT_NORMAL            , ENABLE, 60000,  0,  5 },//样本针注液
    {CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_LIQUID_DETECT_ADJUST            , ENABLE, 60000,  0,  0 },//样本针探液校准
    {CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_LIQUID_DETECT_READ_STATE        , ENABLE, 60000,  6,  1 },//读样本针探液板状态
    {CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_LIQUID_DETECT_READ_RES_AND_VOL  , ENABLE, 60000,  4,  0 },//读取电阻和针尖电压
    {CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_LIQUID_DETECT_WRITE_RES         , ENABLE, 60000,  0,  2 },//写入电阻值
    {CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_LIQUID_DETECT_READ_BASE_VOL     , ENABLE, 60000,  2,  0 },//读取针尖基准电压
    {CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_READ_NEEDLE_UP_DOWN_EM_STOP_POS , ENABLE, 60000,  4,  0 },//读样本针升降急停高度
    {CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_READ_LIQUID_BOARD_VERSION       , ENABLE, 60000,  1,  0 },//读探液板程序版本
};

/*************************************发送指令等待ACK************************************************/
//模块内输入读取
void CanMasterNeedleSampleReadSingleWhileAck(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_SAMPLE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_PORT_IN;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleSampleCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleSampleCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)modInIdx;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//模块内输出写入
void CanMasterNeedleSampleWriteSingleWhileAck(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_SAMPLE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_PORT_OUT;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleSampleCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleSampleCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,canCmdConfigPtr);
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
void CanMasterNeedleSampleResetMotorWhileAck(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_SAMPLE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_MOTOR_RESET;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleSampleCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleSampleCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)modMotorIdx;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//模块内电机走位
void CanMasterNeedleSampleRunStepsMotorWhileAck(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_SAMPLE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_MOTOR_RUN_STEPS;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleSampleCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleSampleCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)modMotorIdx;
    ProtUtilWriteInt32ToBuffer(steps,canCmdSendPtr->cmdBufferPtr+1);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//样本针复位
void CanMasterNeedleSampleResetWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_SAMPLE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_RESET;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleSampleCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleSampleCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//样本针清洗液灌注
void CanMasterNeedleSampleNormalPrimeWhileAck(NEEDLE_PRIME_OPT opt,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_SAMPLE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_PRIME_NORMAL;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleSampleCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleSampleCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)opt;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//样本针强力清洗液灌注
void CanMasterNeedleSamplePowerPrimeWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_SAMPLE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_PRIME_POWER;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleSampleCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleSampleCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//样本针清洗
void CanMasterNeedleSampleNormalCleanWhileAck(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_SAMPLE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_CLEAN;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleSampleCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleSampleCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)opt;
    canCmdSendPtr->cmdBufferPtr[1] = (uint8_t)time;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//样本针强力清洗
void CanMasterNeedleSamplePowerCleanWhileAck(uint8_t count,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_SAMPLE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_CLEAN_POWER;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleSampleCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleSampleCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)count;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//样本针清洗维护
void CanMasterNeedleSampleMaintainCleanWhileAck(NEEDLE_MAINTAIN_OPT opt,NEEDLE_CLEAN_TIME time,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_SAMPLE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_MAINTAIN;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleSampleCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleSampleCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)opt;
    canCmdSendPtr->cmdBufferPtr[1] = (uint8_t)time;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//样本针旋转指定位置
void CanMasterNeedleSampleRotateWhileAck(NEEDLE_SAMPLE_POS targetPos,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_SAMPLE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_ROTATE;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleSampleCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleSampleCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)targetPos;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//样本针吸试管样本
void CanMasterNeedleSampleLiquidAbsorbSampleTubeWhileAck(uint32_t unitCount,int32_t lastLiquidDetectPos,TUBE_TYPE tubeType,
                                                            SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_SAMPLE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_LIQUID_ABSORB_TUBE;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleSampleCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleSampleCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    ProtUtilWriteUint32ToBuffer(unitCount,canCmdSendPtr->cmdBufferPtr);
    ProtUtilWriteInt32ToBuffer(lastLiquidDetectPos,canCmdSendPtr->cmdBufferPtr+4);
    canCmdSendPtr->cmdBufferPtr[8] = (uint8_t)tubeType;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//样本针吸反应盘样本
void CanMasterNeedleSampleLiquidAbsorbPlateWhileAck(uint32_t unitCount,uint32_t cupLiquidResidueAmp1000,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_SAMPLE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_LIQUID_ABSORB_PLATE;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleSampleCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleSampleCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    ProtUtilWriteUint32ToBuffer(unitCount,canCmdSendPtr->cmdBufferPtr);
    ProtUtilWriteUint32ToBuffer(cupLiquidResidueAmp1000,canCmdSendPtr->cmdBufferPtr+4);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//样本针注液
void CanMasterNeedleSampleLiquidInjectWhileAck(uint32_t cupLiquidUlAmp1000,uint8_t mixCount,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_SAMPLE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_LIQUID_INJECT_NORMAL;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleSampleCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleSampleCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    ProtUtilWriteUint32ToBuffer(cupLiquidUlAmp1000,canCmdSendPtr->cmdBufferPtr);
    canCmdSendPtr->cmdBufferPtr[4] = (uint8_t)mixCount;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//样本针探液校准
void CanMasterNeedleSampleLiquidDetectAdjustWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_SAMPLE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_LIQUID_DETECT_ADJUST;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleSampleCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleSampleCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//读样本针探液板状态
void CanMasterNeedleSampleLiquidDetectReadStateWhileAck(LIQUID_DETECT_CONFIG config,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_SAMPLE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_LIQUID_DETECT_READ_STATE;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleSampleCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleSampleCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)config;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//读取电阻和针尖电压
void CanMasterNeedleSampleLiquidDetectReadResVolWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_SAMPLE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_LIQUID_DETECT_READ_RES_AND_VOL;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleSampleCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleSampleCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//写入电阻值
void CanMasterNeedleSampleLiquidDetectWriteResWhileAck(uint16_t resValue,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_SAMPLE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_LIQUID_DETECT_WRITE_RES;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleSampleCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleSampleCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    ProtUtilWriteUint16ToBuffer(resValue,canCmdSendPtr->cmdBufferPtr);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//读取针尖基准电压
void CanMasterNeedleSampleLiquidDetectReadNeedleBaseVolWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_SAMPLE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_LIQUID_DETECT_READ_BASE_VOL;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleSampleCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleSampleCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//读样本针升降急停高度
void CanMasterNeedleSampleReadNeedleUpDownEmStopPosWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_SAMPLE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_READ_NEEDLE_UP_DOWN_EM_STOP_POS;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleSampleCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleSampleCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//读探液板程序版本
void CanMasterNeedleSampleReadLiquidBoardVersionWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_SAMPLE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_READ_LIQUID_BOARD_VERSION;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleSampleCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleSampleCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

/*************************************等待指定指令完成返回************************************************/
//模块内输入读取
void CanMasterNeedleSampleWaitReadSingleResult(SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_SAMPLE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_PORT_IN;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,returnDataBufferPtr,returnDataBufferLength);
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
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//模块内输出写入
void CanMasterNeedleSampleWaitWriteSingleResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_SAMPLE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_PORT_OUT;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//模块内电机复位
void CanMasterNeedleSampleWaitResetMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_SAMPLE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_MOTOR_RESET;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,returnDataBufferPtr,returnDataBufferLength);
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
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//模块内电机走位
void CanMasterNeedleSampleWaitRunStepsMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_SAMPLE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_MOTOR_RUN_STEPS;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,returnDataBufferPtr,returnDataBufferLength);
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
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//样本针复位
void CanMasterNeedleSampleWaitResetResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_SAMPLE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_RESET;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//样本针清洗液灌注
void CanMasterNeedleSampleWaitNormalPrimeResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_SAMPLE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_PRIME_NORMAL;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//样本针强力清洗液灌注
void CanMasterNeedleSampleWaitPowerPrimeResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_SAMPLE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_PRIME_POWER;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//样本针清洗
void CanMasterNeedleSampleWaitNormalCleanResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_SAMPLE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_CLEAN;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//样本针强力清洗
void CanMasterNeedleSampleWaitPowerCleanResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_SAMPLE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_CLEAN_POWER;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//样本针清洗维护
void CanMasterNeedleSampleWaitMaintainCleanResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_SAMPLE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_MAINTAIN;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//样本针旋转指定位置
void CanMasterNeedleSampleWaitRotateResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_SAMPLE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_ROTATE;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//样本针吸试管样本
void CanMasterNeedleSampleWaitLiquidAbsorbSampleTubeResult(SYS_RESULT_PACK* resultPackPtr,NEEDLE_SAMPLE_ABSORB_TUBE_MSG* absorbMsgPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_SAMPLE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_LIQUID_ABSORB_TUBE;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(absorbMsgPtr != NULL)
    {
        //结果解析
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            absorbMsgPtr->currentLiquidDetectPos = ProtUtilReadInt32Buffer(returnDataBufferPtr);
            absorbMsgPtr->pumpLiquidUnitCount = ProtUtilReadUint32Buffer(returnDataBufferPtr+4);
            absorbMsgPtr->liquidResultRerurn = (LIQUID_FUNC_RESULT)returnDataBufferPtr[8];
            absorbMsgPtr->needleLiquidDetectResult = (LIQUID_NEEDLE_LIQUID_DETECT_RESULT)returnDataBufferPtr[9];
            absorbMsgPtr->liquidDetectBaseVol = ProtUtilReadUint16Buffer(returnDataBufferPtr+10);
            absorbMsgPtr->liquidDetectNeedleVol = ProtUtilReadUint16Buffer(returnDataBufferPtr+12);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//样本针吸反应盘样本
void CanMasterNeedleSampleWaitLiquidAbsorbPlateResult(SYS_RESULT_PACK* resultPackPtr,NEEDLE_SAMPLE_ABSORB_PLATE_MSG* absorbMsgPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_SAMPLE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_LIQUID_ABSORB_PLATE;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(absorbMsgPtr != NULL)
    {
        //结果解析
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            absorbMsgPtr->pumpLiquidUnitCount = ProtUtilReadUint32Buffer(returnDataBufferPtr);
            absorbMsgPtr->needleDownPos = ProtUtilReadInt32Buffer(returnDataBufferPtr+4);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//样本针注液
void CanMasterNeedleSampleWaitLiquidInjectResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_SAMPLE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_LIQUID_INJECT_NORMAL;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//样本针探液校准
void CanMasterNeedleSampleWaitLiquidDetectAdjustResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_SAMPLE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_LIQUID_DETECT_ADJUST;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//读样本针探液板状态
void CanMasterNeedleSampleWaitLiquidDetectReadStateResult(SYS_RESULT_PACK* resultPackPtr,uint16_t* needleVol,uint16_t* thresholdValue,
                                                            uint16_t* slopeValue)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_SAMPLE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_LIQUID_DETECT_READ_STATE;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(needleVol != NULL)
    {
        //结果解析
        *needleVol = 0;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *needleVol = ProtUtilReadUint16Buffer(returnDataBufferPtr);
        }
    }
    if(thresholdValue != NULL)
    {
        //结果解析
        *thresholdValue = 0;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *thresholdValue = ProtUtilReadUint16Buffer(returnDataBufferPtr+2);
        }
    }
    if(slopeValue != NULL)
    {
        //结果解析
        *slopeValue = 0;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *slopeValue = ProtUtilReadUint16Buffer(returnDataBufferPtr+4);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//读取电阻和针尖电压
void CanMasterNeedleSampleWaitLiquidDetectReadResVolResult(SYS_RESULT_PACK* resultPackPtr,uint16_t* resValue,uint16_t* volValue)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_SAMPLE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_LIQUID_DETECT_READ_RES_AND_VOL;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(resValue != NULL)
    {
        //结果解析
        *resValue = 0;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *resValue = ProtUtilReadUint16Buffer(returnDataBufferPtr);
        }
    }
    if(volValue != NULL)
    {
        //结果解析
        *volValue = 0;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *volValue = ProtUtilReadUint16Buffer(returnDataBufferPtr+2);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//写入电阻值
void CanMasterNeedleSampleWaitLiquidDetectWriteResResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_SAMPLE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_LIQUID_DETECT_WRITE_RES;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//读取针尖基准电压
void CanMasterNeedleSampleWaitLiquidDetectReadNeedleBaseVolResult(SYS_RESULT_PACK* resultPackPtr,uint16_t* needleBaseVol)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_SAMPLE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_LIQUID_DETECT_READ_BASE_VOL;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(needleBaseVol != NULL)
    {
        //结果解析
        *needleBaseVol = 0;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *needleBaseVol = ProtUtilReadUint16Buffer(returnDataBufferPtr);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//读样本针升降急停高度
void CanMasterNeedleSampleWaitReadNeedleUpDownEmStopPosResult(SYS_RESULT_PACK* resultPackPtr,int32_t* needleEmStopPosPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_SAMPLE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_READ_NEEDLE_UP_DOWN_EM_STOP_POS;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(needleEmStopPosPtr != NULL)
    {
        //结果解析
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *needleEmStopPosPtr = ProtUtilReadInt32Buffer(returnDataBufferPtr);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//读探液板程序版本
void CanMasterNeedleSampleWaitReadLiquidBoardVersionResult(SYS_RESULT_PACK* resultPackPtr,uint8_t* versionStrLengthPtr,uint8_t** versionCodeBufPtrPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_SAMPLE;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_SAMPLE_READ_LIQUID_BOARD_VERSION;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(versionStrLengthPtr != NULL)
    {
        //结果解析
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *versionStrLengthPtr = returnDataBufferPtr[0];
        }
    }
    if(versionCodeBufPtrPtr != NULL)
    {
        *versionCodeBufPtrPtr = NULL;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            //申请内存
            *versionCodeBufPtrPtr = (uint8_t*)CanProtMemMalloc(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,returnDataBufferLength -1);
            UserMemCopy(*versionCodeBufPtrPtr,returnDataBufferPtr+1,returnDataBufferLength -1);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}


/*************************************发送指令等待完成返回************************************************/
//模块内输入读取
void CanMasterNeedleSampleReadSingleWhileReturn(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr)
{
    //发送指令等待ACK
    CanMasterNeedleSampleReadSingleWhileAck(modInIdx,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleSampleReadSingleWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleSampleWaitReadSingleResult(resultPackPtr,pinStatePtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleSampleWaitReadSingleResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//模块内输出写入
void CanMasterNeedleSampleWriteSingleWhileReturn(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterNeedleSampleWriteSingleWhileAck(modOutIdx,pinState,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleSampleWriteSingleWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleSampleWaitWriteSingleResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleSampleWaitWriteSingleResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//模块内电机复位
void CanMasterNeedleSampleResetMotorWhileReturn(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr)
{
    //发送指令等待ACK
    CanMasterNeedleSampleResetMotorWhileAck(modMotorIdx,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleSampleResetMotorWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleSampleWaitResetMotorResult(resultPackPtr,motorCurPosPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleSampleWaitResetMotorResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//模块内电机走位
void CanMasterNeedleSampleRunStepsMotorWhileReturn(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr)
{
    //发送指令等待ACK
    CanMasterNeedleSampleRunStepsMotorWhileAck(modMotorIdx,steps,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleSampleRunStepsMotorWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleSampleWaitRunStepsMotorResult(resultPackPtr,motorCurPosPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleSampleWaitRunStepsMotorResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//样本针复位
void CanMasterNeedleSampleResetWhileReturn(SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterNeedleSampleResetWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleSampleResetWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleSampleWaitResetResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleSampleWaitResetResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//样本针清洗液灌注
void CanMasterNeedleSampleNormalPrimeWhileReturn(NEEDLE_PRIME_OPT opt,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterNeedleSampleNormalPrimeWhileAck(opt,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleSampleNormalPrimeWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleSampleWaitNormalPrimeResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleSampleWaitNormalPrimeResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//样本针强力清洗液灌注
void CanMasterNeedleSamplePowerPrimeWhileReturn(SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterNeedleSamplePowerPrimeWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleSamplePowerPrimeWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleSampleWaitPowerPrimeResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleSampleWaitPowerPrimeResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//样本针清洗
void CanMasterNeedleSampleNormalCleanWhileReturn(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterNeedleSampleNormalCleanWhileAck(opt,time,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleSampleNormalCleanWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleSampleWaitNormalCleanResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleSampleWaitNormalCleanResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//样本针强力清洗
void CanMasterNeedleSamplePowerCleanWhileReturn(uint8_t count,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterNeedleSamplePowerCleanWhileAck(count,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleSamplePowerCleanWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleSampleWaitPowerCleanResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleSampleWaitPowerCleanResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//样本针清洗维护
void CanMasterNeedleSampleMaintainCleanWhileReturn(NEEDLE_MAINTAIN_OPT opt,NEEDLE_CLEAN_TIME time,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterNeedleSampleMaintainCleanWhileAck(opt,time,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleSampleMaintainCleanWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleSampleWaitMaintainCleanResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleSampleWaitMaintainCleanResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//样本针旋转指定位置
void CanMasterNeedleSampleRotateWhileReturn(NEEDLE_SAMPLE_POS targetPos,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterNeedleSampleRotateWhileAck(targetPos,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleSampleRotateWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleSampleWaitRotateResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleSampleWaitRotateResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//样本针吸试管样本
void CanMasterNeedleSampleLiquidAbsorbSampleTubeWhileReturn(uint32_t unitCount,int32_t lastLiquidDetectPos,TUBE_TYPE tubeType,
                                                            SYS_RESULT_PACK* resultPackPtr,NEEDLE_SAMPLE_ABSORB_TUBE_MSG* absorbMsgPtr)
{
    //发送指令等待ACK
    CanMasterNeedleSampleLiquidAbsorbSampleTubeWhileAck(unitCount,lastLiquidDetectPos,tubeType,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleSampleLiquidAbsorbSampleTubeWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleSampleWaitLiquidAbsorbSampleTubeResult(resultPackPtr,absorbMsgPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleSampleWaitLiquidAbsorbSampleTubeResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//样本针吸反应盘样本
void CanMasterNeedleSampleLiquidAbsorbPlateWhileReturn(uint32_t unitCount,uint32_t cupLiquidResidueAmp1000,SYS_RESULT_PACK* resultPackPtr,
                                                            NEEDLE_SAMPLE_ABSORB_PLATE_MSG* absorbMsgPtr)
{
    //发送指令等待ACK
    CanMasterNeedleSampleLiquidAbsorbPlateWhileAck(unitCount,cupLiquidResidueAmp1000,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleSampleLiquidAbsorbPlateWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleSampleWaitLiquidAbsorbPlateResult(resultPackPtr,absorbMsgPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleSampleWaitLiquidAbsorbPlateResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//样本针注液
void CanMasterNeedleSampleLiquidInjectWhileReturn(uint32_t cupLiquidUlAmp1000,uint8_t mixCount,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterNeedleSampleLiquidInjectWhileAck(cupLiquidUlAmp1000,mixCount,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleSampleLiquidInjectWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleSampleWaitLiquidInjectResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleSampleWaitLiquidInjectResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//样本针探液校准
void CanMasterNeedleSampleLiquidDetectAdjustWhileReturn(SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterNeedleSampleLiquidDetectAdjustWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleSampleLiquidDetectAdjustWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleSampleWaitLiquidDetectAdjustResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleSampleWaitLiquidDetectAdjustResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//读样本针探液板状态
void CanMasterNeedleSampleLiquidDetectReadStateWhileReturn(LIQUID_DETECT_CONFIG config,SYS_RESULT_PACK* resultPackPtr,uint16_t* needleVol,
                                                            uint16_t* thresholdValue,uint16_t* slopeValue)
{
    //发送指令等待ACK
    CanMasterNeedleSampleLiquidDetectReadStateWhileAck(config,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleSampleLiquidDetectReadStateWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleSampleWaitLiquidDetectReadStateResult(resultPackPtr,needleVol,thresholdValue,slopeValue);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleSampleWaitLiquidDetectReadStateResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//读取电阻和针尖电压
void CanMasterNeedleSampleLiquidDetectReadResVolWhileReturn(SYS_RESULT_PACK* resultPackPtr,uint16_t* resValue,uint16_t* volValue)
{
    //发送指令等待ACK
    CanMasterNeedleSampleLiquidDetectReadResVolWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleSampleLiquidDetectReadResVolWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleSampleWaitLiquidDetectReadResVolResult(resultPackPtr,resValue,volValue);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleSampleWaitLiquidDetectReadResVolResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//写入电阻值
void CanMasterNeedleSampleLiquidDetectWriteResWhileReturn(uint16_t resValue,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterNeedleSampleLiquidDetectWriteResWhileAck(resValue,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleSampleLiquidDetectWriteResWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleSampleWaitLiquidDetectWriteResResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleSampleWaitLiquidDetectWriteResResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//读取针尖基准电压
void CanMasterNeedleSampleLiquidDetectReadNeedleBaseVolWhileReturn(SYS_RESULT_PACK* resultPackPtr,uint16_t* needleBaseVol)
{
    //发送指令等待ACK
    CanMasterNeedleSampleLiquidDetectReadNeedleBaseVolWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleSampleLiquidDetectReadNeedleBaseVolWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleSampleWaitLiquidDetectReadNeedleBaseVolResult(resultPackPtr,needleBaseVol);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleSampleWaitLiquidDetectReadNeedleBaseVolResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//读样本针升降急停高度
void CanMasterNeedleSampleReadNeedleUpDownEmStopPosWhileReturn(SYS_RESULT_PACK* resultPackPtr,int32_t* needleEmStopPosPtr)
{
    //发送指令等待ACK
    CanMasterNeedleSampleReadNeedleUpDownEmStopPosWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleSampleReadNeedleUpDownEmStopPosWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleSampleWaitReadNeedleUpDownEmStopPosResult(resultPackPtr,needleEmStopPosPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleSampleWaitReadNeedleUpDownEmStopPosResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//读探液板程序版本
void CanMasterNeedleSampleReadLiquidBoardVersionWhileReturn(SYS_RESULT_PACK* resultPackPtr,uint8_t* versionStrLengthPtr,uint8_t** versionCodeBufPtrPtr)
{
    //发送指令等待ACK
    CanMasterNeedleSampleReadLiquidBoardVersionWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleSampleReadLiquidBoardVersionWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleSampleWaitReadLiquidBoardVersionResult(resultPackPtr,versionStrLengthPtr,versionCodeBufPtrPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleSampleWaitReadLiquidBoardVersionResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//释放内存
void CanMasterNeedleSampleFreeBuffer(void* bufferPtr)
{
    if(bufferPtr != NULL)
    {
        CanProtMemFree(CAN_MODULE_NO_NEEDLE_SAMPLE,CAN_BOARD_ID_NEEDLE_SAMPLE,bufferPtr);
    }
}
















