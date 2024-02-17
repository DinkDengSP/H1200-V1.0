/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-25 08:45:09 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-30 15:10:03 +0800
************************************************************************************************/ 
#include "IPC_SrvPortIn.h"
#include "CommonSrvTaskHeader.h"

//通道携带的数据缓存长度
#define LENGTH_BUFFER_IPC_SRV_PORT_IN             128
//IPC通道讯息
static IPC_CHANNEL_DATA ipcChannelDataSrvPortIn;
//通道名称
static const uint8_t* ipcChannelSrvPortInNameStr = "SrvPortIn";



/*****************************************************通道控制************************************************************/
//初始化IPC通道
void IPC_SrvPortInInit(void)
{
    IPC_BaseInitChannelData(&ipcChannelDataSrvPortIn,(uint8_t*)ipcChannelSrvPortInNameStr,
                                &queueSrvTaskPortIn,LENGTH_BUFFER_IPC_SRV_PORT_IN);
}

//清空IPC信道
ERROR_SUB IPC_SrvPortInClearChannelByCmd(uint32_t cmdCode)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = IPC_BaseChannelClearChannel(&ipcChannelDataSrvPortIn,cmdCode);
    return errorSub;
}

//清空全部信道
ERROR_SUB IPC_SrvPortInClearChannel(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //首先清除通讯测试指令
    errorSub = IPC_BaseChannelClearChannel(&ipcChannelDataSrvPortIn,IPC_CMD_MACRO_COMM_TEST);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,IPC_CMD_MACRO_COMM_TEST,errorSub);
        return errorSub;
    }
    //通道内部全部指令清空
    uint32_t cmdCode = 0X00000000;
    for(cmdCode = IPC_CMD_MACRO_PORT_IN_READ_SINGLE;cmdCode <= IPC_CMD_MACRO_PORT_IN_READ_ANY;cmdCode++)
    {
        errorSub = IPC_BaseChannelClearChannel(&ipcChannelDataSrvPortIn,cmdCode);
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
void IPC_SrvPortInWaitCmdReturn(uint32_t cmdCode,SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //先赋初值
    if(returnDataPtr != NULL)
    {
        returnDataPtr->returnBufferPtr = NULL;
        returnDataPtr->returnDataLength = 0;
    }
    //等待指令返回
    errorSub = IPC_BaseWaitCmdReturn(&ipcChannelDataSrvPortIn,cmdCode,resultPackPtr);
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
    if((ipcChannelDataSrvPortIn.channelCache.returnDataBufferLength != 0)&&(returnDataPtr != NULL))
    {
        //申请内存
        returnDataPtr->returnDataLength = ipcChannelDataSrvPortIn.channelCache.returnDataBufferLength;
        //不为0拷贝数据
        if(returnDataPtr->returnDataLength != 0)
        {
            returnDataPtr->returnBufferPtr = IPC_PortMemMallocWhileSuccess(returnDataPtr->returnDataLength);
            //数据拷贝
            UserMemCopy(returnDataPtr->returnBufferPtr,ipcChannelDataSrvPortIn.channelCache.dataBufferPtr,returnDataPtr->returnDataLength);
        }
    }
    return;
}

//等待上一次发送的指令结果返回
void IPC_SrvPortInWaitLastCmdReturn(SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //先赋初值
    if(returnDataPtr != NULL)
    {
        returnDataPtr->returnBufferPtr = NULL;
        returnDataPtr->returnDataLength = 0;
    }
    //等待指令返回
    errorSub = IPC_BaseWaitLastCmdReturn(&ipcChannelDataSrvPortIn,resultPackPtr);
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
    if((ipcChannelDataSrvPortIn.channelCache.returnDataBufferLength != 0)&&(returnDataPtr != NULL))
    {
        //申请内存
        returnDataPtr->returnDataLength = ipcChannelDataSrvPortIn.channelCache.returnDataBufferLength;
        //不为0拷贝数据
        if(returnDataPtr->returnDataLength != 0)
        {
            returnDataPtr->returnBufferPtr = IPC_PortMemMallocWhileSuccess(returnDataPtr->returnDataLength);
            //数据拷贝
            UserMemCopy(returnDataPtr->returnBufferPtr,ipcChannelDataSrvPortIn.channelCache.dataBufferPtr,returnDataPtr->returnDataLength);
        }
    }
    return;
}


/*****************************************************查询指令结果-通用************************************************************/
//查询指定指令的状态
void IPC_SrvPortInQueryCmdState(uint32_t cmdCode,IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                    IPC_RETURN_DATA* returnDataPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //先赋初值
    if(returnDataPtr != NULL)
    {
        returnDataPtr->returnBufferPtr = NULL;
        returnDataPtr->returnDataLength = 0;
    }
    errorSub = IPC_BaseQueryCmdState(&ipcChannelDataSrvPortIn,cmdCode,cmdStagePtr,resultPackPtr);
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
            returnDataPtr->returnDataLength = ipcChannelDataSrvPortIn.channelCache.returnDataBufferLength;
            //不为0拷贝数
            if(returnDataPtr->returnDataLength != 0)
            {
                returnDataPtr->returnBufferPtr = IPC_PortMemMallocWhileSuccess(returnDataPtr->returnDataLength);
                //数据拷贝
                UserMemCopy(returnDataPtr->returnBufferPtr,ipcChannelDataSrvPortIn.channelCache.dataBufferPtr,returnDataPtr->returnDataLength);
            }
        }
        return;
    }
    //设置无错误返回
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//查询上一次发送的指令的状态
void IPC_SrvPortInQueryLastCmdState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //先赋初值
    if(returnDataPtr != NULL)
    {
        returnDataPtr->returnBufferPtr = NULL;
        returnDataPtr->returnDataLength = 0;
    }
    errorSub = IPC_BaseQueryLastCmdState(&ipcChannelDataSrvPortIn,cmdStagePtr,resultPackPtr);
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
            returnDataPtr->returnDataLength = ipcChannelDataSrvPortIn.channelCache.returnDataBufferLength;
            //不为0拷贝数据
            if(returnDataPtr->returnDataLength != 0)
            {
                returnDataPtr->returnBufferPtr = IPC_PortMemMallocWhileSuccess(returnDataPtr->returnDataLength);
                //数据拷贝
                UserMemCopy(returnDataPtr->returnBufferPtr,ipcChannelDataSrvPortIn.channelCache.dataBufferPtr,returnDataPtr->returnDataLength);
            }
        }
        return;
    }
    //设置无错误返回
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}


/*****************************************************等待指令返回-特用************************************************************/
//通讯测试,WAIT
void IPC_SrvPortInCommTestWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = IPC_BaseWaitCmdReturn(&ipcChannelDataSrvPortIn,IPC_CMD_MACRO_COMM_TEST,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,IPC_CMD_MACRO_COMM_TEST,errorSub);
        //IPC报错,返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CMD_WAIT_RESULT,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    return;
}

//单个读取,WAIT
void IPC_SrvPortInReadSingleWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,GPIO_PinState* pinStatePtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    if(pinStatePtr != NULL)
    {
        *pinStatePtr = GPIO_PIN_RESET;
    }
    errorSub = IPC_BaseWaitCmdReturn(&ipcChannelDataSrvPortIn,IPC_CMD_MACRO_PORT_IN_READ_SINGLE,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,IPC_CMD_MACRO_PORT_IN_READ_SINGLE,errorSub);
        //IPC报错,返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CMD_WAIT_RESULT,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    if(resultPackPtr->resultCode == PROT_RESULT_SUCCESS)
    {
        //结果解析
        if(pinStatePtr != NULL)
        {
            *pinStatePtr = (GPIO_PinState)(ipcChannelDataSrvPortIn.channelCache.dataBufferPtr[0]);
        }   
    }
    return;
}

//区间读取,WAIT
void IPC_SrvPortInReadRangeWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,GPIO_PinState** pinStateArrayPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    if(pinStateArrayPtr != NULL)
    {
        //首先设置返回数据为空
        *pinStateArrayPtr = NULL;
    }
    //等待返回
    errorSub = IPC_BaseWaitCmdReturn(&ipcChannelDataSrvPortIn,IPC_CMD_MACRO_PORT_IN_READ_RANGE,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,IPC_CMD_MACRO_PORT_IN_READ_RANGE,errorSub);
        //IPC报错,返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CMD_WAIT_RESULT,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    if(resultPackPtr->resultCode == PROT_RESULT_SUCCESS)
    {
        if(pinStateArrayPtr != NULL)
        {
            //结果解析
            *pinStateArrayPtr = IPC_PortMemMallocWhileSuccess(ipcChannelDataSrvPortIn.channelCache.returnDataBufferLength);
            //数据拷贝
            UserMemCopy((void*)(*pinStateArrayPtr),ipcChannelDataSrvPortIn.channelCache.dataBufferPtr,
                            ipcChannelDataSrvPortIn.channelCache.returnDataBufferLength);
        }
    }
    return;
}

//批量读取,WAIT
void IPC_SrvPortInReadAnyWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,GPIO_PinState** pinStateArrayPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //首先设置返回数据为空
    if(pinStateArrayPtr != NULL)
    {
        //首先设置返回数据为空
        *pinStateArrayPtr = NULL;
    }
    //等待返回
    errorSub = IPC_BaseWaitCmdReturn(&ipcChannelDataSrvPortIn,IPC_CMD_MACRO_PORT_IN_READ_ANY,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,IPC_CMD_MACRO_PORT_IN_READ_ANY,errorSub);
        //IPC报错,返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CMD_WAIT_RESULT,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    if(resultPackPtr->resultCode == PROT_RESULT_SUCCESS)
    {
        //结果解析
        if(pinStateArrayPtr != NULL)
        {
            //结果解析
            *pinStateArrayPtr = IPC_PortMemMallocWhileSuccess(ipcChannelDataSrvPortIn.channelCache.returnDataBufferLength);
            //数据拷贝
            UserMemCopy((void*)(*pinStateArrayPtr),ipcChannelDataSrvPortIn.channelCache.dataBufferPtr,
                            ipcChannelDataSrvPortIn.channelCache.returnDataBufferLength);
        }
    }
    return;
}


/*****************************************************查询指令结果-特用************************************************************/
//通讯测试,QUERY
void IPC_SrvPortInQueryCommTestState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = IPC_BaseQueryCmdState(&ipcChannelDataSrvPortIn,IPC_CMD_MACRO_COMM_TEST,cmdStagePtr,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,IPC_CMD_MACRO_COMM_TEST,errorSub);
        //IPC报错,返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_QUERY_CMD_STATE,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    return;
}

//单个读取,QUERY
void IPC_SrvPortInQueryReadSingleState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,GPIO_PinState* pinStatePtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    if(pinStatePtr != NULL)
    {
        *pinStatePtr = GPIO_PIN_RESET;
    }
    errorSub = IPC_BaseQueryCmdState(&ipcChannelDataSrvPortIn,IPC_CMD_MACRO_PORT_IN_READ_SINGLE,cmdStagePtr,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,IPC_CMD_MACRO_PORT_IN_READ_SINGLE,errorSub);
        //IPC报错,返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_QUERY_CMD_STATE,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    if((*cmdStagePtr == IPC_CMD_STAGE_OVER)&&(resultPackPtr->resultCode == PROT_RESULT_SUCCESS))
    {
        //执行完成,无错误,结果解析
        if(pinStatePtr != NULL)
        {
            *pinStatePtr = (GPIO_PinState)(ipcChannelDataSrvPortIn.channelCache.dataBufferPtr[0]);
        } 
    }
    return;
}

//区间读取,QUERY
void IPC_SrvPortInQueryReadRangeState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,GPIO_PinState** pinStateArrayPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    if(pinStateArrayPtr != NULL)
    {
        //首先设置返回数据为空
        *pinStateArrayPtr = NULL;
    }
    errorSub = IPC_BaseQueryCmdState(&ipcChannelDataSrvPortIn,IPC_CMD_MACRO_PORT_IN_READ_RANGE,cmdStagePtr,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,IPC_CMD_MACRO_PORT_IN_READ_RANGE,errorSub);
        //IPC报错,返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_QUERY_CMD_STATE,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    if((*cmdStagePtr == IPC_CMD_STAGE_OVER)&&(resultPackPtr->resultCode == PROT_RESULT_SUCCESS))
    {
        if(pinStateArrayPtr != NULL)
        {
            //结果解析
            *pinStateArrayPtr = IPC_PortMemMallocWhileSuccess(ipcChannelDataSrvPortIn.channelCache.returnDataBufferLength);
            //数据拷贝
            UserMemCopy((void*)(*pinStateArrayPtr),ipcChannelDataSrvPortIn.channelCache.dataBufferPtr,
                            ipcChannelDataSrvPortIn.channelCache.returnDataBufferLength);
        }
    }
    return;
}

//批量读取,QUERY
void IPC_SrvPortInQueryReadAnyState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,GPIO_PinState** pinStateArrayPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //首先设置返回数据为空
    if(pinStateArrayPtr != NULL)
    {
        //首先设置返回数据为空
        *pinStateArrayPtr = NULL;
    }
    errorSub = IPC_BaseQueryCmdState(&ipcChannelDataSrvPortIn,IPC_CMD_MACRO_PORT_IN_READ_ANY,cmdStagePtr,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,IPC_CMD_MACRO_PORT_IN_READ_ANY,errorSub);
        //IPC报错,返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_QUERY_CMD_STATE,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    if((*cmdStagePtr == IPC_CMD_STAGE_OVER)&&(resultPackPtr->resultCode == PROT_RESULT_SUCCESS))
    {
        if(pinStateArrayPtr != NULL)
        {
            //结果解析
            *pinStateArrayPtr = IPC_PortMemMallocWhileSuccess(ipcChannelDataSrvPortIn.channelCache.returnDataBufferLength);
            //数据拷贝
            UserMemCopy((void*)(*pinStateArrayPtr),ipcChannelDataSrvPortIn.channelCache.dataBufferPtr,
                            ipcChannelDataSrvPortIn.channelCache.returnDataBufferLength);
        }
    }
    return;
}


/*****************************************************发送指令等待ACK************************************************************/
static ERROR_SUB IPC_SrvPortInCheckClearLastCmd(uint32_t cmdCode)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    PROT_CH_STA channelState;
    //首先检查是否上个数据未完成或者完成之后未读取
    channelState = IPC_BaseCheckCmdChannelState(&ipcChannelDataSrvPortIn,cmdCode);
    if(channelState == PROT_CH_STA_BUSY)
    {
        //清除上一次的指令
        errorSub = IPC_BaseChannelClearChannel(&ipcChannelDataSrvPortIn,cmdCode);
        if(errorSub != ERROR_SUB_OK)
        {
            SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,cmdCode,errorSub);
            return errorSub;
        }
    }
    return ERROR_SUB_OK;
}

//通讯测试,ACK
void IPC_SrvPortInCommTestWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = IPC_SrvPortInCheckClearLastCmd(IPC_CMD_MACRO_COMM_TEST);
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
    errorSub = IPC_BaseSendCmdWhileAck(&ipcChannelDataSrvPortIn,&ipcCmdBaseData);
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

//单个读取,ACK
void IPC_SrvPortInReadSingleWhileAck(uint8_t readSingleIndex,SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = IPC_SrvPortInCheckClearLastCmd(IPC_CMD_MACRO_PORT_IN_READ_SINGLE);
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
    ipcCmdBaseData.commandCode = IPC_CMD_MACRO_PORT_IN_READ_SINGLE;
    ipcCmdBaseData.cmdParam1 = readSingleIndex;
    //清除掉上一次的指令了,进行下一次指令发送
    errorSub = IPC_BaseSendCmdWhileAck(&ipcChannelDataSrvPortIn,&ipcCmdBaseData);
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

//区间读取,ACK
void IPC_SrvPortInReadRangeWhileAck(uint8_t readSingleStartIdx,uint8_t readSingleCount,SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = IPC_SrvPortInCheckClearLastCmd(IPC_CMD_MACRO_PORT_IN_READ_RANGE);
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
    ipcCmdBaseData.commandCode = IPC_CMD_MACRO_PORT_IN_READ_RANGE;
    ipcCmdBaseData.cmdParam1 = readSingleStartIdx;
    ipcCmdBaseData.cmdParam2 = readSingleCount;
    //清除掉上一次的指令了,进行下一次指令发送
    errorSub = IPC_BaseSendCmdWhileAck(&ipcChannelDataSrvPortIn,&ipcCmdBaseData);
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

//批量读取,ACK
void IPC_SrvPortInReadAnyWhileAck(uint8_t readSingleCount,uint8_t* pinIndexArray,SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = IPC_SrvPortInCheckClearLastCmd(IPC_CMD_MACRO_PORT_IN_READ_ANY);
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
    ipcCmdBaseData.commandCode = IPC_CMD_MACRO_PORT_IN_READ_ANY;
    //参数设置
    ipcCmdBaseData.cmdParam1 = readSingleCount;
    ipcCmdBaseData.cmdParamBufferPtr = IPC_PortMemMallocWhileSuccess(readSingleCount);
    ipcCmdBaseData.cmdParamBufferLength = readSingleCount;
    UserMemCopy(ipcCmdBaseData.cmdParamBufferPtr,(void*)pinIndexArray,readSingleCount);
    //清除掉上一次的指令了,进行下一次指令发送
    errorSub = IPC_BaseSendCmdWhileAck(&ipcChannelDataSrvPortIn,&ipcCmdBaseData);
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
void IPC_SrvPortInCommTestWhileReturn(SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //清除上一个指令
    errorSub = IPC_SrvPortInCheckClearLastCmd(IPC_CMD_MACRO_COMM_TEST);
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
    errorSub = IPC_BaseSendCmdWhileReturn(&ipcChannelDataSrvPortIn,&ipcCmdBaseData,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,ipcCmdBaseData.commandCode,
                        errorSub);
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CMD_WAIT_RESULT,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    return;
}

//单个读取,RETURN
void IPC_SrvPortInReadSingleWhileReturn(uint8_t readSingleIndex,SYS_RESULT_PACK* resultPackPtr,GPIO_PinState* pinStatePtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    if(pinStatePtr != NULL)
    {
        *pinStatePtr = GPIO_PIN_RESET;
    }
    errorSub = IPC_SrvPortInCheckClearLastCmd(IPC_CMD_MACRO_PORT_IN_READ_SINGLE);
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
    ipcCmdBaseData.commandCode = IPC_CMD_MACRO_PORT_IN_READ_SINGLE;
    //设置参数
    ipcCmdBaseData.cmdParam1 = readSingleIndex;
    //清除掉上一次的指令了,进行下一次指令发送
    errorSub = IPC_BaseSendCmdWhileReturn(&ipcChannelDataSrvPortIn,&ipcCmdBaseData,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,ipcCmdBaseData.commandCode,
                        errorSub);
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CMD_WAIT_RESULT,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    if(resultPackPtr->resultCode == PROT_RESULT_SUCCESS)
    {
        //执行完成,无错误,结果解析
        if(pinStatePtr != NULL)
        {
            *pinStatePtr = (GPIO_PinState)(ipcChannelDataSrvPortIn.channelCache.dataBufferPtr[0]);
        } 
    }
    return;
}

//区间读取,RETURN
void IPC_SrvPortInReadRangeWhileReturn(uint8_t readSingleStartIdx,uint8_t readSingleCount,SYS_RESULT_PACK* resultPackPtr,
                                                GPIO_PinState** pinStateArrayPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    if(pinStateArrayPtr != NULL)
    {
        //首先设置返回数据为空
        *pinStateArrayPtr = NULL;
    }
    errorSub = IPC_SrvPortInCheckClearLastCmd(IPC_CMD_MACRO_PORT_IN_READ_RANGE);
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
    ipcCmdBaseData.commandCode = IPC_CMD_MACRO_PORT_IN_READ_RANGE;
    //设置参数
    ipcCmdBaseData.cmdParam1 = readSingleStartIdx;
    ipcCmdBaseData.cmdParam2 = readSingleCount;
    //清除掉上一次的指令了,进行下一次指令发送
    errorSub = IPC_BaseSendCmdWhileReturn(&ipcChannelDataSrvPortIn,&ipcCmdBaseData,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,ipcCmdBaseData.commandCode,
                        errorSub);
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CMD_WAIT_RESULT,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    if(resultPackPtr->resultCode == PROT_RESULT_SUCCESS)
    {
        if(pinStateArrayPtr != NULL)
        {
            //结果解析
            *pinStateArrayPtr = IPC_PortMemMallocWhileSuccess(ipcChannelDataSrvPortIn.channelCache.returnDataBufferLength);
            //数据拷贝
            UserMemCopy((void*)(*pinStateArrayPtr),ipcChannelDataSrvPortIn.channelCache.dataBufferPtr,
                            ipcChannelDataSrvPortIn.channelCache.returnDataBufferLength);
        }
    }
    return;
}

//批量读取,RETURN
void IPC_SrvPortInReadAnyWhileReturn(uint8_t readSingleCount,uint8_t* pinIndexArray,SYS_RESULT_PACK* resultPackPtr,
                                            GPIO_PinState** pinStateArrayPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //首先设置返回数据为空
    if(pinStateArrayPtr != NULL)
    {
        //首先设置返回数据为空
        *pinStateArrayPtr = NULL;
    }
    errorSub = IPC_SrvPortInCheckClearLastCmd(IPC_CMD_MACRO_PORT_IN_READ_ANY);
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
    ipcCmdBaseData.commandCode = IPC_CMD_MACRO_PORT_IN_READ_ANY;
    //设置参数
    ipcCmdBaseData.cmdParam1 = readSingleCount;
    ipcCmdBaseData.cmdParamBufferPtr = IPC_PortMemMallocWhileSuccess(readSingleCount);
    ipcCmdBaseData.cmdParamBufferLength = readSingleCount;
    UserMemCopy(ipcCmdBaseData.cmdParamBufferPtr,(void*)pinIndexArray,readSingleCount);
    //清除掉上一次的指令了,进行下一次指令发送
    errorSub = IPC_BaseSendCmdWhileReturn(&ipcChannelDataSrvPortIn,&ipcCmdBaseData,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,ipcCmdBaseData.commandCode,
                        errorSub);
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CMD_WAIT_RESULT,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    if(resultPackPtr->resultCode == PROT_RESULT_SUCCESS)
    {
        if(pinStateArrayPtr != NULL)
        {
            //结果解析
            *pinStateArrayPtr = IPC_PortMemMallocWhileSuccess(ipcChannelDataSrvPortIn.channelCache.returnDataBufferLength);
            //数据拷贝
            UserMemCopy((void*)(*pinStateArrayPtr),ipcChannelDataSrvPortIn.channelCache.dataBufferPtr,
                            ipcChannelDataSrvPortIn.channelCache.returnDataBufferLength);
        }
    }
    return;
}


















