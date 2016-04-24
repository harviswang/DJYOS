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
//����ģ��:BSP
//���ߣ�������
//�汾��V1.0.0
//�ļ�����:32λ��ARMϵ�й�����������,Ӱ��C����ƽ̨��ز��ֵĴ���,��C�Ⲣ��ֱ��
//       �������ļ�,�����ɽ����Ŀ¼��arch_feature.h�ļ��������ļ�,C�����
//       ��arch_feature.h�ļ�.
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����  : 2014/12/20
//   �޸���: ������
//   �°汾��:
//   �޸�˵��: �½�
//------------------------------------------------------

#ifndef __ARM_FEATURE_H__
#define __ARM_FEATURE_H__

#ifdef __cplusplus
extern "C" {
#endif

//���涨��cpuϵͳ��ջ����
#define CN_FULL_DOWN_STACK      0       //������������ջ
#define CN_EMPTY_DOWN_STACK     1       //���������Ŀ�ջ
#define CN_FULL_UP_STACK        2       //������������ջ
#define CN_EMPTY_UP_STACK       3       //���������Ŀ�ջ
#define CN_STACK_TYPE           CN_FULL_DOWN_STACK

//����һЩ�������صĺ�
#define CN_MEM_ALIGN            1    //���Ŀ��ϵͳû�ж���Ҫ�󣬸�Ϊ0
#define CN_ALIGN_SIZE           8    //arm(��cm3)Ҫ��8�ֽڶ���

//�߳��������У���������Ĵ�������Ҫ��ջ��ARMϵ��CPU��15���Ĵ����뱣�棨SP�������棩
#define CN_REGISTER_STACK     (15*4)

#if !defined(__MAVERICK__)
/* ARM traditionally used big-endian words; and within those words the
   byte ordering was big or little endian depending upon the target.
   Modern floating-point formats are naturally ordered; in this case
   __VFP_FP__ will be defined, even if soft-float.  */
#ifdef __VFP_FP__
# ifdef __ARMEL__
#  define __IEEE_LITTLE_ENDIAN
# else
#  define __IEEE_BIG_ENDIAN
# endif
#else
# define __IEEE_BIG_ENDIAN
# ifdef __ARMEL__
#  define __IEEE_BYTES_LITTLE_ENDIAN
# endif
#endif
#endif

#ifdef __cplusplus
}
#endif
#endif // __ARM_FEATURE_H__

