#ifndef __ADT_PTR_SORT_H_
#define __ADT_PTR_SORT_H_
#include "ADT_Base.h"

//冒泡排序法
ADT_RESULT ADT_PtrBubbleSort(ADT_SortBaseCompare compareFunc,void** ptrArray,uint32_t arrayLength);;

//插入排序法
ADT_RESULT ADT_PtrInsertSort(ADT_SortBaseCompare compareFunc,void** ptrArray,uint32_t arrayLength);

//选择排序法
ADT_RESULT ADT_PtrSelectSort(ADT_SortBaseCompare compareFunc,void** ptrArray,uint32_t arrayLength);;

//希尔排序法
ADT_RESULT ADT_PtrShellSort(ADT_SortBaseCompare compareFunc,void** ptrArray,uint32_t arrayLength);



#endif
