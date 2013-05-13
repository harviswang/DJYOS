//----------------------------------------------------
//Copyright (C), 2011,  .
//��Ȩ���� (C), 2011,   djy.
//����ģ��: UTF-8�ַ���������
//���ߣ�mjh
//�汾��V1.0.0
//�ļ�����: UTF-8�ַ��������
//����˵��:
//�޶���ʷ:
//    2. ...
//    1. ����:2011-05-17
//       ����:mjh
//       �°汾�ţ�
//       �޸�˵��:��ʼ�汾
//------------------------------------------------------
/* ---------------------------------------------------------------------
	Legal UTF-8 sequences are:

	1st----	2nd----	3rd----	4th---- 5th--- 6th---	Codepoints---

	00-7F                                           0x00000000-   0x0000007F
	C2-DF   80-BF                                   0x00000080-   0x000007FF
	E0      A0-BF   80-BF                           0x00000800-   0x00000FFF
	E1-EC   80-BF   80-BF                           0x00001000-   0x0000CFFF
	ED      80-9F   80-BF                           0x0000D000-   0x0000D7FF
	EE-EF   80-BF   80-BF                           0x0000E000-   0x0000FFFF
	F0      90-BF   80-BF   80-BF                   0x00010000-   0x0003FFFF
	F1-F7   80-BF   80-BF   80-BF                   0x00040000-   0x001FFFFF
    F8      88-BF   80-BF   80-BF  80-BF            0x00200000-   0x00FFFFFF
    F9-FB   80-BF   80-BF   80-BF  80-BF            0x01000000-   0x03FFFFFF
    FC      84-BF   80-BF   80-BF  80-BF   80-BF    0x04000000-   0x3FFFFFFF
    FD      80-BF   80-BF   80-BF  80-BF   80-BF    0x40000000-   0x7FFFFFFF
-------------------------------------------------------------------------- */
/*----------------------------------------------------------------------------
Rule
 U-00000000 - U-0000007F: 0xxxxxxx 
 U-00000080 - U-000007FF: 110xxxxx 10xxxxxx 
 U-00000800 - U-0000FFFF: 1110xxxx 10xxxxxx 10xxxxxx 
 U-00010000 - U-001FFFFF: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx     
 U-00200000 - U-03FFFFFF: 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 
 U-04000000 - U-7FFFFFFF: 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 
---------------------------------------------------------------------------*/
#include "os_inc.h"

#if (cfg_charset_utf8 == 1)

#include "gk_inc.h"

#include "encoding.h"
#include "ascii.h"
#include "utf8.h"

s32 __utf8_mb_to_ucs4(u32* pwc, const u8* s, s32 n);
s32 __utf8_mbs_to_ucs4s(u32* pwcs, const char* mbs, u32 n);
s32 __utf8_ucs4_to_mb(u8* s, u32 wc);
s32 __utf8_ucs4s_to_mbs(char* mbs, const u32* pwcs, u32 n);

//����������ֽڵ�UTF-8�����ַ��ֽ���
#define utf8_len(c)\
    (((u8)c < 0x80) ? 1 : \
        (((u8)c < 0xc2) ? 0 : \
            (((u8)c < 0xe0) ? 2 : \
                (((u8)c < 0xf0) ? 3 : \
                    (((u8)c < 0xf8) ? 4 : \
                        (((u8)c < 0xfc) ? 5 : \
                            (((u8)c < 0xfe) ? 6 : 0)\
                        )\
                    )\
                )\
            )\
        )\
    )

//--------UTF-8�ֽ����кϷ����ж�----------------------------------------
//����: �жϸ�����UTF-8�ֽ������Ƿ�Ϸ�
//����: src �ֽ�����ָ��
//      len �ֽ����еĳ���
//����: 0, �Ƿ�UTF-8�ַ�
//      �Ϸ��ַ����ص�һ��UTF-8�ַ����ֽ���
//-----------------------------------------------------------------
static u8 __utf8_is_legal(const u8* src, s32 len)
{
    u8 a;
    const char *trail;
    s32 length;
    
    if(len <= 0 /*|| src == NULL*/)
        goto __illegal;

    // ����Ϸ�UTF-8�ַ����ֽ���
    length = utf8_len(*src);
        
    // �жϳ���    
    if(length > len)
        goto __illegal;
    
    // ָ�����һ���ֽڵĺ�һ���ֽ�
    trail = (src + length);

    // �жϱ����Ƿ���ȷ    
    switch(length){
    default: goto __illegal;
        case 6: if ((a = (*--trail)) < 0x80 || a > 0xBF) goto __illegal;
        case 5: if ((a = (*--trail)) < 0x80 || a > 0xBF) goto __illegal;
        case 4: if ((a = (*--trail)) < 0x80 || a > 0xBF) goto __illegal;
        case 3: if ((a = (*--trail)) < 0x80 || a > 0xBF) goto __illegal;          
        case 2: if ((a = (*--trail)) > 0xBF) goto __illegal;
        
        // �жϵڶ����ֽ�(�ض���0x80 - 0xBF֮��)
        switch(*src){
            case 0xE0: if (a < 0xA0) goto __illegal; break;
            case 0xED: if (a > 0x9F) goto __illegal; break;
            case 0xF0: if (a < 0x90) goto __illegal; break;
            case 0xF8: if (a < 0x88) goto __illegal; break;
            case 0xFC: if (a < 0x84) goto __illegal; break;
            default:   if (a < 0x80) goto __illegal;
        }
        
        // �ж϶��ֽ�UTF-8�ַ��ĵ�һ���ֽڵķ�Χ
        case 1: if (*src >= 0x80 && *src < 0xC0) goto __illegal;            
    }
    
    // �жϵ�һ���ֽ�
    if (*src > 0xFD) goto __illegal;
    
    return length;
    
__illegal:
    return 0;
}

// ע�Ͳ��� encoding.h-> char_encoding_rsc -> mb_to_ucs4
s32 __utf8_mb_to_ucs4(u32* pwc, const u8* mbs, s32 n)
{
    s32 count, len;
    u32 wc;

    if(!(count = __utf8_is_legal(mbs, n)))
        goto __illegal;

    if(count == 1)
        wc = (u32)*mbs;
    else{ // 2,3,4

        // ���ļ�ͷ��UTF-8����
        len = count;
        wc = (u32)*mbs; 
        wc &= ((1 << (8-len-1)) - 1);
        len--;
        
        while(len-- > 0){
            wc = (wc << 6) | (*++mbs & 0x3f);
        }

        // FIXME �Ƿ���Ҫ��UCS-4�Ϸ����ж�?
    }

    *pwc = wc;

    return count;
    
__illegal:
    return -1;
}

// ע�Ͳ��� encoding.h-> char_encoding_rsc -> mbs_to_uc4s
s32 __utf8_mbs_to_ucs4s(u32* pwcs, const char* mbs, u32 n)
{
    return 0;
}

// ע�Ͳ��� encoding.h-> char_encoding_rsc -> ucs4_to_mb
s32 __utf8_ucs4_to_mb(u8* mbs, u32 wc)
{
    s32 count;

    // ����wc��Ӧ��UTF-8�ַ����ֽ���
    if(wc < 0x00000080)
        count = 1;
    else if(wc < 0x00000800)
        count = 2;
    else if(wc < 0x00010000)
        count = 3;
    else if(wc < 0x00200000)
        count = 4;
    else if(wc < 0x04000000)
        count = 5;
    else if(wc < 0x80000000)
        count = 6;
    else 
        goto __illegal;

    switch(count){
        case 6: mbs[5] = 0x80 | (wc & 0x3f); wc = wc >> 6; wc |= 0x4000000;
        case 5: mbs[4] = 0x80 | (wc & 0x3f); wc = wc >> 6; wc |= 0x200000;
        case 4: mbs[3] = 0x80 | (wc & 0x3f); wc = wc >> 6; wc |= 0x10000;
        case 3: mbs[2] = 0x80 | (wc & 0x3f); wc = wc >> 6; wc |= 0x800;
        case 2: mbs[1] = 0x80 | (wc & 0x3f); wc = wc >> 6; wc |= 0xc0;
        case 1: mbs[0] = wc;
    }

    return count;

__illegal:
        return -1;
}

// ע�Ͳ��� encoding.h-> char_encoding_rsc -> ucs4s_to_mbs
s32 __utf8_ucs4s_to_mbs(char* mbs, const u32* pwcs, u32 n)
{
    return 0;
}

//----��װutf8�ַ����������---------------------------------------------------
//����: ��װutf8�ַ������������ִ��ucs4�ͱ����utf8����֮��ת����
//����: ������
//���أ�1=�ɹ���0=ʧ��
//-----------------------------------------------------------------------------
ptu32_t module_init_encode_utf8(ptu32_t para)
{
    static struct char_encoding_rsc encoding;

    encoding.max_len = 6;
    encoding.mb_to_ucs4 = __utf8_mb_to_ucs4;
    encoding.ucs4_to_mb = __utf8_ucs4_to_mb;
    encoding.mbs_to_ucs4s = __utf8_mbs_to_ucs4s;
    encoding.ucs4s_to_mbs = __utf8_ucs4s_to_mbs;
    if(nls_install_char_encoding(&encoding, cn_nls_charset_utf8))
    {
    	printf_ng("gb2312 encoding install sucess\n\r");
        return 1;
    }else
    {
        djy_error_login(enum_gk_charset_install_error,
                        "gb2312 encoding install fail");
        return 0;
    }
}

#endif      //for #if (cfg_charset_utf8 == 1)
