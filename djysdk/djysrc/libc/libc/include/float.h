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
//����ģ��: stdlib
//���ߣ�������
//�汾��V1.0.0
//�ļ�����: �����޶�
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����  : 2014/12/11
//   �޸���: ������
//   �°汾��:
//   �޸�˵��: �½�
//------------------------------------------------------

#ifndef _FLOAT_H___
#define _FLOAT_H___

//���¶��壬�μ���ISO/IEC 9899:1999 ����27ҳ
#define FLT_RADIX           2
#define DECIMAL_DIG         17

#define FLT_MANT_DIG        24
#define FLT_EPSILON         (1.19209290E-07F)
#define FLT_DIG             6
#define FLT_MIN_EXP         (-125)
#define FLT_MIN             (1.17549435E-38F)
#define FLT_MIN_10_EXP      (-37)
#define FLT_MAX_EXP         128
#define FLT_MAX             (3.40282347E+38F)
#define FLT_MAX_10_EXP      38

#define DBL_MANT_DIG        53
#define DBL_EPSILON         (2.2204460492503131E-16)
#define DBL_DIG             15
#define DBL_MIN_EXP         (-1021)
#define DBL_MIN             (2.2250738585072014E-308)
#define DBL_MIN_10_EXP      (-307)
#define DBL_MAX_EXP         1024
#define DBL_MAX             (1.7976931348623157E+308)
#define DBL_MAX_10_EXP      308

#define LDBL_MANT_DIG       53
#define LDBL_EPSILON        (2.2204460492503131E-16)
#define LDBL_DIG            15
#define LDBL_MIN_EXP        (-1021)
#define LDBL_MIN            (2.2250738585072014E-308)
#define LDBL_MIN_10_EXP     (-307)
#define LDBL_MAX_EXP        1024
#define LDBL_MAX            (1.7976931348623157E+308)
#define LDBL_MAX_10_EXP     308

#endif /* _FLOAT_H___ */

