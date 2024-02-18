#ifndef __ADT_AES_H_
#define __ADT_AES_H_
#include "ADT_Base.h"

//AES KEY长度
#define ADT_AES_KEY_SIZE_128        16
#define ADT_AES_KEY_SIZE_192        24
#define ADT_AES_KEY_SIZE_256        32
#define ADT_AES_BLOCK_SIZE          16

//AES加密解密上下文
typedef struct ADT_AES_CONTEXT
{
    uint32_t        eK[60];
    uint32_t        dK[60];
    uint_fast32_t   Nr;
} ADT_AES_CONTEXT;

//AES初始化
int ADT_AES_Initialise(ADT_AES_CONTEXT *Context,void const *Key,uint32_t KeySize);

//AES加密
void ADT_AES_Encrypt(ADT_AES_CONTEXT const *Context,uint8_t const Input[ADT_AES_BLOCK_SIZE],uint8_t Output[ADT_AES_BLOCK_SIZE]);

//AES解密
void ADT_AES_Decrypt(ADT_AES_CONTEXT const *Context,uint8_t const Input[ADT_AES_BLOCK_SIZE],uint8_t Output[ADT_AES_BLOCK_SIZE]);

//AES分步加密
void ADT_AES_EncryptInPlace(ADT_AES_CONTEXT const *Context,uint8_t Block[ADT_AES_BLOCK_SIZE]);

//AES分步解密
void ADT_AES_DecryptInPlace(ADT_AES_CONTEXT const *Context,uint8_t Block[ADT_AES_BLOCK_SIZE]);

#endif



