/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-11-03 16:12:17 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-11-06 10:54:29 +0800
************************************************************************************************/ 
#include "BoardParam.h"
#include "SystemVersion.h"
#include "DriverConfigBoard.h"
#include BOARD_PARAM_INCLUDE


//参数单次写入
ERROR_SUB BoardParamWrireSingle(uint8_t addr,uint32_t val)
{
    uint32_t writeValue = val;
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //读取指定地址
    uint32_t readData = 0;
    errorSubCode = BOARD_PARAM_READ(addr,(uint8_t*)(&readData),4);
    if(errorSubCode != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSubCode);
        return errorSubCode;
    }
    if(readData == writeValue)
    {
        //不用写入
        return ERROR_SUB_OK;
    }
    //写入
    errorSubCode = BOARD_PARAM_WRITE(addr,(uint8_t*)(&writeValue),4);
    if(errorSubCode != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSubCode);
        return errorSubCode;
    }
    //读取出来
    errorSubCode = BOARD_PARAM_READ(addr,(uint8_t*)(&readData),4);
    if(errorSubCode != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSubCode);
        return errorSubCode;
    }
    if(readData == writeValue)
    {
        //检查正常
        return ERROR_SUB_OK;
    }
    MCU_LogPrintf("%s,WriteVerify Failed\r\n",__func__);
    return ERROR_SUB_BOARD_PARAM_WRITE_VERIFY;
}

//参数单个读取
ERROR_SUB BoardParamReadSingle(uint8_t addr,uint32_t* valPtr)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //读取指定地址
    uint32_t readData = 0;
    errorSubCode = BOARD_PARAM_READ(addr,(uint8_t*)(&readData),4);
    if(errorSubCode != ERROR_SUB_OK)
    {
        MCU_LogShowErrorSubCode(errorSubCode);
        return errorSubCode;
    }
    *valPtr = readData;
    return ERROR_SUB_OK;
}

//读取版本号
static ERROR_SUB BoardParamReadVersion(uint32_t startAddr,BOARD_SOFT_VERSION* versionDatPtr)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //读取指定地址
    uint32_t readData = 0;
    errorSubCode = BoardParamReadSingle(startAddr,&readData);
    if(errorSubCode != ERROR_SUB_OK)
    {
        return errorSubCode;
    }
    //赋值
    versionDatPtr->devVersion = readData;
    errorSubCode = BoardParamReadSingle(startAddr+4,&readData);
    if(errorSubCode != ERROR_SUB_OK)
    {
        return errorSubCode;
    }
    //赋值
    versionDatPtr->tempVersion = readData;
    errorSubCode = BoardParamReadSingle(startAddr+8,&readData);
    if(errorSubCode != ERROR_SUB_OK)
    {
        return errorSubCode;
    }
    //赋值
    versionDatPtr->subVersion = readData;
    errorSubCode = BoardParamReadSingle(startAddr+12,&readData);
    if(errorSubCode != ERROR_SUB_OK)
    {
        return errorSubCode;
    }
    //赋值
    versionDatPtr->mainVersion = readData;
    //回传完成
    return ERROR_SUB_OK;
}

//写入版本号
ERROR_SUB BoardParamWriteVersion(uint32_t startAddr)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //首先进行版本号读取
    BOARD_SOFT_VERSION codeVersion;
    errorSubCode = BoardParamReadVersion(startAddr,&codeVersion);
    if(errorSubCode != ERROR_SUB_OK)
    {
        return errorSubCode;
    }
    //检查版本号对不对的上
    if((codeVersion.devVersion == SYSTEM_DEV_VERSION_STORE)&&(codeVersion.tempVersion == SYSTEM_VERSION_TEMP)&&
        (codeVersion.subVersion == SYSTEM_VERSION_SUB)&&(codeVersion.mainVersion == SYSTEM_VERSION_MAIN))
    {
        //版本对的上,不需要写入
        return ERROR_SUB_OK;
    }
    //开始写入
    codeVersion.devVersion = SYSTEM_DEV_VERSION_STORE;
    codeVersion.tempVersion = SYSTEM_VERSION_TEMP;
    codeVersion.subVersion = SYSTEM_VERSION_SUB;
    codeVersion.mainVersion = SYSTEM_VERSION_MAIN;
    //写入
    errorSubCode = BoardParamWrireSingle(startAddr,codeVersion.devVersion);
    if(errorSubCode != ERROR_SUB_OK)
    {
        return errorSubCode;
    }
    //写入
    errorSubCode = BoardParamWrireSingle(startAddr+4,codeVersion.tempVersion);
    if(errorSubCode != ERROR_SUB_OK)
    {
        return errorSubCode;
    }
    //写入
    errorSubCode = BoardParamWrireSingle(startAddr+8,codeVersion.subVersion);
    if(errorSubCode != ERROR_SUB_OK)
    {
        return errorSubCode;
    }
    //写入
    errorSubCode = BoardParamWrireSingle(startAddr+12,codeVersion.mainVersion);
    if(errorSubCode != ERROR_SUB_OK)
    {
        return errorSubCode;
    }
    return ERROR_SUB_OK;
}

//读取BOOT版本号
ERROR_SUB BoardParamReadBootVersion(BOARD_SOFT_VERSION* versionDatPtr)
{
    return BoardParamReadVersion(ADDR_BOARD_PARAM_BOOT_VERSION,versionDatPtr);
}

//写入BOOT版本号
ERROR_SUB BoardParamWriteBootVersion(void)
{
    return BoardParamWriteVersion(ADDR_BOARD_PARAM_BOOT_VERSION);
}

//读取SERVICE版本号
ERROR_SUB BoardParamReadServiceVersion(BOARD_SOFT_VERSION* versionDatPtr)
{
    return BoardParamReadVersion(ADDR_BOARD_PARAM_SERVICE_VERSION,versionDatPtr);
}

//写入SERVICE版本号
ERROR_SUB BoardParamWriteServiceVersion(void)
{
    return BoardParamWriteVersion(ADDR_BOARD_PARAM_SERVICE_VERSION);
}

//读取APP版本号
ERROR_SUB BoardParamReadAppVersion(BOARD_SOFT_VERSION* versionDatPtr)
{
    return BoardParamReadVersion(ADDR_BOARD_PARAM_APP_VERSION,versionDatPtr);
}

//写入APP版本号
ERROR_SUB BoardParamWriteAppVersion(void)
{
    return BoardParamWriteVersion(ADDR_BOARD_PARAM_APP_VERSION);
}

//检查AT24CXX自检成功标记
ErrorStatus BoardParamCheckFlagAT24CXX(void)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //读取指定地址
    uint32_t readData = 0;
    errorSubCode = BoardParamReadSingle(ADDR_BOARD_PARAM_AT24CXX_CHECK,&readData);
    if(errorSubCode != ERROR_SUB_OK)
    {
        return ERROR;
    }
    if(readData == BOARD_AT24CXX_CHECK_OK_FLAG)
    {
        return SUCCESS;
    }
    return ERROR;
}

//设置AT24CXX自检成功标记
ERROR_SUB BoardParamSetCheckFlagAT24CXX(ErrorStatus set)
{
    uint32_t writeValue = BOARD_AT24CXX_CHECK_OK_FLAG;
    if(set == ERROR)
    {
        //设置取消的话,按位取反标记
        writeValue = (~writeValue);
    }
    return BoardParamWrireSingle(ADDR_BOARD_PARAM_AT24CXX_CHECK,writeValue);
}

//检查FRAM低级格式化标记
ErrorStatus BoardParamCheckFormatFlagFRAM(void)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //读取指定地址
    uint32_t readData = 0;
    errorSubCode = BoardParamReadSingle(ADDR_BOARD_PARAM_FRAM_FORMAT,&readData);
    if(errorSubCode != ERROR_SUB_OK)
    {
        return ERROR;
    }
    if(readData == BOARD_FRAM_FORMAT_OK_FLAG)
    {
        return SUCCESS;
    }
    return ERROR;
}

//设置FRAM低级格式化标记
ERROR_SUB BoardParamSetCheckFormatFlagFRAM(ErrorStatus set)
{
    uint32_t writeValue = BOARD_FRAM_FORMAT_OK_FLAG;
    if(set == ERROR)
    {
        //设置取消的话,按位取反标记
        writeValue = (~writeValue);
    }
    return BoardParamWrireSingle(ADDR_BOARD_PARAM_FRAM_FORMAT,writeValue);
}

//检查FRAM自检成功标记
ErrorStatus BoardParamCheckFlagFRAM(void)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //读取指定地址
    uint32_t readData = 0;
    errorSubCode = BoardParamReadSingle(ADDR_BOARD_PARAM_FRAM_CHECK,&readData);
    if(errorSubCode != ERROR_SUB_OK)
    {
        return ERROR;
    }
    if(readData == BOARD_FRAM_CHECK_OK_FLAG)
    {
        return SUCCESS;
    }
    return ERROR;
}

//设置FRAM自检成功标记
ERROR_SUB BoardParamSetCheckFlagFRAM(ErrorStatus set)
{
    uint32_t writeValue = BOARD_FRAM_CHECK_OK_FLAG;
    if(set == ERROR)
    {
        //设置取消的话,按位取反标记
        writeValue = (~writeValue);
    }
    return BoardParamWrireSingle(ADDR_BOARD_PARAM_FRAM_CHECK,writeValue);
}

//检查EX_FLASH低级格式化标记
ErrorStatus BoardParamCheckExFlashFormatFlag(void)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //读取指定地址
    uint32_t readData = 0;
    errorSubCode = BoardParamReadSingle(ADDR_BOARD_PARAM_EX_FLASH_FORMAT,&readData);
    if(errorSubCode != ERROR_SUB_OK)
    {
        return ERROR;
    }
    if(readData == BOARD_EX_FLASH_FORMAT_OK_FLAG)
    {
        return SUCCESS;
    }
    return ERROR;
}

//设置EX_FLASH低级格式化标记
ERROR_SUB BoardParamSetCheckExFlashFormatFlag(ErrorStatus set)
{
    uint32_t writeValue = BOARD_EX_FLASH_FORMAT_OK_FLAG;
    if(set == ERROR)
    {
        //设置取消的话,按位取反标记
        writeValue = (~writeValue);
    }
    return BoardParamWrireSingle(ADDR_BOARD_PARAM_EX_FLASH_FORMAT,writeValue);
}

//检查EX_FLASH自检成功标记
ErrorStatus BoardParamCheckExFlashFlag(void)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //读取指定地址
    uint32_t readData = 0;
    errorSubCode = BoardParamReadSingle(ADDR_BOARD_PARAM_EX_FLASH_CHECK,&readData);
    if(errorSubCode != ERROR_SUB_OK)
    {
        return ERROR;
    }
    if(readData == BOARD_EX_FLASH_CHECK_OK_FLAG)
    {
        return SUCCESS;
    }
    return ERROR;
}

//设置EX_FLASH自检成功标记
ERROR_SUB BoardParamSetCheckExFlashFlag(ErrorStatus set)
{
    uint32_t writeValue = BOARD_EX_FLASH_CHECK_OK_FLAG;
    if(set == ERROR)
    {
        //设置取消的话,按位取反标记
        writeValue = (~writeValue);
    }
    return BoardParamWrireSingle(ADDR_BOARD_PARAM_EX_FLASH_CHECK,writeValue);
}

//检查EXRAM自检成功标记
ErrorStatus BoardParamCheckExRamFlag(void)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //读取指定地址
    uint32_t readData = 0;
    errorSubCode = BoardParamReadSingle(ADDR_BOARD_PARAM_EXRAM_CHECK,&readData);
    if(errorSubCode != ERROR_SUB_OK)
    {
        return ERROR;
    }
    if(readData == BOARD_EXRAM_CHECK_OK_FLAG)
    {
        return SUCCESS;
    }
    return ERROR;
}

//设置EXRAM自检成功标记
ERROR_SUB BoardParamSetCheckExRamFlag(ErrorStatus set)
{
    uint32_t writeValue = BOARD_EXRAM_CHECK_OK_FLAG;
    if(set == ERROR)
    {
        //设置取消的话,按位取反标记
        writeValue = (~writeValue);
    }
    return BoardParamWrireSingle(ADDR_BOARD_PARAM_EXRAM_CHECK,writeValue);
}

//检查NET PHY1 IP检查标记
ErrorStatus BoardParamCheckNetPhy1IpFlag(void)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //读取指定地址
    uint32_t readData = 0;
    errorSubCode = BoardParamReadSingle(ADDR_BOARD_PARAM_NET_PHY1_IP_CHECK,&readData);
    if(errorSubCode != ERROR_SUB_OK)
    {
        return ERROR;
    }
    if(readData == BOARD_NET_PHY1_IP_CHECK_OK_FLAG)
    {
        return SUCCESS;
    }
    return ERROR;
}

//设置NET PHY1 IP检查标记
ERROR_SUB BoardParamSetCheckNetPhy1IpFlag(ErrorStatus set)
{
    uint32_t writeValue = BOARD_NET_PHY1_IP_CHECK_OK_FLAG;
    if(set == ERROR)
    {
        //设置取消的话,按位取反标记
        writeValue = (~writeValue);
    }
    return BoardParamWrireSingle(ADDR_BOARD_PARAM_NET_PHY1_IP_CHECK,writeValue);
}

//写入NET PHY1 IP值
ERROR_SUB BoardParamSetNetPhy1Ip(uint32_t ipAddr)
{
    return BoardParamWrireSingle(ADDR_BOARD_PARAM_NET_PHY1_IP_ADDR,ipAddr);
}

//写入NET_PHY1默认IP值
ERROR_SUB BoardParamSetNetPhy1DefaultIp(void)
{
    return BoardParamWrireSingle(ADDR_BOARD_PARAM_NET_PHY1_IP_ADDR,DEFAULT_VALUE_BOARD_PARAM_NET_PHY1_IP);
}

//读取NET PHY1 IP值
ERROR_SUB BoardParamReadNetPhy1IpCurrent(uint32_t* ipValue)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //读取指定地址
    errorSubCode = BoardParamReadSingle(ADDR_BOARD_PARAM_NET_PHY1_IP_ADDR,ipValue);
    return errorSubCode;
}

//检查NET PHY2 IP检查标记
ErrorStatus BoardParamCheckNetPhy2IpFlag(void)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //读取指定地址
    uint32_t readData = 0;
    errorSubCode = BoardParamReadSingle(ADDR_BOARD_PARAM_NET_PHY2_IP_CHECK,&readData);
    if(errorSubCode != ERROR_SUB_OK)
    {
        return ERROR;
    }
    if(readData == BOARD_NET_PHY2_IP_CHECK_OK_FLAG)
    {
        return SUCCESS;
    }
    return ERROR;
}

//设置NET PHY2 IP检查标记
ERROR_SUB BoardParamSetCheckNetPhy2IpFlag(ErrorStatus set)
{
    uint32_t writeValue = BOARD_NET_PHY2_IP_CHECK_OK_FLAG;
    if(set == ERROR)
    {
        //设置取消的话,按位取反标记
        writeValue = (~writeValue);
    }
    return BoardParamWrireSingle(ADDR_BOARD_PARAM_NET_PHY2_IP_CHECK,writeValue);
}

//写入NET PHY2 IP值
ERROR_SUB BoardParamSetNetPhy2Ip(uint32_t ipAddr)
{
    return BoardParamWrireSingle(ADDR_BOARD_PARAM_NET_PHY2_IP_ADDR,ipAddr);
}

//写入NET_PHY2默认IP值
ERROR_SUB BoardParamSetNetPhy2DefaultIp(void)
{
    return BoardParamWrireSingle(ADDR_BOARD_PARAM_NET_PHY2_IP_ADDR,DEFAULT_VALUE_BOARD_PARAM_NET_PHY2_IP);
}

//读取NET PHY2 IP值
ERROR_SUB BoardParamReadNetPhy2IpCurrent(uint32_t* ipValue)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //读取指定地址
    errorSubCode = BoardParamReadSingle(ADDR_BOARD_PARAM_NET_PHY2_IP_ADDR,ipValue);
    return errorSubCode;
}














