//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 3. As a constituent part of djyos,do not transplant it to other software
//    without specific prior written permission.

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
// �����Ȩ�����ʹ���߷��������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������
// 3. �������Ϊ�����߲���ϵͳ����ɲ��֣�δ����ǰȡ�õ�������ɣ���������ֲ����
//    �����߲���ϵͳ���������С�

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------

//����ģ��: ASCII�ַ���������
//���ߣ�mjh
//�汾��V1.0.0
//�ļ�����: ASCII�ַ��������
//����˵��:
//�޶���ʷ:
//    2. ...
//    1. ����:2011-05-17
//       ����:mjh
//       �°汾�ţ�
//       �޸�˵��:��ʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "stdio.h"
#include "stddef.h"
#include "ascii.h"
#include "charset.h"
#include "gkernel.h"
s32 Charset_AsciiModuleInitMbToUcs4(u32* pwc, const char* s, s32 n);
s32 Charset_AsciiModuleInitMbsToUcs4s(u32* pwcs, const char* mbs, s32 n);
s32 Charset_AsciiModuleInitUcs4ToMb(char* s, s32 wc);
s32 Charset_AsciiModuleInitUcs4sToMbs(char* mbs, const u32* pwcs, s32 n);

// ע�Ͳ��� encoding.h-> tagCharset -> mb_to_ucs4
s32 Charset_AsciiModuleInitMbToUcs4(u32* pwc, const char* mbs, s32 n)
{
    u8 c = *mbs;

    n = n;

    if(c < 0x80){

        *pwc = (u32)c;
        return 1;
    }

    return -1;
}

// ע�Ͳ��� encoding.h-> tagCharset -> mbs_to_uc4s
s32 Charset_AsciiModuleInitMbsToUcs4s(u32* pwcs, const char* mbs, s32 n)
{
    s32 wcn;
    if(mbs == NULL)
        return 0;
    for(wcn = 0; wcn < n; wcn++)
    {
        if(mbs[wcn] == 0)
        {
            if(pwcs != NULL)
                pwcs[wcn] = (u32)0;
            return wcn;
        }
        else if(mbs[wcn] < 0x80)
        {
            if(pwcs != NULL)
                pwcs[wcn] = (u32)mbs[wcn];
        }else
        {
            return -1;
        }
    }
    return wcn;
}

// ע�Ͳ��� encoding.h-> tagCharset -> ucs4_to_mb
s32 Charset_AsciiModuleInitUcs4ToMb(char* mbs, s32 wc)
{

    if(wc < 0x80){

        *mbs = wc;
        return 1;
    }

    return -1;
}

// ע�Ͳ��� encoding.h-> tagCharset -> ucs4s_to_mbs
s32 Charset_AsciiModuleInitUcs4sToMbs(char* mbs, const u32* pwcs, s32 n)
{
    s32 asciin;
    if(pwcs == NULL)
        return 0;
    for(asciin = 0; asciin < n; asciin++)
    {
        if(pwcs[asciin] == 0)
        {
            if(mbs != NULL)
                mbs[asciin] = '\0';
            return asciin+1;
        }
        else if(mbs[asciin] < 0x80)
        {
            if(mbs != NULL)
                mbs[asciin] = (u8)pwcs[asciin];
        }else
        {
            asciin = -1;
            break;
        }
    }
    return asciin;
}

//----��װascii�ַ���----------------------------------------------------------
//����: ��װascii�ַ�������ϵͳʹ�������ַ�����ʱ��ʹ������ַ������ر�ע�⣬
//      gb2312�Ѿ�������Ӣ���ַ�����ʹ�����Ļ���Ӣ�Ļ�Ͻ���ģ�����Ҫ��װascii
//      �ַ��������ǣ�����GB2312���ֿ�ֻ������ȫ�ǵ�Ӣ���ַ����ʻ���Ҫ��װascii
//      ��������Դ���ߴ�(8*8��8*16)��ѡ��
//����: ������
//����: 1=�ɹ���0=ʧ��
//-----------------------------------------------------------------------------
ptu32_t ModuleInstall_CharsetAscii(ptu32_t para)
{
    static struct tagCharset encoding;

    encoding.max_len = 1;
    encoding.mb_to_ucs4 = Charset_AsciiModuleInitMbToUcs4;
    encoding.ucs4_to_mb = Charset_AsciiModuleInitUcs4ToMb;
    encoding.mbs_to_ucs4s = Charset_AsciiModuleInitMbsToUcs4s;
    encoding.ucs4s_to_mbs = Charset_AsciiModuleInitUcs4sToMbs;
    if( Charset_NlsInstallCharset(&encoding, CN_NLS_CHARSET_ASCII))
    {
        printf("ASCII encoding install sucess\r\n");
        return 1;
    }else
    {
        Djy_SaveLastError(EN_GK_CHARSET_INSTALL_ERROR);
        printf("ASCII encoding install fail\n\r");
        return 0;
    }
}

