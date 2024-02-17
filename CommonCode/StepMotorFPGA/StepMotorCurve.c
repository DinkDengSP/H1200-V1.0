/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-05-10 10:12:57 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-05-10 15:02:49 +0800
************************************************************************************************/ 
#include "StepMotorCurve.h"

//清零曲线数据
static void StepMotorCurveClearMainCurveData(STEP_MOTOR_MAIN_CURVE_TAB_DATA* mainCurveDataPtr)
{
    uint16_t indexUtil = 0;
    //清除内存标记
    for(indexUtil = 0; indexUtil < STEP_MOTOR_MAIN_CURVE_TAB_LEN_MAX; indexUtil++)
    {
        mainCurveDataPtr->spdTableArray[indexUtil] = 0.0f;
        mainCurveDataPtr->stpTableArray[indexUtil] = 0;
    }
    mainCurveDataPtr->tabLengthAfterCalc = 0;
    mainCurveDataPtr->totalRunningSteps = 0;
}

//清零曲线数据
static void StepMotorCurveClearUtilCurveData(STEP_MOTOR_UTIL_CURVE_TAB_DATA* utilCurveDataPtr)
{
    uint16_t indexUtil = 0;
    //清除内存标记
    for(indexUtil = 0; indexUtil < STEP_MOTOR_UTIL_CURVE_TAB_LEN_MAX; indexUtil++)
    {
        utilCurveDataPtr->spdTableArray[indexUtil] = 0.0f;
        utilCurveDataPtr->stpTableArray[indexUtil] = 0;
    }
    utilCurveDataPtr->tabLengthAfterCalc = 0;
    utilCurveDataPtr->totalRunningSteps = 0;
}

//匀速曲线计算
static void StepMotorCurveMainCalcSpdTableUniform(STEP_MOTOR_MAIN_CURVE_TAB_DATA* fullCurveDatPtr,float startRpm)
{
    //启动速度和最大速度相等,不需要加减速,匀速运动即可
    fullCurveDatPtr->spdTableArray[0] = startRpm;
    fullCurveDatPtr->tabLengthAfterCalc = 1;
}

//梯形曲线计算
static void StepMotorCurveMainCalcSpdTableTrapezoid(STEP_MOTOR_MAIN_CURVE_TAB_DATA* fullCurveDatPtr,STEP_MOTOR_ATTRI_CURVE* curveAttriPtr)
{
    //辅助变量
    uint32_t indexOfUtil1 = 0,indexOfUtil2 = 0;
    float calcUtilFloat = 0.0;
    //梯形加速
    for(indexOfUtil1 = 0;indexOfUtil1 < (STEP_MOTOR_MAIN_CURVE_TAB_LEN_MAX/2);indexOfUtil1++)
    {
        //计算本次速度,启动速度加上加速速度
        calcUtilFloat = curveAttriPtr->startRpm + (curveAttriPtr->accelerationStart)*indexOfUtil1;
        //如果本次计算出来的速度大于限制的最大速度,那么说明已经达到最大速度,不能加速了
        if(calcUtilFloat > curveAttriPtr->maxRpm)
        {
            //计算速度超了,但是不满足增量条件的情况下,直接到最大速度
            if(indexOfUtil1 > 0)
            {
                if(curveAttriPtr->maxRpm > fullCurveDatPtr->spdTableArray[indexOfUtil1-1])
                {
                    //到最大速度
                    fullCurveDatPtr->spdTableArray[indexOfUtil1] = curveAttriPtr->maxRpm;
                }
                else
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }
        else
        {
            fullCurveDatPtr->spdTableArray[indexOfUtil1] = calcUtilFloat;
        }
    }
    //加速段计算完成,要进行检测以设置减速段
    if(indexOfUtil1 == 1)
    {
        //说明第一次速度就大于最大速度,那就只有一个启动速度
        fullCurveDatPtr->tabLengthAfterCalc = 1;
        return;
    }
    else
    {
        //可以做对称加减速
        for(indexOfUtil2 = indexOfUtil1 ; indexOfUtil2 < (indexOfUtil1*2) ; indexOfUtil2++)
        {
            fullCurveDatPtr->spdTableArray[indexOfUtil2] = fullCurveDatPtr->spdTableArray[(indexOfUtil1*2)-indexOfUtil2-1];
        }
        //记录速度曲线长度
        fullCurveDatPtr->tabLengthAfterCalc = indexOfUtil2;
    }
}


//S形曲线计算
static void StepMotorCurveMainCalcSpdTableS(STEP_MOTOR_MAIN_CURVE_TAB_DATA* fullCurveDatPtr,STEP_MOTOR_ATTRI_CURVE* curveAttriPtr)
{
    //辅助变量
    uint32_t indexOfUtil1 = 0,indexOfUtil2 = 0;
    //辅助变量
    float accUtilFloat = 0.0, vccUtilFloat = 0.0;
    //匀加速加速点数量
    uint32_t uniformAccPointCount = 0;
    //S形加速,第一段加加速段的速度,需要检测超过最大速度和超过最大加速度的限制条件
    for(indexOfUtil1 = 0;indexOfUtil1 < (STEP_MOTOR_MAIN_CURVE_TAB_LEN_MAX/6);indexOfUtil1++)
    {
        if(indexOfUtil1 >= 1)
        {
            //计算本次的加速度
            accUtilFloat = curveAttriPtr->accelerationStart + (indexOfUtil1*curveAttriPtr->accelerationGradient);
            //检测加速度和速度任意一个超过限制阈值,退出循环进行下一步计算
            if(accUtilFloat >= curveAttriPtr->accelerationMax)
            {
                accUtilFloat = curveAttriPtr->accelerationMax;
                //本次的速度是在上一次的基础上加上本次的加速度
                vccUtilFloat = fullCurveDatPtr->spdTableArray[indexOfUtil1-1] + accUtilFloat;
                if(vccUtilFloat >= curveAttriPtr->maxRpm)
                {
                    if(curveAttriPtr->maxRpm > fullCurveDatPtr->spdTableArray[indexOfUtil1-1])
                    {
                        //vccUtilFloat超过速度阈值,但是还是要填充最大速度
                        fullCurveDatPtr->spdTableArray[indexOfUtil1] = curveAttriPtr->maxRpm;
                        indexOfUtil1 += 1;
                    }
                    //速度超过阈值
                    break;
                }
                else
                {
                    fullCurveDatPtr->spdTableArray[indexOfUtil1] = vccUtilFloat;
                    indexOfUtil1 += 1;
                }
                //加速度超过阈值
                break;
            }
            //本次的速度是在上一次的基础上加上本次的加速度
            vccUtilFloat = fullCurveDatPtr->spdTableArray[indexOfUtil1-1] + accUtilFloat;
            if(vccUtilFloat >= curveAttriPtr->maxRpm)
            {
                if(curveAttriPtr->maxRpm > fullCurveDatPtr->spdTableArray[indexOfUtil1-1])
                {
                    //vccUtilFloat超过速度阈值,但是还是要填充最大速度
                    fullCurveDatPtr->spdTableArray[indexOfUtil1] = curveAttriPtr->maxRpm;
                    indexOfUtil1 += 1;
                }
                //速度超过阈值
                break;
            }
            //加速度和速度都不超过,就是这个点了
            fullCurveDatPtr->spdTableArray[indexOfUtil1] = vccUtilFloat;
        }
        else
        {
            //第一步自然是启动速度
            fullCurveDatPtr->spdTableArray[0] = curveAttriPtr->startRpm;
        }
    }
    //如果序号为1
    if(indexOfUtil1 == 1)
    {
        //一开始就超了,没必要加加速了,就匀速
        fullCurveDatPtr->tabLengthAfterCalc = 1;
        return;
    }
    //如果是速度到达上限了,那现在就可以开始准备对称曲线
    if(vccUtilFloat > curveAttriPtr->maxRpm)
    {
        for(indexOfUtil2 = indexOfUtil1 ; indexOfUtil2 < (indexOfUtil1*2) ; indexOfUtil2++)
        {
            fullCurveDatPtr->spdTableArray[indexOfUtil2] = fullCurveDatPtr->spdTableArray[(indexOfUtil1*2)-indexOfUtil2-1];
        }
        //记录速度曲线长度
        fullCurveDatPtr->tabLengthAfterCalc = indexOfUtil2;
        return;
    }
    //匀加速点数统计
    uniformAccPointCount = indexOfUtil1;
    //运行到这里,有两种情况,第一种是加加速到达上限,第二种是加加速点数达到上限
    //第二种点数达到上限不用管,让电机自动去分配,也就是继续一步一步的分配
    //第一种情况,要查看,如果有一个对称的减加速段之后,还需要多少匀加速段,能够让电机达到最大速度
    if (indexOfUtil1 != (STEP_MOTOR_MAIN_CURVE_TAB_LEN_MAX / 6))
    {
        //怎么算,加加速段和减加速段对称,其速度变化是一致的,也就是速度增量是一致的
        float vccDelta = fullCurveDatPtr->spdTableArray[indexOfUtil1 - 1] - fullCurveDatPtr->spdTableArray[0];
        //当前速度加上这个增量的结果是否超过上限
        float vccFinally = fullCurveDatPtr->spdTableArray[indexOfUtil1 - 1] + vccDelta;
        if (vccFinally > curveAttriPtr->maxRpm)
        {
            //不需要匀加速也能达到上限,直接步入减加速
            uniformAccPointCount = 0;
        }
        else
        {
            //计算速度缺口,以及填补这个速度缺口需要的匀加速点数量
            float vccGap = curveAttriPtr->maxRpm - vccFinally;
            //填补这个缺口需要的匀加速点数量
            uniformAccPointCount = (uint32_t)(vccGap / curveAttriPtr->accelerationMax);
            //因为舍入问题导致匀加速少了一个点的话,需要补回来
            if(vccGap > uniformAccPointCount*(curveAttriPtr->accelerationMax))
            {
                //增加一个计算误差舍入点
                uniformAccPointCount += 1;
            }
            //加速点数量要限制在最大范围内
            if(uniformAccPointCount > ((STEP_MOTOR_MAIN_CURVE_TAB_LEN_MAX / 2) - (indexOfUtil1 * 2)))
            {
                uniformAccPointCount = ((STEP_MOTOR_MAIN_CURVE_TAB_LEN_MAX / 2) - (indexOfUtil1 * 2));
            }
            // //匀加速过程最好少一次,留给减加速用
            // if(uniformAccPointCount > 1)
            // {
            //     uniformAccPointCount -= 1;
            // }
        }
    }
    //如果速度没到上限,那么就可以开始下一步的匀加速段
    for(indexOfUtil2 = indexOfUtil1 ; indexOfUtil2 < (uniformAccPointCount +indexOfUtil1) ; indexOfUtil2++)
    {
        //匀加速段不用检测加速度是否到达上限,只检测速度是否达到上限
        vccUtilFloat = fullCurveDatPtr->spdTableArray[indexOfUtil2-1] + curveAttriPtr->accelerationMax;
        if(vccUtilFloat >= curveAttriPtr->maxRpm)
        {
            if(curveAttriPtr->maxRpm > fullCurveDatPtr->spdTableArray[indexOfUtil2-1])
            {
                fullCurveDatPtr->spdTableArray[indexOfUtil2] = curveAttriPtr->maxRpm;
                indexOfUtil2 += 1;
            }
            //速度超过阈值
            break;
        }
        //速度都不超过,就是这个点了
        fullCurveDatPtr->spdTableArray[indexOfUtil2] = vccUtilFloat;
    }
    //如果是速度到达上限了,那现在就可以开始准备对称曲线
    if(vccUtilFloat >= curveAttriPtr->maxRpm)
    {
        for(indexOfUtil1 = indexOfUtil2 ; indexOfUtil1 < (indexOfUtil2*2) ; indexOfUtil1++)
        {
            fullCurveDatPtr->spdTableArray[indexOfUtil1] = fullCurveDatPtr->spdTableArray[(indexOfUtil2*2)-indexOfUtil1-1];
        }
        //记录速度曲线长度
        fullCurveDatPtr->tabLengthAfterCalc = indexOfUtil1;
        return;
    }
    //如果速度没有到达上限,那么开始下一步的减加速段
    for(indexOfUtil2 = (uniformAccPointCount + indexOfUtil1) ; indexOfUtil2 < (uniformAccPointCount+(indexOfUtil1 * 2)) ; 
            indexOfUtil2++)
    {
        //加加速段如果存在一个最大加速度的加速,那么减加速端应该也有这段加速保持对称
        if((accUtilFloat == curveAttriPtr->accelerationMax)&&(indexOfUtil2 == (uniformAccPointCount + indexOfUtil1)))
        {
            accUtilFloat = accUtilFloat;
        }
        else
        {
            accUtilFloat -= curveAttriPtr->accelerationGradient;
        }
        if(accUtilFloat < 0.0f)
        {
            //加速度已经超过了限制,跑到减速去了
            break;
        }
        //减加速段不用检测加速度是否到达上限,只检测速度是否达到上限
        vccUtilFloat = fullCurveDatPtr->spdTableArray[indexOfUtil2-1] + accUtilFloat;
        if(vccUtilFloat > curveAttriPtr->maxRpm)
        {
            if(curveAttriPtr->maxRpm > fullCurveDatPtr->spdTableArray[indexOfUtil2 -1])
            {
                fullCurveDatPtr->spdTableArray[indexOfUtil2] = curveAttriPtr->maxRpm;
                indexOfUtil2++;
            }
            //速度超过阈值
            break;
        }
        //速度都不超过,就是这个点了
        fullCurveDatPtr->spdTableArray[indexOfUtil2] = vccUtilFloat;
    }
    //减加速段完成,不论速度有没有超过, 此刻都需要做对称曲线
    for(indexOfUtil1 = indexOfUtil2 ; indexOfUtil1 < (indexOfUtil2*2) ; indexOfUtil1++)
    {
        fullCurveDatPtr->spdTableArray[indexOfUtil1] = fullCurveDatPtr->spdTableArray[(indexOfUtil2*2)-indexOfUtil1-1];
    }
    //记录速度曲线长度
    fullCurveDatPtr->tabLengthAfterCalc = indexOfUtil1;
    return;
}

//计算速度曲线,选择电机和电机曲线
static void StepMotorCurveMainCalcSpdTableBase(STEP_MOTOR_MAIN_CURVE_TAB_DATA* fullCurveDatPtr,STEP_MOTOR_ATTRI_CURVE* curveAttriPtr)
{
    //根据曲线选择和曲线配置,生成一份速度表格
    //自动计算模式
    if(curveAttriPtr->startRpm >= curveAttriPtr->maxRpm)
    {
        //匀速计算
        StepMotorCurveMainCalcSpdTableUniform(fullCurveDatPtr,curveAttriPtr->startRpm);
        return;
    }
    else
    {
        //检查该曲线是梯形加速还是S形加速,梯形加速没有加加速,S形加速有
        if(curveAttriPtr->accelerationGradient <= 0.000001f)
        {
            //梯形曲线计算
            StepMotorCurveMainCalcSpdTableTrapezoid(fullCurveDatPtr,curveAttriPtr);
        }
        else
        {
            //S形曲线计算
            StepMotorCurveMainCalcSpdTableS(fullCurveDatPtr,curveAttriPtr);
        }
    }
}

//根据速度比率修正速度曲线
static void StepMotorCurveMainCorrentSpdTableWithRatio(STEP_MOTOR_MAIN_CURVE_TAB_DATA* fullCurveDatPtr,SPEED_RATIO_MODE speedRatioMode,float speedRatio)
{
    //辅助变量
    uint32_t indexOfUtil1 = 0;
    //辅助变量
    float calcUtilFloat = 0.0;
    //计算完成之后,要根据速度比例和速度模式调配速度
    if(speedRatio != 1.000000f)
    {
        for(indexOfUtil1 = 0; indexOfUtil1 < fullCurveDatPtr->tabLengthAfterCalc;indexOfUtil1++)
        {
            if(fullCurveDatPtr->spdTableArray[indexOfUtil1] == 0.0f)
            {
                //零速度就不管了
                continue;
            }
            //根据模式选择不同的速度配比
            if(speedRatioMode == SPEED_RATIO_MODE_GLOBAL)
            {
                //全局速度配比
                fullCurveDatPtr->spdTableArray[indexOfUtil1] = ((fullCurveDatPtr->spdTableArray[indexOfUtil1])*(speedRatio));
            }
            else
            {
                //启动速度之上的速度配比
                if(indexOfUtil1 != 0)
                {
                    //计算当前速度相对启动速度的差值
                    calcUtilFloat = fullCurveDatPtr->spdTableArray[indexOfUtil1] - fullCurveDatPtr->spdTableArray[0];
                    //因为是启动速度之上的速度,所以对于0位置的速度不需要进行计算,计算启动速度之上的比例
                    fullCurveDatPtr->spdTableArray[indexOfUtil1] = calcUtilFloat*(speedRatio);
                    //加上启动速度
                    fullCurveDatPtr->spdTableArray[indexOfUtil1] += fullCurveDatPtr->spdTableArray[0];
                }
            }
        }
    }
    else
    {
        //不需要计算速度比例,直接完成
        return;
    }
}

//分配步进电机步数表格
static void StepMotorCurveMainCalcStpTableBase(STEP_MOTOR_MAIN_CURVE_TAB_DATA* fullCurveDatPtr,uint32_t startSteps,STEP_ALLOC_MODE stepAllocMode,uint32_t totalSteps)
{
    //测试使用,每一阶都是启动步数
    uint32_t indexUtil = 0;
    //步数统计
    uint32_t stepsCount = totalSteps;
    //数组访问辅助
    uint32_t arrayUtil = 0;
    //辅助计算
    float utilCalcFloat = 0;
    uint32_t utilCalcUint = 0;
    //如果只有一步速度曲线,直接到位
    if(fullCurveDatPtr->tabLengthAfterCalc == 1)
    {
        fullCurveDatPtr->stpTableArray[0] = stepsCount;
    }
    else
    {
        for(indexUtil = 0; indexUtil < (fullCurveDatPtr->tabLengthAfterCalc/2) ; indexUtil++)
        {
            if(stepAllocMode == STEP_ALLOC_MODE_FIXED)
            {
                //固定步数分配
                utilCalcUint = startSteps;
                //前面除以2这里乘以2,表征对称曲线
                utilCalcUint *= 2;
            }
            else
            {
                //等时间帧步数分配
                utilCalcFloat = startSteps;
                //当前速度
                utilCalcFloat *= fullCurveDatPtr->spdTableArray[indexUtil];
                //启动速度
                utilCalcFloat /= fullCurveDatPtr->spdTableArray[0];
                //数据转换
                utilCalcUint = (uint32_t)utilCalcFloat;
                //限制不能为0
                if(utilCalcUint == 0)
                {
                    utilCalcUint = 1;
                }
                //计算出的实时步数
                utilCalcUint *= 2;
            }
            if(stepsCount >= utilCalcUint)
            {
                //如果当前的保有的步数足以分配
                fullCurveDatPtr->stpTableArray[indexUtil] = utilCalcUint/2;
                arrayUtil = fullCurveDatPtr->tabLengthAfterCalc;
                arrayUtil -= 1;
                //对称分布
                fullCurveDatPtr->stpTableArray[arrayUtil - indexUtil] = fullCurveDatPtr->stpTableArray[indexUtil];
                //分配完了之后,待分配步数降低
                stepsCount -= utilCalcUint;
            }
            else
            {
                //当前保有的步数不足以分配
                if(indexUtil == 0)
                {
                    //还在分配第一步,就直接只分配在第一步上
                    fullCurveDatPtr->stpTableArray[0] = stepsCount;
                }
                else
                {
                    //不在分配的第一步上了,那么就把不足对称分布的数据分布在上一次的表里面
                    fullCurveDatPtr->stpTableArray[indexUtil-1] += stepsCount;
                }
                //分配完了之后,待分配步数降低
                stepsCount = 0;
                //已经分配完成,退出
                break;
            }
        }
        //步数最后一段分配
        if(stepsCount != 0)
        {
            //还没分配完,将剩余部分分配到对称曲线最高位置
            arrayUtil = fullCurveDatPtr->tabLengthAfterCalc;
            arrayUtil /= 2;
            arrayUtil -= 1;
            fullCurveDatPtr->stpTableArray[arrayUtil] += stepsCount;
            stepsCount = 0;
        }
    }
}

//计算速度曲线的适配步数,刚好走完曲线的步数
static void StepMotorCurveMainCalcStpTableAdjust(STEP_MOTOR_MAIN_CURVE_TAB_DATA* fullCurveDatPtr,float startRpm,uint32_t startSteps,
                                                    STEP_ALLOC_MODE stepAllocMode)
{
    //测试使用,每一阶都是启动步数
    uint32_t indexUtil = 0;
    //数组访问辅助
    uint32_t arrayUtil = 0;
    //辅助计算
    uint32_t utilCalcUint = 0;
    //如果只有一步速度曲线,直接到位,注意持续运转的状态机要考虑匀速情况
    if(fullCurveDatPtr->tabLengthAfterCalc == 1)
    {
        fullCurveDatPtr->stpTableArray[0] = startSteps;
        return;
    }
    //否则,进行速度曲线的计算
    for(indexUtil = 0; indexUtil < (fullCurveDatPtr->tabLengthAfterCalc/2) ; indexUtil++)
    {
        if(stepAllocMode == STEP_ALLOC_MODE_FIXED)
        {
            //固定步数分配
            utilCalcUint = startSteps;
        }
        else
        {
            //等时间帧步数分配
            utilCalcUint = startSteps;
            //当前速度
            utilCalcUint *= fullCurveDatPtr->spdTableArray[indexUtil];
            //启动速度
            utilCalcUint /= startRpm;
            if(utilCalcUint == 0)
            {
                utilCalcUint = 1;
            }
        }
        //进行步数表的写入,对称分配
        fullCurveDatPtr->stpTableArray[indexUtil] = utilCalcUint;
        //对称位置计算
        arrayUtil = fullCurveDatPtr->tabLengthAfterCalc;
        arrayUtil -= 1;
        fullCurveDatPtr->stpTableArray[arrayUtil - indexUtil] = fullCurveDatPtr->stpTableArray[indexUtil];
    }
}

//步数表和速度表进行适配,去除无效速度
static void StepMotorCurveMainCalcTableRemoveInvalidItem(STEP_MOTOR_MAIN_CURVE_TAB_DATA* fullCurveDatPtr)
{
    //空值区域的开始序号
    uint32_t zeroRangeStartIndex = 0;
    //空值区域的结束讯号
    uint32_t zeroRangeEnd = 0;
    //辅助计数器
    uint32_t indexUtil = 0;
    //如果系统里面就一个点,那就不需要处理
    if(fullCurveDatPtr->tabLengthAfterCalc == 1)
    {
        return;
    }
    //注意在搬移无效步数的时候,也要把无效速度也搬移掉
    for(indexUtil = 0; indexUtil < (fullCurveDatPtr->tabLengthAfterCalc/2) ; indexUtil++)
    {
        if(fullCurveDatPtr->stpTableArray[indexUtil] == 0)
        {
            //检测到一个步数零点,直接停止循环
            break;
        }
    }
    //检测循环停止点
    if(indexUtil == (fullCurveDatPtr->tabLengthAfterCalc/2))
    {
        //没有空缺点
        return;
    }
    else
    {
        //有空缺点,先记录空缺起始点
        zeroRangeStartIndex = indexUtil;
        zeroRangeEnd = fullCurveDatPtr->tabLengthAfterCalc - zeroRangeStartIndex;
        //然后进行数据拷贝移除,两边的速度点是对称的
        for(indexUtil = zeroRangeStartIndex;indexUtil < (zeroRangeStartIndex*2);indexUtil++)
        {
            //一方面拷贝速度点
            fullCurveDatPtr->spdTableArray[indexUtil] = fullCurveDatPtr->spdTableArray[zeroRangeEnd + indexUtil - zeroRangeStartIndex];
            //另一方面拷贝步数点
            fullCurveDatPtr->stpTableArray[indexUtil] = fullCurveDatPtr->stpTableArray[zeroRangeEnd + indexUtil - zeroRangeStartIndex];
        }
        //更新不去曲线最大值
        fullCurveDatPtr->tabLengthAfterCalc = zeroRangeStartIndex*2;
        //将多余的点清除掉
        for(indexUtil = fullCurveDatPtr->tabLengthAfterCalc; indexUtil < STEP_MOTOR_MAIN_CURVE_TAB_LEN_MAX;indexUtil++)
        {
            fullCurveDatPtr->spdTableArray[indexUtil] = 0.0;
            fullCurveDatPtr->stpTableArray[indexUtil] = 0;
        }
    }
}

//计算步进电机运转曲线.步数曲线
void StepMotorCurveCalcMainTableWithStep(STEP_MOTOR_MAIN_CURVE_TAB_DATA* dataPtr,STEP_MOTOR_ATTRI_CURVE* curveAttriPtr,
                                            SPEED_RATIO_MODE speedRatioMode,float speedRatio,uint32_t totalSteps)
{
    //计算步进电机速度曲线
    StepMotorCurveMainCalcSpdTableBase(dataPtr,curveAttriPtr);
    //步进电机速度比例计算
    StepMotorCurveMainCorrentSpdTableWithRatio(dataPtr,speedRatioMode,speedRatio);
    //电机步数分配
    StepMotorCurveMainCalcStpTableBase(dataPtr,curveAttriPtr->startSteps,curveAttriPtr->stepAllocMode,totalSteps);
    //去除无效速度
    StepMotorCurveMainCalcTableRemoveInvalidItem(dataPtr);
    //设置总运行步数
    dataPtr->totalRunningSteps = totalSteps;
}

//计算步进电机运转曲线,坐标曲线
void StepMotorCurveCalcMainTableWithCoordinate(STEP_MOTOR_MAIN_CURVE_TAB_DATA* dataPtr,STEP_MOTOR_ATTRI_CURVE* curveAttriPtr,
                                            SPEED_RATIO_MODE speedRatioMode,float speedRatio,int32_t currentPos,int32_t targetPos)
{
    int32_t totalSteps = targetPos - currentPos;
    if(totalSteps < 0)
    {
        totalSteps = 0 - totalSteps;
    }
    //计算步进电机速度曲线
    StepMotorCurveMainCalcSpdTableBase(dataPtr,curveAttriPtr);
    //步进电机速度比例计算
    StepMotorCurveMainCorrentSpdTableWithRatio(dataPtr,speedRatioMode,speedRatio);
    //电机步数分配
    StepMotorCurveMainCalcStpTableBase(dataPtr,curveAttriPtr->startSteps,curveAttriPtr->stepAllocMode,totalSteps);
    //去除无效速度
    StepMotorCurveMainCalcTableRemoveInvalidItem(dataPtr);
    //设置总运行步数
    dataPtr->totalRunningSteps = totalSteps;
}

//计算步进电机运转曲线,回零曲线
void StepMotorCurveCalcMainTableWithReturnZero(STEP_MOTOR_MAIN_CURVE_TAB_DATA* dataPtr,STEP_MOTOR_ATTRI_CURVE* curveAttriPtr,
                                                SPEED_RATIO_MODE speedRatioMode,float speedRatio,int32_t currentCoordinate,
                                                uint32_t returnZeroMaxStep)
{
    int32_t totalSteps = 0;
    if(currentCoordinate < 0)
    {
        currentCoordinate = 0 - currentCoordinate;
    }
    if(returnZeroMaxStep != 0)
    {
        //设置了回零最大步数
        if(currentCoordinate > returnZeroMaxStep)
        {
            //修正步数为最大回零步数
            totalSteps = returnZeroMaxStep;
        }
    }
    else
    {
        //未设置最大回零步数
        totalSteps = currentCoordinate;
    }
    //计算步进电机速度曲线
    StepMotorCurveMainCalcSpdTableBase(dataPtr,curveAttriPtr);
    //步进电机速度比例计算
    StepMotorCurveMainCorrentSpdTableWithRatio(dataPtr,speedRatioMode,speedRatio);
    //电机步数分配
    StepMotorCurveMainCalcStpTableBase(dataPtr,curveAttriPtr->startSteps,curveAttriPtr->stepAllocMode,totalSteps);
    //去除无效速度
    StepMotorCurveMainCalcTableRemoveInvalidItem(dataPtr);
    //设置总运行步数
    dataPtr->totalRunningSteps = totalSteps;
}

//计算步进电机运转曲线.持续运转加速曲线
void StepMotorCurveCalcUtilTableWithRunAlwaysAcc(STEP_MOTOR_MAIN_CURVE_TAB_DATA* mainCurveDataPtr,STEP_MOTOR_UTIL_CURVE_TAB_DATA* utilCurveDataPtr,
                                                    STEP_MOTOR_ATTRI_CURVE* curveAttriPtr,SPEED_RATIO_MODE speedRatioMode,float speedRatio)
{
    //清空满载曲线
    StepMotorCurveClearMainCurveData(mainCurveDataPtr);
    //清空加减速曲线
    StepMotorCurveClearUtilCurveData(utilCurveDataPtr);
    //计算步进电机速度曲线
    StepMotorCurveMainCalcSpdTableBase(mainCurveDataPtr,curveAttriPtr);
    //步进电机速度比例计算
    StepMotorCurveMainCorrentSpdTableWithRatio(mainCurveDataPtr,speedRatioMode,speedRatio);
    //次序运转的步数分配与坐标运行的步数分配不同,他会将每一个速度点都分配一个速度
    StepMotorCurveMainCalcStpTableAdjust(mainCurveDataPtr,curveAttriPtr->startRpm,curveAttriPtr->startSteps,curveAttriPtr->stepAllocMode);
    //计算总运行步数
    if(mainCurveDataPtr->tabLengthAfterCalc == 1)
    {
        mainCurveDataPtr->totalRunningSteps = mainCurveDataPtr->stpTableArray[0];
        //只有一步的情况下,加速曲线也只有一步
        utilCurveDataPtr->spdTableArray[0] = mainCurveDataPtr->spdTableArray[0];
        utilCurveDataPtr->stpTableArray[0] = mainCurveDataPtr->stpTableArray[0];
        utilCurveDataPtr->tabLengthAfterCalc = 1;
        utilCurveDataPtr->totalRunningSteps = utilCurveDataPtr->stpTableArray[0];
        return;
    }
    uint16_t indexUtil = 0;
    uint32_t totalSteps = 0;
    //统计完整曲线步数
    for(indexUtil = 0; indexUtil < mainCurveDataPtr->tabLengthAfterCalc;indexUtil++)
    {
        totalSteps += mainCurveDataPtr->stpTableArray[indexUtil];
    }
    mainCurveDataPtr->totalRunningSteps = totalSteps;
    //加速曲线只有总曲线的一半
    utilCurveDataPtr->totalRunningSteps = totalSteps/2;
    utilCurveDataPtr->tabLengthAfterCalc = mainCurveDataPtr->tabLengthAfterCalc/2;
    //分配加速曲线
    for(indexUtil = 0; indexUtil < utilCurveDataPtr->tabLengthAfterCalc;indexUtil++)
    {
        utilCurveDataPtr->spdTableArray[indexUtil] = mainCurveDataPtr->spdTableArray[indexUtil];
        utilCurveDataPtr->stpTableArray[indexUtil] = mainCurveDataPtr->stpTableArray[indexUtil];
    }
    return;
}

//计算步进电机运转曲线.持续运转减速曲线
void StepMotorCurveCalcUtilTableWithRunAlwaysDec(STEP_MOTOR_MAIN_CURVE_TAB_DATA* mainCurveDataPtr,STEP_MOTOR_UTIL_CURVE_TAB_DATA* utilCurveDataPtr,
                                                    uint16_t currentAccSpdTabIndex)
{
    //清空减速曲线
    StepMotorCurveClearUtilCurveData(utilCurveDataPtr);
    //计算总运行步数
    if(mainCurveDataPtr->tabLengthAfterCalc == 1)
    {
        //只有一步的情况下,加速曲线也只有一步
        utilCurveDataPtr->spdTableArray[0] = mainCurveDataPtr->spdTableArray[0];
        utilCurveDataPtr->stpTableArray[0] = mainCurveDataPtr->stpTableArray[0];
        utilCurveDataPtr->tabLengthAfterCalc = 1;
        utilCurveDataPtr->totalRunningSteps = utilCurveDataPtr->stpTableArray[0];
        return;
    }
    //起始序号,曲线的对称映射
    uint16_t startIndex = mainCurveDataPtr->tabLengthAfterCalc - currentAccSpdTabIndex - 1;
    uint16_t tabDecLen = currentAccSpdTabIndex + 1;
    uint32_t totalSteps = 0;
    uint16_t indexUtil = 0;
    //拷贝映射减速曲线
    for(indexUtil = 0; indexUtil < tabDecLen;indexUtil++)
    {
        utilCurveDataPtr->spdTableArray[indexUtil] = mainCurveDataPtr->spdTableArray[startIndex + indexUtil];
        utilCurveDataPtr->stpTableArray[indexUtil] = mainCurveDataPtr->stpTableArray[startIndex + indexUtil];
        totalSteps += utilCurveDataPtr->stpTableArray[indexUtil];
    }
    utilCurveDataPtr->totalRunningSteps = totalSteps;
    utilCurveDataPtr->tabLengthAfterCalc = tabDecLen;
    return;
}

//计算步进电机运转曲线.从曲线运转到DecStop
void StepMotorCurveCalcUtilTableWithFastDecFromCurveSpd(STEP_MOTOR_MAIN_CURVE_TAB_DATA* mainCurveDataPtr,STEP_MOTOR_UTIL_CURVE_TAB_DATA* utilCurveDataPtr,
                                                        uint16_t currentCurveSpdTabIndex,uint32_t daccSteps)
{
    uint32_t dacc2StopStepsCount = daccSteps;
    //辅助计算
    uint32_t utilIndexStart = 0;
    //辅助计算
    uint32_t utilIndexCalc = 0;
    //辅助计算
    uint32_t utilIndex = 0;
    //减速运行的开始减速速度
    float daccStartSpeed = 0.0;
    //减速运行的每次减速速度
    float daccSpeed = 0.0;
    //如果目前的发出坐标序号已经发完了,那么就创建一个匀速结果
    if(currentCurveSpdTabIndex >= mainCurveDataPtr->tabLengthAfterCalc - 1)
    {
        //匀速
        utilCurveDataPtr->tabLengthAfterCalc = 1;
        //速度设置为启动速度
        utilCurveDataPtr->spdTableArray[0] = mainCurveDataPtr->spdTableArray[0];
        //步数设置为减速步数
        utilCurveDataPtr->stpTableArray[0] = daccSteps;
        //设置总步数
        utilCurveDataPtr->totalRunningSteps = daccSteps;
        return;
    }
    //如果最大坐标序号就是1,也就是匀速运动
    if(mainCurveDataPtr->tabLengthAfterCalc == 1)
    {
        //匀速
        utilCurveDataPtr->tabLengthAfterCalc = 1;
        //速度设置为启动速度
        utilCurveDataPtr->spdTableArray[0] = mainCurveDataPtr->spdTableArray[0];
        //步数设置为减速步数
        utilCurveDataPtr->stpTableArray[0] = daccSteps;
        //设置总步数
        utilCurveDataPtr->totalRunningSteps = daccSteps;
        return;
    }
    //如果现在处于加速过程
    if(currentCurveSpdTabIndex < ((mainCurveDataPtr->tabLengthAfterCalc)/2))
    {
        //计算出对称点
        utilIndexStart = mainCurveDataPtr->tabLengthAfterCalc - currentCurveSpdTabIndex -1;
    }
    else
    {
        //已经在减速过程,继续减速,只是减速过程变快
        utilIndexStart = currentCurveSpdTabIndex;
    }
    //知道了从此刻速度到停止有多少个速度,看看停止步数够不够这么多点的分配,如果不够,就要进行跳格
    utilIndexCalc = mainCurveDataPtr->tabLengthAfterCalc - utilIndexStart;
    //减速点的数量比减速步数多,步数不够分,以步数作为减速依据
    if(utilIndexCalc > daccSteps)
    {
        //步数不够分,必须消减速度表
        utilCurveDataPtr->tabLengthAfterCalc = daccSteps;
        //启动减速速度
        daccStartSpeed = mainCurveDataPtr->spdTableArray[utilIndexStart];
        //每一节的减速速度,从当前速度到停止一共需要的速度区间
        daccSpeed = daccStartSpeed - mainCurveDataPtr->spdTableArray[mainCurveDataPtr->tabLengthAfterCalc - 1];
        //梯形减速,计算每一步的减速比例
        daccSpeed /= daccSteps;
        //分配减速步数
        for(utilIndex = 0;utilIndex < utilCurveDataPtr->tabLengthAfterCalc;utilIndex++)
        {
            //步数肯定是1
            utilCurveDataPtr->stpTableArray[utilIndex] = 1;
            //速度就阶梯递减
            utilCurveDataPtr->spdTableArray[utilIndex] = daccStartSpeed - (utilIndex*daccSpeed);
        }
        //设置总步数
        utilCurveDataPtr->totalRunningSteps = daccSteps;
        return;
    }
    else
    {
        //步数够分
        utilCurveDataPtr->tabLengthAfterCalc = utilIndexCalc;
        //先拷贝速度表
        for(utilIndex = 0;utilIndex < utilCurveDataPtr->tabLengthAfterCalc;utilIndex++)
        {
            utilCurveDataPtr->spdTableArray[utilIndex] = (mainCurveDataPtr)->spdTableArray[utilIndex+utilIndexStart];
            //步数要取余数,提倡平均分配
            utilCurveDataPtr->stpTableArray[utilIndex] = (uint32_t)((daccSteps)/(utilIndexCalc));
            //总步数降低
            dacc2StopStepsCount -= utilCurveDataPtr->stpTableArray[utilIndex];
        }
        //最终多的步数,分配到尾巴上
        utilCurveDataPtr->stpTableArray[utilIndex-1] += dacc2StopStepsCount;
        //设置总步数
        utilCurveDataPtr->totalRunningSteps = daccSteps;
        return;
    }
}

//计算步进电机运转曲线.从定频运转到DecStop
void StepMotorCurveCalcUtilTableWithFastDecFromFixSpd(STEP_MOTOR_UTIL_CURVE_TAB_DATA* utilCurveDataPtr,float highSpd,float lowSpd,uint32_t steps)
{
    uint16_t indexUtil = 0;
    //防止减速步数太多
    if(steps >= STEP_MOTOR_UTIL_CURVE_TAB_LEN_MAX)
    {
        steps = STEP_MOTOR_UTIL_CURVE_TAB_LEN_MAX;
    }
    float daccSpd = (highSpd - lowSpd)/((float)(steps));
    for(indexUtil = 0; indexUtil < steps; indexUtil++)
    {
        utilCurveDataPtr->spdTableArray[indexUtil] = highSpd - (indexUtil+1)*daccSpd;
        utilCurveDataPtr->stpTableArray[indexUtil] = 1;
    }
    utilCurveDataPtr->tabLengthAfterCalc = steps;
    utilCurveDataPtr->totalRunningSteps = steps;
}