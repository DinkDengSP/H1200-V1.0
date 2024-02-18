/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-22 09:20:50 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2024-01-10 15:31:28 +0800
************************************************************************************************/ 
#include "CanMasterNeedleBead.h"
#include "DriverConfigSystem.h"
#include SYS_LOG_INCLUDE_FILE

//通道的模块号和板号定义
#define CAN_MODULE_NO_NEEDLE_BEAD        CAN_MODULE_NO_BOARD2
#define CAN_BOARD_ID_NEEDLE_BEAD         CAN_BOARD_ID_BOARD2

//通道配置数组
//参数分别是 指令  是否需要结果  超时时间  返回最小参数长度 附带参数长度
//附带参数长度为不定长的时候,指示参数长度为0,由上级调用者手动申请内存
static const CAN_PROT_CMD_CONFIG canMasterNeedleBeadCmdConfigArray[] = {
    {CAN_MASTER_CMD_H360_NEEDLE_BEAD_PORT_IN                         , ENABLE, 3000 ,  2,  1 },//模块内输入读取
    {CAN_MASTER_CMD_H360_NEEDLE_BEAD_PORT_OUT                        , ENABLE, 3000 ,  1,  2 },//模块内输出写入
    {CAN_MASTER_CMD_H360_NEEDLE_BEAD_MOTOR_RESET                     , ENABLE, 60000,  5,  1 },//模块内电机复位
    {CAN_MASTER_CMD_H360_NEEDLE_BEAD_MOTOR_RUN_STEPS                 , ENABLE, 60000,  5,  5 },//模块内电机走步数
    {CAN_MASTER_CMD_H360_NEEDLE_BEAD_RESET                           , ENABLE, 60000,  0,  0 },//磁珠针复位,带零位偏移
    {CAN_MASTER_CMD_H360_NEEDLE_BEAD_CLEAN                           , ENABLE, 60000,  0,  2 },//磁珠针清洗
    {CAN_MASTER_CMD_H360_NEEDLE_BEAD_PRIME                           , ENABLE, 60000,  0,  1 },//磁珠针灌注
    {CAN_MASTER_CMD_H360_NEEDLE_BEAD_MAINTAIN_CLEAN                  , ENABLE, 60000,  0,  2 },//磁珠针清洗维护
    {CAN_MASTER_CMD_H360_NEEDLE_BEAD_ROTATE                          , ENABLE, 60000,  0,  1 },//磁珠针旋转指定位置
    {CAN_MASTER_CMD_H360_NEEDLE_BEAD_LIQUID_ABSORB                   , ENABLE, 60000,  15, 9 },//磁珠针吸液
    {CAN_MASTER_CMD_H360_NEEDLE_BEAD_LIQUID_INJECT                   , ENABLE, 60000,  0,  5 },//磁珠针注液
    {CAN_MASTER_CMD_H360_NEEDLE_BEAD_LIQUID_DETECT_ADJUST            , ENABLE, 60000,  0,  0 },//磁珠针探液校准
    {CAN_MASTER_CMD_H360_NEEDLE_BEAD_LIQUID_DETECT_READ_STATE        , ENABLE, 60000,  6,  1 },//读磁珠针探液板状态
    {CAN_MASTER_CMD_H360_NEEDLE_BEAD_LIQUID_DETECT_READ_RES_AND_VOL  , ENABLE, 60000,  4,  0 },//磁珠针读取针尖电阻和电压
    {CAN_MASTER_CMD_H360_NEEDLE_BEAD_LIQUID_DETECT_WRITE_RES         , ENABLE, 60000,  0,  2 },//磁珠针写入探液板电阻
    {CAN_MASTER_CMD_H360_NEEDLE_BEAD_LIQUID_DETECT_READ_BASE_VOL     , ENABLE, 60000,  2,  0 },//磁珠针读取针尖基准电压
    {CAN_MASTER_CMD_H360_NEEDLE_BEAD_LIQUID_RESIDUE_CHECK            , ENABLE, 60000,  6,  0 },//磁珠针液量余量检查
    {CAN_MASTER_CMD_H360_NEEDLE_BEAD_READ_NEEDLE_UP_DOWN_EM_STOP_POS , ENABLE, 60000,  4,  0 },//磁珠针升降急停高度读取
    {CAN_MASTER_CMD_H360_NEEDLE_BEAD_LITTLE_BEAD_MIX                 , ENABLE, 60000,  0,  5 },//磁珠针小磁珠吸吐混匀
    {CAN_MASTER_CMD_H360_NEEDLE_BEAD_READ_LIQUID_BOARD_VERSION       , ENABLE, 60000,  1,  0 },//读取探液板版本号
};

/*************************************发送指令等待ACK************************************************/
//模块内输入读取
void CanMasterNeedleBeadReadSingleWhileAck(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_BEAD;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_BEAD_PORT_IN;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleBeadCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleBeadCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)modInIdx;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//模块内输出写入
void CanMasterNeedleBeadWriteSingleWhileAck(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_BEAD;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_BEAD_PORT_OUT;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleBeadCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleBeadCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,canCmdConfigPtr);
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
void CanMasterNeedleBeadResetMotorWhileAck(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_BEAD;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_BEAD_MOTOR_RESET;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleBeadCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleBeadCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)modMotorIdx;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//模块内电机走位
void CanMasterNeedleBeadRunStepsMotorWhileAck(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_BEAD;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_BEAD_MOTOR_RUN_STEPS;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleBeadCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleBeadCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)modMotorIdx;
    ProtUtilWriteInt32ToBuffer(steps,canCmdSendPtr->cmdBufferPtr+1);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//磁珠针复位,带零位偏移
void CanMasterNeedleBeadResetWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_BEAD;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_BEAD_RESET;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleBeadCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleBeadCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//磁珠针清洗
void CanMasterNeedleBeadCleanWhileAck(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_BEAD;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_BEAD_CLEAN;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleBeadCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleBeadCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)opt;
    canCmdSendPtr->cmdBufferPtr[1] = (uint8_t)time;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//磁珠针灌注
void CanMasterNeedleBeadPrimeWhileAck(NEEDLE_PRIME_OPT opt,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_BEAD;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_BEAD_PRIME;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleBeadCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleBeadCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)opt;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//磁珠针清洗维护
void CanMasterNeedleBeadMaintainCleanWhileAck(NEEDLE_MAINTAIN_OPT opt,NEEDLE_CLEAN_TIME time,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_BEAD;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_BEAD_MAINTAIN_CLEAN;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleBeadCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleBeadCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)opt;
    canCmdSendPtr->cmdBufferPtr[1] = (uint8_t)time;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//磁珠针旋转指定位置
void CanMasterNeedleBeadRotateWhileAck(NEEDLE_BEAD_POS targetPos,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_BEAD;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_BEAD_ROTATE;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleBeadCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleBeadCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)targetPos;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//磁珠针吸液
void CanMasterNeedleBeadLiquidAbsorbWhileAck(uint32_t unitCount,int32_t lastLiquidDetectPos,LIQUID_DETECT_STAGE liquidDetectStage,
                                                SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_BEAD;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_BEAD_LIQUID_ABSORB;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleBeadCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleBeadCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,canCmdConfigPtr);
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

//磁珠针注液
void CanMasterNeedleBeadLiquidInjectWhileAck(uint32_t cupLiquidUlAmp1000,uint8_t mixCount,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_BEAD;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_BEAD_LIQUID_INJECT;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleBeadCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleBeadCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    ProtUtilWriteUint32ToBuffer(cupLiquidUlAmp1000,canCmdSendPtr->cmdBufferPtr);
    canCmdSendPtr->cmdBufferPtr[4] = (uint8_t)mixCount;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//磁珠针探液校准
void CanMasterNeedleBeadLiquidDetectAdjustWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_BEAD;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_BEAD_LIQUID_DETECT_ADJUST;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleBeadCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleBeadCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//读磁珠针探液板状态
void CanMasterNeedleBeadLiquidDetectReadStateWhileAck(LIQUID_DETECT_CONFIG config,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_BEAD;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_BEAD_LIQUID_DETECT_READ_STATE;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleBeadCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleBeadCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)config;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//磁珠针读取针尖电阻和电压
void CanMasterNeedleBeadLiquidDetectReadResVolWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_BEAD;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_BEAD_LIQUID_DETECT_READ_RES_AND_VOL;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleBeadCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleBeadCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//磁珠针写入探液板电阻
void CanMasterNeedleBeadLiquidDetectWriteResWhileAck(uint16_t resValue,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_BEAD;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_BEAD_LIQUID_DETECT_WRITE_RES;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleBeadCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleBeadCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    ProtUtilWriteUint16ToBuffer(resValue,canCmdSendPtr->cmdBufferPtr);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//磁珠针读取针尖基准电压
void CanMasterNeedleBeadLiquidDetectReadNeedleBaseVolWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_BEAD;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_BEAD_LIQUID_DETECT_READ_BASE_VOL;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleBeadCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleBeadCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//磁珠针液量余量检查
void CanMasterNeedleBeadLiquidResidueCheckWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_BEAD;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_BEAD_LIQUID_RESIDUE_CHECK;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleBeadCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleBeadCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//磁珠针升降急停高度读取
void CanMasterNeedleBeadReadNeedleUpDownEmStopPosWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_BEAD;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_BEAD_READ_NEEDLE_UP_DOWN_EM_STOP_POS;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleBeadCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleBeadCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    asm("nop");
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//磁珠针小磁珠吸吐混匀
void CanMasterNeedleBeadLittleBeadPumpMixWhileAck(uint32_t mixLiquidUnit,uint8_t mixCount,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_BEAD;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_BEAD_LITTLE_BEAD_MIX;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleBeadCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleBeadCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    ProtUtilWriteUint32ToBuffer(mixLiquidUnit,canCmdSendPtr->cmdBufferPtr);
    canCmdSendPtr->cmdBufferPtr[4] = mixCount;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//读取探液板版本号
void CanMasterNeedleBeadReadLiquidBoardVersionWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_BEAD;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_BEAD_READ_LIQUID_BOARD_VERSION;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        SYS_LOG_SEND("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterNeedleBeadCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterNeedleBeadCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        SYS_LOG_SEND("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,canCmdConfigPtr);
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
void CanMasterNeedleBeadWaitReadSingleResult(SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_BEAD;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_BEAD_PORT_IN;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,returnDataBufferPtr,returnDataBufferLength);
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
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//模块内输出写入
void CanMasterNeedleBeadWaitWriteSingleResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_BEAD;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_BEAD_PORT_OUT;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//模块内电机复位
void CanMasterNeedleBeadWaitResetMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_BEAD;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_BEAD_MOTOR_RESET;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,returnDataBufferPtr,returnDataBufferLength);
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
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//模块内电机走位
void CanMasterNeedleBeadWaitRunStepsMotorResult(SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_BEAD;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_BEAD_MOTOR_RUN_STEPS;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,returnDataBufferPtr,returnDataBufferLength);
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
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//磁珠针复位,带零位偏移
void CanMasterNeedleBeadWaitResetResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_BEAD;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_BEAD_RESET;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//磁珠针清洗
void CanMasterNeedleBeadWaitCleanResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_BEAD;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_BEAD_CLEAN;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//磁珠针灌注
void CanMasterNeedleBeadWaitPrimeResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_BEAD;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_BEAD_PRIME;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//磁珠针清洗维护
void CanMasterNeedleBeadWaitMaintainCleanResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_BEAD;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_BEAD_MAINTAIN_CLEAN;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//磁珠针旋转指定位置
void CanMasterNeedleBeadWaitRotateResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_BEAD;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_BEAD_ROTATE;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//磁珠针吸液
void CanMasterNeedleBeadWaitLiquidAbsorbResult(SYS_RESULT_PACK* resultPackPtr,NEEDLE_BEAD_ABSORB_MSG* absorbMsgPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_BEAD;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_BEAD_LIQUID_ABSORB;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,returnDataBufferPtr,returnDataBufferLength);
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
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//磁珠针注液
void CanMasterNeedleBeadWaitLiquidInjectResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_BEAD;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_BEAD_LIQUID_INJECT;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//磁珠针探液校准
void CanMasterNeedleBeadWaitLiquidDetectAdjustResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_BEAD;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_BEAD_LIQUID_DETECT_ADJUST;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//读磁珠针探液板状态
void CanMasterNeedleBeadWaitLiquidDetectReadStateResult(SYS_RESULT_PACK* resultPackPtr,uint16_t* needleVol,uint16_t* thresholdValue,
                                                            uint16_t* slopeValue)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_BEAD;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_BEAD_LIQUID_DETECT_READ_STATE;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,returnDataBufferPtr,returnDataBufferLength);
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
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//磁珠针读取针尖电阻和电压
void CanMasterNeedleBeadWaitLiquidDetectReadResVolResult(SYS_RESULT_PACK* resultPackPtr,uint16_t* resValue,uint16_t* volValue)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_BEAD;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_BEAD_LIQUID_DETECT_READ_RES_AND_VOL;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,returnDataBufferPtr,returnDataBufferLength);
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
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//磁珠针写入探液板电阻
void CanMasterNeedleBeadWaitLiquidDetectWriteResResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_BEAD;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_BEAD_LIQUID_DETECT_WRITE_RES;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//磁珠针读取针尖基准电压
void CanMasterNeedleBeadWaitLiquidDetectReadNeedleBaseVolResult(SYS_RESULT_PACK* resultPackPtr,uint16_t* needleBaseVol)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_BEAD;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_BEAD_LIQUID_DETECT_READ_BASE_VOL;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,returnDataBufferPtr,returnDataBufferLength);
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
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//磁珠针液量余量检查
void CanMasterNeedleBeadWaitLiquidResidueCheckResult(SYS_RESULT_PACK* resultPackPtr,NEEDLE_BEAD_RESIDUE_MSG* residueMsgPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_BEAD;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_BEAD_LIQUID_RESIDUE_CHECK;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,returnDataBufferPtr,returnDataBufferLength);
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
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//磁珠针升降急停高度读取
void CanMasterNeedleBeadWaitReadNeedleUpDownEmStopPosResult(SYS_RESULT_PACK* resultPackPtr,int32_t* needleEmStopPosPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_BEAD;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_BEAD_READ_NEEDLE_UP_DOWN_EM_STOP_POS;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,returnDataBufferPtr,returnDataBufferLength);
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
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//磁珠针小磁珠吸吐混匀
void CanMasterNeedleBeadWaitLittleBeadPumpMixResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_BEAD;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_BEAD_LITTLE_BEAD_MIX;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    //参数解析
    asm("nop");
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//读取探液板版本号
void CanMasterNeedleBeadWaitReadLiquidBoardVersionResult(SYS_RESULT_PACK* resultPackPtr,uint8_t* versionStrLengthPtr,uint8_t** versionCodeBufPtrPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_MASTER_CH_H360_NEEDLE_BEAD;
    uint32_t channelCmdCode = (uint32_t)CAN_MASTER_CMD_H360_NEEDLE_BEAD_READ_LIQUID_BOARD_VERSION;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,returnDataBufferPtr,returnDataBufferLength);
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
            *versionCodeBufPtrPtr = (uint8_t*)CanProtMemMalloc(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,returnDataBufferLength -1);
            UserMemCopy(*versionCodeBufPtrPtr,returnDataBufferPtr+1,returnDataBufferLength -1);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

/*************************************发送指令等待完成返回************************************************/
//模块内输入读取
void CanMasterNeedleBeadReadSingleWhileReturn(uint8_t modInIdx,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* pinStatePtr)
{
    //发送指令等待ACK
    CanMasterNeedleBeadReadSingleWhileAck(modInIdx,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleBeadReadSingleWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleBeadWaitReadSingleResult(resultPackPtr,pinStatePtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleBeadWaitReadSingleResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//模块内输出写入
void CanMasterNeedleBeadWriteSingleWhileReturn(uint8_t modOutIdx,PIN_STATE pinState,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterNeedleBeadWriteSingleWhileAck(modOutIdx,pinState,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleBeadWriteSingleWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleBeadWaitWriteSingleResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleBeadWaitWriteSingleResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//模块内电机复位
void CanMasterNeedleBeadResetMotorWhileReturn(uint8_t modMotorIdx,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr)
{
    //发送指令等待ACK
    CanMasterNeedleBeadResetMotorWhileAck(modMotorIdx,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleBeadResetMotorWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleBeadWaitResetMotorResult(resultPackPtr,motorCurPosPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleBeadWaitResetMotorResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//模块内电机走位
void CanMasterNeedleBeadRunStepsMotorWhileReturn(uint8_t modMotorIdx,int32_t steps,SYS_RESULT_PACK* resultPackPtr,int32_t* motorCurPosPtr)
{
    //发送指令等待ACK
    CanMasterNeedleBeadRunStepsMotorWhileAck(modMotorIdx,steps,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleBeadRunStepsMotorWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleBeadWaitRunStepsMotorResult(resultPackPtr,motorCurPosPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleBeadWaitRunStepsMotorResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//磁珠针复位,带零位偏移
void CanMasterNeedleBeadResetWhileReturn(SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterNeedleBeadResetWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleBeadResetWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleBeadWaitResetResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleBeadWaitResetResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//磁珠针清洗
void CanMasterNeedleBeadCleanWhileReturn(NEEDLE_CLEAN_OPT opt,NEEDLE_CLEAN_TIME time,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterNeedleBeadCleanWhileAck(opt,time,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleBeadCleanWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleBeadWaitCleanResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleBeadWaitCleanResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//磁珠针灌注
void CanMasterNeedleBeadPrimeWhileReturn(NEEDLE_PRIME_OPT opt,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterNeedleBeadPrimeWhileAck(opt,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleBeadPrimeWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleBeadWaitPrimeResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleBeadWaitPrimeResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//磁珠针清洗维护
void CanMasterNeedleBeadMaintainCleanWhileReturn(NEEDLE_MAINTAIN_OPT opt,NEEDLE_CLEAN_TIME time,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterNeedleBeadMaintainCleanWhileAck(opt,time,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleBeadMaintainCleanWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleBeadWaitMaintainCleanResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleBeadWaitMaintainCleanResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//磁珠针旋转指定位置
void CanMasterNeedleBeadRotateWhileReturn(NEEDLE_BEAD_POS targetPos,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterNeedleBeadRotateWhileAck(targetPos,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleBeadRotateWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleBeadWaitRotateResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleBeadWaitRotateResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//磁珠针吸液
void CanMasterNeedleBeadLiquidAbsorbWhileReturn(uint32_t unitCount,int32_t lastLiquidDetectPos,LIQUID_DETECT_STAGE liquidDetectStage,
                                                SYS_RESULT_PACK* resultPackPtr,NEEDLE_BEAD_ABSORB_MSG* absorbMsgPtr)
{
    //发送指令等待ACK
    CanMasterNeedleBeadLiquidAbsorbWhileAck(unitCount,lastLiquidDetectPos,liquidDetectStage,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleBeadLiquidAbsorbWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleBeadWaitLiquidAbsorbResult(resultPackPtr,absorbMsgPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleBeadWaitLiquidAbsorbResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//磁珠针注液
void CanMasterNeedleBeadLiquidInjectWhileReturn(uint32_t cupLiquidUlAmp1000,uint8_t mixCount,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterNeedleBeadLiquidInjectWhileAck(cupLiquidUlAmp1000,mixCount,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleBeadLiquidInjectWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleBeadWaitLiquidInjectResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleBeadWaitLiquidInjectResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//磁珠针探液校准
void CanMasterNeedleBeadLiquidDetectAdjustWhileReturn(SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterNeedleBeadLiquidDetectAdjustWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleBeadLiquidDetectAdjustWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleBeadWaitLiquidDetectAdjustResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleBeadWaitLiquidDetectAdjustResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//读磁珠针探液板状态
void CanMasterNeedleBeadLiquidDetectReadStateWhileReturn(LIQUID_DETECT_CONFIG config,SYS_RESULT_PACK* resultPackPtr,uint16_t* needleVol,
                                                            uint16_t* thresholdValue,uint16_t* slopeValue)
{
    //发送指令等待ACK
    CanMasterNeedleBeadLiquidDetectReadStateWhileAck(config,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleBeadLiquidDetectReadStateWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleBeadWaitLiquidDetectReadStateResult(resultPackPtr,needleVol,thresholdValue,slopeValue);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleBeadWaitLiquidDetectReadStateResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//磁珠针读取针尖电阻和电压
void CanMasterNeedleBeadLiquidDetectReadResVolWhileReturn(SYS_RESULT_PACK* resultPackPtr,uint16_t* resValue,uint16_t* volValue)
{
    //发送指令等待ACK
    CanMasterNeedleBeadLiquidDetectReadResVolWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleBeadLiquidDetectReadResVolWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleBeadWaitLiquidDetectReadResVolResult(resultPackPtr,resValue,volValue);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleBeadWaitLiquidDetectReadResVolResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//磁珠针写入探液板电阻
void CanMasterNeedleBeadLiquidDetectWriteResWhileReturn(uint16_t resValue,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterNeedleBeadLiquidDetectWriteResWhileAck(resValue,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleBeadLiquidDetectWriteResWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleBeadWaitLiquidDetectWriteResResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleBeadWaitLiquidDetectWriteResResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//磁珠针读取针尖基准电压
void CanMasterNeedleBeadLiquidDetectReadNeedleBaseVolWhileReturn(SYS_RESULT_PACK* resultPackPtr,uint16_t* needleBaseVol)
{
    //发送指令等待ACK
    CanMasterNeedleBeadLiquidDetectReadNeedleBaseVolWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleBeadLiquidDetectReadNeedleBaseVolWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleBeadWaitLiquidDetectReadNeedleBaseVolResult(resultPackPtr,needleBaseVol);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleBeadWaitLiquidDetectReadNeedleBaseVolResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//磁珠针液量余量检查
void CanMasterNeedleBeadLiquidResidueCheckWhileReturn(SYS_RESULT_PACK* resultPackPtr,NEEDLE_BEAD_RESIDUE_MSG* residueMsgPtr)
{
    //发送指令等待ACK
    CanMasterNeedleBeadLiquidResidueCheckWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleBeadLiquidResidueCheckWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleBeadWaitLiquidResidueCheckResult(resultPackPtr,residueMsgPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleBeadWaitLiquidResidueCheckResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//磁珠针升降急停高度读取
void CanMasterNeedleBeadReadNeedleUpDownEmStopPosWhileReturn(SYS_RESULT_PACK* resultPackPtr,int32_t* needleEmStopPosPtr)
{
    //发送指令等待ACK
    CanMasterNeedleBeadReadNeedleUpDownEmStopPosWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleBeadReadNeedleUpDownEmStopPosWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleBeadWaitReadNeedleUpDownEmStopPosResult(resultPackPtr,needleEmStopPosPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleBeadWaitReadNeedleUpDownEmStopPosResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//磁珠针小磁珠吸吐混匀
void CanMasterNeedleBeadLittleBeadPumpMixWhileReturn(uint32_t mixLiquidUnit,uint8_t mixCount,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterNeedleBeadLittleBeadPumpMixWhileAck(mixLiquidUnit,mixCount,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleBeadLittleBeadPumpMixWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleBeadWaitLittleBeadPumpMixResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleBeadWaitLittleBeadPumpMixResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//读取探液板版本号
void CanMasterNeedleBeadReadLiquidBoardVersionWhileReturn(SYS_RESULT_PACK* resultPackPtr,uint8_t* versionStrLengthPtr,uint8_t** versionCodeBufPtrPtr)
{
    //发送指令等待ACK
    CanMasterNeedleBeadReadLiquidBoardVersionWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleBeadReadLiquidBoardVersionWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterNeedleBeadWaitReadLiquidBoardVersionResult(resultPackPtr,versionStrLengthPtr,versionCodeBufPtrPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        SYS_LOG_SEND("%s,%d,CanMasterNeedleBeadWaitReadLiquidBoardVersionResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//释放内存
void CanMasterNeedleBeadFreeBuffer(void* bufferPtr)
{
    if(bufferPtr != NULL)
    {
        CanProtMemFree(CAN_MODULE_NO_NEEDLE_BEAD,CAN_BOARD_ID_NEEDLE_BEAD,bufferPtr);
    }
}











