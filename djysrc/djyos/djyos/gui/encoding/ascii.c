//----------------------------------------------------
//Copyright (C), 2011,  .
//版权所有 (C), 2011,   djy.
//所属模块: ASCII字符编码驱动
//作者：mjh
//版本：V1.0.0
//文件描述: ASCII字符编码解析
//其他说明:
//修订历史:
//    2. ...
//    1. 日期:2011-05-17
//       作者:mjh
//       新版本号：
//       修改说明:初始版本
//------------------------------------------------------
#include "os_inc.h"

#if (cfg_charset_ascii == 1)

#include "ascii.h"
#include "encoding.h"
s32 __ascii_mb_to_ucs4(u32* pwc, const u8* s, s32 n);
s32 __ascii_mbs_to_ucs4s(u32* pwcs, const char* mbs, u32 n);
s32 __ascii_ucs4_to_mb(u8* s, u32 wc);
s32 __ascii_ucs4s_to_mbs(char* mbs, const u32* pwcs, u32 n);

// 注释参照 encoding.h-> char_encoding_rsc -> mb_to_ucs4
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

// 注释参照 encoding.h-> char_encoding_rsc -> mbs_to_uc4s
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

// 注释参照 encoding.h-> char_encoding_rsc -> ucs4_to_mb
s32 __ascii_ucs4_to_mb(u8* mbs, u32 wc)
{

    if(wc < 0x80){

        *mbs = wc;
        return 1;
    }

    return -1;
}  

// 注释参照 encoding.h-> char_encoding_rsc -> ucs4s_to_mbs
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

//----安装ascii字符集----------------------------------------------------------
//功能: 安装ascii字符集，当系统使用西方字符界面时，使用这个字符集。特别注意，
//      gb2312已经包含了英文字符集，使用中文或中英文混合界面的，不需要安装ascii
//      字符集。但是，由于GB2312的字库只包含了全角的英文字符，故还需要安装ascii
//      的字体资源，尺寸(8*8、8*16)可选。
//参数: 无意义
//返回: 1=成功，0=失败
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

