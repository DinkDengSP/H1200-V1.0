/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-25 14:43:58 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-25 15:58:45 +0800
************************************************************************************************/ 
#include "SrvImplFlashIap.h"
#include "SrvImplBaseFlashIap.h"
#include "SrvTaskHeader.h"

//升级文件头
BOARD_IAP_FILE_INFO boardIapFileInfo;

//SRV升级,回传一个升级准备OK标记
void SrvImplFlashIapSrvUpdata(BOARD_IAP_RECODER* iapRecordPtr)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    SYS_RESULT_PACK resultPack;
    resultPack.resultCode = PROT_RESULT_SUCCESS;
    resultPack.errorMain = ERROR_MAIN_OK;
    resultPack.errorLevel = ERROR_LEVEL_NONE;
    resultPack.errorSub = ERROR_SUB_OK;
    //显示记录信息
    SystemPrintf("%s,iapFileExistFlag: 0X%08X,boardIapSrc: %d,iapFileTotalBytes: %d,crc32Code: 0X%08X\r\n",__func__,
                    iapRecordPtr->iapFileExistFlag,iapRecordPtr->boardIapSrc,iapRecordPtr->iapFileTotalBytes,
                    iapRecordPtr->crc32Code);
    //清除IAP标记,
    errorSubCode = BoardIapWriteSrvRecordDefault();
    if(errorSubCode != ERROR_SUB_OK)
    {
        SystemPrintf("%s,BoardIapWriteSrvRecordDefault Failed,ErrorSub: 0X%08X \r\n",__func__,errorSubCode);
        return;
    }
    //清除成功,回传升级准备成功标记
    Can1ProtSendResult(SYSTEM_MODULE_NO_PC,SYSTEM_BOARD_ID_PC,CAN_MASTER_CH_ACT_IAP,
                        CAN_MASTER_CMD_IAP_PREPARE,resultPack.resultCode,resultPack.errorMain,
                        resultPack.errorLevel,resultPack.errorSub, NULL,0,
                        &resultPack);
    if(resultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        SystemPrintf("E:> %s,%d,Can1ProtSendResult Failed,ErrorSub: 0X%08X\r\n",__func__,__LINE__,
                        resultPack.errorSub);
        return;
    }
    //设置升级标志为Srv升级
    boardIapFileInfo.boardIapMode = BOARD_IAP_MODE_SRV;
    return;
}

//App升级
extern void SrvImplFlashIapAppUpdata(BOARD_IAP_RECODER* iapRecordPtr)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    SYS_RESULT_PACK resultPack;
    resultPack.resultCode = PROT_RESULT_SUCCESS;
    resultPack.errorMain = ERROR_MAIN_OK;
    resultPack.errorLevel = ERROR_LEVEL_NONE;
    resultPack.errorSub = ERROR_SUB_OK;
    //显示记录信息
    SystemPrintf("%s,iapFileExistFlag: 0X%08X,boardIapSrc: %d,iapFileTotalBytes: %d,crc32Code: 0X%08X\r\n",__func__,
                    iapRecordPtr->iapFileExistFlag,iapRecordPtr->boardIapSrc,iapRecordPtr->iapFileTotalBytes,
                    iapRecordPtr->crc32Code);
    //清除IAP标记,
    errorSubCode = BoardIapWriteAppRecordDefault();
    if(errorSubCode != ERROR_SUB_OK)
    {
        SystemPrintf("%s,BoardIapWriteAppRecordDefault Failed,ErrorSub: 0X%08X \r\n",__func__,errorSubCode);
        return;
    }
    //清除成功,回传升级准备成功标记
    Can1ProtSendResult(SYSTEM_MODULE_NO_PC,SYSTEM_BOARD_ID_PC,CAN_MASTER_CH_ACT_IAP,
                        CAN_MASTER_CMD_IAP_PREPARE,resultPack.resultCode,resultPack.errorMain,
                        resultPack.errorLevel,resultPack.errorSub, NULL,0,
                        &resultPack);
    if(resultPack.resultCode != PROT_RESULT_SUCCESS)
    {
        SystemPrintf("E:> %s,%d,Can1ProtSendResult Failed,ErrorSub: 0X%08X\r\n",__func__,__LINE__,
                        resultPack.errorSub);
        return;
    }
    //设置升级标志为App升级
    boardIapFileInfo.boardIapMode = BOARD_IAP_MODE_APP;
    return;
}


//在线升级启动信息
void SrvImplBaseFlashIapStartMsg(BOARD_IAP_FILE_INFO* iapInfoPtr,SYS_RESULT_PACK* resultPackPtr,uint8_t** dataBufPtrPtr,uint16_t* dataBufLen)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    //检查有没有收到升级准备
    if((boardIapFileInfo.boardIapMode != BOARD_IAP_MODE_SRV)&&(boardIapFileInfo.boardIapMode != BOARD_IAP_MODE_APP))
    {
        //错误日志
        SystemPrintf("%s,%d,Not Recv Head Prepare\r\n",__func__,__LINE__);
        //没收到升级准备
        BoardIapClearFileInfo(&boardIapFileInfo);
        //设置错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_FLASH_IAP_START,
                                ERROR_LEVEL_ERROR,ERROR_SUB_BOARD_IAP_BOOT_NOT_PREPARE);
        return;
    }
    //在线升级长度不能为0
    if(boardIapFileInfo.totalFileBytes == 0)
    {
        //错误日志
        SystemPrintf("%s,%d,boardIapFileInfo.totalFileBytes Zero\r\n",__func__,__LINE__);
        BoardIapClearFileInfo(&boardIapFileInfo);
        //设置错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_FLASH_IAP_START,
                                ERROR_LEVEL_ERROR,ERROR_SUB_BOARD_IAP_BOOT_FILE_BYTES_ZERO);
        return;
    }
    //在线升级包数不能为0
    if(boardIapFileInfo.totalPackCount == 0)
    {
        //错误日志
        SystemPrintf("%s,%d,boardIapFileInfo.totalPackCount Zero\r\n",__func__,__LINE__);
        BoardIapClearFileInfo(&boardIapFileInfo);
        //设置错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_FLASH_IAP_START,
                                ERROR_LEVEL_ERROR,ERROR_SUB_BOARD_IAP_BOOT_FILE_PACK_COUNT_ZERO);
        return;
    }
    //在线升级单包数据长度不能为0
    if(boardIapFileInfo.singlePackBytes == 0)
    {
        //错误日志
        SystemPrintf("%s,%d,boardIapFileInfo.singlePackBytes Zero\r\n",__func__,__LINE__);
        BoardIapClearFileInfo(&boardIapFileInfo);
        //设置错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_FLASH_IAP_START,
                                ERROR_LEVEL_ERROR,ERROR_SUB_BOARD_IAP_BOOT_FILE_SINGLE_PACK_BYTES_ZERO);
        return;
    }
    //在线升级单包数据长度必须32字节对齐
    if(boardIapFileInfo.singlePackBytes%32 != 0)
    {
        //错误日志
        SystemPrintf("%s,%d,boardIapFileInfo.singlePackBytes Not Align 32\r\n",__func__,__LINE__);
        BoardIapClearFileInfo(&boardIapFileInfo);
        //设置错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_FLASH_IAP_START,
                                ERROR_LEVEL_ERROR,ERROR_SUB_BOARD_IAP_BOOT_PACK_SINGLE_LENGTH_ALIGH_32BYTE);
        return;
    }
    //单包数据长度最大值
    if(boardIapFileInfo.singlePackBytes >= CAN1_PROT_SINGLE_PACK_DATA_MAX)
    {
        //错误日志
        SystemPrintf("%s,%d,boardIapFileInfo.singlePackBytes More Then 4096\r\n",__func__,__LINE__);
        BoardIapClearFileInfo(&boardIapFileInfo);
        //设置错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_FLASH_IAP_START,
                                ERROR_LEVEL_ERROR,ERROR_SUB_BOARD_IAP_BOOT_FILE_SINGLE_PACK_BYTES_LIMIT);
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
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_FLASH_IAP_START,
                                ERROR_LEVEL_ERROR,ERROR_SUB_BOARD_IAP_BOOT_FILE_PACK_COUNT_NOT_MATCH);
        return;
    }
    //检查字节长度是否超限制
    if(boardIapFileInfo.boardIapMode == BOARD_IAP_MODE_SRV)
    {
        //检查字节长度是否超限制
        if(boardIapFileInfo.totalFileBytes > LENGTH_BOARD_IAP_SRV_BIN_FILE)
        {
            SystemPrintf("%s,%d,boardIapFileInfo.totalFileBytes More Then %d\r\n",__func__,__LINE__,
                            LENGTH_BOARD_IAP_SRV_BIN_FILE);
            //超过可升级程序长度,初始化升级头文件
            BoardIapClearFileInfo(&boardIapFileInfo);
            //设置错误
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_FLASH_IAP_START,
                                    ERROR_LEVEL_ERROR,ERROR_SUB_BOARD_IAP_BOOT_FILE_SRV_BYTES_LIMIT);
            return;
        }
        //清除SRV升级文件信息
        errorSub = BoardIapWriteSrvRecordDefault();
        if(errorSub != ERROR_SUB_OK)
        {
            SystemPrintf("%s,%d,BoardIapWriteSrvRecordDefault Failed,ErrorSub: 0X%08X %d\r\n",__func__,__LINE__,
                            errorSub);
            BoardIapClearFileInfo(&boardIapFileInfo);
            //设置错误
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_FLASH_IAP_START,
                                    ERROR_LEVEL_ERROR,errorSub);
            return;
        }
        //擦除升级区域Flash
        uint32_t eraseWord = boardIapFileInfo.totalFileBytes/4;
        if((boardIapFileInfo.totalFileBytes%4) != 0)
        {
            eraseWord += 1;
        }
        errorSub = MCU_FlashEraseRangeWithCheck(MCU_FLASH_APP,eraseWord);
        if(errorSub != ERROR_SUB_OK)
        {
            SystemPrintf("%s,%d,MCU_FlashEraseRangeWithCheck Failed,ErrorSub: 0X%08X %d\r\n",__func__,__LINE__,
                            errorSub);
            BoardIapClearFileInfo(&boardIapFileInfo);
            //设置错误
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_FLASH_IAP_START,
                                    ERROR_LEVEL_ERROR,errorSub);
            return;
        }
    }
    else
    {
        //检查字节长度是否超限制
        if(boardIapFileInfo.totalFileBytes > LENGTH_BOARD_IAP_APP_BIN_FILE)
        {
            SystemPrintf("%s,%d,boardIapFileInfo.totalFileBytes More Then %d\r\n",__func__,__LINE__,
                            LENGTH_BOARD_IAP_APP_BIN_FILE);
            //超过可升级程序长度,初始化升级头文件
            BoardIapClearFileInfo(&boardIapFileInfo);
            //设置错误
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_FLASH_IAP_START,
                                    ERROR_LEVEL_ERROR,ERROR_SUB_BOARD_IAP_BOOT_FILE_APP_BYTES_LIMIT);
            return;
        }
        //清除APP升级文件信息
        errorSub = BoardIapWriteAppRecordDefault();
        if(errorSub != ERROR_SUB_OK)
        {
            SystemPrintf("%s,%d,BoardIapWriteAppRecordDefault Failed,ErrorSub: 0X%08X %d\r\n",__func__,__LINE__,
                            errorSub);
            BoardIapClearFileInfo(&boardIapFileInfo);
            //设置错误
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_FLASH_IAP_START,
                                    ERROR_LEVEL_ERROR,errorSub);
            return;
        }
        //擦除升级区域Flash
        uint32_t eraseWord = boardIapFileInfo.totalFileBytes/4;
        if((boardIapFileInfo.totalFileBytes%4) != 0)
        {
            eraseWord += 1;
        }
        errorSub = MCU_FlashEraseRangeWithCheck(MCU_FLASH_APP,eraseWord);
        if(errorSub != ERROR_SUB_OK)
        {
            SystemPrintf("%s,%d,MCU_FlashEraseRangeWithCheck Failed,ErrorSub: 0X%08X %d\r\n",__func__,__LINE__,
                            errorSub);
            BoardIapClearFileInfo(&boardIapFileInfo);
            //设置错误
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_FLASH_IAP_START,
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
    if((boardIapFileInfo.boardIapMode != BOARD_IAP_MODE_SRV)&&(boardIapFileInfo.boardIapMode != BOARD_IAP_MODE_APP))
    {
        //错误日志
        SystemPrintf("%s,%d,Not Recv Head Prepare\r\n",__func__,__LINE__);
        //没收到升级准备
        BoardIapClearFileInfo(&boardIapFileInfo);
        //设置错误
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_FLASH_IAP_DATA,
                                ERROR_LEVEL_ERROR,ERROR_SUB_BOARD_IAP_BOOT_NOT_PREPARE);
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
        CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_FLASH_IAP_DATA,
                                    ERROR_LEVEL_ERROR,ERROR_SUB_BOARD_IAP_BOOT_PACK_INDEX_NOT_MATCH);
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
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_FLASH_IAP_DATA,
                                        ERROR_LEVEL_ERROR,ERROR_SUB_BOARD_IAP_BOOT_PACK_SINGLE_LENGTH_NOT_MATCH);
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
                CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_FLASH_IAP_DATA,
                                            ERROR_LEVEL_ERROR,ERROR_SUB_BOARD_IAP_BOOT_DATA_BYTES_NOT_MATCH_HEADER);
                return;
            }
        }
    }
    uint8_t* writeBufferPtrAlign4Byte = NULL;
    uint16_t packDataLengthAlign4Byte = 0;
    if(packDataLength%4 != 0)
    {
        //需要修正到四字节对齐
        packDataLengthAlign4Byte = packDataLength/4;
        packDataLengthAlign4Byte += 1;
        packDataLengthAlign4Byte *= 4;
        writeBufferPtrAlign4Byte = UserMemMallocWhileSuccess(MEM_SRAM_IN,packDataLengthAlign4Byte);
        //全部0XFF
        UserMemSet(writeBufferPtrAlign4Byte,0XFF,packDataLengthAlign4Byte);
        //数据拷贝
        UserMemCopy(writeBufferPtrAlign4Byte,writeBufferPtr,packDataLength);
    }
    else
    {
        //无需修正
        writeBufferPtrAlign4Byte = writeBufferPtr;
        packDataLengthAlign4Byte = packDataLength;
    }
    if(boardIapFileInfo.boardIapMode == BOARD_IAP_MODE_SRV)
    {
        //写入对应的Flash,APP
        errorSub = MCU_FlashWrite(MCU_FLASH_APP+boardIapFileInfo.recordRecvFileByteCount,(uint32_t*)writeBufferPtrAlign4Byte,
                                    packDataLengthAlign4Byte/4);
        //不管对错,先释放内存
        if(writeBufferPtrAlign4Byte != writeBufferPtr)
        {
            UserMemFree(MEM_SRAM_IN,writeBufferPtrAlign4Byte);
        }
        if(errorSub != ERROR_SUB_OK)
        {
            //错误日志
            SystemPrintf("%s,%d,MCU_FlashWrite,ErrorSub: 0X%08X\r\n",__func__,__LINE__,errorSub);
            BoardIapClearFileInfo(&boardIapFileInfo);
            //设置错误
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_FLASH_IAP_DATA,
                                        ERROR_LEVEL_ERROR,errorSub);
            return;
        }
    }
    else
    {
        //写入对应的Flash,APP
        errorSub = MCU_FlashWrite(MCU_FLASH_APP+boardIapFileInfo.recordRecvFileByteCount,(uint32_t*)writeBufferPtrAlign4Byte,
                                    packDataLengthAlign4Byte/4);
        //不管对错,先释放内存
        if(writeBufferPtrAlign4Byte != writeBufferPtr)
        {
            UserMemFree(MEM_SRAM_IN,writeBufferPtrAlign4Byte);
        }
        if(errorSub != ERROR_SUB_OK)
        {
            //错误日志
            SystemPrintf("%s,%d,MCU_FlashWrite,ErrorSub: 0X%08X\r\n",__func__,__LINE__,errorSub);
            BoardIapClearFileInfo(&boardIapFileInfo);
            //设置错误
            CoreUtilSetSystemResultPack(resultPackPtr,PROT_RESULT_FAIL,ERROR_MAIN_ACT_FLASH_IAP_DATA,
                                        ERROR_LEVEL_ERROR,errorSub);
            return;
        }
    }
    //写入完成增加包计数和字节计数
    boardIapFileInfo.recordRecvFilePackCount += 1;
    boardIapFileInfo.recordRecvFileByteCount += packDataLength;
    //正常完成
    CoreUtilSetSystemResultPackNoError(resultPackPtr);
    return;
}





