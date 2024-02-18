#include "CanMasterDeviceMainTest.h"
#include "DriverConfigSystem.h"
#include SYS_LOG_INCLUDE_FILE

//通道配置数组
//参数分别是 指令  是否需要结果  超时时间  最小参数长度 附带参数长度
//附带参数长度为不定长的时候,指示参数长度为0,由上级调用者手动申请内存
static const CAN_PROT_CMD_CONFIG canMasteDeviceMainTestCmdConfigArray[] = {
    {CAN_RAIL_MUTIL_CMD_DEVICE_MAIN_TUBE_IN_POS                 ,        ENABLE,     3000,      7,      6       },//轨道主控 试管到位
    {CAN_RAIL_MUTIL_CMD_DEVICE_MAIN_TUBE_MOV_ARRIVE             ,        DISABLE,    3000,      0,      7       },//轨道主控 试管移动到位
    {CAN_RAIL_MUTIL_CMD_DEVICE_MAIN_DEVICE_TUBE_DUMP            ,        DISABLE,    3000,      0,      6       },//轨道主控 设备试管置废
    {CAN_RAIL_MUTIL_CMD_DEVICE_MAIN_RAIL_STATE_INFORM           ,        DISABLE,    3000,      0,      2       },//轨道主控 轨道状态通知 
    {CAN_RAIL_MUTIL_CMD_DEVICE_MAIN_RAIL_CHANGE_LOCK_RESULT     ,        DISABLE,    3000,      0,      6       },//轨道主控 轨道变轨锁定结果
    {CAN_RAIL_MUTIL_CMD_DEVICE_MAIN_INQUIRE_NEEDLE_POS          ,        ENABLE,     3000,      1,      0       },//轨道主控 样本针状态查询
};

/*************************************发送指令等待ACK************************************************/
//轨道主控 试管到位 Ack
void CanMasterDeviceMainTestTubeInPosWhileAck(RAIL_MODULE_NO railNo,RAIL_FUNC_TYPE railType,uint32_t rackId,uint8_t tubePos,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_DEVICE_MAIN_TEST;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_DEVICE_MAIN_TUBE_IN_POS;
    uint8_t targetModule = DeviceTestConfigArray[railNo][0];
    uint8_t targetBoardID = DeviceTestConfigArray[railNo][1];
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasteDeviceMainTestCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasteDeviceMainTestCmdConfigArray));
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
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)railType;
    ProtUtilWriteUint32ToBuffer(rackId,canCmdSendPtr->cmdBufferPtr+1);
    canCmdSendPtr->cmdBufferPtr[5] = tubePos;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//轨道主控 试管移动到位 Ack
void CanMasterDeviceMainTestTubeMovArriveWhileAck(RAIL_MODULE_NO railNo,RAIL_FUNC_TYPE railType,uint32_t rackId,uint8_t tubePos,uint8_t movResult,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_DEVICE_MAIN_TEST;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_DEVICE_MAIN_TUBE_MOV_ARRIVE;
    uint8_t targetModule = DeviceTestConfigArray[railNo][0];
    uint8_t targetBoardID = DeviceTestConfigArray[railNo][1];
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasteDeviceMainTestCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasteDeviceMainTestCmdConfigArray));
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
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)railType;
    ProtUtilWriteUint32ToBuffer(rackId,canCmdSendPtr->cmdBufferPtr+1);
    canCmdSendPtr->cmdBufferPtr[5] = (uint8_t)tubePos;
    canCmdSendPtr->cmdBufferPtr[6] = (uint8_t)movResult;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//轨道主控 设备试管置废 Ack
void CanMasterDeviceMainTestDeviceTubeDumpWhileAck(RAIL_MODULE_NO railNo,RAIL_FUNC_TYPE railType,uint32_t rackId,uint8_t tubePos,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_DEVICE_MAIN_TEST;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_DEVICE_MAIN_DEVICE_TUBE_DUMP;
    uint8_t targetModule = DeviceTestConfigArray[railNo][0];
    uint8_t targetBoardID = DeviceTestConfigArray[railNo][1];
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasteDeviceMainTestCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasteDeviceMainTestCmdConfigArray));
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
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)railType;
    ProtUtilWriteUint32ToBuffer(rackId,canCmdSendPtr->cmdBufferPtr+1);
    canCmdSendPtr->cmdBufferPtr[5] = (uint8_t)tubePos;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//轨道主控 轨道状态通知 Ack
void CanMasterDeviceMainRailStateInformWhileAck(RAIL_MODULE_NO railNo,RAIL_FUNC_TYPE railType,uint8_t errorFlag,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_DEVICE_MAIN_TEST;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_DEVICE_MAIN_RAIL_STATE_INFORM;
    uint8_t targetModule = DeviceTestConfigArray[railNo][0];
    uint8_t targetBoardID = DeviceTestConfigArray[railNo][1];
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasteDeviceMainTestCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasteDeviceMainTestCmdConfigArray));
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
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)railType;
    canCmdSendPtr->cmdBufferPtr[1] = (uint8_t)errorFlag;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//轨道主控 轨道变轨锁定结果 Ack
void CanMasterDeviceMainRailChangeLockResultWhileAck(RAIL_MODULE_NO railNo,RAIL_FUNC_TYPE railType,uint32_t rackID,uint8_t lockResult,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_DEVICE_MAIN_TEST;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_DEVICE_MAIN_RAIL_CHANGE_LOCK_RESULT;
    uint8_t targetModule = DeviceTestConfigArray[railNo][0];
    uint8_t targetBoardID = DeviceTestConfigArray[railNo][1];
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasteDeviceMainTestCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasteDeviceMainTestCmdConfigArray));
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
    canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)railType;
    ProtUtilWriteUint32ToBuffer(rackID,canCmdSendPtr->cmdBufferPtr);
    canCmdSendPtr->cmdBufferPtr[5] = (uint8_t)lockResult;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}

//轨道主控 查询样本针状态 Ack
void CanMasterDeviceMainTestInquireNeedlePosWhileAck(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_DEVICE_MAIN_TEST;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_DEVICE_MAIN_INQUIRE_NEEDLE_POS;
    uint8_t targetModule = DeviceTestConfigArray[railNo][0];
    uint8_t targetBoardID = DeviceTestConfigArray[railNo][1];
    CAN_PROT_CMD_SEND* canCmdSendPtr = NULL;
    CanProtClearLastCmd(targetModule,targetBoardID,singalChannel,channelCmdCode,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        CAN_PROT_LOG("%s,%d,ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    CAN_PROT_CMD_CONFIG* canCmdConfigPtr = CanProtGetCmdConfig(channelCmdCode,(CAN_PROT_CMD_CONFIG*)canMasteDeviceMainTestCmdConfigArray,
                                                                DIM_ARRAY_ELEMENT_COUNT(canMasteDeviceMainTestCmdConfigArray));
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
    // canCmdSendPtr->cmdBufferPtr[0] = (uint8_t)railType;
    //指令发送
    CanProtSendCmdWaitAck(canCmdSendPtr,resultPackPtr);
    //销毁发送指令
    CanProtDestroyCmdSend(&canCmdSendPtr);
    return;
}
/*************************************等待指定指令完成返回************************************************/
//轨道主控 试管到位 result
void CanMasterDeviceMainTestWaitTubeInPosResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,uint8_t *returnBuffer)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_DEVICE_MAIN_TEST;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_DEVICE_MAIN_TUBE_IN_POS;
    uint8_t targetModule = DeviceTestConfigArray[railNo][0];
    uint8_t targetBoardID = DeviceTestConfigArray[railNo][1];
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
    if(returnBuffer != NULL)
    {
        //结果解析
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            UserMemCopy(returnBuffer,returnDataBufferPtr,returnDataBufferLength);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//轨道主控 查询样本针位置 result
void CanMasterDeviceMainTestWaitInquireNeedlePosResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,uint8_t *returnBuffer)
{
    uint8_t singalChannel = (uint8_t)CAN_RAIL_MUTIL_CH_DEVICE_MAIN_TEST;
    uint32_t channelCmdCode = (uint32_t)CAN_RAIL_MUTIL_CMD_DEVICE_MAIN_INQUIRE_NEEDLE_POS;
    uint8_t targetModule = DeviceTestConfigArray[railNo][0];
    uint8_t targetBoardID = DeviceTestConfigArray[railNo][1];
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
    if(returnBuffer != NULL)
    {
        //结果解析
        if((returnDataBufferLength != 0)&&(returnDataBufferPtr != NULL))
        {
            UserMemCopy(returnBuffer,returnDataBufferPtr,returnDataBufferLength);
        }
    }
    //内存释放
    CanProtFreeRecvDataBuf(targetModule,targetBoardID,returnDataBufferPtr,returnDataBufferLength);
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

/*************************************发送指令等待完成返回************************************************/
//轨道主控 试管到位 return 
void CanMasterDeviceMainTestTubeInPosWhileReturn(RAIL_MODULE_NO railNo,RAIL_FUNC_TYPE railType,uint32_t rackId,uint8_t tubePos,SYS_RESULT_PACK* resultPackPtr,uint8_t *returnBuffer)
{
    //发送指令等待ACK
    CanMasterDeviceMainTestTubeInPosWhileAck(railNo,railType,rackId,tubePos,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterDeviceMainTestTubeInPosWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterDeviceMainTestWaitTubeInPosResult(railNo,resultPackPtr,returnBuffer);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterDeviceMainTestWaitTubeInPosResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}

//轨道主控 查询样本针位置 return 
void CanMasterDeviceMainTestInquireNeedlePosWhileReturn(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,uint8_t *returnBuffer)
{
    //发送指令等待ACK
    CanMasterDeviceMainTestInquireNeedlePosWhileAck(railNo,resultPackPtr);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterDeviceMainTestInquireNeedlePosWhileAck ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
        return;
    }
    //等待结果
    CanMasterDeviceMainTestWaitInquireNeedlePosResult(railNo,resultPackPtr,returnBuffer);
    if(resultPackPtr->resultCode == PROT_RESULT_FAIL)
    {
        //协议自身报错了
        CAN_PROT_LOG("%s,%d,CanMasterDeviceMainTestWaitInquireNeedlePosResult ErrorCode: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
    }
    //回传结果,这个只是协议的结果不是代表指令的执行结果
    return;
}