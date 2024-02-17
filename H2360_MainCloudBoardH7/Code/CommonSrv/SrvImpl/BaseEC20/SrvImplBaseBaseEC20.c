/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-03 16:12:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-09 10:30:11 +0800
************************************************************************************************/ 
#include "SrvImplBaseBaseEC20.h"
#include "CommonSrvTaskHeader.h"


//PING 服务器测试
void SrvImplBaseBaseEC20Ping(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //返回数据设计
    *dataBufLen = 0;
    *dataBufPtrPtr = NULL;
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //检查EC20设备状态
    EC20_MESSAGE* ec20MsgPtr = BoardEC20_GetDeviceMsg();
    EC20_DEVICE_STA currentDeviceState = ec20MsgPtr->deviceState;
    if(currentDeviceState < EC20_DEVICE_STA_CONFIG)
    {
        //设备不存在
        SystemPrintf("%s,EC20 ModuleStateErr,Stage: %d,Error:0X%08X\r\n",__func__,currentDeviceState,
                        ERROR_SUB_BOARD_EC20_MOD_NOT_EXIST);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_MOD_NOT_EXIST,
                                        ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_EC20_MOD_NOT_EXIST);
        return;
    }
    else if(currentDeviceState < EC20_DEVICE_STA_CHK_SIM_RDY)
    {
        //模块未准备好
        SystemPrintf("%s,EC20 ModuleStateErr,Stage: %d,Error:0X%08X\r\n",__func__,currentDeviceState,
                        ERROR_SUB_BOARD_EC20_DEV_NOT_RDY);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_DEV_NOT_RDY,
                                        ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_EC20_DEV_NOT_RDY);
        return;
    }
    else if(currentDeviceState < EC20_DEVICE_STA_SET_CREG)
    {
        //卡未准备好
        SystemPrintf("%s,EC20 ModuleStateErr,Stage: %d,Error:0X%08X\r\n",__func__,currentDeviceState,
                        ERROR_SUB_BOARD_EC20_SIM_NOT_RDY);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_SIM_NOT_RDY,
                                        ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_EC20_SIM_NOT_RDY);
        return;
    }
    else if(currentDeviceState < EC20_DEVICE_STA_GET_MSG)
    {
        //网络未准备好
        SystemPrintf("%s,EC20 ModuleStateErr,Stage: %d,Error:0X%08X\r\n",__func__,currentDeviceState,
                        ERROR_SUB_BOARD_EC20_NET_NOT_RDY);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_NET_NOT_RDY,
                                        ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_EC20_NET_NOT_RDY);
        return;
    }
    else if(currentDeviceState < EC20_DEVICE_STA_OK)
    {
        //网络不通
        SystemPrintf("%s,EC20 ModuleStateErr,Stage: %d,Error:0X%08X\r\n",__func__,currentDeviceState,
                        ERROR_SUB_BOARD_EC20_NET_COMM_FAIL);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_NET_COMM_FAIL,
                                        ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_EC20_NET_COMM_FAIL);
        return;
    }
    //检验合格,开始测试
    errorSubCode = BoardEC20_CheckNetPingBaidu();
    if(errorSubCode != ERROR_SUB_OK)
    {
        SystemPrintf("%s,BoardEC20_CheckNetPingBaidu,Stage: %d,Error:0X%08X\r\n",__func__,
                        currentDeviceState,errorSubCode);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_DEBUG,
                                        ERROR_LEVEL_WARNING,errorSubCode);
        return;
    }
    //正常完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//获取模块厂商信息
void SrvImplBaseBaseEC20QueryManufacturerName(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //检查EC20设备状态
    EC20_MESSAGE* ec20MsgPtr = BoardEC20_GetDeviceMsg();
    EC20_DEVICE_STA currentDeviceState = ec20MsgPtr->deviceState;
    if(currentDeviceState < EC20_DEVICE_STA_CONFIG)
    {
        //设备不存在
        SystemPrintf("%s,EC20 ModuleStateErr,Stage: %d,Error:0X%08X\r\n",__func__,currentDeviceState,
                        ERROR_SUB_BOARD_EC20_MOD_NOT_EXIST);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_MOD_NOT_EXIST,
                                        ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_EC20_MOD_NOT_EXIST);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    else if(currentDeviceState < EC20_DEVICE_STA_CHK_SIM_RDY)
    {
        //模块未准备好
        SystemPrintf("%s,EC20 ModuleStateErr,Stage: %d,Error:0X%08X\r\n",__func__,currentDeviceState,
                        ERROR_SUB_BOARD_EC20_DEV_NOT_RDY);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_DEV_NOT_RDY,
                                        ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_EC20_DEV_NOT_RDY);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    //检验合格,开始测试
    errorSubCode = BoardEC20_GetManufacturerDesp();
    if(errorSubCode != ERROR_SUB_OK)
    {
        SystemPrintf("%s,BoardEC20_GetManufacturerDesp,Stage: %d,Error:0X%08X\r\n",__func__,
                        currentDeviceState,errorSubCode);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_DEBUG,
                                        ERROR_LEVEL_WARNING,errorSubCode);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    //结果长度
    uint16_t returnBufferLength = strlen((const char*)ec20MsgPtr->manufacturerString);
    //内存设定
    *dataBufLen = 2 + returnBufferLength;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
    //写入数据长度
    ProtUtilWriteUint16ToBuffer(returnBufferLength,*dataBufPtrPtr);
    if(returnBufferLength != 0)
    {
        UserMemCopy((*dataBufPtrPtr)+2,ec20MsgPtr->manufacturerString,returnBufferLength);
    }
    //正常完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//获取模块类型信息
void SrvImplBaseBaseEC20QueryModuleType(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //检查EC20设备状态
    EC20_MESSAGE* ec20MsgPtr = BoardEC20_GetDeviceMsg();
    EC20_DEVICE_STA currentDeviceState = ec20MsgPtr->deviceState;
    if(currentDeviceState < EC20_DEVICE_STA_CONFIG)
    {
        //设备不存在
        SystemPrintf("%s,EC20 ModuleStateErr,Stage: %d,Error:0X%08X\r\n",__func__,currentDeviceState,
                        ERROR_SUB_BOARD_EC20_MOD_NOT_EXIST);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_MOD_NOT_EXIST,
                                        ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_EC20_MOD_NOT_EXIST);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    else if(currentDeviceState < EC20_DEVICE_STA_CHK_SIM_RDY)
    {
        //模块未准备好
        SystemPrintf("%s,EC20 ModuleStateErr,Stage: %d,Error:0X%08X\r\n",__func__,currentDeviceState,
                        ERROR_SUB_BOARD_EC20_DEV_NOT_RDY);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_DEV_NOT_RDY,
                                        ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_EC20_DEV_NOT_RDY);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    //检验合格,开始测试
    errorSubCode = BoardEC20_GetModuleTypeDesp();
    if(errorSubCode != ERROR_SUB_OK)
    {
        SystemPrintf("%s,BoardEC20_GetModuleTypeDesp,Stage: %d,Error:0X%08X\r\n",__func__,
                        currentDeviceState,errorSubCode);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_DEBUG,
                                        ERROR_LEVEL_WARNING,errorSubCode);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    //结果长度
    uint16_t returnBufferLength = strlen((const char*)ec20MsgPtr->moduleTypeString);
    //内存设定
    *dataBufLen = 2 + returnBufferLength;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
    //写入数据长度
    ProtUtilWriteUint16ToBuffer(returnBufferLength,*dataBufPtrPtr);
    if(returnBufferLength != 0)
    {
        UserMemCopy((*dataBufPtrPtr)+2,ec20MsgPtr->moduleTypeString,returnBufferLength);
    }
    //正常完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//获取模块软件版本信息
void SrvImplBaseBaseEC20QuerySoftwareVersion(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //检查EC20设备状态
    EC20_MESSAGE* ec20MsgPtr = BoardEC20_GetDeviceMsg();
    EC20_DEVICE_STA currentDeviceState = ec20MsgPtr->deviceState;
    if(currentDeviceState < EC20_DEVICE_STA_CONFIG)
    {
        //设备不存在
        SystemPrintf("%s,EC20 ModuleStateErr,Stage: %d,Error:0X%08X\r\n",__func__,currentDeviceState,
                        ERROR_SUB_BOARD_EC20_MOD_NOT_EXIST);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_MOD_NOT_EXIST,
                                        ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_EC20_MOD_NOT_EXIST);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    else if(currentDeviceState < EC20_DEVICE_STA_CHK_SIM_RDY)
    {
        //模块未准备好
        SystemPrintf("%s,EC20 ModuleStateErr,Stage: %d,Error:0X%08X\r\n",__func__,currentDeviceState,
                        ERROR_SUB_BOARD_EC20_DEV_NOT_RDY);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_DEV_NOT_RDY,
                                        ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_EC20_DEV_NOT_RDY);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    //检验合格,开始测试
    errorSubCode = BoardEC20_GetSoftwareVersionDesp();
    if(errorSubCode != ERROR_SUB_OK)
    {
        SystemPrintf("%s,BoardEC20_GetSoftwareVersionDesp,Stage: %d,Error:0X%08X\r\n",__func__,
                        currentDeviceState,errorSubCode);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_DEBUG,
                                        ERROR_LEVEL_WARNING,errorSubCode);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    //结果长度
    uint16_t returnBufferLength = strlen((const char*)ec20MsgPtr->softwareVersion);
    //内存设定
    *dataBufLen = 2 + returnBufferLength;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
    //写入数据长度
    ProtUtilWriteUint16ToBuffer(returnBufferLength,*dataBufPtrPtr);
    if(returnBufferLength != 0)
    {
        UserMemCopy((*dataBufPtrPtr)+2,ec20MsgPtr->softwareVersion,returnBufferLength);
    }
    //正常完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//获取模块入网许可
void SrvImplBaseBaseEC20QueryModuleIMEI(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //检查EC20设备状态
    EC20_MESSAGE* ec20MsgPtr = BoardEC20_GetDeviceMsg();
    EC20_DEVICE_STA currentDeviceState = ec20MsgPtr->deviceState;
    if(currentDeviceState < EC20_DEVICE_STA_CONFIG)
    {
        //设备不存在
        SystemPrintf("%s,EC20 ModuleStateErr,Stage: %d,Error:0X%08X\r\n",__func__,currentDeviceState,
                        ERROR_SUB_BOARD_EC20_MOD_NOT_EXIST);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_MOD_NOT_EXIST,
                                        ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_EC20_MOD_NOT_EXIST);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    else if(currentDeviceState < EC20_DEVICE_STA_CHK_SIM_RDY)
    {
        //模块未准备好
        SystemPrintf("%s,EC20 ModuleStateErr,Stage: %d,Error:0X%08X\r\n",__func__,currentDeviceState,
                        ERROR_SUB_BOARD_EC20_DEV_NOT_RDY);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_DEV_NOT_RDY,
                                        ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_EC20_DEV_NOT_RDY);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    //检验合格,开始测试
    errorSubCode = BoardEC20_GetIMEICodeDesp();
    if(errorSubCode != ERROR_SUB_OK)
    {
        SystemPrintf("%s,BoardEC20_GetIMEICodeDesp,Stage: %d,Error:0X%08X\r\n",__func__,
                        currentDeviceState,errorSubCode);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_DEBUG,
                                        ERROR_LEVEL_WARNING,errorSubCode);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    //结果长度
    uint16_t returnBufferLength = strlen((const char*)ec20MsgPtr->imeiCodeString);
    //内存设定
    *dataBufLen = 2 + returnBufferLength;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
    //写入数据长度
    ProtUtilWriteUint16ToBuffer(returnBufferLength,*dataBufPtrPtr);
    if(returnBufferLength != 0)
    {
        UserMemCopy((*dataBufPtrPtr)+2,ec20MsgPtr->imeiCodeString,returnBufferLength);
    }
    //正常完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//获取SIM卡IMSI串号
void SrvImplBaseBaseEC20QuerySimIMSI(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //检查EC20设备状态
    EC20_MESSAGE* ec20MsgPtr = BoardEC20_GetDeviceMsg();
    EC20_DEVICE_STA currentDeviceState = ec20MsgPtr->deviceState;
    if(currentDeviceState < EC20_DEVICE_STA_CONFIG)
    {
        //设备不存在
        SystemPrintf("%s,EC20 ModuleStateErr,Stage: %d,Error:0X%08X\r\n",__func__,currentDeviceState,
                        ERROR_SUB_BOARD_EC20_MOD_NOT_EXIST);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_MOD_NOT_EXIST,
                                        ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_EC20_MOD_NOT_EXIST);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    else if(currentDeviceState < EC20_DEVICE_STA_CHK_SIM_RDY)
    {
        //模块未准备好
        SystemPrintf("%s,EC20 ModuleStateErr,Stage: %d,Error:0X%08X\r\n",__func__,currentDeviceState,
                        ERROR_SUB_BOARD_EC20_DEV_NOT_RDY);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_DEV_NOT_RDY,
                                        ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_EC20_DEV_NOT_RDY);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    else if(currentDeviceState < EC20_DEVICE_STA_SET_CREG)
    {
        //卡未准备好
        SystemPrintf("%s,EC20 ModuleStateErr,Stage: %d,Error:0X%08X\r\n",__func__,currentDeviceState,
                        ERROR_SUB_BOARD_EC20_SIM_NOT_RDY);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_SIM_NOT_RDY,
                                        ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_EC20_SIM_NOT_RDY);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    //检验合格,开始测试
    errorSubCode = BoardEC20_GetIMSICodeDesp();
    if(errorSubCode != ERROR_SUB_OK)
    {
        SystemPrintf("%s,BoardEC20_GetIMSICodeDesp,Stage: %d,Error:0X%08X\r\n",__func__,
                        currentDeviceState,errorSubCode);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_DEBUG,
                                        ERROR_LEVEL_WARNING,errorSubCode);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    //结果长度
    uint16_t returnBufferLength = strlen((const char*)ec20MsgPtr->imsiCodeString);
    //内存设定
    *dataBufLen = 2 + returnBufferLength;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
    //写入数据长度
    ProtUtilWriteUint16ToBuffer(returnBufferLength,*dataBufPtrPtr);
    if(returnBufferLength != 0)
    {
        UserMemCopy((*dataBufPtrPtr)+2,ec20MsgPtr->imsiCodeString,returnBufferLength);
    }
    //正常完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//获取SIM卡ICCID识别号
void SrvImplBaseBaseEC20QuerySimICCID(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //检查EC20设备状态
    EC20_MESSAGE* ec20MsgPtr = BoardEC20_GetDeviceMsg();
    EC20_DEVICE_STA currentDeviceState = ec20MsgPtr->deviceState;
    if(currentDeviceState < EC20_DEVICE_STA_CONFIG)
    {
        //设备不存在
        SystemPrintf("%s,EC20 ModuleStateErr,Stage: %d,Error:0X%08X\r\n",__func__,currentDeviceState,
                        ERROR_SUB_BOARD_EC20_MOD_NOT_EXIST);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_MOD_NOT_EXIST,
                                        ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_EC20_MOD_NOT_EXIST);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    else if(currentDeviceState < EC20_DEVICE_STA_CHK_SIM_RDY)
    {
        //模块未准备好
        SystemPrintf("%s,EC20 ModuleStateErr,Stage: %d,Error:0X%08X\r\n",__func__,currentDeviceState,
                        ERROR_SUB_BOARD_EC20_DEV_NOT_RDY);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_DEV_NOT_RDY,
                                        ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_EC20_DEV_NOT_RDY);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    else if(currentDeviceState < EC20_DEVICE_STA_SET_CREG)
    {
        //卡未准备好
        SystemPrintf("%s,EC20 ModuleStateErr,Stage: %d,Error:0X%08X\r\n",__func__,currentDeviceState,
                        ERROR_SUB_BOARD_EC20_SIM_NOT_RDY);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_SIM_NOT_RDY,
                                        ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_EC20_SIM_NOT_RDY);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    //检验合格,开始测试
    errorSubCode = BoardEC20_GetICCIDCodeDesp();
    if(errorSubCode != ERROR_SUB_OK)
    {
        SystemPrintf("%s,BoardEC20_GetICCIDCodeDesp,Stage: %d,Error:0X%08X\r\n",__func__,
                        currentDeviceState,errorSubCode);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_DEBUG,
                                        ERROR_LEVEL_WARNING,errorSubCode);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    //结果长度
    uint16_t returnBufferLength = strlen((const char*)ec20MsgPtr->iccidCodeString);
    //内存设定
    *dataBufLen = 2 + returnBufferLength;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
    //写入数据长度
    ProtUtilWriteUint16ToBuffer(returnBufferLength,*dataBufPtrPtr);
    if(returnBufferLength != 0)
    {
        UserMemCopy((*dataBufPtrPtr)+2,ec20MsgPtr->iccidCodeString,returnBufferLength);
    }
    //正常完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//获取当前模块连接网络全名
void SrvImplBaseBaseEC20QueryNetFullName(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //检查EC20设备状态
    EC20_MESSAGE* ec20MsgPtr = BoardEC20_GetDeviceMsg();
    EC20_DEVICE_STA currentDeviceState = ec20MsgPtr->deviceState;
    if(currentDeviceState < EC20_DEVICE_STA_CONFIG)
    {
        //设备不存在
        SystemPrintf("%s,EC20 ModuleStateErr,Stage: %d,Error:0X%08X\r\n",__func__,currentDeviceState,
                        ERROR_SUB_BOARD_EC20_MOD_NOT_EXIST);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_MOD_NOT_EXIST,
                                        ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_EC20_MOD_NOT_EXIST);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    else if(currentDeviceState < EC20_DEVICE_STA_CHK_SIM_RDY)
    {
        //模块未准备好
        SystemPrintf("%s,EC20 ModuleStateErr,Stage: %d,Error:0X%08X\r\n",__func__,currentDeviceState,
                        ERROR_SUB_BOARD_EC20_DEV_NOT_RDY);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_DEV_NOT_RDY,
                                        ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_EC20_DEV_NOT_RDY);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    else if(currentDeviceState < EC20_DEVICE_STA_SET_CREG)
    {
        //卡未准备好
        SystemPrintf("%s,EC20 ModuleStateErr,Stage: %d,Error:0X%08X\r\n",__func__,currentDeviceState,
                        ERROR_SUB_BOARD_EC20_SIM_NOT_RDY);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_SIM_NOT_RDY,
                                        ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_EC20_SIM_NOT_RDY);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    else if(currentDeviceState < EC20_DEVICE_STA_GET_MSG)
    {
        //网络未准备好
        SystemPrintf("%s,EC20 ModuleStateErr,Stage: %d,Error:0X%08X\r\n",__func__,currentDeviceState,
                        ERROR_SUB_BOARD_EC20_NET_NOT_RDY);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_NET_NOT_RDY,
                                        ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_EC20_NET_NOT_RDY);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    //检验合格,开始测试
    errorSubCode = BoardEC20_GetNetNameDesp();
    if(errorSubCode != ERROR_SUB_OK)
    {
        SystemPrintf("%s,BoardEC20_GetNetNameDesp,Stage: %d,Error:0X%08X\r\n",__func__,
                        currentDeviceState,errorSubCode);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_DEBUG,
                                        ERROR_LEVEL_WARNING,errorSubCode);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    //结果长度
    uint16_t returnBufferLength = strlen((const char*)ec20MsgPtr->fullNetWorkNameString);
    //内存设定
    *dataBufLen = 2 + returnBufferLength;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
    //写入数据长度
    ProtUtilWriteUint16ToBuffer(returnBufferLength,*dataBufPtrPtr);
    if(returnBufferLength != 0)
    {
        UserMemCopy((*dataBufPtrPtr)+2,ec20MsgPtr->fullNetWorkNameString,returnBufferLength);
    }
    //正常完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//获取当前模块连接网络短名
void SrvImplBaseBaseEC20QueryNetShortName(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //检查EC20设备状态
    EC20_MESSAGE* ec20MsgPtr = BoardEC20_GetDeviceMsg();
    EC20_DEVICE_STA currentDeviceState = ec20MsgPtr->deviceState;
    if(currentDeviceState < EC20_DEVICE_STA_CONFIG)
    {
        //设备不存在
        SystemPrintf("%s,EC20 ModuleStateErr,Stage: %d,Error:0X%08X\r\n",__func__,currentDeviceState,
                        ERROR_SUB_BOARD_EC20_MOD_NOT_EXIST);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_MOD_NOT_EXIST,
                                        ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_EC20_MOD_NOT_EXIST);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    else if(currentDeviceState < EC20_DEVICE_STA_CHK_SIM_RDY)
    {
        //模块未准备好
        SystemPrintf("%s,EC20 ModuleStateErr,Stage: %d,Error:0X%08X\r\n",__func__,currentDeviceState,
                        ERROR_SUB_BOARD_EC20_DEV_NOT_RDY);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_DEV_NOT_RDY,
                                        ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_EC20_DEV_NOT_RDY);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    else if(currentDeviceState < EC20_DEVICE_STA_SET_CREG)
    {
        //卡未准备好
        SystemPrintf("%s,EC20 ModuleStateErr,Stage: %d,Error:0X%08X\r\n",__func__,currentDeviceState,
                        ERROR_SUB_BOARD_EC20_SIM_NOT_RDY);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_SIM_NOT_RDY,
                                        ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_EC20_SIM_NOT_RDY);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    else if(currentDeviceState < EC20_DEVICE_STA_GET_MSG)
    {
        //网络未准备好
        SystemPrintf("%s,EC20 ModuleStateErr,Stage: %d,Error:0X%08X\r\n",__func__,currentDeviceState,
                        ERROR_SUB_BOARD_EC20_NET_NOT_RDY);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_NET_NOT_RDY,
                                        ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_EC20_NET_NOT_RDY);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    //检验合格,开始测试
    errorSubCode = BoardEC20_GetNetNameDesp();
    if(errorSubCode != ERROR_SUB_OK)
    {
        SystemPrintf("%s,BoardEC20_GetNetNameDesp,Stage: %d,Error:0X%08X\r\n",__func__,
                        currentDeviceState,errorSubCode);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_DEBUG,
                                        ERROR_LEVEL_WARNING,errorSubCode);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    //结果长度
    uint16_t returnBufferLength = strlen((const char*)ec20MsgPtr->shortNetworkNameString);
    //内存设定
    *dataBufLen = 2 + returnBufferLength;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
    //写入数据长度
    ProtUtilWriteUint16ToBuffer(returnBufferLength,*dataBufPtrPtr);
    if(returnBufferLength != 0)
    {
        UserMemCopy((*dataBufPtrPtr)+2,ec20MsgPtr->shortNetworkNameString,returnBufferLength);
    }
    //正常完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//获取网络服务商名称
void SrvImplBaseBaseEC20QueryNetProvide(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //检查EC20设备状态
    EC20_MESSAGE* ec20MsgPtr = BoardEC20_GetDeviceMsg();
    EC20_DEVICE_STA currentDeviceState = ec20MsgPtr->deviceState;
    if(currentDeviceState < EC20_DEVICE_STA_CONFIG)
    {
        //设备不存在
        SystemPrintf("%s,EC20 ModuleStateErr,Stage: %d,Error:0X%08X\r\n",__func__,currentDeviceState,
                        ERROR_SUB_BOARD_EC20_MOD_NOT_EXIST);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_MOD_NOT_EXIST,
                                        ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_EC20_MOD_NOT_EXIST);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    else if(currentDeviceState < EC20_DEVICE_STA_CHK_SIM_RDY)
    {
        //模块未准备好
        SystemPrintf("%s,EC20 ModuleStateErr,Stage: %d,Error:0X%08X\r\n",__func__,currentDeviceState,
                        ERROR_SUB_BOARD_EC20_DEV_NOT_RDY);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_DEV_NOT_RDY,
                                        ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_EC20_DEV_NOT_RDY);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    else if(currentDeviceState < EC20_DEVICE_STA_SET_CREG)
    {
        //卡未准备好
        SystemPrintf("%s,EC20 ModuleStateErr,Stage: %d,Error:0X%08X\r\n",__func__,currentDeviceState,
                        ERROR_SUB_BOARD_EC20_SIM_NOT_RDY);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_SIM_NOT_RDY,
                                        ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_EC20_SIM_NOT_RDY);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    else if(currentDeviceState < EC20_DEVICE_STA_GET_MSG)
    {
        //网络未准备好
        SystemPrintf("%s,EC20 ModuleStateErr,Stage: %d,Error:0X%08X\r\n",__func__,currentDeviceState,
                        ERROR_SUB_BOARD_EC20_NET_NOT_RDY);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_NET_NOT_RDY,
                                        ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_EC20_NET_NOT_RDY);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    //检验合格,开始测试
    errorSubCode = BoardEC20_GetNetNameDesp();
    if(errorSubCode != ERROR_SUB_OK)
    {
        SystemPrintf("%s,BoardEC20_GetNetNameDesp,Stage: %d,Error:0X%08X\r\n",__func__,
                        currentDeviceState,errorSubCode);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_DEBUG,
                                        ERROR_LEVEL_WARNING,errorSubCode);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    //结果长度
    uint16_t returnBufferLength = strlen((const char*)ec20MsgPtr->netServiceProviderNameString);
    //内存设定
    *dataBufLen = 2 + returnBufferLength;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
    //写入数据长度
    ProtUtilWriteUint16ToBuffer(returnBufferLength,*dataBufPtrPtr);
    if(returnBufferLength != 0)
    {
        UserMemCopy((*dataBufPtrPtr)+2,ec20MsgPtr->netServiceProviderNameString,returnBufferLength);
    }
    //正常完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//获取当前网络制式
void SrvImplBaseBaseEC20QueryNetType(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //检查EC20设备状态
    EC20_MESSAGE* ec20MsgPtr = BoardEC20_GetDeviceMsg();
    EC20_DEVICE_STA currentDeviceState = ec20MsgPtr->deviceState;
    if(currentDeviceState < EC20_DEVICE_STA_CONFIG)
    {
        //设备不存在
        SystemPrintf("%s,EC20 ModuleStateErr,Stage: %d,Error:0X%08X\r\n",__func__,currentDeviceState,
                        ERROR_SUB_BOARD_EC20_MOD_NOT_EXIST);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_MOD_NOT_EXIST,
                                        ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_EC20_MOD_NOT_EXIST);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    else if(currentDeviceState < EC20_DEVICE_STA_CHK_SIM_RDY)
    {
        //模块未准备好
        SystemPrintf("%s,EC20 ModuleStateErr,Stage: %d,Error:0X%08X\r\n",__func__,currentDeviceState,
                        ERROR_SUB_BOARD_EC20_DEV_NOT_RDY);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_DEV_NOT_RDY,
                                        ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_EC20_DEV_NOT_RDY);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    else if(currentDeviceState < EC20_DEVICE_STA_SET_CREG)
    {
        //卡未准备好
        SystemPrintf("%s,EC20 ModuleStateErr,Stage: %d,Error:0X%08X\r\n",__func__,currentDeviceState,
                        ERROR_SUB_BOARD_EC20_SIM_NOT_RDY);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_SIM_NOT_RDY,
                                        ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_EC20_SIM_NOT_RDY);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    else if(currentDeviceState < EC20_DEVICE_STA_GET_MSG)
    {
        //网络未准备好
        SystemPrintf("%s,EC20 ModuleStateErr,Stage: %d,Error:0X%08X\r\n",__func__,currentDeviceState,
                        ERROR_SUB_BOARD_EC20_NET_NOT_RDY);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_NET_NOT_RDY,
                                        ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_EC20_NET_NOT_RDY);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    //检验合格,开始测试
    errorSubCode = BoardEC20_GetNetTypeDesp();
    if(errorSubCode != ERROR_SUB_OK)
    {
        SystemPrintf("%s,BoardEC20_GetNetTypeDesp,Stage: %d,Error:0X%08X\r\n",__func__,
                        currentDeviceState,errorSubCode);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_DEBUG,
                                        ERROR_LEVEL_WARNING,errorSubCode);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    //结果长度
    uint16_t returnBufferLength = strlen((const char*)ec20MsgPtr->networkModeString);
    //内存设定
    *dataBufLen = 2 + returnBufferLength;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
    //写入数据长度
    ProtUtilWriteUint16ToBuffer(returnBufferLength,*dataBufPtrPtr);
    if(returnBufferLength != 0)
    {
        UserMemCopy((*dataBufPtrPtr)+2,ec20MsgPtr->networkModeString,returnBufferLength);
    }
    //正常完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//获取当前网络波段
void SrvImplBaseBaseEC20QueryNetBaud(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //检查EC20设备状态
    EC20_MESSAGE* ec20MsgPtr = BoardEC20_GetDeviceMsg();
    EC20_DEVICE_STA currentDeviceState = ec20MsgPtr->deviceState;
    if(currentDeviceState < EC20_DEVICE_STA_CONFIG)
    {
        //设备不存在
        SystemPrintf("%s,EC20 ModuleStateErr,Stage: %d,Error:0X%08X\r\n",__func__,currentDeviceState,
                        ERROR_SUB_BOARD_EC20_MOD_NOT_EXIST);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_MOD_NOT_EXIST,
                                        ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_EC20_MOD_NOT_EXIST);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    else if(currentDeviceState < EC20_DEVICE_STA_CHK_SIM_RDY)
    {
        //模块未准备好
        SystemPrintf("%s,EC20 ModuleStateErr,Stage: %d,Error:0X%08X\r\n",__func__,currentDeviceState,
                        ERROR_SUB_BOARD_EC20_DEV_NOT_RDY);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_DEV_NOT_RDY,
                                        ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_EC20_DEV_NOT_RDY);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    else if(currentDeviceState < EC20_DEVICE_STA_SET_CREG)
    {
        //卡未准备好
        SystemPrintf("%s,EC20 ModuleStateErr,Stage: %d,Error:0X%08X\r\n",__func__,currentDeviceState,
                        ERROR_SUB_BOARD_EC20_SIM_NOT_RDY);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_SIM_NOT_RDY,
                                        ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_EC20_SIM_NOT_RDY);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    else if(currentDeviceState < EC20_DEVICE_STA_GET_MSG)
    {
        //网络未准备好
        SystemPrintf("%s,EC20 ModuleStateErr,Stage: %d,Error:0X%08X\r\n",__func__,currentDeviceState,
                        ERROR_SUB_BOARD_EC20_NET_NOT_RDY);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_NET_NOT_RDY,
                                        ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_EC20_NET_NOT_RDY);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    //检验合格,开始测试
    errorSubCode = BoardEC20_GetNetTypeDesp();
    if(errorSubCode != ERROR_SUB_OK)
    {
        SystemPrintf("%s,BoardEC20_GetNetTypeDesp,Stage: %d,Error:0X%08X\r\n",__func__,
                        currentDeviceState,errorSubCode);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_DEBUG,
                                        ERROR_LEVEL_WARNING,errorSubCode);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    //结果长度
    uint16_t returnBufferLength = strlen((const char*)ec20MsgPtr->networkBandString);
    //内存设定
    *dataBufLen = 2 + returnBufferLength;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
    //写入数据长度
    ProtUtilWriteUint16ToBuffer(returnBufferLength,*dataBufPtrPtr);
    if(returnBufferLength != 0)
    {
        UserMemCopy((*dataBufPtrPtr)+2,ec20MsgPtr->networkBandString,returnBufferLength);
    }
    //正常完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//获取LAC基站区域码
void SrvImplBaseBaseEC20QueryBaseStationLAC(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //检查EC20设备状态
    EC20_MESSAGE* ec20MsgPtr = BoardEC20_GetDeviceMsg();
    EC20_DEVICE_STA currentDeviceState = ec20MsgPtr->deviceState;
    if(currentDeviceState < EC20_DEVICE_STA_CONFIG)
    {
        //设备不存在
        SystemPrintf("%s,EC20 ModuleStateErr,Stage: %d,Error:0X%08X\r\n",__func__,currentDeviceState,
                        ERROR_SUB_BOARD_EC20_MOD_NOT_EXIST);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_MOD_NOT_EXIST,
                                        ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_EC20_MOD_NOT_EXIST);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    else if(currentDeviceState < EC20_DEVICE_STA_CHK_SIM_RDY)
    {
        //模块未准备好
        SystemPrintf("%s,EC20 ModuleStateErr,Stage: %d,Error:0X%08X\r\n",__func__,currentDeviceState,
                        ERROR_SUB_BOARD_EC20_DEV_NOT_RDY);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_DEV_NOT_RDY,
                                        ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_EC20_DEV_NOT_RDY);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    else if(currentDeviceState < EC20_DEVICE_STA_SET_CREG)
    {
        //卡未准备好
        SystemPrintf("%s,EC20 ModuleStateErr,Stage: %d,Error:0X%08X\r\n",__func__,currentDeviceState,
                        ERROR_SUB_BOARD_EC20_SIM_NOT_RDY);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_SIM_NOT_RDY,
                                        ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_EC20_SIM_NOT_RDY);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    else if(currentDeviceState < EC20_DEVICE_STA_GET_MSG)
    {
        //网络未准备好
        SystemPrintf("%s,EC20 ModuleStateErr,Stage: %d,Error:0X%08X\r\n",__func__,currentDeviceState,
                        ERROR_SUB_BOARD_EC20_NET_NOT_RDY);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_NET_NOT_RDY,
                                        ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_EC20_NET_NOT_RDY);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    //检验合格,开始测试
    errorSubCode = BoardEC20_CheckNetBaseStationMsg();
    if(errorSubCode != ERROR_SUB_OK)
    {
        SystemPrintf("%s,BoardEC20_CheckNetBaseStationMsg,Stage: %d,Error:0X%08X\r\n",__func__,
                        currentDeviceState,errorSubCode);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_DEBUG,
                                        ERROR_LEVEL_WARNING,errorSubCode);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    //结果长度
    uint16_t returnBufferLength = strlen((const char*)ec20MsgPtr->baseStationCodeStringLAC);
    //内存设定
    *dataBufLen = 2 + returnBufferLength;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
    //写入数据长度
    ProtUtilWriteUint16ToBuffer(returnBufferLength,*dataBufPtrPtr);
    if(returnBufferLength != 0)
    {
        UserMemCopy((*dataBufPtrPtr)+2,ec20MsgPtr->baseStationCodeStringLAC,returnBufferLength);
    }
    //正常完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//获取CID基站编号
void SrvImplBaseBaseEC20QueryBaseStationCID(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //检查EC20设备状态
    EC20_MESSAGE* ec20MsgPtr = BoardEC20_GetDeviceMsg();
    EC20_DEVICE_STA currentDeviceState = ec20MsgPtr->deviceState;
    if(currentDeviceState < EC20_DEVICE_STA_CONFIG)
    {
        //设备不存在
        SystemPrintf("%s,EC20 ModuleStateErr,Stage: %d,Error:0X%08X\r\n",__func__,currentDeviceState,
                        ERROR_SUB_BOARD_EC20_MOD_NOT_EXIST);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_MOD_NOT_EXIST,
                                        ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_EC20_MOD_NOT_EXIST);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    else if(currentDeviceState < EC20_DEVICE_STA_CHK_SIM_RDY)
    {
        //模块未准备好
        SystemPrintf("%s,EC20 ModuleStateErr,Stage: %d,Error:0X%08X\r\n",__func__,currentDeviceState,
                        ERROR_SUB_BOARD_EC20_DEV_NOT_RDY);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_DEV_NOT_RDY,
                                        ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_EC20_DEV_NOT_RDY);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    else if(currentDeviceState < EC20_DEVICE_STA_SET_CREG)
    {
        //卡未准备好
        SystemPrintf("%s,EC20 ModuleStateErr,Stage: %d,Error:0X%08X\r\n",__func__,currentDeviceState,
                        ERROR_SUB_BOARD_EC20_SIM_NOT_RDY);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_SIM_NOT_RDY,
                                        ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_EC20_SIM_NOT_RDY);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    else if(currentDeviceState < EC20_DEVICE_STA_GET_MSG)
    {
        //网络未准备好
        SystemPrintf("%s,EC20 ModuleStateErr,Stage: %d,Error:0X%08X\r\n",__func__,currentDeviceState,
                        ERROR_SUB_BOARD_EC20_NET_NOT_RDY);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_NET_NOT_RDY,
                                        ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_EC20_NET_NOT_RDY);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    //检验合格,开始测试
    errorSubCode = BoardEC20_CheckNetBaseStationMsg();
    if(errorSubCode != ERROR_SUB_OK)
    {
        SystemPrintf("%s,BoardEC20_CheckNetBaseStationMsg,Stage: %d,Error:0X%08X\r\n",__func__,
                        currentDeviceState,errorSubCode);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_DEBUG,
                                        ERROR_LEVEL_WARNING,errorSubCode);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    //结果长度
    uint16_t returnBufferLength = strlen((const char*)ec20MsgPtr->baseStationCodeStringCID);
    //内存设定
    *dataBufLen = 2 + returnBufferLength;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
    //写入数据长度
    ProtUtilWriteUint16ToBuffer(returnBufferLength,*dataBufPtrPtr);
    if(returnBufferLength != 0)
    {
        UserMemCopy((*dataBufPtrPtr)+2,ec20MsgPtr->baseStationCodeStringCID,returnBufferLength);
    }
    //正常完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//获取当前信号强度与误码率
void SrvImplBaseBaseEC20QuerySignalStrength(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //检查EC20设备状态
    EC20_MESSAGE* ec20MsgPtr = BoardEC20_GetDeviceMsg();
    EC20_DEVICE_STA currentDeviceState = ec20MsgPtr->deviceState;
    if(currentDeviceState < EC20_DEVICE_STA_CONFIG)
    {
        //设备不存在
        SystemPrintf("%s,EC20 ModuleStateErr,Stage: %d,Error:0X%08X\r\n",__func__,currentDeviceState,
                        ERROR_SUB_BOARD_EC20_MOD_NOT_EXIST);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_MOD_NOT_EXIST,
                                        ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_EC20_MOD_NOT_EXIST);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    else if(currentDeviceState < EC20_DEVICE_STA_CHK_SIM_RDY)
    {
        //模块未准备好
        SystemPrintf("%s,EC20 ModuleStateErr,Stage: %d,Error:0X%08X\r\n",__func__,currentDeviceState,
                        ERROR_SUB_BOARD_EC20_DEV_NOT_RDY);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_DEV_NOT_RDY,
                                        ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_EC20_DEV_NOT_RDY);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    else if(currentDeviceState < EC20_DEVICE_STA_SET_CREG)
    {
        //卡未准备好
        SystemPrintf("%s,EC20 ModuleStateErr,Stage: %d,Error:0X%08X\r\n",__func__,currentDeviceState,
                        ERROR_SUB_BOARD_EC20_SIM_NOT_RDY);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_SIM_NOT_RDY,
                                        ERROR_LEVEL_WARNING,ERROR_SUB_BOARD_EC20_SIM_NOT_RDY);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    //检验合格,开始测试
    errorSubCode = BoardEC20_GetSignalStrength();
    if(errorSubCode != ERROR_SUB_OK)
    {
        SystemPrintf("%s,BoardEC20_GetSignalStrength,Stage: %d,Error:0X%08X\r\n",__func__,
                        currentDeviceState,errorSubCode);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_CLD_BASE_EC20_DEBUG,
                                        ERROR_LEVEL_WARNING,errorSubCode);
        //内存设定
        *dataBufLen = 2;
        *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
        //写入数据长度0
        ProtUtilWriteUint16ToBuffer(0,*dataBufPtrPtr);
        return;
    }
    //内存设定
    *dataBufLen = 2;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
    //写入数据
    (*dataBufPtrPtr)[0] = ec20MsgPtr->singalStrength;
    (*dataBufPtrPtr)[1] = ec20MsgPtr->errorValue;
    //正常完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//查看当前模块网络是否正常
void SrvImplBaseBaseEC20QueryNetState(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //返回数据设计
    *dataBufLen = 1;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
    //检查EC20设备状态
    EC20_MESSAGE* ec20MsgPtr = BoardEC20_GetDeviceMsg();
    EC20_DEVICE_STA currentDeviceState = ec20MsgPtr->deviceState;
    //设备状态打印
    SystemPrintf("%s,EC20 ModuleState: %d\r\n",__func__,currentDeviceState);
    if(currentDeviceState < EC20_DEVICE_STA_CONFIG)
    {
        //设备不存在
        (*dataBufPtrPtr)[0] = 1;
    }
    else if(currentDeviceState < EC20_DEVICE_STA_CHK_SIM_RDY)
    {
        //模块未准备好
        (*dataBufPtrPtr)[0] = 2;
    }
    else if(currentDeviceState < EC20_DEVICE_STA_SET_CREG)
    {
        //卡未准备好
        (*dataBufPtrPtr)[0] = 3;
    }
    else if(currentDeviceState < EC20_DEVICE_STA_GET_MSG)
    {
        //网络未准备好
        (*dataBufPtrPtr)[0] = 4;
    }
    else if(currentDeviceState < EC20_DEVICE_STA_GET_MSG)
    {
        //GPRS功能异常
        (*dataBufPtrPtr)[0] = 5;
    }
    else if(currentDeviceState < EC20_DEVICE_STA_PING_BAIDU)
    {
        //获取设备讯息异常
        (*dataBufPtrPtr)[0] = 7;
    }
    else if(currentDeviceState < EC20_DEVICE_STA_OK)
    {
        //网络不通
        (*dataBufPtrPtr)[0] = 6;
    }
    else
    {
        //设备网络正常
        (*dataBufPtrPtr)[0] = 0;
    }
    //正常完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//通讯设备重启
void SrvImplBaseBaseEC20ModuleRestart(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //返回数据设计
    *dataBufLen = 0;
    *dataBufPtrPtr = NULL;
    //检验合格,开始测试
    BoardEC20_ResetStart();
    //正常完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}






















