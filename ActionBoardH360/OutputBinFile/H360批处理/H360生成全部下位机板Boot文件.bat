@echo off  
setlocal enabledelayedexpansion  
 
del /s /q "..\Imm_H360_Act*.bin"
  
set "sourcePattern=..\ProjectName_Module1_H360ActionBoard_Boot_V*.bin"  
set "newFileName1=..\Imm_H360_Act1_Boot_0x01_V0.0.0.0.bin"  
set "newFileName2=..\Imm_H360_Act2_Boot_0x02_V0.0.0.0.bin" 
set "newFileName3=..\Imm_H360_Act3_Boot_0x03_V0.0.0.0.bin" 
set "newFileName4=..\Imm_H360_Act4_Boot_0x04_V0.0.0.0.bin" 
set "newFileName5=..\Imm_H360_Act5_Boot_0x05_V0.0.0.0.bin" 
  
for %%F in (%sourcePattern%) do (  
    copy "%%F" "%newFileName1%"  
)  

for %%F in (%sourcePattern%) do (  
    copy "%%F" "%newFileName2%"  
)  

for %%F in (%sourcePattern%) do (  
    copy "%%F" "%newFileName3%"  
)  

for %%F in (%sourcePattern%) do (  
    copy "%%F" "%newFileName4%"  
)  

for %%F in (%sourcePattern%) do (  
    copy "%%F" "%newFileName5%"  
)  
  
echo Files copied and renamed successfully.  
pause