/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-16 11:35:28
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-16 13:15:06
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#ifndef __DRIVER_CONFIG_BOARD_H_
#define __DRIVER_CONFIG_BOARD_H_
#include "DriverConfigMCU.h"
#include "DriverConfigTimer.h"
#include "DriverConfigUART.h"

/*---------------------------------------------------BOARD_SDRAM配置----------------------------------------------------------*/
//SDRAM                               
#define BOARD_SDRAM_BASE_ADDR                                   ((uint32_t)0XD0000000)
#define BOARD_SDRAM_CAPACITY                                    (8 * 1024 * 1024)
//自刷新时间,SDRAM refresh period / Number of rows）*SDRAM时钟速度 – 20
// 4096 = 行数12行4096
//      = 64ms(64000) / 4096 *80MHz - 20
//      = 1230 取值1230
#define BOATD_SDRAM_AUTO_REFLUSH_TIME_CYCLE                     1230

/*-----------------------------------------------------------板上内存管理长度-----------------------------------------*/
//SDRAM用作动态内存管理的大小
#define MEM_MANAGE_BYTE_SIZE_SDRAM                              (8 * 1024 * 1024)

/*------------------------------------主板高精度时间戳配置---------------------------------------------------*/
//主板高精度时间戳使用的定时器端口,1US分辨率
#define BOARD_HP_TIME_STAMP_TIMER                               TIM2
//主板高精度时间戳使用的定时器端口的主频        
#define BOARD_HP_TIME_STAMP_TIMER_BUS_FREQ                      TIM2_CLK_SRC_HZ
//计数器最大值,取决于定时器的计数宽度   
#define BOARD_HP_TIME_STAMP_SUB_MAX_COUNT                       UINT32_MAX
//计数时间基准的中断回调周期        
#define BOARD_HP_TIME_STAMP_PERIOD                              (BOARD_HP_TIME_STAMP_SUB_MAX_COUNT - 1)
//主板高精度时间戳定时器中断向量        
#define BOARD_HP_TIME_STAMP_TIMER_IRQ                           TIM2_IRQn
//主板高精度时间戳定时器中断向量函数        
#define BOARD_HP_TIME_STAMP_TIMER_IRQ_HANDLE                    TIM2_IRQHandler
//主板高精度时间戳定时器时钟使能        
#define BOARD_HP_TIME_STAMP_TIMER_CLK_ENABLE()                  __HAL_RCC_TIM2_CLK_ENABLE()
//主板高精度时间戳定时器时钟关闭        
#define BOARD_HP_TIME_STAMP_TIMER_CLK_DISABLE()                 __HAL_RCC_TIM2_CLK_DISABLE()
//主板高精度时间戳定时器强制复位        
#define BOARD_HP_TIME_STAMP_TIMER_FORCE_RESET()                 __HAL_RCC_TIM2_FORCE_RESET()
//主板高精度时间戳定时器取消复位        
#define BOARD_HP_TIME_STAMP_TIMER_RELEASE_RESET()               __HAL_RCC_TIM2_RELEASE_RESET()
//主板高精度时间戳定时器调试时冻结      
#define BOARD_HP_TIME_STAMP_TIMER_DEBUG_FREEZE()                __HAL_DBGMCU_FREEZE_TIM2()
//主板高精度时间戳定时器计数频率控制在1M,1US计数一次        
#define BOARD_HP_TIME_STAMP_TIM_PRESCALER                       ((BOARD_HP_TIME_STAMP_TIMER_BUS_FREQ/1000000)-1)

/*-----------------------------------------------------------板上CAN1管理---------------------------------------------*/
//CAN ID波特率设置,如果没有使用可变波特率,那么这个也是整体波特率设置
#define BOARD_CAN1_ARBITRATION_BAUD_USER                        MCU_CAN_ARBITRATION_BPS_1000K
//CAN 数据波特率设置
#define BOARD_CAN1_DATA_BAUD_USER                               MCU_CAN_DATA_BPS_5000K
//CAN1 AUX输出设置 端口             
#define PIN_BOARD_CAN1_AUX_OUT                                  MCU_PIN_H_15
//CAN1 AUX输出设置 有效电平                 
#define VALID_LEVEL_BOARD_CAN1_AUX_OUT                          GPIO_PIN_SET
//CAN1 AUX输出设置 初始化电平                 
#define INIT_LEVEL_BOARD_CAN1_AUX_OUT                           GPIO_PIN_SET

/*---------------------------------------------------板卡上RS1端口----------------------------------------------------------*/
//RS1 通讯波特率
#define BOARD_RS1_COMM_BAUD                                     115200
//RS1 单字节长度                
#define BOARD_RS1_COMM_LENGTH                                   MCU_UART_LENGTH_8B
//RS1 停止位                
#define BOARD_RS1_COMM_STOP_BIT                                 MCU_UART_STOPBIT1
//RS1 校验模式                  
#define BOARD_RS1_COMM_CHECK_MODE                               MCU_UART_CHECK_MODE_NONE
//RS1 硬件流控制                
#define BOARD_RS1_COMM_HARD_CONTROL                             MCU_UART_HARD_CONTROL_NONE

/*---------------------------------------------------板卡上RS2端口----------------------------------------------------------*/
//RS2 通讯波特率
#define BOARD_RS2_COMM_BAUD                                     115200
//RS2 单字节长度                
#define BOARD_RS2_COMM_LENGTH                                   MCU_UART_LENGTH_8B
//RS2 停止位                
#define BOARD_RS2_COMM_STOP_BIT                                 MCU_UART_STOPBIT1
//RS2 校验模式                  
#define BOARD_RS2_COMM_CHECK_MODE                               MCU_UART_CHECK_MODE_NONE
//RS2 硬件流控制                
#define BOARD_RS2_COMM_HARD_CONTROL                             MCU_UART_HARD_CONTROL_NONE

/*---------------------------------------------------板卡上RS3端口----------------------------------------------------------*/
//RS3 通讯波特率
#define BOARD_RS3_COMM_BAUD                                     115200
//RS3 单字节长度                
#define BOARD_RS3_COMM_LENGTH                                   MCU_UART_LENGTH_8B
//RS3 停止位                
#define BOARD_RS3_COMM_STOP_BIT                                 MCU_UART_STOPBIT1
//RS3 校验模式                  
#define BOARD_RS3_COMM_CHECK_MODE                               MCU_UART_CHECK_MODE_NONE
//RS3 硬件流控制                
#define BOARD_RS3_COMM_HARD_CONTROL                             MCU_UART_HARD_CONTROL_NONE


/*---------------------------------------------------板载MB85RS2MT配置----------------------------------------------------------*/
//铁电存储器的存储空间大小,也是最大地址
#define BOARD_MB85RS2MT_SIZE                                    0x40000
//铁电存储器的芯片ID
#define BOARD_MB85RS2MT_ID                                      0X00087F04
//铁电存储器的芯片ID MASK
#define BOARD_MB85RS2MT_ID_MASK                                 0x000FFFFF
//CS引脚
#define PIN_BOARD_MB85RS2MT_CS                                  MCU_PIN_E_4
//设置状态  
#define VALID_LEVEL_BOARD_MB85RS2MT_CS                          GPIO_PIN_RESET
#define INVALID_LEVEL_BOARD_MB85RS2MT_CS                        PIN_INVALID_LEVEL(VALID_LEVEL_BOARD_MB85RS2MT_CS)
//CPOL配置
#define BOARD_MB85RS2MT_SPI_CPOL                                MCU_SPI_CPOL_HIGH
//CPHA配置
#define BOARD_MB85RS2MT_SPI_CPHA                                MCU_SPI_CPHA_2EDGE
//根据SPI模式配置SPI速度
#if(MCU_SPI4_MODE_USER == MCU_SPI_MODE_INT)
//速度
#define BOARD_MB85RS2MT_SPI_SPEED                               SPI_45_BAUD_RATE_6_25_M
#else
#define BOARD_MB85RS2MT_SPI_SPEED                               SPI_45_BAUD_RATE_25M
#endif
//SPI初始化函数
#define BOARD_MB85RS2MT_SPI_PORT_INIT(speed,cpol,cpha)          MCU_SPI4_Init(speed,cpol,cpha)
//SPI操作函数
#define BOARD_MB85RS2MT_SPI_READ_WRITE(writebufferPtr,readBufferPtr,length) \
                                                                MCU_SPI4_ReadWriteBuffer(writebufferPtr,readBufferPtr,length)
//SPI缓冲区长度
#define BOARD_MB85RS2MT_BUFFER_LEN                              LENGTH_BYTES_SPI4_DMA_BUF
//写入状态寄存器等待生效的时间
#define BOARD_MB85RS2MT_WRITE_SR_RETRY_MAX                      10
//是否打开SPI Flash快速读取功能
#define BOARD_MB85RS2MT_FAST_READ                               CONFIG_FUNC_ENABLE


/*---------------------------------------------------板载IS25LP128F配置----------------------------------------------------------*/
//外置Flash的芯片ID
#define BOARD_IS25LP128F_ID                                     0X009D6018
//外置Flash的芯片ID MASK
#define BOARD_IS25LP128F_ID_MASK                                0x00FFFFFF
//CS引脚
#define PIN_BOARD_IS25LP128F_CS                                 MCU_PIN_F_6
//设置状态  
#define VALID_LEVEL_BOARD_IS25LP128F_CS                         GPIO_PIN_RESET
//CPOL配置
#define BOARD_IS25LP128F_SPI_CPOL                               MCU_SPI_CPOL_HIGH
//CPHA配置
#define BOARD_IS25LP128F_SPI_CPHA                               MCU_SPI_CPHA_2EDGE
//根据SPI模式配置SPI速度
#if(MCU_SPI5_MODE_USER == MCU_SPI_MODE_INT)
//速度
#define BOARD_IS25LP128F_SPI_SPEED                              SPI_45_BAUD_RATE_6_25_M
#else
#define BOARD_IS25LP128F_SPI_SPEED                              SPI_45_BAUD_RATE_25M
#endif
//SPI获取信号量
#define BOARD_IS25LP128F_SPI_GET_LOCK()                         MCU_SPI5_GetLock()
//SPI释放信号量
#define BOARD_IS25LP128F_SPI_RELEASE_LOCK()                     MCU_SPI5_ReleaseLock()
//SPI初始化函数
#define BOARD_IS25LP128F_SPI_PORT_INIT(speed,cpol,cpha)         MCU_SPI5_Init(speed,cpol,cpha)
//SPI操作函数
#define BOARD_IS25LP128F_SPI_READ_WRITE(writebufferPtr,readBufferPtr,length) \
                                                                MCU_SPI5_ReadWriteBuffer(writebufferPtr,readBufferPtr,length)
//SPI缓冲区长度
#define BOARD_IS25LP128F_BUFFER_LEN                             LENGTH_BYTES_SPI5_DMA_BUF
//写入状态寄存器等待生效的时间
#define BOARD_IS25LP128F_WRITE_SR_RETRY_MAX                     10
//SPIFLASH读写时间检查          
#define BOARD_IS25LP128F_OP_TIME_MEASURE_ENABLE                 0
//默认BLOCK Protet设置,与TB组合形成保护         
#define BOARD_IS25LP128F_DEFAULT_BP                             0X00
//默认TB设置            
#define BOARD_IS25LP128F_DEFAULT_TB                             0X00
//是否启用测试用例
#define BOARD_IS25LP128F_TEST_CASE_ENABLE                       CONFIG_FUNC_ENABLE
/*--------------------------------容量设定------------------------------------*/
//页大小，256字节    
#define BOARD_IS25LP128F_PAGE_SIZE                              256   
//每个扇区页数          
#define BOARD_IS25LP128F_FLASH_PAGE_PER_SECTOR                  16
//扇区大小          
#define BOARD_IS25LP128F_SECTOR_SIZE                            (BOARD_IS25LP128F_PAGE_SIZE * BOARD_IS25LP128F_FLASH_PAGE_PER_SECTOR)  
//每块里面扇区数            
#define BOARD_IS25LP128F_FLASH_SECTOR_PER_BLOCK                 16
//总块数            
#define BOARD_IS25LP128F_FLASH_BLOCK_COUNT                      256
//总的扇区数            
#define BOARD_IS25LP128F_FLASH_SECTOR_COUNT                     (BOARD_IS25LP128F_FLASH_BLOCK_COUNT*BOARD_IS25LP128F_FLASH_SECTOR_PER_BLOCK)
//每块字节数            
#define BOARD_IS25LP128F_FLASH_BLOCK_SIZE                       (BOARD_IS25LP128F_SECTOR_SIZE*BOARD_IS25LP128F_FLASH_SECTOR_PER_BLOCK)
/* Flash大小，2^24 = 16MB*/         
#define BOARD_IS25LP128F_FLASH_SIZE                             (BOARD_IS25LP128F_FLASH_BLOCK_SIZE*BOARD_IS25LP128F_FLASH_BLOCK_COUNT)   
/* 末尾地址 */ 				            
#define BOARD_IS25LP128F_END_ADDR    	                        BOARD_IS25LP128F_FLASH_SIZE 
/*--------------------------------超时时间-------------------------------------------------*/
/*指令超时时间*/      
#define BOARD_IS25LP128F_CMD_TIME_OUT                           50  
//设备复位最大延时  
#define BOARD_IS25LP128F_TIMEOUT_MS_RESET                       50
//最大页编程超时时间 最小0.4MS最大3MS   
#define BOARD_IS25LP128F_TIMEOUT_MS_MAX_PAGE_PROGRAM            10
//最大扇区擦除时间,最小50MS,最大400MS   
#define BOARD_IS25LP128F_TIMEOUT_MS_MAX_SECTOR_ERASE            400
//最大块擦除时间,32KB块,最小120MS最大1600MS 
#define BOARD_IS25LP128F_TIMEOUT_MS_MAX_BLOCK_32KB_ERASE        1600
//最大块擦除时间,64KB块,最小150MS最大2000MS 
#define BOARD_IS25LP128F_TIMEOUT_MS_MAX_BLOCK_64KB_ERASE        2000
//最大芯片擦除时间,最小80S最大400S  
#define BOARD_IS25LP128F_TIMEOUT_MS_MAX_CHIP_ERASE              (400*1000)
//读取超时时间,基础 
#define BOARD_IS25LP128F_TIMEOUT_MS_BASE_READ                   10
//读取超时时间,字节 
#define BOARD_IS25LP128F_TIMEOUT_MS_BYTE_READ                   1
//写入超时时间,基础 
#define BOARD_IS25LP128F_TIMEOUT_MS_BASE_WRITE                  10
//写入超时时间,字节 
#define BOARD_IS25LP128F_TIMEOUT_MS_BYTE_WRITE                  1

/*-------------------------------------------板卡参数管理预定义宏-------------------------------------------------------*/
#define BOARD_PARAM_INCLUDE                                     "BoardMB85RS2MT.h"
#define BOARD_PARAM_READ(addr,bufferPtr,length)                 BoardMB85RS2MT_ReadBuffer(addr,bufferPtr,length)
#define BOARD_PARAM_WRITE(addr,bufferPtr,length)                BoardMB85RS2MT_WriteBuffer(addr,bufferPtr,length)

/*-------------------------------------------板卡IAP文件存储位置-------------------------------------------------------*/
#define BOARD_IAP_FILE_INCLUDE                                  "BoardIS25LP128F.h"
#define BOARD_IAP_FILE_READ(addr,bufferPtr,length)              BoardIS25LP128F_ReadBuffer(bufferPtr,addr,length)
#define BOARD_IAP_FILE_WRITE(addr,bufferPtr,length)             BoardIS25LP128F_WriteBufferSafe(bufferPtr,addr,length)


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
/*---------------------------------------------------板卡TCA9555PWR配置----------------------------------------------------------*/
//单次字节写入传输,IIC查询时间上限              
#define TIME_MS_MAX_TCA9555PWR_WAIT_IDLE_BYTE                   100
//等待设备空闲时间          
#define TIME_MS_MAX_TCA9555PWR_WAIT_IDLE                        1000
//TCA9555PWR操作函数配置
//底层单次等待超时时间
#define BOARD_TCA9555PWR_LOW_LEVEL_SINGLE_TIMEOUT_MS            IIC1_TIME_OUT_MS
//底层端口初始化
#define BOARD_TCA9555PWR_LOW_LEVEL_PORT_INIT()                  MCU_IIC1_PortInit()
//底层查询设备是否准备好
#define BOARD_TCA9555PWR_LOW_LEVEL_CHECK_READY(addr)            MCU_IIC1_CheckDeviceReady(addr)
//底层写入端口
#define BOARD_TCA9555PWR_LOW_LEVEL_WRITE(devAddr,bitWidth,memAddr,buffPtr,byteCount,waitTimeMs) \
                                                                MCU_IIC1_WriteBytesWithAddr(devAddr,bitWidth,memAddr,buffPtr,byteCount,\
                                                                                                waitTimeMs);
//底层读取端口
#define BOARD_TCA9555PWR_LOW_LEVEL_READ(devAddr,bitWidth,memAddr,buffPtr,byteCount,waitTimeMs) \
                                                                MCU_IIC1_ReadBytesWithAddr(devAddr,bitWidth,memAddr,buffPtr,byteCount,\
                                                                                                waitTimeMs)
//扩展端口地址
#define ADDR_TCA9555_EXTEND_PORT_IN_1                           0X48
#define ADDR_TCA9555_EXTEND_PORT_OUT_1                          0X42
//等待扩展端口空闲周期
#define TIMES_WAIT_TCA9555_EXTEND_PORT                          10

#endif









