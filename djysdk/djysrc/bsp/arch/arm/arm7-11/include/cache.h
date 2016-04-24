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
//����ģ��:������
//����:  ������.
//�汾��V1.0.0
//�ļ�����:cache������صĴ��롣
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2011-10-21
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __CACHE_H__
#define __CACHE_H__

#ifdef __cplusplus
extern "C" {
#endif

enum _EN_CACHE_OPERATION_TYPE_
{
    EN_WRITE_TO_NO_INVALID_NO_LOCK,    //дͨ������Ҫ�������֧������
};
struct tagCacheTypeFlag
{
    u32 ints_cache_type:12;     //ָ��cache����
    u32 data_cache_type:12;     //����cache����
    u32 uni_cache:1;            //1=ָ������ݶ���cache��0=ͳһcache
    u32 operation_type:4;       //cache�������͡�
};


#if defined( arm720t)
    #define CN_CACHE_SIZE       0x2000
    #define CN_CACHE_WAY        4
    #define CN_CACHE_LINE_SIZE  16
#elif defined( arm740t)     //740t cache size�ɱ䣬��arch.h�ж���cache�ߴ�
    #define CN_CACHE_WAY        4
    #define CN_CACHE_LINE_SIZE  16
#elif defined( arm920t)
    #define CN_CACHE_SIZE       0x4000
    #define CN_CACHE_WAY        64
    #define CN_CACHE_LINE_SIZE  32
    #define CN_C7_OFFSET_SET    5   //��������c7�е�ƫ��λ��
    #define CN_C7_OFFSET_WAY    26  //·������c7�е�ƫ��λ��
    #define CN_C9_OFFSET_WAY    26  //·������c9�е�ƫ��λ��
#elif defined( arm922t)
    #define CN_CACHE_SIZE       0x2000
    #define CN_CACHE_WAY        64
    #define CN_CACHE_LINE_SIZE  32
    #define CN_C7_OFFSET_SET    5   //��������c7�е�ƫ��λ��
    #define CN_C7_OFFSET_WAY    26  //·������c7�е�ƫ��λ��
    #define CN_C9_OFFSET_WAY    26  //·������c9�е�ƫ��λ��
#elif defined( arm940t)
    #define CN_CACHE_SIZE       0x1000
    #define CN_CACHE_WAY        64
    #define CN_CACHE_LINE_SIZE  16
    #define CN_C7_OFFSET_SET    4   //��������c7�е�ƫ��λ��
    #define CN_C7_OFFSET_WAY    26  //·������c7�е�ƫ��λ��
    #define CN_C9_OFFSET_WAY    0   //·������c9�е�ƫ��λ��
#elif defined( arm926ej_s)  //926ej_s cache size�ɱ�,��arch.h�ж���cache�ߴ�
    #define CN_CACHE_WAY        4
    #define CN_CACHE_LINE_SIZE  32
    #define CN_C7_OFFSET_SET    4   //��������c7�е�ƫ��λ��
    #define CN_C7_OFFSET_WAY    30  //·������c7�е�ƫ��λ��
#elif defined( arm946e_s)    //946e_s cache size�ɱ�,��arch.h�ж���cache�ߴ�
    #define CN_CACHE_WAY        4
    #define CN_CACHE_LINE_SIZE  16
    #define CN_C7_OFFSET_SET    4   //��������c7�е�ƫ��λ��
    #define CN_C7_OFFSET_WAY    30  //·������c7�е�ƫ��λ��
    #define CN_C9_OFFSET_WAY    0   //·������c9�е�ƫ��λ��
#elif defined( arm1022e)
    #define CN_CACHE_SIZE       0x4000
    #define CN_CACHE_WAY        64
    #define CN_CACHE_LINE_SIZE  32
    #define CN_C7_OFFSET_SET    5   //��������c7�е�ƫ��λ��
    #define CN_C7_OFFSET_WAY    26  //·������c7�е�ƫ��λ��
    #define CN_C9_OFFSET_WAY    26  //·������c9�е�ƫ��λ��
#elif defined( arm1026ej_s)//1026ej_s cache size�ɱ�,��arch.h�ж���cache�ߴ�
    #define CN_CACHE_WAY        4
    #define CN_CACHE_LINE_SIZE  32
    #define CN_C7_OFFSET_SET    5   //��������c7�е�ƫ��λ��
    #define CN_C7_OFFSET_WAY    30  //·������c7�е�ƫ��λ��
#elif defined( strong_arm)
    #define CN_CACHE_SIZE       0x4000
    #define CN_CACHE_WAY        32
    #define CN_CACHE_LINE_SIZE  16
#elif defined( xscale)
    #define CN_CACHE_SIZE       0x8000
    #define CN_CACHE_WAY        32
    #define CN_CACHE_LINE_SIZE  32
#endif

void Cache_InvalidAll(void);
void Cache_InvalidInst(void);
void Cache_InvalidData(void);
void Cache_EnableAll(void);
void Cache_EnableInst(void);
void Cache_EnableData(void);
void Cache_DisableAll(void);
void Cache_DisableInst(void);
void Cache_DisableData(void);
void Cache_EnableWriteBuf(void);
void Cache_DisableWriteBuf(void);
void Cache_CleanData(void);


#else
//���°�cache��������ȫ���ÿա�
#define Cache_InvalidAll()
#define Cache_InvalidInst()
#define Cache_InvalidData()
#define Cache_EnableAll()
#define Cache_EnableInst()
#define Cache_EnableData()
#define Cache_DisableAll()
#define Cache_DisableInst()
#define Cache_DisableData()
#define Cache_EnableWriteBuf()
#define Cache_DisableWriteBuf()
#define Cache_CleanData()

#endif

#ifdef __cplusplus
}

#endif /*__CACHE_H__*/

