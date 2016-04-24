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
//�ļ�����: ����������ĺ�����
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����  : 2014/12/10
//   �޸���: ������
//   �°汾��:
//   �޸�˵��: �½�
//------------------------------------------------------

#include <stdlib.h>

static u64 s_u64RandSeed = 1;
static u64 s_u64Rand48Seed = 1;

//-----------------------------------------------------------------------------
//����: ��ʼ��α�������rand���������
//����: Seed��������ӣ��������ʱ�ĸ߾���timeֵ�����ߴӶ˿ڶ���һ�����ֵ��
//���: ��
//����: ��
//------------
//���¼�¼:
// 1.����: 2014/12/10
//   ˵��: ����
//   ����: ������
//-----------------------------------------------------------------------------
void srand(u32 Seed)
{
    s_u64RandSeed = Seed;
}

//-----------------------------------------------------------------------------
//����: ȡα�����������ǰ�����һ�������ȥ��ʼ��������ӣ�����srand������
//����: ��
//���: ��
//����: α�����
//------------
//���¼�¼:
// 1.����: 2014/12/10
//   ˵��: ����
//   ����: ������
//-----------------------------------------------------------------------------
s32 rand(void)
{
    s_u64RandSeed = s_u64RandSeed * 6364136223846793005LL + 1;
    return (s32)((s_u64RandSeed >> 32) & CN_LIMIT_SINT32);
}

#define Rand48_M 0x100000000LL
#define Rand48_C 0xB16
#define Rand48_A 0x5DEECE66DLL

//-----------------------------------------------------------------------------
//����: ����32λα�����
//����:
//���: ��
//����: 48λα�����
//------------
//���¼�¼:
// 1.����: 2014/12/10
//   ˵��: ����
//   ����: ������
//-----------------------------------------------------------------------------
u32 lrand48(void)
{
	s_u64Rand48Seed = (Rand48_A * s_u64Rand48Seed + Rand48_C) & 0xFFFFFFFFFFFFLL;
    return (u32)(s_u64Rand48Seed >> 16);
}


//-----------------------------------------------------------------------------
//����: ����0~1֮���α�����
//����:
//���: ��
//����: α���������
//------------
//���¼�¼:
// 1.����: 2014/12/10
//   ˵��: ����
//   ����: ������
//-----------------------------------------------------------------------------
double drand48(void)
{
    u32 x;
    s_u64Rand48Seed = (Rand48_A * s_u64Rand48Seed + Rand48_C) & 0xFFFFFFFFFFFFLL;
    x = (u32)(s_u64Rand48Seed >> 16);
    return  ((double)x / (double)Rand48_M);

}

void srand48(s32 i)
{
    s_u64Rand48Seed  = (long long int)i << 16;
}

