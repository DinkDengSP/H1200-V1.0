/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-03 16:12:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-06 08:54:17 +0800
************************************************************************************************/ 
#include "SrvImplStartHeart.h"
#include "SrvTaskHeader.h"
#include "SrvImplBaseStartHeart.h"


/****************************************网口指令消息处理函数**************************************************/
//占位函数
static void SrvMsgNetStartHeartCmd_HoldPlace(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//NET指令处理函数列表
static const USER_NET_CMD_PROC_UNIT netCmdProcStartHeartUnitArray[] = 
{
    //顺序: 指令码  预期参数最小长度 是否需要结果返回  错误是否独立上报 指令处理函数
    {(uint32_t)NET_MASTER_CMD_HOLD_PLACE         ,0  ,ENABLE ,SrvMsgNetStartHeartCmd_HoldPlace   },
};

void SrvMsgNetStartHeartCmd(void* netCmdRecvPtr)
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
    SystemPrintf("StartHeart Recv Net Cmd: 0X%08X,DataLen: %d\r\n",cmdPackPtr->cmdCode,cmdPackPtr->dataBodyLength);
    //搜索匹配处理器
    uint16_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < DIM_ARRAY_ELEMENT_COUNT(netCmdProcStartHeartUnitArray);indexUtil++)
    {
        if(cmdPackPtr->cmdCode == (uint32_t)(netCmdProcStartHeartUnitArray[indexUtil].commandCode))
        {
            break;
        }
    }
    //判定序号
    if(indexUtil == DIM_ARRAY_ELEMENT_COUNT(netCmdProcStartHeartUnitArray))
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
    if(cmdPackPtr->dataBodyLength < netCmdProcStartHeartUnitArray[indexUtil].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,CommandCode: 0X%08X\r\n",__func__,__LINE__,
                        netCmdProcStartHeartUnitArray[indexUtil].exceptParamLengthMin,cmdPackPtr->dataBodyLength,
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
    if(netCmdProcStartHeartUnitArray[indexUtil].netCmdProcFuncPtr == NULL)
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
    netCmdProcStartHeartUnitArray[indexUtil].netCmdProcFuncPtr(cmdPackPtr,&resultPack,&resultDataBufferPtr,&resultDataLength);
    //结果处理,是否需要结果回传
    if(netCmdProcStartHeartUnitArray[indexUtil].needResultReply == ENABLE)
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
static void SrvMsgNetStartHeartCmd_HoldPlace(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志打印
    SystemPrintf("%s\r\n",__func__);
    //正常完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}


/****************************************网口上传消息处理函数**************************************************/
//占位函数
static void SrvMsgNetStartHeartUpload_PlaceHolder(NET_PROT_UPLOAD_PACK_RECV* uploadPackPtr);

//NET上传消息处理函数列表
static const USER_NET_UPLOAD_PROC_UNIT netUploadProcStartHeartUnitArray[] = 
{
    //顺序是: 指令码 预期参数最小长度 上传消息处理函数
    {(uint32_t)NET_MASTER_REPORT_HOLD_PLACE     ,2 ,SrvMsgNetStartHeartUpload_PlaceHolder},
};

void SrvMsgNetStartHeartUpload(void* netReportDataPtr)
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
    for(indexUtil = 0; indexUtil < DIM_ARRAY_ELEMENT_COUNT(netUploadProcStartHeartUnitArray);indexUtil++)
    {
        if(netUploadPackRecvPtr->uploadID == (uint32_t)(netUploadProcStartHeartUnitArray[indexUtil].uploadId))
        {
            break;
        }
    }
    //判定序号
    if(indexUtil == DIM_ARRAY_ELEMENT_COUNT(netUploadProcStartHeartUnitArray))
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
    if(netUploadPackRecvPtr->paramBufferLength < netUploadProcStartHeartUnitArray[indexUtil].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,UploadID: 0X%08X\r\n",__func__,__LINE__,
                        netUploadProcStartHeartUnitArray[indexUtil].exceptParamLengthMin,netUploadPackRecvPtr->paramBufferLength,
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
    if(netUploadProcStartHeartUnitArray[indexUtil].netUploadProcFuncPtr == NULL)
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
    netUploadProcStartHeartUnitArray[indexUtil].netUploadProcFuncPtr(netUploadPackRecvPtr);
    //处理完成,释放内存
    if((netUploadPackRecvPtr->paramBufferPtr != NULL)&&(netUploadPackRecvPtr->paramBufferLength != 0))
    {
        UserMemFree(MEM_REGION_SYS_MSG_NET,netUploadPackRecvPtr->paramBufferPtr);
    }
    UserMemFree(MEM_REGION_SYS_MSG_NET,netUploadPackRecvPtr);
}

//占位函数
static void SrvMsgNetStartHeartUpload_PlaceHolder(NET_PROT_UPLOAD_PACK_RECV* uploadPackPtr)
{
    asm("nop");
}






















