/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-27 17:08:07 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-02 11:44:39 +0800
************************************************************************************************/ 
#include "AppImplUtil.h"
#include "AppTaskHeader.h"


//任务全局对象初始化
void AppImplUtilVarInit(void)
{
    
}


//任务接收消息处理
void AppImplUtilMsgProcess(void* taskMsgPtr)
{
    
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
}

//任务状态机周期运行
void AppImplUtilFSM_RunPeriod(void)
{
    AppImplUtilFSM_RunPeriodIPC();
}






















