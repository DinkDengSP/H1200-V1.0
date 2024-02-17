/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-29 11:51:45
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-02-04 14:15:59
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#include "AppImplMsgProc.h"
#include "AppImplBaseMsgProc.h"
#include "AppTaskHeader.h"
#include "AppTestFlowImplAction.h"

static APP_TESTFLOW_STATUS appStatus;

static uint32_t g_u32TimerHold[TIMER_TYPE_MAX] =
{
	30000,//心跳上报
	
};

//任务全局对象初始化
void AppImplMsgProcVarInit(void)
{

}



//网口指令消息处理函数
extern void AppMsgNet8700MsgProcCmd(void* net8700CmdRecvPtr);
//网口上传消息处理函数
extern void AppMsgNet8700MsgProcUpload(void* net8700ReportDataPtr);
//网口指令消息处理函数
extern void AppMsgNet5500MsgProcCmd(void* net5500MsgBodyPtr, APP_TESTFLOW_STATUS* appStatus);
//网口上传消息处理函数
extern void AppMsgNet5500MsgProcUpload(void* net8700ReportDataPtr);
//CAN1指令消息处理函数
extern void AppMsgCan1MsgProcCmd(void* can1CmdRecvPtr);
//CAN1上传消息处理函数
extern void AppMsgCan1MsgProcUpload(void* can1ReportDataPtr);
//CAN2指令消息处理函数
extern void AppMsgCan2MsgProcCmd(void* can2CmdRecvPtr);
//CAN2上传消息处理函数
extern void AppMsgCan2MsgProcUpload(void* can2ReportDataPtr);
//串口指令消息处理函数
extern void AppMsgUartMsgProcCmd(void* uartCmdRecvPtr);
//串口上传消息处理函数
extern void AppMsgUartMsgProcUpload(void* uartReportDataPtr);
//PE串口指令消息处理函数
extern void AppMsgPEUartMsgProcCmd(void* peUartCmdRecvPtr);
//PE串口上传消息处理函数
extern void AppMsgPEUartMsgProcUpload(void* peUartReportDataPtr);
//IPC指令消息处理函数
extern void AppMsgIpcMsgProcCmd(void* ipcCmdRecvPtr);
//IPC上传消息处理函数
extern void AppMsgIpcMsgProcUpload(void* ipcReportDataPtr);

//任务接收消息处理
void AppImplMsgProcMsgProcess(void* taskMsgPtr)
{
    SYS_MSG* sysMsgPtr = (SYS_MSG*)taskMsgPtr;
    if(sysMsgPtr->msgSrc == SYS_MSG_PACK_SRC_NET_CMD)
    {
        if(sysMsgPtr->msgBodyPtr != NULL)
        {
            //调用网口消息处理函数
            AppMsgNet8700MsgProcCmd(sysMsgPtr->msgBodyPtr);
        }
        else 
        {
            //报警,消息体为空
            SystemPrintf("E: %s Recv SysMsg NetSrc Cmd,BodyPtr NULL!!\r\n",__func__);
        }
    }
    else if(sysMsgPtr->msgSrc == SYS_MSG_PACK_SRC_NET_UPLOAD)
    {
        if(sysMsgPtr->msgBodyPtr != NULL)
        {
            //调用网口消息处理函数
            AppMsgNet8700MsgProcUpload(sysMsgPtr->msgBodyPtr);
        }
        else 
        {
            //报警,消息体为空
            SystemPrintf("E: %s Recv SysMsg NetSrc Upload,BodyPtr NULL!!\r\n",__func__);
        }
    }
    else if(sysMsgPtr->msgSrc == SYS_MSG_PACK_SRC_NET5500_CMD)
    {
        if(sysMsgPtr->msgBodyPtr != NULL)
        {
            //调用W5500消息处理函数
            AppMsgNet5500MsgProcCmd(sysMsgPtr->msgBodyPtr,&appStatus);
        }
        else 
        {
            //报警,消息体为空
            SystemPrintf("E: %s Recv SysMsg W5500 Cmd,BodyPtr NULL!!\r\n",__func__);
        }
    }
    else if(sysMsgPtr->msgSrc == SYS_MSG_PACK_SRC_NET5500_UPLOAD)
    {
        if(sysMsgPtr->msgBodyPtr != NULL)
        {
            //调用W5500消息处理函数
            AppMsgNet5500MsgProcUpload(sysMsgPtr->msgBodyPtr);
        }
        else 
        {
            //报警,消息体为空
            SystemPrintf("E: %s Recv SysMsg W5500 Upload,BodyPtr NULL!!\r\n",__func__);
        }
    }
    else if(sysMsgPtr->msgSrc == SYS_MSG_PACK_SRC_CAN1_CMD)
    {
        if(sysMsgPtr->msgBodyPtr != NULL)
        {
            //调用CAN1消息处理函数
            AppMsgCan1MsgProcCmd(sysMsgPtr->msgBodyPtr);
        }
        else 
        {
            //报警,消息体为空
            SystemPrintf("E: %s Recv SysMsg Can1Src Cmd,BodyPtr NULL!!\r\n",__func__);
        }
    }
    else if(sysMsgPtr->msgSrc == SYS_MSG_PACK_SRC_CAN1_UPLOAD)
    {
        if(sysMsgPtr->msgBodyPtr != NULL)
        {
            //调用CAN1消息处理函数
            AppMsgCan1MsgProcUpload(sysMsgPtr->msgBodyPtr);
        }
        else 
        {
            //报警,消息体为空
            SystemPrintf("E: %s Recv SysMsg Can1Src Upload,BodyPtr NULL!!\r\n",__func__);
        }
    }
    else if(sysMsgPtr->msgSrc == SYS_MSG_PACK_SRC_CAN2_CMD)
    {
        if(sysMsgPtr->msgBodyPtr != NULL)
        {
            //调用CAN2消息处理函数
            AppMsgCan2MsgProcCmd(sysMsgPtr->msgBodyPtr);
        }
        else 
        {
            //报警,消息体为空
            SystemPrintf("E: %s Recv SysMsg Can2Src Cmd,BodyPtr NULL!!\r\n",__func__);
        }
    }
    else if(sysMsgPtr->msgSrc == SYS_MSG_PACK_SRC_CAN2_UPLOAD)
    {
        if(sysMsgPtr->msgBodyPtr != NULL)
        {
            //调用CAN2消息处理函数
            AppMsgCan2MsgProcUpload(sysMsgPtr->msgBodyPtr);
        }
        else 
        {
            //报警,消息体为空
            SystemPrintf("E: %s Recv SysMsg Can2Src Upload,BodyPtr NULL!!\r\n",__func__);
        }
    }
    else if(sysMsgPtr->msgSrc == SYS_MSG_PACK_SRC_UART_CMD)
    {
        if(sysMsgPtr->msgBodyPtr != NULL)
        {
            //调用串口消息处理函数
            AppMsgUartMsgProcCmd(sysMsgPtr->msgBodyPtr);
        }
        else 
        {
            //报警,消息体为空
            SystemPrintf("E: %s Recv SysMsg UartSrc Cmd,BodyPtr NULL!!\r\n",__func__);
        }
    }
    else if(sysMsgPtr->msgSrc == SYS_MSG_PACK_SRC_UART_UPLOAD)
    {
        if(sysMsgPtr->msgBodyPtr != NULL)
        {
            //调用串口消息处理函数
            AppMsgUartMsgProcUpload(sysMsgPtr->msgBodyPtr);
        }
        else 
        {
            //报警,消息体为空
            SystemPrintf("E: %s Recv SysMsg UartSrc Upload,BodyPtr NULL!!\r\n",__func__);
        }
    }
    else if(sysMsgPtr->msgSrc == SYS_MSG_PACK_SRC_PE_UART_CMD)
    {
        if(sysMsgPtr->msgBodyPtr != NULL)
        {
            //调用PE串口消息处理函数
            AppMsgPEUartMsgProcCmd(sysMsgPtr->msgBodyPtr);
        }
        else 
        {
            //报警,消息体为空
            SystemPrintf("E: %s Recv SysMsg PEUartSrc Cmd,BodyPtr NULL!!\r\n",__func__);
        }
    }
    else if(sysMsgPtr->msgSrc == SYS_MSG_PACK_SRC_PE_UART_UPLOAD)
    {
        if(sysMsgPtr->msgBodyPtr != NULL)
        {
            //调用PE串口消息处理函数
            AppMsgPEUartMsgProcUpload(sysMsgPtr->msgBodyPtr);
        }
        else 
        {
            //报警,消息体为空
            SystemPrintf("E: %s Recv SysMsg PEUartSrc Upload,BodyPtr NULL!!\r\n",__func__);
        }
    }
    else if(sysMsgPtr->msgSrc == SYS_MSG_PACK_SRC_IPC_CMD)
    {
        if(sysMsgPtr->msgBodyPtr != NULL)
        {
            //调用IPC指令处理函数
            AppMsgIpcMsgProcCmd(sysMsgPtr->msgBodyPtr);
        }
        else 
        {
            //报警,消息体为空
            SystemPrintf("E: %s Recv SysMsg IpcSrc Cmd,BodyPtr NULL!!\r\n",__func__);
        }
    }
    else if(sysMsgPtr->msgSrc == SYS_MSG_PACK_SRC_IPC_UPLOAD)
    {
        if(sysMsgPtr->msgBodyPtr != NULL)
        {
            //调用IPC上传消息处理函数
            AppMsgIpcMsgProcUpload(sysMsgPtr->msgBodyPtr);
        }
        else 
        {
            //报警,消息体为空
            SystemPrintf("E: %s Recv SysMsg IpcSrc Upload,BodyPtr NULL!!\r\n",__func__);
        }
    }
    else
    {
        //不支持的消息源,报错
        SystemPrintf("E: %s Recv SysMsg Unsupport,Src: %d\r\n",__func__,sysMsgPtr->msgSrc);
    }
    //释放消息体
    UserMemFree(MEM_REGION_SYS_MSG_MAIN,sysMsgPtr);
}

//任务状态机初始化
void AppImplMsgProcFSM_Init(void)
{
    ERROR_SUB error = ERROR_SUB_OK;
    //初始化变量
    AppTestFlowImplAction_InitParameter(&appStatus);
    //初始化复位
    AppTestFlowImplAction_InitModuleAction(&appStatus, &error);
    //启动心跳上报计时器
    appStatus.u32TimerHander[TIMER_TYPE_HEART] = tx_time_get();
}

//任务状态机周期运行
void AppImplMsgProcFSM_RunPeriod(void)
{
    uint8_t u8Result = TestFlowOK;
    uint8_t u8BufferPos = SAMPLE_WAREHOUSE_INVALID;
    uint8_t*  u8TxBuf = NULL;
    uint32_t u32RackID;
    ERROR_SUB error = ERROR_SUB_OK;
    ULONG currentStateCheckTick = tx_time_get();
    if(appStatus.eC21State == INST_C21_STATE_TEST)
    {
        //心跳上报
        if((currentStateCheckTick > appStatus.u32TimerHander[TIMER_TYPE_HEART]) &&
           ((currentStateCheckTick - appStatus.u32TimerHander[TIMER_TYPE_HEART]) > g_u32TimerHold[TIMER_TYPE_HEART]))
        {
            appStatus.u32TimerHander[TIMER_TYPE_HEART] = tx_time_get();
            C21NetReportZetaHeart();
            SystemPrintf("HEART BEAT\r\n");
        }
        
    }
    //判断接口状态是否正常
    if(appStatus.eInterState != INST_INTER_STATE_SAMPLE_STOP &&
       appStatus.eInterState != INST_INTER_STATE_EMRSTOP)
    {
        //判断C21是否正常
        if(appStatus.eC21State == INST_C21_STATE_TEST)
        {
             //判断是否有申请
            if(appStatus.stC21Reques.eRequestState == INST_C21_REQUEST_STATE_RACKIN)
            {
                //进架申请
                u8BufferPos = AppTestFlowImplAction_QueryRackByID(appStatus.stC21Reques.u32RackID,&appStatus);
                SystemPrintf("RacKInReady: %d,buffer:%d,%d\r\n",appStatus.stC21Reques.u32RackID,u8BufferPos,
                             appStatus.stSampleWareHouseState[u8BufferPos].eBufferState);
                if(u8BufferPos != TestFlowOVF && 
                   appStatus.stSampleWareHouseState[u8BufferPos].eBufferState == BUFFER_STATE_LOCKED)
                {
                    //锁定C21样本仓，准备放样本架
                    appStatus.stC21SampleRackState.u32RackID = appStatus.stC21Reques.u32RackID;
                    appStatus.stC21SampleRackState.eBufferState = BUFFER_STATE_LOCKED;
                    AppTestFlowImplAction_MoveRack2C21(u8BufferPos,&appStatus,appStatus.stC21Reques.u32RackID,&error);
                    if(error != ERROR_SUB_OK)
                    {
                        //异常回复
                        u8TxBuf = TestFlowAllocMemory(328);
                        memset(u8TxBuf, 0, 328);
                        UserMemCopy(u8TxBuf,(uint8_t*)&u32RackID,4);
                        UserMemCopy(u8TxBuf + 4,(uint8_t*)&error,4);
                        UserMemCopy(u8TxBuf + 8, (uint8_t *)appStatus.stC21SampleRackState.u32BarCode,320);
                        C21NetOumengInterReport(MACRO_C21_NET_CMD_ZETA_REPORT_RANK_IN_RESULT,u8TxBuf,328);
                        TestFlowFreeMemory(u8TxBuf);
                        if(error == ERROR_SUB_TESTFLOW_RACK_MOVE_SCANBAR_NG)
                        {
                            AppTestFlowImplAction_DefaultRequest(&appStatus);
                        }
                        else
                        {
                            //更新接口错误状态
                            AppTestFlowImplAction_SetInterfaceState(INST_INTER_STATE_SAMPLE_STOP,&appStatus);
                        }
                    }
                    else
                    {
                        AppTestFlowImplAction_DefaultRequest(&appStatus);
                    }
                }
            }
            
            //出架申请
            if(appStatus.stC21Reques.eRequestState == INST_C21_REQUEST_STATE_RACKOUT)
            {
                error = ERROR_SUB_OK;
                u8BufferPos = AppTestFlowImplAction_QueryRackByID(appStatus.stC21Reques.u32RackID,&appStatus);
                SystemPrintf("RacKOutReady: %d,buffer:%d,%d\r\n",appStatus.stC21Reques.u32RackID,u8BufferPos,
                         appStatus.stSampleWareHouseState[u8BufferPos].eBufferState);
                AppTestFlowImplAction_MoveRackFromC21(u8BufferPos,&appStatus,appStatus.stC21Reques.u32RackID, &error);
                
                if(error != ERROR_SUB_OK)
                {
                    //更新接口错误状态
                    AppTestFlowImplAction_SetInterfaceState(INST_INTER_STATE_SAMPLE_STOP,&appStatus);
                }
                u8TxBuf = TestFlowAllocMemory(8);
                memset(u8TxBuf, 0, 8);
                UserMemCopy(u8TxBuf,(uint8_t*)&error,4);
                UserMemCopy(u8TxBuf + 4,(uint8_t*)&appStatus.stC21Reques.u32RackID,4);
                C21NetOumengInterReport(MACRO_C21_NET_CMD_ZETA_REPORT_RANK_IN_RESULT,u8TxBuf,8);
                TestFlowFreeMemory(u8TxBuf);
            }
        }
        
        //判断样本仓的PE交互试管位的架子是否需要抓走
        if(appStatus.stSampleWareHouseState[RACK_PE_OPR_POS].eBufferState == BUFFER_STATE_EXIST)
        {
            //PE有操作架
            //判断是否需要将架子取走
            if(appStatus.stSampleWareHouseState[RACK_PE_OPR_POS].eRackState == RACK_STATE_TEST_WAIT_PE_CATCH)
            {
                //需要取走，判断是否有有位置
                u8BufferPos =  AppTestFlowImplAction_QueryBuffer(RACK_PE_OPR_POS,BUFFER_STATE_IDLE,&appStatus);
                if(u8BufferPos != SAMPLE_WAREHOUSE_INVALID)
                {
                    //找到位置，将架子移走
                    error = ERROR_SUB_OK;
                    appStatus.stSampleWareHouseState[RACK_PE_OPR_POS].eBufferState = BUFFER_STATE_LOCKED;
                    appStatus.stSampleWareHouseState[u8BufferPos].eBufferState = BUFFER_STATE_LOCKED;
                    u8Result = AppTestFlowImplAction_MoveRack2Buffer(RACK_PE_OPR_POS, u8BufferPos, &appStatus, &error);
                    if(u8Result != TestFlowOK)
                    {
                        //更新接口状态
                        AppTestFlowImplAction_SetInterfaceState(INST_INTER_STATE_SAMPLE_STOP,&appStatus);
                        return;
                    }
                    else
                    {
                        appStatus.stSampleWareHouseState[u8BufferPos].eRackState = RACK_STATE_TEST_WAIT_CATCH;
                    }
                }
                else
                {
                    //没找到位置
                }
            }   
        }
        
        //判断样本仓的PE交互试管位是都需要装载架子
        if(appStatus.stSampleWareHouseState[RACK_PE_OPR_POS].eBufferState == BUFFER_STATE_IDLE)
        {
            //装载测试完成的样本架
            u8BufferPos = AppTestFlowImplAction_QueryBufferRackState(RACK_STATE_TESTED, BUFFER_STATE_EXIST, &appStatus);
            if(u8BufferPos != SAMPLE_WAREHOUSE_INVALID)
            {
                //装载样本架
                appStatus.stSampleWareHouseState[RACK_PE_OPR_POS].eBufferState = BUFFER_STATE_LOCKED;
                appStatus.stSampleWareHouseState[u8BufferPos].eBufferState = BUFFER_STATE_LOCKED;
                error = ERROR_SUB_OK;
                u8Result = AppTestFlowImplAction_MoveRack2Buffer(u8BufferPos, RACK_PE_OPR_POS, &appStatus,&error);
                if(u8Result != TestFlowOK)
                {
                    //更新接口状态
                    AppTestFlowImplAction_SetInterfaceState(INST_INTER_STATE_SAMPLE_STOP,&appStatus);
                    return;
                }
                else
                {
                    //appStatus.stSampleWareHouseState[RACK_PE_OPR_POS].eRackState = RACK_STATE_TEST_WAIT_CATCH;
                }
            }
            else
            {
                //装载空闲架子
                u8BufferPos = AppTestFlowImplAction_QueryBufferRackState(RACK_STATE_IDLE, BUFFER_STATE_EXIST, &appStatus);
                if(u8BufferPos != SAMPLE_WAREHOUSE_INVALID)
                {
                    //装载样本架
                    appStatus.stSampleWareHouseState[RACK_PE_OPR_POS].eBufferState = BUFFER_STATE_LOCKED;
                    appStatus.stSampleWareHouseState[u8BufferPos].eBufferState = BUFFER_STATE_LOCKED;
                    error = ERROR_SUB_OK;
                    u8Result = AppTestFlowImplAction_MoveRack2Buffer(u8BufferPos, RACK_PE_OPR_POS, &appStatus,&error);
                    if(u8Result != TestFlowOK)
                    {
                        //更新接口状态
                        AppTestFlowImplAction_SetInterfaceState(INST_INTER_STATE_SAMPLE_STOP,&appStatus);
                        return;
                    }
                    else
                    {
                        appStatus.stSampleWareHouseState[RACK_PE_OPR_POS].eRackState = RACK_STATE_TEST_READY;
                    } 
                }
            }
        }
    }
}














