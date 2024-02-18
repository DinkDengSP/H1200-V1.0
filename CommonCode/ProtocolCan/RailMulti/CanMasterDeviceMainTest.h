#ifndef _CAN_MASTER_DEVICE_MAIN_TEST_H_
#define _CAN_MASTER_DEVICE_MAIN_TEST_H_

#include "CanRailMultiDataType.h"

/*************************************发送指令等待ACK************************************************/
//轨道主控 试管到位 Ack
void CanMasterDeviceMainTestTubeInPosWhileAck(RAIL_MODULE_NO railNo,RAIL_FUNC_TYPE railType,uint32_t rackId,uint8_t tubePos,SYS_RESULT_PACK* resultPackPtr);

//轨道主控 试管移动到位 Ack
void CanMasterDeviceMainTestTubeMovArriveWhileAck(RAIL_MODULE_NO railNo,RAIL_FUNC_TYPE railType,uint32_t rackId,uint8_t tubePos,uint8_t movResult,SYS_RESULT_PACK* resultPackPtr);

//轨道主控 设备试管置废 Ack
void CanMasterDeviceMainTestDeviceTubeDumpWhileAck(RAIL_MODULE_NO railNo,RAIL_FUNC_TYPE railType,uint32_t rackId,uint8_t tubePos,SYS_RESULT_PACK* resultPackPtr);

//轨道主控 轨道状态通知 Ack
void CanMasterDeviceMainRailStateInformWhileAck(RAIL_MODULE_NO railNo,RAIL_FUNC_TYPE railType,uint8_t errorFlag,SYS_RESULT_PACK* resultPackPtr);

//轨道主控 轨道变轨锁定结果 Ack
void CanMasterDeviceMainRailChangeLockResultWhileAck(RAIL_MODULE_NO railNo,RAIL_FUNC_TYPE railType,uint32_t rackID,uint8_t lockResult,SYS_RESULT_PACK* resultPackPtr);

//轨道主控 查询样本针状态 Ack
void CanMasterDeviceMainTestInquireNeedlePosWhileAck(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr);

/*************************************等待指定指令完成返回************************************************/
//轨道主控 试管到位 result
void CanMasterDeviceMainTestWaitTubeInPosResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,uint8_t *returnBuffer);

//轨道主控 查询样本针位置 result
void CanMasterDeviceMainTestWaitInquireNeedlePosResult(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,uint8_t *returnBuffer);

/*************************************发送指令等待完成返回************************************************/
//轨道主控 试管到位 return 
void CanMasterDeviceMainTestTubeInPosWhileReturn(RAIL_MODULE_NO railNo,RAIL_FUNC_TYPE railType,uint32_t rackId,uint8_t tubePos,SYS_RESULT_PACK* resultPackPtr,uint8_t *returnBuffer);

//轨道主控 查询样本针位置 return 
void CanMasterDeviceMainTestInquireNeedlePosWhileReturn(RAIL_MODULE_NO railNo,SYS_RESULT_PACK* resultPackPtr,uint8_t *returnBuffer);
#endif // _CAN_MASTER_DEVICE_MAIN_TEST_H_

