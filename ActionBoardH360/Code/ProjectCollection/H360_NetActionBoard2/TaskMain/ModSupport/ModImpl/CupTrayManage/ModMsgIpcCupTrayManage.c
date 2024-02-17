/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-22 09:01:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-22 15:57:07 +0800
************************************************************************************************/ 
#include "ModImplCupTrayManage.h"
#include "ModTaskHeader.h"


/*************************************************************IPC指令处理处理***********************************************************/
//通讯测试
static void ModMsgIpcCupTrayManageCmd_CommTest(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//模块内输入读取
static void ModMsgIpcCupTrayManageCmd_PortIn(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//模块内输出写入
static void ModMsgIpcCupTrayManageCmd_PortOut(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//模块内电机复位
static void ModMsgIpcCupTrayManageCmd_MotorReset(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//模块内电机走位
static void ModMsgIpcCupTrayManageCmd_MotorRunSteps(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//设置杯栈门状态
static void ModMsgIpcCupTrayManageCmd_SetDoorState(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//新杯盘栈初始化
static void ModMsgIpcCupTrayManageCmd_NewStackInit(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//空杯盘栈初始化
static void ModMsgIpcCupTrayManageCmd_EmptyStackInit(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//推手初始化
static void ModMsgIpcCupTrayManageCmd_PushInit(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//夹手初始化
static void ModMsgIpcCupTrayManageCmd_ClampInit(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//垃圾桶下降
static void ModMsgIpcCupTrayManageCmd_GarbageDownUnlock(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//垃圾桶上升
static void ModMsgIpcCupTrayManageCmd_GarbegeUpLock(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//杯栈模块整体初始化
static void ModMsgIpcCupTrayManageCmd_InitAll(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//夹手释放
static void ModMsgIpcCupTrayManageCmd_ClampOpen(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//夹手夹紧
static void ModMsgIpcCupTrayManageCmd_ClampClose(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//新杯盘栈上传一个新盘
static void ModMsgIpcCupTrayManageCmd_UploadNewTray(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//空杯盘栈从测试区回收一个空盘
static void ModMsgIpcCupTrayManageCmd_RecycleEmptyTray(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//推手推一个新盘到测试位
static void ModMsgIpcCupTrayManageCmd_PushNewTrayToTest(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//新杯盘栈上升指定层(门开到位状态)
static void ModMsgIpcCupTrayManageCmd_NewStackUpSpecialLayerWithDoorOpenState(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//新杯盘栈回零(门开到位状态)
static void ModMsgIpcCupTrayManageCmd_NewStackReturnZero(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//空杯盘上升指定层(门开到位状态)
static void ModMsgIpcCupTrayManageCmd_EmptyStackUpSpecialLayerWithDoorOpenState(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//空杯盘栈回零(门开到位状态)
static void ModMsgIpcCupTrayManageCmd_EmptyStackReturnZero(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//获取模块信息集合
static void ModMsgIpcCupTrayManageCmd_GetMsgSet(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);


//CAN指令处理函数列表
static const USER_IPC_CMD_PROC_UNIT ipcCupTrayManageMsgCmdProcUnitArray[] = 
{
    //顺序是 指令码 预期参数最小长度  指令处理函数
    {(uint32_t)0XFFFFFFFF                                                  ,0    ,ModMsgIpcCupTrayManageCmd_CommTest              },
    {(uint32_t)IPC_CMD_MACRO_H360_CUP_TRAY_MANAGE_PORT_IN                  ,0    ,ModMsgIpcCupTrayManageCmd_PortIn                },
    {(uint32_t)IPC_CMD_MACRO_H360_CUP_TRAY_MANAGE_PORT_OUT                 ,0    ,ModMsgIpcCupTrayManageCmd_PortOut               },
    {(uint32_t)IPC_CMD_MACRO_H360_CUP_TRAY_MANAGE_MOTOR_RESET              ,0    ,ModMsgIpcCupTrayManageCmd_MotorReset            },
    {(uint32_t)IPC_CMD_MACRO_H360_CUP_TRAY_MANAGE_MOTOR_RUN_STEPS          ,0    ,ModMsgIpcCupTrayManageCmd_MotorRunSteps         },
    {(uint32_t)IPC_CMD_MACRO_H360_CUP_TRAY_MANAGE_SET_DOOR_STATE           ,0    ,ModMsgIpcCupTrayManageCmd_SetDoorState          },
    {(uint32_t)IPC_CMD_MACRO_H360_CUP_TRAY_MANAGE_NEW_STACK_INIT           ,0    ,ModMsgIpcCupTrayManageCmd_NewStackInit          },
    {(uint32_t)IPC_CMD_MACRO_H360_CUP_TRAY_MANAGE_EMPTY_STACK_INIT         ,0    ,ModMsgIpcCupTrayManageCmd_EmptyStackInit        },
    {(uint32_t)IPC_CMD_MACRO_H360_CUP_TRAY_MANAGE_PUSH_INIT                ,0    ,ModMsgIpcCupTrayManageCmd_PushInit              },
    {(uint32_t)IPC_CMD_MACRO_H360_CUP_TRAY_MANAGE_CLAMP_INIT               ,0    ,ModMsgIpcCupTrayManageCmd_ClampInit             },
    {(uint32_t)IPC_CMD_MACRO_H360_CUP_TRAY_MANAGE_GARBAGE_DOWN_UNLOCK      ,0    ,ModMsgIpcCupTrayManageCmd_GarbageDownUnlock     },
    {(uint32_t)IPC_CMD_MACRO_H360_CUP_TRAY_MANAGE_GARBEGE_UP_LOCK          ,0    ,ModMsgIpcCupTrayManageCmd_GarbegeUpLock         },
    {(uint32_t)IPC_CMD_MACRO_H360_CUP_TRAY_MANAGE_INIT_ALL                 ,0    ,ModMsgIpcCupTrayManageCmd_InitAll               },
    {(uint32_t)IPC_CMD_MACRO_H360_CUP_TRAY_MANAGE_CLAMP_OPEN               ,0    ,ModMsgIpcCupTrayManageCmd_ClampOpen             },
    {(uint32_t)IPC_CMD_MACRO_H360_CUP_TRAY_MANAGE_CLAMP_CLOSE              ,0    ,ModMsgIpcCupTrayManageCmd_ClampClose            },
    {(uint32_t)IPC_CMD_MACRO_H360_CUP_TRAY_MANAGE_UPLOAD_NEW_TRAY          ,0    ,ModMsgIpcCupTrayManageCmd_UploadNewTray         },
    {(uint32_t)IPC_CMD_MACRO_H360_CUP_TRAY_MANAGE_RECYCLE_EMPTY_TRAY       ,0    ,ModMsgIpcCupTrayManageCmd_RecycleEmptyTray      },
    {(uint32_t)IPC_CMD_MACRO_H360_CUP_TRAY_MANAGE_PUSH_NEW_TRAY_TO_TEST    ,0    ,ModMsgIpcCupTrayManageCmd_PushNewTrayToTest     },
    {(uint32_t)IPC_CMD_MACRO_H360_CUP_TRAY_MANAGE_NEW_STACK_UP_SPECIAL_LAYER_WITH_DOOR_OPEN_STATE     ,0    ,ModMsgIpcCupTrayManageCmd_NewStackUpSpecialLayerWithDoorOpenState  },
    {(uint32_t)IPC_CMD_MACRO_H360_CUP_TRAY_MANAGE_NEW_STACK_RETURN_ZERO    ,0    ,ModMsgIpcCupTrayManageCmd_NewStackReturnZero    },
    {(uint32_t)IPC_CMD_MACRO_H360_CUP_TRAY_MANAGE_EMPTY_STACK_UP_SPECIAL_LAYER_WITH_DOOR_OPEN_STATE   ,0    ,ModMsgIpcCupTrayManageCmd_EmptyStackUpSpecialLayerWithDoorOpenState  },
    {(uint32_t)IPC_CMD_MACRO_H360_CUP_TRAY_MANAGE_EMPTY_STACK_RETURN_ZERO  ,0    ,ModMsgIpcCupTrayManageCmd_EmptyStackReturnZero  },
    {(uint32_t)IPC_CMD_MACRO_H360_CUP_TRAY_MANAGE_GET_MSG_SET              ,0    ,ModMsgIpcCupTrayManageCmd_GetMsgSet             },
};


//IPC指令消息处理函数
void ModMsgIpcCupTrayManageCmd(void* ipcMsgBodyPtr)
{
    //转换指令码
    IPC_CMD_DATA* ipcCmdMsgPtr = (IPC_CMD_DATA*)ipcMsgBodyPtr;
    if(ipcCmdMsgPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,Recv ipcCmdMsgPtr NULL\r\n",__func__,__LINE__);
        //空指针异常
        return;
    }
    IPC_CMD_WAIT_RESULT* ipcWaitResultPackPtr = ipcCmdMsgPtr->waitResultPackPtr;
    //搜索匹配处理器
    uint16_t indexCupTrayManage = 0;
    for(indexCupTrayManage = 0; indexCupTrayManage < DIM_ARRAY_ELEMENT_COUNT(ipcCupTrayManageMsgCmdProcUnitArray);indexCupTrayManage++)
    {
        if(ipcCmdMsgPtr->cmdBaseData.commandCode == (uint32_t)(ipcCupTrayManageMsgCmdProcUnitArray[indexCupTrayManage].commandCode))
        {
            break;
        }
    }
    //判定序号
    if(indexCupTrayManage == DIM_ARRAY_ELEMENT_COUNT(ipcCupTrayManageMsgCmdProcUnitArray))
    {
        //不支持的指令
        SystemPrintf("E:> %s,%d,Recv Unsupport CommandCode: 0X%08X\r\n",__func__,__LINE__,ipcCmdMsgPtr->cmdBaseData.commandCode);
        //设置等待状态为错误
        ipcWaitResultPackPtr->currentCmdStage = IPC_CMD_STAGE_ERROR_SUPPORT;
        //释放内存
        if((ipcCmdMsgPtr->cmdBaseData.cmdParamBufferPtr != NULL)&&(ipcCmdMsgPtr->cmdBaseData.cmdParamBufferLength != 0))
        {
            IPC_PortMemFree(ipcCmdMsgPtr->cmdBaseData.cmdParamBufferPtr);
        }
        IPC_PortMemFree(ipcCmdMsgPtr);
        return;
    }
    //判定长度
    if(ipcCmdMsgPtr->cmdBaseData.cmdParamBufferLength < ipcCupTrayManageMsgCmdProcUnitArray[indexCupTrayManage].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,CommandCode: 0X%08X\r\n",__func__,__LINE__,
                        ipcCupTrayManageMsgCmdProcUnitArray[indexCupTrayManage].exceptParamLengthMin,ipcCmdMsgPtr->cmdBaseData.cmdParamBufferLength,
                        ipcCmdMsgPtr->cmdBaseData.commandCode);
        //设置等待状态为错误
        ipcWaitResultPackPtr->currentCmdStage = IPC_CMD_STAGE_CMD_PAR_LEN;
        //释放内存
        if((ipcCmdMsgPtr->cmdBaseData.cmdParamBufferPtr != NULL)&&(ipcCmdMsgPtr->cmdBaseData.cmdParamBufferLength != 0))
        {
            IPC_PortMemFree(ipcCmdMsgPtr->cmdBaseData.cmdParamBufferPtr);
        }
        IPC_PortMemFree(ipcCmdMsgPtr);
        return;
    }
    //校验通过执行代码
    SYS_RESULT_PACK sysResultPack;
    //初始化错误返回包
    sysResultPack.resultCode = PROT_RESULT_FAIL;
    sysResultPack.errorMain = ERROR_MAIN_OK;
    sysResultPack.errorLevel = ERROR_LEVEL_NONE;
    sysResultPack.errorSub = ERROR_SUB_OK;
    //检测函数指针
    if(ipcCupTrayManageMsgCmdProcUnitArray[indexCupTrayManage].ipcCmdProcFuncPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,ipcCmdProcFuncPtr NULL,CommandCode: 0X%08X\r\n",__func__,__LINE__,
                        ipcCmdMsgPtr->cmdBaseData.commandCode);
        //设置等待状态为错误
        ipcWaitResultPackPtr->currentCmdStage = IPC_CMD_STAGE_PROC_FUNC_NULL;
        //释放内存
        if((ipcCmdMsgPtr->cmdBaseData.cmdParamBufferPtr != NULL)&&(ipcCmdMsgPtr->cmdBaseData.cmdParamBufferLength != 0))
        {
            IPC_PortMemFree(ipcCmdMsgPtr->cmdBaseData.cmdParamBufferPtr);
        }
        IPC_PortMemFree(ipcCmdMsgPtr);
        return;
    }
    //调用处理函数
    ipcCupTrayManageMsgCmdProcUnitArray[indexCupTrayManage].ipcCmdProcFuncPtr(&(ipcCmdMsgPtr->cmdBaseData),&sysResultPack,ipcCmdMsgPtr->channelCachePtr);
    //处理完成回传
    ipcWaitResultPackPtr->resultPack.resultCode = sysResultPack.resultCode;
    ipcWaitResultPackPtr->resultPack.errorMain = sysResultPack.errorMain;
    ipcWaitResultPackPtr->resultPack.errorLevel = sysResultPack.errorLevel;
    ipcWaitResultPackPtr->resultPack.errorSub = sysResultPack.errorSub;
    //释放内存
    if((ipcCmdMsgPtr->cmdBaseData.cmdParamBufferPtr != NULL)&&(ipcCmdMsgPtr->cmdBaseData.cmdParamBufferLength != 0))
    {
        IPC_PortMemFree(ipcCmdMsgPtr->cmdBaseData.cmdParamBufferPtr);
    }
    IPC_PortMemFree(ipcCmdMsgPtr);
    //状态设置完成
    ipcWaitResultPackPtr->currentCmdStage = IPC_CMD_STAGE_OVER;
    return;
}

//通讯测试
static void ModMsgIpcCupTrayManageCmd_CommTest(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{
#if(IPC_MOD_RUNNING_LOG_SHOW == CONFIG_FUNC_ENABLE)
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
#endif
    //回传数据为0
    channelCachePtr->returnDataBufferLength = 0;
    //构建完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//模块内输入读取
static void ModMsgIpcCupTrayManageCmd_PortIn(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//模块内输出写入
static void ModMsgIpcCupTrayManageCmd_PortOut(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//模块内电机复位
static void ModMsgIpcCupTrayManageCmd_MotorReset(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//模块内电机走位
static void ModMsgIpcCupTrayManageCmd_MotorRunSteps(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//设置杯栈门状态
static void ModMsgIpcCupTrayManageCmd_SetDoorState(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//新杯盘栈初始化
static void ModMsgIpcCupTrayManageCmd_NewStackInit(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//空杯盘栈初始化
static void ModMsgIpcCupTrayManageCmd_EmptyStackInit(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//推手初始化
static void ModMsgIpcCupTrayManageCmd_PushInit(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//夹手初始化
static void ModMsgIpcCupTrayManageCmd_ClampInit(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//垃圾桶下降
static void ModMsgIpcCupTrayManageCmd_GarbageDownUnlock(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//垃圾桶上升
static void ModMsgIpcCupTrayManageCmd_GarbegeUpLock(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//杯栈模块整体初始化
static void ModMsgIpcCupTrayManageCmd_InitAll(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//夹手释放
static void ModMsgIpcCupTrayManageCmd_ClampOpen(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//夹手夹紧
static void ModMsgIpcCupTrayManageCmd_ClampClose(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//新杯盘栈上传一个新盘
static void ModMsgIpcCupTrayManageCmd_UploadNewTray(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//空杯盘栈从测试区回收一个空盘
static void ModMsgIpcCupTrayManageCmd_RecycleEmptyTray(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//推手推一个新盘到测试位
static void ModMsgIpcCupTrayManageCmd_PushNewTrayToTest(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//新杯盘栈上升指定层(门开到位状态)
static void ModMsgIpcCupTrayManageCmd_NewStackUpSpecialLayerWithDoorOpenState(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//新杯盘栈回零(门开到位状态)
static void ModMsgIpcCupTrayManageCmd_NewStackReturnZero(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//空杯盘上升指定层(门开到位状态)
static void ModMsgIpcCupTrayManageCmd_EmptyStackUpSpecialLayerWithDoorOpenState(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//空杯盘栈回零(门开到位状态)
static void ModMsgIpcCupTrayManageCmd_EmptyStackReturnZero(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

}

//获取模块信息集合
static void ModMsgIpcCupTrayManageCmd_GetMsgSet(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{
    
}


/*************************************************************************************************************************************/

/*************************************************************IPC上报讯息处理**********************************************************/
//占位函数
static void ModMsgIpcCupTrayManageUpload_PlaceHolder(IPC_UPLOAD_DATA* ipcUploadDataPtr);

//IPC上传消息处理函数列表
static const USER_IPC_UPLOAD_PROC_UNIT ipcCupTrayManageMsgUploadProcUnitArray[] = 
{
    //顺序是: 指令码 预期参数最小长度 上传消息处理函数
    {(uint32_t)0XFFFFFFFF     ,2 ,ModMsgIpcCupTrayManageUpload_PlaceHolder},
};


//IPC上传消息处理函数
void ModMsgIpcCupTrayManageUpload(void* ipcMsgBodyPtr)
{
    IPC_UPLOAD_DATA* ipcUploadDataPtr = (IPC_UPLOAD_DATA*)ipcMsgBodyPtr;
    if(ipcUploadDataPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,Recv ipcUploadDataPtr NULL\r\n",__func__,__LINE__);
        //空指针异常
        return;
    }
    //搜索匹配处理器
    uint16_t indexCupTrayManage = 0;
    for(indexCupTrayManage = 0; indexCupTrayManage < DIM_ARRAY_ELEMENT_COUNT(ipcCupTrayManageMsgUploadProcUnitArray);indexCupTrayManage++)
    {
        if(ipcUploadDataPtr->uploadID == (uint32_t)(ipcCupTrayManageMsgUploadProcUnitArray[indexCupTrayManage].uploadId))
        {
            break;
        }
    }
    //判定序号
    if(indexCupTrayManage == DIM_ARRAY_ELEMENT_COUNT(ipcCupTrayManageMsgUploadProcUnitArray))
    {
        //不支持的指令
        SystemPrintf("E:> %s,%d,Recv Unsupport UploadID: 0X%08X\r\n",__func__,__LINE__,ipcUploadDataPtr->uploadID);
        //释放内存
        if((ipcUploadDataPtr->dataBufferPtr != NULL)&&(ipcUploadDataPtr->dataLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_IPC,ipcUploadDataPtr->dataBufferPtr);
        }
        UserMemFree(MEM_REGION_SYS_MSG_IPC,ipcUploadDataPtr);
        return;
    }
    //判定长度
    if(ipcUploadDataPtr->dataLength < ipcCupTrayManageMsgUploadProcUnitArray[indexCupTrayManage].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,UploadID: 0X%08X\r\n",__func__,__LINE__,
                        ipcCupTrayManageMsgUploadProcUnitArray[indexCupTrayManage].exceptParamLengthMin,ipcUploadDataPtr->dataLength,
                        ipcUploadDataPtr->uploadID);
        //释放内存
        if((ipcUploadDataPtr->dataBufferPtr != NULL)&&(ipcUploadDataPtr->dataLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_IPC,ipcUploadDataPtr->dataBufferPtr);
        }
        UserMemFree(MEM_REGION_SYS_MSG_IPC,ipcUploadDataPtr);
        return;
    }
    //检测函数指针
    if(ipcCupTrayManageMsgUploadProcUnitArray[indexCupTrayManage].ipcUploadProcFuncPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,ipcUploadProcFuncPtr NULL,UploadID: 0X%08X\r\n",__func__,__LINE__,
                        ipcUploadDataPtr->uploadID);
        //释放内存
        if((ipcUploadDataPtr->dataBufferPtr != NULL)&&(ipcUploadDataPtr->dataLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_IPC,ipcUploadDataPtr->dataBufferPtr);
        }
        UserMemFree(MEM_REGION_SYS_MSG_IPC,ipcUploadDataPtr);
        return;
    }
    //调用指令处理
    ipcCupTrayManageMsgUploadProcUnitArray[indexCupTrayManage].ipcUploadProcFuncPtr(ipcUploadDataPtr);
    //处理完成,释放内存
    if((ipcUploadDataPtr->dataBufferPtr != NULL)&&(ipcUploadDataPtr->dataLength != 0))
    {
        UserMemFree(MEM_REGION_SYS_MSG_IPC,ipcUploadDataPtr->dataBufferPtr);
    }
    UserMemFree(MEM_REGION_SYS_MSG_IPC,ipcUploadDataPtr);
}

//占位函数
static void ModMsgIpcCupTrayManageUpload_PlaceHolder(IPC_UPLOAD_DATA* ipcUploadDataPtr)
{

}

/*************************************************************************************************************************************/















