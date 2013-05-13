//----------------------------------------------------
//Copyright (C), 2011,  .
//��Ȩ���� (C), 2011,   djy.
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
#include "os_inc.h"

#if (cfg_charset_ascii == 1)

#include "ascii.h"
#include "encoding.h"
s32 __ascii_mb_to_ucs4(u32* pwc, const u8* s, s32 n);
s32 __ascii_mbs_to_ucs4s(u32* pwcs, const char* mbs, u32 n);
s32 __ascii_ucs4_to_mb(u8* s, u32 wc);
s32 __ascii_ucs4s_to_mbs(char* mbs, const u32* pwcs, u32 n);

// ע�Ͳ��� encoding.h-> char_encoding_rsc -> mb_to_ucs4
s32 __ascii_mb_to_ucs4(u32* pwc, const u8* mbs, s32 n)
{
    u8 c = *mbs;

    n = n;

    if(c < 0x80){
        
        *pwc = (u32)c;
        return 1;
    }

    return -1;    
}

// ע�Ͳ��� encoding.h-> char_encoding_rsc -> mbs_to_uc4s
s32 __ascii_mbs_to_ucs4s(u32* pwcs, const char* mbs, u32 n)
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

// ע�Ͳ��� encoding.h-> char_encoding_rsc -> ucs4_to_mb
s32 __ascii_ucs4_to_mb(u8* mbs, u32 wc)
{

    if(wc < 0x80){

        *mbs = wc;
        return 1;
    }

    return -1;
}  

// ע�Ͳ��� encoding.h-> char_encoding_rsc -> ucs4s_to_mbs
s32 __ascii_ucs4s_to_mbs(char* mbs, const u32* pwcs, u32 n)
{
    s32 asciin,ascii;
    if(pwcs == NULL)
        return 0;
    for(asciin = 0; asciin < n; asciin++)
    {
        if(pwcs[asciin] == 0)
        {
            if(mbs != NULL)
                mbs[asciin] = '\0';
            return ascii+1;
        }
        else if(mbs[asciin] < 0x80)
        {
            if(mbs != NULL)
                mbs[asciin] = (u8)pwcs[asciin];
        }else
        {
            ascii = -1;
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
ptu32_t module_init_encode_ascii(ptu32_t para)
{
    static struct char_encoding_rsc encoding;

    encoding.max_len = 1;
    encoding.mb_to_ucs4 = __ascii_mb_to_ucs4;
    encoding.ucs4_to_mb = __ascii_ucs4_to_mb;
    encoding.mbs_to_ucs4s = __ascii_mbs_to_ucs4s;
    encoding.ucs4s_to_mbs = __ascii_ucs4s_to_mbs;
    if(nls_install_char_encoding(&encoding, cn_nls_charset_ascii))
    {
    	printf_ng("ASCII encoding install sucess\r\n");
        return 1;
    }else
    {
        djy_error_login(enum_gk_charset_install_error,
                        "ASCII encoding install fail");
        return 0;
    }
}

#endif          //#if (cfg_charset_ascii == 1)

