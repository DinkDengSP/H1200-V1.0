/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-16 11:35:28
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-24 14:20:37
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#include "SrvImplStepMotor16.h"
#include "SrvImplBaseStepMotor16.h"


/*************************************************************IPC指令处理处理***********************************************************/
//通讯测试
static void SrvMsgIpcStepMotor16Cmd_CommTest(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//电机复位
static void SrvMsgIpcStepMotor16Cmd_Reset(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//电机走步数
static void SrvMsgIpcStepMotor16Cmd_RunStep(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//电机走坐标
static void SrvMsgIpcStepMotor16Cmd_RunCoordinate(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//电机回零
static void SrvMsgIpcStepMotor16Cmd_ReturnZero(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//电机持续运转
static void SrvMsgIpcStepMotor16Cmd_RunAlways(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//电机立即停止
static void SrvMsgIpcStepMotor16Cmd_StopNow(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//电机减速停止
static void SrvMsgIpcStepMotor16Cmd_StopSlowDown(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//获取电机当前坐标
static void SrvMsgIpcStepMotor16Cmd_GetCoordinate(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//获取当前原点传感器状态
static void SrvMsgIpcStepMotor16Cmd_ReadOriginSensor(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//电机失能
static void SrvMsgIpcStepMotor16Cmd_Disable(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//电机进入教学模式
static void SrvMsgIpcStepMotor16Cmd_EnterTeachMode(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//电机强行进入默认状态
static void SrvMsgIpcStepMotor16Cmd_ForceToDefault(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);

//IPC指令处理函数列表
static const USER_IPC_CMD_PROC_UNIT ipcStepMotor16MsgCmdProcUnitArray[] = 
{
    //顺序是 指令码 预期参数最小长度  指令处理函数
    {(uint32_t)0XFFFFFFFF     ,0    ,SrvMsgIpcStepMotor16Cmd_CommTest  },
    {(uint32_t)0X00000000     ,0    ,SrvMsgIpcStepMotor16Cmd_Reset               },
    {(uint32_t)0X00000001     ,0    ,SrvMsgIpcStepMotor16Cmd_RunStep             },
    {(uint32_t)0X00000002     ,0    ,SrvMsgIpcStepMotor16Cmd_RunCoordinate       },
    {(uint32_t)0X00000003     ,0    ,SrvMsgIpcStepMotor16Cmd_ReturnZero          },
    {(uint32_t)0X00000004     ,0    ,SrvMsgIpcStepMotor16Cmd_RunAlways           },
    {(uint32_t)0X00000005     ,0    ,SrvMsgIpcStepMotor16Cmd_StopNow             },
    {(uint32_t)0X00000006     ,0    ,SrvMsgIpcStepMotor16Cmd_StopSlowDown        },
    {(uint32_t)0X00000007     ,0    ,SrvMsgIpcStepMotor16Cmd_GetCoordinate       },
    {(uint32_t)0X00000008     ,0    ,SrvMsgIpcStepMotor16Cmd_ReadOriginSensor    },
    {(uint32_t)0X00000009     ,0    ,SrvMsgIpcStepMotor16Cmd_Disable             },
    {(uint32_t)0X0000000A     ,0    ,SrvMsgIpcStepMotor16Cmd_EnterTeachMode      },
    {(uint32_t)0X0000000B     ,0    ,SrvMsgIpcStepMotor16Cmd_ForceToDefault      },
};


//IPC指令消息处理函数
void SrvMsgIpcStepMotor16Cmd(void* ipcMsgBodyPtr)
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
    uint16_t indexStepMotor16 = 0;
    for(indexStepMotor16 = 0; indexStepMotor16 < DIM_ARRAY_ELEMENT_COUNT(ipcStepMotor16MsgCmdProcUnitArray);indexStepMotor16++)
    {
        if(ipcCmdMsgPtr->cmdBaseData.commandCode == (uint32_t)(ipcStepMotor16MsgCmdProcUnitArray[indexStepMotor16].commandCode))
        {
            break;
        }
    }
    //判定序号
    if(indexStepMotor16 == DIM_ARRAY_ELEMENT_COUNT(ipcStepMotor16MsgCmdProcUnitArray))
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
    if(ipcCmdMsgPtr->cmdBaseData.cmdParamBufferLength < ipcStepMotor16MsgCmdProcUnitArray[indexStepMotor16].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,CommandCode: 0X%08X\r\n",__func__,__LINE__,
                        ipcStepMotor16MsgCmdProcUnitArray[indexStepMotor16].exceptParamLengthMin,ipcCmdMsgPtr->cmdBaseData.cmdParamBufferLength,
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
    if(ipcStepMotor16MsgCmdProcUnitArray[indexStepMotor16].ipcCmdProcFuncPtr == NULL)
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
    ipcStepMotor16MsgCmdProcUnitArray[indexStepMotor16].ipcCmdProcFuncPtr(&(ipcCmdMsgPtr->cmdBaseData),&sysResultPack,ipcCmdMsgPtr->channelCachePtr);
    //处理完成回传
    ipcWaitResultPackPtr->resultPack.resultCode = sysResultPack.resultCode;
    ipcWaitResultPackPtr->resultPack.errorMain = sysResultPack.errorMain;
    ipcWaitResultPackPtr->resultPack.errorLevel = sysResultPack.errorLevel;
    ipcWaitResultPackPtr->resultPack.errorSub = sysResultPack.errorSub;
    //打印错误
    if(ipcWaitResultPackPtr->resultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        SystemPrintf("%s,Cmd: 0X%04X,ResultCode: %d,ErrorMain: 0X%04X,ErrorLevel: %d,ErrorSub: 0X%08X\r\n",
                        __func__,ipcCmdMsgPtr->cmdBaseData.commandCode,sysResultPack.errorMain,sysResultPack.errorLevel,
                        sysResultPack.errorSub);
    }
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
static void SrvMsgIpcStepMotor16Cmd_CommTest(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
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
static void SrvMsgIpcStepMotor16Cmd_Reset(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
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
    stepMotorResetCmd.motorNo = STEP_MOTOR_NO16;
    stepMotorResetCmd.selectCruveCorrection = (SM_CURVE_SELECT)resetCorrectCurve;
    stepMotorResetCmd.correctionCoordinate = resetCorrectPos;
    stepMotorResetCmd.timeOutMsSet = timeOutMs;
    //回传数据定义
    uint8_t* returnBufferPtr = NULL;
    uint16_t returnBufferLength = 0;
    //先设置回传数据为0
    channelCachePtr->returnDataBufferLength = 0;
    //调用API
    SrvImplBaseStepMotor16Reset(&stepMotorResetCmd,resultPackPtr,&returnBufferPtr,&returnBufferLength);
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
static void SrvMsgIpcStepMotor16Cmd_RunStep(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{
    //总步数
    int32_t totalSteps = (int32_t)(ipcCmdMsgPtr->cmdParam1);
    //运行曲线
    uint8_t motorRunCurve = (uint8_t)(ipcCmdMsgPtr->cmdParam2);
    //速度比例模式
    uint8_t speedRatioMode = (uint8_t)((ipcCmdMsgPtr->cmdParam2)>>8);
    //速度比例
    uint16_t speedRatioUint16 = (uint16_t)((ipcCmdMsgPtr->cmdParam2)>>16);
    if(speedRatioUint16 > 100)speedRatioUint16 = 100;
    if(speedRatioUint16 == 0)speedRatioUint16 = 1;
    float speedRatio = (float)(((float)speedRatioUint16)/100.0f);
    //换向修正
    uint32_t dirSwitchCorrect = ipcCmdMsgPtr->cmdParam3;
    FunctionalState emStopEnable = (FunctionalState)(ipcCmdMsgPtr->cmdParam4);
    FunctionalState utilStopEnable = (FunctionalState)((ipcCmdMsgPtr->cmdParam4)>>8);
    //超时时间
    uint32_t timeOutMs = ipcCmdMsgPtr->cmdParam5;
#if(IPC_SRV_RUNNING_LOG_SHOW == CONFIG_FUNC_ENABLE)
    //打印参数
    SystemPrintf("%s,totalSteps: %d,motorRunCurve: %d,speedRatioMode: %d,speedRatio: %0.3f,dirSwitchCorrect: %d\r\n",__func__,totalSteps,
                    motorRunCurve,speedRatioMode,(double)speedRatio,dirSwitchCorrect);
    SystemPrintf("%s,emStopEnable: %d,utilStopEnable: %d,\r\n",__func__,emStopEnable,utilStopEnable);
#endif
    //走步数结构体
    StepMotorRunStepsCmd runStepCmd;
    runStepCmd.motorNo = STEP_MOTOR_NO16;
    runStepCmd.totalSteps = totalSteps;
    runStepCmd.dirSwitchCorrectSteps = dirSwitchCorrect;
    runStepCmd.selectCurveConfig = (SM_CURVE_SELECT)motorRunCurve;
    runStepCmd.speedRatioMode = (SPEED_RATIO_MODE)speedRatioMode;
    runStepCmd.speedRatio = speedRatio;
    runStepCmd.emStopSensorSet = emStopEnable;
    runStepCmd.utilStopSensorSet = utilStopEnable;
    runStepCmd.timeOutMsSet = timeOutMs;
    //回传数据定义
    uint8_t* returnBufferPtr = NULL;
    uint16_t returnBufferLength = 0;
    //先设置回传数据为0
    channelCachePtr->returnDataBufferLength = 0;
    //调用API
    SrvImplBaseStepMotor16RunStep(&runStepCmd,resultPackPtr,&returnBufferPtr,&returnBufferLength);
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
static void SrvMsgIpcStepMotor16Cmd_RunCoordinate(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{
    //目标坐标
    int32_t targetCoordinate = (int32_t)(ipcCmdMsgPtr->cmdParam1);
    //运行曲线
    uint8_t motorRunCurve = (uint8_t)(ipcCmdMsgPtr->cmdParam2);
    //速度比例模式
    uint8_t speedRatioMode = (uint8_t)((ipcCmdMsgPtr->cmdParam2)>>8);
    //速度比例
    uint16_t speedRatioUint16 = (uint16_t)((ipcCmdMsgPtr->cmdParam2)>>16);
    if(speedRatioUint16 > 100)speedRatioUint16 = 100;
    if(speedRatioUint16 == 0)speedRatioUint16 = 1;
    float speedRatio = (float)(((float)speedRatioUint16)/100.0f);
    //换向修正
    uint32_t dirSwitchCorrect = ipcCmdMsgPtr->cmdParam3;
    FunctionalState emStopEnable = (FunctionalState)(ipcCmdMsgPtr->cmdParam4);
    FunctionalState utilStopEnable = (FunctionalState)((ipcCmdMsgPtr->cmdParam4)>>8);
    //超时时间
    uint32_t timeOutMs = ipcCmdMsgPtr->cmdParam5;
#if(IPC_SRV_RUNNING_LOG_SHOW == CONFIG_FUNC_ENABLE)
    //打印参数
    SystemPrintf("%s,targetCoordinate: %d,motorRunCurve: %d,speedRatioMode: %d,speedRatio: %0.3f,dirSwitchCorrect: %d\r\n",__func__,targetCoordinate,
                    motorRunCurve,speedRatioMode,(double)speedRatio,dirSwitchCorrect);
    SystemPrintf("%s,emStopEnable: %d,utilStopEnable: %d\r\n",__func__,emStopEnable,utilStopEnable);
#endif
    //走坐标结构体
    StepMotorRun2CoordationCmd runCoordinateCmd;
    runCoordinateCmd.motorNo = STEP_MOTOR_NO16;
    runCoordinateCmd.targetCoordinate = targetCoordinate;
    runCoordinateCmd.selectCurveConfig = (SM_CURVE_SELECT)motorRunCurve;
    runCoordinateCmd.speedRatioMode = (SPEED_RATIO_MODE)speedRatioMode;
    runCoordinateCmd.speedRatio = speedRatio;
    runCoordinateCmd.dirSwitchCorrectSteps = dirSwitchCorrect;
    runCoordinateCmd.emStopSensorSet = emStopEnable;
    runCoordinateCmd.utilStopSensorSet = utilStopEnable;
    runCoordinateCmd.timeOutMsSet = timeOutMs;
    //回传数据定义
    uint8_t* returnBufferPtr = NULL;
    uint16_t returnBufferLength = 0;
    //先设置回传数据为0
    channelCachePtr->returnDataBufferLength = 0;
    //调用API
    SrvImplBaseStepMotor16RunCoordinate(&runCoordinateCmd,resultPackPtr,&returnBufferPtr,&returnBufferLength);
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
static void SrvMsgIpcStepMotor16Cmd_ReturnZero(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{
    //回零后修正曲线
    int32_t returnZeroCorrectPos = (int32_t)(ipcCmdMsgPtr->cmdParam1);
    //运行曲线
    uint8_t motorRunCurve = (uint8_t)(ipcCmdMsgPtr->cmdParam2);
    //速度比例模式
    uint8_t speedRatioMode = (uint8_t)((ipcCmdMsgPtr->cmdParam2)>>8);
    //速度比例
    uint16_t speedRatioUint16 = (uint16_t)((ipcCmdMsgPtr->cmdParam2)>>16);
    if(speedRatioUint16 > 100)speedRatioUint16 = 100;
    if(speedRatioUint16 == 0)speedRatioUint16 = 1;
    float speedRatio = (float)(((float)speedRatioUint16)/100.0f);
    //换向修正
    uint32_t dirSwitchCorrect = ipcCmdMsgPtr->cmdParam3;
    FunctionalState emStopEnable = (FunctionalState)(ipcCmdMsgPtr->cmdParam4);
    FunctionalState utilStopEnable = (FunctionalState)((ipcCmdMsgPtr->cmdParam4)>>8);
    //回零是否复位
    uint8_t needReset = (uint8_t)((ipcCmdMsgPtr->cmdParam4)>>24);
    //超时时间
    uint32_t timeOutMs = ipcCmdMsgPtr->cmdParam5;
#if(IPC_SRV_RUNNING_LOG_SHOW == CONFIG_FUNC_ENABLE)
    //打印参数
    SystemPrintf("%s,returnZeroCorrectPos: %d,motorRunCurve: %d,speedRatioMode: %d,speedRatio: %0.3f,dirSwitchCorrect: %d\r\n",__func__,
                    returnZeroCorrectPos,motorRunCurve,speedRatioMode,(double)speedRatio,dirSwitchCorrect);
    SystemPrintf("%s,NeedReset: %d,emStopEnable: %d,utilStopEnable: %d\r\n",__func__,needReset,emStopEnable,utilStopEnable);
#endif
    //回零结构体
    StepMotorReturnZeroCmd returnZeroCmd;
    returnZeroCmd.motorNo = STEP_MOTOR_NO16;
    returnZeroCmd.correctionCoordinate = returnZeroCorrectPos;
    returnZeroCmd.selectCurveConfig = (SM_CURVE_SELECT)motorRunCurve;
    returnZeroCmd.speedRatioMode = (SPEED_RATIO_MODE)speedRatioMode;
    returnZeroCmd.speedRatio = speedRatio;
    returnZeroCmd.resetFlag = (FunctionalState)needReset;
    returnZeroCmd.dirSwitchCorrectSteps = dirSwitchCorrect;
    returnZeroCmd.emStopSensorSet = emStopEnable;
    returnZeroCmd.utilStopSensorSet = utilStopEnable;
    returnZeroCmd.timeOutMsSet = timeOutMs;
    //回传数据定义
    uint8_t* returnBufferPtr = NULL;
    uint16_t returnBufferLength = 0;
    //先设置回传数据为0
    channelCachePtr->returnDataBufferLength = 0;
    //调用API
    SrvImplBaseStepMotor16ReturnZero(&returnZeroCmd,resultPackPtr,&returnBufferPtr,&returnBufferLength);
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
static void SrvMsgIpcStepMotor16Cmd_RunAlways(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{
    //运行方向
    uint8_t runAlwaysDir = (uint8_t)(ipcCmdMsgPtr->cmdParam1);
    //运行曲线
    uint8_t motorRunCurve = (uint8_t)(ipcCmdMsgPtr->cmdParam2);
    //速度比例模式
    uint8_t speedRatioMode = (uint8_t)((ipcCmdMsgPtr->cmdParam2)>>8);
    //速度比例
    uint16_t speedRatioUint16 = (uint16_t)((ipcCmdMsgPtr->cmdParam2)>>16);
    if(speedRatioUint16 > 100)speedRatioUint16 = 100;
    if(speedRatioUint16 == 0)speedRatioUint16 = 1;
    float speedRatio = (float)(((float)speedRatioUint16)/100.0f);
#if(IPC_SRV_RUNNING_LOG_SHOW == CONFIG_FUNC_ENABLE)
    //打印参数
    SystemPrintf("%s,runAlwaysDir: %d,motorRunCurve: %d,speedRatioMode: %d,speedRatio: %0.3f\r\n",__func__,
                    runAlwaysDir,motorRunCurve,speedRatioMode,(double)speedRatio);
#endif
    //持续运转结构体
    StepMotorRunAlwaysCmd runAlwaysCmd;
    runAlwaysCmd.motorNo = STEP_MOTOR_NO16;
    runAlwaysCmd.selectCurveConfig = (SM_CURVE_SELECT)motorRunCurve;
    runAlwaysCmd.speedRatioMode = (SPEED_RATIO_MODE)speedRatioMode;
    runAlwaysCmd.speedRatio = speedRatio;
    runAlwaysCmd.motorDir = (STEP_MOTOR_DIR)runAlwaysDir;
    //回传数据定义
    uint8_t* returnBufferPtr = NULL;
    uint16_t returnBufferLength = 0;
    //先设置回传数据为0
    channelCachePtr->returnDataBufferLength = 0;
    //调用API
    SrvImplBaseStepMotor16RunAlways(&runAlwaysCmd,resultPackPtr,&returnBufferPtr,&returnBufferLength);
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
static void SrvMsgIpcStepMotor16Cmd_StopNow(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
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
    SrvImplBaseStepMotor16StopNow(resultPackPtr,&returnBufferPtr,&returnBufferLength);
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
static void SrvMsgIpcStepMotor16Cmd_StopSlowDown(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
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
    SrvImplBaseStepMotor16StopSlowDown(slowDownMode,slowDownSteps,timeOutMs,resultPackPtr,&returnBufferPtr,&returnBufferLength);
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
static void SrvMsgIpcStepMotor16Cmd_GetCoordinate(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
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
    SrvImplBaseStepMotor16GetCoordinate(resultPackPtr,&returnBufferPtr,&returnBufferLength);
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
static void SrvMsgIpcStepMotor16Cmd_ReadOriginSensor(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
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
    SrvImplBaseStepMotor16ReadOriginSensor(resultPackPtr,&returnBufferPtr,&returnBufferLength);
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

//电机失能
static void SrvMsgIpcStepMotor16Cmd_Disable(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
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
    SrvImplBaseStepMotor16Disable(resultPackPtr,&returnBufferPtr,&returnBufferLength);
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

//电机进入教学模式
static void SrvMsgIpcStepMotor16Cmd_EnterTeachMode(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
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
    SrvImplBaseStepMotor16EnterTeachMode(resultPackPtr,&returnBufferPtr,&returnBufferLength);
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

//电机强行进入默认状态
static void SrvMsgIpcStepMotor16Cmd_ForceToDefault(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
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
    SrvImplBaseStepMotor16ForceToDefault(resultPackPtr,&returnBufferPtr,&returnBufferLength);
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
static void SrvMsgIpcStepMotor16Upload_PlaceHolder(IPC_UPLOAD_DATA* ipcUploadDataPtr);

//IPC上传消息处理函数列表
static const USER_IPC_UPLOAD_PROC_UNIT ipcStepMotor16MsgUploadProcUnitArray[] = 
{
    //顺序是: 指令码 预期参数最小长度 上传消息处理函数
    {(uint32_t)0XFFFFFFFF     ,2 ,SrvMsgIpcStepMotor16Upload_PlaceHolder},
};


//IPC上传消息处理函数
void SrvMsgIpcStepMotor16Upload(void* ipcMsgBodyPtr)
{
    IPC_UPLOAD_DATA* ipcUploadDataPtr = (IPC_UPLOAD_DATA*)ipcMsgBodyPtr;
    if(ipcUploadDataPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,Recv ipcUploadDataPtr NULL\r\n",__func__,__LINE__);
        //空指针异常
        return;
    }
    //搜索匹配处理器
    uint16_t indexStepMotor16 = 0;
    for(indexStepMotor16 = 0; indexStepMotor16 < DIM_ARRAY_ELEMENT_COUNT(ipcStepMotor16MsgUploadProcUnitArray);indexStepMotor16++)
    {
        if(ipcUploadDataPtr->uploadID == (uint32_t)(ipcStepMotor16MsgUploadProcUnitArray[indexStepMotor16].uploadId))
        {
            break;
        }
    }
    //判定序号
    if(indexStepMotor16 == DIM_ARRAY_ELEMENT_COUNT(ipcStepMotor16MsgUploadProcUnitArray))
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
    if(ipcUploadDataPtr->dataLength < ipcStepMotor16MsgUploadProcUnitArray[indexStepMotor16].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,UploadID: 0X%08X\r\n",__func__,__LINE__,
                        ipcStepMotor16MsgUploadProcUnitArray[indexStepMotor16].exceptParamLengthMin,ipcUploadDataPtr->dataLength,
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
    if(ipcStepMotor16MsgUploadProcUnitArray[indexStepMotor16].ipcUploadProcFuncPtr == NULL)
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
    ipcStepMotor16MsgUploadProcUnitArray[indexStepMotor16].ipcUploadProcFuncPtr(ipcUploadDataPtr);
    //处理完成,释放内存
    if((ipcUploadDataPtr->dataBufferPtr != NULL)&&(ipcUploadDataPtr->dataLength != 0))
    {
        UserMemFree(MEM_REGION_SYS_MSG_IPC,ipcUploadDataPtr->dataBufferPtr);
    }
    UserMemFree(MEM_REGION_SYS_MSG_IPC,ipcUploadDataPtr);
}

//占位函数
static void SrvMsgIpcStepMotor16Upload_PlaceHolder(IPC_UPLOAD_DATA* ipcUploadDataPtr)
{

}

/*************************************************************************************************************************************/











