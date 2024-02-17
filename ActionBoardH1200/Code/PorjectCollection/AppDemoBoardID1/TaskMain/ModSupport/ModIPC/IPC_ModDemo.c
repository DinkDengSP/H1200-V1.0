/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-25 10:44:02 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-09 21:00:01 +0800
************************************************************************************************/ 
#include "IPC_ModDemo.h"
#include "ModTaskHeader.h"
#include "IPC_Port.h"

//通道携带的数据缓存长度
#define LENGTH_BUFFER_IPC_MOD_DEMO             128
//IPC通道讯息
static IPC_CHANNEL_DATA ipcChannelDataModDemo;
//通道名称
static const uint8_t* ipcChannelModDemoNameStr = "ModDemo";

/*****************************************************通道控制************************************************************/
//初始化IPC通道
void IPC_ModDemoInit(void)
{
    IPC_BaseInitChannelData(&ipcChannelDataModDemo,(uint8_t*)ipcChannelModDemoNameStr,
                                &queueModTaskDemo,LENGTH_BUFFER_IPC_MOD_DEMO);
}

//清空IPC信道
ERROR_SUB IPC_ModDemoClearChannelByCmd(uint32_t cmdCode)
{
    return IPC_BaseChannelClearChannel(&ipcChannelDataModDemo,cmdCode);
}

//清空全部信道
ERROR_SUB IPC_ModDemoClearChannel(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //首先清除通讯测试指令
    errorSub = IPC_BaseChannelClearChannel(&ipcChannelDataModDemo,IPC_CMD_MACRO_COMM_TEST);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,IPC_CMD_MACRO_COMM_TEST,errorSub);
        return errorSub;
    }
    //通道内部全部指令清空
    uint32_t cmdCode = 0X00000000;
    for(cmdCode = IPC_CMD_MACRO_COMM_TEST;cmdCode < IPC_CMD_MACRO_COMM_TEST;cmdCode++)
    {
        errorSub = IPC_BaseChannelClearChannel(&ipcChannelDataModDemo,cmdCode);
        if(errorSub != ERROR_SUB_OK)
        {
            SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,cmdCode,errorSub);
            return errorSub;
        }
    }
    return ERROR_SUB_OK;
}

/*****************************************************等待指令返回************************************************************/
//等待指定指令结果返回
void IPC_ModDemoWaitCmdReturn(uint32_t cmdCode,SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //先赋初值
    if(returnDataPtr != NULL)
    {
        returnDataPtr->returnBufferPtr = NULL;
        returnDataPtr->returnDataLength = 0;
    }
    //等待指令返回
    errorSub = IPC_BaseWaitCmdReturn(&ipcChannelDataModDemo,cmdCode,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,cmdCode,errorSub);
        //IPC报错,返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CMD_WAIT_RESULT,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    if(resultPackPtr->resultCode != PROT_RESULT_SUCCESS)
    {
        //结果失败,数据不要
        return;
    }
    //结果是否存在数据,如果存在数据,需要申请内存将数据返回
    if((ipcChannelDataModDemo.channelCache.returnDataBufferLength != 0)&&(returnDataPtr != NULL))
    {
        //申请内存
        returnDataPtr->returnDataLength = ipcChannelDataModDemo.channelCache.returnDataBufferLength;
        //不为0拷贝数据
        if(returnDataPtr->returnDataLength != 0)
        {
            returnDataPtr->returnBufferPtr = IPC_PortMemMallocWhileSuccess(returnDataPtr->returnDataLength);
            //数据拷贝
            UserMemCopy(returnDataPtr->returnBufferPtr,ipcChannelDataModDemo.channelCache.dataBufferPtr,returnDataPtr->returnDataLength);
        }
    }
    return;
}

//等待上一次发送的指令结果返回
void IPC_ModDemoWaitLastCmdReturn(SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //先赋初值
    if(returnDataPtr != NULL)
    {
        returnDataPtr->returnBufferPtr = NULL;
        returnDataPtr->returnDataLength = 0;
    }
    //等待指令返回
    errorSub = IPC_BaseWaitLastCmdReturn(&ipcChannelDataModDemo,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,ErrorSub: 0X%08X\r\n",__func__,errorSub);
        //IPC报错,返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CMD_WAIT_RESULT,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    if(resultPackPtr->resultCode != PROT_RESULT_SUCCESS)
    {
        //结果失败,数据不要
        return;
    }
    //结果是否存在数据,如果存在数据,需要申请内存将数据返回
    if((ipcChannelDataModDemo.channelCache.returnDataBufferLength != 0)&&(returnDataPtr != NULL))
    {
        //申请内存
        returnDataPtr->returnDataLength = ipcChannelDataModDemo.channelCache.returnDataBufferLength;
        //不为0拷贝数据
        if(returnDataPtr->returnDataLength != 0)
        {
            returnDataPtr->returnBufferPtr = IPC_PortMemMallocWhileSuccess(returnDataPtr->returnDataLength);
            //数据拷贝
            UserMemCopy(returnDataPtr->returnBufferPtr,ipcChannelDataModDemo.channelCache.dataBufferPtr,returnDataPtr->returnDataLength);
        }
    }
    return;
}

/*****************************************************查询指令结果-通用************************************************************/
//查询指定指令的状态
void IPC_ModDemoQueryCmdState(uint32_t cmdCode,IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                        IPC_RETURN_DATA* returnDataPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //先赋初值
    if(returnDataPtr != NULL)
    {
        returnDataPtr->returnBufferPtr = NULL;
        returnDataPtr->returnDataLength = 0;
    }
    errorSub = IPC_BaseQueryCmdState(&ipcChannelDataModDemo,cmdCode,cmdStagePtr,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,cmdCode,errorSub);
        //IPC报错,返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_QUERY_CMD_STATE,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    if(*cmdStagePtr == IPC_CMD_STAGE_OVER)
    {
        if(resultPackPtr->resultCode != PROT_RESULT_SUCCESS)
        {
            //结果失败,数据不要
            return;
        }
        if(returnDataPtr != NULL)
        {
            //申请内存
            returnDataPtr->returnDataLength = ipcChannelDataModDemo.channelCache.returnDataBufferLength;
            //不为0拷贝数
            if(returnDataPtr->returnDataLength != 0)
            {
                returnDataPtr->returnBufferPtr = IPC_PortMemMallocWhileSuccess(returnDataPtr->returnDataLength);
                //数据拷贝
                UserMemCopy(returnDataPtr->returnBufferPtr,ipcChannelDataModDemo.channelCache.dataBufferPtr,returnDataPtr->returnDataLength);
            }
        }
        return;
    }
    //设置无错误返回
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//查询上一次发送的指令的状态
void IPC_ModDemoQueryLastCmdState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //先赋初值
    if(returnDataPtr != NULL)
    {
        returnDataPtr->returnBufferPtr = NULL;
        returnDataPtr->returnDataLength = 0;
    }
    errorSub = IPC_BaseQueryLastCmdState(&ipcChannelDataModDemo,cmdStagePtr,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,ErrorSub: 0X%08X\r\n",__func__,errorSub);
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_QUERY_CMD_STATE,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    if(*cmdStagePtr == IPC_CMD_STAGE_OVER)
    {
        if(resultPackPtr->resultCode != PROT_RESULT_SUCCESS)
        {
            //结果失败,数据不要
            return;
        }
        if(returnDataPtr != NULL)
        {
            //申请内存
            returnDataPtr->returnDataLength = ipcChannelDataModDemo.channelCache.returnDataBufferLength;
            //不为0拷贝数据
            if(returnDataPtr->returnDataLength != 0)
            {
                returnDataPtr->returnBufferPtr = IPC_PortMemMallocWhileSuccess(returnDataPtr->returnDataLength);
                //数据拷贝
                UserMemCopy(returnDataPtr->returnBufferPtr,ipcChannelDataModDemo.channelCache.dataBufferPtr,returnDataPtr->returnDataLength);
            }
        }
        return;
    }
    //设置无错误返回
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

/*****************************************************发送指令等待ACK************************************************************/
static ERROR_SUB IPC_ModDemoCheckClearLastCmd(uint32_t cmdCode)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    PROT_CH_STA channelState;
    //首先检查是否上个数据未完成或者完成之后未读取
    channelState = IPC_BaseCheckCmdChannelState(&ipcChannelDataModDemo,cmdCode);
    if(channelState == PROT_CH_STA_BUSY)
    {
        //清除上一次的指令
        errorSub = IPC_BaseChannelClearChannel(&ipcChannelDataModDemo,cmdCode);
        if(errorSub != ERROR_SUB_OK)
        {
            SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,cmdCode,errorSub);
            return errorSub;
        }
    }
    return ERROR_SUB_OK;
}

//通讯测试,ACK
void IPC_ModDemoCommTestWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //清除上一个指令
    errorSub = IPC_ModDemoCheckClearLastCmd(IPC_CMD_MACRO_COMM_TEST);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CLEAR_LAST_CMD,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    //设置参数
    IPC_CMD_BASE_DATA ipcCmdBaseData;
    //先设置为默认值
    IPC_BaseSetCmdPackDefault(&ipcCmdBaseData);
    //然后写入
    ipcCmdBaseData.commandCode = IPC_CMD_MACRO_COMM_TEST;
    //清除掉上一次的指令了,进行下一次指令发送
    errorSub = IPC_BaseSendCmdWhileAck(&ipcChannelDataModDemo,&ipcCmdBaseData);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,ipcCmdBaseData.commandCode,
                        errorSub);
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CMD_WAIT_ACK,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    //设置无错误返回
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

/*****************************************************发送指令等待返回************************************************************/
//通讯测试,RETURN
void IPC_ModDemoCommTestWhileReturn(SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    if(returnDataPtr != NULL)
    {
        returnDataPtr->returnBufferPtr = NULL;
        returnDataPtr->returnDataLength = 0;
    }
    //清除上一个指令
    errorSub = IPC_ModDemoCheckClearLastCmd(IPC_CMD_MACRO_COMM_TEST);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CLEAR_LAST_CMD,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    //设置参数
    IPC_CMD_BASE_DATA ipcCmdBaseData;
    //先设置为默认值
    IPC_BaseSetCmdPackDefault(&ipcCmdBaseData);
    //然后写入
    ipcCmdBaseData.commandCode = IPC_CMD_MACRO_COMM_TEST;
    //清除掉上一次的指令了,进行下一次指令发送
    errorSub = IPC_BaseSendCmdWhileReturn(&ipcChannelDataModDemo,&ipcCmdBaseData,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,ipcCmdBaseData.commandCode,
                        errorSub);
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CMD_WAIT_RESULT,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    if((resultPackPtr->resultCode == PROT_RESULT_SUCCESS)&&(returnDataPtr != NULL))
    {
        //正常完成
        returnDataPtr->returnDataLength = ipcChannelDataModDemo.channelCache.returnDataBufferLength;
        //不为0拷贝数据
        if(returnDataPtr->returnDataLength != 0)
        {
            returnDataPtr->returnBufferPtr = IPC_PortMemMallocWhileSuccess(returnDataPtr->returnDataLength);
            //数据拷贝
            UserMemCopy(returnDataPtr->returnBufferPtr,ipcChannelDataModDemo.channelCache.dataBufferPtr,returnDataPtr->returnDataLength);
        }
    }
    return;
}
