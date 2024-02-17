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
module led_breath
#(
    parameter       CLK_FREQ        = 32'd50_000_000;//系统时钟参数定义
    parameter       LED_VALID_LEVEL = 1'b0           //LED有效电平
)
(
    input wire      sys_clk ,       //系统时钟
    input wire      sys_rst_n ,     //全局复位
    output reg      led_out         //输出信号，控制led灯
);

/**************************************参数定义**************************************/
localparam CNT_1US_MAX = (CLK_FREQ/32'd1000_000) -1;
localparam CNT_1MS_MAX = 10'd999;
localparam CNT_1S_MAX = 10'd999;
localparam LED_INVALID_LEVEL = ~(LED_VALID_LEVEL);

/**************************************寄存器定义**************************************/
reg [9:0] cnt_1us ;
reg [9:0] cnt_1ms ;
reg [9:0] cnt_1s ;
reg cnt_1s_en ;

/******************************************************************************/
/********************************MainCode**************************************/
/******************************************************************************/
//cnt_1us:1us计数器
always@(posedge sys_clk or negedge sys_rst_n) begin
    if(sys_rst_n == 1'b0)
        cnt_1us <= 10'd0;
    else if(cnt_1us == CNT_1US_MAX)
        cnt_1us <= 10'd0;
    else
        cnt_1us <= cnt_1us + 1'b1;
end


//cnt_1ms:1ms计数器
always@(posedge sys_clk or negedge sys_rst_n) begin
    if(sys_rst_n == 1'b0)
        cnt_1ms <= 10'd0;
    else if(cnt_1ms == CNT_1MS_MAX && cnt_1us == CNT_1US_MAX)
        cnt_1ms <= 10'd0;
    else if(cnt_1us == CNT_1US_MAX)
        cnt_1ms <= cnt_1ms + 1'b1;
    else
        cnt_1ms <= cnt_1ms;
end


//cnt_1s:1s计数器
always@(posedge sys_clk or negedge sys_rst_n) begin
    if(sys_rst_n == 1'b0)
        cnt_1s <= 10'd0;
    else if(cnt_1s == CNT_1S_MAX && cnt_1ms == CNT_1MS_MAX&& cnt_1us == CNT_1US_MAX)
        cnt_1s <= 10'd0;
    else if(cnt_1ms == CNT_1MS_MAX && cnt_1us == CNT_1US_MAX)
        cnt_1s <= cnt_1s + 1'b1;
    else
        cnt_1s <= cnt_1s;
end

//cnt_1s_en:1s计数器标志信号
always@(posedge sys_clk or negedge sys_rst_n) begin
    if(sys_rst_n == 1'b0)
        cnt_1s_en <= 1'b0;
    else if(cnt_1s == CNT_1S_MAX && cnt_1ms == CNT_1MS_MAX&& cnt_1us == CNT_1US_MAX)
        cnt_1s_en <= ~cnt_1s_en;
    else
        cnt_1s_en <= cnt_1s_en;
end


//led_out:输出信号连接到外部的led灯
//led_out为低电平时间时cnt_1s计数器的计数值总是小于cnt_1ms计数器的计数值，
//这简直就是太完美的发现了，我们让cnt_1s计数器的计数值大于cnt_1ms计数器的计数值
//就可以实现每一个1ms的时间小段的低电平持续时间都不一样且渐渐增加的效果。同理呼吸灯从 
//亮到灭正好是相反的过程
always@(posedge sys_clk or negedge sys_rst_n) begin
    if(sys_rst_n == 1'b0)
        led_out <= LED_INVALID_LEVEL;
    else if((cnt_1s_en == 1'b1 && cnt_1ms < cnt_1s)||(cnt_1s_en == 1'b0 && cnt_1ms > cnt_1s))
        led_out <= LED_INVALID_LEVEL;
    else
        led_out <= LED_VALID_LEVEL;
end

endmodule








