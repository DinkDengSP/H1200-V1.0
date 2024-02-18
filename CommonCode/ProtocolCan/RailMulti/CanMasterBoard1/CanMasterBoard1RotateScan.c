/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-24 10:44:38 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-24 13:52:03 +0800
************************************************************************************************/ 
#include "CanMasterBoard1RotateScan.h"
#include "DriverConfigSystem.h"
#include SYS_LOG_INCLUDE_FILE

//通道配置数组
//参数分别是 指令  是否需要结果  超时时间  最小参数长度 附带参数长度
//附带参数长度为不定长的时候,指示参数长度为0,由上级调用者手动申请内存
static const CAN_PROT_CMD_CONFIG canMasterBoard1RotateScanCmdConfigArray[] = {
    {CAN_RAIL_MUTIL_CMD_BOARD1_ROTATE_SCAN_READ_INPUT                             ,        ENABLE,     60000,      2,      1       },//旋转扫码 输入读取
    {CAN_RAIL_MUTIL_CMD_BOARD1_ROTATE_SCAN_WRITE_OUTPUT                           ,        ENABLE,     60000,      1,      2       },//旋转扫码 输出写出
    {CAN_RAIL_MUTIL_CMD_BOARD1_ROTATE_SCAN_MOTOR_RESET                            ,        ENABLE,     60000,      5,      1       },//旋转扫码 电机复位
    {CAN_RAIL_MUTIL_CMD_BOARD1_ROTATE_SCAN_MOTOR_RUN_STEP                         ,        ENABLE,     60000,      5,      5       },//旋转扫码 电机走步数
    {CAN_RAIL_MUTIL_CMD_BOARD1_ROTATE_SCAN_RESET_ALL                              ,        ENABLE,     60000,      0,      0       },//旋转扫码 复位
    {CAN_RAIL_MUTIL_CMD_BOARD1_ROTATE_SCAN_PREPARE_RECV_RACK                      ,        ENABLE,     60000,      0,      1       },//旋转扫码 准备接收试管架
    {CAN_RAIL_MUTIL_CMD_BOARD1_ROTATE_SCAN_RACK_SCAN_ROTATE_TARGET_DIR            ,        ENABLE,     60000,      4,      2       },//旋转扫码 扫码并指定标签方向
    {CAN_RAIL_MUTIL_CMD_BOARD1_ROTATE_SCAN_RACK_ROTATE_AND_SCAN                   ,        ENABLE,     60000,      4,      2       },//旋转扫码 样本架旋转并扫码
};

/*************************************发送指令等待ACK************************************************/
//旋转扫码 输入读取
void CanMasterBoard1RotateScanHandReadInputWhileAck(uint8_t index,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD1_ROTATE_SCAN;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD1_ROTATE_SCAN_READ_INPUT;
    uint8_t targetModule = CAN_MODULE_NO_BOARD1;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD1;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard1RotateScanCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard1RotateScanCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(targetModule,targetBoardID,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)index;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//旋转扫码 输出写出
void CanMasterBoard1RotateScanHandWriteOutputWhileAck(uint8_t index,PIN_STATE outState,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD1_ROTATE_SCAN;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD1_ROTATE_SCAN_WRITE_OUTPUT;
    uint8_t targetModule = CAN_MODULE_NO_BOARD1;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD1;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard1RotateScanCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard1RotateScanCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(targetModule,targetBoardID,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)index;
    canCmdSendPtr->cmdBufferPtr[1] = (uint8_t)outState;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}


//旋转扫码 指定步进电机复位
void CanMasterBoard1RotateScanStepMotorResetWhileAck(uint8_t motorIndex,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD1_ROTATE_SCAN;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD1_ROTATE_SCAN_MOTOR_RESET;
    uint8_t targetModule = CAN_MODULE_NO_BOARD1;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD1;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard1RotateScanCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard1RotateScanCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(targetModule,targetBoardID,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)motorIndex;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//旋转扫码 电机走步数
void CanMasterBoard1RotateScanStepMotorRunStepsWhileAck(uint8_t motorIndex,int32_t steps,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD1_ROTATE_SCAN;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD1_ROTATE_SCAN_MOTOR_RUN_STEP;
    uint8_t targetModule = CAN_MODULE_NO_BOARD1;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD1;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard1RotateScanCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard1RotateScanCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(targetModule,targetBoardID,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)motorIndex;
    ProtUtilWriteInt32ToBuffer(steps,canCmdSendPtr->cmdBufferPtr+1);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//旋转扫码 整体复位
void CanMasterBoard1RotateScanResetAllWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD1_ROTATE_SCAN;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD1_ROTATE_SCAN_RESET_ALL;
    uint8_t targetModule = CAN_MODULE_NO_BOARD1;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD1;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard1RotateScanCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard1RotateScanCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(targetModule,targetBoardID,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    // canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)motorIndex;
    // ProtUtilWriteInt32ToBuffer(steps,canCmdSendPtr->cmdBufferPtr+1);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//旋转扫码 准备接收试管架
void CanMasterBoard1RotateScanPrepareRecvRackWhileAck(RACK_BAR_DIR rackBarDir,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD1_ROTATE_SCAN;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD1_ROTATE_SCAN_PREPARE_RECV_RACK;
    uint8_t targetModule = CAN_MODULE_NO_BOARD1;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD1;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard1RotateScanCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard1RotateScanCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(targetModule,targetBoardID,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)rackBarDir;
    // ProtUtilWriteInt32ToBuffer(steps,canCmdSendPtr->cmdBufferPtr+1);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//旋转扫码 样本架扫码并指定标签方向
void CanMasterBoard1RotateScanRackScanRotateDirWhileAck(uint8_t scanFlag,RACK_BAR_DIR rackBarDir,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD1_ROTATE_SCAN;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD1_ROTATE_SCAN_RACK_SCAN_ROTATE_TARGET_DIR;
    uint8_t targetModule = CAN_MODULE_NO_BOARD1;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD1;
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard1RotateScanCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasterBoard1RotateScanCmdConfigArray));
    if(canCmdConfigPtr == NULL)
    {
        CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
        //设置错误码
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
        return;
    }
    //创建指令头
    canCmdSendPtr = CanProtCreateCmdSend(targetModule,targetBoardID,singalChannel,channelCmdCode,canCmdConfigPtr);
    //参数赋值
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)scanFlag;
    canCmdSendPtr->cmdBufferPtr[1] = (uint8_t)rackBarDir;
    // ProtUtilWriteInt32ToBuffer(steps,canCmdSendPtr->cmdBufferPtr+1);
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}


// //旋转扫码 样本架旋转并扫码
// void CanMasterBoard1RotateScanRackRotateAndScanWhileAck(uint8_t rotateFlag,uint8_t scanFlag,SYS_RESULT_PACK* resultPackPtr)
// {
//     uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD1_ROTATE_SCAN;
//     uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD1_ROTATE_SCAN_RACK_ROTATE_AND_SCAN;
//     uint8_t targetModule = CAN_MODULE_NO_BOARD1;
//     uint8_t targetBoardID = CAN_BOARD_ID_BOARD1;
//     CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
//     CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
//     if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
//     {
//         CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
//         return;
//     }
//     CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasterBoard1RotateScanCmdConfigArray,
//                                                                 DIM_ARRAY_ELEMENT_COUNT(canMasterBoard1RotateScanCmdConfigArray));
//     if(canCmdConfigPtr == NULL)
//     {
//         CAN_PROT_LOG("%s,%d,canCmdConfigPtr NULL\r\n",__func__,__LINE__);
//         //设置错误码
//         CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CAN_PROT_GET_CMD_CONFIG,ERROR_LEVEL_ERROR,
//                                         ERROR_SUB_BOARD_CAN_PROT_CMD_CONFIG_NOT_EXIST);
//         return;
//     }
//     //创建指令头
//     canCmdSendPtr = CanProtCreateCmdSend(targetModule,targetBoardID,singalChannel,channelCmdCode,canCmdConfigPtr);
//     //参数赋值
//     canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)rotateFlag;
//     canCmdSendPtr->cmdBufferPtr[1] = (uint8_t)scanFlag;
//     // ProtUtilWriteInt32ToBuffer(steps,canCmdSendPtr->cmdBufferPtr+1);
//     //指令发送
//     CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
//     //销毁发送指令
//     CanProtDestroyCmdSend(&canCmdSendPtr);
//     return;
// }



/*************************************等待指定指令完成返回************************************************/
//旋转扫码 输入读取
void CanMasterBoard1RotateScanHandWaitReadInputResult(SYS_RESULT_PACK* resultPackPtr,PIN_STATE* sensorStatePtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD1_ROTATE_SCAN;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD1_ROTATE_SCAN_READ_INPUT;
    uint8_t targetModule = CAN_MODULE_NO_BOARD1;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD1;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    if(sensorStatePtr != NULL)
    {
        //结果解析
        *sensorStatePtr = PIN_STATE_INVALID;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *sensorStatePtr = (PIN_STATE)(returnDataBufferPtr[0]);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//旋转扫码 输出写出
void CanMasterBoard1RotateScanHandWaitWriteOutputResult(SYS_RESULT_PACK* resultPackPtr,uint8_t* writeIndex)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD1_ROTATE_SCAN;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD1_ROTATE_SCAN_WRITE_OUTPUT;
    uint8_t targetModule = CAN_MODULE_NO_BOARD1;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD1;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    if(writeIndex != NULL)
    {
        //结果解析
        *writeIndex = 0;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *writeIndex = (uint8_t)(returnDataBufferPtr[0]);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//旋转扫码 指定步进电机复位
void CanMasterBoard1RotateScanWaitStepMotorResetResult(SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD1_ROTATE_SCAN;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD1_ROTATE_SCAN_MOTOR_RESET;
    uint8_t targetModule = CAN_MODULE_NO_BOARD1;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD1;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    if(modSmCanResultPtr != NULL)
    {
        //结果解析
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            modSmCanResultPtr->motorIndex = returnDataBufferPtr[0];
            modSmCanResultPtr->motorPosCurrent = ProtUtilReadInt32Buffer(returnDataBufferPtr+1);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//旋转扫码 电机走步数
void CanMasterBoard1RotateScanWaitStepMotorRunStepsResult(SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD1_ROTATE_SCAN;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD1_ROTATE_SCAN_MOTOR_RUN_STEP;
    uint8_t targetModule = CAN_MODULE_NO_BOARD1;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD1;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    if(modSmCanResultPtr != NULL)
    {
        //结果解析
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            modSmCanResultPtr->motorIndex = returnDataBufferPtr[0];
            modSmCanResultPtr->motorPosCurrent = ProtUtilReadInt32Buffer(returnDataBufferPtr+1);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//旋转扫码 整体复位
void CanMasterBoard1RotateScanWaitResetAllResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD1_ROTATE_SCAN;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD1_ROTATE_SCAN_RESET_ALL;
    uint8_t targetModule = CAN_MODULE_NO_BOARD1;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD1;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    // if(rackExistPtr != NULL)
    // {
    //     //结果解析
    //     *rackExistPtr = SENSOR_STATE_INVALID;
    //     if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
    //     {
    //         *rackExistPtr = (SENSOR_STATE)(returnDataBufferPtr[0]);
    //     }
    // }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//旋转扫码 准备接收试管架
void CanMasterBoard1RotateScanWaitPrepareRecvRackResult(SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD1_ROTATE_SCAN;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD1_ROTATE_SCAN_PREPARE_RECV_RACK;
    uint8_t targetModule = CAN_MODULE_NO_BOARD1;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD1;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    // if(rackIdPtr != NULL)
    // {
    //     //结果解析
    //     *rackIdPtr = 0;
    //     if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
    //     {
    //         *rackIdPtr = ProtUtilReadUint32Buffer(returnDataBufferPtr);
    //     }
    // }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//旋转扫码 样本架扫码并指定标签方向
void CanMasterBoard1RotateScanWaitRackScanRotateDirResult(SYS_RESULT_PACK* resultPackPtr,uint32_t *rackIdPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD1_ROTATE_SCAN;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD1_ROTATE_SCAN_RACK_SCAN_ROTATE_TARGET_DIR;
    uint8_t targetModule = CAN_MODULE_NO_BOARD1;
    uint8_t targetBoardID = CAN_BOARD_ID_BOARD1;
    //保存结果
    uint8_t* returnDataBufferPtr = NULL;
    uint16_t returnDataBufferLength = 0;
    //等待指令返回
    CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
                            &returnDataBufferPtr,&returnDataBufferLength);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        //释放内存
        CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
        return;
    }
    if(rackIdPtr != NULL)
    {
        //结果解析
        *rackIdPtr = 0;
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            *rackIdPtr = ProtUtilReadUint32Buffer(returnDataBufferPtr);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

// //旋转扫码 样本架旋转并扫码
// void CanMasterBoard1RotateScanWaitRackRotateAndScanResult(SYS_RESULT_PACK* resultPackPtr,uint32_t *rackIdPtr)
// {
//     uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_SUB_BOARD1_ROTATE_SCAN;
//     uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_BOARD1_ROTATE_SCAN_RACK_ROTATE_AND_SCAN;
//     uint8_t targetModule = CAN_MODULE_NO_BOARD1;
//     uint8_t targetBoardID = CAN_BOARD_ID_BOARD1;
//     //保存结果
//     uint8_t* returnDataBufferPtr = NULL;
//     uint16_t returnDataBufferLength = 0;
//     //等待指令返回
//     CanProtCmdWaitResult(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr,
//                             &returnDataBufferPtr,&returnDataBufferLength);
//     if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
//     {
//         //协议自身报错了
//         CAN_PROT_LOG("%s,%d,CanProtCmdWaitResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
//         //释放内存
//         CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
//         return;
//     }
//     if(rackIdPtr != NULL)
//     {
//         //结果解析
//         *rackIdPtr = 0;
//         if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
//         {
//             *rackIdPtr = ProtUtilReadUint32Buffer(returnDataBufferPtr);
//         }
//     }
//     //内存释放
//     CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
//     //回传结果,这个只是协议的结果不是代表指令的执行结果
//     return;
// }


/*************************************发送指令等待完成返回************************************************/
//旋转扫码 输入读取
void CanMasterBoard1RotateScanHandReadInputWhileReturn(uint8_t index,SYS_RESULT_PACK* resultPackPtr,PIN_STATE* sensorStatePtr)
{
    //发送指令等待ACK
    CanMasterBoard1RotateScanHandReadInputWhileAck(index,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard1RotateScanHandReadInputWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard1RotateScanHandWaitReadInputResult(resultPackPtr,sensorStatePtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard1RotateScanHandWaitReadInputResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//旋转扫码 输出写出
void CanMasterBoard1RotateScanHandWriteOutputWhileReturn(uint8_t index,PIN_STATE outState,SYS_RESULT_PACK* resultPackPtr,uint8_t* writeIndex)
{
    //发送指令等待ACK
    CanMasterBoard1RotateScanHandWriteOutputWhileAck(index,outState,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard1RotateScanHandWriteOutputWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard1RotateScanHandWaitWriteOutputResult(resultPackPtr,writeIndex);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard1RotateScanHandWaitWriteOutputResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//旋转扫码 指定步进电机复位
void CanMasterBoard1RotateScanStepMotorResetWhileReturn(uint8_t motorIndex,SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr)
{
    //发送指令等待ACK
    CanMasterBoard1RotateScanStepMotorResetWhileAck(motorIndex,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard1RotateScanStepMotorResetWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard1RotateScanWaitStepMotorResetResult(resultPackPtr,modSmCanResultPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard1RotateScanWaitStepMotorResetResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//旋转扫码 电机走步数
void CanMasterBoard1RotateScanStepMotorRunStepsWhileReturn(uint8_t motorIndex,int32_t steps,SYS_RESULT_PACK* resultPackPtr,MOD_SM_CAN_RESULT *modSmCanResultPtr)
{
    //发送指令等待ACK
    CanMasterBoard1RotateScanStepMotorRunStepsWhileAck(motorIndex,steps,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard1RotateScanStepMotorRunStepsWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard1RotateScanWaitStepMotorRunStepsResult(resultPackPtr,modSmCanResultPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard1RotateScanWaitStepMotorResetResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//旋转扫码 整体复位
void CanMasterBoard1RotateScanResetAllWhileReturn(SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterBoard1RotateScanResetAllWhileAck(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard1RotateScanResetAllWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard1RotateScanWaitResetAllResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard1RotateScanWaitResetAllResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}


//旋转扫码 准备接收试管架
void CanMasterBoard1RotateScanPrepareRecvRackWhileReturn(RACK_BAR_DIR rackBarDir,SYS_RESULT_PACK* resultPackPtr)
{
    //发送指令等待ACK
    CanMasterBoard1RotateScanPrepareRecvRackWhileAck(rackBarDir,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard1RotateScanPrepareRecvRackWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard1RotateScanWaitPrepareRecvRackResult(resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard1RotateScanWaitPrepareRecvRackResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}


//旋转扫码 样本架扫码并指定标签方向
void CanMasterBoard1RotateScanRackScanRotateDirWhileReturn(uint8_t scanFlag,RACK_BAR_DIR rackBarDir,SYS_RESULT_PACK* resultPackPtr,uint32_t *rackIdPtr)
{
    //发送指令等待ACK
    CanMasterBoard1RotateScanRackScanRotateDirWhileAck(scanFlag,rackBarDir,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard1RotateScanRackScanRotateDirWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterBoard1RotateScanWaitRackScanRotateDirResult(resultPackPtr,rackIdPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterBoard1RotateScanWaitRackScanRotateDirResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}


// //旋转扫码 样本架旋转并扫码
// void CanMasterBoard1RotateScanRackRotateAndScanWhileReturn(uint8_t rotateFlag,uint8_t scanFlag,SYS_RESULT_PACK* resultPackPtr,uint32_t *rackIdPtr)
// {
//     //发送指令等待ACK
//     CanMasterBoard1RotateScanRackRotateAndScanWhileAck(rotateFlag,scanFlag,resultPackPtr);
//     if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
//     {
//         //协议自身报错了
//         CAN_PROT_LOG("%s,%d,CanMasterBoard1RotateScanRackRotateAndScanWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
//         return;
//     }
//     //等待结果
//     CanMasterBoard1RotateScanWaitRackRotateAndScanResult(resultPackPtr,rackIdPtr);
//     if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
//     {
//         //协议自身报错了
//         CAN_PROT_LOG("%s,%d,CanMasterBoard1RotateScanWaitRackRotateAndScanResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
//     }
//     //回传结果,这个只是协议的结果不是代表指令的执行结果
//     return;
// }



