/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-10-16 17:20:56 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-10-16 17:21:17 +0800
************************************************************************************************/ 
#include "MCU_HSEM.h"
#include "DriverConfigMCU.h"

//注册的通道释放回调函数
static MCU_HSEM_SemChannelFreeCallBack hsemFreeCallBackPtrArray[HSEM_SEMID_MAX+1] = {NULL};
//HSEM初始化标记
static uint8_t hsemInitFlag = 0;

//硬件信号量复位
void MCU_HSEM_DeInit(void)
{
    hsemInitFlag = 0;
    //关闭时钟
    __HAL_RCC_HSEM_CLK_DISABLE();
    //先关闭中断
    HAL_NVIC_DisableIRQ(HSEM1_IRQn);
    //清空回调函数讯息
    uint8_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < DIM_ARRAY_ELEMENT_COUNT(hsemFreeCallBackPtrArray); indexUtil++)
    {
        hsemFreeCallBackPtrArray[indexUtil] = NULL;
    }
    //释放全部的信号量
    MCU_HSEM_UnLockAllSelfCore();
}

//初始化硬件信号量
void MCU_HSEM_Init(void)
{
    if(hsemInitFlag == 1)
    {
        return;
    }
    hsemInitFlag = 1;
    //使能HSEM时钟
    __HAL_RCC_HSEM_CLK_ENABLE();
    //将外设分配给C2
    __HAL_RCC_C2_HSEM_CLK_ENABLE();
    //先关闭中断
    HAL_NVIC_DisableIRQ(HSEM1_IRQn);
    //读取当前系统clear key
    uint16_t clearKeyLocal = HAL_HSEM_GetClearKey();
    if(clearKeyLocal != MCU_HSEM_CLEAR_KEY_M7)
    {
        //设置CLEAR KEY
        HAL_HSEM_SetClearKey((uint32_t)MCU_HSEM_CLEAR_KEY_M7);
    }
    //清空回调函数讯息
    uint8_t indexUtil = 0;
    for(indexUtil = 0; indexUtil < DIM_ARRAY_ELEMENT_COUNT(hsemFreeCallBackPtrArray); indexUtil++)
    {
        hsemFreeCallBackPtrArray[indexUtil] = NULL;
    }
    //设置中断优先级
    HAL_NVIC_SetPriority(HSEM1_IRQn, MCU_HSEM_PREE_PRI, MCU_HSEM_SUB_PRI);
    //打开中断
    HAL_NVIC_EnableIRQ(HSEM1_IRQn);
}


//信号量上锁
MCU_HSEM_RESULT MCU_HSEM_LockDetail(MCU_HSEM_SEM_ID semID,uint8_t processID)
{
    HAL_StatusTypeDef halState = HAL_OK;
    //加锁进入原子操作
    uint32_t lastIntState = CoreLockSchedule();
    //调用加锁API
    halState = HAL_HSEM_Take((uint32_t)semID,(uint32_t)processID);
    if(halState != HAL_OK)
    {
        //解除锁定
        CoreUnlockSchedule(lastIntState);
        //上锁失败
        return MCU_HSEM_RESULT_FAIL;
    }
    //解除锁定
    CoreUnlockSchedule(lastIntState);
    //上锁成功
    return MCU_HSEM_RESULT_OK;
}

//信号量上锁,线程ID默认设置为0
MCU_HSEM_RESULT MCU_HSEM_LockSample(MCU_HSEM_SEM_ID semID)
{
    HAL_StatusTypeDef halState = HAL_OK;
    //加锁进入原子操作
    uint32_t lastIntState = CoreLockSchedule();
    //调用加锁API
    halState = HAL_HSEM_FastTake((uint32_t)semID);
    if(halState != HAL_OK)
    {
        //解除锁定
        CoreUnlockSchedule(lastIntState);
        //上锁失败
        return MCU_HSEM_RESULT_FAIL;
    }
    //解除锁定
    CoreUnlockSchedule(lastIntState);
    //上锁成功
    return MCU_HSEM_RESULT_OK;
}

//信号量解锁,可以设置解锁线程ID
MCU_HSEM_RESULT MCU_HSEM_UnLockDetail(MCU_HSEM_SEM_ID semID,uint8_t processID)
{
    //解锁进入原子操作
    uint32_t lastIntState = CoreLockSchedule();
    //调用解锁API
    HAL_HSEM_Release((uint32_t)semID,processID);
    //解锁完成检查是否成功
    uint32_t unlockResult = 0;
    unlockResult = HAL_HSEM_IsSemTaken((uint32_t)semID);
    if(unlockResult == 1)
    {
        //解除锁定
        CoreUnlockSchedule(lastIntState);
        //解锁失败
        return MCU_HSEM_RESULT_FAIL;
    }
    //解除锁定
    CoreUnlockSchedule(lastIntState);
    //解锁成功
    return MCU_HSEM_RESULT_OK;
}

//信号量解锁,线程ID默认设置为0
MCU_HSEM_RESULT MCU_HSEM_UnLockSample(MCU_HSEM_SEM_ID semID)
{
    //解锁进入原子操作
    uint32_t lastIntState = CoreLockSchedule();
    //调用解锁API
    HAL_HSEM_Release((uint32_t)semID,0);
    //解锁完成检查是否成功
    uint32_t unlockResult = 0;
    unlockResult = HAL_HSEM_IsSemTaken((uint32_t)semID);
    if(unlockResult == 1)
    {
        //解除锁定
        CoreUnlockSchedule(lastIntState);
        //解锁失败
        return MCU_HSEM_RESULT_FAIL;
    }
    //解除锁定
    CoreUnlockSchedule(lastIntState);
    //解锁成功
    return MCU_HSEM_RESULT_OK;
}

//清空全部的信号量
void MCU_HSEM_UnLockAllSelfCore(void)
{
    //解锁进入原子操作
    uint32_t lastIntState = CoreLockSchedule();
    //调用API
    HAL_HSEM_ReleaseAll(HAL_HSEM_GetClearKey(),HSEM_CPU1_COREID);
    //解除锁定
    CoreUnlockSchedule(lastIntState);
}

//查询信号量当前状态
MCU_HSEM_STATE MCU_HSEM_QueryState(MCU_HSEM_SEM_ID semID)
{
    //进入原子操作
    uint32_t lastIntState = CoreLockSchedule();
    uint32_t unlockResult = 0;
    unlockResult = HAL_HSEM_IsSemTaken((uint32_t)semID);
    if(unlockResult == 1)
    {
        //解除锁定
        CoreUnlockSchedule(lastIntState);
        //解锁失败
        return MCU_HSEM_STATE_LOCK;
    }
    //解除锁定
    CoreUnlockSchedule(lastIntState);
    //解锁成功
    return MCU_HSEM_STATE_UNLOCK;
}

//注册释放事件
void MCU_HSEM_RegisterEventCallBack(MCU_HSEM_SEM_ID semID,MCU_HSEM_SemChannelFreeCallBack callBackFunc)
{
    uint32_t channelMaskID = 0;
    channelMaskID = (0X00000001<<((uint32_t)(semID)));
    //进入原子操作
    uint32_t lastIntState = CoreLockSchedule();
    //首先关闭
    HAL_HSEM_DeactivateNotification(channelMaskID);
    //设置回调
    hsemFreeCallBackPtrArray[(uint8_t)semID] = callBackFunc;
    //查看回调函数是否打开
    if(callBackFunc != NULL)
    {
        HAL_HSEM_ActivateNotification(channelMaskID);
    }
    //解除锁定
    CoreUnlockSchedule(lastIntState);
}

//解除注册释放事件
void MCU_HSEM_UnRegisterEventCallBack(MCU_HSEM_SEM_ID semID)
{
    uint32_t channelMaskID = 0;
    channelMaskID = (0X00000001<<((uint32_t)(semID)));
    //进入原子操作
    uint32_t lastIntState = CoreLockSchedule();
    //首先关闭
    HAL_HSEM_DeactivateNotification(channelMaskID);
    //清除回调
    hsemFreeCallBackPtrArray[(uint8_t)semID] = NULL;
    //解除锁定
    CoreUnlockSchedule(lastIntState);
}


//中断回调函数
void HAL_HSEM_FreeCallback(uint32_t SemMask)
{
    //轮询状态码调用对应的回调函数
    uint16_t indexUtil = 0;
    uint32_t channelMaskID = 0;
    for(indexUtil = 0; indexUtil < DIM_ARRAY_ELEMENT_COUNT(hsemFreeCallBackPtrArray); indexUtil++)
    {
        channelMaskID = (0X00000001<<indexUtil);
        if((channelMaskID&SemMask) != 0)
        {
            if(hsemFreeCallBackPtrArray[indexUtil] != NULL)
            {
                //设置了回调函数的,重新启用通知
                HAL_HSEM_ActivateNotification(channelMaskID);
                //调用回调函数,使用计算的通道号
                hsemFreeCallBackPtrArray[indexUtil]();
            }
            else
            {
                //没有回调函数,关闭通知
                HAL_HSEM_DeactivateNotification(channelMaskID);
            }
        }
    }
}

//HSEM信号量释放中断
void HSEM1_IRQHandler(void)
{
    //通知系统中断
    _tx_thread_context_save();
    //调用HAL库中断函数
    HAL_HSEM_IRQHandler();
    //通知系统退出中断
    _tx_thread_context_restore();
}









