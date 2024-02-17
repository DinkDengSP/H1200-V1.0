/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-31 16:34:18
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-02-01 11:27:17
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#include "SrvImplUtil.h"
#include "SrvImplBaseUtil.h"
#include "CommonSrvTaskHeader.h"


/*******************************************CAN指令处理函数列表*************************************************/
//占位函数
static void SrvMsgCan1UtilCmd_PlaceHolder(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//CAN指令处理函数列表
static const USER_CAN_CMD_PROC_UNIT can1CmdProcUtilUnitArray[] = 
{
    //顺序是 指令码 预期参数最小长度 是否需要结果返回  错误是否独立上报 指令处理函数
    {CAN_MASTER_CMD_PLACE_HOLDER                  ,0  ,ENABLE ,SrvMsgCan1UtilCmd_PlaceHolder  },
};


//CAN指令消息处理函数
void SrvMsgCan1UtilCmd(void* can1MsgBodyPtr)
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
    for(indexUtil = 0; indexUtil < DIM_ARRAY_ELEMENT_COUNT(can1CmdProcUtilUnitArray);indexUtil++)
    {
        if(cmdPackPtr->cmdValue == (uint32_t)(can1CmdProcUtilUnitArray[indexUtil].commandCode))
        {
            break;
        }
    }
    //判定序号
    if(indexUtil == DIM_ARRAY_ELEMENT_COUNT(can1CmdProcUtilUnitArray))
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
    if(cmdPackPtr->paramBufferLength < can1CmdProcUtilUnitArray[indexUtil].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,CommandCode: 0X%08X\r\n",__func__,__LINE__,
                        can1CmdProcUtilUnitArray[indexUtil].exceptParamLengthMin,cmdPackPtr->paramBufferLength,
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
    if(can1CmdProcUtilUnitArray[indexUtil].canCmdProcFuncPtr == NULL)
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
    can1CmdProcUtilUnitArray[indexUtil].canCmdProcFuncPtr(cmdPackPtr,&resultPack,&resultDataBufferPtr,&resultDataLength);
    //结果处理,是否需要结果回传
    if(can1CmdProcUtilUnitArray[indexUtil].needResultReply == ENABLE)
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
static void SrvMsgCan1UtilCmd_PlaceHolder(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

/*************************************************************************************************************************************/

/*************************************************************CAN上传参数处理***********************************************************/
//占位函数
static void SrvMsgCan1UtilUpload_PlaceHolder(CAN_PROT_UPLOAD_PACK* canUploadPackPtr);

//CAN上传消息处理函数列表
static const USER_CAN_UPLOAD_PROC_UNIT can1MsgUploadProcUtilUnitArray[] = 
{
    //顺序是: 指令码 预期参数最小长度 上传消息处理函数
    {CAN_MASTER_REPORT_ID_HOLD_PLACE     ,2 ,SrvMsgCan1UtilUpload_PlaceHolder},
};

//CAN上传消息处理函数
void SrvMsgCan1UtilUpload(void* can1MsgBodyPtr)
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
    for(indexUtil = 0; indexUtil < DIM_ARRAY_ELEMENT_COUNT(can1MsgUploadProcUtilUnitArray);indexUtil++)
    {
        if(canUploadPackPtr->uploadID == (uint32_t)(can1MsgUploadProcUtilUnitArray[indexUtil].uploadId))
        {
            break;
        }
    }
    //判定序号
    if(indexUtil == DIM_ARRAY_ELEMENT_COUNT(can1MsgUploadProcUtilUnitArray))
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
    if(canUploadPackPtr->uploadDataBufferLength < can1MsgUploadProcUtilUnitArray[indexUtil].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,UploadID: 0X%08X\r\n",__func__,__LINE__,
                        can1MsgUploadProcUtilUnitArray[indexUtil].exceptParamLengthMin,canUploadPackPtr->uploadDataBufferLength,
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
    if(can1MsgUploadProcUtilUnitArray[indexUtil].canUploadProcFuncPtr == NULL)
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
    can1MsgUploadProcUtilUnitArray[indexUtil].canUploadProcFuncPtr(canUploadPackPtr);
    //处理完成,释放内存
    if((canUploadPackPtr->uploadDataBufferPtr != NULL)&&(canUploadPackPtr->uploadDataBufferLength != 0))
    {
        UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackPtr->uploadDataBufferPtr);
    }
    UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackPtr);
}

//占位函数
static void SrvMsgCan1UtilUpload_PlaceHolder(CAN_PROT_UPLOAD_PACK* canUploadPackPtr)
{

}
/*************************************************************************************************************************************/





