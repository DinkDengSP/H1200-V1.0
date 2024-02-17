/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-22 09:01:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-12-11 14:46:58 +0800
************************************************************************************************/ 
#include "ModImplCupTrayManage.h"
#include "ModTaskHeader.h"


/*******************************************CAN指令处理函数列表*************************************************/
//占位函数
static void ModMsgCan1CupTrayManageCmd_PlaceHolder(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//模块内输入读取
static void ModMsgCan1CupTrayManageCmd_PortIn(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//模块内输出写入
static void ModMsgCan1CupTrayManageCmd_PortOut(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//模块内电机复位
static void ModMsgCan1CupTrayManageCmd_MotorReset(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//模块内电机走位
static void ModMsgCan1CupTrayManageCmd_MotorRunSteps(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//设置杯栈门状态
static void ModMsgCan1CupTrayManageCmd_SetDoorState(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//新杯盘栈初始化
static void ModMsgCan1CupTrayManageCmd_NewStackInit(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//空杯盘栈初始化
static void ModMsgCan1CupTrayManageCmd_EmptyStackInit(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//推手初始化
static void ModMsgCan1CupTrayManageCmd_PushInit(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//夹手初始化
static void ModMsgCan1CupTrayManageCmd_ClampInit(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//垃圾桶下降
static void ModMsgCan1CupTrayManageCmd_GarbageDownUnlock(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//垃圾桶上升
static void ModMsgCan1CupTrayManageCmd_GarbegeUpLock(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//杯栈模块整体初始化
static void ModMsgCan1CupTrayManageCmd_InitAll(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//夹手释放
static void ModMsgCan1CupTrayManageCmd_ClampOpen(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//夹手夹紧
static void ModMsgCan1CupTrayManageCmd_ClampClose(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//新杯盘栈上传一个新盘
static void ModMsgCan1CupTrayManageCmd_UploadNewTray(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//空杯盘栈从测试区回收一个空盘
static void ModMsgCan1CupTrayManageCmd_RecycleEmptyTray(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//推手推一个新盘到测试位
static void ModMsgCan1CupTrayManageCmd_PushNewTrayToTest(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//新杯盘栈上升指定层(门开到位状态)
static void ModMsgCan1CupTrayManageCmd_NewStackUpSpecialLayerWithDoorOpenState(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//新杯盘栈回零(门开到位状态)
static void ModMsgCan1CupTrayManageCmd_NewStackReturnZero(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//空杯盘上升指定层(门开到位状态)
static void ModMsgCan1CupTrayManageCmd_EmptyStackUpSpecialLayerWithDoorOpenState(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//空杯盘栈回零(门开到位状态)
static void ModMsgCan1CupTrayManageCmd_EmptyStackReturnZero(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//获取模块信息集合
static void ModMsgCan1CupTrayManageCmd_GetMsgSet(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);


//CAN指令处理函数列表
static const USER_CAN_CMD_PROC_UNIT can1CmdProcCupTrayManageUnitArray[] = 
{
    //顺序是 指令码 预期参数最小长度 是否需要结果返回  错误是否独立上报 指令处理函数
    {CAN_MASTER_CMD_PLACE_HOLDER                                                            ,0  ,ENABLE ,ModMsgCan1CupTrayManageCmd_PlaceHolder                                 },
    {CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_PORT_IN                                            ,1  ,ENABLE ,ModMsgCan1CupTrayManageCmd_PortIn                                      },
    {CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_PORT_OUT                                           ,2  ,ENABLE ,ModMsgCan1CupTrayManageCmd_PortOut                                     },
    {CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_MOTOR_RESET                                        ,1  ,ENABLE ,ModMsgCan1CupTrayManageCmd_MotorReset                                  },
    {CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_MOTOR_RUN_STEPS                                    ,5  ,ENABLE ,ModMsgCan1CupTrayManageCmd_MotorRunSteps                               },
    {CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_SET_DOOR_STATE                                     ,1  ,ENABLE ,ModMsgCan1CupTrayManageCmd_SetDoorState                                },
    {CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_NEW_STACK_INIT                                     ,0  ,ENABLE ,ModMsgCan1CupTrayManageCmd_NewStackInit                                },
    {CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_EMPTY_STACK_INIT                                   ,0  ,ENABLE ,ModMsgCan1CupTrayManageCmd_EmptyStackInit                              },
    {CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_PUSH_INIT                                          ,0  ,ENABLE ,ModMsgCan1CupTrayManageCmd_PushInit                                    },
    {CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_CLAMP_INIT                                         ,0  ,ENABLE ,ModMsgCan1CupTrayManageCmd_ClampInit                                   },
    {CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_GARBAGE_DOWN_UNLOCK                                ,1  ,ENABLE ,ModMsgCan1CupTrayManageCmd_GarbageDownUnlock                           },
    {CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_GARBEGE_UP_LOCK                                    ,1  ,ENABLE ,ModMsgCan1CupTrayManageCmd_GarbegeUpLock                               },
    {CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_INIT_ALL                                           ,0  ,ENABLE ,ModMsgCan1CupTrayManageCmd_InitAll                                     },
    {CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_CLAMP_OPEN                                         ,0  ,ENABLE ,ModMsgCan1CupTrayManageCmd_ClampOpen                                   },
    {CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_CLAMP_CLOSE                                        ,0  ,ENABLE ,ModMsgCan1CupTrayManageCmd_ClampClose                                  },
    {CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_UPLOAD_NEW_TRAY                                    ,0  ,ENABLE ,ModMsgCan1CupTrayManageCmd_UploadNewTray                               },
    {CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_RECYCLE_EMPTY_TRAY                                 ,0  ,ENABLE ,ModMsgCan1CupTrayManageCmd_RecycleEmptyTray                            },
    {CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_PUSH_NEW_TRAY_TO_TEST                              ,0  ,ENABLE ,ModMsgCan1CupTrayManageCmd_PushNewTrayToTest                           },
    {CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_NEW_STACK_UP_SPECIAL_LAYER_WITH_DOOR_OPEN_STATE    ,1  ,ENABLE ,ModMsgCan1CupTrayManageCmd_NewStackUpSpecialLayerWithDoorOpenState     },
    {CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_NEW_STACK_RETURN_ZERO                              ,0  ,ENABLE ,ModMsgCan1CupTrayManageCmd_NewStackReturnZero                          },
    {CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_EMPTY_STACK_UP_SPECIAL_LAYER_WITH_DOOR_OPEN_STATE  ,1  ,ENABLE ,ModMsgCan1CupTrayManageCmd_EmptyStackUpSpecialLayerWithDoorOpenState   },
    {CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_EMPTY_STACK_RETURN_ZERO                            ,0  ,ENABLE ,ModMsgCan1CupTrayManageCmd_EmptyStackReturnZero                        },
    {CAN_MASTER_CMD_H360_CUP_TRAY_MANAGE_GET_MSG_SET                                        ,0  ,ENABLE ,ModMsgCan1CupTrayManageCmd_GetMsgSet                                   },
};


//CAN指令消息处理函数
void ModMsgCan1CupTrayManageCmd(void* can1MsgBodyPtr)
{
    //转换指令码
    CAN_PROT_CMD_PACK* cmdPackPtr = (CAN_PROT_CMD_PACK*)can1MsgBodyPtr;
    if(cmdPackPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,Recv cmdPackPtr NULL\r\n",__func__,__LINE__);
        //空指针异常
        return;
    }
    //搜索匹配处理器
    uint16_t indexCupTrayManage = 0;
    for(indexCupTrayManage = 0; indexCupTrayManage < DIM_ARRAY_ELEMENT_COUNT(can1CmdProcCupTrayManageUnitArray);indexCupTrayManage++)
    {
        if(cmdPackPtr->cmdValue == (uint32_t)(can1CmdProcCupTrayManageUnitArray[indexCupTrayManage].commandCode))
        {
            break;
        }
    }
    //判定序号
    if(indexCupTrayManage == DIM_ARRAY_ELEMENT_COUNT(can1CmdProcCupTrayManageUnitArray))
    {
        //不支持的指令
        SystemPrintf("E:> %s,%d,Recv Unsupport CommandCode: 0X%08X\r\n",__func__,__LINE__,cmdPackPtr->cmdValue);
        //释放内存
        if((cmdPackPtr->paramBufferPtr != NULL)&&(cmdPackPtr->paramBufferLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_CAN,cmdPackPtr->paramBufferPtr);
        }
        UserMemFree(MEM_REGION_SYS_MSG_CAN,cmdPackPtr);
        return;
    }
    //判定长度
    if(cmdPackPtr->paramBufferLength < can1CmdProcCupTrayManageUnitArray[indexCupTrayManage].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,CommandCode: 0X%08X\r\n",__func__,__LINE__,
                        can1CmdProcCupTrayManageUnitArray[indexCupTrayManage].exceptParamLengthMin,cmdPackPtr->paramBufferLength,
                        cmdPackPtr->cmdValue);
        //释放内存
        if((cmdPackPtr->paramBufferPtr != NULL)&&(cmdPackPtr->paramBufferLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_CAN,cmdPackPtr->paramBufferPtr);
        }
        UserMemFree(MEM_REGION_SYS_MSG_CAN,cmdPackPtr);
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
    if(can1CmdProcCupTrayManageUnitArray[indexCupTrayManage].canCmdProcFuncPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,canCmdProcFuncPtr NULL,CommandCode: 0X%08X\r\n",__func__,__LINE__,
                        cmdPackPtr->cmdValue);
        //释放内存
        if((cmdPackPtr->paramBufferPtr != NULL)&&(cmdPackPtr->paramBufferLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_CAN,cmdPackPtr->paramBufferPtr);
        }
        UserMemFree(MEM_REGION_SYS_MSG_CAN,cmdPackPtr);
        return;
    }
    //调用处理函数
    can1CmdProcCupTrayManageUnitArray[indexCupTrayManage].canCmdProcFuncPtr(cmdPackPtr,&resultPack,&resultDataBufferPtr,&resultDataLength);
    //结果处理,是否需要结果回传
    if(can1CmdProcCupTrayManageUnitArray[indexCupTrayManage].needResultReply == ENABLE)
    {
        //需要结果回传
        Can1ProtSendResultByRecv(cmdPackPtr,&resultPack,resultDataBufferPtr,resultDataLength);
    }
    //释放内存
    if((resultDataBufferPtr != NULL)&&(resultDataLength != 0))
    {
        UserMemFree(MEM_REGION_MOD_TASK,resultDataBufferPtr);
    }
    if((cmdPackPtr->paramBufferPtr != NULL)&&(cmdPackPtr->paramBufferLength != 0))
    {
        UserMemFree(MEM_REGION_SYS_MSG_CAN,cmdPackPtr->paramBufferPtr);
    }
    UserMemFree(MEM_REGION_SYS_MSG_CAN,cmdPackPtr);
}

//占位函数
static void ModMsgCan1CupTrayManageCmd_PlaceHolder(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//模块内输入读取
static void ModMsgCan1CupTrayManageCmd_PortIn(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //获取序号
    uint8_t portReadIndex = cmdPackPtr->paramBufferPtr[0];
    //日志显示指令
    SystemPrintf("%s,portReadIndex: %d\r\n",__func__,portReadIndex);
    //调用函数实现
    ModImplBaseCupTrayManageReadPortIn(portReadIndex,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//模块内输出写入
static void ModMsgCan1CupTrayManageCmd_PortOut(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //获取序号
    uint8_t portWriteIndex = cmdPackPtr->paramBufferPtr[0];
    //获取值
    PIN_STATE setState = (PIN_STATE)(cmdPackPtr->paramBufferPtr[0]);
    //日志显示指令
    SystemPrintf("%s,portWriteIndex: %d,setState: %d\r\n",__func__,portWriteIndex,(uint8_t)setState);
    //调用函数实现
    ModImplBaseCupTrayManageWritePortOut(portWriteIndex,setState,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//模块内电机复位
static void ModMsgCan1CupTrayManageCmd_MotorReset(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    uint8_t stepMotorIdx = cmdPackPtr->paramBufferPtr[0];
    //日志显示指令
    SystemPrintf("%s,stepMotorIdx: %d\r\n",__func__,stepMotorIdx);
    //调用函数实现
    ModImplBaseCupTrayManageStepMotorReset(stepMotorIdx,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//模块内电机走位
static void ModMsgCan1CupTrayManageCmd_MotorRunSteps(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    uint8_t stepMotorIdx = cmdPackPtr->paramBufferPtr[0];
    int32_t runSteps = ProtUtilReadInt32Buffer(cmdPackPtr->paramBufferPtr+1);
    //日志显示指令
    SystemPrintf("%s,stepMotorIdx: %d,runSteps : %d\r\n",__func__,stepMotorIdx,runSteps);
    //调用函数实现
    ModImplBaseCupTrayManageStepMotorRunSteps(stepMotorIdx,runSteps,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//设置杯栈门状态
static void ModMsgCan1CupTrayManageCmd_SetDoorState(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    CUP_TRAY_MANAGE_DOOR_STATE doorState = (CUP_TRAY_MANAGE_DOOR_STATE)(cmdPackPtr->paramBufferPtr[0]);
    //日志显示指令
    SystemPrintf("%s,doorState: %d\r\n",__func__,doorState);
    //调用函数实现
    ModImplBaseCupTrayManageSetDoorState(doorState,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//新杯盘栈初始化
static void ModMsgCan1CupTrayManageCmd_NewStackInit(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //调用函数实现
    ModImplBaseCupTrayManageNewStackInit(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//空杯盘栈初始化
static void ModMsgCan1CupTrayManageCmd_EmptyStackInit(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //调用函数实现
    ModImplBaseCupTrayManageEmptyStackInit(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//推手初始化
static void ModMsgCan1CupTrayManageCmd_PushInit(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //调用函数实现
    ModImplBaseCupTrayManagePushInit(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//夹手初始化
static void ModMsgCan1CupTrayManageCmd_ClampInit(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //调用函数实现
    ModImplBaseCupTrayManageClampInit(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//垃圾桶下降
static void ModMsgCan1CupTrayManageCmd_GarbageDownUnlock(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    GARBAGE_SELECT garbageNo = (GARBAGE_SELECT)(cmdPackPtr->paramBufferPtr[0]);
    //日志显示指令
    SystemPrintf("%s,garbageNo: %d\r\n",__func__,garbageNo);
    //调用函数实现
    ModImplBaseCupTrayManageGarbageDownUnlock(garbageNo,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//垃圾桶上升
static void ModMsgCan1CupTrayManageCmd_GarbegeUpLock(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    GARBAGE_SELECT garbageNo = (GARBAGE_SELECT)(cmdPackPtr->paramBufferPtr[0]);
    //日志显示指令
    SystemPrintf("%s, garbageNo: %d\r\n",__func__,garbageNo);
    //调用函数实现
    ModImplBaseCupTrayManageGarbegeUpLock(garbageNo,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//杯栈模块整体初始化
static void ModMsgCan1CupTrayManageCmd_InitAll(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //调用函数实现
    ModImplBaseCupTrayManageInitAll(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//夹手释放
static void ModMsgCan1CupTrayManageCmd_ClampOpen(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //调用函数实现
    ModImplBaseCupTrayManageClampOpen(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//夹手夹紧
static void ModMsgCan1CupTrayManageCmd_ClampClose(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //调用函数实现
    ModImplBaseCupTrayManageClampClose(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//新杯盘栈上传一个新盘
static void ModMsgCan1CupTrayManageCmd_UploadNewTray(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //调用函数实现
    ModImplBaseCupTrayManageUploadNewTray(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//空杯盘栈从测试区回收一个空盘
static void ModMsgCan1CupTrayManageCmd_RecycleEmptyTray(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{   
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //调用函数实现
    ModImplBaseCupTrayManageRecycleEmptyTray(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//推手推一个新盘到测试位
static void ModMsgCan1CupTrayManageCmd_PushNewTrayToTest(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //调用函数实现
    ModImplBaseCupTrayManagePushNewTrayToTest(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//新杯盘栈上升指定层(门开到位状态)
static void ModMsgCan1CupTrayManageCmd_NewStackUpSpecialLayerWithDoorOpenState(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    uint8_t layers = cmdPackPtr->paramBufferPtr[0];
    //日志显示指令
    SystemPrintf("%s, layers: %d\r\n",__func__,layers);
    //调用函数实现
    ModImplBaseCupTrayManageNewStackUpSpecialLayerWithDoorOpenState(layers,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//新杯盘栈回零(门开到位状态)
static void ModMsgCan1CupTrayManageCmd_NewStackReturnZero(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //调用函数实现
    ModImplBaseCupTrayManageNewStackReturnZero(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//空杯盘上升指定层(门开到位状态)
static void ModMsgCan1CupTrayManageCmd_EmptyStackUpSpecialLayerWithDoorOpenState(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    uint8_t layers = cmdPackPtr->paramBufferPtr[0];
    //日志显示指令
    SystemPrintf("%s, layers: %d\r\n",__func__,layers);
    //调用函数实现
    ModImplBaseCupTrayManageEmptyStackUpSpecialLayerWithDoorOpenState(layers,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//空杯盘栈回零(门开到位状态)
static void ModMsgCan1CupTrayManageCmd_EmptyStackReturnZero(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //调用函数实现
    ModImplBaseCupTrayManageEmptyStackReturnZero(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//获取模块信息集合
static void ModMsgCan1CupTrayManageCmd_GetMsgSet(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //调用函数实现
    ModImplBaseCupTrayManageGetMsgSet(resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}
/*************************************************************************************************************************************/

/*************************************************************CAN上传参数处理***********************************************************/
//占位函数
static void ModMsgCan1CupTrayManageUpload_PlaceHolder(CAN_PROT_UPLOAD_PACK* canUploadPackPtr);

//CAN上传消息处理函数列表
static const USER_CAN_UPLOAD_PROC_UNIT can1MsgUploadProcCupTrayManageUnitArray[] = 
{
    //顺序是: 指令码 预期参数最小长度 上传消息处理函数
    {CAN_MASTER_REPORT_ID_HOLD_PLACE     ,2 ,ModMsgCan1CupTrayManageUpload_PlaceHolder},
};

//CAN上传消息处理函数
void ModMsgCan1CupTrayManageUpload(void* can1MsgBodyPtr)
{
    CAN_PROT_UPLOAD_PACK* canUploadPackPtr = (CAN_PROT_UPLOAD_PACK*)can1MsgBodyPtr;
    if(canUploadPackPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,Recv canUploadPackPtr NULL\r\n",__func__,__LINE__);
        //空指针异常
        return;
    }
    //搜索匹配处理器
    uint16_t indexCupTrayManage = 0;
    for(indexCupTrayManage = 0; indexCupTrayManage < DIM_ARRAY_ELEMENT_COUNT(can1MsgUploadProcCupTrayManageUnitArray);indexCupTrayManage++)
    {
        if(canUploadPackPtr->uploadID == (uint32_t)(can1MsgUploadProcCupTrayManageUnitArray[indexCupTrayManage].uploadId))
        {
            break;
        }
    }
    //判定序号
    if(indexCupTrayManage == DIM_ARRAY_ELEMENT_COUNT(can1MsgUploadProcCupTrayManageUnitArray))
    {
        //不支持的指令
        SystemPrintf("E:> %s,%d,Recv Unsupport UploadID: 0X%08X\r\n",__func__,__LINE__,canUploadPackPtr->uploadID);
        //释放内存
        if((canUploadPackPtr->uploadDataBufferPtr != NULL)&&(canUploadPackPtr->uploadDataBufferLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackPtr->uploadDataBufferPtr);
        }
        UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackPtr);
        return;
    }
    //判定长度
    if(canUploadPackPtr->uploadDataBufferLength < can1MsgUploadProcCupTrayManageUnitArray[indexCupTrayManage].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,UploadID: 0X%08X\r\n",__func__,__LINE__,
                        can1MsgUploadProcCupTrayManageUnitArray[indexCupTrayManage].exceptParamLengthMin,canUploadPackPtr->uploadDataBufferLength,
                        canUploadPackPtr->uploadID);
        //释放内存
        if((canUploadPackPtr->uploadDataBufferPtr != NULL)&&(canUploadPackPtr->uploadDataBufferLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackPtr->uploadDataBufferPtr);
        }
        UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackPtr);
        return;
    }
    //检测函数指针
    if(can1MsgUploadProcCupTrayManageUnitArray[indexCupTrayManage].canUploadProcFuncPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,canUploadProcFuncPtr NULL,UploadID: 0X%08X\r\n",__func__,__LINE__,
                        canUploadPackPtr->uploadID);
        //释放内存
        if((canUploadPackPtr->uploadDataBufferPtr != NULL)&&(canUploadPackPtr->uploadDataBufferLength != 0))
        {
            UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackPtr->uploadDataBufferPtr);
        }
        UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackPtr);
        return;
    }
    //调用指令处理
    can1MsgUploadProcCupTrayManageUnitArray[indexCupTrayManage].canUploadProcFuncPtr(canUploadPackPtr);
    //处理完成,释放内存
    if((canUploadPackPtr->uploadDataBufferPtr != NULL)&&(canUploadPackPtr->uploadDataBufferLength != 0))
    {
        UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackPtr->uploadDataBufferPtr);
    }
    UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackPtr);
}

//占位函数
static void ModMsgCan1CupTrayManageUpload_PlaceHolder(CAN_PROT_UPLOAD_PACK* canUploadPackPtr)
{

}
/*************************************************************************************************************************************/











