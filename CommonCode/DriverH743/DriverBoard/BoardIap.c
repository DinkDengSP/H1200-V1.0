/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-17 11:09:41 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-20 17:37:47 +0800
************************************************************************************************/ 
#include "BoardIap.h"
#include "DriverConfigBoard.h"
#include BOARD_IAP_FILE_INCLUDE

//CRC32辅助计算标记
static const uint32_t boardIapCrc32UtilArray[256] = 
{
    0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL,
    0x076dc419L, 0x706af48fL, 0xe963a535L, 0x9e6495a3L,
    0x0edb8832L, 0x79dcb8a4L, 0xe0d5e91eL, 0x97d2d988L,
    0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L, 0x90bf1d91L,
    0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
    0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L,
    0x136c9856L, 0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL,
    0x14015c4fL, 0x63066cd9L, 0xfa0f3d63L, 0x8d080df5L,
    0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L, 0xa2677172L,
    0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
    0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L,
    0x32d86ce3L, 0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L,
    0x26d930acL, 0x51de003aL, 0xc8d75180L, 0xbfd06116L,
    0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L, 0xb8bda50fL,
    0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
    0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL,
    0x76dc4190L, 0x01db7106L, 0x98d220bcL, 0xefd5102aL,
    0x71b18589L, 0x06b6b51fL, 0x9fbfe4a5L, 0xe8b8d433L,
    0x7807c9a2L, 0x0f00f934L, 0x9609a88eL, 0xe10e9818L,
    0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
    0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL,
    0x6c0695edL, 0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L,
    0x65b0d9c6L, 0x12b7e950L, 0x8bbeb8eaL, 0xfcb9887cL,
    0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L, 0xfbd44c65L,
    0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
    0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL,
    0x4369e96aL, 0x346ed9fcL, 0xad678846L, 0xda60b8d0L,
    0x44042d73L, 0x33031de5L, 0xaa0a4c5fL, 0xdd0d7cc9L,
    0x5005713cL, 0x270241aaL, 0xbe0b1010L, 0xc90c2086L,
    0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
    0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L,
    0x59b33d17L, 0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL,
    0xedb88320L, 0x9abfb3b6L, 0x03b6e20cL, 0x74b1d29aL,
    0xead54739L, 0x9dd277afL, 0x04db2615L, 0x73dc1683L,
    0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
    0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L,
    0xf00f9344L, 0x8708a3d2L, 0x1e01f268L, 0x6906c2feL,
    0xf762575dL, 0x806567cbL, 0x196c3671L, 0x6e6b06e7L,
    0xfed41b76L, 0x89d32be0L, 0x10da7a5aL, 0x67dd4accL,
    0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
    0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L,
    0xd1bb67f1L, 0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL,
    0xd80d2bdaL, 0xaf0a1b4cL, 0x36034af6L, 0x41047a60L,
    0xdf60efc3L, 0xa867df55L, 0x316e8eefL, 0x4669be79L,
    0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
    0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL,
    0xc5ba3bbeL, 0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L,
    0xc2d7ffa7L, 0xb5d0cf31L, 0x2cd99e8bL, 0x5bdeae1dL,
    0x9b64c2b0L, 0xec63f226L, 0x756aa39cL, 0x026d930aL,
    0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
    0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L,
    0x92d28e9bL, 0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L,
    0x86d3d2d4L, 0xf1d4e242L, 0x68ddb3f8L, 0x1fda836eL,
    0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L, 0x18b74777L,
    0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
    0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L,
    0xa00ae278L, 0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L,
    0xa7672661L, 0xd06016f7L, 0x4969474dL, 0x3e6e77dbL,
    0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L, 0x37d83bf0L,
    0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
    0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L,
    0xbad03605L, 0xcdd70693L, 0x54de5729L, 0x23d967bfL,
    0xb3667a2eL, 0xc4614ab8L, 0x5d681b02L, 0x2a6f2b94L,
    0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL, 0x2d02ef8dL
};

//跳转函数原型
typedef void (*BoardIapJmpAddressFuncPtr)(void);

//计算CRC32
uint32_t BoardIapCalcCRC32(const uint8_t *pu8Data, uint32_t u32Size)
{
    uint32_t indexUtil, crc;
    crc = 0xFFFFFFFF;
    for (indexUtil = 0; indexUtil < u32Size; indexUtil++)
    {
        crc = boardIapCrc32UtilArray[(crc ^ pu8Data[indexUtil]) & 0xff] ^ (crc >> 8);
    }
    return crc^0xFFFFFFFF;
}

//IAP头清空
void BoardIapClearFileInfo(BOARD_IAP_FILE_INFO* iapFileInfoPtr)
{
    iapFileInfoPtr->boardIapMode = BOARD_IAP_MODE_NONE;
    iapFileInfoPtr->totalPackCount = 0;
    iapFileInfoPtr->totalFileBytes = 0;
    iapFileInfoPtr->singlePackBytes = 0;
    iapFileInfoPtr->recordRecvFilePackCount = 0;
    iapFileInfoPtr->recordRecvFileByteCount = 0;
    iapFileInfoPtr->crc32CodeRecv = 0;
}

//读取IAP BOOT文件
ERROR_SUB BoardIapReadBootBinFile(uint32_t addrOffset,uint8_t* bufferPtr,uint32_t byteLength)
{
    uint32_t readByteCount = 0;
    ERROR_SUB errorSub = ERROR_SUB_OK;
    for(readByteCount = 0; readByteCount < byteLength; )
    {
        if(byteLength - readByteCount >= UINT16_MAX)
        {
            errorSub = BOARD_IAP_FILE_READ(ADDR_BOARD_IAP_BOOT_BIN_FILE_START+addrOffset + readByteCount,bufferPtr + readByteCount,UINT16_MAX);
            if(errorSub != ERROR_SUB_OK)
            {
                return errorSub;
            }
            readByteCount += UINT16_MAX;
        }
        else
        {
            errorSub = BOARD_IAP_FILE_READ(ADDR_BOARD_IAP_BOOT_BIN_FILE_START+addrOffset + readByteCount,bufferPtr + readByteCount,byteLength - readByteCount);
            if(errorSub != ERROR_SUB_OK)
            {
                return errorSub;
            }
            readByteCount = byteLength;
        }
    }
    return ERROR_SUB_OK;
}

//写入IAP BOOT文件
ERROR_SUB BoardIapWriteBootBinFile(uint32_t addrOffset,uint8_t* bufferPtr,uint32_t byteLength)
{
    uint32_t writeByteCount = 0;
    ERROR_SUB errorSub = ERROR_SUB_OK;
    for(writeByteCount = 0; writeByteCount < byteLength; )
    {
        if(byteLength - writeByteCount >= UINT16_MAX)
        {
            errorSub = BOARD_IAP_FILE_WRITE(ADDR_BOARD_IAP_BOOT_BIN_FILE_START+addrOffset + writeByteCount,bufferPtr + writeByteCount,UINT16_MAX);
            if(errorSub != ERROR_SUB_OK)
            {
                return errorSub;
            }
            writeByteCount += UINT16_MAX;
        }
        else
        {
            errorSub = BOARD_IAP_FILE_WRITE(ADDR_BOARD_IAP_BOOT_BIN_FILE_START+addrOffset + writeByteCount,bufferPtr + writeByteCount,byteLength - writeByteCount);
            if(errorSub != ERROR_SUB_OK)
            {
                return errorSub;
            }
            writeByteCount = byteLength;
        }
    }
    return ERROR_SUB_OK;
}

//读取IAP Service文件
ERROR_SUB BoardIapReadSrvBinFile(uint32_t addrOffset,uint8_t* bufferPtr,uint32_t byteLength)
{
    uint32_t readByteCount = 0;
    ERROR_SUB errorSub = ERROR_SUB_OK;
    for(readByteCount = 0; readByteCount < byteLength; )
    {
        if(byteLength - readByteCount >= UINT16_MAX)
        {
            errorSub = BOARD_IAP_FILE_READ(ADDR_BOARD_IAP_SRV_BIN_FILE_START+addrOffset + readByteCount,bufferPtr + readByteCount,UINT16_MAX);
            if(errorSub != ERROR_SUB_OK)
            {
                return errorSub;
            }
            readByteCount += UINT16_MAX;
        }
        else
        {
            errorSub = BOARD_IAP_FILE_READ(ADDR_BOARD_IAP_SRV_BIN_FILE_START+addrOffset + readByteCount,bufferPtr + readByteCount,byteLength - readByteCount);
            if(errorSub != ERROR_SUB_OK)
            {
                return errorSub;
            }
            readByteCount = byteLength;
        }
    }
    return ERROR_SUB_OK;
}

//写入IAP Service文件
ERROR_SUB BoardIapWriteSrvBinFile(uint32_t addrOffset,uint8_t* bufferPtr,uint32_t byteLength)
{
    uint32_t writeByteCount = 0;
    ERROR_SUB errorSub = ERROR_SUB_OK;
    for(writeByteCount = 0; writeByteCount < byteLength; )
    {
        if(byteLength - writeByteCount >= UINT16_MAX)
        {
            errorSub = BOARD_IAP_FILE_WRITE(ADDR_BOARD_IAP_SRV_BIN_FILE_START+addrOffset + writeByteCount,bufferPtr + writeByteCount,UINT16_MAX);
            if(errorSub != ERROR_SUB_OK)
            {
                return errorSub;
            }
            writeByteCount += UINT16_MAX;
        }
        else
        {
            errorSub = BOARD_IAP_FILE_WRITE(ADDR_BOARD_IAP_SRV_BIN_FILE_START+addrOffset + writeByteCount,bufferPtr + writeByteCount,byteLength - writeByteCount);
            if(errorSub != ERROR_SUB_OK)
            {
                return errorSub;
            }
            writeByteCount = byteLength;
        }
    }
    return ERROR_SUB_OK;
}

//读取IAP App文件
ERROR_SUB BoardIapReadAppBinFile(uint32_t addrOffset,uint8_t* bufferPtr,uint32_t byteLength)
{
    uint32_t readByteCount = 0;
    ERROR_SUB errorSub = ERROR_SUB_OK;
    for(readByteCount = 0; readByteCount < byteLength; )
    {
        if(byteLength - readByteCount >= UINT16_MAX)
        {
            errorSub = BOARD_IAP_FILE_READ(ADDR_BOARD_IAP_APP_BIN_FILE_START+addrOffset + readByteCount,bufferPtr + readByteCount,UINT16_MAX);
            if(errorSub != ERROR_SUB_OK)
            {
                return errorSub;
            }
            readByteCount += UINT16_MAX;
        }
        else
        {
            errorSub = BOARD_IAP_FILE_READ(ADDR_BOARD_IAP_APP_BIN_FILE_START+addrOffset + readByteCount,bufferPtr + readByteCount,byteLength - readByteCount);
            if(errorSub != ERROR_SUB_OK)
            {
                return errorSub;
            }
            readByteCount = byteLength;
        }
    }
    return ERROR_SUB_OK;
}

//写入IAP App文件
ERROR_SUB BoardIapWriteAppBinFile(uint32_t addrOffset,uint8_t* bufferPtr,uint32_t byteLength)
{
    uint32_t writeByteCount = 0;
    ERROR_SUB errorSub = ERROR_SUB_OK;
    for(writeByteCount = 0; writeByteCount < byteLength; )
    {
        if(byteLength - writeByteCount >= UINT16_MAX)
        {
            errorSub = BOARD_IAP_FILE_WRITE(ADDR_BOARD_IAP_APP_BIN_FILE_START+addrOffset + writeByteCount,bufferPtr + writeByteCount,UINT16_MAX);
            if(errorSub != ERROR_SUB_OK)
            {
                return errorSub;
            }
            writeByteCount += UINT16_MAX;
        }
        else
        {
            errorSub = BOARD_IAP_FILE_WRITE(ADDR_BOARD_IAP_APP_BIN_FILE_START+addrOffset + writeByteCount,bufferPtr + writeByteCount,byteLength - writeByteCount);
            if(errorSub != ERROR_SUB_OK)
            {
                return errorSub;
            }
            writeByteCount = byteLength;
        }
    }
    return ERROR_SUB_OK;
}

//读取系统运行模式标记
ERROR_SUB BoardIapReadCurrentMode(uint32_t* runningMode)
{
    return BoardParamReadSingle(ADDR_BOARD_IAP_PARAM_START+OFFSET_BOARD_IAP_RUNNING_MODE,runningMode);
}

//写入系统运行模式BOOT
ERROR_SUB BoardIapSetRunningModeBoot(void)
{
    return BoardParamWrireSingle(ADDR_BOARD_IAP_PARAM_START+OFFSET_BOARD_IAP_RUNNING_MODE,FLAG_BOARD_IAP_CURRENT_RUN_MODE_BOOT);
}

//写入系统运行模式APP
ERROR_SUB BoardIapSetRunningModeApp(void)
{
    return BoardParamWrireSingle(ADDR_BOARD_IAP_PARAM_START+OFFSET_BOARD_IAP_RUNNING_MODE,FLAG_BOARD_IAP_CURRENT_RUN_MODE_APP);
}

//读取板卡IAP模式标记
ERROR_SUB BoardIapReadRunMode(BOARD_RUN_MODE* runMode)
{
    //首先读取IAP标记信息
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint32_t runningModeFlag = 0;
    //读取标记,如果没有标记,设置为APP模式
    errorSub = BoardIapReadCurrentMode(&runningModeFlag);
    if(errorSub != ERROR_SUB_OK)
    {
        return errorSub;
    }
    //检查标记讯息
    if(runningModeFlag == FLAG_BOARD_IAP_CURRENT_RUN_MODE_BOOT)
    {
        *runMode = BOARD_RUN_MODE_BOOT;
        //只支持这两种模式
        return ERROR_SUB_OK;
    }
    if(runningModeFlag == FLAG_BOARD_IAP_CURRENT_RUN_MODE_APP)
    {
        *runMode = BOARD_RUN_MODE_APP;
        //只支持这两种模式
        return ERROR_SUB_OK;
    }
    //没有标记,默认设置为APP标记
    errorSub = BoardIapSetRunningModeApp();
    if(errorSub != ERROR_SUB_OK)
    {
        return errorSub;
    }
    *runMode = BOARD_RUN_MODE_APP;
    return ERROR_SUB_OK;
}

//读取 BOOT IAP升级记录信息
static ERROR_SUB BoardIapReadRecord(uint32_t offset,BOARD_IAP_RECODER* recordMsgPtr)
{
    uint32_t readData = 0;
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    errorSubCode = BoardParamReadSingle(ADDR_BOARD_IAP_PARAM_START+offset,&readData);
    if(errorSubCode != ERROR_SUB_OK)
    {
        return errorSubCode;
    }
    recordMsgPtr->iapFileExistFlag = (BOARD_IAP_RECODER_FLAG)readData;
    errorSubCode = BoardParamReadSingle(ADDR_BOARD_IAP_PARAM_START+offset+4,&readData);
    if(errorSubCode != ERROR_SUB_OK)
    {
        return errorSubCode;
    }
    recordMsgPtr->boardIapSrc = (BOARD_IAP_SRC)readData;
    errorSubCode = BoardParamReadSingle(ADDR_BOARD_IAP_PARAM_START+offset+8,&readData);
    if(errorSubCode != ERROR_SUB_OK)
    {
        return errorSubCode;
    }
    recordMsgPtr->iapFileTotalBytes = readData;
    errorSubCode = BoardParamReadSingle(ADDR_BOARD_IAP_PARAM_START+offset+12,&readData);
    if(errorSubCode != ERROR_SUB_OK)
    {
        return errorSubCode;
    }
    recordMsgPtr->crc32Code = readData;
    return ERROR_SUB_OK;
}

//写入 IAP记录信息
static ERROR_SUB BoardIapWriteRecord(uint32_t offset,BOARD_IAP_RECODER* recordMsgPtr)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    errorSubCode = BoardParamWrireSingle(ADDR_BOARD_IAP_PARAM_START+offset,recordMsgPtr->iapFileExistFlag);
    if(errorSubCode != ERROR_SUB_OK)
    {
        return errorSubCode;
    }
    errorSubCode = BoardParamWrireSingle(ADDR_BOARD_IAP_PARAM_START+offset+4,recordMsgPtr->boardIapSrc);
    if(errorSubCode != ERROR_SUB_OK)
    {
        return errorSubCode;
    }
    errorSubCode = BoardParamWrireSingle(ADDR_BOARD_IAP_PARAM_START+offset+8,recordMsgPtr->iapFileTotalBytes);
    if(errorSubCode != ERROR_SUB_OK)
    {
        return errorSubCode;
    }
    errorSubCode = BoardParamWrireSingle(ADDR_BOARD_IAP_PARAM_START+offset+12,recordMsgPtr->crc32Code);
    if(errorSubCode != ERROR_SUB_OK)
    {
        return errorSubCode;
    }
    return ERROR_SUB_OK;
}

//读取 BOOT IAP升级记录信息
ERROR_SUB BoardIapReadBootRecord(BOARD_IAP_RECODER* recordMsgPtr)
{
    return BoardIapReadRecord(OFFSET_BOARD_IAP_RECORD_BOOT,recordMsgPtr);
}

//写入BOOT IAP记录信息
ERROR_SUB BoardIapWriteBootRecord(BOARD_IAP_RECODER* recordMsgPtr)
{
    return BoardIapWriteRecord(OFFSET_BOARD_IAP_RECORD_BOOT,recordMsgPtr);
}

//初始化BOOT IAP记录信息
ERROR_SUB BoardIapWriteBootRecordDefault(void)
{
    BOARD_IAP_RECODER recoder;
    recoder.iapFileExistFlag = BOARD_IAP_RECODER_FLAG_NOT_EXIST;
    recoder.boardIapSrc = BOARD_IAP_SRC_NONE;
    recoder.crc32Code = 0;
    recoder.iapFileTotalBytes = 0;
    return BoardIapWriteBootRecord(&recoder);
}

//读取 Service IAP升级记录信息
ERROR_SUB BoardIapReadServiceRecord(BOARD_IAP_RECODER* recordMsgPtr)
{
    return BoardIapReadRecord(OFFSET_BOARD_IAP_RECORD_SERVICE,recordMsgPtr);
}

//写入 Service IAP记录信息
ERROR_SUB BoardIapWriteServiceRecord(BOARD_IAP_RECODER* recordMsgPtr)
{
    return BoardIapWriteRecord(OFFSET_BOARD_IAP_RECORD_SERVICE,recordMsgPtr);
}

//初始化 Service IAP记录信息
ERROR_SUB BoardIapWriteServiceRecordDefault(void)
{
    BOARD_IAP_RECODER recoder;
    recoder.iapFileExistFlag = BOARD_IAP_RECODER_FLAG_NOT_EXIST;
    recoder.boardIapSrc = BOARD_IAP_SRC_NONE;
    recoder.crc32Code = 0;
    recoder.iapFileTotalBytes = 0;
    return BoardIapWriteServiceRecord(&recoder);
}

//读取 APP IAP升级记录信息
ERROR_SUB BoardIapReadAppRecord(BOARD_IAP_RECODER* recordMsgPtr)
{
    return BoardIapReadRecord(OFFSET_BOARD_IAP_RECORD_APP,recordMsgPtr);
}

//写入 APP IAP记录信息
ERROR_SUB BoardIapWriteAppRecord(BOARD_IAP_RECODER* recordMsgPtr)
{
    return BoardIapWriteRecord(OFFSET_BOARD_IAP_RECORD_APP,recordMsgPtr);
}

//初始化 APP IAP记录信息
ERROR_SUB BoardIapWriteAppRecordDefault(void)
{
    BOARD_IAP_RECODER recoder;
    recoder.iapFileExistFlag = BOARD_IAP_RECODER_FLAG_NOT_EXIST;
    recoder.boardIapSrc = BOARD_IAP_SRC_NONE;
    recoder.crc32Code = 0;
    recoder.iapFileTotalBytes = 0;
    return BoardIapWriteAppRecord(&recoder);
}


//板卡IAP跳转到APP
ERROR_SUB BoardIapJumpApp(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint32_t app_address = MCU_FLASH_SRV_MAIN;
    //首先检测APP是否存在
    uint32_t stackPointAddr = ((*(volatile uint32_t*)app_address)&0xF0000000);
    //检查栈顶地址是否合法.D1_DTCMRAM_BASE是DTCM的起始地址,也是程序的栈顶地址
    //如果这个位置的数据不对,那么说明没有APP
    if(stackPointAddr != D1_DTCMRAM_BASE)	
    {
        MCU_LogPrintf("BoardIapJumpApp App stackPointAddr: 0X%08X,Need 0X%08X\r\n",stackPointAddr,D1_DTCMRAM_BASE);
        //不存在APP程序,设置失败并设置运行模式是BOOT模式
        errorSub = BoardIapSetRunningModeBoot();
        if(errorSub != ERROR_SUB_OK)
        {
            MCU_LogPrintf("BoardIapSetRunningModeBoot Failed,ErrorSub: 0X%08X\r\n",errorSub);
        }
        return ERROR_SUB_BOARD_IAP_APP_NOT_EXIST;
    }
    //系统运行模式修改为APP模式
    errorSub = BoardIapSetRunningModeApp();
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogPrintf("BoardIapSetRunningModeApp Failed,ErrorSub: 0X%08X\r\n",errorSub);
        return errorSub;
    }
    //存在APP而且可以跳转
    MCU_LogPrintf("BoardIapJumpApp Start\r\n");
    CoreDelayMs(10);
    //系统锁定
    CoreLockIntGlobal();
    //清除系统全部中断
    CoreClearSystemAllInt();
    //关闭MPU
    HAL_MPU_Disable();
    //关闭CACHE
    CoreUtilCacheWriteThrough();
    SCB_DisableICache();
    SCB_DisableDCache();
    //设置新的中断向量
    SCB->VTOR = app_address;
    //设置主堆栈MSP
    __set_MSP(*(uint32_t*)app_address);
    //设置线程PSP
    __set_PSP(*(uint32_t*)app_address);
    //强制切换到特权级
    __set_CONTROL(0);
    //系统刷新,保证设置生效
    __ISB();
    //清空CACHE
    SCB_InvalidateICache();
    //清空DCACHE
    SCB_CleanDCache();
    //强制跳转
    ((BoardIapJmpAddressFuncPtr)(*(uint32_t*)(app_address + 4)))();
    //正常不可能到这里
    return ERROR_SUB_BOARD_IAP_JMP_APP_FAILED;
}

//板卡IAP跳转到BOOT
ERROR_SUB BoardIapJumpBoot(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint32_t boot_address = MCU_FLASH_SRV_BOOT;
    //首先检测BOOT是否存在
    uint32_t stackPointAddr = ((*(volatile uint32_t*)boot_address)&0xF0000000);
    //检查栈顶地址是否合法.D1_DTCMRAM_BASE是DTCM的起始地址,也是程序的栈顶地址
    //如果这个位置的数据不对,那么说明没有BOOT
    if(stackPointAddr != D1_DTCMRAM_BASE)	
    {
        MCU_LogPrintf("BoardIapJumpBoot Boot stackPointAddr: 0X%08X,Need 0X%08X\r\n",stackPointAddr,D1_DTCMRAM_BASE);
        //不存在Boot程序,设置失败并设置运行模式是APP模式
        errorSub = BoardIapSetRunningModeApp();
        if(errorSub != ERROR_SUB_OK)
        {
            MCU_LogPrintf("BoardIapSetRunningModeApp Failed,ErrorSub: 0X%08X\r\n",errorSub);
        }
        return ERROR_SUB_BOARD_IAP_BOOT_NOT_EXIST;
    }
    //系统运行模式修改为BOOT模式
    errorSub = BoardIapSetRunningModeBoot();
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogPrintf("BoardIapSetRunningModeBoot Failed,ErrorSub: 0X%08X\r\n",errorSub);
        return errorSub;
    }
    //存在Boot而且可以跳转
    MCU_LogPrintf("BoardIapJumpBoot Start\r\n");
    CoreDelayMs(10);
    //系统锁定
    CoreLockIntGlobal();
    //清除系统全部中断
    CoreClearSystemAllInt();
    //关闭MPU
    HAL_MPU_Disable();
    //关闭CACHE
    CoreUtilCacheWriteThrough();
    SCB_DisableICache();
    SCB_DisableDCache();
    //清空CACHE
    SCB_InvalidateICache();
    //清空DCACHE
    SCB_CleanDCache();
    //系统刷新,保证设置生效
    __ISB();
    //调用复位,直接回到boot里面
    CoreSystemReset();
    //正常不可能到这里
    return ERROR_SUB_BOARD_IAP_JMP_BOOT_FAILED;
}

//检查是否满足跳转APP的条件
ErrorStatus BoardIapCheckAppExist(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint32_t app_address = MCU_FLASH_SRV_MAIN;
    //首先检测APP是否存在
    uint32_t stackPointAddr = ((*(volatile uint32_t*)app_address)&0xF0000000);
    //检查栈顶地址是否合法.D1_DTCMRAM_BASE是DTCM的起始地址,也是程序的栈顶地址
    //如果这个位置的数据不对,那么说明没有APP
    if(stackPointAddr != D1_DTCMRAM_BASE)	
    {
        MCU_LogPrintf("BoardIapJumpApp App stackPointAddr: 0X%08X,Need 0X%08X\r\n",stackPointAddr,D1_DTCMRAM_BASE);
        //不存在APP程序,设置失败并设置运行模式是BOOT模式
        errorSub = BoardIapSetRunningModeBoot();
        return ERROR;
    }
    //系统运行模式修改为APP模式
    errorSub = BoardIapSetRunningModeApp();
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogPrintf("BoardIapSetRunningModeApp Failed,ErrorSub: 0X%08X\r\n",errorSub);
        return ERROR;
    }
    return SUCCESS;
}

//检查是否满足跳转到BOOT的条件
ErrorStatus BoardIapCheckBootExist(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint32_t boot_address = MCU_FLASH_SRV_BOOT;
    //首先检测BOOT是否存在
    uint32_t stackPointAddr = ((*(volatile uint32_t*)boot_address)&0xF0000000);
    //检查栈顶地址是否合法.D1_DTCMRAM_BASE是DTCM的起始地址,也是程序的栈顶地址
    //如果这个位置的数据不对,那么说明没有BOOT
    if(stackPointAddr != D1_DTCMRAM_BASE)	
    {
        MCU_LogPrintf("BoardIapJumpBoot Boot stackPointAddr: 0X%08X,Need 0X%08X\r\n",stackPointAddr,D1_DTCMRAM_BASE);
        //不存在Boot程序,设置失败并设置运行模式是APP模式
        errorSub = BoardIapSetRunningModeApp();
        return ERROR;
    }
    //系统运行模式修改为BOOT模式
    errorSub = BoardIapSetRunningModeBoot();
    if(errorSub != ERROR_SUB_OK)
    {
        MCU_LogPrintf("BoardIapSetRunningModeBoot Failed,ErrorSub: 0X%08X\r\n",errorSub);
        return ERROR;
    }
    return SUCCESS;
}


//板上IAP初始化
ERROR_SUB BoardIapInit(void)
{
    //首先读取IAP标记信息
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint32_t runningModeFlag = 0;
    BOARD_IAP_RECODER iapRecoder;
    //首先读取当前模式,如果当前模式不对,那么设置为BOOT模式
    errorSub = BoardIapReadCurrentMode(&runningModeFlag);
    if(errorSub != ERROR_SUB_OK)
    {
        return errorSub;
    }
    //检查标记位是否正确,不正确设置标记
    if((runningModeFlag != FLAG_BOARD_IAP_CURRENT_RUN_MODE_BOOT)&&(runningModeFlag != FLAG_BOARD_IAP_CURRENT_RUN_MODE_APP))
    {
        //没标记就设置为APP模式
        errorSub = BoardIapSetRunningModeApp();
        if(errorSub != ERROR_SUB_OK)
        {
            return errorSub;
        }
    }
    //三个文件记录,如果文件记录异常,设置默认文件记录,BOOT
    errorSub = BoardIapReadBootRecord(&iapRecoder);
    if(errorSub != ERROR_SUB_OK)
    {
        return errorSub;
    }
    if(((iapRecoder.iapFileExistFlag != BOARD_IAP_RECODER_FLAG_NOT_EXIST)&&(iapRecoder.iapFileExistFlag != BOARD_IAP_RECODER_FLAG_EXIST))||
            (iapRecoder.iapFileTotalBytes > LENGTH_BOARD_IAP_BOOT_BIN_FILE))
    {
        errorSub = BoardIapWriteBootRecordDefault();
        if(errorSub != ERROR_SUB_OK)
        {
            return errorSub;
        }
    }
    //SERVICE
    errorSub = BoardIapReadServiceRecord(&iapRecoder);
    if(errorSub != ERROR_SUB_OK)
    {
        return errorSub;
    }
    if(((iapRecoder.iapFileExistFlag != BOARD_IAP_RECODER_FLAG_NOT_EXIST)&&(iapRecoder.iapFileExistFlag != BOARD_IAP_RECODER_FLAG_EXIST))||
            (iapRecoder.iapFileTotalBytes > LENGTH_BOARD_IAP_SRV_BIN_FILE))
    {
        errorSub = BoardIapWriteServiceRecordDefault();
        if(errorSub != ERROR_SUB_OK)
        {
            return errorSub;
        }
    }
    //APP
    errorSub = BoardIapReadAppRecord(&iapRecoder);
    if(errorSub != ERROR_SUB_OK)
    {
        return errorSub;
    }
    if(((iapRecoder.iapFileExistFlag != BOARD_IAP_RECODER_FLAG_NOT_EXIST)&&(iapRecoder.iapFileExistFlag != BOARD_IAP_RECODER_FLAG_EXIST))||
            (iapRecoder.iapFileTotalBytes > LENGTH_BOARD_IAP_APP_BIN_FILE))
    {
        errorSub = BoardIapWriteAppRecordDefault();
        if(errorSub != ERROR_SUB_OK)
        {
            return errorSub;
        }
    }
    return ERROR_SUB_OK;
}





























