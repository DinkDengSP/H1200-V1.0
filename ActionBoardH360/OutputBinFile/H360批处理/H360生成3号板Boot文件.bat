@echo off  
setlocal enabledelayedexpansion  
  
del /s /q "Imm_H360_Act3_Boot_0x03*.bin"

set "sourcePattern=..\ProjectName_Module1_H360ActionBoard_Boot_V*.bin"  
set "newFileName=..\Imm_H360_Act3_Boot_0x03_V0.0.0.0.bin"  
  
for %%F in (%sourcePattern%) do (  
    copy "%%F" "%newFileName%"  
)  
  
echo Files copied and renamed successfully.  
pause