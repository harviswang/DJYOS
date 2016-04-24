//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������¶������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�����������У���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�����������У��Լ�����
//    ������������

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
//����ģ��: c��
//���ߣ�������
//�汾��V1.0.0
//�ļ�����: c���׼���Ͷ��壬����BSP�С�
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����  : 2014/12/17
//   �޸���: ������
//   �°汾��:
//   �޸�˵��: �½�
//------------------------------------------------------
#ifndef _STDINT_H
#define _STDINT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <limits.h>
#include <arch_stdint.h>

typedef uint64_t                u64;
typedef sint64_t                s64;
typedef uint32_t                u32;
typedef sint32_t                s32;
typedef uint16_t                u16;
typedef sint16_t                s16;
typedef uint8_t                 u8;
typedef sint8_t                 s8;
typedef uint64_t const          uc64;
typedef sint64_t const          sc64;
typedef uint32_t const          uc32;
typedef sint32_t const          sc32;
typedef uint16_t const          uc16;
typedef sint16_t const          sc16;
typedef uint8_t  const          uc8;
typedef sint8_t  const          sc8;
typedef unsigned char           ucpu_char;
typedef char                    scpu_char;

typedef volatile ucpu_t         vucpu_t;
typedef volatile scpu_t         vscpu_t;
typedef volatile uint64_t       vu64;
typedef volatile sint64_t       vs64;
typedef volatile uint32_t       vu32;
typedef volatile sint32_t       vs32;
typedef volatile uint16_t       vu16;
typedef volatile sint16_t       vs16;
typedef volatile uint8_t        vu8;
typedef volatile sint8_t        vs8;
typedef volatile ucpu_t   const vuccpu_t;
typedef volatile scpu_t   const vsccpu_t;
typedef volatile uint64_t const vuc64;
typedef volatile sint64_t const vsc64;
typedef volatile uint32_t const vuc32;
typedef volatile sint32_t const vsc32;
typedef volatile uint16_t const vuc16;
typedef volatile sint16_t const vsc16;
typedef volatile uint8_t  const vuc8;
typedef volatile sint8_t  const vsc8;

#define CN_LIMIT_UINT64         0xffffffffffffffff
#define CN_LIMIT_UINT32         0xffffffff
#define CN_LIMIT_UINT16         0xffff
#define CN_LIMIT_UINT8          0xff
#define CN_LIMIT_SINT64         0x7fffffffffffffff
#define CN_LIMIT_SINT32         0x7fffffff
#define CN_LIMIT_SINT16         0x7fff
#define CN_LIMIT_SINT8          0x7f

#define CN_MIN_UINT64           0
#define CN_MIN_UINT32           0
#define CN_MIN_UINT16           0
#define CN_MIN_UINT8            0
#define CN_MIN_SINT64           0x8000000000000000
#define CN_MIN_SINT32           0x80000000
#define CN_MIN_SINT16           0x8000
#define CN_MIN_SINT8            0x80

//�������ͣ��Ǳ�׼c����Ҫ��ġ�
typedef s8 int8_t ;
typedef s8 __int8_t;
typedef u8 __uint8_t;

typedef s8 int_least8_t;
typedef u8 uint_least8_t;

typedef s16 int16_t ;
typedef s16 __int16_t;
typedef u16 __uint16_t;

typedef s16 int_least16_t;
typedef u16 uint_least16_t;

typedef s32 int32_t ;
typedef s32 __int32_t;
typedef u32 __uint32_t;

typedef s32 int_least32_t;
typedef u32 uint_least32_t;

typedef s64 int64_t ;

typedef s64 int_least64_t;
typedef u64 uint_least64_t;

typedef u32 uint_fast8_t;

typedef s32 int_fast16_t;
typedef u32 uint_fast16_t;

typedef s32 int_fast32_t;
typedef u32 uint_fast32_t;

typedef s64 int_fast64_t;
typedef u64 uint_fast64_t;

typedef s64 intmax_t;
typedef u64 uintmax_t;

typedef s8 * intptr_t;
typedef u8 * uintptr_t;

#define INTPTR_MIN      (1<<(CN_PTR_BITS-1))
#define INTPTR_MAX      ((1<<(CN_PTR_BITS-1))-1)
#define UINTPTR_MAX     ((1<<CN_PTR_BITS)-1)


/* Limits of Specified-Width Integer Types */

#define INT8_MIN            CN_MIN_SINT8
#define INT8_MAX            CN_LIMIT_SINT8
#define UINT8_MAX           CN_LIMIT_UINT8

#define INT_LEAST8_MIN      CN_MIN_SINT8
#define INT_LEAST8_MAX      CN_LIMIT_SINT8
#define UINT_LEAST8_MAX     CN_LIMIT_UINT8

#define INT16_MIN           CN_MIN_SINT16
#define INT16_MAX           CN_LIMIT_SINT16
#define UINT16_MAX          CN_LIMIT_UINT16

#define INT_LEAST16_MIN     CN_MIN_SINT16
#define INT_LEAST16_MAX     CN_LIMIT_SINT16
#define UINT_LEAST16_MAX    CN_LIMIT_UINT16

#define INT32_MIN           CN_MIN_SINT32
#define INT32_MAX           CN_LIMIT_SINT32
#define UINT32_MAX          CN_LIMIT_UINT32

#define INT_LEAST32_MIN     CN_MIN_SINT32
#define INT_LEAST32_MAX     CN_LIMIT_SINT32
#define UINT_LEAST32_MAX    CN_LIMIT_UINT32

#define INT64_MIN           CN_MIN_SINT64
#define INT64_MAX           CN_LIMIT_SINT64
#define UINT64_MAX          CN_LIMIT_UINT64

#define INT_LEAST64_MIN     CN_MIN_SINT64
#define INT_LEAST64_MAX     CN_LIMIT_SINT64
#define UINT_LEAST64_MAX    CN_LIMIT_UINT64

#define INT_FAST8_MIN       CN_MIN_SINT32
#define INT_FAST8_MAX       CN_LIMIT_SINT32
#define UINT_FAST8_MAX      CN_LIMIT_UINT32

#define INT_FAST16_MIN      CN_MIN_SINT32
#define INT_FAST16_MAX      CN_LIMIT_SINT32
#define UINT_FAST16_MAX     CN_LIMIT_UINT32

#define INT_FAST32_MIN      CN_MIN_SINT32
#define INT_FAST32_MAX      CN_LIMIT_SINT32
#define UINT_FAST32_MAX     CN_LIMIT_UINT32

#define INT_FAST64_MIN      CN_MIN_SINT64
#define INT_FAST64_MAX      CN_LIMIT_SINT64
#define UINT_FAST64_MAX     CN_LIMIT_UINT64

#define INTMAX_MIN          CN_MIN_SINT64
#define INTMAX_MAX          CN_LIMIT_SINT64
#define UINTMAX_MAX         CN_LIMIT_UINT32

#define SIZE_MAX            CN_LIMIT_UINT32

#define PTRDIFF_MAX         CN_LIMIT_PTU32
#define PTRDIFF_MIN         CN_MIN_PTU32

#define     __WCHAR32
#if defined(__WCHAR32)
    #define WCHAR_MIN       CN_MIN_SINT32
    #define WCHAR_MAX       CN_LIMIT_SINT32
    #define WINT_MIN       0
    #define WINT_MAX       CN_LIMIT_UINT32
#else
    #define WCHAR_MIN       CN_MIN_SINT16
    #define WCHAR_MAX       CN_LIMIT_SINT16
    #define WINT_MIN       0
    #define WINT_MAX       CN_LIMIT_UINT16
#endif


#ifdef __cplusplus
}
#endif

#endif /* _STDINT_H */
