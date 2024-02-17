/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-21 10:19:22 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-21 16:32:03 +0800
************************************************************************************************/ 
#include "BoardAT24CXX.h"
#include "DriverConfigBoard.h"
#include "tx_mutex.h"

//设备互斥信号量
static TX_MUTEX mutexAT24CXX;

//获取设备互斥量
void BoardAT24CXX_GetLock(void)
{
    //申请设备互斥信号量
    tx_mutex_get(&mutexAT24CXX,TX_WAIT_FOREVER);
}

//释放设备互斥量
void BoardAT24CXX_ReleaseLock(void)
{
    //申请设备互斥信号量
    tx_mutex_put(&mutexAT24CXX);
}

//初始化
void BoardAT24CXX_Init(void)
{
    if(mutexAT24CXX.tx_mutex_id == TX_MUTEX_ID)
    {
        //已经初始化过了
        return;
    }
    //创建设备互斥信号量
    tx_mutex_create(&mutexAT24CXX,"mutexAT24CXX",TX_INHERIT);
    //端口初始化
    BOARD_AT24CXX_LOW_LEVEL_PORT_INIT();
}

//查询设备是否空闲,通过检查器件应答的方式，判断内部写操作是否完成, 一般小于 10ms
//CLK频率为200KHz时,查询次数为30次左右
ERROR_SUB BoardAT24CXX_WaitIdle(uint16_t retryCount)
{
    HAL_StatusTypeDef halState = HAL_OK;
    //申请设备互斥信号量
    BoardAT24CXX_GetLock();
    while(retryCount != 0)
    {
        halState = BOARD_AT24CXX_LOW_LEVEL_CHECK_READY(ADDR_DEVICE_AT24CXX_USE);
        if(halState == HAL_OK)
        {
            //释放设备互斥信号量
            BoardAT24CXX_ReleaseLock();
            return ERROR_SUB_OK;
        }
        //还要等
        retryCount--;
    }
    //释放设备互斥信号量
    BoardAT24CXX_ReleaseLock();
    //显示错误
    MCU_LogPrintf("%s TimeOut\r\n",__func__);
    return ERROR_SUB_BOARD_AT24CXX_WAIT_IDLE;
}

//写入页数据
ERROR_SUB BoardAT24CXX_WritePage(uint16_t addr,uint8_t* writeBufPtr,uint16_t byteCount)
{
    HAL_StatusTypeDef halState = HAL_OK;
    //申请设备互斥信号量
    BoardAT24CXX_GetLock();
    //检查空闲
    if(ERROR_SUB_OK != BoardAT24CXX_WaitIdle(TIME_MS_MAX_AT24CXX_WAIT_IDLE/BOARD_AT24CXX_LOW_LEVEL_SINGLE_TIMEOUT_MS))
    {
        //没检查到空闲,释放设备互斥信号量
        BoardAT24CXX_ReleaseLock();
        return ERROR_SUB_BOARD_AT24CXX_WAIT_IDLE;
    }
#if(BYTE_SIZE_AT24CXX_USE > BYTE_SIZE_AT24C16)
    halState = BOARD_AT24CXX_LOW_LEVEL_WRITE(ADDR_DEVICE_AT24CXX_USE,IIC_SUB_ADDR_SIZE_16BIT,addr,writeBufPtr,byteCount,
                                                TIME_MS_MAX_AT24CXX_WAIT_IDLE_BYTE);
#else
    uint8_t deviceAddr = ADDR_DEVICE_AT24CXX_USE + ((addr/256)<<1);
    halState = BOARD_AT24CXX_LOW_LEVEL_WRITE(deviceAddr,IIC_SUB_ADDR_SIZE_8BIT,addr,writeBufPtr,byteCount,
                                                TIME_MS_MAX_AT24CXX_WAIT_IDLE_BYTE);
#endif
    if(halState != HAL_OK)
    {
        MCU_LogShowHalState(halState,0);
        //如果发生错误,不需要等待有效,直接释放设备互斥量
        BoardAT24CXX_ReleaseLock();
        return ERROR_SUB_BOARD_AT24CXX_WRITE_PAGE;
    }
    //检查空闲
    if(ERROR_SUB_OK != BoardAT24CXX_WaitIdle(TIME_MS_MAX_AT24CXX_WAIT_IDLE/BOARD_AT24CXX_LOW_LEVEL_SINGLE_TIMEOUT_MS))
    {
        //没检查到空闲,释放设备互斥信号量
        BoardAT24CXX_ReleaseLock();
        return ERROR_SUB_BOARD_AT24CXX_WAIT_IDLE;
    }
    //完成,空闲
    BoardAT24CXX_ReleaseLock();
    return ERROR_SUB_OK;
}

//读取页数据
ERROR_SUB BoardAT24CXX_ReadPage(uint16_t addr,uint8_t* readBufPtr,uint16_t byteCount)
{
    HAL_StatusTypeDef halState = HAL_OK;
    //申请设备互斥信号量
    BoardAT24CXX_GetLock();
    //检查空闲
    if(ERROR_SUB_OK != BoardAT24CXX_WaitIdle(TIME_MS_MAX_AT24CXX_WAIT_IDLE/BOARD_AT24CXX_LOW_LEVEL_SINGLE_TIMEOUT_MS))
    {
        //没检查到空闲,释放设备互斥信号量
        BoardAT24CXX_ReleaseLock();
        return ERROR_SUB_BOARD_AT24CXX_WAIT_IDLE;
    }
#if(BYTE_SIZE_AT24CXX_USE > BYTE_SIZE_AT24C16)
    halState = BOARD_AT24CXX_LOW_LEVEL_READ(ADDR_DEVICE_AT24CXX_USE,IIC_SUB_ADDR_SIZE_16BIT,addr,readBufPtr,byteCount,
                                                TIME_MS_MAX_AT24CXX_WAIT_IDLE_BYTE);
#else
    uint8_t deviceAddr = ADDR_DEVICE_AT24CXX_USE + ((addr/256)<<1);
    halState = BOARD_AT24CXX_LOW_LEVEL_READ(deviceAddr,IIC_SUB_ADDR_SIZE_8BIT,addr,readBufPtr,byteCount,
                                                TIME_MS_MAX_AT24CXX_WAIT_IDLE_BYTE);
#endif
    //完成,空闲
    BoardAT24CXX_ReleaseLock();
    if(halState != HAL_OK)
    {
        MCU_LogShowHalState(halState,0);
        return ERROR_SUB_BOARD_AT24CXX_READ;
    }
    return ERROR_SUB_OK;
}


//写入数据
ERROR_SUB BoardAT24CXX_WriteMultiByte(uint16_t addr,uint8_t* writeBufPtr,uint16_t byteCount)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //第一页的页偏移地址
    uint8_t firstPageOffset = addr % PAGE_SIZE_AT24CXX_USE;
    //第一页剩余空间
    uint8_t firstPageRemain = PAGE_SIZE_AT24CXX_USE - firstPageOffset;
    //计算几个整页
    uint8_t numOfPage =  byteCount / PAGE_SIZE_AT24CXX_USE;
    //计算最后的单页多少字节
    uint8_t numOfSingle = byteCount % PAGE_SIZE_AT24CXX_USE;
    //申请设备互斥信号量
    BoardAT24CXX_GetLock();
    //刚好从某一页的起始地址开始
    if(firstPageOffset == 0) 
    {
        //不足一页的情况下直接写
        if(numOfPage == 0) 
        {
            errorSubCode = BoardAT24CXX_WritePage(addr,writeBufPtr, byteCount);
            if(errorSubCode != ERROR_SUB_OK)
            {
                //完成,空闲
                BoardAT24CXX_ReleaseLock();
                //错误显示
                MCU_LogShowErrorSubCode(errorSubCode);
                return errorSubCode;
            }
        }
        //存在多页面
        else  
        {
            //先写入多页
            while(numOfPage--)
            {
                errorSubCode = BoardAT24CXX_WritePage(addr,writeBufPtr, PAGE_SIZE_AT24CXX_USE);
                if(errorSubCode != ERROR_SUB_OK)
                {
                    //完成,空闲
                    BoardAT24CXX_ReleaseLock();
                    //错误显示
                    MCU_LogShowErrorSubCode(errorSubCode);
                    return errorSubCode;
                }
                addr +=  PAGE_SIZE_AT24CXX_USE;
                writeBufPtr += PAGE_SIZE_AT24CXX_USE;
            }
            //最后一页
            if(numOfSingle != 0)
            {
                errorSubCode = BoardAT24CXX_WritePage(addr,writeBufPtr, numOfSingle);
                if(errorSubCode != ERROR_SUB_OK)
                {
                    //完成,空闲
                    BoardAT24CXX_ReleaseLock();
                    //错误显示
                    MCU_LogShowErrorSubCode(errorSubCode);
                    return errorSubCode;
                }
            }
        }
    }
    //并不对其某一页的开始地址
    else 
    {
        //不足一页的情况下直接写
        if(numOfPage == 0) 
        {
            //存在一种情况,两个字节刚好跨页
            if(byteCount > firstPageRemain)
            {
                //写入上一页残留
                errorSubCode = BoardAT24CXX_WritePage(addr,writeBufPtr,firstPageRemain);
                if(errorSubCode != ERROR_SUB_OK)
                {
                    //完成,空闲
                    BoardAT24CXX_ReleaseLock();
                    //错误显示
                    MCU_LogShowErrorSubCode(errorSubCode);
                    return errorSubCode;
                }
                //地址和计数增加
                writeBufPtr += firstPageRemain;
                byteCount -= firstPageRemain;
                addr += firstPageRemain;
                //没写完的写到下一页
                errorSubCode = BoardAT24CXX_WritePage(addr,writeBufPtr,byteCount);
                if(errorSubCode != ERROR_SUB_OK)
                {
                    //完成,空闲
                    BoardAT24CXX_ReleaseLock();
                    //错误显示
                    MCU_LogShowErrorSubCode(errorSubCode);
                    return errorSubCode;
                }
            }
            else
            {
                errorSubCode = BoardAT24CXX_WritePage(addr,writeBufPtr,byteCount);
                if(errorSubCode != ERROR_SUB_OK)
                {
                    //完成,空闲
                    BoardAT24CXX_ReleaseLock();
                    //错误显示
                    MCU_LogShowErrorSubCode(errorSubCode);
                    return errorSubCode;
                }
            }
        }
        //存在多页面
        else
        {
            byteCount -= firstPageRemain;
            numOfPage =  byteCount / PAGE_SIZE_AT24CXX_USE;
            numOfSingle = byteCount % PAGE_SIZE_AT24CXX_USE;	
            //第一页
            if(firstPageRemain != 0)
            {  
                errorSubCode = BoardAT24CXX_WritePage(addr,writeBufPtr, firstPageRemain);
                if(errorSubCode != ERROR_SUB_OK)
                {
                    //完成,空闲
                    BoardAT24CXX_ReleaseLock();
                    //错误显示
                    MCU_LogShowErrorSubCode(errorSubCode);
                    return errorSubCode;
                }
                addr += firstPageRemain;
                writeBufPtr += firstPageRemain;
            } 
            //后续整页
            while(numOfPage--)
            {
                errorSubCode = BoardAT24CXX_WritePage(addr,writeBufPtr, PAGE_SIZE_AT24CXX_USE);
                if(errorSubCode != ERROR_SUB_OK)
                {
                    //完成,空闲
                    BoardAT24CXX_ReleaseLock();
                    //错误显示
                    MCU_LogShowErrorSubCode(errorSubCode);
                    return errorSubCode;
                }
                addr +=  PAGE_SIZE_AT24CXX_USE;
                writeBufPtr += PAGE_SIZE_AT24CXX_USE;
            }
            //最后一页
            if(numOfSingle != 0)
            {
                errorSubCode = BoardAT24CXX_WritePage(addr,writeBufPtr, numOfSingle);
                if(errorSubCode != ERROR_SUB_OK)
                {
                    //完成,空闲
                    BoardAT24CXX_ReleaseLock();
                    //错误显示
                    MCU_LogShowErrorSubCode(errorSubCode);
                    return errorSubCode;
                }
            }
        }
    } 
    //完成,空闲
    BoardAT24CXX_ReleaseLock();
    return ERROR_SUB_OK; 
}


//读取数据
ERROR_SUB BoardAT24CXX_ReadMultiByte(uint16_t addr,uint8_t* readBufPtr,uint16_t byteCount)
{
    ERROR_SUB errorSubCode = ERROR_SUB_OK;
    //第一页的页偏移地址
    uint8_t firstPageOffset = addr % PAGE_SIZE_AT24CXX_USE;
    //第一页剩余空间
    uint8_t firstPageRemain = PAGE_SIZE_AT24CXX_USE - firstPageOffset;
    //计算几个整页
    uint8_t numOfPage =  byteCount / PAGE_SIZE_AT24CXX_USE;
    //计算最后的单页多少字节
    uint8_t numOfSingle = byteCount % PAGE_SIZE_AT24CXX_USE;
    //申请设备互斥信号量
    BoardAT24CXX_GetLock();
    //刚好从某一页的起始地址开始
    if(firstPageOffset == 0) 
    {
        //不足一页的情况下直接读
        if(numOfPage == 0) 
        {
            errorSubCode = BoardAT24CXX_ReadPage(addr,readBufPtr, byteCount);
            if(errorSubCode != ERROR_SUB_OK)
            {
                //完成,空闲
                BoardAT24CXX_ReleaseLock();
                //错误显示
                MCU_LogShowErrorSubCode(errorSubCode);
                return errorSubCode;
            }
        }
        //存在多页面
        else  
        {
            //先读入多页
            while(numOfPage--)
            {
                errorSubCode = BoardAT24CXX_ReadPage(addr,readBufPtr, PAGE_SIZE_AT24CXX_USE);
                if(errorSubCode != ERROR_SUB_OK)
                {
                    //完成,空闲
                    BoardAT24CXX_ReleaseLock();
                    //错误显示
                    MCU_LogShowErrorSubCode(errorSubCode);
                    return errorSubCode;
                }
                addr +=  PAGE_SIZE_AT24CXX_USE;
                readBufPtr += PAGE_SIZE_AT24CXX_USE;
            }
            //最后一页
            if(numOfSingle != 0)
            {
                errorSubCode = BoardAT24CXX_ReadPage(addr,readBufPtr, numOfSingle);
                if(errorSubCode != ERROR_SUB_OK)
                {
                    //完成,空闲
                    BoardAT24CXX_ReleaseLock();
                    //错误显示
                    MCU_LogShowErrorSubCode(errorSubCode);
                    return errorSubCode;
                }
            }
        }
    }
    //并不对其某一页的开始地址
    else 
    {
        //不足一页的情况下直接读
        if(numOfPage == 0) 
        {
            //存在一种情况,两个字节刚好跨页
            if(byteCount > firstPageRemain)
            {
                //读入上一页残留
                errorSubCode = BoardAT24CXX_ReadPage(addr,readBufPtr,firstPageRemain);
                if(errorSubCode != ERROR_SUB_OK)
                {
                    //完成,空闲
                    BoardAT24CXX_ReleaseLock();
                    //错误显示
                    MCU_LogShowErrorSubCode(errorSubCode);
                    return errorSubCode;
                }
                //地址和计数增加
                readBufPtr += firstPageRemain;
                byteCount -= firstPageRemain;
                addr += firstPageRemain;
                //没读完的读到下一页
                errorSubCode = BoardAT24CXX_ReadPage(addr,readBufPtr,byteCount);
                if(errorSubCode != ERROR_SUB_OK)
                {
                    //完成,空闲
                    BoardAT24CXX_ReleaseLock();
                    //错误显示
                    MCU_LogShowErrorSubCode(errorSubCode);
                    return errorSubCode;
                }
            }
            else
            {
                errorSubCode = BoardAT24CXX_ReadPage(addr,readBufPtr,byteCount);
                if(errorSubCode != ERROR_SUB_OK)
                {
                    //完成,空闲
                    BoardAT24CXX_ReleaseLock();
                    //错误显示
                    MCU_LogShowErrorSubCode(errorSubCode);
                    return errorSubCode;
                }
            }
        }
        //存在多页面
        else
        {
            byteCount -= firstPageRemain;
            numOfPage =  byteCount / PAGE_SIZE_AT24CXX_USE;
            numOfSingle = byteCount % PAGE_SIZE_AT24CXX_USE;	
            //第一页
            if(firstPageRemain != 0)
            {  
                errorSubCode = BoardAT24CXX_ReadPage(addr,readBufPtr, firstPageRemain);
                if(errorSubCode != ERROR_SUB_OK)
                {
                    //完成,空闲
                    BoardAT24CXX_ReleaseLock();
                    //错误显示
                    MCU_LogShowErrorSubCode(errorSubCode);
                    return errorSubCode;
                }
                addr += firstPageRemain;
                readBufPtr += firstPageRemain;
            } 
            //后续整页
            while(numOfPage--)
            {
                errorSubCode = BoardAT24CXX_ReadPage(addr,readBufPtr, PAGE_SIZE_AT24CXX_USE);
                if(errorSubCode != ERROR_SUB_OK)
                {
                    //完成,空闲
                    BoardAT24CXX_ReleaseLock();
                    //错误显示
                    MCU_LogShowErrorSubCode(errorSubCode);
                    return errorSubCode;
                }
                addr +=  PAGE_SIZE_AT24CXX_USE;
                readBufPtr += PAGE_SIZE_AT24CXX_USE;
            }
            //最后一页
            if(numOfSingle != 0)
            {
                errorSubCode = BoardAT24CXX_ReadPage(addr,readBufPtr, numOfSingle);
                if(errorSubCode != ERROR_SUB_OK)
                {
                    //完成,空闲
                    BoardAT24CXX_ReleaseLock();
                    //错误显示
                    MCU_LogShowErrorSubCode(errorSubCode);
                    return errorSubCode;
                }
            }
        }
    } 
    //完成,空闲
    BoardAT24CXX_ReleaseLock();
    return ERROR_SUB_OK; 
}

//自检
ERROR_SUB BoardAT24CXX_Check(void)
{
    ERROR_SUB errorSub = ERROR_SUB_OK;
    uint32_t srcDat = 0;
    uint32_t writeDat = MCU_RandomGetNextUnsigned(0,0XFFFFFFFF);
    uint32_t readDat = 0;
    uint16_t testAddr = MCU_RandomGetNextUnsigned(0,BYTE_SIZE_AT24CXX_USE);
    //自检开始
    MCU_LogPrintf("%s Start,Addr: 0X%08X,WriteData: 0X%08X\r\n",__func__,testAddr,writeDat);
    //申请设备互斥信号量
    BoardAT24CXX_GetLock();
    //读取源
    errorSub = BoardAT24CXX_ReadMultiByte(testAddr,(uint8_t*)(&srcDat),4);
    if(errorSub != ERROR_SUB_OK)
    {
        //释放信号量
        BoardAT24CXX_ReleaseLock();
        //错误日志打印
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //写入新的数据
    errorSub = BoardAT24CXX_WriteMultiByte(testAddr,(uint8_t*)(&writeDat),4);
    if(errorSub != ERROR_SUB_OK)
    {
        //释放信号量
        BoardAT24CXX_ReleaseLock();
        //错误日志打印
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //读取出来
    errorSub = BoardAT24CXX_ReadMultiByte(testAddr,(uint8_t*)(&readDat),4);
    if(errorSub != ERROR_SUB_OK)
    {
        //释放信号量
        BoardAT24CXX_ReleaseLock();
        //错误日志打印
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //比较数据
    if(readDat != writeDat)
    {
        //释放信号量
        BoardAT24CXX_ReleaseLock();
        //错误日志打印
        MCU_LogShowErrorSubCode(errorSub);
        return ERROR_SUB_BOARD_AT24CXX_CHECK_FRIST;
    }
    //原始数据写回去
    errorSub = BoardAT24CXX_WriteMultiByte(testAddr,(uint8_t*)(&srcDat),4);
    if(errorSub != ERROR_SUB_OK)
    {
        //释放信号量
        BoardAT24CXX_ReleaseLock();
        //错误日志打印
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //设置异常值
    readDat = 0XAA5555AA;
    //读取出来
    errorSub = BoardAT24CXX_ReadMultiByte(testAddr,(uint8_t*)(&readDat),4);
    if(errorSub != ERROR_SUB_OK)
    {
        //释放信号量
        BoardAT24CXX_ReleaseLock();
        //错误日志打印
        MCU_LogShowErrorSubCode(errorSub);
        return errorSub;
    }
    //比较数据
    if(readDat != srcDat)
    {
        //释放信号量
        BoardAT24CXX_ReleaseLock();
        //错误日志打印
        MCU_LogShowErrorSubCode(errorSub);
        return ERROR_SUB_BOARD_AT24CXX_CHECK_SECOND;
    }
    //释放信号量
    BoardAT24CXX_ReleaseLock();
    //成功
    MCU_LogPrintf("%s Success Over\r\n",__func__);
    return ERROR_SUB_OK;
}

static uint8_t boardAT24CXX_EraseChipUtilArray[PAGE_SIZE_AT24CXX_USE] = {0};
//全片擦除
ERROR_SUB BoardAT24CXX_ChipErase(void)
{
    uint16_t indexPage = 0;
    uint16_t indexByte = 0;
    ERROR_SUB errorSub = ERROR_SUB_OK;
    MCU_LogPrintf("%s Start\r\n",__func__);
    for(indexPage = 0; indexPage < (BYTE_SIZE_AT24CXX_USE/PAGE_SIZE_AT24CXX_USE);indexPage++)
    {
        //擦除设置为BOARD_AT24CXX_ERASE_CHIP_INIT_VAL
        UserMemSet((void*)boardAT24CXX_EraseChipUtilArray,BOARD_AT24CXX_ERASE_CHIP_INIT_VAL,PAGE_SIZE_AT24CXX_USE);
        //读取指定块
        errorSub = BoardAT24CXX_ReadMultiByte(indexPage*PAGE_SIZE_AT24CXX_USE,(uint8_t*)(boardAT24CXX_EraseChipUtilArray),PAGE_SIZE_AT24CXX_USE);
        if(errorSub != ERROR_SUB_OK)
        {
            return errorSub;
        }
        //数据比对
        for(indexByte = 0;indexByte < PAGE_SIZE_AT24CXX_USE;indexByte++)
        {
            if(boardAT24CXX_EraseChipUtilArray[indexByte] != BOARD_AT24CXX_ERASE_CHIP_INIT_VAL)
            {
                break;
            }
        }
        if(indexByte == PAGE_SIZE_AT24CXX_USE)
        {
            //不需要擦除
            continue;
        }
        //写入指定块
        errorSub = BoardAT24CXX_WriteMultiByte(indexPage*PAGE_SIZE_AT24CXX_USE,(uint8_t*)(boardAT24CXX_EraseChipUtilArray),PAGE_SIZE_AT24CXX_USE);
        if(errorSub != ERROR_SUB_OK)
        {
            return errorSub;
        }
        //写入完成,清除数组
        UserMemClear((void*)boardAT24CXX_EraseChipUtilArray,PAGE_SIZE_AT24CXX_USE);
        //读取指定块
        errorSub = BoardAT24CXX_ReadMultiByte(indexPage*PAGE_SIZE_AT24CXX_USE,(uint8_t*)(boardAT24CXX_EraseChipUtilArray),PAGE_SIZE_AT24CXX_USE);
        if(errorSub != ERROR_SUB_OK)
        {
            return errorSub;
        }
        //数据比对
        for(indexByte = 0;indexByte < PAGE_SIZE_AT24CXX_USE;indexByte++)
        {
            if(boardAT24CXX_EraseChipUtilArray[indexByte] != BOARD_AT24CXX_ERASE_CHIP_INIT_VAL)
            {
                return ERROR_SUB_BOARD_AT24CXX_CHIP_ERASE;
            }
        }
    }
    //成功
    MCU_LogPrintf("%s Success Over\r\n",__func__);
    return ERROR_SUB_OK;
}










