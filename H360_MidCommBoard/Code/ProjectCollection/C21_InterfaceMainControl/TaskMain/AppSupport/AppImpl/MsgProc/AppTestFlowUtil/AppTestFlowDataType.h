#ifndef __APP_TEST_FLOW_DATA_TYPE_
#define __APP_TEST_FLOW_DATA_TYPE_
#include "AppImplBase.h"
#define RACK_PE_OPR_POS             0               //PE操作架位置
#define RACK_TUBE_NUM               10              //架子试管个数
#define RACK_TUBE_BARCODE_LEN       32              //架子试管条码长度
#define SAMPLE_WAREHOUSE_NUM        23              //样本仓存贮位数
#define SAMPLE_WAREHOUSE_INVALID    0xFF            //样本仓无效位置
#define RACK_IVALID                 0               //无效架号
#define TIMER_TICK_UNUSED           0xFFFFFFFF      //无效计时器
#define InitSysResult(p)            {p->resultCode = PROT_RESULT_SUCCESS;\
                                     p->errorMain = ERROR_MAIN_OK;\
                                     p->errorLevel = ERROR_LEVEL_NONE;\
                                     p->errorSub = ERROR_SUB_OK;}
//计算结构体长度
#define TESTFLOW_LENGTH_BYTE(structName)              (sizeof(structName)/sizeof(uint8_t))

#define TestFlowAllocMemory(size)              UserMemMalloc(MEM_REGION_APP_TASK, (size))
#define TestFlowFreeMemory(p)                  UserMemFree(MEM_REGION_APP_TASK, (p))

#define TestFlowOK                  (1u)
#define TestFlowNG                  (0u)
#define TestFlowOVF                 (0xFF)
//是否区分锁定状态
typedef enum
{
    BUFFER_NOT_NEED = 0,        //不需要锁定状态
    BUFFER_NEED,                //锁定状态
}BUFFER_NEED_LOCK;
//接口可测试状态
typedef enum
{
    INTER_NOT_TESTABLE = 0,     //接口不可测试
    INTER_TESTABLE,             //接口可测试
}INTER_TEST_STATUS;
//架子试管存在状态
typedef enum 
{
    RACK_TUBE_NOT_EXIST = 0,    //试管不存在
    RACK_TUBE_EXIST = 1,        //试管存在 
}RACK_TUBE_EXISTENCE;

//试管测试状态
typedef enum
{
    TUBE_STATE_INCOMPLETE = 0,       //未完成
    TUBE_STATE_COMPLETE,             //完成
}TUBE_STATE;  

//架子状态
typedef enum RACK_STATE
{
    RACK_STATE_IDLE = 0,            //空闲，无须测试
    RACK_STATE_TEST_READY,          //准备测试
    //RACK_STATE_TEST_WAIT_PE_PUT,    //PE需要放的样本架
    RACK_STATE_TEST_WAIT_PE_CATCH,  //PE需要取的样本架
    RACK_STATE_TEST_WAIT_CATCH,     //抓取去测试
    RACK_STATE_TEST_PAUSE,          //测试暂停
    RACK_STATE_TESTED,              //测试完成
}RACK_STATE;

//缓冲区状态
typedef enum BUFFER_STATE
{
    BUFFER_STATE_IDLE = 0,      //无架子
    BUFFER_STATE_EXIST,         //有架子
    BUFFER_STATE_LOCKED,        //位置已经锁定
    BUFFER_STATE_LOCKED_IDLE,   //位置锁定有架子
    BUFFER_STATE_LOCKED_EXIST,  //位置锁定无架子
}BUFFER_STATE;

//样本仓
typedef struct 
{
    uint32_t u32RackID;     //架号
    RACK_TUBE_EXISTENCE eTubeExist[RACK_TUBE_NUM];   //是否存在试管   
    TUBE_STATE eTubeState[RACK_TUBE_NUM];            //试管测试状态*
    uint32_t u32BarCode[RACK_TUBE_NUM][RACK_TUBE_BARCODE_LEN];      //试管条码
    RACK_STATE eRackState;                           //试管状态*
    BUFFER_STATE eBufferState;                       //缓冲位状态
    
}SAMPLE_WAREHOUSE_STATE;

//接口状态
typedef enum
{
    INST_INTER_STATE_IDLE = 0,      //空闲
    INST_INTER_STATE_TEST,          //测试中
    INST_INTER_STATE_WARNING,       //警告级别错误中
    INST_INTER_STATE_SAMPLE_STOP,   //部分停止错误中
    INST_INTER_STATE_EMRSTOP,       //急停级别错误中
}INST_INTER_STATE;

//C21状态
typedef enum
{
    INST_C21_STATE_IDLE = 0,        //空闲
    INST_C21_STATE_TEST,            //可测
    INST_C21_STATE_ERROR,           //异常
}INST_C21_STATE;

//C21请求状态
typedef enum 
{
    INST_C21_REQUEST_STATE_IDLE = 0, //无请求
    INST_C21_REQUEST_STATE_RACKIN,   //申请进架
    INST_C21_REQUEST_STATE_RACKOUT,  //申请出架
}INST_C21_REQUEST_STATE;
   
//C21请求
typedef struct
{
    uint32_t u32RackID;                     //架号
    TUBE_STATE eTubeState[RACK_TUBE_NUM];   //试管状态
    INST_C21_REQUEST_STATE eRequestState;   //申请状态
}INST_C21_REQUEST;

//计时器类型
typedef enum
{
    TIMER_TYPE_HEART = 0,
    TIMER_TYPE_MAX,
}TIMER_TYPE;

//流程全局变量
typedef struct
{
    SAMPLE_WAREHOUSE_STATE stSampleWareHouseState[SAMPLE_WAREHOUSE_NUM];     //样本仓状态
    SAMPLE_WAREHOUSE_STATE stC21SampleRackState;    //C21样本仓状态
    INST_C21_STATE eC21State;                   //C21状态
    INST_INTER_STATE eInterState;               //接口状态
    INST_C21_REQUEST stC21Reques;         //C21请求状态
    //PE状态
    
    uint32_t u32TimerHander[TIMER_TYPE_MAX];              //计时器
}APP_TESTFLOW_STATUS;


//设置样本仓结构默认值
extern void AppDataTypeDefault_SampleWarehouse(SAMPLE_WAREHOUSE_STATE* warehouse);
//设置C21请求结构默认值
extern void AppDataTypeDefault_C21Request(INST_C21_REQUEST* request);
#endif