/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-18 19:36:42 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-09-18 19:36:42 +0800
************************************************************************************************/  
#ifndef __BOARD_IAP_H_
#define __BOARD_IAP_H_
#include "BoardIapConfig.h"

//当前程序运行标记
typedef enum BOARD_RUN_MODE
{
    BOARD_RUN_MODE_BOOT = 0,
    BOARD_RUN_MODE_APP  = 1,
}BOARD_RUN_MODE;

//程序强制跳转目标
typedef enum BOARD_FORCE_JMP
{
    BOARD_FORCE_JMP_BOOT    = 0,
    BOARD_FORCE_JMP_MAIN    = 1,
}BOARD_FORCE_JMP;

//IAP模式
typedef enum BOARD_IAP_MODE 
{
    BOARD_IAP_MODE_BOOT = 0X00,
    BOARD_IAP_MODE_SRV  = 0X01,
    BOARD_IAP_MODE_APP  = 0X02,
    BOARD_IAP_MODE_NONE = 0XFF,
}BOARD_IAP_MODE;

//IAP源
typedef enum BOARD_IAP_SRC
{
    BOARD_IAP_SRC_NET       = 0X00,
    BOARD_IAP_SRC_UART      = 0X01,
    BOARD_IAP_SRC_CAN1      = 0X02,
    BOARD_IAP_SRC_CAN2      = 0X03,
    BOARD_IAP_SRC_NONE      = 0XFF,
}BOARD_IAP_SRC;

//IAP 文件记录是否存在
typedef enum BOARD_IAP_RECODER_FLAG
{
    BOARD_IAP_RECODER_FLAG_NOT_EXIST = 0XA5A5A5A5,
    BOARD_IAP_RECODER_FLAG_EXIST     = 0X5A5A5A5A,
}BOARD_IAP_RECODER_FLAG;

//在线升级文件信息
typedef struct BOARD_IAP_FILE_INFO
{
    BOARD_IAP_MODE boardIapMode;
    uint32_t totalFileBytes;//升级文件总字节数
    uint32_t totalPackCount;//升级文件总包数
    uint32_t singlePackBytes;//升级文件单包字节数
    uint32_t recordRecvFilePackCount;//已经接收到的文件包数量
    uint32_t recordRecvFileByteCount;//已经接收到的文件字节数量
    uint32_t crc32CodeRecv;//接收到的数据的CRC校验码
}BOARD_IAP_FILE_INFO;

//在线升级文件记录信息
typedef struct BOARD_IAP_RECODER
{
    BOARD_IAP_RECODER_FLAG iapFileExistFlag;
    BOARD_IAP_SRC boardIapSrc;
    uint32_t crc32Code;
    uint32_t iapFileTotalBytes;
}BOARD_IAP_RECODER;

//IAP头清空
void BoardIapClearFileInfo(BOARD_IAP_FILE_INFO* iapFileInfoPtr);

//计算CRC32
uint32_t BoardIapCalcCRC32(const uint8_t *pu8Data, uint32_t u32Size);

//读取IAP BOOT文件
ERROR_SUB BoardIapReadBootBinFile(uint32_t addrOffset,uint8_t* bufferPtr,uint32_t byteLength);
//写入IAP BOOT文件
ERROR_SUB BoardIapWriteBootBinFile(uint32_t addrOffset,uint8_t* bufferPtr,uint32_t byteLength);

//读取IAP Service文件
ERROR_SUB BoardIapReadSrvBinFile(uint32_t addrOffset,uint8_t* bufferPtr,uint32_t byteLength);
//写入IAP Service文件
ERROR_SUB BoardIapWriteSrvBinFile(uint32_t addrOffset,uint8_t* bufferPtr,uint32_t byteLength);

//读取IAP App文件
ERROR_SUB BoardIapReadAppBinFile(uint32_t addrOffset,uint8_t* bufferPtr,uint32_t byteLength);
//写入IAP App文件
ERROR_SUB BoardIapWriteAppBinFile(uint32_t addrOffset,uint8_t* bufferPtr,uint32_t byteLength);

//读取系统运行模式标记
ERROR_SUB BoardIapReadCurrentMode(uint32_t* runningMode);
//写入系统运行模式BOOT
ERROR_SUB BoardIapSetRunningModeBoot(void);
//写入系统运行模式APP
ERROR_SUB BoardIapSetRunningModeApp(void);
//读取板卡IAP模式标记
ERROR_SUB BoardIapReadRunMode(BOARD_RUN_MODE* runMode);

//读取 BOOT IAP升级记录信息
ERROR_SUB BoardIapReadBootRecord(BOARD_IAP_RECODER* recordMsgPtr);
//写入BOOT IAP记录信息
ERROR_SUB BoardIapWriteBootRecord(BOARD_IAP_RECODER* recordMsgPtr);
//初始化BOOT IAP记录信息
ERROR_SUB BoardIapWriteBootRecordDefault(void);

//读取 Service IAP升级记录信息
ERROR_SUB BoardIapReadServiceRecord(BOARD_IAP_RECODER* recordMsgPtr);
//写入 Service IAP记录信息
ERROR_SUB BoardIapWriteServiceRecord(BOARD_IAP_RECODER* recordMsgPtr);
//初始化 Service IAP记录信息
ERROR_SUB BoardIapWriteServiceRecordDefault(void);

//读取 APP IAP升级记录信息
ERROR_SUB BoardIapReadAppRecord(BOARD_IAP_RECODER* recordMsgPtr);
//写入 APP IAP记录信息
ERROR_SUB BoardIapWriteAppRecord(BOARD_IAP_RECODER* recordMsgPtr);
//初始化 APP IAP记录信息
ERROR_SUB BoardIapWriteAppRecordDefault(void);

//板卡IAP跳转到APP
ERROR_SUB BoardIapJumpApp(void);
//板卡IAP跳转到BOOT
ERROR_SUB BoardIapJumpBoot(void);

//检查是否满足跳转APP的条件
ErrorStatus BoardIapCheckAppExist(void);
//检查是否满足跳转到BOOT的条件
ErrorStatus BoardIapCheckBootExist(void);

//板上IAP初始化
ERROR_SUB BoardIapInit(void);


#endif

