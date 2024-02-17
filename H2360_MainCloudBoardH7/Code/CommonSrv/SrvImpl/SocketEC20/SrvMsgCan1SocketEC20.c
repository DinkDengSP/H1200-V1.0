/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-03 16:12:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-08 17:17:56 +0800
************************************************************************************************/ 
#include "SrvImplSocketEC20.h"
#include "CommonSrvTaskHeader.h"
#include "SrvImplBaseSocketEC20.h"


/*******************************************CAN指令处理函数列表*************************************************/
//占位函数
static void SrvMsgCan1SocketEC20Cmd_PlaceHolder(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//设备连接网络
static void SrvMsgCan1SocketEC20Cmd_Connect(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//设备断开网络连接
static void SrvMsgCan1SocketEC20Cmd_Disconnect(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//设备发送一包数据
static void SrvMsgCan1SocketEC20Cmd_SendWithoutConnect(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//设备读取一次数据
static void SrvMsgCan1SocketEC20Cmd_ReadData(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//设备集成发送并接收数据
static void SrvMsgCan1SocketEC20Cmd_SendRecvWithConnect(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//CAN指令处理函数列表
static const USER_CAN_CMD_PROC_UNIT can1CmdProcSocketEC20UnitArray[] = 
{
    //顺序是 指令码 预期参数最小长度 是否需要结果返回  错误是否独立上报 指令处理函数
    {CAN_MASTER_CMD_PLACE_HOLDER                  ,0  ,ENABLE ,SrvMsgCan1SocketEC20Cmd_PlaceHolder  },
    {CAN_MASTER_CMD_CLD_SOCKET_CONNECT            ,6  ,ENABLE ,SrvMsgCan1SocketEC20Cmd_Connect   },
    {CAN_MASTER_CMD_CLD_SOCKET_DISCONNECT         ,0  ,ENABLE ,SrvMsgCan1SocketEC20Cmd_Disconnect   },
    {CAN_MASTER_CMD_CLD_SOCKET_SEND_PACK_ONLY     ,2  ,ENABLE ,SrvMsgCan1SocketEC20Cmd_SendWithoutConnect   },
    {CAN_MASTER_CMD_CLD_SOCKET_RECV_PACK_ONLY     ,4  ,ENABLE ,SrvMsgCan1SocketEC20Cmd_ReadData   },
    {CAN_MASTER_CMD_CLD_SOCKET_SEND_RECV_PACK     ,12 ,ENABLE ,SrvMsgCan1SocketEC20Cmd_SendRecvWithConnect   },
};


//CAN指令消息处理函数
void SrvMsgCan1SocketEC20Cmd(void* can1MsgBodyPtr)
{
    //转换指令码
    CAN_PROT_CMD_PACK* cmdPackPtr = (CAN_PROT_CMD_PACK*)can1MsgBodyPtr;
    if(cmdPackPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,Recv cmdPackPtr NULL\r\n",__func__,__LINE__);
        //空指针异常
        return;
    }
    //搜索匹配处理器
    uint16_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < DIM_ARRAY_ELEMENT_COUNT(can1CmdProcSocketEC20UnitArray);indexUtil++)
    {
        if(cmdPackPtr->cmdValue == (uint32_t)(can1CmdProcSocketEC20UnitArray[indexUtil].commandCode))
        {
            break;
        }
    }
    //判定序号
    if(indexUtil == DIM_ARRAY_ELEMENT_COUNT(can1CmdProcSocketEC20UnitArray))
    {
        //不支持的指令
        SystemPrintf("E:> %s,%d,Recv Unsupport CommandCode: 0X%08X\r\n",__func__,__LINE__,cmdPackPtr->cmdValue);
        //释放内存
        if((cmdPackPtr->paramBufferPtr != NULL)&&(cmdPackPtr->paramBufferLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_CAN,cmdPackPtr->paramBufferPtr);
        }
        UserMemFree(MEM_REGION_SYS_MSG_CAN,cmdPackPtr);
        return;
    }
    //判定长度
    if(cmdPackPtr->paramBufferLength < can1CmdProcSocketEC20UnitArray[indexUtil].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,CommandCode: 0X%08X\r\n",__func__,__LINE__,
                        can1CmdProcSocketEC20UnitArray[indexUtil].exceptParamLengthMin,cmdPackPtr->paramBufferLength,
                        cmdPackPtr->cmdValue);
        //释放内存
        if((cmdPackPtr->paramBufferPtr != NULL)&&(cmdPackPtr->paramBufferLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_CAN,cmdPackPtr->paramBufferPtr);
        }
        UserMemFree(MEM_REGION_SYS_MSG_CAN,cmdPackPtr);
        return;
    }
    //校验通过执行代码
    SYS_RESULT_PACK resultPack;
    uint8_t* resultDataBufferPtr = NULL;
    uint16_t resultDataLength = 0;
    //初始化错误返回包
    resultPack.resultCode = PROT_RESULT_FAIL;
    resultPack.errorMain = ERROR_MAIN_OK;
    resultPack.errorLevel = ERROR_LEVEL_NONE;
    resultPack.errorSub = ERROR_SUB_OK;
    //检测函数指针
    if(can1CmdProcSocketEC20UnitArray[indexUtil].canCmdProcFuncPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,canCmdProcFuncPtr NULL,CommandCode: 0X%08X\r\n",__func__,__LINE__,
                        cmdPackPtr->cmdValue);
        //释放内存
        if((cmdPackPtr->paramBufferPtr != NULL)&&(cmdPackPtr->paramBufferLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_CAN,cmdPackPtr->paramBufferPtr);
        }
        UserMemFree(MEM_REGION_SYS_MSG_CAN,cmdPackPtr);
        return;
    }
    //调用处理函数
    can1CmdProcSocketEC20UnitArray[indexUtil].canCmdProcFuncPtr(cmdPackPtr,&resultPack,&resultDataBufferPtr,&resultDataLength);
    //结果处理,是否需要结果回传
    if(can1CmdProcSocketEC20UnitArray[indexUtil].needResultReply == ENABLE)
    {
        //需要结果回传
        Can1ProtSendResultByRecv(cmdPackPtr,&resultPack,resultDataBufferPtr,resultDataLength);
    }
    //释放内存
    if((resultDataBufferPtr != NULL)&&(resultDataLength != 0))
    {
        UserMemFree(MEM_REGION_SRV_TASK,resultDataBufferPtr);
    }
    if((cmdPackPtr->paramBufferPtr != NULL)&&(cmdPackPtr->paramBufferLength != 0))
    {
        UserMemFree(MEM_REGION_SYS_MSG_CAN,cmdPackPtr->paramBufferPtr);
    }
    UserMemFree(MEM_REGION_SYS_MSG_CAN,cmdPackPtr);
}

//占位函数
static void SrvMsgCan1SocketEC20Cmd_PlaceHolder(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//设备连接网络
static void SrvMsgCan1SocketEC20Cmd_Connect(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //获取参数
    uint8_t ip1,ip2,ip3,ip4;
    uint16_t remotePort;
    ip1 = cmdPackPtr->paramBufferPtr[0];
    ip2 = cmdPackPtr->paramBufferPtr[1];
    ip3 = cmdPackPtr->paramBufferPtr[2];
    ip4 = cmdPackPtr->paramBufferPtr[3];
    remotePort = ProtUtilReadUint16Buffer(cmdPackPtr->paramBufferPtr+4);
    //日志打印
    SystemPrintf("%s,IP:%d.%d.%d.%d,Port: %d\r\n",__func__,ip1,ip2,ip3,ip4,remotePort);
    //执行指令
    SrvImplBaseSocketEC20Connect(ip1,ip2,ip3,ip4,remotePort,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//设备断开网络连接
static void SrvMsgCan1SocketEC20Cmd_Disconnect(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志打印
    SystemPrintf("%s\r\n",__func__);
    //执行指令
    SrvImplBaseSocketEC20Disconnect(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//设备发送一包数据
static void SrvMsgCan1SocketEC20Cmd_SendWithoutConnect(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //获取参数
    uint16_t sendLength = ProtUtilReadUint16Buffer(cmdPackPtr->paramBufferPtr);
    uint8_t* sendBufferPtr = cmdPackPtr->paramBufferPtr+2;
    //日志打印
    SystemPrintf("%s,SendLength: %d\r\n",__func__,sendLength);
    //执行指令
    SrvImplBaseSocketEC20SendWithoutConnect(sendBufferPtr,sendLength,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//设备读取一次数据
static void SrvMsgCan1SocketEC20Cmd_ReadData(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //获取参数
    uint16_t exceptRecvDataLength = ProtUtilReadUint16Buffer(cmdPackPtr->paramBufferPtr);
    uint16_t exceptRecvDataTimeOutMs = ProtUtilReadUint16Buffer(cmdPackPtr->paramBufferPtr+2);
    //日志打印
    SystemPrintf("%s,RecvDataLen: %d,TimeOutMs: %d\r\n",__func__,exceptRecvDataLength,exceptRecvDataTimeOutMs);
    //执行指令
    SrvImplBaseSocketEC20ReadData(exceptRecvDataLength,exceptRecvDataTimeOutMs,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//设备集成发送并接收数据
static void SrvMsgCan1SocketEC20Cmd_SendRecvWithConnect(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //获取参数
    uint8_t ip1,ip2,ip3,ip4;
    uint16_t remotePort;
    ip1 = cmdPackPtr->paramBufferPtr[0];
    ip2 = cmdPackPtr->paramBufferPtr[1];
    ip3 = cmdPackPtr->paramBufferPtr[2];
    ip4 = cmdPackPtr->paramBufferPtr[3];
    remotePort = ProtUtilReadUint16Buffer(cmdPackPtr->paramBufferPtr+4);
    uint16_t exceptRecvDataLength = ProtUtilReadUint16Buffer(cmdPackPtr->paramBufferPtr+6);
    uint16_t exceptRecvDataTimeOutMs = ProtUtilReadUint16Buffer(cmdPackPtr->paramBufferPtr+8);
    uint16_t sendLength = ProtUtilReadUint16Buffer(cmdPackPtr->paramBufferPtr+10);
    uint8_t* sendBufferPtr = cmdPackPtr->paramBufferPtr+12;
    //日志打印
    SystemPrintf("%s,IP:%d.%d.%d.%d,Port: %d\r\n",__func__,ip1,ip2,ip3,ip4,remotePort);
    //日志打印
    SystemPrintf("%s,SendLength: %d,RecvDataLen: %d,TimeOutMs: %d\r\n",__func__,sendLength,
                    exceptRecvDataLength,exceptRecvDataTimeOutMs);
    //执行指令
    SrvImplBaseSocketEC20SendRecvWithConnect(ip1,ip2,ip3,ip4,remotePort,sendBufferPtr,sendLength,exceptRecvDataLength,
                                                exceptRecvDataTimeOutMs,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

/*************************************************************************************************************************************/

/*************************************************************CAN上传参数处理***********************************************************/
//占位函数
static void SrvMsgCan1SocketEC20Upload_PlaceHolder(CAN_PROT_UPLOAD_PACK* canUploadPackPtr);

//CAN上传消息处理函数列表
static const USER_CAN_UPLOAD_PROC_UNIT can1MsgUploadProcSocketEC20UnitArray[] = 
{
    //顺序是: 指令码 预期参数最小长度 上传消息处理函数
    {CAN_MASTER_REPORT_ID_HOLD_PLACE     ,2 ,SrvMsgCan1SocketEC20Upload_PlaceHolder},
};

//CAN上传消息处理函数
void SrvMsgCan1SocketEC20Upload(void* can1MsgBodyPtr)
{
    CAN_PROT_UPLOAD_PACK* canUploadPackPtr = (CAN_PROT_UPLOAD_PACK*)can1MsgBodyPtr;
    if(canUploadPackPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,Recv canUploadPackPtr NULL\r\n",__func__,__LINE__);
        //空指针异常
        return;
    }
    //搜索匹配处理器
    uint16_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < DIM_ARRAY_ELEMENT_COUNT(can1MsgUploadProcSocketEC20UnitArray);indexUtil++)
    {
        if(canUploadPackPtr->uploadID == (uint32_t)(can1MsgUploadProcSocketEC20UnitArray[indexUtil].uploadId))
        {
            break;
        }
    }
    //判定序号
    if(indexUtil == DIM_ARRAY_ELEMENT_COUNT(can1MsgUploadProcSocketEC20UnitArray))
    {
        //不支持的指令
        SystemPrintf("E:> %s,%d,Recv Unsupport UploadID: 0X%08X\r\n",__func__,__LINE__,canUploadPackPtr->uploadID);
        //释放内存
        if((canUploadPackPtr->uploadDataBufferPtr != NULL)&&(canUploadPackPtr->uploadDataBufferLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackPtr->uploadDataBufferPtr);
        }
        UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackPtr);
        return;
    }
    //判定长度
    if(canUploadPackPtr->uploadDataBufferLength < can1MsgUploadProcSocketEC20UnitArray[indexUtil].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,UploadID: 0X%08X\r\n",__func__,__LINE__,
                        can1MsgUploadProcSocketEC20UnitArray[indexUtil].exceptParamLengthMin,canUploadPackPtr->uploadDataBufferLength,
                        canUploadPackPtr->uploadID);
        //释放内存
        if((canUploadPackPtr->uploadDataBufferPtr != NULL)&&(canUploadPackPtr->uploadDataBufferLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackPtr->uploadDataBufferPtr);
        }
        UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackPtr);
        return;
    }
    //检测函数指针
    if(can1MsgUploadProcSocketEC20UnitArray[indexUtil].canUploadProcFuncPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,canUploadProcFuncPtr NULL,UploadID: 0X%08X\r\n",__func__,__LINE__,
                        canUploadPackPtr->uploadID);
        //释放内存
        if((canUploadPackPtr->uploadDataBufferPtr != NULL)&&(canUploadPackPtr->uploadDataBufferLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackPtr->uploadDataBufferPtr);
        }
        UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackPtr);
        return;
    }
    //调用指令处理
    can1MsgUploadProcSocketEC20UnitArray[indexUtil].canUploadProcFuncPtr(canUploadPackPtr);
    //处理完成,释放内存
    if((canUploadPackPtr->uploadDataBufferPtr != NULL)&&(canUploadPackPtr->uploadDataBufferLength != 0))
    {
        UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackPtr->uploadDataBufferPtr);
    }
    UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackPtr);
}

//占位函数
static void SrvMsgCan1SocketEC20Upload_PlaceHolder(CAN_PROT_UPLOAD_PACK* canUploadPackPtr)
{

}
/*************************************************************************************************************************************/
























