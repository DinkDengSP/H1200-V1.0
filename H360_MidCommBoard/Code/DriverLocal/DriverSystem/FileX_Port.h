/*********************************************************************************************** 
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-08-21 10:19:22 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :    
** ModifyRecord2 :    
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-08-21 15:56:01 +0800
************************************************************************************************/ 
#ifndef __FILE_X_PORT_H_
#define __FILE_X_PORT_H_
#include "DriverHeaderBoard.h"

/******************************************************************************/
//显示文件系统根目录
void FileSystemViewRootDir(uint8_t* mediaNameStr,FX_MEDIA* fxMidiaPtr);

/******************************************************************************/
//文件系统变量
extern FX_MEDIA  fxSDCardDiskVar; 

//FileX 驱动
VOID  FileX_DriverSDCard(FX_MEDIA *media_ptr);

//文件系统格式化
ERROR_SUB FileSystemSDCardFormat(void);

//文件系统挂载
ERROR_SUB FileSystemSDCardMount(void);

//文件系统卸载
ERROR_SUB FileSystemSDCardUnmount(void);


#endif


