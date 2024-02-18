/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-04-26 10:16:12 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-05-08 14:03:12 +0800
************************************************************************************************/ 
#include "BoardFPGA_Base.h"
#include "DriverHeaderMCU.h"

//FPGA读取寄存器
uint16_t BoardFPGA_ReadReg16(uint16_t regAddr)
{
    uint16_t readTemp = 0x0000;
    readTemp = *(volatile uint16_t *)BOARD_FPGA_CALC_ADDR(regAddr);
    return readTemp;
}

//FPGA读取寄存器
uint32_t BoardFPGA_ReadReg32(uint16_t regAddr)
{
    uint32_t readTemp = 0X00000000;
    //关闭全局中断
    BOARD_FPGA_LOCK();
    //读取高位
    readTemp += *(volatile uint16_t *)BOARD_FPGA_CALC_ADDR(regAddr);
    //移位
    readTemp <<= 16;
    //读取低位
    readTemp += *(volatile uint16_t *)BOARD_FPGA_CALC_ADDR(regAddr-1);
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
    return readTemp;
}

//读取写入忙状态
static FPGA_WRITE_STATE BoardFPGA_GetWriteState(void)
{
    uint16_t busyState = BoardFPGA_ReadReg16(FPGA_REG_READ_ADDR_CHIP_BUSY);
    if((busyState &0x0001)!= 0)
    {
        return FPGA_WRITE_STATE_BUSY;
    }
    return FPGA_WRITE_STATE_IDLE;
}

//等待写入空闲
static ERROR_SUB BoardFPGA_WaitWriteIdle(void)
{
    uint16_t waitClkCount = 0;
    do
    {
        if(FPGA_WRITE_STATE_IDLE == BoardFPGA_GetWriteState())
        {
            return ERROR_SUB_OK;
        }
        waitClkCount++;
        if(waitClkCount > BOARD_FPGA_WRITE_WAIT_CYCLE_MAX)
        {
            return ERROR_SUB_BOARD_FPGA_WRITE_WAIT_TIME_OUT;
        }
    } while (1);
}

//FPGA写入寄存器
static void BoardFPGA_WriteReg16(uint16_t regAddr,uint16_t regValue)
{
    *(volatile uint16_t *)BOARD_FPGA_CALC_ADDR(regAddr) = regValue;
}


//写入FPGA指定地址指定数据
ERROR_SUB BoardFPGA_WriteDataByAddr(uint32_t addr,uint32_t data)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //关闭全局中断
    BOARD_FPGA_LOCK();
    //等待写入空闲
    // errorSub = BoardFPGA_WaitWriteIdle();
    // if(errorSub != ERROR_SUB_OK)
    // {
    //     //打开全局中断
    //     BOARD_FPGA_UNLOCK(lastIntState);
    //     return errorSub;
    // }
    //写入地址,低字节
    BoardFPGA_WriteReg16(FPGA_REG_WRITE_BASE_ADDR_LOW,(uint16_t)addr);
    //写入地址,高字节
    BoardFPGA_WriteReg16(FPGA_REG_WRITE_BASE_ADDR_HIGH,(uint16_t)(addr>>16));
    //写入数据,低字节
    BoardFPGA_WriteReg16(FPGA_REG_WRITE_BASE_DATA_LOW,(uint16_t)data);
    //写入数据,高字节
    BoardFPGA_WriteReg16(FPGA_REG_WRITE_BASE_DATA_HIGH,(uint16_t)(data>>16));
    //等待写入空闲
    errorSub = BoardFPGA_WaitWriteIdle();
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        return errorSub;
    }
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
    return errorSub;
}

//写入FPGA指定地址指定数据并读取相应的地址查看是否成功写入
ERROR_SUB BoardFPGA_WriteDataByAddrWithCheck(uint32_t writeAddr,uint32_t data,uint16_t readCheckAddr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //关闭全局中断
    BOARD_FPGA_LOCK();
    //首先数据写入
    errorSub = BoardFPGA_WriteDataByAddr(writeAddr,data);
    if(errorSub != ERROR_SUB_OK)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        return errorSub;
    }
    //然后读取寄存器查看是否写入成功
    uint32_t readRegVal = BoardFPGA_ReadReg32(readCheckAddr);
    if(readRegVal == data)
    {
        //打开全局中断
        BOARD_FPGA_UNLOCK(lastIntState);
        return ERROR_SUB_OK;
    }
    //打开全局中断
    BOARD_FPGA_UNLOCK(lastIntState);
    //显示不匹配的地方
    MCU_LogPrintf("%s,writeAddr: 0X%08X,WriteVal: 0X%08X,ReadVal: 0X%08X\r\n",__func__,writeAddr,data,readRegVal);
    return ERROR_SUB_BOARD_FPGA_WRITE_READ_NOT_MATCH;
}

//FPGA复位
void BoardFPGA_PortReset(void)
{
    //复位
    MCU_PinWriteSingle(BOARD_FPGA_TRST_PIN,VALID_LEVEL_BOARD_FPGA_TRST);
    //延时取消复位
    CoreDelayMs(10);
    //取消复位
    MCU_PinWriteSingle(BOARD_FPGA_TRST_PIN,PIN_INVALID_LEVEL(VALID_LEVEL_BOARD_FPGA_TRST));
}

//端口初始化
void BoardFPGA_PortInit(void)
{
    HAL_StatusTypeDef halState = HAL_OK;
    //需要使用的数据结构r
    SRAM_HandleTypeDef hsram = {0};
    FMC_NORSRAM_TimingTypeDef SRAM_Timing = {0};
    //打开FMC时钟
    __HAL_RCC_FMC_CLK_ENABLE();
	/* FMC SRAM所涉及到GPIO配置 */
	MCU_FMC_PortInit();
    //SRAM实体关联
    hsram.Instance = FMC_NORSRAM_DEVICE;
    hsram.Extended = FMC_NORSRAM_EXTENDED_DEVICE;
    HAL_SRAM_DeInit(&hsram);
	//SRAM时间配置,静态存储器当成设备用1/240M=4ns
    //地址建立时间,4ns单位
	SRAM_Timing.AddressSetupTime      = BOARD_FPGA_ADDRESS_SETUP_CYCLE;
    //地址保持时间,（ADDHLD）模式A未用到
    SRAM_Timing.AddressHoldTime       = 0x00;
    //数据保持时间（DATAST）+ 1个HCLK 4ns单位
    SRAM_Timing.DataSetupTime         = BOARD_FPGA_DATA_SETUP_CYCLE;
    //设置总线转换周期，仅用于复用模式的NOR操作
    SRAM_Timing.BusTurnAroundDuration = 0x00;
    //设置时钟分频，仅用于同步类型的存储器
    SRAM_Timing.CLKDivision           = 0x00;
    //数据保持时间，仅用于同步型的NOR
    SRAM_Timing.DataLatency           = 0x00;
    //选择匹配SRAM的模式
    SRAM_Timing.AccessMode            = FMC_ACCESS_MODE_A;
    //SRAM时序配置
    hsram.Init.NSBank            = FMC_NORSRAM_BANK1;//使用NE1
    hsram.Init.DataAddressMux    = FMC_DATA_ADDRESS_MUX_DISABLE;//地址/数据线不复用
    hsram.Init.MemoryType        = FMC_MEMORY_TYPE_SRAM;//SRAM
    hsram.Init.MemoryDataWidth   = FMC_NORSRAM_MEM_BUS_WIDTH_16;//16位数据宽度
    hsram.Init.BurstAccessMode   = FMC_BURST_ACCESS_MODE_DISABLE;//是否使能突发访问,仅对同步突发存储器有效,此处未用到
    hsram.Init.WaitSignalPolarity= FMC_WAIT_SIGNAL_POLARITY_LOW;//等待信号的极性,仅在突发模式访问下有用
    hsram.Init.WaitSignalActive  = FMC_WAIT_TIMING_BEFORE_WS;//存储器是在等待周期之前的一个时钟周期还是等待周期期间使能NWAIT
    hsram.Init.WriteOperation    = FMC_WRITE_OPERATION_ENABLE;//存储器写使能
    hsram.Init.WriteFifo         = FMC_WRITE_FIFO_DISABLE;//环绕模式
    hsram.Init.WaitSignal        = FMC_WAIT_SIGNAL_DISABLE;//等待使能位,此处未用到
    hsram.Init.ExtendedMode      = FMC_EXTENDED_MODE_DISABLE;//读写使用相同的时序
    hsram.Init.AsynchronousWait  = FMC_ASYNCHRONOUS_WAIT_DISABLE;//是否使能同步传输模式下的等待信号,此处未用到
    hsram.Init.WriteBurst        = FMC_WRITE_BURST_DISABLE;//禁止突发写
	/* 配置SRAM控制器基本参数 */
	halState = HAL_SRAM_Init(&hsram, &SRAM_Timing, &SRAM_Timing);
    if(halState != HAL_OK)
    {
        MCU_LogShowHalState(halState,0);
        while(1);
    }
    if(HAL_SRAM_STATE_PROTECTED != HAL_SRAM_GetState(&hsram))
    {
        return;
    }
    //使能SRAM写入
    halState = HAL_SRAM_WriteOperation_Enable(&hsram);
    if(halState != HAL_OK)
    {
        MCU_LogShowHalState(halState,0);
        while(1);
    }
    //初始化config_done引脚
    MCU_PinInit(BOARD_FPGA_CONFIG_DONE_PIN,MCU_PIN_MODE_INPUT,MCU_PIN_PULL_NOPULL,MCU_PIN_SPEED_FREQ_MEDIUM,0);
    //初始化nCONFIG
    MCU_PinInit(BOARD_FPGA_N_CONFIG_PIN,MCU_PIN_MODE_INPUT,MCU_PIN_PULL_NOPULL,MCU_PIN_SPEED_FREQ_MEDIUM,0);
    //初始化nSTATUS
    MCU_PinInit(BOARD_FPGA_N_STATUS_PIN,MCU_PIN_MODE_INPUT,MCU_PIN_PULL_NOPULL,MCU_PIN_SPEED_FREQ_MEDIUM,0);
    //初始化复位引脚
    MCU_PinInit(BOARD_FPGA_TRST_PIN,MCU_PIN_MODE_OUTPUT_PP,MCU_PIN_PULL_PULLDOWN,MCU_PIN_SPEED_FREQ_MEDIUM,0);
    //初始化配置清空引脚
    MCU_PinInit(BOARD_FPGA_CLRN_PIN,MCU_PIN_MODE_INPUT,MCU_PIN_PULL_NOPULL,MCU_PIN_SPEED_FREQ_MEDIUM,0);
    //FPGA复位
    BoardFPGA_PortReset();
}


//FPGA自检
ERROR_SUB BoardFPGA_CheckExist(void)
{
    uint32_t utilTemp = 0X00000000;
    //首先检测config_done
    // GPIO_PinState configDoneState = MCU_PinReadSingle(BOARD_FPGA_CONFIG_DONE_PIN);
    // if(configDoneState != VALID_LEVEL_BOARD_FPGA_CONFIG_DONE)
    // {
    //     //错误显示
    //     MCU_LogShowErrorSubCode(ERROR_SUB_BOARD_FPGA_NOT_CONFIG_DONE);
    //     return ERROR_SUB_BOARD_FPGA_NOT_CONFIG_DONE;
    // }
    //读取ID
    utilTemp = BoardFPGA_ReadReg32(FPGA_REG_READ_ADDR_CHIP_ID);
    if(utilTemp != BOARD_FPGA_CHIP_ID)
    {
        //错误显示
        MCU_LogShowErrorSubCode(ERROR_SUB_BOARD_FPGA_FMC_CHECK_EXIST_ID_MATCH);
        return ERROR_SUB_BOARD_FPGA_FMC_CHECK_EXIST_ID_MATCH;
    }
    return ERROR_SUB_OK;
}

//获取软件版本
FPGA_SOFT_VERSION BoardFPGA_GetVersion(void)
{
    FPGA_SOFT_VERSION softVersion;
    softVersion.valueAll = BoardFPGA_ReadReg32(FPGA_REG_READ_ADDR_VERSION);
    return softVersion;
}

//导入读取电机中断状态API
extern STEP_MOTOR_INT BoardFPGA_StepMotorGetIntSrc(void);
extern FPGA_STEP_MOTOR_REG_STATE4 BoardFPGA_StepMotorReadRegMotorState(STEP_MOTOR_NO motorNo);
extern FPGA_STEP_MOTOR_REG_CONTROL BoardFPGA_StepMotorReadRegControl(STEP_MOTOR_NO motorNo);
extern ERROR_SUB BoardFPGA_StepMotorWriteRegControl(STEP_MOTOR_NO motorNo,FPGA_STEP_MOTOR_REG_CONTROL regControl);
//中断回调函数
static BoardFPGA_StepMotorIntCallBack stepMotorIntCallBackFuncPtr = NULL;

//步进电机中断1-4
static void BoardFPGA_StepMotorIntCallBackMain1_4(void)
{
    //读取步进电机当前中断状态
    STEP_MOTOR_INT motorIntState = BoardFPGA_StepMotorGetIntSrc();
    FPGA_STEP_MOTOR_REG_STATE4 motorStateReg4;
    GPIO_PinState intPinState = GPIO_PIN_RESET;
    //轮询检测步进电机中断状态并进行处理
    uint32_t motorIntUtil = 0X00000000;
    STEP_MOTOR_NO motorIndexUtil = STEP_MOTOR_NO1;
    do
    {
        //依次处理电机的中断
        for(motorIndexUtil = STEP_MOTOR_NO1;motorIndexUtil <= STEP_MOTOR_NO4; motorIndexUtil++)
        {
            motorIntUtil = (0X00000001 << ((uint8_t)motorIndexUtil));
            if((motorIntState.valAll&motorIntUtil) != 0)
            {
                //当前电机存在中断,读取电机状态
                motorStateReg4 = BoardFPGA_StepMotorReadRegMotorState(motorIndexUtil);
                //使用回调函数
                if(stepMotorIntCallBackFuncPtr != NULL)
                {
                    stepMotorIntCallBackFuncPtr(motorIndexUtil,motorStateReg4.valBits.motorState,motorStateReg4.valBits.stopReason);
                }
            }
        }
        //读取中断端口状态
        intPinState = MCU_PinReadSingle(BOARD_FPGA_INT0_PIN);
        //读取中断线状态
        motorIntState = BoardFPGA_StepMotorGetIntSrc();
    } while ((intPinState == GPIO_PIN_SET)||(motorIntState.valAll != 0));//端口高电平,或者中断线依然有效,持续运行
}

//步进电机中断5-8
static void BoardFPGA_StepMotorIntCallBackMain5_8(void)
{
    //读取步进电机当前中断状态
    STEP_MOTOR_INT motorIntState = BoardFPGA_StepMotorGetIntSrc();
    FPGA_STEP_MOTOR_REG_STATE4 motorStateReg4;
    GPIO_PinState intPinState = GPIO_PIN_RESET;
    //轮询检测步进电机中断状态并进行处理
    uint32_t motorIntUtil = 0X00000000;
    STEP_MOTOR_NO motorIndexUtil = STEP_MOTOR_NO1;
    do
    {
        //依次处理电机的中断
        for(motorIndexUtil = STEP_MOTOR_NO5;motorIndexUtil <= STEP_MOTOR_NO8; motorIndexUtil++)
        {
            motorIntUtil = (0X00000001 << ((uint8_t)motorIndexUtil));
            if((motorIntState.valAll&motorIntUtil) != 0)
            {
                //当前电机存在中断,读取电机状态
                motorStateReg4 = BoardFPGA_StepMotorReadRegMotorState(motorIndexUtil);
                //使用回调函数
                if(stepMotorIntCallBackFuncPtr != NULL)
                {
                    stepMotorIntCallBackFuncPtr(motorIndexUtil,motorStateReg4.valBits.motorState,motorStateReg4.valBits.stopReason);
                }
            }
        }
        //读取中断端口状态
        intPinState = MCU_PinReadSingle(BOARD_FPGA_INT1_PIN);
        //读取中断线状态
        motorIntState = BoardFPGA_StepMotorGetIntSrc();
    } while ((intPinState == GPIO_PIN_SET)||(motorIntState.valAll != 0));//端口高电平,或者中断线依然有效,持续运行
}

//步进电机中断9-12
static void BoardFPGA_StepMotorIntCallBackMain9_12(void)
{
    //读取步进电机当前中断状态
    STEP_MOTOR_INT motorIntState = BoardFPGA_StepMotorGetIntSrc();
    FPGA_STEP_MOTOR_REG_STATE4 motorStateReg4;
    GPIO_PinState intPinState = GPIO_PIN_RESET;
    //轮询检测步进电机中断状态并进行处理
    uint32_t motorIntUtil = 0X00000000;
    STEP_MOTOR_NO motorIndexUtil = STEP_MOTOR_NO1;
    do
    {
        //依次处理电机的中断
        for(motorIndexUtil = STEP_MOTOR_NO9;motorIndexUtil <= STEP_MOTOR_NO12; motorIndexUtil++)
        {
            motorIntUtil = (0X00000001 << ((uint8_t)motorIndexUtil));
            if((motorIntState.valAll&motorIntUtil) != 0)
            {
                //当前电机存在中断,读取电机状态
                motorStateReg4 = BoardFPGA_StepMotorReadRegMotorState(motorIndexUtil);
                //使用回调函数
                if(stepMotorIntCallBackFuncPtr != NULL)
                {
                    stepMotorIntCallBackFuncPtr(motorIndexUtil,motorStateReg4.valBits.motorState,motorStateReg4.valBits.stopReason);
                }
            }
        }
        //读取中断端口状态
        intPinState = MCU_PinReadSingle(BOARD_FPGA_INT2_PIN);
        //读取中断线状态
        motorIntState = BoardFPGA_StepMotorGetIntSrc();
    } while ((intPinState == GPIO_PIN_SET)||(motorIntState.valAll != 0));//端口高电平,或者中断线依然有效,持续运行
}

//步进电机中断13-16
static void BoardFPGA_StepMotorIntCallBackMain13_16(void)
{
    //读取步进电机当前中断状态
    STEP_MOTOR_INT motorIntState = BoardFPGA_StepMotorGetIntSrc();
    FPGA_STEP_MOTOR_REG_STATE4 motorStateReg4;
    GPIO_PinState intPinState = GPIO_PIN_RESET;
    //轮询检测步进电机中断状态并进行处理
    uint32_t motorIntUtil = 0X00000000;
    STEP_MOTOR_NO motorIndexUtil = STEP_MOTOR_NO1;
    do
    {
        //依次处理电机的中断
        for(motorIndexUtil = STEP_MOTOR_NO13;motorIndexUtil <= STEP_MOTOR_NO16; motorIndexUtil++)
        {
            motorIntUtil = (0X00000001 << ((uint8_t)motorIndexUtil));
            if((motorIntState.valAll&motorIntUtil) != 0)
            {
                //当前电机存在中断,读取电机状态
                motorStateReg4 = BoardFPGA_StepMotorReadRegMotorState(motorIndexUtil);
                //使用回调函数
                if(stepMotorIntCallBackFuncPtr != NULL)
                {
                    stepMotorIntCallBackFuncPtr(motorIndexUtil,motorStateReg4.valBits.motorState,motorStateReg4.valBits.stopReason);
                }
            }
        }
        //读取中断端口状态
        intPinState = MCU_PinReadSingle(BOARD_FPGA_INT3_PIN);
        //读取中断线状态
        motorIntState = BoardFPGA_StepMotorGetIntSrc();
    } while ((intPinState == GPIO_PIN_SET)||(motorIntState.valAll != 0));//端口高电平,或者中断线依然有效,持续运行
}

//中断配置
void BoardFPGA_StepMotorIntConfig(BoardFPGA_StepMotorIntCallBack callBack)
{
    //设置中断回调函数
    stepMotorIntCallBackFuncPtr = callBack;
    MCU_PinExtiInit(BOARD_FPGA_INT0_PIN,MCU_EXTI_MODE_IT_RISING,MCU_PIN_PULL_NOPULL,MCU_PIN_SPEED_FREQ_HIGH,
                        BOARD_FPGA_STEP_MOTOR_PREE_PRI,BOARD_FPGA_STEP_MOTOR_SUB_PRI,
                        BoardFPGA_StepMotorIntCallBackMain1_4);
    MCU_PinExtiInit(BOARD_FPGA_INT1_PIN,MCU_EXTI_MODE_IT_RISING,MCU_PIN_PULL_NOPULL,MCU_PIN_SPEED_FREQ_HIGH,
                        BOARD_FPGA_STEP_MOTOR_PREE_PRI,BOARD_FPGA_STEP_MOTOR_SUB_PRI,
                        BoardFPGA_StepMotorIntCallBackMain5_8);
    MCU_PinExtiInit(BOARD_FPGA_INT2_PIN,MCU_EXTI_MODE_IT_RISING,MCU_PIN_PULL_NOPULL,MCU_PIN_SPEED_FREQ_HIGH,
                        BOARD_FPGA_STEP_MOTOR_PREE_PRI,BOARD_FPGA_STEP_MOTOR_SUB_PRI,
                        BoardFPGA_StepMotorIntCallBackMain9_12);
    MCU_PinExtiInit(BOARD_FPGA_INT3_PIN,MCU_EXTI_MODE_IT_RISING,MCU_PIN_PULL_NOPULL,MCU_PIN_SPEED_FREQ_HIGH,
                        BOARD_FPGA_STEP_MOTOR_PREE_PRI,BOARD_FPGA_STEP_MOTOR_SUB_PRI,
                        BoardFPGA_StepMotorIntCallBackMain13_16);
}