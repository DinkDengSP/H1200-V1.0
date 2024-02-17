/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-31 08:43:34 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-01 16:36:41 +0800
************************************************************************************************/ 
#include "SrvImplStepMotor7.h"
#include "SrvImplBaseStepMotor7.h"
#include "CommonSrvTaskHeader.h"

/*******************************************CAN指令处理函数列表*************************************************/
//占位函数
static void SrvMsgCan1StepMotor7Cmd_PlaceHolder(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//电机复位
static void SrvMsgCan1StepMotor7Cmd_Reset(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//电机走步数
static void SrvMsgCan1StepMotor7Cmd_RunStep(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//电机走坐标
static void SrvMsgCan1StepMotor7Cmd_RunCoordinate(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//电机回零
static void SrvMsgCan1StepMotor7Cmd_ReturnZero(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//电机持续运转
static void SrvMsgCan1StepMotor7Cmd_RunAlways(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//电机立即停止
static void SrvMsgCan1StepMotor7Cmd_StopNow(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//电机减速停止
static void SrvMsgCan1StepMotor7Cmd_StopSlowDown(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//获取电机当前坐标
static void SrvMsgCan1StepMotor7Cmd_GetCoordinate(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//获取当前原点传感器状态
static void SrvMsgCan1StepMotor7Cmd_ReadOriginSensor(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);


//CAN指令处理函数列表
static const USER_CAN_CMD_PROC_UNIT can1CmdProcStepMotor7UnitArray[] = 
{
    //顺序是 指令码 预期参数最小长度 是否需要结果返回  错误是否独立上报 指令处理函数
    {CAN_MASTER_CMD_PLACE_HOLDER            ,0  ,ENABLE ,SrvMsgCan1StepMotor7Cmd_PlaceHolder  },
    {CAN_MASTER_CMD_SM_RESET                ,0  ,ENABLE ,SrvMsgCan1StepMotor7Cmd_Reset             },
    {CAN_MASTER_CMD_SM_RUN_STEPS            ,4  ,ENABLE ,SrvMsgCan1StepMotor7Cmd_RunStep           },
    {CAN_MASTER_CMD_SM_RUN_COORDINATE       ,4  ,ENABLE ,SrvMsgCan1StepMotor7Cmd_RunCoordinate     },
    {CAN_MASTER_CMD_SM_RETURN_ZERO          ,0  ,ENABLE ,SrvMsgCan1StepMotor7Cmd_ReturnZero        },
    {CAN_MASTER_CMD_SM_RUN_ALWAYS           ,1  ,ENABLE ,SrvMsgCan1StepMotor7Cmd_RunAlways         },
    {CAN_MASTER_CMD_SM_STOP_NOW             ,0  ,ENABLE ,SrvMsgCan1StepMotor7Cmd_StopNow           },
    {CAN_MASTER_CMD_SM_STOP_SLOW_DOWN       ,0  ,ENABLE ,SrvMsgCan1StepMotor7Cmd_StopSlowDown      },
    {CAN_MASTER_CMD_SM_GET_COORDINATE       ,0  ,ENABLE ,SrvMsgCan1StepMotor7Cmd_GetCoordinate     },
    {CAN_MASTER_CMD_SM_READ_ORIGIN_SENSOR   ,0  ,ENABLE ,SrvMsgCan1StepMotor7Cmd_ReadOriginSensor  },
};


//CAN指令消息处理函数
void SrvMsgCan1StepMotor7Cmd(void* can1MsgBodyPtr)
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
    for(indexUtil = 0; indexUtil < DIM_ARRAY_ELEMENT_COUNT(can1CmdProcStepMotor7UnitArray);indexUtil++)
    {
        if(cmdPackPtr->cmdValue == (uint32_t)(can1CmdProcStepMotor7UnitArray[indexUtil].commandCode))
        {
            break;
        }
    }
    //判定序号
    if(indexUtil == DIM_ARRAY_ELEMENT_COUNT(can1CmdProcStepMotor7UnitArray))
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
    if(cmdPackPtr->paramBufferLength < can1CmdProcStepMotor7UnitArray[indexUtil].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,CommandCode: 0X%08X\r\n",__func__,__LINE__,
                        can1CmdProcStepMotor7UnitArray[indexUtil].exceptParamLengthMin,cmdPackPtr->paramBufferLength,
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
    if(can1CmdProcStepMotor7UnitArray[indexUtil].canCmdProcFuncPtr == NULL)
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
    can1CmdProcStepMotor7UnitArray[indexUtil].canCmdProcFuncPtr(cmdPackPtr,&resultPack,&resultDataBufferPtr,&resultDataLength);
    //结果处理,是否需要结果回传
    if(can1CmdProcStepMotor7UnitArray[indexUtil].needResultReply == ENABLE)
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
static void SrvMsgCan1StepMotor7Cmd_PlaceHolder(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//电机复位
static void SrvMsgCan1StepMotor7Cmd_Reset(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //复位修正
    int32_t resetCorrectPos = ProtUtilReadInt32Buffer(cmdPackPtr->paramBufferPtr);
    //复位修正曲线
    uint8_t resetCorrectCurve = cmdPackPtr->paramBufferPtr[4];
    //超时时间
    uint32_t timeOutMs = ProtUtilReadUint32Buffer(cmdPackPtr->paramBufferPtr+5);
    //打印参数
    SystemPrintf("%s,resetCorrectPos: %d,resetCorrectCurve: %d,timeOutMs: %d\r\n",__func__,resetCorrectPos,
                    resetCorrectCurve,timeOutMs);
    //电机复位
    StepMotorResetCmd stepMotorResetCmd;
    stepMotorResetCmd.motorNo = STEP_MOTOR_NO7;
    stepMotorResetCmd.correctionCoordinate = resetCorrectPos;
    stepMotorResetCmd.timeOutSet = timeOutMs;
    //调用实现API
    SrvImplBaseStepMotor7Reset(&stepMotorResetCmd,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//电机走步数
static void SrvMsgCan1StepMotor7Cmd_RunStep(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //总步数
    int32_t totalSteps = ProtUtilReadInt32Buffer(cmdPackPtr->paramBufferPtr);
    //运行曲线
    uint8_t runStepCurve = cmdPackPtr->paramBufferPtr[4];
    //速度比例模式
    uint8_t speedRatioMode = cmdPackPtr->paramBufferPtr[5];
    //速度比例
    uint16_t speedRatioUint16 = ProtUtilReadUint16Buffer(cmdPackPtr->paramBufferPtr+6);
    if(speedRatioUint16 > 100)speedRatioUint16 = 100;
    if(speedRatioUint16 == 0)speedRatioUint16 = 1;
    float speedRatio = (float)(((float)speedRatioUint16)/100.0f);
    //换向修正
    uint32_t dirSwitchCorrect = ProtUtilReadUint32Buffer(cmdPackPtr->paramBufferPtr+8);
    uint8_t emStopEnable = cmdPackPtr->paramBufferPtr[12];
    uint8_t utilStop1Enable = cmdPackPtr->paramBufferPtr[13];
    uint8_t utilStop2Enable = cmdPackPtr->paramBufferPtr[14];
    //超时时间
    uint32_t timeOutMs = ProtUtilReadUint32Buffer(cmdPackPtr->paramBufferPtr+15);
    //打印参数
    SystemPrintf("%s,totalSteps: %d,runStepCurve: %d,speedRatioMode: %d,speedRatio: %0.3f,dirSwitchCorrect: %d\r\n",__func__,totalSteps,
                    runStepCurve,speedRatioMode,(double)speedRatio,dirSwitchCorrect);
    SystemPrintf("%s,emStopEnable: %d,utilStop1Enable: %d,utilStop2Enable: %d\r\n",__func__,emStopEnable,utilStop1Enable,utilStop2Enable);
    //走步数结构体
    StepMotorRunStepsCmd runStepCmd;
    runStepCmd.motorNo = STEP_MOTOR_NO7;
    runStepCmd.specialSteps = totalSteps;
    runStepCmd.selectConfig = runStepCurve;
    runStepCmd.speedRatioMode = (SPEED_RATIO_MODE)speedRatioMode;
    runStepCmd.speedRatio = speedRatioUint16;
    runStepCmd.utilStop1Enable = utilStop1Enable;
    runStepCmd.utilStop2Enable = utilStop2Enable;
    runStepCmd.timeOutSet = timeOutMs;
    //执行走步数
    SrvImplBaseStepMotor7RunStep(&runStepCmd,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//电机走坐标
static void SrvMsgCan1StepMotor7Cmd_RunCoordinate(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //目标坐标
    int32_t targetCoordinate = ProtUtilReadInt32Buffer(cmdPackPtr->paramBufferPtr);
    //运行曲线
    uint8_t runStepCurve = cmdPackPtr->paramBufferPtr[4];
    //速度比例模式
    uint8_t speedRatioMode = cmdPackPtr->paramBufferPtr[5];
    //速度比例
    uint16_t speedRatioUint16 = ProtUtilReadUint16Buffer(cmdPackPtr->paramBufferPtr+6);
    if(speedRatioUint16 > 100)speedRatioUint16 = 100;
    if(speedRatioUint16 == 0)speedRatioUint16 = 1;
    float speedRatio = (float)(((float)speedRatioUint16)/100.0f);
    //换向修正
    uint32_t dirSwitchCorrect = ProtUtilReadUint32Buffer(cmdPackPtr->paramBufferPtr+8);
    uint8_t emStopEnable = cmdPackPtr->paramBufferPtr[12];
    uint8_t utilStop1Enable = cmdPackPtr->paramBufferPtr[13];
    uint8_t utilStop2Enable = cmdPackPtr->paramBufferPtr[14];
    //超时时间
    uint32_t timeOutMs = ProtUtilReadUint32Buffer(cmdPackPtr->paramBufferPtr+15);
    //打印参数
    SystemPrintf("%s,targetCoordinate: %d,runStepCurve: %d,speedRatioMode: %d,speedRatio: %0.3f,dirSwitchCorrect: %d\r\n",__func__,targetCoordinate,
                    runStepCurve,speedRatioMode,(double)speedRatio,dirSwitchCorrect);
    SystemPrintf("%s,emStopEnable: %d,utilStop1Enable: %d,utilStop2Enable: %d\r\n",__func__,emStopEnable,utilStop1Enable,utilStop2Enable);
    //走坐标结构体
    StepMotorRun2CoordationCmd runCoordinateCmd;
    runCoordinateCmd.motorNo = STEP_MOTOR_NO7;
    runCoordinateCmd.targetCoordinate = targetCoordinate;
    runCoordinateCmd.selectConfig = runStepCurve;
    runCoordinateCmd.speedRatioMode = (SPEED_RATIO_MODE)speedRatioMode;
    runCoordinateCmd.speedRatio = speedRatioUint16;
    runCoordinateCmd.utilStop1Enable = utilStop1Enable;
    runCoordinateCmd.utilStop2Enable = utilStop2Enable;
    runCoordinateCmd.timeOutSet = timeOutMs;
    //执行走坐标
    SrvImplBaseStepMotor7RunCoordinate(&runCoordinateCmd,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//电机回零
static void SrvMsgCan1StepMotor7Cmd_ReturnZero(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //回零后修正曲线
    int32_t returnZeroCorrectPos = ProtUtilReadInt32Buffer(cmdPackPtr->paramBufferPtr);
    //运行曲线
    uint8_t runStepCurve = cmdPackPtr->paramBufferPtr[4];
    //速度比例模式
    uint8_t speedRatioMode = cmdPackPtr->paramBufferPtr[5];
    //速度比例
    uint16_t speedRatioUint16 = ProtUtilReadUint16Buffer(cmdPackPtr->paramBufferPtr+6);
    if(speedRatioUint16 > 100)speedRatioUint16 = 100;
    if(speedRatioUint16 == 0)speedRatioUint16 = 1;
    float speedRatio = (float)(((float)speedRatioUint16)/100.0f);
    //回零是否复位
    uint8_t needReset = cmdPackPtr->paramBufferPtr[8];
    //换向修正
    uint32_t dirSwitchCorrect = ProtUtilReadUint32Buffer(cmdPackPtr->paramBufferPtr+9);
    uint8_t emStopEnable = cmdPackPtr->paramBufferPtr[13];
    uint8_t utilStop1Enable = cmdPackPtr->paramBufferPtr[14];
    uint8_t utilStop2Enable = cmdPackPtr->paramBufferPtr[15];
    //超时时间
    uint32_t timeOutMs = ProtUtilReadUint32Buffer(cmdPackPtr->paramBufferPtr+16);
    //打印参数
    SystemPrintf("%s,returnZeroCorrectPos: %d,runStepCurve: %d,speedRatioMode: %d,speedRatio: %0.3f,dirSwitchCorrect: %d\r\n",__func__,
                    returnZeroCorrectPos,runStepCurve,speedRatioMode,(double)speedRatio,dirSwitchCorrect);
    SystemPrintf("%s,NeedReset: %d,emStopEnable: %d,utilStop1Enable: %d,utilStop2Enable: %d\r\n",__func__,needReset,emStopEnable,utilStop1Enable,
                    utilStop2Enable);
    //回零结构体
    StepMotorReturnZeroCmd returnZeroCmd;
    returnZeroCmd.motorNo = STEP_MOTOR_NO7;
    returnZeroCmd.correctionCoordinate = returnZeroCorrectPos;
    returnZeroCmd.selectConfig = runStepCurve;
    returnZeroCmd.speedRatioMode = (SPEED_RATIO_MODE)speedRatioMode;
    returnZeroCmd.speedRatio = speedRatioUint16;
    returnZeroCmd.utilStop1Enable = utilStop1Enable;
    returnZeroCmd.utilStop2Enable = utilStop2Enable;
    returnZeroCmd.timeOutSet = timeOutMs;
    //执行电机回零
    SrvImplBaseStepMotor7ReturnZero(&returnZeroCmd,needReset,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//电机持续运转
static void SrvMsgCan1StepMotor7Cmd_RunAlways(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //运行方向
    uint8_t runAlwaysDir = cmdPackPtr->paramBufferPtr[0];
    //运行曲线
    uint8_t runStepCurve = cmdPackPtr->paramBufferPtr[1];
    //速度比例模式
    uint8_t speedRatioMode = cmdPackPtr->paramBufferPtr[2];
    //速度比例
    uint16_t speedRatioUint16 = ProtUtilReadUint16Buffer(cmdPackPtr->paramBufferPtr+3);
    if(speedRatioUint16 > 100)speedRatioUint16 = 100;
    if(speedRatioUint16 == 0)speedRatioUint16 = 1;
    float speedRatio = (float)(((float)speedRatioUint16)/100.0f);
    //打印参数
    SystemPrintf("%s,runAlwaysDir: %d,runStepCurve: %d,speedRatioMode: %d,speedRatio: %0.3f\r\n",__func__,
                    runAlwaysDir,runStepCurve,speedRatioMode,(double)speedRatio);
    //持续运转结构体
    StepMotorRunAlwaysCmd runAlwaysCmd;
    runAlwaysCmd.motorNo = STEP_MOTOR_NO7;
    runAlwaysCmd.selectConfig = runStepCurve;
    runAlwaysCmd.speedRatioMode = (SPEED_RATIO_MODE)speedRatioMode;
    runAlwaysCmd.speedRatio = speedRatioUint16;
    runAlwaysCmd.dir = (STEP_MOTOR_DIR)runAlwaysDir;
    //执行持续运转
    SrvImplBaseStepMotor7RunAlways(&runAlwaysCmd,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//电机立即停止
static void SrvMsgCan1StepMotor7Cmd_StopNow(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //打印参数
    SystemPrintf("%s\r\n",__func__);
    //立即停止
    SrvImplBaseStepMotor7StopNow(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//电机减速停止
static void SrvMsgCan1StepMotor7Cmd_StopSlowDown(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //减速模式
    uint8_t slowDownMode = cmdPackPtr->paramBufferPtr[0];
    //减速步数
    uint32_t slowDownSteps = ProtUtilReadUint32Buffer(cmdPackPtr->paramBufferPtr+1);
    //超时时间
    uint32_t timeOutMs = ProtUtilReadUint32Buffer(cmdPackPtr->paramBufferPtr+5);
    //打印参数
    SystemPrintf("%s,slowDownMode: %d,slowDownSteps: %d,timeOutMs: %d\r\n",__func__,
                    slowDownMode,slowDownSteps,timeOutMs);
    //执行减速停止
    SrvImplBaseStepMotor7StopSlowDown(slowDownMode,slowDownSteps,timeOutMs,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//获取电机当前坐标
static void SrvMsgCan1StepMotor7Cmd_GetCoordinate(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //打印参数
    SystemPrintf("%s\r\n",__func__);
    //调用实现API
    SrvImplBaseStepMotor7GetCoordinate(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//获取当前原点传感器状态
static void SrvMsgCan1StepMotor7Cmd_ReadOriginSensor(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //打印参数
    SystemPrintf("%s\r\n",__func__);
    //调用实现API
    SrvImplBaseStepMotor7ReadOriginSensor(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}
/*************************************************************************************************************************************/

/*************************************************************CAN上传参数处理***********************************************************/
//占位函数
static void SrvMsgCan1StepMotor7Upload_PlaceHolder(CAN_PROT_UPLOAD_PACK* canUploadPackPtr);

//CAN上传消息处理函数列表
static const USER_CAN_UPLOAD_PROC_UNIT can1MsgUploadProcUnitArray[] = 
{
    //顺序是: 指令码 预期参数最小长度 上传消息处理函数
    {CAN_MASTER_REPORT_ID_HOLD_PLACE     ,2 ,SrvMsgCan1StepMotor7Upload_PlaceHolder},
};

//CAN上传消息处理函数
void SrvMsgCan1StepMotor7Upload(void* can1MsgBodyPtr)
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
    for(indexUtil = 0; indexUtil < DIM_ARRAY_ELEMENT_COUNT(can1MsgUploadProcUnitArray);indexUtil++)
    {
        if(canUploadPackPtr->uploadID == (uint32_t)(can1MsgUploadProcUnitArray[indexUtil].uploadId))
        {
            break;
        }
    }
    //判定序号
    if(indexUtil == DIM_ARRAY_ELEMENT_COUNT(can1MsgUploadProcUnitArray))
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
    if(canUploadPackPtr->uploadDataBufferLength < can1MsgUploadProcUnitArray[indexUtil].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,UploadID: 0X%08X\r\n",__func__,__LINE__,
                        can1MsgUploadProcUnitArray[indexUtil].exceptParamLengthMin,canUploadPackPtr->uploadDataBufferLength,
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
    if(can1MsgUploadProcUnitArray[indexUtil].canUploadProcFuncPtr == NULL)
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
    can1MsgUploadProcUnitArray[indexUtil].canUploadProcFuncPtr(canUploadPackPtr);
    //处理完成,释放内存
    if((canUploadPackPtr->uploadDataBufferPtr != NULL)&&(canUploadPackPtr->uploadDataBufferLength != 0))
    {
        UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackPtr->uploadDataBufferPtr);
    }
    UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackPtr);
}

//占位函数
static void SrvMsgCan1StepMotor7Upload_PlaceHolder(CAN_PROT_UPLOAD_PACK* canUploadPackPtr)
{

}
/*************************************************************************************************************************************/










