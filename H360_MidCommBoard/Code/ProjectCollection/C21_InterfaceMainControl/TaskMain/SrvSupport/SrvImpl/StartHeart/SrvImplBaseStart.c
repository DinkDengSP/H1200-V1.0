/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-31 16:34:18
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-02-04 09:46:21
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#include "SrvImplBaseStart.h"
#include "SrvTaskHeader.h"
#include "SrvTaskHeader.h"
#include "SrvTaskHeader.h"
#include "ModTaskHeader.h"
#include "AppTaskHeader.h"
#include "SystemParam.h"
#include "SystemState.h"

//MCU ID显示
void SrvImplStartHeartShowChipInfo(void)
{
    MCU_CHIP_ID info;
    MCU_ChipID_Read(&info);
    MCU_LogSendString("CPU: STM32H743IIT6\r\n");
    MCU_LogPrintf("\tID_SN0: 0X%08X\r\n",info.idSn0);
    MCU_LogPrintf("\tID_SN1: 0X%08X\r\n",info.idSn1);
    MCU_LogPrintf("\tID_SN2: 0X%08X\r\n",info.idSn2);
    MCU_LogPrintf("\tID_FlashSize: %d KB\r\n",info.flashSizeID);
}


//RTC初始化
void SrvImplStartHeartRTC_Init(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    do
    {
        //RTC初始化
        errorSub = MCU_RTC_Init();
        if(errorSub != ERROR_SUB_OK)
        {
            MCU_LogPrintf("MCU_RTC_Init Failed,Code: 0X%08X\r\n",errorSub);
            CoreDelayMs(500);
        }
    }while(errorSub != ERROR_SUB_OK);
    RTC_TIMER* rtcTimerPtr = NULL;
    errorSub = MCU_RTC_GetTimeRecordSync(&rtcTimerPtr);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogPrintf("MCU_RTC_GetTimeRecordSync Failed,Code: 0X%08X\r\n",errorSub);
        //卡住
        while(1);
    }
    //当前时间打印
    MCU_LogPrintf("CurrentWeek: %d,CurrentTime %04d-%02d-%02d,%02d:%02d:%02d:%03d\r\n",rtcTimerPtr->weekDay,
                        rtcTimerPtr->year,rtcTimerPtr->month,rtcTimerPtr->day,rtcTimerPtr->hour,rtcTimerPtr->minute,
                        rtcTimerPtr->second,rtcTimerPtr->subSecond);
}


//第三方组件初始化
void SrvImplStartHeartThirdComponentInit(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK; 
    //FileX初始化
    fx_system_initialize();
    //获取系统时间
    RTC_TIMER* rtcTimerPtr;
    errorSub = MCU_RTC_GetTimeRecordSync(&rtcTimerPtr);
    if(errorSub == ERROR_SUB_OK)
    {
        //更新文件系统时间
        fx_system_date_set(rtcTimerPtr->year,rtcTimerPtr->month,rtcTimerPtr->day);
        fx_system_time_set(rtcTimerPtr->hour,rtcTimerPtr->minute,rtcTimerPtr->second);
    }
    else
    {
        MCU_LogPrintf("MCU_RTC_GetTimeRecordSync Failed,Code: 0X%08X\r\n",errorSub);
    }
    //NetX网络系统初始化
    nx_system_initialize();
    //网络加密相关程序初始化
    nx_crypto_initialize();
    //网络TLS数据传输初始化
    nx_secure_tls_initialize();
    //网络DTLS数据传输初始化
    nx_secure_dtls_initialize();
}


//MB85RS2MT初始化
void SrvImplStartHeartMB85RS2MT_Init(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //端口初始化
    BoardMB85RS2MT_PortInit();
    //配置默认状态
    do
    {
        errorSub = BoardMB85RS2MT_ConfigDefault();
        if(errorSub != ERROR_SUB_OK)
        {
            MCU_LogPrintf("BoardMB85RS2MT_ConfigDefault Failed,Code: 0X%08X\r\n",errorSub);
            CoreDelayMs(500);
            BoardLedToogle(BOARD_LED_RED);
        }
    }while(errorSub != ERROR_SUB_OK);
    //检查自检标记决定是否需要自检
    if(ERROR == BoardParamCheckFlagFRAM())
    {
        MCU_LogPrintf("%s,BoardMB85RS2MT_SelfCheck Start\r\n",__func__);
        do
        {
            errorSub = BoardMB85RS2MT_SelfCheck();
            if(errorSub != ERROR_SUB_OK)
            {
                MCU_LogPrintf("BoardMB85RS2MT_SelfCheck Failed,ErrorCode: 0X%08X\r\n",errorSub);
                CoreDelayMs(500);
                BoardLedToogle(BOARD_LED_RED);
            }
        }while(errorSub != ERROR_SUB_OK);
        //写入标记
        errorSub = BoardParamSetCheckFlagFRAM(SUCCESS);
        if(errorSub != ERROR_SUB_OK)
        {
            MCU_LogPrintf("BoardParamSetCheckFlagFRAM Failed,Code: 0X%08X\r\n",errorSub);
            while(1);
        }
    }
    //检查低格标记决定是否需要格式化
    if(ERROR == BoardParamCheckFormatFlagFRAM())
    {
        MCU_LogPrintf("%s,BoardMB85RS2MT_SelfCheck Start\r\n",__func__);
        do
        {
            errorSub = BoardMB85RS2MT_SelfCheck();
            if(errorSub != ERROR_SUB_OK)
            {
                MCU_LogPrintf("BoardMB85RS2MT_SelfCheck Failed,ErrorCode: 0X%08X\r\n",errorSub);
                CoreDelayMs(500);
                BoardLedToogle(BOARD_LED_RED);
            }
        }while(errorSub != ERROR_SUB_OK);
        MCU_LogPrintf("%s,BoardMB85RS2MT_EraseChip Start\r\n",__func__);
        //芯片擦除
        errorSub = BoardMB85RS2MT_EraseChip();
        if(errorSub != ERROR_SUB_OK)
        {
            MCU_LogPrintf("%s,BoardMB85RS2MT_EraseChip Failed : 0X%08X\r\n",__func__,errorSub);
            while(1);
        }
        //擦除完成
        MCU_LogPrintf("%s,BoardMB85RS2MT_EraseChip End\r\n",__func__);
        //写入标记
        errorSub = BoardParamSetCheckFormatFlagFRAM(SUCCESS);
        if(errorSub != ERROR_SUB_OK)
        {
            MCU_LogPrintf("BoardParamSetCheckFormatFlagFRAM Failed,Code: 0X%08X\r\n",errorSub);
            while(1);
        }
    }
    //关闭报警灯
    BoardLedWrite(BOARD_LED_RED,PIN_STATE_INVALID);
    MCU_LogPrintf("%s, Success Over\r\n",__func__);
}


//SDRAM初始化
void SrvImplStartHeartSDRAM_Init(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //端口初始化
    BoardSDRAM_Init();
    //自检
    //BoardSDRAM_Check();
    //输出初始化信息
    MCU_LogSendString("SrvImplStartSDRAM_Init Start\r\n");
    //自检标记
    if(ERROR == BoardParamCheckExRamFlag())
    {
        do
        {
            MCU_LogSendString("BoardSDRAM_Check Start\r\n");
            //SRAM自检
            errorSub = BoardSDRAM_Check();
            if(errorSub != ERROR_SUB_OK)
            {
                MCU_LogPrintf("BoardSDRAM_Check Failed,Code: 0X%08X\r\n",errorSub);
                CoreDelayMs(500);
                BoardLedToogle(BOARD_LED_RED);
            }
        }while(errorSub != ERROR_SUB_OK);
        //写入标记
        errorSub = BoardParamSetCheckExRamFlag(SUCCESS);
        if(errorSub != ERROR_SUB_OK)
        {
            MCU_LogPrintf("BoardParamSetCheckExRamFlag Failed,Code: 0X%08X\r\n",errorSub);
            while(1);
        }
    }
    //关闭报警灯
    BoardLedWrite(BOARD_LED_RED,PIN_STATE_INVALID);
    //输出完成讯息
    MCU_LogPrintf("%s, Success Over\r\n",__func__);
}


//日志任务配置
static const SYSTEM_TASK_CREATE_CONFIG SrvTaskSysLogConfig = {
    &tcbSrvTaskSysLog,NAME_SRV_TASK_SYS_LOG,SrvTaskSysLogFuncEntry,0,stackBufferSrvTaskSysLog,
        STK_SIZE_SRV_TASK_SYS_LOG,PRIO_SRV_TASK_SYS_LOG,PRIO_SRV_TASK_SYS_LOG,
        TIME_SLICE_SRV_TASK,TX_AUTO_START
};
//创建日志任务
void SrvImplStartHeartCreateSysLogTask(void)
{
    //创建 日志任务 消息队列
    tx_queue_create(&queueSrvTaskSysLog,"queueSrvTaskSysLog",1,(void*)queueBufferSrvTaskSysLog,
                        sizeof(queueBufferSrvTaskSysLog)/sizeof(uint8_t));
    //创建日志任务
    tx_thread_create(SrvTaskSysLogConfig.tcbTaskPtr,        /* 任务控制块地址 */   
                     SrvTaskSysLogConfig.taskNamePtr,       /* 任务名 */
                     SrvTaskSysLogConfig.taskFuncEntryPtr,  /* 启动任务函数地址 */
                     SrvTaskSysLogConfig.threadInput,        /* 传递给任务的参数 */
                     SrvTaskSysLogConfig.taskStkBasePtr,    /* 堆栈基地址 */
                     SrvTaskSysLogConfig.taskStkSize,       /* 堆栈空间大小 */  
                     SrvTaskSysLogConfig.priority,          /* 任务优先级*/
                     SrvTaskSysLogConfig.preempt_threshold, /* 任务抢占阀值 */
                     SrvTaskSysLogConfig.time_slice,         /* 不开启时间片 */
                     SrvTaskSysLogConfig.auto_start);       /* 创建后是否立即启动 */
    //延时等待任务创建
    CoreDelayMs(5);
}


//AT24CXX初始化
void SrvImplStartAT24CXX_Init(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //初始化
    BoardAT24CXX_Init();
    //检查AT24CXX自检标记
    if(ERROR == BoardParamCheckFlagAT24CXX())
    {
        //需要自检
        do
        {
            errorSub = BoardAT24CXX_Check();
            if(errorSub != ERROR_SUB_OK)
            {
                MCU_LogPrintf("BoardAT24CXX_Check Failed,Code: 0X%08X\r\n",errorSub);
                CoreDelayMs(1000);
                BoardLedToogle(BOARD_LED_RED);
            }
        }while(errorSub != ERROR_SUB_OK);
        //是否执行全片擦除
#if(BOARD_AT24CXX_ERASE_CHIP_ENABLE == CONFIG_FUNC_ENABLE)
        //全片擦除,可选
        errorSub = BoardAT24CXX_ChipErase();
        if(errorSub != ERROR_SUB_OK)
        {
            MCU_LogPrintf("BoardAT24CXX_ChipErase Failed,ErrorCode: 0X%08X\r\n",errorSub);
            BoardLedWrite(BOARD_LED_RED,PIN_STATE_VALID);
            while(1);
        }
#endif
        //自检完成,写入自检标记
        errorSub = BoardParamSetCheckFlagAT24CXX(SUCCESS);
        if(errorSub != ERROR_SUB_OK)
        {
            MCU_LogPrintf("BoardParamSetCheckFlagAT24CXX Failed,Code: 0X%08X\r\n",errorSub);
            BoardLedWrite(BOARD_LED_RED,PIN_STATE_VALID);
            while(1);
        }
    }
    BoardLedWrite(BOARD_LED_RED,PIN_STATE_INVALID);
    //执行完成日志打印
    MCU_LogPrintf("%s, Success Over\r\n",__func__);
}

//写入Srv版本号初始化
void SrvImplStartHeartWriteSrvVersion(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    do
    {
        //写入Srv版本号
        errorSub = BoardParamWriteServiceVersion();
        if(errorSub != ERROR_SUB_OK)
        {
            MCU_LogPrintf("BoardParamWriteServiceVersion Failed,Code: 0X%08X\r\n",errorSub);
            //灯光闪烁
            BoardLedToogle(BOARD_LED_RED);
            CoreDelayMs(100);
        }
    }while(errorSub != ERROR_SUB_OK);
}


//IAP管理
void SrvImplStartHeartIapInit(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //IAP管理初始化,防止异常数据
    errorSub = BoardIapInit();
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogPrintf("BoardIapInit Failed,Code: 0X%08X\r\n",errorSub);
        do
        {
            BoardLedToogle(BOARD_LED_RED);
            CoreDelayMs(500);
        }while(1);
    }
    //检查板卡ID,如果是0,那么回到BOOT里面去
    if(0X00 == BoardIdentityPortGet())
    {
        MCU_LogPrintf("Board Identity Module 0,JumpToBoot For Upgrated\r\n");
        //跳转到BOOT
        BoardIapJumpBoot();
        while(1);
    }
    //S5拨码有效,强制停留在BOOT中
    if(PIN_STATE_VALID == BoardDipKeyReadSingle(BOARD_DIP_KEY_S5))
    {
        SystemPrintf("DIP Key S5 Select,,BootStart For Upgrated\r\n");
        //跳转到BOOT
        BoardIapJumpBoot();
        while(1);
    }
    //获取IAP状态
    BOARD_RUN_MODE iapRunMode;
    errorSub = BoardIapReadRunMode(&iapRunMode);
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogPrintf("BoardIapReadRunMode Failed,Code: 0X%08X\r\n",errorSub);
        do
        {
            BoardLedToogle(BOARD_LED_RED);
            CoreDelayMs(500);
        }while(1);
    }
    //如果是APP,直接返回
    if(iapRunMode == BOARD_RUN_MODE_APP)
    {
        return;
    }
    //写入APP模式
    errorSub = BoardIapSetRunningModeApp();
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogPrintf("BoardIapSetRunningModeApp Failed,Code: 0X%08X\r\n",errorSub);
        do
        {
            BoardLedToogle(BOARD_LED_RED);
            CoreDelayMs(500);
        }while(1);
    }
    return;
}


//LAN8700网卡初始化
void SrvImplStartHeartLAN8700_Init(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    do
    {
        //LAN8700初始化
        errorSub = BoardLAN8700_Init();
        if(errorSub != ERROR_SUB_OK)
        {
            MCU_LogPrintf("BoardLAN8700_Init Failed,Code: 0X%08X\r\n",errorSub);
            CoreDelayMs(500);
        }
    }while(errorSub != ERROR_SUB_OK);
    uint32_t phyID = 0X00000000;
    do
    {
        //LAN8700读取网卡ID
        errorSub = BoardLAN8700_ReadID(&phyID);
        if(errorSub != ERROR_SUB_OK)
        {
            MCU_LogPrintf("BoardLAN8700_ReadID Failed,Code: 0X%08X\r\n",errorSub);
            CoreDelayMs(500);
        }
        else
        {
            MCU_LogPrintf("BoardLAN8700_ReadID PHY ID: 0X%08X\r\n",phyID);
        }
    }while(errorSub != ERROR_SUB_OK);
    //检查IP标记
    if(ERROR == BoardParamCheckNetPhy1IpFlag())
    {
        //没有标记,写入默认地址
        errorSub = BoardParamSetNetPhy1DefaultIp();
        if(errorSub != ERROR_SUB_OK)
        {
            MCU_LogPrintf("BoardParamSetNetPhy1DefaultIp Failed,Code: 0X%08X\r\n",errorSub);
            while(1);
        }
        //写入标记
        errorSub = BoardParamSetCheckNetPhy1IpFlag(SUCCESS);
        if(errorSub != ERROR_SUB_OK)
        {
            MCU_LogPrintf("BoardParamSetCheckNetPhy1IpFlag Failed,Code: 0X%08X\r\n",errorSub);
            while(1);
        }
    }
    MCU_LogPrintf("%s, Success Over\r\n",__func__);
}


//W5500初始化
void SrvImplStartHeartW5500_Init(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    BoardW5500_PortInit();
    do
    {
        //W5500自检
        errorSub = BoardW5500_CheckVersion();
        if(errorSub != ERROR_SUB_OK)
        {
            MCU_LogPrintf("BoardW5500_CheckVersion Failed,Code: 0X%08X\r\n",errorSub);
            CoreDelayMs(500);
        }

    }while(errorSub != ERROR_SUB_OK);
    //IP检查
    if(ERROR == BoardParamCheckNetPhy2IpFlag())
    {
        //写入默认IP
        errorSub = BoardParamSetNetPhy2DefaultIp();
        if(errorSub != ERROR_SUB_OK)
        {
            MCU_LogPrintf("BoardParamSetNetPhy2DefaultIp Failed,Code: 0X%08X\r\n",errorSub);
            while(1);
        }
        //写入设置标记
        errorSub = BoardParamSetCheckNetPhy2IpFlag(SUCCESS);
        if(errorSub != ERROR_SUB_OK)
        {
            MCU_LogPrintf("BoardParamSetCheckNetPhy2IpFlag Failed,Code: 0X%08X\r\n",errorSub);
            while(1);
        }
    }
}


//初始化SDCARD
void SrvImplStartHeartSDCardInit(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    HAL_SD_CardInfoTypeDef cardInfo;
    //初始化
    do
    {
        //初始化SDCARD
        errorSub = BoardSD_Init();
        if(errorSub == ERROR_SUB_OK)
        {
            errorSub = BoardSD_GetCardInfo(&cardInfo);
            if(errorSub == ERROR_SUB_OK)
            {
                break;
            }
        }
        //自检失败,延时闪灯并再次自检
        CoreDelayMs(100);
        BoardLedToogle(BOARD_LED_RED);
        //复位SDCARD
        BoardSD_DeInit();
    }while(1);
    //显示SDCARD信息
    BoardSD_ShowCardMsg(&cardInfo);
    //SD卡文件系统挂载
    errorSub = FileSystemSDCardMount();
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogPrintf("FileSystemSDCardMount Failed,Code: 0X%08X\r\n",errorSub);
        while(1);
    }
    //挂载成功,打印文件系统根目录讯息
    FileSystemViewRootDir(NAME_FILEX_SDCARD,&fxSDCardDiskVar);
}

//启动外部电源
void SrvImplStartHeartPowerOn(void)
{
    uint8_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < 5; indexUtil++)
    {
        BoardPowerControlWriteSingle((BOARD_POWER_NO)indexUtil,PIN_STATE_VALID);
        //延迟放电电源浪涌
        CoreDelayMs(100);
    }
}











