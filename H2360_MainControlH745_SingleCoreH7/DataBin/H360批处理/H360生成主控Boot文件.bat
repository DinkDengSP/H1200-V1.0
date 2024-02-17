@echo off  
setlocal enabledelayedexpansion  

del /s /q "..\Imm_H360_Main_Boot_0x1A_V0*.bin"
  
set "sourcePattern=..\H745_SingleCore_DeviceMainControlBoard_Boot_V*.bin"  
set "newFileName=..\Imm_H360_Main_Boot_0x1A_V0.0.0.0.bin"  
  
for %%F in (%sourcePattern%) do (  
    copy "%%F" "%newFileName%"  
)  
  
echo Files copied and renamed successfully.  
pause