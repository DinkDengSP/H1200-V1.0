/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-20 08:40:07 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-20 13:24:18 +0800
************************************************************************************************/ 
#ifndef __CAN_PROT_PORT_H_
#define __CAN_PROT_PORT_H_
#include "DriverHeaderBoard.h"
#include "Can1ProtPort.h"
#include "Can2ProtPort.h"

/****************************************************SOCKET ALL***********************************************************/
//错误信息打印函数
#define CAN_PROT_LOG(format,...)                        MCU_RTT_Printf(format, ##__VA_ARGS__)     
//创建CAN过滤器
#define CAN_PROT_FILTER_ID(moduleno,boardId)            (((uint32_t)(moduleno&0X07))<<23)|(((uint32_t)(boardId&0X1F))<<13) 
//创建掩码
#define CAN_PROT_FILTER_MASK                            ((0X00000007<<23)|(0X0000001F<<13))                    

//CAN接收长包,两包数据的间隔时间,防止漏包
#define TIME_OUT_MS_INTERVAL_LONG_PACK_CAN_PROT         100
//CAN指令数据包检测周期,越小系统负载越大
#define TIME_MS_INTERVAL_CMD_QUERY_CAN_PROT             10
//结果上传通道忙的时候,等待通道空闲时间
#define TIME_OUT_MS_MAX_CAN_PROT_WAIT_RESULT_CH         1000
//数据上报上传通道忙的时候,等待通道空闲时间
#define TIME_OUT_MS_MAX_CAN_PROT_WAIT_UPLOAD_CH         1000
//清除指令时候强制清除未完成的指令
#define CAN_PROT_FORCE_CLEAR_NOT_COMPLETE               CONFIG_FUNC_ENABLE
/****************************************************指令转发单元数据结构***************************************************/
//CAN指令转发单元配置
typedef struct CAN_PROT_CMD_FWD_UNIT
{
    uint8_t channel;
    TX_QUEUE* dispatchQueuePtr;
}CAN_PROT_CMD_FWD_UNIT;

//CAN指令转发单元配置
typedef struct CAN_CMD_PACK_DISPATCH_UNIT
{
    uint8_t channel;//通道序号
    TX_QUEUE* dispatchQueuePtr;//转发目标
}CAN_CMD_PACK_DISPATCH_UNIT;

/*************************************************************************************************************************/
//CRC校验算法
uint16_t CanProtPortCalcCRC16(uint8_t* bufferPtr,uint16_t dataLength);

//通过配置计算SOCKET组
PORT_CAN  CanProtPortCalcPort(uint8_t moduleNo,uint8_t boardID);

//系统板卡ID定义
#define CAN_MODULE_NO_TEMP_CONTROL                  SYSTEM_MODULE_NO_ACT
#define CAN_BOARD_ID_TEMP_CONTROL                   SYSTEM_BOARD_ID_TC_H360

#endif













