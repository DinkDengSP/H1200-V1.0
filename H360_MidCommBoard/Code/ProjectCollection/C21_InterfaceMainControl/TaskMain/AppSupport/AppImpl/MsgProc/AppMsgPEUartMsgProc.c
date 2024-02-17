/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-29 11:51:45
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-02-05 15:11:50
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#include "AppImplBaseMsgProc.h"
#include "AppImplMsgProc.h"
#include "AppTaskHeader.h"


/*************************************************************串口指令处理*************************************************************/
//PE接口向欧蒙接口上报设备状态
static void AppMsgPEUartMsgProcess_ReportPeState(PE_UART_PACKAGE* peUartCmdMsgPtr);
//PE接口查询欧蒙接口设备的状态
static void AppMsgPEUartMsgProcess_QueryZetaDeviceState(PE_UART_PACKAGE* peUartCmdMsgPtr);
//向PE接口上报欧蒙接口的设备状态
static void AppMsgPEUartMsgProcess_ReportZetaState(PE_UART_PACKAGE* peUartCmdMsgPtr);
//欧蒙接口查询PE接口设备的状态
static void AppMsgPEUartMsgProcess_QueryPeDeviceState(PE_UART_PACKAGE* peUartCmdMsgPtr);
//PE接口查询欧蒙接口空试管架是否准备好
static void AppMsgPEUartMsgProcess_QueryZetaEmptyRankState(PE_UART_PACKAGE* peUartCmdMsgPtr);
//欧蒙接口上报空架子是否准备好
static void AppMsgPEUartMsgProcess_ReportEmptyRankState(PE_UART_PACKAGE* peUartCmdMsgPtr);
//PE接口上报试管已经放到位
static void AppMsgPEUartMsgProcess_ReportTubeReady(PE_UART_PACKAGE* peUartCmdMsgPtr);
//欧蒙接口查询试管能不能返回PE接口
static void AppMsgPEUartMsgProcess_QueryTubeReturnPermit(PE_UART_PACKAGE* peUartCmdMsgPtr);
//PE接口返回试管能不能回到PE接口
static void AppMsgPEUartMsgProcess_ReportTubeReturnPermit(PE_UART_PACKAGE* peUartCmdMsgPtr);
//欧蒙接口上报需要抓走的试管的位置
static void AppMsgPEUartMsgProcess_ReportNeedCatchOutTubeState(PE_UART_PACKAGE* peUartCmdMsgPtr);
//PE接口上报开始抓取指定位置的试管
static void AppMsgPEUartMsgProcess_ReportStartCatchTube(PE_UART_PACKAGE* peUartCmdMsgPtr);
//PE接口上报抓取完成
static void AppMsgPEUartMsgProcess_ReportTubeCatchOver(PE_UART_PACKAGE* peUartCmdMsgPtr);

//UART指令处理函数列表
static const PE_UART_CMD_PROC_UNIT peUartMsgCmdProcessUnitArray[] = 
{
    //顺序: 指令码  预期参数最小长度 指令处理函数
    {MACRO_PE_UART_CMD_PE_DEVICE_STATE_REPORT           ,0 ,AppMsgPEUartMsgProcess_ReportPeState                },
    {MACRO_PE_UART_CMD_QUERY_OUMENG_INTER_STATE         ,0 ,AppMsgPEUartMsgProcess_QueryZetaDeviceState         },
    {MACRO_PE_UART_CMD_OUMENG_DEVICE_STATE_REPORT       ,0 ,AppMsgPEUartMsgProcess_ReportZetaState              },
    {MACRO_PE_UART_CMD_QUERY_PE_DEVICE_STATE            ,0 ,AppMsgPEUartMsgProcess_QueryPeDeviceState           },
    {MACRO_PE_UART_CMD_QUERY_EMPTY_RANK_IS_READY        ,0 ,AppMsgPEUartMsgProcess_QueryZetaEmptyRankState      },
    {MACRO_PE_UART_CMD_RESP_QUERY_RANK_READY            ,0 ,AppMsgPEUartMsgProcess_ReportEmptyRankState         },
    {MACRO_PE_UART_CMD_REPORT_TUBE_PUT_READY            ,0 ,AppMsgPEUartMsgProcess_ReportTubeReady              },
    {MACRO_PE_UART_CMD_QUERY_TUBE_RETURN_PE_PERMIT      ,0 ,AppMsgPEUartMsgProcess_QueryTubeReturnPermit        },
    {MACRO_PE_UART_CMD_RESP_TUBE_RETURN_PE_PERMIT       ,0 ,AppMsgPEUartMsgProcess_ReportTubeReturnPermit       },
    {MACRO_PE_UART_CMD_NOTIFY_PE_RANK_CATCH_POS         ,0 ,AppMsgPEUartMsgProcess_ReportNeedCatchOutTubeState  },
    {MACRO_PE_UART_CMD_PE_REPORT_START_CATCH_TUBE       ,0 ,AppMsgPEUartMsgProcess_ReportStartCatchTube         },
    {MACRO_PE_UART_CMD_PE_REPORT_END_CATCH_TUBE         ,0 ,AppMsgPEUartMsgProcess_ReportTubeCatchOver          },
};


//串口指令消息处理函数
void AppMsgPEUartMsgProcCmd(void* peuartMsgBodyPtr)
{
    //转换指令码
    PE_UART_PACKAGE* peUartCmdMsgPtr = (PE_UART_PACKAGE*)peuartMsgBodyPtr;
    if(peUartCmdMsgPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,Recv PEUartCmdMsgPtr NULL\r\n",__func__,__LINE__);
        //空指针异常
        return;
    }
    //搜索匹配处理器
    uint16_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < DIM_ARRAY_ELEMENT_COUNT(peUartMsgCmdProcessUnitArray);indexUtil++)
    {
        if(peUartCmdMsgPtr->cmdCode == (uint32_t)(peUartMsgCmdProcessUnitArray[indexUtil].commandCode))
        {
            break;
        }
    }
    //判定序号
    if(indexUtil == DIM_ARRAY_ELEMENT_COUNT(peUartMsgCmdProcessUnitArray))
    {
        //不支持的指令
        SystemPrintf("E:> %s,%d,Recv Unsupport CommandCode: 0X%08X\r\n",__func__,__LINE__,peUartCmdMsgPtr->cmdCode);
        //释放内存
        if((peUartCmdMsgPtr->dataBodyBufferPtr != NULL)&&(peUartCmdMsgPtr->dataBodyLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_UART,peUartCmdMsgPtr->dataBodyBufferPtr);
        }
        UserMemFree(MEM_REGION_SYS_MSG_UART,peUartCmdMsgPtr);
        return;
    }
    //判定长度
    if(peUartCmdMsgPtr->dataBodyLength < peUartMsgCmdProcessUnitArray[indexUtil].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,CommandCode: 0X%08X\r\n",__func__,__LINE__,
                        peUartMsgCmdProcessUnitArray[indexUtil].exceptParamLengthMin,peUartCmdMsgPtr->dataBodyLength,
                        peUartCmdMsgPtr->cmdCode);
        //释放内存
        if((peUartCmdMsgPtr->dataBodyBufferPtr != NULL)&&(peUartCmdMsgPtr->dataBodyLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_UART,peUartCmdMsgPtr->dataBodyBufferPtr);
        }
        UserMemFree(MEM_REGION_SYS_MSG_UART,peUartCmdMsgPtr);
        return;
    }
    //检测函数指针
    if(peUartMsgCmdProcessUnitArray[indexUtil].peUartCmdProcFuncPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,peUartCmdProcFuncPtr NULL,CommandCode: 0X%08X\r\n",__func__,__LINE__,
                        peUartCmdMsgPtr->cmdCode);
        //释放内存
        if((peUartCmdMsgPtr->dataBodyBufferPtr != NULL)&&(peUartCmdMsgPtr->dataBodyLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_UART,peUartCmdMsgPtr->dataBodyBufferPtr);
        }
        UserMemFree(MEM_REGION_SYS_MSG_UART,peUartCmdMsgPtr);
        return;
    }
    //调用处理函数
    peUartMsgCmdProcessUnitArray[indexUtil].peUartCmdProcFuncPtr(peUartCmdMsgPtr);
    if((peUartCmdMsgPtr->dataBodyBufferPtr != NULL)&&(peUartCmdMsgPtr->dataBodyLength != 0))
    {
        UserMemFree(MEM_REGION_SYS_MSG_UART,peUartCmdMsgPtr->dataBodyBufferPtr);
    }
    UserMemFree(MEM_REGION_SYS_MSG_UART,peUartCmdMsgPtr);
}

//PE接口向欧蒙接口上报设备状态
static void AppMsgPEUartMsgProcess_ReportPeState(PE_UART_PACKAGE* peUartCmdMsgPtr)
{
    //PE接口上报的状态
    PE_UART_DEVICE_STATE peDeviceState = (PE_UART_DEVICE_STATE)(peUartCmdMsgPtr->dataBodyBufferPtr[0]);
    SystemPrintf("%s,peDeviceState : %d,DataLength: %d\r\n",__func__,peDeviceState,peUartCmdMsgPtr->dataBodyLength);
    //后续处理
}

//PE接口查询欧蒙接口设备的状态
static void AppMsgPEUartMsgProcess_QueryZetaDeviceState(PE_UART_PACKAGE* peUartCmdMsgPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //收到该指令应该返回欧蒙接口设备状态
    SystemPrintf("%s,DataLength: %d\r\n",__func__,peUartCmdMsgPtr->dataBodyLength);
    //回传欧蒙接口设备状态
    PE_UART_DEVICE_STATE oumengInterDeviceState = PE_UART_DEVICE_STATE_NORMAL;
    //回传
    errorSub = PeUartOumengInterReportZetaState(oumengInterDeviceState);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s, PeUartOumengInterReportZetaState Failed: 0X%08X\r\n",__func__,errorSub);
    }
}

//向PE接口上报欧蒙接口的设备状态
static void AppMsgPEUartMsgProcess_ReportZetaState(PE_UART_PACKAGE* peUartCmdMsgPtr)
{
    //这个指令是不应该收到的指令,PE接收,欧蒙接口不接收
    SystemPrintf("ERROR! %s,DataLength: %d\r\n",__func__,peUartCmdMsgPtr->dataBodyLength);
}

//欧蒙接口查询PE接口设备的状态
static void AppMsgPEUartMsgProcess_QueryPeDeviceState(PE_UART_PACKAGE* peUartCmdMsgPtr)
{
    //这个指令是欧蒙接口发送给PE接口的,主控本身不应该接收
    SystemPrintf("ERROR! %s,DataLength: %d\r\n",__func__,peUartCmdMsgPtr->dataBodyLength);
}

//PE接口查询欧蒙接口空试管架是否准备好
static void AppMsgPEUartMsgProcess_QueryZetaEmptyRankState(PE_UART_PACKAGE* peUartCmdMsgPtr)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //收到这个指令,需要向PE接口上报欧蒙接口的试管架状态
    SystemPrintf("ERROR! %s,DataLength: %d\r\n",__func__,peUartCmdMsgPtr->dataBodyLength);
    PE_UART_EMPTY_RANK_STATE emptyRankState = PE_UART_EMPTY_RANK_STATE_NOT_READY;
    //回传
    errorSub = PeUartOumengInterReportEmptyRankState(emptyRankState);
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s, PeUartOumengInterReportEmptyRankState Failed: 0X%08X\r\n",__func__,errorSub);
    }
}

//欧蒙接口上报空架子是否准备好
static void AppMsgPEUartMsgProcess_ReportEmptyRankState(PE_UART_PACKAGE* peUartCmdMsgPtr)
{
    //这个指令是PE接收设备接收处理的,欧蒙接口设备不应该收到这个指令
    SystemPrintf("ERROR! %s,DataLength: %d\r\n",__func__,peUartCmdMsgPtr->dataBodyLength);
}

//PE接口上报试管已经放到位
static void AppMsgPEUartMsgProcess_ReportTubeReady(PE_UART_PACKAGE* peUartCmdMsgPtr)
{
    PE_UART_TUBE_POS tubePos;
    uint8_t* tubeBarCodePtr = NULL;
    uint16_t tubeBarCodeLength = 0;
    //PE接口上报欧蒙接口试管已经到位
    tubePos = (PE_UART_TUBE_POS)(peUartCmdMsgPtr->dataBodyBufferPtr[0]);
    tubeBarCodeLength = peUartCmdMsgPtr->dataBodyLength - 1;
    if(tubeBarCodeLength != 0)
    {
        tubeBarCodePtr = UserMemMallocWhileSuccess(MEM_REGION_APP_TASK,tubeBarCodeLength);
        UserMemCopy(tubeBarCodePtr,peUartCmdMsgPtr->dataBodyBufferPtr+1,tubeBarCodeLength);
    }
    else
    {
        tubeBarCodePtr = NULL;
    }
    SystemPrintf("%s,tubePos : %d,DataLength: %d\r\n",__func__,tubePos,peUartCmdMsgPtr->dataBodyLength);
    if(tubeBarCodePtr != NULL)
    {
        SystemSendBuffer(tubeBarCodePtr,tubeBarCodeLength);
        SystemPrintf("\r\n");
    }
    //对数据处理

}

//欧蒙接口查询试管能不能返回PE接口
static void AppMsgPEUartMsgProcess_QueryTubeReturnPermit(PE_UART_PACKAGE* peUartCmdMsgPtr)
{
    //这个是欧蒙接口发给PE的指令,欧蒙接口自身不应该受到
    SystemPrintf("ERROR! %s,DataLength: %d\r\n",__func__,peUartCmdMsgPtr->dataBodyLength);
}

//PE接口返回试管能不能回到PE接口
static void AppMsgPEUartMsgProcess_ReportTubeReturnPermit(PE_UART_PACKAGE* peUartCmdMsgPtr)
{
    PE_UART_TUBE_RETURN tubeReturnPermit;
    tubeReturnPermit = (PE_UART_TUBE_RETURN)(peUartCmdMsgPtr->dataBodyBufferPtr[0]);
    SystemPrintf("%s,tubeReturnPermit : %d,DataLength: %d\r\n",__func__,tubeReturnPermit,peUartCmdMsgPtr->dataBodyLength);
    //后续处理
}

//欧蒙接口上报需要抓走的试管的位置
static void AppMsgPEUartMsgProcess_ReportNeedCatchOutTubeState(PE_UART_PACKAGE* peUartCmdMsgPtr)
{
    //这个是欧蒙接口上报给PE接口的指令,欧蒙接口自身不应该收到
    SystemPrintf("ERROR! %s,DataLength: %d\r\n",__func__,peUartCmdMsgPtr->dataBodyLength);
}

//PE接口上报开始抓取指定位置的试管
static void AppMsgPEUartMsgProcess_ReportStartCatchTube(PE_UART_PACKAGE* peUartCmdMsgPtr)
{
    PE_UART_TUBE_POS startCatchTubePos;
    //PE接口上报开始抓试管
    startCatchTubePos = (PE_UART_TUBE_POS)(peUartCmdMsgPtr->dataBodyBufferPtr[0]);
    SystemPrintf("%s,startCatchTubePos : %d,DataLength: %d\r\n",__func__,startCatchTubePos,peUartCmdMsgPtr->dataBodyLength);
    //后续处理
}

//PE接口上报抓取完成
static void AppMsgPEUartMsgProcess_ReportTubeCatchOver(PE_UART_PACKAGE* peUartCmdMsgPtr)
{
    PE_UART_TUBE_POS catchOverTubePos;
    //PE接口上报开始抓试管
    catchOverTubePos = (PE_UART_TUBE_POS)(peUartCmdMsgPtr->dataBodyBufferPtr[0]);
    SystemPrintf("%s,catchOverTubePos : %d,DataLength: %d\r\n",__func__,catchOverTubePos,peUartCmdMsgPtr->dataBodyLength);
    //后续处理
}



//PE串口上传消息处理函数
void AppMsgPEUartMsgProcUpload(void* peUartReportDataPtr)
{
    return;
}