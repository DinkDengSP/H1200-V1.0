/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-20 18:20:58 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-24 14:10:37 +0800
************************************************************************************************/ 
#ifndef __CAN_MASTER_UTIL_H_
#define __CAN_MASTER_UTIL_H_
#include "CanMasterDataType.h"


/*************************************发送指令等待ACK************************************************/
//联机
void CanMasterUtilOnlineWhileAck(uint8_t targetModule,uint8_t targetBoardID,PROT_CONNECT_FLAG protConnectFlag,
                                        SYS_RESULT_PACK* resultPackPtr);
//脱机
void CanMasterUtilOffLineWhileAck(uint8_t targetModule,uint8_t targetBoardID,PROT_DISCONNECT_FLAG disconnectFlag,
                                        SYS_RESULT_PACK* resultPackPtr);
//UTC时间同步
void CanMasterUtilUtcSyncWhileAck(uint8_t targetModule,uint8_t targetBoardID,uint64_t utcStamp,
                                        SYS_RESULT_PACK* resultPackPtr);
//心跳获取
void CanMasterUtilGetHeartWhileAck(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr);
//急停
void CanMasterUtilEmStopWhileAck(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr);
//版本号获取
void CanMasterUtilGetVersionWhileAck(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr);
//设置运行模式
void CanMasterUtilSetRunningModeWhileAck(uint8_t targetModule,uint8_t targetBoardID,SYS_RUNNING_MODE runningMode,SYS_RESULT_PACK* resultPackPtr);

/*************************************等待指定指令完成返回************************************************/
//联机
void CanMasterUtilWaitOnlineResult(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr,
                                            BOARD_RUN_MODE* boardRunModePtr);
//脱机
void CanMasterUtilWaitOffLineResult(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr,
                                            BOARD_RUN_MODE* boardRunModePtr);
//UTC时间同步
void CanMasterUtilWaitUtcSyncResult(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr);
//心跳获取
void CanMasterUtilWaitGetHeartResult(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr,
                                            uint64_t* utcStampPtr,BOARD_RUN_MODE* boardRunModePtr);
//急停
void CanMasterUtilWaitEmStopResult(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr);
//版本号获取
void CanMasterUtilWaitGetVersionResult(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr,
                                                BOARD_SOFT_VERSION* bootVersionPtr,BOARD_SOFT_VERSION* srvVersionPtr,
                                                BOARD_SOFT_VERSION* appVersionPtr);
//设置运行模式
void CanMasterUtilWaitSetRunningModeResult(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr);
/*************************************发送指令等待完成返回************************************************/
//联机
void CanMasterUtilOnlineWhileReturn(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr,
                                                PROT_CONNECT_FLAG protConnectFlag,BOARD_RUN_MODE* boardRunModePtr);
//脱机
void CanMasterUtilOffLineWhileReturn(uint8_t targetModule,uint8_t targetBoardID,PROT_DISCONNECT_FLAG disconnectFlag,
                                                    SYS_RESULT_PACK* resultPackPtr,BOARD_RUN_MODE* boardRunModePtr);
//UTC时间同步
void CanMasterUtilUtcSyncWhileReturn(uint8_t targetModule,uint8_t targetBoardID,uint64_t utcStamp,
                                                    SYS_RESULT_PACK* resultPackPtr);                                                    
//心跳获取
void CanMasterUtilGetHeartWhileReturn(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr,
                                                uint64_t* utcStampPtr,BOARD_RUN_MODE* boardRunModePtr);
//急停
void CanMasterUtilEmStopWhileReturn(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr);
//版本号获取
void CanMasterUtilGetVersionWhileReturn(uint8_t targetModule,uint8_t targetBoardID,SYS_RESULT_PACK* resultPackPtr,
                                                BOARD_SOFT_VERSION* bootVersionPtr,BOARD_SOFT_VERSION* srvVersionPtr,
                                                BOARD_SOFT_VERSION* appVersionPtr);
//设置运行模式
void CanMasterUtilSetRunningModeWhileReturn(uint8_t targetModule,uint8_t targetBoardID,SYS_RUNNING_MODE runningMode,SYS_RESULT_PACK* resultPackPtr);

#endif

































