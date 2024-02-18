/***********************************************************************************************
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-03-09 10:42:18 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :
** ModifyRecord2 :
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-03-09 10:51:50 +0800
************************************************************************************************/
#include "ADT_MD5.h"

//计算辅助
#define F(x, y, z)          ((z) ^ ((x) & ((y) ^ (z))))
#define G(x, y, z)          ((y) ^ ((z) & ((x) ^ (y))))
#define H(x, y, z)          ((x) ^ (y) ^ (z))
#define I(x, y, z)          ((y) ^ ((x) | ~(z)))

//步进计算
#define ADT_MD5_STEP(f, a, b, c, d, x, t, s)                \
    (a) += f((b), (c), (d)) + (x) + (t);                     \
    (a) = (((a) << (s)) | (((a)&0xffffffff) >> (32 - (s)))); \
    (a) += (b);


static void *ADT_MD5_TransformFunction(ADT_MD5_CONTEXT *ctx,void const *data,uintmax_t size)
{
    uint8_t *ptr;
    uint32_t a;
    uint32_t b;
    uint32_t c;
    uint32_t d;
    uint32_t saved_a;
    uint32_t saved_b;
    uint32_t saved_c;
    uint32_t saved_d;

#define GET(n) (ctx->block[(n)])
#define SET(n) (ctx->block[(n)] = \
                    ((uint32_t)ptr[(n)*4 + 0] << 0) | ((uint32_t)ptr[(n)*4 + 1] << 8) | ((uint32_t)ptr[(n)*4 + 2] << 16) | ((uint32_t)ptr[(n)*4 + 3] << 24))

    ptr = (uint8_t *)data;

    a = ctx->a;
    b = ctx->b;
    c = ctx->c;
    d = ctx->d;

    do
    {
        saved_a = a;
        saved_b = b;
        saved_c = c;
        saved_d = d;

        // Round 1
        ADT_MD5_STEP(F, a, b, c, d, SET(0), 0xd76aa478, 7)
        ADT_MD5_STEP(F, d, a, b, c, SET(1), 0xe8c7b756, 12)
        ADT_MD5_STEP(F, c, d, a, b, SET(2), 0x242070db, 17)
        ADT_MD5_STEP(F, b, c, d, a, SET(3), 0xc1bdceee, 22)
        ADT_MD5_STEP(F, a, b, c, d, SET(4), 0xf57c0faf, 7)
        ADT_MD5_STEP(F, d, a, b, c, SET(5), 0x4787c62a, 12)
        ADT_MD5_STEP(F, c, d, a, b, SET(6), 0xa8304613, 17)
        ADT_MD5_STEP(F, b, c, d, a, SET(7), 0xfd469501, 22)
        ADT_MD5_STEP(F, a, b, c, d, SET(8), 0x698098d8, 7)
        ADT_MD5_STEP(F, d, a, b, c, SET(9), 0x8b44f7af, 12)
        ADT_MD5_STEP(F, c, d, a, b, SET(10), 0xffff5bb1, 17)
        ADT_MD5_STEP(F, b, c, d, a, SET(11), 0x895cd7be, 22)
        ADT_MD5_STEP(F, a, b, c, d, SET(12), 0x6b901122, 7)
        ADT_MD5_STEP(F, d, a, b, c, SET(13), 0xfd987193, 12)
        ADT_MD5_STEP(F, c, d, a, b, SET(14), 0xa679438e, 17)
        ADT_MD5_STEP(F, b, c, d, a, SET(15), 0x49b40821, 22)

        // Round 2
        ADT_MD5_STEP(G, a, b, c, d, GET(1), 0xf61e2562, 5)
        ADT_MD5_STEP(G, d, a, b, c, GET(6), 0xc040b340, 9)
        ADT_MD5_STEP(G, c, d, a, b, GET(11), 0x265e5a51, 14)
        ADT_MD5_STEP(G, b, c, d, a, GET(0), 0xe9b6c7aa, 20)
        ADT_MD5_STEP(G, a, b, c, d, GET(5), 0xd62f105d, 5)
        ADT_MD5_STEP(G, d, a, b, c, GET(10), 0x02441453, 9)
        ADT_MD5_STEP(G, c, d, a, b, GET(15), 0xd8a1e681, 14)
        ADT_MD5_STEP(G, b, c, d, a, GET(4), 0xe7d3fbc8, 20)
        ADT_MD5_STEP(G, a, b, c, d, GET(9), 0x21e1cde6, 5)
        ADT_MD5_STEP(G, d, a, b, c, GET(14), 0xc33707d6, 9)
        ADT_MD5_STEP(G, c, d, a, b, GET(3), 0xf4d50d87, 14)
        ADT_MD5_STEP(G, b, c, d, a, GET(8), 0x455a14ed, 20)
        ADT_MD5_STEP(G, a, b, c, d, GET(13), 0xa9e3e905, 5)
        ADT_MD5_STEP(G, d, a, b, c, GET(2), 0xfcefa3f8, 9)
        ADT_MD5_STEP(G, c, d, a, b, GET(7), 0x676f02d9, 14)
        ADT_MD5_STEP(G, b, c, d, a, GET(12), 0x8d2a4c8a, 20)

        // Round 3
        ADT_MD5_STEP(H, a, b, c, d, GET(5), 0xfffa3942, 4)
        ADT_MD5_STEP(H, d, a, b, c, GET(8), 0x8771f681, 11)
        ADT_MD5_STEP(H, c, d, a, b, GET(11), 0x6d9d6122, 16)
        ADT_MD5_STEP(H, b, c, d, a, GET(14), 0xfde5380c, 23)
        ADT_MD5_STEP(H, a, b, c, d, GET(1), 0xa4beea44, 4)
        ADT_MD5_STEP(H, d, a, b, c, GET(4), 0x4bdecfa9, 11)
        ADT_MD5_STEP(H, c, d, a, b, GET(7), 0xf6bb4b60, 16)
        ADT_MD5_STEP(H, b, c, d, a, GET(10), 0xbebfbc70, 23)
        ADT_MD5_STEP(H, a, b, c, d, GET(13), 0x289b7ec6, 4)
        ADT_MD5_STEP(H, d, a, b, c, GET(0), 0xeaa127fa, 11)
        ADT_MD5_STEP(H, c, d, a, b, GET(3), 0xd4ef3085, 16)
        ADT_MD5_STEP(H, b, c, d, a, GET(6), 0x04881d05, 23)
        ADT_MD5_STEP(H, a, b, c, d, GET(9), 0xd9d4d039, 4)
        ADT_MD5_STEP(H, d, a, b, c, GET(12), 0xe6db99e5, 11)
        ADT_MD5_STEP(H, c, d, a, b, GET(15), 0x1fa27cf8, 16)
        ADT_MD5_STEP(H, b, c, d, a, GET(2), 0xc4ac5665, 23)

        // Round 4
        ADT_MD5_STEP(I, a, b, c, d, GET(0), 0xf4292244, 6)
        ADT_MD5_STEP(I, d, a, b, c, GET(7), 0x432aff97, 10)
        ADT_MD5_STEP(I, c, d, a, b, GET(14), 0xab9423a7, 15)
        ADT_MD5_STEP(I, b, c, d, a, GET(5), 0xfc93a039, 21)
        ADT_MD5_STEP(I, a, b, c, d, GET(12), 0x655b59c3, 6)
        ADT_MD5_STEP(I, d, a, b, c, GET(3), 0x8f0ccc92, 10)
        ADT_MD5_STEP(I, c, d, a, b, GET(10), 0xffeff47d, 15)
        ADT_MD5_STEP(I, b, c, d, a, GET(1), 0x85845dd1, 21)
        ADT_MD5_STEP(I, a, b, c, d, GET(8), 0x6fa87e4f, 6)
        ADT_MD5_STEP(I, d, a, b, c, GET(15), 0xfe2ce6e0, 10)
        ADT_MD5_STEP(I, c, d, a, b, GET(6), 0xa3014314, 15)
        ADT_MD5_STEP(I, b, c, d, a, GET(13), 0x4e0811a1, 21)
        ADT_MD5_STEP(I, a, b, c, d, GET(4), 0xf7537e82, 6)
        ADT_MD5_STEP(I, d, a, b, c, GET(11), 0xbd3af235, 10)
        ADT_MD5_STEP(I, c, d, a, b, GET(2), 0x2ad7d2bb, 15)
        ADT_MD5_STEP(I, b, c, d, a, GET(9), 0xeb86d391, 21)

        a += saved_a;
        b += saved_b;
        c += saved_c;
        d += saved_d;

        ptr += 64;
    } while (size -= 64);

    ctx->a = a;
    ctx->b = b;
    ctx->c = c;
    ctx->d = d;

#undef GET
#undef SET

    return ptr;
}

//MD5上下文初始化
void ADT_MD5_ContextInitialise(ADT_MD5_CONTEXT *Context)
{
    Context->a = 0x67452301;
    Context->b = 0xefcdab89;
    Context->c = 0x98badcfe;
    Context->d = 0x10325476;

    Context->lo = 0;
    Context->hi = 0;
}

//MD5分步计算
void ADT_MD5_Update(ADT_MD5_CONTEXT *Context,void const *Buffer,uint32_t BufferSize)
{
    uint32_t saved_lo;
    uint32_t used;
    uint32_t free;

    saved_lo = Context->lo;
    if ((Context->lo = (saved_lo + BufferSize) & 0x1fffffff) < saved_lo)
    {
        Context->hi++;
    }
    Context->hi += (uint32_t)(BufferSize >> 29);

    used = saved_lo & 0x3f;

    if (used)
    {
        free = 64 - used;

        if (BufferSize < free)
        {
            ADT_BASE_MEM_COPY(&Context->buffer[used], Buffer, BufferSize);
            return;
        }

        ADT_BASE_MEM_COPY(&Context->buffer[used], Buffer, free);
        Buffer = (uint8_t *)Buffer + free;
        BufferSize -= free;
        ADT_MD5_TransformFunction(Context, Context->buffer, 64);
    }

    if (BufferSize >= 64)
    {
        Buffer = ADT_MD5_TransformFunction(Context, Buffer, BufferSize & ~(unsigned long)0x3f);
        BufferSize &= 0x3f;
    }

    ADT_BASE_MEM_COPY(Context->buffer, Buffer, BufferSize);
}

//MD5结束计算
void ADT_MD5_Finalise(ADT_MD5_CONTEXT *Context,ADT_MD5_HASH *Digest)
{
    uint32_t used;
    uint32_t free;

    used = Context->lo & 0x3f;

    Context->buffer[used++] = 0x80;

    free = 64 - used;

    if (free < 8)
    {
        ADT_MEM_SET(&Context->buffer[used], 0, free);
        ADT_MD5_TransformFunction(Context, Context->buffer, 64);
        used = 0;
        free = 64;
    }

    ADT_MEM_SET(&Context->buffer[used], 0, free - 8);

    Context->lo <<= 3;
    Context->buffer[56] = (uint8_t)(Context->lo);
    Context->buffer[57] = (uint8_t)(Context->lo >> 8);
    Context->buffer[58] = (uint8_t)(Context->lo >> 16);
    Context->buffer[59] = (uint8_t)(Context->lo >> 24);
    Context->buffer[60] = (uint8_t)(Context->hi);
    Context->buffer[61] = (uint8_t)(Context->hi >> 8);
    Context->buffer[62] = (uint8_t)(Context->hi >> 16);
    Context->buffer[63] = (uint8_t)(Context->hi >> 24);

    ADT_MD5_TransformFunction(Context, Context->buffer, 64);

    Digest->bytes[0] = (uint8_t)(Context->a);
    Digest->bytes[1] = (uint8_t)(Context->a >> 8);
    Digest->bytes[2] = (uint8_t)(Context->a >> 16);
    Digest->bytes[3] = (uint8_t)(Context->a >> 24);
    Digest->bytes[4] = (uint8_t)(Context->b);
    Digest->bytes[5] = (uint8_t)(Context->b >> 8);
    Digest->bytes[6] = (uint8_t)(Context->b >> 16);
    Digest->bytes[7] = (uint8_t)(Context->b >> 24);
    Digest->bytes[8] = (uint8_t)(Context->c);
    Digest->bytes[9] = (uint8_t)(Context->c >> 8);
    Digest->bytes[10] = (uint8_t)(Context->c >> 16);
    Digest->bytes[11] = (uint8_t)(Context->c >> 24);
    Digest->bytes[12] = (uint8_t)(Context->d);
    Digest->bytes[13] = (uint8_t)(Context->d >> 8);
    Digest->bytes[14] = (uint8_t)(Context->d >> 16);
    Digest->bytes[15] = (uint8_t)(Context->d >> 24);
}

//MD5单次计算
void ADT_MD5_Calculate(void const *Buffer,uint32_t BufferSize,ADT_MD5_HASH *Digest)
{
    ADT_MD5_CONTEXT context;

    ADT_MD5_ContextInitialise(&context);
    ADT_MD5_Update(&context, Buffer, BufferSize);
    ADT_MD5_Finalise(&context, Digest);
}

//取消宏定义
#undef F
#undef G
#undef H
#undef I
#undef ADT_MD5_STEP