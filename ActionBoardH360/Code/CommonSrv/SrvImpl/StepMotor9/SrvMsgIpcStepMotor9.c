/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-31 08:43:34 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-01 16:35:21 +0800
************************************************************************************************/ 
#include "SrvImplStepMotor9.h"
#include "SrvImplBaseStepMotor9.h"
#include "CommonSrvTaskHeader.h"
#include "IPC_CommonSrvHeader.h"

/*************************************************************IPC指令处理处理***********************************************************/
//通讯测试
static void SrvMsgIpcStepMotor9Cmd_CommTest(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//电机复位
static void SrvMsgIpcStepMotor9Cmd_Reset(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//电机走步数
static void SrvMsgIpcStepMotor9Cmd_RunStep(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//电机走坐标
static void SrvMsgIpcStepMotor9Cmd_RunCoordinate(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//电机回零
static void SrvMsgIpcStepMotor9Cmd_ReturnZero(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//电机持续运转
static void SrvMsgIpcStepMotor9Cmd_RunAlways(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//电机立即停止
static void SrvMsgIpcStepMotor9Cmd_StopNow(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//电机减速停止
static void SrvMsgIpcStepMotor9Cmd_StopSlowDown(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//获取电机当前坐标
static void SrvMsgIpcStepMotor9Cmd_GetCoordinate(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//获取当前原点传感器状态
static void SrvMsgIpcStepMotor9Cmd_ReadOriginSensor(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);

//CAN指令处理函数列表
static const USER_IPC_CMD_PROC_UNIT ipcStepMotor9MsgCmdProcUnitArray[] = 
{
    //顺序是 指令码 预期参数最小长度  指令处理函数
    {(uint32_t)0XFFFFFFFF     ,0    ,SrvMsgIpcStepMotor9Cmd_CommTest            },
    {(uint32_t)0X00000000     ,0    ,SrvMsgIpcStepMotor9Cmd_Reset               },
    {(uint32_t)0X00000001     ,0    ,SrvMsgIpcStepMotor9Cmd_RunStep             },
    {(uint32_t)0X00000002     ,0    ,SrvMsgIpcStepMotor9Cmd_RunCoordinate       },
    {(uint32_t)0X00000003     ,0    ,SrvMsgIpcStepMotor9Cmd_ReturnZero          },
    {(uint32_t)0X00000004     ,0    ,SrvMsgIpcStepMotor9Cmd_RunAlways           },
    {(uint32_t)0X00000005     ,0    ,SrvMsgIpcStepMotor9Cmd_StopNow             },
    {(uint32_t)0X00000006     ,0    ,SrvMsgIpcStepMotor9Cmd_StopSlowDown        },
    {(uint32_t)0X00000007     ,0    ,SrvMsgIpcStepMotor9Cmd_GetCoordinate       },
    {(uint32_t)0X00000008     ,0    ,SrvMsgIpcStepMotor9Cmd_ReadOriginSensor    },
};


//IPC指令消息处理函数
void SrvMsgIpcStepMotor9Cmd(void* ipcMsgBodyPtr)
{
    //转换指令码
    IPC_CMD_DATA* ipcCmdMsgPtr = (IPC_CMD_DATA*)ipcMsgBodyPtr;
    if(ipcCmdMsgPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,Recv ipcCmdMsgPtr NULL\r\n",__func__,__LINE__);
        //空指针异常
        return;
    }
    IPC_CMD_WAIT_RESULT* ipcWaitResultPackPtr = ipcCmdMsgPtr->waitResultPackPtr;
    //搜索匹配处理器
    uint16_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < DIM_ARRAY_ELEMENT_COUNT(ipcStepMotor9MsgCmdProcUnitArray);indexUtil++)
    {
        if(ipcCmdMsgPtr->cmdBaseData.commandCode == (uint32_t)(ipcStepMotor9MsgCmdProcUnitArray[indexUtil].commandCode))
        {
            break;
        }
    }
    //判定序号
    if(indexUtil == DIM_ARRAY_ELEMENT_COUNT(ipcStepMotor9MsgCmdProcUnitArray))
    {
        //不支持的指令
        SystemPrintf("E:> %s,%d,Recv Unsupport CommandCode: 0X%08X\r\n",__func__,__LINE__,ipcCmdMsgPtr->cmdBaseData.commandCode);
        //设置等待状态为错误
        ipcWaitResultPackPtr->currentCmdStage = IPC_CMD_STAGE_ERROR_SUPPORT;
        //释放内存
        if((ipcCmdMsgPtr->cmdBaseData.cmdParamBufferPtr != NULL)&&(ipcCmdMsgPtr->cmdBaseData.cmdParamBufferLength != 0))
        {
            IPC_PortMemFree(ipcCmdMsgPtr->cmdBaseData.cmdParamBufferPtr);
        }
        IPC_PortMemFree(ipcCmdMsgPtr);
        return;
    }
    //判定长度
    if(ipcCmdMsgPtr->cmdBaseData.cmdParamBufferLength < ipcStepMotor9MsgCmdProcUnitArray[indexUtil].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,CommandCode: 0X%08X\r\n",__func__,__LINE__,
                        ipcStepMotor9MsgCmdProcUnitArray[indexUtil].exceptParamLengthMin,ipcCmdMsgPtr->cmdBaseData.cmdParamBufferLength,
                        ipcCmdMsgPtr->cmdBaseData.commandCode);
        //设置等待状态为错误
        ipcWaitResultPackPtr->currentCmdStage = IPC_CMD_STAGE_CMD_PAR_LEN;
        //释放内存
        if((ipcCmdMsgPtr->cmdBaseData.cmdParamBufferPtr != NULL)&&(ipcCmdMsgPtr->cmdBaseData.cmdParamBufferLength != 0))
        {
            IPC_PortMemFree(ipcCmdMsgPtr->cmdBaseData.cmdParamBufferPtr);
        }
        IPC_PortMemFree(ipcCmdMsgPtr);
        return;
    }
    //校验通过执行代码
    SYS_RESULT_PACK sysResultPack;
    //初始化错误返回包
    sysResultPack.resultCode = PROT_RESULT_FAIL;
    sysResultPack.errorMain = ERROR_MAIN_OK;
    sysResultPack.errorLevel = ERROR_LEVEL_NONE;
    sysResultPack.errorSub = ERROR_SUB_OK;
    //检测函数指针
    if(ipcStepMotor9MsgCmdProcUnitArray[indexUtil].ipcCmdProcFuncPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,ipcCmdProcFuncPtr NULL,CommandCode: 0X%08X\r\n",__func__,__LINE__,
                        ipcCmdMsgPtr->cmdBaseData.commandCode);
        //设置等待状态为错误
        ipcWaitResultPackPtr->currentCmdStage = IPC_CMD_STAGE_PROC_FUNC_NULL;
        //释放内存
        if((ipcCmdMsgPtr->cmdBaseData.cmdParamBufferPtr != NULL)&&(ipcCmdMsgPtr->cmdBaseData.cmdParamBufferLength != 0))
        {
            IPC_PortMemFree(ipcCmdMsgPtr->cmdBaseData.cmdParamBufferPtr);
        }
        IPC_PortMemFree(ipcCmdMsgPtr);
        return;
    }
    //调用处理函数
    ipcStepMotor9MsgCmdProcUnitArray[indexUtil].ipcCmdProcFuncPtr(&(ipcCmdMsgPtr->cmdBaseData),&sysResultPack,ipcCmdMsgPtr->channelCachePtr);
    //处理完成回传
    ipcWaitResultPackPtr->resultPack.resultCode = sysResultPack.resultCode;
    ipcWaitResultPackPtr->resultPack.errorMain = sysResultPack.errorMain;
    ipcWaitResultPackPtr->resultPack.errorLevel = sysResultPack.errorLevel;
    ipcWaitResultPackPtr->resultPack.errorSub = sysResultPack.errorSub;
    //释放内存
    if((ipcCmdMsgPtr->cmdBaseData.cmdParamBufferPtr != NULL)&&(ipcCmdMsgPtr->cmdBaseData.cmdParamBufferLength != 0))
    {
        IPC_PortMemFree(ipcCmdMsgPtr->cmdBaseData.cmdParamBufferPtr);
    }
    IPC_PortMemFree(ipcCmdMsgPtr);
    //状态设置完成
    ipcWaitResultPackPtr->currentCmdStage = IPC_CMD_STAGE_OVER;
    return;
}

//通讯测试
static void SrvMsgIpcStepMotor9Cmd_CommTest(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{
#if(IPC_SRV_RUNNING_LOG_SHOW == CONFIG_FUNC_ENABLE)
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
#endif
    //回传数据为0
    channelCachePtr->returnDataBufferLength = 0;
    //构建完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//电机复位
static void SrvMsgIpcStepMotor9Cmd_Reset(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{
    //复位修正
    int32_t resetCorrectPos = (int32_t)(ipcCmdMsgPtr->cmdParam1);
    //复位修正曲线
    uint8_t resetCorrectCurve = (uint8_t)(ipcCmdMsgPtr->cmdParam2);
    //超时时间
    uint32_t timeOutMs = ipcCmdMsgPtr->cmdParam3;
#if(IPC_SRV_RUNNING_LOG_SHOW == CONFIG_FUNC_ENABLE)
    //打印参数
    SystemPrintf("%s,resetCorrectPos: %d,resetCorrectCurve: %d,timeOutMs: %d\r\n",__func__,resetCorrectPos,
                    resetCorrectCurve,timeOutMs);
#endif
    //电机复位
    StepMotorResetCmd stepMotorResetCmd;
    stepMotorResetCmd.motorNo = STEP_MOTOR_NO9;
    stepMotorResetCmd.correctionCoordinate = resetCorrectPos;
    stepMotorResetCmd.timeOutSet = timeOutMs;
    //回传数据定义
    uint8_t* returnBufferPtr = NULL;
    uint16_t returnBufferLength = 0;
    //先设置回传数据为0
    channelCachePtr->returnDataBufferLength = 0;
    //调用API
    SrvImplBaseStepMotor9Reset(&stepMotorResetCmd,resultPackPtr,&returnBufferPtr,&returnBufferLength);
    //对结果进行处理
    if(resultPackPtr->resultCode == PROT_RESULT_SUCCESS)
    {
        if((returnBufferPtr != NULL)&&(returnBufferLength != 0))
        {
            if(returnBufferLength > channelCachePtr->carryDataBufferLength)
            {
                //缓存太小放不下
                SystemPrintf("%s,returnBufferLength More Then Carry Buffer: %d,%d\r\n",__func__,returnBufferLength,channelCachePtr->carryDataBufferLength);
                CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_STEP_MOTOR,ERROR_LEVEL_WARNING,
                                                ERROR_SUB_BOARD_IPC_CMD_CARRY_BUFFER_LEN_NOT_ENOUGH);
                return;
            }
            else
            {
                //拷贝回传数据
                channelCachePtr->returnDataBufferLength = returnBufferLength;
                UserMemCopy(channelCachePtr->dataBufferPtr,returnBufferPtr,returnBufferLength);
            }
        }
    }
    //数据处理完成之后释放内存
    if((returnBufferPtr != NULL)&&(returnBufferLength != 0))
    {
        UserMemFree(MEM_REGION_SRV_TASK,returnBufferPtr);
    }
    return;
}

//电机走步数
static void SrvMsgIpcStepMotor9Cmd_RunStep(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{
    //总步数
    int32_t totalSteps = (int32_t)(ipcCmdMsgPtr->cmdParam1);
    //运行曲线
    uint8_t runStepCurve = (uint8_t)(ipcCmdMsgPtr->cmdParam2);
    //速度比例模式
    uint8_t speedRatioMode = (uint8_t)((ipcCmdMsgPtr->cmdParam2)>>8);
    //速度比例
    uint16_t speedRatioUint16 = (uint16_t)((ipcCmdMsgPtr->cmdParam2)>>16);
    if(speedRatioUint16 > 100)speedRatioUint16 = 100;
    if(speedRatioUint16 == 0)speedRatioUint16 = 1;
    float speedRatio = (float)(((float)speedRatioUint16)/100.0f);
    //换向修正
    uint32_t dirSwitchCorrect = ipcCmdMsgPtr->cmdParam3;
    uint8_t emStopEnable = (uint8_t)(ipcCmdMsgPtr->cmdParam4);
    uint8_t utilStop1Enable = (uint8_t)((ipcCmdMsgPtr->cmdParam4)>>8);
    uint8_t utilStop2Enable = (uint8_t)((ipcCmdMsgPtr->cmdParam4)>>16);
    //超时时间
    uint32_t timeOutMs = ipcCmdMsgPtr->cmdParam5;
#if(IPC_SRV_RUNNING_LOG_SHOW == CONFIG_FUNC_ENABLE)
    //打印参数
    SystemPrintf("%s,totalSteps: %d,runStepCurve: %d,speedRatioMode: %d,speedRatio: %0.3f,dirSwitchCorrect: %d\r\n",__func__,totalSteps,
                    runStepCurve,speedRatioMode,(double)speedRatio,dirSwitchCorrect);
    SystemPrintf("%s,emStopEnable: %d,utilStop1Enable: %d,utilStop2Enable: %d\r\n",__func__,emStopEnable,utilStop1Enable,utilStop2Enable);
#endif
    //走步数结构体
    StepMotorRunStepsCmd runStepCmd;
    runStepCmd.motorNo = STEP_MOTOR_NO9;
    runStepCmd.specialSteps = totalSteps;
    runStepCmd.selectConfig = runStepCurve;
    runStepCmd.speedRatioMode = (SPEED_RATIO_MODE)speedRatioMode;
    runStepCmd.speedRatio = speedRatioUint16;
    runStepCmd.utilStop1Enable = utilStop1Enable;
    runStepCmd.utilStop2Enable = utilStop2Enable;
    runStepCmd.timeOutSet = timeOutMs;
    //回传数据定义
    uint8_t* returnBufferPtr = NULL;
    uint16_t returnBufferLength = 0;
    //先设置回传数据为0
    channelCachePtr->returnDataBufferLength = 0;
    //调用API
    SrvImplBaseStepMotor9RunStep(&runStepCmd,resultPackPtr,&returnBufferPtr,&returnBufferLength);
    //对结果进行处理
    if(resultPackPtr->resultCode == PROT_RESULT_SUCCESS)
    {
        if((returnBufferPtr != NULL)&&(returnBufferLength != 0))
        {
            if(returnBufferLength > channelCachePtr->carryDataBufferLength)
            {
                //缓存太小放不下
                SystemPrintf("%s,returnBufferLength More Then Carry Buffer: %d,%d\r\n",__func__,returnBufferLength,channelCachePtr->carryDataBufferLength);
                CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_STEP_MOTOR,ERROR_LEVEL_WARNING,
                                                ERROR_SUB_BOARD_IPC_CMD_CARRY_BUFFER_LEN_NOT_ENOUGH);
                return;
            }
            else
            {
                //拷贝回传数据
                channelCachePtr->returnDataBufferLength = returnBufferLength;
                UserMemCopy(channelCachePtr->dataBufferPtr,returnBufferPtr,returnBufferLength);
            }
        }
    }
    //数据处理完成之后释放内存
    if((returnBufferPtr != NULL)&&(returnBufferLength != 0))
    {
        UserMemFree(MEM_REGION_SRV_TASK,returnBufferPtr);
    }
    return;
}

//电机走坐标
static void SrvMsgIpcStepMotor9Cmd_RunCoordinate(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{
    //目标坐标
    int32_t targetCoordinate = (int32_t)(ipcCmdMsgPtr->cmdParam1);
    //运行曲线
    uint8_t runStepCurve = (uint8_t)(ipcCmdMsgPtr->cmdParam2);
    //速度比例模式
    uint8_t speedRatioMode = (uint8_t)((ipcCmdMsgPtr->cmdParam2)>>8);
    //速度比例
    uint16_t speedRatioUint16 = (uint16_t)((ipcCmdMsgPtr->cmdParam2)>>16);
    if(speedRatioUint16 > 100)speedRatioUint16 = 100;
    if(speedRatioUint16 == 0)speedRatioUint16 = 1;
    float speedRatio = (float)(((float)speedRatioUint16)/100.0f);
    //换向修正
    uint32_t dirSwitchCorrect = ipcCmdMsgPtr->cmdParam3;
    uint8_t emStopEnable = (uint8_t)(ipcCmdMsgPtr->cmdParam4);
    uint8_t utilStop1Enable = (uint8_t)((ipcCmdMsgPtr->cmdParam4)>>8);
    uint8_t utilStop2Enable = (uint8_t)((ipcCmdMsgPtr->cmdParam4)>>16);
    //超时时间
    uint32_t timeOutMs = ipcCmdMsgPtr->cmdParam5;
#if(IPC_SRV_RUNNING_LOG_SHOW == CONFIG_FUNC_ENABLE)
    //打印参数
    SystemPrintf("%s,targetCoordinate: %d,runStepCurve: %d,speedRatioMode: %d,speedRatio: %0.3f,dirSwitchCorrect: %d\r\n",__func__,targetCoordinate,
                    runStepCurve,speedRatioMode,(double)speedRatio,dirSwitchCorrect);
    SystemPrintf("%s,emStopEnable: %d,utilStop1Enable: %d,utilStop2Enable: %d\r\n",__func__,emStopEnable,utilStop1Enable,utilStop2Enable);
#endif
    //走坐标结构体
    StepMotorRun2CoordationCmd runCoordinateCmd;
    runCoordinateCmd.motorNo = STEP_MOTOR_NO9;
    runCoordinateCmd.targetCoordinate = targetCoordinate;
    runCoordinateCmd.selectConfig = runStepCurve;
    runCoordinateCmd.speedRatioMode = (SPEED_RATIO_MODE)speedRatioMode;
    runCoordinateCmd.speedRatio = speedRatioUint16;
    runCoordinateCmd.utilStop1Enable = utilStop1Enable;
    runCoordinateCmd.utilStop2Enable = utilStop2Enable;
    runCoordinateCmd.timeOutSet = timeOutMs;
    //回传数据定义
    uint8_t* returnBufferPtr = NULL;
    uint16_t returnBufferLength = 0;
    //先设置回传数据为0
    channelCachePtr->returnDataBufferLength = 0;
    //调用API
    SrvImplBaseStepMotor9RunCoordinate(&runCoordinateCmd,resultPackPtr,&returnBufferPtr,&returnBufferLength);
    //对结果进行处理
    if(resultPackPtr->resultCode == PROT_RESULT_SUCCESS)
    {
        if((returnBufferPtr != NULL)&&(returnBufferLength != 0))
        {
            if(returnBufferLength > channelCachePtr->carryDataBufferLength)
            {
                //缓存太小放不下
                SystemPrintf("%s,returnBufferLength More Then Carry Buffer: %d,%d\r\n",__func__,returnBufferLength,channelCachePtr->carryDataBufferLength);
                CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_STEP_MOTOR,ERROR_LEVEL_WARNING,
                                                ERROR_SUB_BOARD_IPC_CMD_CARRY_BUFFER_LEN_NOT_ENOUGH);
                return;
            }
            else
            {
                //拷贝回传数据
                channelCachePtr->returnDataBufferLength = returnBufferLength;
                UserMemCopy(channelCachePtr->dataBufferPtr,returnBufferPtr,returnBufferLength);
            }
        }
    }
    //数据处理完成之后释放内存
    if((returnBufferPtr != NULL)&&(returnBufferLength != 0))
    {
        UserMemFree(MEM_REGION_SRV_TASK,returnBufferPtr);
    }
    return;
}

//电机回零
static void SrvMsgIpcStepMotor9Cmd_ReturnZero(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{
    //回零后修正曲线
    int32_t returnZeroCorrectPos = (int32_t)(ipcCmdMsgPtr->cmdParam1);
    //运行曲线
    uint8_t runStepCurve = (uint8_t)(ipcCmdMsgPtr->cmdParam2);
    //速度比例模式
    uint8_t speedRatioMode = (uint8_t)((ipcCmdMsgPtr->cmdParam2)>>8);
    //速度比例
    uint16_t speedRatioUint16 = (uint16_t)((ipcCmdMsgPtr->cmdParam2)>>16);
    if(speedRatioUint16 > 100)speedRatioUint16 = 100;
    if(speedRatioUint16 == 0)speedRatioUint16 = 1;
    float speedRatio = (float)(((float)speedRatioUint16)/100.0f);
    //换向修正
    uint32_t dirSwitchCorrect = ipcCmdMsgPtr->cmdParam3;
    uint8_t emStopEnable = (uint8_t)(ipcCmdMsgPtr->cmdParam4);
    uint8_t utilStop1Enable = (uint8_t)((ipcCmdMsgPtr->cmdParam4)>>8);
    uint8_t utilStop2Enable = (uint8_t)((ipcCmdMsgPtr->cmdParam4)>>16);
    //回零是否复位
    uint8_t needReset = (uint8_t)((ipcCmdMsgPtr->cmdParam4)>>24);
    //超时时间
    uint32_t timeOutMs = ipcCmdMsgPtr->cmdParam5;
#if(IPC_SRV_RUNNING_LOG_SHOW == CONFIG_FUNC_ENABLE)
    //打印参数
    SystemPrintf("%s,returnZeroCorrectPos: %d,runStepCurve: %d,speedRatioMode: %d,speedRatio: %0.3f,dirSwitchCorrect: %d\r\n",__func__,
                    returnZeroCorrectPos,runStepCurve,speedRatioMode,(double)speedRatio,dirSwitchCorrect);
    SystemPrintf("%s,NeedReset: %d,emStopEnable: %d,utilStop1Enable: %d,utilStop2Enable: %d\r\n",__func__,needReset,emStopEnable,utilStop1Enable,
                    utilStop2Enable);
#endif
    //回零结构体
    StepMotorReturnZeroCmd returnZeroCmd;
    returnZeroCmd.motorNo = STEP_MOTOR_NO9;
    returnZeroCmd.correctionCoordinate = returnZeroCorrectPos;
    returnZeroCmd.selectConfig = runStepCurve;
    returnZeroCmd.speedRatioMode = (SPEED_RATIO_MODE)speedRatioMode;
    returnZeroCmd.speedRatio = speedRatioUint16;
    returnZeroCmd.utilStop1Enable = utilStop1Enable;
    returnZeroCmd.utilStop2Enable = utilStop2Enable;
    returnZeroCmd.timeOutSet = timeOutMs;
    //回传数据定义
    uint8_t* returnBufferPtr = NULL;
    uint16_t returnBufferLength = 0;
    //先设置回传数据为0
    channelCachePtr->returnDataBufferLength = 0;
    //调用API
    SrvImplBaseStepMotor9ReturnZero(&returnZeroCmd,needReset,resultPackPtr,&returnBufferPtr,&returnBufferLength);
    //对结果进行处理
    if(resultPackPtr->resultCode == PROT_RESULT_SUCCESS)
    {
        if((returnBufferPtr != NULL)&&(returnBufferLength != 0))
        {
            if(returnBufferLength > channelCachePtr->carryDataBufferLength)
            {
                //缓存太小放不下
                SystemPrintf("%s,returnBufferLength More Then Carry Buffer: %d,%d\r\n",__func__,returnBufferLength,channelCachePtr->carryDataBufferLength);
                CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_STEP_MOTOR,ERROR_LEVEL_WARNING,
                                                ERROR_SUB_BOARD_IPC_CMD_CARRY_BUFFER_LEN_NOT_ENOUGH);
                return;
            }
            else
            {
                //拷贝回传数据
                channelCachePtr->returnDataBufferLength = returnBufferLength;
                UserMemCopy(channelCachePtr->dataBufferPtr,returnBufferPtr,returnBufferLength);
            }
        }
    }
    //数据处理完成之后释放内存
    if((returnBufferPtr != NULL)&&(returnBufferLength != 0))
    {
        UserMemFree(MEM_REGION_SRV_TASK,returnBufferPtr);
    }
    return;
}

//电机持续运转
static void SrvMsgIpcStepMotor9Cmd_RunAlways(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{
    //运行方向
    uint8_t runAlwaysDir = (uint8_t)(ipcCmdMsgPtr->cmdParam1);
    //运行曲线
    uint8_t runStepCurve = (uint8_t)(ipcCmdMsgPtr->cmdParam2);
    //速度比例模式
    uint8_t speedRatioMode = (uint8_t)((ipcCmdMsgPtr->cmdParam2)>>8);
    //速度比例
    uint16_t speedRatioUint16 = (uint16_t)((ipcCmdMsgPtr->cmdParam2)>>16);
    if(speedRatioUint16 > 100)speedRatioUint16 = 100;
    if(speedRatioUint16 == 0)speedRatioUint16 = 1;
    float speedRatio = (float)(((float)speedRatioUint16)/100.0f);
#if(IPC_SRV_RUNNING_LOG_SHOW == CONFIG_FUNC_ENABLE)
    //打印参数
    SystemPrintf("%s,runAlwaysDir: %d,runStepCurve: %d,speedRatioMode: %d,speedRatio: %0.3f\r\n",__func__,
                    runAlwaysDir,runStepCurve,speedRatioMode,(double)speedRatio);
#endif
    //持续运转结构体
    StepMotorRunAlwaysCmd runAlwaysCmd;
    runAlwaysCmd.motorNo = STEP_MOTOR_NO9;
    runAlwaysCmd.selectConfig = runStepCurve;
    runAlwaysCmd.speedRatioMode = (SPEED_RATIO_MODE)speedRatioMode;
    runAlwaysCmd.speedRatio = speedRatioUint16;
    runAlwaysCmd.dir = (STEP_MOTOR_DIR)runAlwaysDir;
    //回传数据定义
    uint8_t* returnBufferPtr = NULL;
    uint16_t returnBufferLength = 0;
    //先设置回传数据为0
    channelCachePtr->returnDataBufferLength = 0;
    //调用API
    SrvImplBaseStepMotor9RunAlways(&runAlwaysCmd,resultPackPtr,&returnBufferPtr,&returnBufferLength);
    //对结果进行处理
    if(resultPackPtr->resultCode == PROT_RESULT_SUCCESS)
    {
        if((returnBufferPtr != NULL)&&(returnBufferLength != 0))
        {
            if(returnBufferLength > channelCachePtr->carryDataBufferLength)
            {
                //缓存太小放不下
                SystemPrintf("%s,returnBufferLength More Then Carry Buffer: %d,%d\r\n",__func__,returnBufferLength,channelCachePtr->carryDataBufferLength);
                CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_STEP_MOTOR,ERROR_LEVEL_WARNING,
                                                ERROR_SUB_BOARD_IPC_CMD_CARRY_BUFFER_LEN_NOT_ENOUGH);
                return;
            }
            else
            {
                //拷贝回传数据
                channelCachePtr->returnDataBufferLength = returnBufferLength;
                UserMemCopy(channelCachePtr->dataBufferPtr,returnBufferPtr,returnBufferLength);
            }
        }
    }
    //数据处理完成之后释放内存
    if((returnBufferPtr != NULL)&&(returnBufferLength != 0))
    {
        UserMemFree(MEM_REGION_SRV_TASK,returnBufferPtr);
    }
    return;
}

//电机立即停止
static void SrvMsgIpcStepMotor9Cmd_StopNow(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{
#if(IPC_SRV_RUNNING_LOG_SHOW == CONFIG_FUNC_ENABLE)
    //打印参数
    SystemPrintf("%s\r\n",__func__);
#endif
    //回传数据定义
    uint8_t* returnBufferPtr = NULL;
    uint16_t returnBufferLength = 0;
    //先设置回传数据为0
    channelCachePtr->returnDataBufferLength = 0;
    //调用API
    SrvImplBaseStepMotor9StopNow(resultPackPtr,&returnBufferPtr,&returnBufferLength);
    //对结果进行处理
    if(resultPackPtr->resultCode == PROT_RESULT_SUCCESS)
    {
        if((returnBufferPtr != NULL)&&(returnBufferLength != 0))
        {
            if(returnBufferLength > channelCachePtr->carryDataBufferLength)
            {
                //缓存太小放不下
                SystemPrintf("%s,returnBufferLength More Then Carry Buffer: %d,%d\r\n",__func__,returnBufferLength,channelCachePtr->carryDataBufferLength);
                CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_STEP_MOTOR,ERROR_LEVEL_WARNING,
                                                ERROR_SUB_BOARD_IPC_CMD_CARRY_BUFFER_LEN_NOT_ENOUGH);
                return;
            }
            else
            {
                //拷贝回传数据
                channelCachePtr->returnDataBufferLength = returnBufferLength;
                UserMemCopy(channelCachePtr->dataBufferPtr,returnBufferPtr,returnBufferLength);
            }
        }
    }
    //数据处理完成之后释放内存
    if((returnBufferPtr != NULL)&&(returnBufferLength != 0))
    {
        UserMemFree(MEM_REGION_SRV_TASK,returnBufferPtr);
    }
    return;
}

//电机减速停止
static void SrvMsgIpcStepMotor9Cmd_StopSlowDown(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{
    //减速模式
    uint8_t slowDownMode = (uint8_t)(ipcCmdMsgPtr->cmdParam1);
    //减速步数
    uint32_t slowDownSteps = ipcCmdMsgPtr->cmdParam2;
    //超时时间
    uint32_t timeOutMs = ipcCmdMsgPtr->cmdParam3;
#if(IPC_SRV_RUNNING_LOG_SHOW == CONFIG_FUNC_ENABLE)
    //打印参数
    SystemPrintf("%s,slowDownMode: %d,slowDownSteps: %d,timeOutMs: %d\r\n",__func__,
                    slowDownMode,slowDownSteps,timeOutMs);
#endif
    //回传数据定义
    uint8_t* returnBufferPtr = NULL;
    uint16_t returnBufferLength = 0;
    //先设置回传数据为0
    channelCachePtr->returnDataBufferLength = 0;
    //调用API
    SrvImplBaseStepMotor9StopSlowDown(slowDownMode,slowDownSteps,timeOutMs,resultPackPtr,&returnBufferPtr,&returnBufferLength);
    //对结果进行处理
    if(resultPackPtr->resultCode == PROT_RESULT_SUCCESS)
    {
        if((returnBufferPtr != NULL)&&(returnBufferLength != 0))
        {
            if(returnBufferLength > channelCachePtr->carryDataBufferLength)
            {
                //缓存太小放不下
                SystemPrintf("%s,returnBufferLength More Then Carry Buffer: %d,%d\r\n",__func__,returnBufferLength,channelCachePtr->carryDataBufferLength);
                CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_STEP_MOTOR,ERROR_LEVEL_WARNING,
                                                ERROR_SUB_BOARD_IPC_CMD_CARRY_BUFFER_LEN_NOT_ENOUGH);
                return;
            }
            else
            {
                //拷贝回传数据
                channelCachePtr->returnDataBufferLength = returnBufferLength;
                UserMemCopy(channelCachePtr->dataBufferPtr,returnBufferPtr,returnBufferLength);
            }
        }
    }
    //数据处理完成之后释放内存
    if((returnBufferPtr != NULL)&&(returnBufferLength != 0))
    {
        UserMemFree(MEM_REGION_SRV_TASK,returnBufferPtr);
    }
    return;
}

//获取电机当前坐标
static void SrvMsgIpcStepMotor9Cmd_GetCoordinate(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{
#if(IPC_SRV_RUNNING_LOG_SHOW == CONFIG_FUNC_ENABLE)
    //打印参数
    SystemPrintf("%s\r\n",__func__);
#endif
    //回传数据定义
    uint8_t* returnBufferPtr = NULL;
    uint16_t returnBufferLength = 0;
    //先设置回传数据为0
    channelCachePtr->returnDataBufferLength = 0;
    //调用API实现
    SrvImplBaseStepMotor9GetCoordinate(resultPackPtr,&returnBufferPtr,&returnBufferLength);
    //对结果进行处理
    if(resultPackPtr->resultCode == PROT_RESULT_SUCCESS)
    {
        if((returnBufferPtr != NULL)&&(returnBufferLength != 0))
        {
            if(returnBufferLength > channelCachePtr->carryDataBufferLength)
            {
                //缓存太小放不下
                SystemPrintf("%s,returnBufferLength More Then Carry Buffer: %d,%d\r\n",__func__,returnBufferLength,channelCachePtr->carryDataBufferLength);
                CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_STEP_MOTOR,ERROR_LEVEL_WARNING,
                                                ERROR_SUB_BOARD_IPC_CMD_CARRY_BUFFER_LEN_NOT_ENOUGH);
                return;
            }
            else
            {
                //拷贝回传数据
                channelCachePtr->returnDataBufferLength = returnBufferLength;
                UserMemCopy(channelCachePtr->dataBufferPtr,returnBufferPtr,returnBufferLength);
            }
        }
    }
    //数据处理完成之后释放内存
    if((returnBufferPtr != NULL)&&(returnBufferLength != 0))
    {
        UserMemFree(MEM_REGION_SRV_TASK,returnBufferPtr);
    }
    return;
}

//获取当前原点传感器状态
static void SrvMsgIpcStepMotor9Cmd_ReadOriginSensor(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{
#if(IPC_SRV_RUNNING_LOG_SHOW == CONFIG_FUNC_ENABLE)
    //打印参数
    SystemPrintf("%s\r\n",__func__);
#endif
    //回传数据定义
    uint8_t* returnBufferPtr = NULL;
    uint16_t returnBufferLength = 0;
    //先设置回传数据为0
    channelCachePtr->returnDataBufferLength = 0;
    //调用API实现
    SrvImplBaseStepMotor9ReadOriginSensor(resultPackPtr,&returnBufferPtr,&returnBufferLength);
    //对结果进行处理
    if(resultPackPtr->resultCode == PROT_RESULT_SUCCESS)
    {
        if((returnBufferPtr != NULL)&&(returnBufferLength != 0))
        {
            if(returnBufferLength > channelCachePtr->carryDataBufferLength)
            {
                //缓存太小放不下
                SystemPrintf("%s,returnBufferLength More Then Carry Buffer: %d,%d\r\n",__func__,returnBufferLength,channelCachePtr->carryDataBufferLength);
                CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_STEP_MOTOR,ERROR_LEVEL_WARNING,
                                                ERROR_SUB_BOARD_IPC_CMD_CARRY_BUFFER_LEN_NOT_ENOUGH);
                return;
            }
            else
            {
                //拷贝回传数据
                channelCachePtr->returnDataBufferLength = returnBufferLength;
                UserMemCopy(channelCachePtr->dataBufferPtr,returnBufferPtr,returnBufferLength);
            }
        }
    }
    //数据处理完成之后释放内存
    if((returnBufferPtr != NULL)&&(returnBufferLength != 0))
    {
        UserMemFree(MEM_REGION_SRV_TASK,returnBufferPtr);
    }
    return;
}



/*************************************************************************************************************************************/

/*************************************************************IPC上报讯息处理**********************************************************/
//占位函数
static void SrvMsgIpcStepMotor9Upload_PlaceHolder(IPC_UPLOAD_DATA* ipcUploadDataPtr);

//IPC上传消息处理函数列表
static const USER_IPC_UPLOAD_PROC_UNIT ipcStepMotor9MsgUploadProcUnitArray[] = 
{
    //顺序是: 指令码 预期参数最小长度 上传消息处理函数
    {(uint32_t)0XFFFFFFFF     ,2 ,SrvMsgIpcStepMotor9Upload_PlaceHolder},
};


//IPC上传消息处理函数
void SrvMsgIpcStepMotor9Upload(void* ipcMsgBodyPtr)
{
    IPC_UPLOAD_DATA* ipcUploadDataPtr = (IPC_UPLOAD_DATA*)ipcMsgBodyPtr;
    if(ipcUploadDataPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,Recv ipcUploadDataPtr NULL\r\n",__func__,__LINE__);
        //空指针异常
        return;
    }
    //搜索匹配处理器
    uint16_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < DIM_ARRAY_ELEMENT_COUNT(ipcStepMotor9MsgUploadProcUnitArray);indexUtil++)
    {
        if(ipcUploadDataPtr->uploadID == (uint32_t)(ipcStepMotor9MsgUploadProcUnitArray[indexUtil].uploadId))
        {
            break;
        }
    }
    //判定序号
    if(indexUtil == DIM_ARRAY_ELEMENT_COUNT(ipcStepMotor9MsgUploadProcUnitArray))
    {
        //不支持的指令
        SystemPrintf("E:> %s,%d,Recv Unsupport UploadID: 0X%08X\r\n",__func__,__LINE__,ipcUploadDataPtr->uploadID);
        //释放内存
        if((ipcUploadDataPtr->dataBufferPtr != NULL)&&(ipcUploadDataPtr->dataLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_IPC,ipcUploadDataPtr->dataBufferPtr);
        }
        UserMemFree(MEM_REGION_SYS_MSG_IPC,ipcUploadDataPtr);
        return;
    }
    //判定长度
    if(ipcUploadDataPtr->dataLength < ipcStepMotor9MsgUploadProcUnitArray[indexUtil].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,UploadID: 0X%08X\r\n",__func__,__LINE__,
                        ipcStepMotor9MsgUploadProcUnitArray[indexUtil].exceptParamLengthMin,ipcUploadDataPtr->dataLength,
                        ipcUploadDataPtr->uploadID);
        //释放内存
        if((ipcUploadDataPtr->dataBufferPtr != NULL)&&(ipcUploadDataPtr->dataLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_IPC,ipcUploadDataPtr->dataBufferPtr);
        }
        UserMemFree(MEM_REGION_SYS_MSG_IPC,ipcUploadDataPtr);
        return;
    }
    //检测函数指针
    if(ipcStepMotor9MsgUploadProcUnitArray[indexUtil].ipcUploadProcFuncPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,ipcUploadProcFuncPtr NULL,UploadID: 0X%08X\r\n",__func__,__LINE__,
                        ipcUploadDataPtr->uploadID);
        //释放内存
        if((ipcUploadDataPtr->dataBufferPtr != NULL)&&(ipcUploadDataPtr->dataLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_IPC,ipcUploadDataPtr->dataBufferPtr);
        }
        UserMemFree(MEM_REGION_SYS_MSG_IPC,ipcUploadDataPtr);
        return;
    }
    //调用指令处理
    ipcStepMotor9MsgUploadProcUnitArray[indexUtil].ipcUploadProcFuncPtr(ipcUploadDataPtr);
    //处理完成,释放内存
    if((ipcUploadDataPtr->dataBufferPtr != NULL)&&(ipcUploadDataPtr->dataLength != 0))
    {
        UserMemFree(MEM_REGION_SYS_MSG_IPC,ipcUploadDataPtr->dataBufferPtr);
    }
    UserMemFree(MEM_REGION_SYS_MSG_IPC,ipcUploadDataPtr);
}

//占位函数
static void SrvMsgIpcStepMotor9Upload_PlaceHolder(IPC_UPLOAD_DATA* ipcUploadDataPtr)
{

}

/*************************************************************************************************************************************/










