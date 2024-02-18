/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-02-06 09:53:45 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-02-08 20:33:16 +0800
************************************************************************************************/ 
#ifndef __ADT_INC_H_
#define __ADT_INC_H_

#include "ADT_Base.h"
#include "ADT_CircularByteQueue.h"
#include "ADT_CircularPtrQueue.h"
#include "ADT_PtrStack.h"
#include "ADT_ByteStack.h"



// /*---------------------------ADTByte栈测试-----------------------------------------*/
// //是否使能ADTByte栈测试
// #define SRV_HEART_ADT_BYTE_STACK_TEST_ENABLE                    0
// //ADTByte栈测试演示周期
// #define SRV_HEART_PERIOD_MS_ADT_BYTE_STACK_TEST_SHOW            5000
// /*---------------------------ADTPtr堆栈测试-----------------------------------------*/
// //是否使能ADT_Ptr栈测试
// #define SRV_HEART_ADT_PTR_STACK_TEST_ENABLE                     0
// //ADT_Ptr栈测试演示周期
// #define SRV_HEART_PERIOD_MS_ADT_PTR_STACK_TEST_SHOW             5000


// #if(SRV_HEART_ADT_BYTE_STACK_TEST_ENABLE != 0)
// //上次测试栈存储时间戳
// static int32_t lastHeartAdtByteStackTestTimeTick = 0;
// //测试栈存储
// void SrvImplHeartSubADT_ByteStackTestInit(void)
// {
//     lastHeartAdtByteStackTestTimeTick = tx_time_get();
// }

// //测试栈存储
// void SrvImplHeartSubADT_ByteStackTestRunPeriod()
// {
//     if(tx_time_get() - lastHeartAdtByteStackTestTimeTick < MS_TO_OS_TICK(SRV_HEART_PERIOD_MS_ADT_BYTE_STACK_TEST_SHOW))
//     {
//         return;
//     }
//     //更新时间周期
//     lastHeartAdtByteStackTestTimeTick = tx_time_get();
//     //开始测试
//     ADT_RESULT adtResult = ADT_RESULT_FAIL;
// 	ADT_BYTE_STACK_T adtStackPtr = NULL;
//     adtStackPtr = ADT_ByteStackCreate(20,MemMallocSramIn,MemFreeSramIn,CoreLockIntBasePri,CoreUnlockIntBasePri,MCU_RTT_SendStringInt);
// 	uint8_t stackTestByteArray[]={1,2,3,4,5,6,7,8,9};
// 	uint8_t indexUtil = 0;
//     uint8_t readData = 0;
//     //半深度测试
//     for(indexUtil = 0; indexUtil < DIM_ARRAY_ELEMENT_COUNT(stackTestByteArray)/2;indexUtil++)
//     {
//         ADT_ByteStackPush(adtStackPtr,stackTestByteArray[indexUtil]);
//         SystemPrintf("%s,Push Element %d\r\n",__func__,stackTestByteArray[indexUtil]);
//     }
//     do
//     {
//         adtResult = ADT_ByteStackPop(adtStackPtr,&readData);
//         if(adtResult == ADT_RESULT_OK)
//         {
//             SystemPrintf("%s,Pop Element %d\r\n",__func__,readData);
//         }
//     }while(adtResult == ADT_RESULT_OK);
//     //全深度测试
//     for(indexUtil = 0; indexUtil < DIM_ARRAY_ELEMENT_COUNT(stackTestByteArray);indexUtil++)
//     {
//         ADT_ByteStackPush(adtStackPtr,stackTestByteArray[indexUtil]);
//         SystemPrintf("%s,Push Element %d\r\n",__func__,stackTestByteArray[indexUtil]);
//     }
//     do
//     {
//         adtResult = ADT_ByteStackPop(adtStackPtr,&readData);
//         if(adtResult == ADT_RESULT_OK)
//         {
//             SystemPrintf("%s,Pop Element %d\r\n",__func__,readData);
//         }
//     }while(adtResult == ADT_RESULT_OK);
//     //释放
//     ADT_ByteStackFree(&adtStackPtr);
// }
// #endif


// /*------------------------------------ADT指针栈测试----------------------------------------*/
// #if(SRV_HEART_ADT_PTR_STACK_TEST_ENABLE != 0)
// //上次测试栈存储时间戳
// static int32_t lastHeartAdtPtrStackTestTimeTick = 0;
// //测试数组
// static const uint8_t adtPtrStackTestArray[][25] = 
// {
//     "adtPtrStackTestArray1",
//     "adtPtrStackTestArray2",
//     "adtPtrStackTestArray3",
//     "adtPtrStackTestArray4",
//     "adtPtrStackTestArray5",
//     "adtPtrStackTestArray6",
//     "adtPtrStackTestArray7",
//     "adtPtrStackTestArray8",
//     "adtPtrStackTestArray9",
// };
// #define ADT_PTR_STACK_TEST_ARRAY_LEN            9
// //测试栈存储
// void SrvImplHeartSubADT_PtrStackTestInit(void)
// {
//     lastHeartAdtPtrStackTestTimeTick = tx_time_get();
// }

// //测试栈存储
// void SrvImplHeartSubADT_PtrStackTestRunPeriod()
// {
//     if(tx_time_get() - lastHeartAdtPtrStackTestTimeTick < MS_TO_OS_TICK(SRV_HEART_PERIOD_MS_ADT_PTR_STACK_TEST_SHOW))
//     {
//         return;
//     }
//     //更新时间周期
//     lastHeartAdtPtrStackTestTimeTick = tx_time_get();
//     //开始测试
//     ADT_RESULT adtResult = ADT_RESULT_FAIL;
// 	ADT_PTR_STACK_T adtStackPtr = NULL;
//     adtStackPtr = ADT_PtrStackCreate(20,MemMallocSramIn,MemFreeSramIn,CoreLockIntBasePri,CoreUnlockIntBasePri,MCU_RTT_SendStringInt);
// 	uint8_t indexUtil = 0;
//     void* readDataPtr = 0;
//     //半深度测试
//     for(indexUtil = 0; indexUtil < ADT_PTR_STACK_TEST_ARRAY_LEN/2;indexUtil++)
//     {
//         ADT_PtrStackPush(adtStackPtr,(void*)adtPtrStackTestArray[indexUtil]);
//         SystemPrintf("%s,Push Element %s\r\n",__func__,adtPtrStackTestArray[indexUtil]);
//     }
//     do
//     {
//         adtResult = ADT_PtrStackPop(adtStackPtr,&readDataPtr);
//         if(adtResult == ADT_RESULT_OK)
//         {
//             SystemPrintf("%s,Pop Element %s\r\n",__func__,readDataPtr);
//         }
//     }while(adtResult == ADT_RESULT_OK);
//     //全深度测试
//     for(indexUtil = 0; indexUtil < ADT_PTR_STACK_TEST_ARRAY_LEN;indexUtil++)
//     {
//         ADT_PtrStackPush(adtStackPtr,(void*)adtPtrStackTestArray[indexUtil]);
//         SystemPrintf("%s,Push Element %s\r\n",__func__,adtPtrStackTestArray[indexUtil]);
//     }
//     do
//     {
//         adtResult = ADT_PtrStackPop(adtStackPtr,&readDataPtr);
//         if(adtResult == ADT_RESULT_OK)
//         {
//             SystemPrintf("%s,Pop Element %s\r\n",__func__,readDataPtr);
//         }
//     }while(adtResult == ADT_RESULT_OK);
//     //释放
//     ADT_PtrStackFree(&adtStackPtr);
// }
// #endif

// //字节栈测试初始化
// #if(SRV_HEART_ADT_BYTE_STACK_TEST_ENABLE != 0)
//     SrvImplHeartSubADT_ByteStackTestInit();
// #endif
//     //指针栈测试初始化
// #if(SRV_HEART_ADT_PTR_STACK_TEST_ENABLE != 0)
//     SrvImplHeartSubADT_PtrStackTestInit();
// #endif

// //字节栈测试
// #if(SRV_HEART_ADT_BYTE_STACK_TEST_ENABLE != 0)
//     SrvImplHeartSubADT_ByteStackTestRunPeriod();
// #endif
//     //指针栈测试
// #if(SRV_HEART_ADT_PTR_STACK_TEST_ENABLE != 0)
//     SrvImplHeartSubADT_PtrStackTestRunPeriod();
// #endif

#endif



