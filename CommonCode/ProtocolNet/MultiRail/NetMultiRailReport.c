#include "NetMultiRailReport.h"
#include "SysStateDataType.h"



//系统状态设置根据子状态
uint8_t SystemStateCheckContainState(MODULE_SYSTEM_STATE *sysReportUnitPtr,SYSTEM_STATE state)
{
    if(sysReportUnitPtr->sampleModuleLocalState == state)
    {
        return 1;
    }
    // if(sysReportUnitPtr->sampleModuleOnlineState == state)
    // {
    //     return 1;
    // }
    for(uint8_t i = 0; i < 4;i++)
    {
        if(sysReportUnitPtr->railTestArrayState[i] == state)
        {
            return 1;
        }
    }
    return 0;
}

//系统状态设置根据子状态
void SystemStateSetByModuleSubState(MODULE_SYSTEM_STATE *sysReportUnitPtr)
{
    if(SystemStateCheckContainState(sysReportUnitPtr,SYSTEM_STATE_ERROR_STOP))
    {
        sysReportUnitPtr->systemState = SYSTEM_STATE_ERROR_STOP;
    }
    else if(SystemStateCheckContainState(sysReportUnitPtr,SYSTEM_STATE_RACK_STOP))
    {
        sysReportUnitPtr->systemState = SYSTEM_STATE_RACK_STOP;
    }
    else if(SystemStateCheckContainState(sysReportUnitPtr,SYSTEM_STATE_RACK_PAUSE))
    {
        sysReportUnitPtr->systemState = SYSTEM_STATE_RACK_PAUSE;
    }
    else if(SystemStateCheckContainState(sysReportUnitPtr,SYSTEM_STATE_TEST))
    {
        sysReportUnitPtr->systemState = SYSTEM_STATE_TEST;
    }
    else if(SystemStateCheckContainState(sysReportUnitPtr,SYSTEM_STATE_RESET))
    {
        sysReportUnitPtr->systemState = SYSTEM_STATE_RESET;
    }
    else if(SystemStateCheckContainState(sysReportUnitPtr,SYSTEM_STATE_CLEAR_RACK))
    {
        sysReportUnitPtr->systemState = SYSTEM_STATE_CLEAR_RACK;
    }
    else if(SystemStateCheckContainState(sysReportUnitPtr,SYSTEM_STATE_RESET_FAIL))
    {
        sysReportUnitPtr->systemState = SYSTEM_STATE_RESET_FAIL;
    }
    else if(SystemStateCheckContainState(sysReportUnitPtr,SYSTEM_STATE_STANDBY))
    {
        sysReportUnitPtr->systemState = SYSTEM_STATE_STANDBY;
    }
    else 
    {
        sysReportUnitPtr->systemState = sysReportUnitPtr->sampleModuleLocalState;
    }
    //全部轨道异常，修改进样台异常
}

//系统状态上传
void NetMultiRailReportSystemState(MODULE_SYSTEM_STATE *sysReportUnitPtr)
{
	uint8_t reportBuff[12] = {0};
    //修正总状态
    SystemStateSetByModuleSubState(sysReportUnitPtr);
    //样本台状态
    ProtUtilWriteUint16ToBuffer(sysReportUnitPtr->systemState,reportBuff);
    //联机接口状态
    ProtUtilWriteUint16ToBuffer(sysReportUnitPtr->sampleModuleOnlineState,reportBuff+2);
    //轨道模块1
    ProtUtilWriteUint16ToBuffer(sysReportUnitPtr->railTestArrayState[0],reportBuff+4);
    //轨道模块2
    ProtUtilWriteUint16ToBuffer(sysReportUnitPtr->railTestArrayState[1],reportBuff+6);
    //轨道模块3
    ProtUtilWriteUint16ToBuffer(sysReportUnitPtr->railTestArrayState[2],reportBuff+8);
    //轨道模块4
    ProtUtilWriteUint16ToBuffer(sysReportUnitPtr->railTestArrayState[3],reportBuff+10);
    //上报状态
    NetMasterReportMsgToPC(NET_MASTER_REPORT_MULTI_RAIL_STATE,reportBuff,12);
}

//条码数据上传
void NetMultiRailReportTubeRackState(uint32_t rackID,uint8_t *tubeReportBuffer,uint16_t buferLength)
{
    uint8_t * bufferPtr = NULL;
    bufferPtr = NetProtMemMalloc(buferLength +8);
    ProtUtilWriteUint32ToBuffer(rackID,bufferPtr);
    for(uint8_t i= 0; i < RACK_TUBE_NUM_MAX; i++)
    {
        if(tubeReportBuffer[0]  & (0X01 << i))
        {
            bufferPtr[4+i] = 1;
        }
        else 
        {
            bufferPtr[4+i] = 0;
        }
    }
    UserMemCopy(bufferPtr+9,tubeReportBuffer,buferLength);
    NetMasterReportMsgToPC(NET_MASTER_REPORT_MULTI_RACK_TUBE_CODE,bufferPtr,buferLength+8);
}

//试管架进入测试轨道上报
void NetMultiRailReportRackEntryTargetRail(uint32_t rackID,uint8_t railIndex)
{
	uint8_t reportBuff[5] = {0};
    ProtUtilWriteUint32ToBuffer(rackID,reportBuff);
    reportBuff[4] = railIndex;
    NetMasterReportMsgToPC(NET_MASTER_REPORT_MULTI_RACK_ENTRY_RAIL,reportBuff,5);
}

//试管架离开测试轨道上报
void NetMultiRailReportRackLeaveTargetRail(uint32_t rackID,uint8_t railIndex)
{
    uint8_t reportBuff[5] = {0};
    ProtUtilWriteUint32ToBuffer(rackID,reportBuff);
    reportBuff[4] = railIndex;
    NetMasterReportMsgToPC(NET_MASTER_REPORT_MULTI_RACK_LEAVE_RAIL,reportBuff,5);
}

//试管架回到样本仓
void NetMultiRailReportRackBackSampleStore(uint32_t rackID,uint8_t trayIndex,uint8_t rackIndex)
{
    uint8_t reportBuff[6] = {0};
    ProtUtilWriteUint32ToBuffer(rackID,reportBuff);
    reportBuff[4] = trayIndex;
    reportBuff[5] = rackIndex;
    NetMasterReportMsgToPC(NET_MASTER_REPORT_MULTI_RACK_BACK_SAMPLE_STORE,reportBuff,6);
}
