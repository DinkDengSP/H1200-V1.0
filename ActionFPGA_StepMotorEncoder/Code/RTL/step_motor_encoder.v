/************************************************************************ 
 ** Author       : 邓小俊
 ** Date         : 2024-01-19 13:30:05
 ** Description  : When I Has Free Time ,I Will Write Description Below.
 ** description  : 
 ** LastEditors  : 邓小俊
 ** LastEditTime : 2024-01-25 13:36:21
 ** Email: xiaojun.deng@aliyun.com
 ** Copyright (c) 2024 by 公司名称, All Rights Reserved. 
 ************************************************************************/
module step_motor_encoder
#(
    parameter SYS_CLK_FREQ                      = 32'd60_000_000,//输入时钟
    parameter BASE_REG_INIT_VAL                 = 32'h0000_0000,//基础的寄存器初始值
    parameter FILTER_CLK_SET                    = 6'd9//滤波器设定,多少位滤波
)
(
    //通用时钟复位
    input           sys_clk  ,//输入时钟
    input           sys_rst_n,//复位,低电平有效
    //输入的编码器采样信号
    input           motor_encoder_a,//输入信号A相
    input           motor_encoder_b,//输入信号B相
    //编码器有效信号
    input           encoder_a_valid_level,//编码器A相有效信号
    input           encoder_b_valid_level,//编码器B相有效信号
    //编码器启停
    input           encoder_start,
    //编码器写入信号
    input[31:0]     encoder_position_calc_factor_fp,//编码器计数值计算到实际电机坐标的系数
    input           encoder_position_clear_singal,//清空编码器计数值信号
    //输出的编码器计数值
    output[31:0]    encoder_position
);

/*************************************************************************************/
/*********************************参数定义*********************************************/
/*************************************************************************************/



/*************************************************************************************/    
/**********************************网线定义********************************************/
/*************************************************************************************/
//编码器采样信号线,真实逻辑值
wire motor_encoder_a_real_logic_wire;
wire motor_encoder_b_real_logic_wire;

//编码器计数值的增长控制和方向控制
wire motor_encoder_count_change_flag_wire; 
wire motor_encoder_direction_current_wire;
/*************************************************************************************/
/*********************************寄存器定义*******************************************/
/*************************************************************************************/
//编码器采样真实逻辑值的滤波寄存器
reg[FILTER_CLK_SET-1:0] motor_encoder_a_real_logic_filter_reg;
reg[FILTER_CLK_SET-1:0] motor_encoder_b_real_logic_filter_reg;

//编码器信号真实逻辑值经过滤波之后的输出
reg motor_encoder_a_real_logic_reg;
reg motor_encoder_b_real_logic_reg;

//编码器信号的采样寄存器,采样的是真实逻辑值滤波之后的输出
reg [2:0] motor_encoder_a_sample_delay_reg;
reg [2:0] motor_encoder_b_sample_delay_reg;

//编码器写入信号采样
reg[1:0] encoder_position_clear_singal_reg;

//当前编码器的计数值
//实际计数值,定点数
reg signed [31:0] motor_encoder_position_int;
//转换成为浮点型计数值
wire signed [31:0] motor_encoder_position_fp;
//计算出来的浮点型计数值
wire signed [31:0] motor_encoder_position_result_fp;
//转换出来的定点型计数值
wire signed [31:0] motor_encoder_position_result_int;
/*************************************************************************************/
/**********************************IP实例化********************************************/
/*************************************************************************************/
//内部计数值定点转浮点
motor_encoder_int_to_fp motor_encoder_int_to_fp_inst
(
    .clk    (sys_clk),                  //时钟信号 
    .areset (~sys_rst_n),               //复位信号
    .en     (encoder_start),            //转换使能
    .a      (motor_encoder_position_int),    //输入定点数  
    .q      (motor_encoder_position_fp)      //输出浮点数 
);

//内部计算浮点型编码器坐标
motor_encoder_mul_pos motor_encoder_mul_pos_inst
(
    .clk    (sys_clk),    //时钟信号
    .areset (~sys_rst_n), //复位信号
    .en     (encoder_start), //转换使能    
    .a      (motor_encoder_position_fp),//浮点型的计数值     
    .b      (encoder_position_calc_factor_fp),//计算系数      
    .q      (motor_encoder_position_result_fp)//计算结果      
);

//计算结果浮点转定点
motor_encoder_fp_to_int motor_encoder_fp_to_int_inst
(
    .clk    (sys_clk),    //时钟信号
    .areset (~sys_rst_n), //复位信号
    .en     (encoder_start), //转换使能    
    .a      (motor_encoder_position_result_fp),//浮点型的计数值     
    .q      (motor_encoder_position_result_int)  //转换出来的定点型计数值  
);

/*************************************************************************************/
/**********************************组合逻辑代码****************************************/
/*************************************************************************************/
//编码器信号采样,根据有效电平进行电平反转,1有效则是原始值,0有效则信号反转
assign motor_encoder_a_real_logic_wire = (encoder_a_valid_level == 1'b1)?motor_encoder_a:~motor_encoder_a;
assign motor_encoder_b_real_logic_wire = (encoder_b_valid_level == 1'b1)?motor_encoder_b:~motor_encoder_b;

//输出寄存器
assign encoder_position = (encoder_start == 1'b0)?32'd0:motor_encoder_position_result_int;

//编码器的增长控制和方向控制
assign motor_encoder_count_change_flag_wire = motor_encoder_a_sample_delay_reg[1] ^ motor_encoder_a_sample_delay_reg[2] ^ motor_encoder_b_sample_delay_reg[1] ^ motor_encoder_b_sample_delay_reg[2]; 
assign motor_encoder_direction_current_wire = motor_encoder_a_sample_delay_reg[1] ^ motor_encoder_b_sample_delay_reg[2];
/*************************************************************************************/
/**********************************时序逻辑代码****************************************/
/*************************************************************************************/
//A相信号采集
always @(posedge sys_clk or negedge sys_rst_n) 
begin 
    if (1'b0 == sys_rst_n) 
    begin 
        motor_encoder_a_real_logic_filter_reg <= BASE_REG_INIT_VAL[FILTER_CLK_SET-1:0];
    end
    else 
    begin
        motor_encoder_a_real_logic_filter_reg <= {motor_encoder_a_real_logic_filter_reg[FILTER_CLK_SET-2:0],motor_encoder_a_real_logic_wire};
    end
end

//B相信号采集
always @(posedge sys_clk or negedge sys_rst_n) 
begin 
    if (1'b0 == sys_rst_n) 
    begin 
        motor_encoder_b_real_logic_filter_reg <= BASE_REG_INIT_VAL[FILTER_CLK_SET-1:0];
    end
    else 
    begin
        motor_encoder_b_real_logic_filter_reg <= {motor_encoder_b_real_logic_filter_reg[FILTER_CLK_SET-2:0],motor_encoder_b_real_logic_wire};
    end
end

//A相滤波之后的真实信号
always @(posedge sys_clk or negedge sys_rst_n) 
begin 
    if (1'b0 == sys_rst_n) 
    begin 
        motor_encoder_a_real_logic_reg <= 1'b0;
    end
    else 
    begin
        if((&motor_encoder_a_real_logic_filter_reg) == 1'b1)begin
            //采样到多个高电平,信号为1
            motor_encoder_a_real_logic_reg <= 1'b1;
        end
        else if((|motor_encoder_a_real_logic_filter_reg) == 1'b0) begin
            //采样到多个低电平,信号为0
            motor_encoder_a_real_logic_reg <= 1'b0;
        end
        else begin
            //保持不变
            motor_encoder_a_real_logic_reg <= motor_encoder_a_real_logic_reg;
        end
    end
end

//B相
always @(posedge sys_clk or negedge sys_rst_n) 
begin 
    if (1'b0 == sys_rst_n) 
    begin 
        motor_encoder_b_real_logic_reg <= 1'b0;
    end
    else 
    begin
        if((&motor_encoder_b_real_logic_filter_reg) == 1'b1)begin
            //采样到多个高电平,信号为1
            motor_encoder_b_real_logic_reg <= 1'b1;
        end
        else if((|motor_encoder_b_real_logic_filter_reg) == 1'b0) begin
            //采样到多个低电平,信号为0
            motor_encoder_b_real_logic_reg <= 1'b0;
        end
        else begin
            //保持不变
            motor_encoder_b_real_logic_reg <= motor_encoder_b_real_logic_reg;
        end
    end
end

//A相采样序列
always @(posedge sys_clk or negedge sys_rst_n) 
begin
    if (1'b0 == sys_rst_n) 
    begin 
        motor_encoder_a_sample_delay_reg <= 3'b000;
    end
    else begin
        motor_encoder_a_sample_delay_reg <= {motor_encoder_a_sample_delay_reg[1:0],motor_encoder_a_real_logic_reg};
    end
end

//B相采样序列
always @(posedge sys_clk or negedge sys_rst_n) 
begin
    if (1'b0 == sys_rst_n) 
    begin 
        motor_encoder_b_sample_delay_reg <= 3'b000;
    end
    else begin
        motor_encoder_b_sample_delay_reg <= {motor_encoder_b_sample_delay_reg[1:0],motor_encoder_b_real_logic_reg};
    end
end

//采样编码器清空信号
always @(posedge sys_clk or negedge sys_rst_n) 
begin
    if (1'b0 == sys_rst_n) 
    begin 
        //初始化复位
        encoder_position_clear_singal_reg <= 2'b00;
    end
    else begin
        //信号采样
        encoder_position_clear_singal_reg <= {encoder_position_clear_singal_reg[0],encoder_position_clear_singal};
    end
end

//编码器位置坐标信号更新逻辑
always @(posedge sys_clk or negedge sys_rst_n) 
begin
    if (1'b0 == sys_rst_n) 
    begin 
        motor_encoder_position_int <= 32'd0;
    end
    else begin
        if(2'b01 == encoder_position_clear_singal_reg) begin
            //清空计数值
            motor_encoder_position_int <= 32'd0;
        end
        else if(encoder_start == 1'b1) begin
            //查看电机是否使能
            if(motor_encoder_count_change_flag_wire == 1'b1) begin
                //产生触发信号,查看方向
                if(motor_encoder_direction_current_wire == 1'b1) begin
                    //正方向,坐标增加
                    motor_encoder_position_int <= motor_encoder_position_int + 1'd1;
                end
                else begin
                    //负方向,坐标减少
                    motor_encoder_position_int <= motor_encoder_position_int - 1'd1;
                end
            end
            else begin
                //保持不变
                motor_encoder_position_int <= motor_encoder_position_int;
            end
        end
        else begin
            //保持不变
            motor_encoder_position_int <= motor_encoder_position_int;
        end
    end
end


endmodule	






