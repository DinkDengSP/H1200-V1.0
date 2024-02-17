/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-09-04 11:57:26 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-17 17:12:41 +0800
************************************************************************************************/ 
#include "ProtocolC21NetUtil.h"

//写入无符号16位数据到缓存
void ProtocolC21NetUtilWriteUint16ToBuffer(uint16_t val,uint8_t* buffer)
{
    buffer[0] = (uint8_t)val;
    buffer[1] = (uint8_t)(val>>8);
}

//写入无符号32位数据到缓存
void ProtocolC21NetUtilWriteUint32ToBuffer(uint32_t val,uint8_t* buffer)
{
    buffer[3] = (uint8_t)(val>>24);
    buffer[2] = (uint8_t)(val>>16);
    buffer[1] = (uint8_t)(val>>8);
    buffer[0] = (uint8_t)val;
}

//写入无符号64位数据到缓存
void ProtocolC21NetUtilWriteUint64ToBuffer(uint64_t val,uint8_t* buffer)
{
    buffer[7] = (uint8_t)(val>>56);
    buffer[6] = (uint8_t)(val>>48);
    buffer[5] = (uint8_t)(val>>40);
    buffer[4] = (uint8_t)(val>>32);
    buffer[3] = (uint8_t)(val>>24);
    buffer[2] = (uint8_t)(val>>16);
    buffer[1] = (uint8_t)(val>>8);
    buffer[0] = (uint8_t)val;
}


//写入有符号16位数据到缓存
void ProtocolC21NetUtilWriteInt16ToBuffer(int16_t val,uint8_t* buffer)
{
    uint16_t writeValue = 0;
    writeValue = *((uint16_t*)(&val));
    ProtocolC21NetUtilWriteUint16ToBuffer(writeValue,buffer);
}

//写入有符号32位数据到缓存
void ProtocolC21NetUtilWriteInt32ToBuffer(int32_t val,uint8_t* buffer)
{
    uint32_t writeValue = 0;
    writeValue = *((uint32_t*)(&val));
    ProtocolC21NetUtilWriteUint32ToBuffer(writeValue,buffer);
}

//写入有符号64位数据到缓存
void ProtocolC21NetUtilWriteInt64ToBuffer(int64_t val,uint8_t* buffer)
{
    uint64_t writeValue = 0;
    writeValue = *((uint64_t*)(&val));
    ProtocolC21NetUtilWriteUint64ToBuffer(writeValue,buffer);
}

//从协议包中获取32位无符号整型数据
uint32_t ProtocolC21NetUtilReadUint32Buffer(uint8_t* bufferPtr)
{
    uint32_t localTemp = 0;
    localTemp += bufferPtr[3];
    localTemp <<= 8;
    localTemp += bufferPtr[2];
    localTemp <<= 8;
    localTemp += bufferPtr[1];
    localTemp <<= 8;
    localTemp += bufferPtr[0];
    //返回结果
    return localTemp;
}

//从协议包中获取16位无符号整型数据
uint16_t ProtocolC21NetUtilReadUint16Buffer(uint8_t* bufferPtr)
{
    uint16_t localTemp = 0;
    localTemp += bufferPtr[1];
    localTemp <<= 8;
    localTemp += bufferPtr[0];
    //返回结果
    return localTemp;
}

//从缓存中读取64位无符号数
uint64_t ProtocolC21NetUtilReadUint64FormBuffer(uint8_t* buffer)
{
    uint64_t localTemp = 0;
    localTemp += buffer[7];
    localTemp <<= 8;
    localTemp += buffer[6];
    localTemp <<= 8;
    localTemp += buffer[5];
    localTemp <<= 8;
    localTemp += buffer[4];
    localTemp <<= 8;
    localTemp += buffer[3];
    localTemp <<= 8;
    localTemp += buffer[2];
    localTemp <<= 8;
    localTemp += buffer[1];
    localTemp <<= 8;
    localTemp += buffer[0];
    //返回结果
    return localTemp;
}

//从协议包中获取16位有符号整型数据
int16_t ProtocolC21NetUtilReadInt16Buffer(uint8_t* bufferPtr)
{
    uint16_t readValueUint = ProtocolC21NetUtilReadUint16Buffer(bufferPtr);
    int16_t readValueInt = 0;
    readValueInt = *((int16_t*)(&readValueUint));
    return readValueInt;
}

//从协议包中获取32位有符号整型数据
int32_t ProtocolC21NetUtilReadInt32Buffer(uint8_t* bufferPtr)
{
    uint32_t readValueUint = ProtocolC21NetUtilReadUint32Buffer(bufferPtr);
    int32_t readValueInt = 0;
    readValueInt = *((int32_t*)(&readValueUint));
    return readValueInt;
}

//从缓存中读取64位有符号数
int64_t ProtocolC21NetUtilReadInt64FormBuffer(uint8_t* bufferPtr)
{
    uint64_t readValueUint = ProtocolC21NetUtilReadUint64FormBuffer(bufferPtr);
    int64_t readValueInt = 0;
    readValueInt = *((int64_t*)(&readValueUint));
    return readValueInt;
}










