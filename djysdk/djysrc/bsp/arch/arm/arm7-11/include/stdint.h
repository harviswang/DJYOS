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
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
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
//����ģ��:ƽ̨�޹��������Ͷ���
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����: �ں�����ͷ�ļ�
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-10-16
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __STDINT_H__
#define __STDINT_H__

#ifdef __cplusplus
extern "C" {
#endif


typedef unsigned  long long     uint64_t;
typedef signed    long long     sint64_t;
typedef unsigned  int           uint32_t;
typedef int                     sint32_t;
typedef int                     int32_t;
typedef unsigned  short         uint16_t;
typedef short int               sint16_t;
typedef short int               int16_t;
typedef unsigned  char          uint8_t;
typedef signed  char            sint8_t;
typedef signed  char            int8_t;
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

typedef volatile uint64_t       vu64;
typedef volatile sint64_t       vs64;
typedef volatile uint32_t       vu32;
typedef volatile sint32_t       vs32;
typedef volatile uint16_t       vu16;
typedef volatile sint16_t       vs16;
typedef volatile uint8_t        vu8;
typedef volatile sint8_t        vs8;
typedef volatile uint64_t const vuc64;
typedef volatile sint64_t const vsc64;
typedef volatile uint32_t const vuc32;
typedef volatile sint32_t const vsc32;
typedef volatile uint16_t const vuc16;
typedef volatile sint16_t const vsc16;
typedef volatile uint8_t  const vuc8;
typedef volatile sint8_t  const vsc8;


//ptu32_t������cpu��Ѱַ��Χ�йأ�Ҳ��CPU�Ľṹ�йأ������ϵͳ��ָ�볤��С�ڻ�
//����32λ����ptu32_t������Ϊ32λ��������ָ��ȳ���������;�ж�:
//1.�����ڴ�����ָ���޶�,�������������ڴ泤�ȡ�
//2.���ڿ�����Ҫת����ָ��ʹ�õ��������ݣ�����struct pan_device�ṹ�е�
//  ptu32_t  private_tag
//һ����˵ptu32_t��ucpu_t��ͬ,��Ҳ������,������8λ����16λ����,����51����ucpu_t
//��8λ��,��Ѱַ��Χ��16λ��,ptu32_t=16λ.80c296��Ƭ����ucpu_t��16λ�ģ�Ѱַ��Χ
//��24λ�ģ�ptu32_t=32λ��
typedef uint32_t                ptu32_t;

typedef uint32_t                ucpu_t;

typedef sint32_t                scpu_t;

typedef u8                      ufast_t;

typedef s8                      sfast_t;

typedef u64                     align_type;     //arm�У�u64���������ж���Ҫ��

#ifndef false
  #define false (ufast_t)0
#endif
#ifndef true
  #define true  (ufast_t)1
#endif

typedef ufast_t                 bool_t;

//typedef s64             time_t;

#define CN_LIMIT_UINT64         0xffffffffffffffff
#define CN_LIMIT_UINT32         0xffffffff
#define CN_LIMIT_PTU32          0xffffffff
#define CN_LIMIT_UINT16         0xffff
#define CN_LIMIT_UINT8          0xff
#define CN_LIMIT_SINT64         0x7fffffffffffffff
#define CN_LIMIT_SINT32         0x7fffffff
#define CN_LIMIT_SINT16         0x7fff
#define CN_LIMIT_SINT8          0x7f

#define CN_MIN_UINT64           0
#define CN_MIN_PTU32            0
#define CN_MIN_UINT32           0
#define CN_MIN_UINT16           0
#define CN_MIN_UINT8            0
#define CN_MIN_SINT64           0x8000000000000000
#define CN_MIN_SINT32           0x80000000
#define CN_MIN_SINT16           0x8000
#define CN_MIN_SINT8            0x80

#ifdef __cplusplus
}
#endif
#endif // __STDINT_H_


