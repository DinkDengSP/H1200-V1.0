/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-24 13:00:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-07 12:18:41 +0800
************************************************************************************************/ 
#include "CanMasterNeedleReagent.h"
#include "DriverConfigSystem.h"
#include SYS_LOG_INCLUDE_FILE

//通道的模块号和板号定义
#define CAN_MODULE_NO_NEEDLE_REAGENT        CAN_MODULE_NO_BOARD4
#define CAN_BOARD_ID_NEEDLE_REAGENT         CAN_BOARD_ID_BOARD4

//通道配置数组
//参数分别是 指令  是否需要结果  超时时间  返回最小参数长度 附带参数长度
//附带参数长度为不定长的时候,指示参数长度为0,由上级调用者手动申请内存
static const CAN_PROT_CMD_CONFIG canMasterNeedleReagentCmdConfigArray[] = {
    {CAN_MASTER_CMD_H360_NEEDLE_REAGENT_PORT_IN                         , ENABLE, 3000 ,  2,  1 },//模块内输入读取
    {CAN_MASTER_CMD_H360_NEEDLE_REAGENT_PORT_OUT                        , ENABLE, 3000 ,  1,  2 },//模块内输出写入
    {CAN_MASTER_CMD_H360_NEEDLE_REAGENT_MOTOR_RESET                     , ENABLE, 60000,  5,  1 },//模块内电机复位
    {CAN_MASTER_CMD_H360_NEEDLE_REAGENT_MOTOR_RUN_STEPS                 , ENABLE, 60000,  5,  5 },//模块内电机走步数
    {CAN_MASTER_CMD_H360_NEEDLE_REAGENT_RESET                           , ENABLE, 60000,  0,  0 },//试剂针复位,带零位偏移
    {CAN_MASTER_CMD_H360_NEEDLE_REAGENT_CLEAN                           , ENABLE, 60000,  0,  2 },//试剂针清洗
    {CAN_MASTER_CMD_H360_NEEDLE_REAGENT_PRIME                           , ENABLE, 60000,  0,  1 },//试剂针灌注
    {CAN_MASTER_CMD_H360_NEEDLE_REAGENT_MAINTAIN_CLEAN                  , ENABLE, 60000,  0,  2 },//试剂针清洗维护
    {CAN_MASTER_CMD_H360_NEEDLE_REAGENT_ROTATE                          , ENABLE, 60000,  0,  1 },//试剂针旋转指定位置
    {CAN_MASTER_CMD_H360_NEEDLE_REAGENT_LIQUID_ABSORB                   , ENABLE, 60000,  15, 9 },//试剂针吸液
    {CAN_MASTER_CMD_H360_NEEDLE_REAGENT_LIQUID_INJECT                   , ENABLE, 60000,  0,  5 },//试剂针注液
    {CAN_MASTER_CMD_H360_NEEDLE_REAGENT_LIQUID_DETECT_ADJUST            , ENABLE, 60000,  0,  0 },//试剂针探液校准
    {CAN_MASTER_CMD_H360_NEEDLE_REAGENT_LIQUID_DETECT_READ_STATE        , ENABLE, 60000,  6,  1 },//读试剂针探液板状态
    {CAN_MASTER_CMD_H360_NEEDLE_REAGENT_LIQUID_DETECT_READ_RES_AND_VOL  , ENABLE, 60000,  4,  0 },//试剂针读取针尖电阻和电压
    {CAN_MASTER_CMD_H360_NEEDLE_REAGENT_LIQUID_DETECT_WRITE_RES         , ENABLE, 60000,  0,  2 },//试剂针写入探液板电阻
    {CAN_MASTER_CMD_H360_NEEDLE_REAGENT_LIQUID_DETECT_READ_BASE_VOL     , ENABLE, 60000,  2,  0 },//试剂针读取针尖基准电压
    {CAN_MASTER_CMD_H360_NEEDLE_REAGENT_LIQUID_RESIDUE_CHECK            , ENABLE, 60000,  6,  0 },//试剂针液量余量检查
    {CAN_MASTER_CMD_H360_NEEDLE_REAGENT_READ_NEEDLE_UP_DOWN_EM_STOP_POS , ENABLE, 60000,  4,  0 },//试剂针升降急停高度读取
    {CAN_MASTER_CMD_H360_NEEDLE_REAGENT_READ_LIQUID_BOARD_VERSION       , ENABLE, 60000,  1,  0 },//读取探液板版本号
};

/*************************************发送指令等待ACK************************************************/
//模块内输入读取
void CanMasterNeedleReagentReadSingleWhileAck(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_REAGENT;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_REAGENT_PORT_IN;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleReagentCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleReagentCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)modInIdx;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//模块内输出写入
void CanMasterNeedleReagentWriteSingleWhileAck(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_REAGENT;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_REAGENT_PORT_OUT;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleReagentCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleReagentCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,canCmdConfigPtr);
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
void CanMasterNeedleReagentResetMotorWhileAck(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_REAGENT;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_REAGENT_MOTOR_RESET;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleReagentCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleReagentCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)modMotorIdx;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//模块内电机走位
void CanMasterNeedleReagentRunStepsMotorWhileAck(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_REAGENT;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_REAGENT_MOTOR_RUN_STEPS;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleReagentCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleReagentCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)modMotorIdx;
    ProtUtilWriteInt32ToBuffer(steps,canCmdSendPtr->cmdBufferPtr+1);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//试剂针复位,带零位偏移
void CanMasterNeedleReagentResetWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_REAGENT;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_REAGENT_RESET;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleReagentCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleReagentCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//试剂针清洗
void CanMasterNeedleReagentCleanWhileAck(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_REAGENT;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_REAGENT_CLEAN;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleReagentCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleReagentCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)opt;
    canCmdSendPtr->cmdBufferPtr[1] = (uint8_t)time;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//试剂针灌注
void CanMasterNeedleReagentPrimeWhileAck(NEEDLE_PRIME_OPT opt,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_REAGENT;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_REAGENT_PRIME;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleReagentCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleReagentCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)opt;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//试剂针清洗维护
void CanMasterNeedleReagentMaintainCleanWhileAck(NEEDLE_MAINTAIN_OPT opt,NEEDLE_CLEAN_TIME time,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_REAGENT;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_REAGENT_MAINTAIN_CLEAN;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleReagentCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleReagentCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)opt;
    canCmdSendPtr->cmdBufferPtr[1] = (uint8_t)time;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//试剂针旋转指定位置
void CanMasterNeedleReagentRotateWhileAck(NEEDLE_REAGENT_POS targetPos,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_REAGENT;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_REAGENT_ROTATE;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleReagentCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleReagentCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)targetPos;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//试剂针吸液
void CanMasterNeedleReagentLiquidAbsorbWhileAck(uint32_t unitCount,int32_t lastLiquidDetectPos,LIQUID_DETECT_STAGE liquidDetectStage,
                                                SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_REAGENT;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_REAGENT_LIQUID_ABSORB;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleReagentCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleReagentCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    ProtUtilWriteUint32ToBuffer(unitCount,canCmdSendPtr->cmdBufferPtr);
    ProtUtilWriteInt32ToBuffer(lastLiquidDetectPos,canCmdSendPtr->cmdBufferPtr+4);
    canCmdSendPtr->cmdBufferPtr[8] = (uint8_t)liquidDetectStage;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//试剂针注液
void CanMasterNeedleReagentLiquidInjectWhileAck(uint32_t cupLiquidUlAmp1000,uint8_t mixCount,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_REAGENT;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_REAGENT_LIQUID_INJECT;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleReagentCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleReagentCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    ProtUtilWriteUint32ToBuffer(cupLiquidUlAmp1000,canCmdSendPtr->cmdBufferPtr);
    canCmdSendPtr->cmdBufferPtr[4] = (uint8_t)mixCount;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//试剂针探液校准
void CanMasterNeedleReagentLiquidDetectAdjustWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_REAGENT;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_REAGENT_LIQUID_DETECT_ADJUST;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleReagentCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleReagentCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//读试剂针探液板状态
void CanMasterNeedleReagentLiquidDetectReadStateWhileAck(LIQUID_DETECT_CONFIG config,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_REAGENT;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_REAGENT_LIQUID_DETECT_READ_STATE;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleReagentCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleReagentCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)config;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//试剂针读取针尖电阻和电压
void CanMasterNeedleReagentLiquidDetectReadResVolWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_REAGENT;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_REAGENT_LIQUID_DETECT_READ_RES_AND_VOL;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleReagentCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleReagentCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//试剂针写入探液板电阻
void CanMasterNeedleReagentLiquidDetectWriteResWhileAck(uint16_t resValue,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_REAGENT;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_REAGENT_LIQUID_DETECT_WRITE_RES;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleReagentCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleReagentCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    ProtUtilWriteUint16ToBuffer(resValue,canCmdSendPtr->cmdBufferPtr);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//试剂针读取针尖基准电压
void CanMasterNeedleReagentLiquidDetectReadNeedleBaseVolWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_REAGENT;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_REAGENT_LIQUID_DETECT_READ_BASE_VOL;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleReagentCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleReagentCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//试剂针液量余量检查
void CanMasterNeedleReagentLiquidResidueCheckWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_REAGENT;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_REAGENT_LIQUID_RESIDUE_CHECK;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleReagentCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleReagentCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//试剂针升降急停高度读取
void CanMasterNeedleReagentReadNeedleUpDownEmStopPosWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_REAGENT;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_REAGENT_READ_NEEDLE_UP_DOWN_EM_STOP_POS;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleReagentCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleReagentCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//读取探液板版本号
void CanMasterNeedleReagentReadLiquidBoardVersionWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_REAGENT;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_REAGENT_READ_LIQUID_BOARD_VERSION;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleReagentCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleReagentCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,canCmdConfigPtr);
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
void CanMasterNeedleReagentWaitReadSingleResult(SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_REAGENT;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_REAGENT_PORT_IN;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,returnDataBufferPtr,returnDataBufferLength);
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
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//模块内输出写入
void CanMasterNeedleReagentWaitWriteSingleResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_REAGENT;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_REAGENT_PORT_OUT;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//模块内电机复位
void CanMasterNeedleReagentWaitResetMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_REAGENT;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_REAGENT_MOTOR_RESET;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,returnDataBufferPtr,returnDataBufferLength);
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
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//模块内电机走位
void CanMasterNeedleReagentWaitRunStepsMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_REAGENT;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_REAGENT_MOTOR_RUN_STEPS;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,returnDataBufferPtr,returnDataBufferLength);
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
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//试剂针复位,带零位偏移
void CanMasterNeedleReagentWaitResetResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_REAGENT;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_REAGENT_RESET;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//试剂针清洗
void CanMasterNeedleReagentWaitCleanResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_REAGENT;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_REAGENT_CLEAN;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//试剂针灌注
void CanMasterNeedleReagentWaitPrimeResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_REAGENT;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_REAGENT_PRIME;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//试剂针清洗维护
void CanMasterNeedleReagentWaitMaintainCleanResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_REAGENT;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_REAGENT_MAINTAIN_CLEAN;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//试剂针旋转指定位置
void CanMasterNeedleReagentWaitRotateResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_REAGENT;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_REAGENT_ROTATE;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//试剂针吸液
void CanMasterNeedleReagentWaitLiquidAbsorbResult(SYS_RESULT_PACK* resultPackPtr,NEEDLE_REAGENT_ABSORB_MSG* absorbMsgPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_REAGENT;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_REAGENT_LIQUID_ABSORB;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,returnDataBufferPtr,returnDataBufferLength);
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
            absorbMsgPtr->liquidDetectStageReturn = (LIQUID_DETECT_STAGE)returnDataBufferPtr[10];
            absorbMsgPtr->liquidDetectBaseVol = ProtUtilReadUint16Buffer(returnDataBufferPtr+11);
            absorbMsgPtr->liquidDetectNeedleVol = ProtUtilReadUint16Buffer(returnDataBufferPtr+13);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//试剂针注液
void CanMasterNeedleReagentWaitLiquidInjectResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_REAGENT;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_REAGENT_LIQUID_INJECT;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//试剂针探液校准
void CanMasterNeedleReagentWaitLiquidDetectAdjustResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_REAGENT;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_REAGENT_LIQUID_DETECT_ADJUST;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//读试剂针探液板状态
void CanMasterNeedleReagentWaitLiquidDetectReadStateResult(SYS_RESULT_PACK* resultPackPtr,uint16_t* needleVol,uint16_t* thresholdValue,
                                                            uint16_t* slopeValue)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_REAGENT;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_REAGENT_LIQUID_DETECT_READ_STATE;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,returnDataBufferPtr,returnDataBufferLength);
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
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//试剂针读取针尖电阻和电压
void CanMasterNeedleReagentWaitLiquidDetectReadResVolResult(SYS_RESULT_PACK* resultPackPtr,uint16_t* resValue,uint16_t* volValue)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_REAGENT;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_REAGENT_LIQUID_DETECT_READ_RES_AND_VOL;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,returnDataBufferPtr,returnDataBufferLength);
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
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//试剂针写入探液板电阻
void CanMasterNeedleReagentWaitLiquidDetectWriteResResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_REAGENT;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_REAGENT_LIQUID_DETECT_WRITE_RES;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//试剂针读取针尖基准电压
void CanMasterNeedleReagentWaitLiquidDetectReadNeedleBaseVolResult(SYS_RESULT_PACK* resultPackPtr,uint16_t* needleBaseVol)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_REAGENT;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_REAGENT_LIQUID_DETECT_READ_BASE_VOL;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,returnDataBufferPtr,returnDataBufferLength);
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
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//试剂针液量余量检查
void CanMasterNeedleReagentWaitLiquidResidueCheckResult(SYS_RESULT_PACK* resultPackPtr,NEEDLE_REAGENT_RESIDUE_MSG* residueMsgPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_REAGENT;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_REAGENT_LIQUID_RESIDUE_CHECK;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(residueMsgPtr != NULL)
    {
        //结果解析
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            residueMsgPtr->currentLiquidDetectPos = ProtUtilReadInt32Buffer(returnDataBufferPtr);
            residueMsgPtr->liquidResultRerurn = (LIQUID_FUNC_RESULT)(returnDataBufferPtr[4]);
            residueMsgPtr->needleLiquidDetectResult = (LIQUID_NEEDLE_LIQUID_DETECT_RESULT)(returnDataBufferPtr[5]);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//试剂针升降急停高度读取
void CanMasterNeedleReagentWaitReadNeedleUpDownEmStopPosResult(SYS_RESULT_PACK* resultPackPtr,int32_t* needleEmStopPosPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_REAGENT;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_REAGENT_READ_NEEDLE_UP_DOWN_EM_STOP_POS;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,returnDataBufferPtr,returnDataBufferLength);
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
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//读取探液板版本号
void CanMasterNeedleReagentWaitReadLiquidBoardVersionResult(SYS_RESULT_PACK* resultPackPtr,uint8_t* versionStrLengthPtr,uint8_t** versionCodeBufPtrPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_REAGENT;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_REAGENT_READ_LIQUID_BOARD_VERSION;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,returnDataBufferPtr,returnDataBufferLength);
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
            *versionCodeBufPtrPtr = (uint8_t*)CanProtMemMalloc(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,returnDataBufferLength -1);
            UserMemCopy(*versionCodeBufPtrPtr,returnDataBufferPtr+1,returnDataBufferLength -1);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

/*************************************发送指令等待完成返回************************************************/
//模块内输入读取
void CanMasterNeedleReagentReadSingleWhileReturn(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr)
{
    //发送指令等待ACK
    CanMasterNeedleReagentReadSingleWhileAck(modInIdx,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleReagentReadSingleWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleReagentWaitReadSingleResult(resultPackPtr,pinStatePtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleReagentWaitReadSingleResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//模块内输出写入
void CanMasterNeedleReagentWriteSingleWhileReturn(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterNeedleReagentWriteSingleWhileAck(modOutIdx,pinState,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleReagentWriteSingleWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleReagentWaitWriteSingleResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleReagentWaitWriteSingleResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//模块内电机复位
void CanMasterNeedleReagentResetMotorWhileReturn(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr)
{
    //发送指令等待ACK
    CanMasterNeedleReagentResetMotorWhileAck(modMotorIdx,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleReagentResetMotorWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleReagentWaitResetMotorResult(resultPackPtr,motorCurPosPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleReagentWaitResetMotorResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//模块内电机走位
void CanMasterNeedleReagentRunStepsMotorWhileReturn(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr)
{
    //发送指令等待ACK
    CanMasterNeedleReagentRunStepsMotorWhileAck(modMotorIdx,steps,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleReagentRunStepsMotorWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleReagentWaitRunStepsMotorResult(resultPackPtr,motorCurPosPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleReagentWaitRunStepsMotorResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}


//试剂针复位,带零位偏移
void CanMasterNeedleReagentResetWhileReturn(SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterNeedleReagentResetWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleReagentResetWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleReagentWaitResetResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleReagentWaitResetResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//试剂针清洗
void CanMasterNeedleReagentCleanWhileReturn(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterNeedleReagentCleanWhileAck(opt,time,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleReagentCleanWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleReagentWaitCleanResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleReagentWaitCleanResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//试剂针灌注
void CanMasterNeedleReagentPrimeWhileReturn(NEEDLE_PRIME_OPT opt,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterNeedleReagentPrimeWhileAck(opt,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleReagentPrimeWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleReagentWaitPrimeResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleReagentWaitPrimeResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//试剂针清洗维护
void CanMasterNeedleReagentMaintainCleanWhileReturn(NEEDLE_MAINTAIN_OPT opt,NEEDLE_CLEAN_TIME time,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterNeedleReagentMaintainCleanWhileAck(opt,time,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleReagentMaintainCleanWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleReagentWaitMaintainCleanResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleReagentWaitMaintainCleanResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//试剂针旋转指定位置
void CanMasterNeedleReagentRotateWhileReturn(NEEDLE_REAGENT_POS targetPos,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterNeedleReagentRotateWhileAck(targetPos,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleReagentRotateWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleReagentWaitRotateResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleReagentWaitRotateResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//试剂针吸液
void CanMasterNeedleReagentLiquidAbsorbWhileReturn(uint32_t unitCount,int32_t lastLiquidDetectPos,LIQUID_DETECT_STAGE liquidDetectStage,
                                                SYS_RESULT_PACK* resultPackPtr,NEEDLE_REAGENT_ABSORB_MSG* absorbMsgPtr)
{
    //发送指令等待ACK
    CanMasterNeedleReagentLiquidAbsorbWhileAck(unitCount,lastLiquidDetectPos,liquidDetectStage,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleReagentLiquidAbsorbWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleReagentWaitLiquidAbsorbResult(resultPackPtr,absorbMsgPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleReagentWaitLiquidAbsorbResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//试剂针注液
void CanMasterNeedleReagentLiquidInjectWhileReturn(uint32_t cupLiquidUlAmp1000,uint8_t mixCount,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterNeedleReagentLiquidInjectWhileAck(cupLiquidUlAmp1000,mixCount,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleReagentLiquidInjectWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleReagentWaitLiquidInjectResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleReagentWaitLiquidInjectResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//试剂针探液校准
void CanMasterNeedleReagentLiquidDetectAdjustWhileReturn(SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterNeedleReagentLiquidDetectAdjustWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleReagentLiquidDetectAdjustWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleReagentWaitLiquidDetectAdjustResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleReagentWaitLiquidDetectAdjustResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//读试剂针探液板状态
void CanMasterNeedleReagentLiquidDetectReadStateWhileReturn(LIQUID_DETECT_CONFIG config,SYS_RESULT_PACK* resultPackPtr,uint16_t* needleVol,
                                                            uint16_t* thresholdValue,uint16_t* slopeValue)
{
    //发送指令等待ACK
    CanMasterNeedleReagentLiquidDetectReadStateWhileAck(config,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleReagentLiquidDetectReadStateWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleReagentWaitLiquidDetectReadStateResult(resultPackPtr,needleVol,thresholdValue,slopeValue);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleReagentWaitLiquidDetectReadStateResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//试剂针读取针尖电阻和电压
void CanMasterNeedleReagentLiquidDetectReadResVolWhileReturn(SYS_RESULT_PACK* resultPackPtr,uint16_t* resValue,uint16_t* volValue)
{
    //发送指令等待ACK
    CanMasterNeedleReagentLiquidDetectReadResVolWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleReagentLiquidDetectReadResVolWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleReagentWaitLiquidDetectReadResVolResult(resultPackPtr,resValue,volValue);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleReagentWaitLiquidDetectReadResVolResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//试剂针写入探液板电阻
void CanMasterNeedleReagentLiquidDetectWriteResWhileReturn(uint16_t resValue,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterNeedleReagentLiquidDetectWriteResWhileAck(resValue,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleReagentLiquidDetectWriteResWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleReagentWaitLiquidDetectWriteResResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleReagentWaitLiquidDetectWriteResResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//试剂针读取针尖基准电压
void CanMasterNeedleReagentLiquidDetectReadNeedleBaseVolWhileReturn(SYS_RESULT_PACK* resultPackPtr,uint16_t* needleBaseVol)
{
    //发送指令等待ACK
    CanMasterNeedleReagentLiquidDetectReadNeedleBaseVolWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleReagentLiquidDetectReadNeedleBaseVolWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleReagentWaitLiquidDetectReadNeedleBaseVolResult(resultPackPtr,needleBaseVol);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleReagentWaitLiquidDetectReadNeedleBaseVolResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//试剂针液量余量检查
void CanMasterNeedleReagentLiquidResidueCheckWhileReturn(SYS_RESULT_PACK* resultPackPtr,NEEDLE_REAGENT_RESIDUE_MSG* residueMsgPtr)
{
    //发送指令等待ACK
    CanMasterNeedleReagentLiquidResidueCheckWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleReagentLiquidResidueCheckWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleReagentWaitLiquidResidueCheckResult(resultPackPtr,residueMsgPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleReagentWaitLiquidResidueCheckResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//试剂针升降急停高度读取
void CanMasterNeedleReagentReadNeedleUpDownEmStopPosWhileReturn(SYS_RESULT_PACK* resultPackPtr,int32_t* needleEmStopPosPtr)
{
    //发送指令等待ACK
    CanMasterNeedleReagentReadNeedleUpDownEmStopPosWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleReagentReadNeedleUpDownEmStopPosWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleReagentWaitReadNeedleUpDownEmStopPosResult(resultPackPtr,needleEmStopPosPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleReagentWaitReadNeedleUpDownEmStopPosResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//读取探液板版本号
void CanMasterNeedleReagentReadLiquidBoardVersionWhileReturn(SYS_RESULT_PACK* resultPackPtr,uint8_t* versionStrLengthPtr,uint8_t** versionCodeBufPtrPtr)
{
    //发送指令等待ACK
    CanMasterNeedleReagentReadLiquidBoardVersionWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleReagentReadLiquidBoardVersionWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleReagentWaitReadLiquidBoardVersionResult(resultPackPtr,versionStrLengthPtr,versionCodeBufPtrPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleReagentWaitReadLiquidBoardVersionResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//释放内存
void CanMasterNeedleReagentFreeBuffer(void* bufferPtr)
{
    if(bufferPtr != NULL)
    {
        CanProtMemFree(CAN_MODULE_NO_NEEDLE_REAGENT,CAN_BOARD_ID_NEEDLE_REAGENT,bufferPtr);
    }
}

















