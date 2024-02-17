/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-16 13:31:54 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-16 17:25:04 +0800
************************************************************************************************/ 
#include "MCU_LogShow.h"

//根据编译器加载不同的头文件
#if ((defined __ICCARM__) && (__VER__ >= 8000000)) || ((defined __ICCRX__)  && (__VER__ >= 400))
  #include <LowLevelIOInterface.h>
#else
  #include <yfuns.h>
#endif

#pragma module_name = "?__write"

//printf的底层实现
size_t __write(int handle, const unsigned char * buffer, size_t size) 
{
    (void) handle;  /* Not used, avoid warning */
    MCU_LogSendBuffer((uint8_t*)buffer, (uint16_t)size);
    return size;
}

//ptintf的底层实现
size_t __write_buffered(int handle, const unsigned char * buffer, size_t size) 
{
  (void) handle;  /* Not used, avoid warning */
  MCU_LogSendBuffer((uint8_t*)buffer, (uint16_t)size);
  return size;
}

#if(LOW_LEVEL_SHOW_MODE_USER == LOW_LEVEL_SHOW_MODE_OFF)
//底层打印数组,空实现
void MCU_LogShowArrayBuffer(uint8_t* prefixStr,uint8_t* bufferPtr,uint16_t bufferLength)
{
    asm("nop");
}

#else

//显示缓存的长度
#define BYTE_LENGTH_LOW_LEVEL_SHOW_ARRAY        LENGTH_BYTES_BUF_TO_HEX_STR(LOW_LEVEL_BUF_SHOW_SINGLE_LINE)
//显示缓存
static uint8_t MCU_LogShowArrayTempBuf[BYTE_LENGTH_LOW_LEVEL_SHOW_ARRAY];
//底层打印数组
void MCU_LogShowArrayBuffer(uint8_t* prefixStr,uint8_t* bufferPtr,uint16_t bufferLength)
{
    uint16_t indexUtilCount = 0;
    uint16_t resultStringLen = 0;
    //申请内存
    uint8_t* bufferStringPtr = MCU_LogShowArrayTempBuf;
    //打印加锁
    MCU_LogShowGetLock();
    //首先打印前缀
    MCU_LogSendString(prefixStr);
    //循环转换并打印
    do
    {
        //数据转换
        if(bufferLength - indexUtilCount >= LOW_LEVEL_BUF_SHOW_SINGLE_LINE)
        {
            CoreConvertByteBufToHexStr(bufferPtr + indexUtilCount,LOW_LEVEL_BUF_SHOW_SINGLE_LINE,bufferStringPtr,
                                        LENGTH_BYTES_BUF_TO_HEX_STR(LOW_LEVEL_BUF_SHOW_SINGLE_LINE),&resultStringLen);
            indexUtilCount += LOW_LEVEL_BUF_SHOW_SINGLE_LINE;
        }
        else
        {
            CoreConvertByteBufToHexStr(bufferPtr + indexUtilCount,(bufferLength - indexUtilCount),bufferStringPtr,
                                        LENGTH_BYTES_BUF_TO_HEX_STR((bufferLength - indexUtilCount)),&resultStringLen);
            indexUtilCount = bufferLength;
        }
        //数据打印
        MCU_LogSendString(bufferStringPtr);
    }while(indexUtilCount < bufferLength);
    //打印解锁
    MCU_LogShowReleaseLock();
    return;
}
#endif
















