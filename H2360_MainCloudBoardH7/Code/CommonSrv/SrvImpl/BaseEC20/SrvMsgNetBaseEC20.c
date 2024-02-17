/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-03 16:12:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-08 16:35:21 +0800
************************************************************************************************/ 
#include "SrvImplBaseEC20.h"
#include "CommonSrvTaskHeader.h"
#include "SrvImplBaseBaseEC20.h"


/****************************************网口指令消息处理函数**************************************************/
//占位函数
static void SrvMsgNetBaseEC20Cmd_HoldPlace(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//PING 服务器测试
static void SrvMsgNetBaseEC20Cmd_Ping(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//获取模块厂商信息
static void SrvMsgNetBaseEC20Cmd_QueryManufacturerName(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//获取模块类型信息
static void SrvMsgNetBaseEC20Cmd_QueryModuleType(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//获取模块软件版本信息
static void SrvMsgNetBaseEC20Cmd_QuerySoftwareVersion(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//获取模块入网许可
static void SrvMsgNetBaseEC20Cmd_QueryModuleIMEI(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//获取SIM卡IMSI串号
static void SrvMsgNetBaseEC20Cmd_QuerySimIMSI(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//获取SIM卡ICCID识别号
static void SrvMsgNetBaseEC20Cmd_QuerySimICCID(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//获取当前模块连接网络全名
static void SrvMsgNetBaseEC20Cmd_QueryNetFullName(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//获取当前模块连接网络短名
static void SrvMsgNetBaseEC20Cmd_QueryNetShortName(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//获取网络服务商名称
static void SrvMsgNetBaseEC20Cmd_QueryNetProvide(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//获取当前网络制式
static void SrvMsgNetBaseEC20Cmd_QueryNetType(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//获取当前网络波段
static void SrvMsgNetBaseEC20Cmd_QueryNetBaud(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//获取LAC基站区域码
static void SrvMsgNetBaseEC20Cmd_QueryBaseStationLAC(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//获取CID基站编号
static void SrvMsgNetBaseEC20Cmd_QueryBaseStationCID(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//获取当前信号强度与误码率
static void SrvMsgNetBaseEC20Cmd_QuerySignalStrength(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//查看当前模块网络是否正常
static void SrvMsgNetBaseEC20Cmd_QueryNetState(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//通讯设备重启
static void SrvMsgNetBaseEC20Cmd_ModuleRestart(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//NET指令处理函数列表
static const USER_NET_CMD_PROC_UNIT netCmdProcBaseEC20UnitArray[] = 
{
    //顺序: 指令码  预期参数最小长度 是否需要结果返回  错误是否独立上报 指令处理函数
    {(uint32_t)NET_MASTER_CMD_HOLD_PLACE                            ,0  ,ENABLE ,SrvMsgNetBaseEC20Cmd_HoldPlace   },
    {(uint32_t)NET_MASTER_CMD_CLD_BASE_PING_BAIDU                   ,0  ,ENABLE ,SrvMsgNetBaseEC20Cmd_Ping   },
    {(uint32_t)NET_MASTER_CMD_CLD_BASE_GET_DESP_MANUFACTURE         ,0  ,ENABLE ,SrvMsgNetBaseEC20Cmd_QueryManufacturerName   },
    {(uint32_t)NET_MASTER_CMD_CLD_BASE_GET_DESP_MODULE_TYPE         ,0  ,ENABLE ,SrvMsgNetBaseEC20Cmd_QueryModuleType   },
    {(uint32_t)NET_MASTER_CMD_CLD_BASE_GET_DESP_SOFT_VERSION        ,0  ,ENABLE ,SrvMsgNetBaseEC20Cmd_QuerySoftwareVersion   },
    {(uint32_t)NET_MASTER_CMD_CLD_BASE_GET_DESP_IMEI                ,0  ,ENABLE ,SrvMsgNetBaseEC20Cmd_QueryModuleIMEI   },
    {(uint32_t)NET_MASTER_CMD_CLD_BASE_GET_DESP_ISMI                ,0  ,ENABLE ,SrvMsgNetBaseEC20Cmd_QuerySimIMSI   },
    {(uint32_t)NET_MASTER_CMD_CLD_BASE_GET_DESP_ICCID               ,0  ,ENABLE ,SrvMsgNetBaseEC20Cmd_QuerySimICCID   },
    {(uint32_t)NET_MASTER_CMD_CLD_BASE_GET_DESP_NET_NAME_LONG       ,0  ,ENABLE ,SrvMsgNetBaseEC20Cmd_QueryNetFullName   },
    {(uint32_t)NET_MASTER_CMD_CLD_BASE_GET_DESP_NET_NAME_SHORT      ,0  ,ENABLE ,SrvMsgNetBaseEC20Cmd_QueryNetShortName   },
    {(uint32_t)NET_MASTER_CMD_CLD_BASE_GET_DESP_SRV_PROVID_NAME     ,0  ,ENABLE ,SrvMsgNetBaseEC20Cmd_QueryNetProvide   },
    {(uint32_t)NET_MASTER_CMD_CLD_BASE_GET_DESP_NET_TYPE            ,0  ,ENABLE ,SrvMsgNetBaseEC20Cmd_QueryNetType   },
    {(uint32_t)NET_MASTER_CMD_CLD_BASE_GET_DESP_NET_BAUD            ,0  ,ENABLE ,SrvMsgNetBaseEC20Cmd_QueryNetBaud   },
    {(uint32_t)NET_MASTER_CMD_CLD_BASE_GET_DESP_NET_LAC             ,0  ,ENABLE ,SrvMsgNetBaseEC20Cmd_QueryBaseStationLAC   },
    {(uint32_t)NET_MASTER_CMD_CLD_BASE_GET_DESP_NET_CID             ,0  ,ENABLE ,SrvMsgNetBaseEC20Cmd_QueryBaseStationCID   },
    {(uint32_t)NET_MASTER_CMD_CLD_BASE_GET_DESP_STRENGTH_ERR        ,0  ,ENABLE ,SrvMsgNetBaseEC20Cmd_QuerySignalStrength   },
    {(uint32_t)NET_MASTER_CMD_CLD_BASE_CHECK_NET_STATE              ,0  ,ENABLE ,SrvMsgNetBaseEC20Cmd_QueryNetState   },
    {(uint32_t)NET_MASTER_CMD_CLD_BASE_DEVICE_REBOOT                ,0  ,ENABLE ,SrvMsgNetBaseEC20Cmd_ModuleRestart   },
};

void SrvMsgNetBaseEC20Cmd(void* netCmdRecvPtr)
{
    //转换指令码
    NET_PROT_CMD_PACK_RECV* cmdPackPtr = (NET_PROT_CMD_PACK_RECV*)netCmdRecvPtr;
    if(cmdPackPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,Recv NetCmdMsgPtr NULL\r\n",__func__,__LINE__);
        //空指针异常
        return;
    }
    //接收打印显示
    SystemPrintf("BaseEC20 Recv Net Cmd: 0X%08X,DataLen: %d\r\n",cmdPackPtr->cmdCode,cmdPackPtr->dataBodyLength);
    //搜索匹配处理器
    uint16_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < DIM_ARRAY_ELEMENT_COUNT(netCmdProcBaseEC20UnitArray);indexUtil++)
    {
        if(cmdPackPtr->cmdCode == (uint32_t)(netCmdProcBaseEC20UnitArray[indexUtil].commandCode))
        {
            break;
        }
    }
    //判定序号
    if(indexUtil == DIM_ARRAY_ELEMENT_COUNT(netCmdProcBaseEC20UnitArray))
    {
        //不支持的指令
        SystemPrintf("E:> %s,%d,Recv Unsupport CommandCode: 0X%08X\r\n",__func__,__LINE__,cmdPackPtr->cmdCode);
        //释放内存
        if((cmdPackPtr->dataBodyBufferPtr != NULL)&&(cmdPackPtr->dataBodyLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_NET,cmdPackPtr->dataBodyBufferPtr);
        }
        UserMemFree(MEM_REGION_SYS_MSG_NET,cmdPackPtr);
        return;
    }
    //判定长度
    if(cmdPackPtr->dataBodyLength < netCmdProcBaseEC20UnitArray[indexUtil].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,CommandCode: 0X%08X\r\n",__func__,__LINE__,
                        netCmdProcBaseEC20UnitArray[indexUtil].exceptParamLengthMin,cmdPackPtr->dataBodyLength,
                        cmdPackPtr->cmdCode);
        //释放内存
        if((cmdPackPtr->dataBodyBufferPtr != NULL)&&(cmdPackPtr->dataBodyLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_NET,cmdPackPtr->dataBodyBufferPtr);
        }
        UserMemFree(MEM_REGION_SYS_MSG_NET,cmdPackPtr);
        return;
    }
    //校验通过执行代码
    SYS_RESULT_PACK resultPack;
    uint8_t* resultDataBufferPtr = NULL;
    uint16_t resultDataLength = 0;
    //初始化错误返回包
    resultPack.resultCode = PROT_RESULT_FAIL;
    resultPack.errorMain = ERROR_MAIN_OK;
    resultPack.errorLevel = ERROR_LEVEL_NONE;
    resultPack.errorSub = ERROR_SUB_OK;
    //检测函数指针
    if(netCmdProcBaseEC20UnitArray[indexUtil].netCmdProcFuncPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,netCmdProcFuncPtr NULL,CommandCode: 0X%08X\r\n",__func__,__LINE__,
                        cmdPackPtr->cmdCode);
        //释放内存
        if((cmdPackPtr->dataBodyBufferPtr != NULL)&&(cmdPackPtr->dataBodyLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_NET,cmdPackPtr->dataBodyBufferPtr);
        }
        UserMemFree(MEM_REGION_SYS_MSG_NET,cmdPackPtr);
        return;
    }
    //调用处理函数
    netCmdProcBaseEC20UnitArray[indexUtil].netCmdProcFuncPtr(cmdPackPtr,&resultPack,&resultDataBufferPtr,&resultDataLength);
    //结果处理,是否需要结果回传
    if(netCmdProcBaseEC20UnitArray[indexUtil].needResultReply == ENABLE)
    {
        //需要结果回传
        NetMasterSendResultByRecv(cmdPackPtr,&resultPack,resultDataBufferPtr,resultDataLength);
    }
    //释放内存
    if((resultDataBufferPtr != NULL)&&(resultDataLength != 0))
    {
        UserMemFree(MEM_REGION_SRV_TASK,resultDataBufferPtr);
    }
    if((cmdPackPtr->dataBodyBufferPtr != NULL)&&(cmdPackPtr->dataBodyLength != 0))
    {
        UserMemFree(MEM_REGION_SYS_MSG_NET,cmdPackPtr->dataBodyBufferPtr);
    }
    UserMemFree(MEM_REGION_SYS_MSG_NET,cmdPackPtr);
}

//占位函数
static void SrvMsgNetBaseEC20Cmd_HoldPlace(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志打印
    SystemPrintf("%s\r\n",__func__);
    //正常完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//PING 服务器测试
static void SrvMsgNetBaseEC20Cmd_Ping(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志打印
    SystemPrintf("%s\r\n",__func__);
    //执行指令
    SrvImplBaseBaseEC20Ping(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//获取模块厂商信息
static void SrvMsgNetBaseEC20Cmd_QueryManufacturerName(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志打印
    SystemPrintf("%s\r\n",__func__);
    //执行指令
    SrvImplBaseBaseEC20QueryManufacturerName(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//获取模块类型信息
static void SrvMsgNetBaseEC20Cmd_QueryModuleType(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志打印
    SystemPrintf("%s\r\n",__func__);
    //执行指令
    SrvImplBaseBaseEC20QueryModuleType(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//获取模块软件版本信息
static void SrvMsgNetBaseEC20Cmd_QuerySoftwareVersion(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志打印
    SystemPrintf("%s\r\n",__func__);
    //执行指令
    SrvImplBaseBaseEC20QuerySoftwareVersion(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//获取模块入网许可
static void SrvMsgNetBaseEC20Cmd_QueryModuleIMEI(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志打印
    SystemPrintf("%s\r\n",__func__);
    //执行指令
    SrvImplBaseBaseEC20QueryModuleIMEI(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//获取SIM卡IMSI串号
static void SrvMsgNetBaseEC20Cmd_QuerySimIMSI(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志打印
    SystemPrintf("%s\r\n",__func__);
    //执行指令
    SrvImplBaseBaseEC20QuerySimIMSI(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//获取SIM卡ICCID识别号
static void SrvMsgNetBaseEC20Cmd_QuerySimICCID(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志打印
    SystemPrintf("%s\r\n",__func__);
    //执行指令
    SrvImplBaseBaseEC20QuerySimICCID(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//获取当前模块连接网络全名
static void SrvMsgNetBaseEC20Cmd_QueryNetFullName(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志打印
    SystemPrintf("%s\r\n",__func__);
    //执行指令
    SrvImplBaseBaseEC20QueryNetFullName(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//获取当前模块连接网络短名
static void SrvMsgNetBaseEC20Cmd_QueryNetShortName(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志打印
    SystemPrintf("%s\r\n",__func__);
    //执行指令
    SrvImplBaseBaseEC20QueryNetShortName(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//获取网络服务商名称
static void SrvMsgNetBaseEC20Cmd_QueryNetProvide(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志打印
    SystemPrintf("%s\r\n",__func__);
    //执行指令
    SrvImplBaseBaseEC20QueryNetProvide(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//获取当前网络制式
static void SrvMsgNetBaseEC20Cmd_QueryNetType(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志打印
    SystemPrintf("%s\r\n",__func__);
    //执行指令
    SrvImplBaseBaseEC20QueryNetType(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//获取当前网络波段
static void SrvMsgNetBaseEC20Cmd_QueryNetBaud(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志打印
    SystemPrintf("%s\r\n",__func__);
    //执行指令
    SrvImplBaseBaseEC20QueryNetBaud(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//获取LAC基站区域码
static void SrvMsgNetBaseEC20Cmd_QueryBaseStationLAC(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志打印
    SystemPrintf("%s\r\n",__func__);
    //执行指令
    SrvImplBaseBaseEC20QueryBaseStationLAC(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//获取CID基站编号
static void SrvMsgNetBaseEC20Cmd_QueryBaseStationCID(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志打印
    SystemPrintf("%s\r\n",__func__);
    //执行指令
    SrvImplBaseBaseEC20QueryBaseStationCID(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//获取当前信号强度与误码率
static void SrvMsgNetBaseEC20Cmd_QuerySignalStrength(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志打印
    SystemPrintf("%s\r\n",__func__);
    //执行指令
    SrvImplBaseBaseEC20QuerySignalStrength(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//查看当前模块网络是否正常
static void SrvMsgNetBaseEC20Cmd_QueryNetState(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志打印
    SystemPrintf("%s\r\n",__func__);
    //执行指令
    SrvImplBaseBaseEC20QueryNetState(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//通讯设备重启
static void SrvMsgNetBaseEC20Cmd_ModuleRestart(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志打印
    SystemPrintf("%s\r\n",__func__);
    //执行指令
    SrvImplBaseBaseEC20ModuleRestart(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}



/****************************************网口上传消息处理函数**************************************************/
//占位函数
static void SrvMsgNetBaseEC20Upload_PlaceHolder(NET_PROT_UPLOAD_PACK_RECV* uploadPackPtr);

//NET上传消息处理函数列表
static const USER_NET_UPLOAD_PROC_UNIT netUploadProcBaseEC20UnitArray[] = 
{
    //顺序是: 指令码 预期参数最小长度 上传消息处理函数
    {(uint32_t)NET_MASTER_REPORT_HOLD_PLACE     ,2 ,SrvMsgNetBaseEC20Upload_PlaceHolder},
};

void SrvMsgNetBaseEC20Upload(void* netReportDataPtr)
{
    NET_PROT_UPLOAD_PACK_RECV* netUploadPackRecvPtr = (NET_PROT_UPLOAD_PACK_RECV*)netReportDataPtr;
    if(netUploadPackRecvPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,Recv netUploadPackRecvPtr NULL\r\n",__func__,__LINE__);
        //空指针异常
        return;
    }
    //搜索匹配处理器
    uint16_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < DIM_ARRAY_ELEMENT_COUNT(netUploadProcBaseEC20UnitArray);indexUtil++)
    {
        if(netUploadPackRecvPtr->uploadID == (uint32_t)(netUploadProcBaseEC20UnitArray[indexUtil].uploadId))
        {
            break;
        }
    }
    //判定序号
    if(indexUtil == DIM_ARRAY_ELEMENT_COUNT(netUploadProcBaseEC20UnitArray))
    {
        //不支持的指令
        SystemPrintf("E:> %s,%d,Recv Unsupport UploadID: 0X%08X\r\n",__func__,__LINE__,netUploadPackRecvPtr->uploadID);
        //释放内存
        if((netUploadPackRecvPtr->paramBufferPtr != NULL)&&(netUploadPackRecvPtr->paramBufferLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_NET,netUploadPackRecvPtr->paramBufferPtr);
        }
        UserMemFree(MEM_REGION_SYS_MSG_NET,netUploadPackRecvPtr);
        return;
    }
    //判定长度
    if(netUploadPackRecvPtr->paramBufferLength < netUploadProcBaseEC20UnitArray[indexUtil].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,UploadID: 0X%08X\r\n",__func__,__LINE__,
                        netUploadProcBaseEC20UnitArray[indexUtil].exceptParamLengthMin,netUploadPackRecvPtr->paramBufferLength,
                        netUploadPackRecvPtr->uploadID);
        //释放内存
        if((netUploadPackRecvPtr->paramBufferPtr != NULL)&&(netUploadPackRecvPtr->paramBufferLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_NET,netUploadPackRecvPtr->paramBufferPtr);
        }
        UserMemFree(MEM_REGION_SYS_MSG_NET,netUploadPackRecvPtr);
        return;
    }
    //检测函数指针
    if(netUploadProcBaseEC20UnitArray[indexUtil].netUploadProcFuncPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,netUploadProcFuncPtr NULL,UploadID: 0X%08X\r\n",__func__,__LINE__,
                        netUploadPackRecvPtr->uploadID);
        //释放内存
        if((netUploadPackRecvPtr->paramBufferPtr != NULL)&&(netUploadPackRecvPtr->paramBufferLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_NET,netUploadPackRecvPtr->paramBufferPtr);
        }
        UserMemFree(MEM_REGION_SYS_MSG_NET,netUploadPackRecvPtr);
        return;
    }
    //调用指令处理
    netUploadProcBaseEC20UnitArray[indexUtil].netUploadProcFuncPtr(netUploadPackRecvPtr);
    //处理完成,释放内存
    if((netUploadPackRecvPtr->paramBufferPtr != NULL)&&(netUploadPackRecvPtr->paramBufferLength != 0))
    {
        UserMemFree(MEM_REGION_SYS_MSG_NET,netUploadPackRecvPtr->paramBufferPtr);
    }
    UserMemFree(MEM_REGION_SYS_MSG_NET,netUploadPackRecvPtr);
}

//占位函数
static void SrvMsgNetBaseEC20Upload_PlaceHolder(NET_PROT_UPLOAD_PACK_RECV* uploadPackPtr)
{
    asm("nop");
}






















