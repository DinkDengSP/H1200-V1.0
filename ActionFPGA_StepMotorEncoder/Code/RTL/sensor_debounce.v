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
module sensor_debounce
#(
    parameter SYS_CLK_FREQ                      = 32'd50_000_000,//输入时钟
    parameter DEBOUNCE_CLK                      = 32'd49
)
(
    //通用时钟复位
    input           sys_clk  ,          //输入时钟
    input           sys_rst_n,          //复位,低电平有效
    //输入信号以及输入信号有效状态
    input           sensor_input,       //输入信号
    input           sensor_valid_level, //输入信号有效电平
    //输出传感器消抖后的状态
    output reg      sensor_debounce_val//输出信号当前状态
);

/*************************************************************************************/
/*********************************参数定义*********************************************/
/*************************************************************************************/
localparam SENSOR_STATE_IDLE = 2'b01;
localparam SENSOR_STATE_WAIT = 2'b10;

/*************************************************************************************/    
/**********************************网线定义********************************************/
/*************************************************************************************/

/*************************************************************************************/
/*********************************寄存器定义*******************************************/
/*************************************************************************************/
reg real_sensor_val_reg;
reg deb_sensor_val_reg;
reg[31:0] clk_cnt;
reg[1:0] sensor_state;
/*************************************************************************************/
/**********************************组合逻辑代码****************************************/
/*************************************************************************************/

/*************************************************************************************/
/**********************************时序逻辑代码****************************************/
/*************************************************************************************/
//外部信号实时采样
always @(posedge sys_clk or negedge sys_rst_n) 
begin
    if (1'b0 == sys_rst_n) begin 
        //初始化输出状态无效
        real_sensor_val_reg <= (~sensor_valid_level);
    end
    else begin
        real_sensor_val_reg <= sensor_input;
    end
end

//时钟计数器,外部信号和采样信号不等的时候开始计数
always @(posedge sys_clk or negedge sys_rst_n) 
begin
    if (1'b0 == sys_rst_n) begin 
        //初始化计时器清零
        clk_cnt <= 32'd0;
    end
    else begin
        if(sensor_state == SENSOR_STATE_WAIT)begin
            if(clk_cnt == DEBOUNCE_CLK) begin
                //计数器已经到达滤波值
                clk_cnt <= clk_cnt;
            end
            else begin
                //计数器增加
                clk_cnt <= clk_cnt + 1'b1;
            end
        end
        else begin
            //空闲状态无变化
            clk_cnt <= 32'd0;
        end
    end
end

//状态机更新机制
always @(posedge sys_clk or negedge sys_rst_n) 
begin 
    if (1'b0 == sys_rst_n) begin 
        //初始化状态机空闲
        sensor_state <= SENSOR_STATE_IDLE;
    end
    else begin
        case(sensor_state)
            SENSOR_STATE_IDLE : begin
                if(deb_sensor_val_reg != real_sensor_val_reg) begin
                    sensor_state <= SENSOR_STATE_WAIT;
                end
                else begin
                    sensor_state <= SENSOR_STATE_WAIT;
                end
                deb_sensor_val_reg <= deb_sensor_val_reg;
            end
            SENSOR_STATE_WAIT : begin
                if(deb_sensor_val_reg == real_sensor_val_reg) begin
                    sensor_state <= SENSOR_STATE_IDLE;
                    deb_sensor_val_reg <= deb_sensor_val_reg;
                end
                else if(clk_cnt == DEBOUNCE_CLK) begin
                    sensor_state <= SENSOR_STATE_IDLE;
                    deb_sensor_val_reg <= real_sensor_val_reg;
                end
                else begin
                    deb_sensor_val_reg <= deb_sensor_val_reg;
                end
            end
        endcase
    end
end


//信号根据当前滤波值和有效值转换为输出值
always @(posedge sys_clk or negedge sys_rst_n) 
begin 
    if (1'b0 == sys_rst_n) begin 
        //初始化输出状态无效
        sensor_debounce_val <= (~sensor_valid_level);
    end
    else begin
        if(1'b1 == sensor_valid_level) begin
            //高电平有效
            sensor_debounce_val <= deb_sensor_val_reg;
        end
        else begin
            //低电平有效
            sensor_debounce_val <= (~deb_sensor_val_reg);
        end
    end
end

endmodule








