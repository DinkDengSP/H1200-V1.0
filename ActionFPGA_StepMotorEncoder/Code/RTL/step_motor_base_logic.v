/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-19 13:30:05
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-19 13:57:44
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
module step_motor_base_logic 
(
    //通用时钟复位
    input           sys_clk  ,      //输入时钟
    input           sys_rst_n,      //复位,低电平有效
    //外部控制信号
    input           trig_start,     //电机启动信号
    input[31:0]     spd_cnt_period, //输入的脉冲周期
    input           step_motor_pluse_idle_level,//脉冲引脚空闲状态设定 
    //外部输出信号
    output reg      busy_state,     //模块正忙状态
    output reg      pluse_out       //脉冲输出
);


/*************************************************************************************/
/*********************************参数定义*********************************************/
/*************************************************************************************/
localparam MODULE_STATE_IDLE = 3'b001;//模块空闲
localparam MODULE_STATE_LOW  = 3'b010;//模块输出脉冲低有效
localparam MODULE_STATE_HIGH = 3'b100;//模块输出脉冲高有效


/*************************************************************************************/    
/**********************************网线定义********************************************/
/*************************************************************************************/



/*************************************************************************************/
/*********************************寄存器定义*******************************************/
/*************************************************************************************/
//模块状态
reg[2:0] module_state;
//启动检测序列
reg[1:0] trig_start_delay_reg;
//计数器
reg[31:0] spd_cnt_period_reg;
//内部时钟计数器
reg[31:0] cnt_clk_reg;

/*************************************************************************************/
/**********************************组合逻辑代码****************************************/
/*************************************************************************************/



/*************************************************************************************/
/**********************************时序逻辑代码****************************************/
/*************************************************************************************/
//检测启动脉冲
always @(posedge sys_clk or negedge sys_rst_n) 
begin
    if(1'b0 == sys_rst_n) begin
        //启动脉冲清空
        trig_start_delay_reg <= 2'b00;
    end
    else begin
        trig_start_delay_reg <= {trig_start_delay_reg[0],trig_start};
    end
end

//锁存计数器
always @(posedge sys_clk or negedge sys_rst_n) 
begin
    if(1'b0 == sys_rst_n) begin
        //计数器清零
        spd_cnt_period_reg <= 32'd0;
    end
    else begin
        if(MODULE_STATE_IDLE == module_state) begin
            //空闲状态下才能修改计数器
            if(2'b01 == trig_start_delay_reg) begin
                //锁存计数器
                spd_cnt_period_reg <= spd_cnt_period;
            end
            else begin
                //计数器保持
                spd_cnt_period_reg <= spd_cnt_period_reg;
            end
        end
        else begin
            //计数器保持
            spd_cnt_period_reg <= spd_cnt_period_reg;
        end
    end
end

//状态机转换机制
always @(posedge sys_clk or negedge sys_rst_n) 
begin
    if(1'b0 == sys_rst_n) begin
        //复位处于空闲状态
        module_state <= MODULE_STATE_IDLE;
    end
    else begin
        case(module_state)
            MODULE_STATE_IDLE: begin
                //空闲状态,检测到启动脉冲切入LOW状态
                if(2'b01 == trig_start_delay_reg) begin
                    module_state <= MODULE_STATE_LOW;
                end
                else begin
                    module_state <= module_state;
                end
            end
            MODULE_STATE_LOW: begin
                //检测到计数值匹配,切入高
                if(cnt_clk_reg[30:0] == spd_cnt_period_reg[31:1]) begin
                    module_state <= MODULE_STATE_HIGH;
                end
                else begin
                    module_state <= module_state;
                end
            end
            MODULE_STATE_HIGH: begin
                //检测到计数值达到最大,切入空闲
                if(cnt_clk_reg[31:0] == (spd_cnt_period_reg[31:0] -1)) begin
                    module_state <= MODULE_STATE_IDLE;
                end
                else begin
                    module_state <= module_state;
                end
            end
            default: begin
                module_state <= module_state;
            end
        endcase
    end
end


//状态机下执行任务
always @(posedge sys_clk or negedge sys_rst_n) 
begin
    if(1'b0 == sys_rst_n) begin
        //复位处于空闲状态
        cnt_clk_reg <= 32'd0;
        pluse_out <= step_motor_pluse_idle_level;
    end
    else begin
        case(module_state)
            MODULE_STATE_IDLE: begin
                //时间计数清零
                cnt_clk_reg <= 32'd0;
                //忙状态清零
                busy_state <= 1'b0;
                //输出空闲脉冲
                pluse_out <= step_motor_pluse_idle_level;
            end
            MODULE_STATE_LOW: begin
                //时间增加
                cnt_clk_reg <= cnt_clk_reg + 1'b1;
                //忙状态置1
                busy_state <= 1'b1;
                //输出空闲脉冲
                pluse_out <= step_motor_pluse_idle_level;
            end
            MODULE_STATE_HIGH: begin
                //时间增加
                cnt_clk_reg <= cnt_clk_reg + 1'b1;
                //忙状态置1
                busy_state <= 1'b1;
                //输出高脉冲
                pluse_out <= (~step_motor_pluse_idle_level);
            end
            default begin
                //时间计数清零
                cnt_clk_reg <= 32'd0;
                //忙状态清零
                busy_state <= 1'b0;
                //输出空闲脉冲
                pluse_out <= step_motor_pluse_idle_level;
            end
        endcase
    end
end

    
endmodule









