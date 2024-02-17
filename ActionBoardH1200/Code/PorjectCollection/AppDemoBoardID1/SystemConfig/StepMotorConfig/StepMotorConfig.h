#ifndef __STEP_MOTOR_CONFIG_H_
#define __STEP_MOTOR_CONFIG_H_

//步进电机上电间隔时间,防止同时上电电源顶不住
#define STEP_MOTOR_ENABLE_TIME_INTERVAL_MS              100
//运行时检测编码器信号的阈值(发出多少步数之后检测编码器讯息,该参数是无细分的步数)
#define STEP_MOTOR_CHECK_ENCODER_STEPS_NO_DIV           20
//步进电机申请电机信号量的最大等待时间
#define STEP_MOTOR_GET_MUTEX_MAX_TIME_MS                100
//电机超时时间最大值
#define STEP_MOTOR_TIME_OUT_MAX_MS                      60000
//步进电机运转状态查询周期
#define STEP_MOTOR_RUN_STATE_QUERY_INTERVAL_MS          10
//是否使能电机中断状态打印
#define STEP_MOTOR_ENABLE_INT_MSG_SHOW                  CONFIG_FUNC_ENABLE

//内存申请
#define STEP_MOTOR_MEM_MALLOC(size)                     BoardMemMallocWhileSuccessSDRAM(size)
//内存释放
#define STEP_MOTOR_MEM_FREE(memPtr)                     BoardMemFreeSDRAM(memPtr)


#endif


