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
//����ģ��:C��
//���ߣ�������
//�汾��V1.0.0
//�ļ�����: ����ת�����ַ���
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2015-01-26
//   ����: ������
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
//----��ֵת�����ַ���---------------------------------------------------------
//����: ����ֵdת����Ϊָ�����������ַ���
//����: d��Ҫת������ֵ
//      buf�����ת���õ��ַ����Ļ�����
//      scale��ת���Ľ��ƣ���Ϊ-10,10,-16,16
//			scaleΪ��ֵ��ʱ���ʾdΪ�з�����������Ϊ�޷�������
//����: 1
//������d=5628, scale=10 �������Ϊ buf="5628"
//-----------------------------------------------------------------------------
char * itoa(s32 d, char *buf, s32 scale)
{
	unsigned char str_inv[16];	// ���������
	u32 ud;	// d���޷��ű�ʾ
	char *str = buf;
	u8 i, j, k;

	i = 0;
	// ������ȥ��
	if (scale < 0)
	{
		scale = -scale;
		if (d < 0)
		{
			ud = (unsigned int)(-d);
			str_inv[i++] = '-';
		}
		else
		{
			ud = (unsigned int)d;
		}
	}
	else
	{
		ud = (unsigned int)d;
	}

	// 16������ת��
	if (scale == 16)
	{
		do	// ���4λ�Ƿ���ֵ
		{
			str_inv[i] = (u8)(ud & 0x0F);	// ��ĩ4λ��ֵ
			ud >>= 4;	// ����4λ

			if ((str_inv[i] >= 0) && (str_inv[i] <= 9))
			{
				str_inv[i] += '0';
			}
			else if ((str_inv[i] >= 0x0A) && (str_inv[i] <= 0x0F))
			{
				str_inv[i] += ('a'-0x0A);
			}

			i++;
		} while (ud);
		i--;	// �˸�����λ��

		// �������
		if (str_inv[0] == '-')
		{
			*str++ = '-';
			j = 1;
		}
		else
		{
			j = 0;
		}

		k = i-j;	// ����λ��
		k += 1;		// ѭ����ȡ����
		for (; k; k--)
		{
			*str++ = str_inv[i--];
		}
		*str = '\0';	// ������
	}
	else if (scale == 10)
	{
		// ����ת��
		do
		{
			str_inv[i++] = (u8)((ud % 10) + '0');
			ud /= 10;
		} while (ud);
		i--;	// �˸�����λ��

		// �������
		if (str_inv[0] == '-')
		{
			*str++ = '-';
			j = 1;
		}
		else
		{
			j = 0;
		}

		k = i-j;	// ����λ��
		k += 1;		// ѭ����ȡ����
		for (; k; k--)
		{
			*str++ = str_inv[i--];
		}
		*str = '\0';	// ������
	}
    return buf;
}
