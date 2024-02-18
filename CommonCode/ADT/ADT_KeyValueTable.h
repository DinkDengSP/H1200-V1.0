#ifndef __ADT_KEY_VALUE_TABLE_H_
#define __ADT_KEY_VALUE_TABLE_H_
#include "ADT_Base.h"

#define ADT_T ADT_KEY_VALUE_TABLE_T
//指定一个数据类型,****对外部文件ADT_KEY_VALUE_TABLE_T数据类型是一个指向
//****ADT_KEY_VALUE_TABLE_T结构体的指针(注意这句话)

//的指针,不透明指针的实现,需要基础知识如下.
//C语言存在两个命名空间,所有的变量,函数,类型定义,和枚举定义共享一个名称空间
//所有的全局结构,联合和枚举标记则共享另一个名称空间
//typedef struct ADT_T *ADT_T里面的*ADT_T名称空间是类型定义
//struct ADT_T 的名称空间是全局结构,两个名称空间的名字可以一样.
//而预处理并不需要知道ADT_KEY_VALUE_TABLE_T的具体类型,这个具体类型是在链接阶段的时候绑定的
//这样的处理可以实现程序对外部的细节隐藏
typedef struct ADT_T *ADT_T;

//表初始化
ADT_T ADT_KeyValueTableCreate(uint16_t bucketCapacitySet,ADT_MemMallocFuncPtr memMallocFunc,ADT_MemFreeFuncPtr memFreeFunc,
                                ADT_LockSystem lockSysFuncPtr,ADT_UnLockSystem unlockSysFuncPtr,ADT_SendString sendStrFunc,
                                ADT_KeyValueTableCompare compFuncPtr,ADT_KeyValueTableCreateHashCode hashCodeCreateFuncPtr);

//销毁表
void ADT_KeyValueTableDestroy(ADT_T* keyValueTablePtrPtr);

//释放全部的表数据
void ADT_KeyValueTableClear(ADT_T keyValueTablePtr);

//获取当前表中元素数量
uint32_t ADT_KeyValueTableGetElementCount(ADT_T keyValueTablePtr);

//添加一个键值对,如果原来就存在这个键,新的值取代旧的值,旧的值被返回,否则返回空
void* ADT_KeyValueTablePut(ADT_T keyValueTablePtr,const void *key, void *value,ADT_RESULT* putResultPtr);

//获取一个键值对,返回查找值,如果不存在则返回空
void* ADT_KeyValueTableGet(ADT_T keyValueTablePtr,const void *key,ADT_RESULT* putResultPtr);

//删除指定键值对,返回被删除的值,如果不存在则返回空
void* ADT_KeyValueTableRemove(ADT_T keyValueTablePtr,const void *key,ADT_RESULT* putResultPtr);

//链表转换为指针数组
void** ADT_KeyValueTableToArray(ADT_T keyValueTablePtr,uint16_t* elementLength);

//释放指针数组
void ADT_KeyValueTableFreeArray(ADT_T keyValueTablePtr,void* arrayPtr);

//遍历列表
void ADT_KeyValueTableTraverse(ADT_T keyValueTablePtr,void operationFunc(const void* key, void** valuePtr, void *param),
                                    void* param);

#undef ADT_T

#endif



