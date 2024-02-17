#include "AppTestFlowDataType.h"
//设置样本仓结构默认值
void AppDataTypeDefault_SampleWarehouse(SAMPLE_WAREHOUSE_STATE* warehouse)
{
    uint8_t index = 0;
    warehouse->u32RackID = RACK_IVALID;
    for(index = 0; index < RACK_TUBE_NUM; index++)
    {
        warehouse->eTubeExist[index] = RACK_TUBE_NOT_EXIST;
        warehouse->eTubeState[index] =  TUBE_STATE_INCOMPLETE;
        memset(warehouse->u32BarCode[index], 0 , RACK_TUBE_BARCODE_LEN);
    }
    warehouse->eRackState = RACK_STATE_IDLE;
    warehouse->eBufferState = BUFFER_STATE_IDLE;
}

//设置C21请求结构默认值
void AppDataTypeDefault_C21Request(INST_C21_REQUEST* request)
{
    uint8_t index = 0;
    request->u32RackID = RACK_IVALID;
    request->eRequestState = INST_C21_REQUEST_STATE_IDLE;
    for(index = 0; index < RACK_TUBE_NUM; index++)
    {
        request->eTubeState[index] =  TUBE_STATE_INCOMPLETE;
    }
}
