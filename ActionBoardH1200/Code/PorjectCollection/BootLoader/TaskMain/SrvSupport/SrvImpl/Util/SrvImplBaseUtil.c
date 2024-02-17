/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-16 11:35:28
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-17 10:42:20
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#include "SrvImplBaseUtil.h"
#include "SrvImplUtilConfig.h"


//联机
void SrvImplBaseUtilOnline(PROT_CONNECT_FLAG protConnectFlag,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //返回内存长度设定
    *dataBufLen = 2;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,(*dataBufLen));
    uint8_t* resultBufferPtr = *dataBufPtrPtr;
    resultBufferPtr[0] = (uint8_t)protConnectFlag;
    resultBufferPtr[1] = (uint8_t)BOARD_RUN_MODE_BOOT;
    //构建完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
}

//脱机
void SrvImplBaseUtilOffLine(PROT_DISCONNECT_FLAG disconnectFlag,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //返回内存长度设定
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,1);
    *dataBufLen = 1;
    uint8_t* resultBufferPtr = *dataBufPtrPtr;
    resultBufferPtr[0] = (uint8_t)BOARD_RUN_MODE_BOOT;
    //构建完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
}

//UTC同步
void SrvImplBaseUtilUtcSync(uint64_t utcTimeStamp,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    RTC_TIMER* rtcTimePtr = NULL;
    errorSub = MCU_RTC_SyncTimeByUTC((uint32_t)(utcTimeStamp));
    if(errorSub != ERROR_SUB_OK)
    {
        //报错
        SystemPrintf("%s,MCU_RTC_SyncTimeByUTC ErrorSub: 0X%08X\r\n",__func__,errorSub);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_UTIL_UTC_SYNC_RTC,ERROR_LEVEL_WARNING,errorSub);
        return;
    }
    //获取打印当前时间
    errorSub = MCU_RTC_GetTimeRecordSync(&rtcTimePtr);
    if(errorSub != ERROR_SUB_OK)
    {
        //报错
        SystemPrintf("%s,MCU_RTC_GetTimeRecordSync ErrorSub: 0X%08X\r\n",__func__,errorSub);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_UTIL_UTC_SYNC_RTC,ERROR_LEVEL_WARNING,errorSub);
        return;
    }
    SystemPrintf("CurrentWeek: %d,CurrentTime %04d-%02d-%02d,%02d:%02d:%02d\r\n",rtcTimePtr->weekDay,
                    rtcTimePtr->year,rtcTimePtr->month,rtcTimePtr->day,rtcTimePtr->hour,rtcTimePtr->minute,
                    rtcTimePtr->second);
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
}

//心跳获取
void SrvImplBaseUtilHeartGet(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint32_t utcTimeStamp = 0;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,9);
    *dataBufLen = 9;
    uint8_t* resultBufferPtr = *dataBufPtrPtr;
    //获取时间
    errorSub = MCU_RTC_GetTimeCurrentUTC(&utcTimeStamp);
    if(errorSub != ERROR_SUB_OK)
    {
        //报错
        SystemPrintf("%s,MCU_RTC_GetTimeCurrentUTC ErrorSub: 0X%08X\r\n",__func__,errorSub);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_UTIL_GET_HEART_RTC,ERROR_LEVEL_WARNING,errorSub);
        return;
    }
    //写入时间
    ProtUtilWriteUint64ToBuffer((uint64_t)(utcTimeStamp*1000),resultBufferPtr);
    //写入模式
    resultBufferPtr[8] = (uint8_t)BOARD_RUN_MODE_BOOT;
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
}

//急停
void SrvImplBaseUtilEmStop(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
}

//获取版本号
void SrvImplBaseUtilGetVersion(SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    BOARD_SOFT_VERSION versionCode;
    ERROR_SUB errorSub = ERROR_SUB_OK;
    *dataBufPtrPtr = UserMemMallocWhileSuccess(MEM_REGION_SRV_TASK,21);
    *dataBufLen = 21;
    uint8_t* resultBufferPtr = *dataBufPtrPtr;
    //BOOT版本号
    errorSub = BoardParamReadBootVersion(&versionCode);
    if(errorSub != ERROR_SUB_OK)
    {
        //报错
        SystemPrintf("%s,BoardParamReadBootVersion ErrorSub: 0X%08X\r\n",__func__,errorSub);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_UTIL_READ_VERSION_BOOT,ERROR_LEVEL_WARNING,errorSub);
        return;
    }
    //打印版本号
    SystemPrintf("BootVersion: %d.%d.%d-%d\r\n",versionCode.mainVersion,versionCode.subVersion,versionCode.tempVersion,
                    versionCode.devVersion);
    //写入BOOT版本号
    resultBufferPtr[0] = (uint8_t)versionCode.mainVersion;
    resultBufferPtr[1] = (uint8_t)versionCode.subVersion;
    resultBufferPtr[2] = (uint8_t)versionCode.tempVersion;
    ProtUtilWriteUint32ToBuffer(versionCode.devVersion,resultBufferPtr+3);
    //SRV版本号
    errorSub = BoardParamReadServiceVersion(&versionCode);
    if(errorSub != ERROR_SUB_OK)
    {
        //报错
        SystemPrintf("%s,BoardParamReadServiceVersion ErrorSub: 0X%08X\r\n",__func__,errorSub);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_UTIL_READ_VERSION_SRV,ERROR_LEVEL_WARNING,errorSub);
        return;
    }
    //打印版本号
    SystemPrintf("SrvVersion: %d.%d.%d-%d\r\n",versionCode.mainVersion,versionCode.subVersion,versionCode.tempVersion,
                    versionCode.devVersion);
    resultBufferPtr[7] = (uint8_t)versionCode.mainVersion;
    resultBufferPtr[8] = (uint8_t)versionCode.subVersion;
    resultBufferPtr[9] = (uint8_t)versionCode.tempVersion;
    ProtUtilWriteUint32ToBuffer(versionCode.devVersion,resultBufferPtr+10);
    //APP版本号
    errorSub = BoardParamReadAppVersion(&versionCode);
    if(errorSub != ERROR_SUB_OK)
    {
        //报错
        SystemPrintf("%s,BoardParamReadAppVersion ErrorSub: 0X%08X\r\n",__func__,errorSub);
        //返回错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_UTIL_READ_VERSION_APP,ERROR_LEVEL_WARNING,errorSub);
        return;
    }
    //打印版本号
    SystemPrintf("AppVersion: %d.%d.%d-%d\r\n",versionCode.mainVersion,versionCode.subVersion,versionCode.tempVersion,
                    versionCode.devVersion);
    resultBufferPtr[14] = (uint8_t)versionCode.mainVersion;
    resultBufferPtr[15] = (uint8_t)versionCode.subVersion;
    resultBufferPtr[16] = (uint8_t)versionCode.tempVersion;
    ProtUtilWriteUint32ToBuffer(versionCode.devVersion,resultBufferPtr+17);
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
}

//设置运行模式
__weak void SrvImplBaseUtilSetRunningMode(SYS_RUNNING_MODE sysRunningMode,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    *dataBufPtrPtr = NULL;
    *dataBufLen = 0;
    //返回无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
}








