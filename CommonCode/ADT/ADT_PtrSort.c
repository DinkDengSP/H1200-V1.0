/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-03-09 10:43:11 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-03-09 10:43:11 +0800
************************************************************************************************/ 
#include "ADT_PtrSort.h"

//元素交换
static void ADT_PtrBubbleSortSwapElement(void** ptrArray,int pos1,int pos2)
{
    void* ptrElementUtil = NULL;
    ptrElementUtil = ptrArray[pos1];
    ptrArray[pos1] = ptrArray[pos2];
    ptrArray[pos2] = ptrElementUtil;
}

//冒泡排序法
ADT_RESULT ADT_PtrBubbleSort(ADT_SortBaseCompare compareFunc,void** ptrArray,uint32_t arrayLength)
{
    uint32_t indexMain, indexSub;
    if(compareFunc == NULL)
    {
        return ADT_RESULT_FAIL;
    }
    if(ptrArray == NULL)
    {
        return ADT_RESULT_FAIL;
    }
    if(arrayLength == 0)
    {
        return ADT_RESULT_FAIL;
    }
    if(arrayLength == 1)
    {
        return ADT_RESULT_OK;
    }
    for (indexMain = 0; indexMain < arrayLength - 1; indexMain++)
    {
        for (indexSub = indexMain + 1; indexSub < arrayLength; indexSub++)
        {
            if(compareFunc(ptrArray[indexMain],ptrArray[indexSub]) > 0)
            {
                ADT_PtrBubbleSortSwapElement(ptrArray,indexMain,indexSub);
            }
        }
    }
    return ADT_RESULT_OK;
}


//插入排序法
ADT_RESULT ADT_PtrInsertSort(ADT_SortBaseCompare compareFunc,void** ptrArray,uint32_t arrayLength)
{
    int indexMain, indexSub;
    void* tmp;
    if(compareFunc == NULL)
    {
        return ADT_RESULT_FAIL;
    }
    if(ptrArray == NULL)
    {
        return ADT_RESULT_FAIL;
    }
    if(arrayLength == 0)
    {
        return ADT_RESULT_FAIL;
    }
    if(arrayLength == 1)
    {
        return ADT_RESULT_OK;
    }
    for(indexMain = 1; indexMain < arrayLength; indexMain++)
    {
        tmp =  ptrArray[indexMain];
        for (indexSub = indexMain; ((indexSub > 0) && (compareFunc(ptrArray[indexSub - 1],tmp) > 0)); indexSub--)
        {
            ptrArray[indexSub] = ptrArray[indexSub - 1];
        } 
        ptrArray[indexSub] = tmp;
    }
    return ADT_RESULT_OK;
}

//元素交换
static void ADT_PtrSelectSortSwapElement(void** ptrArray,int pos1,int pos2)
{
    void* ptrElementUtil = NULL;
    ptrElementUtil = ptrArray[pos1];
    ptrArray[pos1] = ptrArray[pos2];
    ptrArray[pos2] = ptrElementUtil;
}

//选择排序法
ADT_RESULT ADT_PtrSelectSort(ADT_SortBaseCompare compareFunc,void** ptrArray,uint32_t arrayLength)
{
    void* min;
    int32_t minpos = -1;
    int indexMain, indexSub;
    if(compareFunc == NULL)
    {
        return ADT_RESULT_FAIL;
    }
    if(ptrArray == NULL)
    {
        return ADT_RESULT_FAIL;
    }
    if(arrayLength == 0)
    {
        return ADT_RESULT_FAIL;
    }
    if(arrayLength == 1)
    {
        return ADT_RESULT_OK;
    }
    for (indexMain = 0; indexMain < arrayLength - 1; indexMain++)
    {
        min = ptrArray[indexMain];
        minpos = indexMain;
        for (indexSub = indexMain + 1; indexSub < arrayLength; indexSub++)
        {
            if (compareFunc(ptrArray[indexSub],min) < 0)
            {
                min = ptrArray[indexSub];;
                minpos = indexSub;
            }
        } 
        if (minpos != indexSub)
        {
            ADT_PtrSelectSortSwapElement(ptrArray, indexMain, minpos);
        }
    }
    return ADT_RESULT_OK;
}


static int hibbard[]={1,3,5};
//int hibbard[]={1,3,7,15,31};

//希尔排序法
ADT_RESULT ADT_PtrShellSort(ADT_SortBaseCompare compareFunc,void** ptrArray,uint32_t arrayLength)
{
    int step, stepsp = sizeof(hibbard) / sizeof(int) - 1;
    int indexMain, indexSub;
    void* tmp;
    if(compareFunc == NULL)
    {
        return ADT_RESULT_FAIL;
    }
    if(ptrArray == NULL)
    {
        return ADT_RESULT_FAIL;
    }
    if(arrayLength == 0)
    {
        return ADT_RESULT_FAIL;
    }
    if(arrayLength == 1)
    {
        return ADT_RESULT_OK;
    }
    while (stepsp >= 0)
    {
        step = hibbard[stepsp--];
        for (indexMain = step; indexMain < arrayLength; indexMain++)
        {
            tmp = ptrArray[indexMain];
            for (indexSub = indexMain; ((indexSub >= step)&&(compareFunc(ptrArray[indexSub - step],tmp) > 0)); indexSub -= step)
            {
                ptrArray[indexSub] = ptrArray[indexSub - step];
            }
            ptrArray[indexSub] = tmp;
        }
    }
    return ADT_RESULT_OK;
}
