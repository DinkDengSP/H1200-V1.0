/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-31 08:43:34 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-31 15:54:34 +0800
************************************************************************************************/ 
#include "SrvImplFlashIap.h"
#include "SrvImplBaseFlashIap.h"
#include "CommonSrvTaskHeader.h"

//导入升级头文件
extern BOARD_IAP_FILE_INFO boardIapFileInfo;

/*******************************************CAN指令处理函数列表*************************************************/
//占位函数
static void SrvMsgCan2FlashIapCmd_PlaceHolder(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//升级准备
static void SrvMsgCan2FlashIapCmd_Prepare(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//升级开始讯息
static void SrvMsgCan2FlashIapCmd_StartMsg(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//升级数据传输
static void SrvMsgCan2FlashIapCmd_DataTrans(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//升级数据校验
static void SrvMsgCan2FlashIapCmd_DataCheck(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//强制跳转
static void SrvMsgCan2FlashIapCmd_ForceJmp(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);

//CAN指令处理函数列表
static const USER_CAN_CMD_PROC_UNIT can2CmdProcFlashIapUnitArray[] = 
{
    //顺序是 指令码 预期参数最小长度 是否需要结果返回  错误是否独立上报 指令处理函数
    {CAN_MASTER_CMD_PLACE_HOLDER    ,0  ,ENABLE ,SrvMsgCan2FlashIapCmd_PlaceHolder  },
    {CAN_MASTER_CMD_IAP_PREPARE     ,1  ,ENABLE ,SrvMsgCan2FlashIapCmd_Prepare  },
    {CAN_MASTER_CMD_IAP_START_MSG   ,12 ,ENABLE ,SrvMsgCan2FlashIapCmd_StartMsg },
    {CAN_MASTER_CMD_IAP_DATA_TRANS  ,2  ,ENABLE ,SrvMsgCan2FlashIapCmd_DataTrans},
    {CAN_MASTER_CMD_IAP_DATA_CHECK  ,4  ,ENABLE ,SrvMsgCan2FlashIapCmd_DataCheck},
    {CAN_MASTER_CMD_IAP_FORCE_JMP   ,1  ,ENABLE ,SrvMsgCan2FlashIapCmd_ForceJmp },
};


//CAN指令消息处理函数
void SrvMsgCan2FlashIapCmd(void* can2MsgBodyPtr)
{
    //转换指令码
    CAN_PROT_CMD_PACK* cmdPackPtr = (CAN_PROT_CMD_PACK*)can2MsgBodyPtr;
    if(cmdPackPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,Recv cmdPackPtr NULL\r\n",__func__,__LINE__);
        //空指针异常
        return;
    }
    //搜索匹配处理器
    uint16_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < DIM_ARRAY_ELEMENT_COUNT(can2CmdProcFlashIapUnitArray);indexUtil++)
    {
        if(cmdPackPtr->cmdValue == (uint32_t)(can2CmdProcFlashIapUnitArray[indexUtil].commandCode))
        {
            break;
        }
    }
    //判定序号
    if(indexUtil == DIM_ARRAY_ELEMENT_COUNT(can2CmdProcFlashIapUnitArray))
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
    if(cmdPackPtr->paramBufferLength < can2CmdProcFlashIapUnitArray[indexUtil].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,CommandCode: 0X%08X\r\n",__func__,__LINE__,
                        can2CmdProcFlashIapUnitArray[indexUtil].exceptParamLengthMin,cmdPackPtr->paramBufferLength,
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
    if(can2CmdProcFlashIapUnitArray[indexUtil].canCmdProcFuncPtr == NULL)
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
    can2CmdProcFlashIapUnitArray[indexUtil].canCmdProcFuncPtr(cmdPackPtr,&resultPack,&resultDataBufferPtr,&resultDataLength);
    //结果处理,是否需要结果回传
    if(can2CmdProcFlashIapUnitArray[indexUtil].needResultReply == ENABLE)
    {
        //需要结果回传
        Can2ProtSendResultByRecv(cmdPackPtr,&resultPack,resultDataBufferPtr,resultDataLength);
    }
    //释放内存
    if((resultDataBufferPtr != NULL)&&(resultDataLength != 0))
    {
        UserMemFree(MEM_REGION_SRV_TASK,resultDataBufferPtr);
    }
    if((cmdPackPtr->paramBufferPtr != NULL)&&(cmdPackPtr->paramBufferLength != 0))
    {
        UserMemFree(MEM_REGION_SYS_MSG_CAN,cmdPackPtr->paramBufferPtr);
    }
    UserMemFree(MEM_REGION_SYS_MSG_CAN,cmdPackPtr);
}

//占位函数
static void SrvMsgCan2FlashIapCmd_PlaceHolder(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志显示指令
    SystemPrintf("%s\r\n",__func__);
    //构建完成无错误
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//升级准备
static void SrvMsgCan2FlashIapCmd_Prepare(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    BOARD_IAP_RECODER iapRecoder;
    ERROR_SUB errorSub = ERROR_SUB_OK;
    ERROR_SUB errorSubSub = ERROR_SUB_OK;
    //初始化升级头文件
    BoardIapClearFileInfo(&boardIapFileInfo);
    //获取升级类型,BOOT下只能升级APP,如果想要升级BOOT,就跳转到APP里面去
    uint8_t fileIapMode = cmdPackPtr->paramBufferPtr[0];
    //日志打印
    SystemPrintf("FlashIap,Mode: %d\r\n",fileIapMode);
    if(fileIapMode == BOARD_IAP_MODE_APP)
    {
        //设置标记并跳转BOOT
        iapRecoder.iapFileExistFlag = BOARD_IAP_RECODER_FLAG_EXIST;
        iapRecoder.iapFileTotalBytes = 0;
        iapRecoder.boardIapSrc = BOARD_IAP_SRC_CAN2;
        iapRecoder.crc32Code = 0X00000000;
        //写入标记
        errorSub = BoardIapWriteAppRecord(&iapRecoder);
        if(errorSub != ERROR_SUB_OK)
        {
            //清除标记
            errorSubSub = BoardIapWriteAppRecordDefault();
            if(errorSubSub != ERROR_SUB_OK)
            {
                //错误日志
                SystemPrintf("%s,%d,BoardIapWriteAppRecordDefault,ErrorSub: 0X%08X\r\n",__func__,__LINE__,errorSubSub);
            }
            //错误日志
            SystemPrintf("%s,%d,BoardIapWriteAppRecord,ErrorSub: 0X%08X\r\n",__func__,__LINE__,errorSub);
            //清除升级讯息
            BoardIapClearFileInfo(&boardIapFileInfo);
            //设置错误信息
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_FLASH_IAP_PREPARE,
                                ERROR_LEVEL_ERROR,errorSub);
            return;
        }
        //检查BOOT是否存在
        if(ERROR == BoardIapCheckBootExist())
        {
            //APP不存在,升级不了
            SystemPrintf("%s,%d,BoardIapCheckBootExist,BootNotExist.\r\n",__func__,__LINE__);
            //清除升级讯息
            BoardIapClearFileInfo(&boardIapFileInfo);
            //设置错误信息
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_FLASH_IAP_PREPARE,
                                ERROR_LEVEL_ERROR,ERROR_SUB_BOARD_IAP_BOOT_NOT_EXIST);
            return;
        }
        //延时100MS
        CoreDelayMs(100);
        //跳转到Boot
        BoardIapJumpBoot();
        //死循环
        while(1);
    }
    else if(fileIapMode == BOARD_IAP_MODE_BOOT)
    {
        boardIapFileInfo.boardIapMode = BOARD_IAP_MODE_BOOT;
        //在线升级的文件是BOOT
        CoreUtilSetSystemResultPackNoError(resultPackPtr);
        return;
    }
    else if(fileIapMode == BOARD_IAP_MODE_SRV)
    {
        //设置标记并跳转BOOT
        iapRecoder.iapFileExistFlag = BOARD_IAP_RECODER_FLAG_EXIST;
        iapRecoder.iapFileTotalBytes = 0;
        iapRecoder.boardIapSrc = BOARD_IAP_SRC_CAN2;
        iapRecoder.crc32Code = 0X00000000;
        //写入标记
        errorSub = BoardIapWriteSrvRecord(&iapRecoder);
        if(errorSub != ERROR_SUB_OK)
        {
            //清除标记
            errorSubSub = BoardIapWriteSrvRecordDefault();
            if(errorSubSub != ERROR_SUB_OK)
            {
                //错误日志
                SystemPrintf("%s,%d,BoardIapWriteSrvRecordDefault,ErrorSub: 0X%08X\r\n",__func__,__LINE__,errorSubSub);
            }
            //错误日志
            SystemPrintf("%s,%d,BoardIapWriteSrvRecord,ErrorSub: 0X%08X\r\n",__func__,__LINE__,errorSub);
            //清除升级讯息
            BoardIapClearFileInfo(&boardIapFileInfo);
            //设置错误信息
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_FLASH_IAP_PREPARE,
                                ERROR_LEVEL_ERROR,errorSub);
            return;
        }
        //检查BOOT是否存在
        if(ERROR == BoardIapCheckBootExist())
        {
            //APP不存在,升级不了
            SystemPrintf("%s,%d,BoardIapCheckBootExist,BootNotExist.\r\n",__func__,__LINE__);
            //清除升级讯息
            BoardIapClearFileInfo(&boardIapFileInfo);
            //设置错误信息
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_FLASH_IAP_PREPARE,
                                ERROR_LEVEL_ERROR,ERROR_SUB_BOARD_IAP_BOOT_NOT_EXIST);
            return;
        }
        //延时100MS
        CoreDelayMs(100);
        //跳转到Boot
        BoardIapJumpBoot();
        //死循环
        while(1);
    }
    else
    {
        //错误日志
        SystemPrintf("%s,%d,NotInvalid Mode: %d\r\n",__func__,__LINE__,fileIapMode);
        //设置错误信息
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_FLASH_IAP_PREPARE,
                            ERROR_LEVEL_ERROR,ERROR_SUB_BOARD_IAP_APP_INVALID_TYPE);
        return;
    }
}

//升级开始讯息
static void SrvMsgCan2FlashIapCmd_StartMsg(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //升级开始讯息获取
    boardIapFileInfo.totalFileBytes = ProtUtilReadUint32Buffer(cmdPackPtr->paramBufferPtr);
    boardIapFileInfo.totalPackCount = ProtUtilReadUint32Buffer(cmdPackPtr->paramBufferPtr+4);
    boardIapFileInfo.singlePackBytes = ProtUtilReadUint32Buffer(cmdPackPtr->paramBufferPtr+8);
    //日志打印
    SystemPrintf("FlashIap,StartMsg.totalFileBytes: %d,totalPackCount: %d,singlePackBytes: %d\r\n",
                    boardIapFileInfo.totalFileBytes,boardIapFileInfo.totalPackCount,boardIapFileInfo.singlePackBytes);
    //在线升级启动信息
    SrvImplBaseFlashIapStartMsg(&boardIapFileInfo,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//升级数据传输
static void SrvMsgCan2FlashIapCmd_DataTrans(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //获取数据元素讯息
    uint16_t currentPackIndex = ProtUtilReadUint16Buffer(cmdPackPtr->paramBufferPtr);
    //获取参数长度
    uint16_t packDataLength = cmdPackPtr->paramBufferLength - 2;
    //获取数据指针
    uint8_t* writeBufferPtr = cmdPackPtr->paramBufferPtr + 2;
    //日志打印
    SystemPrintf("FlashIap,DataTrans,PackIndex: %d,DataLength: %d\r\n",currentPackIndex,packDataLength);
    //在线升级数据传输
    SrvImplBaseFlashIapDataTrans(currentPackIndex,packDataLength,writeBufferPtr,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//升级数据校验
static void SrvMsgCan2FlashIapCmd_DataCheck(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //检查是否设置了模式
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //计算CRC
    uint32_t crc32CodeByCalc = 0XFFFFFFFF;
    //检查有没有收到升级准备
    if(boardIapFileInfo.boardIapMode != BOARD_IAP_MODE_BOOT)
    {
        //错误日志
        SystemPrintf("%s,%d,Not Recv Head Prepare\r\n",__func__,__LINE__);
        //没收到升级准备
        BoardIapClearFileInfo(&boardIapFileInfo);
        //设置错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_FLASH_IAP_CRC,
                                ERROR_LEVEL_ERROR,ERROR_SUB_BOARD_IAP_APP_NOT_PREPARE);
        return;
    }
    //检查接收包数量和接收数据长度是否完全匹配
    if(boardIapFileInfo.recordRecvFilePackCount != boardIapFileInfo.totalPackCount)
    {
        //错误日志
        SystemPrintf("%s,%d,PackCountNotMatch,Recv: %d,Record: %d\r\n",__func__,__LINE__,
                        boardIapFileInfo.recordRecvFilePackCount,boardIapFileInfo.totalPackCount);
        //包数量不匹配
        BoardIapClearFileInfo(&boardIapFileInfo);
        //设置错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_FLASH_IAP_CRC,
                                ERROR_LEVEL_ERROR,ERROR_SUB_BOARD_IAP_APP_FILE_PACK_COUNT_NOT_MATCH);
        return;
    }
    if(boardIapFileInfo.recordRecvFileByteCount != boardIapFileInfo.totalFileBytes)
    {
        //错误日志
        SystemPrintf("%s,%d,fileBytes Not Match,Recv: %d,Record: %d\r\n",__func__,__LINE__,
                        boardIapFileInfo.recordRecvFileByteCount,boardIapFileInfo.totalFileBytes);
        //字节数量不匹配
        BoardIapClearFileInfo(&boardIapFileInfo);
        //设置错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_FLASH_IAP_CRC,
                                ERROR_LEVEL_ERROR,ERROR_SUB_BOARD_IAP_APP_DATA_BYTES_NOT_MATCH_HEADER);
        return;
    }
    //获取发送过来的CRC
    boardIapFileInfo.crc32CodeRecv = ProtUtilReadUint32Buffer(cmdPackPtr->paramBufferPtr);
    //日志打印
    SystemPrintf("FlashIap,Crc32 Recv: 0X%08X\r\n",boardIapFileInfo.crc32CodeRecv);
    //计算写入数据CRC
    crc32CodeByCalc = BoardIapCalcCRC32Once((uint8_t const*)MCU_FLASH_BOOT,boardIapFileInfo.totalFileBytes);
    //匹配CRC
    if(crc32CodeByCalc != boardIapFileInfo.crc32CodeRecv)
    {
        //错误日志
        SystemPrintf("%s,%d,CRC32 NotMatch,Recv: 0X%08X,Calc: 0X%08X\r\n",__func__,__LINE__,
                        boardIapFileInfo.crc32CodeRecv,crc32CodeByCalc);
        //清除记录
        BoardIapClearFileInfo(&boardIapFileInfo);
        //设置错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_FLASH_IAP_CRC,
                                ERROR_LEVEL_ERROR,ERROR_SUB_BOARD_IAP_APP_DATA_CRC32_NOT_MATCH);
        return;
    }
    //清空标记
    errorSub = BoardIapWriteAppRecordDefault();
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s ,%d,BoardIapWriteAppRecordDefault ErrorSub: 0X%08X\r\n",__func__,__LINE__,errorSub);
        //设置错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_FLASH_IAP_CRC,
                                ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    //清空标记
    errorSub = BoardIapWriteSrvRecordDefault();
    if(errorSub != ERROR_SUB_OK)
    {
        SystemPrintf("%s ,%d,BoardIapWriteSrvRecordDefault ErrorSub: 0X%08X\r\n",__func__,__LINE__,errorSub);
        //设置错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_FLASH_IAP_CRC,
                                ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    //正常完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    //回传成功
    Can2ProtSendResultByRecv(cmdPackPtr,resultPackPtr,NULL,0);
    //延迟跳转
    CoreDelayMs(200);
    //跳转到APP
    errorSub = BoardIapJumpApp();
    //跳转失败
    SystemPrintf("%s ,%d,BoardIapJumpApp ErrorSub: 0X%08X\r\n",__func__,__LINE__,errorSub);
    while(1);
}


//强制跳转
static void SrvMsgCan2FlashIapCmd_ForceJmp(CAN_PROT_CMD_PACK* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    BOARD_FORCE_JMP jmpTarget;
    if(0 == cmdPackPtr->paramBufferPtr[0])
    {
        jmpTarget = BOARD_FORCE_JMP_BOOT;
    }
    else
    {
        jmpTarget = BOARD_FORCE_JMP_MAIN;
    }
    //日志打印
    SystemPrintf("FlashIap,ForceJump: %d\r\n",(uint8_t)jmpTarget);
    if(jmpTarget == BOARD_FORCE_JMP_MAIN)
    {
        //跳转成功
        CoreUtilSetSystemResultPackNoError(resultPackPtr);
        return;
    }
    else
    {
        if(ERROR == BoardIapCheckBootExist())
        {
            SystemPrintf("%s ,%d,BoardIapCheckBootExist ErrorSub: 0X%08X\r\n",__func__,__LINE__,ERROR_SUB_BOARD_IAP_APP_FORCE_JMP_BOOT_NOT_EXIST);
            //设置错误
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_FLASH_IAP_FORCE_JMP,
                                    ERROR_LEVEL_ERROR,ERROR_SUB_BOARD_IAP_APP_FORCE_JMP_BOOT_NOT_EXIST);
            return;
        }
        //正常完成
        CoreUtilSetSystemResultPackNoError(resultPackPtr);
        //回传成功
        Can2ProtSendResultByRecv(cmdPackPtr,resultPackPtr,NULL,0);
        //延迟跳转
        CoreDelayMs(200);
        //跳转到BOOT
        errorSub = BoardIapJumpBoot();
        //跳转失败
        SystemPrintf("%s ,%d,BoardIapJumpBoot ErrorSub: 0X%08X\r\n",__func__,__LINE__,errorSub);
        while(1);
    }
}
/*************************************************************************************************************************************/

/*************************************************************CAN上传参数处理***********************************************************/
//占位函数
static void SrvMsgCan2FlashIapUpload_PlaceHolder(CAN_PROT_UPLOAD_PACK* canUploadPackPtr);

//CAN上传消息处理函数列表
static const USER_CAN_UPLOAD_PROC_UNIT can2MsgUploadProcUnitArray[] = 
{
    //顺序是: 指令码 预期参数最小长度 上传消息处理函数
    {CAN_MASTER_REPORT_ID_HOLD_PLACE     ,2 ,SrvMsgCan2FlashIapUpload_PlaceHolder},
};

//CAN上传消息处理函数
void SrvMsgCan2FlashIapUpload(void* can2MsgBodyPtr)
{
    CAN_PROT_UPLOAD_PACK* canUploadPackPtr = (CAN_PROT_UPLOAD_PACK*)can2MsgBodyPtr;
    if(canUploadPackPtr == NULL)
    {
        SystemPrintf("E:> %s,%d,Recv canUploadPackPtr NULL\r\n",__func__,__LINE__);
        //空指针异常
        return;
    }
    //搜索匹配处理器
    uint16_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < DIM_ARRAY_ELEMENT_COUNT(can2MsgUploadProcUnitArray);indexUtil++)
    {
        if(canUploadPackPtr->uploadID == (uint32_t)(can2MsgUploadProcUnitArray[indexUtil].uploadId))
        {
            break;
        }
    }
    //判定序号
    if(indexUtil == DIM_ARRAY_ELEMENT_COUNT(can2MsgUploadProcUnitArray))
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
    if(canUploadPackPtr->uploadDataBufferLength < can2MsgUploadProcUnitArray[indexUtil].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,UploadID: 0X%08X\r\n",__func__,__LINE__,
                        can2MsgUploadProcUnitArray[indexUtil].exceptParamLengthMin,canUploadPackPtr->uploadDataBufferLength,
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
    if(can2MsgUploadProcUnitArray[indexUtil].canUploadProcFuncPtr == NULL)
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
    can2MsgUploadProcUnitArray[indexUtil].canUploadProcFuncPtr(canUploadPackPtr);
    //处理完成,释放内存
    if((canUploadPackPtr->uploadDataBufferPtr != NULL)&&(canUploadPackPtr->uploadDataBufferLength != 0))
    {
        UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackPtr->uploadDataBufferPtr);
    }
    UserMemFree(MEM_REGION_SYS_MSG_CAN,canUploadPackPtr);
}

//占位函数
static void SrvMsgCan2FlashIapUpload_PlaceHolder(CAN_PROT_UPLOAD_PACK* canUploadPackPtr)
{

}
/*************************************************************************************************************************************/









