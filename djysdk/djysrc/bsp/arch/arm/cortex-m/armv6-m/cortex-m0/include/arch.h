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
//����ģ��:�ѹ���ģ��
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
#ifndef __ARCH_H_
#define __ARCH_H_

#ifdef __cplusplus
extern "C" {
#endif

//�洢����С�˵�����
#define CN_CFG_LITTLE_ENDIAN       0
#define CN_CFG_BIG_ENDIAN          1
#define CN_CFG_BYTE_ORDER          CN_CFG_LITTLE_ENDIAN

//��CPU�ֳ���ص�����
#define CN_CPU_BITS     32  //�������ֳ�
#define CN_PTR_BITS     32  //CPU��ַλ������ֲ����
#define CN_CPU_BITS_SUFFIX_ZERO 5
#define CN_BYTE_BITS    8   //�ֽ�λ��

    #define CN_LIMIT_UCPU   CN_LIMIT_UINT32
    #define CN_MIN_UCPU     (0)

    #define CN_LIMIT_SCPU   CN_LIMIT_SINT32
    #define CN_MIN_SCPU     CN_MIN_SINT32

    #define CN_LIMIT_UFAST  CN_LIMIT_UINT8
    #define CN_MIN_UFAST    (0)

    #define CN_LIMIT_SFAST  CN_LIMIT_SINT8
    #define CN_MIN_SFAST    CN_MIN_SINT8

//���涨��cpuϵͳ��ջ����
#define CN_FULL_DOWN_STACK  0   //������������ջ
#define CN_EMPTY_DOWN_STACK 1   //���������Ŀ�ջ
#define CN_FULL_UP_STACK    2   //������������ջ
#define CN_EMPTY_UP_STACK   3   //���������Ŀ�ջ
#define CN_STACK_TYPE       CN_FULL_DOWN_STACK

//�����Ƿ�֧��cache��loader���ݱ����������Ƿ���cacheͬ��
#define cn_cache_used       0
#define cn_mmu_used         0   //�����Ƿ�֧��mmu
#define cortex_m0

//�����ں�ջ����ȼ��㷽��:ջ����������Ǹ�ϵͳ����
#define CN_KERNEL_STACK     300     //�����ں�ջ���߳���ҪΪϵͳ����������ӵ�ջ
                                    //Ŀǰ���api����dev_add_root_device ��256bytes
                                    //----2009-10-11

//����һЩ�������صĺ�
#define MEM_ALIGN           1           //���Ŀ��ϵͳû�ж���Ҫ�󣬸�Ϊ0
#define ALIGN_SIZE          8           //arm(��cm3)Ҫ��8�ֽڶ���

#define align_down(bound,x) ((x)&(~((bound)-1)))    //��x��bound�߽����¶���
#define align_up(bound,x)   (((x)+((bound)-1))&(~((bound)-1)))  //���϶���,

//����������ϵͳ�����ݶ��뷽ʽ����ֲ����
#define align_down_sys(x)   align_down(8,x)   //armҪ��8�ֽڶ���
#define align_up_sys(x)     align_up(8,x)     //armҪ��8�ֽڶ���

#define define_align_buf(name,size)     \
                align_type name[align_up_sys(size)/sizeof(ALIGN_SIZE)]

#ifdef __cplusplus
}
#endif
#endif // __ARCH_H_


