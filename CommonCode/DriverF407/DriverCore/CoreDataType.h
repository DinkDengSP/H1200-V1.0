/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-23 09:44:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-25 13:30:22 +0800
************************************************************************************************/ 
#ifndef __CORE_DATA_TYPE_H_
#define __CORE_DATA_TYPE_H_
#include "stm32f4xx_hal.h"
#include "tx_api.h"
#include "ErrorCode.h"

/*-----------------------------------------版本号结构体---------------------------------------*/
//系统软件版本号
typedef struct BOARD_SOFT_VERSION
{
    uint32_t mainVersion;
    uint32_t subVersion;
    uint32_t tempVersion;
    uint32_t devVersion;
}BOARD_SOFT_VERSION;

/*------------------------------------------------定时器相关---------------------------------------------------*/
//基础定时器回调函数
typedef void (*MCU_TimerBaseCallBack)(void);
//PWM输出匹配回调函数
typedef void (*MCU_TimerPWM_MatchCallBack)(void);
//定时器基础配置,16位定时器
typedef struct MCU_TIMER_BASE16_CONFIG
{
    uint16_t freqDivParam;//分频数值
    uint16_t countUpValue;//计数溢出值
    MCU_TimerBaseCallBack callBackFuncPtr;//回调函数
}MCU_TIMER_BASE16_CONFIG;
//定时器基础配置,32位定时器
typedef struct MCU_TIMER_BASE32_CONFIG
{
    uint16_t freqDivParam;//分频数值
    uint32_t countUpValue;//计数溢出值
    MCU_TimerBaseCallBack callBackFuncPtr;//回调函数
}MCU_TIMER_BASE32_CONFIG;

/*--------------------------------------------------SPI相关------------------------------------------------*/
//SPI信号极性
typedef enum MCU_SPI_CPOL
{
    MCU_SPI_CPOL_HIGH   = SPI_POLARITY_HIGH,
    MCU_SPI_CPOL_LOW    = SPI_POLARITY_LOW,
}MCU_SPI_CPOL;

//SPI采样点设置
typedef enum MCU_SPI_CPHA
{
    MCU_SPI_CPHA_1EDGE = SPI_PHASE_1EDGE,
    MCU_SPI_CPHA_2EDGE = SPI_PHASE_2EDGE,
}MCU_SPI_CPHA;

//SPI传输状态
typedef enum SPI_TRANS_STATE
{
    SPI_TRANS_STATE_IDLE        = 0x00,//空闲状态
    SPI_TRANS_STATE_START       = 0x01,//启动状态
    SPI_TRANS_STATE_END         = 0x02,//结束状态
    SPI_TRANS_STATE_ERROR       = 0x03,//错误状态
}SPI_TRANS_STATE;

//SPI1456接在APB2上,最大为系统频率的1/2
typedef enum MCU_SPI_1456_CLK
{
    SPI1456_BAUD_RATE_42M     = SPI_BAUDRATEPRESCALER_2,
    SPI1456_BAUD_RATE_21M     = SPI_BAUDRATEPRESCALER_4,
    SPI1456_BAUD_RATE_10_5M   = SPI_BAUDRATEPRESCALER_8,
    SPI1456_BAUD_RATE_5_25M   = SPI_BAUDRATEPRESCALER_16,
    SPI1456_BAUD_RATE_2_62_M  = SPI_BAUDRATEPRESCALER_32,
    SPI1456_BAUD_RATE_1_31_M  = SPI_BAUDRATEPRESCALER_64,
    SPI1456_BAUD_RATE_656K    = SPI_BAUDRATEPRESCALER_128,
    SPI1456_BAUD_RATE_328K    = SPI_BAUDRATEPRESCALER_256,
}MCU_SPI_1456_CLK;

//SPI23接在APB1上,最大为系统主频1/4
typedef enum MCU_SPI_23_CLK
{
    SPI23_BAUD_RATE_21M    = SPI_BAUDRATEPRESCALER_2,
    SPI23_BAUD_RATE_10_5M  = SPI_BAUDRATEPRESCALER_4,
    SPI23_BAUD_RATE_5_25M  = SPI_BAUDRATEPRESCALER_8,
    SPI23_BAUD_RATE_2_62_M = SPI_BAUDRATEPRESCALER_16,
    SPI23_BAUD_RATE_1_31_M = SPI_BAUDRATEPRESCALER_32,
    SPI23_BAUD_RATE_656K   = SPI_BAUDRATEPRESCALER_64,
    SPI23_BAUD_RATE_328K   = SPI_BAUDRATEPRESCALER_128,
    SPI23_BAUD_RATE_164K   = SPI_BAUDRATEPRESCALER_256,
}MCU_SPI_23_CLK;

//SPI1456通讯配置
typedef struct SPI1456_CONFIG
{
    MCU_SPI_1456_CLK baudRate;
    MCU_SPI_CPOL cpolConfig;
    MCU_SPI_CPHA cphaConfig;
}SPI1456_CONFIG;

//SPI23通讯配置
typedef struct SPI23_CONFIG
{
    MCU_SPI_23_CLK baudRate;
    MCU_SPI_CPOL cpolConfig;
    MCU_SPI_CPHA cphaConfig;
}SPI23_CONFIG;

/*------------------------------------------------IIC相关---------------------------------------------------*/
//IIC读写操作定义,在地址操作的时候,地址位的最低位区分读写
#define MCU_IIC_WRITE       0X00
#define MCU_IIC_READ        0X01

//SDA输入输出方向设置
typedef enum SDA_DIR
{
    SDA_DIR_IN      = 0X00,
    SDA_DIR_OUT     = 0X01,
}SDA_DIR;

//IIC的ACK状态
typedef enum IIC_ACK_STATE
{
    IIC_ACK_NONE    = 0X00,
    IIC_ACK_OK      = 0X01,
}IIC_ACK_STATE;

//IIC设备次级地址
typedef enum IIC_SUB_ADDR_SIZE
{
    IIC_SUB_ADDR_SIZE_8BIT   = 0x00,//8BIT地址
    IIC_SUB_ADDR_SIZE_16BIT  = 0x01,//16BIT地址
}IIC_SUB_ADDR_SIZE;

/*----------------------------------------------硬件串口相关-------------------------------------------------*/
//字长
typedef enum MCU_UART_LENGTH
{
    MCU_UART_LENGTH_8B = UART_WORDLENGTH_8B,
    MCU_UART_LENGTH_9B = UART_WORDLENGTH_9B,
}MCU_UART_LENGTH;

//停止位
typedef enum MCU_UART_STOPBIT
{
    MCU_UART_STOPBIT1   = UART_STOPBITS_1,
    MCU_UART_STOPBIT2   = UART_STOPBITS_2,
} MCU_UART_STOPBIT;

//校验位
typedef enum MCU_UART_CHECK_MODE
{
    MCU_UART_CHECK_MODE_NONE = UART_PARITY_NONE,
    MCU_UART_CHECK_MODE_EVEN = UART_PARITY_EVEN,
    MCU_UART_CHECK_MODE_ODD  = UART_PARITY_ODD,
} MCU_UART_CHECK_MODE;

//硬件流控制
typedef enum MCU_UART_HARD_CONTROL
{
    MCU_UART_HARD_CONTROL_NONE    = UART_HWCONTROL_NONE,
    MCU_UART_HARD_CONTROL_RTS     = UART_HWCONTROL_RTS,
    MCU_UART_HARD_CONTROL_CTS     = UART_HWCONTROL_CTS,
    MCU_UART_HARD_CONTROL_RTS_CTS = UART_HWCONTROL_RTS_CTS,
} MCU_UART_HARD_CONTROL;

//传输正在进行标志
typedef enum MCU_UART_TRANS_STATE
{
    MCU_UART_TRANS_IDLE = 0X00,//传输已完成
    MCU_UART_TRANS_ING  = 0X01,//传输正在进行
}MCU_UART_TRANS_STATE;

//串口数据源
typedef enum UART_PACK_SRC
{
    UART_PACK_SRC_1      = 0X00,
    UART_PACK_SRC_2      = 0X01,
    UART_PACK_SRC_3      = 0X02,
    UART_PACK_SRC_4      = 0X03,
    UART_PACK_SRC_6      = 0X04,
}UART_PACK_SRC;

typedef struct UART_RECV_PACK
{
    UART_PACK_SRC packSrc;
    uint8_t* uartRecvDataBuffer;
    uint16_t uartRecvDataLength;
}UART_RECV_PACK;

//串口接收中断处理函数
typedef ErrorStatus (*MCU_UartRecvIntProcFunc)(UART_RECV_PACK* uartPackRecvPtr);
//串口发送之前的回调函数
typedef void (*MCU_UartSendDataStartProcFunc)(void);
//串口发送完成之后的回调函数
typedef void (*MCU_UartSendDataOverProcFunc)(void);

//串口回调函数
typedef struct MCU_UART_CALLBACK
{
    //串口接收中断处理函数
    MCU_UartRecvIntProcFunc recvIntCallBack;
    //串口发送之前的回调函数
    MCU_UartSendDataStartProcFunc sendDataStartCallBack;
    //串口发送完成之后的回调函数
    MCU_UartSendDataOverProcFunc sendDataOverCallBack;
}MCU_UART_CALLBACK;

/*----------------------------------------------CAN通讯相关-------------------------------------------------*/
//CAN数据段波特率
typedef enum MCU_CAN_BPS
{
    MCU_CAN_BPS_1000K   = 0,
    MCU_CAN_BPS_500K    = 1,
    MCU_CAN_BPS_250K    = 2,
    MCU_CAN_BPS_125K    = 3,
    MCU_CAN_BPS_50K     = 4,
}MCU_CAN_BPS;

//CAN总线滤波器配置,标准帧
typedef struct CAN_STD_FILTER
{
    uint32_t fifoNumber;//过滤器配置,过滤到哪一个FIFO
    uint32_t filterID;//匹配模式,表示过滤ID,意思就是需要匹配的ID的状态
    uint32_t filterMask;//匹配模式,表示过滤ID掩码,意思就是上面的这个ID的哪些位需要匹配
}CAN_STD_FILTER;

//CAN总线滤波器配置,扩展帧
typedef struct CAN_EXTEND_FILTER
{
    uint32_t fifoNumber;//过滤器配置,过滤到哪一个FIFO
    uint32_t filterID;//匹配模式,表示过滤ID,意思就是需要匹配的ID的状态
    uint32_t filterMask;//匹配模式,表示过滤ID掩码,意思就是上面的这个ID的哪些位需要匹配
}CAN_EXTEND_FILTER;

//CAN模块初始化配置信息
typedef struct MCU_CAN_BPS_SETTING
{
    uint32_t tqWithSJW;//同步段
    uint32_t tqWithBS1;
    uint32_t tqWithBS2;
    uint16_t brpDiv;//分频系数
} MCU_CAN_BPS_SETTING;

//CAN模块的各项信息
typedef struct MCU_CAN_INFO
{
    HAL_CAN_StateTypeDef state;
    uint32_t errorCode;
    uint8_t reserve;
}MCU_CAN_INFO;

//CAN数据源
typedef enum CAN_PACK_SRC
{
    CAN_PACK_SRC_1  = 0X00,//CAN1端口数据
    CAN_PACK_SRC_2  = 0X01,//CAN2端口数据
}CAN_PACK_SRC;

//CAN数据包类型
typedef enum CAN_ID_TYPE
{
    CAN_ID_TYPE_STD = CAN_ID_STD,//标准帧
    CAN_ID_TYPE_EXT = CAN_ID_EXT,//扩展帧
}CAN_ID_TYPE;

//CAN接收数据包结构
typedef struct CAN_RECV_PACK
{
    CAN_PACK_SRC packSrc;//数据包源
    CAN_ID_TYPE idType;//ID类型
    uint32_t recvCmdID;//接收到的ID
    uint8_t* recvDataBufferPtr;//接收数据指针
    uint8_t recvDataLength;//接收数据长度
}CAN_RECV_PACK;

//处理接收数据的回调函数
typedef ErrorStatus (*MCU_CAN_RecvPackProcFunc)(CAN_RECV_PACK* recvDataPackPtr);

/*--------------------------------------------高级通讯协议基础相关---------------------------------------------------*/
//指令包类型定义
typedef enum PROT_PACK_TYPE
{
    PROT_PACK_TYPE_LOG              = 0X00,//日志包
    PROT_PACK_TYPE_CMD              = 0X01,//指令包
    PROT_PACK_TYPE_ACK_CMD          = 0X02,//指令ACK包
    PROT_PACK_TYPE_RESULT           = 0X03,//结果包
    PROT_PACK_TYPE_ACK_RESULT       = 0X04,//结果ACK包
    PROT_PACK_TYPE_DATA_UPLOAD      = 0X05,//数据上传包
    PROT_PACK_TYPE_ACK_DATA_UPLOAD  = 0X06,//数据上传ACK包
    PROT_PACK_TYPE_RESERVE          = 0X07,//保留包类型
}PROT_PACK_TYPE;

//指令状态定义
typedef enum PROT_CMD_STA
{
    PROT_CMD_STA_WAIT_ACK         = 0X00,//指令等待ACK
    PROT_CMD_STA_ACK_ERR          = 0X01,//指令ACK报错
    PROT_CMD_STA_ACK_TIME_OUT     = 0X02,//指令ACK超时
    PROT_CMD_STA_ACK_RETRY_FAIL   = 0X03,//指令重发失败
    PROT_CMD_STA_ACK_OVER         = 0X04,//指令不需要结果,ACK完成
    PROT_CMD_STA_WAIT_RESULT      = 0X05,//指令等待结果
    PROT_CMD_STA_RESULT_TIME_OUT  = 0X06,//指令等待结果超时
    PROT_CMD_STA_RESULT_LEN_SHORT = 0X07,//指令结果长度不住
    PROT_CMD_STA_RESULT_BUF_MALLOC= 0X08,//结果内存申请失败
    PROT_CMD_STA_OVER             = 0X09,//指令完成
}PROT_CMD_STA;

//协议结果定义
typedef enum PROT_RESULT
{
    PROT_RESULT_SUCCESS = 0X00,//指令执行成功
    PROT_RESULT_FAIL    = 0X01,//指令执行失败
}PROT_RESULT;

//协议ACK定义
typedef enum PROT_ACK
{
    PROT_ACK_REJECT = 0X00,//指令拒绝
    PROT_ACK_OK     = 0X01,//指令接受
}PROT_ACK;

//通道状态
typedef enum PROT_CH_STA
{
    PROT_CH_STA_IDLE  = 0X00,//通道空闲
    PROT_CH_STA_BUSY  = 0X01,//通道忙
}PROT_CH_STA;

//实体是否繁忙标记
typedef enum ENTITY_STA
{
    ENTITY_STA_IDLE     = 0X00,//实体空闲
    ENTITY_STA_BUSY     = 0X01,//实体繁忙
}ENTITY_STA;

//协议联机表示
typedef enum PROT_CONNECT_FLAG
{
    PROT_CONNECT_FLAG_DEBUG = 0X00,//调试联机
    PROT_CONNECT_FLAG_FLOW  = 0X01,//流程联机
}PROT_CONNECT_FLAG;

//协议脱机标识
typedef enum PROT_DISCONNECT_FLAG
{
    PROT_DISCONNECT_FLAG_NONE       = 0X00,//脱机不急停
    PROT_DISCONNECT_FLAG_EMSTOP     = 0X01,//脱机急停
}PROT_DISCONNECT_FLAG;

//运行模式
typedef enum SYS_RUNNING_MODE
{
    SYS_RUNNING_MODE_DEBUG   = 0X00,//老化模式
    SYS_RUNNING_MODE_RELEASE = 0X01,//正常测试模式
}SYS_RUNNING_MODE;

//系统数据指令交互的结果包定义
typedef struct SYS_RESULT_PACK
{
    PROT_RESULT resultCode;//结果码
    ERROR_MAIN  errorMain;//主错误码
    ERROR_LEVEL errorLevel;//错误级别
    ERROR_SUB   errorSub;//子错误码
}SYS_RESULT_PACK;

//转发配置单元信息
typedef struct NET_PROT_PACK_FWD_UNIT
{
    uint32_t cmdValue;
    TX_QUEUE* fwdQueuePtr;
}NET_PROT_PACK_FWD_UNIT;

//系统上传数据转发单元配置
typedef struct PROT_UPLOAD_PACK_FWD_UNIT
{
    uint32_t uploadID;
    TX_QUEUE* dispatchQueuePtr;
}PROT_UPLOAD_PACK_FWD_UNIT;


#endif












