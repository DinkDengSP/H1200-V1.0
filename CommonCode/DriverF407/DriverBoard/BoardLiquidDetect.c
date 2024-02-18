/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-02-24 17:29:21 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-08 10:50:12 +0800
************************************************************************************************/ 
#include "BoardLiquidDetect.h"
#include "tx_mutex.h"
#include "ADT_CircularDWordQueue.h"

/*------------------------------------------设备驱动硬相关的-----------------------------------------------*/
//设备互斥信号量
static TX_MUTEX mutexBoardLiquidDetect;
//设备消息队列
static TX_QUEUE queueBoardLiquidDetectRecvMsg;
//消息缓冲区
static uint32_t queueBufferBoardLiquidDetectRecvMsg[LENGTH_QUEUE_BOARD_LIQUID_DETECT_RECV_MSG];
//设备定时器,用于定时处理串口消息
static TX_TIMER timerBoardLiquidDetectRecvMsg;
//当前接收数据长度
static uint16_t recvDataLengthBoardLiquidDetect;
//当前接受数据缓存
static uint8_t recvDataBufferBoardLiquidDetect[COMM_CACHE_LENGTH_BOARD_LIQUID_DETECT];
//临时指令缓存
static uint8_t cmdBufferLocal[COMM_CACHE_LENGTH_BOARD_LIQUID_DETECT];
//事件上报回调处理函数
static BoardLiquidDetectAutoUploadPackProc autoUploadLiquidDetectEventProcFuncPtr = NULL;

/*---------------------------------------------指令相关的缓存---------------------------------------------------*/
//主动上传数据包
static BOARD_LIQUID_DETECT_UPLOAD_PACK boardLiquidDetectUploadPack;
//上报校准过程值缓存
static uint16_t liquidDetectBaseAdjustValue = 0;
//当前基准电阻值
static uint16_t liquidDetectBaseResValue = 0;
//基准电阻写入返回标记
static uint16_t liquidDetectWriteResReturnFlag = 0;
//写入基准值校准收到返回的标识
static uint16_t flagLiquidDetectAdjustBase = 0;
//基准值校准的结果代码
static uint16_t resultLiquidDetectAdjustBase = 0;
//发送开关液面探测信号数据上传接收到返回的标识
static uint16_t flagLiquidDetectSwitchDataUpdate = 0;
//发送开关液面探测功能接收到返回的标识
static uint16_t flagLiquidDetectEnbaleFunc = 0;
//液面探测使能信号状态
static uint16_t stateLiquidDetectEnbale = 0;
//参数信息
static BOARD_LIQUID_DETECT_PARAM boardLiquidDetectParam;
//配置讯息
static BOARD_LIQUID_DETECT_CONFIG boardLiquidDetectConfig;
//探液板状态讯息
static BOARD_LIQUID_DETECT_STATE boardLiquidDetectState;
//探液板触发讯息
static BOARD_LIQUID_DETECT_TRIG_MSG boardLiquidDetectTrigMsg;

//串口接收转发函数
static ErrorStatus BoardLiquidDetectRecvMsgDispatch(UART_RECV_PACK* uartPackRecvPtr)
{
    uint32_t txResult = TX_SUCCESS;
    txResult = tx_queue_send(&queueBoardLiquidDetectRecvMsg,(void*)(&uartPackRecvPtr),TX_NO_WAIT);
    if(txResult != TX_SUCCESS)
    {
        MCU_RTT_SendStringInt("BoardLiquidDetectRecvMsgDispatch tx_queue_send Failed\r\n");
        return ERROR;
    }
    return SUCCESS;
}

//单字节接受处理程序
static void BoardLiquidDetectRecvSingleProc(uint8_t byteVal)
{
    if((recvDataLengthBoardLiquidDetect&0x8000) != 0)
    {
        //已经接收了一帧,还没处理
    }
    else if((recvDataLengthBoardLiquidDetect&0x4000) != 0)
    {
        //已经接收到0x0d,现在要等待接收0x0a
        if(byteVal == 0x0a)
        {
            recvDataLengthBoardLiquidDetect |= 0x8000;
        }
        else
        {
            //取消等待0X0A模式,进行特殊处理
            recvDataLengthBoardLiquidDetect &= ~0x4000;
            if (recvDataLengthBoardLiquidDetect >= COMM_CACHE_LENGTH_BOARD_LIQUID_DETECT)
            {
                recvDataLengthBoardLiquidDetect = 0;
            }
            //补充上一次接受的0X0D
            recvDataBufferBoardLiquidDetect[recvDataLengthBoardLiquidDetect] = 0x0d;
            recvDataLengthBoardLiquidDetect++;
            if(byteVal == 0x0d)
            {
                recvDataLengthBoardLiquidDetect |= 0X4000;
            }
            else
            {
                //有效数据接收
                recvDataBufferBoardLiquidDetect[recvDataLengthBoardLiquidDetect] = byteVal;
                recvDataLengthBoardLiquidDetect++;
            }
        }
    }
    else
    {
        if(byteVal == 0x0d)
        {
            recvDataLengthBoardLiquidDetect |= 0x4000;
        }
        else if (recvDataLengthBoardLiquidDetect >= COMM_CACHE_LENGTH_BOARD_LIQUID_DETECT)
        {
            recvDataLengthBoardLiquidDetect = 0;
        }
        else
        {
            //有效数据接收
            recvDataBufferBoardLiquidDetect[recvDataLengthBoardLiquidDetect] = byteVal;
            recvDataLengthBoardLiquidDetect++;
        }
    }
}

//数据包处理
static void BoardLiquidDetectRecvPackProc(void)
{
    if((recvDataLengthBoardLiquidDetect&0x8000) == 0X0000)
    {
        //没收到完整的一包
        return;
    }
    //收到完整的一包
    switch(recvDataBufferBoardLiquidDetect[0])
    {
        case CMD_LIQUID_DETECT_AUTO_UPDATE_BASE_ADC://主动上报基准值
            boardLiquidDetectUploadPack.baseVol = 0;
            boardLiquidDetectUploadPack.baseVol = recvDataBufferBoardLiquidDetect[1];
            boardLiquidDetectUploadPack.baseVol <<= 8;
            boardLiquidDetectUploadPack.baseVol += recvDataBufferBoardLiquidDetect[2];
            //回调上报
            if(autoUploadLiquidDetectEventProcFuncPtr != NULL)
            {
                autoUploadLiquidDetectEventProcFuncPtr(BOARD_LIQUID_DETECT_UPLOAD_PACK_TYPE_BASE_VOL,
                                                        boardLiquidDetectUploadPack.baseVol);
            }
            break;
        case CMD_LIQUID_DETECT_AUTO_UPDATE_FOLLOW_ADC://主动上报跟随值
            boardLiquidDetectUploadPack.needleTipVolOriginal = 0;
            boardLiquidDetectUploadPack.needleTipVolOriginal = recvDataBufferBoardLiquidDetect[1];
            boardLiquidDetectUploadPack.needleTipVolOriginal <<= 8;
            boardLiquidDetectUploadPack.needleTipVolOriginal += recvDataBufferBoardLiquidDetect[2];
            //回调上报
            if(autoUploadLiquidDetectEventProcFuncPtr != NULL)
            {
                autoUploadLiquidDetectEventProcFuncPtr(BOARD_LIQUID_DETECT_UPLOAD_PACK_TYPE_NEEDLE_VOL_ORIGINAL,
                                                        boardLiquidDetectUploadPack.needleTipVolOriginal);
            }
            break;
        case CMD_LIQUID_DETECT_AUTO_UPDATE_FILTER_ADC://主动上报滤波后跟随值
            boardLiquidDetectUploadPack.needleTipVolFilter = 0;
            boardLiquidDetectUploadPack.needleTipVolFilter = recvDataBufferBoardLiquidDetect[1];
            boardLiquidDetectUploadPack.needleTipVolFilter <<= 8;
            boardLiquidDetectUploadPack.needleTipVolFilter += recvDataBufferBoardLiquidDetect[2];
            //回调上报
            if(autoUploadLiquidDetectEventProcFuncPtr != NULL)
            {
                autoUploadLiquidDetectEventProcFuncPtr(BOARD_LIQUID_DETECT_UPLOAD_PACK_TYPE_NEEDLE_VOL_FILTER,
                                                        boardLiquidDetectUploadPack.needleTipVolFilter);
            }
            break;
        case CMD_LIQUID_DETECT_AUTO_UPDATE_TRIG_FLAG://主动上报液面探测状态
            boardLiquidDetectUploadPack.liquidDetectState = 0;
            boardLiquidDetectUploadPack.liquidDetectState = recvDataBufferBoardLiquidDetect[2];
            //回调上报
            if(autoUploadLiquidDetectEventProcFuncPtr != NULL)
            {
                autoUploadLiquidDetectEventProcFuncPtr(BOARD_LIQUID_DETECT_UPLOAD_PACK_TYPE_STATE,
                                                        boardLiquidDetectUploadPack.liquidDetectState);
            }
            break;
        case LIQUID_DETECT_CMD_READ_NEEDLE_BASE_VOL://读取基准电压值
            boardLiquidDetectState.baseVol = 0;
            boardLiquidDetectState.baseVol = recvDataBufferBoardLiquidDetect[1];
            boardLiquidDetectState.baseVol <<= 8;
            boardLiquidDetectState.baseVol += recvDataBufferBoardLiquidDetect[2];
            break;
        case LIQUID_DETECT_CMD_READ_NEEDLE_CURRENT_VOL://读取针尖电压值
            boardLiquidDetectState.needleTipVol = 0;
            boardLiquidDetectState.needleTipVol = recvDataBufferBoardLiquidDetect[1];
            boardLiquidDetectState.needleTipVol <<= 8;
            boardLiquidDetectState.needleTipVol += recvDataBufferBoardLiquidDetect[2];
            break;
        case CMD_LIQUID_DETECT_AUTO_UPDATE_ADJUST_VALUE://主动上报校准过程状态
            liquidDetectBaseAdjustValue = 0;
            liquidDetectBaseAdjustValue = recvDataBufferBoardLiquidDetect[1];
            liquidDetectBaseAdjustValue <<= 8;
            liquidDetectBaseAdjustValue += recvDataBufferBoardLiquidDetect[2];
            break;
        case LIQUID_DETECT_CMD_READ_THRESHOLD_VALUE://读取阈值
            boardLiquidDetectParam.thresholdVal = 0;
            boardLiquidDetectParam.thresholdVal = recvDataBufferBoardLiquidDetect[1];
            boardLiquidDetectParam.thresholdVal <<= 8;
            boardLiquidDetectParam.thresholdVal += recvDataBufferBoardLiquidDetect[2];
            break;
        case LIQUID_DETECT_CMD_READ_THRESHOLD_COUNT_LIMIT://读取阈值检测计数
            boardLiquidDetectParam.thresholdCount = 0;
            boardLiquidDetectParam.thresholdCount = recvDataBufferBoardLiquidDetect[1];
            boardLiquidDetectParam.thresholdCount <<= 8;
            boardLiquidDetectParam.thresholdCount += recvDataBufferBoardLiquidDetect[2];
            break;
        case LIQUID_DETECT_CMD_READ_SLOPE_VALUE://读取斜率
            boardLiquidDetectParam.slopeVal = 0;
            boardLiquidDetectParam.slopeVal = recvDataBufferBoardLiquidDetect[1];
            boardLiquidDetectParam.slopeVal <<= 8;
            boardLiquidDetectParam.slopeVal += recvDataBufferBoardLiquidDetect[2];
            break;
        case LIQUID_DETECT_CMD_READ_SLOPE_COUNT_LIMIT://读取斜率检测计数
            boardLiquidDetectParam.slopeCount = 0;
            boardLiquidDetectParam.slopeCount = recvDataBufferBoardLiquidDetect[1];
            boardLiquidDetectParam.slopeCount <<= 8;
            boardLiquidDetectParam.slopeCount += recvDataBufferBoardLiquidDetect[2];
            break;
        case LIQUID_DETECT_CMD_READ_SIGNAL_HOLD_MS://读取信号保持时间
            boardLiquidDetectConfig.singleHoldTimeMs = 0;
            boardLiquidDetectConfig.singleHoldTimeMs = recvDataBufferBoardLiquidDetect[1];
            boardLiquidDetectConfig.singleHoldTimeMs <<= 8;
            boardLiquidDetectConfig.singleHoldTimeMs += recvDataBufferBoardLiquidDetect[2];
            break;
        case LIQUID_DETECT_CMD_READ_AD5272_RES://读取电子电阻值
            liquidDetectBaseResValue = 0;
            liquidDetectBaseResValue = recvDataBufferBoardLiquidDetect[1];
            liquidDetectBaseResValue <<= 8;
            liquidDetectBaseResValue += recvDataBufferBoardLiquidDetect[2];
            break;
        case LIQUID_DETECT_CMD_READ_MAX_NEEDLE_BASE_VOL://读取探液过程中的基准电压最大值
            boardLiquidDetectTrigMsg.baseVolMax = 0;
            boardLiquidDetectTrigMsg.baseVolMax = recvDataBufferBoardLiquidDetect[1];
            boardLiquidDetectTrigMsg.baseVolMax <<= 8;
            boardLiquidDetectTrigMsg.baseVolMax += recvDataBufferBoardLiquidDetect[2];
            break;
        case LIQUID_DETECT_CMD_READ_MAX_NEEDLE_VOL://读取探液过程中的针尖电压最大值
            boardLiquidDetectTrigMsg.needleTipVolMax = 0;
            boardLiquidDetectTrigMsg.needleTipVolMax = recvDataBufferBoardLiquidDetect[1];
            boardLiquidDetectTrigMsg.needleTipVolMax <<= 8;
            boardLiquidDetectTrigMsg.needleTipVolMax += recvDataBufferBoardLiquidDetect[2];
            break;
        case LIQUID_DETECT_CMD_WRITE_AD5272_RES://写入基准电阻
            liquidDetectWriteResReturnFlag = 1;
            break;
        case LIQUID_DETECT_CMD_FUNC_ADJUST_BASE://基准值校准
            flagLiquidDetectAdjustBase = 1;
            resultLiquidDetectAdjustBase = recvDataBufferBoardLiquidDetect[1];
            break;
        case LIQUID_DETECT_CMD_FUNC_SWITCH_LIQUID_DATA_UPDATE://开关液面探测数据上传
            flagLiquidDetectSwitchDataUpdate = 1;
            break;
        case LIQUID_DETECT_CMD_FUNC_SWITCH_LIQUID_ENABLE://开关液面探测功能
            flagLiquidDetectEnbaleFunc = 1;
            stateLiquidDetectEnbale = recvDataBufferBoardLiquidDetect[2];
            break;
        case LIQUID_DETECT_CMD_READ_LIQUID_BOARD_VERSION://读取探液板版本
            //版本号字符串长度
            boardLiquidDetectConfig.softwareVersionLength = 0;
            boardLiquidDetectConfig.softwareVersionLength = (uint8_t)(recvDataLengthBoardLiquidDetect&0X00FF);
            //减少一个字节,那是指令码
            boardLiquidDetectConfig.softwareVersionLength -= 1;
            //拷贝数据
            UserMemCopy(boardLiquidDetectConfig.softwareVersionBuf,recvDataBufferBoardLiquidDetect+1,boardLiquidDetectConfig.softwareVersionLength);
            //接收标记设置
            boardLiquidDetectConfig.softwareVersionRespFlag = 0XFFFF;
            break;
        default:
            break;
    }
    //处理完成,数据清除
    recvDataLengthBoardLiquidDetect = 0;
}

//数据处理
static void BoardLiquidDetectRecvPackProcess(uint8_t* packBufPtr,uint16_t packLength)
{
    uint16_t indexUtil = 0;
    for(indexUtil = 0;indexUtil < packLength;indexUtil++)
    {
        //单字节接收处理
        BoardLiquidDetectRecvSingleProc(packBufPtr[indexUtil]);
        //数据包检测处理
        BoardLiquidDetectRecvPackProc();
    }
}


//串口接收处理函数
static void BoardLiquidDetectRecvMsgProcessPeriod(ULONG unusedParam)
{
    //从消息队列中获取消息
    uint32_t txResult = TX_SUCCESS;
    UART_RECV_PACK* queueMsgPtr = NULL;
    do
    {
        queueMsgPtr = NULL;
        txResult = tx_queue_receive(&queueBoardLiquidDetectRecvMsg,(void*)(&queueMsgPtr),TX_NO_WAIT);
        if((txResult != TX_SUCCESS)||(queueMsgPtr == NULL))
        {
            //没有消息
            return;
        }
        //存在消息,进行解析
        if(queueMsgPtr->packSrc != COMM_PACK_SRC_BOARD_LIQUID_DETECT)
        {
            //不是我想要的数据
            continue;
        }
        //数据处理
        BoardLiquidDetectRecvPackProcess(queueMsgPtr->uartRecvDataBuffer,queueMsgPtr->uartRecvDataLength);
        //处理完成,释放数据包
        RELEASE_PACK_BOARD_LIQUID_DETECT(queueMsgPtr);
    }while(1);
}

//获取设备锁
static void BoardLiquidDetectGetLock(void)
{
    tx_mutex_get(&mutexBoardLiquidDetect,TX_WAIT_FOREVER);
}

//释放设备锁
static void BoardLiquidDetectReleaseLock(void)
{
    tx_mutex_put(&mutexBoardLiquidDetect);
}

//清除接收缓存
static void BoardLiquidDetectClearRecvCache(void)
{
    uint32_t txResult = TX_SUCCESS;
    UART_RECV_PACK* queueMsgPtr = NULL;
    //定时器停止
    tx_timer_deactivate(&timerBoardLiquidDetectRecvMsg);
    //清空消息队列
    do
    {
        queueMsgPtr = NULL;
        txResult = tx_queue_receive(&queueBoardLiquidDetectRecvMsg,(void*)(&queueMsgPtr),TX_NO_WAIT);
        if((txResult != TX_SUCCESS)||(queueMsgPtr == NULL))
        {
            //没有消息
            break;
        }
        //存在消息,进行解析
        if(queueMsgPtr->packSrc != COMM_PACK_SRC_BOARD_LIQUID_DETECT)
        {
            //不是我想要的数据
            continue;
        }
        //处理完成,释放数据包
        RELEASE_PACK_BOARD_LIQUID_DETECT(queueMsgPtr);
    }while(1);
    //清除内部缓存
    recvDataLengthBoardLiquidDetect = 0;
    UserMemClear(recvDataBufferBoardLiquidDetect,COMM_CACHE_LENGTH_BOARD_LIQUID_DETECT);
}

//初始化端口
void BoardLiquidDetectPortInit(BoardLiquidDetectAutoUploadPackProc funcPtr)
{
    //初始化相关资源
    if(mutexBoardLiquidDetect.tx_mutex_id == TX_MUTEX_ID)
    {
        //已经初始化,直接返回
        return;
    }
    //创建互斥信号量
    tx_mutex_create(&mutexBoardLiquidDetect,"mutexBoardLiquidDetect",TX_INHERIT);
    //创建消息队列
    tx_queue_create(&queueBoardLiquidDetectRecvMsg,"queueBoardLiquidDetectRecvMsg",1,(void*)queueBufferBoardLiquidDetectRecvMsg,
                        sizeof(queueBufferBoardLiquidDetectRecvMsg)/sizeof(uint8_t));
    //创建定时器任务,创建的时候不使能
    tx_timer_create(&timerBoardLiquidDetectRecvMsg,"timerBoardLiquidDetectRecvMsg",BoardLiquidDetectRecvMsgProcessPeriod,0,
                        MS_TO_OS_TICK(PERIOD_MS_PROC_BOARD_LIQUID_DETECT_RECV_MSG),
                        MS_TO_OS_TICK(PERIOD_MS_PROC_BOARD_LIQUID_DETECT_RECV_MSG),
                        TX_NO_ACTIVATE);
    //串口资源初始化
    COMM_INIT_BOARD_LIQUID_DETECT(BOARD_LIQUID_DETECT_BAUD,BOARD_LIQUID_DETECT_LENGTH,BOARD_LIQUID_DETECT_STOP_BIT,
                                        BOARD_LIQUID_DETECT_CHECK_MODE,BOARD_LIQUID_DETECT_HARD_CONTROL);
    //回调函数注册
    COMM_CALLBACL_REGISTER_BOARD_LIQUID_DETECT(BoardLiquidDetectRecvMsgDispatch,NULL,NULL);
    //接收数据缓存清空
    BoardLiquidDetectClearRecvCache();
    //映射回调函数
    autoUploadLiquidDetectEventProcFuncPtr = funcPtr;
    //关闭液面探测数据上传
    BoardLiquidDetectSetDataUpload(DISABLE);
}

//发送指令
static void BoardLiquidDetectSendCommand(uint8_t* buffer,uint16_t length,uint16_t* setValBeforeSend,uint16_t setVal)
{
    //清空缓存
    BoardLiquidDetectClearRecvCache();
    //启动定时器
    tx_timer_activate(&timerBoardLiquidDetectRecvMsg);
    //发送前设置某个变量为指定状态
    *setValBeforeSend = setVal;
    //首先发送指令
    COMM_SEND_CMD_BUF_BOARD_LIQUID_DETECT(buffer,length);
}

//等待返回
static ErrorStatus BoardLiquidDetectWaitResp(uint32_t timeOutMs,FunctionalState stopTimer,uint16_t* waitValAfterSend,uint16_t notEqualVal)
{
    //等待开始时间
    uint32_t cmdSendOverTimeStamp = tx_time_get();
    //查询结果
    do
    {
        if(*waitValAfterSend != notEqualVal)
        {
            //等待值变化了
            break;
        }
        //延时,延时时间与定时器时间相等
        CoreDelayMs(PERIOD_MS_PROC_BOARD_LIQUID_DETECT_RECV_MSG);
    }while (tx_time_get() - cmdSendOverTimeStamp < MS_TO_OS_TICK(timeOutMs));
    if(stopTimer == ENABLE)
    {
        //退出查询,停止定时器
        tx_timer_deactivate(&timerBoardLiquidDetectRecvMsg);
    }
    //超时没收到指定数据
    if(*waitValAfterSend == notEqualVal)
    {
        return ERROR;
    }
    return SUCCESS;
}

//创建指令缓存
static void BoardLiquidDetectCreateCmd(uint8_t cmdValue,uint16_t paramValue)
{
    cmdBufferLocal[0] = cmdValue;
    cmdBufferLocal[1] = (uint8_t)(paramValue>>8);
    cmdBufferLocal[2] = (uint8_t)(paramValue);
    cmdBufferLocal[3] = 0X0D;
    cmdBufferLocal[4] = 0X0A;
}

//检测设备是否存在
ERROR_SUB BoardLiquidDetectConfigDefault(void)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    BOARD_LIQUID_DETECT_PARAM liquidDetectParam = {0};
    //获取设备锁
    BoardLiquidDetectGetLock();
    //首先关闭液面探测上传
    errorSubCode = BoardLiquidDetectSetDataUpload(DISABLE);
    if(errorSubCode != ERROR_SUB_OK)
    {
        //释放设备锁
        BoardLiquidDetectReleaseLock();
        return errorSubCode;
    }
    //关闭液面探测功能
    errorSubCode = BoardLiquidDetectStop();
    if(errorSubCode != ERROR_SUB_OK)
    {
        //释放设备锁
        BoardLiquidDetectReleaseLock();
        return errorSubCode;
    }
    //读取液面探测检测阈值1
    errorSubCode = BoardLiquidDetectGetParam(0,&liquidDetectParam);
    if(errorSubCode != ERROR_SUB_OK)
    {
        //释放设备锁
        BoardLiquidDetectReleaseLock();
        return errorSubCode;
    }
    //读取基准电阻
    uint16_t baseRes = 0;
    errorSubCode = BoardLiquidDetectReadBaseRes(&baseRes);
    if(errorSubCode != ERROR_SUB_OK)
    {
        //释放设备锁
        BoardLiquidDetectReleaseLock();
        return errorSubCode;
    }
    //释放设备锁
    BoardLiquidDetectReleaseLock();
    return errorSubCode;
}

//获取液面探测的指定参数信息
ERROR_SUB BoardLiquidDetectGetParam(uint8_t paramNo,BOARD_LIQUID_DETECT_PARAM* paramPtr)
{
    ErrorStatus liquidDetectState = SUCCESS;
    uint8_t retryCount = 0;
    //获取设备锁
    BoardLiquidDetectGetLock();
    //首先创建指令缓存
    BoardLiquidDetectCreateCmd(LIQUID_DETECT_CMD_READ_THRESHOLD_VALUE,(uint16_t)paramNo);
    retryCount = 0;
    do
    {
        //发送指令,发送前设置指定变量为指定值
        BoardLiquidDetectSendCommand(cmdBufferLocal,5,&(boardLiquidDetectParam.thresholdVal),0XFFFF);
        //等待返回,等待指定变量不是指定值
        liquidDetectState = BoardLiquidDetectWaitResp(TIME_OUT_MS_BOARD_LIQUID_DETECT,ENABLE,&(boardLiquidDetectParam.thresholdVal),0XFFFF);
        if(liquidDetectState == SUCCESS)
        {
            //成功
            break;
        }
        //没检测到,重试次数增加
        retryCount++;
    }while(retryCount < LIQUID_DETECT_BOARD_COMM_ERR_RETRY_COUNT);
    //解析结果
    if(liquidDetectState == ERROR)
    {
        //释放设备锁
        BoardLiquidDetectReleaseLock();
        MCU_LogShowErrorSubCode(ERROR_SUB_BOARD_LIQUID_DETECT_READ_THRESHOLD_VAL);
        return ERROR_SUB_BOARD_LIQUID_DETECT_READ_THRESHOLD_VAL;
    }
    //首先创建指令缓存
    BoardLiquidDetectCreateCmd(LIQUID_DETECT_CMD_READ_THRESHOLD_COUNT_LIMIT,(uint16_t)paramNo);
    retryCount = 0;
    do
    {
        //发送指令,发送前设置指定变量为指定值
        BoardLiquidDetectSendCommand(cmdBufferLocal,5,&(boardLiquidDetectParam.thresholdCount),0XFFFF);
        //等待返回,等待指定变量不是指定值
        liquidDetectState = BoardLiquidDetectWaitResp(TIME_OUT_MS_BOARD_LIQUID_DETECT,ENABLE,&(boardLiquidDetectParam.thresholdCount),0XFFFF);
        if(liquidDetectState == SUCCESS)
        {
            //成功
            break;
        }
        //没检测到,重试次数增加
        retryCount++;
    }while(retryCount < LIQUID_DETECT_BOARD_COMM_ERR_RETRY_COUNT);
    //解析结果
    if(liquidDetectState == ERROR)
    {
        //释放设备锁
        BoardLiquidDetectReleaseLock();
        MCU_LogShowErrorSubCode(ERROR_SUB_BOARD_LIQUID_DETECT_READ_THRESHOLD_COUNT);
        return ERROR_SUB_BOARD_LIQUID_DETECT_READ_THRESHOLD_COUNT;
    }
    //首先创建指令缓存
    BoardLiquidDetectCreateCmd(LIQUID_DETECT_CMD_READ_SLOPE_VALUE,(uint16_t)paramNo);
    retryCount = 0;
    do
    {
        //发送指令,发送前设置指定变量为指定值
        BoardLiquidDetectSendCommand(cmdBufferLocal,5,&(boardLiquidDetectParam.slopeVal),0XFFFF);
        //等待返回,等待指定变量不是指定值
        liquidDetectState = BoardLiquidDetectWaitResp(TIME_OUT_MS_BOARD_LIQUID_DETECT,ENABLE,&(boardLiquidDetectParam.slopeVal),0XFFFF);
        if(liquidDetectState == SUCCESS)
        {
            //成功
            break;
        }
        //没检测到,重试次数增加
        retryCount++;
    }while(retryCount < LIQUID_DETECT_BOARD_COMM_ERR_RETRY_COUNT);
    //解析结果
    if(liquidDetectState == ERROR)
    {
        //释放设备锁
        BoardLiquidDetectReleaseLock();
        MCU_LogShowErrorSubCode(ERROR_SUB_BOARD_LIQUID_DETECT_READ_SLOPE_VAL);
        return ERROR_SUB_BOARD_LIQUID_DETECT_READ_SLOPE_VAL;
    }
    //首先创建指令缓存
    BoardLiquidDetectCreateCmd(LIQUID_DETECT_CMD_READ_SLOPE_COUNT_LIMIT,(uint16_t)paramNo);
    retryCount = 0;
    do
    {
        //发送指令,发送前设置指定变量为指定值
        BoardLiquidDetectSendCommand(cmdBufferLocal,5,&(boardLiquidDetectParam.slopeCount),0XFFFF);
        //等待返回,等待指定变量不是指定值
        liquidDetectState = BoardLiquidDetectWaitResp(TIME_OUT_MS_BOARD_LIQUID_DETECT,ENABLE,&(boardLiquidDetectParam.slopeCount),0XFFFF);
        if(liquidDetectState == SUCCESS)
        {
            //成功
            break;
        }
        //没检测到,重试次数增加
        retryCount++;
    }while(retryCount < LIQUID_DETECT_BOARD_COMM_ERR_RETRY_COUNT);
    //解析结果
    if(liquidDetectState == ERROR)
    {
        //释放设备锁
        BoardLiquidDetectReleaseLock();
        MCU_LogShowErrorSubCode(ERROR_SUB_BOARD_LIQUID_DETECT_READ_SLOPE_COUNT);
        return ERROR_SUB_BOARD_LIQUID_DETECT_READ_SLOPE_COUNT;
    }
    //释放设备锁
    BoardLiquidDetectReleaseLock();
    //结果返回
    paramPtr->thresholdVal = boardLiquidDetectParam.thresholdVal;
    paramPtr->thresholdCount = boardLiquidDetectParam.thresholdCount;
    paramPtr->slopeVal = boardLiquidDetectParam.slopeVal;
    paramPtr->slopeCount = boardLiquidDetectParam.slopeCount;
    return ERROR_SUB_OK;
}

//获取液面探测配置讯息
ERROR_SUB BoardLiquidDetectGetConfig(BOARD_LIQUID_DETECT_CONFIG** configPtrPtr)
{
    ErrorStatus liquidDetectState = SUCCESS;
    uint8_t retryCount = 0;
    //获取设备锁
    BoardLiquidDetectGetLock();
    //首先创建指令缓存
    BoardLiquidDetectCreateCmd(LIQUID_DETECT_CMD_READ_SIGNAL_HOLD_MS,0);
    retryCount = 0;
    do
    {
        //发送指令,发送前设置指定变量为指定值
        BoardLiquidDetectSendCommand(cmdBufferLocal,5,&(boardLiquidDetectConfig.singleHoldTimeMs),0XFFFF);
        //等待返回,等待指定变量不是指定值
        liquidDetectState = BoardLiquidDetectWaitResp(TIME_OUT_MS_BOARD_LIQUID_DETECT,ENABLE,&(boardLiquidDetectConfig.singleHoldTimeMs),0XFFFF);
        if(liquidDetectState == SUCCESS)
        {
            //成功
            break;
        }
        //没检测到,重试次数增加
        retryCount++;
    }while(retryCount < LIQUID_DETECT_BOARD_COMM_ERR_RETRY_COUNT);
    //解析结果
    if(liquidDetectState == ERROR)
    {
        //释放设备锁
        BoardLiquidDetectReleaseLock();
        MCU_LogShowErrorSubCode(ERROR_SUB_BOARD_LIQUID_DETECT_READ_SINGAL_HOLD_TIME);
        return ERROR_SUB_BOARD_LIQUID_DETECT_READ_SINGAL_HOLD_TIME;
    }
    //首先创建指令缓存
    BoardLiquidDetectCreateCmd(LIQUID_DETECT_CMD_READ_LIQUID_BOARD_VERSION,0);
    retryCount = 0;
    do
    {
        //发送指令,发送前设置指定变量为指定值
        BoardLiquidDetectSendCommand(cmdBufferLocal,5,&(boardLiquidDetectConfig.softwareVersionRespFlag),0X0000);
        //等待返回,等待指定变量不是指定值
        liquidDetectState = BoardLiquidDetectWaitResp(TIME_OUT_MS_BOARD_LIQUID_DETECT,ENABLE,&(boardLiquidDetectConfig.softwareVersionRespFlag),0X0000);
        if(liquidDetectState == SUCCESS)
        {
            //成功
            break;
        }
        //没检测到,重试次数增加
        retryCount++;
    }while(retryCount < LIQUID_DETECT_BOARD_COMM_ERR_RETRY_COUNT);
    //解析结果
    if(liquidDetectState == ERROR)
    {
        //释放设备锁
        BoardLiquidDetectReleaseLock();
        MCU_LogShowErrorSubCode(ERROR_SUB_BOARD_LIQUID_DETECT_READ_SOFTWARE_VERSION);
        return ERROR_SUB_BOARD_LIQUID_DETECT_READ_SOFTWARE_VERSION;
    }
    //释放设备锁
    BoardLiquidDetectReleaseLock();
    //结果返回
    *configPtrPtr = &boardLiquidDetectConfig;
    return ERROR_SUB_OK;
}

//获取探液板当前状态
ERROR_SUB BoardLiquidDetectGetState(BOARD_LIQUID_DETECT_STATE* statePtr)
{
    ErrorStatus liquidDetectState = SUCCESS;
    uint8_t retryCount = 0;
    //获取设备锁
    BoardLiquidDetectGetLock();
    //首先创建指令缓存
    BoardLiquidDetectCreateCmd(LIQUID_DETECT_CMD_READ_NEEDLE_BASE_VOL,0);
    retryCount = 0;
    do
    {
        //发送指令,发送前设置指定变量为指定值
        BoardLiquidDetectSendCommand(cmdBufferLocal,5,&(boardLiquidDetectState.baseVol),0XFFFF);
        //等待返回,等待指定变量不是指定值
        liquidDetectState = BoardLiquidDetectWaitResp(TIME_OUT_MS_BOARD_LIQUID_DETECT,ENABLE,&(boardLiquidDetectState.baseVol),0XFFFF);
        if(liquidDetectState == SUCCESS)
        {
            //成功
            break;
        }
        //没检测到,重试次数增加
        retryCount++;
    }while(retryCount < LIQUID_DETECT_BOARD_COMM_ERR_RETRY_COUNT);
    //解析结果
    if(liquidDetectState == ERROR)
    {
        //释放设备锁
        BoardLiquidDetectReleaseLock();
        MCU_LogShowErrorSubCode(ERROR_SUB_BOARD_LIQUID_DETECT_READ_BASE_VOL);
        return ERROR_SUB_BOARD_LIQUID_DETECT_READ_BASE_VOL;
    }
    //首先创建指令缓存
    BoardLiquidDetectCreateCmd(LIQUID_DETECT_CMD_READ_NEEDLE_CURRENT_VOL,0);
    retryCount = 0;
    do
    {
        //发送指令,发送前设置指定变量为指定值
        BoardLiquidDetectSendCommand(cmdBufferLocal,5,&(boardLiquidDetectState.needleTipVol),0XFFFF);
        //等待返回,等待指定变量不是指定值
        liquidDetectState = BoardLiquidDetectWaitResp(TIME_OUT_MS_BOARD_LIQUID_DETECT,ENABLE,&(boardLiquidDetectState.needleTipVol),0XFFFF);
        if(liquidDetectState == SUCCESS)
        {
            //成功
            break;
        }
        //没检测到,重试次数增加
        retryCount++;
    }while(retryCount < LIQUID_DETECT_BOARD_COMM_ERR_RETRY_COUNT);
    //解析结果
    if(liquidDetectState == ERROR)
    {
        //释放设备锁
        BoardLiquidDetectReleaseLock();
        MCU_LogShowErrorSubCode(ERROR_SUB_BOARD_LIQUID_DETECT_READ_NEEDLE_TIP_VOL);
        return ERROR_SUB_BOARD_LIQUID_DETECT_READ_NEEDLE_TIP_VOL;
    }
    //释放设备锁
    BoardLiquidDetectReleaseLock();
    //结果返回
    statePtr->baseVol = boardLiquidDetectState.baseVol;
    statePtr->needleTipVol = boardLiquidDetectState.needleTipVol;
    return ERROR_SUB_OK;
}

//获取探液板触发讯息
ERROR_SUB BoardLiquidDetectTrigMsg(BOARD_LIQUID_DETECT_TRIG_MSG* trigMsgPtr)
{
    ErrorStatus liquidDetectState = SUCCESS;
    uint8_t retryCount = 0;
    //获取设备锁
    BoardLiquidDetectGetLock();
    //首先创建指令缓存
    BoardLiquidDetectCreateCmd(LIQUID_DETECT_CMD_READ_MAX_NEEDLE_BASE_VOL,0);
    retryCount = 0;
    do
    {
        //发送指令,发送前设置指定变量为指定值
        BoardLiquidDetectSendCommand(cmdBufferLocal,5,&(boardLiquidDetectTrigMsg.baseVolMax),0XFFFF);
        //等待返回,等待指定变量不是指定值
        liquidDetectState = BoardLiquidDetectWaitResp(TIME_OUT_MS_BOARD_LIQUID_DETECT,ENABLE,&(boardLiquidDetectTrigMsg.baseVolMax),0XFFFF);
        if(liquidDetectState == SUCCESS)
        {
            //成功
            break;
        }
        //没检测到,重试次数增加
        retryCount++;
    }while(retryCount < LIQUID_DETECT_BOARD_COMM_ERR_RETRY_COUNT);
    //解析结果
    if(liquidDetectState == ERROR)
    {
        //释放设备锁
        BoardLiquidDetectReleaseLock();
        MCU_LogShowErrorSubCode(ERROR_SUB_BOARD_LIQUID_DETECT_READ_BASE_MAX_VOL);
        return ERROR_SUB_BOARD_LIQUID_DETECT_READ_BASE_MAX_VOL;
    }
    //首先创建指令缓存
    BoardLiquidDetectCreateCmd(LIQUID_DETECT_CMD_READ_MAX_NEEDLE_VOL,0);
    retryCount = 0;
    do
    {
        //发送指令,发送前设置指定变量为指定值
        BoardLiquidDetectSendCommand(cmdBufferLocal,5,&(boardLiquidDetectTrigMsg.needleTipVolMax),0XFFFF);
        //等待返回,等待指定变量不是指定值
        liquidDetectState = BoardLiquidDetectWaitResp(TIME_OUT_MS_BOARD_LIQUID_DETECT,ENABLE,&(boardLiquidDetectTrigMsg.needleTipVolMax),0XFFFF);
        if(liquidDetectState == SUCCESS)
        {
            //成功
            break;
        }
        //没检测到,重试次数增加
        retryCount++;
    }while(retryCount < LIQUID_DETECT_BOARD_COMM_ERR_RETRY_COUNT);
    //解析结果
    if(liquidDetectState == ERROR)
    {
        //释放设备锁
        BoardLiquidDetectReleaseLock();
        MCU_LogShowErrorSubCode(ERROR_SUB_BOARD_LIQUID_DETECT_READ_NEEDLE_MAX_VOL);
        return ERROR_SUB_BOARD_LIQUID_DETECT_READ_NEEDLE_MAX_VOL;
    }
    //释放设备锁
    BoardLiquidDetectReleaseLock();
    //结果返回
    trigMsgPtr->baseVolMax = boardLiquidDetectTrigMsg.baseVolMax;
    trigMsgPtr->needleTipVolMax = boardLiquidDetectTrigMsg.needleTipVolMax;
    return ERROR_SUB_OK;
}

//读取基准电阻值
ERROR_SUB BoardLiquidDetectReadBaseRes(uint16_t* resValPtr)
{
    ErrorStatus liquidDetectState = SUCCESS;
    uint8_t retryCount = 0;
    //获取设备锁
    BoardLiquidDetectGetLock();
    //首先创建指令缓存
    BoardLiquidDetectCreateCmd(LIQUID_DETECT_CMD_READ_AD5272_RES,0);
    retryCount = 0;
    do
    {
        //发送指令,发送前设置指定变量为指定值
        BoardLiquidDetectSendCommand(cmdBufferLocal,5,&liquidDetectBaseResValue,0XFFFF);
        //等待返回,等待指定变量不是指定值
        liquidDetectState = BoardLiquidDetectWaitResp(TIME_OUT_MS_BOARD_LIQUID_DETECT,ENABLE,&liquidDetectBaseResValue,0XFFFF);
        if(liquidDetectState == SUCCESS)
        {
            //成功
            break;
        }
        //没检测到,重试次数增加
        retryCount++;
    }while(retryCount < LIQUID_DETECT_BOARD_COMM_ERR_RETRY_COUNT);
    //释放设备锁
    BoardLiquidDetectReleaseLock();
    //解析结果
    if(liquidDetectState == ERROR)
    {
        *resValPtr = 0;
        MCU_LogShowErrorSubCode(ERROR_SUB_BOARD_LIQUID_DETECT_READ_BASE_RES);
        return ERROR_SUB_BOARD_LIQUID_DETECT_READ_BASE_RES;
    }
    else
    {
        *resValPtr = liquidDetectBaseResValue;
        return ERROR_SUB_OK;
    }
}

//写入基准电阻值
ERROR_SUB BoardLiquidDetectWriteBaseRes(uint16_t resVal)
{
    if(resVal > LIQUID_DETECT_BOARD_BASE_RES_MAX)
    {
        resVal = LIQUID_DETECT_BOARD_BASE_RES_MAX;
    }
    ErrorStatus liquidDetectState = SUCCESS;
    //获取设备锁
    BoardLiquidDetectGetLock();
    //首先创建指令缓存
    BoardLiquidDetectCreateCmd(LIQUID_DETECT_CMD_WRITE_AD5272_RES,resVal);
    uint8_t retryCount = 0;
    do
    {
        //发送指令,发送前设置指定变量为指定值
        BoardLiquidDetectSendCommand(cmdBufferLocal,5,&liquidDetectWriteResReturnFlag,0);
        //等待返回,等待指定变量不是指定值
        liquidDetectState = BoardLiquidDetectWaitResp(TIME_OUT_MS_BOARD_LIQUID_DETECT,ENABLE,&liquidDetectWriteResReturnFlag,0);
        if(liquidDetectState == SUCCESS)
        {
            //成功
            break;
        }
        //没检测到,重试次数增加
        retryCount++;
    }while(retryCount < LIQUID_DETECT_BOARD_COMM_ERR_RETRY_COUNT);
    //释放设备锁
    BoardLiquidDetectReleaseLock();
    //解析结果
    if(liquidDetectState == ERROR)
    {
        MCU_LogShowErrorSubCode(ERROR_SUB_BOARD_LIQUID_DETECT_WRITE_BASE_RES);
        return ERROR_SUB_BOARD_LIQUID_DETECT_WRITE_BASE_RES;
    }
    else
    {
        return ERROR_SUB_OK;
    }
}

//关闭液面探测
ERROR_SUB BoardLiquidDetectStop(void)
{
    ErrorStatus liquidDetectState = SUCCESS;
    //获取设备锁
    BoardLiquidDetectGetLock();
    //首先创建指令缓存
    BoardLiquidDetectCreateCmd(LIQUID_DETECT_CMD_FUNC_SWITCH_LIQUID_ENABLE,0);
    uint8_t retryCount = 0;
    do
    {
        //发送指令,发送前设置指定变量为指定值
        BoardLiquidDetectSendCommand(cmdBufferLocal,5,&flagLiquidDetectEnbaleFunc,0);
        //等待返回,等待指定变量不是指定值
        liquidDetectState = BoardLiquidDetectWaitResp(TIME_OUT_MS_BOARD_LIQUID_DETECT,ENABLE,&flagLiquidDetectEnbaleFunc,0);
        if(liquidDetectState == SUCCESS)
        {
            //成功
            break;
        }
        //没检测到,重试次数增加
        retryCount++;
    }while(retryCount < LIQUID_DETECT_BOARD_COMM_ERR_RETRY_COUNT);
    //释放设备锁
    BoardLiquidDetectReleaseLock();
    //解析结果
    if(liquidDetectState == ERROR)
    {
        MCU_LogShowErrorSubCode(ERROR_SUB_BOARD_LIQUID_DETECT_DISABLE_FUNC);
        return ERROR_SUB_BOARD_LIQUID_DETECT_DISABLE_FUNC;
    }
    else
    {
        return ERROR_SUB_OK;
    }
}

//打开液面检测
ERROR_SUB BoardLiquidDetectStart(BOARD_LIQUID_DETECT_FUNC enableMode,uint8_t* funcResultPtr)
{
    ErrorStatus liquidDetectState = SUCCESS;
    //获取设备锁
    BoardLiquidDetectGetLock();
    //首先创建指令缓存
    BoardLiquidDetectCreateCmd(LIQUID_DETECT_CMD_FUNC_SWITCH_LIQUID_ENABLE,(uint16_t)enableMode);
    uint8_t retryCount = 0;
    do
    {
        //发送指令,发送前设置指定变量为指定值
        BoardLiquidDetectSendCommand(cmdBufferLocal,5,&flagLiquidDetectEnbaleFunc,0);
        //等待返回,等待指定变量不是指定值
        liquidDetectState = BoardLiquidDetectWaitResp(TIME_OUT_MS_BOARD_LIQUID_DETECT_ENABLE,ENABLE,&flagLiquidDetectEnbaleFunc,0);
        if(liquidDetectState == SUCCESS)
        {
            //成功
            break;
        }
        //没检测到,重试次数增加
        retryCount++;
    }while(retryCount < LIQUID_DETECT_BOARD_COMM_ERR_RETRY_COUNT);
    //释放设备锁
    BoardLiquidDetectReleaseLock();
    //解析结果
    if(liquidDetectState == ERROR)
    {
        MCU_LogShowErrorSubCode(ERROR_SUB_BOARD_LIQUID_DETECT_ENABLE_FUNC);
        return ERROR_SUB_BOARD_LIQUID_DETECT_ENABLE_FUNC;
    }
    //查看基准电压级别
    if(stateLiquidDetectEnbale == (uint16_t)enableMode)
    {
        *funcResultPtr = 0X00;//打开成功
    }
    else if(stateLiquidDetectEnbale == 0)
    {
        *funcResultPtr = 0X01;//打开失败
    }
    else
    {
        *funcResultPtr = stateLiquidDetectEnbale;
    }
    return ERROR_SUB_OK;
}

//液面探测自校准
ERROR_SUB BoardLiquidDetectSelfAdjust(uint32_t timeoutMs)
{
    ErrorStatus liquidDetectState = SUCCESS;
    //获取设备锁
    BoardLiquidDetectGetLock();
    //首先创建指令缓存
    BoardLiquidDetectCreateCmd(LIQUID_DETECT_CMD_FUNC_ADJUST_BASE,0);
    uint8_t retryCount = 0;
    do
    {
        //发送指令,发送前设置指定变量为指定值
        BoardLiquidDetectSendCommand(cmdBufferLocal,5,&flagLiquidDetectAdjustBase,0);
        //等待返回,等待指定变量不是指定值
        liquidDetectState = BoardLiquidDetectWaitResp(timeoutMs,ENABLE,&flagLiquidDetectAdjustBase,0);
        if(liquidDetectState == SUCCESS)
        {
            //成功
            break;
        }
        //没检测到,重试次数增加
        retryCount++;
    }while(retryCount < LIQUID_DETECT_BOARD_COMM_ERR_RETRY_COUNT);
    //释放设备锁
    BoardLiquidDetectReleaseLock();
    //解析结果
    if(liquidDetectState == ERROR)
    {
        MCU_LogShowErrorSubCode(ERROR_SUB_BOARD_LIQUID_DETECT_SELF_ADJUST);
        return ERROR_SUB_BOARD_LIQUID_DETECT_SELF_ADJUST;
    }
    //查看校准结果
    if(resultLiquidDetectAdjustBase == 0x00)
    {
        return ERROR_SUB_OK;
    }
    else if(resultLiquidDetectAdjustBase == 0x01)
    {
        MCU_LogShowErrorSubCode(ERROR_SUB_BOARD_LIQUID_DETECT_SELF_ADJUST_MAX_NOT_REACH);
        //校准达不到最大值
        return ERROR_SUB_BOARD_LIQUID_DETECT_SELF_ADJUST_MAX_NOT_REACH;
    }
    else if(resultLiquidDetectAdjustBase == 0x02)
    {
        MCU_LogShowErrorSubCode(ERROR_SUB_BOARD_LIQUID_DETECT_SELF_ADJUST_MIN_NOT_REACH);
        //校准达不到最小值
        return ERROR_SUB_BOARD_LIQUID_DETECT_SELF_ADJUST_MIN_NOT_REACH;
    }
    else if(resultLiquidDetectAdjustBase == 0x03)
    {
        MCU_LogShowErrorSubCode(ERROR_SUB_BOARD_LIQUID_DETECT_SELF_ADJUST_FAIL);
        //校准找不到合适的校准值
        return ERROR_SUB_BOARD_LIQUID_DETECT_SELF_ADJUST_FAIL;
    }
    else
    {
        return ERROR_SUB_OK;
    }
}

//开关液面探测数据上传
ERROR_SUB BoardLiquidDetectSetDataUpload(FunctionalState setState)
{
    ErrorStatus liquidDetectState = SUCCESS;
    uint16_t paramVal = 0;
    if(setState == ENABLE)
    {
        paramVal = 1;
    }
    else
    {
        paramVal = 0;
    }
    //获取设备锁
    BoardLiquidDetectGetLock();
    //首先创建指令缓存
    BoardLiquidDetectCreateCmd(LIQUID_DETECT_CMD_FUNC_SWITCH_LIQUID_DATA_UPDATE,paramVal);
    uint8_t retryCount = 0;
    do
    {
        //发送指令,发送前设置指定变量为指定值
        BoardLiquidDetectSendCommand(cmdBufferLocal,5,&flagLiquidDetectSwitchDataUpdate,0);
        //等待返回,等待指定变量不是指定值
        liquidDetectState = BoardLiquidDetectWaitResp(TIME_OUT_MS_BOARD_LIQUID_DETECT,ENABLE,&flagLiquidDetectSwitchDataUpdate,0);
        if(liquidDetectState == SUCCESS)
        {
            //成功
            break;
        }
        //没检测到,重试次数增加
        retryCount++;
    }while(retryCount < LIQUID_DETECT_BOARD_COMM_ERR_RETRY_COUNT);
    //释放设备锁
    BoardLiquidDetectReleaseLock();
    //解析结果
    if(liquidDetectState == ERROR)
    {
        MCU_LogShowErrorSubCode(ERROR_SUB_BOARD_LIQUID_DETECT_SET_DATA_UPLOAD);
        return ERROR_SUB_BOARD_LIQUID_DETECT_SET_DATA_UPLOAD;
    }
    else
    {
        return ERROR_SUB_OK;
    }
}

//停止液面探测数据接收
void BoardLiquidDetectStopRecv(void)
{
    uint32_t txResult = TX_SUCCESS;
    UART_RECV_PACK* queueMsgPtr = NULL;
    //获取设备锁
    BoardLiquidDetectGetLock();
    //定时器停止
    tx_timer_deactivate(&timerBoardLiquidDetectRecvMsg);
    //清空消息队列
    do
    {
        queueMsgPtr = NULL;
        txResult = tx_queue_receive(&queueBoardLiquidDetectRecvMsg,(void*)(&queueMsgPtr),TX_NO_WAIT);
        if((txResult != TX_SUCCESS)||(queueMsgPtr == NULL))
        {
            //没有消息
            break;
        }
        //存在消息,进行解析
        if(queueMsgPtr->packSrc != COMM_PACK_SRC_BOARD_LIQUID_DETECT)
        {
            //不是我想要的数据
            continue;
        }
        //处理完成,释放数据包
        RELEASE_PACK_BOARD_LIQUID_DETECT(queueMsgPtr);
    }while(1);
    //清除内部缓存
    recvDataLengthBoardLiquidDetect = 0;
    UserMemClear(recvDataBufferBoardLiquidDetect,COMM_CACHE_LENGTH_BOARD_LIQUID_DETECT);
    //释放设备锁
    BoardLiquidDetectReleaseLock();
}





