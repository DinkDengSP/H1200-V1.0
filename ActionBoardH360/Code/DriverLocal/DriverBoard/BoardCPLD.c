/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-30 10:08:12 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-31 14:44:18 +0800
************************************************************************************************/ 
#include "BoardCPLD.h"

//将RPM速度转换为CPLD实际写入值的辅助计算参数,具体的计算公式看相关CPLD速度计算的文档,计算模式是
//该参数直接除以RPM值,取整即可
#define STEP_MOTOR_1RPM_CALC_UTIL_BASE                  (468714.0f)
//写入宏
#define BOARD_CPLD_WRITE_REG_LOW_LEVEL(addr, value)     *(__IO uint8_t *)(addr) = value
//读取宏
#define BOARD_CPLD_READ_REG_LOW_LEVEL(addr, value)      value = *(__IO uint8_t *)(addr)
//电机回调函数,中断处理
static BOARD_CPLD_MotorIntProc motorIntCallBackFuncPtr = NULL;
//输出端口
static uint8_t portOutValueTempArrayCPLD[5] = {0XFF, 0XFF, 0XFF, 0XFF, 0XFF};
//输入端口
static uint8_t portInValueTempArray[2] = {0x00, 0x00};
//步进电机缓存数组
static BOARD_CPLD_MOTOR_TEMP cpldStepMotorTemp[12] = {0};

//中断回调函数
void BoardCPLD_StepMotorIntProcess(void);
//读取当前步进电机中断状态
static uint16_t BoardCPLD_ReadMotorIntFlagAll(void);
//清除当前步进电机中断状态
static void BoardCPLD_ClearCurrentStepMotorInt(uint16_t motorIntRead);
//CPLD 步进电机全部禁止
static void BoardCPLD_DisableAllStepMotor(void);
//CPLD步进电机全部使能
static void BoardCPLD_EnableAllStepMotor(void);
//清除全部电机中断
static void BoardCPLD_ClearAllStepMotorInt(void);

//端口初始化
static void BoardCPLD_PortInit(void)
{
    /* FSMC SRAM所涉及到GPIO配置 */
	MCU_FSMC_PortInit();
    //输入端口初始化
    MCU_PinInit(PIN_BOARD_CPLD_INT0,MCU_PIN_MODE_INPUT,MCU_PIN_PULL_PULLUP,MCU_PIN_SPEED_FREQ_VERY_HIGH,0);
    MCU_PinInit(PIN_BOARD_CPLD_INT1,MCU_PIN_MODE_INPUT,MCU_PIN_PULL_PULLUP,MCU_PIN_SPEED_FREQ_VERY_HIGH,0);
    MCU_PinInit(PIN_BOARD_CPLD_INT2,MCU_PIN_MODE_INPUT,MCU_PIN_PULL_PULLUP,MCU_PIN_SPEED_FREQ_VERY_HIGH,0);
    MCU_PinInit(PIN_BOARD_CPLD_INT3,MCU_PIN_MODE_INPUT,MCU_PIN_PULL_PULLUP,MCU_PIN_SPEED_FREQ_VERY_HIGH,0);
    //RST端口初始化
    MCU_PinInit(PIN_BOARD_CPLD_RESET,MCU_PIN_MODE_OUTPUT_PP,MCU_PIN_PULL_PULLUP,MCU_PIN_SPEED_FREQ_HIGH,0);
    //CLK端口初始化
    MCU_PinInit(PIN_BOARD_CPLD_CLK,MCU_PIN_MODE_OUTPUT_PP,MCU_PIN_PULL_PULLUP,MCU_PIN_SPEED_FREQ_HIGH,0);
}

//时序初始化
static void BoardCPLD_SeqInit(void)
{
    //端口初始化
    HAL_StatusTypeDef halState = HAL_OK;
    //需要使用的数据结构r
    SRAM_HandleTypeDef CPLD_Handler = {0};
    FSMC_NORSRAM_TimingTypeDef SRAM_Timing = {0};
	/* FSMC SRAM所涉及到GPIO配置 */
	MCU_FSMC_PortInit();
    //CPLD特殊端口配置
    //FSMC时序配置
    //SRAM实体关联
    CPLD_Handler.Instance       = FSMC_NORSRAM_DEVICE;
    CPLD_Handler.Extended       = FSMC_NORSRAM_EXTENDED_DEVICE;
    CPLD_Handler.Init.NSBank    = FSMC_NORSRAM_BANK1;//使用NE1
    HAL_SRAM_DeInit(&CPLD_Handler);
	//SRAM时间配置,静态存储器当成设备用
    //地址建立时间（ADDSET）为,一个单位6ns
	SRAM_Timing.AddressSetupTime      = BOARD_CPLD_ADDRESS_SETUP_CYCLE;
    //地址保持时间,（ADDHLD）模式A未用到
    SRAM_Timing.AddressHoldTime       = 0x00;
    //数据保存时间为 ,,一个单位6ns
    SRAM_Timing.DataSetupTime         = BOARD_CPLD_DATA_SETUP_CYCLE;
    //设置总线转换周期，仅用于复用模式的NOR操作
    SRAM_Timing.BusTurnAroundDuration = 0x00;
    //设置时钟分频，仅用于同步类型的存储器
    SRAM_Timing.CLKDivision           = 0x00;
    //数据保持时间，仅用于同步型的NOR
    SRAM_Timing.DataLatency           = 0x00;
    //选择匹配SRAM的模式
    SRAM_Timing.AccessMode            = FSMC_ACCESS_MODE_A;
    //SRAM时序配置
    CPLD_Handler.Init.DataAddressMux    =FSMC_DATA_ADDRESS_MUX_DISABLE;//地址/数据线不复用
    CPLD_Handler.Init.MemoryType        =FSMC_MEMORY_TYPE_SRAM;//SRAM
    CPLD_Handler.Init.MemoryDataWidth   =FSMC_NORSRAM_MEM_BUS_WIDTH_8;//8位数据宽度
    CPLD_Handler.Init.BurstAccessMode   =FSMC_BURST_ACCESS_MODE_DISABLE;//是否使能突发访问,仅对同步突发存储器有效,此处未用到
    CPLD_Handler.Init.WaitSignalPolarity=FSMC_WAIT_SIGNAL_POLARITY_LOW;//等待信号的极性,仅在突发模式访问下有用
    CPLD_Handler.Init.WrapMode          =FSMC_WRAP_MODE_DISABLE;
    CPLD_Handler.Init.WaitSignalActive  =FSMC_WAIT_TIMING_BEFORE_WS;//存储器是在等待周期之前的一个时钟周期还是等待周期期间使能NWAIT
    CPLD_Handler.Init.WriteOperation    =FSMC_WRITE_OPERATION_ENABLE;//存储器写使能
    CPLD_Handler.Init.WaitSignal        =FSMC_WAIT_SIGNAL_DISABLE;//等待使能位,此处未用到
    CPLD_Handler.Init.ExtendedMode      =FSMC_EXTENDED_MODE_DISABLE;//读写使用相同的时序
    CPLD_Handler.Init.AsynchronousWait  =FSMC_ASYNCHRONOUS_WAIT_DISABLE;//是否使能同步传输模式下的等待信号,此处未用到
    CPLD_Handler.Init.WriteBurst        =FSMC_WRITE_BURST_DISABLE;//禁止突发写
	/* 配置CPLD控制器基本参数 */
	halState = HAL_SRAM_Init(&CPLD_Handler, &SRAM_Timing, &SRAM_Timing);
    if(halState != HAL_OK)
    {
        MCU_LogShowHalState(halState,0);
        while(1);
    }
    if(HAL_SRAM_STATE_PROTECTED == HAL_SRAM_GetState(&CPLD_Handler))
    {
        //使能SRAM写入
        halState = HAL_SRAM_WriteOperation_Enable(&CPLD_Handler);
        if(halState != HAL_OK)
        {
            MCU_LogShowHalState(halState,0);
            while(1);
        }
    }
}

//中断初始化
static void BoardCPLD_IntInit(BOARD_CPLD_MotorIntProc callBack)
{
    //将指定的中断讯息清除
    BoardCPLD_ClearCurrentStepMotorInt(0X0FFF);
    if(callBack == NULL)
    {
        BoardCPLD_DisableAllStepMotor();
    }
    else
    {
        BoardCPLD_EnableAllStepMotor();
    }
    //清除全部电机中断
    BoardCPLD_ClearAllStepMotorInt();
    //中断端口初始化
    MCU_PinExtiInit(PIN_BOARD_CPLD_INT1,INT_MODE_PIN_BOARD_CPLD_INT1,MCU_PIN_PULL_PULLUP,
                        MCU_PIN_SPEED_FREQ_HIGH,BOARD_CPLD_PREE_PRI_INT,BOARD_CPLD_SUB_PRI_INT,
                        BoardCPLD_StepMotorIntProcess);
}


//CPLD复位
void BoardCPLD_ChipReset(void)
{
    //RESET高电平
    MCU_PinWriteSingle(PIN_BOARD_CPLD_RESET, PIN_INVALID_LEVEL(VALID_LEVEL_PIN_BOARD_CPLD_RESET));
    //延时等待
    CoreDelayMs(20);
    //RESET低电平
    MCU_PinWriteSingle(PIN_BOARD_CPLD_RESET, VALID_LEVEL_PIN_BOARD_CPLD_RESET);
    //延时等待
    CoreDelayMs(20);
    //高电平
    MCU_PinWriteSingle(PIN_BOARD_CPLD_RESET, PIN_INVALID_LEVEL(VALID_LEVEL_PIN_BOARD_CPLD_RESET));
    //延时等待
    CoreDelayMs(20);
    //初始化输入输出状态
    uint8_t indexUtil = 0;
    for(uint8_t indexUtil = 0; indexUtil < 5 ; indexUtil++)
    {
        portOutValueTempArrayCPLD[indexUtil] = 0XFF;
    }
    for(indexUtil = 0; indexUtil < 1; indexUtil++)
    {
        portInValueTempArray[indexUtil] = 0x00;
    }
}

//CPLD识别码
static uint16_t BoardCPLD_ReadChipID(void)
{
    uint16_t cpldID = 0;
    uint8_t readTemp;
    BOARD_CPLD_READ_REG_LOW_LEVEL(BOARD_CPLD_REG_ID_CODE_CC, readTemp);
    cpldID += readTemp;
    cpldID <<= 8;
    BOARD_CPLD_READ_REG_LOW_LEVEL(BOARD_CPLD_REG_ID_CODE_55, readTemp);
    cpldID += readTemp;
    return cpldID;
}


//CPLD初始化,带复位
ERROR_SUB BoardCPLD_Init(BOARD_CPLD_MotorIntProc callBack)
{
    //端口初始化
    BoardCPLD_PortInit();
    //时序初始化
    BoardCPLD_SeqInit();
    //CPLD复位
    BoardCPLD_ChipReset();
    //检查ID
    if(BOARD_CPLD_ID_CODE != BoardCPLD_ReadChipID())
    {
        return ERROR_SUB_BOARD_CPLD_CHECK_ID;
    }
    //中断回调函数设定
    motorIntCallBackFuncPtr = callBack;
    //中断初始化
    BoardCPLD_IntInit(callBack);
    return ERROR_SUB_OK;
}


//CPLD写入单个IO
void BoardCPLD_WriteOutputPin(BOARD_CPLD_OUT_PIN outPin,GPIO_PinState bitValue)
{
    uint8_t portIndex = outPin / 8;
    uint8_t pinIndex = outPin % 8;
    if (bitValue == GPIO_PIN_SET)
    {
        //写1
        portOutValueTempArrayCPLD[portIndex] |= (0x01 << pinIndex);
    }
    else
    {
        //清零
        portOutValueTempArrayCPLD[portIndex] &= ~(0x01 << pinIndex);
    }
    //写入
    BOARD_CPLD_WRITE_REG_LOW_LEVEL(BOARD_CPLD_REG_WRITE_PORT_OUT1 + portIndex, portOutValueTempArrayCPLD[portIndex]);
    return;
}

//CPLD读取单个IO
GPIO_PinState BoardCPLD_ReadInputPin(BOARD_CPLD_IN_PIN inPin)
{
    uint8_t portIndex = inPin / 8;
    uint8_t pinIndex = inPin % 8;
    uint8_t readTemp = 0;
    //读取
    BOARD_CPLD_READ_REG_LOW_LEVEL((BOARD_CPLD_REG_READ_PORT_IN_0 + portIndex), readTemp);
    //将读取到的值转换为bitAction
    uint8_t pinValue = (readTemp >> pinIndex);
    if (pinValue == 0)
    {
        portInValueTempArray[portIndex] &= ~(1 << pinIndex);
        return GPIO_PIN_RESET;
    }
    else
    {
        portInValueTempArray[portIndex] |= (1 << pinIndex);
        return GPIO_PIN_SET;
    }
}


//CPLD 步进电机全部禁止
static void BoardCPLD_DisableAllStepMotor(void)
{
    BOARD_CPLD_MOTOR motorNo = BOARD_CPLD_MOTOR1;
    for(motorNo = BOARD_CPLD_MOTOR1; motorNo <= BOARD_CPLD_MOTOR12; motorNo++)
    {
        //禁止电机
        BoardCPLD_WriteStepMotorSingle(motorNo,
                                        BOARD_CPLD_MOTOR_EN_DISABLE,
                                        BOARD_CPLD_MOTOR_DIR_CW,
                                        BOARD_CPLD_MOTOR_STOP_ON,
                                        BOARD_CPLD_MOTOR_FREQ_COUNT_MAX,
                                        1,
                                        0);
        //延时防止电源冲击
        CoreDelayMs(BOARD_CPLD_MOTOR_DISABLE_INTERVAL_MS);
    }
}

//CPLD步进电机全部使能
static void BoardCPLD_EnableAllStepMotor(void)
{
    BOARD_CPLD_MOTOR motorNo = BOARD_CPLD_MOTOR1;
    for(motorNo = BOARD_CPLD_MOTOR1; motorNo <= BOARD_CPLD_MOTOR12; motorNo++)
    {
        //使能电机
        BoardCPLD_WriteStepMotorSingle(motorNo,
                                        BOARD_CPLD_MOTOR_EN_ENABLE,
                                        BOARD_CPLD_MOTOR_DIR_CW,
                                        BOARD_CPLD_MOTOR_STOP_ON,
                                        BOARD_CPLD_MOTOR_FREQ_COUNT_MAX,
                                        1,
                                        0);
        //延时防止电源冲击
        CoreDelayMs(BOARD_CPLD_MOTOR_ENABLE_INTERVAL_MS);
    }
}

//读取当前步进电机中断状态
static uint16_t BoardCPLD_ReadMotorIntFlagAll(void)
{
    uint16_t stepMotorIntFlagAll = 0;
    uint8_t stepMotorIntTemp = 0;
    //申请锁定必备的变量
    BOARD_CPLD_THREAD_LOCK_VAR_ALLOC();
    //锁定调度
    BOARD_CPLD_THREAD_LOCK();
    //读取高八位
    BOARD_CPLD_READ_REG_LOW_LEVEL(BOARD_CPLD_REG_READ_MOTOR_INT_H, stepMotorIntTemp);
    //赋值
    stepMotorIntFlagAll = stepMotorIntTemp;
    //读取低八位
    BOARD_CPLD_READ_REG_LOW_LEVEL(BOARD_CPLD_REG_READ_MOTOR_INT_L, stepMotorIntTemp);
    //上一次读取的数据右移
    stepMotorIntFlagAll<<= 8;
    //赋值
    stepMotorIntFlagAll += stepMotorIntTemp;
    //清除无效字节
    stepMotorIntFlagAll &= 0X0FFF;
    //解锁调度
    BOARD_CPLD_THREAD_UNLOCK();
    //返回
    return stepMotorIntFlagAll;
}

//清除当前步进电机中断状态
static void BoardCPLD_ClearCurrentStepMotorInt(uint16_t motorIntRead)
{
    uint8_t writeTempSingle = 0;
    //申请锁定必备的变量
    BOARD_CPLD_THREAD_LOCK_VAR_ALLOC();
    //清除低八位
    writeTempSingle = (uint8_t)(motorIntRead);
    BOARD_CPLD_THREAD_LOCK();
    BOARD_CPLD_WRITE_REG_LOW_LEVEL(BOARD_CPLD_REG_WRITE_MOTOR_INT_CLEAR_L, writeTempSingle);
    //清除高四位
    writeTempSingle = (uint8_t)(motorIntRead>>8);
    BOARD_CPLD_WRITE_REG_LOW_LEVEL(BOARD_CPLD_REG_WRITE_MOTOR_INT_CLEAR_H, writeTempSingle);
    BOARD_CPLD_THREAD_UNLOCK();
}

//CPLD写入一次电机数据
void BoardCPLD_WriteStepMotorSingle(BOARD_CPLD_MOTOR index,BOARD_CPLD_MOTOR_EN enable,BOARD_CPLD_MOTOR_DIR dir,
                                        BOARD_CPLD_MOTOR_STOP stopFlag,float rpm,uint8_t div,uint8_t stepCount)
{
    uint16_t temp = 0;
    uint16_t freqCount;
    //申请锁定必备的变量
    BOARD_CPLD_THREAD_LOCK_VAR_ALLOC();
    if(index > BOARD_CPLD_MOTOR12)
    {
        return;
    }
    rpm *= div;
    if(rpm >= BOARD_CPLD_MOTOR_SPEED_MAX_RPM)
    {
        rpm = BOARD_CPLD_MOTOR_SPEED_MAX_RPM;
    }
    if(rpm <= BOARD_CPLD_MOTOR_SPEED_MIN_RPM)
    {
        rpm = BOARD_CPLD_MOTOR_SPEED_MIN_RPM;
    }
    //计算出写入值
    freqCount = (uint16_t)(STEP_MOTOR_1RPM_CALC_UTIL_BASE/rpm);
    //写入值的上下限进行限制计算
    if(freqCount >= BOARD_CPLD_MOTOR_FREQ_COUNT_MAX)
    {
        freqCount = BOARD_CPLD_MOTOR_FREQ_COUNT_MAX;
    }
    if(freqCount <= BOARD_CPLD_MOTOR_FREQ_COUNT_MIN)
    {
        freqCount = BOARD_CPLD_MOTOR_FREQ_COUNT_MIN;
    }
    //写入时候必须锁定,防止其他中断打断
    BOARD_CPLD_THREAD_LOCK();
    //使能
    temp = enable;
    temp <<= BOARD_CPLD_SM_ENABLE_BIT_SHIFT;
    cpldStepMotorTemp[index].motorReg = 0;
    cpldStepMotorTemp[index].motorReg |= temp;
    //方向
    temp = dir;
    temp <<= BOARD_CPLD_SM_DIR_BIT_SHIFT;
    cpldStepMotorTemp[index].motorReg |= temp;
    //急停
    temp = stopFlag;
    temp <<= BOARD_CPLD_SM_STOP_BIT_SHIFT;
    cpldStepMotorTemp[index].motorReg |= temp;
    //频率计数
    cpldStepMotorTemp[index].motorReg |= freqCount;
    //计数值
    cpldStepMotorTemp[index].motorCount = stepCount;
    //写入数据
    //先写入计数值
    BOARD_CPLD_WRITE_REG_LOW_LEVEL(BOARD_CPLD_REG_WRITE_MOTOR_1_COUNT+(index*2), cpldStepMotorTemp[index].motorCount);
    //写入寄存器低位
    BOARD_CPLD_WRITE_REG_LOW_LEVEL(BOARD_CPLD_REG_WRITE_MOTOR_COMMON_REG_L, (uint8_t)(cpldStepMotorTemp[index].motorReg));
    //写入寄存器高位
    BOARD_CPLD_WRITE_REG_LOW_LEVEL(BOARD_CPLD_REG_WRITE_MOTOR_1_REG_H+(index*2), (uint8_t)(cpldStepMotorTemp[index].motorReg>>8));
    //锁定之后需要解锁
    BOARD_CPLD_THREAD_UNLOCK();
}

//CPLD立即进入下一步,减速时候需要用
void BoardCPLD_WriteStepMotorRunNextStep(BOARD_CPLD_MOTOR index)
{
    //申请锁定必备的变量
    BOARD_CPLD_THREAD_LOCK_VAR_ALLOC();
    if(index > BOARD_CPLD_MOTOR12)
    {
        return;
    }
    BOARD_CPLD_THREAD_LOCK();
    //直接将计数值写成0即可
    cpldStepMotorTemp[index].motorCount = 0;
    //先写入计数值
    BOARD_CPLD_WRITE_REG_LOW_LEVEL(BOARD_CPLD_REG_WRITE_MOTOR_1_COUNT+(index*2), cpldStepMotorTemp[index].motorCount);
    //写入寄存器低位
    BOARD_CPLD_WRITE_REG_LOW_LEVEL(BOARD_CPLD_REG_WRITE_MOTOR_COMMON_REG_L, (uint8_t)(cpldStepMotorTemp[index].motorReg));
    //写入寄存器高位
    BOARD_CPLD_WRITE_REG_LOW_LEVEL(BOARD_CPLD_REG_WRITE_MOTOR_1_REG_H+(index*2), (uint8_t)(cpldStepMotorTemp[index].motorReg>>8));
    BOARD_CPLD_THREAD_UNLOCK();
}

//CPLD 立即急停,在触发急停时候发送,同时,在脉冲发送完成之后的中断中也要发送
void BoardCPLD_StepMotorEmergencyStop(BOARD_CPLD_MOTOR index)
{
    //申请锁定必备的变量
    BOARD_CPLD_THREAD_LOCK_VAR_ALLOC();
    if(index > BOARD_CPLD_MOTOR12)
    {
        return;
    }
    BOARD_CPLD_THREAD_LOCK();
    cpldStepMotorTemp[index].motorReg &= ~((0x0001)<<BOARD_CPLD_SM_STOP_BIT_SHIFT);
    cpldStepMotorTemp[index].motorCount = 0;
    //先写入计数值
    BOARD_CPLD_WRITE_REG_LOW_LEVEL(BOARD_CPLD_REG_WRITE_MOTOR_1_COUNT+(index*2), cpldStepMotorTemp[index].motorCount);
    //写入寄存器低位
    BOARD_CPLD_WRITE_REG_LOW_LEVEL(BOARD_CPLD_REG_WRITE_MOTOR_COMMON_REG_L, (uint8_t)(cpldStepMotorTemp[index].motorReg));
    //写入寄存器高位
    BOARD_CPLD_WRITE_REG_LOW_LEVEL(BOARD_CPLD_REG_WRITE_MOTOR_1_REG_H+(index*2), (uint8_t)(cpldStepMotorTemp[index].motorReg>>8));
    BOARD_CPLD_THREAD_UNLOCK();
}

//清除全部电机中断
static void BoardCPLD_ClearAllStepMotorInt(void)
{
    //清除电机中断讯息
    do
    {
        //将指定的中断讯息清除
        BoardCPLD_ClearCurrentStepMotorInt(0X0FFF);
        //等待中断信号必须清除
        if(VALID_LEVEL_PIN_BOARD_CPLD_INT1 == MCU_PinReadSingle(PIN_BOARD_CPLD_INT1))
        {
            CoreDelayMinTick();
        }
    }while(VALID_LEVEL_PIN_BOARD_CPLD_INT1 ==  MCU_PinReadSingle(PIN_BOARD_CPLD_INT1));
}


//中断回调函数
void BoardCPLD_StepMotorIntProcess(void)
{
    BOARD_CPLD_MOTOR motorNo = BOARD_CPLD_MOTOR1;
    do
    {
        //读取电机中断讯号
        uint16_t mototIntSignal = BoardCPLD_ReadMotorIntFlagAll();
        //将指定的中断讯息清除
        BoardCPLD_ClearCurrentStepMotorInt(mototIntSignal);
        //设定发生中断的电机急停,也是清除中断信息
        for(motorNo = BOARD_CPLD_MOTOR1; motorNo <= BOARD_CPLD_MOTOR12; motorNo++)
        {
            if((mototIntSignal>>motorNo)&0x0001)
            {
                //对当前电机写入停止,切换进入下一次执行状态
               BoardCPLD_WriteStepMotorSingle(motorNo,
                                                BOARD_CPLD_MOTOR_EN_ENABLE,
                                                BOARD_CPLD_MOTOR_DIR_CW,
                                                BOARD_CPLD_MOTOR_STOP_ON,
                                                BOARD_CPLD_MOTOR_FREQ_COUNT_MIN,
                                                1,
                                                0);
            }
        }
        //处理电机事件
        for (motorNo = BOARD_CPLD_MOTOR1; motorNo <= BOARD_CPLD_MOTOR12; motorNo++)
        {
            if((mototIntSignal>>motorNo)&0x0001)
            {
                //然后调用各个电机的中断处理函数,决定接下来的处理方式
                if (motorIntCallBackFuncPtr != NULL)
                {
                    motorIntCallBackFuncPtr(motorNo);
                }
            }
        }
    }while(VALID_LEVEL_PIN_BOARD_CPLD_INT1 == MCU_PinReadSingle(PIN_BOARD_CPLD_INT1));
}












