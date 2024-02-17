package require ::quartus::project

#########################################系统时钟与复位线##########################################
#系统输入晶振 50M
set_location_assignment PIN_G1 -to sys_clk_50m
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to sys_clk_50m
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to sys_clk_50m
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to sys_clk_50m

#系统输入晶振 24M
set_location_assignment PIN_G22 -to sys_clk_24m
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to sys_clk_24m
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to sys_clk_24m
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to sys_clk_24m

#系统输入复位端口
set_location_assignment PIN_N22 -to sys_rst_n
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to sys_rst_n
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to sys_rst_n
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to sys_rst_n

#########################################呼吸LED##########################################
#系统呼吸灯
set_location_assignment PIN_C13 -to led_breath_1
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to led_breath_1
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to led_breath_1
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to led_breath_1

#########################################FSMC控制线##########################################
#FSMC写入使能信号
set_location_assignment PIN_Y15 -to fsmc_comm_nwe
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to fsmc_comm_nwe
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to fsmc_comm_nwe
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to fsmc_comm_nwe

#FSMC读取使能信号
set_location_assignment PIN_W15 -to fsmc_comm_noe
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to fsmc_comm_noe
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to fsmc_comm_noe
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to fsmc_comm_noe

#FSMC片选
set_location_assignment PIN_Y14 -to fsmc_comm_ne
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to fsmc_comm_ne
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to fsmc_comm_ne
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to fsmc_comm_ne

#FSMC nwait
set_location_assignment PIN_W14 -to fsmc_comm_nwait
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to fsmc_comm_nwait
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to fsmc_comm_nwait
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to fsmc_comm_nwait

#FSMC NADV
set_location_assignment PIN_W13 -to fsmc_comm_nadv
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to fsmc_comm_nadv
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to fsmc_comm_nadv
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to fsmc_comm_nadv

#########################################FSMC地址线##########################################
#FSMC地址线
set_location_assignment PIN_M16 -to fsmc_comm_addr[0]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to fsmc_comm_addr[0]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to fsmc_comm_addr[0]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to fsmc_comm_addr[0]

#FSMC地址线
set_location_assignment PIN_M19 -to fsmc_comm_addr[1]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to fsmc_comm_addr[1]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to fsmc_comm_addr[1]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to fsmc_comm_addr[1]

#FSMC地址线
set_location_assignment PIN_M20 -to fsmc_comm_addr[2]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to fsmc_comm_addr[2]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to fsmc_comm_addr[2]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to fsmc_comm_addr[2]

#FSMC地址线
set_location_assignment PIN_K19 -to fsmc_comm_addr[3]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to fsmc_comm_addr[3]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to fsmc_comm_addr[3]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to fsmc_comm_addr[3]

#FSMC地址线
set_location_assignment PIN_T19 -to fsmc_comm_addr[4]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to fsmc_comm_addr[4]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to fsmc_comm_addr[4]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to fsmc_comm_addr[4]

#FSMC地址线
set_location_assignment PIN_T20 -to fsmc_comm_addr[5]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to fsmc_comm_addr[5]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to fsmc_comm_addr[5]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to fsmc_comm_addr[5]

#FSMC地址线
set_location_assignment PIN_R19 -to fsmc_comm_addr[6]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to fsmc_comm_addr[6]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to fsmc_comm_addr[6]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to fsmc_comm_addr[6]

#FSMC地址线
set_location_assignment PIN_R20 -to fsmc_comm_addr[7]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to fsmc_comm_addr[7]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to fsmc_comm_addr[7]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to fsmc_comm_addr[7]

#FSMC地址线
set_location_assignment PIN_U20 -to fsmc_comm_addr[8]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to fsmc_comm_addr[8]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to fsmc_comm_addr[8]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to fsmc_comm_addr[8]

#FSMC地址线
set_location_assignment PIN_U19 -to fsmc_comm_addr[9]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to fsmc_comm_addr[9]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to fsmc_comm_addr[9]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to fsmc_comm_addr[9]

#FSMC地址线
set_location_assignment PIN_N20 -to fsmc_comm_addr[10]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to fsmc_comm_addr[10]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to fsmc_comm_addr[10]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to fsmc_comm_addr[10]

#FSMC地址线
set_location_assignment PIN_T18 -to fsmc_comm_addr[11]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to fsmc_comm_addr[11]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to fsmc_comm_addr[11]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to fsmc_comm_addr[11]

#FSMC地址线
set_location_assignment PIN_W20 -to fsmc_comm_addr[12]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to fsmc_comm_addr[12]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to fsmc_comm_addr[12]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to fsmc_comm_addr[12]

#FSMC地址线
set_location_assignment PIN_U16 -to fsmc_comm_addr[13]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to fsmc_comm_addr[13]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to fsmc_comm_addr[13]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to fsmc_comm_addr[13]

#FSMC地址线
set_location_assignment PIN_P20 -to fsmc_comm_addr[14]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to fsmc_comm_addr[14]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to fsmc_comm_addr[14]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to fsmc_comm_addr[14]

#FSMC地址线
set_location_assignment PIN_N19 -to fsmc_comm_addr[15]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to fsmc_comm_addr[15]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to fsmc_comm_addr[15]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to fsmc_comm_addr[15]

#########################################FSMC数据线##########################################
#FSMC数据线
set_location_assignment PIN_Y21 -to fsmc_comm_data[0]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to fsmc_comm_data[0]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to fsmc_comm_data[0]
#端口方向设置
set_instance_assignment -name DIRECTION "Bidir" -to fsmc_comm_data[0]

#FSMC数据线
set_location_assignment PIN_W22 -to fsmc_comm_data[1]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to fsmc_comm_data[1]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to fsmc_comm_data[1]
#端口方向设置
set_instance_assignment -name DIRECTION "Bidir" -to fsmc_comm_data[1]

#FSMC数据线
set_location_assignment PIN_W21 -to fsmc_comm_data[2]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to fsmc_comm_data[2]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to fsmc_comm_data[2]
#端口方向设置
set_instance_assignment -name DIRECTION "Bidir" -to fsmc_comm_data[2]

#FSMC数据线
set_location_assignment PIN_V22 -to fsmc_comm_data[3]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to fsmc_comm_data[3]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to fsmc_comm_data[3]
#端口方向设置
set_instance_assignment -name DIRECTION "Bidir" -to fsmc_comm_data[3]

#FSMC数据线
set_location_assignment PIN_V21 -to fsmc_comm_data[4]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to fsmc_comm_data[4]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to fsmc_comm_data[4]
#端口方向设置
set_instance_assignment -name DIRECTION "Bidir" -to fsmc_comm_data[4]

#FSMC数据线
set_location_assignment PIN_U22 -to fsmc_comm_data[5]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to fsmc_comm_data[5]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to fsmc_comm_data[5]
#端口方向设置
set_instance_assignment -name DIRECTION "Bidir" -to fsmc_comm_data[5]

#FSMC数据线
set_location_assignment PIN_U21 -to fsmc_comm_data[6]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to fsmc_comm_data[6]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to fsmc_comm_data[6]
#端口方向设置
set_instance_assignment -name DIRECTION "Bidir" -to fsmc_comm_data[6]

#FSMC数据线
set_location_assignment PIN_R22 -to fsmc_comm_data[7]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to fsmc_comm_data[7]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to fsmc_comm_data[7]
#端口方向设置
set_instance_assignment -name DIRECTION "Bidir" -to fsmc_comm_data[7]

#FSMC数据线
set_location_assignment PIN_Y22 -to fsmc_comm_data[8]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to fsmc_comm_data[8]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to fsmc_comm_data[8]
#端口方向设置
set_instance_assignment -name DIRECTION "Bidir" -to fsmc_comm_data[8]

#FSMC数据线
set_location_assignment PIN_AA21 -to fsmc_comm_data[9]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to fsmc_comm_data[9]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to fsmc_comm_data[9]
#端口方向设置
set_instance_assignment -name DIRECTION "Bidir" -to fsmc_comm_data[9]

#FSMC数据线
set_location_assignment PIN_AA22 -to fsmc_comm_data[10]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to fsmc_comm_data[10]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to fsmc_comm_data[10]
#端口方向设置
set_instance_assignment -name DIRECTION "Bidir" -to fsmc_comm_data[10]

#FSMC数据线
set_location_assignment PIN_AB20 -to fsmc_comm_data[11]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to fsmc_comm_data[11]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to fsmc_comm_data[11]
#端口方向设置
set_instance_assignment -name DIRECTION "Bidir" -to fsmc_comm_data[11]

#FSMC数据线
set_location_assignment PIN_AA20 -to fsmc_comm_data[12]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to fsmc_comm_data[12]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to fsmc_comm_data[12]
#端口方向设置
set_instance_assignment -name DIRECTION "Bidir" -to fsmc_comm_data[12]

#FSMC数据线
set_location_assignment PIN_AB19 -to fsmc_comm_data[13]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to fsmc_comm_data[13]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to fsmc_comm_data[13]
#端口方向设置
set_instance_assignment -name DIRECTION "Bidir" -to fsmc_comm_data[13]

#FSMC数据线
set_location_assignment PIN_AA19 -to fsmc_comm_data[14]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to fsmc_comm_data[14]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to fsmc_comm_data[14]
#端口方向设置
set_instance_assignment -name DIRECTION "Bidir" -to fsmc_comm_data[14]

#FSMC数据线
set_location_assignment PIN_AB18 -to fsmc_comm_data[15]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to fsmc_comm_data[15]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to fsmc_comm_data[15]
#端口方向设置
set_instance_assignment -name DIRECTION "Bidir" -to fsmc_comm_data[15]


#####################################电机中断信号线########################################
#电机中断
set_location_assignment PIN_W19 -to motor_int_out[0]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_int_out[0]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_int_out[0]
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to motor_int_out[0]

#电机中断
set_location_assignment PIN_U17 -to motor_int_out[1]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_int_out[1]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_int_out[1]
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to motor_int_out[1]

#电机中断
set_location_assignment PIN_W17 -to motor_int_out[2]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_int_out[2]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_int_out[2]
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to motor_int_out[2]

#电机中断
set_location_assignment PIN_Y17 -to motor_int_out[3]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_int_out[3]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_int_out[3]
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to motor_int_out[3]

#########################################步进电机使能信号##########################################
#电机使能线
set_location_assignment PIN_T3 -to motor_out_enable[1]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_out_enable[1]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_out_enable[1]
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to motor_out_enable[1]

#电机使能线
set_location_assignment PIN_R5 -to motor_out_enable[2]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_out_enable[2]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_out_enable[2]
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to motor_out_enable[2]

#电机使能线
set_location_assignment PIN_N6 -to motor_out_enable[3]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_out_enable[3]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_out_enable[3]
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to motor_out_enable[3]

#电机使能线
set_location_assignment PIN_N5 -to motor_out_enable[4]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_out_enable[4]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_out_enable[4]
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to motor_out_enable[4]

#电机使能线
set_location_assignment PIN_P5 -to motor_out_enable[5]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_out_enable[5]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_out_enable[5]
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to motor_out_enable[5]

#电机使能线
set_location_assignment PIN_M5 -to motor_out_enable[6]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_out_enable[6]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_out_enable[6]
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to motor_out_enable[6]

#电机使能线
set_location_assignment PIN_M6 -to motor_out_enable[7]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_out_enable[7]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_out_enable[7]
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to motor_out_enable[7]

#电机使能线
set_location_assignment PIN_L6 -to motor_out_enable[8]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_out_enable[8]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_out_enable[8]
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to motor_out_enable[8]

#电机使能线
set_location_assignment PIN_J6 -to motor_out_enable[9]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_out_enable[9]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_out_enable[9]
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to motor_out_enable[9]

#电机使能线
set_location_assignment PIN_H6 -to motor_out_enable[10]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_out_enable[10]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_out_enable[10]
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to motor_out_enable[10]

#电机使能线
set_location_assignment PIN_J5 -to motor_out_enable[11]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_out_enable[11]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_out_enable[11]
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to motor_out_enable[11]

#电机使能线
set_location_assignment PIN_G5 -to motor_out_enable[12]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_out_enable[12]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_out_enable[12]
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to motor_out_enable[12]

#电机使能线
set_location_assignment PIN_H5 -to motor_out_enable[13]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_out_enable[13]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_out_enable[13]
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to motor_out_enable[13]

#电机使能线
set_location_assignment PIN_F1 -to motor_out_enable[14]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_out_enable[14]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_out_enable[14]
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to motor_out_enable[14]

#电机使能线
set_location_assignment PIN_G7 -to motor_out_enable[15]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_out_enable[15]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_out_enable[15]
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to motor_out_enable[15]

#电机使能线
set_location_assignment PIN_B1 -to motor_out_enable[16]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_out_enable[16]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_out_enable[16]
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to motor_out_enable[16]

#########################################步进电机方向信号##########################################
#电机方向线
set_location_assignment PIN_T4 -to motor_out_dir[1]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_out_dir[1]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_out_dir[1]
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to motor_out_dir[1]

#电机方向线
set_location_assignment PIN_R3 -to motor_out_dir[2]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_out_dir[2]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_out_dir[2]
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to motor_out_dir[2]

#电机方向线
set_location_assignment PIN_R2 -to motor_out_dir[3]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_out_dir[3]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_out_dir[3]
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to motor_out_dir[3]

#电机方向线
set_location_assignment PIN_P2 -to motor_out_dir[4]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_out_dir[4]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_out_dir[4]
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to motor_out_dir[4]

#电机方向线
set_location_assignment PIN_P3 -to motor_out_dir[5]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_out_dir[5]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_out_dir[5]
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to motor_out_dir[5]

#电机方向线
set_location_assignment PIN_M3 -to motor_out_dir[6]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_out_dir[6]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_out_dir[6]
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to motor_out_dir[6]

#电机方向线
set_location_assignment PIN_N1 -to motor_out_dir[7]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_out_dir[7]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_out_dir[7]
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to motor_out_dir[7]

#电机方向线
set_location_assignment PIN_M1 -to motor_out_dir[8]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_out_dir[8]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_out_dir[8]
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to motor_out_dir[8]

#电机方向线
set_location_assignment PIN_J2 -to motor_out_dir[9]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_out_dir[9]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_out_dir[9]
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to motor_out_dir[9]

#电机方向线
set_location_assignment PIN_H2 -to motor_out_dir[10]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_out_dir[10]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_out_dir[10]
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to motor_out_dir[10]

#电机方向线
set_location_assignment PIN_J3 -to motor_out_dir[11]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_out_dir[11]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_out_dir[11]
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to motor_out_dir[11]

#电机方向线
set_location_assignment PIN_G3 -to motor_out_dir[12]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_out_dir[12]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_out_dir[12]
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to motor_out_dir[12]

#电机方向线
set_location_assignment PIN_H4 -to motor_out_dir[13]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_out_dir[13]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_out_dir[13]
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to motor_out_dir[13]

#电机方向线
set_location_assignment PIN_F2 -to motor_out_dir[14]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_out_dir[14]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_out_dir[14]
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to motor_out_dir[14]

#电机方向线
set_location_assignment PIN_E4 -to motor_out_dir[15]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_out_dir[15]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_out_dir[15]
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to motor_out_dir[15]

#电机方向线
set_location_assignment PIN_D2 -to motor_out_dir[16]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_out_dir[16]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_out_dir[16]
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to motor_out_dir[16]

#########################################步进电机脉冲信号##########################################
#电机脉冲线
set_location_assignment PIN_T5 -to motor_out_pluse[1]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_out_pluse[1]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_out_pluse[1]
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to motor_out_pluse[1]

#电机脉冲线
set_location_assignment PIN_R4 -to motor_out_pluse[2]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_out_pluse[2]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_out_pluse[2]
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to motor_out_pluse[2]

#电机脉冲线
set_location_assignment PIN_R1 -to motor_out_pluse[3]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_out_pluse[3]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_out_pluse[3]
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to motor_out_pluse[3]

#电机脉冲线
set_location_assignment PIN_P1 -to motor_out_pluse[4]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_out_pluse[4]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_out_pluse[4]
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to motor_out_pluse[4]

#电机脉冲线
set_location_assignment PIN_P4 -to motor_out_pluse[5]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_out_pluse[5]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_out_pluse[5]
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to motor_out_pluse[5]

#电机脉冲线
set_location_assignment PIN_M4 -to motor_out_pluse[6]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_out_pluse[6]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_out_pluse[6]
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to motor_out_pluse[6]

#电机脉冲线
set_location_assignment PIN_N2 -to motor_out_pluse[7]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_out_pluse[7]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_out_pluse[7]
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to motor_out_pluse[7]

#电机脉冲线
set_location_assignment PIN_M2 -to motor_out_pluse[8]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_out_pluse[8]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_out_pluse[8]
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to motor_out_pluse[8]

#电机脉冲线
set_location_assignment PIN_J1 -to motor_out_pluse[9]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_out_pluse[9]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_out_pluse[9]
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to motor_out_pluse[9]

#电机脉冲线
set_location_assignment PIN_H1 -to motor_out_pluse[10]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_out_pluse[10]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_out_pluse[10]
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to motor_out_pluse[10]

#电机脉冲线
set_location_assignment PIN_J4 -to motor_out_pluse[11]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_out_pluse[11]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_out_pluse[11]
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to motor_out_pluse[11]

#电机脉冲线
set_location_assignment PIN_G4 -to motor_out_pluse[12]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_out_pluse[12]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_out_pluse[12]
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to motor_out_pluse[12]

#电机脉冲线
set_location_assignment PIN_H3 -to motor_out_pluse[13]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_out_pluse[13]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_out_pluse[13]
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to motor_out_pluse[13]

#电机脉冲线
set_location_assignment PIN_E1 -to motor_out_pluse[14]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_out_pluse[14]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_out_pluse[14]
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to motor_out_pluse[14]

#电机脉冲线
set_location_assignment PIN_E3 -to motor_out_pluse[15]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_out_pluse[15]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_out_pluse[15]
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to motor_out_pluse[15]

#电机脉冲线
set_location_assignment PIN_B2 -to motor_out_pluse[16]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_out_pluse[16]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_out_pluse[16]
#端口方向设置
set_instance_assignment -name DIRECTION "Output" -to motor_out_pluse[16]

#########################################步进电机原点输入传感器信号##########################################
##原点信号线
set_location_assignment PIN_R18 -to motor_sensor_in_reset[1]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_sensor_in_reset[1]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_sensor_in_reset[1]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_sensor_in_reset[1]

##原点信号线
set_location_assignment PIN_N18 -to motor_sensor_in_reset[2]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_sensor_in_reset[2]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_sensor_in_reset[2]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_sensor_in_reset[2]

##原点信号线
set_location_assignment PIN_R16 -to motor_sensor_in_reset[3]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_sensor_in_reset[3]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_sensor_in_reset[3]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_sensor_in_reset[3]

##原点信号线
set_location_assignment PIN_R17 -to motor_sensor_in_reset[4]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_sensor_in_reset[4]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_sensor_in_reset[4]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_sensor_in_reset[4]

##原点信号线
set_location_assignment PIN_T17 -to motor_sensor_in_reset[5]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_sensor_in_reset[5]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_sensor_in_reset[5]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_sensor_in_reset[5]

##原点信号线
set_location_assignment PIN_T16 -to motor_sensor_in_reset[6]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_sensor_in_reset[6]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_sensor_in_reset[6]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_sensor_in_reset[6]

##原点信号线
set_location_assignment PIN_Y13 -to motor_sensor_in_reset[7]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_sensor_in_reset[7]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_sensor_in_reset[7]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_sensor_in_reset[7]

##原点信号线
set_location_assignment PIN_V13 -to motor_sensor_in_reset[8]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_sensor_in_reset[8]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_sensor_in_reset[8]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_sensor_in_reset[8]

##原点信号线
set_location_assignment PIN_V12 -to motor_sensor_in_reset[9]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_sensor_in_reset[9]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_sensor_in_reset[9]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_sensor_in_reset[9]

##原点信号线
set_location_assignment PIN_V11 -to motor_sensor_in_reset[10]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_sensor_in_reset[10]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_sensor_in_reset[10]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_sensor_in_reset[10]

##原点信号线
set_location_assignment PIN_V14 -to motor_sensor_in_reset[11]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_sensor_in_reset[11]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_sensor_in_reset[11]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_sensor_in_reset[11]

##原点信号线
set_location_assignment PIN_R14 -to motor_sensor_in_reset[12]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_sensor_in_reset[12]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_sensor_in_reset[12]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_sensor_in_reset[12]

##原点信号线
set_location_assignment PIN_U10 -to motor_sensor_in_reset[13]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_sensor_in_reset[13]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_sensor_in_reset[13]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_sensor_in_reset[13]

##原点信号线
set_location_assignment PIN_U15 -to motor_sensor_in_reset[14]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_sensor_in_reset[14]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_sensor_in_reset[14]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_sensor_in_reset[14]

##原点信号线
set_location_assignment PIN_AA18 -to motor_sensor_in_reset[15]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_sensor_in_reset[15]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_sensor_in_reset[15]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_sensor_in_reset[15]

##原点信号线
set_location_assignment PIN_V4 -to motor_sensor_in_reset[16]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_sensor_in_reset[16]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_sensor_in_reset[16]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_sensor_in_reset[16]
#########################################步进电机急停输入传感器信号##########################################
##急停信号线
set_location_assignment PIN_V3 -to motor_sensor_in_emergency_stop[1]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_sensor_in_emergency_stop[1]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_sensor_in_emergency_stop[1]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_sensor_in_emergency_stop[1]

##急停信号线
set_location_assignment PIN_V2 -to motor_sensor_in_emergency_stop[2]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_sensor_in_emergency_stop[2]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_sensor_in_emergency_stop[2]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_sensor_in_emergency_stop[2]

##急停信号线
set_location_assignment PIN_V1 -to motor_sensor_in_emergency_stop[3]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_sensor_in_emergency_stop[3]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_sensor_in_emergency_stop[3]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_sensor_in_emergency_stop[3]

##急停信号线
set_location_assignment PIN_U2 -to motor_sensor_in_emergency_stop[4]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_sensor_in_emergency_stop[4]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_sensor_in_emergency_stop[4]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_sensor_in_emergency_stop[4]

##急停信号线
set_location_assignment PIN_U1 -to motor_sensor_in_emergency_stop[5]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_sensor_in_emergency_stop[5]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_sensor_in_emergency_stop[5]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_sensor_in_emergency_stop[5]

##急停信号线
set_location_assignment PIN_AA3 -to motor_sensor_in_emergency_stop[6]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_sensor_in_emergency_stop[6]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_sensor_in_emergency_stop[6]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_sensor_in_emergency_stop[6]

##急停信号线
set_location_assignment PIN_W1 -to motor_sensor_in_emergency_stop[7]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_sensor_in_emergency_stop[7]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_sensor_in_emergency_stop[7]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_sensor_in_emergency_stop[7]

##急停信号线
set_location_assignment PIN_W2 -to motor_sensor_in_emergency_stop[8]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_sensor_in_emergency_stop[8]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_sensor_in_emergency_stop[8]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_sensor_in_emergency_stop[8]

##急停信号线
set_location_assignment PIN_AB4 -to motor_sensor_in_emergency_stop[9]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_sensor_in_emergency_stop[9]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_sensor_in_emergency_stop[9]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_sensor_in_emergency_stop[9]

##急停信号线
set_location_assignment PIN_AB6 -to motor_sensor_in_emergency_stop[10]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_sensor_in_emergency_stop[10]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_sensor_in_emergency_stop[10]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_sensor_in_emergency_stop[10]

##急停信号线
set_location_assignment PIN_AB5 -to motor_sensor_in_emergency_stop[11]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_sensor_in_emergency_stop[11]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_sensor_in_emergency_stop[11]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_sensor_in_emergency_stop[11]

##急停信号线
set_location_assignment PIN_Y2 -to motor_sensor_in_emergency_stop[12]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_sensor_in_emergency_stop[12]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_sensor_in_emergency_stop[12]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_sensor_in_emergency_stop[12]

##急停信号线
set_location_assignment PIN_B3 -to motor_sensor_in_emergency_stop[13]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_sensor_in_emergency_stop[13]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_sensor_in_emergency_stop[13]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_sensor_in_emergency_stop[13]

##急停信号线
set_location_assignment PIN_A3 -to motor_sensor_in_emergency_stop[14]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_sensor_in_emergency_stop[14]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_sensor_in_emergency_stop[14]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_sensor_in_emergency_stop[14]

##急停信号线
set_location_assignment PIN_B4 -to motor_sensor_in_emergency_stop[15]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_sensor_in_emergency_stop[15]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_sensor_in_emergency_stop[15]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_sensor_in_emergency_stop[15]

##急停信号线
set_location_assignment PIN_A4 -to motor_sensor_in_emergency_stop[16]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_sensor_in_emergency_stop[16]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_sensor_in_emergency_stop[16]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_sensor_in_emergency_stop[16]
#########################################步进电机辅助急停输入传感器信号##########################################
##辅助急停信号线
set_location_assignment PIN_A5 -to motor_sensor_in_util_stop[1]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_sensor_in_util_stop[1]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_sensor_in_util_stop[1]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_sensor_in_util_stop[1]

##辅助急停信号线
set_location_assignment PIN_B6 -to motor_sensor_in_util_stop[2]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_sensor_in_util_stop[2]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_sensor_in_util_stop[2]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_sensor_in_util_stop[2]

##辅助急停信号线
set_location_assignment PIN_B7 -to motor_sensor_in_util_stop[3]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_sensor_in_util_stop[3]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_sensor_in_util_stop[3]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_sensor_in_util_stop[3]

##辅助急停信号线
set_location_assignment PIN_A6 -to motor_sensor_in_util_stop[4]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_sensor_in_util_stop[4]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_sensor_in_util_stop[4]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_sensor_in_util_stop[4]

##辅助急停信号线
set_location_assignment PIN_A7 -to motor_sensor_in_util_stop[5]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_sensor_in_util_stop[5]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_sensor_in_util_stop[5]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_sensor_in_util_stop[5]

##辅助急停信号线
set_location_assignment PIN_A9 -to motor_sensor_in_util_stop[6]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_sensor_in_util_stop[6]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_sensor_in_util_stop[6]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_sensor_in_util_stop[6]

##辅助急停信号线
set_location_assignment PIN_B5 -to motor_sensor_in_util_stop[7]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_sensor_in_util_stop[7]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_sensor_in_util_stop[7]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_sensor_in_util_stop[7]

##辅助急停信号线
set_location_assignment PIN_E6 -to motor_sensor_in_util_stop[8]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_sensor_in_util_stop[8]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_sensor_in_util_stop[8]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_sensor_in_util_stop[8]

##辅助急停信号线
set_location_assignment PIN_H7 -to motor_sensor_in_util_stop[9]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_sensor_in_util_stop[9]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_sensor_in_util_stop[9]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_sensor_in_util_stop[9]

##辅助急停信号线
set_location_assignment PIN_F7 -to motor_sensor_in_util_stop[10]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_sensor_in_util_stop[10]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_sensor_in_util_stop[10]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_sensor_in_util_stop[10]

##辅助急停信号线
set_location_assignment PIN_F8 -to motor_sensor_in_util_stop[11]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_sensor_in_util_stop[11]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_sensor_in_util_stop[11]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_sensor_in_util_stop[11]

##辅助急停信号线
set_location_assignment PIN_F9 -to motor_sensor_in_util_stop[12]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_sensor_in_util_stop[12]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_sensor_in_util_stop[12]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_sensor_in_util_stop[12]

##辅助急停信号线
set_location_assignment PIN_F10 -to motor_sensor_in_util_stop[13]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_sensor_in_util_stop[13]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_sensor_in_util_stop[13]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_sensor_in_util_stop[13]

##辅助急停信号线
set_location_assignment PIN_F11 -to motor_sensor_in_util_stop[14]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_sensor_in_util_stop[14]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_sensor_in_util_stop[14]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_sensor_in_util_stop[14]

##辅助急停信号线
set_location_assignment PIN_F13 -to motor_sensor_in_util_stop[15]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_sensor_in_util_stop[15]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_sensor_in_util_stop[15]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_sensor_in_util_stop[15]

##辅助急停信号线
set_location_assignment PIN_E13 -to motor_sensor_in_util_stop[16]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_sensor_in_util_stop[16]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_sensor_in_util_stop[16]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_sensor_in_util_stop[16]
#########################################步进电机编码器A相信号##########################################
##步进电机编码器输入A相
set_location_assignment PIN_AA16 -to motor_encoder_a[1]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_encoder_a[1]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_encoder_a[1]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_encoder_a[1]

##步进电机编码器输入A相
set_location_assignment PIN_AA15 -to motor_encoder_a[2]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_encoder_a[2]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_encoder_a[2]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_encoder_a[2]

##步进电机编码器输入A相
set_location_assignment PIN_AA14 -to motor_encoder_a[3]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_encoder_a[3]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_encoder_a[3]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_encoder_a[3]

##步进电机编码器输入A相
set_location_assignment PIN_AA13 -to motor_encoder_a[4]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_encoder_a[4]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_encoder_a[4]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_encoder_a[4]

##步进电机编码器输入A相
set_location_assignment PIN_AA10 -to motor_encoder_a[5]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_encoder_a[5]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_encoder_a[5]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_encoder_a[5]

##步进电机编码器输入A相
set_location_assignment PIN_AA9 -to motor_encoder_a[6]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_encoder_a[6]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_encoder_a[6]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_encoder_a[6]

##步进电机编码器输入A相
set_location_assignment PIN_AA8 -to motor_encoder_a[7]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_encoder_a[7]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_encoder_a[7]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_encoder_a[7]

##步进电机编码器输入A相
set_location_assignment PIN_AA7 -to motor_encoder_a[8]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_encoder_a[8]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_encoder_a[8]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_encoder_a[8]

##步进电机编码器输入A相
set_location_assignment PIN_Y10 -to motor_encoder_a[9]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_encoder_a[9]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_encoder_a[9]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_encoder_a[9]

##步进电机编码器输入A相
set_location_assignment PIN_V8 -to motor_encoder_a[10]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_encoder_a[10]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_encoder_a[10]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_encoder_a[10]

##步进电机编码器输入A相
set_location_assignment PIN_W7 -to motor_encoder_a[11]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_encoder_a[11]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_encoder_a[11]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_encoder_a[11]

##步进电机编码器输入A相
set_location_assignment PIN_AA6 -to motor_encoder_a[12]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_encoder_a[12]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_encoder_a[12]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_encoder_a[12]

##步进电机编码器输入A相
set_location_assignment PIN_C3 -to motor_encoder_a[13]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_encoder_a[13]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_encoder_a[13]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_encoder_a[13]

##步进电机编码器输入A相
set_location_assignment PIN_C6 -to motor_encoder_a[14]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_encoder_a[14]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_encoder_a[14]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_encoder_a[14]

##步进电机编码器输入A相
set_location_assignment PIN_D8 -to motor_encoder_a[15]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_encoder_a[15]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_encoder_a[15]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_encoder_a[15]

##步进电机编码器输入A相
set_location_assignment PIN_C8 -to motor_encoder_a[16]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_encoder_a[16]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_encoder_a[16]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_encoder_a[16]

#########################################步进电机编码器B相信号##########################################
##步进电机编码器输入B相
set_location_assignment PIN_AB16 -to motor_encoder_b[1]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_encoder_b[1]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_encoder_b[1]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_encoder_b[1]

##步进电机编码器输入B相
set_location_assignment PIN_AB15 -to motor_encoder_b[2]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_encoder_b[2]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_encoder_b[2]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_encoder_b[2]

##步进电机编码器输入B相
set_location_assignment PIN_AB14 -to motor_encoder_b[3]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_encoder_b[3]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_encoder_b[3]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_encoder_b[3]

##步进电机编码器输入B相
set_location_assignment PIN_AB13 -to motor_encoder_b[4]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_encoder_b[4]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_encoder_b[4]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_encoder_b[4]

##步进电机编码器输入B相
set_location_assignment PIN_AB10 -to motor_encoder_b[5]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_encoder_b[5]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_encoder_b[5]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_encoder_b[5]

##步进电机编码器输入B相
set_location_assignment PIN_AB9 -to motor_encoder_b[6]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_encoder_b[6]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_encoder_b[6]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_encoder_b[6]

##步进电机编码器输入B相
set_location_assignment PIN_AB8 -to motor_encoder_b[7]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_encoder_b[7]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_encoder_b[7]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_encoder_b[7]

##步进电机编码器输入B相
set_location_assignment PIN_AB7 -to motor_encoder_b[8]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_encoder_b[8]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_encoder_b[8]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_encoder_b[8]

##步进电机编码器输入B相
set_location_assignment PIN_W10 -to motor_encoder_b[9]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_encoder_b[9]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_encoder_b[9]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_encoder_b[9]

##步进电机编码器输入B相
set_location_assignment PIN_U9 -to motor_encoder_b[10]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_encoder_b[10]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_encoder_b[10]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_encoder_b[10]

##步进电机编码器输入B相
set_location_assignment PIN_Y7 -to motor_encoder_b[11]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_encoder_b[11]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_encoder_b[11]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_encoder_b[11]

##步进电机编码器输入B相
set_location_assignment PIN_AA5 -to motor_encoder_b[12]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_encoder_b[12]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_encoder_b[12]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_encoder_b[12]

##步进电机编码器输入B相
set_location_assignment PIN_C4 -to motor_encoder_b[13]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_encoder_b[13]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_encoder_b[13]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_encoder_b[13]

##步进电机编码器输入B相
set_location_assignment PIN_D7 -to motor_encoder_b[14]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_encoder_b[14]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_encoder_b[14]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_encoder_b[14]

##步进电机编码器输入B相
set_location_assignment PIN_E8 -to motor_encoder_b[15]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_encoder_b[15]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_encoder_b[15]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_encoder_b[15]

##步进电机编码器输入B相
set_location_assignment PIN_C7 -to motor_encoder_b[16]
#端口电平设置
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to motor_encoder_b[16]
#端口驱动强度设置
set_instance_assignment -name CURRENT_STRENGTH_NEW "8MA" -to motor_encoder_b[16]
#端口方向设置
set_instance_assignment -name DIRECTION "Input" -to motor_encoder_b[16]










