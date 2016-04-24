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
//����ģ��:BSP
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����: 32λARMϵ����C���stdint��ص�����,��C�Ⲣ��ֱ�Ӱ������ļ�,������
//        �����Ŀ¼��arch-stdint.h�ļ��н��������ļ�.C���stdint.h�����
//        arch-stdint.h�ļ�.
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-10-16
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __ARM_STDINT_H__
#define __ARM_STDINT_H__

#ifdef __cplusplus
extern "C" {
#endif

//������������
typedef unsigned  long long     uint64_t;
typedef signed    long long     sint64_t;
typedef unsigned  int           uint32_t;
typedef int                     sint32_t;
typedef unsigned  short         uint16_t;
typedef short int               sint16_t;
typedef unsigned  char          uint8_t;
typedef signed  char            sint8_t;

//������CPU������һ��ָ�����������ͣ���������Ҫ�������������ԵĲ������Լ�
//��Ҫ���ٵĲ�����
//���������Ժ�ԭ�Ӳ�������ͬ:
//������������ָ���κ�ʱ���ܶ�����ȷֵ�����ݣ������������:
//  uint32_t    b32;
//  b32++;  //b32��ԭֵ��0x0000ffff
//��32λrisc���ϣ�ִ�й���Ϊ��
//ȡb32��ַ���Ĵ���0-->ȡb32����ֵ���Ĵ���1-->�Ĵ���1��1-->�ѼĴ���1д��b32.
//�����������������ﱻ�жϻ��߱������ȼ��̴߳�ϣ����жϻ�����ȼ��߳��ж�b32��
//Ҫô�õ�0x0000ffff��Ҫô�õ�0x00010000,������������Ϊ����ȷ����.
//����16λ���ϣ�ִ�й�����
//1��ȡ0x0000���Ĵ���r0��ȡ0xffff���Ĵ���r1
//2��ִ�мӲ�����ʹr0=0x0001,r1=0x0000
//3����r0д��b32�ĸ�16λ��
//4����r1д��b32�ĵ�16λ��
//����ڵ�3�͵�4����֮�䱻�жϴ�ϣ����ж����b32�����õ������0x00001ffff��
//��ôԭ�Ӳ�����?��������ִ�й��̲����κ��жϻ��߸����ȼ��̴߳�ϣ�����������룬
//���b32������Ϊԭ�ӱ�������������16λ����32λ���ϣ�b32�����Եõ���ȷ��ֵ��Ŀ
//ǰ��djyos����֧��ԭ�ӱ�����ֻ��ʹ��ԭ�Ӳ���ʵ��ԭ�ӱ�����Ч����
typedef uint32_t                ucpu_t;
typedef sint32_t                scpu_t;

//ptu32_t������cpu��Ѱַ��Χ�йأ�Ҳ��CPU�Ľṹ�йأ������ϵͳ��ָ�볤��С�ڻ�
//����32λ����ptu32_t������Ϊ32λ��������ָ��ȳ���������;�ж�:
//1.�����ڴ�����ָ���޶�,�������������ڴ泤�ȡ�
//2.���ڿ�����Ҫת����ָ��ʹ�õ��������ݣ�����struct pan_device�ṹ�е�
//  ptu32_t  private_tag
//һ����˵ptu32_t��ucpu_t��ͬ,��Ҳ������,������8λ����16λ����,����51����ucpu_t
//��8λ��,��Ѱַ��Χ��16λ��,ptu32_t=16λ.80c296��Ƭ����ucpu_t��16λ�ģ�Ѱַ��Χ
//��24λ�ģ�ptu32_t=32λ��
typedef uint32_t                ptu32_t;

//�������ͣ�һ�����CPU�ֳ��ȳ������ͣ������������ֱ���ڼĴ����в���
typedef ucpu_t                  ufast_t;
typedef scpu_t                  sfast_t;

//�������ͣ�ѡ����ӦCPU��������������
typedef ufast_t                 bool_t;

//��CPU�ֳ���ص�����
#define CN_CPU_BITS             32  //�������ֳ�
#define CN_PTR_BITS             32  //ָ���ֳ�����ֲ����
#define CN_CPU_BITS_SUFFIX_ZERO 5
#define CN_BYTE_BITS            8   //�ֽ�λ��

#define CN_LIMIT_PTU32          CN_LIMIT_UINT32
#define CN_MIN_PTU32            0
#define CN_LIMIT_ALIGN          CN_LIMIT_UINT64
#define CN_MIN_ALIGN            0

#define CN_LIMIT_UCPU           CN_LIMIT_UINT32
#define CN_MIN_UCPU             0
#define CN_LIMIT_SCPU           CN_LIMIT_SINT32
#define CN_MIN_SCPU             CN_MIN_SINT32
//�����ǿ����������ͣ�һ����˵��CPU�����ǿ����������ͣ�����Щϵͳ�洢�����С��
//CPU�ֳ������´Ӵ洢����һ��cpu�ֳ�������Ҫ����洢�����ڣ�����,16λ��32λcpu
//��,�ֽڲ���Ҳ����һ�����.������Щ���ֳ�û��Ҫ�������������˵��ʹ����洢��
//��ȵȳ��ı���Ҳ��������ٵġ�����16λSDRAM��44b0x����������16λ��.
#define CN_LIMIT_UFAST          CN_LIMIT_UINT32
#define CN_MIN_UFAST            (0)
#define CN_LIMIT_SFAST          CN_LIMIT_SINT32
#define CN_MIN_SFAST            CN_MIN_SINT32

//������isoC ����Ҫ��
#undef  INT_MAX
#undef  UINT_MAX
#define INT_MAX                 0x7fffffff
#define UINT_MAX                0xffffffff

typedef uint64_t                align_type;     //arm�У�u64���������ж���Ҫ��

#ifdef __cplusplus
}
#endif
#endif // __ARM_STDINT_H__



