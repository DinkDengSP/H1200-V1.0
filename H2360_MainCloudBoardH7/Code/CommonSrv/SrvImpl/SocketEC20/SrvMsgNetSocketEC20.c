/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-03 16:12:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-08 17:04:42 +0800
************************************************************************************************/ 
#include "SrvImplSocketEC20.h"
#include "CommonSrvTaskHeader.h"
#include "SrvImplBaseSocketEC20.h"


/****************************************网口指令消息处理函数**************************************************/
//占位函数
static void SrvMsgNetSocketEC20Cmd_HoldPlace(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//设备连接网络
static void SrvMsgNetSocketEC20Cmd_Connect(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//设备断开网络连接
static void SrvMsgNetSocketEC20Cmd_Disconnect(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//设备发送一包数据
static void SrvMsgNetSocketEC20Cmd_SendWithoutConnect(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//设备读取一次数据
static void SrvMsgNetSocketEC20Cmd_ReadData(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//设备集成发送并接收数据
static void SrvMsgNetSocketEC20Cmd_SendRecvWithConnect(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//NET指令处理函数列表
static const USER_NET_CMD_PROC_UNIT netCmdProcSocketEC20UnitArray[] = 
{
    //顺序: 指令码  预期参数最小长度 是否需要结果返回  错误是否独立上报 指令处理函数
    {(uint32_t)NET_MASTER_CMD_HOLD_PLACE                    ,0  ,ENABLE ,SrvMsgNetSocketEC20Cmd_HoldPlace   },
    {(uint32_t)NET_MASTER_CMD_CLD_SOCKET_CONNECT            ,6  ,ENABLE ,SrvMsgNetSocketEC20Cmd_Connect   },
    {(uint32_t)NET_MASTER_CMD_CLD_SOCKET_DISCONNECT         ,0  ,ENABLE ,SrvMsgNetSocketEC20Cmd_Disconnect   },
    {(uint32_t)NET_MASTER_CMD_CLD_SOCKET_SEND_PACK_ONLY     ,2  ,ENABLE ,SrvMsgNetSocketEC20Cmd_SendWithoutConnect   },
    {(uint32_t)NET_MASTER_CMD_CLD_SOCKET_RECV_PACK_ONLY     ,4  ,ENABLE ,SrvMsgNetSocketEC20Cmd_ReadData   },
    {(uint32_t)NET_MASTER_CMD_CLD_SOCKET_SEND_RECV_PACK     ,12  ,ENABLE ,SrvMsgNetSocketEC20Cmd_SendRecvWithConnect   },
};

void SrvMsgNetSocketEC20Cmd(void* netCmdRecvPtr)
{
    //转换指令码
    NET_PROT_CMD_PACK_RECV* cmdPackPtr = (NET_PROT_CMD_PACK_RECV*)netCmdRecvPtr;
    if(cmdPackPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,Recv NetCmdMsgPtr NULL\r\n",__func__,__LINE__);
        //空指针异常
        return;
    }
    //接收打印显示
    SystemPrintf("SocketEC20 Recv Net Cmd: 0X%08X,DataLen: %d\r\n",cmdPackPtr->cmdCode,cmdPackPtr->dataBodyLength);
    //搜索匹配处理器
    uint16_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < DIM_ARRAY_ELEMENT_COUNT(netCmdProcSocketEC20UnitArray);indexUtil++)
    {
        if(cmdPackPtr->cmdCode == (uint32_t)(netCmdProcSocketEC20UnitArray[indexUtil].commandCode))
        {
            break;
        }
    }
    //判定序号
    if(indexUtil == DIM_ARRAY_ELEMENT_COUNT(netCmdProcSocketEC20UnitArray))
    {
        //不支持的指令
        SystemPrintf("E:> %s,%d,Recv Unsupport CommandCode: 0X%08X\r\n",__func__,__LINE__,cmdPackPtr->cmdCode);
        //释放内存
        if((cmdPackPtr->dataBodyBufferPtr != NULL)&&(cmdPackPtr->dataBodyLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_NET,cmdPackPtr->dataBodyBufferPtr);
        }
        UserMemFree(MEM_REGION_SYS_MSG_NET,cmdPackPtr);
        return;
    }
    //判定长度
    if(cmdPackPtr->dataBodyLength < netCmdProcSocketEC20UnitArray[indexUtil].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,CommandCode: 0X%08X\r\n",__func__,__LINE__,
                        netCmdProcSocketEC20UnitArray[indexUtil].exceptParamLengthMin,cmdPackPtr->dataBodyLength,
                        cmdPackPtr->cmdCode);
        //释放内存
        if((cmdPackPtr->dataBodyBufferPtr != NULL)&&(cmdPackPtr->dataBodyLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_NET,cmdPackPtr->dataBodyBufferPtr);
        }
        UserMemFree(MEM_REGION_SYS_MSG_NET,cmdPackPtr);
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
    if(netCmdProcSocketEC20UnitArray[indexUtil].netCmdProcFuncPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,netCmdProcFuncPtr NULL,CommandCode: 0X%08X\r\n",__func__,__LINE__,
                        cmdPackPtr->cmdCode);
        //释放内存
        if((cmdPackPtr->dataBodyBufferPtr != NULL)&&(cmdPackPtr->dataBodyLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_NET,cmdPackPtr->dataBodyBufferPtr);
        }
        UserMemFree(MEM_REGION_SYS_MSG_NET,cmdPackPtr);
        return;
    }
    //调用处理函数
    netCmdProcSocketEC20UnitArray[indexUtil].netCmdProcFuncPtr(cmdPackPtr,&resultPack,&resultDataBufferPtr,&resultDataLength);
    //结果处理,是否需要结果回传
    if(netCmdProcSocketEC20UnitArray[indexUtil].needResultReply == ENABLE)
    {
        //需要结果回传
        NetMasterSendResultByRecv(cmdPackPtr,&resultPack,resultDataBufferPtr,resultDataLength);
    }
    //释放内存
    if((resultDataBufferPtr != NULL)&&(resultDataLength != 0))
    {
        UserMemFree(MEM_REGION_SRV_TASK,resultDataBufferPtr);
    }
    if((cmdPackPtr->dataBodyBufferPtr != NULL)&&(cmdPackPtr->dataBodyLength != 0))
    {
        UserMemFree(MEM_REGION_SYS_MSG_NET,cmdPackPtr->dataBodyBufferPtr);
    }
    UserMemFree(MEM_REGION_SYS_MSG_NET,cmdPackPtr);
}

//占位函数
static void SrvMsgNetSocketEC20Cmd_HoldPlace(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志打印
    SystemPrintf("%s\r\n",__func__);
    //正常完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//设备连接网络
static void SrvMsgNetSocketEC20Cmd_Connect(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //获取参数
    uint8_t ip1,ip2,ip3,ip4;
    uint16_t remotePort;
    ip1 = cmdPackPtr->dataBodyBufferPtr[0];
    ip2 = cmdPackPtr->dataBodyBufferPtr[1];
    ip3 = cmdPackPtr->dataBodyBufferPtr[2];
    ip4 = cmdPackPtr->dataBodyBufferPtr[3];
    remotePort = ProtUtilReadUint16Buffer(cmdPackPtr->dataBodyBufferPtr+4);
    //日志打印
    SystemPrintf("%s,IP:%d.%d.%d.%d,Port: %d\r\n",__func__,ip1,ip2,ip3,ip4,remotePort);
    //执行指令
    SrvImplBaseSocketEC20Connect(ip1,ip2,ip3,ip4,remotePort,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//设备断开网络连接
static void SrvMsgNetSocketEC20Cmd_Disconnect(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志打印
    SystemPrintf("%s\r\n",__func__);
    //执行指令
    SrvImplBaseSocketEC20Disconnect(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//设备发送一包数据
static void SrvMsgNetSocketEC20Cmd_SendWithoutConnect(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //获取参数
    uint16_t sendLength = ProtUtilReadUint16Buffer(cmdPackPtr->dataBodyBufferPtr);
    uint8_t* sendBufferPtr = cmdPackPtr->dataBodyBufferPtr+2;
    //日志打印
    SystemPrintf("%s,SendLength: %d\r\n",__func__,sendLength);
    //执行指令
    SrvImplBaseSocketEC20SendWithoutConnect(sendBufferPtr,sendLength,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//设备读取一次数据
static void SrvMsgNetSocketEC20Cmd_ReadData(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //获取参数
    uint16_t exceptRecvDataLength = ProtUtilReadUint16Buffer(cmdPackPtr->dataBodyBufferPtr);
    uint16_t exceptRecvDataTimeOutMs = ProtUtilReadUint16Buffer(cmdPackPtr->dataBodyBufferPtr+2);
    //日志打印
    SystemPrintf("%s,RecvDataLen: %d,TimeOutMs: %d\r\n",__func__,exceptRecvDataLength,exceptRecvDataTimeOutMs);
    //执行指令
    SrvImplBaseSocketEC20ReadData(exceptRecvDataLength,exceptRecvDataTimeOutMs,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//设备集成发送并接收数据
static void SrvMsgNetSocketEC20Cmd_SendRecvWithConnect(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //获取参数
    uint8_t ip1,ip2,ip3,ip4;
    uint16_t remotePort;
    ip1 = cmdPackPtr->dataBodyBufferPtr[0];
    ip2 = cmdPackPtr->dataBodyBufferPtr[1];
    ip3 = cmdPackPtr->dataBodyBufferPtr[2];
    ip4 = cmdPackPtr->dataBodyBufferPtr[3];
    remotePort = ProtUtilReadUint16Buffer(cmdPackPtr->dataBodyBufferPtr+4);
    uint16_t exceptRecvDataLength = ProtUtilReadUint16Buffer(cmdPackPtr->dataBodyBufferPtr+6);
    uint16_t exceptRecvDataTimeOutMs = ProtUtilReadUint16Buffer(cmdPackPtr->dataBodyBufferPtr+8);
    uint16_t sendLength = ProtUtilReadUint16Buffer(cmdPackPtr->dataBodyBufferPtr+10);
    uint8_t* sendBufferPtr = cmdPackPtr->dataBodyBufferPtr+12;
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


/****************************************网口上传消息处理函数**************************************************/
//占位函数
static void SrvMsgNetSocketEC20Upload_PlaceHolder(NET_PROT_UPLOAD_PACK_RECV* uploadPackPtr);

//NET上传消息处理函数列表
static const USER_NET_UPLOAD_PROC_UNIT netUploadProcSocketEC20UnitArray[] = 
{
    //顺序是: 指令码 预期参数最小长度 上传消息处理函数
    {(uint32_t)NET_MASTER_REPORT_HOLD_PLACE     ,2 ,SrvMsgNetSocketEC20Upload_PlaceHolder},
};

void SrvMsgNetSocketEC20Upload(void* netReportDataPtr)
{
    NET_PROT_UPLOAD_PACK_RECV* netUploadPackRecvPtr = (NET_PROT_UPLOAD_PACK_RECV*)netReportDataPtr;
    if(netUploadPackRecvPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,Recv netUploadPackRecvPtr NULL\r\n",__func__,__LINE__);
        //空指针异常
        return;
    }
    //搜索匹配处理器
    uint16_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < DIM_ARRAY_ELEMENT_COUNT(netUploadProcSocketEC20UnitArray);indexUtil++)
    {
        if(netUploadPackRecvPtr->uploadID == (uint32_t)(netUploadProcSocketEC20UnitArray[indexUtil].uploadId))
        {
            break;
        }
    }
    //判定序号
    if(indexUtil == DIM_ARRAY_ELEMENT_COUNT(netUploadProcSocketEC20UnitArray))
    {
        //不支持的指令
        SystemPrintf("E:> %s,%d,Recv Unsupport UploadID: 0X%08X\r\n",__func__,__LINE__,netUploadPackRecvPtr->uploadID);
        //释放内存
        if((netUploadPackRecvPtr->paramBufferPtr != NULL)&&(netUploadPackRecvPtr->paramBufferLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_NET,netUploadPackRecvPtr->paramBufferPtr);
        }
        UserMemFree(MEM_REGION_SYS_MSG_NET,netUploadPackRecvPtr);
        return;
    }
    //判定长度
    if(netUploadPackRecvPtr->paramBufferLength < netUploadProcSocketEC20UnitArray[indexUtil].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,UploadID: 0X%08X\r\n",__func__,__LINE__,
                        netUploadProcSocketEC20UnitArray[indexUtil].exceptParamLengthMin,netUploadPackRecvPtr->paramBufferLength,
                        netUploadPackRecvPtr->uploadID);
        //释放内存
        if((netUploadPackRecvPtr->paramBufferPtr != NULL)&&(netUploadPackRecvPtr->paramBufferLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_NET,netUploadPackRecvPtr->paramBufferPtr);
        }
        UserMemFree(MEM_REGION_SYS_MSG_NET,netUploadPackRecvPtr);
        return;
    }
    //检测函数指针
    if(netUploadProcSocketEC20UnitArray[indexUtil].netUploadProcFuncPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,netUploadProcFuncPtr NULL,UploadID: 0X%08X\r\n",__func__,__LINE__,
                        netUploadPackRecvPtr->uploadID);
        //释放内存
        if((netUploadPackRecvPtr->paramBufferPtr != NULL)&&(netUploadPackRecvPtr->paramBufferLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_NET,netUploadPackRecvPtr->paramBufferPtr);
        }
        UserMemFree(MEM_REGION_SYS_MSG_NET,netUploadPackRecvPtr);
        return;
    }
    //调用指令处理
    netUploadProcSocketEC20UnitArray[indexUtil].netUploadProcFuncPtr(netUploadPackRecvPtr);
    //处理完成,释放内存
    if((netUploadPackRecvPtr->paramBufferPtr != NULL)&&(netUploadPackRecvPtr->paramBufferLength != 0))
    {
        UserMemFree(MEM_REGION_SYS_MSG_NET,netUploadPackRecvPtr->paramBufferPtr);
    }
    UserMemFree(MEM_REGION_SYS_MSG_NET,netUploadPackRecvPtr);
}

//占位函数
static void SrvMsgNetSocketEC20Upload_PlaceHolder(NET_PROT_UPLOAD_PACK_RECV* uploadPackPtr)
{
    asm("nop");
}






















