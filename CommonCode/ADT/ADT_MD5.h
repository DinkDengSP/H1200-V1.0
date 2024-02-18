#ifndef __ADT_MD5_H_
#define __ADT_MD5_H_
#include "ADT_Base.h"

//MD5尺寸
#define ADT_MD5_HASH_SIZE           ( 128 / 8 )
//MD5上下文
typedef struct ADT_MD5_CONTEXT
{
    uint32_t     lo;
    uint32_t     hi;
    uint32_t     a;
    uint32_t     b;
    uint32_t     c;
    uint32_t     d;
    uint8_t      buffer[64];
    uint32_t     block[16];
} ADT_MD5_CONTEXT;

//MD5计算结果
typedef struct ADT_MD5_HASH
{
    uint8_t      bytes [ADT_MD5_HASH_SIZE];
} ADT_MD5_HASH;

//MD5上下文初始化
void ADT_MD5_ContextInitialise(ADT_MD5_CONTEXT* Context);

//MD5分步计算
void ADT_MD5_Update(ADT_MD5_CONTEXT* Context,void const* Buffer, uint32_t BufferSize);

//MD5完成计算
void ADT_MD5_Finalise(ADT_MD5_CONTEXT* Context,ADT_MD5_HASH* Digest);

//MD5单次计算
void ADT_MD5_Calculate(void  const* Buffer,uint32_t BufferSize,ADT_MD5_HASH* Digest);



#endif



