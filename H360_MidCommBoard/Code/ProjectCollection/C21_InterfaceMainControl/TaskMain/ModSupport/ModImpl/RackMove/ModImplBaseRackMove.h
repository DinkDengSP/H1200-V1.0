/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-31 16:34:18
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-02-01 11:27:17
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#ifndef __MOD_IMPL_BASE_RACK_MOVE_H_
#define __MOD_IMPL_BASE_RACK_MOVE_H_
#include "ModImplBase.h"

//试管架移动单元电机复位
void ModImplBaseRackMoveMotorReset(STEP_MOTOR_RACK_MOVE motorNo,SYS_RESULT_PACK* resultPackPtr,
                                    uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//试管架移动单元电机走步数
void ModImplBaseRackMoveMotorRunStep(STEP_MOTOR_RACK_MOVE motorNo,int32_t runSteps,SYS_RESULT_PACK* resultPackPtr,
                                        uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//试管架移动单元电机状态读取
void ModImplBaseRackMoveMotorStateRead(STEP_MOTOR_RACK_MOVE motorNo,SYS_RESULT_PACK* resultPackPtr,
                                        uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//试管架移动单元传感器信号单个读取
void ModImplBaseRackMoveSensorReadSingle(RACK_MOVE_SENSOR rackMoveSensorNo,SYS_RESULT_PACK* resultPackPtr,
                                                uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//试管架移动单元传感器信号批量读取
void ModImplBaseRackMoveSensorReadAll(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//获取试管架移动单元当前位置
void ModImplBaseRackMoveGetCurrentPos(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//试管架移动单元复位
void ModImplBaseRackMoveModuleReset(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//试管架移动单元到空闲位置
void ModImplBaseRackMoveModuleToIdle(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//横移电机移动到指定试管架仓位置
void ModImplBaseRackMoveLaterallyToStoreByPos(RACK_STORE_HOUSE rackStoreHouse,SYS_RESULT_PACK* resultPackPtr,
                                                uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//横移电机移动到C21接口位置
void ModImplBaseRackMoveLaterallyToC21(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//装载机构移动到装载试管架仓位置
void ModImplBaseRackMoveRunToLoadRackStorePos(RACK_STORE_HOUSE rackStoreHouse,SYS_RESULT_PACK* resultPackPtr,
                                                    uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//装载机构移动到卸载试管架仓位置
void ModImplBaseRackMoveRunToUnLoadRackStorePos(RACK_STORE_HOUSE rackStoreHouse,SYS_RESULT_PACK* resultPackPtr,
                                                    uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//装载机构移动到装载C21位置
void ModImplBaseRackMoveRunToLoadRackC21Pos(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//装载机构移动到卸载C21位置
void ModImplBaseRackMoveRunToUnLoadRackC21(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//在当前位置执行试管架仓样本架装载
void ModImplBaseRackMoveLoadInRackFormStoreCurrent(FunctionalState barScanSet,SYS_RESULT_PACK* resultPackPtr,
                                                        uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//在当前位置执行试管架仓样本架放入
void ModImplBaseRackMovePushOutRackToStoreCurrent(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                                        uint16_t* dataBufLen);

//在当前位置执行C21样本架装载
void ModImplBaseRackMoveLoadInRackFormC21Current(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                                        uint16_t* dataBufLen);

//在当前位置执行C21样本架放入
void ModImplBaseRackMovePushOutRackToC21Current(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                                    uint16_t* dataBufLen);

//集成指令,从试管架仓中装载一个试管架
void ModImplBaseRackMoveLoadInRackFormStoreByPos(RACK_STORE_HOUSE rackStoreHouse,FunctionalState barScanSet,
                                                    SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//集成指令,往试管架仓中放入一个试管架
void ModImplBaseRackMovePushOutRackToStoreByPos(RACK_STORE_HOUSE rackStoreHouse,SYS_RESULT_PACK* resultPackPtr,
                                                    uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//集成指令,从C21接口装载一个试管架
void ModImplBaseRackMoveLoadInRackFormC21(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                                uint16_t* dataBufLen);

//集成指令,往C21接口放入一个试管架
void ModImplBaseRackMovePushOutRackToC21(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                                uint16_t* dataBufLen);

//集成指令,从试管仓中取出一个试管架放入另一个试管仓中
void ModImplBaseRackMoveLoadInRackFormStoreByPosToStoreByPos(RACK_STORE_HOUSE rackStoreHouseSrc,RACK_STORE_HOUSE rackStoreHouseDst,
                                                                FunctionalState barScanSet,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                                                uint16_t* dataBufLen);

//集成指令,从试管仓中取出一个试管架放入C21中
void ModImplBaseRackMoveLoadInRackFormStoreByPosToC21(RACK_STORE_HOUSE rackStoreHouse,FunctionalState barScanSet,SYS_RESULT_PACK* resultPackPtr,
                                                            uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//从C21中取出一个试管架放入试管架仓中
void ModImplBaseRackMoveLoadInRackFormC21ToRackStore(RACK_STORE_HOUSE rackStoreHouse,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,
                                                        uint16_t* dataBufLen);



#endif




