/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-31 16:34:18
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-02-02 14:16:26
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#include "SrvImplBasePortIn.h"
#include "SrvImplPortIn.h"
#include "CommonSrvTaskHeader.h"


/****************************************网口指令消息处理函数**************************************************/
//占位函数
static void SrvMsgNet8700PortInCmd_PlaceHolder(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//输入单个读取
static void SrvMsgNet8700PortInCmd_PortInReadSingle(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* sysResultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen); 
//输入区间读取        
static void SrvMsgNet8700PortInCmd_PortInReadRange(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* sysResultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);  
//输入批量读取
static void SrvMsgNet8700PortInCmd_PortInReadAny(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* sysResultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//NET指令处理函数列表
static const USER_NET_CMD_PROC_UNIT net8700CmdProcPortInUnitArray[] = 
{
    //顺序: 指令码  预期参数最小长度 是否需要结果返回  错误是否独立上报 指令处理函数
    {(uint32_t)NET_MASTER_CMD_HOLD_PLACE                 ,0  ,ENABLE ,SrvMsgNet8700PortInCmd_PlaceHolder        },
    {(uint32_t)NET_MASTER_CMD_MAIN_PORT_IN_READ_SINGLE   ,0  ,ENABLE ,SrvMsgNet8700PortInCmd_PortInReadSingle   },
    {(uint32_t)NET_MASTER_CMD_MAIN_PORT_IN_READ_RANGE    ,0  ,ENABLE ,SrvMsgNet8700PortInCmd_PortInReadRange    },
    {(uint32_t)NET_MASTER_CMD_MAIN_PORT_IN_READ_ANY      ,0  ,ENABLE ,SrvMsgNet8700PortInCmd_PortInReadAny      },
};

void SrvMsgNet8700PortInCmd(void* net8700CmdRecvPtr)
{
    //转换指令码
    NET_PROT_CMD_PACK_RECV* cmdPackPtr = (NET_PROT_CMD_PACK_RECV*)net8700CmdRecvPtr;
    if(cmdPackPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,Recv Net8700CmdMsgPtr NULL\r\n",__func__,__LINE__);
        //空指针异常
        return;
    }
    //接收打印显示
    SystemPrintf("PortIn Recv Net8700 Cmd: 0X%08X,DataLen: %d\r\n",cmdPackPtr->cmdCode,cmdPackPtr->dataBodyLength);
    //搜索匹配处理器
    uint16_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < DIM_ARRAY_ELEMENT_COUNT(net8700CmdProcPortInUnitArray);indexUtil++)
    {
        if(cmdPackPtr->cmdCode == (uint32_t)(net8700CmdProcPortInUnitArray[indexUtil].commandCode))
        {
            break;
        }
    }
    //判定序号
    if(indexUtil == DIM_ARRAY_ELEMENT_COUNT(net8700CmdProcPortInUnitArray))
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
    if(cmdPackPtr->dataBodyLength < net8700CmdProcPortInUnitArray[indexUtil].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,CommandCode: 0X%08X\r\n",__func__,__LINE__,
                        net8700CmdProcPortInUnitArray[indexUtil].exceptParamLengthMin,cmdPackPtr->dataBodyLength,
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
    if(net8700CmdProcPortInUnitArray[indexUtil].netCmdProcFuncPtr == NULL)
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
    net8700CmdProcPortInUnitArray[indexUtil].netCmdProcFuncPtr(cmdPackPtr,&resultPack,&resultDataBufferPtr,&resultDataLength);
    //结果处理,是否需要结果回传
    if(net8700CmdProcPortInUnitArray[indexUtil].needResultReply == ENABLE)
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
static void SrvMsgNet8700PortInCmd_PlaceHolder(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志打印
    SystemPrintf("%s\r\n",__func__);
    //正常完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
}


//输入单个读取
static void SrvMsgNet8700PortInCmd_PortInReadSingle(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* sysResultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    uint8_t readSingleIndex = cmdPackPtr->dataBodyBufferPtr[0];
    //日志参数打印
    SystemPrintf("%s,readSingleIndex : %d\r\n",__func__,readSingleIndex);
    //执行输入读取
    SrvImplBasePortInReadSingle(readSingleIndex,sysResultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//输入区间读取        
static void SrvMsgNet8700PortInCmd_PortInReadRange(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* sysResultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    uint8_t readSingleStartIdx = cmdPackPtr->dataBodyBufferPtr[0];
    uint8_t readSingleCount = cmdPackPtr->dataBodyBufferPtr[1];
    //日志参数打印
    SystemPrintf("%s,readSingleStartIdx : %d,readSingleCount: %d\r\n",__func__,readSingleStartIdx,readSingleCount);
    //执行输入区间
    SrvImplBasePortInReadRange(readSingleStartIdx,readSingleCount,sysResultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}
  
//输入批量读取
static void SrvMsgNet8700PortInCmd_PortInReadAny(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* sysResultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    uint8_t readSingleCount = cmdPackPtr->dataBodyLength;
    //日志参数打印
    SystemPrintf("%s,readSingleCount : %d\r\n",__func__,readSingleCount);
    uint8_t* readSingalIdxArrayPtr = cmdPackPtr->dataBodyBufferPtr;
    //执行批量读取
    SrvImplBasePortInReadAny(readSingleCount,readSingalIdxArrayPtr,sysResultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

/****************************************网口上传消息处理函数**************************************************/
//占位函数
static void SrvMsgNet8700PortInUpload_PlaceHolder(NET_PROT_UPLOAD_PACK_RECV* uploadPackPtr);

//NET上传消息处理函数列表
static const USER_NET_UPLOAD_PROC_UNIT netUploadPortInProcUnitArray[] = 
{
    //顺序是: 指令码 预期参数最小长度 上传消息处理函数
    {(uint32_t)NET_MASTER_REPORT_HOLD_PLACE     ,2 ,SrvMsgNet8700PortInUpload_PlaceHolder},
};

void SrvMsgNet8700PortInUpload(void* net8700ReportDataPtr)
{
    NET_PROT_UPLOAD_PACK_RECV* netUploadPackRecvPtr = (NET_PROT_UPLOAD_PACK_RECV*)net8700ReportDataPtr;
    if(netUploadPackRecvPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,Recv net8700UploadPackRecvPtr NULL\r\n",__func__,__LINE__);
        //空指针异常
        return;
    }
    //搜索匹配处理器
    uint16_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < DIM_ARRAY_ELEMENT_COUNT(netUploadPortInProcUnitArray);indexUtil++)
    {
        if(netUploadPackRecvPtr->uploadID == (uint32_t)(netUploadPortInProcUnitArray[indexUtil].uploadId))
        {
            break;
        }
    }
    //判定序号
    if(indexUtil == DIM_ARRAY_ELEMENT_COUNT(netUploadPortInProcUnitArray))
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
    if(netUploadPackRecvPtr->paramBufferLength < netUploadPortInProcUnitArray[indexUtil].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,UploadID: 0X%08X\r\n",__func__,__LINE__,
                        netUploadPortInProcUnitArray[indexUtil].exceptParamLengthMin,netUploadPackRecvPtr->paramBufferLength,
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
    if(netUploadPortInProcUnitArray[indexUtil].netUploadProcFuncPtr == NULL)
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
    netUploadPortInProcUnitArray[indexUtil].netUploadProcFuncPtr(netUploadPackRecvPtr);
    //处理完成,释放内存
    if((netUploadPackRecvPtr->paramBufferPtr != NULL)&&(netUploadPackRecvPtr->paramBufferLength != 0))
    {
        UserMemFree(MEM_REGION_SYS_MSG_NET,netUploadPackRecvPtr->paramBufferPtr);
    }
    UserMemFree(MEM_REGION_SYS_MSG_NET,netUploadPackRecvPtr);
}

//占位函数
static void SrvMsgNet8700PortInUpload_PlaceHolder(NET_PROT_UPLOAD_PACK_RECV* uploadPackPtr)
{
    asm("nop");
}









