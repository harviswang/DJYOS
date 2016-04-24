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

//����ģ��: �ַ���������
//���ߣ�mjh
//�汾��V1.0.0
//�ļ�����: �ַ�������ؿ⺯��
//����˵��: ���ļ��ڵĺ�������Ϊ�⺯��ʹ�ã�������������ҵ��ϰ�ߣ�����ģ��ǰ׺
//�޶���ʷ:
//    2. ...
//    1. ����:2011-05-17
//       ����:mjh
//       �°汾�ţ�
//       �޸�˵��:��ʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "stddef.h"
#include "charset.h"
#include "loc_string.h"

//----�����ַ�����-------------------------------------------------------------
//����: ����һ����ǰ�ַ����Ķ��ֽ��ַ��ĳ���(�ֽ���)��
//����: mbs��������ֽ��ַ��Ļ�����ָ��
//      n������ⳤ�ȣ�
//����: 0: mbs��NULLָ�����ָ��մ���
//      -1:mbsָ��Ĳ��ǺϷ����ֽ��ַ������߳���n��δ�ܼ�⵽�������ֽ��ַ�
//      ����:mbs�������ڵ�һ���������ֽ��ַ��ĳ��ȡ�
//-----------------------------------------------------------------------------
s32 mblen  (const char* mbs, u32 n)
{
    u32 wc;
    struct tagCharset* cur_enc;

    if(mbs == NULL)
        return 0;

    cur_enc = Charset_NlsGetCurCharset();
    if(cur_enc == NULL)
        return 0;

    return cur_enc->mb_to_ucs4(&wc,mbs,n);
}

//----�����ַ�����-------------------------------------------------------------
//����: ����һ��ָ���ַ������ֽ��ַ��ĳ���(�ֽ���)��
//����: mbs��������ֽ��ַ��Ļ�����ָ��
//      n������ⳤ�ȣ�
//����: 0: mbs��NULLָ�����ָ��մ���
//      -1:mbsָ��Ĳ��ǺϷ����ֽ��ַ������߳���n��δ�ܼ�⵽�������ֽ��ַ�
//      ����:mbs�������ڵ�һ���������ֽ��ַ��ĳ��ȡ�
//-----------------------------------------------------------------------------
s32 mblen_l(const char* mbs, u32 n, struct tagCharset* locale)
{
    u32 wc;

    if((mbs == NULL) || (locale == NULL))
        return 0;

    return locale->mb_to_ucs4(&wc,mbs,n);
}

//----���ֽ��ַ�תΪucs4�ַ�---------------------------------------------------
//����: ��һ����ǰ�ַ����Ķ��ֽ��ַ�ת��Ϊucs4�ַ���
//����: pwc������ת�������ָ�룬�������ɵ��÷��ṩ
//      mbs��������ֽ��ַ��Ļ�����ָ��
//      n������ⳤ�ȣ�
//����: 0: pwc,mbs��NULLָ�����mbsָ��մ���
//      -1:mbsָ��Ĳ��ǺϷ����ֽ��ַ������߳���n��δ�ܼ�⵽�������ֽ��ַ�
//      ����:mbs�������ڵ�һ���������ֽ��ַ��ĳ��ȡ�
//-----------------------------------------------------------------------------
s32 mbtowc(u32* pwc, const char* mbs, u32 n)
{
    struct tagCharset* cur_enc;

    if((pwc == NULL) || (mbs == NULL))
        return 0;

    cur_enc = Charset_NlsGetCurCharset();
    if(cur_enc == NULL)
        return 0;
    return cur_enc->mb_to_ucs4(pwc,mbs,n);
}

//-----------------------------------------------------------------------------
//����: ���c������һ���Ϸ��ĵ�ǰ�ַ����ַ�������WEOF�����򷵻�һ��ucs4�ַ���
//����: c����ת�����ַ�
//���: ��
//����: WEOF������ucs4�ַ�
//------------
//���¼�¼:
// 1.����: 2014/12/10
//   ˵��: ����
//   ����: ������
//-----------------------------------------------------------------------------
s32 btowc(char c)
{
    #warning  ��ʵ��
    return 0;
}
//----���ֽ��ַ�תΪucs4�ַ�---------------------------------------------------
//����: ��һ��ָ���ַ����Ķ��ֽ��ַ�ת��Ϊucs4�ַ���
//����: pwc������ת�������ָ�룬�������ɵ��÷��ṩ
//      mbs��������ֽ��ַ��Ļ�����ָ��
//      n������ⳤ�ȣ�
//����: 0: pwc,mbs��NULLָ�����mbsָ��մ���
//      -1:mbsָ��Ĳ��ǺϷ����ֽ��ַ������߳���n��δ�ܼ�⵽�������ֽ��ַ�
//      ����:mbs�������ڵ�һ���������ֽ��ַ��ĳ��ȡ�
//-----------------------------------------------------------------------------
s32 mbtowc_l(u32* pwc, const char* mbs, u32 n, struct tagCharset* locale)
{
    if((pwc == NULL) || (mbs == NULL) || (locale == NULL))
        return 0;

    return locale->mb_to_ucs4(pwc,mbs,n);
}

//----���ֽ��ַ���תΪucs4��---------------------------------------------------
//����: ��һ����ǰ�ַ����Ķ��ֽ��ַ���ת��Ϊucs4�ַ���
//����: pwcs������ת�������ָ�룬�������ɵ��÷��ṩ�����գ��򱾺���ת��Ϊֻ����
//          ����ת�����������ֽ���
//      mbs��������ֽ��ַ��Ļ�����ָ��
//      n������ⳤ�ȣ�
//����: 0: mbs��NULLָ��.
//      -1:������������ǰ���в���ת�����ַ�
//      ����:�õ����ַ�����=n��ʾԴ���ǲ�������������'\0'��
//-----------------------------------------------------------------------------
s32 mbstowcs(u32* pwcs, const char* mbs, u32 n)
{
    struct tagCharset* cur_enc;

    if(pwcs == NULL)
        return 0;

    cur_enc = Charset_NlsGetCurCharset();
    if(cur_enc == NULL)
        return 0;
    return cur_enc->mbs_to_ucs4s(pwcs,mbs,n);
}

//----���ֽ��ַ���תΪucs4��---------------------------------------------------
//����: ��һ��ָ���ַ����Ķ��ֽ��ַ���ת��Ϊucs4�ַ���
//����: pwcs������ת�������ָ�룬�������ɵ��÷��ṩ�����գ��򱾺���ת��Ϊֻ����
//          ����ת�����������ֽ���
//      mbs��������ֽ��ַ��Ļ�����ָ��
//      n������ⳤ�ȣ�
//����: 0: mbs��NULLָ��.
//      -1:������������ǰ���в���ת�����ַ�
//      ����:�õ����ַ�����=n��ʾԴ���ǲ�������������'\0'��
//-----------------------------------------------------------------------------
s32 mbstowcs_l(u32* pwcs, const char* mbs, u32 n,
                    struct tagCharset* locale)
{
    if((pwcs == NULL) || (locale == NULL))
        return 0;

    return locale->mbs_to_ucs4s(pwcs,mbs,n);
}

//----ucs4�ַ�תΪ���ֽ��ַ�---------------------------------------------------
//����: ��һ��ucs4�ַ�ת��Ϊ��ǰ�ַ����Ķ��ֽ��ַ���
//����: mb������ת�������ָ�룬�������ɵ��÷��ṩ
//      wc����ת�����ַ�
//����: 0: mb��NULLָ��
//      -1:wc���Ǵ�ת�����ַ�������Ч���ַ�
//      ����:ת��������ַ�����(�ֽ���)
//-----------------------------------------------------------------------------
s32 wctomb(char* mb, u32 wc)
{
    struct tagCharset* cur_enc;

    if(mb == NULL)
        return 0;

    cur_enc = Charset_NlsGetCurCharset();
    if(cur_enc == NULL)
        return 0;
    return cur_enc->ucs4_to_mb(mb, wc);
}

//----ucs4�ַ�תΪ���ֽ��ַ�---------------------------------------------------
//����: ��һ��ucs4�ַ�ת��Ϊָ���ַ����Ķ��ֽ��ַ���
//����: mb������ת�������ָ�룬�������ɵ��÷��ṩ
//      wc����ת�����ַ�
//����: 0: mb��NULLָ��
//      -1:wc���Ǵ�ת�����ַ�������Ч���ַ�
//      ����:ת��������ַ�����(�ֽ���)
//-----------------------------------------------------------------------------
s32 wctomb_l(char* mb, u32 wc,struct tagCharset* locale)
{

    if((mb == NULL) || (locale == NULL))
        return 0;

    return locale->ucs4_to_mb(mb, wc);
}

//----ucs4�ַ���תΪ���ֽ��ַ���-----------------------------------------------
//����: ��һ��ucs4�ַ���ת��Ϊ��ǰ�ַ����Ķ��ֽ��ַ�����
//����: mbs������ת�������ָ�룬�������ɵ��÷��ṩ�����գ��򱾺���ת��Ϊֻ����
//          ����ת�����������ֽ���
//      pwcs����ת�����ַ�
//      n������ⳤ�ȣ������������򳤶ȴﵽn����ת����ע��ucs4�Ľ�����������
//          4��0x00.
//����: 0: pwcs��NULLָ��
//      -1:������������ǰ���в���ת�����ַ�
//      ����:д��mbs���������ֽ���������������'\0'
//-----------------------------------------------------------------------------
s32 wcstombs(char* mbs, const u32* pwcs, u32 n)
{
    struct tagCharset* cur_enc;

    if(pwcs == NULL)
        return 0;

    cur_enc = Charset_NlsGetCurCharset();
    if(cur_enc == NULL)
        return 0;
    return cur_enc->ucs4s_to_mbs(mbs,pwcs,n);
}

//----ucs4�ַ���תΪ���ֽ��ַ���-----------------------------------------------
//����: ��һ��ucs4�ַ���ת��Ϊָ���ַ����Ķ��ֽ��ַ�����
//����: mbs������ת�������ָ�룬�������ɵ��÷��ṩ�����գ��򱾺���ת��Ϊֻ����
//          ����ת�����������ֽ���
//      pwcs����ת�����ַ�
//      n������ⳤ�ȣ������������򳤶ȴﵽn����ת����ע��ucs4�Ľ�����������
//          4��0x00.
//����: 0: pwcs��NULLָ��
//      -1:������������ǰ���в���ת�����ַ�
//      ����:д��mbs���������ֽ���������������'\0'
//-----------------------------------------------------------------------------
s32 wcstombs_l(char* mbs, const u32* pwcs, u32 n,
                struct tagCharset* locale)
{
    if((pwcs == NULL) || (locale == NULL))
        return 0;

    return locale->ucs4s_to_mbs(mbs,pwcs,n);
}

