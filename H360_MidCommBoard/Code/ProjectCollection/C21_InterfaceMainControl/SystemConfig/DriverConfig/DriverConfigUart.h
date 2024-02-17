/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-29 11:51:45
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-29 13:08:48
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#ifndef __DRIVER_CONFIG_UART_H_
#define __DRIVER_CONFIG_UART_H_
#include "DriverConfigMCU.h"


/*---------------------------------------芯片串口------------------------------------------*/
//串口模式
#define MCU_UART_TRANS_MODE_TX                          0
#define MCU_UART_TRANS_MODE_RX                          1
#define MCU_UART_TRANS_MODE_TXRX                        2
//串口数据发送模式
#define MCU_UART_TX_FUNC_NONE                           0//无发送功能
#define MCU_UART_TX_FUNC_POOL                           1//查询发送
#define MCU_UART_TX_FUNC_INT                            2//中断发送
#define MCU_UART_TX_FUNC_DMA                            3//DMA发送
//串口数据接收模式
#define MCU_UART_RX_FUNC_NONE                           0//无接收功能
#define MCU_UART_RX_FUNC_INT                            1//中断接收
#define MCU_UART_RX_FUNC_DMA                            2//DMA接收
//必须打开UART内部回调
#if(USE_HAL_UART_REGISTER_CALLBACKS == 0)
#error "Lib UART Must Use CallBack FuncPtr !!!"
#endif
//必须打开USART内部回调
#if(USE_HAL_USART_REGISTER_CALLBACKS == 0)
#error "Lib USART Must Use CallBack FuncPtr !!!"
#endif


/*--------------------------------------------------------串口1-----------------------------------------------------------*/
//用户选定的串口模式
#define MCU_UART1_TRANS_MODE_USER                       MCU_UART_TRANS_MODE_TXRX
//定义串口模式
#if(MCU_UART1_TRANS_MODE_USER == MCU_UART_TRANS_MODE_TX)
#define MCU_UART1_TRANS_MODE                            UART_MODE_TX
#elif(MCU_UART1_TRANS_MODE_USER == MCU_UART_TRANS_MODE_RX)
#define MCU_UART1_TRANS_MODE                            UART_MODE_RX
#elif(MCU_UART1_TRANS_MODE_USER == MCU_UART_TRANS_MODE_TXRX)
#define MCU_UART1_TRANS_MODE                            UART_MODE_TX_RX
#endif
//定义串口发送方法
#define MCU_UART1_TX_FUNC_USER                          MCU_UART_TX_FUNC_DMA
//定义串口接收方法
#define MCU_UART1_RX_FUNC_USER                          MCU_UART_RX_FUNC_DMA
//发送方法定义,1为同步发送,发送必须等待完成 0为异步发送,写到缓冲区里面自动发送
#define UART1_TRANS_FUNC_SYNC                           0
//等待时间,9600下,单字节发送最长时间也不到2MS
#define MCU_UART1_SINGLE_BYTE_TRANS_TIME_MS_MAX         2
//串口传输指定长度数据的最长等待时间
#define MCU_UART1_WAIT_TRANS_MAX(bufferByteMax)         (MCU_UART1_SINGLE_BYTE_TRANS_TIME_MS_MAX*(bufferByteMax))
//是否打开一个采样位方法,打开的话,将禁止噪声检测标记
#define MCU_UART1_ONE_BIT_SAMPLE                        UART_ONE_BIT_SAMPLE_ENABLE
//过采样模式设定,8位速度更快,16位对波特率的兼容性更好
#define MCU_UART1_OVERSAMPLING_MODE                     UART_OVERSAMPLING_16
//是否关闭ORE过载
#define MCU_UART1_DISABLE_ORE                           1


//TX引脚定义
#define UART1_PIN_TX                                    MCU_PIN_A_9
//串口用于printf的缓冲区的长度      
#define LENGTH_UART1_BUFFER_WITH_FORMAT                 1024
//发送数据的串口缓冲区的长度        
#define LENGTH_UART1_BUFFER_WITH_SEND                   1024
//使用DMA单块发送数据,单块的长度        
#define LENGTH_UART1_BUFFER_WITH_BLOCK                  256

//RX引脚定义
#define UART1_PIN_RX                                    MCU_PIN_A_10
//串口用于接收的BLOCK数量       
#define COUNT_UART1_RECV_BLOCK                          8
//串口用于接收的BLOCK的每一个的长度     
#define LENGTH_UART1_RECV_SINGLE_BLOCK                  1024

//发送DMA各项配置映射
#define MCU_UART1_DMA_TX_CLK_ENABLE()                   __HAL_RCC_DMA1_CLK_ENABLE()
#define MCU_UART1_DMA_TX_INSTANCE                       DMA1_Stream0
#define MCU_UART1_DMA_TX_PRIORITY                       DMA_PRIORITY_MEDIUM
#define MCU_UART1_DMA_TX_IRQ                            DMA1_Stream0_IRQn
#define MCU_UART1_DMA_TX_IRQ_HANDLER                    DMA1_Stream0_IRQHandler

//接收DMA的各项配置 
#define MCU_UART1_DMA_RX_CLK_ENABLE()                   __HAL_RCC_DMA1_CLK_ENABLE()
#define MCU_UART1_DMA_RX_INSTANCE                       DMA1_Stream1
#define MCU_UART1_DMA_RX_PRIORITY                       DMA_PRIORITY_VERY_HIGH
#define MCU_UART1_DMA_RX_IRQ                            DMA1_Stream1_IRQn
#define MCU_UART1_DMA_RX_IRQ_HANDLER                    DMA1_Stream1_IRQHandler

//错误配置报警,防止用户配错模式导致程序异常
#if(MCU_UART1_TRANS_MODE_USER == MCU_UART_TRANS_MODE_TX)&&(MCU_UART1_RX_FUNC_USER != MCU_UART_RX_FUNC_NONE)
//只定义了发送,但是却定义了接收模式,错误,接受模式应该是 MCU_UART_RX_FUNC_NONE
#error "MCU_UART1 Only Define TX,Should Not Define RX USER Func"
#endif
#if(MCU_UART1_TRANS_MODE_USER == MCU_UART_TRANS_MODE_RX)&&(MCU_UART1_TX_FUNC_USER != MCU_UART_TX_FUNC_NONE)
//只定义了接收,但是却定义了发送模式,错误,发送模式应该是 MCU_UART_TX_FUNC_NONE
#error "MCU_UART1 Only Define RX,Should Not Define TX USER Func"
#endif
#if(MCU_UART1_TRANS_MODE_USER == MCU_UART_TRANS_MODE_RX)&&(MCU_UART1_RX_FUNC_USER == MCU_UART_RX_FUNC_NONE)
//定义了接收,却没有定义接收方法
#error "MCU_UART1 Define RX,Should Define RX USER Func"
#endif
#if(MCU_UART1_TRANS_MODE_USER == MCU_UART_TRANS_MODE_TX)&&(MCU_UART1_TX_FUNC_USER == MCU_UART_TX_FUNC_NONE)
//定义了发送,却没有定义发送方法
#error "MCU_UART1 Define TX,Should Define TX USER Func"
#endif
#if(MCU_UART1_TRANS_MODE_USER == MCU_UART_TRANS_MODE_TXRX)&&(MCU_UART1_RX_FUNC_USER == MCU_UART_RX_FUNC_NONE)
//定义了发送接收,却没有定义接收方法
#error "MCU_UART1 Define TXRX,Should Define RX USER Func"
#endif
#if(MCU_UART1_TRANS_MODE_USER == MCU_UART_TRANS_MODE_TXRX)&&(MCU_UART1_TX_FUNC_USER == MCU_UART_TX_FUNC_NONE)
//定义了发送接收,却没有定义发送方法
#error "MCU_UART1 Define TXRX,Should Define TX USER Func"
#endif


/*--------------------------------------------------------串口6-----------------------------------------------------------*/
//用户选定的串口模式
#define MCU_UART6_TRANS_MODE_USER                       MCU_UART_TRANS_MODE_TXRX
//定义串口模式
#if(MCU_UART6_TRANS_MODE_USER == MCU_UART_TRANS_MODE_TX)
#define MCU_UART6_TRANS_MODE                            UART_MODE_TX
#elif(MCU_UART6_TRANS_MODE_USER == MCU_UART_TRANS_MODE_RX)
#define MCU_UART6_TRANS_MODE                            UART_MODE_RX
#elif(MCU_UART6_TRANS_MODE_USER == MCU_UART_TRANS_MODE_TXRX)
#define MCU_UART6_TRANS_MODE                            UART_MODE_TX_RX
#endif
//定义串口发送方法
#define MCU_UART6_TX_FUNC_USER                          MCU_UART_TX_FUNC_DMA
//定义串口接收方法
#define MCU_UART6_RX_FUNC_USER                          MCU_UART_RX_FUNC_DMA
//发送方法定义,1为同步发送,发送必须等待完成 0为异步发送,写到缓冲区里面自动发送
#define UART6_TRANS_FUNC_SYNC                           0
//等待时间,9600下,单字节发送最长时间也不到2MS
#define MCU_UART6_SINGLE_BYTE_TRANS_TIME_MS_MAX         2
//串口传输指定长度数据的最长等待时间
#define MCU_UART6_WAIT_TRANS_MAX(bufferByteMax)         (MCU_UART6_SINGLE_BYTE_TRANS_TIME_MS_MAX*(bufferByteMax))
//是否打开一个采样位方法,打开的话,将禁止噪声检测标记
#define MCU_UART6_ONE_BIT_SAMPLE                        UART_ONE_BIT_SAMPLE_ENABLE
//过采样模式设定,8位速度更快,16位对波特率的兼容性更好
#define MCU_UART6_OVERSAMPLING_MODE                     UART_OVERSAMPLING_16
//是否关闭ORE过载
#define MCU_UART6_DISABLE_ORE                           1


//TX引脚定义
#define UART6_PIN_TX                                    MCU_PIN_C_6
//串口用于printf的缓冲区的长度      
#define LENGTH_UART6_BUFFER_WITH_FORMAT                 1024
//发送数据的串口缓冲区的长度        
#define LENGTH_UART6_BUFFER_WITH_SEND                   1024
//使用DMA单块发送数据,单块的长度        
#define LENGTH_UART6_BUFFER_WITH_BLOCK                  256

//RX引脚定义
#define UART6_PIN_RX                                    MCU_PIN_C_7
//串口用于接收的BLOCK数量       
#define COUNT_UART6_RECV_BLOCK                          8
//串口用于接收的BLOCK的每一个的长度     
#define LENGTH_UART6_RECV_SINGLE_BLOCK                  1024

//发送DMA各项配置映射
#define MCU_UART6_DMA_TX_CLK_ENABLE()                   __HAL_RCC_DMA1_CLK_ENABLE()
#define MCU_UART6_DMA_TX_INSTANCE                       DMA1_Stream2
#define MCU_UART6_DMA_TX_PRIORITY                       DMA_PRIORITY_MEDIUM
#define MCU_UART6_DMA_TX_IRQ                            DMA1_Stream2_IRQn
#define MCU_UART6_DMA_TX_IRQ_HANDLER                    DMA1_Stream2_IRQHandler

//接收DMA的各项配置 
#define MCU_UART6_DMA_RX_CLK_ENABLE()                   __HAL_RCC_DMA1_CLK_ENABLE()
#define MCU_UART6_DMA_RX_INSTANCE                       DMA1_Stream3
#define MCU_UART6_DMA_RX_PRIORITY                       DMA_PRIORITY_VERY_HIGH
#define MCU_UART6_DMA_RX_IRQ                            DMA1_Stream3_IRQn
#define MCU_UART6_DMA_RX_IRQ_HANDLER                    DMA1_Stream3_IRQHandler

//错误配置报警,防止用户配错模式导致程序异常
#if(MCU_UART6_TRANS_MODE_USER == MCU_UART_TRANS_MODE_TX)&&(MCU_UART6_RX_FUNC_USER != MCU_UART_RX_FUNC_NONE)
//只定义了发送,但是却定义了接收模式,错误,接受模式应该是 MCU_UART_RX_FUNC_NONE
#error "MCU_UART6 Only Define TX,Should Not Define RX USER Func"
#endif
#if(MCU_UART6_TRANS_MODE_USER == MCU_UART_TRANS_MODE_RX)&&(MCU_UART6_TX_FUNC_USER != MCU_UART_TX_FUNC_NONE)
//只定义了接收,但是却定义了发送模式,错误,发送模式应该是 MCU_UART_TX_FUNC_NONE
#error "MCU_UART6 Only Define RX,Should Not Define TX USER Func"
#endif
#if(MCU_UART6_TRANS_MODE_USER == MCU_UART_TRANS_MODE_RX)&&(MCU_UART6_RX_FUNC_USER == MCU_UART_RX_FUNC_NONE)
//定义了接收,却没有定义接收方法
#error "MCU_UART6 Define RX,Should Define RX USER Func"
#endif
#if(MCU_UART6_TRANS_MODE_USER == MCU_UART_TRANS_MODE_TX)&&(MCU_UART6_TX_FUNC_USER == MCU_UART_TX_FUNC_NONE)
//定义了发送,却没有定义发送方法
#error "MCU_UART6 Define TX,Should Define TX USER Func"
#endif
#if(MCU_UART6_TRANS_MODE_USER == MCU_UART_TRANS_MODE_TXRX)&&(MCU_UART6_RX_FUNC_USER == MCU_UART_RX_FUNC_NONE)
//定义了发送接收,却没有定义接收方法
#error "MCU_UART6 Define TXRX,Should Define RX USER Func"
#endif
#if(MCU_UART6_TRANS_MODE_USER == MCU_UART_TRANS_MODE_TXRX)&&(MCU_UART6_TX_FUNC_USER == MCU_UART_TX_FUNC_NONE)
//定义了发送接收,却没有定义发送方法
#error "MCU_UART6 Define TXRX,Should Define TX USER Func"
#endif

#endif





