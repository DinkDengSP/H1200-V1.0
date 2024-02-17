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
#include "ModImplBaseRackStore.h"
#include "ModImplRackStore.h"
#include "ModImplRackStoreConfig.h"

//读取样本架仓指定仓位单个传感器状态
void ModImplBaseRackStoreReadSingleSensorByPos(RACK_STORE_HOUSE rackStoreHouse,SYS_RESULT_PACK* resultPackPtr,
                                                        uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    PIN_STATE storeSensorState;
    //返回数据
    *dataBufLen = 1;
    *dataBufPtrPtr = NULL;
    if(0 != *dataBufLen)
    {
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_MOD_TASK,(*dataBufLen));
    }
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    //调用CAN接口
    CanMasterRackStoreSendReadSingleSensorByPosWhileReturn(rackStoreHouse,resultPackPtr,&storeSensorState);
    //结果检查
    if(resultPackPtr->resultCode != PROT_RESULT_SUCCESS)
    {
        return;
    }
    //结果写入
    UserMemCopy(*dataBufPtrPtr,(uint8_t*)(&storeSensorState),1);
    return;
}

//读取样本架仓指定仓位全部传感器状态
void ModImplBaseRackStoreReadAllSensorState(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    RACK_STORE_SENSOR_SET storeSensorSet;
    //返回数据
    *dataBufLen = DIM_STRUCT_LENGTH_BYTE(RACK_STORE_SENSOR_SET);
    *dataBufPtrPtr = NULL;
    if(0 != *dataBufLen)
    {
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_MOD_TASK,(*dataBufLen));
    }
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    //调用CAN接口
    CanMasterRackStoreSendReadAllSensorStateWhileReturn(resultPackPtr,&storeSensorSet);
    //结果检查
    if(resultPackPtr->resultCode != PROT_RESULT_SUCCESS)
    {
        return;
    }
    //结果写入
    UserMemCopy(*dataBufPtrPtr,(uint8_t*)(&storeSensorSet),DIM_STRUCT_LENGTH_BYTE(RACK_STORE_SENSOR_SET));
    return;
}



