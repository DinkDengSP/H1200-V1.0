/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-24 13:00:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-28 11:55:20 +0800
************************************************************************************************/ 
#include "CanMasterLiquidAuto.h"
#include "DriverConfigSystem.h"
#include SYS_LOG_INCLUDE_FILE

//通道的模块号和板号定义
#define CAN_MODULE_NO_LIQUID_AUTO        CAN_MODULE_NO_BOARD1
#define CAN_BOARD_ID_LIQUID_AUTO         CAN_BOARD_ID_BOARD1

//通道配置数组
//参数分别是 指令  是否需要结果  超时时间  返回最小参数长度 附带参数长度
//附带参数长度为不定长的时候,指示参数长度为0,由上级调用者手动申请内存
static const CAN_PROT_CMD_CONFIG canMasterLiquidAutoCmdConfigArray[] = {
    {CAN_MASTER_CMD_H360_LIQUID_AUTO_PORT_IN                                    , ENABLE, 3000 ,  2,  1 },//模块内输入读取
    {CAN_MASTER_CMD_H360_LIQUID_AUTO_PORT_OUT                                   , ENABLE, 3000 ,  1,  2 },//模块内输出写入
    {CAN_MASTER_CMD_H360_LIQUID_AUTO_MOTOR_RESET                                , ENABLE, 60000,  5,  1 },//模块内电机复位
    {CAN_MASTER_CMD_H360_LIQUID_AUTO_MOTOR_RUN_STEPS                            , ENABLE, 60000,  5,  5 },//模块内电机走步数
    {CAN_MASTER_CMD_H360_LIQUID_AUTO_INIT                                       , ENABLE, 5000 ,  0,  0 },//初始化
    {CAN_MASTER_CMD_H360_LIQUID_AUTO_FORCE_WASTE_CLR_ONCE                       , ENABLE, 5000 ,  0,  0 },//强制排一次废液
    {CAN_MASTER_CMD_H360_LIQUID_AUTO_EN_AUTO_WASTE_CLR                          , ENABLE, 5000 ,  0,  0 },//自动排废液打开
    {CAN_MASTER_CMD_H360_LIQUID_AUTO_DIS_AUTO_WASTE_CLR                         , ENABLE, 5000 ,  0,  0 },//自动排废液关闭
    {CAN_MASTER_CMD_H360_LIQUID_AUTO_CHK_AUTO_WASTE_CLR_STA                     , ENABLE, 5000 ,  1,  0 },//查看排废液状态
    {CAN_MASTER_CMD_H360_LIQUID_AUTO_CLOSE_DILUTION                             , ENABLE, 5000 ,  0,  0 },//关闭稀释
    {CAN_MASTER_CMD_H360_LIQUID_AUTO_OPEN_DILUTION                              , ENABLE, 5000 ,  0,  1 },//打开稀释
    {CAN_MASTER_CMD_H360_LIQUID_AUTO_READ_AUTO_DILUTE_PURE_WATER_PULSE          , ENABLE, 5000 ,  4,  0 },//读取自动稀释纯水脉冲数
    {CAN_MASTER_CMD_H360_LIQUID_AUTO_RESET_AUTO_DILUTE_PURE_WATER_PULSE         , ENABLE, 5000 ,  0,  0 },//清除自动稀释纯水脉冲数
    {CAN_MASTER_CMD_H360_LIQUID_AUTO_READ_AUTO_DILUTE_PURE_WATER_TRIG_FLAG      , ENABLE, 5000 ,  1,  0 },//读取自动稀释纯水触发标记
    {CAN_MASTER_CMD_H360_LIQUID_AUTO_REGISTER_AUTO_DILUTE_PURE_WATER_PULSE      , ENABLE, 5000 ,  0,  4 },//注册自动稀释纯水脉冲数

};

/*************************************发送指令等待ACK************************************************/
//模块内输入读取
void CanMasterLiquidAutoReadSingleWhileAck(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_LIQUID_AUTO;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_LIQUID_AUTO_PORT_IN;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterLiquidAutoCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterLiquidAutoCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)modInIdx;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//模块内输出写入
void CanMasterLiquidAutoWriteSingleWhileAck(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_LIQUID_AUTO;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_LIQUID_AUTO_PORT_OUT;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterLiquidAutoCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterLiquidAutoCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,singalChannel,channelCmdCode,canCmdConfigPtr);
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
void CanMasterLiquidAutoResetMotorWhileAck(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_LIQUID_AUTO;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_LIQUID_AUTO_MOTOR_RESET;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterLiquidAutoCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterLiquidAutoCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)modMotorIdx;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//模块内电机走位
void CanMasterLiquidAutoRunStepsMotorWhileAck(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_LIQUID_AUTO;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_LIQUID_AUTO_MOTOR_RUN_STEPS;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterLiquidAutoCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterLiquidAutoCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)modMotorIdx;
    ProtUtilWriteInt32ToBuffer(steps,canCmdSendPtr->cmdBufferPtr+1);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//初始化
void CanMasterLiquidAutoInitWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_LIQUID_AUTO;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_LIQUID_AUTO_INIT;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterLiquidAutoCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterLiquidAutoCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//强制排一次废液
void CanMasterLiquidAutoForceTrigWasteLiquidWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_LIQUID_AUTO;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_LIQUID_AUTO_FORCE_WASTE_CLR_ONCE;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterLiquidAutoCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterLiquidAutoCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//自动排废液打开
void CanMasterLiquidAutoEnableWasteLiquidAutomaticWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_LIQUID_AUTO;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_LIQUID_AUTO_EN_AUTO_WASTE_CLR;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterLiquidAutoCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterLiquidAutoCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//自动排废液关闭
void CanMasterLiquidAutoDisableWasteLiquidAutomaticWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_LIQUID_AUTO;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_LIQUID_AUTO_DIS_AUTO_WASTE_CLR;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterLiquidAutoCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterLiquidAutoCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//查看排废液状态
void CanMasterLiquidAutoCheckStateWasteLiquidWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_LIQUID_AUTO;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_LIQUID_AUTO_CHK_AUTO_WASTE_CLR_STA;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterLiquidAutoCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterLiquidAutoCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//关闭稀释
void CanMasterLiquidAutoCloseLiquidDilutionThreeNeedleCleanWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_LIQUID_AUTO;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_LIQUID_AUTO_CLOSE_DILUTION;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterLiquidAutoCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterLiquidAutoCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//打开稀释
void CanMasterLiquidAutoOpenLiquidDilutionThreeNeedleCleanWhileAck(DILUTION_CH channel,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_LIQUID_AUTO;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_LIQUID_AUTO_OPEN_DILUTION;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterLiquidAutoCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterLiquidAutoCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数设置
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)channel;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//读取自动稀释配置瓶纯水脉冲数
void CanMasterLiquidAutoGetLiquidDilutionPureWaterPulseWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_LIQUID_AUTO;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_LIQUID_AUTO_READ_AUTO_DILUTE_PURE_WATER_PULSE;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterLiquidAutoCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterLiquidAutoCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数设置
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//清除自动稀释配置瓶纯水脉冲数
void CanMasterLiquidAutoResetLiquidDilutionPureWaterPulseWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_LIQUID_AUTO;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_LIQUID_AUTO_RESET_AUTO_DILUTE_PURE_WATER_PULSE;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterLiquidAutoCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterLiquidAutoCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数设置
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//读取自动稀释配置瓶纯水触发标记
void CanMasterLiquidAutoGetLiquidDilutionPureWaterTrigFlagWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_LIQUID_AUTO;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_LIQUID_AUTO_READ_AUTO_DILUTE_PURE_WATER_TRIG_FLAG;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterLiquidAutoCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterLiquidAutoCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数设置
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//注册自动稀释配置瓶纯水脉冲数
void CanMasterLiquidAutoRegisterLiquidDilutionPureWaterPulseWithAtuoOffWhileAck(uint32_t trigCount,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_LIQUID_AUTO;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_LIQUID_AUTO_REGISTER_AUTO_DILUTE_PURE_WATER_PULSE;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterLiquidAutoCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterLiquidAutoCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数设置
    ProtUtilWriteUint32ToBuffer(trigCount,canCmdSendPtr->cmdBufferPtr);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

/*************************************等待指定指令完成返回************************************************/
//模块内输入读取
void CanMasterLiquidAutoWaitReadSingleResult(SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_LIQUID_AUTO;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_LIQUID_AUTO_PORT_IN;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,returnDataBufferPtr,returnDataBufferLength);
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
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//模块内输出写入
void CanMasterLiquidAutoWaitWriteSingleResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_LIQUID_AUTO;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_LIQUID_AUTO_PORT_OUT;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//模块内电机复位
void CanMasterLiquidAutoWaitResetMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_LIQUID_AUTO;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_LIQUID_AUTO_MOTOR_RESET;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,returnDataBufferPtr,returnDataBufferLength);
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
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//模块内电机走位
void CanMasterLiquidAutoWaitRunStepsMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_LIQUID_AUTO;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_LIQUID_AUTO_MOTOR_RUN_STEPS;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,returnDataBufferPtr,returnDataBufferLength);
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
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//初始化
void CanMasterLiquidAutoWaitInitResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_LIQUID_AUTO;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_LIQUID_AUTO_INIT;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//强制排一次废液
void CanMasterLiquidAutoWaitForceTrigWasteLiquidResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_LIQUID_AUTO;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_LIQUID_AUTO_FORCE_WASTE_CLR_ONCE;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//自动排废液打开
void CanMasterLiquidAutoWaitEnableWasteLiquidAutomaticResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_LIQUID_AUTO;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_LIQUID_AUTO_EN_AUTO_WASTE_CLR;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//自动排废液关闭
void CanMasterLiquidAutoWaitDisableWasteLiquidAutomaticResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_LIQUID_AUTO;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_LIQUID_AUTO_DIS_AUTO_WASTE_CLR;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//查看排废液状态
void CanMasterLiquidAutoWaitCheckStateWasteLiquidResult(SYS_RESULT_PACK* resultPackPtr,LIQUID_AUTOMATIC_STATE* statePtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_LIQUID_AUTO;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_LIQUID_AUTO_CHK_AUTO_WASTE_CLR_STA;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(statePtr != NULL)
    {
        //结果解析
        *statePtr = LIQUID_AUTOMATIC_STATE_OFF;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *statePtr = (LIQUID_AUTOMATIC_STATE)(returnDataBufferPtr[0]);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//关闭稀释
void CanMasterLiquidAutoWaitCloseLiquidDilutionThreeNeedleCleanResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_LIQUID_AUTO;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_LIQUID_AUTO_CLOSE_DILUTION;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//打开稀释
void CanMasterLiquidAutoWaitOpenLiquidDilutionThreeNeedleCleanResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_LIQUID_AUTO;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_LIQUID_AUTO_OPEN_DILUTION;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//读取自动稀释配置瓶纯水脉冲数
void CanMasterLiquidAutoGetLiquidDilutionPureWaterPulseResult(SYS_RESULT_PACK* resultPackPtr,uint32_t* liquidCount)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_LIQUID_AUTO;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_LIQUID_AUTO_READ_AUTO_DILUTE_PURE_WATER_PULSE;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(liquidCount != NULL)
    {
        //结果解析
        *liquidCount = 0;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *liquidCount = ProtUtilReadUint32Buffer(returnDataBufferPtr);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//清除自动稀释配置瓶纯水脉冲数
void CanMasterLiquidAutoResetLiquidDilutionPureWaterPulseResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_LIQUID_AUTO;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_LIQUID_AUTO_RESET_AUTO_DILUTE_PURE_WATER_PULSE;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//读取自动稀释配置瓶纯水触发标记
void CanMasterLiquidAutoGetLiquidDilutionPureWaterTrigFlagResult(SYS_RESULT_PACK* resultPackPtr,uint8_t* trigFlag)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_LIQUID_AUTO;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_LIQUID_AUTO_READ_AUTO_DILUTE_PURE_WATER_TRIG_FLAG;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    if(trigFlag != NULL)
    {
        //结果解析
        *trigFlag = 0;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *trigFlag = returnDataBufferPtr[0];
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//注册自动稀释配置瓶纯水脉冲数
void CanMasterLiquidAutoRegisterLiquidDilutionPureWaterPulseWithAtuoOffResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_LIQUID_AUTO;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_LIQUID_AUTO_REGISTER_AUTO_DILUTE_PURE_WATER_PULSE;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_LIQUID_AUTO,CAN_BOARD_ID_LIQUID_AUTO,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

/*************************************发送指令等待完成返回************************************************/
//模块内输入读取
void CanMasterLiquidAutoReadSingleWhileReturn(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr)
{
    //发送指令等待ACK
    CanMasterLiquidAutoReadSingleWhileAck(modInIdx,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterLiquidAutoReadSingleWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterLiquidAutoWaitReadSingleResult(resultPackPtr,pinStatePtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterLiquidAutoWaitReadSingleResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//模块内输出写入
void CanMasterLiquidAutoWriteSingleWhileReturn(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterLiquidAutoWriteSingleWhileAck(modOutIdx,pinState,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterLiquidAutoWriteSingleWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterLiquidAutoWaitWriteSingleResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterLiquidAutoWaitWriteSingleResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//模块内电机复位
void CanMasterLiquidAutoResetMotorWhileReturn(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr)
{
    //发送指令等待ACK
    CanMasterLiquidAutoResetMotorWhileAck(modMotorIdx,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterLiquidAutoResetMotorWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterLiquidAutoWaitResetMotorResult(resultPackPtr,motorCurPosPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterLiquidAutoWaitResetMotorResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//模块内电机走位
void CanMasterLiquidAutoRunStepsMotorWhileReturn(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr)
{
    //发送指令等待ACK
    CanMasterLiquidAutoRunStepsMotorWhileAck(modMotorIdx,steps,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterLiquidAutoRunStepsMotorWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterLiquidAutoWaitRunStepsMotorResult(resultPackPtr,motorCurPosPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterLiquidAutoWaitRunStepsMotorResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//初始化
void CanMasterLiquidAutoInitWhileReturn(SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterLiquidAutoInitWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterLiquidAutoInitWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterLiquidAutoWaitInitResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterLiquidAutoWaitInitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//强制排一次废液
void CanMasterLiquidAutoForceTrigWasteLiquidWhileReturn(SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterLiquidAutoForceTrigWasteLiquidWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterLiquidAutoForceTrigWasteLiquidWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterLiquidAutoWaitForceTrigWasteLiquidResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterLiquidAutoWaitForceTrigWasteLiquidResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//自动排废液打开
void CanMasterLiquidAutoEnableWasteLiquidAutomaticWhileReturn(SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterLiquidAutoEnableWasteLiquidAutomaticWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterLiquidAutoEnableWasteLiquidAutomaticWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterLiquidAutoWaitEnableWasteLiquidAutomaticResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterLiquidAutoWaitEnableWasteLiquidAutomaticResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//自动排废液关闭
void CanMasterLiquidAutoDisableWasteLiquidAutomaticWhileReturn(SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterLiquidAutoDisableWasteLiquidAutomaticWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterLiquidAutoDisableWasteLiquidAutomaticWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterLiquidAutoWaitDisableWasteLiquidAutomaticResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterLiquidAutoWaitDisableWasteLiquidAutomaticResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//查看排废液状态
void CanMasterLiquidAutoCheckStateWasteLiquidWhileReturn(SYS_RESULT_PACK* resultPackPtr,LIQUID_AUTOMATIC_STATE* statePtr)
{
    //发送指令等待ACK
    CanMasterLiquidAutoCheckStateWasteLiquidWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterLiquidAutoCheckStateWasteLiquidWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterLiquidAutoWaitCheckStateWasteLiquidResult(resultPackPtr,statePtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterLiquidAutoWaitCheckStateWasteLiquidResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//关闭稀释
void CanMasterLiquidAutoCloseLiquidDilutionThreeNeedleCleanWhileReturn(SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterLiquidAutoCloseLiquidDilutionThreeNeedleCleanWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterLiquidAutoCloseLiquidDilutionThreeNeedleCleanWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterLiquidAutoWaitCloseLiquidDilutionThreeNeedleCleanResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterLiquidAutoWaitCloseLiquidDilutionThreeNeedleCleanResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//打开稀释
void CanMasterLiquidAutoOpenLiquidDilutionThreeNeedleCleanWhileReturn(DILUTION_CH channel,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterLiquidAutoOpenLiquidDilutionThreeNeedleCleanWhileAck(channel,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterLiquidAutoOpenLiquidDilutionThreeNeedleCleanWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterLiquidAutoWaitOpenLiquidDilutionThreeNeedleCleanResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterLiquidAutoWaitOpenLiquidDilutionThreeNeedleCleanResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//读取自动稀释配置瓶纯水脉冲数
void CanMasterLiquidAutoGetLiquidDilutionPureWaterPulseWhileReturn(SYS_RESULT_PACK* resultPackPtr,uint32_t* liquidCount)
{
    //发送指令等待ACK
    CanMasterLiquidAutoGetLiquidDilutionPureWaterPulseWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterLiquidAutoGetLiquidDilutionPureWaterPulseWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterLiquidAutoGetLiquidDilutionPureWaterPulseResult(resultPackPtr,liquidCount);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterLiquidAutoGetLiquidDilutionPureWaterPulseResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}
//清除自动稀释配置瓶纯水脉冲数
void CanMasterLiquidAutoResetLiquidDilutionPureWaterPulseWhileReturn(SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterLiquidAutoResetLiquidDilutionPureWaterPulseWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterLiquidAutoResetLiquidDilutionPureWaterPulseWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterLiquidAutoResetLiquidDilutionPureWaterPulseResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterLiquidAutoResetLiquidDilutionPureWaterPulseResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//读取自动稀释配置瓶纯水触发标记
void CanMasterLiquidAutoGetLiquidDilutionPureWaterTrigFlagWhileReturn(SYS_RESULT_PACK* resultPackPtr,uint8_t* trigFlag)
{
    //发送指令等待ACK
    CanMasterLiquidAutoGetLiquidDilutionPureWaterTrigFlagWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterLiquidAutoGetLiquidDilutionPureWaterTrigFlagWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterLiquidAutoGetLiquidDilutionPureWaterTrigFlagResult(resultPackPtr,trigFlag);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterLiquidAutoGetLiquidDilutionPureWaterTrigFlagResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//注册自动稀释配置瓶纯水脉冲数
void CanMasterLiquidAutoRegisterLiquidDilutionPureWaterPulseWithAtuoOffWhileReturn(uint32_t trigCount,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterLiquidAutoRegisterLiquidDilutionPureWaterPulseWithAtuoOffWhileAck(trigCount,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterLiquidAutoRegisterLiquidDilutionPureWaterPulseWithAtuoOffWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterLiquidAutoRegisterLiquidDilutionPureWaterPulseWithAtuoOffResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterLiquidAutoRegisterLiquidDilutionPureWaterPulseWithAtuoOffResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}


















