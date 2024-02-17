/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-24 10:44:38 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-24 11:58:06 +0800
************************************************************************************************/ 
#ifndef __CAN_MASTER_PARAM_H_
#define __CAN_MASTER_PARAM_H_
#include "CanMasterUtil.h"

/*************************************发送指令等待ACK************************************************/
//单个参数读取
void CanMasterParamReadSingleWhileAck(uint8_t targetModule,uint8_t targetBoardID,uint8_t mainIndex,uint8_t subIndex,
                                            SYS_RESULT_PACK* resultPackPtr);
//批量参数读取
void CanMasterParamReadPairsWhileAck(uint8_t targetModule,uint8_t targetBoardID,uint8_t* mainIndexArrayPtr,uint8_t* subIndexArrayPtr,
                                            uint8_t pairs,SYS_RESULT_PACK* resultPackPtr);
//单个参数写入
void CanMasterParamWriteSingleWhileAck(uint8_t targetModule,uint8_t targetBoardID,uint8_t mainIndex,uint8_t subIndex,int32_t paramWrite,
                                            SYS_RESULT_PACK* resultPackPtr);
//批量参数写入
void CanMasterParamWritePairsWhileAck(uint8_t targetModule,uint8_t targetBoardID,uint8_t* mainIndexArrayPtr,uint8_t* subIndexArrayPtr,
                                            int32_t* paramWriteArrayPtr,uint8_t pairs,SYS_RESULT_PACK* resultPackPtr);


/*************************************等待指定指令完成返回************************************************/
//单个参数读取
void CanMasterParamWaitReadSingleResult(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr,int32_t* readParam);
//批量参数读取
void CanMasterParamWaitReadPairsResult(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr,int32_t* readParamArrayPtr);
//单个参数写入
void CanMasterParamWaitWriteSingleResult(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr);
//批量参数写入
void CanMasterParamWaitWritePairsResult(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr);


/*************************************发送指令等待完成返回************************************************/
//单个参数读取
void CanMasterParamReadSingleWhileReturn(uint8_t targetModule,uint8_t targetBoardID,uint8_t mainIndex,uint8_t subIndex,
                                                SYS_RESULT_PACK* resultPackPtr,int32_t* readParam);
//批量参数读取
void CanMasterParamReadPairsWhileReturn(uint8_t targetModule,uint8_t targetBoardID,uint8_t* mainIndexArrayPtr,uint8_t* subIndexArrayPtr,
                                                uint8_t pairs,SYS_RESULT_PACK* resultPackPtr,int32_t* readParamArrayPtr);
//单个参数写入
void CanMasterParamWriteSingleWhileReturn(uint8_t targetModule,uint8_t targetBoardID,uint8_t mainIndex,uint8_t subIndex,int32_t paramWrite,
                                                SYS_RESULT_PACK* resultPackPtr);
//批量参数写入
void CanMasterParamWritePairsWhileReturn(uint8_t targetModule,uint8_t targetBoardID,uint8_t* mainIndexArrayPtr,uint8_t* subIndexArrayPtr,
                                                int32_t* paramWriteArrayPtr,uint8_t pairs,SYS_RESULT_PACK* resultPackPtr);




#endif
















