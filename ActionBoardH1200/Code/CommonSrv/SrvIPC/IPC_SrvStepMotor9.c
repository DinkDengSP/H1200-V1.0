/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-25 08:45:09 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-04 14:38:19 +0800
************************************************************************************************/ 
#include "IPC_SrvStepMotor9.h"
#include "CommonSrvTaskHeader.h"
#include "StepMotorMain.h"

//通道携带的数据缓存长度
#define LENGTH_BUFFER_IPC_SRV_STEP_MOTOR9             32
//IPC通道讯息
static IPC_CHANNEL_DATA ipcChannelDataSrvStepMotor9;
//通道名称
static const uint8_t* ipcChannelSrvStepMotor9NameStr = "SrvStepMotor9";

/*****************************************************通道控制************************************************************/
//初始化IPC通道
void IPC_SrvStepMotor9Init(void)
{
    IPC_BaseInitChannelData(&ipcChannelDataSrvStepMotor9,(uint8_t*)ipcChannelSrvStepMotor9NameStr,
                                &queueSrvTaskStepMotor9,LENGTH_BUFFER_IPC_SRV_STEP_MOTOR9);
}

//清空IPC信道
ERROR_SUB IPC_SrvStepMotor9ClearChannelByCmd(uint32_t cmdCode)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = IPC_BaseChannelClearChannel(&ipcChannelDataSrvStepMotor9,cmdCode);
    return errorSub;
}

//清空全部信道
ERROR_SUB IPC_SrvStepMotor9ClearChannel(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //首先清除通讯测试指令
    errorSub = IPC_BaseChannelClearChannel(&ipcChannelDataSrvStepMotor9,IPC_CMD_MACRO_COMM_TEST);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,IPC_CMD_MACRO_COMM_TEST,errorSub);
        return errorSub;
    }
    //通道内部全部指令清空
    uint32_t cmdCode = 0X00000000;
    for(cmdCode = IPC_CMD_MACRO_STEP_MOTOR_RESET;cmdCode <= IPC_CMD_MACRO_STEP_MOTOR_READ_ORIGIN_SENSOR;cmdCode++)
    {
        errorSub = IPC_BaseChannelClearChannel(&ipcChannelDataSrvStepMotor9,cmdCode);
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
void IPC_SrvStepMotor9WaitCmdReturn(uint32_t cmdCode,SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //先赋初值
    if(returnDataPtr != NULL)
    {
        returnDataPtr->returnBufferPtr = NULL;
        returnDataPtr->returnDataLength = 0;
    }
    //等待指令返回
    errorSub = IPC_BaseWaitCmdReturn(&ipcChannelDataSrvStepMotor9,cmdCode,resultPackPtr);
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
    if((ipcChannelDataSrvStepMotor9.channelCache.returnDataBufferLength != 0)&&(returnDataPtr != NULL))
    {
        //申请内存
        returnDataPtr->returnDataLength = ipcChannelDataSrvStepMotor9.channelCache.returnDataBufferLength;
        //不为0拷贝数据
        if(returnDataPtr->returnDataLength != 0)
        {
            returnDataPtr->returnBufferPtr = IPC_PortMemMallocWhileSuccess(returnDataPtr->returnDataLength);
            //数据拷贝
            UserMemCopy(returnDataPtr->returnBufferPtr,ipcChannelDataSrvStepMotor9.channelCache.dataBufferPtr,returnDataPtr->returnDataLength);
        }
    }
    return;
}

//等待上一次发送的指令结果返回
void IPC_SrvStepMotor9WaitLastCmdReturn(SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //先赋初值
    if(returnDataPtr != NULL)
    {
        returnDataPtr->returnBufferPtr = NULL;
        returnDataPtr->returnDataLength = 0;
    }
    //等待指令返回
    errorSub = IPC_BaseWaitLastCmdReturn(&ipcChannelDataSrvStepMotor9,resultPackPtr);
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
    if((ipcChannelDataSrvStepMotor9.channelCache.returnDataBufferLength != 0)&&(returnDataPtr != NULL))
    {
        //申请内存
        returnDataPtr->returnDataLength = ipcChannelDataSrvStepMotor9.channelCache.returnDataBufferLength;
        //不为0拷贝数据
        if(returnDataPtr->returnDataLength != 0)
        {
            returnDataPtr->returnBufferPtr = IPC_PortMemMallocWhileSuccess(returnDataPtr->returnDataLength);
            //数据拷贝
            UserMemCopy(returnDataPtr->returnBufferPtr,ipcChannelDataSrvStepMotor9.channelCache.dataBufferPtr,returnDataPtr->returnDataLength);
        }
    }
    return;
}


/*****************************************************查询指令结果-通用************************************************************/
//查询指定指令的状态
void IPC_SrvStepMotor9QueryCmdState(uint32_t cmdCode,IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                        IPC_RETURN_DATA* returnDataPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //先赋初值
    if(returnDataPtr != NULL)
    {
        returnDataPtr->returnBufferPtr = NULL;
        returnDataPtr->returnDataLength = 0;
    }
    errorSub = IPC_BaseQueryCmdState(&ipcChannelDataSrvStepMotor9,cmdCode,cmdStagePtr,resultPackPtr);
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
            returnDataPtr->returnDataLength = ipcChannelDataSrvStepMotor9.channelCache.returnDataBufferLength;
            //不为0拷贝数
            if(returnDataPtr->returnDataLength != 0)
            {
                returnDataPtr->returnBufferPtr = IPC_PortMemMallocWhileSuccess(returnDataPtr->returnDataLength);
                //数据拷贝
                UserMemCopy(returnDataPtr->returnBufferPtr,ipcChannelDataSrvStepMotor9.channelCache.dataBufferPtr,returnDataPtr->returnDataLength);
            }
        }
        return;
    }
    //设置无错误返回
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//查询上一次发送的指令的状态
void IPC_SrvStepMotor9QueryLastCmdState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,IPC_RETURN_DATA* returnDataPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //先赋初值
    if(returnDataPtr != NULL)
    {
        returnDataPtr->returnBufferPtr = NULL;
        returnDataPtr->returnDataLength = 0;
    }
    errorSub = IPC_BaseQueryLastCmdState(&ipcChannelDataSrvStepMotor9,cmdStagePtr,resultPackPtr);
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
            returnDataPtr->returnDataLength = ipcChannelDataSrvStepMotor9.channelCache.returnDataBufferLength;
            //不为0拷贝数据
            if(returnDataPtr->returnDataLength != 0)
            {
                returnDataPtr->returnBufferPtr = IPC_PortMemMallocWhileSuccess(returnDataPtr->returnDataLength);
                //数据拷贝
                UserMemCopy(returnDataPtr->returnBufferPtr,ipcChannelDataSrvStepMotor9.channelCache.dataBufferPtr,returnDataPtr->returnDataLength);
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
void IPC_SrvStepMotor9CommTestWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = IPC_BaseWaitCmdReturn(&ipcChannelDataSrvStepMotor9,IPC_CMD_MACRO_COMM_TEST,resultPackPtr);
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

//电机复位,WAIT
void IPC_SrvStepMotor9ResetWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,IPC_STEP_MOTOR_STATE* motorStatePtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    if(motorStatePtr != NULL)
    {
        //状态初始化
        IPC_StepMotorBaseSetStateDefault(motorStatePtr);
    }
    //等待返回
    errorSub = IPC_BaseWaitCmdReturn(&ipcChannelDataSrvStepMotor9,IPC_CMD_MACRO_STEP_MOTOR_RESET,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,IPC_CMD_MACRO_STEP_MOTOR_RESET,errorSub);
        //IPC报错,返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CMD_WAIT_RESULT,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    if(resultPackPtr->resultCode == PROT_RESULT_SUCCESS)
    {
        //结果解析
        if(motorStatePtr != NULL)
        {
            IPC_SrvStepMotorGetResultMotorState(&ipcChannelDataSrvStepMotor9,motorStatePtr);
        }
    }
    return;
}

//电机走步数,WAIT
void IPC_SrvStepMotor9RunStepWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,IPC_STEP_MOTOR_STATE* motorStatePtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //状态初始化
    if(motorStatePtr != NULL)
    {
        IPC_StepMotorBaseSetStateDefault(motorStatePtr);
    }
    //等待返回
    errorSub = IPC_BaseWaitCmdReturn(&ipcChannelDataSrvStepMotor9,IPC_CMD_MACRO_STEP_MOTOR_RUN_STEPS,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,IPC_CMD_MACRO_STEP_MOTOR_RUN_STEPS,errorSub);
        //IPC报错,返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CMD_WAIT_RESULT,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    if(resultPackPtr->resultCode == PROT_RESULT_SUCCESS)
    {
        //结果解析
        if(motorStatePtr != NULL)
        {
            IPC_SrvStepMotorGetResultMotorState(&ipcChannelDataSrvStepMotor9,motorStatePtr);
        }
    }
    return;
}

//电机走坐标,WAIT
void IPC_SrvStepMotor9RunCoordinateWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,IPC_STEP_MOTOR_STATE* motorStatePtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //状态初始化
    if(motorStatePtr != NULL)
    {
        IPC_StepMotorBaseSetStateDefault(motorStatePtr);
    }
    //等待返回
    errorSub = IPC_BaseWaitCmdReturn(&ipcChannelDataSrvStepMotor9,IPC_CMD_MACRO_STEP_MOTOR_RUN_COORDINATE,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,IPC_CMD_MACRO_STEP_MOTOR_RUN_COORDINATE,errorSub);
        //IPC报错,返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CMD_WAIT_RESULT,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    if(resultPackPtr->resultCode == PROT_RESULT_SUCCESS)
    {
        //结果解析
        if(motorStatePtr != NULL)
        {
            IPC_SrvStepMotorGetResultMotorState(&ipcChannelDataSrvStepMotor9,motorStatePtr);
        }
    }
    return;
}

//电机回零,WAIT
void IPC_SrvStepMotor9ReturnZeroWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,IPC_STEP_MOTOR_STATE* motorStatePtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //状态初始化
    if(motorStatePtr != NULL)
    {
        IPC_StepMotorBaseSetStateDefault(motorStatePtr);
    }
    //等待返回
    errorSub = IPC_BaseWaitCmdReturn(&ipcChannelDataSrvStepMotor9,IPC_CMD_MACRO_STEP_MOTOR_RETURN_ZERO,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,IPC_CMD_MACRO_STEP_MOTOR_RETURN_ZERO,errorSub);
        //IPC报错,返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CMD_WAIT_RESULT,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    if(resultPackPtr->resultCode == PROT_RESULT_SUCCESS)
    {
        //结果解析
        if(motorStatePtr != NULL)
        {
            IPC_SrvStepMotorGetResultMotorState(&ipcChannelDataSrvStepMotor9,motorStatePtr);
        }
    }
    return;
}

//电机持续运转,WAIT
void IPC_SrvStepMotor9RunAlwaysWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //等待返回
    errorSub = IPC_BaseWaitCmdReturn(&ipcChannelDataSrvStepMotor9,IPC_CMD_MACRO_STEP_MOTOR_RUN_ALWAYS,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,IPC_CMD_MACRO_STEP_MOTOR_RUN_ALWAYS,errorSub);
        //IPC报错,返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CMD_WAIT_RESULT,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    return;
}

//电机立即停止,WAIT
void IPC_SrvStepMotor9StopNowWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //等待返回
    errorSub = IPC_BaseWaitCmdReturn(&ipcChannelDataSrvStepMotor9,IPC_CMD_MACRO_STEP_MOTOR_STOP_NOW,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,IPC_CMD_MACRO_STEP_MOTOR_STOP_NOW,errorSub);
        //IPC报错,返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CMD_WAIT_RESULT,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    return;
}

//电机减速停止,WAIT
void IPC_SrvStepMotor9StopSlowDownWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,IPC_STEP_MOTOR_STATE* motorStatePtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //状态初始化
    if(motorStatePtr != NULL)
    {
        IPC_StepMotorBaseSetStateDefault(motorStatePtr);
    }
    //等待返回
    errorSub = IPC_BaseWaitCmdReturn(&ipcChannelDataSrvStepMotor9,IPC_CMD_MACRO_STEP_MOTOR_STOP_SLOW_DOWN,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,IPC_CMD_MACRO_STEP_MOTOR_STOP_SLOW_DOWN,errorSub);
        //IPC报错,返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CMD_WAIT_RESULT,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    if(resultPackPtr->resultCode == PROT_RESULT_SUCCESS)
    {
        //结果解析
        if(motorStatePtr != NULL)
        {
            IPC_SrvStepMotorGetResultMotorState(&ipcChannelDataSrvStepMotor9,motorStatePtr);
        }
    }
    return;
}

//获取电机当前坐标,WAIT
void IPC_SrvStepMotor9GetCoordinateWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,IPC_STEP_MOTOR_POS* motorPosPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //状态初始化
    if(motorPosPtr != NULL)
    {
        IPC_StepMotorBaseSetPosDefault(motorPosPtr);
    }
    //等待返回
    errorSub = IPC_BaseWaitCmdReturn(&ipcChannelDataSrvStepMotor9,IPC_CMD_MACRO_STEP_MOTOR_GET_COORDINATE,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,IPC_CMD_MACRO_STEP_MOTOR_GET_COORDINATE,errorSub);
        //IPC报错,返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CMD_WAIT_RESULT,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    if(resultPackPtr->resultCode == PROT_RESULT_SUCCESS)
    {
        //结果解析
        if(motorPosPtr != NULL)
        {
            IPC_SrvStepMotorGetResultMotorPos(&ipcChannelDataSrvStepMotor9,motorPosPtr);
        }
    }
    return;
}

//获取当前原点传感器状态,WAIT
void IPC_SrvStepMotor9ReadOriginSensorWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr,IPC_STEP_MOTOR_SENSOR* motorSensorPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //状态初始化
    if(motorSensorPtr != NULL)
    {
        IPC_StepMotorBaseSetSensorDefault(motorSensorPtr);
    }
    //等待返回
    errorSub = IPC_BaseWaitCmdReturn(&ipcChannelDataSrvStepMotor9,IPC_CMD_MACRO_STEP_MOTOR_READ_ORIGIN_SENSOR,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,IPC_CMD_MACRO_STEP_MOTOR_READ_ORIGIN_SENSOR,errorSub);
        //IPC报错,返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CMD_WAIT_RESULT,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    if(resultPackPtr->resultCode == PROT_RESULT_SUCCESS)
    {
        //结果解析
        if(motorSensorPtr != NULL)
        {
            IPC_SrvStepMotorGetResultMotorSensor(&ipcChannelDataSrvStepMotor9,motorSensorPtr);
        }
    }
    return;
}


//电机失能,WAIT
void IPC_SrvStepMotor9DisableWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //等待返回
    errorSub = IPC_BaseWaitCmdReturn(&ipcChannelDataSrvStepMotor9,IPC_CMD_MACRO_STEP_MOTOR_DISABLE,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,IPC_CMD_MACRO_STEP_MOTOR_DISABLE,errorSub);
        //IPC报错,返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CMD_WAIT_RESULT,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    return;
}

//电机进入教学模式,WAIT
void IPC_SrvStepMotor9EnterTeachModeWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //等待返回
    errorSub = IPC_BaseWaitCmdReturn(&ipcChannelDataSrvStepMotor9,IPC_CMD_MACRO_STEP_MOTOR_ENTER_TEACH_MODE,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,IPC_CMD_MACRO_STEP_MOTOR_ENTER_TEACH_MODE,errorSub);
        //IPC报错,返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CMD_WAIT_RESULT,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    return;
}

//电机强行进入默认状态,WAIT
void IPC_SrvStepMotor9ForceEnterDefaultWaitCmdReturn(SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //等待返回
    errorSub = IPC_BaseWaitCmdReturn(&ipcChannelDataSrvStepMotor9,IPC_CMD_MACRO_STEP_MOTOR_FORCE_TO_DEFAULT,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,IPC_CMD_MACRO_STEP_MOTOR_FORCE_TO_DEFAULT,errorSub);
        //IPC报错,返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CMD_WAIT_RESULT,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    return;
}

/*****************************************************查询指令结果-特用************************************************************/
//通讯测试,QUERY
void IPC_SrvStepMotor9QueryCommTestState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = IPC_BaseQueryCmdState(&ipcChannelDataSrvStepMotor9,IPC_CMD_MACRO_COMM_TEST,cmdStagePtr,resultPackPtr);
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

//电机复位,QUERY
void IPC_SrvStepMotor9QueryResetState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                            IPC_STEP_MOTOR_STATE* motorStatePtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //状态初始化
    if(motorStatePtr != NULL)
    {
        //状态初始化
        IPC_StepMotorBaseSetStateDefault(motorStatePtr);
    }
    //查询状态
    errorSub = IPC_BaseQueryCmdState(&ipcChannelDataSrvStepMotor9,IPC_CMD_MACRO_STEP_MOTOR_RESET,cmdStagePtr,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,IPC_CMD_MACRO_STEP_MOTOR_RESET,errorSub);
        //IPC报错,返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_QUERY_CMD_STATE,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    if((*cmdStagePtr == IPC_CMD_STAGE_OVER)&&(resultPackPtr->resultCode == PROT_RESULT_SUCCESS))
    {
        //结果解析
        if(motorStatePtr != NULL)
        {
            IPC_SrvStepMotorGetResultMotorState(&ipcChannelDataSrvStepMotor9,motorStatePtr);
        }
    }
    return;
}

//电机走步数,QUERY
void IPC_SrvStepMotor9QueryRunStepState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                            IPC_STEP_MOTOR_STATE* motorStatePtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //状态初始化
    if(motorStatePtr != NULL)
    {
        IPC_StepMotorBaseSetStateDefault(motorStatePtr);
    }
    //查询状态
    errorSub = IPC_BaseQueryCmdState(&ipcChannelDataSrvStepMotor9,IPC_CMD_MACRO_STEP_MOTOR_RUN_STEPS,cmdStagePtr,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,IPC_CMD_MACRO_STEP_MOTOR_RUN_STEPS,errorSub);
        //IPC报错,返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_QUERY_CMD_STATE,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    if((*cmdStagePtr == IPC_CMD_STAGE_OVER)&&(resultPackPtr->resultCode == PROT_RESULT_SUCCESS))
    {
        //结果解析
        if(motorStatePtr != NULL)
        {
            IPC_SrvStepMotorGetResultMotorState(&ipcChannelDataSrvStepMotor9,motorStatePtr);
        }
    }
    return;
}

//电机走坐标,QUERY
void IPC_SrvStepMotor9QueryRunCoordinateState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                                IPC_STEP_MOTOR_STATE* motorStatePtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //状态初始化
    if(motorStatePtr != NULL)
    {
        IPC_StepMotorBaseSetStateDefault(motorStatePtr);
    }
    //查询状态
    errorSub = IPC_BaseQueryCmdState(&ipcChannelDataSrvStepMotor9,IPC_CMD_MACRO_STEP_MOTOR_RUN_COORDINATE,cmdStagePtr,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,IPC_CMD_MACRO_STEP_MOTOR_RUN_COORDINATE,errorSub);
        //IPC报错,返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_QUERY_CMD_STATE,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    if((*cmdStagePtr == IPC_CMD_STAGE_OVER)&&(resultPackPtr->resultCode == PROT_RESULT_SUCCESS))
    {
        //结果解析
        if(motorStatePtr != NULL)
        {
            IPC_SrvStepMotorGetResultMotorState(&ipcChannelDataSrvStepMotor9,motorStatePtr);
        }
    }
    return;
}

//电机回零,QUERY
void IPC_SrvStepMotor9QueryReturnZeroState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                            IPC_STEP_MOTOR_STATE* motorStatePtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //状态初始化
    if(motorStatePtr != NULL)
    {
        IPC_StepMotorBaseSetStateDefault(motorStatePtr);
    }
    //查询状态
    errorSub = IPC_BaseQueryCmdState(&ipcChannelDataSrvStepMotor9,IPC_CMD_MACRO_STEP_MOTOR_RETURN_ZERO,cmdStagePtr,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,IPC_CMD_MACRO_STEP_MOTOR_RETURN_ZERO,errorSub);
        //IPC报错,返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_QUERY_CMD_STATE,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    if((*cmdStagePtr == IPC_CMD_STAGE_OVER)&&(resultPackPtr->resultCode == PROT_RESULT_SUCCESS))
    {
        //结果解析
        if(motorStatePtr != NULL)
        {
            IPC_SrvStepMotorGetResultMotorState(&ipcChannelDataSrvStepMotor9,motorStatePtr);
        }
    }
    return;
}

//电机持续运转,QUERY
void IPC_SrvStepMotor9QueryRunAlwaysState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //查询状态
    errorSub = IPC_BaseQueryCmdState(&ipcChannelDataSrvStepMotor9,IPC_CMD_MACRO_STEP_MOTOR_RUN_ALWAYS,cmdStagePtr,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,IPC_CMD_MACRO_STEP_MOTOR_RUN_ALWAYS,errorSub);
        //IPC报错,返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_QUERY_CMD_STATE,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    return;
}

//电机立即停止,QUERY
void IPC_SrvStepMotor9QueryStopNowState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //查询状态
    errorSub = IPC_BaseQueryCmdState(&ipcChannelDataSrvStepMotor9,IPC_CMD_MACRO_STEP_MOTOR_STOP_NOW,cmdStagePtr,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,IPC_CMD_MACRO_STEP_MOTOR_STOP_NOW,errorSub);
        //IPC报错,返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_QUERY_CMD_STATE,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    return;
}

//电机减速停止,QUERY
void IPC_SrvStepMotor9QueryStopSlowDownState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                                IPC_STEP_MOTOR_STATE* motorStatePtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //状态初始化
    if(motorStatePtr != NULL)
    {
        IPC_StepMotorBaseSetStateDefault(motorStatePtr);
    }
    //查询状态
    errorSub = IPC_BaseQueryCmdState(&ipcChannelDataSrvStepMotor9,IPC_CMD_MACRO_STEP_MOTOR_STOP_SLOW_DOWN,cmdStagePtr,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,IPC_CMD_MACRO_STEP_MOTOR_STOP_SLOW_DOWN,errorSub);
        //IPC报错,返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_QUERY_CMD_STATE,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    if((*cmdStagePtr == IPC_CMD_STAGE_OVER)&&(resultPackPtr->resultCode == PROT_RESULT_SUCCESS))
    {
        //结果解析
        if(motorStatePtr != NULL)
        {
            IPC_SrvStepMotorGetResultMotorState(&ipcChannelDataSrvStepMotor9,motorStatePtr);
        }
    }
    return;
}

//获取电机当前坐标,QUERY
void IPC_SrvStepMotor9QueryGetCoordinateState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                                IPC_STEP_MOTOR_POS* motorPosPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //状态初始化
    if(motorPosPtr != NULL)
    {
        IPC_StepMotorBaseSetPosDefault(motorPosPtr);
    }
    //查询状态
    errorSub = IPC_BaseQueryCmdState(&ipcChannelDataSrvStepMotor9,IPC_CMD_MACRO_STEP_MOTOR_GET_COORDINATE,cmdStagePtr,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,IPC_CMD_MACRO_STEP_MOTOR_GET_COORDINATE,errorSub);
        //IPC报错,返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_QUERY_CMD_STATE,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    if((*cmdStagePtr == IPC_CMD_STAGE_OVER)&&(resultPackPtr->resultCode == PROT_RESULT_SUCCESS))
    {
        //结果解析
        if(motorPosPtr != NULL)
        {
            IPC_SrvStepMotorGetResultMotorPos(&ipcChannelDataSrvStepMotor9,motorPosPtr);
        }
    }
    return;
}

//获取当前原点传感器状态,QUERY
void IPC_SrvStepMotor9QueryReadOriginSensorState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr,
                                                    IPC_STEP_MOTOR_SENSOR* motorSensorPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //状态初始化
    if(motorSensorPtr != NULL)
    {
        IPC_StepMotorBaseSetSensorDefault(motorSensorPtr);
    }
    //查询状态
    errorSub = IPC_BaseQueryCmdState(&ipcChannelDataSrvStepMotor9,IPC_CMD_MACRO_STEP_MOTOR_READ_ORIGIN_SENSOR,cmdStagePtr,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,IPC_CMD_MACRO_STEP_MOTOR_READ_ORIGIN_SENSOR,errorSub);
        //IPC报错,返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_QUERY_CMD_STATE,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    if((*cmdStagePtr == IPC_CMD_STAGE_OVER)&&(resultPackPtr->resultCode == PROT_RESULT_SUCCESS))
    {
        //结果解析
        if(motorSensorPtr != NULL)
        {
            IPC_SrvStepMotorGetResultMotorSensor(&ipcChannelDataSrvStepMotor9,motorSensorPtr);
        }
    }
    return;
}

//电机失能,QUERY
void IPC_SrvStepMotor9QueryDisableState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //查询状态
    errorSub = IPC_BaseQueryCmdState(&ipcChannelDataSrvStepMotor9,IPC_CMD_MACRO_STEP_MOTOR_DISABLE,cmdStagePtr,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,IPC_CMD_MACRO_STEP_MOTOR_DISABLE,errorSub);
        //IPC报错,返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_QUERY_CMD_STATE,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    return;
}

//电机进入教学模式,QUERY
void IPC_SrvStepMotor9QueryEnterTeachModeState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //查询状态
    errorSub = IPC_BaseQueryCmdState(&ipcChannelDataSrvStepMotor9,IPC_CMD_MACRO_STEP_MOTOR_ENTER_TEACH_MODE,cmdStagePtr,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,IPC_CMD_MACRO_STEP_MOTOR_ENTER_TEACH_MODE,errorSub);
        //IPC报错,返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_QUERY_CMD_STATE,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    return;
}

//电机强行进入默认状态,QUERY
void IPC_SrvStepMotor9QueryForceEnterDefaultState(IPC_CMD_STAGE* cmdStagePtr,SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //查询状态
    errorSub = IPC_BaseQueryCmdState(&ipcChannelDataSrvStepMotor9,IPC_CMD_MACRO_STEP_MOTOR_FORCE_TO_DEFAULT,cmdStagePtr,resultPackPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,IPC_CMD_MACRO_STEP_MOTOR_FORCE_TO_DEFAULT,errorSub);
        //IPC报错,返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_QUERY_CMD_STATE,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    return;
}


/*****************************************************发送指令等待ACK************************************************************/
static ERROR_SUB IPC_SrvStepMotor9CheckClearLastCmd(uint32_t cmdCode)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    PROT_CH_STA channelState;
    //首先检查是否上个数据未完成或者完成之后未读取
    channelState = IPC_BaseCheckCmdChannelState(&ipcChannelDataSrvStepMotor9,cmdCode);
    if(channelState == PROT_CH_STA_BUSY)
    {
        //清除上一次的指令
        errorSub = IPC_BaseChannelClearChannel(&ipcChannelDataSrvStepMotor9,cmdCode);
        if(errorSub != ERROR_SUB_OK)
        {
            SystemPrintf("%s,Cmd: 0X%08X,ErrorSub: 0X%08X\r\n",__func__,cmdCode,errorSub);
            return errorSub;
        }
    }
    return ERROR_SUB_OK;
}

//通讯测试,ACK
void IPC_SrvStepMotor9CommTestWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = IPC_SrvStepMotor9CheckClearLastCmd(IPC_CMD_MACRO_COMM_TEST);
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
    errorSub = IPC_BaseSendCmdWhileAck(&ipcChannelDataSrvStepMotor9,&ipcCmdBaseData);
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

//电机复位,ACK
void IPC_SrvStepMotor9ResetWhileAck(IPC_STEP_MOTOR_CMD_RESET* motorResetCmdPtr,SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = IPC_SrvStepMotor9CheckClearLastCmd(IPC_CMD_MACRO_STEP_MOTOR_RESET);
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
    IPC_SrvStepMotorSetResetCmdParam(&ipcCmdBaseData,motorResetCmdPtr);
    //清除掉上一次的指令了,进行下一次指令发送
    errorSub = IPC_BaseSendCmdWhileAck(&ipcChannelDataSrvStepMotor9,&ipcCmdBaseData);
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

//电机走步数,ACK
void IPC_SrvStepMotor9RunStepWhileAck(IPC_STEP_MOTOR_CMD_RUN_STEP* motorRunStepCmdPtr,SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = IPC_SrvStepMotor9CheckClearLastCmd(IPC_CMD_MACRO_STEP_MOTOR_RUN_STEPS);
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
    IPC_SrvStepMotorSetRunStepCmdParam(&ipcCmdBaseData,motorRunStepCmdPtr);
    //清除掉上一次的指令了,进行下一次指令发送
    errorSub = IPC_BaseSendCmdWhileAck(&ipcChannelDataSrvStepMotor9,&ipcCmdBaseData);
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

//电机走坐标,ACK
void IPC_SrvStepMotor9RunCoordinateWhileAck(IPC_STEP_MOTOR_CMD_RUN_COORDINATE* motorRunCoordinateCmdPtr,SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = IPC_SrvStepMotor9CheckClearLastCmd(IPC_CMD_MACRO_STEP_MOTOR_RUN_COORDINATE);
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
    IPC_SrvStepMotorSetRunCoordinateCmdParam(&ipcCmdBaseData,motorRunCoordinateCmdPtr);
    //清除掉上一次的指令了,进行下一次指令发送
    errorSub = IPC_BaseSendCmdWhileAck(&ipcChannelDataSrvStepMotor9,&ipcCmdBaseData);
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

//电机回零,ACK
void IPC_SrvStepMotor9ReturnZeroWhileAck(IPC_STEP_MOTOR_CMD_RETURN_ZERO* motorReturnZeroCmdPtr,SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = IPC_SrvStepMotor9CheckClearLastCmd(IPC_CMD_MACRO_STEP_MOTOR_RETURN_ZERO);
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
    IPC_SrvStepMotorSetReturnZeroCmdParam(&ipcCmdBaseData,motorReturnZeroCmdPtr);
    //清除掉上一次的指令了,进行下一次指令发送
    errorSub = IPC_BaseSendCmdWhileAck(&ipcChannelDataSrvStepMotor9,&ipcCmdBaseData);
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

//电机持续运转,ACK
void IPC_SrvStepMotor9RunAlwaysWhileAck(IPC_STEP_MOTOR_CMD_RUN_ALWAY* motorRunAlwayCmdPtr,SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = IPC_SrvStepMotor9CheckClearLastCmd(IPC_CMD_MACRO_STEP_MOTOR_RUN_ALWAYS);
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
    IPC_SrvStepMotorSetRunAlwayCmdParam(&ipcCmdBaseData,motorRunAlwayCmdPtr);
    //清除掉上一次的指令了,进行下一次指令发送
    errorSub = IPC_BaseSendCmdWhileAck(&ipcChannelDataSrvStepMotor9,&ipcCmdBaseData);
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

//电机立即停止,ACK
void IPC_SrvStepMotor9StopNowWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = IPC_SrvStepMotor9CheckClearLastCmd(IPC_CMD_MACRO_STEP_MOTOR_STOP_NOW);
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
    ipcCmdBaseData.commandCode = IPC_CMD_MACRO_STEP_MOTOR_STOP_NOW;
    //清除掉上一次的指令了,进行下一次指令发送
    errorSub = IPC_BaseSendCmdWhileAck(&ipcChannelDataSrvStepMotor9,&ipcCmdBaseData);
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

//电机减速停止,ACK
void IPC_SrvStepMotor9StopSlowDownWhileAck(IPC_STEP_MOTOR_CMD_STOP_SLOW* motorStopSlowCmdPtr,SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = IPC_SrvStepMotor9CheckClearLastCmd(IPC_CMD_MACRO_STEP_MOTOR_STOP_SLOW_DOWN);
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
    IPC_SrvStepMotorSetSlowStopCmdParam(&ipcCmdBaseData,motorStopSlowCmdPtr);
    //清除掉上一次的指令了,进行下一次指令发送
    errorSub = IPC_BaseSendCmdWhileAck(&ipcChannelDataSrvStepMotor9,&ipcCmdBaseData);
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

//获取电机当前坐标,ACK
void IPC_SrvStepMotor9GetCoordinateWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = IPC_SrvStepMotor9CheckClearLastCmd(IPC_CMD_MACRO_STEP_MOTOR_GET_COORDINATE);
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
    ipcCmdBaseData.commandCode = IPC_CMD_MACRO_STEP_MOTOR_GET_COORDINATE;
    //清除掉上一次的指令了,进行下一次指令发送
    errorSub = IPC_BaseSendCmdWhileAck(&ipcChannelDataSrvStepMotor9,&ipcCmdBaseData);
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

//获取当前原点传感器状态,ACK
void IPC_SrvStepMotor9ReadOriginSensorWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = IPC_SrvStepMotor9CheckClearLastCmd(IPC_CMD_MACRO_STEP_MOTOR_READ_ORIGIN_SENSOR);
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
    ipcCmdBaseData.commandCode = IPC_CMD_MACRO_STEP_MOTOR_READ_ORIGIN_SENSOR;
    //清除掉上一次的指令了,进行下一次指令发送
    errorSub = IPC_BaseSendCmdWhileAck(&ipcChannelDataSrvStepMotor9,&ipcCmdBaseData);
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

//电机失能,ACK
void IPC_SrvStepMotor9DisableWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = IPC_SrvStepMotor9CheckClearLastCmd(IPC_CMD_MACRO_STEP_MOTOR_DISABLE);
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
    ipcCmdBaseData.commandCode = IPC_CMD_MACRO_STEP_MOTOR_DISABLE;
    //清除掉上一次的指令了,进行下一次指令发送
    errorSub = IPC_BaseSendCmdWhileAck(&ipcChannelDataSrvStepMotor9,&ipcCmdBaseData);
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

//电机进入教学模式,ACK
void IPC_SrvStepMotor9EnterTeachModeWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = IPC_SrvStepMotor9CheckClearLastCmd(IPC_CMD_MACRO_STEP_MOTOR_ENTER_TEACH_MODE);
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
    ipcCmdBaseData.commandCode = IPC_CMD_MACRO_STEP_MOTOR_ENTER_TEACH_MODE;
    //清除掉上一次的指令了,进行下一次指令发送
    errorSub = IPC_BaseSendCmdWhileAck(&ipcChannelDataSrvStepMotor9,&ipcCmdBaseData);
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

//电机强行进入默认状态,ACK
void IPC_SrvStepMotor9ForceEnterDefaultWhileAck(SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = IPC_SrvStepMotor9CheckClearLastCmd(IPC_CMD_MACRO_STEP_MOTOR_FORCE_TO_DEFAULT);
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
    ipcCmdBaseData.commandCode = IPC_CMD_MACRO_STEP_MOTOR_FORCE_TO_DEFAULT;
    //清除掉上一次的指令了,进行下一次指令发送
    errorSub = IPC_BaseSendCmdWhileAck(&ipcChannelDataSrvStepMotor9,&ipcCmdBaseData);
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
void IPC_SrvStepMotor9CommTestWhileReturn(SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //清除上一个指令
    errorSub = IPC_SrvStepMotor9CheckClearLastCmd(IPC_CMD_MACRO_COMM_TEST);
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
    errorSub = IPC_BaseSendCmdWhileReturn(&ipcChannelDataSrvStepMotor9,&ipcCmdBaseData,resultPackPtr);
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

//电机复位,RETURN
void IPC_SrvStepMotor9ResetWhileReturn(IPC_STEP_MOTOR_CMD_RESET* motorResetCmdPtr,SYS_RESULT_PACK* resultPackPtr,
                                                IPC_STEP_MOTOR_STATE* motorStatePtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = IPC_SrvStepMotor9CheckClearLastCmd(IPC_CMD_MACRO_STEP_MOTOR_RESET);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CLEAR_LAST_CMD,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    //状态初始化
    if(motorStatePtr != NULL)
    {
        IPC_StepMotorBaseSetStateDefault(motorStatePtr);
    }
    //设置参数
    IPC_CMD_BASE_DATA ipcCmdBaseData;
    //先设置为默认值
    IPC_BaseSetCmdPackDefault(&ipcCmdBaseData);
    //然后写入
    IPC_SrvStepMotorSetResetCmdParam(&ipcCmdBaseData,motorResetCmdPtr);
    //清除掉上一次的指令了,进行下一次指令发送
    errorSub = IPC_BaseSendCmdWhileReturn(&ipcChannelDataSrvStepMotor9,&ipcCmdBaseData,resultPackPtr);
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
        //结果解析
        if(motorStatePtr != NULL)
        {
            IPC_SrvStepMotorGetResultMotorState(&ipcChannelDataSrvStepMotor9,motorStatePtr);
        }
    }
    return;
}

//电机走步数,RETURN
void IPC_SrvStepMotor9RunStepWhileReturn(IPC_STEP_MOTOR_CMD_RUN_STEP* motorRunStepCmdPtr,SYS_RESULT_PACK* resultPackPtr,
                                                IPC_STEP_MOTOR_STATE* motorStatePtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = IPC_SrvStepMotor9CheckClearLastCmd(IPC_CMD_MACRO_STEP_MOTOR_RUN_STEPS);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CLEAR_LAST_CMD,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    //状态初始化
    if(motorStatePtr != NULL)
    {
        IPC_StepMotorBaseSetStateDefault(motorStatePtr);
    }
    //设置参数
    IPC_CMD_BASE_DATA ipcCmdBaseData;
    //先设置为默认值
    IPC_BaseSetCmdPackDefault(&ipcCmdBaseData);
    //然后写入
    IPC_SrvStepMotorSetRunStepCmdParam(&ipcCmdBaseData,motorRunStepCmdPtr);
    //清除掉上一次的指令了,进行下一次指令发送
    errorSub = IPC_BaseSendCmdWhileReturn(&ipcChannelDataSrvStepMotor9,&ipcCmdBaseData,resultPackPtr);
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
        //结果解析
        if(motorStatePtr != NULL)
        {
            IPC_SrvStepMotorGetResultMotorState(&ipcChannelDataSrvStepMotor9,motorStatePtr);
        }
    }
    return;
}

//电机走坐标,RETURN
void IPC_SrvStepMotor9RunCoordinateWhileReturn(IPC_STEP_MOTOR_CMD_RUN_COORDINATE* motorRunCoordinateCmdPtr,SYS_RESULT_PACK* resultPackPtr,
                                                        IPC_STEP_MOTOR_STATE* motorStatePtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = IPC_SrvStepMotor9CheckClearLastCmd(IPC_CMD_MACRO_STEP_MOTOR_RUN_COORDINATE);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CLEAR_LAST_CMD,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    //状态初始化
    if(motorStatePtr != NULL)
    {
        IPC_StepMotorBaseSetStateDefault(motorStatePtr);
    }
    //设置参数
    IPC_CMD_BASE_DATA ipcCmdBaseData;
    //先设置为默认值
    IPC_BaseSetCmdPackDefault(&ipcCmdBaseData);
    //然后写入
    IPC_SrvStepMotorSetRunCoordinateCmdParam(&ipcCmdBaseData,motorRunCoordinateCmdPtr);
    //清除掉上一次的指令了,进行下一次指令发送
    errorSub = IPC_BaseSendCmdWhileReturn(&ipcChannelDataSrvStepMotor9,&ipcCmdBaseData,resultPackPtr);
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
        //结果解析
        if(motorStatePtr != NULL)
        {
            IPC_SrvStepMotorGetResultMotorState(&ipcChannelDataSrvStepMotor9,motorStatePtr);
        }
    }
    return;
}

//电机回零,RETURN
void IPC_SrvStepMotor9ReturnZeroWhileReturn(IPC_STEP_MOTOR_CMD_RETURN_ZERO* motorReturnZeroCmdPtr,SYS_RESULT_PACK* resultPackPtr,
                                                    IPC_STEP_MOTOR_STATE* motorStatePtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = IPC_SrvStepMotor9CheckClearLastCmd(IPC_CMD_MACRO_STEP_MOTOR_RETURN_ZERO);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CLEAR_LAST_CMD,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    //状态初始化
    if(motorStatePtr != NULL)
    {
        IPC_StepMotorBaseSetStateDefault(motorStatePtr);
    }
    //设置参数
    IPC_CMD_BASE_DATA ipcCmdBaseData;
    //先设置为默认值
    IPC_BaseSetCmdPackDefault(&ipcCmdBaseData);
    //然后写入
    IPC_SrvStepMotorSetReturnZeroCmdParam(&ipcCmdBaseData,motorReturnZeroCmdPtr);
    //清除掉上一次的指令了,进行下一次指令发送
    errorSub = IPC_BaseSendCmdWhileReturn(&ipcChannelDataSrvStepMotor9,&ipcCmdBaseData,resultPackPtr);
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
        //结果解析
        if(motorStatePtr != NULL)
        {
            IPC_SrvStepMotorGetResultMotorState(&ipcChannelDataSrvStepMotor9,motorStatePtr);
        }
    }
    return;
}

//电机持续运转,RETURN
void IPC_SrvStepMotor9RunAlwaysWhileReturn(IPC_STEP_MOTOR_CMD_RUN_ALWAY* motorRunAlwayCmdPtr,SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = IPC_SrvStepMotor9CheckClearLastCmd(IPC_CMD_MACRO_STEP_MOTOR_RUN_ALWAYS);
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
    IPC_SrvStepMotorSetRunAlwayCmdParam(&ipcCmdBaseData,motorRunAlwayCmdPtr);
    //清除掉上一次的指令了,进行下一次指令发送
    errorSub = IPC_BaseSendCmdWhileReturn(&ipcChannelDataSrvStepMotor9,&ipcCmdBaseData,resultPackPtr);
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

//电机立即停止,RETURN
void IPC_SrvStepMotor9StopNowWhileReturn(SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = IPC_SrvStepMotor9CheckClearLastCmd(IPC_CMD_MACRO_STEP_MOTOR_STOP_NOW);
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
    ipcCmdBaseData.commandCode = IPC_CMD_MACRO_STEP_MOTOR_STOP_NOW;
    //清除掉上一次的指令了,进行下一次指令发送
    errorSub = IPC_BaseSendCmdWhileReturn(&ipcChannelDataSrvStepMotor9,&ipcCmdBaseData,resultPackPtr);
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

//电机减速停止,RETURN
void IPC_SrvStepMotor9StopSlowDownWhileReturn(IPC_STEP_MOTOR_CMD_STOP_SLOW* motorStopSlowCmdPtr,SYS_RESULT_PACK* resultPackPtr,
                                                        IPC_STEP_MOTOR_STATE* motorStatePtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = IPC_SrvStepMotor9CheckClearLastCmd(IPC_CMD_MACRO_STEP_MOTOR_STOP_SLOW_DOWN);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CLEAR_LAST_CMD,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    //状态初始化
    if(motorStatePtr != NULL)
    {
        IPC_StepMotorBaseSetStateDefault(motorStatePtr);
    }
    //设置参数
    IPC_CMD_BASE_DATA ipcCmdBaseData;
    //先设置为默认值
    IPC_BaseSetCmdPackDefault(&ipcCmdBaseData);
    //然后写入
    IPC_SrvStepMotorSetSlowStopCmdParam(&ipcCmdBaseData,motorStopSlowCmdPtr);
    //清除掉上一次的指令了,进行下一次指令发送
    errorSub = IPC_BaseSendCmdWhileReturn(&ipcChannelDataSrvStepMotor9,&ipcCmdBaseData,resultPackPtr);
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
        //结果解析
        if(motorStatePtr != NULL)
        {
            IPC_SrvStepMotorGetResultMotorState(&ipcChannelDataSrvStepMotor9,motorStatePtr);
        }
    }
    return;
}

//获取电机当前坐标,RETURN
void IPC_SrvStepMotor9GetCoordinateWhileReturn(SYS_RESULT_PACK* resultPackPtr,IPC_STEP_MOTOR_POS* motorPosPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = IPC_SrvStepMotor9CheckClearLastCmd(IPC_CMD_MACRO_STEP_MOTOR_GET_COORDINATE);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CLEAR_LAST_CMD,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    //状态初始化
    if(motorPosPtr != NULL)
    {
        IPC_StepMotorBaseSetPosDefault(motorPosPtr);
    }
    //设置参数
    IPC_CMD_BASE_DATA ipcCmdBaseData;
    //先设置为默认值
    IPC_BaseSetCmdPackDefault(&ipcCmdBaseData);
    //然后写入
    ipcCmdBaseData.commandCode = IPC_CMD_MACRO_STEP_MOTOR_GET_COORDINATE;
    //清除掉上一次的指令了,进行下一次指令发送
    errorSub = IPC_BaseSendCmdWhileReturn(&ipcChannelDataSrvStepMotor9,&ipcCmdBaseData,resultPackPtr);
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
        //结果解析
        if(motorPosPtr != NULL)
        {
            IPC_SrvStepMotorGetResultMotorPos(&ipcChannelDataSrvStepMotor9,motorPosPtr);
        }
    }
    return;
}

//获取当前原点传感器状态,RETURN
void IPC_SrvStepMotor9ReadOriginSensorWhileReturn(SYS_RESULT_PACK* resultPackPtr,IPC_STEP_MOTOR_SENSOR* motorSensorPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = IPC_SrvStepMotor9CheckClearLastCmd(IPC_CMD_MACRO_STEP_MOTOR_READ_ORIGIN_SENSOR);
    if(errorSub != ERROR_SUB_OK)
    {
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_IPC_PROT_CLEAR_LAST_CMD,ERROR_LEVEL_ERROR,
                                        errorSub);
        return;
    }
    //状态初始化
    if(motorSensorPtr != NULL)
    {
        IPC_StepMotorBaseSetSensorDefault(motorSensorPtr);
    }
    //设置参数
    IPC_CMD_BASE_DATA ipcCmdBaseData;
    //先设置为默认值
    IPC_BaseSetCmdPackDefault(&ipcCmdBaseData);
    //然后写入
    ipcCmdBaseData.commandCode = IPC_CMD_MACRO_STEP_MOTOR_READ_ORIGIN_SENSOR;
    //清除掉上一次的指令了,进行下一次指令发送
    errorSub = IPC_BaseSendCmdWhileReturn(&ipcChannelDataSrvStepMotor9,&ipcCmdBaseData,resultPackPtr);
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
        //结果解析
        if(motorSensorPtr != NULL)
        {
            IPC_SrvStepMotorGetResultMotorSensor(&ipcChannelDataSrvStepMotor9,motorSensorPtr);
        }
    }
    return;
}

//电机失能,RETURN
void IPC_SrvStepMotor9DisableWhileReturn(SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = IPC_SrvStepMotor9CheckClearLastCmd(IPC_CMD_MACRO_STEP_MOTOR_DISABLE);
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
    ipcCmdBaseData.commandCode = IPC_CMD_MACRO_STEP_MOTOR_DISABLE;
    //清除掉上一次的指令了,进行下一次指令发送
    errorSub = IPC_BaseSendCmdWhileReturn(&ipcChannelDataSrvStepMotor9,&ipcCmdBaseData,resultPackPtr);
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

//电机进入教学模式,RETURN
void IPC_SrvStepMotor9EnterTeachModeWhileReturn(SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = IPC_SrvStepMotor9CheckClearLastCmd(IPC_CMD_MACRO_STEP_MOTOR_ENTER_TEACH_MODE);
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
    ipcCmdBaseData.commandCode = IPC_CMD_MACRO_STEP_MOTOR_ENTER_TEACH_MODE;
    //清除掉上一次的指令了,进行下一次指令发送
    errorSub = IPC_BaseSendCmdWhileReturn(&ipcChannelDataSrvStepMotor9,&ipcCmdBaseData,resultPackPtr);
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

//电机强行进入默认状态,RETURN
void IPC_SrvStepMotor9ForceEnterDefaultWhileReturn(SYS_RESULT_PACK* resultPackPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = IPC_SrvStepMotor9CheckClearLastCmd(IPC_CMD_MACRO_STEP_MOTOR_FORCE_TO_DEFAULT);
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
    ipcCmdBaseData.commandCode = IPC_CMD_MACRO_STEP_MOTOR_FORCE_TO_DEFAULT;
    //清除掉上一次的指令了,进行下一次指令发送
    errorSub = IPC_BaseSendCmdWhileReturn(&ipcChannelDataSrvStepMotor9,&ipcCmdBaseData,resultPackPtr);
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


/*****************************************************独立API************************************************************/
//设置电机状态
void IPC_SrvStepMotor9SetState(STEP_MOTOR_STATE_MAIN mainState,STEP_MOTOR_STATE_SUB subState)
{
    STEP_MOTOR_DATA_SET* motorDataSetPtr = StepMotorBaseGetDataSetPtr(STEP_MOTOR_NO9);
    motorDataSetPtr->stateSet.mainState = mainState;
    motorDataSetPtr->stateSet.subState = subState;
}

//电机急停
void IPC_SrvStepMotor9EmergencyStop(void)
{
    //步进电机写入急停
    StepMotorMainEmStopCmd(STEP_MOTOR_NO9);
}

//修改电机坐标
void IPC_SrvStepMotor9ModifyMotorCordinate(int32_t motorCurrentCoordinate)
{
    STEP_MOTOR_DATA_SET* motorDataSetPtr = StepMotorBaseGetDataSetPtr(STEP_MOTOR_NO9);
    if((motorDataSetPtr->attriPtr->motorRunAttri.motorExist == ENABLE)&&
        (motorDataSetPtr->attriPtr->motorEncoderAttri.encoderSet == DISABLE))
    {
        //强制初始化,电机存在且编码器不存在才能强行设置坐标
        StepMotorMainForceToDefaultState(STEP_MOTOR_NO9);
        //设置坐标
        motorDataSetPtr->stateSet.currentMotorCoordinate = motorCurrentCoordinate;
        motorDataSetPtr->stateSet.baseMotorCoordinate = motorCurrentCoordinate;
    }
    else
    {
        //有编码器的情况下不能强行设置坐标
        return;
    }
}

//获取电机实时坐标
int32_t IPC_SrvStepMotor9GetRealTimeCoordinate(void)
{
    int32_t motorPos;
    int32_t encoderPos;
    StepMotorReadMotorRealTimePosAndEncoderPos(STEP_MOTOR_NO9,&motorPos,&encoderPos);
    return motorPos;
}

//检查电机是否指令急停
FlagStatus IPC_SrvStepMotor9CheckEmergencyStopCmd(void)
{
    STEP_MOTOR_DATA_SET* motorDataSetPtr = StepMotorBaseGetDataSetPtr(STEP_MOTOR_NO9);
    if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&
        (motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_EM_STOP_CMD))
    {
        return SET;
    }
    return RESET;
}

//检查电机是否信号急停
FlagStatus IPC_SrvStepMotor9CheckEmergencyStopSignal(void)
{
    STEP_MOTOR_DATA_SET* motorDataSetPtr = StepMotorBaseGetDataSetPtr(STEP_MOTOR_NO9);
    if((motorDataSetPtr->stateSet.mainState == STEP_MOTOR_STATE_MAIN_STOP)&&
        (motorDataSetPtr->stateSet.subState == STEP_MOTOR_STATE_SUB_STOP_EM_STOP_SINGLE))
    {
        return SET;
    }
    return RESET;
}














