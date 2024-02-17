/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-13 11:21:37 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-13 13:40:09 +0800
************************************************************************************************/ 
#ifndef __BOARD_DS18B20_CONFIG_H_
#define __BOARD_DS18B20_CONFIG_H_
#include "DriverHeaderMCU.h"

//当总线只有一个18b20的时候,用来读取64位的ROM数据,包括 八位family数据 48位唯一序列号 8位CRC 一共64位标识符
#define DS18B20_COMMAND_READ_ROM		0X33
//该指令之后发送64位标识符,然后主机发送的指令只有匹配,这个标识符的芯片才会响应,其他的不会响应.如果所有从机都不响应,那么接下来应该发送复位脉冲
#define DS18B20_COMMAND_MATCH_ROM		0X55
//用在总线默认只有一个设备的时候,跳过设备识别阶段,默认总线上的全部指令,都由默认的芯片来访问,多芯片总线不能使用这个指令,会造成冲突
#define DS18B20_COMMAND_SKIP_ROM		0XCC
//当系统刚启动的时候,允许主机去获取系统中所有的设备的ROM
#define DS18B20_COMMAND_SEARCH_ROM		0XF0
//设备在初始化的时候响应指令,只有触发闹钟条件的才能触发指令
#define DS18B20_COMMAND_ALARM_SEARCH	0XEC
//写暂存器,从TH寄存器开始,写三个字节
#define DS18B20_WRITE_SCRATCHPAD	    0X4E
//读取暂存器,该命令之后 读取九个字节 八个寄存器 一个CRC
#define DS18B20_READ_SCRATCHPAD		    0XBE
//将暂存器中的信息存储到EE存储区中
#define DS18B20_COPY_SCRATCHPAD		    0X48
//启动芯片的温度转换,该指令之后读取总线,如果读取到的是0,总线繁忙 1 温度转换完成
#define DS18B20_CONVERT_TEMP		    0X44
//从EE存储区中读取信息到寄存器中
#define DS18B20_RECALL_E2			    0XB8
//读取芯片电源需求 0 总线寄生 1外部电源
#define DS18B20_READ_POWER_SUPPLY	    0XB4
//芯片检测的时候的最长的等待响应时间,单位是US
#define DS18B20_CHECK_RETRY_TIME	    240




#endif


