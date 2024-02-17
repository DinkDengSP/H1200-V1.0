/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-31 16:34:18
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-02-04 14:39:56
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#include "SrvImplStartHeart.h"
#include "SrvImplBaseStart.h"
#include "SrvTaskHeader.h"
#include "SrvTaskHeader.h"
#include "ModTaskHeader.h"
#include "AppTaskHeader.h"
#include "SystemParam.h"
#include "SystemState.h"
#include "IPC_AppHeader.h"

//导入LAN8700协议栈初始化
extern void SrvImplSocketNetLAN8700StackInit(void);
//导入W5500协议栈初始化
extern void SrvImplSocketNetW5500StackInit(void);

/*--------------------------系统启动初始化相关函数------------------------------*/
//系统设备初始化
void SrvImplStartHeartDeviceInit(void)
{
    //RTT初始化
    MCU_RTT_Init();
    //CRC初始化
    MCU_CRC_Init();
    //随机数初始化
    MCU_RandomInit();
    //内存管理初始化
    UserMemInit(MEM_AXI_SRAM);
    UserMemInit(MEM_D2_SRAM1);
    UserMemInit(MEM_D3_SRAM4);
    //初始化LED,全部点亮
    BoardLedInit(BOARD_LED_GREEN,PIN_STATE_VALID);
    BoardLedInit(BOARD_LED_YELLOW,PIN_STATE_VALID);
    BoardLedInit(BOARD_LED_RED,PIN_STATE_VALID);
    //蜂鸣器初始化
    BoardBeepInit(PIN_STATE_INVALID);
    //电源端口初始化,默认初始化电源全部关闭
    BoardPowerControlInit();
    //报警灯初始化
    BoardAlarmLampInit(PIN_STATE_INVALID);
    //拨码LED初始化
    BoardDipLedInitAll(PIN_STATE_INVALID);
    //拨码按键初始化
    BoardDipKeyInitAll();
    //板卡ID初始化
    BoardIdentityPortInit();
    //MCU ID显示
    SrvImplStartHeartShowChipInfo();
    //RTC初始化
    SrvImplStartHeartRTC_Init();
    //第三方组件初始化
    SrvImplStartHeartThirdComponentInit();
    //铁电存储器初始化
    SrvImplStartHeartMB85RS2MT_Init();
    //SDRAM初始化
    SrvImplStartHeartSDRAM_Init();
    //内存管理初始化
    UserMemInit(MEM_SDRAM1);
    UserMemInit(MEM_SDRAM2);
    //RS1日志端口初始化
    BoardRS1Uart6_Init(NULL,NULL,NULL);
    //AT24CXX初始化
    SrvImplStartAT24CXX_Init();
    //SD卡初始化
    SrvImplStartHeartSDCardInit();
    //写入Srv版本号
    SrvImplStartHeartWriteSrvVersion();
    //IAP初始化
    SrvImplStartHeartIapInit();
    //8700网卡初始化
    SrvImplStartHeartLAN8700_Init();
    //5500网卡初始化
    SrvImplStartHeartW5500_Init();
    //初始化完成之后,调用日志的初始化
    SrvImplStartHeartCreateSysLogTask();
    //主板高精度时间戳初始化
    BoardHpTimeStampInit();
    //系统参数初始化
    SystemParamInit();
    //系统状态初始化
    SystemStateInit();
    //网卡协议栈初始化,LAN8700
    SrvImplSocketNetLAN8700StackInit();
    //初始化网卡协议栈
    NetProtInit(NetProtGetSelfModuleNo(),NetProtGetSelfBoardID());
    //网卡协议栈初始化,W5500
    SrvImplSocketNetW5500StackInit();
    //初始化C21端口网口协议栈
    ProtocolC21NetSocketInit();
    //主板ID获取
    uint32_t canFilterID = 0;
    uint32_t canFilterMask = CAN_PROT_FILTER_MASK;
    //CAN1协议栈初始化
    Can1ProtInit(Can1ProtPortGetSelfModuleNo(),Can1ProtPortGetSelfBoardID());
    canFilterID = CAN_PROT_FILTER_ID(Can1ProtPortGetSelfModuleNo(),Can1ProtPortGetSelfBoardID());
    BoardCAN1_Init(canFilterID,canFilterMask,Can1RecvDataDispatchToSrvTask);
    //CAN2协议栈初始化
    Can2ProtInit(Can2ProtPortGetSelfModuleNo(),Can2ProtPortGetSelfBoardID());
    canFilterID = CAN_PROT_FILTER_ID(Can2ProtPortGetSelfModuleNo(),Can2ProtPortGetSelfBoardID());
    BoardCAN2_Init(canFilterID,canFilterMask,Can2RecvDataDispatchToSrvTask);
#if(PROT_UART_USER_LH_LOCAL == CONFIG_FUNC_ENABLE)
    //串口协议栈初始化
    UartProtInit(UartProtGetSelfModuleNo(),UartProtGetSelfBoardID());
#else
    ProtocolPEUartSocketInit();
#endif
    //RS2初始化
    BoardRS2Uart1_Init(UartRecvDataDispatchToSrvTask,NULL,NULL);
    //启动电源控制的外部设备
    SrvImplStartHeartPowerOn();
}

//公共服务层数据初始化
void SrvImplStartHeartCreateCommonSrvData(void)
{
    //SOCKET CAN1消息队列
    tx_queue_create(&queueSrvTaskSocketCan1,"queueSrvTaskSocketCan1",1,(void*)queueBufferSrvTaskSocketCan1,
                        sizeof(queueBufferSrvTaskSocketCan1)/sizeof(uint8_t));
    //SOCKET CAN1 ACK SEND消息队列
    tx_queue_create(&queueSocketCan1AckSend,"queueSocketCan1AckSend",1,(void*)queueBufferSocketCan1AckSend,
                        sizeof(queueBufferSocketCan1AckSend)/sizeof(uint8_t));
    //SOCKET CAN2消息队列
    tx_queue_create(&queueSrvTaskSocketCan2,"queueSrvTaskSocketCan2",1,(void*)queueBufferSrvTaskSocketCan2,
                        sizeof(queueBufferSrvTaskSocketCan2)/sizeof(uint8_t));
    //SOCKET CAN2 ACK SEND消息队列
    tx_queue_create(&queueSocketCan2AckSend,"queueSocketCan2AckSend",1,(void*)queueBufferSocketCan2AckSend,
                        sizeof(queueBufferSocketCan2AckSend)/sizeof(uint8_t));
    //SOCKET NET 8700消息队列
    tx_queue_create(&queueSrvTaskSocketNet8700,"queueSrvTaskSocketNet8700",1,(void*)queueBufferSrvTaskSocketNet8700,
                        sizeof(queueBufferSrvTaskSocketNet8700)/sizeof(uint8_t));
    //SOCKET NET 8700 ACK SEND消息队列
    tx_queue_create(&queueSocketNet8700AckSend,"queueSocketNet8700AckSend",1,(void*)queueBufferSocketNet8700AckSend,
                        sizeof(queueBufferSocketNet8700AckSend)/sizeof(uint8_t));
    //SOCKET NET 5500 消息队列
    tx_queue_create(&queueSrvTaskSocketNet5500,"queueSrvTaskSocketNet5500",1,(void*)queueBufferSrvTaskSocketNet5500,
                        sizeof(queueBufferSrvTaskSocketNet5500)/sizeof(uint8_t));
    //SOCKET NET 5500 ACK SEND消息队列
    tx_queue_create(&queueSocketNet5500AckSend,"queueSocketNet5500AckSend",1,(void*)queueBufferSocketNet5500AckSend,
                        sizeof(queueBufferSocketNet5500AckSend)/sizeof(uint8_t));
    //SOCKET UART消息队列
    tx_queue_create(&queueSrvTaskSocketUart,"queueSrvTaskSocketUart",1,(void*)queueBufferSrvTaskSocketUart,
                        sizeof(queueBufferSrvTaskSocketUart)/sizeof(uint8_t));
    //SOCKET NET ACK SEND消息队列
    tx_queue_create(&queueSocketUartAckSend,"queueSocketUartAckSend",1,(void*)queueBufferSocketUartAckSend,
                        sizeof(queueBufferSocketUartAckSend)/sizeof(uint8_t));
    //定时任务消息队列
    tx_queue_create(&queueSrvTaskTimer,"queueSrvTaskTimer",1,(void*)queueBufferSrvTaskTimer,
                       sizeof(queueBufferSrvTaskTimer)/sizeof(uint8_t));
    //辅助任务消息队列
    tx_queue_create(&queueSrvTaskUtil,"queueSrvTaskUtil",1,(void*)queueBufferSrvTaskUtil,
                       sizeof(queueBufferSrvTaskUtil)/sizeof(uint8_t));
    //输入读取任务消息队列
    tx_queue_create(&queueSrvTaskPortIn,"queueSrvTaskPortIn",1,(void*)queueBufferSrvTaskPortIn,
                       sizeof(queueBufferSrvTaskPortIn)/sizeof(uint8_t));
    //输入写入任务消息队列
    tx_queue_create(&queueSrvTaskPortOut,"queueSrvTaskPortOut",1,(void*)queueBufferSrvTaskPortOut,
                       sizeof(queueBufferSrvTaskPortOut)/sizeof(uint8_t));
    //参数读写任务消息队列
    tx_queue_create(&queueSrvTaskParam,"queueSrvTaskParam",1,(void*)queueBufferSrvTaskParam,
                       sizeof(queueBufferSrvTaskParam)/sizeof(uint8_t));
    //在线升级任务消息队列
    tx_queue_create(&queueSrvTaskFlashIap,"queueSrvTaskFlashIap",1,(void*)queueBufferSrvTaskFlashIap,
                        sizeof(queueBufferSrvTaskFlashIap)/sizeof(uint8_t));
    //FORWARD CAN消息队列
    tx_queue_create(&queueSrvTaskForwardSocketCan,"queueSrvTaskForwardSocketCan",1,(void*)queueBufferSrvTaskForwardSocketCan,
                        sizeof(queueBufferSrvTaskForwardSocketCan)/sizeof(uint8_t));
    //FORWARD NET消息队列
    tx_queue_create(&queueSrvTaskForwardSocketNet,"queueSrvTaskForwardSocketNet",1,(void*)queueBufferSrvTaskForwardSocketNet,
                        sizeof(queueBufferSrvTaskForwardSocketNet)/sizeof(uint8_t));
    //FORWARD UART消息队列
    tx_queue_create(&queueSrvTaskForwardSocketUart,"queueSrvTaskForwardSocketUart",1,(void*)queueBufferSrvTaskForwardSocketUart,
                        sizeof(queueBufferSrvTaskForwardSocketUart)/sizeof(uint8_t));
    //构建IPC环境
    IPC_SrvFlashIapInit();
    IPC_SrvParamInit();
    IPC_SrvPortInInit();
    IPC_SrvPortOutInit();
    IPC_SrvSysLogInit();
    IPC_SrvUtilInit();
}


//通用服务层任务配置数组
static const SYSTEM_TASK_CREATE_CONFIG arrayCommonSrvTaskConfig[] = {
    //CAN1口接收任务
    {&tcbSrvTaskSocketCan1,NAME_SRV_TASK_SOCKET_CAN1,SrvTaskSocketCan1FuncEntry,0,stackBufferSrvTaskSocketCan1,
        STK_SIZE_SRV_TASK_SOCKET_CAN1,PRIO_SRV_TASK_SOCKET_CAN1,PRIO_SRV_TASK_SOCKET_CAN1,
        TIME_SLICE_COMM_SRV_TASK,TX_AUTO_START},
    //CAN2口接收任务
    {&tcbSrvTaskSocketCan2,NAME_SRV_TASK_SOCKET_CAN2,SrvTaskSocketCan2FuncEntry,0,stackBufferSrvTaskSocketCan2,
        STK_SIZE_SRV_TASK_SOCKET_CAN2,PRIO_SRV_TASK_SOCKET_CAN2,PRIO_SRV_TASK_SOCKET_CAN2,
        TIME_SLICE_COMM_SRV_TASK,TX_AUTO_START},
    //网口8700接收任务
    {&tcbSrvTaskSocketNet8700,NAME_SRV_TASK_SOCKET_NET_8700,SrvTaskSocketNet8700FuncEntry,0,stackBufferSrvTaskSocketNet8700,
        STK_SIZE_SRV_TASK_SOCKET_NET_8700,PRIO_SRV_TASK_SOCKET_NET_8700,PRIO_SRV_TASK_SOCKET_NET_8700,
        TIME_SLICE_COMM_SRV_TASK,TX_AUTO_START},
    //网口5500接收任务
    {&tcbSrvTaskSocketNet5500,NAME_SRV_TASK_SOCKET_NET_5500,SrvTaskSocketNet5500FuncEntry,0,stackBufferSrvTaskSocketNet5500,
        STK_SIZE_SRV_TASK_SOCKET_NET_5500,PRIO_SRV_TASK_SOCKET_NET_5500,PRIO_SRV_TASK_SOCKET_NET_5500,
        TIME_SLICE_COMM_SRV_TASK,TX_AUTO_START},
    //串口接收任务
    {&tcbSrvTaskSocketUart,NAME_SRV_TASK_SOCKET_UART,SrvTaskSocketUartFuncEntry,0,stackBufferSrvTaskSocketUart,
        STK_SIZE_SRV_TASK_SOCKET_UART,PRIO_SRV_TASK_SOCKET_UART,PRIO_SRV_TASK_SOCKET_UART,
        TIME_SLICE_COMM_SRV_TASK,TX_AUTO_START},
    //定时查询任务
    {&tcbSrvTaskTimer,NAME_SRV_TASK_TIMER,SrvTaskTimerFuncEntry,0,stackBufferSrvTaskTimer,
        STK_SIZE_SRV_TASK_TIMER,PRIO_SRV_TASK_TIMER,PRIO_SRV_TASK_TIMER,
        TIME_SLICE_COMM_SRV_TASK,TX_AUTO_START},
    //系统辅助任务
    {&tcbSrvTaskUtil,NAME_SRV_TASK_UTIL,SrvTaskUtilFuncEntry,0,stackBufferSrvTaskUtil,
        STK_SIZE_SRV_TASK_UTIL,PRIO_SRV_TASK_UTIL,PRIO_SRV_TASK_UTIL,
        TIME_SLICE_COMM_SRV_TASK,TX_AUTO_START},
    //输入读取任务
    {&tcbSrvTaskPortIn,NAME_SRV_TASK_PORT_IN,SrvTaskPortInFuncEntry,0,stackBufferSrvTaskPortIn,
        STK_SIZE_SRV_TASK_PORT_IN,PRIO_SRV_TASK_PORT_IN,PRIO_SRV_TASK_PORT_IN,
        TIME_SLICE_COMM_SRV_TASK,TX_AUTO_START},
    //输出写入任务
    {&tcbSrvTaskPortOut,NAME_SRV_TASK_PORT_OUT,SrvTaskPortOutFuncEntry,0,stackBufferSrvTaskPortOut,
        STK_SIZE_SRV_TASK_PORT_OUT,PRIO_SRV_TASK_PORT_OUT,PRIO_SRV_TASK_PORT_OUT,
        TIME_SLICE_COMM_SRV_TASK,TX_AUTO_START},
    //参数读写任务
    {&tcbSrvTaskParam,NAME_SRV_TASK_PARAM,SrvTaskParamFuncEntry,0,stackBufferSrvTaskParam,
        STK_SIZE_SRV_TASK_PARAM,PRIO_SRV_TASK_PARAM,PRIO_SRV_TASK_PARAM,
        TIME_SLICE_COMM_SRV_TASK,TX_AUTO_START},
    //在线升级任务
    {&tcbSrvTaskFlashIap,NAME_SRV_TASK_FLASH_IAP,SrvTaskFlashIapFuncEntry,0,stackBufferSrvTaskFlashIap,
        STK_SIZE_SRV_TASK_FLASH_IAP,PRIO_SRV_TASK_FLASH_IAP,PRIO_SRV_TASK_FLASH_IAP,
        TIME_SLICE_COMM_SRV_TASK,TX_AUTO_START},
    //CAN口透传任务
    {&tcbSrvTaskForwardSocketCan,NAME_SRV_TASK_FORWARD_SOCKET_CAN,SrvTaskForwardSocketCanFuncEntry,0,stackBufferSrvTaskForwardSocketCan,
        STK_SIZE_SRV_TASK_FORWARD_SOCKET_CAN,PRIO_SRV_TASK_FORWARD_SOCKET_CAN,PRIO_SRV_TASK_FORWARD_SOCKET_CAN,
        TIME_SLICE_COMM_SRV_TASK,TX_AUTO_START},
    //网口透传任务
    {&tcbSrvTaskForwardSocketNet,NAME_SRV_TASK_FORWARD_SOCKET_NET,SrvTaskForwardSocketNetFuncEntry,0,stackBufferSrvTaskForwardSocketNet,
        STK_SIZE_SRV_TASK_FORWARD_SOCKET_NET,PRIO_SRV_TASK_FORWARD_SOCKET_NET,PRIO_SRV_TASK_FORWARD_SOCKET_NET,
        TIME_SLICE_COMM_SRV_TASK,TX_AUTO_START},
    //串口透传
    {&tcbSrvTaskForwardSocketUart,NAME_SRV_TASK_FORWARD_SOCKET_UART,SrvTaskForwardSocketUartFuncEntry,0,stackBufferSrvTaskForwardSocketUart,
        STK_SIZE_SRV_TASK_FORWARD_SOCKET_UART,PRIO_SRV_TASK_FORWARD_SOCKET_UART,PRIO_SRV_TASK_FORWARD_SOCKET_UART,
        TIME_SLICE_COMM_SRV_TASK,TX_AUTO_START},
};

//公共服务层任务初始化
void SrvImplStartHeartCreateCommonSrvTask(void)
{
    //通用服务层任务创建
    int indexUtil = 0;
    //配置服务初始化
    for(indexUtil = 0; indexUtil < DIM_ARRAY_ELEMENT_COUNT(arrayCommonSrvTaskConfig);indexUtil++)
    {
        tx_thread_create(arrayCommonSrvTaskConfig[indexUtil].tcbTaskPtr,        /* 任务控制块地址 */   
                         arrayCommonSrvTaskConfig[indexUtil].taskNamePtr,       /* 任务名 */
                         arrayCommonSrvTaskConfig[indexUtil].taskFuncEntryPtr,  /* 启动任务函数地址 */
                         arrayCommonSrvTaskConfig[indexUtil].threadInput,        /* 传递给任务的参数 */
                         arrayCommonSrvTaskConfig[indexUtil].taskStkBasePtr,    /* 堆栈基地址 */
                         arrayCommonSrvTaskConfig[indexUtil].taskStkSize,       /* 堆栈空间大小 */  
                         arrayCommonSrvTaskConfig[indexUtil].priority,          /* 任务优先级*/
                         arrayCommonSrvTaskConfig[indexUtil].preempt_threshold, /* 任务抢占阀值 */
                         arrayCommonSrvTaskConfig[indexUtil].time_slice,         /* 不开启时间片 */
                         arrayCommonSrvTaskConfig[indexUtil].auto_start);       /* 创建后是否立即启动 */
    }
}

//服务层数据初始化
void SrvImplStartHeartCreateSrvData(void)
{
    //Demo消息队列
    tx_queue_create(&queueSrvTaskDemo,"queueSrvTaskDemo",1,(void*)queueBufferSrvTaskDemo,
                        sizeof(queueBufferSrvTaskDemo)/sizeof(uint8_t));
    //构建IPC环境
    IPC_SrvDemoInit();
    IPC_SrvHeartInit();
    IPC_SrvTimerInit();
}

//服务层任务配置数组
static const SYSTEM_TASK_CREATE_CONFIG arraySrvTaskConfig[] = {
    //服务层DEMO任务
    {&tcbSrvTaskDemo,NAME_SRV_TASK_DEMO,SrvTaskDemoFuncEntry,0,stackBufferSrvTaskDemo,
        STK_SIZE_SRV_TASK_DEMO,PRIO_SRV_TASK_DEMO,PRIO_SRV_TASK_DEMO,
        TIME_SLICE_SRV_TASK,TX_AUTO_START},
};

//服务层任务初始化
void SrvImplStartHeartCreateSrvTask(void)
{
    //通用服务层任务创建
    int indexUtil = 0;
    //配置服务初始化
    for(indexUtil = 0; indexUtil < DIM_ARRAY_ELEMENT_COUNT(arraySrvTaskConfig);indexUtil++)
    {
        tx_thread_create(arraySrvTaskConfig[indexUtil].tcbTaskPtr,        /* 任务控制块地址 */   
                         arraySrvTaskConfig[indexUtil].taskNamePtr,       /* 任务名 */
                         arraySrvTaskConfig[indexUtil].taskFuncEntryPtr,  /* 启动任务函数地址 */
                         arraySrvTaskConfig[indexUtil].threadInput,        /* 传递给任务的参数 */
                         arraySrvTaskConfig[indexUtil].taskStkBasePtr,    /* 堆栈基地址 */
                         arraySrvTaskConfig[indexUtil].taskStkSize,       /* 堆栈空间大小 */  
                         arraySrvTaskConfig[indexUtil].priority,          /* 任务优先级*/
                         arraySrvTaskConfig[indexUtil].preempt_threshold, /* 任务抢占阀值 */
                         arraySrvTaskConfig[indexUtil].time_slice,         /* 不开启时间片 */
                         arraySrvTaskConfig[indexUtil].auto_start);       /* 创建后是否立即启动 */
    }
}

//模块层数据初始化
void SrvImplStartHeartCreateModuleData(void)
{
    //模块层消息队列初始化
    //创建 模块辅助任务 消息队列
    tx_queue_create(&queueModTaskDemo,"queueModTaskDemo",1,(void*)queueBufferModTaskDemo,
                       sizeof(queueBufferModTaskDemo)/sizeof(uint8_t));
    //创建 模块层样本架装载辅助查询 消息队列
    tx_queue_create(&queueModTaskRackStore,"queueModTaskRackStore",1,(void*)queueBufferModTaskRackStore,
                       sizeof(queueBufferModTaskRackStore)/sizeof(uint8_t));
    //创建 模块层样本架移动 消息队列
    tx_queue_create(&queueModTaskRackMove,"queueModTaskRackMove",1,(void*)queueBufferModTaskRackMove,
                       sizeof(queueBufferModTaskRackMove)/sizeof(uint8_t));
    //构建IPC环境
    IPC_ModDemoInit();
    IPC_ModRackStoreInit();
    IPC_ModRackMoveInit();
}

//模块层任务配置数组
static const SYSTEM_TASK_CREATE_CONFIG arrayModTaskConfig[] = {
    //系统辅助任务
    {&tcbModTaskDemo,NAME_MOD_TASK_DEMO,ModTaskDemoFuncEntry,0,stackBufferModTaskDemo,
        STK_SIZE_MOD_TASK_DEMO,PRIO_MOD_TASK_DEMO,PRIO_MOD_TASK_DEMO,
        TIME_SLICE_MOD_TASK,TX_AUTO_START},
    //模块层样本架装载辅助查询
    {&tcbModTaskRackStore,NAME_MOD_TASK_RACK_STORE,ModTaskRackStoreFuncEntry,0,stackBufferModTaskRackStore,
        STK_SIZE_MOD_TASK_RACK_STORE,PRIO_MOD_TASK_RACK_STORE,PRIO_MOD_TASK_RACK_STORE,
        TIME_SLICE_MOD_TASK,TX_AUTO_START},
    //模块层样本架移动
    {&tcbModTaskRackMove,NAME_MOD_TASK_RACK_MOVE,ModTaskRackMoveFuncEntry,0,stackBufferModTaskRackMove,
        STK_SIZE_MOD_TASK_RACK_MOVE,PRIO_MOD_TASK_RACK_MOVE,PRIO_MOD_TASK_RACK_MOVE,
        TIME_SLICE_MOD_TASK,TX_AUTO_START},
};

//模块层任务初始化
void SrvImplStartHeartCreateModuleTask(void)
{
    int indexUtil = 0;
    //配置模块任务初始化
    for(indexUtil = 0; indexUtil < DIM_ARRAY_ELEMENT_COUNT(arrayModTaskConfig);indexUtil++)
    {
        tx_thread_create(arrayModTaskConfig[indexUtil].tcbTaskPtr,        /* 任务控制块地址 */   
                         arrayModTaskConfig[indexUtil].taskNamePtr,       /* 任务名 */
                         arrayModTaskConfig[indexUtil].taskFuncEntryPtr,  /* 启动任务函数地址 */
                         arrayModTaskConfig[indexUtil].threadInput,        /* 传递给任务的参数 */
                         arrayModTaskConfig[indexUtil].taskStkBasePtr,    /* 堆栈基地址 */
                         arrayModTaskConfig[indexUtil].taskStkSize,       /* 堆栈空间大小 */  
                         arrayModTaskConfig[indexUtil].priority,          /* 任务优先级*/
                         arrayModTaskConfig[indexUtil].preempt_threshold, /* 任务抢占阀值 */
                         arrayModTaskConfig[indexUtil].time_slice,         /* 不开启时间片 */
                         arrayModTaskConfig[indexUtil].auto_start);       /* 创建后是否立即启动 */
    }
}

//应用层数据初始化
void SrvImplStartHeartCreateAppData(void)
{
    //应用层消息队列初始化
    //创建 应用辅助任务 消息队列
    tx_queue_create(&queueAppTaskDemo,"queueAppTaskDemo",1,(void*)queueBufferAppTaskDemo,
                       sizeof(queueBufferAppTaskDemo)/sizeof(uint8_t));
    //创建 应用层消息处理任务 消息队列
    tx_queue_create(&queueAppTaskMsgProc,"queueAppTaskMsgProc",1,(void*)queueBufferAppTaskMsgProc,
                       sizeof(queueBufferAppTaskMsgProc)/sizeof(uint8_t));
    //创建 应用层流程处理任务 消息队列
    tx_queue_create(&queueAppTaskTestFlow,"queueAppTaskTestFlow",1,(void*)queueBufferAppTaskTestFlow,
                       sizeof(queueBufferAppTaskTestFlow)/sizeof(uint8_t));
    //构建IPC环境
    IPC_AppDemoInit();
}

//应用层任务配置数组
static const SYSTEM_TASK_CREATE_CONFIG arrayAppTaskConfig[] = {
    //系统辅助任务
    {&tcbAppTaskDemo,NAME_APP_TASK_DEMO,AppTaskDemoFuncEntry,0,stackBufferAppTaskDemo,
        STK_SIZE_APP_TASK_DEMO,PRIO_APP_TASK_DEMO,PRIO_APP_TASK_DEMO,
        TIME_SLICE_APP_TASK,TX_AUTO_START},
    //应用层消息处理任务
    {&tcbAppTaskMsgProc,NAME_APP_TASK_MSG_PROC,AppTaskMsgProcFuncEntry,0,stackBufferAppTaskMsgProc,
        STK_SIZE_APP_TASK_MSG_PROC,PRIO_APP_TASK_MSG_PROC,PRIO_APP_TASK_MSG_PROC,
        TIME_SLICE_APP_TASK,TX_AUTO_START},
    //应用层流程处理任务
    {&tcbAppTaskTestFlow,NAME_APP_TASK_TEST_FLOW,AppTaskTestFlowFuncEntry,0,stackBufferAppTaskTestFlow,
        STK_SIZE_APP_TASK_TEST_FLOW,PRIO_APP_TASK_TEST_FLOW,PRIO_APP_TASK_TEST_FLOW,
        TIME_SLICE_APP_TASK,TX_AUTO_START},
};

//应用层任务初始化
void SrvImplStartHeartCreateAppTask(void)
{
    int indexUtil = 0;
    //配置应用任务初始化
    for(indexUtil = 0; indexUtil < DIM_ARRAY_ELEMENT_COUNT(arrayAppTaskConfig);indexUtil++)
    {
        tx_thread_create(arrayAppTaskConfig[indexUtil].tcbTaskPtr,        /* 任务控制块地址 */   
                         arrayAppTaskConfig[indexUtil].taskNamePtr,       /* 任务名 */
                         arrayAppTaskConfig[indexUtil].taskFuncEntryPtr,  /* 启动任务函数地址 */
                         arrayAppTaskConfig[indexUtil].threadInput,        /* 传递给任务的参数 */
                         arrayAppTaskConfig[indexUtil].taskStkBasePtr,    /* 堆栈基地址 */
                         arrayAppTaskConfig[indexUtil].taskStkSize,       /* 堆栈空间大小 */  
                         arrayAppTaskConfig[indexUtil].priority,          /* 任务优先级*/
                         arrayAppTaskConfig[indexUtil].preempt_threshold, /* 任务抢占阀值 */
                         arrayAppTaskConfig[indexUtil].time_slice,         /* 不开启时间片 */
                         arrayAppTaskConfig[indexUtil].auto_start);       /* 创建后是否立即启动 */
    }
}






