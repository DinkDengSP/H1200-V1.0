/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-29 11:51:45
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-30 11:24:05
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#include "SrvImplFlashIap.h"
#include "SrvImplBaseFlashIap.h"
#include "SrvTaskHeader.h"

//导入升级头文件
extern BOARD_IAP_FILE_INFO boardIapFileInfo;


/****************************************网口指令消息处理函数**************************************************/
//占位函数
static void SrvMsgNet8700FlashIapCmd_HoldPlace(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//在线升级准备
static void SrvMsgNet8700FlashIapCmd_Prepare(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//在线升级启动信息
static void SrvMsgNet8700FlashIapCmd_StartMsg(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//在线升级数据传输
static void SrvMsgNet8700FlashIapCmd_DataTrans(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//在线升级数据校验
static void SrvMsgNet8700FlashIapCmd_DataCheck(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);
//在线升级强制跳转
static void SrvMsgNet8700FlashIapCmd_ForceJmp(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen);


//NET指令处理函数列表
static const USER_NET_CMD_PROC_UNIT net8700CmdProcFlashIapUnitArray[] = 
{
    //顺序: 指令码  预期参数最小长度 是否需要结果返回  错误是否独立上报 指令处理函数
    {(uint32_t)NET_MASTER_CMD_HOLD_PLACE            ,0  ,ENABLE ,SrvMsgNet8700FlashIapCmd_HoldPlace },
    {(uint32_t)NET_MASTER_CMD_MAIN_IAP_PREPARE      ,1  ,ENABLE ,SrvMsgNet8700FlashIapCmd_Prepare   },
    {(uint32_t)NET_MASTER_CMD_MAIN_IAP_START_MSG    ,12 ,ENABLE ,SrvMsgNet8700FlashIapCmd_StartMsg  },
    {(uint32_t)NET_MASTER_CMD_MAIN_IAP_DATA_TRANS   ,2  ,ENABLE ,SrvMsgNet8700FlashIapCmd_DataTrans },
    {(uint32_t)NET_MASTER_CMD_MAIN_IAP_DATA_CHECK   ,4  ,ENABLE ,SrvMsgNet8700FlashIapCmd_DataCheck },
    {(uint32_t)NET_MASTER_CMD_MAIN_IAP_FORCE_JMP    ,1  ,ENABLE ,SrvMsgNet8700FlashIapCmd_ForceJmp  },
};

void SrvMsgNet8700FlashIapCmd(void* netCmdRecvPtr)
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
    SystemPrintf("FlashIap Recv Net Cmd: 0X%08X,DataLen: %d\r\n",cmdPackPtr->cmdCode,cmdPackPtr->dataBodyLength);
    //搜索匹配处理器
    uint16_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < DIM_ARRAY_ELEMENT_COUNT(net8700CmdProcFlashIapUnitArray);indexUtil++)
    {
        if(cmdPackPtr->cmdCode == (uint32_t)(net8700CmdProcFlashIapUnitArray[indexUtil].commandCode))
        {
            break;
        }
    }
    //判定序号
    if(indexUtil == DIM_ARRAY_ELEMENT_COUNT(net8700CmdProcFlashIapUnitArray))
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
    if(cmdPackPtr->dataBodyLength < net8700CmdProcFlashIapUnitArray[indexUtil].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,CommandCode: 0X%08X\r\n",__func__,__LINE__,
                        net8700CmdProcFlashIapUnitArray[indexUtil].exceptParamLengthMin,cmdPackPtr->dataBodyLength,
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
    if(net8700CmdProcFlashIapUnitArray[indexUtil].netCmdProcFuncPtr == NULL)
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
    net8700CmdProcFlashIapUnitArray[indexUtil].netCmdProcFuncPtr(cmdPackPtr,&resultPack,&resultDataBufferPtr,&resultDataLength);
    //结果处理,是否需要结果回传
    if(net8700CmdProcFlashIapUnitArray[indexUtil].needResultReply == ENABLE)
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
static void SrvMsgNet8700FlashIapCmd_HoldPlace(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //日志打印
    SystemPrintf("%s\r\n",__func__);
    //正常完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//在线升级准备
static void SrvMsgNet8700FlashIapCmd_Prepare(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //获取升级类型,IAP下只能升级APP
    uint8_t fileIapMode = cmdPackPtr->dataBodyBufferPtr[0];
    //日志打印
    SystemPrintf("FlashIap,Mode: %d\r\n",fileIapMode);
    //执行在线升级准备
    SrvImplBaseFlashIapPrepare(fileIapMode,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//在线升级启动信息
static void SrvMsgNet8700FlashIapCmd_StartMsg(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //升级开始讯息获取
    boardIapFileInfo.totalFileBytes = ProtUtilReadUint32Buffer(cmdPackPtr->dataBodyBufferPtr);
    boardIapFileInfo.totalPackCount = ProtUtilReadUint32Buffer(cmdPackPtr->dataBodyBufferPtr+4);
    boardIapFileInfo.singlePackBytes = ProtUtilReadUint32Buffer(cmdPackPtr->dataBodyBufferPtr+8);
    //日志打印
    SystemPrintf("FlashIap,StartMsg.totalFileBytes: %d,totalPackCount: %d,singlePackBytes: %d\r\n",
                    boardIapFileInfo.totalFileBytes,boardIapFileInfo.totalPackCount,boardIapFileInfo.singlePackBytes);
    //在线升级启动信息
    SrvImplBaseFlashIapStartMsg(&boardIapFileInfo,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//在线升级数据传输
static void SrvMsgNet8700FlashIapCmd_DataTrans(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //获取数据元素讯息
    uint16_t currentPackIndex = ProtUtilReadUint16Buffer(cmdPackPtr->dataBodyBufferPtr);
    //获取参数长度
    uint16_t packDataLength = cmdPackPtr->dataBodyLength - 2;
    //获取数据指针
    uint8_t* writeBufferPtr = cmdPackPtr->dataBodyBufferPtr + 2;
    //日志打印
    SystemPrintf("FlashIap,DataTrans,PackIndex: %d,DataLength: %d\r\n",currentPackIndex,packDataLength);
    //在线升级数据传输
    SrvImplBaseFlashIapDataTrans(currentPackIndex,packDataLength,writeBufferPtr,resultPackPtr,dataBufPtrPtr,dataBufLen);
    return;
}

//在线升级数据校验
static void SrvMsgNet8700FlashIapCmd_DataCheck(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    MCU_FLASH_CMP_RESULT flashCmpResult = MCU_FLASH_CMP_EQU;
    //检查是否设置了模式
    ERROR_SUB errorSub = ERROR_SUB_OK;
    ERROR_SUB errorSubLocal = ERROR_SUB_OK;
    boardIapFileInfo.crc32CodeRecv = ProtUtilReadUint32Buffer(cmdPackPtr->dataBodyBufferPtr);
    //日志打印
    SystemPrintf("FlashIap,Crc32 Recv: 0X%08X\r\n",boardIapFileInfo.crc32CodeRecv);
    //检查有没有收到升级准备
    if((boardIapFileInfo.boardIapMode != BOARD_IAP_MODE_BOOT)&&(boardIapFileInfo.boardIapMode != BOARD_IAP_MODE_SRV)&&
        (boardIapFileInfo.boardIapMode != BOARD_IAP_MODE_APP))
    {
        //错误日志
        SystemPrintf("%s,%d,Not Recv Head Prepare\r\n",__func__,__LINE__);
        //没收到升级准备
        BoardIapClearFileInfo(&boardIapFileInfo);
        //设置错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_DMC_FLASH_IAP_CRC,
                                    ERROR_LEVEL_ERROR,ERROR_SUB_BOARD_IAP_BOOT_NOT_PREPARE);
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
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_DMC_FLASH_IAP_CRC,
                                    ERROR_LEVEL_ERROR,ERROR_SUB_BOARD_IAP_BOOT_FILE_PACK_COUNT_NOT_MATCH);
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
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_DMC_FLASH_IAP_CRC,
                                    ERROR_LEVEL_ERROR,ERROR_SUB_BOARD_IAP_BOOT_DATA_BYTES_NOT_MATCH_HEADER);
        return;
    }
    //申请内存
    uint8_t* iapFileBinArray = UserMemMallocWhileSuccess(MEM_SDRAM1,boardIapFileInfo.totalFileBytes);
    //数据读取
    if(boardIapFileInfo.boardIapMode == BOARD_IAP_MODE_BOOT)
    {
        errorSub = BoardIapReadBootBinFile(0,iapFileBinArray,boardIapFileInfo.totalFileBytes);
    }
    else if(boardIapFileInfo.boardIapMode == BOARD_IAP_MODE_SRV)
    {
        errorSub = BoardIapReadSrvBinFile(0,iapFileBinArray,boardIapFileInfo.totalFileBytes);
    }
    else 
    {
        errorSub = BoardIapReadSrvBinFile(0,iapFileBinArray,boardIapFileInfo.totalFileBytes);
    }
    if(errorSub != ERROR_SUB_OK)
    {
        //错误日志
        SystemPrintf("%s,%d,ReadBinFileFailed,ErrorSub: 0X%08X\r\n",__func__,__LINE__,errorSub);
        BoardIapClearFileInfo(&boardIapFileInfo);
        //释放内存
        UserMemFree(MEM_SDRAM1,iapFileBinArray);
        //设置错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_DMC_FLASH_IAP_CRC,
                                    ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    //计算CRC
    uint32_t crc32CodeByCalc = 0XFFFFFFFF;
    crc32CodeByCalc = BoardIapCalcCRC32(iapFileBinArray,boardIapFileInfo.totalFileBytes);
    //匹配CRC
    if(crc32CodeByCalc != boardIapFileInfo.crc32CodeRecv)
    {
        //错误日志
        SystemPrintf("%s,%d,CRC32 NotMatch,Recv: 0X%08X,Calc: 0X%08X\r\n",__func__,__LINE__,
                        boardIapFileInfo.crc32CodeRecv,crc32CodeByCalc);
        //清除记录
        BoardIapClearFileInfo(&boardIapFileInfo);
        //释放内存
        UserMemFree(MEM_SDRAM1,iapFileBinArray);
        //CRC不匹配
        BoardIapClearFileInfo(&boardIapFileInfo);
        //设置错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_DMC_FLASH_IAP_CRC,
                                    ERROR_LEVEL_ERROR,ERROR_SUB_BOARD_IAP_BOOT_DATA_CRC32_NOT_MATCH);
        return;
    }
    BOARD_IAP_RECODER iapRecoder;
    iapRecoder.iapFileExistFlag = BOARD_IAP_RECODER_FLAG_EXIST;
    iapRecoder.boardIapSrc = BOARD_IAP_SRC_NET;
    iapRecoder.iapFileTotalBytes = boardIapFileInfo.totalFileBytes;
    iapRecoder.crc32Code = crc32CodeByCalc;
    //此时逻辑变化,如果是BOOT升级,写入BOOT升级标记并且转入APP,如果是SRV或者APP,执行写入
    if(boardIapFileInfo.boardIapMode == BOARD_IAP_MODE_BOOT)
    {
        //写入BOOT升级文件信息
        errorSub = BoardIapWriteBootRecord(&iapRecoder);
        if(errorSub != ERROR_SUB_OK)
        {
            //错误日志
            SystemPrintf("%s ,%d,BoardIapWriteBootRecord ErrorSub: 0X%08X\r\n",__func__,__LINE__,errorSub);
            //释放内存
            UserMemFree(MEM_SDRAM1,iapFileBinArray);
            BoardIapClearFileInfo(&boardIapFileInfo);
            //设置错误
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_DMC_FLASH_IAP_CRC,
                                        ERROR_LEVEL_ERROR,errorSub);
            return;
        }
        //释放内存
        UserMemFree(MEM_SDRAM1,iapFileBinArray);
        //正常完成
        CoreUtilSetSystemResultPackNoError(resultPackPtr);
        //回传成功
        NetMasterSendResultByRecv(cmdPackPtr,resultPackPtr,NULL,0);
        //延时跳转
        CoreDelayMs(200);
        //跳转到APP中进行BOOT升级
        errorSub = BoardIapJumpApp();
        //跳转错误
        SystemPrintf("%s,%d,BoardIapJumpApp Failed,ErrorCode: 0X%08X\r\n",__func__,__LINE__,errorSub);
        //但凡能走到这里,都出现了异常,清除信息并且报错
        errorSub = BoardIapWriteBootRecordDefault();
        if(errorSub != ERROR_SUB_OK)
        {
            SystemPrintf("%s,%d,BoardIapWriteAppRecordDefault Failed,ErrorCode: 0X%08X\r\n",
                            __func__,__LINE__,errorSub);
        }
        BoardIapClearFileInfo(&boardIapFileInfo);
        return;
    }
    else
    {
        //擦除Flash
        errorSub = MCU_FlashEraseRangeWithCheck(MCU_FLASH_SRV_MAIN,boardIapFileInfo.totalFileBytes);
        if(errorSub != ERROR_SUB_OK)
        {
            SystemPrintf("%s,MCU_FlashEraseRangeWithCheck Failed,ErrorSub: 0X%08X \r\n",__func__,errorSub);
            //释放内存
            UserMemFree(MEM_SDRAM1,iapFileBinArray);
            //清除IAP标记,设置BOOT模式进行下次升级
            errorSub = BoardIapWriteServiceRecordDefault();
            if(errorSub != ERROR_SUB_OK)
            {
                BoardIapClearFileInfo(&boardIapFileInfo);
                //设置错误
                CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_DMC_FLASH_IAP_CRC,
                                            ERROR_LEVEL_ERROR,errorSubLocal);
                SystemPrintf("%s ,%d,BoardIapWriteServiceRecordDefault ErrorSub: 0X%08X\r\n",__func__,__LINE__,errorSubLocal);
                return;
            }
            //设置错误
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_DMC_FLASH_IAP_CRC,
                                        ERROR_LEVEL_ERROR,errorSub);
            return;
        }
        uint8_t retryCount = 0;
        do
        {
            retryCount++;
            //直接开始升级SRV
            errorSub = MCU_FlashWrite(MCU_FLASH_SRV_MAIN,(uint8_t*)iapFileBinArray,boardIapFileInfo.totalFileBytes);
            if(errorSub != ERROR_SUB_OK)
            {
                continue;
            }
            //匹配校验
            flashCmpResult = MCU_FlashCompare(MCU_FLASH_SRV_MAIN,iapFileBinArray,boardIapFileInfo.totalFileBytes);
            if(flashCmpResult != MCU_FLASH_CMP_EQU)
            {
                continue;
            }
            //执行到这里,无错误,跳出循环
            break;
        }while(retryCount < RETRY_COUNT_MAX_IAP_WRITE_FLASH);
        //直接开始升级SRV
        if(errorSub != ERROR_SUB_OK)
        {
            //写入flash失败
            SystemPrintf("%s ,%d,MCU_FlashWrite ErrorSub: 0X%08X\r\n",__func__,__LINE__,errorSub);
            //释放内存
            UserMemFree(MEM_SDRAM1,iapFileBinArray);
            //清空标记
            errorSubLocal = BoardIapWriteServiceRecordDefault();
            if(errorSubLocal != ERROR_SUB_OK)
            {
                BoardIapClearFileInfo(&boardIapFileInfo);
                //设置错误
                CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_DMC_FLASH_IAP_CRC,
                                            ERROR_LEVEL_ERROR,errorSubLocal);
                SystemPrintf("%s ,%d,BoardIapWriteServiceRecordDefault ErrorSub: 0X%08X\r\n",__func__,__LINE__,errorSubLocal);
                return;
            }
            //设置错误
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_DMC_FLASH_IAP_CRC,
                                        ERROR_LEVEL_ERROR,errorSub);
            return;
        }
        //匹配校验
        if(flashCmpResult != MCU_FLASH_CMP_EQU)
        {
            //写入flash失败
            SystemPrintf("%s ,%d,MCU_FlashCompare NotMatch,Code: %d\r\n",__func__,__LINE__,flashCmpResult);
            //释放内存
            UserMemFree(MEM_SDRAM1,iapFileBinArray);
            //清空标记
            errorSubLocal = BoardIapWriteServiceRecordDefault();
            if(errorSubLocal != ERROR_SUB_OK)
            {
                BoardIapClearFileInfo(&boardIapFileInfo);
                //设置错误
                CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_DMC_FLASH_IAP_CRC,
                                            ERROR_LEVEL_ERROR,errorSubLocal);
                SystemPrintf("%s ,%d,BoardIapWriteServiceRecordDefault ErrorSub: 0X%08X\r\n",__func__,__LINE__,errorSubLocal);
                return;
            }
            //设置错误
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_DMC_FLASH_IAP_CRC,
                                        ERROR_LEVEL_ERROR,errorSub);
            SystemPrintf("%s ,%d,BoardIapAppFileNotExistRecord ErrorSub: 0X%08X\r\n",__func__,__LINE__,errorSub);
            return;
        }
        //校验正确,清除记录,释放内存
        UserMemFree(MEM_SDRAM1,iapFileBinArray);
        BoardIapClearFileInfo(&boardIapFileInfo);
        //清空标记
        errorSubLocal = BoardIapWriteServiceRecordDefault();
        if(errorSubLocal != ERROR_SUB_OK)
        {
            //设置错误
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_DMC_FLASH_IAP_CRC,
                                        ERROR_LEVEL_ERROR,errorSubLocal);
            SystemPrintf("%s ,%d,BoardIapWriteServiceRecordDefault ErrorSub: 0X%08X\r\n",__func__,__LINE__,errorSubLocal);
            return;
        }
        //正常完成
        CoreUtilSetSystemResultPackNoError(resultPackPtr);
        //回传成功
        NetMasterSendResultByRecv(cmdPackPtr,resultPackPtr,NULL,0);
        //延迟跳转
        CoreDelayMs(200);
        //跳转到APP
        errorSub = BoardIapJumpApp();
        //跳转失败
        SystemPrintf("%s ,%d,BoardIapJumpApp ErrorSub: 0X%08X\r\n",__func__,__LINE__,errorSub);
        return;
    }
}

//在线升级强制跳转
static void SrvMsgNet8700FlashIapCmd_ForceJmp(NET_PROT_CMD_PACK_RECV* cmdPackPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    BOARD_FORCE_JMP jmpTarget;
    if(0 == cmdPackPtr->dataBodyBufferPtr[0])
    {
        jmpTarget = BOARD_FORCE_JMP_BOOT;
    }
    else
    {
        jmpTarget = BOARD_FORCE_JMP_MAIN;
    }
    //日志打印
    SystemPrintf("FlashIap,ForceJump: %d\r\n",(uint8_t)jmpTarget);
    if(jmpTarget == BOARD_FORCE_JMP_BOOT)
    {
        //跳转成功
        CoreUtilSetSystemResultPackNoError(resultPackPtr);
        return;
    }
    else
    {
        if(ERROR == BoardIapCheckAppExist())
        {
            //不存在APP
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_DMC_FLASH_IAP_FORCE_JMP,ERROR_LEVEL_ERROR,
                                        ERROR_SUB_BOARD_IAP_BOOT_FORCE_JMP_APP_NOT_EXIST);
            SystemPrintf("%s ,%d,BoardIapCheckAppExist ErrorSub: 0X%08X\r\n",__func__,__LINE__,resultPackPtr->errorSub);
            return;
        }
        //正常完成
        CoreUtilSetSystemResultPackNoError(resultPackPtr);
        //回传成功
        NetMasterSendResultByRecv(cmdPackPtr,resultPackPtr,NULL,0);
        //延迟跳转
        CoreDelayMs(200);
        //跳转到APP
        errorSub = BoardIapJumpApp();
        //跳转失败
        SystemPrintf("%s ,%d,BoardIapJumpApp ErrorSub: 0X%08X\r\n",__func__,__LINE__,errorSub);
        return;
    }
}

/****************************************网口上传消息处理函数**************************************************/
//占位函数
static void SrvMsgNet8700FlashIapUpload_PlaceHolder(NET_PROT_UPLOAD_PACK_RECV* uploadPackPtr);

//NET上传消息处理函数列表
static const USER_NET_UPLOAD_PROC_UNIT netUploadProcFlashIapUnitArray[] = 
{
    //顺序是: 指令码 预期参数最小长度 上传消息处理函数
    {(uint32_t)NET_MASTER_REPORT_HOLD_PLACE     ,2 ,SrvMsgNet8700FlashIapUpload_PlaceHolder},
};

void SrvMsgNet8700FlashIapUpload(void* netReportDataPtr)
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
    for(indexUtil = 0; indexUtil < DIM_ARRAY_ELEMENT_COUNT(netUploadProcFlashIapUnitArray);indexUtil++)
    {
        if(netUploadPackRecvPtr->uploadID == (uint32_t)(netUploadProcFlashIapUnitArray[indexUtil].uploadId))
        {
            break;
        }
    }
    //判定序号
    if(indexUtil == DIM_ARRAY_ELEMENT_COUNT(netUploadProcFlashIapUnitArray))
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
    if(netUploadPackRecvPtr->paramBufferLength < netUploadProcFlashIapUnitArray[indexUtil].exceptParamLengthMin)
    {
        //小于最小指令预期长度
        SystemPrintf("E:> %s,%d,Recv ExceptMinParamLength:%d ,Real: %d,UploadID: 0X%08X\r\n",__func__,__LINE__,
                        netUploadProcFlashIapUnitArray[indexUtil].exceptParamLengthMin,netUploadPackRecvPtr->paramBufferLength,
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
    if(netUploadProcFlashIapUnitArray[indexUtil].netUploadProcFuncPtr == NULL)
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
    netUploadProcFlashIapUnitArray[indexUtil].netUploadProcFuncPtr(netUploadPackRecvPtr);
    //处理完成,释放内存
    if((netUploadPackRecvPtr->paramBufferPtr != NULL)&&(netUploadPackRecvPtr->paramBufferLength != 0))
    {
        UserMemFree(MEM_REGION_SYS_MSG_NET,netUploadPackRecvPtr->paramBufferPtr);
    }
    UserMemFree(MEM_REGION_SYS_MSG_NET,netUploadPackRecvPtr);
}

//占位函数
static void SrvMsgNet8700FlashIapUpload_PlaceHolder(NET_PROT_UPLOAD_PACK_RECV* uploadPackPtr)
{
    asm("nop");
}



