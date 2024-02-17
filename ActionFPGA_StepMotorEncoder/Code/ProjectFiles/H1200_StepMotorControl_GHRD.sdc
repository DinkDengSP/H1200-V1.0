## Generated SDC file "H1200_StepMotorControl_GHRD.sdc"

## Copyright (C) 2023  Intel Corporation. All rights reserved.
## Your use of Intel Corporation's design tools, logic functions 
## and other software and tools, and any partner logic 
## functions, and any output files from any of the foregoing 
## (including device programming or simulation files), and any 
## associated documentation or information are expressly subject 
## to the terms and conditions of the Intel Program License 
## Subscription Agreement, the Intel Quartus Prime License Agreement,
## the Intel FPGA IP License Agreement, or other applicable license
## agreement, including, without limitation, that your use is for
## the sole purpose of programming logic devices manufactured by
## Intel and sold by Intel or its authorized distributors.  Please
## refer to the applicable agreement for further details, at
## https://fpgasoftware.intel.com/eula.


## VENDOR  "Altera"
## PROGRAM "Quartus Prime"
## VERSION "Version 22.1std.1 Build 917 02/14/2023 SC Lite Edition"

## DATE    "Thu Jan 25 13:53:03 2024"

##
## DEVICE  "10CL080YU484I7G"
##


#**************************************************************
# Time Information
#**************************************************************

set_time_format -unit ns -decimal_places 3



#**************************************************************
# Create Clock
#**************************************************************

create_clock -name {sys_clk_24m} -period 41.666 -waveform { 0.000 20.833 } [get_ports {sys_clk_24m}]
create_clock -name {sys_clk_50m} -period 20.000 -waveform { 0.000 10.000 } [get_ports {sys_clk_50m}]
create_clock -name {sys_rst_n} -period 1.000 -waveform { 0.000 0.500 } [get_ports {sys_rst_n}]


#**************************************************************
# Create Generated Clock
#**************************************************************

create_generated_clock -name {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]} -source [get_pins {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|inclk[0]}] -duty_cycle 50/1 -multiply_by 5 -divide_by 2 -master_clock {sys_clk_24m} [get_pins {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}] 
create_generated_clock -name {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]} -source [get_pins {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|inclk[0]}] -duty_cycle 50/1 -multiply_by 5 -divide_by 2 -master_clock {sys_clk_24m} [get_pins {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}] 
create_generated_clock -name {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[2]} -source [get_pins {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|inclk[0]}] -duty_cycle 50/1 -multiply_by 5 -divide_by 2 -master_clock {sys_clk_24m} [get_pins {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[2]}] 
create_generated_clock -name {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[3]} -source [get_pins {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|inclk[0]}] -duty_cycle 50/1 -multiply_by 5 -divide_by 2 -master_clock {sys_clk_24m} [get_pins {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[3]}] 
create_generated_clock -name {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[4]} -source [get_pins {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|inclk[0]}] -duty_cycle 50/1 -multiply_by 25 -divide_by 12 -master_clock {sys_clk_24m} [get_pins {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[4]}] 
create_generated_clock -name {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]} -source [get_pins {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|inclk[0]}] -duty_cycle 50/1 -multiply_by 7 -divide_by 5 -master_clock {sys_clk_50m} [get_pins {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}] 
create_generated_clock -name {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]} -source [get_pins {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|inclk[0]}] -duty_cycle 50/1 -multiply_by 2 -master_clock {sys_clk_50m} [get_pins {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}] 
create_generated_clock -name {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[3]} -source [get_pins {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|inclk[0]}] -duty_cycle 50/1 -multiply_by 1 -master_clock {sys_clk_50m} [get_pins {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[3]}] 
create_generated_clock -name {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[4]} -source [get_pins {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|inclk[0]}] -duty_cycle 50/1 -multiply_by 1 -master_clock {sys_clk_50m} [get_pins {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[4]}] 


#**************************************************************
# Set Clock Latency
#**************************************************************



#**************************************************************
# Set Clock Uncertainty
#**************************************************************

set_clock_uncertainty -rise_from [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}] -rise_to [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}]  0.020  
set_clock_uncertainty -rise_from [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}] -fall_to [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}]  0.020  
set_clock_uncertainty -rise_from [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}] -rise_to [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}]  0.160  
set_clock_uncertainty -rise_from [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}] -fall_to [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}]  0.160  
set_clock_uncertainty -fall_from [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}] -rise_to [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}]  0.020  
set_clock_uncertainty -fall_from [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}] -fall_to [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}]  0.020  
set_clock_uncertainty -fall_from [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}] -rise_to [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}]  0.160  
set_clock_uncertainty -fall_from [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}] -fall_to [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}]  0.160  
set_clock_uncertainty -rise_from [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}] -rise_to [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}]  0.020  
set_clock_uncertainty -rise_from [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}] -fall_to [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}]  0.020  
set_clock_uncertainty -rise_from [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}] -rise_to [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}]  0.160  
set_clock_uncertainty -rise_from [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}] -fall_to [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}]  0.160  
set_clock_uncertainty -fall_from [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}] -rise_to [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}]  0.020  
set_clock_uncertainty -fall_from [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}] -fall_to [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}]  0.020  
set_clock_uncertainty -fall_from [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}] -rise_to [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}]  0.160  
set_clock_uncertainty -fall_from [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}] -fall_to [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}]  0.160  
set_clock_uncertainty -rise_from [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[2]}] -rise_to [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[2]}]  0.020  
set_clock_uncertainty -rise_from [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[2]}] -fall_to [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[2]}]  0.020  
set_clock_uncertainty -rise_from [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[2]}] -rise_to [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}]  0.160  
set_clock_uncertainty -rise_from [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[2]}] -fall_to [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}]  0.160  
set_clock_uncertainty -fall_from [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[2]}] -rise_to [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[2]}]  0.020  
set_clock_uncertainty -fall_from [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[2]}] -fall_to [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[2]}]  0.020  
set_clock_uncertainty -fall_from [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[2]}] -rise_to [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}]  0.160  
set_clock_uncertainty -fall_from [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[2]}] -fall_to [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}]  0.160  
set_clock_uncertainty -rise_from [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[3]}] -rise_to [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[3]}]  0.020  
set_clock_uncertainty -rise_from [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[3]}] -fall_to [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[3]}]  0.020  
set_clock_uncertainty -rise_from [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[3]}] -rise_to [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}]  0.160  
set_clock_uncertainty -rise_from [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[3]}] -fall_to [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}]  0.160  
set_clock_uncertainty -fall_from [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[3]}] -rise_to [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[3]}]  0.020  
set_clock_uncertainty -fall_from [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[3]}] -fall_to [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[3]}]  0.020  
set_clock_uncertainty -fall_from [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[3]}] -rise_to [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}]  0.160  
set_clock_uncertainty -fall_from [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[3]}] -fall_to [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}]  0.160  
set_clock_uncertainty -rise_from [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[4]}] -rise_to [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[4]}]  0.020  
set_clock_uncertainty -rise_from [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[4]}] -fall_to [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[4]}]  0.020  
set_clock_uncertainty -fall_from [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[4]}] -rise_to [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[4]}]  0.020  
set_clock_uncertainty -fall_from [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[4]}] -fall_to [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[4]}]  0.020  
set_clock_uncertainty -rise_from [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}] -rise_to [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}]  0.020  
set_clock_uncertainty -rise_from [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}] -fall_to [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}]  0.020  
set_clock_uncertainty -rise_from [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}] -rise_to [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}]  0.020  
set_clock_uncertainty -rise_from [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}] -fall_to [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}]  0.020  
set_clock_uncertainty -fall_from [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}] -rise_to [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}]  0.020  
set_clock_uncertainty -fall_from [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}] -fall_to [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}]  0.020  
set_clock_uncertainty -fall_from [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}] -rise_to [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}]  0.020  
set_clock_uncertainty -fall_from [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}] -fall_to [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}]  0.020  
set_clock_uncertainty -rise_from [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}] -rise_to [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}]  0.160  
set_clock_uncertainty -rise_from [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}] -fall_to [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}]  0.160  
set_clock_uncertainty -rise_from [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}] -rise_to [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}]  0.160  
set_clock_uncertainty -rise_from [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}] -fall_to [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}]  0.160  
set_clock_uncertainty -rise_from [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}] -rise_to [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[2]}]  0.160  
set_clock_uncertainty -rise_from [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}] -fall_to [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[2]}]  0.160  
set_clock_uncertainty -rise_from [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}] -rise_to [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[3]}]  0.160  
set_clock_uncertainty -rise_from [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}] -fall_to [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[3]}]  0.160  
set_clock_uncertainty -rise_from [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}] -rise_to [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}]  0.020  
set_clock_uncertainty -rise_from [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}] -fall_to [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}]  0.020  
set_clock_uncertainty -rise_from [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}] -rise_to [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[3]}]  0.020  
set_clock_uncertainty -rise_from [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}] -fall_to [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[3]}]  0.020  
set_clock_uncertainty -rise_from [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}] -rise_to [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[4]}]  0.020  
set_clock_uncertainty -rise_from [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}] -fall_to [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[4]}]  0.020  
set_clock_uncertainty -fall_from [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}] -rise_to [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}]  0.160  
set_clock_uncertainty -fall_from [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}] -fall_to [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}]  0.160  
set_clock_uncertainty -fall_from [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}] -rise_to [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}]  0.160  
set_clock_uncertainty -fall_from [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}] -fall_to [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}]  0.160  
set_clock_uncertainty -fall_from [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}] -rise_to [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[2]}]  0.160  
set_clock_uncertainty -fall_from [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}] -fall_to [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[2]}]  0.160  
set_clock_uncertainty -fall_from [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}] -rise_to [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[3]}]  0.160  
set_clock_uncertainty -fall_from [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}] -fall_to [get_clocks {clk_24m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[3]}]  0.160  
set_clock_uncertainty -fall_from [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}] -rise_to [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}]  0.020  
set_clock_uncertainty -fall_from [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}] -fall_to [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}]  0.020  
set_clock_uncertainty -fall_from [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}] -rise_to [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[3]}]  0.020  
set_clock_uncertainty -fall_from [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}] -fall_to [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[3]}]  0.020  
set_clock_uncertainty -fall_from [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}] -rise_to [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[4]}]  0.020  
set_clock_uncertainty -fall_from [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[1]}] -fall_to [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[4]}]  0.020  
set_clock_uncertainty -rise_from [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[3]}] -rise_to [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}]  0.020  
set_clock_uncertainty -rise_from [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[3]}] -fall_to [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}]  0.020  
set_clock_uncertainty -rise_from [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[3]}] -rise_to [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[3]}]  0.020  
set_clock_uncertainty -rise_from [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[3]}] -fall_to [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[3]}]  0.020  
set_clock_uncertainty -fall_from [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[3]}] -rise_to [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}]  0.020  
set_clock_uncertainty -fall_from [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[3]}] -fall_to [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}]  0.020  
set_clock_uncertainty -fall_from [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[3]}] -rise_to [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[3]}]  0.020  
set_clock_uncertainty -fall_from [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[3]}] -fall_to [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[3]}]  0.020  
set_clock_uncertainty -rise_from [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[4]}] -rise_to [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}]  0.020  
set_clock_uncertainty -rise_from [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[4]}] -fall_to [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}]  0.020  
set_clock_uncertainty -rise_from [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[4]}] -rise_to [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[4]}]  0.020  
set_clock_uncertainty -rise_from [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[4]}] -fall_to [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[4]}]  0.020  
set_clock_uncertainty -fall_from [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[4]}] -rise_to [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}]  0.020  
set_clock_uncertainty -fall_from [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[4]}] -fall_to [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[0]}]  0.020  
set_clock_uncertainty -fall_from [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[4]}] -rise_to [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[4]}]  0.020  
set_clock_uncertainty -fall_from [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[4]}] -fall_to [get_clocks {clk_50m_pll_ip_inst|altpll_component|auto_generated|pll1|clk[4]}]  0.020  


#**************************************************************
# Set Input Delay
#**************************************************************



#**************************************************************
# Set Output Delay
#**************************************************************



#**************************************************************
# Set Clock Groups
#**************************************************************



#**************************************************************
# Set False Path
#**************************************************************



#**************************************************************
# Set Multicycle Path
#**************************************************************



#**************************************************************
# Set Maximum Delay
#**************************************************************



#**************************************************************
# Set Minimum Delay
#**************************************************************



#**************************************************************
# Set Input Transition
#**************************************************************

