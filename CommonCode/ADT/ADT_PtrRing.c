/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-03-06 14:43:40 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-03-07 10:19:55 +0800
************************************************************************************************/ 
#include "ADT_PtrRing.h"

//宏定义结构体类型,这种定义的好处是不用出现大量的各种类型名,代码整洁
//理解上差点意思,但是使用者不需要太了解内部结构
#define ADT_T                           ADT_PTR_RING_T
#define ADT_NODE_T                      ADT_PTR_RING_NODE_T
//结构体的类型ID,用于程序运行时的检查
#define ADT_PTR_RING_T_ID               0X2020071A

//节点讯息
struct ADT_NODE_T
{
    void* nodeData;//节点保存的数据
    struct ADT_NODE_T* leftNodePtr;//上一个节点
    struct ADT_NODE_T* rightNodePtr;//下一个节点
};

//结构体定义
struct ADT_T 
{
    //函数指针
    ADT_MemMallocFuncPtr memMallocFunc;
    ADT_MemFreeFuncPtr memFreeFunc;
    ADT_LockSystem sysLockFunc;
    ADT_UnLockSystem sysUnlockFunc;
    ADT_SendString sendStrFunc;
    //识别ID
    uint32_t adtTypeID;
    uint32_t currentSize;       //当前有效数据长度
    //节点数据定义
    struct ADT_NODE_T* headNodePtr;
};

//循环指针链表初始化
ADT_T ADT_PtrRingCreate(ADT_MemMallocFuncPtr memMallocFunc,ADT_MemFreeFuncPtr memFreeFunc,
                                    ADT_LockSystem lockSysFuncPtr,ADT_UnLockSystem unlockSysFuncPtr,ADT_SendString sendStrFunc)
{
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //回调函数检查
    ADT_CALL_BACK_CHECK(ADT_PtrRingCreate,memMallocFunc,memFreeFunc,lockSysFuncPtr,unlockSysFuncPtr,sendStrFunc);
    //生成变量
    ADT_T ptrRingPtr = NULL;
    //内存申请
    ADT_MALLOC_SELF(ptrRingPtr,memMallocFunc);
    //函数映射
    ADT_CALL_BACK_REGISTER(ptrRingPtr,memMallocFunc,memFreeFunc,lockSysFuncPtr,unlockSysFuncPtr,sendStrFunc);
    //系统锁定
    ADT_SYS_LOCK(ptrRingPtr);
    //数据复位
    ptrRingPtr->currentSize = 0;
    //创建头节点
    ptrRingPtr->headNodePtr = NULL;
    //功能辅助
    ADT_SET_IDENTIFY_CODE_SET(ptrRingPtr,ADT_PTR_RING_T_ID);
    //系统解锁
    ADT_SYS_UNLOCK(ptrRingPtr);
    //返回结构体指针
    return ptrRingPtr;
}

//循环指针链表检查链表是否为空
ADT_BOOL ADT_PtrRingIsEmpty(ADT_T ptrRingPtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(ptrRingPtr,ADT_PtrRingIsEmpty);
    //ID检查
    ADT_BASE_ID_CHECK(ptrRingPtr,ADT_PtrRingIsEmpty,ADT_PTR_RING_T_ID);
    //数据检查
    if(ptrRingPtr->currentSize == 0)
    {
        return ADT_BOOL_TRUE;
    }
    else
    {
        return ADT_BOOL_FALSE;
    }
}

//循环指针链表获取链表元素个数
uint32_t ADT_PtrRingGetNodeCount(ADT_T ptrRingPtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(ptrRingPtr,ADT_PtrRingGetNodeCount);
    //ID检查
    ADT_BASE_ID_CHECK(ptrRingPtr,ADT_PtrRingGetNodeCount,ADT_PTR_RING_T_ID);
    //返回当前尺寸
    return ptrRingPtr->currentSize;
}

//循环指针链表清空
void ADT_PtrRingClear(ADT_T ptrRingPtr)
{
    ADT_NODE_T p, q;
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(ptrRingPtr,ADT_PtrRingClear);
    if(ptrRingPtr->currentSize == 0)
    {
        //空链表无操作
        return;
    }
    //ID检查
    ADT_BASE_ID_CHECK(ptrRingPtr,ADT_PtrRingClear,ADT_PTR_RING_T_ID);
    //系统锁定
    ADT_SYS_LOCK(ptrRingPtr);
    p = ptrRingPtr->headNodePtr;
    if (p != NULL) 
    {
        int n = ptrRingPtr->currentSize;
        for (; n--; p = q) 
        {
            q = p->rightNodePtr;
            ADT_MEM_FREE(ptrRingPtr,p);
        }
    }
    //初始化为空节点
    ptrRingPtr->headNodePtr = NULL;
    //节点数量设置为0
    ptrRingPtr->currentSize = 0;
    //系统解锁
    ADT_SYS_UNLOCK(ptrRingPtr);
    return;
}

//循环指针链表销毁
void ADT_PtrRingDestroy(ADT_T* ptrRingPtrPtr)
{
    //指针检查
    ADT_BASE_PTR_CHECK(ptrRingPtrPtr,ADT_PtrRingDestroy);
    //定义实体
    ADT_T ptrRingPtr = (*ptrRingPtrPtr);
    //清空数据
    ADT_PtrRingClear(ptrRingPtr);
    //释放自身
    ADT_FREE_SELF(ptrRingPtr);
    return;
}


//获取指定节点元素
void* ADT_PtrRingGetIndex(ADT_T ptrRingPtr,uint32_t index,ADT_RESULT* resultPtr)
{
    ADT_NODE_T q;
    uint32_t findLoopCount = 0;
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(ptrRingPtr,ADT_PtrRingTraverse);
    //ID检查
    ADT_BASE_ID_CHECK(ptrRingPtr,ADT_PtrRingTraverse,ADT_PTR_RING_T_ID);
    //系统锁定
    ADT_SYS_LOCK(ptrRingPtr);
    //没那么多元素
    if(index >= ptrRingPtr->currentSize)
    {
        //系统解锁
        ADT_SYS_UNLOCK(ptrRingPtr);
        *resultPtr = ADT_RESULT_FAIL;
        return NULL;
    }
    //不为空才能执行
    if(ptrRingPtr->currentSize == 0)
    {
        //系统解锁
        ADT_SYS_UNLOCK(ptrRingPtr);
        *resultPtr = ADT_RESULT_FAIL;
        return NULL;
    }
    //获取头节点
    q = ptrRingPtr->headNodePtr;
    //序号小于一半的尺寸
    if (index <= ptrRingPtr->currentSize/2) 
    {
        //往右边
        for (findLoopCount = index; findLoopCount-- > 0;)
        {
            q = q->rightNodePtr;
        }
    } 
    else 
    {
        //序号大于一半的尺寸,往左边
        for (findLoopCount = ptrRingPtr->currentSize - index; findLoopCount-- > 0;)
        {
            q = q->leftNodePtr;
        }
    }
    //系统解锁
    ADT_SYS_UNLOCK(ptrRingPtr);
    *resultPtr = ADT_RESULT_OK;
    //节点数据返回
    return q->nodeData;
}

//循环链表修改指定位置节点数据
void* ADT_PtrRingPutIndex(ADT_T ptrRingPtr,uint32_t index,void* element,ADT_RESULT* resultPtr)
{
    ADT_NODE_T q;
    void *prev;
    uint32_t findLoopCount = 0;
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(ptrRingPtr,ADT_PtrRingTraverse);
    //ID检查
    ADT_BASE_ID_CHECK(ptrRingPtr,ADT_PtrRingTraverse,ADT_PTR_RING_T_ID);
    //限制空节点
    if(element == NULL)
    {
        *resultPtr = ADT_RESULT_FAIL;
        return NULL;
    }
    //系统锁定
    ADT_SYS_LOCK(ptrRingPtr);
    //没那么多元素
    if(index >= ptrRingPtr->currentSize)
    {
        //系统解锁
        ADT_SYS_UNLOCK(ptrRingPtr);
        *resultPtr = ADT_RESULT_FAIL;
        return NULL;
    }
    //不为空才能执行
    if(ptrRingPtr->currentSize == 0)
    {
        //系统解锁
        ADT_SYS_UNLOCK(ptrRingPtr);
        *resultPtr = ADT_RESULT_FAIL;
        return NULL;
    }
    //头节点获取
    q = ptrRingPtr->headNodePtr;
    if (index <= ptrRingPtr->currentSize/2) 
    {
        //序号小于一半的尺寸,往右边
        for (findLoopCount = index; findLoopCount-- > 0;)
        {
            q = q->rightNodePtr;
        }
    } 
    else 
    {
        //序号大于一半的尺寸,往左边
        for (findLoopCount = ptrRingPtr->currentSize - index; findLoopCount-- > 0;)
        {
            q = q->leftNodePtr;
        }
            
    }
    //取出原来的数据
    prev = q->nodeData;
    //写入新的数据
    q->nodeData = element;
    //系统解锁
    ADT_SYS_UNLOCK(ptrRingPtr);
    *resultPtr = ADT_RESULT_OK;
    return prev;
}

//循环指针链表插入元素到最高位
ADT_RESULT ADT_PtrRingAddHi(ADT_T ptrRingPtr,void* element)
{
    ADT_NODE_T q;
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(ptrRingPtr,ADT_PtrRingTraverse);
    //ID检查
    ADT_BASE_ID_CHECK(ptrRingPtr,ADT_PtrRingTraverse,ADT_PTR_RING_T_ID);
    //限制空节点
    if(element == NULL)
    {
        return ADT_RESULT_FAIL;
    }
    //系统锁定
    ADT_SYS_LOCK(ptrRingPtr);
    //生成新的节点
    ADT_NODE_T newNodePtr = ADT_MEM_MALLOC(ptrRingPtr,sizeof(*newNodePtr));
    q = ptrRingPtr->headNodePtr;
    if (q != NULL) 
    {
        //新的节点取代头节点
        //新节点的上一个节点指向头节点的上一个节点
        newNodePtr->leftNodePtr = q->leftNodePtr;
        //新节点的下一个节点指向头节点
        newNodePtr->rightNodePtr = q;
        //头节点的下一个节点的上一个节点指向新节点
        q->leftNodePtr->rightNodePtr = newNodePtr;
        //头节点的上一个节点指向新节点
        q->leftNodePtr = newNodePtr;
    } 
    else 
    {
        //新的节点就是头节点
        newNodePtr->rightNodePtr = newNodePtr;
        newNodePtr->leftNodePtr = newNodePtr;
        ptrRingPtr->headNodePtr = newNodePtr;
    }
    ptrRingPtr->currentSize++;
    newNodePtr->nodeData = element;
    //系统解锁
    ADT_SYS_UNLOCK(ptrRingPtr);
    return ADT_RESULT_OK;
}

//循环指针链表插入元素到最低位
ADT_RESULT ADT_PtrRingAddLo(ADT_T ptrRingPtr,void* element)
{
    ADT_NODE_T q;
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(ptrRingPtr,ADT_PtrRingTraverse);
    //ID检查
    ADT_BASE_ID_CHECK(ptrRingPtr,ADT_PtrRingTraverse,ADT_PTR_RING_T_ID);
    //限制空节点
    if(element == NULL)
    {
        return ADT_RESULT_FAIL;
    }
    //系统锁定
    ADT_SYS_LOCK(ptrRingPtr);
    //生成新的节点
    ADT_NODE_T newNodePtr = ADT_MEM_MALLOC(ptrRingPtr,sizeof(*newNodePtr));
    q = ptrRingPtr->headNodePtr;
    if (q != NULL) 
    {
        //新的节点取代头节点
        //新节点的上一个节点指向头节点的上一个节点
        newNodePtr->leftNodePtr = q->leftNodePtr;
        //新节点的下一个节点指向头节点
        newNodePtr->rightNodePtr = q;
        //头节点的下一个节点的上一个节点指向新节点
        q->leftNodePtr->rightNodePtr = newNodePtr;
        //头节点的上一个节点指向新节点
        q->leftNodePtr = newNodePtr;
    } 
    else 
    {
        //新的节点就是头节点
        newNodePtr->rightNodePtr = newNodePtr;
        newNodePtr->leftNodePtr = newNodePtr;
        ptrRingPtr->headNodePtr = newNodePtr;
    }
    ptrRingPtr->currentSize++;
    newNodePtr->nodeData = element;
    //头部向左移动一格
    ptrRingPtr->headNodePtr = ptrRingPtr->headNodePtr->leftNodePtr;
    //系统解锁
    ADT_SYS_UNLOCK(ptrRingPtr);
    return ADT_RESULT_OK;
}

//循环指针链表插入元素到指定节点后方
ADT_RESULT ADT_PtrRingAddIndex(ADT_T ptrRingPtr,int32_t index,void* element)
{
    ADT_RESULT adtResult = ADT_RESULT_OK;
    int32_t indexMin = 0 - ptrRingPtr->currentSize;
    int32_t indexMax = ptrRingPtr->currentSize + 1;
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(ptrRingPtr,ADT_PtrRingTraverse);
    //ID检查
    ADT_BASE_ID_CHECK(ptrRingPtr,ADT_PtrRingTraverse,ADT_PTR_RING_T_ID);
    //限制空节点
    if(element == NULL)
    {
        return ADT_RESULT_FAIL;
    }
    //限制位置
    if((index < indexMin)||(index > indexMax))
    {
        return ADT_RESULT_FAIL;
    }
    //系统锁定
    ADT_SYS_LOCK(ptrRingPtr);
    if (index == 1 || index == indexMin)
    {
        adtResult = ADT_PtrRingAddLo(ptrRingPtr, element);
        //系统解锁
        ADT_SYS_UNLOCK(ptrRingPtr);
        return adtResult;
    }
    else if ((index == 0) || (index == indexMax))
    {
        adtResult = ADT_PtrRingAddHi(ptrRingPtr, element);
        //系统解锁
        ADT_SYS_UNLOCK(ptrRingPtr);
        return adtResult;
    }
    else 
    {
        ADT_NODE_T p, q;
        int indexUtil = (index < 0)?(index + ptrRingPtr->currentSize):(index - 1);
        int findLoop = 0;
        //先得到头节点
        q = ptrRingPtr->headNodePtr;
        if (indexUtil <= ptrRingPtr->currentSize/2) 
        {
            for (findLoop = indexUtil; findLoop-- > 0;)
            {
                q = q->rightNodePtr;
            }
                
        } 
        else 
        {
            for (findLoop = ptrRingPtr->currentSize - indexUtil; findLoop-- > 0;)
            {
                q = q->leftNodePtr;
            }   
        }
        //找到了插入节点,这个时候是Q
        //生成新的节点
        p = ADT_MEM_MALLOC(ptrRingPtr,sizeof(*p));
        p->nodeData = element;
        //插入到当前节点Q的左边
        //新节点的左节点是原先节点的左节点
        p->leftNodePtr = q->leftNodePtr;
        //新节点的右节点是原先节点
        p->rightNodePtr = q;
        //原先节点的左节点的右节点成为新节点
        q->leftNodePtr->rightNodePtr = p;
        //原先节点的左节点成为新节点
        q->leftNodePtr = p;
        //节点数量增加
        ptrRingPtr->currentSize++;
        //系统解锁
        ADT_SYS_UNLOCK(ptrRingPtr);
        return ADT_RESULT_OK;
    }
}

//弹出指定位置元素
void* ADT_PtrRingRemoveIndex(ADT_T ptrRingPtr,uint32_t index)
{
    void *x = NULL;
    ADT_NODE_T q;
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(ptrRingPtr,ADT_PtrRingTraverse);
    //ID检查
    ADT_BASE_ID_CHECK(ptrRingPtr,ADT_PtrRingTraverse,ADT_PTR_RING_T_ID);
    //限制空节点
    if(ptrRingPtr->currentSize == 0)
    {
        return NULL;
    }
    //限制位置
    if(index >= ptrRingPtr->currentSize)
    {
        return NULL;
    }
    //系统锁定
    ADT_SYS_LOCK(ptrRingPtr);
    int findLoop;
    //获取头节点
    q = ptrRingPtr->headNodePtr;
    if (index <= ptrRingPtr->currentSize/2) 
    {
        for (findLoop = index; findLoop-- > 0;)
        {
            q = q->rightNodePtr;
        }   
    } 
    else 
    {
        for (findLoop = ptrRingPtr->currentSize - index; findLoop-- > 0;)
        {
            q = q->leftNodePtr;
        }
            
    }
    if (index == 0)
    {
        //插入0位置,头节点右移
        ptrRingPtr->headNodePtr = ptrRingPtr->headNodePtr->rightNodePtr;
    }
    //获取值
    x = q->nodeData;
    //删除节点
    q->leftNodePtr->rightNodePtr = q->rightNodePtr;
    q->rightNodePtr->leftNodePtr = q->leftNodePtr;
    //释放当前节点
    ADT_MEM_FREE(ptrRingPtr,q);
    //移除之后长度如果为0,则设置头节点为空节点
    ptrRingPtr->currentSize--;
    if (ptrRingPtr->currentSize == 0)
    {
        ptrRingPtr->headNodePtr = NULL;
    }
    //系统解锁
    ADT_SYS_UNLOCK(ptrRingPtr);
    return x;
}

//顶部弹出一个元素
void* ADT_PtrRingRemoveHi(ADT_T ptrRingPtr)
{
    void *x = NULL;
    ADT_NODE_T q;
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(ptrRingPtr,ADT_PtrRingTraverse);
    //ID检查
    ADT_BASE_ID_CHECK(ptrRingPtr,ADT_PtrRingTraverse,ADT_PTR_RING_T_ID);
    //限制空节点
    if(ptrRingPtr->currentSize == 0)
    {
        return NULL;
    }
    //系统锁定
    ADT_SYS_LOCK(ptrRingPtr);
    q = ptrRingPtr->headNodePtr->leftNodePtr;
    x = q->nodeData;
    //删除节点
    q->leftNodePtr->rightNodePtr = q->rightNodePtr;
    q->rightNodePtr->leftNodePtr = q->leftNodePtr;
    //释放元素
    ADT_MEM_FREE(ptrRingPtr,q);
    //移除之后长度如果为0,则设置头节点为空节点
    ptrRingPtr->currentSize--;
    if (ptrRingPtr->currentSize == 0)
    {
        ptrRingPtr->headNodePtr = NULL;
    }
    //系统解锁
    ADT_SYS_UNLOCK(ptrRingPtr);
    return x;
}

//底部弹出一个元素
void* ADT_PtrRingRemoveLo(ADT_T ptrRingPtr)
{
    void *x = NULL;
    ADT_NODE_T q;
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(ptrRingPtr,ADT_PtrRingTraverse);
    //ID检查
    ADT_BASE_ID_CHECK(ptrRingPtr,ADT_PtrRingTraverse,ADT_PTR_RING_T_ID);
    //限制空节点
    if(ptrRingPtr->currentSize == 0)
    {
        return NULL;
    }
    //系统锁定
    ADT_SYS_LOCK(ptrRingPtr);
    //头节点先右移
    ptrRingPtr->headNodePtr = ptrRingPtr->headNodePtr->rightNodePtr;
    //获取顶部节点
    q = ptrRingPtr->headNodePtr->leftNodePtr;
    x = q->nodeData;
    //删除节点
    q->leftNodePtr->rightNodePtr = q->rightNodePtr;
    q->rightNodePtr->leftNodePtr = q->leftNodePtr;
    //释放元素
    ADT_MEM_FREE(ptrRingPtr,q);
    //移除之后长度如果为0,则设置头节点为空节点
    ptrRingPtr->currentSize--;
    if (ptrRingPtr->currentSize == 0)
    {
        ptrRingPtr->headNodePtr = NULL;
    }
    //系统解锁
    ADT_SYS_UNLOCK(ptrRingPtr);
    return x;
}

//环移动指定元素
ADT_RESULT ADT_PtrRingRotate(ADT_T ptrRingPtr,int32_t rotatePos)
{
    ADT_NODE_T q;
    int32_t indexUtil = 0;
    int32_t rotatePosMin = 0 - (int32_t)(ptrRingPtr->currentSize);
    int32_t rotatePosMax = (int32_t)(ptrRingPtr->currentSize);
    //ADT系统锁定前置
    ADT_SYS_LOCK_ALLOC(lastState);
    //指针检查
    ADT_BASE_PTR_CHECK(ptrRingPtr,ADT_PtrRingTraverse);
    //ID检查
    ADT_BASE_ID_CHECK(ptrRingPtr,ADT_PtrRingTraverse,ADT_PTR_RING_T_ID);
    //限制空节点
    if(ptrRingPtr->currentSize == 0)
    {
        return ADT_RESULT_FAIL;
    }
    //检查旋转
    if((rotatePos < rotatePosMin)||(rotatePos > rotatePosMax))
    {
        return ADT_RESULT_FAIL;
    }
    //系统锁定
    ADT_SYS_LOCK(ptrRingPtr);
    if (rotatePos >= 0)
    {
        indexUtil = rotatePos % ptrRingPtr->currentSize;
    }
    else
    {
        indexUtil = rotatePos + ptrRingPtr->currentSize;
    }
    int findLoop;
    q = ptrRingPtr->headNodePtr;
    if (indexUtil <= ptrRingPtr->currentSize/2) 
    {
        for (findLoop = indexUtil; findLoop-- > 0;)
        {
            q = q->rightNodePtr;
        }
            
    } 
    else 
    {
        for (findLoop = ptrRingPtr->currentSize - indexUtil; findLoop-- > 0;)
        {
            q = q->leftNodePtr;
        } 
    }
    //变化头节点
    ptrRingPtr->headNodePtr = q;
    //系统解锁
    ADT_SYS_UNLOCK(ptrRingPtr);
    return ADT_RESULT_OK;
}

