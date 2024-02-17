/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-23 09:44:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-31 15:37:50 +0800
************************************************************************************************/ 
#ifndef __DRIVER_CONFIG_BOARD_H_
#define __DRIVER_CONFIG_BOARD_H_
#include "DriverConfigMCU.h"


/*---------------------------------------------------板卡上RS1端口----------------------------------------------------------*/
//RS1 通讯波特率
#define BOARD_RS1_BAUD                                          115200
//RS1 单字节长度                
#define BOARD_RS1_LENGTH                                        MCU_UART_LENGTH_8B
//RS1 停止位                
#define BOARD_RS1_STOP_BIT                                      MCU_UART_STOPBIT1
//RS1 校验模式                  
#define BOARD_RS1_CHECK_MODE                                    MCU_UART_CHECK_MODE_NONE
//RS1 硬件流控制                
#define BOARD_RS1_HARD_CONTROL                                  MCU_UART_HARD_CONTROL_NONE

/*---------------------------------------------------板卡上RS2端口----------------------------------------------------------*/
//RS2 通讯波特率
#define BOARD_RS2_BAUD                                          115200
//RS2 单字节长度                
#define BOARD_RS2_LENGTH                                        MCU_UART_LENGTH_8B
//RS2 停止位                
#define BOARD_RS2_STOP_BIT                                      MCU_UART_STOPBIT1
//RS2 校验模式                  
#define BOARD_RS2_CHECK_MODE                                    MCU_UART_CHECK_MODE_NONE
//RS2 硬件流控制                
#define BOARD_RS2_HARD_CONTROL                                  MCU_UART_HARD_CONTROL_NONE


/*---------------------------------------------------板载MB85RS2MT配置----------------------------------------------------------*/
//铁电存储器的存储空间大小,也是最大地址
#define BOARD_MB85RS2MT_SIZE                                    0x40000
//铁电存储器的芯片ID
#define BOARD_MB85RS2MT_ID                                      0X00087F04
//铁电存储器的芯片ID MASK
#define BOARD_MB85RS2MT_ID_MASK                                 0x000FFFFF
//CS引脚
#define PIN_BOARD_MB85RS2MT_CS                                  MCU_PIN_A_15
//设置状态  
#define VALID_LEVEL_BOARD_MB85RS2MT_CS                          GPIO_PIN_RESET
#define INVALID_LEVEL_BOARD_MB85RS2MT_CS                        PIN_INVALID_LEVEL(VALID_LEVEL_BOARD_MB85RS2MT_CS)
//CPOL配置
#define BOARD_MB85RS2MT_SPI_CPOL                                MCU_SPI_CPOL_HIGH
//CPHA配置
#define BOARD_MB85RS2MT_SPI_CPHA                                MCU_SPI_CPHA_2EDGE
//根据SPI模式配置SPI速度
#if(MCU_SPI3_MODE_USER == MCU_SPI_MODE_INT)
//速度
#define BOARD_MB85RS2MT_SPI_SPEED                               SPI23_BAUD_RATE_5_25M
#else
#define BOARD_MB85RS2MT_SPI_SPEED                               SPI23_BAUD_RATE_21M
#endif
//SPI初始化函数
#define BOARD_MB85RS2MT_SPI_PORT_INIT(speed,cpol,cpha)          MCU_SPI3_Init(speed,cpol,cpha)
//SPI操作函数
#define BOARD_MB85RS2MT_SPI_READ_WRITE(writebufferPtr,readBufferPtr,length) \
                                                                MCU_SPI3_ReadWriteBuffer(writebufferPtr,readBufferPtr,length)
//SPI缓冲区长度
#define BOARD_MB85RS2MT_BUFFER_LEN                              LENGTH_BYTES_SPI3_DMA_BUF
//写入状态寄存器等待生效的时间
#define BOARD_MB85RS2MT_WRITE_SR_RETRY_MAX                      10
//是否打开SPI Flash快速读取功能
#define BOARD_MB85RS2MT_FAST_READ                               CONFIG_FUNC_ENABLE

/*-------------------------------------------板卡参数管理预定义宏-------------------------------------------------------*/
#define BOARD_PARAM_INCLUDE                                     "BoardMB85RS2MT.h"
#define BOARD_PARAM_READ(addr,bufferPtr,length)                 BoardMB85RS2MT_ReadBuffer(addr,bufferPtr,length)
#define BOARD_PARAM_WRITE(addr,bufferPtr,length)                BoardMB85RS2MT_WriteBuffer(addr,bufferPtr,length)

/*-------------------------------------------板上报警输出-------------------------------------------------------*/
//红灯
#define VALID_LEVEL_BOARD_ALARM_LED_RED                         GPIO_PIN_SET
//绿灯
#define VALID_LEVEL_BOARD_ALARM_GREEN_RED                       GPIO_PIN_SET
//黄灯
#define VALID_LEVEL_BOARD_ALARM_YELLOW_RED                      GPIO_PIN_SET
//喇叭报警
#define VALID_LEVEL_BOARD_ALARM_BUZZER_RED                      GPIO_PIN_SET
//AU外部接口报警
#define VALID_LEVEL_BOARD_ALARM_AU_RED                          GPIO_PIN_SET

/*-----------------------------------------------------------板上CAN1管理---------------------------------------------*/
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
//是否打开CAN PHY上电检测
#define BOARD_CAN2_VDD_SENSE_FUNC                               CONFIG_FUNC_DISABLE
#if(BOARD_CAN2_VDD_SENSE_FUNC == CONFIG_FUNC_ENABLE)
//CAN2 PHY上电检测 端口
#define PIN_BOARD_CAN2_VDD_SENSE                                MCU_PIN_B_8
//CAN2 PHY上电检测 有效电平                 
#define VALID_LEVEL_BOARD_CAN2_VDD_SENSE                        GPIO_PIN_RESET
//CAN2 PHY上电检测 端口模式                 
#define PULL_MODE_BOARD_CAN2_VDD_SENSE                          MCU_PIN_PULL_PULLUP
//CAN2 PHY上电检测 端口速度                 
#define SPEED_BOARD_CAN2_VDD_SENSE                              MCU_PIN_SPEED_FREQ_MEDIUM
#endif


#endif



