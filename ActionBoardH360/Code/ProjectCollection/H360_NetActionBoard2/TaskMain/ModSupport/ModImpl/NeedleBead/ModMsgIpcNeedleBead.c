/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-22 09:01:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-22 16:26:48 +0800
************************************************************************************************/ 
#include "ModImplNeedleBead.h"
#include "ModTaskHeader.h"


/*************************************************************IPC指令处理处理***********************************************************/
//通讯测试
static void ModMsgIpcNeedleBeadCmd_CommTest(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//模块内输入读取
static void ModMsgIpcNeedleBeadCmd_PortIn(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);  
//模块内输出写入                 
static void ModMsgIpcNeedleBeadCmd_PortOut(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);   
//模块内电机复位               
static void ModMsgIpcNeedleBeadCmd_MotorReset(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);  
//模块内电机走位             
static void ModMsgIpcNeedleBeadCmd_RunSteps(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr); 
//磁珠针复位,带零位偏移                
static void ModMsgIpcNeedleBeadCmd_Reset(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);  
//磁珠针清洗                  
static void ModMsgIpcNeedleBeadCmd_Clean(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr); 
//磁珠针灌注                   
static void ModMsgIpcNeedleBeadCmd_Prime(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);    
//磁珠针清洗维护                
static void ModMsgIpcNeedleBeadCmd_MaintainClean(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//磁珠针旋转指定位置            
static void ModMsgIpcNeedleBeadCmd_Rotate(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr); 
//磁珠针吸液                  
static void ModMsgIpcNeedleBeadCmd_LiquidAbsorb(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr); 
//磁珠针注液            
static void ModMsgIpcNeedleBeadCmd_LiquidInject(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr); 
//磁珠针探液校准            
static void ModMsgIpcNeedleBeadCmd_LiquidDetectAdjust(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//读磁珠针探液板状态       
static void ModMsgIpcNeedleBeadCmd_LiquidDetectReadState(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//磁珠针读取针尖电阻和电压    
static void ModMsgIpcNeedleBeadCmd_LiquidDetectReadResAndVol(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//磁珠针写入探液板电阻
static void ModMsgIpcNeedleBeadCmd_LiquidDetectWriteRes(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);  
//磁珠针读取针尖基准电压   
static void ModMsgIpcNeedleBeadCmd_LiquidDetectReadBaseVol(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr); 
//磁珠针液量余量检查 
static void ModMsgIpcNeedleBeadCmd_LiquidResidueCheck(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);  
//磁珠针升降急停高度读取     
static void ModMsgIpcNeedleBeadCmd_ReadNeedleUpDownEmStopPos(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//磁珠针小磁珠吸吐混匀
static void ModMsgIpcNeedleBeadCmd_LittleBeadMix(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);
//读取探液板版本号            
static void ModMsgIpcNeedleBeadCmd_ReadLiquidBoardVersion(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr);   


//CAN指令处理函数列表
static const USER_IPC_CMD_PROC_UNIT ipcNeedleBeadMsgCmdProcUnitArray[] = 
{
    //顺序是 指令码 预期参数最小长度  指令处理函数
    {(uint32_t)0XFFFFFFFF                                                       ,0    ,ModMsgIpcNeedleBeadCmd_CommTest                  },
    {(uint32_t)IPC_CMD_MACRO_H360_NEEDLE_BEAD_PORT_IN                           ,0    ,ModMsgIpcNeedleBeadCmd_PortIn                    },
    {(uint32_t)IPC_CMD_MACRO_H360_NEEDLE_BEAD_PORT_OUT                          ,0    ,ModMsgIpcNeedleBeadCmd_PortOut                   },
    {(uint32_t)IPC_CMD_MACRO_H360_NEEDLE_BEAD_MOTOR_RESET                       ,0    ,ModMsgIpcNeedleBeadCmd_MotorReset                },
    {(uint32_t)IPC_CMD_MACRO_H360_NEEDLE_BEAD_MOTOR_RUN_STEPS                   ,0    ,ModMsgIpcNeedleBeadCmd_RunSteps                  },
    {(uint32_t)IPC_CMD_MACRO_H360_NEEDLE_BEAD_RESET                             ,0    ,ModMsgIpcNeedleBeadCmd_Reset                     },
    {(uint32_t)IPC_CMD_MACRO_H360_NEEDLE_BEAD_CLEAN                             ,0    ,ModMsgIpcNeedleBeadCmd_Clean                     },
    {(uint32_t)IPC_CMD_MACRO_H360_NEEDLE_BEAD_PRIME                             ,0    ,ModMsgIpcNeedleBeadCmd_Prime                     },
    {(uint32_t)IPC_CMD_MACRO_H360_NEEDLE_BEAD_MAINTAIN_CLEAN                    ,0    ,ModMsgIpcNeedleBeadCmd_MaintainClean             },
    {(uint32_t)IPC_CMD_MACRO_H360_NEEDLE_BEAD_ROTATE                            ,0    ,ModMsgIpcNeedleBeadCmd_Rotate                    },
    {(uint32_t)IPC_CMD_MACRO_H360_NEEDLE_BEAD_LIQUID_ABSORB                     ,0    ,ModMsgIpcNeedleBeadCmd_LiquidAbsorb              },
    {(uint32_t)IPC_CMD_MACRO_H360_NEEDLE_BEAD_LIQUID_INJECT                     ,0    ,ModMsgIpcNeedleBeadCmd_LiquidInject              },
    {(uint32_t)IPC_CMD_MACRO_H360_NEEDLE_BEAD_LIQUID_DETECT_ADJUST              ,0    ,ModMsgIpcNeedleBeadCmd_LiquidDetectAdjust        },
    {(uint32_t)IPC_CMD_MACRO_H360_NEEDLE_BEAD_LIQUID_DETECT_READ_STATE          ,0    ,ModMsgIpcNeedleBeadCmd_LiquidDetectReadState     },
    {(uint32_t)IPC_CMD_MACRO_H360_NEEDLE_BEAD_LIQUID_DETECT_READ_RES_AND_VOL    ,0    ,ModMsgIpcNeedleBeadCmd_LiquidDetectReadResAndVol },
    {(uint32_t)IPC_CMD_MACRO_H360_NEEDLE_BEAD_LIQUID_DETECT_WRITE_RES           ,0    ,ModMsgIpcNeedleBeadCmd_LiquidDetectWriteRes      },
    {(uint32_t)IPC_CMD_MACRO_H360_NEEDLE_BEAD_LIQUID_DETECT_READ_BASE_VOL       ,0    ,ModMsgIpcNeedleBeadCmd_LiquidDetectReadBaseVol   },
    {(uint32_t)IPC_CMD_MACRO_H360_NEEDLE_BEAD_LIQUID_RESIDUE_CHECK              ,0    ,ModMsgIpcNeedleBeadCmd_LiquidResidueCheck        },
    {(uint32_t)IPC_CMD_MACRO_H360_NEEDLE_BEAD_READ_NEEDLE_UP_DOWN_EM_STOP_POS   ,0    ,ModMsgIpcNeedleBeadCmd_ReadNeedleUpDownEmStopPos },
    {(uint32_t)IPC_CMD_MACRO_H360_NEEDLE_BEAD_LITTLE_BEAD_MIX                   ,0    ,ModMsgIpcNeedleBeadCmd_LittleBeadMix             },
    {(uint32_t)IPC_CMD_MACRO_H360_NEEDLE_BEAD_READ_LIQUID_BOARD_VERSION         ,0    ,ModMsgIpcNeedleBeadCmd_ReadLiquidBoardVersion    },
};


//IPC指令消息处理函数
void ModMsgIpcNeedleBeadCmd(void* ipcMsgBodyPtr)
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
    uint16_t indexNeedleBead = 0;
    for(indexNeedleBead = 0; indexNeedleBead < DIM_ARRAY_ELEMENT_COUNT(ipcNeedleBeadMsgCmdProcUnitArray);indexNeedleBead++)
    {
        if(ipcCmdMsgPtr->cmdBaseData.commandCode == (uint32_t)(ipcNeedleBeadMsgCmdProcUnitArray[indexNeedleBead].commandCode))
        {
            break;
        }
    }
    //判定序号
    if(indexNeedleBead == DIM_ARRAY_ELEMENT_COUNT(ipcNeedleBeadMsgCmdProcUnitArray))
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
    if(ipcCmdMsgPtr->cmdBaseData.cmdParamBufferLength < ipcNeedleBeadMsgCmdProcUnitArray[indexNeedleBead].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,CommandCode: 0X%08X\r\n",__func__,__LINE__,
                        ipcNeedleBeadMsgCmdProcUnitArray[indexNeedleBead].exceptParamLengthMin,ipcCmdMsgPtr->cmdBaseData.cmdParamBufferLength,
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
    if(ipcNeedleBeadMsgCmdProcUnitArray[indexNeedleBead].ipcCmdProcFuncPtr == NULL)
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
    ipcNeedleBeadMsgCmdProcUnitArray[indexNeedleBead].ipcCmdProcFuncPtr(&(ipcCmdMsgPtr->cmdBaseData),&sysResultPack,ipcCmdMsgPtr->channelCachePtr);
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
static void ModMsgIpcNeedleBeadCmd_CommTest(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
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
static void ModMsgIpcNeedleBeadCmd_PortIn(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

} 

//模块内输出写入                 
static void ModMsgIpcNeedleBeadCmd_PortOut(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

} 
   
//模块内电机复位               
static void ModMsgIpcNeedleBeadCmd_MotorReset(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

} 
  
//模块内电机走位             
static void ModMsgIpcNeedleBeadCmd_RunSteps(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

} 
 
//磁珠针复位,带零位偏移                
static void ModMsgIpcNeedleBeadCmd_Reset(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

} 
 
//磁珠针清洗                  
static void ModMsgIpcNeedleBeadCmd_Clean(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

} 
 
//磁珠针灌注                   
static void ModMsgIpcNeedleBeadCmd_Prime(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

} 
   
//磁珠针清洗维护                
static void ModMsgIpcNeedleBeadCmd_MaintainClean(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

} 

//磁珠针旋转指定位置            
static void ModMsgIpcNeedleBeadCmd_Rotate(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

} 
 
//磁珠针吸液                  
static void ModMsgIpcNeedleBeadCmd_LiquidAbsorb(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

} 
 
//磁珠针注液            
static void ModMsgIpcNeedleBeadCmd_LiquidInject(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

} 
 
//磁珠针探液校准            
static void ModMsgIpcNeedleBeadCmd_LiquidDetectAdjust(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

} 

//读磁珠针探液板状态       
static void ModMsgIpcNeedleBeadCmd_LiquidDetectReadState(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

} 

//磁珠针读取针尖电阻和电压    
static void ModMsgIpcNeedleBeadCmd_LiquidDetectReadResAndVol(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

} 

//磁珠针写入探液板电阻
static void ModMsgIpcNeedleBeadCmd_LiquidDetectWriteRes(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

} 
  
//磁珠针读取针尖基准电压   
static void ModMsgIpcNeedleBeadCmd_LiquidDetectReadBaseVol(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

} 

//磁珠针液量余量检查 
static void ModMsgIpcNeedleBeadCmd_LiquidResidueCheck(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

} 
 
//磁珠针升降急停高度读取     
static void ModMsgIpcNeedleBeadCmd_ReadNeedleUpDownEmStopPos(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

} 

//磁珠针小磁珠吸吐混匀
static void ModMsgIpcNeedleBeadCmd_LittleBeadMix(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

} 

//读取探液板版本号            
static void ModMsgIpcNeedleBeadCmd_ReadLiquidBoardVersion(IPC_CMD_BASE_DATA* ipcCmdMsgPtr,SYS_RESULT_PACK* resultPackPtr,IPC_CHANNEL_CACHE* channelCachePtr)
{

} 
 

/*************************************************************************************************************************************/

/*************************************************************IPC上报讯息处理**********************************************************/
//占位函数
static void ModMsgIpcNeedleBeadUpload_PlaceHolder(IPC_UPLOAD_DATA* ipcUploadDataPtr);

//IPC上传消息处理函数列表
static const USER_IPC_UPLOAD_PROC_UNIT ipcNeedleBeadMsgUploadProcUnitArray[] = 
{
    //顺序是: 指令码 预期参数最小长度 上传消息处理函数
    {(uint32_t)0XFFFFFFFF     ,2 ,ModMsgIpcNeedleBeadUpload_PlaceHolder},
};


//IPC上传消息处理函数
void ModMsgIpcNeedleBeadUpload(void* ipcMsgBodyPtr)
{
    IPC_UPLOAD_DATA* ipcUploadDataPtr = (IPC_UPLOAD_DATA*)ipcMsgBodyPtr;
    if(ipcUploadDataPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,Recv ipcUploadDataPtr NULL\r\n",__func__,__LINE__);
        //空指针异常
        return;
    }
    //搜索匹配处理器
    uint16_t indexNeedleBead = 0;
    for(indexNeedleBead = 0; indexNeedleBead < DIM_ARRAY_ELEMENT_COUNT(ipcNeedleBeadMsgUploadProcUnitArray);indexNeedleBead++)
    {
        if(ipcUploadDataPtr->uploadID == (uint32_t)(ipcNeedleBeadMsgUploadProcUnitArray[indexNeedleBead].uploadId))
        {
            break;
        }
    }
    //判定序号
    if(indexNeedleBead == DIM_ARRAY_ELEMENT_COUNT(ipcNeedleBeadMsgUploadProcUnitArray))
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
    if(ipcUploadDataPtr->dataLength < ipcNeedleBeadMsgUploadProcUnitArray[indexNeedleBead].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,UploadID: 0X%08X\r\n",__func__,__LINE__,
                        ipcNeedleBeadMsgUploadProcUnitArray[indexNeedleBead].exceptParamLengthMin,ipcUploadDataPtr->dataLength,
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
    if(ipcNeedleBeadMsgUploadProcUnitArray[indexNeedleBead].ipcUploadProcFuncPtr == NULL)
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
    ipcNeedleBeadMsgUploadProcUnitArray[indexNeedleBead].ipcUploadProcFuncPtr(ipcUploadDataPtr);
    //处理完成,释放内存
    if((ipcUploadDataPtr->dataBufferPtr != NULL)&&(ipcUploadDataPtr->dataLength != 0))
    {
        UserMemFree(MEM_REGION_SYS_MSG_IPC,ipcUploadDataPtr->dataBufferPtr);
    }
    UserMemFree(MEM_REGION_SYS_MSG_IPC,ipcUploadDataPtr);
}

//占位函数
static void ModMsgIpcNeedleBeadUpload_PlaceHolder(IPC_UPLOAD_DATA* ipcUploadDataPtr)
{

}

/*************************************************************************************************************************************/















