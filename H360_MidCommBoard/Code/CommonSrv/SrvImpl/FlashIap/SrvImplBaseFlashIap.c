/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-31 16:34:18
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-02-01 11:27:17
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
#include "SrvImplBaseFlashIap.h"


//升级文件头
BOARD_IAP_FILE_INFO boardIapFileInfo;

//BOOT升级
void SrvImplBaseFlashIapBootUpdata(BOARD_IAP_RECODER* iapRecordPtr)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    uint32_t crcCalc = 0;
    //显示记录信息
    SystemPrintf("%s,iapFileExistFlag: 0X%08X,boardIapSrc: %d,iapFileTotalBytes: %d,crc32Code: 0X%08X\r\n",__func__,iapRecordPtr->iapFileExistFlag,
                    iapRecordPtr->boardIapSrc,iapRecordPtr->iapFileTotalBytes,iapRecordPtr->crc32Code);
    //申请内存
    uint8_t* srvBinArrayPtr = UserMemMallocWhileSuccess(MEM_SDRAM1,iapRecordPtr->iapFileTotalBytes);
    //读取BOOT升级文件
    errorSubCode = BoardIapReadBootBinFile(0,srvBinArrayPtr,iapRecordPtr->iapFileTotalBytes);
    if(errorSubCode != ERROR_SUB_OK)
    {
        SystemPrintf("%s,BoardIapReadBootBinFile Failed,ErrorSub: 0X%08X \r\n",__func__,errorSubCode);
        //清除IAP标记,设置APP模式进行下次升级
        errorSubCode = BoardIapWriteBootRecordDefault();
        if(errorSubCode != ERROR_SUB_OK)
        {
            SystemPrintf("%s,BoardIapWriteBootRecordDefault Failed,ErrorSub: 0X%08X \r\n",__func__,errorSubCode);
        }
        //设置APP模式,下次继续升级
        errorSubCode = BoardIapSetRunningModeApp();
        if(errorSubCode != ERROR_SUB_OK)
        {
            SystemPrintf("%s,BoardIapSetRunningModeApp Failed,ErrorSub: 0X%08X \r\n",__func__,errorSubCode);
        }
        //释放内存
        UserMemFree(MEM_SDRAM1,srvBinArrayPtr);
        return;
    }
    //计算CRC,看看是否匹配
    crcCalc = BoardIapCalcCRC32(srvBinArrayPtr,iapRecordPtr->iapFileTotalBytes);
    //查看CRC是否匹配
    if(crcCalc != iapRecordPtr->crc32Code)
    {
        SystemPrintf("%s,BoardIapCalcCRC32 NotMatch,Calc: 0X%08X,Src: 0X%08X \r\n",__func__,crcCalc,iapRecordPtr->crc32Code);
        //清除IAP标记,设置APP模式进行下次升级
        errorSubCode = BoardIapWriteBootRecordDefault();
        if(errorSubCode != ERROR_SUB_OK)
        {
            SystemPrintf("%s,BoardIapWriteBootRecordDefault Failed,ErrorSub: 0X%08X \r\n",__func__,errorSubCode);
        }
        //设置APP模式,下次继续升级
        errorSubCode = BoardIapSetRunningModeApp();
        if(errorSubCode != ERROR_SUB_OK)
        {
            SystemPrintf("%s,BoardIapSetRunningModeApp Failed,ErrorSub: 0X%08X \r\n",__func__,errorSubCode);
        }
        //释放内存
        UserMemFree(MEM_SDRAM1,srvBinArrayPtr);
        return;
    }
    //CRC匹配,擦除Flash
    errorSubCode = MCU_FlashEraseRangeWithCheck(MCU_FLASH_SRV_BOOT,iapRecordPtr->iapFileTotalBytes);
    if(errorSubCode != ERROR_SUB_OK)
    {
        SystemPrintf("%s,MCU_FlashEraseRangeWithCheck Failed,ErrorSub: 0X%08X \r\n",__func__,errorSubCode);
        //清除IAP标记,设置APP模式进行下次升级
        errorSubCode = BoardIapWriteBootRecordDefault();
        if(errorSubCode != ERROR_SUB_OK)
        {
            SystemPrintf("%s,BoardIapWriteBootRecordDefault Failed,ErrorSub: 0X%08X \r\n",__func__,errorSubCode);
        }
        //设置APP模式,下次继续升级
        errorSubCode = BoardIapSetRunningModeApp();
        if(errorSubCode != ERROR_SUB_OK)
        {
            SystemPrintf("%s,BoardIapSetRunningModeApp Failed,ErrorSub: 0X%08X \r\n",__func__,errorSubCode);
        }
        //释放内存
        UserMemFree(MEM_SDRAM1,srvBinArrayPtr);
        return;
    }
    uint8_t retryCount = 0;
    //比较写入是否正确
    MCU_FLASH_CMP_RESULT flashCmpResult = MCU_FLASH_CMP_EQU;
    do
    {
        retryCount++;
        //CRC匹配,开始写入FLASH
        errorSubCode = MCU_FlashWrite(MCU_FLASH_SRV_BOOT,srvBinArrayPtr,iapRecordPtr->iapFileTotalBytes);
        if(errorSubCode != ERROR_SUB_OK)
        {
            //结束循环,下一次
            continue;
        }
        //写入完成,延时等待稳定
        CoreDelayMs(100);
        //比较写入是否正确
        flashCmpResult = MCU_FlashCompare(MCU_FLASH_SRV_BOOT,srvBinArrayPtr,iapRecordPtr->iapFileTotalBytes);
        if(flashCmpResult != MCU_FLASH_CMP_EQU)
        {
            //FLASH不匹配
            continue;
        }
        //执行到这里,无错误,跳出循环
        break;
    }while(retryCount < RETRY_COUNT_MAX_IAP_WRITE_FLASH);
    //写入失败检测
    if(errorSubCode != ERROR_SUB_OK)
    {
        SystemPrintf("%s,MCU_FlashWrite Failed,ErrorSub: 0X%08X \r\n",__func__,errorSubCode);
        //清除IAP标记,设置APP模式进行下次升级
        errorSubCode = BoardIapWriteBootRecordDefault();
        if(errorSubCode != ERROR_SUB_OK)
        {
            SystemPrintf("%s,BoardIapWriteBootRecordDefault Failed,ErrorSub: 0X%08X \r\n",__func__,errorSubCode);
        }
        //设置APP模式,下次继续升级
        errorSubCode = BoardIapSetRunningModeApp();
        if(errorSubCode != ERROR_SUB_OK)
        {
            SystemPrintf("%s,BoardIapSetRunningModeApp Failed,ErrorSub: 0X%08X \r\n",__func__,errorSubCode);
        }
        //释放内存
        UserMemFree(MEM_SDRAM1,srvBinArrayPtr);
        return;
    }
    //Flash匹配检测
    if(flashCmpResult != MCU_FLASH_CMP_EQU)
    {
        //FLASH不匹配
        SystemPrintf("%s,MCU_FlashCompare Failed,CompareResult: %d \r\n",__func__,flashCmpResult);
        //清除IAP标记,设置APP模式进行下次升级
        errorSubCode = BoardIapWriteBootRecordDefault();
        if(errorSubCode != ERROR_SUB_OK)
        {
            SystemPrintf("%s,BoardIapWriteBootRecordDefault Failed,ErrorSub: 0X%08X \r\n",__func__,errorSubCode);
        }
        //设置APP模式,下次继续升级
        errorSubCode = BoardIapSetRunningModeApp();
        if(errorSubCode != ERROR_SUB_OK)
        {
            SystemPrintf("%s,BoardIapSetRunningModeApp Failed,ErrorSub: 0X%08X \r\n",__func__,errorSubCode);
        }
        //释放内存
        UserMemFree(MEM_SDRAM1,srvBinArrayPtr);
        return;
    }
    //匹配成功,写入完成,清除相应的标记
    errorSubCode = BoardIapWriteBootRecordDefault();
    if(errorSubCode != ERROR_SUB_OK)
    {
        SystemPrintf("%s,BoardIapWriteBootRecordDefault Failed,ErrorSub: 0X%08X \r\n",__func__,errorSubCode);
        //设置APP模式,下次继续升级
        errorSubCode = BoardIapSetRunningModeApp();
        if(errorSubCode != ERROR_SUB_OK)
        {
            SystemPrintf("%s,BoardIapSetRunningModeApp Failed,ErrorSub: 0X%08X \r\n",__func__,errorSubCode);
        }
        //释放内存
        UserMemFree(MEM_SDRAM1,srvBinArrayPtr);
        return;
    }
    //释放内存
    UserMemFree(MEM_SDRAM1,srvBinArrayPtr);
    return;
}


//在线升级准备
void SrvImplBaseFlashIapPrepare(uint8_t fileIapMode,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //初始化升级头文件
    BoardIapClearFileInfo(&boardIapFileInfo);
    //判定升级目标
    if(fileIapMode == BOARD_IAP_MODE_BOOT)
    {
        boardIapFileInfo.boardIapMode = BOARD_IAP_MODE_BOOT;
        //在线升级的文件是BOOT,
        CoreUtilSetSystemResultPackNoError(resultPackPtr);
        return;
    }
    if(fileIapMode == BOARD_IAP_MODE_SRV)
    {
        boardIapFileInfo.boardIapMode = BOARD_IAP_MODE_SRV;
        //升级的文件是Srv,返回完成
        CoreUtilSetSystemResultPackNoError(resultPackPtr);
        return;
    }
    else if(fileIapMode == BOARD_IAP_MODE_APP)
    {
        boardIapFileInfo.boardIapMode = BOARD_IAP_MODE_APP;
        //在线升级的文件是APP,
        CoreUtilSetSystemResultPackNoError(resultPackPtr);
        return;
    }
    else
    {
        //错误日志
        SystemPrintf("%s,%d,NotInvalid Mode: %d\r\n",__func__,__LINE__,fileIapMode);
        //在线升级文件类型不支持
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_DMC_FLASH_IAP_PREPARE,
                                ERROR_LEVEL_ERROR,ERROR_SUB_BOARD_IAP_APP_INVALID_TYPE);
        return;
    }
}

//在线升级启动信息
void SrvImplBaseFlashIapStartMsg(BOARD_IAP_FILE_INFO* iapInfoPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //检查有没有收到升级准备
    if((boardIapFileInfo.boardIapMode != BOARD_IAP_MODE_BOOT)&&(boardIapFileInfo.boardIapMode != BOARD_IAP_MODE_SRV)&&
        (boardIapFileInfo.boardIapMode != BOARD_IAP_MODE_APP))
    {
        //错误日志
        SystemPrintf("%s,%d,Not Recv Head Prepare\r\n",__func__,__LINE__);
        //没收到升级准备
        BoardIapClearFileInfo(&boardIapFileInfo);
        //设置错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_DMC_FLASH_IAP_START,
                                ERROR_LEVEL_ERROR,ERROR_SUB_BOARD_IAP_APP_NOT_PREPARE);
        return;
    }
    //在线升级长度不能为0
    if(boardIapFileInfo.totalFileBytes == 0)
    {
        //错误日志
        SystemPrintf("%s,%d,boardIapFileInfo.totalFileBytes Zero\r\n",__func__,__LINE__);
        BoardIapClearFileInfo(&boardIapFileInfo);
        //设置错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_DMC_FLASH_IAP_START,
                                ERROR_LEVEL_ERROR,ERROR_SUB_BOARD_IAP_APP_FILE_BYTES_ZERO);
        return;
    }
    //在线升级包数不能为0
    if(boardIapFileInfo.totalPackCount == 0)
    {
        //错误日志
        SystemPrintf("%s,%d,boardIapFileInfo.totalPackCount Zero\r\n",__func__,__LINE__);
        BoardIapClearFileInfo(&boardIapFileInfo);
        //设置错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_DMC_FLASH_IAP_START,
                                ERROR_LEVEL_ERROR,ERROR_SUB_BOARD_IAP_APP_FILE_PACK_COUNT_ZERO);
        return;
    }
    //在线升级单包数据长度不能为0
    if(boardIapFileInfo.singlePackBytes == 0)
    {
        //错误日志
        SystemPrintf("%s,%d,boardIapFileInfo.singlePackBytes Zero\r\n",__func__,__LINE__);
        BoardIapClearFileInfo(&boardIapFileInfo);
        //设置错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_DMC_FLASH_IAP_START,
                                ERROR_LEVEL_ERROR,ERROR_SUB_BOARD_IAP_APP_FILE_SINGLE_PACK_BYTES_ZERO);
        return;
    }
    //在线升级单包数据长度必须32字节对齐
    if(boardIapFileInfo.singlePackBytes%32 != 0)
    {
        //错误日志
        SystemPrintf("%s,%d,boardIapFileInfo.singlePackBytes Not Align 32\r\n",__func__,__LINE__);
        BoardIapClearFileInfo(&boardIapFileInfo);
        //设置错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_DMC_FLASH_IAP_START,
                                ERROR_LEVEL_ERROR,ERROR_SUB_BOARD_IAP_APP_PACK_SINGLE_LENGTH_ALIGH_32BYTE);
        return;
    }
    //单包数据长度最大值
    if(boardIapFileInfo.singlePackBytes >= 4096)
    {
        //错误日志
        SystemPrintf("%s,%d,boardIapFileInfo.singlePackBytes More Then 4096\r\n",__func__,__LINE__);
        BoardIapClearFileInfo(&boardIapFileInfo);
        //设置错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_DMC_FLASH_IAP_START,
                                ERROR_LEVEL_ERROR,ERROR_SUB_BOARD_IAP_APP_FILE_SINGLE_PACK_BYTES_LIMIT);
        return;
    }
    uint32_t packCountLocal = boardIapFileInfo.totalFileBytes/boardIapFileInfo.singlePackBytes;
    if((boardIapFileInfo.totalFileBytes % boardIapFileInfo.singlePackBytes) != 0)
    {
        packCountLocal += 1;
    }
    //包数量和包长度匹配
    if(packCountLocal != boardIapFileInfo.totalPackCount)
    {
        SystemPrintf("%s,%d,PackCount Not Match,Recv: %d,Calc: %d\r\n",__func__,__LINE__,
                        boardIapFileInfo.totalPackCount,packCountLocal);
        BoardIapClearFileInfo(&boardIapFileInfo);
        //设置错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_DMC_FLASH_IAP_START,
                                ERROR_LEVEL_ERROR,ERROR_SUB_BOARD_IAP_APP_FILE_PACK_COUNT_NOT_MATCH);
        return;
    }

    //检查字节长度是否超限制
    if(boardIapFileInfo.boardIapMode == BOARD_IAP_MODE_BOOT)
    {
        if(boardIapFileInfo.totalFileBytes > LENGTH_BOARD_IAP_BOOT_BIN_FILE)
        {
            SystemPrintf("%s,%d,boardIapFileInfo.totalFileBytes More Then %d\r\n",__func__,__LINE__,
                            LENGTH_BOARD_IAP_BOOT_BIN_FILE);
            //超过可升级程序长度,初始化升级头文件
            BoardIapClearFileInfo(&boardIapFileInfo);
            //设置错误
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_DMC_FLASH_IAP_START,
                                        ERROR_LEVEL_ERROR,ERROR_SUB_BOARD_IAP_APP_FILE_BOOT_BYTES_LIMIT);
            return;
        }
        //清除BOOT升级文件信息
        errorSub = BoardIapWriteBootRecordDefault();
        if(errorSub != ERROR_SUB_OK)
        {
            SystemPrintf("%s,%d,BoardIapWriteBootRecordDefault Failed,ErrorSub: 0X%08X %d\r\n",__func__,__LINE__,
                            errorSub);
            BoardIapClearFileInfo(&boardIapFileInfo);
            //设置错误
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_DMC_FLASH_IAP_START,
                                        ERROR_LEVEL_ERROR,errorSub);
            return;
        }
    }
    else
    {
        if(boardIapFileInfo.totalFileBytes > LENGTH_BOARD_IAP_SRV_BIN_FILE)
        {
            SystemPrintf("%s,%d,boardIapFileInfo.totalFileBytes More Then %d\r\n",__func__,__LINE__,
                            LENGTH_BOARD_IAP_SRV_BIN_FILE);
            //超过可升级程序长度,初始化升级头文件
            BoardIapClearFileInfo(&boardIapFileInfo);
            //设置错误
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_DMC_FLASH_IAP_START,
                                        ERROR_LEVEL_ERROR,ERROR_SUB_BOARD_IAP_APP_FILE_SRV_BYTES_LIMIT);
            return;
        }
        //清除APP升级文件信息
        errorSub = BoardIapWriteServiceRecordDefault();
        if(errorSub != ERROR_SUB_OK)
        {
            SystemPrintf("%s,%d,BoardIapWriteServiceRecordDefault Failed,ErrorSub: 0X%08X %d\r\n",__func__,__LINE__,
                            errorSub);
            BoardIapClearFileInfo(&boardIapFileInfo);
            //设置错误
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_DMC_FLASH_IAP_START,
                                        ERROR_LEVEL_ERROR,errorSub);
            return;
        }
    }
    //正常完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}

//在线升级数据传输
void SrvImplBaseFlashIapDataTrans(uint16_t currentPackIndex,uint16_t packDataLength,uint8_t* writeBufferPtr,SYS_RESULT_PACK* resultPackPtr,
                                    uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    //检查是否设置了模式
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //检查有没有收到升级准备
    if((boardIapFileInfo.boardIapMode != BOARD_IAP_MODE_BOOT)&&(boardIapFileInfo.boardIapMode != BOARD_IAP_MODE_SRV)&&
        (boardIapFileInfo.boardIapMode != BOARD_IAP_MODE_APP))
    {
        //错误日志
        SystemPrintf("%s,%d,Not Recv Head Prepare\r\n",__func__,__LINE__);
        //没收到升级准备
        BoardIapClearFileInfo(&boardIapFileInfo);
        //设置错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_DMC_FLASH_IAP_START,
                                    ERROR_LEVEL_ERROR,ERROR_SUB_BOARD_IAP_APP_NOT_PREPARE);
        return;
    }
    //如果当前数据包序号大于接收到的数据包序号,可能重发了,不需要处理直接完成
    if(boardIapFileInfo.recordRecvFilePackCount > currentPackIndex)
    {
        //错误日志
        SystemPrintf("%s,%d,PackIndexNotMatch,Recv : %d,Record: %d\r\n",__func__,__LINE__,currentPackIndex,
                        boardIapFileInfo.recordRecvFilePackCount);
        //正常完成
        CoreUtilSetSystemResultPackNoError(resultPackPtr);
        return;
    }
    //检查当前数据
    if(currentPackIndex != boardIapFileInfo.recordRecvFilePackCount)
    {
        //错误日志
        SystemPrintf("%s,%d,PackIndexNotMatch,Recv : %d,Record: %d\r\n",__func__,__LINE__,currentPackIndex,
                        boardIapFileInfo.recordRecvFilePackCount);
        //包序号不匹配
        BoardIapClearFileInfo(&boardIapFileInfo);
        //设置错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_DMC_FLASH_IAP_DATA,
                                    ERROR_LEVEL_ERROR,ERROR_SUB_BOARD_IAP_APP_PACK_INDEX_NOT_MATCH);
        return;
    }
    //包长度匹配
    if(packDataLength != boardIapFileInfo.singlePackBytes)
    {
        //包长度不匹配的情况下,检查是不是最后一包,如果是最后一包,可以不匹配
        if(currentPackIndex != boardIapFileInfo.totalPackCount -1)
        {
            //错误日志
            SystemPrintf("%s,%d,PackLengthNotMatch,Recv : %d,Record: %d\r\n",__func__,__LINE__,packDataLength,
                            boardIapFileInfo.singlePackBytes);
            BoardIapClearFileInfo(&boardIapFileInfo);
            //设置错误
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_DMC_FLASH_IAP_DATA,
                                        ERROR_LEVEL_ERROR,ERROR_SUB_BOARD_IAP_APP_PACK_SINGLE_LENGTH_NOT_MATCH);
            return;
        }
        else
        {
            //最后一包,接收到的数据字节长度加上已经接收到的长度是否等于最终长度
            if((boardIapFileInfo.recordRecvFileByteCount + packDataLength) != boardIapFileInfo.totalFileBytes)
            {
                //错误日志
                SystemPrintf("%s,%d,PackLengthNotMatch,Recv : %d,Record: %d\r\n",__func__,__LINE__,packDataLength,
                                boardIapFileInfo.singlePackBytes);
                BoardIapClearFileInfo(&boardIapFileInfo);
                //设置错误
                CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_DMC_FLASH_IAP_DATA,
                                            ERROR_LEVEL_ERROR,ERROR_SUB_BOARD_IAP_APP_DATA_BYTES_NOT_MATCH_HEADER);
                return;
            }
        }
    }
    //BOOT和APP在不同的位置写入
    if(boardIapFileInfo.boardIapMode == BOARD_IAP_MODE_BOOT)
    {
        errorSub = BoardIapWriteBootBinFile(boardIapFileInfo.recordRecvFileByteCount,(uint8_t*)writeBufferPtr,
                                    packDataLength);
    }
    else
    {
        errorSub = BoardIapWriteSrvBinFile(boardIapFileInfo.recordRecvFileByteCount,(uint8_t*)writeBufferPtr,
                                    packDataLength);
    }
    if(errorSub != ERROR_SUB_OK)
    {
        //错误日志
        SystemPrintf("%s,%d,WriteBinFileFailed,ErrorSub: 0X%08X\r\n",__func__,__LINE__,errorSub);
        BoardIapClearFileInfo(&boardIapFileInfo);
        //设置错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_DMC_FLASH_IAP_DATA,
                                    ERROR_LEVEL_ERROR,errorSub);
        return;
    }
    //写入完成增加包计数和字节计数
    boardIapFileInfo.recordRecvFilePackCount += 1;
    boardIapFileInfo.recordRecvFileByteCount += packDataLength;
    //正常完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}







