#ifndef __ADT_SHA1_H_
#define __ADT_SHA1_H_
#include "ADT_Base.h"

//SHA计算结果长度
#define ADT_SHA1_HASH_SIZE           ( 160 / 8 )
//计算上下文
typedef struct
{
    uint32_t        State[5];
    uint32_t        Count[2];
    uint8_t         Buffer[64];
} ADT_SHA1_CONTEXT;

//计算结果
typedef struct
{
    uint8_t      bytes [ADT_SHA1_HASH_SIZE];
} ADT_SHA1_HASH;

//初始化SHA1计算上下文
void ADT_SHA1_Initialise(ADT_SHA1_CONTEXT *Context);

//SHA1分步计算
void ADT_SHA1_Update(ADT_SHA1_CONTEXT *Context,void const *Buffer,uint32_t BufferSize);

//SHA1计算结果
void ADT_SHA1_Finalise(ADT_SHA1_CONTEXT *Context,ADT_SHA1_HASH *Digest);

//SHA1单次计算
void ADT_SHA1_Calculate(void const *Buffer,uint32_t BufferSize,ADT_SHA1_HASH *Digest);

#endif


