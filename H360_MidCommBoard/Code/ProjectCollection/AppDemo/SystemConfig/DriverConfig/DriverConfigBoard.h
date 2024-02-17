/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-29 11:51:45
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-30 08:49:45
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#ifndef __DRIVER_CONFIG_BOARD_H_
#define __DRIVER_CONFIG_BOARD_H_
#include "DriverConfigMCU.h"
#include "DriverConfigTimer.h"
#include "DriverConfigCAN.h"
#include "DriverConfigUart.h"

/*---------------------------------------------------BOARD_SDRAM配置----------------------------------------------------------*/
//第一块SDRAM
#define BOARD_SDRAM1_BASE_ADDR                                  ((uint32_t)0XC0000000)
#define BOARD_SDRAM1_CAPACITY                                   (32 * 1024 * 1024)
//第二块SDRAM                               
#define BOARD_SDRAM2_BASE_ADDR                                  ((uint32_t)0XD0000000)
#define BOARD_SDRAM2_CAPACITY                                   (32 * 1024 * 1024)
//自刷新时间,SDRAM refresh period / Number of rows）*SDRAM时钟速度 – 20
// 8192= 行数12行4096 双SDRAM 2*4096=8192
//      = 64ms(64000) / 8192 *100MHz - 20
//      = 761.25 取值760
#define BOATD_SDRAM_AUTO_REFLUSH_TIME_CYCLE                     760

/*-----------------------------------------------------------板上内存管理长度-----------------------------------------*/
//SDRAM1用作动态内存管理的大小
#define MEM_MANAGE_BYTE_SIZE_SDRAM1                             (30 * 1024 * 1024)
//SDRAM2用作动态内存管理的大小        
#define MEM_MANAGE_BYTE_SIZE_SDRAM2                             (32 * 1024 * 1024)

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
#define PIN_BOARD_MB85RS2MT_CS                                  MCU_PIN_I_0
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
#define BOARD_MB85RS2MT_SPI_SPEED                               SPI_123_BAUD_RATE_6_25_M
#else
#define BOARD_MB85RS2MT_SPI_SPEED                               SPI_123_BAUD_RATE_25M
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

/*-------------------------------------------板卡IAP文件存储位置-------------------------------------------------------*/
#define BOARD_IAP_FILE_INCLUDE                                  "MCU_Flash.h"
#define BOARD_IAP_FILE_READ(addr,bufferPtr,length)              MCU_FlashReadBufferBytes(addr,bufferPtr,length)
#define BOARD_IAP_FILE_WRITE(addr,bufferPtr,length)             MCU_FlashWrite(addr,bufferPtr,length)

//存放在线升级BOOT文件的起始地址
#define ADDR_BOARD_IAP_BOOT_BIN_FILE_START                      ADDR_FLASH_SECTOR_0_BANK2
#define LENGTH_BOARD_IAP_BOOT_BIN_FILE                          (256*1024)

//存放在线升级SRV文件的起始地址
#define ADDR_BOARD_IAP_SRV_BIN_FILE_START                       (ADDR_BOARD_IAP_BOOT_BIN_FILE_START+LENGTH_BOARD_IAP_BOOT_BIN_FILE)
#define LENGTH_BOARD_IAP_SRV_BIN_FILE                           (768*1024)

//存放在线升级APP文件的起始地址
#define ADDR_BOARD_IAP_APP_BIN_FILE_START                       (ADDR_BOARD_IAP_BOOT_BIN_FILE_START+LENGTH_BOARD_IAP_BOOT_BIN_FILE)
#define LENGTH_BOARD_IAP_APP_BIN_FILE                           (768*1024)


/*-----------------------------------------------------------板上CAN1管理---------------------------------------------*/
//CAN ID波特率设置,如果没有使用可变波特率,那么这个也是整体波特率设置
#define BOARD_CAN1_ARBITRATION_BAUD_USER                        MCU_CAN_ARBITRATION_BPS_1000K
//是否打开可变数据波特率
#if(MCU_CAN1_MODE_USER != MCU_CAN_MODE_FD_BRS)
//CAN 数据波特率设置
#define BOARD_CAN1_DATA_BAUD_USER                               MCU_CAN_DATA_BPS_5000K
#endif
//是否打开CAN PHY上电检测
#define BOARD_CAN1_VDD_SENSE_FUNC                               CONFIG_FUNC_ENABLE
#if(BOARD_CAN1_VDD_SENSE_FUNC == CONFIG_FUNC_ENABLE)
//CAN1 PHY上电检测 端口             
#define PIN_BOARD_CAN1_VDD_SENSE                                MCU_PIN_B_7
//CAN1 PHY上电检测 有效电平                 
#define VALID_LEVEL_BOARD_CAN1_VDD_SENSE                        GPIO_PIN_RESET
//CAN1 PHY上电检测 端口模式                 
#define PULL_MODE_BOARD_CAN1_VDD_SENSE                          MCU_PIN_PULL_PULLUP
//CAN1 PHY上电检测 端口速度                 
#define SPEED_BOARD_CAN1_VDD_SENSE                              MCU_PIN_SPEED_FREQ_MEDIUM
#endif

/*-----------------------------------------------------------板上CAN2管理---------------------------------------------*/
//CAN ID波特率设置,如果没有使用可变波特率,那么这个也是整体波特率设置
#define BOARD_CAN2_ARBITRATION_BAUD_USER                        MCU_CAN_ARBITRATION_BPS_1000K
//是否打开可变数据波特率
#if(MCU_CAN2_MODE_USER != MCU_CAN_MODE_FD_BRS)
//CAN 数据波特率设置
#define BOARD_CAN2_DATA_BAUD_USER                               MCU_CAN_DATA_BPS_5000K
#endif
//是否打开CAN PHY上电检测
#define BOARD_CAN2_VDD_SENSE_FUNC                               CONFIG_FUNC_ENABLE
#if(BOARD_CAN2_VDD_SENSE_FUNC == CONFIG_FUNC_ENABLE)
//CAN2 PHY上电检测 端口
#define PIN_BOARD_CAN2_VDD_SENSE                                MCU_PIN_B_4
//CAN2 PHY上电检测 有效电平                 
#define VALID_LEVEL_BOARD_CAN2_VDD_SENSE                        GPIO_PIN_RESET
//CAN2 PHY上电检测 端口模式                 
#define PULL_MODE_BOARD_CAN2_VDD_SENSE                          MCU_PIN_PULL_PULLUP
//CAN2 PHY上电检测 端口速度                 
#define SPEED_BOARD_CAN2_VDD_SENSE                              MCU_PIN_SPEED_FREQ_MEDIUM
#endif


/*---------------------------------------------------板卡上RS1端口----------------------------------------------------------*/
//RS1_UART6 通讯波特率
#define BOARD_RS1_UART6_BAUD                                    19200
//RS1_UART6 单字节长度             
#define BOARD_RS1_UART6_LENGTH                                  MCU_UART_LENGTH_8B
//RS1_UART6 停止位             
#define BOARD_RS1_UART6_STOP_BIT                                MCU_UART_STOPBIT1
//RS1_UART6 校验模式               
#define BOARD_RS1_UART6_CHECK_MODE                              MCU_UART_CHECK_MODE_NONE
//RS1_UART6 硬件流控制             
#define BOARD_RS1_UART6_HARD_CONTROL                            MCU_UART_HARD_CONTROL_NONE

/*---------------------------------------------------板卡上RS2端口----------------------------------------------------------*/
//RS2_UART1 通讯波特率
#define BOARD_RS2_UART1_BAUD                                    115200
//RS2_UART1 单字节长度             
#define BOARD_RS2_UART1_LENGTH                                  MCU_UART_LENGTH_8B
//RS2_UART1 停止位             
#define BOARD_RS2_UART1_STOP_BIT                                MCU_UART_STOPBIT1
//RS2_UART1 校验模式               
#define BOARD_RS2_UART1_CHECK_MODE                              MCU_UART_CHECK_MODE_NONE
//RS2_UART1 硬件流控制             
#define BOARD_RS2_UART1_HARD_CONTROL                            MCU_UART_HARD_CONTROL_NONE


/*-------------------------------------------------------SDCARD配置----------------------------------------------------*/
//端口配置                                
#define PIN_BOARD_SDIO_D0                                       MCU_PIN_C_8
#define PIN_BOARD_SDIO_D1                                       MCU_PIN_C_9
#define PIN_BOARD_SDIO_D2                                       MCU_PIN_C_10
#define PIN_BOARD_SDIO_D3                                       MCU_PIN_C_11
#define PIN_BOARD_SDIO_CK                                       MCU_PIN_C_12
#define PIN_BOARD_SDIO_CMD                                      MCU_PIN_D_2                  
//SD卡是否启用DMA模式,DMA模式下,不需要CPU干涉数据传输,解放CPU,但是在缓冲区小的情况下
//需要执行数据的拷贝,可能性能还不如中断模式.中断模式下,会频繁触发SDMMC中断,具体怎么用,看使用者取舍
//SD接口的DMA只支持AXI_SRAM访问,所以如果加载文件系统的时候,文件系统缓存在别的RAM分区,就需要使用缓存中转
#define BOARD_SD_ENABLE_DMA                                     CONFIG_FUNC_ENABLE
//定义SD卡是否使用PLL2 R时钟,200MHZ
#define BOARD_SD_CLK_USE_PLL2R                                  CONFIG_FUNC_DISABLE
//SD卡读写时间检查开关
#define BOARD_SD_OP_TIME_MEASURE_ENABLE                         CONFIG_FUNC_DISABLE
//SD卡通讯超时时间,单个块时间,如果同时多个块,驱动自动加倍
#define BOARD_SD_TIME_OUT_MS                                    200
//SD卡等待空闲超时时间
#define BOARD_SD_WAIT_IDLE_TIME_OUT                             1000
//中断事件打印是否开启
#define BOARD_SD_INT_SHOW                                       CONFIG_FUNC_DISABLE
//SDCARD扇区大小
#define BOARD_SD_CARD_SECTOR_BYTE_SIZE                          512
//DMA传输使用的缓冲区大小,有多少个扇区,使能DMA的情况下才有用
#define BOARD_SD_DMA_BUFFER_SECTOR_COUNT                        16

/*---------------------------------------------------BOARD_LAN8700配置----------------------------------------------------------*/
//PHY地址
#define BOARD_LAN8700_PHY_ADDR                                  0
//芯片超时时间配置
//软件复位超时时间
#define BOARD_LAN8700_SW_RESET_TIME_OUT_MS                      ((uint32_t)500U)
//初始化超时时间
#define BOARD_LAN8700_INIT_TIME_OUT_MS                          ((uint32_t)2000U)
//PHY设备ID
#define BOARD_LAN8700_PHY_ID_A                                  (0X0007C0C4)
//PHY设备ID
#define BOARD_LAN8700_PHY_ID_B                                  (0X0007C0C3)
//自协商超时时间
#define BOARD_LAN8700_AUTO_NEGO_TIME_OUT_MS                     ((uint32_t)2000U)
//LAN8700复位引脚
#define BOARD_LAN8700_PIN_RESET                                 MCU_PIN_D_3
//LAN8700复位引脚有效电平
#define VALID_LEVEL_BOARD_LAN8700_PIN_RESET                     GPIO_PIN_RESET
//LAN8700数据中断引脚
#define BOARD_LAN8700_PIN_INT                                   MCU_PIN_G_11


/*---------------------------------------------------BOARD_W5500配置----------------------------------------------------------*/
//缓存长度定义
#define BOARD_W5500_SOCKET_BUF_SIZE_0KB                         0
#define BOARD_W5500_SOCKET_BUF_SIZE_1KB                         1
#define BOARD_W5500_SOCKET_BUF_SIZE_2KB                         2
#define BOARD_W5500_SOCKET_BUF_SIZE_4KB                         4
#define BOARD_W5500_SOCKET_BUF_SIZE_8KB                         8
#define BOARD_W5500_SOCKET_BUF_SIZE_16KB                        16
//版本代码
#define BOARD_W5500_VERSION_CODE                                0X04
//根据SPI模式配置SPI速度
#if(MCU_SPI1_MODE_USER == MCU_SPI_MODE_INT)
//速度
#define BOARD_W5500_SPI_SPEED                                   SPI_45_BAUD_RATE_6_25_M
#else
#define BOARD_W5500_SPI_SPEED                                   SPI_45_BAUD_RATE_50M
#endif
//SPI CPOL
#define BOARD_W5500_SPI_CPOL                                    MCU_SPI_CPOL_HIGH
//SPI CPHA
#define BOARD_W5500_SPI_CPHA                                    MCU_SPI_CPHA_2EDGE
//SPI获取信号量
#define BOARD_W5500_SPI_GET_LOCK()                              MCU_SPI5_GetLock()
//SPI释放信号量
#define BOARD_W5500_SPI_RELEASE_LOCK()                          MCU_SPI5_ReleaseLock()
//SPI初始化函数
#define BOARD_W5500_SPI_PORT_INIT(speed,cpol,cpha)              MCU_SPI5_Init(speed,cpol,cpha)
//SPI操作函数
#define BOARD_W5500_SPI_READ_WRITE(writebufferPtr,readBufferPtr,length) \
                                                                MCU_SPI5_ReadWriteBuffer(writebufferPtr,readBufferPtr,length)
//BOARD_W5500数据发送超时时间
#define BOARD_W5500_SEND_TIME_OUT_MAX                           200
//选中端口
#define BOARD_W5500_PIN_SELECT                                  MCU_PIN_F_6
//选中端口有效电平
#define VALID_LEVEL_PIN_BOARD_W5500_SELECT                      GPIO_PIN_RESET
//复位端口空选项
#define BOARD_W5500_PIN_RESET_NULL                              0XFFFFFFFF
//复位端口
#define BOARD_W5500_PIN_RESET                                   BOARD_W5500_PIN_RESET_NULL
//复位端口有效电平
#define VALID_LEVEL_PIN_BOARD_W5500_RESET                       GPIO_PIN_RESET
//中断输入引脚
#define BOARD_W5500_PIN_INT                                     MCU_PIN_F_10
//BOARD_W5500使用的内存空间
#define BOARD_W5500_USE_MEM_REGION                              MEM_AXI_SRAM
//BOARD_W5500 MAC地址定义
#define BOARD_W5500_MAC_ADDR_0                                  0X90
#define BOARD_W5500_MAC_ADDR_1                                  0X05
#define BOARD_W5500_MAC_ADDR_2                                  0X25
#define BOARD_W5500_MAC_ADDR_3                                  0X89
#define BOARD_W5500_MAC_ADDR_4                                  0X12
#define BOARD_W5500_MAC_ADDR_5                                  0X21
//BOARD_W5500 子网掩码配置
#define BOARD_W5500_SUB_NET_MASK1                               255
#define BOARD_W5500_SUB_NET_MASK2                               255
#define BOARD_W5500_SUB_NET_MASK3                               255
#define BOARD_W5500_SUB_NET_MASK4                               0
//应用中发送缓存定义
#define BOARD_W5500_SOCKET_0_TX_BUF_SIZE                        BOARD_W5500_SOCKET_BUF_SIZE_16KB
//应用中接收缓存定义                                
#define BOARD_W5500_SOCKET_0_RX_BUF_SIZE                        BOARD_W5500_SOCKET_BUF_SIZE_16KB

#endif








