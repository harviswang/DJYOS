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
//����ģ��:��������
//����:  ������.
//�汾��V1.0.0
//�ļ�����:��������
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef _ENDIAN_H_
#define _ENDIAN_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "arch_feature.h"
#include "stdint.h"
u16 pick_little_16bit(u8 *buf,u32 index);
u32 pick_little_32bit(u8 *buf,u32 index);
u64 pick_little_64bit(u8 *buf,u32 index);
void fill_little_16bit(u8 *buf,u32 index,u16 data);
void fill_little_32bit(u8 *buf,u32 index,u32 data);
void fill_little_64bit(u8 *buf,u32 index,u64 data);
u16 pick_big_16bit(u8 *buf,u32 index);
u32 pick_big_32bit(u8 *buf,u32 index);
u64 pick_big_64bit(u8 *buf,u32 index);
void fill_big_16bit(u8 *buf,u32 index,u16 data);
void fill_big_32bit(u8 *buf,u32 index,u32 data);
void fill_big_64bit(u8 *buf,u32 index,u64 data);

//ϵͳ�ֽ���ת��
 //----16λ���ݴ�С�˶Ի�---------------------------------------------------------
 //����: 16λ��������С�˶һ�
 //����: value,�����16λ����
 //����: ת�����16λ����
 //------------------------------------------------------------------------------
#define swaps(value) (((value&((u16)0xff00))>>8)|((value&((u16)0x00ff))<<8))
 //----32λ���ݴ�С�˶Ի�---------------------------------------------------------
 //����: 32λ��������С�˶һ�
 //����: value,�����32λ����
 //����: ת�����32λ����
 //------------------------------------------------------------------------------

#define swapl(value)  (((value&((u32)0xff000000))>>24)|((value&((u32)0xff0000))>>8)|\
                      ((value&((u32)0xff00))<<8)|((value&((u32)0xff))<<24))
 //----16λ���ݱ��ض�ת��ΪС��---------------------------------------------------
 //����: 16λ���ݱ��ض�ת��ΪС��
 //����: value,�����16λ����
 //����: ת�����16λ����
 //------------------------------------------------------------------------------

#if(CN_CFG_BYTE_ORDER == CN_CFG_BIG_ENDIAN)
#define htoles(value)  swaps(value)
#define htolel(value)  swapl(value)
#define htobes(value)  (value)
#define htobel(value)  (value)
#else
#define htoles(value)  (value)
#define htolel(value)  (value)
#define htobes(value)  swaps(value)
#define htobel(value)  swapl(value)
#endif

//�����ֽ�����
#define htons      htobes      //�����ֽ���ת��Ϊ�����ֽ���16λ��
#define htonl      htobel      //�����ֽ���ת��Ϊ�����ֽ���32λ��
#define ntohs      htobes      //�����ֽ���ת��Ϊ�����ֽ���16λ��
#define ntohl      htobel      //�����ֽ���ת��Ϊ�����ֽ���32λ

#ifdef __cplusplus
}
#endif

#endif // _ENDIAN_H_

