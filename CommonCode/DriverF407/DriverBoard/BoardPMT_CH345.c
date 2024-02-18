#include "BoardPMT_CH345.h"
#include "tx_mutex.h"
#include "ADT_CircularDWordQueue.h"
#include "DriverConfigBoard.h"

//设备当前通讯模式设定
typedef enum PMT_CH345_COMM_MODE
{
    PMT_CH345_COMM_MODE_NONE            = 0X00,//无模式设定
    PMT_CH345_COMM_MODE_CHECK           = 0X01,//通讯测试模式
    PMT_CH345_COMM_MODE_VERSION         = 0X02,//版本检测模式
    PMT_CH345_COMM_MODE_SET_GATE_TIME   = 0X03,//设置门控时间
    PMT_CH345_COMM_MODE_SET_PLUSE_UNIT  = 0X04,//设置脉冲分辨时间
    PMT_CH345_COMM_MODE_STOP            = 0x05,//停止并清空数据
    PMT_CH345_COMM_MODE_SINGLE_SAMPLE   = 0X06,//单次采样模式
    PMT_CH345_COMM_MODE_CYCLIC_SAMPLE   = 0X07,//循环采样模式
    PMT_CH345_COMM_MODE_READ_LAST_RESULT= 0X08,//读取上一次采样结果
}PMT_CH345_COMM_MODE;

//设备互斥信号量
static TX_MUTEX mutexBoardPmtCh345;
//设备消息队列
static TX_QUEUE queueBoardPmtCh345RecvMsg;
//消息缓冲区
uint32_t queueBufferBoardPmtCh345RecvMsg[LENGTH_QUEUE_BOARD_PMT_CH345_RECV_MSG];
//设备定时器,用于定时处理串口消息
static TX_TIMER timerBoardPmtCh345RecvMsg;
//设备当前模式
static PMT_CH345_COMM_MODE currentCommMode = PMT_CH345_COMM_MODE_NONE;
//当前接收数据长度
static uint16_t recvDataLengthBoardPmtCh345;
//当前接受数据缓存
static uint8_t recvDataBufferBoardPmtCh345[COMM_CACHE_LENGTH_BOARD_PMT_CH345];
//接收数据队列
static ADT_CIRCULAR_DWORD_QUEUE_T recvQueueMeasureLightPointPtr;
//临时指令缓存
static uint8_t cmdBufferLocal[COMM_CACHE_LENGTH_BOARD_PMT_CH345];

/*---------------------------------预定义光子测量模块体系----------------------------------*/
//检测串口通断
static const uint8_t commandPmtCh345CheckExist[]            = "Q\n";
//预期返回字符串    
static const uint8_t respPmtCh345CheckExist[]               = "!";
//等待数据返回的超时时间    
#define TIME_OUT_MS_PMT_CH345_CMD_CHECK_EXIST               500
//预期返回数据长度  
#define EXCEPT_RECV_LEN_PMT_CH345_CMD_CHECK_EXIST           1
//匹配返回数据长度  
#define EXCEPT_MATCH_LEN_PMT_CH345_CMD_CHECK_EXIST          1


//读取版本号
static const uint8_t commandPmtCh345ReadDeviceVersion[]     = "?\n";
//预期返回字符串  
static const uint8_t respPmtCh345ReadDeviceVersion[]        = "V6.1.10";
//等待数据返回的超时时间
#define TIME_OUT_MS_PMT_CH345_CMD_READ_DEVICE_VERSION       500
//预期返回数据长度
#define EXCEPT_RECV_LEN_PMT_CH345_CMD_READ_DEVICE_VERSION   7
//匹配返回数据长度
#define EXCEPT_MATCH_LEN_PMT_CH345_CMD_READ_DEVICE_VERSION  4

//设置门控时间
//预期返回字符串  
static const uint8_t respPmtCh345SetGateTimeUnit10Ms[]      = "TXX";
//等待数据返回的超时时间
#define TIME_OUT_MS_PMT_CH345_CMD_SET_GATE_TIME             500
//预期返回数据长度
#define EXCEPT_RECV_LEN_PMT_CH345_CMD_SET_GATE_TIME         3
//匹配返回数据长度
#define EXCEPT_MATCH_LEN_PMT_CH345_CMD_SET_GATE_TIME        1

//设置脉冲分辨时间
//预期返回字符串  
static const uint8_t respPmtCh345SetPluseUnitNs[]           = "CX";
//等待数据返回的超时时间
#define TIME_OUT_MS_PMT_CH345_CMD_SET_PLUSE_UNIT            500
//预期返回数据长度
#define EXCEPT_RECV_LEN_PMT_CH345_CMD_SET_PLUSE_UNIT        2
//匹配返回数据长度
#define EXCEPT_MATCH_LEN_PMT_CH345_CMD_SET_PLUSE_UNIT       1


//采样停止
static const uint8_t commandPmtCh345Stop[]                  = "E\n";
//预期返回字符串  
static const uint8_t respPmtCh345Stop[]                     = "E";
//等待数据返回的超时时间
#define TIME_OUT_MS_PMT_CH345_CMD_STOP                      500
//预期返回数据长度
#define EXCEPT_RECV_LEN_PMT_CH345_CMD_STOP                  1
//匹配返回数据长度
#define EXCEPT_MATCH_LEN_PMT_CH345_CMD_STOP                 1

//单次采样
static const uint8_t commandPmtCh345SingleSample[]          = "J\n";
//循环采样
static const uint8_t commandPmtCh345CyclicSample[]          = "S\n";

//串口接收转发函数
static ErrorStatus BoardPMT_CH345_RecvMsgDispatch(UART_RECV_PACK* uartPackRecvPtr)
{
    uint32_t txResult = TX_SUCCESS;
    txResult = tx_queue_send(&queueBoardPmtCh345RecvMsg,(void*)(&uartPackRecvPtr),TX_NO_WAIT);
    if(txResult != TX_SUCCESS)
    {
        MCU_RTT_SendStringInt("BoardPMT_CH345_RecvMsgDispatch tx_queue_send Failed\r\n");
        return ERROR;
    }
    return SUCCESS;
}

//数据处理
static void BoardPMT_CH345_RecvPackProcess(uint8_t* packBufPtr,uint16_t packLength)
{
    uint16_t indexUtil = 0;
    uint32_t resultLightSensor = 0;
    if((currentCommMode == PMT_CH345_COMM_MODE_CHECK)||(currentCommMode == PMT_CH345_COMM_MODE_VERSION)||
        (currentCommMode == PMT_CH345_COMM_MODE_SET_GATE_TIME)||(currentCommMode == PMT_CH345_COMM_MODE_SET_PLUSE_UNIT)||
        (currentCommMode == PMT_CH345_COMM_MODE_STOP))
    {
        //通用指令,使用缓存存储(通讯测试,版本检测,门控时间设置,脉冲分辨时间设置,测光停止)
        for(indexUtil = 0; indexUtil < packLength; indexUtil++)
        {
            if(recvDataLengthBoardPmtCh345 < COMM_CACHE_LENGTH_BOARD_PMT_CH345)
            {
                recvDataBufferBoardPmtCh345[recvDataLengthBoardPmtCh345] = packBufPtr[indexUtil];
                recvDataLengthBoardPmtCh345++;
            }
        }
    }
    else if((currentCommMode == PMT_CH345_COMM_MODE_SINGLE_SAMPLE)||(currentCommMode == PMT_CH345_COMM_MODE_CYCLIC_SAMPLE))
    {
        //数据采集模式
        for(indexUtil = 0; indexUtil < packLength; indexUtil++)
        {
            if((recvDataLengthBoardPmtCh345 == 0)&&(packBufPtr[indexUtil] == 0XFF))
            {
                //数据长度为0的时候,必须受到0XFF
                recvDataBufferBoardPmtCh345[recvDataLengthBoardPmtCh345] = packBufPtr[indexUtil];
                recvDataLengthBoardPmtCh345++;
            }
            else
            {
                //正常接收
                recvDataBufferBoardPmtCh345[recvDataLengthBoardPmtCh345] = packBufPtr[indexUtil];
                recvDataLengthBoardPmtCh345++;
            }
            //检查是否收满一帧
            if(recvDataLengthBoardPmtCh345 == 6)
            {
                resultLightSensor = 0;
                resultLightSensor += recvDataBufferBoardPmtCh345[1];
                resultLightSensor += recvDataBufferBoardPmtCh345[2];
                resultLightSensor += recvDataBufferBoardPmtCh345[3];
                resultLightSensor += recvDataBufferBoardPmtCh345[4];
                if(recvDataBufferBoardPmtCh345[5] == ((uint8_t)resultLightSensor))
                {
                    //校验和正确
                    resultLightSensor = 0;
                    resultLightSensor += recvDataBufferBoardPmtCh345[1];
                    resultLightSensor <<= 8;
                    resultLightSensor += recvDataBufferBoardPmtCh345[2];
                    resultLightSensor <<= 8;
                    resultLightSensor += recvDataBufferBoardPmtCh345[3];
                    resultLightSensor <<= 8;
                    resultLightSensor += recvDataBufferBoardPmtCh345[4]; 
                    //将数据保存到队列中
                    ADT_CircularDWordQueueWriteOnce(recvQueueMeasureLightPointPtr,resultLightSensor);
                }
                else
                {
                    //校验和不正确
                    MCU_LogPrintf("PMT CH345,CheckSum,Recv: %d,Calc: %d\r\n",recvDataBufferBoardPmtCh345[5],(uint8_t)resultLightSensor);
                }
                //处理完成,重新归零
                recvDataLengthBoardPmtCh345 = 0;
            }
        }
    }
    else
    {
        //没有任何驱动收到指令
        //不处理,直接返回
    }
}


//串口接收处理函数
static void BoardPMT_CH345_RecvMsgProcessPeriod(ULONG unusedParam)
{
    //从消息队列中获取消息
    uint32_t txResult = TX_SUCCESS;
    UART_RECV_PACK* queueMsgPtr = NULL;
    do
    {
        queueMsgPtr = NULL;
        txResult = tx_queue_receive(&queueBoardPmtCh345RecvMsg,(void*)(&queueMsgPtr),TX_NO_WAIT);
        if((txResult != TX_SUCCESS)||(queueMsgPtr == NULL))
        {
            //没有消息
            return;
        }
        //存在消息,进行解析
        if(queueMsgPtr->packSrc != COMM_PACK_SRC_BOARD_PMT_CH345)
        {
            //不是我想要的数据
            continue;
        }
        //数据处理
        BoardPMT_CH345_RecvPackProcess(queueMsgPtr->uartRecvDataBuffer,queueMsgPtr->uartRecvDataLength);
        //处理完成,释放数据包
        RELEASE_PACK_BOARD_PMT_CH345(queueMsgPtr);
    }while(1);
}

//清除接收缓存
static void BoardPMT_CH345_ClearRecvCache(void)
{
    uint32_t txResult = TX_SUCCESS;
    UART_RECV_PACK* queueMsgPtr = NULL;
    //定时器停止
    tx_timer_deactivate(&timerBoardPmtCh345RecvMsg);
    //清空消息队列
    do
    {
        queueMsgPtr = NULL;
        txResult = tx_queue_receive(&queueBoardPmtCh345RecvMsg,(void*)(&queueMsgPtr),TX_NO_WAIT);
        if((txResult != TX_SUCCESS)||(queueMsgPtr == NULL))
        {
            //没有消息
            break;
        }
        //存在消息,进行解析
        if(queueMsgPtr->packSrc != COMM_PACK_SRC_BOARD_PMT_CH345)
        {
            //不是我想要的数据
            continue;
        }
        //处理完成,释放数据包
        RELEASE_PACK_BOARD_PMT_CH345(queueMsgPtr);
    }while(1);
    //清除内部缓存
    recvDataLengthBoardPmtCh345 = 0;
    UserMemClear(recvDataBufferBoardPmtCh345,COMM_CACHE_LENGTH_BOARD_PMT_CH345);
    //清除测光数据
    ADT_CircularDWordQueueClear(recvQueueMeasureLightPointPtr);
    //通讯模式设置为空
    currentCommMode = PMT_CH345_COMM_MODE_NONE;
}

//获取设备锁
static void BoardPMT_CH345_GetLock(void)
{
    tx_mutex_get(&mutexBoardPmtCh345,TX_WAIT_FOREVER);
}

//释放设备锁
static void BoardPMT_CH345_ReleaseLock(void)
{
    tx_mutex_put(&mutexBoardPmtCh345);
}

//光子测量模块端口初始化
void BoardPMT_CH345_PortInit(void)
{
    //初始化相关资源
    if(mutexBoardPmtCh345.tx_mutex_id == TX_MUTEX_ID)
    {
        //已经初始化,直接返回
        return;
    }
    //创建互斥信号量
    tx_mutex_create(&mutexBoardPmtCh345,"mutexBoardPmtCh345",TX_INHERIT);
    //创建消息队列
    tx_queue_create(&queueBoardPmtCh345RecvMsg,"queueBoardPmtCh345RecvMsg",1,(void*)queueBufferBoardPmtCh345RecvMsg,
                        sizeof(queueBufferBoardPmtCh345RecvMsg)/sizeof(uint8_t));
    //创建定时器任务,创建的时候不使能
    tx_timer_create(&timerBoardPmtCh345RecvMsg,"timerBoardPmtCh345RecvMsg",BoardPMT_CH345_RecvMsgProcessPeriod,0,
                        MS_TO_OS_TICK(PERIOD_MS_PROC_BOARD_PMT_CH345_RECV_MSG),
                        MS_TO_OS_TICK(PERIOD_MS_PROC_BOARD_PMT_CH345_RECV_MSG),
                        TX_NO_ACTIVATE);
    //创建测光点
    recvQueueMeasureLightPointPtr = ADT_CircularDWordQueueCreate(MAX_COUNT_BOARD_PMT_CH345_MEASURE_LIGHT_POINT,ADT_FULL_OPTION_BLOCK,
                                                                    MEM_MALLOC_BOARD_PMT_CH345_PTR,MEM_FREE_BOARD_PMT_CH345_PTR,
                                                                    CoreLockIntBasePri,CoreUnlockIntBasePri,MCU_RTT_SendStringInt);
    //串口资源初始化
    COMM_INIT_BOARD_PMT_CH345(BOARD_PMT_CH345_BAUD,BOARD_PMT_CH345_LENGTH,BOARD_PMT_CH345_STOP_BIT,BOARD_PMT_CH345_CHECK_MODE,
                                BOARD_PMT_CH345_HARD_CONTROL);
    //回调函数注册
    COMM_CALLBACL_REGISTER_BOARD_PMT_CH345(BoardPMT_CH345_RecvMsgDispatch,NULL,NULL);
    //接收数据缓存清空
    BoardPMT_CH345_ClearRecvCache();
}

//发送指令
static void BoardPMT_CH345_SendCommand(PMT_CH345_COMM_MODE mode,uint8_t* cmdStringPtr)
{
    //清空缓存
    BoardPMT_CH345_ClearRecvCache();
    //设置通讯模式
    currentCommMode = mode;
    //启动定时器
    tx_timer_activate(&timerBoardPmtCh345RecvMsg);
    //首先发送指令
    COMM_SEND_CMD_STRING_BOARD_PMT_CH345(cmdStringPtr);
}

//等待返回
static ErrorStatus BoardPMT_CH345_WaitResp(uint32_t timeOutMs,uint16_t exceptRespLength)
{
    //等待开始时间
    uint32_t cmdSendOverTimeStamp = tx_time_get();
    //查询结果
    do
    {
        if(recvDataLengthBoardPmtCh345 >= exceptRespLength)
        {
            //收到了预期长度,返回
            break;
        }
        //延时,延时时间与定时器时间相等
        CoreDelayMs(PERIOD_MS_PROC_BOARD_PMT_CH345_RECV_MSG);
    }while (tx_time_get() - cmdSendOverTimeStamp < MS_TO_OS_TICK(timeOutMs));
    //退出查询,停止定时器
    tx_timer_deactivate(&timerBoardPmtCh345RecvMsg);
    //模式设置
    currentCommMode = PMT_CH345_COMM_MODE_NONE;
    //超时没收到指定数据
    if(recvDataLengthBoardPmtCh345 < exceptRespLength)
    {
        return ERROR;
    }
    return SUCCESS;
}

//匹配结果
static ErrorStatus BoardPMT_CH345_MatchResultBuffer(uint8_t* matchBufferPtr,uint16_t mathLength)
{
    //等到数据了,执行数据匹配
    if(0 != UserMemCmp(recvDataBufferBoardPmtCh345,(uint8_t*)matchBufferPtr,mathLength))
    {
        return ERROR;
    }
    return SUCCESS;
}

//测试光子测量模块通讯是否正常
static ERROR_SUB BoardPMT_CH345_CheckExist(void)
{
    ErrorStatus pmtState = SUCCESS;
    //发送指令
    BoardPMT_CH345_SendCommand(PMT_CH345_COMM_MODE_CHECK,(uint8_t*)commandPmtCh345CheckExist);
    //等待返回
    pmtState = BoardPMT_CH345_WaitResp(TIME_OUT_MS_PMT_CH345_CMD_CHECK_EXIST,
                                            EXCEPT_RECV_LEN_PMT_CH345_CMD_CHECK_EXIST);
    if(pmtState == ERROR)
    {
        //超时没等到数据
        return ERROR_SUB_BOARD_PMT_CH345_CHECK_EXIST_TIME_OUT;
    }
    //等到数据了,执行数据匹配
    pmtState = BoardPMT_CH345_MatchResultBuffer((uint8_t*)respPmtCh345CheckExist,EXCEPT_MATCH_LEN_PMT_CH345_CMD_CHECK_EXIST);
    //没匹配上
    if(pmtState == ERROR)
    {
        return ERROR_SUB_BOARD_PMT_CH345_CHECK_EXIST_RESP_MATCH;
    }
    //正常匹配上了
    return ERROR_SUB_OK;
}


//光子测量模块版本号检测
static ERROR_SUB BoardPMT_CH345_CheckVersionCode(void)
{
    ErrorStatus pmtState = SUCCESS;
    //发送指令
    BoardPMT_CH345_SendCommand(PMT_CH345_COMM_MODE_VERSION,(uint8_t*)commandPmtCh345ReadDeviceVersion);
    //等待返回
    pmtState = BoardPMT_CH345_WaitResp(TIME_OUT_MS_PMT_CH345_CMD_READ_DEVICE_VERSION,
                                            EXCEPT_RECV_LEN_PMT_CH345_CMD_READ_DEVICE_VERSION);
    if(pmtState == ERROR)
    {
        //超时没等到数据
        return ERROR_SUB_BOARD_PMT_CH345_READ_DEVICE_VERSION_TIME_OUT;
    }
    //等到数据了,执行数据匹配
    pmtState = BoardPMT_CH345_MatchResultBuffer((uint8_t*)respPmtCh345ReadDeviceVersion,
                                                    EXCEPT_MATCH_LEN_PMT_CH345_CMD_READ_DEVICE_VERSION);
    //没匹配上
    if(pmtState == ERROR)
    {
        return ERROR_SUB_BOARD_PMT_CH345_READ_DEVICE_VERSION_RESP_MATCH;
    }
    //打印版本号
    MCU_LogPrintf("PMT CH345 Version: %s\r\n",recvDataBufferBoardPmtCh345);
    //正常匹配上了
    return ERROR_SUB_OK;
}

//设置光子测量模块的门控时间
static ERROR_SUB BoardPMT_CH345_SetGateTimeUnit10Ms(uint16_t gateTimeMultipleMs)
{
    ErrorStatus pmtState = SUCCESS;
    //锁定线程
    uint32_t oldState = CoreLockSchedule();
    //数据清空
    UserMemClear(cmdBufferLocal,COMM_CACHE_LENGTH_BOARD_PMT_CH345);
    if(gateTimeMultipleMs < 10)
    {
        gateTimeMultipleMs = 10;
    }
    else
    {
        gateTimeMultipleMs /= 10;
    }
    //指令串转换
    snprintf((char *)cmdBufferLocal, COMM_CACHE_LENGTH_BOARD_PMT_CH345,"T%d\n",gateTimeMultipleMs);
    //格式转换完成,打开调度器
    CoreUnlockSchedule(oldState);
    //发送指令
    BoardPMT_CH345_SendCommand(PMT_CH345_COMM_MODE_SET_GATE_TIME,cmdBufferLocal);
    //等待返回
    pmtState = BoardPMT_CH345_WaitResp(TIME_OUT_MS_PMT_CH345_CMD_SET_GATE_TIME,
                                            EXCEPT_RECV_LEN_PMT_CH345_CMD_SET_GATE_TIME);
    if(pmtState == ERROR)
    {
        //超时没等到数据
        return ERROR_SUB_BOARD_PMT_CH345_SET_GATE_TIME_TIME_OUT;
    }
    //等到数据了,执行数据匹配
    pmtState = BoardPMT_CH345_MatchResultBuffer((uint8_t*)respPmtCh345SetGateTimeUnit10Ms,
                                                    EXCEPT_MATCH_LEN_PMT_CH345_CMD_SET_GATE_TIME);
    //没匹配上
    if(pmtState == ERROR)
    {
        return ERROR_SUB_BOARD_PMT_CH345_SET_GATE_TIME_RESP_MATCH;
    }
    //检测返回的门控时间和设置的门控时间是否一致
    uint16_t returnGateTime = 0;
    returnGateTime += recvDataBufferBoardPmtCh345[1];
    returnGateTime <<= 8;
    returnGateTime += recvDataBufferBoardPmtCh345[2];
    if(gateTimeMultipleMs != returnGateTime)
    {
        //发出的门控时间和返回的门控时间不匹配
        return ERROR_SUB_BOARD_PMT_CH345_SET_GATE_TIME_SRC_DST_MATCH;
    }
    return ERROR_SUB_OK;
}

//设置光子测量模块探头脉冲对分辨时间
static ERROR_SUB BoardPMT_CH345_SetPluseResolutionTimeUnitNs(uint8_t timeNs)
{
    ErrorStatus pmtState = SUCCESS;
    //锁定线程
    uint32_t oldState = CoreLockSchedule();
    //数据清空
    UserMemClear(cmdBufferLocal,COMM_CACHE_LENGTH_BOARD_PMT_CH345);
    //指令串转换
    snprintf((char *)cmdBufferLocal, COMM_CACHE_LENGTH_BOARD_PMT_CH345,"C%d\n",timeNs);
    //发送指令
    BoardPMT_CH345_SendCommand(PMT_CH345_COMM_MODE_SET_PLUSE_UNIT,cmdBufferLocal);
    //格式转换完成,打开调度器
    CoreUnlockSchedule(oldState);
    //等待返回
    pmtState = BoardPMT_CH345_WaitResp(TIME_OUT_MS_PMT_CH345_CMD_SET_PLUSE_UNIT,
                                            EXCEPT_RECV_LEN_PMT_CH345_CMD_SET_PLUSE_UNIT);
    if(pmtState == ERROR)
    {
        //超时没等到数据
        return ERROR_SUB_BOARD_PMT_CH345_SET_PLUSE_UNIT_TIME_OUT;
    }
    //等到数据了,执行数据匹配
    pmtState = BoardPMT_CH345_MatchResultBuffer((uint8_t*)respPmtCh345SetPluseUnitNs,
                                                    EXCEPT_MATCH_LEN_PMT_CH345_CMD_SET_PLUSE_UNIT);
    //没匹配上
    if(pmtState == ERROR)
    {
        return ERROR_SUB_BOARD_PMT_CH345_SET_PLUSE_UNIT_RESP_MATCH;
    }
    //检测返回的门控时间和设置的门控时间是否一致
    uint8_t returnPluseUnit = 0;
    returnPluseUnit += recvDataBufferBoardPmtCh345[1];
    if(returnPluseUnit != timeNs)
    {
        //发出的门控时间和返回的门控时间不匹配
        return ERROR_SUB_BOARD_PMT_CH345_SET_PLUSE_UNIT_SRC_DST_MATCH;
    }
    return ERROR_SUB_OK;
}

//计数单元停止发送数据并清零
ERROR_SUB BoardPMT_CH345_StopPhotonCountAndClear(void)
{
    ErrorStatus pmtState = SUCCESS;
    //发送指令
    BoardPMT_CH345_SendCommand(PMT_CH345_COMM_MODE_STOP,(uint8_t*)commandPmtCh345Stop);
    //等待返回
    pmtState = BoardPMT_CH345_WaitResp(TIME_OUT_MS_PMT_CH345_CMD_STOP,EXCEPT_RECV_LEN_PMT_CH345_CMD_STOP);
    if(pmtState == ERROR)
    {
        //超时没等到数据
        return ERROR_SUB_BOARD_PMT_CH345_STOP_TIME_OUT;
    }
    //等到数据了,执行数据匹配
    pmtState = BoardPMT_CH345_MatchResultBuffer((uint8_t*)respPmtCh345Stop,EXCEPT_MATCH_LEN_PMT_CH345_CMD_STOP);
    //没匹配上
    if(pmtState == ERROR)
    {
        return ERROR_SUB_BOARD_PMT_CH345_STOP_RESP_MATCH;
    }
    //正常匹配上了
    return ERROR_SUB_OK;
}

//将光子测量模块申请的数据释放掉,主要用在多次数据采集中
void BoardPMT_CH345_ReleaseMem(void* memPtr)
{
    if(memPtr != NULL)
    {
        MEM_FREE_BOARD_PMT_CH345(memPtr);
    }
}

//启动计数,连续发送计数值
static void BoardPMT_CH345_StartContinuousPhotonCount(void)
{
    //发送指令
    BoardPMT_CH345_SendCommand(PMT_CH345_COMM_MODE_CYCLIC_SAMPLE,(uint8_t*)commandPmtCh345CyclicSample);
}

//启动计数,发送单次计数值,启动并执行一次
static void BoardPMT_CH345_StartSinglePhotonCount(void)
{
    //发送指令
    BoardPMT_CH345_SendCommand(PMT_CH345_COMM_MODE_SINGLE_SAMPLE,(uint8_t*)commandPmtCh345SingleSample);
}

////读取上一次单次计数的结果
//static void BoardPMT_CH345_ReadLastPhotonCountFormDevice(uint32_t* lastResult)
//{
//    //发送指令
//    BoardPMT_CH345_SendCommand(PMT_CH345_COMM_MODE_READ_LAST_RESULT,(uint8_t*)commandPmtCh345ReadLastResult);
//}

//光子测量模块配置初始化状态
ERROR_SUB BoardPMT_CH345_ConfigDefault(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t retryCount = 0;
    //首先获取设备锁
    BoardPMT_CH345_GetLock();
    //检测设备是否在线
    retryCount = 0;
    do
    {
        errorSub = BoardPMT_CH345_CheckExist();
        if(errorSub != ERROR_SUB_OK)
        {
            //异常
            retryCount++;
            if(retryCount >= RETRY_MAX_COMM_BOARD_PMT_CH345)
            {
                break;
            }
            //继续下一次
            continue;
        }
        //正常完成
        break;
    }while(retryCount < RETRY_MAX_COMM_BOARD_PMT_CH345);
    if(retryCount >= RETRY_MAX_COMM_BOARD_PMT_CH345)
    {
        //释放设备锁
        BoardPMT_CH345_ReleaseLock();
        //返回错误
        return errorSub;
    }
    //检测设备版本号
    retryCount = 0;
    do
    {
        errorSub = BoardPMT_CH345_CheckVersionCode();
        if(errorSub != ERROR_SUB_OK)
        {
            //异常
            retryCount++;
            if(retryCount >= RETRY_MAX_COMM_BOARD_PMT_CH345)
            {
                break;
            }
            //继续下一次
            continue;
        }
        //正常完成
        break;
    }while(retryCount < RETRY_MAX_COMM_BOARD_PMT_CH345);
    if(retryCount >= RETRY_MAX_COMM_BOARD_PMT_CH345)
    {
        //释放设备锁
        BoardPMT_CH345_ReleaseLock();
        //返回错误
        return errorSub;
    }
    //设置默认脉冲分辨时间
    retryCount = 0;
    do
    {
        errorSub = BoardPMT_CH345_SetPluseResolutionTimeUnitNs(UNIT_TIME_NS_PLUSE_BOARD_PMT_CH345);
        if(errorSub != ERROR_SUB_OK)
        {
            //异常
            retryCount++;
            if(retryCount >= RETRY_MAX_COMM_BOARD_PMT_CH345)
            {
                break;
            }
            //继续下一次
            continue;
        }
        //正常完成
        break;
    }while(retryCount < RETRY_MAX_COMM_BOARD_PMT_CH345);
    if(retryCount >= RETRY_MAX_COMM_BOARD_PMT_CH345)
    {
        //释放设备锁
        BoardPMT_CH345_ReleaseLock();
        //返回错误
        return errorSub;
    }
    //释放设备锁
    BoardPMT_CH345_ReleaseLock();
    return errorSub;
}

//测量模块单次采样开始,参数为门控时间
ERROR_SUB BoardPMT_CH345_SingleSampleStart(uint16_t gateTimeMultiple10Ms)
{
    //设置门控时间
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t retryCount = 0;
    //首先获取设备锁
    BoardPMT_CH345_GetLock();
    //计数单元停止
    retryCount = 0;
    do
    {
        errorSub = BoardPMT_CH345_StopPhotonCountAndClear();
        if(errorSub != ERROR_SUB_OK)
        {
            //异常
            retryCount++;
            if(retryCount >= RETRY_MAX_COMM_BOARD_PMT_CH345)
            {
                break;
            }
            //继续下一次
            continue;
        }
        //正常完成
        break;
    }while(retryCount < RETRY_MAX_COMM_BOARD_PMT_CH345);
    if(retryCount >= RETRY_MAX_COMM_BOARD_PMT_CH345)
    {
        //释放设备锁
        BoardPMT_CH345_ReleaseLock();
        //返回错误
        return errorSub;
    }
    //设置门控时间
    retryCount = 0;
    do
    {
        errorSub = BoardPMT_CH345_SetGateTimeUnit10Ms(gateTimeMultiple10Ms);
        if(errorSub != ERROR_SUB_OK)
        {
            //异常
            retryCount++;
            if(retryCount >= RETRY_MAX_COMM_BOARD_PMT_CH345)
            {
                break;
            }
            //继续下一次
            continue;
        }
        //正常完成
        break;
    }while(retryCount < RETRY_MAX_COMM_BOARD_PMT_CH345);
    if(retryCount >= RETRY_MAX_COMM_BOARD_PMT_CH345)
    {
        //释放设备锁
        BoardPMT_CH345_ReleaseLock();
        //返回错误
        return errorSub;
    }
    //启动单次采样
    BoardPMT_CH345_StartSinglePhotonCount();
    //释放设备锁
    BoardPMT_CH345_ReleaseLock();
    return errorSub;
}

//测量模块单次采样,查询是否收到结果
ERROR_SUB BoardPMT_CH345_SingleSampleCheck(uint16_t* lightPointCount)
{
    if(currentCommMode != PMT_CH345_COMM_MODE_SINGLE_SAMPLE)
    {
        return ERROR_SUB_BOARD_PMT_CH345_MODE_NOT_SINGLE_SAMPLE;
    }
    //首先获取设备锁
    BoardPMT_CH345_GetLock();
    //读取测光点数量
    *lightPointCount = ADT_CircularDWordQueueGetCurrentSize(recvQueueMeasureLightPointPtr);
    //释放设备锁
    BoardPMT_CH345_ReleaseLock();
    return ERROR_SUB_OK;
}

//测量模块单次采样获取采样结果
ERROR_SUB BoardPMT_CH345_SingleSampleGetResult(uint32_t* dataResultPtr)
{
    uint16_t lightPointCount = 0;
    if(currentCommMode != PMT_CH345_COMM_MODE_SINGLE_SAMPLE)
    {
        return ERROR_SUB_BOARD_PMT_CH345_MODE_NOT_SINGLE_SAMPLE;
    }
    //首先获取设备锁
    BoardPMT_CH345_GetLock();
    //读取测光点数量
    lightPointCount = ADT_CircularDWordQueueGetCurrentSize(recvQueueMeasureLightPointPtr);
    //测光点数量必须大于0
    if(lightPointCount == 0)
    {
        //释放设备锁
        BoardPMT_CH345_ReleaseLock();
        return ERROR_SUB_BOARD_PMT_CH345_SINGLE_SAMPLE_POINT_ZERO;
    }
    *dataResultPtr = 0;
    //获取测光值
    ADT_CircularDWordQueueReadOnce(recvQueueMeasureLightPointPtr,dataResultPtr);
    //清除缓存
    BoardPMT_CH345_ClearRecvCache();
    //释放设备锁
    BoardPMT_CH345_ReleaseLock();
    return ERROR_SUB_OK; 
}

//测量模块循环采样开始,参数为门控时间
ERROR_SUB BoardPMT_CH345_CyclicSampleStart(uint16_t gateTimeMultipleMs)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint8_t retryCount = 0;
    //首先获取设备锁
    BoardPMT_CH345_GetLock();
    //计数单元停止
    retryCount = 0;
    do
    {
        errorSub = BoardPMT_CH345_StopPhotonCountAndClear();
        if(errorSub != ERROR_SUB_OK)
        {
            //异常
            retryCount++;
            if(retryCount >= RETRY_MAX_COMM_BOARD_PMT_CH345)
            {
                break;
            }
            //继续下一次
            continue;
        }
        //正常完成
        break;
    }while(retryCount < RETRY_MAX_COMM_BOARD_PMT_CH345);
    if(retryCount >= RETRY_MAX_COMM_BOARD_PMT_CH345)
    {
        //释放设备锁
        BoardPMT_CH345_ReleaseLock();
        //返回错误
        return errorSub;
    }
    //设置门控时间
    retryCount = 0;
    if(gateTimeMultipleMs < 10)
    {
        gateTimeMultipleMs = 1;
    }
    else
    {
        gateTimeMultipleMs /= 10;
    }
    do
    {
        errorSub = BoardPMT_CH345_SetGateTimeUnit10Ms(gateTimeMultipleMs);
        if(errorSub != ERROR_SUB_OK)
        {
            //异常
            retryCount++;
            if(retryCount >= RETRY_MAX_COMM_BOARD_PMT_CH345)
            {
                break;
            }
            //继续下一次
            continue;
        }
        //正常完成
        break;
    }while(retryCount < RETRY_MAX_COMM_BOARD_PMT_CH345);
    if(retryCount >= RETRY_MAX_COMM_BOARD_PMT_CH345)
    {
        //释放设备锁
        BoardPMT_CH345_ReleaseLock();
        //返回错误
        return errorSub;
    }
    //启动连续采样
    BoardPMT_CH345_StartContinuousPhotonCount();
    //释放设备锁
    BoardPMT_CH345_ReleaseLock();
    return errorSub;
}

//测量模块循环采样,数据监测,存在几个数据
ERROR_SUB BoardPMT_CH345_CyclicSampleCheckCurrentPoint(uint16_t* lightPointCount)
{
    if(currentCommMode != PMT_CH345_COMM_MODE_CYCLIC_SAMPLE)
    {
        return ERROR_SUB_BOARD_PMT_CH345_MODE_NOT_CYCLIC_SAMPLE;
    }
    //首先获取设备锁
    BoardPMT_CH345_GetLock();
    //读取测光点数量
    *lightPointCount = ADT_CircularDWordQueueGetCurrentSize(recvQueueMeasureLightPointPtr);
    //释放设备锁
    BoardPMT_CH345_ReleaseLock();
    return ERROR_SUB_OK; 
}

//测量模块循环采样结束,参数为采样点数
ERROR_SUB BoardPMT_CH345_CyclicSampleGetResultStop(uint32_t** dataResultArrayPtrPtr,uint8_t* realResultCount)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint16_t lightPointCount = 0;
    uint16_t realResultCountLocal = 0;
    if(currentCommMode != PMT_CH345_COMM_MODE_CYCLIC_SAMPLE)
    {
        return ERROR_SUB_BOARD_PMT_CH345_MODE_NOT_CYCLIC_SAMPLE;
    }
    //首先获取设备锁
    BoardPMT_CH345_GetLock();
    //读取测光点数量
    lightPointCount = ADT_CircularDWordQueueGetCurrentSize(recvQueueMeasureLightPointPtr);
    if(lightPointCount == 0)
    {
        *realResultCount = 0;
        *dataResultArrayPtrPtr = NULL;
        //释放设备锁
        BoardPMT_CH345_ReleaseLock();
        return ERROR_SUB_OK; 
    }
    //申请内存
    *dataResultArrayPtrPtr = MEM_MALLOC_BOARD_PMT_CH345(lightPointCount*sizeof(uint32_t));
    //读取数据
    ADT_CircularDWordQueueReadBuffer(recvQueueMeasureLightPointPtr,*dataResultArrayPtrPtr,lightPointCount,&realResultCountLocal);
    *realResultCount = (uint8_t)realResultCountLocal;
    //发送停止采样
    uint8_t retryCount = 0;
    //计数单元停止
    retryCount = 0;
    do
    {
        errorSub = BoardPMT_CH345_StopPhotonCountAndClear();
        if(errorSub != ERROR_SUB_OK)
        {
            //异常
            retryCount++;
            if(retryCount >= RETRY_MAX_COMM_BOARD_PMT_CH345)
            {
                break;
            }
            //继续下一次
            continue;
        }
        //正常完成
        break;
    }while(retryCount < RETRY_MAX_COMM_BOARD_PMT_CH345);
    if(retryCount >= RETRY_MAX_COMM_BOARD_PMT_CH345)
    {
        //释放设备锁
        BoardPMT_CH345_ReleaseLock();
        //返回错误
        return errorSub;
    }
    //释放设备锁
    BoardPMT_CH345_ReleaseLock();
    return ERROR_SUB_OK; 
}

//测量模块停止
ERROR_SUB BoardPMT_CH345_Stop(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //发送停止采样
    uint8_t retryCount = 0;
    //首先获取设备锁
    BoardPMT_CH345_GetLock();
    //计数单元停止
    retryCount = 0;
    do
    {
        errorSub = BoardPMT_CH345_StopPhotonCountAndClear();
        if(errorSub != ERROR_SUB_OK)
        {
            //异常
            retryCount++;
            if(retryCount >= RETRY_MAX_COMM_BOARD_PMT_CH345)
            {
                break;
            }
            //继续下一次
            continue;
        }
        //正常完成
        break;
    }while(retryCount < RETRY_MAX_COMM_BOARD_PMT_CH345);
    if(retryCount >= RETRY_MAX_COMM_BOARD_PMT_CH345)
    {
        //释放设备锁
        BoardPMT_CH345_ReleaseLock();
        //返回错误
        return errorSub;
    }
    //释放设备锁
    BoardPMT_CH345_ReleaseLock();
    return ERROR_SUB_OK; 
}
