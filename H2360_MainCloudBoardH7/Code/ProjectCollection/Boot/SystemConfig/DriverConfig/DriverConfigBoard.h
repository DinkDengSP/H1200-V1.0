/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-03 21:10:26 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-07 09:50:30 +0800
************************************************************************************************/ 
#ifndef __DRIVER_CONFIG_BOARD_H_
#define __DRIVER_CONFIG_BOARD_H_
#include "DriverConfigMCU.h"
#include "DriverConfigTimer.h"
#include "DriverConfigUART.h"

/*---------------------------------------------------BOARD_SDRAM配置----------------------------------------------------------*/
/**ISWV51216 512*16/2 1M字节*/
//对IS61LV25616/SRAMWV25616,地址线范围为A0~A17 
//对IS61LV51216/SRAMWV51216,地址线范围为A0~A18
//使用第四块 11 ->C
#define BOARD_SRAM_BASE_ADDR                                    ((uint32_t)(0x68000000))
//1M容量
#define BOARD_SRAM_CAPACITY                                     (1*1024*1024)

/*-----------------------------------------------------------板上内存管理长度-----------------------------------------*/
//SRAM用作动态内存管理的大小
#define MEM_MANAGE_BYTE_SIZE_SRAM                               (768 * 1024)

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

/*---------------------------------------------------板卡上RS1端口----------------------------------------------------------*/
//RS1_UART1 通讯波特率
#define BOARD_RS1_UART1_BAUD                                    115200
//RS1_UART1 单字节长度             
#define BOARD_RS1_UART1_LENGTH                                  MCU_UART_LENGTH_8B
//RS1_UART1 停止位             
#define BOARD_RS1_UART1_STOP_BIT                                MCU_UART_STOPBIT1
//RS1_UART1 校验模式               
#define BOARD_RS1_UART1_CHECK_MODE                              MCU_UART_CHECK_MODE_NONE
//RS1_UART1 硬件流控制             
#define BOARD_RS1_UART1_HARD_CONTROL                            MCU_UART_HARD_CONTROL_NONE

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
#define PIN_BOARD_CAN1_VDD_SENSE                                MCU_PIN_A_15
//CAN1 PHY上电检测 有效电平                 
#define VALID_LEVEL_BOARD_CAN1_VDD_SENSE                        GPIO_PIN_RESET
//CAN1 PHY上电检测 端口模式                 
#define PULL_MODE_BOARD_CAN1_VDD_SENSE                          MCU_PIN_PULL_PULLUP
//CAN1 PHY上电检测 端口速度                 
#define SPEED_BOARD_CAN1_VDD_SENSE                              MCU_PIN_SPEED_FREQ_MEDIUM
#endif

/*---------------------------------------------------板载MB85RS2MT配置----------------------------------------------------------*/
//铁电存储器的存储空间大小,也是最大地址
#define BOARD_MB85RS2MT_SIZE                                    0x40000
//铁电存储器的芯片ID
#define BOARD_MB85RS2MT_ID                                      0X00087F04
//铁电存储器的芯片ID MASK
#define BOARD_MB85RS2MT_ID_MASK                                 0x000FFFFF
//CS引脚
#define PIN_BOARD_MB85RS2MT_CS                                  MCU_PIN_F_6
//设置状态  
#define VALID_LEVEL_BOARD_MB85RS2MT_CS                          GPIO_PIN_RESET
#define INVALID_LEVEL_BOARD_MB85RS2MT_CS                        PIN_INVALID_LEVEL(VALID_LEVEL_BOARD_MB85RS2MT_CS)
//CPOL配置
#define BOARD_MB85RS2MT_SPI_CPOL                                MCU_SPI_CPOL_HIGH
//CPHA配置
#define BOARD_MB85RS2MT_SPI_CPHA                                MCU_SPI_CPHA_2EDGE
//根据SPI模式配置SPI速度
#if(MCU_SPI5_MODE_USER == MCU_SPI_MODE_INT)
//速度
#define BOARD_MB85RS2MT_SPI_SPEED                               SPI_45_BAUD_RATE_6_25_M
#else
#define BOARD_MB85RS2MT_SPI_SPEED                               SPI_45_BAUD_RATE_25M
#endif
//SPI初始化函数
#define BOARD_MB85RS2MT_SPI_PORT_INIT(speed,cpol,cpha)          MCU_SPI5_Init(speed,cpol,cpha)
//SPI操作函数
#define BOARD_MB85RS2MT_SPI_READ_WRITE(writebufferPtr,readBufferPtr,length) \
                                                                MCU_SPI5_ReadWriteBuffer(writebufferPtr,readBufferPtr,length)
//SPI缓冲区长度
#define BOARD_MB85RS2MT_BUFFER_LEN                              LENGTH_BYTES_SPI5_DMA_BUF
//写入状态寄存器等待生效的时间
#define BOARD_MB85RS2MT_WRITE_SR_RETRY_MAX                      10
//是否打开SPI Flash快速读取功能
#define BOARD_MB85RS2MT_FAST_READ                               CONFIG_FUNC_ENABLE

/*-------------------------------------------板卡参数管理预定义宏-------------------------------------------------------*/
#define BOARD_PARAM_INCLUDE                                     "BoardMB85RS2MT.h"
#define BOARD_PARAM_READ(addr,bufferPtr,length)                 BoardMB85RS2MT_ReadBuffer(addr,bufferPtr,length)
#define BOARD_PARAM_WRITE(addr,bufferPtr,length)                BoardMB85RS2MT_WriteBuffer(addr,bufferPtr,length)
/*-------------------------------------------板卡IAP文件存储位置-------------------------------------------------------*/
#define BOARD_IAP_FILE_INCLUDE                                  "MCU_Flash.h"
#define BOARD_IAP_FILE_READ(addr,bufferPtr,length)              MCU_FlashReadBufferBytes(addr,bufferPtr,length)
#define BOARD_IAP_FILE_WRITE(addr,bufferPtr,length)             MCU_FlashWrite(addr,bufferPtr,length)


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



#endif



















