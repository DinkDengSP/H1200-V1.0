/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-21 10:49:58 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-10 14:11:57 +0800
************************************************************************************************/ 
#ifndef __DRIVER_CONFIG_BOARD_H_
#define __DRIVER_CONFIG_BOARD_H_
#include "DriverConfigMCU.h"
#include "DriverConfigCan.h"
#include "DriverConfigUART.h"
#include "DriverConfigTimer.h"

/*---------------------------------------------------板卡SRAM配置----------------------------------------------------------*/
/**ISWV51216 512*16/2 1M字节*/
//对IS61LV25616/IS62WV25616,地址线范围为A0~A17 
//对IS61LV51216/IS62WV51216,地址线范围为A0~A18
//使用第四块 11 ->C
#define BOARD_SRAM_BASE_ADDR                                    ((uint32_t)(0x6C000000))
//1M容量    
#define BOARD_SRAM_CAPACITY                                     (1*1024*1024)
//地址建立时间  
#define BOARD_SRAM_ADDRESS_SETUP_CYCLE                          0X00
//数据保存时间  
#define BOARD_SRAM_DATA_SETUP_CYCLE                             0X08

/*---------------------------------------------------板卡内存管理----------------------------------------------------------*/
//板卡内存管理长度
#define MEM_MANAGE_BYTE_SIZE_SRAM_EXT                           1*1024*1024

/*---------------------------------------------------板卡CPLD配置管理----------------------------------------------------------*/
//电机最低转速,单位 转每秒 不得低于该转速
#define BOARD_CPLD_MOTOR_SPEED_MIN_RPM                          (57.3F)
//电机最高转速,不得高于该转速               
#define BOARD_CPLD_MOTOR_SPEED_MAX_RPM                          (16160.0F)
//CPLD挂载在FSMC的地址线,NE1,NCE2 [27:26] = 00              
#define BOARD_CPLD_ADDR_BASE                                    (0X60000000)
//生成状态用来存放锁中断变量                
#define BOARD_CPLD_THREAD_LOCK_VAR_ALLOC()                      uint32_t lastIntState;
//步进电机写入数据的时候不能发生中断,中断锁定               
#define BOARD_CPLD_THREAD_LOCK()                                lastIntState = CoreLockIntGlobal();
//中断释放              
#define BOARD_CPLD_THREAD_UNLOCK()                              CoreUnlockIntGlobal(lastIntState)
//使能信号有效电平反转
#define BOARD_BOARD_CPLD_MOTOR_ENABLE_REVERSE                   CONFIG_FUNC_ENABLE
//地址建立时间
#define BOARD_CPLD_ADDRESS_SETUP_CYCLE                          0X01
//数据保存时间  
#define BOARD_CPLD_DATA_SETUP_CYCLE                             0X08
//端口定义  
//复位端口  
#define PIN_BOARD_CPLD_RESET                                    MCU_PIN_F_11
//复位端口有效电平  
#define VALID_LEVEL_PIN_BOARD_CPLD_RESET                        GPIO_PIN_RESET
//INT0  
#define PIN_BOARD_CPLD_INT0                                     MCU_PIN_C_12
//INT1  
#define PIN_BOARD_CPLD_INT1                                     MCU_PIN_D_2
//INT1中断电平  
#define VALID_LEVEL_PIN_BOARD_CPLD_INT1                         GPIO_PIN_RESET
//中断模式  
#define INT_MODE_PIN_BOARD_CPLD_INT1                            MCU_EXTI_MODE_IT_FALLING
//INT2  
#define PIN_BOARD_CPLD_INT2                                     MCU_PIN_G_6
//INT3  
#define PIN_BOARD_CPLD_INT3                                     MCU_PIN_G_7
//CLK   
#define PIN_BOARD_CPLD_CLK                                      MCU_PIN_D_3
//步进电机上电使能间隔  
#define BOARD_CPLD_MOTOR_ENABLE_INTERVAL_MS                     100
//步进电机掉电间隔时间  
#define BOARD_CPLD_MOTOR_DISABLE_INTERVAL_MS                    100
/*---------------------------------------------------板卡AT24CXX配置----------------------------------------------------------*/
//设备地址
#define ADDR_DEVICE_AT24CXX_USE                                 0xA0
//板卡上的芯片字节容量              
#define BYTE_SIZE_AT24CXX_USE                                   BYTE_SIZE_AT24C256
//板卡上的芯片页字节容量                    
#define PAGE_SIZE_AT24CXX_USE                                   PAGE_SIZE_AT24C256
//单次字节写入传输,IIC查询时间上限              
#define TIME_MS_MAX_AT24CXX_WAIT_IDLE_BYTE                      100
//等待设备空闲时间          
#define TIME_MS_MAX_AT24CXX_WAIT_IDLE                           1000
//是否打开芯片擦除开关          
#define BOARD_AT24CXX_ERASE_CHIP_ENABLE                         CONFIG_FUNC_ENABLE
//擦除默认值                
#define BOARD_AT24CXX_ERASE_CHIP_INIT_VAL                       0XFF
//AT24CXX操作函数配置
//底层单次等待超时时间
#define BOARD_AT24CXX_LOW_LEVEL_SINGLE_TIMEOUT_MS               IIC2_TIME_OUT_MS
//底层端口初始化
#define BOARD_AT24CXX_LOW_LEVEL_PORT_INIT()                     MCU_IIC2_PortInit()
//底层查询设备是否准备好
#define BOARD_AT24CXX_LOW_LEVEL_CHECK_READY(addr)               MCU_IIC2_CheckDeviceReady(addr)
//底层写入端口
#define BOARD_AT24CXX_LOW_LEVEL_WRITE(devAddr,bitWidth,memAddr,buffPtr,byteCount,waitTimeMs) \
                                                                MCU_IIC2_WriteBytesWithAddr(devAddr,bitWidth,memAddr,buffPtr,byteCount,\
                                                                                                waitTimeMs);
//底层读取端口
#define BOARD_AT24CXX_LOW_LEVEL_READ(devAddr,bitWidth,memAddr,buffPtr,byteCount,waitTimeMs) \
                                                                MCU_IIC2_ReadBytesWithAddr(devAddr,bitWidth,memAddr,buffPtr,byteCount,\
                                                                                                waitTimeMs)
/*---------------------------------------------------板载MB85RS2MT配置----------------------------------------------------------*/
//铁电存储器的存储空间大小,也是最大地址
#define BOARD_MB85RS2MT_SIZE                                    0x40000
//铁电存储器的芯片ID
#define BOARD_MB85RS2MT_ID                                      0X00087F04
//铁电存储器的芯片ID MASK
#define BOARD_MB85RS2MT_ID_MASK                                 0x000FFFFF
//CS引脚
#define PIN_BOARD_MB85RS2MT_CS                                  MCU_PIN_B_12
//设置状态  
#define VALID_LEVEL_BOARD_MB85RS2MT_CS                          GPIO_PIN_RESET
#define INVALID_LEVEL_BOARD_MB85RS2MT_CS                        PIN_INVALID_LEVEL(VALID_LEVEL_BOARD_MB85RS2MT_CS)
//CPOL配置
#define BOARD_MB85RS2MT_SPI_CPOL                                MCU_SPI_CPOL_HIGH
//CPHA配置
#define BOARD_MB85RS2MT_SPI_CPHA                                MCU_SPI_CPHA_2EDGE
//根据SPI模式配置SPI速度
#if(MCU_SPI2_MODE_USER == MCU_SPI_MODE_INT)
//速度
#define BOARD_MB85RS2MT_SPI_SPEED                               SPI23_BAUD_RATE_5_25M
#else
#define BOARD_MB85RS2MT_SPI_SPEED                               SPI23_BAUD_RATE_21M
#endif
//SPI初始化函数
#define BOARD_MB85RS2MT_SPI_PORT_INIT(speed,cpol,cpha)          MCU_SPI2_Init(speed,cpol,cpha)
//SPI操作函数
#define BOARD_MB85RS2MT_SPI_READ_WRITE(writebufferPtr,readBufferPtr,length) \
                                                                MCU_SPI2_ReadWriteBuffer(writebufferPtr,readBufferPtr,length)
//SPI缓冲区长度
#define BOARD_MB85RS2MT_BUFFER_LEN                              LENGTH_BYTES_SPI2_DMA_BUF
//写入状态寄存器等待生效的时间
#define BOARD_MB85RS2MT_WRITE_SR_RETRY_MAX                      10
//是否打开SPI Flash快速读取功能
#define BOARD_MB85RS2MT_FAST_READ                               CONFIG_FUNC_ENABLE
/*-------------------------------------------板卡参数管理预定义宏-------------------------------------------------------*/
#define BOARD_PARAM_INCLUDE                                     "BoardMB85RS2MT.h"
#define BOARD_PARAM_READ(addr,bufferPtr,length)                 BoardMB85RS2MT_ReadBuffer(addr,bufferPtr,length)
#define BOARD_PARAM_WRITE(addr,bufferPtr,length)                BoardMB85RS2MT_WriteBuffer(addr,bufferPtr,length)

/*-----------------------------------------------------------板上CAN1管理---------------------------------------------*/
//是否打开CAN PHY上电检测
#define BOARD_CAN1_VDD_SENSE_FUNC                               CONFIG_FUNC_ENABLE
#if(BOARD_CAN1_VDD_SENSE_FUNC == CONFIG_FUNC_ENABLE)
//CAN1 PHY上电检测 端口             
#define PIN_BOARD_CAN1_VDD_SENSE                                MCU_PIN_A_8
//CAN1 PHY上电检测 有效电平                 
#define VALID_LEVEL_BOARD_CAN1_VDD_SENSE                        GPIO_PIN_RESET
//CAN1 PHY上电检测 端口模式                 
#define PULL_MODE_BOARD_CAN1_VDD_SENSE                          MCU_PIN_PULL_PULLUP
//CAN1 PHY上电检测 端口速度                 
#define SPEED_BOARD_CAN1_VDD_SENSE                              MCU_PIN_SPEED_FREQ_MEDIUM
#endif

/*---------------------------------------------------板卡上RS232A端口----------------------------------------------------------*/
//RS232A 通讯波特率
#define BOARD_RS232A_BAUD                                       115200
//RS232A 单字节长度                
#define BOARD_RS232A_LENGTH                                     MCU_UART_LENGTH_8B
//RS232A 停止位                
#define BOARD_RS232A_STOP_BIT                                   MCU_UART_STOPBIT1
//RS232A 校验模式                  
#define BOARD_RS232A_CHECK_MODE                                 MCU_UART_CHECK_MODE_NONE
//RS232A 硬件流控制                
#define BOARD_RS232A_HARD_CONTROL                               MCU_UART_HARD_CONTROL_NONE

/*---------------------------------------------------板卡上RS232B端口----------------------------------------------------------*/
//RS232B 通讯波特率
#define BOARD_RS232B_BAUD                                       115200
//RS232B 单字节长度                
#define BOARD_RS232B_LENGTH                                     MCU_UART_LENGTH_8B
//RS232B 停止位                
#define BOARD_RS232B_STOP_BIT                                   MCU_UART_STOPBIT1
//RS232B 校验模式                  
#define BOARD_RS232B_CHECK_MODE                                 MCU_UART_CHECK_MODE_NONE
//RS232B 硬件流控制                
#define BOARD_RS232B_HARD_CONTROL                               MCU_UART_HARD_CONTROL_NONE

/*---------------------------------------------------板卡PMT配置----------------------------------------------------------*/
//PMT_CH345 通讯波特率
#define BOARD_PMT_CH345_BAUD                                    19200
//PMT_CH345 单字节长度             
#define BOARD_PMT_CH345_LENGTH                                  MCU_UART_LENGTH_8B
//PMT_CH345 停止位             
#define BOARD_PMT_CH345_STOP_BIT                                MCU_UART_STOPBIT1
//PMT_CH345 校验模式               
#define BOARD_PMT_CH345_CHECK_MODE                              MCU_UART_CHECK_MODE_NONE
//PMT_CH345 硬件流控制             
#define BOARD_PMT_CH345_HARD_CONTROL                            MCU_UART_HARD_CONTROL_NONE
//PMT接收消息缓冲区,最好和MCU串口接收数据块保持一致
#define LENGTH_QUEUE_BOARD_PMT_CH345_RECV_MSG                   COUNT_UART1_RECV_BLOCK
//PMT消息处理循环
#define PERIOD_MS_PROC_BOARD_PMT_CH345_RECV_MSG                 20
//PMT使用的内存申请函数
#define MEM_MALLOC_BOARD_PMT_CH345(size)                        MCU_MemMallocWhileSuccessSramIn(size)
#define MEM_MALLOC_BOARD_PMT_CH345_PTR                          MCU_MemMallocWhileSuccessSramIn
//PMT使用的内存释放函数
#define MEM_FREE_BOARD_PMT_CH345(ptr)                           MCU_MemFreeSramIn(ptr)
#define MEM_FREE_BOARD_PMT_CH345_PTR                            MCU_MemFreeSramIn
//通讯缓存数据长度
#define COMM_CACHE_LENGTH_BOARD_PMT_CH345                       20
//通讯异常的重试次数
#define RETRY_MAX_COMM_BOARD_PMT_CH345                          3
//默认脉冲分辨时间
#define UNIT_TIME_NS_PLUSE_BOARD_PMT_CH345                      17
//PMT最大能保存的测光点数量
#define MAX_COUNT_BOARD_PMT_CH345_MEASURE_LIGHT_POINT           200
//PMT串口数据
#define COMM_PACK_SRC_BOARD_PMT_CH345                           UART_PACK_SRC_1
//PMT使用的通讯端口初始化
#define COMM_INIT_BOARD_PMT_CH345(baud,length,stopbit,checkMode,hardControl)        \
                                                                MCU_UART1_Init(baud,length,stopbit,checkMode,hardControl)
//PMT方法注册
#define COMM_CALLBACL_REGISTER_BOARD_PMT_CH345(recvIntCallBack,beforeSendCallBack,afterSendCallBack)    \
                                                                MCU_UART1_ConfigCallBack(recvIntCallBack,beforeSendCallBack,afterSendCallBack)
//处理完成的数据返回给底层驱动
#define RELEASE_PACK_BOARD_PMT_CH345(packPtr)                   MCU_UART1_RecvDatRelease(packPtr)
//发送指令
#define COMM_SEND_CMD_STRING_BOARD_PMT_CH345(str)               MCU_UART1_SendString(str)

#endif






















