/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-27 17:08:07 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-10 16:41:16 +0800
************************************************************************************************/ 
#include "AppImplUtil.h"
#include "AppTaskHeader.h"


//任务全局对象初始化
void AppImplUtilVarInit(void)
{
    
}


//CAN1指令消息处理函数
extern void AppMsgCan1UtilCmd(void* can1CmdRecvPtr);
//CAN1上传消息处理函数
extern void AppMsgCan1UtilUpload(void* can1ReportDataPtr);
//IPC指令消息处理函数
extern void AppMsgIpcUtilCmd(void* ipcCmdRecvPtr);
//IPC上传消息处理函数
extern void AppMsgIpcUtilUpload(void* ipcReportDataPtr);



//任务接收消息处理
void AppImplUtilMsgProcess(void* taskMsgPtr)
{
    SYS_MSG* sysMsgPtr = (SYS_MSG*)taskMsgPtr;
    if(sysMsgPtr->msgSrc == SYS_MSG_PACK_SRC_CAN1_CMD)
    {
        if(sysMsgPtr->msgBodyPtr != NULL)
        {
            //调用CAN1消息处理函数
            AppMsgCan1UtilCmd(sysMsgPtr->msgBodyPtr);
        }
        else 
        {
            //报警,消息体为空
            SystemPrintf("E: %s Recv SysMsg Can1Src Upload,BodyPtr NULL!!\r\n",__func__);
        }
    }
    else if(sysMsgPtr->msgSrc == SYS_MSG_PACK_SRC_CAN1_UPLOAD)
    {
        if(sysMsgPtr->msgBodyPtr != NULL)
        {
            //调用CAN1消息处理函数
            AppMsgCan1UtilUpload(sysMsgPtr->msgBodyPtr);
        }
        else 
        {
            //报警,消息体为空
            SystemPrintf("E: %s Recv SysMsg Can1Src Upload,BodyPtr NULL!!\r\n",__func__);
        }
    }
    else if(sysMsgPtr->msgSrc == SYS_MSG_PACK_SRC_IPC_CMD)
    {
        if(sysMsgPtr->msgBodyPtr != NULL)
        {
            //调用IPC消息处理函数
            AppMsgIpcUtilCmd(sysMsgPtr->msgBodyPtr);
        }
        else 
        {
            //报警,消息体为空
            SystemPrintf("E: %s Recv SysMsg IpcSrc Upload,BodyPtr NULL!!\r\n",__func__);
        }
    }
    else if(sysMsgPtr->msgSrc == SYS_MSG_PACK_SRC_IPC_UPLOAD)
    {
        if(sysMsgPtr->msgBodyPtr != NULL)
        {
            //调用IPC消息处理函数
            AppMsgIpcUtilUpload(sysMsgPtr->msgBodyPtr);
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
void AppImplUtilFSM_Init(void)
{
    
}


void AppImplUtilFSM_RunPeriodIPC(void)
{
    uint16_t indexUtilMain = 0;
    uint16_t indexUtilMotor = 0;
    SYS_RESULT_PACK resultPack;
    IPC_STEP_MOTOR_CMD_RESET stepMotorResetCmd;
    IPC_STEP_MOTOR_CMD_RUN_STEP stepMotorRunStepCmd;
    IPC_STEP_MOTOR_CMD_RUN_COORDINATE stepMotorRunCoordinateCmd;
    STEP_MOTOR_NO motorNo = STEP_MOTOR_NO1;
    IPC_RETURN_DATA ipcReturnData;
    IPC_CMD_STAGE cmdStage;
    //复位等待ACK
    for(indexUtilMotor = STEP_MOTOR_NO1; indexUtilMotor <= STEP_MOTOR_NO12;indexUtilMotor++)
    {
        motorNo = (STEP_MOTOR_NO)indexUtilMotor;
        IPC_StepMotorBaseSetResetCmdDefault(&stepMotorResetCmd);
        stepMotorResetCmd.resetCorrectPos = 0;
        IPC_SrvStepMotorResetWhileAck(motorNo,&stepMotorResetCmd,&resultPack);
        if(resultPack.resultCode != PROT_RESULT_SUCCESS)
        {
            SystemPrintf("IPC_SrvStepMotorResetWhileAck Failed,MotorNo: %d,ErrorCode: 0X%08X\r\n",motorNo,resultPack.errorSub);
            return;
        }
    }
    //等待复位完成
    for(indexUtilMotor = STEP_MOTOR_NO1; indexUtilMotor <= STEP_MOTOR_NO12;indexUtilMotor++)
    {
        motorNo = (STEP_MOTOR_NO)indexUtilMotor;
        IPC_SrvStepMotorResetWaitCmdReturn(motorNo,&resultPack,NULL);
        if(resultPack.resultCode != PROT_RESULT_SUCCESS)
        {
            SystemPrintf("IPC_SrvStepMotorResetWaitCmdReturn Failed,MotorNo: %d,ErrorCode: 0X%08X\r\n",motorNo,resultPack.errorSub);
            return;
        }
    }
    //打印当前坐标
    for(indexUtilMotor = STEP_MOTOR_NO1; indexUtilMotor <= STEP_MOTOR_NO12;indexUtilMotor++)
    {
        motorNo = (STEP_MOTOR_NO)indexUtilMotor;
        SystemPrintf("Motor: %d CurrentPos: :%d\r\n",motorNo,IPC_SrvStepMotorGetRealTimeCoordinate(motorNo));
    }
    //正常运行五次,一次1600步
    for(indexUtilMain = 0; indexUtilMain < 5; indexUtilMain++)
    {
        //走步数
        for(indexUtilMotor = STEP_MOTOR_NO1; indexUtilMotor <= STEP_MOTOR_NO12;indexUtilMotor++)
        {
            motorNo = (STEP_MOTOR_NO)indexUtilMotor;
            //设置参数
            IPC_StepMotorBaseSetRunStepCmdDefault(&stepMotorRunStepCmd);
            stepMotorRunStepCmd.totalSteps = 16000;
            //发出指令
            IPC_SrvStepMotorRunStepWhileAck(motorNo,&stepMotorRunStepCmd,&resultPack);
            if(resultPack.resultCode != PROT_RESULT_SUCCESS)
            {
                SystemPrintf("IPC_SrvStepMotorRunStepWhileAck Failed,MotorNo: %d,ErrorCode: 0X%08X\r\n",motorNo,resultPack.errorSub);
                return;
            }
        }
        //等待返回
        for(indexUtilMotor = STEP_MOTOR_NO1; indexUtilMotor <= STEP_MOTOR_NO12;indexUtilMotor++)
        {
            motorNo = (STEP_MOTOR_NO)indexUtilMotor;
            IPC_SrvStepMotorRunStepWaitCmdReturn(motorNo,&resultPack,NULL);
            if(resultPack.resultCode != PROT_RESULT_SUCCESS)
            {
                SystemPrintf("IPC_SrvStepMotorRunStepWaitCmdReturn Failed,MotorNo: %d,ErrorCode: 0X%08X\r\n",motorNo,resultPack.errorSub);
                return;
            }
        }
        //打印当前坐标
        for(indexUtilMotor = STEP_MOTOR_NO1; indexUtilMotor <= STEP_MOTOR_NO12;indexUtilMotor++)
        {
            motorNo = (STEP_MOTOR_NO)indexUtilMotor;
            SystemPrintf("Motor: %d CurrentPos: :%d\r\n",motorNo,IPC_SrvStepMotorGetRealTimeCoordinate(motorNo));
        }
    }
    //走到坐标零点
    for(indexUtilMotor = STEP_MOTOR_NO1; indexUtilMotor <= STEP_MOTOR_NO12;indexUtilMotor++)
    {
        motorNo = (STEP_MOTOR_NO)indexUtilMotor;
        //回到零点,设置参数
        IPC_StepMotorBaseSetRunCoordinateCmdDefault(&stepMotorRunCoordinateCmd);
        stepMotorRunCoordinateCmd.targetCoordinate = 0;
        //发出指令
        IPC_SrvStepMotorRunCoordinateWhileAck(motorNo,&stepMotorRunCoordinateCmd,&resultPack);
        if(resultPack.resultCode != PROT_RESULT_SUCCESS)
        {
            SystemPrintf("IPC_SrvStepMotorRunCoordinateWhileAck Failed,MotorNo: %d,ErrorCode: 0X%08X\r\n",motorNo,resultPack.errorSub);
            return;
        }
    }
    for(indexUtilMotor = STEP_MOTOR_NO1; indexUtilMotor <= STEP_MOTOR_NO12;indexUtilMotor++)
    {
        motorNo = (STEP_MOTOR_NO)indexUtilMotor;
        //等待返回
        IPC_SrvStepMotorRunCoordinateWaitCmdReturn(motorNo,&resultPack,NULL);
        if(resultPack.resultCode != PROT_RESULT_SUCCESS)
        {
            SystemPrintf("IPC_SrvStepMotorRunCoordinateWaitCmdReturn Failed,MotorNo: %d,ErrorCode: 0X%08X\r\n",motorNo,resultPack.errorSub);
            return;
        }
    }
    //打印当前坐标
    for(indexUtilMotor = STEP_MOTOR_NO1; indexUtilMotor <= STEP_MOTOR_NO12;indexUtilMotor++)
    {
        motorNo = (STEP_MOTOR_NO)indexUtilMotor;
        SystemPrintf("Motor: %d CurrentPos: :%d\r\n",motorNo,IPC_SrvStepMotorGetRealTimeCoordinate(motorNo));
    }
    //反向运行1600步,运行五次
    for(indexUtilMain = 0; indexUtilMain < 5; indexUtilMain++)
    {
        for(indexUtilMotor = STEP_MOTOR_NO1; indexUtilMotor <= STEP_MOTOR_NO12;indexUtilMotor++)
        {
            motorNo = (STEP_MOTOR_NO)indexUtilMotor;
            //设置参数
            IPC_StepMotorBaseSetRunStepCmdDefault(&stepMotorRunStepCmd);
            stepMotorRunStepCmd.totalSteps = -16000;
            //发出指令
            IPC_SrvStepMotorRunStepWhileAck(motorNo,&stepMotorRunStepCmd,&resultPack);
            if(resultPack.resultCode != PROT_RESULT_SUCCESS)
            {
                SystemPrintf("IPC_SrvStepMotorRunStepWhileAck Failed,MotorNo: %d,ErrorCode: 0X%08X\r\n",motorNo,resultPack.errorSub);
                return;
            }
        }
        for(indexUtilMotor = STEP_MOTOR_NO1; indexUtilMotor <= STEP_MOTOR_NO12;indexUtilMotor++)
        {
            motorNo = (STEP_MOTOR_NO)indexUtilMotor;
            //等待返回
            IPC_SrvStepMotorRunStepWaitCmdReturn(motorNo,&resultPack,NULL);
            if(resultPack.resultCode != PROT_RESULT_SUCCESS)
            {
                SystemPrintf("IPC_SrvStepMotorRunStepWaitCmdReturn Failed,MotorNo: %d,ErrorCode: 0X%08X\r\n",motorNo,resultPack.errorSub);
                return;
            }
        }
        //打印当前坐标
        for(indexUtilMotor = STEP_MOTOR_NO1; indexUtilMotor <= STEP_MOTOR_NO12;indexUtilMotor++)
        {
            motorNo = (STEP_MOTOR_NO)indexUtilMotor;
            SystemPrintf("Motor: %d CurrentPos: :%d\r\n",motorNo,IPC_SrvStepMotorGetRealTimeCoordinate(motorNo));
        }
    }
    for(indexUtilMotor = STEP_MOTOR_NO1; indexUtilMotor <= STEP_MOTOR_NO12;indexUtilMotor++)
    {
        motorNo = (STEP_MOTOR_NO)indexUtilMotor;
        //回到零点,设置参数
        IPC_StepMotorBaseSetRunCoordinateCmdDefault(&stepMotorRunCoordinateCmd);
        stepMotorRunCoordinateCmd.targetCoordinate = 0;
        //发出指令
        IPC_SrvStepMotorRunCoordinateWhileAck(motorNo,&stepMotorRunCoordinateCmd,&resultPack);
        if(resultPack.resultCode != PROT_RESULT_SUCCESS)
        {
            SystemPrintf("IPC_SrvStepMotorRunCoordinateWhileAck Failed,MotorNo: %d,ErrorCode: 0X%08X\r\n",motorNo,resultPack.resultCode);
            return;
        }
    }
    for(indexUtilMotor = STEP_MOTOR_NO1; indexUtilMotor <= STEP_MOTOR_NO12;indexUtilMotor++)
    {
        motorNo = (STEP_MOTOR_NO)indexUtilMotor;
        //等待返回
        IPC_SrvStepMotorRunCoordinateWaitCmdReturn(motorNo,&resultPack,NULL);
        if(resultPack.resultCode != PROT_RESULT_SUCCESS)
        {
            SystemPrintf("IPC_SrvStepMotorRunCoordinateWaitCmdReturn Failed,MotorNo: %d,ErrorCode: 0X%08X\r\n",motorNo,resultPack.resultCode);
            return;
        }
    }
    //打印当前坐标
    for(indexUtilMotor = STEP_MOTOR_NO1; indexUtilMotor <= STEP_MOTOR_NO12;indexUtilMotor++)
    {
        motorNo = (STEP_MOTOR_NO)indexUtilMotor;
        SystemPrintf("Motor: %d CurrentPos: :%d\r\n",motorNo,IPC_SrvStepMotorGetRealTimeCoordinate(motorNo));
    }
    //执行IPC测试
    ipcReturnData.returnBufferPtr = NULL;
    ipcReturnData.returnDataLength = 0;
    //发送指令等待ACK
    IPC_SrvUtilOnLineWhileAck(PROT_CONNECT_FLAG_DEBUG,&resultPack);
    if(resultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        SystemPrintf("IPC_SrvUtilOnLineWhileAck Failed,ResultCode: %d,ErrorMain: 0X%04X,ErrorLevel: %d,ErrorSub: 0X%08X\r\n",
                        resultPack.resultCode,resultPack.errorMain,resultPack.errorLevel,resultPack.errorSub);
        return;
    }
    //等待返回
    IPC_SrvUtilWaitCmdReturn(IPC_CMD_MACRO_UTIL_ONLINE,&resultPack,&ipcReturnData);
    if(resultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        SystemPrintf("IPC_SrvUtilWaitCmdReturn Failed,ResultCode: %d,ErrorMain: 0X%04X,ErrorLevel: %d,ErrorSub: 0X%08X\r\n",
                        resultPack.resultCode,resultPack.errorMain,resultPack.errorLevel,resultPack.errorSub);
        return;
    }
    //解析数据
    if((ipcReturnData.returnBufferPtr != NULL)&&(ipcReturnData.returnDataLength != 0))
    {
        SystemPrintf("IPC_SrvUtilWaitCmdReturn ReturnDataLength: %d\r\n",ipcReturnData.returnDataLength);
        //释放内存
        IPC_CommonFreeReturnData(&ipcReturnData);
    }
    //执行IPC测试
    ipcReturnData.returnBufferPtr = NULL;
    ipcReturnData.returnDataLength = 0;
    //发送指令等待ACK
    IPC_SrvUtilHeartGetWhileAck(&resultPack);
    if(resultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        SystemPrintf("IPC_SrvUtilOnLineWhileAck Failed,ResultCode: %d,ErrorMain: 0X%04X,ErrorLevel: %d,ErrorSub: 0X%08X\r\n",
                        resultPack.resultCode,resultPack.errorMain,resultPack.errorLevel,resultPack.errorSub);
        return;
    }
    //查询完成结果
    do
    {
        IPC_SrvUtilQueryCmdState(IPC_CMD_MACRO_UTIL_HEART_GET,&cmdStage,&resultPack,&ipcReturnData);
        if(resultPack.resultCode != PROT_RESULT_SUCCESS)
        {
            SystemPrintf("IPC_SrvUtilQueryCmdState Failed,ResultCode: %d,ErrorMain: 0X%04X,ErrorLevel: %d,ErrorSub: 0X%08X\r\n",
                            resultPack.resultCode,resultPack.errorMain,resultPack.errorLevel,resultPack.errorSub);
            return;
        }
    }while(cmdStage == IPC_CMD_STAGE_WAIT);
    //解析数据
    if((ipcReturnData.returnBufferPtr != NULL)&&(ipcReturnData.returnDataLength != 0))
    {
        SystemPrintf("IPC_SrvUtilHeartGetWhileAck ReturnDataLength: %d\r\n",ipcReturnData.returnDataLength);
        //释放内存
        IPC_CommonFreeReturnData(&ipcReturnData);
    }
    //SRV IPC TIMER
    IPC_SrvTimerCommTestWhileAck(&resultPack);
    if(resultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        SystemPrintf("IPC_SrvTimerCommTestWhileAck Failed,ResultCode: %d,ErrorMain: 0X%04X,ErrorLevel: %d,ErrorSub: 0X%08X\r\n",
                        resultPack.resultCode,resultPack.errorMain,resultPack.errorLevel,resultPack.errorSub);
        return;
    }
    IPC_SrvTimerWaitLastCmdReturn(&resultPack,NULL);
    if(resultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        SystemPrintf("IPC_SrvTimerWaitLastCmdReturn Failed,ResultCode: %d,ErrorMain: 0X%04X,ErrorLevel: %d,ErrorSub: 0X%08X\r\n",
                        resultPack.resultCode,resultPack.errorMain,resultPack.errorLevel,resultPack.errorSub);
        return;
    }
    //SRV IPC FlashIAP
    IPC_SrvFlashIapCommTestWhileAck(&resultPack);
    if(resultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        SystemPrintf("IPC_SrvFlashIapCommTestWhileAck Failed,ResultCode: %d,ErrorMain: 0X%04X,ErrorLevel: %d,ErrorSub: 0X%08X\r\n",
                        resultPack.resultCode,resultPack.errorMain,resultPack.errorLevel,resultPack.errorSub);
        return;
    }
    IPC_SrvFlashIapWaitLastCmdReturn(&resultPack,NULL);
    if(resultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        SystemPrintf("IPC_SrvFlashIapWaitLastCmdReturn Failed,ResultCode: %d,ErrorMain: 0X%04X,ErrorLevel: %d,ErrorSub: 0X%08X\r\n",
                        resultPack.resultCode,resultPack.errorMain,resultPack.errorLevel,resultPack.errorSub);
        return;
    }
    //SRV IPC Demo
    IPC_SrvDemoCommTestWhileAck(&resultPack);
    if(resultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        SystemPrintf("IPC_SrvDemoCommTestWhileAck Failed,ResultCode: %d,ErrorMain: 0X%04X,ErrorLevel: %d,ErrorSub: 0X%08X\r\n",
                        resultPack.resultCode,resultPack.errorMain,resultPack.errorLevel,resultPack.errorSub);
        return;
    }
    IPC_SrvDemoWaitLastCmdReturn(&resultPack,NULL);
    if(resultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        SystemPrintf("IPC_SrvDemoWaitLastCmdReturn Failed,ResultCode: %d,ErrorMain: 0X%04X,ErrorLevel: %d,ErrorSub: 0X%08X\r\n",
                        resultPack.resultCode,resultPack.errorMain,resultPack.errorLevel,resultPack.errorSub);
        return;
    }
    //MOD IPC UTIL
    IPC_ModUtilCommTestWhileAck(&resultPack);
    if(resultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        SystemPrintf("IPC_ModUtilCommTestWhileAck Failed,ResultCode: %d,ErrorMain: 0X%04X,ErrorLevel: %d,ErrorSub: 0X%08X\r\n",
                        resultPack.resultCode,resultPack.errorMain,resultPack.errorLevel,resultPack.errorSub);
        return;
    }
    IPC_ModUtilWaitLastCmdReturn(&resultPack,NULL);
    if(resultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        SystemPrintf("IPC_ModUtilWaitLastCmdReturn Failed,ResultCode: %d,ErrorMain: 0X%04X,ErrorLevel: %d,ErrorSub: 0X%08X\r\n",
                        resultPack.resultCode,resultPack.errorMain,resultPack.errorLevel,resultPack.errorSub);
        return;
    }
    SystemPrintf("%s,Success Run Once\r\n",__func__);
}

//任务状态机周期运行
void AppImplUtilFSM_RunPeriod(void)
{
    AppImplUtilFSM_RunPeriodIPC();
}






















