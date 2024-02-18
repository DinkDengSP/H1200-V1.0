/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-11 13:33:37 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-09-11 15:53:08 +0800
************************************************************************************************/ 
#ifndef __PROTOCOL_CAN_RACK_STORE_H_
#define __PROTOCOL_CAN_RACK_STORE_H_
#include "ProtocolCanOumengInterBase.h"

//单字节对齐
#pragma pack(push,1)
//试管架存储单元状态集合
typedef struct RACK_STORE_SENSOR_SET
{
    PIN_STATE rackWareHouse1SensorState;    //样本架仓1传感器状态
    PIN_STATE rackWareHouse2SensorState;    //
    PIN_STATE rackWareHouse3SensorState;    //
    PIN_STATE rackWareHouse4SensorState;    //
    PIN_STATE rackWareHouse5SensorState;    //
    PIN_STATE rackWareHouse6SensorState;    //
    PIN_STATE rackWareHouse7SensorState;    //
    PIN_STATE rackWareHouse8SensorState;    //
    PIN_STATE rackWareHouse9SensorState;    //
    PIN_STATE rackWareHouse10SensorState;   //
    PIN_STATE rackWareHouse11SensorState;   //
    PIN_STATE rackWareHouse12SensorState;   //
    PIN_STATE rackWareHouse13SensorState;   //
    PIN_STATE rackWareHouse14SensorState;   //
    PIN_STATE rackWareHouse15SensorState;   //
    PIN_STATE rackWareHouse16SensorState;   //
    PIN_STATE rackWareHouse17SensorState;   //
    PIN_STATE rackWareHouse18SensorState;   //
    PIN_STATE rackWareHouse19SensorState;   //
    PIN_STATE rackWareHouse20SensorState;   //
    PIN_STATE rackWareHouse21SensorState;   //
    PIN_STATE rackWareHouse22SensorState;   //
    PIN_STATE rackWareHouse23SensorState;   //样本架仓23传感器状态
}RACK_STORE_SENSOR_SET;
#pragma pack(pop) 


/*********************************************************发送指令等待ACK********************************************************************/
//读取样本架仓指定仓位单个传感器状态
void CanMasterRackStoreSendReadSingleSensorByPosWhileAck(RACK_STORE_HOUSE rackHousePos,SYS_RESULT_PACK* resultPackPtr);
//读取样本架仓指定仓位全部传感器状态
void CanMasterRackStoreSendReadAllSensorStateWhileAck(SYS_RESULT_PACK* resultPackPtr);


/*********************************************************等待指定指令完成返回********************************************************************/
//读取样本架仓指定仓位单个传感器状态
void CanMasterRackStoreWaitReadSingleSensorByPosResult(SYS_RESULT_PACK* resultBasePack,PIN_STATE* sensorStatePtr);
//读取样本架仓指定仓位全部传感器状态
void CanMasterRackStoreWaitReadAllSensorStateResult(SYS_RESULT_PACK* resultBasePack,RACK_STORE_SENSOR_SET* sensorStateSetPtr);


/*********************************************************发送指令等待完成返回********************************************************************/
//读取样本架仓指定仓位单个传感器状态
void CanMasterRackStoreSendReadSingleSensorByPosWhileReturn(RACK_STORE_HOUSE rackHousePos,SYS_RESULT_PACK* resultBasePack,PIN_STATE* sensorStatePtr);
//读取样本架仓指定仓位全部传感器状态
void CanMasterRackStoreSendReadAllSensorStateWhileReturn(SYS_RESULT_PACK* resultBasePack,RACK_STORE_SENSOR_SET* sensorStateSetPtr);

#endif



