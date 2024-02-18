/*******************************************************************
**Description: Copyright(c) 2018-2025 LiHeBio,All rights reserved.
**Author: DengXiaoJun
**Date: 2021-03-31 11:22:16
**LastEditors: DengXiaoJun
**LastEditTime: 2021-04-02 17:31:11
**ModifyRecord1:    
**ModifyRecord2:    
**ModifyRecord3:    
**ModifyRecord4:    
**ModifyRecord5:    
******************************************************************/
#include "StepMotorTable.h"
#include "StepMotor.h"

//匀速曲线计算
static void StepMotorTableCalcSpeedTableUniform(STEP_MOTOR_NO motorNo,uint8_t selectCurveIndex)
{
    //启动速度和最大速度相等,不需要加减速,匀速运动即可
    stepMotorVarArray[motorNo].var.stepMotorSpeedTableDirect[0] = stepMotorVarArray[motorNo].attriPtr->speedTableCollect[selectCurveIndex].startSpeed;
    stepMotorVarArray[motorNo].var.currentTableMaxLength = 1;
}

//梯形曲线计算
static void StepMotorTableCalcSpeedTableTrapezoidCurve(STEP_MOTOR_NO motorNo,uint8_t selectCurveIndex)
{
    //辅助变量
    uint32_t indexOfUtil1 = 0,indexOfUtil2 = 0;
    float calcUtilFloat = 0.0;
    //梯形加速
    for(indexOfUtil1 = 0;indexOfUtil1 < (SM_SPEED_TABLE_LENGTH_MAX/2);indexOfUtil1++)
    {
        //计算本次速度,启动速度加上加速速度
        calcUtilFloat = stepMotorVarArray[motorNo].attriPtr->speedTableCollect[selectCurveIndex].startSpeed + 
                            ((stepMotorVarArray[motorNo].attriPtr->speedTableCollect[selectCurveIndex].startAcc)*indexOfUtil1);
        //如果本次计算出来的速度大于限制的最大速度,那么说明已经达到最大速度,不能加速了
        if(calcUtilFloat > stepMotorVarArray[motorNo].attriPtr->speedTableCollect[selectCurveIndex].maxSpeed)
        {
            //计算速度超了,但是不满足增量条件的情况下,直接到最大速度
            if(indexOfUtil1 > 0)
            {
                if(stepMotorVarArray[motorNo].attriPtr->speedTableCollect[selectCurveIndex].maxSpeed > 
                    stepMotorVarArray[motorNo].var.stepMotorSpeedTableDirect[indexOfUtil1-1])
                {
                    //到最大速度
                    stepMotorVarArray[motorNo].var.stepMotorSpeedTableDirect[indexOfUtil1] = 
                        stepMotorVarArray[motorNo].attriPtr->speedTableCollect[selectCurveIndex].maxSpeed;
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
            stepMotorVarArray[motorNo].var.stepMotorSpeedTableDirect[indexOfUtil1] = calcUtilFloat;
        }
    }
    //加速段计算完成,要进行检测以设置减速段
    if(indexOfUtil1 == 1)
    {
        //说明第一次速度就大于最大速度,那就只有一个启动速度
        stepMotorVarArray[motorNo].var.currentTableMaxLength = 1;
        return;
    }
    else
    {
        //可以做对称加减速
        for(indexOfUtil2 = indexOfUtil1 ; indexOfUtil2 < (indexOfUtil1*2) ; indexOfUtil2++)
        {
            stepMotorVarArray[motorNo].var.stepMotorSpeedTableDirect[indexOfUtil2] = 
                stepMotorVarArray[motorNo].var.stepMotorSpeedTableDirect[(indexOfUtil1*2)-indexOfUtil2-1];
        }
        //记录速度曲线长度
        stepMotorVarArray[motorNo].var.currentTableMaxLength = indexOfUtil2;
    }
}

//S形曲线计算
static void StepMotorTableCalcSpeedTableS_Curve(STEP_MOTOR_NO motorNo,uint8_t selectCurveIndex)
{
    //辅助变量
    uint32_t indexOfUtil1 = 0,indexOfUtil2 = 0;
    //辅助变量
    float accUtilFloat = 0.0, vccUtilFloat = 0.0;
    //匀加速加速点数量
    uint32_t uniformAccPointCount = 0;
    //S形加速,第一段加加速段的速度,需要检测超过最大速度和超过最大加速度的限制条件
    for(indexOfUtil1 = 0;indexOfUtil1 < (SM_SPEED_TABLE_LENGTH_MAX/6);indexOfUtil1++)
    {
        if(indexOfUtil1 >= 1)
        {
            //计算本次的加速度
            accUtilFloat = stepMotorVarArray[motorNo].attriPtr->speedTableCollect[selectCurveIndex].startAcc + 
                            ((indexOfUtil1-1)*stepMotorVarArray[motorNo].attriPtr->speedTableCollect[selectCurveIndex].accOfAcc);
            //检测加速度和速度任意一个超过限制阈值,退出循环进行下一步计算
            if(accUtilFloat >= stepMotorVarArray[motorNo].attriPtr->speedTableCollect[selectCurveIndex].maxAcc)
            {
                accUtilFloat = stepMotorVarArray[motorNo].attriPtr->speedTableCollect[selectCurveIndex].maxAcc;
                //本次的速度是在上一次的基础上加上本次的加速度
                vccUtilFloat = stepMotorVarArray[motorNo].var.stepMotorSpeedTableDirect[indexOfUtil1-1] + accUtilFloat;
                if(vccUtilFloat >= stepMotorVarArray[motorNo].attriPtr->speedTableCollect[selectCurveIndex].maxSpeed)
                {
                    if(stepMotorVarArray[motorNo].attriPtr->speedTableCollect[selectCurveIndex].maxSpeed > 
                        stepMotorVarArray[motorNo].var.stepMotorSpeedTableDirect[indexOfUtil1-1])
                    {
                        //vccUtilFloat超过速度阈值,但是还是要填充最大速度
                        stepMotorVarArray[motorNo].var.stepMotorSpeedTableDirect[indexOfUtil1] = 
                            stepMotorVarArray[motorNo].attriPtr->speedTableCollect[selectCurveIndex].maxSpeed;
                        indexOfUtil1 += 1;
                    }
                    //速度超过阈值
                    break;
                }
                else
                {
                    stepMotorVarArray[motorNo].var.stepMotorSpeedTableDirect[indexOfUtil1] = vccUtilFloat;
                    indexOfUtil1 += 1;
                }
                //加速度超过阈值
                break;
            }
            //本次的速度是在上一次的基础上加上本次的加速度
            vccUtilFloat = stepMotorVarArray[motorNo].var.stepMotorSpeedTableDirect[indexOfUtil1-1] + accUtilFloat;
            if(vccUtilFloat >= stepMotorVarArray[motorNo].attriPtr->speedTableCollect[selectCurveIndex].maxSpeed)
            {
                if(stepMotorVarArray[motorNo].attriPtr->speedTableCollect[selectCurveIndex].maxSpeed > 
                        stepMotorVarArray[motorNo].var.stepMotorSpeedTableDirect[indexOfUtil1-1])
                {
                    //vccUtilFloat超过速度阈值,但是还是要填充最大速度
                    stepMotorVarArray[motorNo].var.stepMotorSpeedTableDirect[indexOfUtil1] = 
                        stepMotorVarArray[motorNo].attriPtr->speedTableCollect[selectCurveIndex].maxSpeed;
                    indexOfUtil1 += 1;
                }
                //速度超过阈值
                break;
            }
            //加速度和速度都不超过,就是这个点了
            stepMotorVarArray[motorNo].var.stepMotorSpeedTableDirect[indexOfUtil1] = vccUtilFloat;
        }
        else
        {
            //第一步自然是启动速度
            stepMotorVarArray[motorNo].var.stepMotorSpeedTableDirect[0] = 
                stepMotorVarArray[motorNo].attriPtr->speedTableCollect[selectCurveIndex].startSpeed;
        }
    }
    //如果序号为1
    if(indexOfUtil1 == 1)
    {
        //一开始就超了,没必要加加速了,就匀速
        stepMotorVarArray[motorNo].var.currentTableMaxLength = 1;
        return;
    }
    //如果是速度到达上限了,那现在就可以开始准备对称曲线
    if(vccUtilFloat > stepMotorVarArray[motorNo].attriPtr->speedTableCollect[selectCurveIndex].maxSpeed)
    {
        for(indexOfUtil2 = indexOfUtil1 ; indexOfUtil2 < (indexOfUtil1*2) ; indexOfUtil2++)
        {
            stepMotorVarArray[motorNo].var.stepMotorSpeedTableDirect[indexOfUtil2] = 
                stepMotorVarArray[motorNo].var.stepMotorSpeedTableDirect[(indexOfUtil1*2)-indexOfUtil2-1];
        }
        //记录速度曲线长度
        stepMotorVarArray[motorNo].var.currentTableMaxLength = indexOfUtil2;
        return;
    }
    //匀加速点数统计
    uniformAccPointCount = indexOfUtil1;
    //运行到这里,有两种情况,第一种是加加速到达上限,第二种是加加速点数达到上限
    //第二种点数达到上限不用管,让电机自动去分配,也就是继续一步一步的分配
    //第一种情况,要查看,如果有一个对称的减加速段之后,还需要多少匀加速段,能够让电机达到最大速度
    if (indexOfUtil1 != (SM_SPEED_TABLE_LENGTH_MAX / 6))
    {
        //怎么算,加加速段和减加速段对称,其速度变化是一致的,也就是速度增量是一致的
        float vccDelta = stepMotorVarArray[motorNo].var.stepMotorSpeedTableDirect[indexOfUtil1 - 1] - 
                    stepMotorVarArray[motorNo].var.stepMotorSpeedTableDirect[0];
        //当前速度加上这个增量的结果是否超过上限
        float vccFinally = stepMotorVarArray[motorNo].var.stepMotorSpeedTableDirect[indexOfUtil1 - 1] + vccDelta;
        if (vccFinally > stepMotorVarArray[motorNo].attriPtr->speedTableCollect[selectCurveIndex].maxSpeed)
        {
            //不需要匀加速也能达到上限,直接步入减加速
            uniformAccPointCount = 0;
        }
        else
        {
            //计算速度缺口,以及填补这个速度缺口需要的匀加速点数量
            float vccGap = stepMotorVarArray[motorNo].attriPtr->speedTableCollect[selectCurveIndex].maxSpeed - vccFinally;
            //填补这个缺口需要的匀加速点数量
            uniformAccPointCount = (uint32_t)(vccGap / stepMotorVarArray[motorNo].attriPtr->speedTableCollect[selectCurveIndex].maxAcc);
            //因为舍入问题导致匀加速少了一个点的话,需要补回来
            if(vccGap > uniformAccPointCount*(stepMotorVarArray[motorNo].attriPtr->speedTableCollect[selectCurveIndex].maxAcc))
            {
                //增加一个计算误差舍入点
                uniformAccPointCount += 1;
            }
            //加速点数量要限制在最大范围内
            if(uniformAccPointCount > ((SM_SPEED_TABLE_LENGTH_MAX / 2) - (indexOfUtil1 * 2)))
            {
                uniformAccPointCount = ((SM_SPEED_TABLE_LENGTH_MAX / 2) - (indexOfUtil1 * 2));
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
        vccUtilFloat = stepMotorVarArray[motorNo].var.stepMotorSpeedTableDirect[indexOfUtil2-1] + 
                        stepMotorVarArray[motorNo].attriPtr->speedTableCollect[selectCurveIndex].maxAcc;
        if(vccUtilFloat >= stepMotorVarArray[motorNo].attriPtr->speedTableCollect[selectCurveIndex].maxSpeed)
        {
            if(stepMotorVarArray[motorNo].attriPtr->speedTableCollect[selectCurveIndex].maxSpeed >
                 stepMotorVarArray[motorNo].var.stepMotorSpeedTableDirect[indexOfUtil2-1])
            {
                stepMotorVarArray[motorNo].var.stepMotorSpeedTableDirect[indexOfUtil2] = 
                        stepMotorVarArray[motorNo].attriPtr->speedTableCollect[selectCurveIndex].maxSpeed;
                indexOfUtil2 += 1;
            }
            //速度超过阈值
            break;
        }
        //速度都不超过,就是这个点了
        stepMotorVarArray[motorNo].var.stepMotorSpeedTableDirect[indexOfUtil2] = vccUtilFloat;
    }
    //如果是速度到达上限了,那现在就可以开始准备对称曲线
    if(vccUtilFloat >= stepMotorVarArray[motorNo].attriPtr->speedTableCollect[selectCurveIndex].maxSpeed)
    {
        for(indexOfUtil1 = indexOfUtil2 ; indexOfUtil1 < (indexOfUtil2*2) ; indexOfUtil1++)
        {
            stepMotorVarArray[motorNo].var.stepMotorSpeedTableDirect[indexOfUtil1] = 
                stepMotorVarArray[motorNo].var.stepMotorSpeedTableDirect[(indexOfUtil2*2)-indexOfUtil1-1];
        }
        //记录速度曲线长度
        stepMotorVarArray[motorNo].var.currentTableMaxLength = indexOfUtil1;
        return;
    }
    //如果速度没有到达上限,那么开始下一步的减加速段
    for(indexOfUtil2 = (uniformAccPointCount + indexOfUtil1) ; indexOfUtil2 < (uniformAccPointCount+(indexOfUtil1 * 2)) ; 
            indexOfUtil2++)
    {
        //加加速段如果存在一个最大加速度的加速,那么减加速端应该也有这段加速保持对称
        if((accUtilFloat == stepMotorVarArray[motorNo].attriPtr->speedTableCollect[selectCurveIndex].maxAcc)&&
                (indexOfUtil2 == (uniformAccPointCount + indexOfUtil1)))
        {
            accUtilFloat = accUtilFloat;
        }
        else
        {
            accUtilFloat -= stepMotorVarArray[motorNo].attriPtr->speedTableCollect[selectCurveIndex].accOfAcc;
        }
        if(accUtilFloat < 0.0f)
        {
            //加速度已经超过了限制,跑到减速去了
            break;
        }
        //减加速段不用检测加速度是否到达上限,只检测速度是否达到上限
        vccUtilFloat = stepMotorVarArray[motorNo].var.stepMotorSpeedTableDirect[indexOfUtil2-1] + accUtilFloat;
        if(vccUtilFloat > stepMotorVarArray[motorNo].attriPtr->speedTableCollect[selectCurveIndex].maxSpeed)
        {
            if(stepMotorVarArray[motorNo].attriPtr->speedTableCollect[selectCurveIndex].maxSpeed > 
                    stepMotorVarArray[motorNo].var.stepMotorSpeedTableDirect[indexOfUtil2 -1])
            {
                stepMotorVarArray[motorNo].var.stepMotorSpeedTableDirect[indexOfUtil2] = 
                    stepMotorVarArray[motorNo].attriPtr->speedTableCollect[selectCurveIndex].maxSpeed;
                indexOfUtil2++;
            }
            //速度超过阈值
            break;
        }
        //速度都不超过,就是这个点了
        stepMotorVarArray[motorNo].var.stepMotorSpeedTableDirect[indexOfUtil2] = vccUtilFloat;
    }
    //减加速段完成,不论速度有没有超过, 此刻都需要做对称曲线
    for(indexOfUtil1 = indexOfUtil2 ; indexOfUtil1 < (indexOfUtil2*2) ; indexOfUtil1++)
    {
        stepMotorVarArray[motorNo].var.stepMotorSpeedTableDirect[indexOfUtil1] = 
            stepMotorVarArray[motorNo].var.stepMotorSpeedTableDirect[(indexOfUtil2*2)-indexOfUtil1-1];
    }
    //记录速度曲线长度
    stepMotorVarArray[motorNo].var.currentTableMaxLength = indexOfUtil1;
    return;
}

//计算速度曲线,选择电机和电机曲线
static void StepMotorTableCalcSpeedTable(STEP_MOTOR_NO motorNo,uint8_t selectCurveIndex)
{
    //根据曲线选择和曲线配置,生成一份速度表格
    //自动计算模式
    if(stepMotorVarArray[motorNo].attriPtr->speedTableCollect[selectCurveIndex].startSpeed >= 
        stepMotorVarArray[motorNo].attriPtr->speedTableCollect[selectCurveIndex].maxSpeed)
    {
        //匀速计算
        StepMotorTableCalcSpeedTableUniform(motorNo,selectCurveIndex);
        return;
    }
    else
    {
        //检查该曲线是梯形加速还是S形加速,梯形加速没有加加速,S形加速有
        if(stepMotorVarArray[motorNo].attriPtr->speedTableCollect[selectCurveIndex].accOfAcc <= (float)(0.000001))
        {
            //梯形曲线计算
            StepMotorTableCalcSpeedTableTrapezoidCurve(motorNo,selectCurveIndex);
        }
        else
        {
            //S形曲线计算
            StepMotorTableCalcSpeedTableS_Curve(motorNo,selectCurveIndex);
        }
    }
}

//根据速度比率修正速度曲线
static void StepMotorTableCalcSpeedWithRatio(STEP_MOTOR_NO motorNo,SPEED_RATIO_MODE mode,uint8_t ratio)
{
    //辅助变量
    uint32_t indexOfUtil1 = 0;
    //辅助变量
    float calcUtilFloat = 0.0;
    //计算完成之后,要根据速度比例和速度模式调配速度
    if(ratio != 100)
    {
        for(indexOfUtil1 = 0; indexOfUtil1 < stepMotorVarArray[motorNo].var.currentTableMaxLength;indexOfUtil1++)
        {
            //根据模式选择不同的速度配比
            if(mode == SPEED_RATIO_MODE_GLOBAL)
            {
                //全局速度配比
                stepMotorVarArray[motorNo].var.stepMotorSpeedTableDirect[indexOfUtil1] = 
                                ((stepMotorVarArray[motorNo].var.stepMotorSpeedTableDirect[indexOfUtil1]*ratio)/((float)100.0));
            }
            else
            {
                //启动速度之上的速度配比
                if(indexOfUtil1 != 0)
                {
                    //计算当前速度相对启动速度的差值
                    calcUtilFloat = stepMotorVarArray[motorNo].var.stepMotorSpeedTableDirect[indexOfUtil1] - 
                                            stepMotorVarArray[motorNo].var.stepMotorSpeedTableDirect[0];
                    //因为是启动速度之上的速度,所以对于0位置的速度不需要进行计算,计算启动速度之上的比例
                    stepMotorVarArray[motorNo].var.stepMotorSpeedTableDirect[indexOfUtil1] = 
                        ((calcUtilFloat*ratio)/((float)(100.0)));
                    //加上启动速度
                    stepMotorVarArray[motorNo].var.stepMotorSpeedTableDirect[indexOfUtil1] += 
                        stepMotorVarArray[motorNo].var.stepMotorSpeedTableDirect[0];
                }
            }
        }
    }
    else
    {
        //不需要计算速度比例,直接完成
    }
}

//分配步进电机步数表格
static void StepMotorTableCalcStepTable(int32_t steps,STEP_MOTOR_NO motorNo,uint8_t selectCurveIndex)
{
    //测试使用,每一阶都是启动步数
    uint32_t indexUtil = 0;
    //步数统计
    uint32_t stepsCount = 0;
    //数组访问辅助
    uint32_t arrayUtil = 0;
    if(steps < 0)
    {
        stepsCount = 0 - steps;
    }
    else
    {
        stepsCount = (uint32_t)(steps);
    }
    //辅助计算
    uint32_t utilCalcUint = 0;
    //如果只有一步速度曲线,直接到位
    if(stepMotorVarArray[motorNo].var.currentTableMaxLength == 1)
    {
        stepMotorVarArray[motorNo].var.stepMotorStepTableDirect[0] = stepsCount;
    }
    else
    {
        for(indexUtil = 0; indexUtil < (stepMotorVarArray[motorNo].var.currentTableMaxLength/2) ; indexUtil++)
        {
            if(stepMotorVarArray[motorNo].attriPtr->speedTableCollect[selectCurveIndex].stepMode == FIXED)
            {
                //固定步数分配
                utilCalcUint = stepMotorVarArray[motorNo].attriPtr->speedTableCollect[selectCurveIndex].startStep;
                utilCalcUint *= 2;
            }
            else
            {
                //等时间帧步数分配
                utilCalcUint = stepMotorVarArray[motorNo].attriPtr->speedTableCollect[selectCurveIndex].startStep;
                //当前速度
                utilCalcUint *= stepMotorVarArray[motorNo].var.stepMotorSpeedTableDirect[indexUtil];
                //启动速度
                utilCalcUint /= stepMotorVarArray[motorNo].var.stepMotorSpeedTableDirect[0];
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
                stepMotorVarArray[motorNo].var.stepMotorStepTableDirect[indexUtil] = utilCalcUint/2;
                arrayUtil = stepMotorVarArray[motorNo].var.currentTableMaxLength;
                arrayUtil -= 1;
                stepMotorVarArray[motorNo].var.stepMotorStepTableDirect[arrayUtil - indexUtil] = stepMotorVarArray[motorNo].var.stepMotorStepTableDirect[indexUtil];
                //分配完了之后,待分配步数降低
                stepsCount -= utilCalcUint;
            }
            else
            {
                //当前保有的步数不足以分配
                if(indexUtil == 0)
                {
                    //还在分配第一步,就直接只分配在第一步上
                    stepMotorVarArray[motorNo].var.stepMotorStepTableDirect[0] = stepsCount;
                }
                else
                {
                    //不在分配的第一步上了,那么就把不足对称分布的数据分布在上一次的表里面
                    stepMotorVarArray[motorNo].var.stepMotorStepTableDirect[indexUtil-1] += stepsCount;
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
            arrayUtil = stepMotorVarArray[motorNo].var.currentTableMaxLength;
            arrayUtil /= 2;
            arrayUtil -= 1;
            stepMotorVarArray[motorNo].var.stepMotorStepTableDirect[arrayUtil] += stepsCount;
            stepsCount = 0;
        }
    }
}

static void StepMotorTableCalcStepTableWithRunAlways(STEP_MOTOR_NO motorNo,uint8_t selectCurveIndex)
{
    //测试使用,每一阶都是启动步数
    uint32_t indexUtil = 0;
    //数组访问辅助
    uint32_t arrayUtil = 0;
    //辅助计算
    uint32_t utilCalcUint = 0;
    //如果只有一步速度曲线,直接到位,注意次序运转的状态机要考虑匀速情况
    if(stepMotorVarArray[motorNo].var.currentTableMaxLength == 1)
    {
        stepMotorVarArray[motorNo].var.stepMotorStepTableDirect[0] = 
            stepMotorVarArray[motorNo].attriPtr->speedTableCollect[selectCurveIndex].startStep;
    }
    //否则,进行速度曲线的计算
    for(indexUtil = 0; indexUtil < (stepMotorVarArray[motorNo].var.currentTableMaxLength/2) ; indexUtil++)
    {
        if(stepMotorVarArray[motorNo].attriPtr->speedTableCollect[selectCurveIndex].stepMode == FIXED)
        {
            //固定步数分配
            utilCalcUint = stepMotorVarArray[motorNo].attriPtr->speedTableCollect[selectCurveIndex].startStep;
        }
        else
        {
            //等时间帧步数分配
            utilCalcUint = stepMotorVarArray[motorNo].attriPtr->speedTableCollect[selectCurveIndex].startStep;
            //当前速度
            utilCalcUint *= stepMotorVarArray[motorNo].var.stepMotorSpeedTableDirect[indexUtil];
            //启动速度
            utilCalcUint /= stepMotorVarArray[motorNo].attriPtr->speedTableCollect[selectCurveIndex].startSpeed;
        }
        //进行步数表的写入,对称分配
        stepMotorVarArray[motorNo].var.stepMotorStepTableDirect[indexUtil] = utilCalcUint;
        arrayUtil = stepMotorVarArray[motorNo].var.currentTableMaxLength;
        arrayUtil -= 1;
        stepMotorVarArray[motorNo].var.stepMotorStepTableDirect[arrayUtil - indexUtil] = stepMotorVarArray[motorNo].var.stepMotorStepTableDirect[indexUtil];
    }
}

//步数表和速度表进行适配,去除无效速度
static void StepMotorTableAdapterWithStepSpeedTable(STEP_MOTOR_NO motorNo)
{
    //空值区域的开始序号
    uint32_t zeroRangeStartIndex = 0;
    //空值区域的结束讯号
    uint32_t zeroRangeEnd = 0;
    //辅助计数器
    uint32_t indexUtil = 0;
    //如果系统里面就一个点,那就不需要处理
    if(stepMotorVarArray[motorNo].var.currentTableMaxLength == 1)
    {
        return;
    }
    //注意在搬移无效步数的时候,也要把无效速度也搬移掉
    for(indexUtil = 0; indexUtil < (stepMotorVarArray[motorNo].var.currentTableMaxLength/2) ; indexUtil++)
    {
        if(stepMotorVarArray[motorNo].var.stepMotorStepTableDirect[indexUtil] == 0)
        {
            //检测到一个步数零点,直接停止循环
            break;
        }
    }
    //检测循环停止点
    if(indexUtil == (stepMotorVarArray[motorNo].var.currentTableMaxLength/2))
    {
        //没有空缺点
        return;
    }
    else
    {
        //有空缺点,先记录空缺起始点
        zeroRangeStartIndex = indexUtil;
        zeroRangeEnd = stepMotorVarArray[motorNo].var.currentTableMaxLength - zeroRangeStartIndex;
        //然后进行数据拷贝移除,两边的速度点是对称的
        for(indexUtil = zeroRangeStartIndex;indexUtil < (zeroRangeStartIndex*2);indexUtil++)
        {
            //一方面拷贝速度点
            stepMotorVarArray[motorNo].var.stepMotorSpeedTableDirect[indexUtil] = 
                stepMotorVarArray[motorNo].var.stepMotorSpeedTableDirect[zeroRangeEnd + indexUtil - zeroRangeStartIndex];
            //另一方面拷贝步数点
            stepMotorVarArray[motorNo].var.stepMotorStepTableDirect[indexUtil] = 
                stepMotorVarArray[motorNo].var.stepMotorStepTableDirect[zeroRangeEnd + indexUtil - zeroRangeStartIndex];
        }
        //更新不去曲线最大值
        stepMotorVarArray[motorNo].var.currentTableMaxLength = zeroRangeStartIndex*2;
        //将多余的点清除掉
        for(indexUtil = stepMotorVarArray[motorNo].var.currentTableMaxLength; indexUtil < SM_SPEED_TABLE_LENGTH_MAX;indexUtil++)
        {
            //一方面拷贝速度点
            stepMotorVarArray[motorNo].var.stepMotorSpeedTableDirect[indexUtil] = 0.0;
            //另一方面拷贝步数点
            stepMotorVarArray[motorNo].var.stepMotorStepTableDirect[indexUtil] = 0;
        }
    }
}


//使用步数计算步进电机加速曲线
void StepMotorTableCalcWithStep(int32_t steps,STEP_MOTOR_NO motorNo,uint8_t selectConfig,SPEED_RATIO_MODE mode,uint8_t ratio)
{
    //计算步进电机速度曲线
    StepMotorTableCalcSpeedTable(motorNo,selectConfig);
    //步进电机速度比例计算
    StepMotorTableCalcSpeedWithRatio(motorNo,mode,ratio);
    //电机步数分配
    StepMotorTableCalcStepTable(steps,motorNo,selectConfig);
    //去除无效速度
    StepMotorTableAdapterWithStepSpeedTable(motorNo);
}

//使用坐标计算步进电机加速曲线
void StepMotorTableCalcWithCoordinate(int32_t targetCoordinate,STEP_MOTOR_NO motorNo,uint8_t selectConfig,SPEED_RATIO_MODE mode,uint8_t ratio)
{
    //坐标计算的步数是目标坐标减去电机当前坐标
    int32_t steps = targetCoordinate - stepMotorVarArray[motorNo].var.currentCoordinate;
    //计算步进电机速度曲线
    StepMotorTableCalcSpeedTable(motorNo,selectConfig);
    //步进电机速度比例计算
    StepMotorTableCalcSpeedWithRatio(motorNo,mode,ratio);
    //电机步数分配
    StepMotorTableCalcStepTable(steps,motorNo,selectConfig);
    //去除无效速度
    StepMotorTableAdapterWithStepSpeedTable(motorNo);
}

//回零计算电机加速曲线
void StepMotorTableCalcWithReturnZero(STEP_MOTOR_NO motorNo,uint8_t selectConfig,SPEED_RATIO_MODE mode,uint8_t ratio)
{
    //回零时候的步数是根据当前坐标来的
    int32_t steps = 0 - stepMotorVarArray[motorNo].var.currentCoordinate;
    //计算步进电机速度曲线
    StepMotorTableCalcSpeedTable(motorNo,selectConfig);
    //步进电机速度比例计算
    StepMotorTableCalcSpeedWithRatio(motorNo,mode,ratio);
    //电机步数分配
    StepMotorTableCalcStepTable(steps,motorNo,selectConfig);
    //去除无效速度
    StepMotorTableAdapterWithStepSpeedTable(motorNo);
}

//次序运转计算电机加速曲线
void StepMotorTableCalcWithRunAlways(STEP_MOTOR_NO motorNo,uint8_t selectConfig,SPEED_RATIO_MODE mode,uint8_t ratio)
{
    //计算步进电机速度曲线
    StepMotorTableCalcSpeedTable(motorNo,selectConfig);
    //步进电机速度比例计算
    StepMotorTableCalcSpeedWithRatio(motorNo,mode,ratio);
    //次序运转的步数分配与坐标运行的步数分配不同,他会将每一个速度点都分配一个速度
    StepMotorTableCalcStepTableWithRunAlways(motorNo,selectConfig);
    //持续运转的计算策略不需要去除无效速度
}

//计算步进电机减速曲线,加速过程的核心问题,怎么样计算减速步数表
void StepMotorTableCalcDaccTable(STEP_MOTOR_VAR* stepMotorVarPtr)
{
    uint32_t dacc2StopStepsCount = stepMotorVarPtr->attriPtr->baseConfig.dacc2StopSteps;
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
    if(stepMotorVarPtr->var.currentTableIndex >= stepMotorVarPtr->var.currentTableMaxLength)
    {
        //匀速
        stepMotorVarPtr->var.currentDaccTableMaxLength = 1;
        stepMotorVarPtr->var.currentDaccTableIndex = 0;
        //速度设置为启动速度
        stepMotorVarPtr->var.stepMotorDaccSpeedTableDirect[0] = stepMotorVarPtr->var.stepMotorSpeedTableDirect[0];
        //步数设置为减速步数
        stepMotorVarPtr->var.stepMotorDaccStepTableDirect[0] = stepMotorVarPtr->attriPtr->baseConfig.dacc2StopSteps;
        return;
    }
    //如果最大坐标序号就是1,也就是匀速运动
    if(stepMotorVarPtr->var.currentTableMaxLength == 1)
    {
        //匀速
        stepMotorVarPtr->var.currentDaccTableMaxLength = 1;
        stepMotorVarPtr->var.currentDaccTableIndex = 0;
        //速度设置为启动速度
        stepMotorVarPtr->var.stepMotorDaccSpeedTableDirect[0] = stepMotorVarPtr->var.stepMotorSpeedTableDirect[0];
        //步数设置为减速步数
        stepMotorVarPtr->var.stepMotorDaccStepTableDirect[0] = stepMotorVarPtr->attriPtr->baseConfig.dacc2StopSteps;
        return;
    }
    //如果现在处于加速过程
    if(stepMotorVarPtr->var.currentTableIndex < ((stepMotorVarPtr->var.currentTableMaxLength)/2))
    {
        //计算出对称点
        utilIndexStart = stepMotorVarPtr->var.currentTableMaxLength - stepMotorVarPtr->var.currentTableIndex -1;
    }
    else
    {
        //已经在减速过程,继续减速,只是减速过程变快
        utilIndexStart = stepMotorVarPtr->var.currentTableIndex;
    }
    //知道了从此刻速度到停止有多少个速度,看看停止步数够不够这么多点的分配,如果不够,就要进行跳格
    utilIndexCalc = stepMotorVarPtr->var.currentTableMaxLength - utilIndexStart;
    //减速点的数量比减速步数多,步数不够分,以步数作为减速依据
    if(utilIndexCalc > stepMotorVarPtr->attriPtr->baseConfig.dacc2StopSteps)
    {
        //步数不够分,必须消减速度表
        stepMotorVarPtr->var.currentDaccTableMaxLength = stepMotorVarPtr->attriPtr->baseConfig.dacc2StopSteps;
        //启动减速速度
        daccStartSpeed = stepMotorVarPtr->var.stepMotorSpeedTableDirect[utilIndexStart];
        //每一节的减速速度
        daccSpeed = daccStartSpeed - stepMotorVarPtr->var.stepMotorSpeedTableDirect[(stepMotorVarPtr->var.currentTableMaxLength)-1];
        daccSpeed /= stepMotorVarPtr->attriPtr->baseConfig.dacc2StopSteps;
        for(utilIndex = 0;utilIndex < stepMotorVarPtr->var.currentDaccTableMaxLength;utilIndex++)
        {
            //步数肯定是1
            stepMotorVarPtr->var.stepMotorDaccStepTableDirect[utilIndex] = 1;
            //速度就阶梯递减
            stepMotorVarPtr->var.stepMotorDaccSpeedTableDirect[utilIndex] = daccStartSpeed - (utilIndex*daccSpeed);
        }
        stepMotorVarPtr->var.currentDaccTableIndex = 0;
    }
    else
    {
        //步数够分
        stepMotorVarPtr->var.currentDaccTableMaxLength = utilIndexCalc;
        //先拷贝速度表
        for(utilIndex = 0;utilIndex < stepMotorVarPtr->var.currentDaccTableMaxLength;utilIndex++)
        {
            stepMotorVarPtr->var.stepMotorDaccSpeedTableDirect[utilIndex] = 
                stepMotorVarPtr->var.stepMotorSpeedTableDirect[utilIndex+utilIndexStart];
            //步数要取余数
            stepMotorVarPtr->var.stepMotorDaccStepTableDirect[utilIndex] = 
                (uint32_t)((stepMotorVarPtr->attriPtr->baseConfig.dacc2StopSteps)/(utilIndexCalc));
            //总步数降低
            dacc2StopStepsCount -= stepMotorVarPtr->var.stepMotorDaccStepTableDirect[utilIndex];
        }
        //最终多的步数,分配到尾巴上
        stepMotorVarPtr->var.stepMotorDaccStepTableDirect[utilIndex-1] += dacc2StopStepsCount;
        stepMotorVarPtr->var.currentDaccTableIndex = 0;
    }
}




//计算步进电机持续运行减速曲线1,加速过程的核心问题,怎么样计算减速步数表
void StepMotorTableCalcSlowDown1Table(STEP_MOTOR_VAR* stepMotorVarPtr)
{
    uint32_t dacc2StopStepsCount = stepMotorVarPtr->attriPtr->baseConfig.slowDown1StopSteps;
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
    if(stepMotorVarPtr->var.currentTableIndex >= stepMotorVarPtr->var.currentTableMaxLength)
    {
        //匀速
        stepMotorVarPtr->var.currentDaccTableMaxLength = 1;
        stepMotorVarPtr->var.currentDaccTableIndex = 0;
        //速度设置为启动速度
        stepMotorVarPtr->var.stepMotorDaccSpeedTableDirect[0] = stepMotorVarPtr->var.stepMotorSpeedTableDirect[0];
        //步数设置为减速步数
        stepMotorVarPtr->var.stepMotorDaccStepTableDirect[0] = stepMotorVarPtr->attriPtr->baseConfig.slowDown1StopSteps;
        return;
    } 
    //如果最大坐标序号就是1,也就是匀速运动
    if(stepMotorVarPtr->var.currentTableMaxLength == 1)
    {
        //匀速
        stepMotorVarPtr->var.currentDaccTableMaxLength = 1;
        stepMotorVarPtr->var.currentDaccTableIndex = 0;
        //速度设置为启动速度
        stepMotorVarPtr->var.stepMotorDaccSpeedTableDirect[0] = stepMotorVarPtr->var.stepMotorSpeedTableDirect[0];
        //步数设置为减速步数
        stepMotorVarPtr->var.stepMotorDaccStepTableDirect[0] = stepMotorVarPtr->attriPtr->baseConfig.slowDown1StopSteps;
        return;
    }
    //如果现在处于加速过程
    if(stepMotorVarPtr->var.currentTableIndex < ((stepMotorVarPtr->var.currentTableMaxLength)/2))
    {
        //计算出对称点
        utilIndexStart = stepMotorVarPtr->var.currentTableMaxLength - stepMotorVarPtr->var.currentTableIndex -1;
    }
    else
    {
        //已经在减速过程,继续减速,只是减速过程变快
        utilIndexStart = stepMotorVarPtr->var.currentTableIndex;
    }
    //知道了从此刻速度到停止有多少个速度,看看停止步数够不够这么多点的分配,如果不够,就要进行跳格
    utilIndexCalc = stepMotorVarPtr->var.currentTableMaxLength - utilIndexStart;
    //减速点的数量比减速步数多,步数不够分,以步数作为减速依据
    if(utilIndexCalc > stepMotorVarPtr->attriPtr->baseConfig.slowDown1StopSteps)
    {
        //步数不够分,必须消减速度表
        stepMotorVarPtr->var.currentDaccTableMaxLength = stepMotorVarPtr->attriPtr->baseConfig.slowDown1StopSteps;
        //启动减速速度
        daccStartSpeed = stepMotorVarPtr->var.stepMotorSpeedTableDirect[utilIndexStart];
        //每一节的减速速度
        daccSpeed = daccStartSpeed - stepMotorVarPtr->var.stepMotorSpeedTableDirect[(stepMotorVarPtr->var.currentTableMaxLength)-1];
        daccSpeed /= stepMotorVarPtr->attriPtr->baseConfig.slowDown1StopSteps;
        for(utilIndex = 0;utilIndex < stepMotorVarPtr->var.currentDaccTableMaxLength;utilIndex++)
        {
            //步数肯定是1
            stepMotorVarPtr->var.stepMotorDaccStepTableDirect[utilIndex] = 1;
            //速度就阶梯递减
            stepMotorVarPtr->var.stepMotorDaccSpeedTableDirect[utilIndex] = daccStartSpeed - (utilIndex*daccSpeed);
        }
        stepMotorVarPtr->var.currentDaccTableIndex = 0;
    }
    else
    {
        //步数够分
        stepMotorVarPtr->var.currentDaccTableMaxLength = utilIndexCalc;
        //先拷贝速度表
        for(utilIndex = 0;utilIndex < stepMotorVarPtr->var.currentDaccTableMaxLength;utilIndex++)
        {
            stepMotorVarPtr->var.stepMotorDaccSpeedTableDirect[utilIndex] = 
                stepMotorVarPtr->var.stepMotorSpeedTableDirect[utilIndex+utilIndexStart];
            //步数要取余数
            stepMotorVarPtr->var.stepMotorDaccStepTableDirect[utilIndex] = 
                (uint32_t)((stepMotorVarPtr->attriPtr->baseConfig.slowDown1StopSteps)/(utilIndexCalc));
            //总步数降低
            dacc2StopStepsCount -= stepMotorVarPtr->var.stepMotorDaccStepTableDirect[utilIndex];
        }
        //最终多的步数,分配到尾巴上
        stepMotorVarPtr->var.stepMotorDaccStepTableDirect[utilIndex-1] += dacc2StopStepsCount;
        stepMotorVarPtr->var.currentDaccTableIndex = 0;
    }
}
//计算步进电机持续运行减速曲线2,加速过程的核心问题,怎么样计算减速步数表
void StepMotorTableCalcSlowDown2Table(STEP_MOTOR_VAR* stepMotorVarPtr)
{
    uint32_t dacc2StopStepsCount = stepMotorVarPtr->attriPtr->baseConfig.slowDown2StopSteps;
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
    if(stepMotorVarPtr->var.currentTableIndex >= stepMotorVarPtr->var.currentTableMaxLength)
    {
        //匀速
        stepMotorVarPtr->var.currentDaccTableMaxLength = 1;
        stepMotorVarPtr->var.currentDaccTableIndex = 0;
        //速度设置为启动速度
        stepMotorVarPtr->var.stepMotorDaccSpeedTableDirect[0] = stepMotorVarPtr->var.stepMotorSpeedTableDirect[0];
        //步数设置为减速步数
        stepMotorVarPtr->var.stepMotorDaccStepTableDirect[0] = stepMotorVarPtr->attriPtr->baseConfig.slowDown2StopSteps;
        return;
    } 
    //如果最大坐标序号就是1,也就是匀速运动
    if(stepMotorVarPtr->var.currentTableMaxLength == 1)
    {
        //匀速
        stepMotorVarPtr->var.currentDaccTableMaxLength = 1;
        stepMotorVarPtr->var.currentDaccTableIndex = 0;
        //速度设置为启动速度
        stepMotorVarPtr->var.stepMotorDaccSpeedTableDirect[0] = stepMotorVarPtr->var.stepMotorSpeedTableDirect[0];
        //步数设置为减速步数
        stepMotorVarPtr->var.stepMotorDaccStepTableDirect[0] = stepMotorVarPtr->attriPtr->baseConfig.slowDown2StopSteps;
        return;
    }
    //如果现在处于加速过程
    if(stepMotorVarPtr->var.currentTableIndex < ((stepMotorVarPtr->var.currentTableMaxLength)/2))
    {
        //计算出对称点
        utilIndexStart = stepMotorVarPtr->var.currentTableMaxLength - stepMotorVarPtr->var.currentTableIndex -1;
    }
    else
    {
        //已经在减速过程,继续减速,只是减速过程变快
        utilIndexStart = stepMotorVarPtr->var.currentTableIndex;
    }
    //知道了从此刻速度到停止有多少个速度,看看停止步数够不够这么多点的分配,如果不够,就要进行跳格
    utilIndexCalc = stepMotorVarPtr->var.currentTableMaxLength - utilIndexStart;
    //减速点的数量比减速步数多,步数不够分,以步数作为减速依据
    if(utilIndexCalc > stepMotorVarPtr->attriPtr->baseConfig.slowDown2StopSteps)
    {
        //步数不够分,必须消减速度表
        stepMotorVarPtr->var.currentDaccTableMaxLength = stepMotorVarPtr->attriPtr->baseConfig.slowDown2StopSteps;
        //启动减速速度
        daccStartSpeed = stepMotorVarPtr->var.stepMotorSpeedTableDirect[utilIndexStart];
        //每一节的减速速度
        daccSpeed = daccStartSpeed - stepMotorVarPtr->var.stepMotorSpeedTableDirect[(stepMotorVarPtr->var.currentTableMaxLength)-1];
        daccSpeed /= stepMotorVarPtr->attriPtr->baseConfig.slowDown2StopSteps;
        for(utilIndex = 0;utilIndex < stepMotorVarPtr->var.currentDaccTableMaxLength;utilIndex++)
        {
            //步数肯定是1
            stepMotorVarPtr->var.stepMotorDaccStepTableDirect[utilIndex] = 1;
            //速度就阶梯递减
            stepMotorVarPtr->var.stepMotorDaccSpeedTableDirect[utilIndex] = daccStartSpeed - (utilIndex*daccSpeed);
        }
        stepMotorVarPtr->var.currentDaccTableIndex = 0;
    }
    else
    {
        //步数够分
        stepMotorVarPtr->var.currentDaccTableMaxLength = utilIndexCalc;
        //先拷贝速度表
        for(utilIndex = 0;utilIndex < stepMotorVarPtr->var.currentDaccTableMaxLength;utilIndex++)
        {
            stepMotorVarPtr->var.stepMotorDaccSpeedTableDirect[utilIndex] = 
                stepMotorVarPtr->var.stepMotorSpeedTableDirect[utilIndex+utilIndexStart];
            //步数要取余数
            stepMotorVarPtr->var.stepMotorDaccStepTableDirect[utilIndex] = 
                (uint32_t)((stepMotorVarPtr->attriPtr->baseConfig.slowDown2StopSteps)/(utilIndexCalc));
            //总步数降低
            dacc2StopStepsCount -= stepMotorVarPtr->var.stepMotorDaccStepTableDirect[utilIndex];
        }
        //最终多的步数,分配到尾巴上
        stepMotorVarPtr->var.stepMotorDaccStepTableDirect[utilIndex-1] += dacc2StopStepsCount;
        stepMotorVarPtr->var.currentDaccTableIndex = 0;
    }
}

