/***********************************************************************************************
** Author        : DengXiaoJun(邓小俊)
** Date          : 2023-03-09 11:31:44 +0800
** Description   : When I Has Time ,I Will Write Description Below:
** ModifyRecord1 :
** ModifyRecord2 :
** LastEditors   : DengXiaoJun(邓小俊)
** LastEditTime  : 2023-03-09 11:31:52 +0800
************************************************************************************************/
#include "ADT_SHA1.h"

// 是否使用小端计算
#define USE_LITTLE_ENDIAN_SHORTCUT

//定义联合体,方便访问
typedef union
{
    uint8_t c[64];
    uint32_t l[16];
}ADT_SHA1_CHAR64_LONG16;

#define ADT_SHA1_LOAD32H(x, y)                 \
    {                                          \
        x = ((uint32_t)((y)[0] & 255) << 24) | \
            ((uint32_t)((y)[1] & 255) << 16) | \
            ((uint32_t)((y)[2] & 255) << 8) |  \
            ((uint32_t)((y)[3] & 255));        \
    }

#define ADT_SHA1_ROL(value, bits) (((value) << (bits)) | ((value) >> (32 - (bits))))

#ifdef USE_LITTLE_ENDIAN_SHORTCUT
#define ADT_SHA1_BLK0(indexMain) (block->l[indexMain] = (ADT_SHA1_ROL(block->l[indexMain], 24) & 0xFF00FF00) | (ADT_SHA1_ROL(block->l[indexMain], 8) & 0x00FF00FF))
#else
#define ADT_SHA1_BLK0(indexMain) block->l[indexMain]
#endif

#define ADT_SHA1_BLK(indexMain) (block->l[indexMain & 15] = ADT_SHA1_ROL(block->l[(indexMain + 13) & 15] ^ block->l[(indexMain + 8) & 15] ^ block->l[(indexMain + 2) & 15] ^ block->l[indexMain & 15], 1))

// (R0+R1), R2, R3, R4 are the different operations used in SHA1
#define ADT_SHA1_R0(v, w, x, y, z, indexMain)                                     \
    z += ((w & (x ^ y)) ^ y) + ADT_SHA1_BLK0(indexMain) + 0x5A827999 + ADT_SHA1_ROL(v, 5); \
    w = ADT_SHA1_ROL(w, 30);
#define ADT_SHA1_R1(v, w, x, y, z, indexMain)                                    \
    z += ((w & (x ^ y)) ^ y) + ADT_SHA1_BLK(indexMain) + 0x5A827999 + ADT_SHA1_ROL(v, 5); \
    w = ADT_SHA1_ROL(w, 30);
#define ADT_SHA1_R2(v, w, x, y, z, indexMain)                            \
    z += (w ^ x ^ y) + ADT_SHA1_BLK(indexMain) + 0x6ED9EBA1 + ADT_SHA1_ROL(v, 5); \
    w = ADT_SHA1_ROL(w, 30);
#define ADT_SHA1_R3(v, w, x, y, z, indexMain)                                          \
    z += (((w | x) & y) | (w & x)) + ADT_SHA1_BLK(indexMain) + 0x8F1BBCDC + ADT_SHA1_ROL(v, 5); \
    w = ADT_SHA1_ROL(w, 30);
#define ADT_SHA1_R4(v, w, x, y, z, indexMain)                            \
    z += (w ^ x ^ y) + ADT_SHA1_BLK(indexMain) + 0xCA62C1D6 + ADT_SHA1_ROL(v, 5); \
    w = ADT_SHA1_ROL(w, 30);

// Loads the 128 bits from ByteArray into WordArray, treating ByteArray as big endian data
#ifdef USE_LITTLE_ENDIAN_SHORTCUT
#define ADT_SHA1_Load128BitsAsWords(WordArray, ByteArray) \
    ADT_BASE_MEM_COPY(WordArray, ByteArray, 64)
#else
#define ADT_SHA1_Load128BitsAsWords(WordArray, ByteArray)   \
    {                                                       \
        uint32_t indexMain;                                         \
        for (indexMain = 0; indexMain < 16; indexMain++)                            \
        {                                                   \
            ADT_SHA1_LOAD32H((WordArray)[indexMain], (ByteArray) + (indexMain * 4)); \
        }                                                   \
    }
#endif

//块计算
static void ADT_SHA1_TransformFunction(uint32_t state[5],uint8_t const buffer[64])
{
    uint32_t a;
    uint32_t b;
    uint32_t c;
    uint32_t d;
    uint32_t e;
    uint8_t workspace[64];
    ADT_SHA1_CHAR64_LONG16 *block = (ADT_SHA1_CHAR64_LONG16 *)workspace;

    ADT_SHA1_Load128BitsAsWords((void*)(block->l), buffer);

    // Copy context->state[] to working vars
    a = state[0];
    b = state[1];
    c = state[2];
    d = state[3];
    e = state[4];

    // 4 rounds of 20 operations each. Loop unrolled.
    ADT_SHA1_R0(a, b, c, d, e, 0);
    ADT_SHA1_R0(e, a, b, c, d, 1);
    ADT_SHA1_R0(d, e, a, b, c, 2);
    ADT_SHA1_R0(c, d, e, a, b, 3);
    ADT_SHA1_R0(b, c, d, e, a, 4);
    ADT_SHA1_R0(a, b, c, d, e, 5);
    ADT_SHA1_R0(e, a, b, c, d, 6);
    ADT_SHA1_R0(d, e, a, b, c, 7);
    ADT_SHA1_R0(c, d, e, a, b, 8);
    ADT_SHA1_R0(b, c, d, e, a, 9);
    ADT_SHA1_R0(a, b, c, d, e, 10);
    ADT_SHA1_R0(e, a, b, c, d, 11);
    ADT_SHA1_R0(d, e, a, b, c, 12);
    ADT_SHA1_R0(c, d, e, a, b, 13);
    ADT_SHA1_R0(b, c, d, e, a, 14);
    ADT_SHA1_R0(a, b, c, d, e, 15);
    ADT_SHA1_R1(e, a, b, c, d, 16);
    ADT_SHA1_R1(d, e, a, b, c, 17);
    ADT_SHA1_R1(c, d, e, a, b, 18);
    ADT_SHA1_R1(b, c, d, e, a, 19);
    ADT_SHA1_R2(a, b, c, d, e, 20);
    ADT_SHA1_R2(e, a, b, c, d, 21);
    ADT_SHA1_R2(d, e, a, b, c, 22);
    ADT_SHA1_R2(c, d, e, a, b, 23);
    ADT_SHA1_R2(b, c, d, e, a, 24);
    ADT_SHA1_R2(a, b, c, d, e, 25);
    ADT_SHA1_R2(e, a, b, c, d, 26);
    ADT_SHA1_R2(d, e, a, b, c, 27);
    ADT_SHA1_R2(c, d, e, a, b, 28);
    ADT_SHA1_R2(b, c, d, e, a, 29);
    ADT_SHA1_R2(a, b, c, d, e, 30);
    ADT_SHA1_R2(e, a, b, c, d, 31);
    ADT_SHA1_R2(d, e, a, b, c, 32);
    ADT_SHA1_R2(c, d, e, a, b, 33);
    ADT_SHA1_R2(b, c, d, e, a, 34);
    ADT_SHA1_R2(a, b, c, d, e, 35);
    ADT_SHA1_R2(e, a, b, c, d, 36);
    ADT_SHA1_R2(d, e, a, b, c, 37);
    ADT_SHA1_R2(c, d, e, a, b, 38);
    ADT_SHA1_R2(b, c, d, e, a, 39);
    ADT_SHA1_R3(a, b, c, d, e, 40);
    ADT_SHA1_R3(e, a, b, c, d, 41);
    ADT_SHA1_R3(d, e, a, b, c, 42);
    ADT_SHA1_R3(c, d, e, a, b, 43);
    ADT_SHA1_R3(b, c, d, e, a, 44);
    ADT_SHA1_R3(a, b, c, d, e, 45);
    ADT_SHA1_R3(e, a, b, c, d, 46);
    ADT_SHA1_R3(d, e, a, b, c, 47);
    ADT_SHA1_R3(c, d, e, a, b, 48);
    ADT_SHA1_R3(b, c, d, e, a, 49);
    ADT_SHA1_R3(a, b, c, d, e, 50);
    ADT_SHA1_R3(e, a, b, c, d, 51);
    ADT_SHA1_R3(d, e, a, b, c, 52);
    ADT_SHA1_R3(c, d, e, a, b, 53);
    ADT_SHA1_R3(b, c, d, e, a, 54);
    ADT_SHA1_R3(a, b, c, d, e, 55);
    ADT_SHA1_R3(e, a, b, c, d, 56);
    ADT_SHA1_R3(d, e, a, b, c, 57);
    ADT_SHA1_R3(c, d, e, a, b, 58);
    ADT_SHA1_R3(b, c, d, e, a, 59);
    ADT_SHA1_R4(a, b, c, d, e, 60);
    ADT_SHA1_R4(e, a, b, c, d, 61);
    ADT_SHA1_R4(d, e, a, b, c, 62);
    ADT_SHA1_R4(c, d, e, a, b, 63);
    ADT_SHA1_R4(b, c, d, e, a, 64);
    ADT_SHA1_R4(a, b, c, d, e, 65);
    ADT_SHA1_R4(e, a, b, c, d, 66);
    ADT_SHA1_R4(d, e, a, b, c, 67);
    ADT_SHA1_R4(c, d, e, a, b, 68);
    ADT_SHA1_R4(b, c, d, e, a, 69);
    ADT_SHA1_R4(a, b, c, d, e, 70);
    ADT_SHA1_R4(e, a, b, c, d, 71);
    ADT_SHA1_R4(d, e, a, b, c, 72);
    ADT_SHA1_R4(c, d, e, a, b, 73);
    ADT_SHA1_R4(b, c, d, e, a, 74);
    ADT_SHA1_R4(a, b, c, d, e, 75);
    ADT_SHA1_R4(e, a, b, c, d, 76);
    ADT_SHA1_R4(d, e, a, b, c, 77);
    ADT_SHA1_R4(c, d, e, a, b, 78);
    ADT_SHA1_R4(b, c, d, e, a, 79);

    // Add the working vars back into context.state[]
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;
}

void ADT_SHA1_Initialise(ADT_SHA1_CONTEXT *Context)
{
    // SHA1 initialisation constants
    Context->State[0] = 0x67452301;
    Context->State[1] = 0xEFCDAB89;
    Context->State[2] = 0x98BADCFE;
    Context->State[3] = 0x10325476;
    Context->State[4] = 0xC3D2E1F0;
    Context->Count[0] = 0;
    Context->Count[1] = 0;
}

void ADT_SHA1_Update(ADT_SHA1_CONTEXT *Context,void const *Buffer,uint32_t BufferSize)
{
    uint32_t indexMain;
    uint32_t indexSub;

    indexSub = (Context->Count[0] >> 3) & 63;
    if ((Context->Count[0] += BufferSize << 3) < (BufferSize << 3))
    {
        Context->Count[1]++;
    }

    Context->Count[1] += (BufferSize >> 29);
    if ((indexSub + BufferSize) > 63)
    {
        indexMain = 64 - indexSub;
        ADT_BASE_MEM_COPY(&Context->Buffer[indexSub], Buffer, indexMain);
        ADT_SHA1_TransformFunction(Context->State, Context->Buffer);
        for (; indexMain + 63 < BufferSize; indexMain += 64)
        {
            ADT_SHA1_TransformFunction(Context->State, (uint8_t *)Buffer + indexMain);
        }
        indexSub = 0;
    }
    else
    {
        indexMain = 0;
    }

    ADT_BASE_MEM_COPY(&Context->Buffer[indexSub], &((uint8_t *)Buffer)[indexMain], BufferSize - indexMain);
}

void ADT_SHA1_Finalise(ADT_SHA1_CONTEXT *Context,ADT_SHA1_HASH *Digest)
{
    uint32_t indexMain;
    uint8_t finalcount[8];

    for (indexMain = 0; indexMain < 8; indexMain++)
    {
        finalcount[indexMain] = (unsigned char)((Context->Count[(indexMain >= 4 ? 0 : 1)] >> ((3 - (indexMain & 3)) * 8)) & 255); // Endian independent
    }
    ADT_SHA1_Update(Context, (uint8_t *)"\x80", 1);
    while ((Context->Count[0] & 504) != 448)
    {
        ADT_SHA1_Update(Context, (uint8_t *)"\0", 1);
    }

    ADT_SHA1_Update(Context, finalcount, 8); // Should cause a Sha1TransformFunction()
    for (indexMain = 0; indexMain < ADT_SHA1_HASH_SIZE; indexMain++)
    {
        Digest->bytes[indexMain] = (uint8_t)((Context->State[indexMain >> 2] >> ((3 - (indexMain & 3)) * 8)) & 255);
    }
}

void ADT_SHA1_Calculate(void const *Buffer,uint32_t BufferSize,ADT_SHA1_HASH *Digest)
{
    ADT_SHA1_CONTEXT context;

    ADT_SHA1_Initialise(&context);
    ADT_SHA1_Update(&context, Buffer, BufferSize);
    ADT_SHA1_Finalise(&context, Digest);
}
