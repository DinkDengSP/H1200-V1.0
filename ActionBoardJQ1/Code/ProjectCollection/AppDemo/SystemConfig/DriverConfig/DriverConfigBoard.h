/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-21 10:49:58 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-09-27 10:00:46 +0800
************************************************************************************************/ 
#ifndef __DRIVER_CONFIG_BOARD_H_
#define __DRIVER_CONFIG_BOARD_H_
#include "DriverConfigMCU.h"
#include "DriverConfigCan.h"
#include "DriverConfigUART.h"
#include "DriverConfigTimer.h"

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

/*---------------------------------------------------板卡上RS2端口----------------------------------------------------------*/
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

/*---------------------------------------------------板卡上RS485端口----------------------------------------------------------*/
//RS485 通讯波特率
#define BOARD_RS485_BAUD                                        115200
//RS485 单字节长度                  
#define BOARD_RS485_LENGTH                                      MCU_UART_LENGTH_8B
//RS485 停止位                  
#define BOARD_RS485_STOP_BIT                                    MCU_UART_STOPBIT1
//RS485 校验模式                    
#define BOARD_RS485_CHECK_MODE                                  MCU_UART_CHECK_MODE_NONE

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
#define VALID_LEVEL_BOARD_ALARM_BUZZER                          GPIO_PIN_SET

/*-------------------------------------------板上DCVALVE输出-------------------------------------------------------*/
//dcvalve1
#define VALID_LEVEL_BOARD_DC_VALVE1                             GPIO_PIN_RESET
//dcvalve2
#define VALID_LEVEL_BOARD_DC_VALVE2                             GPIO_PIN_RESET
/*-------------------------------------------板上DCMOTOR输出-------------------------------------------------------*/
//DC MOTOR1
#define VALID_LEVEL_BOARD_DC_MOTOR1                             GPIO_PIN_SET
//DC MOTOR2
#define VALID_LEVEL_BOARD_DC_MOTOR2                             GPIO_PIN_SET
//DC MOTOR3
#define VALID_LEVEL_BOARD_DC_MOTOR3                             GPIO_PIN_SET

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

/*---------------------------------------------------板卡上条码枪CR100M通用配置----------------------------------------------------------*/
//通讯缓存数据长度
#define COMM_CACHE_LENGTH_BOARD_BARSCAN_CR100M                  50
/*---------------------------------------------------板卡上条码枪CR100MA端口----------------------------------------------------------*/
//BARSCAN_CR100MA 通讯波特率
#define BOARD_BARSCAN_CR100MA_BAUD                              9600
//BARSCAN_CR100MA 单字节长度             
#define BOARD_BARSCAN_CR100MA_LENGTH                            MCU_UART_LENGTH_8B
//BARSCAN_CR100MA 停止位             
#define BOARD_BARSCAN_CR100MA_STOP_BIT                          MCU_UART_STOPBIT1
//BARSCAN_CR100MA 校验模式               
#define BOARD_BARSCAN_CR100MA_CHECK_MODE                        MCU_UART_CHECK_MODE_NONE
//BARSCAN_CR100MA 硬件流控制             
#define BOARD_BARSCAN_CR100MA_HARD_CONTROL                      MCU_UART_HARD_CONTROL_NONE
//BARSCAN_CR100MA接收消息缓冲区,最好和MCU串口接收数据块保持一   
#define LENGTH_QUEUE_BOARD_BARSCAN_CR100MA_RECV_MSG             COUNT_UART1_RECV_BLOCK
//BARSCAN_CR100MA消息处理循环时间
#define PERIOD_MS_PROC_BOARD_BARSCAN_CR100MA_RECV_MSG           20
//最大存放的结果数量
#define BOARD_BARSCAN_CR100MA_RESULT_BLOCK_COUNT                21
//通讯异常的重试次数
#define RETRY_MAX_COMM_BOARD_BARSCAN_CR100MA                    3
//版本号检查匹配数据长度
#define VERSION_CHECK_MATCH_CODE_LENGTH                         5
//单包数据长度
#define LENGTH_MIN_BARCODE_CR100M                               2
//BARSCAN_CR100MA串口数据
#define COMM_PACK_SRC_BOARD_BARSCAN_CR100MA                     UART_PACK_SRC_1
//BARSCAN_CR100MA使用的通讯端口初始化
#define COMM_INIT_BOARD_BARSCAN_CR100MA(baud,length,stopbit,checkMode,hardControl)        \
                                                                MCU_UART1_Init(baud,length,stopbit,checkMode,hardControl)
//BARSCAN_CR100MA方法注册
#define COMM_CALLBACL_REGISTER_BOARD_BARSCAN_CR100MA(recvIntCallBack,beforeSendCallBack,afterSendCallBack)    \
                                                                MCU_UART1_ConfigCallBack(recvIntCallBack,beforeSendCallBack,afterSendCallBack)
//处理完成的数据返回给底层驱动
#define RELEASE_PACK_BOARD_BARSCAN_CR100MA(packPtr)             MCU_UART1_RecvDatRelease(packPtr)
//发送指令
#define COMM_SEND_CMD_BUF_BOARD_BARSCAN_CR100MA(buf,len)        MCU_UART1_SendBuffer(buf,len)
//BARSCAN_CR100MA使用的内存申请函数
#define MEM_MALLOC_BOARD_BARSCAN_CR100MA(size)                  MCU_MemMallocWhileSuccessSramIn(size)
//BARSCAN_CR100MA使用的内存释放函数
#define MEM_FREE_BOARD_BARSCAN_CR100MA(ptr)                     MCU_MemFreeSramIn(ptr)


#endif






















