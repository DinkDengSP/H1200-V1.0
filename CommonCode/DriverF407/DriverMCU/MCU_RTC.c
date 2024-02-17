/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-23 09:44:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-23 13:49:47 +0800
************************************************************************************************/ 
#include "MCU_RTC.h"
#include "MCU_LogShow.h"

//RTC句柄
static RTC_HandleTypeDef handleRTC;
//驱动内部保存的RTC时间
static RTC_TIMER rtcTimerCurrent;


//设置时间
static ERROR_SUB MCU_RTC_SetTime(uint8_t hour, uint8_t min, uint8_t second)
{
    HAL_StatusTypeDef halState = HAL_OK;
    //时间结构体
    RTC_TimeTypeDef RTC_TimeStructure;
	//设置时间
	RTC_TimeStructure.Hours = hour;
	RTC_TimeStructure.Minutes = min;
	RTC_TimeStructure.Seconds = second;
    //24小时制
	RTC_TimeStructure.TimeFormat = RTC_HOURFORMAT12_AM;
	RTC_TimeStructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    RTC_TimeStructure.StoreOperation = RTC_STOREOPERATION_RESET;
    //写入芯片
    halState = HAL_RTC_SetTime(&handleRTC, &RTC_TimeStructure, RTC_FORMAT_BIN);
    if(halState != HAL_OK)
	{
        MCU_LogShowHalState(halState,handleRTC.State);
        return ERROR_SUB_MCU_RTC_SET_TIME;
    }
    return ERROR_SUB_OK;
}

//设置日期
static ERROR_SUB MCU_RTC_SetData(uint8_t year, uint8_t month, uint8_t data, uint8_t week)
{
    HAL_StatusTypeDef halState = HAL_OK;
    //日期结构体
    RTC_DateTypeDef RTC_DateStructure;
    //设置日期
	RTC_DateStructure.Date = data;
	RTC_DateStructure.Month = month;
	RTC_DateStructure.WeekDay = week;
	RTC_DateStructure.Year = year;
    //写入芯片
	halState = HAL_RTC_SetDate(&handleRTC, &RTC_DateStructure, RTC_FORMAT_BIN);
    if(halState != HAL_OK)
	{
        MCU_LogShowHalState(halState,handleRTC.State);
        return ERROR_SUB_MCU_RTC_SET_DATA;
    }
    return ERROR_SUB_OK;
}

//芯片RTC初始化
ERROR_SUB MCU_RTC_Init(void)
{
    HAL_StatusTypeDef halState = HAL_OK;
    ERROR_SUB errorCodeSub = ERROR_SUB_OK;
	//时钟初始化变量
	RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct = {0};
    //使能电源时钟CLK
    __HAL_RCC_PWR_CLK_ENABLE();
    //rtc选择时钟
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    #if(CORE_RTC_USE_CLK == CONFIG_FUNC_DISABLE)
	PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
    #else
    PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
    #endif
    //时钟选择配置，取消备份区域写保护
    halState = HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
    if(halState != HAL_OK)
	{
        MCU_LogShowHalState(halState,0);
        #if(CORE_RTC_USE_LSE == CONFIG_FUNC_DISABLE)
        return ERROR_SUB_MCU_RTC_CLOCK_CONFIG_LSI;
        #else
        return ERROR_SUB_MCU_RTC_CLOCK_CONFIG_LSE;
        #endif
    }
	//RTC时钟使能
	__HAL_RCC_RTC_ENABLE(); 
    //取消备份区域写保护
	HAL_PWR_EnableBkUpAccess();
    /* 等待 RTC APB 寄存器同步 */
    HAL_RTC_WaitForSynchro(&handleRTC);
    //初始化RTC时钟 时钟1HZ = 32768/(127+1)/(255+1)
    handleRTC.Instance = RTC;
    //24小时格式
    handleRTC.Init.HourFormat     = RTC_HOURFORMAT_24; 
    //异步预分频系数 7位
    handleRTC.Init.AsynchPrediv   = 127;    
    //同步预分频系数 15位
    handleRTC.Init.SynchPrediv    = 255;    
    handleRTC.Init.OutPut         = RTC_OUTPUT_DISABLE; 
    handleRTC.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    handleRTC.Init.OutPutType     = RTC_OUTPUT_TYPE_OPENDRAIN;
    //首先复位RTC,看数据标记决定是否复位
    if(HAL_RTCEx_BKUPRead(&handleRTC,RTC_BKP_DR0) != MCU_RTC_INIT_FLAG_BKP)
    {
        halState = HAL_RTC_DeInit(&handleRTC);
        if(halState != HAL_OK)
        {
            MCU_LogShowHalState(halState,handleRTC.State);
            return ERROR_SUB_MCU_RTC_DEINIT;
        }
    }
    //然后初始化RTC
    halState = HAL_RTC_Init(&handleRTC);
    if(halState != HAL_OK)
	{
        MCU_LogShowHalState(halState,handleRTC.State);
        return ERROR_SUB_MCU_RTC_INIT;
    }
    //是否第一次配置
    if(HAL_RTCEx_BKUPRead(&handleRTC,RTC_BKP_DR0) != MCU_RTC_INIT_FLAG_BKP)
    {
        //写入RTC时间日期,先写入时间
        errorCodeSub = MCU_RTC_SetTime(MCU_RTC_INIT_DEFAULT_HOUR,MCU_RTC_INIT_DEFAULT_MINUTE,
                                        MCU_RTC_INIT_DEFAULT_SECOND);
        if(errorCodeSub != ERROR_SUB_OK)
        {
            MCU_LogShowErrorSubCode(errorCodeSub);
            return errorCodeSub;
        }
        //写入日期
        errorCodeSub = MCU_RTC_SetData(MCU_RTC_INIT_DEFAULT_YEAR,MCU_RTC_INIT_DEFAULT_MOUTH,
                                        MCU_RTC_INIT_DEFAULT_DAY,MCU_RTC_INIT_DEFAULT_WEEKDAY);
        if(errorCodeSub != ERROR_SUB_OK)
        {
            MCU_LogShowErrorSubCode(errorCodeSub);
            return errorCodeSub;
        }
        //设置备份域标记
        HAL_RTCEx_BKUPWrite(&handleRTC,RTC_BKP_DR0,MCU_RTC_INIT_FLAG_BKP);
    }
    return ERROR_SUB_OK;
}

//RTC同步UTC标准时间
ERROR_SUB MCU_RTC_SyncTimeByUTC(uint32_t timeStampUTC)
{
    struct tm *timeNowPtr;
    time_t time = timeStampUTC;	
    ERROR_SUB errorCodeSub = ERROR_SUB_OK;
    //锁定
    uint32_t lastIntState = CoreLockSchedule();
    //计算当前时间
    timeNowPtr = localtime(&time); 
    //将时间写入RTC
    errorCodeSub = MCU_RTC_SetTime(timeNowPtr->tm_hour, timeNowPtr->tm_min, timeNowPtr->tm_sec);
    if(errorCodeSub != ERROR_SUB_OK)
    {
        //解锁
        CoreUnlockSchedule(lastIntState);
        MCU_LogShowErrorSubCode(errorCodeSub);
        return errorCodeSub;
    }
    //将日期写入,UTC时间是对1900年的时间,我们用2000年的时间,tm结构体月份0-11
    errorCodeSub = MCU_RTC_SetData((uint8_t)(timeNowPtr->tm_year+1900-2000), (uint8_t)(timeNowPtr->tm_mon+1), (uint8_t)timeNowPtr->tm_mday, 
                                    (uint8_t)(timeNowPtr->tm_wday));
    //解锁
    CoreUnlockSchedule(lastIntState);
    if(errorCodeSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorCodeSub);
    }
    return errorCodeSub;
}

//RTC同步RTC时间
ERROR_SUB MCU_RTC_SyncTimeByRTC(RTC_TIMER* rtcTimerPtr)
{
    ERROR_SUB errorCodeSub = ERROR_SUB_OK;
    //锁定
    uint32_t lastIntState = CoreLockSchedule();
    //将时间写入RTC
    errorCodeSub = MCU_RTC_SetTime(rtcTimerPtr->hour, rtcTimerPtr->minute, rtcTimerPtr->second);
    if(errorCodeSub != ERROR_SUB_OK)
    {
        //解锁
        CoreUnlockSchedule(lastIntState);
        MCU_LogShowErrorSubCode(errorCodeSub);
        return errorCodeSub;
    }
    //将日期写入
    errorCodeSub = MCU_RTC_SetData((uint8_t)(rtcTimerPtr->year-2000), (uint8_t)(rtcTimerPtr->month), 
                                    (uint8_t)rtcTimerPtr->day, (uint8_t)(rtcTimerPtr->weekDay));
    //解锁
    CoreUnlockSchedule(lastIntState);
    if(errorCodeSub != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorCodeSub);
    }
    return errorCodeSub;
}

//获取RTC内部时间
ERROR_SUB MCU_RTC_GetTimeCurrent(RTC_TIMER* rtcTimerPtr)
{
    RTC_DateTypeDef  date;
	RTC_TimeTypeDef  time;
    ERROR_SUB errorCodeSub = ERROR_SUB_OK;
    //状态标识
    HAL_StatusTypeDef halState = HAL_OK;
    //首先设置数据空
    UserMemClear((void*)rtcTimerPtr,LENGTH_RTC_TIMER);
    //句柄
	handleRTC.Instance = RTC;
    //锁定
    uint32_t lastIntState = CoreLockSchedule();
	/* CPU BUG: 必须先读取时间，再读取日期 */
    halState = HAL_RTC_GetTime(&handleRTC, &time, FORMAT_BIN);
    if(halState != HAL_OK)
	{
        MCU_LogShowHalState(halState,handleRTC.State);
        //解锁
        CoreUnlockSchedule(lastIntState);
        return errorCodeSub;
    }
    halState = HAL_RTC_GetDate(&handleRTC, &date, FORMAT_BIN);
    if(halState != HAL_OK)
	{
        MCU_LogShowHalState(halState,handleRTC.State);
        //解锁
        CoreUnlockSchedule(lastIntState);
        return errorCodeSub;
    }
    //解锁
    CoreUnlockSchedule(lastIntState);
    //时间转换
	rtcTimerPtr->year = date.Year + 2000;
	rtcTimerPtr->month = date.Month;
	rtcTimerPtr->day = date.Date;
    rtcTimerPtr->weekDay = date.WeekDay;
	rtcTimerPtr->hour = time.Hours;	/* 小时 */
	rtcTimerPtr->minute = time.Minutes; 	/* 分钟 */
	rtcTimerPtr->second = time.Seconds;	/* 秒 */
    rtcTimerPtr->subSecond = (time.SecondFraction - time.SubSeconds) * 1000 / (1+time.SecondFraction);/*毫秒*/
    //获取到的时间拷贝到系统当前时间
    UserMemCopy((void*)&rtcTimerCurrent,(void*)rtcTimerPtr,LENGTH_RTC_TIMER);
    return ERROR_SUB_OK;
}

//获取RTC内部时间
ERROR_SUB MCU_RTC_GetTimeCurrentUTC(uint32_t* rtcTimerPtr)
{
    RTC_TIMER rtcTimer;
    ERROR_SUB errorSub = ERROR_SUB_OK;
    errorSub = MCU_RTC_GetTimeCurrent(&rtcTimer);
    if(errorSub != ERROR_SUB_OK)
    {
        return errorSub;
    }
    struct tm timeNow;
    timeNow.tm_year = rtcTimer.year - 1900;
    timeNow.tm_mon = rtcTimer.month -1;
    timeNow.tm_mday = rtcTimer.day;
    timeNow.tm_wday = rtcTimer.weekDay;
    timeNow.tm_hour = rtcTimer.hour;
    timeNow.tm_min = rtcTimer.minute;
    timeNow.tm_sec = rtcTimer.second;
    //计算UTC
    *rtcTimerPtr = (uint32_t)mktime(&timeNow);
    return ERROR_SUB_OK;
}

//获取驱动中保存的当前时间,获取之前会去读一次
ERROR_SUB MCU_RTC_GetTimeRecordSync(RTC_TIMER** rtcTimerPtrPtr)
{
    RTC_DateTypeDef  date;
	RTC_TimeTypeDef  time;
    ERROR_SUB errorCodeSub = ERROR_SUB_OK;
    //状态标识
    HAL_StatusTypeDef halState = HAL_OK;
    *rtcTimerPtrPtr = NULL;
    //首先设置数据空
    UserMemClear((void*)&rtcTimerCurrent,LENGTH_RTC_TIMER);
    //句柄
	handleRTC.Instance = RTC;
    //锁定
    uint32_t lastIntState = CoreLockSchedule();
	/* CPU BUG: 必须先读取时间，再读取日期 */
    halState = HAL_RTC_GetTime(&handleRTC, &time, FORMAT_BIN);
    if(halState != HAL_OK)
	{
        MCU_LogShowHalState(halState,handleRTC.State);
        //解锁
        CoreUnlockSchedule(lastIntState);
        return errorCodeSub;
    }
    halState = HAL_RTC_GetDate(&handleRTC, &date, FORMAT_BIN);
    if(halState != HAL_OK)
	{
        MCU_LogShowHalState(halState,handleRTC.State);
        //解锁
        CoreUnlockSchedule(lastIntState);
        return errorCodeSub;
    }
    //解锁
    CoreUnlockSchedule(lastIntState);
    //时间转换
	rtcTimerCurrent.year = date.Year + 2000;
	rtcTimerCurrent.month = date.Month;
	rtcTimerCurrent.day = date.Date;
    rtcTimerCurrent.weekDay = date.WeekDay;
	rtcTimerCurrent.hour = time.Hours;	/* 小时 */
	rtcTimerCurrent.minute = time.Minutes; 	/* 分钟 */
	rtcTimerCurrent.second = time.Seconds;	/* 秒 */
    rtcTimerCurrent.subSecond = (time.SecondFraction - time.SubSeconds) * 1000 / (1+time.SecondFraction);/*毫秒*/
    //赋值
    *rtcTimerPtrPtr = &rtcTimerCurrent;
    return ERROR_SUB_OK;
}

//闹钟回调函数
static MCU_RTC_CallBack rtcAlarmCallBackFuncPtrA = NULL;
//设置闹钟时间(按星期闹铃,24小时制)
//week:星期几(1~7) @ref  RTC_WeekDay_Definitions
ERROR_SUB MCU_RTC_SetAlarmA(uint8_t week,uint8_t hour,uint8_t min,uint8_t sec,MCU_RTC_CallBack alarmCallBackA)
{
    RTC_AlarmTypeDef RTC_AlarmSturuct;
    //状态标识
    HAL_StatusTypeDef halState = HAL_OK;
    //设置时间
    RTC_AlarmSturuct.AlarmTime.Hours=hour;  //小时
    RTC_AlarmSturuct.AlarmTime.Minutes=min; //分钟
    RTC_AlarmSturuct.AlarmTime.Seconds=sec; //秒
    RTC_AlarmSturuct.AlarmTime.SubSeconds=0;
    RTC_AlarmSturuct.AlarmTime.TimeFormat=RTC_HOURFORMAT12_AM;
    //设置参数,匹配模式
    RTC_AlarmSturuct.AlarmMask=MCU_RTC_ALARM_A_MATCH_MASK;
    //是否匹配亚秒
    RTC_AlarmSturuct.AlarmSubSecondMask=RTC_ALARMSUBSECONDMASK_NONE;
    //标识星期而不是日期
    RTC_AlarmSturuct.AlarmDateWeekDaySel=RTC_ALARMDATEWEEKDAYSEL_WEEKDAY;
    RTC_AlarmSturuct.AlarmDateWeekDay=week; 
    RTC_AlarmSturuct.Alarm=RTC_ALARM_A;
    //先关闭中断
    HAL_NVIC_DisableIRQ(RTC_Alarm_IRQn);
    rtcAlarmCallBackFuncPtrA = NULL;
    //设置闹钟
    halState = HAL_RTC_SetAlarm_IT(&handleRTC,&RTC_AlarmSturuct,RTC_FORMAT_BIN);
    if(halState != HAL_OK)
	{
        MCU_LogShowHalState(halState,handleRTC.State);
        return ERROR_SUB_MCU_RTC_SET_ALARM_A;
    }
    //设置中断优先级
    HAL_NVIC_SetPriority(RTC_Alarm_IRQn,MCU_RTC_ALARM_PREE_PRI,MCU_RTC_ALARM_SUB_PRI); 
    HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
    //设置回调函数
    rtcAlarmCallBackFuncPtrA = alarmCallBackA;
    return ERROR_SUB_OK;
}

//闹钟回调函数
static MCU_RTC_CallBack rtcAlarmCallBackFuncPtrB = NULL;
//设置闹钟时间(按星期闹铃,24小时制)
//week:星期几(1~7) @ref  RTC_WeekDay_Definitions
ERROR_SUB MCU_RTC_SetAlarmB(uint8_t week,uint8_t hour,uint8_t min,uint8_t sec,MCU_RTC_CallBack alarmCallBackB)
{
    RTC_AlarmTypeDef RTC_AlarmSturuct;
    //状态标识
    HAL_StatusTypeDef halState = HAL_OK;
    //设置时间
    RTC_AlarmSturuct.AlarmTime.Hours=hour;  //小时
    RTC_AlarmSturuct.AlarmTime.Minutes=min; //分钟
    RTC_AlarmSturuct.AlarmTime.Seconds=sec; //秒
    RTC_AlarmSturuct.AlarmTime.SubSeconds=0;
    RTC_AlarmSturuct.AlarmTime.TimeFormat=RTC_HOURFORMAT12_AM;
    //设置参数,匹配模式
    RTC_AlarmSturuct.AlarmMask=MCU_RTC_ALARM_B_MATCH_MASK;
    //是否匹配亚秒
    RTC_AlarmSturuct.AlarmSubSecondMask=RTC_ALARMSUBSECONDMASK_NONE;
    //标识星期而不是日期
    RTC_AlarmSturuct.AlarmDateWeekDaySel=RTC_ALARMDATEWEEKDAYSEL_WEEKDAY;
    RTC_AlarmSturuct.AlarmDateWeekDay=week; 
    RTC_AlarmSturuct.Alarm=RTC_ALARM_B;
    //先关闭中断
    HAL_NVIC_DisableIRQ(RTC_Alarm_IRQn);
    rtcAlarmCallBackFuncPtrB = NULL;
    //设置闹钟
    halState = HAL_RTC_SetAlarm_IT(&handleRTC,&RTC_AlarmSturuct,RTC_FORMAT_BIN);
    if(halState != HAL_OK)
	{
        MCU_LogShowHalState(halState,handleRTC.State);
        return ERROR_SUB_MCU_RTC_SET_ALARM_B;
    }
    //设置中断优先级
    HAL_NVIC_SetPriority(RTC_Alarm_IRQn,MCU_RTC_ALARM_PREE_PRI,MCU_RTC_ALARM_SUB_PRI); 
    HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
    //设置回调函数
    rtcAlarmCallBackFuncPtrB = alarmCallBackB;
    return ERROR_SUB_OK;
}

//周期性唤醒回调函数
static MCU_RTC_CallBack rtcWakeupCallBackFunc = NULL;
//周期性唤醒设定
ERROR_SUB MCU_RTC_SetPeriodicWakeUp(uint16_t second,MCU_RTC_CallBack wakeupCallBackPtr)
{
    //状态标识
    HAL_StatusTypeDef halState = HAL_OK;
    //清除RTC WAKE UP的标志
    __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&handleRTC, RTC_FLAG_WUTF);
    rtcWakeupCallBackFunc = NULL;
    //先关闭中断
    HAL_NVIC_DisableIRQ(RTC_WKUP_IRQn);
    //设置重装载值和时钟
	halState = HAL_RTCEx_SetWakeUpTimer_IT(&handleRTC,second,RTC_WAKEUPCLOCK_CK_SPRE_16BITS);  
    if(halState != HAL_OK)
	{
        MCU_LogShowHalState(halState,handleRTC.State);
        return ERROR_SUB_MCU_RTC_SET_WAKRUP;
    }
    //设置中断优先级
    HAL_NVIC_SetPriority(RTC_WKUP_IRQn,MCU_RTC_WAKEUP_PREE_PRI,MCU_RTC_WAKEUP_SUB_PRI); 
    HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn); 
    //设置回调函数
    rtcWakeupCallBackFunc = wakeupCallBackPtr;
    return ERROR_SUB_OK;          
}

//RTC闹钟中断服务函数
void RTC_Alarm_IRQHandler(void)
{
    //进入中断,调用ThreadX方法告知系统
    _tx_thread_context_save();
    //调用RTC中断处理函数
    HAL_RTC_AlarmIRQHandler(&handleRTC);
    //退出中断,调用ThreadX方法告知系统
    _tx_thread_context_restore();
}

//RTC WAKE UP中断服务函数
void RTC_WKUP_IRQHandler(void)
{
    //进入中断,调用ThreadX方法告知系统
    _tx_thread_context_save();
    //调用RTC中断处理函数
    HAL_RTCEx_WakeUpTimerIRQHandler(&handleRTC); 
    //退出中断,调用ThreadX方法告知系统
    _tx_thread_context_restore();
}
    
//RTC闹钟A中断处理回调函数
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
    if(rtcAlarmCallBackFuncPtrA != NULL)
    {
        rtcAlarmCallBackFuncPtrA();
    }
}

//RTC闹钟B中断处理回调函数
void HAL_RTCEx_AlarmBEventCallback(RTC_HandleTypeDef *hrtc)
{
    if(rtcAlarmCallBackFuncPtrB != NULL)
    {
        rtcAlarmCallBackFuncPtrB();
    }
}

//RTC WAKE UP中断处理
void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
    if(rtcWakeupCallBackFunc != NULL)
    {
        rtcWakeupCallBackFunc();
    }
}










