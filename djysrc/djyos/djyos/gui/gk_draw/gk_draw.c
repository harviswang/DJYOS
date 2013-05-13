//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//��Ȩ���� (C), 2004-2009,   ������.
//����ģ��: gui
//����:  ������.
//�汾��V1.0.0
//�ļ�����: ����Ļ������صĲ���
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2010-03-30
//   ����:  ������.
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "os_inc.h"

#if (cfg_gui_kernel == 1)

#include "gk_inc.h"
#include <stdlib.h>
#include <string.h>

#define __gk_swap(a, b){a^=b; b^=a; a^=b;}
//�г�����Ԫ��դ�����沨��ʽ��ÿ��Ԫ���������е��±꼴�����Ӧ�Ĺ�դ������
char *rop3_table[256] =
{
    "0",            //cn_r3_blackness
    "DPSoon",
    "DPSona",
    "PSon",
    "SDPona",
    "DPon",
    "PDSxnon",
    "PDSaon",
    "SDPnaa",
    "PDSxon",
    "DPna",
    "PSDnaon",
    "SPna",
    "PDSnaon",
    "PDSonon",
    "Pn",
    "PDSona",      //10
    "DSon",        //cn_r3_notsrcerase
    "SDPxnon",
    "SDPaon",
    "DPSxnon",
    "DPSaon",
    "PSDPSanaxx",
    "SSPxDSxaxn",
    "SPxPDxa",
    "SDPSanaxn",
    "PDSPaox",
    "SDPSxaxn",
    "PSDPaox",
    "DSPDxaxn",
    "PDSox",
    "PDSoan",
    "DPSnaa",      //20
    "SDPxon",
    "DSna",
    "SPDnaon",
    "SPxDSxa",
    "PDSPanaxn",
    "SDPSaox",
    "SDPSxnox",
    "DPSxa",
    "PSDPSaoxxn",
    "DPSana",
    "SSPxPDxaxn",
    "SPDSoax",
    "PSDnox",
    "PSDPxox",
    "PSDnoan",
    "PSna",         //30
    "SDPnaon",
    "SDPSoox",
    "Sn",           //cn_r3_notsrccopy
    "SPDSaox",
    "SPDSxnox",
    "SDPox",
    "SDPoan",
    "PSDPoax",
    "SPDnox",
    "SPDSxox",
    "SPDnoan",
    "PSx",
    "SPDSonox",
    "SPDSnaox",
    "PSan",
    "PSDnaa",      //40
    "DPSxon",
    "SDxPDxa",
    "SPDSanaxn",
    "SDna",        //cn_r3_srcerase
    "DPSnaon",
    "DSPDaox",
    "PSDPxaxn",
    "SDPxa",
    "PDSPDaoxxn",
    "DPSDoax",
    "PDSnox",
    "SDPana",
    "SSPxDSxoxn",
    "PDSPxox",
    "PDSnoan",
    "PDna",         //50
    "DSPnaon",
    "DPSDaox",
    "SPDSxaxn",
    "DPSonon",
    "Dn",           //cn_r3_dstinvert
    "DPSox",
    "DPSoan",
    "PDSPoax",
    "DPSnox",
    "DPx",          //cn_r3_patinvert
    "DPSDonox",
    "DPSDxox",
    "DPSnoan",
    "DPSDnaox",
    "DPan",
    "PDSxa",        //60
    "DSPDSaoxxn",
    "DSPDoax",
    "SDPnox",
    "SDPSoax",
    "DSPnox",
    "DSx",          //cn_r3_srcinvert
    "SDPSonox",
    "DSPDSonoxxn",
    "PDSxxn",
    "DPSax",
    "PSDPSoaxxn",
    "SDPax",
    "PDSPDoaxxn",
    "SDPSnoax",
    "PDSxnan",
    "PDSana",       //70
    "SSDxPDxaxn",
    "SDPSxox",
    "SDPnoan",
    "DSPDxox",
    "DSPnoan",
    "SDPSnaox",
    "DSan",
    "PDSax",
    "DSPDSoaxxn",
    "DPSDnoax",
    "SDPxnan",
    "SPDSnoax",
    "DPSxnan",
    "SPxDSxo",
    "DPSaan",
    "DPSaa",        //80
    "SPxDSxon",
    "DPSxna",
    "SPDSnoaxn",
    "SDPxna",
    "PDSPnoaxn",
    "DSPDSoaxx",
    "PDSaxn",
    "DSa",          //cn_r3_srcand
    "SDPSnaoxn",
    "DSPnoa",
    "DSPDxoxn",
    "SDPnoa",
    "SDPSxoxn",
    "SSDxPDxax",
    "PDSanan",
    "PDSxna",       //90
    "SDPSnoaxn",
    "DPSDPoaxx",
    "SPDaxn",
    "PSDPSoaxx",
    "DPSaxn",
    "DPSxx",
    "PSDPSonoxx",
    "SDPSonoxn",
    "DSxn",
    "DPSnax",
    "SDPSoaxn",
    "SPDnax",
    "DSPDoaxn",
    "DSPDSaoxx",
    "PDSxan",
    "DPa",          //A0
    "PDSPnaoxn",
    "DPSnoa",
    "DPSDxoxn",
    "PDSPonoxn",
    "PDxn",
    "DSPnax",
    "PDSPoaxn",
    "DPSoa",
    "DPSoxn",
    "D",            //cn_r3_nop
    "DPSono",
    "SPDSxax",
    "DPSDaoxn",
    "DSPnao",
    "DPno",
    "PDSnoa",       //B0
    "PDSPxoxn",
    "SSPxDSxox",
    "SDPanan",
    "PSDnax",
    "DPSDoaxn",
    "DPSDPaoxx",
    "SDPxan",
    "PSDPxax",
    "DSPDaoxn",
    "DPSnao",
    "DSno",         //cn_r3_mergepaint
    "SPDSanax",
    "SDxPDxan",
    "DPSxo",
    "DPSano",
    "PSa",          //C0,cn_r3_mergecopy
    "SPDSnaoxn",
    "SPDSonoxn",
    "PSxn",
    "SPDnoa",
    "SPDSxoxn",
    "SDPnax",
    "PSDPoaxn",
    "SDPoa",
    "SPDoxn",
    "DPSDxax",
    "SPDSaoxn",
    "S",            //cn_r3_srccopy
    "SDPono",
    "SDPnao",
    "SPno",
    "PSDnoa",       //D0
    "PSDPxoxn",
    "PDSnax",
    "SPDSoaxn",
    "SSPxPDxax",
    "DPSanan",
    "PSDPSaoxx",
    "DPSxan",
    "PDSPxax",
    "SDPSaoxn",
    "DPSDanax",
    "SPxDSxan",
    "SPDnao",
    "SDno",
    "SDPxo",
    "SDPano",
    "PDSoa",        //E0
    "PDSoxn",
    "DSPDxax",
    "PSDPaoxn",
    "SDPSxax",
    "PDSPaoxn",
    "SDPSanax",
    "SPxPDxan",
    "SSPxDSxax",
    "DSPDSanaxxn",
    "DPSao",
    "DPSxno",
    "SDPao",
    "SDPxno",
    "DSo",          //cn_r3_srcpaint
    "SDPnoo",
    "P",            //F0,cn_r3_patcopy
    "PDSono",
    "PDSnao",
    "PSno",
    "PSDnao",
    "PDno",
    "PDSxo",
    "PDSano",
    "PDSao",
    "PDSxno",
    "DPo",
    "DPSnoo",      //cn_r3_patpaint
    "PSo",
    "PSDnoo",
    "DPSoo",
    "1",           //cn_r3_whiteness  
 };

#define MAX(x,y)    (x>y?x:y)
#define MIN(x,y)    (x<y?x:y)
//----��դ�������1------------------------------------------------------------
//����: ���ָ���Ĺ�դ�������Ƿ���ҪĿ��λͼ�μ����㡣
//����: rop_code����Ԫ/��Ԫ��դ������
//����: true=��Ҫ��false=����Ҫ
//ע: �ж��߼����ʽ(((fore_code & cn_rop_d)>>1) == (fore_code & ~cn_rop_d))��
//    ���壬̫����ע���޷�д�£���ο��������׵��顣
//-----------------------------------------------------------------------------
bool_t gk_rop_needsno_destination(u32 rop_code)
{
    u8 fore_code,back_code;
    fore_code = (rop_code>>16) & 0xff;      //bit16~23Ϊǰ����դ������
    if(rop_code & cn_rop_ext_en)            //����չ��rop�룬�ж���չ����
    {   //��rop4����Ҫǰ��rop��ͱ���rop�빲ͬ��������
        //��Ҫ�ж����ֹ�դ���������ֹ�դ����������Ҫ�ŷ���false
        if((rop_code & cn_rop_rop4_en) != 0)
        {
            back_code = (rop_code>>24) & 0xff;      //bit24~31Ϊ������դ������
            //����((fore_code & cn_rop_d)>>1) == (fore_code & ~cn_rop_d)
            //ǰ����դ��������ҪĿ��λͼ��������դ������֮����
            //ǰ��������դ����������ҪĿ��λͼ���룬�ŷ���false
            if((((fore_code & cn_rop_d)>>1) == (fore_code & ~cn_rop_d))
               && (((back_code & cn_rop_d)>>1) == (back_code & ~cn_rop_d)))
                return false;
            else
                return true; 
        }
        else
            return true;                    //��rop4���϶���ҪĿ�����ز�������        
    }else
    //������չ�Ĺ�դ�����룬ֻ��һ�ֹ�դ����
    {
        if(((fore_code & cn_rop_d)>>1) == (fore_code & ~cn_rop_d))
           return false;
        else
            return true;
    }
}

//----��դ�������2------------------------------------------------------------
//����: ���ָ���Ĺ�դ�������Ƿ���ҪԴλͼ�μ����㡣
//����: rop_code����Ԫ/��Ԫ��դ������
//����: true=��Ҫ��false=����Ҫ
//ע: �ж��߼����ʽ(((fore_code & cn_rop_s)>>1) == (fore_code & ~cn_rop_s))��
//    ���壬̫����ע���޷�д�£���ο��������׵��顣
//-----------------------------------------------------------------------------
bool_t gk_rop_needsno_source(u32 rop_code)
{
    u8 fore_code,back_code;
    fore_code = (rop_code>>16) & 0xff;      //bit16~23Ϊǰ����դ������
    if(rop_code & cn_rop_ext_en)            //����չ��rop�룬�ж���չ����
    {   //��rop4����Ҫǰ��rop��ͱ���rop�빲ͬ��������
        //��Ҫ�ж����ֹ�դ���������ֹ�դ����������Ҫ�ŷ���false
        if((rop_code & cn_rop_rop4_en) != 0)
        {
            back_code = (rop_code>>24) & 0xff;      //bit24~31Ϊ������դ������
            //����((fore_code & cn_rop_s)>>2) == (fore_code & ~cn_rop_s)
            //ǰ����դ��������ҪԴλͼ��������դ������֮����
            //ǰ��������դ����������ҪԴλͼ���룬�ŷ���false
            if((((fore_code & cn_rop_s)>>2) == (fore_code & ~cn_rop_s))
               && (((back_code & cn_rop_s)>>2) == (back_code & ~cn_rop_s)))
                return false;
            else
                return true;
        }            
        else
            return true;                    //��rop4���϶���ҪԴ���ز�������            
    }else
    //������չ�Ĺ�դ�����룬ֻ��һ�ֹ�դ����
    {
        if(((fore_code & cn_rop_s)>>2) == (fore_code & ~cn_rop_s))
           return false;
        else
            return true;
    }
}

//----��դ�������3------------------------------------------------------------
//����: ���ָ���Ĺ�դ�������Ƿ���Ҫpatternλͼ�μ����㡣
//����: rop_code����Ԫ/��Ԫ��դ������
//����: true=��Ҫ��false=����Ҫ
//ע: �ж��߼����ʽ(((fore_code & cn_rop_p)>>1) == (fore_code & ~cn_rop_p))��
//    ���壬̫����ע���޷�д�£���ο��������׵��顣
//-----------------------------------------------------------------------------
bool_t gk_rop_needsno_pattern(u32 rop_code)
{
    u8 fore_code,back_code;
    fore_code = (rop_code>>16) & 0xff;      //bit16~23Ϊǰ����դ������
    if(rop_code & cn_rop_ext_en)            //����չ��rop�룬�ж���չ����
    {   //��rop4����Ҫǰ��rop��ͱ���rop�빲ͬ��������
        //��Ҫ�ж����ֹ�դ���������ֹ�դ����������Ҫ�ŷ���false
        if((rop_code & cn_rop_rop4_en) != 0)
        {
            back_code = (rop_code>>24) & 0xff;      //bit24~31Ϊ������դ������
            //����((fore_code & cn_rop_p)>>4) == (fore_code & ~cn_rop_p)
            //ǰ����դ��������Ҫpatternλͼ��������դ������֮����
            //ǰ��������դ����������Ҫpatternλͼ���룬�ŷ���false
            if((((fore_code & cn_rop_p)>>4) == (fore_code & ~cn_rop_p))
               && (((back_code & cn_rop_p)>>4) == (back_code & ~cn_rop_p)))
                return false;
            else
                return true;
        }            
        else
            return false;           //��rop4���϶�����ҪԴpattern�ز�������        
    }else
    //������չ�Ĺ�դ�����룬ֻ��һ�ֹ�դ����
    {
        if(((fore_code & cn_rop_p)>>4) == (fore_code & ~cn_rop_p))
           return false;
        else
            return true;
    }
}

//----���ظ�ʽת����rgb24------------------------------------------------------
//����: ��ɫ��ʽת������һ�����ظ�ʽ��ɫת�������ɫ�����������ɫ��alpha��Ϣ��
//      ������cn_sys_pf_a8r8g8b8��ʽ��������cn_sys_pf_e8r8g8b8��ʽ��
//����: pf_type�����ظ�ʽ
//      color��Դɫ��
//����: ���ɫ��ʽ
//-----------------------------------------------------------------------------
u32 __gk_convert_color_to_24(u16 pf_type,u32 color)
{
    u32 a=0,r,g,b;
    switch(pf_type)
    {
        case cn_sys_pf_r5g6b5:
        {
            r = (color>>8) & 0xf8;
            g = (color>>3) & 0xfc;
            b = (color<<3) & 0xf8;
        }break;
        case cn_sys_pf_a1r5g5b5:
        {
            a = color>>24;
            r = (color>>8) & 0xf8;
            g = (color>>2) & 0xf8;
            b = (color<<3) & 0xf8;
        }break;
        case cn_sys_pf_gray1:
        {
            r = color<<7;
            g = r;
            b = r;
        }break;
        case cn_sys_pf_gray2:
        {
            r = color<<6;
            g = r;
            b = r;
        }break;
        case cn_sys_pf_gray4:
        {
            r = color<<4;
            g = r;
            b = r;
        }break;
        case cn_sys_pf_gray8:
        {
            r = color & 0xff;
            g = r;
            b = r;
        }break;
        case cn_sys_pf_r3g3b2:
        {
            r = color & 0xe0;
            g = (color<<3) & 0xe0;
            b = (color<<5) & 0xc0;
        }break;
        case cn_sys_pf_r4g4b4e4:
        {
            r = (color>>8) & 0xf0;
            g = (color>>4) & 0xf0;
            b = color & 0xf0;
        }break;
        case cn_sys_pf_e4r4g4b4:
        {
            r = (color>>4) & 0xf0;
            g = color & 0xf0;
            b = (color<<4) & 0xf0;
        }break;
        case cn_sys_pf_r5g5b5c1:
        {
            u32 c = (color & 1)<<2;
            r = ((color>>8) & 0xf8) + c;
            g = ((color>>3) & 0xf8) + c;
            b = ((color<<2) & 0xfc);        
        }break;
        case cn_sys_pf_e6r6g6b6:
        {
            r = (color>>10) & 0xfc;
            g = (color>>4) & 0xfc;
            b = (color<<2) & 0xfc;
        }break;
        case cn_sys_pf_e12r6g6b6:
        {
            r = (color>>10) & 0xfc;
            g = (color>>4) & 0xfc;
            b = (color<<2) & 0xfc;
        }break;
        case cn_sys_pf_r10g10b10:
        {
            r = (color>>22) & 0xff;
            g = (color>>12) & 0xff;
            b = (color>>2) & 0xff;
        }break;
        default:
        {
            return color;
        }break;
    }
    return (a<<24) | (r<<16) | (g<<8) | b;
}

//----rgb24ת��������----------------------------------------------------------
//����: ��һ��rgb24��ʽ����ɫת���ɱ��ظ�ʽ�����pf_type�к���alpha���أ���Դ
//      ��ʽ������cn_sys_pf_a8r8g8b8�����������cn_sys_pf_e8r8g8b8��
//      ���������ڴ�����ʾ��Ŀ���ʽ��֧�ֵ�ɫ���ʽ��
//����: pf_type��Ŀ���ʽ
//      color��Դɫ��
//����: pf_type��ʽ��ɫ��
//-----------------------------------------------------------------------------
u32 __gk_convert_color_to_pf(u16 pf_type,u32 color)
{
    u32 a,r,g,b;
    a = color>>24;
    r = (color>>16) &0xff;
    g = (color>>8) & 0xff;
    b = color & 0xff;
    switch(pf_type)
    {
        case cn_sys_pf_r5g6b5:
        {
            return ((r>>3)<<11) | ((g>>2)<<5) | (b>>3);
        }break;
        case cn_sys_pf_a1r5g5b5:
        {
            return ((a>>7)<<15) | ((r>>3)<<10) | ((g>>3)<<5) | (b>>3);
        }break;
        case cn_sys_pf_gray1:
        {
            //����rgbֵ����hlsģ�͵�����
            return (((MAX(MAX(r,g),b)+MIN(MIN(r,g),b))*1) + 255 )/(2*255); 
        }break;
        case cn_sys_pf_gray2:
        {
            //����rgbֵ����hlsģ�͵�����
            return (((MAX(MAX(r,g),b)+MIN(MIN(r,g),b))*3) + 255 )/(2*255); 
        }break;
        case cn_sys_pf_gray4:
        {
            //����rgbֵ����hlsģ�͵�����
            return (((MAX(MAX(r,g),b)+MIN(MIN(r,g),b))*15) + 255 )/(2*255); 
        }break;
        case cn_sys_pf_gray8:
        {
            //����rgbֵ����hlsģ�͵�����
            return (((MAX(MAX(r,g),b)+MIN(MIN(r,g),b))*255) + 255 )/(2*255); 
        }break;
        case cn_sys_pf_r3g3b2:
        {
            return ((r>>5)<<5) | ((g>>5)<<2) | (b>>6);
        }break;
        case cn_sys_pf_r4g4b4e4:
        {
            return ((r>>4)<<12) | ((g>>4)<<8) | ((b>>4)<<4);
        }break;
        case cn_sys_pf_e4r4g4b4:
        {
            return ((r>>4)<<8) | ((g>>4)<<4) | (b>>4);
        }break;
        case cn_sys_pf_r5g5b5c1:    //ʵ���϶��������λ
        {
            return ((r>>3)<<11) | ((g>>3)<<6) | ((b>>3)<<1);
        }break;
        case cn_sys_pf_e6r6g6b6:
        {
            return ((r>>2)<<12) | ((g>>2)<<6) | (b>>2);
        }break;
        case cn_sys_pf_e12r6g6b6:
        {
            return ((r>>2)<<12) | ((g>>2)<<6) | (b>>2);
        }break;
        case cn_sys_pf_r10g10b10:
        {
            return (r<<20) | (g<<10) | b;
        }break;
        default:
        {
            return color;
        }
        break;
    }
    return color;
}
//----������ɫ��ʽ����໥ת��-------------------------------------------------
//����: ������ɫ��ʽ����໥ת����
//����: dpf_type��Ŀ���ʽ
//      spf_type��Դ��ʽ
//      color��Դɫ��
//����: dpf_type��ʽ��ɫ��
//-----------------------------------------------------------------------------
u32 __gk_convert_part_pf_to_pf(u16 dpf_type,u16 spf_type,u32 color)
{
    u32 result;
    //Դ��ʽ��Ŀ���ʽ��ͬ��ֱ�ӷ���Դɫ��
    if(spf_type == dpf_type)
        return color;
    //��Դɫ��ת�������ɫ
    result = __gk_convert_color_to_24(spf_type,color);
    //�����ɫת����Ŀ���ʽ����ɫ
    result = __gk_convert_color_to_pf(dpf_type,result);
    return result;
}
//----rop2���-----------------------------------------------------------------
//����: ����rop2ɫ�ʻ�ϡ�
//����: dest��������ɫ
//      pen��������ɫ
//      rop2_code����Ԫ��դ������
//����: ��Ϻ��ɫ��
//-----------------------------------------------------------------------------
u32 __gk_blend_rop2(u32 dest,u32 pen,u32 rop2_code)
{
    switch(rop2_code)
    {
        case cn_r2_black:         // ��Ϻ����ɫΪ��ɫ(black)  
        {
            return cn_color_black;
        }break;
        case cn_r2_notmergepen:   //��Ϻ����ɫΪ~(dest | pen) 
        {
            return ~(dest | pen);
        }break;
        case cn_r2_masknotpen:    //��Ϻ����ɫΪ~pen & dest  
        {
            return ~pen & dest;
        }break;
        case cn_r2_notcopypen:    //��Ϻ����ɫΪ~pen           
        {
            return ~pen;
        }break;
        case cn_r2_maskpennot:    //��Ϻ����ɫΪ~dest & pen  
        {
            return ~dest & pen;
        }break;
        case cn_r2_not:           //��Ϻ����ɫΪ~dest          
        {
            return ~dest;
        }break;
        case cn_r2_xorpen:        //��Ϻ����ɫΪdest ^ pen   
        {
            return dest ^ pen;
        }break;
        case cn_r2_notmaskpen:    //��Ϻ����ɫΪ~(dest & pen)
        {
            return ~(dest & pen);
        }break;
        case cn_r2_maskpen:       //��Ϻ����ɫΪdest & pen   
        {
            return dest & pen;
        }break;
        case cn_r2_notxorpen:     //��Ϻ����ɫΪ~(dest ^ pen)
        {
            return ~(dest ^ pen);
        }break;
        case cn_r2_nop:           //��Ϻ����ɫΪdest��ԭɫ��   
        {
            return dest;
        }break;
        case cn_r2_mergenotpen:   //��Ϻ����ɫΪ~pen | dest   
        {
            return ~pen | dest;
        }break;
        case cn_r2_copypen:       //��Ϻ����ɫΪpen������ɫ��  
        {
            return pen;
        }break;
        case cn_r2_mergepennot:   //��Ϻ����ɫΪ~dest | pen   
        {
            return ~dest | pen;
        }break;
        case cn_r2_mergepen:      //��Ϻ����ɫΪdest | pen    
        {
            return dest | pen;
        }break;
        case cn_r2_white:         //��Ϻ����ɫΪ��ɫ(white)  
        {
            return cn_color_white ;
        }break;
        default:
        {
            return pen;
            break;
        }
    }
    return 0;
}
//----rop3���-----------------------------------------------------------------
//����: ����rop3ɫ�ʻ�ϡ�
//����: dest��Ŀ��ɫ��
//      pat��ͼ��ɫ��
//      src��Դɫ��
//      rop3_code����Ԫ��դ������
//����: ��Ϻ��ɫ��
//ע��: ��ϸ�Ľ��ͣ�djyosͼ�α��������ϸ����
//-----------------------------------------------------------------------------
u32 __gk_blend_rop3(u32 dest,u32 src,u32 pat,u8 rop3_code)
{
    u32 j,m,k;
    u32 s[12];
    char *rpm;
    //����rop3_table����rop3_code��Ӧ����Ӧ���沨��ʽ��������Ϻ��ɫ�� 
    rpm = rop3_table[rop3_code];
    
    if(rpm[0] == '0')           //Ŀ��λͼ��ɫΪ��ɫ      
        return 0;
    else if(rpm[0] == '1')      //Ŀ��λͼ��ɫΪ��ɫ 
        return 0xffffff;
    else        //�����沨��ʽ����Ŀ��λͼ��ɫ��ͼ��λͼ��ɫ��Դλͼ��ɫ���
    {
        m = 0;
        k = 0;
        //����rop3_table���õ���ʾ�沨��ʽ�������е�Ԫ����
        //�����12���ַ�(�������������)
        //������ĵ�һ����Աɨ�裬���ݺ������������ν�������õ���Ϻ����ɫ
        //��������Ϊ��������������ǰ��һ���ַ�(���д�����ǰ������)Ϊ������
        //��������Ϊ˫������������ǰ�������ַ�(���д�����ǰ������)Ϊ������
        for(j = 0;j<12;j++)
        {   
            if(rpm[j] == '\0')      //�沨��ʽ�������������ռ�����
                return s[j-1]; 
            else if(rpm[j] == 'S')  //������ΪS  
            {
                //S֮ǰ���������Ҫ���м����������
                //���е��沨��ʽ�д�����������м�������
                //�м�����������x
                if(rpm[j-1] == 'x') 
                {
                    m -= 2;
                    k++;
                    s[j] = src;
                }
                else
                    s[j] = src;
            }
            else if(rpm[j] == 'D')  //������ΪD 
            {
                if(rpm[j-1] == 'x') //D֮ǰ���������Ҫ���м���������� 
                {
                    m -= 2;
                    k++;
                    s[j] = dest;
                }
                else
                    s[j] = dest;
            }
            else if(rpm[j] == 'P')  //������ΪP
            {
                if(rpm[j-1] == 'x') //P֮ǰ���������Ҫ���м����������
                {
                    m -= 2;
                    k++;
                    s[j] = pat;
                }
                else
                    s[j] = pat;
            }
            else if(rpm[j] == 'n')  //�����Ϊn���������ǰ��һ���ַ�������ȡ��
            {
                s[j] = ~s[j-1];
                m++;
                s[j-1] = s[j-1-m];
            }
            else if(rpm[j] == 'x')//�����Ϊx���������ǰ�������ַ����������
            {
                s[j] = s[j-1]^s[j-2];
                m+=2;
                //��"������+�����+������"���Ƶ�����������д���
                if(k == 1)
                {
                    m = 2;
                    k++;
                }
                else
                {
                    if(k == 2)
                        m = 6;
                } 
                s[j-1] = s[j-1-m];
            }
            else if(rpm[j] == 'o')//�����Ϊo���������ǰ�������ַ������ݻ�����
            {
                s[j] = s[j-1]|s[j-2];
                m+=2;
                //��"������+�����+������"���Ƶ�����������д���
                if(k == 1)
                {
                    m = 2;
                    k++;
                }
                else
                {
                    if(k == 2)
                        m = 6;
                } 
                s[j-1] = s[j-1-m];
            }
            else 
            {
                if(rpm[j] == 'a')//�����Ϊa���������ǰ�������ַ�������������
               {    
                    s[j] = s[j-1]&s[j-2];
                    m+=2;
                    //��"������+�����+������"���Ƶ�����������д���
                    if(k == 1)
                    {
                        m = 2;
                        k++;
                    }
                    else 
                    {
                        if(k == 2)
                            m = 6;
                    }
                    s[j-1] = s[j-1-m];
                }
            }
        }
        return 0;
    }
}
//----�ж�Ŀ������ڵ�����-----------------------------------------------------
//����: �ж�Ŀ������ڵ����򣬷ֳɾ���������ҡ��ϡ����Ĳ��֡�
//����: x��y��Ŀ���,��Դ��ڵ�����
//      limit��������,��Դ��ڵ�����
//����: ��־��c��c = 0��ʾĿ����ھ������ڣ������ھ�������
//-----------------------------------------------------------------------------
u32 __gk_point_sect(s32 x,s32 y,struct st_rect *limit)  //ȷ��
{
    s32 c;
    c = 0;
    
    if(x < limit->left)                 //�ж�Ŀ����Ƿ��ھ�������
        c |= 0x01;
    else
    {
        if(x > limit->right)            //�ж�Ŀ����Ƿ��ھ������ҷ�
        c |= 0x02;
    }
    if(y < limit->top)                  //�ж�Ŀ����Ƿ��ھ������Ϸ�
        c |= 0x04;
    else
    {
        if(y > limit->bottom)           //�ж�Ŀ����Ƿ��ھ������·�
        c |= 0x08;
    }
    return c;
}
//----��ȡֱ�߶��������Ľ���-------------------------------------------------
//����: ��ȡֱ�߶��������Ľ��㡣
//����: limit��������,��Դ��ڵ�����
//      x1��y1��x2��y2�����յ�,��Դ��ڵ�����
//����: ��־��flag��flag = 0��ʾб�߲���limit�ڣ�flag = 1��ʾб���е���limit��
//˵��: �˴���ֱ�߶������б�߶ε�
//-----------------------------------------------------------------------------
u32 __gk_oline_sect_inter(struct st_rect *limit,s32 *x1,s32 *y1,s32 *x2,s32 *y2)//ȷ��
{
    s32 c1,c2,i=0;
    s32 flag = 1;

    c1 = __gk_point_sect(*x1,*y1,limit);
    c2 = __gk_point_sect(*x2,*y2,limit);

    if((c1 == 0)&&(c2 == 0))        //б����limit��
        return flag;
    else if((c1&c2) != 0)           //б����limit�⣬�����߶���limit��һ���߽���
    {
        flag = 0;
        return flag;
    }
    else                                                    
    {
        if(c1 == 0)                 //��(x1,y1)��limit�ڣ���(x2,y2)�϶�����
        {
            do
            {
                if((c2&0x01) != 0)
                {
                    *y2 = *y1+(*y1-*y1)*(limit->left-*x1)/(*x1-*x1);                    
                    *x2 = limit->left;
                    c2 = __gk_point_sect(*x2,*y2,limit);                    
                }
                else if((c2&0x02) != 0)
                {
                    *y2 = *y1+(*y2-*y1)*(limit->right-*x1)/(*x2-*x1);                    
                    *x2 = limit->right;
                    c2 = __gk_point_sect(*x2,*y2,limit);                    
                }
                else if((c2&0x04) != 0)
                {
                    *x2 = *x1+(*x2-*x1)*(limit->top-*y1)/(*y2-*y1);                    
                    *y2 = limit->top;
                    c2 = __gk_point_sect(*x2,*y2,limit);
                }
                else 
                {
                    *x2 = *x1+(*x2-*x1)*(limit->bottom-*y1)/(*y2-*y1);
                    *y2 = limit->bottom;
                    c2 = __gk_point_sect(*x2,*y2,limit);
                }
            }while(c2 != 0);
            return flag;
        }
        else                    //��(x1,y1)����limit�ڣ���(x2,y2)������limit��
        {                       //Ҳ������limit��
            do{
                if((c1&0x01) != 0)
                {
                    *y1 = *y1+(*y2-*y1)*(limit->left-*x1)/(*x2-*x1);
                    *x1 = limit->left;
                    c1 = __gk_point_sect(*x1,*y1,limit);
                    i++;
                }
                else if((c1&0x02) != 0)
                {
                    *y1 = *y1+(*y2-*y1)*(limit->right-*x1)/(*x2-*x1);
                    *x1 = limit->right;
                    c1 = __gk_point_sect(*x1,*y1,limit);
                    i++;
                }
                else if((c1&0x04) != 0)
                {
                    *x1 = *x1+(*x2-*x1)*(limit->top-*y1)/(*y2-*y1);
                    *y1 = limit->top;
                    c1 = __gk_point_sect(*x1,*y1,limit);
                    i++;
                }
                else 
                {
                    if((c1&0x08) != 0)
                    {
                        *x1 = *x1+(*x2-*x1)*(limit->bottom-*y1)/(*y2-*y1);
                        *y1 = limit->bottom;
                        c1 = __gk_point_sect(*x1,*y1,limit);
                        i++;
                    }
                }
                //ֱ�ߺ�limit�н���ʱ��һ���˵���������ν���
                if(i>2)                     
                {                           
                    flag = 0;               
                    return flag;
                }
            }while(c1 != 0); 
            if(c2 == 0)
                return flag;
            else
            {
                do
                {
                    if((c2&0x01) != 0)
                    {
                        *y2 = *y1+(*y2-*y1)*(limit->left-*x1)/(*x2-*x1);
                        *x2 = limit->left;
                        c2 = __gk_point_sect(*x2,*y2,limit);                    
                    }
                    else if((c2&0x02) != 0)
                    {
                        *y2 = *y1+(*y2-*y1)*(limit->right-*x1)/(*x2-*x1);
                        *x2 = limit->right;
                        c2 = __gk_point_sect(*x2,*y2,limit);                    
                    }
                    else if((c2&0x04) != 0)
                    {
                        *x2 = *x1+(*x2-*x1)*(limit->top-*y1)/(*y2-*y1);
                        *y2 = limit->top;
                        c2 = __gk_point_sect(*x2,*y2,limit);
                    }
                    else 
                    {
                        *x2 = *x1+(*x2-*x1)*(limit->bottom-*y1)/(*y2-*y1);
                        *y2 = limit->bottom;
                        c2 = __gk_point_sect(*x2,*y2,limit);
                    }
                }while(c2 != 0);
                return flag;
            }
        }
    }
}

//----����б�߲��������-------------------------------------------------------
//����: ����б�߲���־λ��
//����: gkwin��Ŀ�괰��
//      x1��y1��x2��y2�����յ�
//      color����������ɫ
//      rop2_code����Ԫ��դ������
//����: ��
//˵��: ���õ���Bresenham�㷨
//-----------------------------------------------------------------------------
void __gk_draw_oline(struct gkwin_rsc *gkwin,s32 x1,s32 y1,
                                        s32 x2,s32 y2,u32 color,u32 rop2_code)
{    
    u32 pf_color;
    s32 x,y,dx,dy,s1,s2,p,interchange,i;

    struct st_rect_bitmap *bitmap;
    bitmap = &gkwin->wm_bitmap;
     
    x = x1;
    y = y1;
    dx = abs(x2-x1);
    dy = abs(y2-y1);
    //��־����б�߷���
    if(x2 > x1)   
        s1 = 1;
    else
        s1 = -1;
    if(y2 > y1)
        s2 = 1;
    else
        s2 = -1;
    //��־���仯�ı�����interchangeΪ1��y�仯���Ϊ0��x�仯���
    if(dy > dx)
    {
        __gk_swap(dx, dy);
        interchange = 1;
    }
    else
        interchange = 0;
    p = (dy<<1)-dx;            //���ó�ʼ����б�ֵ
    //��ɫ��ʽΪ�Կ��Զ����ʽ
    if(bitmap->pf_type & cn_custom_pf)
    {
        for(i=1;i<=dx;i++)//hjj
        {
            gkwin->disp->draw.set_pixel_bm(bitmap,x,y,color,rop2_code);
            //ȡ����ѧ��Ͻ�������
            if(p >= 0)          
            {
                if(interchange == 0)
                    y = y+s2;
                else
                    x = x+s1;
                p = p-(dx<<1);
            }
            if(interchange == 0)
                x = x+s1; 
            else
                y = y+s2;
            p = p+(dy<<1);
        }
    } 
    else
    {//��ɫ��ʽΪ������ʽ
        pf_color = __gk_convert_color_to_pf(bitmap->pf_type,color);
        for(i=1;i<=dx;i++)
        {
            __gk_set_pixel_rop2_bm(bitmap,x,y,pf_color,rop2_code);
            //ȡ����ѧ��Ͻ�������
            if(p >= 0)          
            {
                if(interchange == 0)
                    y = y+s2;
                else
                    x = x+s1;
                p = p-(dx<<1);
            }
            if(interchange == 0)
                x = x+s1; 
            else
                y = y+s2;
            p = p+(dy<<1);
        }
    }
}

//----��ȡλͼ�е�����---------------------------------------------------------
//����: ��һ��bitmap��ȡ���ء�
//����: bitmap��Ŀ��λͼ
//      x��y��Ŀ�����ص�����
//����: ������ɫֵ
//-----------------------------------------------------------------------------
u32 __gk_get_pixel_bm(struct st_rect_bitmap *bitmap, s32 x,s32 y)
{
    u8 color_bits,bit_offset;
    u32 color=0,byte_offset;
    color_bits = bitmap->pf_type>>8;
    //����λͼ��ɫ��ʽ��ȡ����
    switch(color_bits)
    {
        case 1:
        {
            //���������Ӧ���ֽ�ƫ����
            byte_offset = y*bitmap->linebytes + x/8;
            //������������Ӧ�ֽ��е�λƫ��
            bit_offset = 7-x%8;     //���λ��ʾ����ߣ����λ���ұ�
            //ȡ������ɫ
            color = (u32)(bitmap->bm_bits[byte_offset]>>bit_offset)&0x1;
        }break;
        case 2:
        {
            byte_offset = y*bitmap->linebytes + x/4;
            bit_offset = (3-x%4)<<1;     //���λ��ʾ����ߣ����λ���ұ�
            color = (u32)(bitmap->bm_bits[byte_offset]>>bit_offset)&0x3;
        }break;
        case 4:
        {
            byte_offset = y*bitmap->linebytes + x/2;
            bit_offset = (1-x%2)<<2;     //���λ��ʾ����ߣ����λ���ұ�
            color = (u32)(bitmap->bm_bits[byte_offset]>>bit_offset)&0xf;
        }break;
        case 8:
        {
            byte_offset = y*bitmap->linebytes + x;
            color = (u32)bitmap->bm_bits[byte_offset];
        }break;
        case 12:
        {
            byte_offset = y*bitmap->linebytes + x*3/2;
            bit_offset = (x*12)%8;
            //ע:��������⣬���ɰ�Ŀ���ַǿ��ת����16λָ�롣
            if(bit_offset == 0)
            {
                color = (u32)bitmap->bm_bits[byte_offset]+
                        (u32)((bitmap->bm_bits[byte_offset+1]&0xf)<<8);
            }
            else
            {
                color = (u32)((bitmap->bm_bits[byte_offset]&0xf0)>>4)+
                        (u32)(bitmap->bm_bits[byte_offset+1]<<4);
            }
        }break;
        case 16:
        {
            byte_offset = y*bitmap->linebytes + x*2;
            color = (u32)(bitmap->bm_bits[byte_offset]
                    |(bitmap->bm_bits[byte_offset+1]<<8));
        }break;
        case 24:
        {
            byte_offset = y*bitmap->linebytes + x*3;
            color = (u32)bitmap->bm_bits[byte_offset];
            color += (u32)bitmap->bm_bits[byte_offset+1]<<8;
            color += (u32)bitmap->bm_bits[byte_offset+2]<<16;
        }break;
        case 32:
        {
            byte_offset = y*bitmap->linebytes + x*4;
            color = (u32)(*(u32*)(&bitmap->bm_bits[byte_offset]));
        }break;
        default:
        break;
    }
    return color;
}
//----��ȡĿ����ɫ��argb����---------------------------------------------------
//����: ��ȡĿ����ɫ��argb������
//����: pf_type��Ŀ����ɫ��ɫ��ʽ
//      color��Ŀ����ɫ
//      a��r��g��b��Ŀ����ɫ��a��r��g��b����
//����: ��
//-----------------------------------------------------------------------------
void __gk_get_rgb_bm(u16 pf_type,u32 color,u32 *a,u32 *r,
                                            u32 *g,u32 *b)
{
    //����λͼ��ɫ��ʽ����Ŀ����ɫ�е�rgb����
    switch(pf_type)
    {
        case cn_sys_pf_r5g6b5:
        {
            *r = (color>>11) & 0x1f;
            *g = (color>>5) & 0x3f;
            *b = color & 0x1f;
        }break;
        case cn_sys_pf_a1r5g5b5:
        {
            *a = (color>>24) & 0x01;
            *r = (color>>10) & 0x1f;
            *g = (color>>5) & 0x1f;
            *b = color & 0x1f;
        }break;
        case cn_sys_pf_gray1:
        {
            *r = color & 0x01;
            *g = *r;
            *b = *r;
        }break;
        case cn_sys_pf_gray2:
        {
            *r = color & 0x03;
            *g = *r;
            *b = *r;
        }break;
        case cn_sys_pf_gray4:
        {
            *r = color & 0x0f;
            *g = *r;
            *b = *r;
        }break;
        case cn_sys_pf_gray8:
        {
            *r = color & 0xff;
            *g = *r;
            *b = *r;
        }break;
        case cn_sys_pf_r3g3b2:
        {
            *r = (color>>5) & 0x07;
            *g = (color<<2) & 0x07;
            *b = color & 0x03;
        }break;
        case cn_sys_pf_r4g4b4e4:
        {
            *r = (color>>12) & 0x0f;
            *g = (color>>8) & 0x0f;
            *b = (color>>4) & 0x0f;
        }break;
        case cn_sys_pf_e4r4g4b4:
        {       
            *r = (color>>8) & 0x0f;
            *g = (color>>4) & 0x0f;
            *b = color & 0x0f;
        }break;
        case cn_sys_pf_r5g5b5c1:            
        {
            u32 c;
            c = (color & 1);
            *r = (((color>>10) & 0x3e)+c);
            *g = (((color>>5) & 0x3e)+c);
            *b = color & 0x3f;
        }break;
        case cn_sys_pf_e6r6g6b6:
        {
            *r = (color>>12) & 0x3f;
            *g = (color>>6) & 0x3f;
            *b = color & 0x3f;
        }break;
        case cn_sys_pf_e12r6g6b6:
        {
            *r = (color>>12) & 0x3f;
            *g = (color>>6) & 0x3f;
            *b = color & 0x3f;
        }break;   
        case cn_sys_pf_r10g10b10:
        {             
            *r = (color>>20) & 0x3ff;
            *g = (color>>10) & 0x3ff;
            *b = color & 0x3ff;
        }break;
        case cn_sys_pf_e8r8g8b8:
        {
            *r = (color>>16) & 0xff;
            *g = (color>>8) & 0xff;
            *b = color & 0xff;
        }break;   
        case cn_sys_pf_a8r8g8b8:
        {
            *a = (color>>24) & 0xff;
            *r = (color>>16) & 0xff;
            *g = (color>>8) & 0xff;
            *b = color & 0xff;
        }break;   
        default:
        {
            *a = 0;
            *r = 0;
            *g = 0;
            *b = 0;
        }
        break;
    }
}
//----argb��������ȡ���-------------------------------------------------------
//����: ������λͼ��ָ�������������alpha���ֵ
//����: dst_bitmap��Ŀ��λͼ��ʹ�ñ���alpha
//      src_bitmap��Դλͼ��ʹ��ǰ��alpha
//      x_dst��y_dst��Ŀ��λͼ�е�Ŀ������
//      x_src��y_src��Դλͼ�е�Ŀ������
//      fore��ǰ��alpha
//      back������alpha
//����: ��Ϻ����ɫ
//˵��: ����alpha��˫alpha����Ҫ����color,RGB�ֱ����
//-----------------------------------------------------------------------------
u32 __gk_rop_alpha(struct st_rect_bitmap *dst_bitmap,
                    struct st_rect_bitmap *src_bitmap,s32 x_dst,s32 y_dst,
                    s32 x_src,s32 y_src,u32 fore,u32 back)
{
    u16 pf_type;
    u32 a,r,g,b;
    u32 a_src=0,r_src,g_src,b_src;
    u32 a_dst=0,r_dst,g_dst,b_dst;
    u32 color,color_src,color_dst;
    //��ȡĿ��λͼ�е���ɫ
    color_dst = __gk_get_pixel_bm(dst_bitmap,x_dst,y_dst);
    //��ȡԴλͼ�е���ɫ
    color_src = __gk_get_pixel_bm(src_bitmap,x_src,y_src);
    color_src = __gk_convert_part_pf_to_pf(dst_bitmap->pf_type, 
                                        	src_bitmap->pf_type,color_src);
    pf_type = dst_bitmap->pf_type;
    //��ȡĿ��λͼ��ɫ��rgb����
    __gk_get_rgb_bm(pf_type,color_dst,&a_dst,&r_dst,&g_dst,&b_dst);
    //��ȡԴλͼ��ɫ��rgb����
    __gk_get_rgb_bm(pf_type,color_src,&a_src,&r_src,&g_src,&b_src);
    //����Ŀ��λͼ��ɫ��ʽ��ȡ��Ϻ����ɫ            
    switch(pf_type)
    {
        case cn_sys_pf_r5g6b5:
        {
            r = r_src*fore/255+r_dst*back/255;   
            g = g_src*fore/255+g_dst*back/255;   
            b = b_src*fore/255+b_dst*back/255; 
            color = (r<<11) | (g<<5) | b;
            return color;
        }break;
        case cn_sys_pf_a1r5g5b5:
        {
            a = a_src*fore/255+a_dst*back/255;   
            r = r_src*fore/255+r_dst*back/255;   
            g = g_src*fore/255+g_dst*back/255;   
            b = b_src*fore/255+b_dst*back/255; 
            color = (a<<15) | (r<<10) | (g<<5) | b;
            return color;
        }break;
        case cn_sys_pf_gray1:
        {
            r = r_src*fore/255+r_dst*back/255;   
            g = g_src*fore/255+g_dst*back/255;   
            b = b_src*fore/255+b_dst*back/255; 
            color = (((MAX(MAX(r,g),b)+MIN(MIN(r,g),b))*1) + 255 )/(2*255);
            return color;
        }break;
        case cn_sys_pf_gray2:
        {
            r = r_src*fore/255+r_dst*back/255;   
            g = g_src*fore/255+g_dst*back/255;   
            b = b_src*fore/255+b_dst*back/255; 
            color = (((MAX(MAX(r,g),b)+MIN(MIN(r,g),b))*3) + 255 )/(2*255);
            return color;
        }break;
        case cn_sys_pf_gray4:
        {
            r = r_src*fore/255+r_dst*back/255;   
            g = g_src*fore/255+g_dst*back/255;   
            b = b_src*fore/255+b_dst*back/255; 
            color = (((MAX(MAX(r,g),b)+MIN(MIN(r,g),b))*15) + 255 )/(2*255);
            return color;
        }break;
        case cn_sys_pf_gray8:
        {
            r = r_src*fore/255+r_dst*back/255;   
            g = g_src*fore/255+g_dst*back/255;   
            b = b_src*fore/255+b_dst*back/255; 
            color = (((MAX(MAX(r,g),b)+MIN(MIN(r,g),b))*255) + 255 )/(2*255);
            return color;
        }break;
        case cn_sys_pf_r3g3b2:
        {
            r = r_src*fore/255+r_dst*back/255;   
            g = g_src*fore/255+g_dst*back/255;   
            b = b_src*fore/255+b_dst*back/255; 
            color = (r<<5) | (g<<2) | b;
            return color;
        }break;
        case cn_sys_pf_r4g4b4e4:
        {
            r = r_src*fore/255+r_dst*back/255;   
            g = g_src*fore/255+g_dst*back/255;   
            b = b_src*fore/255+b_dst*back/255; 
            color = (r<<16) | (g<<8) | (b<<4);
            return color;
        }break;
        case cn_sys_pf_e4r4g4b4:
        {
            r = r_src*fore/255+r_dst*back/255;   
            g = g_src*fore/255+g_dst*back/255;   
            b = b_src*fore/255+b_dst*back/255; 
            color = (r<<8) | (g<<4) | b;
            return color;
        }break;
        case cn_sys_pf_r5g5b5c1:
        {
            u32 c;
            r = r_src*fore/255+r_dst*back/255;   
            g = g_src*fore/255+g_dst*back/255;   
            b = b_src*fore/255+b_dst*back/255; 
            c = r & 1;
            r = r>>1;
            g = g>>1;
            b = b>>1;
            color = (r<<11) | (g<<6) | (b<<1) | c;
            return color;
        }break;
        case cn_sys_pf_e6r6g6b6:
        {
            r = r_src*fore/255+r_dst*back/255;   
            g = g_src*fore/255+g_dst*back/255;   
            b = b_src*fore/255+b_dst*back/255; 
            color = (r<<12) | (g<<6) | b;
            return color;
        }break;
        case cn_sys_pf_e12r6g6b6:
        {
            r = r_src*fore/255+r_dst*back/255;   
            g = g_src*fore/255+g_dst*back/255;   
            b = b_src*fore/255+b_dst*back/255; 
            color = (r<<12) | (g<<6) | b;
            return color;
        }break;
        case cn_sys_pf_r10g10b10:
        {
            r = r_src*fore/255+r_dst*back/255;   
            g = g_src*fore/255+g_dst*back/255;   
            b = b_src*fore/255+b_dst*back/255; 
            color = (r<<20) | (g<<10) | b;
            return color;
        }break;
        case cn_sys_pf_e8r8g8b8:
        {   
            r = r_src*fore/255+r_dst*back/255;   
            g = g_src*fore/255+g_dst*back/255;   
            b = b_src*fore/255+b_dst*back/255; 
            color = (r<<16) | (g<<8) | b;
            return color;
        }break;   
        case cn_sys_pf_a8r8g8b8:
        {
            a = a_src*fore/255+a_dst*back/255;   
            r = r_src*fore/255+r_dst*back/255;   
            g = g_src*fore/255+g_dst*back/255;   
            b = b_src*fore/255+b_dst*back/255; 
            color = (a<<24) | (r<<16) | (g<<8) | b;
            return color;
        }break;   
        default:
        {
            return 0;
        }
        break;
    }

    return 0;
}
//----���ڴ滭��(��չrop_code)-------------------------------------------------
//����: ��һ��bitmap�ϻ��㡣
//����: dst_bitmap��Ŀ��λͼ
//      src_bitmap��Դλͼ
//      pat��patternָ��
//      x_dst��y_dst��Ŀ��λͼ�е�Ŀ������
//      x_src��y_src��Դλͼ�е�Ŀ������
//      rop_code����դ�����룬��չ�Ĺ�դ������
//      bitmsk������λͼ
//      transparentcolor��͸��ɫ
//����: ��
//-----------------------------------------------------------------------------
void __gk_set_pixel_rop_ext_bm(struct st_rect_bitmap *dst_bitmap,
                                struct st_rect_bitmap *src_bitmap,u8 *pat,
                                s32 x_dst,s32 y_dst,s32 x_src,s32 y_src,
                                u32 rop_code,struct st_rect_bitmap *bitmsk,
                                u32 transparentcolor)
{
    u8 bit_offset;
    u32 fore,back;
    u32 color;
    u32 byte_offset_msk;

    if((rop_code & cn_rop_transparentcolor) != 0)   //transparentcolor��ɫ͸��
    {
        color = __gk_get_pixel_bm(src_bitmap,x_src,y_src);
    	color = __gk_convert_part_pf_to_pf(dst_bitmap->pf_type,
                                        src_bitmap->pf_type,color);
        if(color != transparentcolor)//color�� transparentcolor��ͬ�����ǰ��ɫ
            __gk_set_pixel_rop2_bm(dst_bitmap,x_dst,y_dst,
                                	color,cn_r2_copypen);                                                               
        else
            return;     //color��transparentcolor��ͬ��͸�����������ɫ
    }
    else if((rop_code & cn_rop_rop4_en) != 0)//rop4���ܣ�bit24~31��ʾ����������
    {
        byte_offset_msk = y_src*bitmsk->linebytes+x_src/8;
        bit_offset = x_src%8;
        if(((bitmsk->bm_bits[byte_offset_msk]>>(7-bit_offset))&0x1) == 0)                    
        {//����λͼ��������Ϊ0ʱ��ʹ�ñ�����դ������bit24~31��ʾ������դ������
        	__gk_set_pixel_rop3_bm(dst_bitmap,src_bitmap,pat,
                    x_dst,y_dst,x_src,y_src,(rop_code>>24)<<16);
        }
        else
        {//����λͼ��������Ϊ1ʱ��ʹ��ǰ����դ������bit16~23��ʾǰ����դ������
        	__gk_set_pixel_rop3_bm(dst_bitmap,src_bitmap,pat,
                    x_dst,y_dst,x_src,y_src,rop_code);
        }                     
    } 
    //alpha���,һ������ֻ������һ��alpha
    else if( (((rop_code & cn_rop_alpha_en) != 0)
                        && ((rop_code & cn_rop_double_alpha) ==0)
                        && ((rop_code & cn_rop_pbp_alpha) == 0)  ) 
                || (((rop_code & cn_rop_alpha_en) == 0)
                        && ((rop_code & cn_rop_double_alpha) !=0) 
                        && ((rop_code & cn_rop_pbp_alpha) == 0)  )
                || (((rop_code & cn_rop_alpha_en) == 0) 
                        && ((rop_code & cn_rop_double_alpha) ==0)
        && ((rop_code & cn_rop_pbp_alpha) != 0))   )
    {
        //����alpha��ϣ�bit0~7=alpha
        if((rop_code & cn_rop_alpha_en) != 0)
        {
            fore = rop_code & 0xff; //bit0~7Ϊǰ��alphaֵ
            back = 255-fore;        //ǰ��alpha+����alpha=255
            //����alpha��Ϻ����ɫ
            color = __gk_rop_alpha(dst_bitmap,src_bitmap,x_dst,y_dst,
                                	x_src,y_src,fore,back);
            //��������
            __gk_set_pixel_rop2_bm(dst_bitmap,x_dst,y_dst,
                                	color,cn_r2_copypen);
        }
        //����˫alpha��ϣ�bit0~7=ǰ��alpha��bit16~23=����alpha
        else if((rop_code & cn_rop_double_alpha) != 0)                                            
        {
            fore = rop_code & 0xff;         //bit0~7Ϊǰ��alphaֵ
            back = (rop_code>>16) & 0xff;   //bit16~23Ϊ����alphaֵ
            //����alpha��Ϻ����ɫ
            color = __gk_rop_alpha(dst_bitmap,src_bitmap,x_dst,y_dst,
                                	x_src,y_src,fore,back);
            //��������
            __gk_set_pixel_rop2_bm(dst_bitmap,x_dst,y_dst,
                                	color,cn_r2_copypen);
        }
        //���alpha��ϣ��������ظ�ʽֻ����cn_sys_pf_a8r8g8b8
        else if((rop_code & cn_rop_pbp_alpha) != 0)     
        {
            if(src_bitmap->pf_type == cn_sys_pf_a8r8g8b8)      
            {
                fore = __gk_get_pixel_bm(src_bitmap,x_src,y_src);
                fore = fore>>24;
                back = 255-fore;        //ǰ��alpha+����alpha=255
                //����alpha��Ϻ����ɫ
                color = __gk_rop_alpha(dst_bitmap,src_bitmap,x_dst,y_dst,
                                        x_src,y_src,fore,back);
                //��������
                __gk_set_pixel_rop2_bm(dst_bitmap,x_dst,y_dst,
                                        color,cn_r2_copypen);
            }
            else
                return;  
        }
    }
}
//----���ڴ滭��(ִ�ж�Ԫ��դ����)---------------------------------------------
//����: ��һ��bitmap�ϻ��㣬ִ�ж�Ԫ��դ���������bitmap�����alpha�ģ���alpha
//      ������ᡣ
//����: bitmap��Ŀ��λͼ
//      x��y����ͼ����
//      color����ɫ��bitmap�ļ��ݸ�ʽ
//      rop2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void __gk_set_pixel_rop2_bm(struct st_rect_bitmap *bitmap,
                        s32 x,s32 y,u32 color,u32 rop2_code)
{
    u8 bit_offset;
    u8 color_bits;
    u32 byte_offset;
    u32 pf,alpha;
    color_bits = bitmap->pf_type >>8;
    //����λͼ��ɫ��ʽ���ڴ��ϻ���
    switch(color_bits)
    {
        case 1:
        {
            byte_offset = y*bitmap->linebytes + x/8;//���Ӧ������ֽ�ƫ����
            //���Ӧ�������ֽ��е�λƫ������λ���ұߣ���λ����
            bit_offset = 7-x%8; 
            //Ŀ��λͼ�϶�Ӧ�������ɫ
            pf = (u32)bitmap->bm_bits[byte_offset];
            //���ж�Ԫ��դ���������ɫ
            pf = __gk_blend_rop2(pf,color<<bit_offset,rop2_code)
                    &(0x1<<bit_offset);
            //������Ŀ��λ�ģ�����λ��ɫ
            bitmap->bm_bits[byte_offset] &= ~(1<<bit_offset);
            //����Ϻ����ɫ��ӵ���Ӧλ
            bitmap->bm_bits[byte_offset] |= (u8)pf;
        }break;
        case 2:
        {
            byte_offset = y*bitmap->linebytes + x/4;//���Ӧ������ֽ�ƫ����
            //���Ӧ�������ֽ��е�λƫ������λ���ұߣ���λ����
            bit_offset = (3-x%4)<<1;   
            //Ŀ��λͼ�϶�Ӧ�������ɫ
            pf = (u32)bitmap->bm_bits[byte_offset];
            //���ж�Ԫ��դ���������ɫ
            pf = __gk_blend_rop2(pf,color<<bit_offset,rop2_code)
                    &(0x3<<bit_offset);
            //������Ŀ��λ�ģ�����λ��ɫ
            bitmap->bm_bits[byte_offset] &= ~(3<<bit_offset);
            //����Ϻ����ɫ��ӵ���Ӧλ
            bitmap->bm_bits[byte_offset] |= (u8)pf;
        }break;
        case 4:
        {
            byte_offset = y*bitmap->linebytes + x/2;//���Ӧ������ֽ�ƫ����
            //���Ӧ�������ֽ��е�λƫ������λ���ұߣ���λ����
            bit_offset = (1-x%2)<<2;   
            //Ŀ��λͼ�϶�Ӧ�������ɫ
            pf = (u32)bitmap->bm_bits[byte_offset];
            //���ж�Ԫ��դ���������ɫ
            pf = __gk_blend_rop2(pf,color<<bit_offset,rop2_code)
                    &(0xf<<bit_offset);
            //������Ŀ��λ�ģ�����λ��ɫ
            bitmap->bm_bits[byte_offset] &= ~(0xf<<bit_offset);
            //����Ϻ����ɫ��ӵ���Ӧλ
            bitmap->bm_bits[byte_offset] |= (u8)pf;
        }break;
        case 8:            
        {
            byte_offset = y*bitmap->linebytes + x;//���Ӧ������ֽ�ƫ����
            //���ж�Ԫ��դ���������ɫ
            pf = __gk_blend_rop2((u32)bitmap->bm_bits[byte_offset],
                    color,rop2_code);
            bitmap->bm_bits[byte_offset] = (u8)pf;//����Ϻ����ɫ��ӵ���Ӧλ
        }break;
        case 12:
        {
            byte_offset = y*bitmap->linebytes + x*3/2;//���Ӧ������ֽ�ƫ����
            bit_offset = (x*12)%8;              //���Ӧ�������ֽ��е�λƫ����
            //ע:��������⣬���ɰ�Ŀ���ַǿ��ת����16λָ�롣
            pf = (u32)bitmap->bm_bits[byte_offset] 
                  + (u32)(bitmap->bm_bits[byte_offset]<<8);
            pf = pf>>bit_offset;
            pf = __gk_blend_rop2(pf,color,rop2_code);//���ж�Ԫ��դ���������ɫ
            if(bit_offset == 0)
            {
                bitmap->bm_bits[byte_offset] = (u8)(pf&0xff);
                bitmap->bm_bits[byte_offset+1] &= 0xf0;
                bitmap->bm_bits[byte_offset+1] |= (u8)(pf>>8);
            }else
            {
                bitmap->bm_bits[byte_offset] &= 0xf;
                bitmap->bm_bits[byte_offset] |= (u8)((pf&0xf)<<4);
                bitmap->bm_bits[byte_offset+1] = (u8)(pf>>4);
            }
        }break;
        case 16:
        {
            byte_offset = y*bitmap->linebytes + x*2;//���Ӧ������ֽ�ƫ����
            //Ŀ��λͼ�϶�Ӧ�������ɫ
            pf = (u32)(*(u16*)(&bitmap->bm_bits[byte_offset]));
//            alpha = pf;
            //���ж�Ԫ��դ���������ɫ
            pf = __gk_blend_rop2(pf,color,rop2_code);
//            if(bitmap->pf_type == cn_sys_pf_a1r5g5b5)
//            {
//                pf &= 0x7fff;
//                pf |= alpha & 0x8000;
//            }
            //ע:��������⣬���ɰ�Ŀ���ַǿ��ת����16λָ�롣
            bitmap->bm_bits[byte_offset] = (u8)pf;
            bitmap->bm_bits[byte_offset+1] = (u8)(pf>>8);
        }break;
        case 24:
        {
            byte_offset = y*bitmap->linebytes + x*3;//���Ӧ������ֽ�ƫ����
            pf = (u32)bitmap->bm_bits[byte_offset];
            pf += (u32)bitmap->bm_bits[byte_offset+1]<<8;
            pf += (u32)bitmap->bm_bits[byte_offset+2]<<16;
            //���ж�Ԫ��դ���������ɫ
            pf = __gk_blend_rop2(pf,color,rop2_code);
            bitmap->bm_bits[byte_offset] = (u8)pf;
            bitmap->bm_bits[byte_offset+1] = (u8)(pf >> 8);
            bitmap->bm_bits[byte_offset+2] = (u8)(pf >> 16);
        }break;
        case 32:
        {
            byte_offset = y*bitmap->linebytes + x*4;//���Ӧ������ֽ�ƫ����
            pf = (u32)(*(u32*)(&bitmap->bm_bits[byte_offset]));
            alpha = pf;
            //���ж�Ԫ��դ���������ɫ
            pf = __gk_blend_rop2(pf,color,rop2_code);
            if(bitmap->pf_type == cn_sys_pf_a8r8g8b8)
            {
                pf &= 0x00ffffff;
                pf |= alpha & 0xff000000;
            }
            //ע:��������⣬���ɰ�Ŀ���ַǿ��ת����32λָ�롣
            bitmap->bm_bits[byte_offset] = (u8)pf;
            bitmap->bm_bits[byte_offset+1] = (u8)(pf >> 8);
            bitmap->bm_bits[byte_offset+2] = (u8)(pf >> 16);
            bitmap->bm_bits[byte_offset+3] = (u8)(pf >> 24);
        }break;
    }
}
//----���ڴ滭��(ִ����Ԫ��դ����)---------------------------------------------
//����: ��һ��bitmap�ϻ��㣬ִ����Ԫ��դ���������bitmap�����alpha�ģ���alpha
//      ������ᡣ
//����: dst_bitmap��Ŀ��λͼ
//      src_bitmap��Դλͼ
//      pat��patternָ��
//      x_dst��y_dst��Ŀ��λͼ�е�Ŀ������
//      x_src��y_src��Դλͼ�е�Ŀ������
//      rop3_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void __gk_set_pixel_rop3_bm(struct st_rect_bitmap *dst_bitmap,
                                struct st_rect_bitmap *src_bitmap,u8 *pat,
                                s32 x_dst,s32 y_dst,s32 x_src,s32 y_src,
                                u32 rop3_code)
{
    u8 dst_bit_offset,pat_bit_offset;
    u8 color_bits;
    u32 byte_offset_dst,byte_offset_pat;
    u32 pf,pf_src,pf_pat,alpha,temp;
    //Ŀ��λͼ��ɫ��ʽ
    color_bits = dst_bitmap->pf_type >>8;
    //ȡԴλͼ��ɫ��Դλͼ��ɫ��ʽ���̶�
    pf_src = __gk_get_pixel_bm(src_bitmap,x_src,y_src);
    //��ȡ����Դλͼ��ɫ������ɫ��ʽת�����õ���Ŀ��λͼ��ɫ��ʽһ�µ���ɫ
    pf_src = __gk_convert_part_pf_to_pf(dst_bitmap->pf_type,
                                        src_bitmap->pf_type,pf_src);
    switch(color_bits)
    {
        case 1:
        {
            //Ŀ��λͼ��������ֽ�ƫ����
            byte_offset_dst= y_dst*dst_bitmap->linebytes + x_dst/8;
            //Ŀ��λͼ���������ֽ��е�λƫ��������λ���ұߣ���λ����
            dst_bit_offset = 7-x_dst%8;
            //ͼ��λͼ���������ֽ��е�λƫ��������λ���ұߣ���λ����
            pat_bit_offset = 7-x_src%8;
            //ͼ��λͼ��������ֽ�ƫ����
            byte_offset_pat = y_src%8; 
            //Ŀ��λͼ�����������ֽڵ�����(��һλ)
            temp = (u32)dst_bitmap->bm_bits[byte_offset_dst];
            pf = temp>>dst_bit_offset;    //Ŀ��λͼ�е�Ŀ������
            //ͼ��λͼ�е�Ŀ������(��һλ)
            pf_pat = (u32)pat[byte_offset_pat]>>pat_bit_offset;
            //rop3���
            pf =__gk_blend_rop3(pf,pf_src,pf_pat,(u8)(rop3_code>>16));
            dst_bitmap->bm_bits[byte_offset_dst] =
                            ( temp
                                  & (~(1<<dst_bit_offset)))
                             | ( (u8)((pf&1)<<dst_bit_offset));//������
        }break;
        case 2:
        {
            //Ŀ��λͼ��������ֽ�ƫ����
            byte_offset_dst= y_dst*dst_bitmap->linebytes + x_dst/4;
            //Ŀ��λͼ���������ֽ��е�λƫ��������λ���ұߣ���λ����
            dst_bit_offset = (3-x_dst%4)<<1;
            //ͼ��λͼ���������ֽ��е�λƫ��������λ���ұߣ���λ����
            pat_bit_offset = (3-x_src%4)<<1;
            //ͼ��λͼ��������ֽ�ƫ����
            byte_offset_pat = (y_src%8)*2+(x_src%8)/4; 
            //Ŀ��λͼ�е�Ŀ������(����λ)
            pf = (u32)dst_bitmap->bm_bits[byte_offset_dst]>>dst_bit_offset;
            //ͼ��λͼ�е�Ŀ������(����λ)
            pf_pat = (u32)pat[byte_offset_pat]>>pat_bit_offset;
            //rop3���
            pf =__gk_blend_rop3(pf,pf_src,pf_pat,(u8)(rop3_code>>16));
            //������
            dst_bitmap->bm_bits[byte_offset_dst] &= ~(3<<dst_bit_offset);
            dst_bitmap->bm_bits[byte_offset_dst] |= (u8)(pf&3)<<dst_bit_offset;
        }break;
        case 4:
        {
            //Ŀ��λͼ��������ֽ�ƫ����
            byte_offset_dst= y_dst*dst_bitmap->linebytes + x_dst/2;
            //Ŀ��λͼ���������ֽ��е�λƫ��������λ���ұߣ���λ����
            dst_bit_offset = (1-x_dst%2)<<2;
            //ͼ��λͼ���������ֽ��е�λƫ��������λ���ұߣ���λ����
            pat_bit_offset = (1-x_src%2)<<2;
            //ͼ��λͼ��������ֽ�ƫ����
            byte_offset_pat = (y_src%8)*4+(x_src%8)/2; 
            //Ŀ��λͼ�е�Ŀ������(����λ)
            pf = (u32)dst_bitmap->bm_bits[byte_offset_dst]>>dst_bit_offset;
            //ͼ��λͼ�е�Ŀ������(����λ)
            pf_pat = (u32)pat[byte_offset_pat]>>pat_bit_offset;
            //rop3���
            pf =__gk_blend_rop3(pf,pf_src,pf_pat,(u8)(rop3_code>>16));
            //������
            dst_bitmap->bm_bits[byte_offset_dst] &= ~(0x0f<<dst_bit_offset);
            dst_bitmap->bm_bits[byte_offset_dst] |= (u8)(pf&0x0f)<<dst_bit_offset;
        }break;
        case 8:
        {
            //Ŀ��λͼ��������ֽ�ƫ����
            byte_offset_dst= y_dst*dst_bitmap->linebytes + x_dst;
            //ͼ��λͼ��������ֽ�ƫ����
            byte_offset_pat = (y_src%8)*8+x_src%8; 
            //Ŀ��λͼ�е�Ŀ������
            pf = (u32)dst_bitmap->bm_bits[byte_offset_dst];
            //ͼ��λͼ�е�Ŀ������
            pf_pat = (u32)pat[byte_offset_pat];
            //rop3���
            pf=__gk_blend_rop3(pf,pf_src,pf_pat,(u8)(rop3_code>>16));
            //������
            dst_bitmap->bm_bits[byte_offset_dst] = (u8)pf;
        }break;
        case 12:
        {
            //Ŀ��λͼ��������ֽ�ƫ����
            byte_offset_dst= y_dst*dst_bitmap->linebytes + x_dst*3/2;
            //Ŀ��λͼ���������ֽ��е�λƫ��������λ���ұߣ���λ����
            dst_bit_offset = (x_dst*12)%8;
            //ͼ��λͼ���������ֽ��е�λƫ��������λ���ұߣ���λ����
            pat_bit_offset = (x_src*12)%8;
            //Ŀ��λͼ��������ֽ�ƫ����
            byte_offset_pat = (y_src%8)*12 + ((x_src%8)*12)/8; 
            //ע:��������⣬���ɰ�Ŀ���ַǿ��ת����16λָ�롣
            //ͼ��λͼ�е�Ŀ������
            pf_pat = (u32)pat[byte_offset_pat]+(u32)(pat[byte_offset_pat]<<8);
            pf_pat = pf_pat>>pat_bit_offset;
            //Ŀ��λͼ�е�Ŀ������
            pf = (u32)dst_bitmap->bm_bits[byte_offset_dst] 
                  + (u32)(dst_bitmap->bm_bits[byte_offset_dst]<<8);
            pf = pf>>dst_bit_offset;
            //rop3���
            pf = __gk_blend_rop3(pf,pf_src,pf_pat,(u8)(rop3_code>>16));
            //������
            if(dst_bit_offset == 0)
            {
                dst_bitmap->bm_bits[byte_offset_dst] = (u8)(pf&0xff);
                dst_bitmap->bm_bits[byte_offset_dst+1] &= 0xf0;
                dst_bitmap->bm_bits[byte_offset_dst+1] |= (u8)(pf>>8);
            }else
            {
                dst_bitmap->bm_bits[byte_offset_dst] &= 0xf;
                dst_bitmap->bm_bits[byte_offset_dst] |= (u8)((pf&0xf)<<4);
                dst_bitmap->bm_bits[byte_offset_dst+1] = (u8)(pf>>4);
            }
        }break;
        case 16:
        {
            //Ŀ��λͼ��������ֽ�ƫ����
            byte_offset_dst = y_dst*dst_bitmap->linebytes + x_dst*2;
            //ͼ��λͼ��������ֽ�ƫ����
            byte_offset_pat = ((y_src%8)*16) + (x_src%8)*2;
            //ͼ��λͼ�е�Ŀ������
            pf_pat = (u32)(*(u16*)(&pat[byte_offset_pat]));
            //Ŀ��λͼ�е�Ŀ������
            pf = (u32)(*(u16*)(&dst_bitmap->bm_bits[byte_offset_dst]));            
//            alpha = pf;
            //rop3���
            pf = __gk_blend_rop3(pf,pf_src,pf_pat,(u8)(rop3_code>>16));
//            if(dst_bitmap->pf_type == cn_sys_pf_a1r5g5b5)
//            {
//                pf &= 0x7fff;
//                pf |= alpha & 0x8000;
//            }
            //ע:��������⣬���ɰ�Ŀ���ַǿ��ת����16λָ�롣
            //������
            dst_bitmap->bm_bits[byte_offset_dst] = (u8)pf;
            dst_bitmap->bm_bits[byte_offset_dst+1] = (u8)(pf>>8);
        }break;
        case 24:
        {
            //Ŀ��λͼ��������ֽ�ƫ����
            byte_offset_dst = y_dst*dst_bitmap->linebytes + x_dst*3;
            //ͼ��λͼ��������ֽ�ƫ����
            byte_offset_pat = ((y_src%8)*24) + (x_src%8)*3;
            //ͼ��λͼ�е�Ŀ������
            pf_pat = (u32)pat[byte_offset_pat];
            pf_pat += (u32)(pat[byte_offset_pat+1]<<8);
            pf_pat += (u32)(pat[byte_offset_pat+2]<<16);
            //Ŀ��λͼ�е�Ŀ������
            pf = (u32)dst_bitmap->bm_bits[byte_offset_dst];
            pf += (u32)dst_bitmap->bm_bits[byte_offset_dst+1]<<8;
            pf += (u32)dst_bitmap->bm_bits[byte_offset_dst+2]<<16;
            //rop3���
            pf = __gk_blend_rop3(pf,pf_src,pf_pat,(u8)(rop3_code>>16));
            //������
            dst_bitmap->bm_bits[byte_offset_dst] = (u8)pf;
            dst_bitmap->bm_bits[byte_offset_dst+1] = (u8)(pf >> 8);
            dst_bitmap->bm_bits[byte_offset_dst+2] = (u8)(pf >> 16);
        }break;
        case 32:
        {
            //Ŀ��λͼ��������ֽ�ƫ����
            byte_offset_dst= y_dst*dst_bitmap->linebytes + x_dst*4;
            //ͼ��λͼ��������ֽ�ƫ����
            byte_offset_pat = ((y_src%8)*32) + (x_src%8)*4;
            //ͼ��λͼ�е�Ŀ������
            pf_pat = (u32)(*(u32*)(&pat[byte_offset_pat]));
            //Ŀ��λͼ�е�Ŀ������
            pf = (u32)(*(u32*)(&dst_bitmap->bm_bits[byte_offset_dst]));
            alpha = pf;
            //rop3���
            pf = __gk_blend_rop3(pf,pf_src,pf_pat,(u8)(rop3_code>>16));
            if(dst_bitmap->pf_type == cn_sys_pf_a8r8g8b8)
            {
                pf &= 0x00ffffff;
                pf |= alpha & 0xff000000;
            }
            //ע:��������⣬���ɰ�Ŀ���ַǿ��ת����32λָ�롣
            //������
            dst_bitmap->bm_bits[byte_offset_dst] = (u8)pf;
            dst_bitmap->bm_bits[byte_offset_dst+1] = (u8)(pf >> 8);
            dst_bitmap->bm_bits[byte_offset_dst+2] = (u8)(pf >> 16);
            dst_bitmap->bm_bits[byte_offset_dst+3] = (u8)(pf >> 24);
        }break;
    }
}
//----����---------------------------------------------------------------------
//����: �ڴ����ڻ�һ�����ص㡣
//����: �����ǽṹpara�ĳ�Ա
//      gkwin��Ŀ�괰��ָ��
//      x��y����ͼ����
//      color������ʹ�õ���ɫ
//      rop2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void gk_set_pixel(struct gksc_para_set_pixel *para)
{
    struct clip_rect *clip;
    struct gkwin_rsc *fb_gkwin;
    struct st_rect_bitmap *bitmap;
    u32 pf_color;
    s32 offsetx,offsety;
    if((para->rop2_code == cn_r2_nop) || (para->rop2_code > cn_r2_last))
        return;                                     //��ִ�в���
    //˵����win buffer����ֱ��д������Ϊfalse
    if((para->gkwin->wm_bitmap.bm_bits != NULL)
        && (para->gkwin->direct_screen == false))     
    {   //������:��win buffer�л�ͼ����־changed_msk
        bitmap = &para->gkwin->wm_bitmap;
        //����ʾ��ʹ��cn_custom_pf��ʽ��Ӧ����Ӳ��ʵ�ֻ�������
        if(bitmap->pf_type & cn_custom_pf)
        {
            para->gkwin->disp->draw.set_pixel_bm(bitmap,para->x,para->y,
                                                 para->color,para->rop2_code);
        }else//�����ʵ�ֻ�������
        {
            //�涨�����߸�������ɫ�������ɫ
            pf_color = __gk_convert_color_to_pf(bitmap->pf_type,para->color);
            __gk_set_pixel_rop2_bm(bitmap,para->x,para->y,
                                pf_color,para->rop2_code);      //��������
        }
        __gk_shading_pixel(para->gkwin,para->x,para->y);//��־���ص�changed_msk
    }else
    {
        clip = para->gkwin->visible_clip;
        if(clip == NULL)        //������Ϊ�գ�ֱ�ӷ���
            return ;
        fb_gkwin = para->gkwin->disp->frame_buffer;
        offsetx = para->gkwin->absx0 +para->x;
        offsety = para->gkwin->absy0 +para->y;
        //��frame buffer����ֱ��д������Ϊfalse
        if((fb_gkwin != NULL) && (para->gkwin->direct_screen == false))         
        {   //������:��frame buffer�л�ͼ����ֻ��gkwin�еĿ�������
            bitmap = &fb_gkwin->wm_bitmap;
            do{
                //���ض�Ӧ�������ڿ������ڣ����������
                if(!((offsetx<clip->rect.left) || (offsetx>=clip->rect.right)
                    || (offsety<clip->rect.top) ||(offsety>=clip->rect.bottom)))
                {   
                    //����ʾ��ʹ��cn_custom_pf��ʽ��Ӧ����Ӳ��driverʵ�ֻ�������
                    if(bitmap->pf_type & cn_custom_pf)
                    {
                        fb_gkwin->disp->draw.set_pixel_bm(bitmap,offsetx,
                                           offsety,para->color,para->rop2_code);
                    }else//�����ʵ�ֻ�������
                    {
                        //�涨�����߸�������ɫ��ɫ��ʽ�������ɫ
                        pf_color = __gk_convert_color_to_pf(
                                                bitmap->pf_type,para->color);
                        __gk_set_pixel_rop2_bm(bitmap,offsetx,offsety,
                                                pf_color,para->rop2_code);
                    }
                    //��־���ص�changed_msk
                    __gk_shading_pixel(fb_gkwin,offsetx,offsety);
                    break;      //�����Ѿ����ƣ��˳�ѭ��
                }else//���ض�Ӧ�����겻�ڿ������ڣ�ָ����һ��������
                    clip = clip->next;
            }while(clip != para->gkwin->visible_clip);
        }
        else
        //��win buffer��Ҳ��frame buffer��ֱ�ӻ���screen��
        //ֱ��д������Ϊtrue���������޻���������ֱ�ӻ���screen��
        {
            do{
                //���ض�Ӧ�������ڿ������ڣ����������
                if(!((offsetx<clip->rect.left) || (offsetx>=clip->rect.right)
                    || (offsety<clip->rect.top) ||(offsety>=clip->rect.bottom)))
                {
                    __gk_set_pixel_screen(para->gkwin->disp,offsetx,offsety,
                                          para->color,para->rop2_code);
                    break;      //�����Ѿ����ƣ��˳�ѭ��
                }else//���ض�Ӧ�����겻�ڿ������ڣ�ָ����һ��������
                    clip = clip->next;
            }while(clip != para->gkwin->visible_clip);
        }
    }
}

//----����changed_msk:����-----------------------------------------------------
//����: ��������ʱ�����ô��ڵ�changed_msk���Ѹ�д����������ɫ���ɵ�����ȷ������
//      �Ϸ���
//����: gkwin��Ŀ�괰��ָ��
//      x��y��Ŀ�����ض�Ӧ������
//����: ��
//-----------------------------------------------------------------------------
void __gk_shading_pixel(struct gkwin_rsc *gkwin,s32 x,s32 y)
{
    s32 msk_x,msk_y;    
    if(gkwin->change_flag == cn_gkwin_change_all)   //�Ѿ�ȫ����ɫ
    {
        return;
    }
    gkwin->change_flag = cn_gkwin_change_part;
    msk_x = x/8;       //����x��mskλͼ�еڼ���
    msk_y = y/8;       //����y��mskλͼ�еڼ���
    //���������ڵ�changed_msk�е�bit��Ϊ1
    __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),msk_x,msk_y,
                                cn_color_white,cn_r2_copypen);
    return;
}

//----����changed_msk----------------------------------------------------------
//����: ��һ�����ڵ�rect�޶�����������Ϊ����д
//����: bitmap��Ŀ��bitmap
//      rect����д����
//����: ��
//˵��: �ڲ����������������Ϸ��ԣ��ɵ����߱�֤����ĺϷ���
//-----------------------------------------------------------------------------
void __gk_shading_rect(struct gkwin_rsc *gkwin,struct st_rect *rect)
{
    struct st_rect_bitmap *bitmap;
    s32 x,y;
    s32 msk_x1,msk_x2,msk_y1,msk_y2;

    if(gkwin->change_flag == cn_gkwin_change_all)   //�Ѿ�ȫ����ɫ
    {
        return;
    }
    bitmap = &gkwin->wm_bitmap;
    if((rect->left <= 0) && (rect->top <= 0) 
            && (rect->right >= bitmap->width)&&(rect->bottom >= bitmap->height))
    {
        gkwin->change_flag = cn_gkwin_change_all;   //��msk�����򸲸���������
        return;
    }
    
    gkwin->change_flag = cn_gkwin_change_part;
    msk_x1 = rect->left/8;          //����x1��mskλͼ�еڼ���
    msk_x2 = (rect->right-1)/8;     //����x2��mskλͼ�еڼ��У��������յ㣬-1
    msk_y1 = rect->top/8;           //����y1��mskλͼ�еڼ���
    msk_y2 = (rect->bottom-1)/8;    //����y2��mskλͼ�еڼ��У��������յ㣬-1

    for(y = msk_y1;y <= msk_y2;y++)
    {
        for(x = msk_x1;x <= msk_x2;x++)
            __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),x,y,
                                cn_color_white,cn_r2_copypen);
    }
}

//----����changed_msk----------------------------------------------------------
//����: �ڴ��ڻ�frame buffer����ֱ��ʱ�����ô��ڵ�changed_msk��
//      �Ѹ�д����������ɫ��
//����: gkwin��Ŀ�괰��
//      limit�������ͼ����������һ�����ε����򣬳����������ֱ�߲���
//      x1��y1��x2��y2�� ��㡢�յ�����
//����: ��
//-----------------------------------------------------------------------------
void __gk_shading_line(struct gkwin_rsc *gkwin,struct st_rect *limit,
                     s32 x1,s32 y1,s32 x2,s32 y2)
{
    if(gkwin->change_flag == cn_gkwin_change_all)   //�Ѿ�ȫ����ɫ
    {
        return;
    }
    gkwin->change_flag = cn_gkwin_change_part;
    
    if(y1 == y2)        //ˮƽֱ����ɫ
    {
        s32 x;
        s32 msk_x1,msk_x2,msk_y1;
        if(x1>x2)
            __gk_swap(x1,x2);
        if((y1 <= limit->top) || (y1 >= limit->bottom)
            ||(x2 <= limit->left) || (x1 >= limit->right))
            return;         //����ֱ����limit֮��
        if(x1 < limit->top)
            x1 = limit->top;
        if(x2 > limit->bottom)
            x2 = limit->bottom;
        msk_x1 = x1/8;     //����x1��mskλͼ�еڼ���
        msk_x2 = (x2-1)/8; //����(x2-1)��mskλͼ�еڼ��У�-1����Ϊ�յ㲻��������
        msk_y1 = y1/8;     //����y1��mskλͼ�еڼ���
        for(x = msk_x1;x <= msk_x2;x++)
        {
            //��changed_msk��Ӧ��bit��1
            __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),x,msk_y1,
                                    cn_color_white,cn_r2_copypen);
        }

    }
    else if(x1 == x2)   //��ֱ����ɫ
    {
        s32 y;
        s32 msk_x1,msk_y1,msk_y2;
        if(y1>y2)
            __gk_swap(y1,y2);
        if((x1<limit->left) || (x1 >= limit->right)
            ||(y2 <= limit->top) || (y1 >= limit->bottom))
            return;         //����ֱ����limit֮��
        if(y1 < limit->top)
            y1 = limit->top;
        if(y2 > limit->bottom)
            y2 = limit->bottom;
        msk_x1 = x1/8;     //����x1��mskλͼ�еڼ���        
        msk_y1 = y1/8;     //����y1��mskλͼ�еڼ���
        msk_y2 = (y2-1)/8; //����(y2-1)��mskλͼ�еڼ��У�-1����Ϊ�յ㲻��������
        for(y = msk_y1;y <= msk_y2;y++)
        {
            //��changed_msk��Ӧ��bit��1
            __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),msk_x1,y,
                                    cn_color_white,cn_r2_copypen);
        }
    }
    else               
    {
        s32 x,y;
        s32 msk_x,msk_y;
        s32 dx,dy;
        dx = abs(x2-x1);
        dy = abs(y2-y1);

        //x����仯����
        if(dx > dy)
        {            
            if(((x1>x2)&&(y1>y2)) || ((x1<x2)&&(y1<y2)))
            {//x��y����������ͬ����ͬ������ͬ��
                //ѡȡx1��x2�н�С������Ϊx1
                if(x1 > x2)
                {
                    __gk_swap(x1,x2);
                    __gk_swap(y1,y2);
                }
                //���¶��룬ȡ8*8���أ�xÿ�仯8���غ�y��ֱ�ߵĽ��㣬
                //��б�������еĵ����ڵ�8*8���������Ӧ��
                //��changed_msk�е�bit��Ϊ1������˵���뿴djygui����
                for(x = align_down(8,x1); x <= x2; x+=8)
                {
                    //�Լ���Ľ�������������봦��x��y����������ͬ
                    //������0.5Ϊ��׼��
                    y = ((((x-x1)<<1)*(y2-y1)+(x2-x1))/((x2-x1)<<1))+y1;
                    msk_x = x/8;    //����x��mskλͼ�еڼ���
                    msk_y = y/8;    //����y��mskλͼ�еڼ���
                    //��changed_msk��Ӧ��bit��1
                    __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),msk_x,
                                         msk_y,cn_color_white,cn_r2_copypen);
                    //��־(msk_x��msk_y)�����һ����
                    msk_y += 1;
                    //��changed_msk��Ӧ��bit��1
                    __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),msk_x,
                                         msk_y,cn_color_white,cn_r2_copypen);
                }
            }
            else
            {//x��y���������෴����x����y����x����y��
                //ѡȡx1��x2�н�С������Ϊx1
                if(x1 > x2)
                {
                    __gk_swap(x1,x2);
                    __gk_swap(y1,y2);
                }
                //���¶��룬ȡ8*8���أ�xÿ�仯8���غ�y��ֱ�ߵĽ��㣬
                //��б�������еĵ����ڵ�8*8���������Ӧ��
                //��changed_msk�е�bit��Ϊ1������˵���뿴djygui����
                for(x = align_down(8,x1); x <= x2; x+=8)
                {
                    //�����ý�������������봦��yΪ������������-0.5Ϊ��׼��
                    y = ((((x-x1)<<1)*(y2-y1)-(x2-x1))/((x2-x1)<<1))+y1;
                    msk_x = x/8;    //����x��mskλͼ�еڼ���
                    msk_y = y/8;    //����y��mskλͼ�еڼ���
                    //��changed_msk��Ӧ��bit��1
                    __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),msk_x,
                                         msk_y,cn_color_white,cn_r2_copypen);
                    //��־(msk_x��msk_y)�����һ����
                    msk_y -= 1;
                    //��changed_msk��Ӧ��bit��1
                    __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),msk_x,
                                         msk_y,cn_color_white,cn_r2_copypen);
                }
            }
        }else
        {//y����仯���󣬻���x��y�仯�����
            if(((x1>x2)&&(y1>y2)) || ((x1<x2)&&(y1<y2)))
            {//x��y����������ͬ����ͬ������ͬ��   
                //ѡȡy1��y2�н�С������Ϊy1
                if(y1 > y2)
                {
                    __gk_swap(x1,x2);
                    __gk_swap(y1,y2);
                }
                //���¶��룬ȡ8*8���أ�yÿ�仯8���غ�x��ֱ�ߵĽ��㣬
                //��б�������еĵ����ڵ�8*8���������Ӧ��
                //��changed_msk�е�bit��Ϊ1������˵���뿴djygui����
                for(y = align_down(8,y1); y <= y2; y+=8)
                {
                    x = ((((y-y1)<<1)*(x2-x1)+(y2-y1))/((y2-y1)<<1))+x1;
                    msk_x = x/8;    //����x��mskλͼ�еڼ���
                    msk_y = y/8;    //����y��mskλͼ�еڼ���
                    //��changed_msk��Ӧ��bit��1
                    __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),msk_x,
                                         msk_y,cn_color_white,cn_r2_copypen);
                    msk_x += 1;
                    //��changed_msk��Ӧ��bit��1
                    __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),msk_x,
                                         msk_y,cn_color_white,cn_r2_copypen);
                }
            }
            else
            {//x��y���������෴����x����y����x����y��
                //ѡȡy1��y2�н�С������Ϊy1
                if(y1 > y2)
                {
                    __gk_swap(x1,x2);
                    __gk_swap(y1,y2);
                }
                //���¶��룬ȡ8*8���أ�yÿ�仯8���غ�x��ֱ�ߵĽ��㣬
                //��б�������еĵ����ڵ�8*8���������Ӧ��
                //��changed_msk�е�bit��Ϊ1������˵���뿴djygui����
                for(y = align_down(8,y1); y <= y2; y+=8)
                {
                    x = ((((y-y1)<<1)*(x2-x1)-(y2-y1))/((y2-y1)<<1))+x1;
                    msk_x = x/8;    //����x��mskλͼ�еڼ���
                    msk_y = y/8;    //����y��mskλͼ�еڼ���
                    //��changed_msk��Ӧ��bit��1
                    __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),msk_x,
                                         msk_y,cn_color_white,cn_r2_copypen);
                    msk_x -= 1;
                    //��changed_msk��Ӧ��bit��1
                    __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),msk_x,
                                             msk_y,cn_color_white,cn_r2_copypen);
                }
            }
        }
    }
}

//----���changed_msk----------------------------------------------------------
//����: ���һ�����ڵĸ�д��־��
//����: gkwin��Ŀ�괰��ָ��
//����: ��
//-----------------------------------------------------------------------------
void __gk_shading_clear(struct gkwin_rsc *gkwin)
{
    u32 msk_size;
    if(gkwin->changed_msk.bm_bits)
    {
        msk_size = 
            (gkwin->wm_bitmap.width +7)/8 /8*((gkwin->wm_bitmap.height +7)/8);
        memset(gkwin->changed_msk.bm_bits,0,msk_size);
    }
}

//----����ֱ��(�����˵�)-------------------------------------------------------
//����: �ڴ����ڴӵ�ǰ��괦��ʼ��һ����ֱ�ߡ�
//����: gkwin��Ŀ�괰��ָ��
//      limit�����Ƶ���������ֻ����ֱ����limit�����ڲ��Ĳ���
//      x1��y1����ʼ�������
//      y2���������������
//      color����ɫ��cn_sys_pf_e8r8g8b8��ʽ
//      rop2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void __gk_vlineto_bm(struct gkwin_rsc *gkwin,struct st_rect *limit,
                     s32 x1,s32 y1,s32 y2,u32 color,u32 rop2_code)
{
    s32 y;
    s32 msk_x1,msk_y1,msk_y2;
    u32 pf_color;
    struct st_rect_bitmap *bitmap;
    if(y1>y2)
        __gk_swap(y1,y2);
    if((x1<limit->left) || (x1 >= limit->right)
        ||(y2 <= limit->top) || (y1 >= limit->bottom))
        return;         //����ֱ����limit֮��
    bitmap = &gkwin->wm_bitmap;
    //��ֱ�߳���limit�Ĳ��ֲ�����
    if(y1 < limit->top)
        y1 = limit->top;
    if(y2 > limit->bottom)
        y2 = limit->bottom;
    if(bitmap->pf_type & cn_custom_pf)
    {
        for(y = y1; y < y2;y++)
        {
            gkwin->disp->draw.set_pixel_bm(bitmap,x1,y,color,rop2_code);
        }
    }else
    {
        //�û�������ɫ��ɫ��ʽΪrgb24
        pf_color = __gk_convert_color_to_pf(bitmap->pf_type,color);
        for(y = y1;y < y2;y++)
        {
            __gk_set_pixel_rop2_bm(bitmap,x1,y,pf_color,rop2_code);
        }
    }

    //���洦��changed_msk��־λͼ���Ѵ��������߶��޸ĵ�������ɫ
    if(gkwin->change_flag != cn_gkwin_change_all)
    {
        gkwin->change_flag = cn_gkwin_change_part;  //��־���ڲ����޸�
        msk_x1 = x1/8;     //����x1��mskλͼ�еڼ���        
        msk_y1 = y1/8;     //����y1��mskλͼ�еڼ���
        msk_y2 = (y2-1)/8; //����(y2-1)��mskλͼ�еڼ��У�-1����Ϊ�յ㲻��������
        for(y = msk_y1;y <= msk_y2;y++)
        {   //��changed_msk��Ӧ��bit��1
            __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),msk_x1,y,
                                    cn_color_white,cn_r2_copypen);
        }
    }
}

//----��ˮƽֱ��(�����˵�)-------------------------------------------------------
//����: �ڴ����ڴӵ�ǰ��괦��ʼ��һ��ˮƽֱ�ߡ�
//����: gkwin��Ŀ�괰��ָ��
//      limit�����Ƶ���������ֻ����ֱ����limit�����ڲ��Ĳ���
//      x1��y1����ʼ�������
//      x2��������ĺ�����
//      color����ɫ��cn_sys_pf_e8r8g8b8��ʽ
//      rop2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void __gk_hlineto_bm(struct gkwin_rsc *gkwin,struct st_rect *limit,
                     s32 x1,s32 y1,s32 x2,u32 color,u32 rop2_code)
{
    s32 x;
    s32 msk_x1,msk_x2,msk_y1;
    u32 pf_color;
    struct st_rect_bitmap *bitmap;
    if(x1>x2)
        __gk_swap(x1,x2);
    if((y1<limit->top) || (y1 >= limit->bottom)
        ||(x2 <= limit->left) || (x1 >= limit->right))
        return;         //����ֱ����limit֮��
    bitmap = &gkwin->wm_bitmap;
    //ˮƽ����limit�ⲿ�ֲ�����
    if(x1 < limit->left)        
        x1 = limit->left;
    if(x2 > limit->right)
        x2 = limit->right;
    if(bitmap->pf_type & cn_custom_pf)
    {
        for(x = x1; x < x2;x++)
        {
            gkwin->disp->draw.set_pixel_bm(bitmap,x,y1,color,rop2_code);
        }
    }else
    {
        //�û�������ɫ��ɫ��ʽΪrgb24
        pf_color = __gk_convert_color_to_pf(bitmap->pf_type,color);
        for(x = x1;x < x2;x++)  
        {//��1��2��4λɫscreen�����ѭ���ܺ�cpu�����ǸĽ�--db
            __gk_set_pixel_rop2_bm(bitmap,x,y1,pf_color,rop2_code);
        }
    }

    //���洦��changed_msk��־λͼ���Ѵ��������߶��޸ĵ�������ɫ
    if(gkwin->change_flag != cn_gkwin_change_all)
    {
        gkwin->change_flag = cn_gkwin_change_part;  //��־���ڲ����޸�
        msk_x1 = x1/8;     //����x1��mskλͼ�еڼ���
        msk_x2 = (x2-1)/8; //����(x2-1)��mskλͼ�еڼ��У�-1����Ϊ�յ㲻��������
        msk_y1 = y1/8;     //����y1��mskλͼ�еڼ���
        for(x = msk_x1;x <= msk_x2;x++)
        {   //��changed_msk��Ӧ��bit��1
            __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),x,msk_y1,
                                    cn_color_white,cn_r2_copypen);
        }
    }
}
//----��б��(�����˵�)---------------------------------------------------------
//����: �ڴ����ڴӵ�ǰ��괦��ʼ��һ��б�ߡ�
//����: gkwin��Ŀ�괰��ָ��
//      limit�����Ƶ���������ֻ����ֱ����limit�����ڲ��Ĳ���
//      x1��y1����ʼ�������
//      x2��y2�������������
//      color����ɫ��cn_sys_pf_e8r8g8b8��ʽ
//      rop2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void __gk_olineto_bm(struct gkwin_rsc *gkwin,struct st_rect *limit,//ȷ��
                       s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 rop2_code)
{
    s32 x,y;
    s32 msk_x,msk_y;
    s32 dx,dy;

    if(__gk_oline_sect_inter(limit,&x1,&y1,&x2,&y2))//ȡ��б����limit�Ľ���
    {
        __gk_draw_oline(gkwin,x1,y1,x2,y2,color,rop2_code);//����б��

        //���洦��changed_msk��־λͼ���Ѵ��������߶��޸ĵ�������ɫ
        if(gkwin->change_flag != cn_gkwin_change_all)
        {        
            gkwin->change_flag = cn_gkwin_change_part;//��־���ڲ����޸�
            dx = abs(x2-x1);
            dy = abs(y2-y1);
            //x����仯����
            if(dx > dy)
            {            
                if(((x1>x2)&&(y1>y2)) || ((x1<x2)&&(y1<y2)))
                {//x��y����������ͬ����ͬ������ͬ��
                    //ѡȡx1��x2�н�С������Ϊx1
                    if(x1 > x2)
                    {
                        __gk_swap(x1,x2);
                        __gk_swap(y1,y2);
                    }
                    //���¶��룬ȡ8*8���أ�xÿ�仯8���غ�y��ֱ�ߵĽ��㣬
                    //��б�������еĵ����ڵ�8*8���������Ӧ��
                    //��changed_msk�е�bit��Ϊ1������˵���뿴djygui����
                    for(x = align_down(8,x1); x <= x2; x+=8)
                    {
                        //�Լ���Ľ�������������봦��x��y����������ͬ
                        //������0.5Ϊ��׼��
                        y = ((((x-x1)<<1)*(y2-y1)+(x2-x1))/((x2-x1)<<1))+y1;
                        msk_x = x/8;    //����x��mskλͼ�еڼ���
                        msk_y = y/8;    //����y��mskλͼ�еڼ���
                        //��changed_msk��Ӧ��bit��1
                        __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),msk_x,
                                             msk_y,cn_color_white,cn_r2_copypen);
                        //��־(msk_x��msk_y)�����һ����
                        msk_y += 1;
                        //��changed_msk��Ӧ��bit��1
                        __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),msk_x,
                                             msk_y,cn_color_white,cn_r2_copypen);
                    }
                }
                else
                {//x��y���������෴����x����y����x����y��
                    //ѡȡx1��x2�н�С������Ϊx1
                    if(x1 > x2)
                    {
                        __gk_swap(x1,x2);
                        __gk_swap(y1,y2);
                    }
                    //���¶��룬ȡ8*8���أ�xÿ�仯8���غ�y��ֱ�ߵĽ��㣬
                    //��б�������еĵ����ڵ�8*8���������Ӧ��
                    //��changed_msk�е�bit��Ϊ1������˵���뿴djygui����
                    for(x = align_down(8,x1); x <= x2; x+=8)
                    {
                        //�����ý�������������봦��yΪ������������-0.5Ϊ��׼��
                        y = ((((x-x1)<<1)*(y2-y1)-(x2-x1))/((x2-x1)<<1))+y1;
                        msk_x = x/8;    //����x��mskλͼ�еڼ���
                        msk_y = y/8;    //����y��mskλͼ�еڼ���
                        //��changed_msk��Ӧ��bit��1
                        __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),msk_x,
                                             msk_y,cn_color_white,cn_r2_copypen);
                        //��־(msk_x��msk_y)�����һ����
                        msk_y -= 1;
                        //��changed_msk��Ӧ��bit��1
                        __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),msk_x,
                                             msk_y,cn_color_white,cn_r2_copypen);
                    }
                }
            }else
            {//y����仯���󣬻���x��y�仯�����
                if(((x1>x2)&&(y1>y2)) || ((x1<x2)&&(y1<y2)))
                {//x��y����������ͬ����ͬ������ͬ��   
                    //ѡȡy1��y2�н�С������Ϊy1
                    if(y1 > y2)
                    {
                        __gk_swap(x1,x2);
                        __gk_swap(y1,y2);
                    }
                    //���¶��룬ȡ8*8���أ�yÿ�仯8���غ�x��ֱ�ߵĽ��㣬
                    //��б�������еĵ����ڵ�8*8���������Ӧ��
                    //��changed_msk�е�bit��Ϊ1������˵���뿴djygui����
                    for(y = align_down(8,y1); y <= y2; y+=8)
                    {
                        x = ((((y-y1)<<1)*(x2-x1)+(y2-y1))/((y2-y1)<<1))+x1;
                        msk_x = x/8;    //����x��mskλͼ�еڼ���
                        msk_y = y/8;    //����y��mskλͼ�еڼ���
                        //��changed_msk��Ӧ��bit��1
                        __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),msk_x,
                                             msk_y,cn_color_white,cn_r2_copypen);
                        msk_x += 1;
                        //��changed_msk��Ӧ��bit��1
                        __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),msk_x,
                                             msk_y,cn_color_white,cn_r2_copypen);
                    }
                }
                else
                {//x��y���������෴����x����y����x����y��
                    //ѡȡy1��y2�н�С������Ϊy1
                    if(y1 > y2)
                    {
                        __gk_swap(x1,x2);
                        __gk_swap(y1,y2);
                    }
                    //���¶��룬ȡ8*8���أ�yÿ�仯8���غ�x��ֱ�ߵĽ��㣬
                    //��б�������еĵ����ڵ�8*8���������Ӧ��
                    //��changed_msk�е�bit��Ϊ1������˵���뿴djygui����
                    for(y = align_down(8,y1); y <= y2; y+=8)
                    {
                        x = ((((y-y1)<<1)*(x2-x1)-(y2-y1))/((y2-y1)<<1))+x1;
                        msk_x = x/8;    //����x��mskλͼ�еڼ���
                        msk_y = y/8;    //����y��mskλͼ�еڼ���
                        //��changed_msk��Ӧ��bit��1
                        __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),msk_x,
                                             msk_y,cn_color_white,cn_r2_copypen);
                        msk_x -= 1;
                        //��changed_msk��Ӧ��bit��1
                        __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),msk_x,
                                                 msk_y,cn_color_white,cn_r2_copypen);
                    }
                }
            }
        }
    }
}
//----��ֱ��(�����˵�)---------------------------------------------------------
//����: �ڴ����ϻ�ֱ�ߣ��˵㲻�����봦��changed_msk����
//����: gkwin��Ŀ�괰��ָ��
//      limit�������ͼ����������һ�����ε�����,�����������ֱ�߲���
//      x1��y1��x2��y2�������յ�����
//      color����ͼ���õ���ɫ
//      rop2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void __gk_lineto_bm(struct gkwin_rsc *gkwin,struct st_rect *limit,
                    s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 rop2_code)
{
    if(y1 == y2)        //��ˮƽ��
        __gk_hlineto_bm(gkwin,limit,x1,y1,x2,color,rop2_code);
    else if(x1 == x2)   //�洹ֱ��
        __gk_vlineto_bm(gkwin,limit,x1,y1,y2,color,rop2_code);
    else                //��б��
        __gk_olineto_bm(gkwin,limit,x1,y1,x2,y2,color,rop2_code);
}

//----��screen�ϻ�����---------------------------------------------------------
//����: ��screen��ֱ�ӻ��㡣
//����: gkwin�����Ƶ�Ŀ�괰��
//      x��y�� ����
//      color����ͼ���õ���ɫ��cn_sys_pf_e8r8g8b8��ʽ
//      r2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void __gk_set_pixel_screen(struct display_rsc *display,s32 x,s32 y,
                           u32 color,u32 rop2_code)
{
    struct display_rsc *mirror,*current;

    display->draw.set_pixel_screen(x,y,color,rop2_code);//��screen�ϻ���
    mirror = (struct display_rsc *)display->node.child;
    if(mirror == NULL)      //û�о�����ʾ����ֱ�ӷ���
        return;
    current = mirror;
    while(current != NULL)
    {
        current->draw.set_pixel_screen(x,y,color,rop2_code);
        current = (struct display_rsc *)rsc_trave_scion(&mirror->node,
                &current->node);
    }
}

//----��ֱ��(�����˵�)---------------------------------------------------------
//����: ��screen��ֱ�ӻ�ֱ�ߣ��˵㲻����
//����: display�����Ƶ�Ŀ����ʾ��
//      limit�������ͼ����������һ�����ε����򣬳����������ֱ�߲���
//      x1��y1��x2��y2�������յ�����
//      color����ͼ���õ���ɫ��cn_sys_pf_e8r8g8b8��ʽ
//      r2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void __gk_lineto_screen(struct display_rsc *display,struct st_rect *limit,
                   s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 rop2_code)
{
    struct display_rsc *mirror,*current;
    s32 x,y,ax,ay,s1,s2,p,interchange,i;

    if(display->draw.line_screen(limit, x1, y1, x2, y2, color,rop2_code))
    {
    }else   //Ӳ�����ٲ�֧��ֱ�߻��ƣ����ʵ��
    {
        if(y1 == y2)    //��ˮƽ��
        {
            if(x1 > x2)
                __gk_swap(x1,x2);
            if((y1 < limit->top) || (y1 >= limit->bottom)
                ||(x2 <= limit->left) || (x1 >= limit->right))
                return;         //����ֱ����limit֮��
            if(x1 < limit->left)
                x1 = limit->left;
            if(x2 > limit->right)
                x2 = limit->right;
            for(x = x1; x < x2;x++)
            {
                display->draw.set_pixel_screen(x,y1,color,rop2_code);
            }
        }else if(x1 == x2)   //�洹ֱ��
        {
            if(y1 > y2)
                __gk_swap(y1,y2);          
            if((x1 < limit->left) || (x1 >= limit->right)
                ||(y2 <= limit->top) || (y1 >= limit->bottom))
                return;         //����ֱ����limit֮��
            if(y1 < limit->top)
                y1 = limit->top;
            if(y2 > limit->bottom)
                y2 = limit->bottom;
            for(y = y1; y < y2;y++)
            {
                display->draw.set_pixel_screen(x1,y,color,rop2_code);
            }
        }else            //��б��
        {
            if( __gk_oline_sect_inter(limit,&x1,&y1,&x2,&y2))
            {
                x = x1;
                y = y1;
                ax = abs(x2-x1);
                ay = abs(y2-y1);
                if(x2 > x1)   //����б�߷���
                   s1 = 1;
                else
                   s1 = -1;
                if(y2 > y1)
                   s2 = 1;
                else
                   s2 = -1;
                if(ay > ax)
                {
                    __gk_swap(ax,ay);
                    interchange = 1;
                }
                else
                    interchange = 0;
                p = (ay<<1)-ax;   //���ó�ʼ����б�ֵ
                for(i = 1;i <= ax;i++)
                {
                    display->draw.set_pixel_screen(x,y,color,rop2_code);

                    if(p >= 0)     //ȡ����ѧ��Ͻ�������
                    {
                        if(interchange == 0)
                           y = y+s2;
                        else
                           x = x+s1;
                        p = p-(ax<<1);
                    }
                    if(interchange == 0)
                        x = x+s1; 
                    else
                        y = y+s2;
                    p = p+(ay<<1);
                }   
            }
        }
    }
    mirror = (struct display_rsc *)display->node.child;
    if(mirror == NULL)      //û�о�����ʾ����ֱ�ӷ���
        return;
    current = mirror;
    while(current != NULL)
    {
        current->draw.line_screen(limit, x1, y1, x2, y2, color,rop2_code);
        current = (struct display_rsc *)rsc_trave_scion(&mirror->node,
                &current->node);
    }    
}
//----����bitmap��screen-------------------------------------------------------
//����: ��bitmap�Ĳ��ִ��ڴ洫�͵�screen��
//����: gkwin�����Ƶ�Ŀ�괰��
//      rect����ͼ����һ�����ε����򣬳���������Ĳ��ֲ���
//      bitmap�����Ƶ�λͼ
//      x��y��bitmapҪ���Ͳ������Ͻ�����
//����: ��
//-----------------------------------------------------------------------------
void __gk_blt_bm_to_screen(struct display_rsc *display,struct st_rect *rect,
                            struct st_rect_bitmap *bitmap,s32 x,s32 y)
{
    u32 color;
    s32 x_bmap,y_bmap,x_rect,y_rect;
    struct display_rsc *mirror,*current; 

    if(display->draw.bm_to_screen(rect,bitmap,x,y))
    {
    }
    else//bitmap��ɫ��ʽ����ʾ����һ������Ӳ�����ٲ�֧�������ʵ��
    {
        //����bitmap��screen
        y_bmap = y;
        for(y_rect = rect->top;y_rect < rect->bottom;y_rect++)
        {
            x_bmap = x;
            for(x_rect = rect->left;x_rect < rect->right;x_rect++)
            {
                color = __gk_get_pixel_bm(bitmap,x_bmap,y_bmap);
                color = __gk_convert_color_to_24(bitmap->pf_type,color);
                display->draw.set_pixel_screen(x_rect,y_rect,
                                                color,cn_r2_copypen);
                x_bmap++;
            }
            y_bmap++;
        }
    }
    mirror = (struct display_rsc *)display->node.child;
    if(mirror == NULL)      //û�о�����ʾ����ֱ�ӷ���
        return;
    current = mirror;
    while(current != NULL)
    {
        current->draw.bm_to_screen(rect,bitmap,x,y);
        current = (struct display_rsc *)rsc_trave_scion(&mirror->node,
                                                        &current->node);
    }
}

//----������-----------------------------------------------------------------
//����: ��dst_bitmap��dst_rect��ռ�ݵ�λ���ϣ���color��ɫ��䡣color�ĸ�ʽ��
//      cn_sys_pf_e8r8g8b8��Ҫת����dst_bitmap->pf_typeƥ�����ɫ��䡣
//����: dst_bitmap��������λͼ
//      dst_rect������λ��
//      color��������ɫ����ʽ��cn_sys_pf_e8r8g8b8
//����: ��
//-----------------------------------------------------------------------------
void __gk_fill_rect(struct st_rect_bitmap *dst_bitmap,
                     struct st_rect *dst_rect,u32 color)
{
    s32 x,y,x1,y1,x2,y2;
    u8 bit_offset,i;
    u8 color_bits;
    u32 byte_offset,temp_bytes;
    u32 pf,pf_color,alpha;
    u8 bit_offset1,bit_offset2,start_byte,end_byte,middle_byte;       
    u32 byte_offset1,byte_offset2;

    x1 = dst_rect->left;
    y1 = dst_rect->top;
    y2 = dst_rect->bottom;
    x2 = dst_rect->right;
    temp_bytes = dst_bitmap->linebytes;
    //color�ĸ�ʽ��cn_sys_pf_e8r8g8b8��ת����dst_bitmap->pf_typeƥ��ĸ�ʽ
    pf_color = __gk_convert_color_to_pf(dst_bitmap->pf_type,color);
    color_bits = dst_bitmap->pf_type >>8;   //������λͼ����ɫ��ʽ
    start_byte = 0;
    end_byte = 0;
    switch(color_bits)
    {
        case 1:
        {    
            pf = pf_color & 0x01;               //ȡ�����������ɫ
            byte_offset1 = y1*temp_bytes+x1/8;  //dst_rect������߽������ֽ�
            byte_offset2 = y1*temp_bytes+x2/8;  //dst_rect�����ұ߽������ֽ�
            bit_offset1 = 7-x1%8;   //dst_rect��߽��������ֽڵ�λ��x=0��bit=7
            bit_offset2 = x2%8;     //7��ȥdst_rect�ұ߽��������ֽڵ�λ��7-bit
            //dst_rect���е����ֽ���β�ֽڲ���һ���ֽ���
            if(byte_offset1 != byte_offset2)
            {
                //dst_rectÿ�е����ֽڸ�λ����ɫ
                //���ֽ�ȡ��ɫ��λ����bit��ͬ���ұ��Ǹ�λ
                for(i = 0;i <= bit_offset1;i++) 
                    start_byte |= pf<<i;
                for(i = 0;i <= bit_offset2;i++) //dst_rectÿ�е�β�ֽڸ�λ����ɫ
                    end_byte |= pf<<(7-i);      //���ֽ�ȡ��ɫ��λ����7-bit��ͬ
                //ȡÿ���м��ֽڵ���ɫ����ɫֻ��0��1�������
                if(pf == 1) 
                    middle_byte = 0xff;
                else
                    middle_byte = 0;
                for(y = y1;y < y2;y++)  //�����ɫ
                {   
                    //���dst_rectÿ���м��ֽڵ���ɫ
                    for(x = byte_offset1+1; x < byte_offset2; x++)
                    {
                        dst_bitmap->bm_bits[x] = middle_byte;
                    }
                    dst_bitmap->bm_bits[byte_offset1] 
                                            &= (0xff>>bit_offset1)<<bit_offset1;
                    dst_bitmap->bm_bits[byte_offset1] 
                                            |= start_byte;  //�ֽ�ͷ��ֵ 
                    dst_bitmap->bm_bits[byte_offset2] 
                                            &= 0xff>>(bit_offset2+1);   
                    dst_bitmap->bm_bits[byte_offset2] 
                                            |= end_byte;    //�ֽ�β��ֵ 
                    //�ֽ�������
                    byte_offset1 += temp_bytes;
                    byte_offset2 += temp_bytes;
                }
            }
            else//dst_rect���е����ֽ���β�ֽ���һ���ֽ���
            {   //bit1����bit2���ң��ұ�Ϊ��λ���˴���bit_offset2=7-bit2
                for(i = 7-bit_offset2;i <= bit_offset1;i++)
                {
                    end_byte |= 1<<i;
                    start_byte |= pf<<i;
                }
                for(y = y1;y < y2;y++)
                {   //����Ŀ���ֽ��ڲ���Ҫ����λ
                    dst_bitmap->bm_bits[byte_offset1] &= ~end_byte;
                    //�����Ӧ��λ
                    dst_bitmap->bm_bits[byte_offset1] |= start_byte;
                    byte_offset1 += temp_bytes; //�ֽ�������
                }
            }
        }break;
        case 2:
        {
            pf = pf_color & 0x03;               //ȡ�����������ɫ
            byte_offset1 = y1*temp_bytes+x1/4;  //dst_rect������߽������ֽ�
            byte_offset2 = y1*temp_bytes+x2/4;  //dst_rect�����ұ߽������ֽ�
            bit_offset1 = (3-x1%4)<<1;//dst_rect��߽��������ֽڵ�λ��x=0��bit=7
            bit_offset2 = (x2%4)<<1;//7��ȥdst_rect�ұ߽��������ֽڵ�λ��7-bit
            middle_byte = (u8)((pf<<6)|(pf<<4)|(pf<<2)|pf);//ÿ���м��ֽڵ���ɫ
            //dst_rect���е����ֽ���β�ֽڲ���һ���ֽ���
            if(byte_offset1 != byte_offset2)
            {   //dst_rectÿ�е����ֽڸ�λ����ɫ
                //���ֽ�ȡ��ɫ��λ����bit��ͬ���ұ��Ǹ�λ��λ����2�ı���
                for(i = 0;i <= bit_offset1;i+=2)
                    start_byte |= pf<<i;
                //dst_rectÿ�е�β�ֽڸ�λ����ɫ�����ֽ�ȡ��ɫ��λ����7-bit��ͬ
                for(i = 0;i <= bit_offset2;i+=2)
                    end_byte |= pf<<(6-i);
                for(y = y1;y < y2;y++)  //�����ɫ
                {
                    //���dst_rectÿ���м��ֽڵ���ɫ
                    for(x = byte_offset1+1; x < byte_offset2; x++)
                    {
                        dst_bitmap->bm_bits[x] = middle_byte;
                    }
                    dst_bitmap->bm_bits[byte_offset1] 
                                            &= (0xff>>bit_offset1)<<bit_offset1;
                    dst_bitmap->bm_bits[byte_offset1] |= start_byte;//�ֽ�ͷ��ֵ 
                    dst_bitmap->bm_bits[byte_offset2] &= 0xff>>(bit_offset2+1);
                    dst_bitmap->bm_bits[byte_offset2] |= end_byte;  //�ֽ�ͷ��ֵ 
                    //�ֽ�������
                    byte_offset1 += temp_bytes;
                    byte_offset2 += temp_bytes;
                }
            }
            else//dst_rect���е����ֽ���β�ֽ���һ���ֽ���
            {   //bit1����bit2���ң��ұ�Ϊ��λ���˴���bit_offset2=7-bit2
                for(i = 6-bit_offset2;i <= bit_offset1;i = i+2)
                {
                    end_byte |= 0x3<<i;
                    start_byte |= pf<<i;
                }
                for(y = y1;y < y2;y++)
                {   //����Ŀ���ֽ��ڲ���Ҫ����λ
                    dst_bitmap->bm_bits[byte_offset1] &= ~end_byte;
                    //�����Ӧ��λ
                    dst_bitmap->bm_bits[byte_offset1] |= start_byte;
                    byte_offset1 += temp_bytes;//�ֽ�������
                }
            }
        }break;
        case 4:
        {
            pf = pf_color & 0xf;                //ȡ�����������ɫ
            byte_offset1 = y1*temp_bytes+x1/2;  //dst_rect������߽������ֽ�
            byte_offset2 = y1*temp_bytes+x2/2;  //dst_rect�����ұ߽������ֽ�
            bit_offset1 = (1-x1%2)<<2;//dst_rect��߽��������ֽڵ�λ��x=0��bit=7
            bit_offset2 = (x2%2)<<2;//7��ȥdst_rect�ұ߽��������ֽڵ�λ��7-bit
            middle_byte = (u8)((pf<<4)|pf);     //ÿ���м��ֽڵ���ɫ
            //dst_rect���е����ֽ���β�ֽڲ���һ���ֽ���
            if(byte_offset1 != byte_offset2)
            {
                if(bit_offset1 == 4)            //��߽��������ֽ���ߵ�һ������
                    start_byte = middle_byte;   //�ұ��Ǹ�λ�����ֽ������ֽ����
                else                            //��߽��������ֽ��ұߵ�һ������
                    start_byte = pf;            //�ұ��Ǹ�λ�����ֽ��ǰ��ֽ����
                if(bit_offset2 == 4)            //�ұ߽��������ֽ��ұߵ�һ������
                    start_byte = middle_byte;   //�ұ��Ǹ�λ��β�ֽ������ֽ����
                else                            //�ұ߽��������ֽ���ߵ�һ������
                    start_byte = pf<<4;         //�ұ��Ǹ�λ��β�ֽ��ǰ��ֽ����
                for(y = y1;y < y2;y++)          //�����ɫ
                {
                    //���dst_rectÿ���м��ֽڵ���ɫ
                    for(x = byte_offset1+1; x < byte_offset2; x++)
                    {
                        dst_bitmap->bm_bits[x] = middle_byte;
                    }
                    dst_bitmap->bm_bits[byte_offset1] &= 0xff<<(bit_offset1+4);
                    dst_bitmap->bm_bits[byte_offset1] |= start_byte;//�ֽ�ͷ��ֵ 
                    dst_bitmap->bm_bits[byte_offset2] &= 0xff>>(bit_offset2+4);
                    dst_bitmap->bm_bits[byte_offset2] |= end_byte;  //�ֽ�β��ֵ
                    //�ֽ�������
                    byte_offset1 += temp_bytes;
                    byte_offset2 += temp_bytes;
                }
            }
            else//dst_rect���е����ֽ���β�ֽ���һ���ֽ���
            {   //bit1����bit2���ң��ұ�Ϊ��λ���˴���bit_offset2=7-bit2
                for(i = 4-bit_offset2;i <= bit_offset1;i = i+4)
                {
                    end_byte |= 0xf<<i;
                    start_byte |= pf<<i;
                }
                for(y = y1;y < y2;y++)
                {   //����Ŀ���ֽ��ڲ���Ҫ����λ
                    dst_bitmap->bm_bits[byte_offset1] &= ~end_byte;
                    //�����Ӧ��λ
                    dst_bitmap->bm_bits[byte_offset1] |= start_byte;
                    byte_offset1 += temp_bytes;     //�ֽ�������
                }
            }
        }break;
        case 8:
        {
            pf = pf_color;                      //ȡ�����������ɫ
            byte_offset1 = y1*temp_bytes+x1;    //dst_rect������߽������ֽ�
            byte_offset2 = y1*temp_bytes+x2;    //dst_rect�����ұ߽������ֽ�
            for(y = y1;y < y2;y++)      //�����ɫ��ÿ�����һ������
            {
                //���dst_rectÿ���м��ֽڵ���ɫ
                for(x = byte_offset1;x <= byte_offset2;x++)
                {                            
                    dst_bitmap->bm_bits[x] = (u8)pf;
                }
                //�ֽ�������
                byte_offset1 += temp_bytes;
                byte_offset2 += temp_bytes;
             }                                        
        }break;
        case 12:
        {
            pf = pf_color;           //ȡ�����������ɫ
            for(y = y1;y < y2;y++)  //�����ɫ
            {
                temp_bytes = y*temp_bytes;              //y�е��ֽ�ƫ����
                for(x = x1;x < x2;x++)
                {                                        
                    byte_offset = temp_bytes + x*3/2;   //Ŀ�����������ֽ�
                    bit_offset = (x*12)%8;              //Ŀ�����ص�λƫ����
                    if(bit_offset == 0)
                    {
                        dst_bitmap->bm_bits[byte_offset] = (u8)(pf&0xff);
                        dst_bitmap->bm_bits[byte_offset+1] &= 0xf0;
                        dst_bitmap->bm_bits[byte_offset+1] |= (u8)(pf>>8);
                    }else
                    {
                        dst_bitmap->bm_bits[byte_offset] &= 0x0f;
                        dst_bitmap->bm_bits[byte_offset] |= (u8)pf<<4;
                        dst_bitmap->bm_bits[byte_offset+1] = (u8)(pf>>4);
                    }
                }
             }                                        
        }break;
        case 16:
        {
            pf = pf_color;                       //ȡ�����������ɫ
            if((u32)dst_bitmap->bm_bits & 1 )      //�Ƕ����ַ
            {
                byte_offset1 = y1*temp_bytes+x1*2;  //dst_rect������߽������ֽ�
                byte_offset2 = y1*temp_bytes+x2*2;  //dst_rect�����ұ߽������ֽ�
                for(y = y1;y < y2;y++)              //�����ɫ��ÿ�������������
                {
                    for(x = byte_offset1;x <= byte_offset2;x = x+2)
                    {                                        
    //                    alpha = pf;
    //                    if(dst_bitmap->pf_type == cn_sys_pf_a1r5g5b5)
    //                    {
    //                        pf &= 0x7fff;
    //                        pf |= alpha & 0x8000;
    //                    }
                        //ע:��������⣬���ɰ�Ŀ���ַǿ��ת����16λָ�롣
                        dst_bitmap->bm_bits[x] = (u8)pf;
                        dst_bitmap->bm_bits[x+1] = (u8)(pf>>8);
                    }
                    //�ֽ�������
                    byte_offset1 += temp_bytes;
                    byte_offset2 += temp_bytes;
                }
            }
            else                        //����ĵ�ַ
            {
                u16 *vm;
                vm = (u16*)((u32)dst_bitmap->bm_bits+y1*dst_bitmap->linebytes);
                for(y = y1;y < y2;y++)              //�����ɫ��ÿ�������������
                {
                    for(x = x1;x < x2;x++)
                    {                                        
    //                    alpha = pf;
    //                    if(dst_bitmap->pf_type == cn_sys_pf_a1r5g5b5)
    //                    {
    //                        pf &= 0x7fff;
    //                        pf |= alpha & 0x8000;
    //                    }
                        vm[x] = pf;
                    }
                    vm += dst_bitmap->linebytes>>1;
                }
                
            }
        }break;
        case 24:
        {
            pf = pf_color;                      //ȡ�����������ɫ
            byte_offset1 = y1*temp_bytes+x1*3;  //dst_rect������߽������ֽ�
            byte_offset2 = y1*temp_bytes+x2*3;  //dst_rect�����ұ߽������ֽ�
            for(y = y1;y < y2;y++)              //�����ɫ��ÿ�����һ������
            {
                for(x = byte_offset1;x <= byte_offset2;x = x+3)
                {                                                    
                    dst_bitmap->bm_bits[x] = (u8)pf;
                    dst_bitmap->bm_bits[x+1] = (u8)(pf >> 8);
                    dst_bitmap->bm_bits[x+2] = (u8)(pf >> 16);
                }
                //�ֽ�������
                byte_offset1 += temp_bytes;
                byte_offset2 += temp_bytes;
             }                                                            
        }break;
        case 32:
        {
            pf = pf_color;                      //ȡ�����������ɫ
            byte_offset1 = y1*temp_bytes+x1*4;  //dst_rect������߽������ֽ�
            byte_offset2 = y1*temp_bytes+x2*4;  //dst_rect�����ұ߽������ֽ�
            for(y = y1;y < y2;y++)              //�����ɫ��ÿ�����һ������
            {
                for(x = byte_offset1;x <= byte_offset2;x = x+4)
                {                                                                
                    pf = pf_color;
                    alpha = pf;
                    if(dst_bitmap->pf_type == cn_sys_pf_a8r8g8b8)
                    {   
                        pf &= 0x00ffffff;
                        pf |= alpha & 0xff000000;
                    }
                    //ע:��������⣬���ɰ�Ŀ���ַǿ��ת����32λָ�롣
                    dst_bitmap->bm_bits[x] = (u8)pf;
                    dst_bitmap->bm_bits[x+1] = (u8)(pf >> 8);
                    dst_bitmap->bm_bits[x+2] = (u8)(pf >> 16);
                    dst_bitmap->bm_bits[x+3] = (u8)(pf >> 24);
                }
                //�ֽ�������
                byte_offset1 += temp_bytes;
                byte_offset2 += temp_bytes;
             }                                                            
        }break; 
    }
}

//----���λͼ-----------------------------------------------------------------
//����: ��������__gk_fill_rect���������������Ҫ���ľ���������bitmap��
//����: dst_bitmap,������λͼ
//      color��������ɫ����ʽ��cn_sys_pf_e8r8g8b8
//����: ��
//-----------------------------------------------------------------------------
void __gk_fill_bm(struct st_rect_bitmap *dst_bitmap,u32 color)
{
    u32 pf_color,color_bits,alpha;
    u32 byte_offset,total_bytes;

    //color�ĸ�ʽ��cn_sys_pf_e8r8g8b8��ת����dst_bitmap->pf_typeƥ��ĸ�ʽ
    pf_color = __gk_convert_color_to_pf(dst_bitmap->pf_type,color);
    color_bits = dst_bitmap->pf_type >>8;//�����λͼ����ɫ��ʽ
    switch(color_bits)
    {
        case 1:
        {//�����ɫ��ֱ�ӽ���Ӧ��λ����
            if(1 == pf_color)
                memset( dst_bitmap->bm_bits,0xff,
                        dst_bitmap->linebytes*dst_bitmap->height);
            else
                memset( dst_bitmap->bm_bits,0,
                        dst_bitmap->linebytes*dst_bitmap->height);
        }break;
        case 2:
        {//�����ɫ��ֱ�ӽ���Ӧ��λ����
            memset( dst_bitmap->bm_bits,
                    pf_color + (pf_color<<2) + (pf_color<<4) + (pf_color<<6),
                    dst_bitmap->linebytes*dst_bitmap->height);
        }break;
        case 4:
        {//�����ɫ��ֱ�ӽ���Ӧ��λ����
            memset( dst_bitmap->bm_bits,(u8)(pf_color|(pf_color<<4)),
                    dst_bitmap->linebytes*dst_bitmap->height);
        }break;
        case 8:
        {//ֱ�ӽ���Ӧ��λ����
            memset( dst_bitmap->bm_bits,(u8)pf_color,
                    dst_bitmap->linebytes*dst_bitmap->height);
        }break;
        case 12:
        {   //�ܹ���Ҫ�����ֽ���
            total_bytes = dst_bitmap->linebytes*dst_bitmap->height;
            //12λ����ɫ��ʽ��һ�ο����3���ֽڣ�����������
            for(byte_offset = 0;byte_offset < total_bytes;)
            {     
                dst_bitmap->bm_bits[byte_offset] = (u8)(pf_color&0xff);
                dst_bitmap->bm_bits[byte_offset+1] = (u8)((pf_color>>8)&0xf);
                dst_bitmap->bm_bits[byte_offset+1] |= (u8)((pf_color&0xf)<<4);
                dst_bitmap->bm_bits[byte_offset+2] = (u8)(pf_color>>4);
                byte_offset += 3;
            }                                        
        }break;
        case 16:
        {   //�ܹ���Ҫ�����ֽ���            
            total_bytes = dst_bitmap->linebytes*dst_bitmap->height;
            if((u32)dst_bitmap->bm_bits & 1 )
            {
                for(byte_offset = 0;byte_offset < total_bytes;byte_offset += 2)
                {
    //                alpha = pf_color;
    //                if(dst_bitmap->pf_type == cn_sys_pf_a1r5g5b5)
    //                {
    //                    pf_color &= 0x7fff;
    //                    pf_color |= alpha & 0x8000;
    //                }
                    //ע:��������⣬���ɰ�Ŀ���ַǿ��ת����16λָ�롣
                    //һ��дһ�����أ������ֽ�
                    dst_bitmap->bm_bits[byte_offset] = (u8)pf_color;
                    dst_bitmap->bm_bits[byte_offset+1] = (u8)(pf_color>>8);
                }
            }
            else
            {
                u16 *vm;
                vm = (u16*)(u32)dst_bitmap->bm_bits;
                for(byte_offset = 0;byte_offset < total_bytes>>1;byte_offset++)
                {
                    vm[byte_offset] = pf_color;
                }
            }
        }break;
        case 24:
        {   //�ܹ���Ҫ�����ֽ���   
            total_bytes = dst_bitmap->linebytes*dst_bitmap->height;
            for(byte_offset = 0;byte_offset < total_bytes;)
            {
                dst_bitmap->bm_bits[byte_offset] = (u8)pf_color;
                dst_bitmap->bm_bits[byte_offset+1] = (u8)(pf_color >> 8);
                dst_bitmap->bm_bits[byte_offset+2] = (u8)(pf_color >> 16);
                byte_offset = +3;   //һ��дһ�����أ������ֽ�
             }                                                            
        }break;
        case 32:
        {   //�ܹ���Ҫ�����ֽ���
            total_bytes = dst_bitmap->linebytes*dst_bitmap->height;
            for(byte_offset = 0;byte_offset < total_bytes;)
            {
                alpha = pf_color;
                if(dst_bitmap->pf_type == cn_sys_pf_a8r8g8b8)
                {   
                    pf_color &= 0x00ffffff;
                    pf_color |= alpha & 0xff000000;
                }
                //ע:��������⣬���ɰ�Ŀ���ַǿ��ת����32λָ�롣
                dst_bitmap->bm_bits[byte_offset] = (u8)pf_color;
                dst_bitmap->bm_bits[byte_offset+1] = (u8)(pf_color >> 8);
                dst_bitmap->bm_bits[byte_offset+2] = (u8)(pf_color >> 16);
                dst_bitmap->bm_bits[byte_offset+3] = (u8)(pf_color >> 24);
                byte_offset = +4;   //һ��дһ�����أ��ĸ��ֽ�
             }                                                            
        }break; 
    }
}
//----����8�����ص�------------------------------------------------------------
//����: ����8�����ص㣬�ڻ�Բ������µ��ã�������Բ�İ˷����ԣ�
//      ͬʱ��־changed_msk��
//����: gkwin��Ŀ�괰��
//      bitmap��Ŀ��λͼ
//      x0��y0��Բ������
//      x��y��Ҫ���Ƶĵ�����꣬�����Բ��
//      color����������ɫ
//      rop2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void __gk_set_all_pixels_circle(struct gkwin_rsc *gkwin,
                                struct st_rect_bitmap *bitmap,s32 x0,s32 y0,
                                s32 x,s32 y,u32 color,u32 rop2_code)
{
    //����8�����ص�
    __gk_set_pixel_rop2_bm(bitmap,x0+x,y0+y,color,rop2_code);
    __gk_set_pixel_rop2_bm(bitmap,x0+x,y0-y,color,rop2_code);
    __gk_set_pixel_rop2_bm(bitmap,x0-x,y0+y,color,rop2_code);
    __gk_set_pixel_rop2_bm(bitmap,x0-x,y0-y,color,rop2_code);
    __gk_set_pixel_rop2_bm(bitmap,x0+y,y0+x,color,rop2_code);
    __gk_set_pixel_rop2_bm(bitmap,x0+y,y0-x,color,rop2_code);
    __gk_set_pixel_rop2_bm(bitmap,x0-y,y0+x,color,rop2_code);
    __gk_set_pixel_rop2_bm(bitmap,x0-y,y0-x,color,rop2_code);
    //��8�����ص�ֱ����changed_msk��־
    __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),(x0+x)/8,(y0+y)/8,
                                cn_color_white,cn_r2_copypen);
    __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),(x0+x)/8,(y0-y)/8,
                                cn_color_white,cn_r2_copypen);
    __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),(x0-x)/8,(y0+y)/8,
                                cn_color_white,cn_r2_copypen);
    __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),(x0-x)/8,(y0-y)/8,
                                cn_color_white,cn_r2_copypen);
    __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),(x0+y)/8,(y0+x)/8,
                                cn_color_white,cn_r2_copypen);
    __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),(x0+y)/8,(y0-x)/8,
                                cn_color_white,cn_r2_copypen);
    __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),(x0-y)/8,(y0+x)/8,
                                cn_color_white,cn_r2_copypen);
    __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),(x0-y)/8,(y0-x)/8,
                                cn_color_white,cn_r2_copypen);
}
//----����Ŀ��㲢��ɫ---------------------------------------------------------
//����: ����Ŀ��㣬ͬʱ��־changed_msk��
//����: gkwin��Ŀ�괰��
//      limit�����Ƶ���������ֻ����ֱ����limit�����ڲ��Ĳ���
//      bitmap��Ŀ��λͼ
//      x��y��Ҫ���Ƶĵ������
//      color����������ɫ
//      rop2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void __gk_pixel_sect_inter(struct gkwin_rsc *gkwin,struct st_rect *limit,
                                struct st_rect_bitmap *bitmap,s32 x,s32 y,
                                u32 color,u32 rop2_code)
{
    if((x >= limit->left)&&(x < limit->right)
        &&(y >= limit->top)&&(y < limit->bottom))
    {//�����Ƶ�Ŀ������������ڣ����㲢��־changed_msk
        __gk_set_pixel_rop2_bm(bitmap,x,y,color,rop2_code);
        __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),x/8,y/8,
                                cn_color_white,cn_r2_copypen);
    }
    else    //�����Ƶ�Ŀ������������⣬ֱ�ӷ���
        return;
}
//----���ƶ��Ŀ��㲢��־-----------------------------------------------------
//����: ���ƶ��Ŀ��㣬ͬʱ��־changed_msk��
//����: gkwin��Ŀ�괰��
//      limit�����Ƶ���������ֻ����ֱ����limit�����ڲ��Ĳ���
//      bitmap��Ŀ��λͼ
//      x0��y0��Բ������
//      x��y��Ҫ���Ƶĵ������
//      color����������ɫ
//      rop2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void __gk_set_part_pixels_circle(struct gkwin_rsc *gkwin,struct st_rect *limit,
                                struct st_rect_bitmap *bitmap,s32 x0,s32 y0,
                                s32 x,s32 y,u32 color,u32 rop2_code)
{
    __gk_pixel_sect_inter(gkwin,limit,bitmap,x0+x,y0+y,color,rop2_code);
    __gk_pixel_sect_inter(gkwin,limit,bitmap,x0+x,y0-y,color,rop2_code);
    __gk_pixel_sect_inter(gkwin,limit,bitmap,x0-x,y0+y,color,rop2_code);
    __gk_pixel_sect_inter(gkwin,limit,bitmap,x0-x,y0-y,color,rop2_code);
    __gk_pixel_sect_inter(gkwin,limit,bitmap,x0+y,y0+x,color,rop2_code);
    __gk_pixel_sect_inter(gkwin,limit,bitmap,x0+y,y0-x,color,rop2_code);
    __gk_pixel_sect_inter(gkwin,limit,bitmap,x0-y,y0+x,color,rop2_code);
    __gk_pixel_sect_inter(gkwin,limit,bitmap,x0-y,y0-x,color,rop2_code);
} 
//----��Բ---------------------------------------------------------------------
//����: �ڴ����ڻ�һ��Բ��
//����: gkwin��Ŀ�괰��
//      limit�����Ƶ���������ֻ����ֱ����limit�����ڲ��Ĳ���
//      x0��y0��Բ������
//      r��Բ�İ뾶
//      color����������ɫ
//      rop2_code����Ԫ��դ������
//����: ��
//˵��: ʹ���е㻭Բ�㷨
//-----------------------------------------------------------------------------
void __gk_draw_circle_bm(struct gkwin_rsc *gkwin,struct st_rect *limit,
                        s32 x0,s32 y0,s32 r,u32 color,u32 rop2_code)
{
    s32 x,y,e;
    s32 left,right,top,bottom;
    u32 pf_color,flag;
    struct st_rect_bitmap *bitmap;

    //ѡȡ(x,y)Ϊ(0,r)��Ϊ��ʼ����(����ʵ�ʵ�Բ��ƽ�Ƶ�ԭ���Բ)��Բ�ϵ������    
    x = 0;
    y = r;    
    e = 1-r;
    left = limit->left-x0;      //������߽絽Բ��x����ľ���
    top = limit->top-y0;        //������߽絽Բ��y����ľ���
    right = limit->right-x0;    //�����ұ߽絽Բ��x����ľ���
    bottom = limit->bottom-y0;  //�����ұ߽絽Բ��y����ľ���
    bitmap = &gkwin->wm_bitmap;
    //Ҫ���������ɫ����ɫ��ʽΪ24λ
    //��������ǰ�轫24λ����Ϊ����ʾ��һ�µ���ɫ��ʽ
    pf_color = __gk_convert_color_to_pf(gkwin->disp->pixel_format,color);

    //����Բ����limit��
    if((x0-limit->left >= r)&&(limit->right-x0 > r)
        &&(y0-limit->top >= r)&&(limit->bottom-y0 > r))
        flag = 1;
    //Բ�ϵĵ㲻����limit�ڣ�������Բ�ڴ�����Ͳ����ڴ������������
    else if((left*left+top*top < r*r)
        &&(right*right+top*top < r*r)
        &&(left*left+bottom*bottom < r*r)
        &&(right*right+bottom*bottom < r*r))
        return;
    //Բ�ϵĵ㲿����limit��
    else
        flag = 0;
    if(flag)
    {//����Բ����limit�ڣ�������ķ�֮һ��Բ������
     //���ݶԳ��ԣ��ɵ�Բ���������е������
        while(x <= y)
        {
            //�����жϣ�����8�����أ�ͬʱ��־���ص�changed_msk
            __gk_set_all_pixels_circle(gkwin,bitmap,x0,y0,x,y,
                                                pf_color,rop2_code);
            if(e < 0)
                e += x*2+3;
            else
            {
                e += 2*(x-y)+5;
                y--;
            } 
            x++;
        }
    }
    else
    {//Բ�ϲ��ֵ���limit�ڣ���Ҫ��Բ�ϸ������Ƿ���limit�������ж�
        while(x <= y)
        {
            //�ж�Ҫ���Ƶ��λ�ú󣬾����Ƿ���ƣ����Ƶ�����ͬʱ��־changed_msk
            __gk_set_part_pixels_circle(gkwin,limit,bitmap,x0,y0,x,y,
                                                pf_color,rop2_code);
            if(e < 0)
                e += x*2+3;
            else
            {
                e += 2*(x-y)+5;
                y--;
            } 
            x++;
        }
    }
    if(gkwin->change_flag != cn_gkwin_change_all)
        //��һ��Բֻ�ı��˴��ڵĲ�������
        gkwin->change_flag = cn_gkwin_change_part;
}
//----Ŀ���Ļ���-------------------------------------------------------------
//����: ��Ŀ������ڵ�λ�ã������Ƿ���Ҫ���ơ�
//����: gkwin��Ŀ�괰��
//      limit�����Ƶ���������ֻ����ֱ����limit�����ڲ��Ĳ���
//      x��y��Ŀ��������
//      color����Բʹ�õ���ɫ
//      rop2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void __gk_pixel_sect_inter_screen(struct display_rsc *display,
                                    struct st_rect *limit,
                                    s32 x,s32 y,u32 color,u32 rop2_code)
{
    if((x >= limit->left)&&(x < limit->right)
        &&(y >= limit->top)&&(y < limit->bottom))
    //Ŀ�����limit�ڣ���Ҫ����
        display->draw.set_pixel_screen(x,y,color,rop2_code);
    else
        return;
}

//----��screen�ϻ�Բ-----------------------------------------------------------
//����: ��screen�ϻ�һ��Բ��
//����: gkwin��Ŀ�괰��
//      limit�����Ƶ���������ֻ����ֱ����limit�����ڲ��Ĳ���
//      x0��y0��Բ������
//      r��Բ�İ뾶
//      color����Բʹ�õ���ɫ
//      rop2_code����Ԫ��դ������
//����: ��
//ʹ���е㻭Բ�㷨
//-----------------------------------------------------------------------------
void __gk_draw_circle_screen(struct display_rsc *display,struct st_rect *limit,
                        s32 x0,s32 y0,s32 r,u32 color,u32 rop2_code)//ȷ��
{
    s32 x,y,e;
    s32 left,right,top,bottom;
    u32 pf_color,flag;
    struct display_rsc *mirror,*current;
    struct disp_draw *mydraw;

    mydraw = &display->draw;
    //ѡȡ(x,y)Ϊ(0,r)��Ϊ��ʼ����(����ʵ�ʵ�Բ��ƽ�Ƶ�ԭ���Բ)��Բ�ϵ������        
    x = 0;
    y = r;
    e = 1-r;
    left = x0 - limit->left;
    top = y0 - limit->top;
    right = limit->right-x0;
    bottom = limit->bottom-y0;
    pf_color = __gk_convert_color_to_pf(display->pixel_format,color);

    //����Բ����limit��
    if((x0-limit->left >= r)&&(limit->right-x0 > r)
        &&(y0-limit->top >= r)&&(limit->bottom-y0 > r))
        flag = 1;
    
    //Բ�ϵĵ㶼����limit��
    else if((left*left+top*top < r*r)
        &&(right*right+top*top < r*r)
        &&(left*left+bottom*bottom < r*r)
        &&(right*right+bottom*bottom < r*r))
        return;
    //Բ�ϵĵ㲿����limit��
    else
        flag = 0;

    if(flag) 
    {//����Բ����limit�ڣ�����Բ�İ˷����ԣ�ֻ��Ҫ����˷�֮һ��Բ������
        while(x <= y)
        {
            mydraw->set_pixel_screen(x0+x,y0+y,color,rop2_code);
            mydraw->set_pixel_screen(x0+x,y0-y,color,rop2_code);
            mydraw->set_pixel_screen(x0-x,y0+y,color,rop2_code);
            mydraw->set_pixel_screen(x0-x,y0-y,color,rop2_code);
            mydraw->set_pixel_screen(x0+y,y0+x,color,rop2_code);
            mydraw->set_pixel_screen(x0+y,y0-x,color,rop2_code);
            mydraw->set_pixel_screen(x0-y,y0+x,color,rop2_code);
            mydraw->set_pixel_screen(x0-y,y0-x,color,rop2_code);
            if(e < 0)
                e += x*2+3;
            else
            {
                e += 2*(x-y)+5;
                y--;
            } 
            x++;
        }
    }
    else
    {//Բ�ϵĵ㲿����limit�ڣ���Ҫ����Բ�ϸ������Ƿ���limit��
        while(x <= y)
        {
            __gk_pixel_sect_inter_screen(display,limit,
                                x0+x,y0+y,color,rop2_code);
            __gk_pixel_sect_inter_screen(display,limit,
                                x0+x,y0-y,color,rop2_code);
            __gk_pixel_sect_inter_screen(display,limit,
                                x0-x,y0+y,color,rop2_code);
            __gk_pixel_sect_inter_screen(display,limit,
                                x0-x,y0-y,color,rop2_code);
            __gk_pixel_sect_inter_screen(display,limit,
                                x0+y,y0+x,color,rop2_code);
            __gk_pixel_sect_inter_screen(display,limit,
                                x0+y,y0-x,color,rop2_code);
            __gk_pixel_sect_inter_screen(display,limit,
                                x0-y,y0+x,color,rop2_code);
            __gk_pixel_sect_inter_screen(display,limit,
                                x0-y,y0-x,color,rop2_code);
            if(e < 0)
                e += x*2+3;
            else
            {
                e += 2*(x-y)+5;
                y--;
            } 
            x++;
        }
    }
    //������ʾ
    mirror = (struct display_rsc *)display->node.child;
    if(mirror == NULL)      //û�о�����ʾ����ֱ�ӷ���
        return;
    current = mirror;
    while(current != NULL)
    {//���ھ�����ʾ��
        if(flag) 
        {//����Բ����limit�ڣ�����Բ�İ˷����ԣ�ֻ��Ҫ����˷�֮һ��Բ������
            while(x <= y)
            {
                mydraw->set_pixel_screen(x0+x,y0+y,color,rop2_code);
                mydraw->set_pixel_screen(x0+x,y0-y,color,rop2_code);
                mydraw->set_pixel_screen(x0-x,y0+y,color,rop2_code);
                mydraw->set_pixel_screen(x0-x,y0-y,color,rop2_code);
                mydraw->set_pixel_screen(x0+y,y0+x,color,rop2_code);
                mydraw->set_pixel_screen(x0+y,y0-x,color,rop2_code);
                mydraw->set_pixel_screen(x0-y,y0+x,color,rop2_code);
                mydraw->set_pixel_screen(x0-y,y0-x,color,rop2_code);
                if(e < 0)
                    e += x*2+3;
                else
                {
                    e += 2*(x-y)+5;
                    y--;
                } 
                x++;
            }
        }
        else
        {//Բ�ϵĵ㲿����limit�ڣ���Ҫ����Բ�ϸ������Ƿ���limit��
            while(x <= y)
            {
                //ֻ������limit�ڵ�����
                if((x0+x >= limit->left)&&(x0+x < limit->right)
                        &&(y0+y >= limit->top)&&(y0+y < limit->bottom))
                    current->draw.set_pixel_screen(x0+x,y0+y,color,rop2_code);
                if((x0+x >= limit->left)&&(x0+x < limit->right)
                        &&(y0-y >= limit->top)&&(y0+y < limit->bottom))
                    current->draw.set_pixel_screen(x0+x,y0-y,color,rop2_code);
                if((x0-x >= limit->left)&&(x0-x < limit->right)
                        &&(y0+y >= limit->top)&&(y0+y < limit->bottom))
                    current->draw.set_pixel_screen(x0-x,y0+y,color,rop2_code);
                if((x0-x >= limit->left)&&(x0-x < limit->right)
                        &&(y0-y >= limit->top)&&(y0-y < limit->bottom))
                    current->draw.set_pixel_screen(x0-x,y0-y,color,rop2_code);
                if((x0+y >= limit->left)&&(x0+y < limit->right)
                        &&(y0+x >= limit->top)&&(y0+x < limit->bottom))
                    current->draw.set_pixel_screen(x0+y,y0+x,color,rop2_code);
                if((x0+y >= limit->left)&&(x0+y < limit->right)
                        &&(y0-x >= limit->top)&&(y0-x < limit->bottom))
                    current->draw.set_pixel_screen(x0+y,y0-x,color,rop2_code);
                if((x0-y >= limit->left)&&(x0-y < limit->right)
                        &&(y0+x >= limit->top)&&(y0+x < limit->bottom))
                    current->draw.set_pixel_screen(x0-y,y0+x,color,rop2_code);
                if((x0-y >= limit->left)&&(x0-y < limit->right)
                        &&(y0-x >= limit->top)&&(y0-x < limit->bottom))
                    current->draw.set_pixel_screen(x0-y,y0-x,color,rop2_code);
                if(e < 0)
                    e += x*2+3;
                else
                {
                    e += 2*(x-y)+5;
                    y--;
                } 
                x++;
            }
        }
        current = (struct display_rsc *)rsc_trave_scion(&mirror->node,
                &current->node);
    } 
}
//----��Բ---------------------------------------------------------------------
//����: �ڴ����ڻ�һ��Բ��
//����: gkwin��Ŀ�괰��
//      x0��y0��Բ�����꣬����ڴ���
//      r��Բ�İ뾶
//      color����Բʹ�õ���ɫ
//      rop2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void gk_draw_circle(struct gksc_para_draw_circle *para)//ȷ��
{
    struct st_rect limit;
    struct clip_rect *clip;
    struct gkwin_rsc *fb_gkwin;
    struct disp_draw *my_draw_fun;
    s32 offsetx,offsety;
    if((para->rop2_code == cn_r2_nop) || (para->rop2_code > cn_r2_last))
        return;                                         //��ִ�в���
    if(para->r <= 0)                                         //�������
        return;
    my_draw_fun = &para->gkwin->disp->draw;
    //˵����win buffer����ֱ��д������Ϊfalse
    if((para->gkwin->wm_bitmap.bm_bits != NULL)
        && (para->gkwin->direct_screen ==false))     
    {   //������:��win buffer�л�ͼ����־changed_msk
        limit.left = 0;
        limit.top = 0;
        limit.right = para->gkwin->wm_bitmap.width;
        limit.bottom = para->gkwin->wm_bitmap.height;
        //��Ӳ�����ٲ�֧��Բ�Ļ��ƿ��ǣ������ʵ��
        __gk_draw_circle_bm(para->gkwin,&limit,para->x0,para->y0,para->r,
                                    para->color,para->rop2_code);
    }else
    {
        clip = para->gkwin->visible_clip;
        if(clip == NULL)
            return ;
        fb_gkwin = para->gkwin->disp->frame_buffer;
        offsetx = para->gkwin->absx0;
        offsety = para->gkwin->absy0;
        //��frame buffer����ֱ��д������Ϊfalse
        if((fb_gkwin != NULL) && (para->gkwin->direct_screen == false))     
        {   //������:��frame buffer�л�ͼ����ֻ��gkwin�еĿ�������
            do{
                //��Ӳ�����ٲ�֧��Բ�Ļ��ƿ��ǣ������ʵ��
                __gk_draw_circle_bm(fb_gkwin,&clip->rect,
                                    para->x0+offsetx,para->y0+offsety,
                                    para->r,para->color,para->rop2_code);
                clip = clip->next;
            }while(clip != para->gkwin->visible_clip);
        }else       
        //��win buffer��Ҳ��frame buffer��ֱ�ӻ���screen��
        //ֱ��д������Ϊtrue���������޻���������ֱ�ӻ���screen��
        {
            do{
                __gk_draw_circle_screen(para->gkwin->disp,&clip->rect,
                            para->x0+offsetx,para->y0+offsety,para->r,
                            para->color,para->rop2_code);
                clip = clip->next;
            }while(clip != para->gkwin->visible_clip);
        }
    }
}

//----������������-------------------------------------------------------------
//����: �ڴ����ϻ����������ߡ�
//����: gkwin��Ŀ�괰��
//      limit�������ͼ����������һ�����ε����򣬳����������ֱ�߲���
//      x1��y1��x2��y2��x3��y3��x4��y4���˵�����
//      color����ͼ���õ���ɫ
//      rop2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void __gk_Bezier_bm(struct gkwin_rsc *gkwin,struct st_rect *limit,
                    float x1,float y1,float x2,float y2,float x3,float y3,
                    float x4,float y4,u32 color,u32 rop2_code)
{
    float a,b,c;
    float x12,y12,x23,y23,x34,y34;
    float x1223,y1223,x2334,y2334;
    float x,y,ab;

    //a��b��c��ֱ��(x1,y1)��(x4,y4)��һ��ʽax+by+c=0ʱ��Ӧ�ĸ�������
    a = y4-y1;
    b = x1-x4;
    c = y1*(x4-x1)-x1*(y4-y1);
    ab = a*a+b*b;
    
    //��(x2,y2)��(x3,y3)��ֱ��(x1,y1)��(x4,y4)�ľ���С��1(������)��
    //����ֱ�ߴ�������
    if(((a*x2+b*y2+c)*(a*x2+b*y2+c) < ab) &&((a*x3+b*y3+c)*(a*x3+b*y3+c) < ab))
    {
        __gk_lineto_bm(gkwin,limit,x1,y1,x4,y4,
                                color,rop2_code);
        return;
    }
    x12 = x1+x2;
    y12 = y1+y2;
    x23 = x2+x3;
    y23 = y2+y3;
    x34 = x3+x4;
    y34 = y3+y4;
    
    x1223 = x12+x23;
    y1223 = y12+y23;
    x2334 = x23+x34;
    y2334 = y23+y34;
    
    x = x1223+x2334;
    y = y1223+y2334;

    //�������м��������Ƶ㵽�Զ˵�Ϊ���յ��ֱ�ߵľ���С��1(������)ʱ��
    //�����ߵ��е㴦�ָ�����
    __gk_Bezier_bm(gkwin,limit,x1,y1,x12/2,y12/2,
                    x1223/4,y1223/4,x/8,y/8,color,rop2_code);
    __gk_Bezier_bm(gkwin,limit,x/8,y/8,x2334/4,y2334/4,
                    x34/2,y34/2,x4,y4,color,rop2_code);
}

//----��screen�ϻ�����������---------------------------------------------------
//����: ��screen�ϻ����������ߡ�
//����: gkwin��Ŀ�괰��
//      limit�������ͼ����������һ�����ε����򣬳����������ֱ�߲���
//      x1��y1��x2��y2��x3��y3��x4��y4���˵�����
//      color����ͼ���õ���ɫ
//      rop2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void __gk_Bezier_screen(struct display_rsc *display,struct st_rect *limit,
                    float x1,float y1,float x2,float y2,float x3,float y3,
                    float x4,float y4,u32 color,u32 rop2_code)
{
    float a,b,c;
    float x12,y12,x23,y23,x34,y34;
    float x1223,y1223,x2334,y2334;
    float x,y,ab;

    //a��b��c��ֱ��(x1,y1)��(x4,y4)��һ��ʽax+by+c=0ʱ��Ӧ�ĸ�������    
    a = y4-y1;
    b = x1-x4;
    c = y1*(x4-x1)-x1*(y4-y1);
    ab = a*a+b*b;

    //��(x2,y2)��(x3,y3)��ֱ��(x1,y1)��(x4,y4)�ľ���С��1(������)��
    //����ֱ�ߴ�������
    if(((a*x2+b*y2+c)*(a*x2+b*y2+c) < ab) &&((a*x3+b*y3+c)*(a*x3+b*y3+c) < ab))
    {
        __gk_lineto_screen(display,limit,x1,y1,x4,y4,color,rop2_code);
        return;
    }
    x12 = x1+x2;
    y12 = y1+y2;
    x23 = x2+x3;
    y23 = y2+y3;
    x34 = x3+x4;
    y34 = y3+y4;
    
    x1223 = x12+x23;
    y1223 = y12+y23;
    x2334 = x23+x34;
    y2334 = y23+y34;
    
    x = x1223+x2334;
    y = y1223+y2334;

    //�������м��������Ƶ㵽�Զ˵�Ϊ���յ��ֱ�ߵľ���С��1(������)ʱ��
    //�����ߵ��е㴦�ָ�����
    __gk_Bezier_screen(display,limit,x1,y1,x12/2,y12/2,
                    x1223/4,y1223/4,x/8,y/8,color,rop2_code);
    __gk_Bezier_screen(display,limit,x/8,y/8,x2334/4,y2334/4,
                    x34/2,y34/2,x4,y4,color,rop2_code);
}
//----������������-------------------------------------------------------------
//����: �ڴ����ڻ�һ�����������ߡ�
//����: �����ǽṹpara�ĳ�Ա
//      gkwin��Ŀ�괰��ָ��
//      x1��y1��x2��y2��x3��y3��x4��y4������Bezier���ߵ��ĸ����Ƶ�
//      color������ʹ�õ���ɫ
//      r2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void gk_Bezier(struct gksc_para_Bezier *para)//ȷ��
{
    s32 offsetx,offsety;
    struct st_rect limit;
    struct clip_rect *clip;
    struct gkwin_rsc *fb_gkwin;
    struct disp_draw *my_draw_fun;
    if((para->rop2_code == cn_r2_nop) || (para->rop2_code > cn_r2_last))
        return;                                         //��ִ�в���
    my_draw_fun = &para->gkwin->disp->draw;
    //˵����win buffer����ֱ��д������Ϊfalse
    if((para->gkwin->wm_bitmap.bm_bits != NULL)
        && (para->gkwin->direct_screen == false))     
    {   //������:��win buffer�л�ͼ����־changed_msk
        limit.left = 0;
        limit.top = 0;
        limit.right = para->gkwin->wm_bitmap.width;
        limit.bottom = para->gkwin->wm_bitmap.height;
        //��Ӳ�����ٲ�֧�ֱ��������ߵĻ��ƿ��ǣ������ʵ��
        __gk_Bezier_bm(para->gkwin,&limit,para->x1,para->y1,para->x2,para->y2,
                            para->x3,para->y3,para->x4,para->y4,
                            para->color,para->rop2_code);
    }else
    {
        clip = para->gkwin->visible_clip;
        if(clip == NULL)
            return ;
        fb_gkwin = para->gkwin->disp->frame_buffer;
        offsetx = para->gkwin->absx0;
        offsety = para->gkwin->absy0;
        //��frame buffer����ֱ��д������Ϊfalse
        if((fb_gkwin != NULL) && (para->gkwin->direct_screen == false))     
        {   //������:��frame buffer�л�ͼ����ֻ��gkwin�еĿ�������
            do{
                //��Ӳ�����ٲ�֧�ֱ��������ߵĻ��ƿ��ǣ������ʵ��
                __gk_Bezier_bm(fb_gkwin,&clip->rect,para->x1+offsetx,
                            para->y1+offsety,para->x2+offsetx,para->y2+offsety,
                            para->x3+offsetx,para->y3+offsety,para->x4+offsetx,
                            para->y4+offsety,para->color,para->rop2_code);
                clip = clip->next;
            }while(clip != para->gkwin->visible_clip);
        }else       
        //��win buffer��Ҳ��frame buffer��ֱ�ӻ���screen��
        //ֱ��д������Ϊtrue���������޻�������ֱ�ӻ���screen��
        {
            do{
                __gk_Bezier_screen(para->gkwin->disp,&clip->rect,para->x1+offsetx,
                            para->y1+offsety,para->x2+offsetx,para->y2+offsety,
                            para->x3+offsetx,para->y3+offsety,para->x4+offsetx,
                            para->y4+offsety,para->color,para->rop2_code);
                clip = clip->next;
            }while(clip != para->gkwin->visible_clip);
        }
    }
}
//----��ֱ��-------------------------------------------------------------------
//����: �ڴ����ڻ�һ��ֱ�ߡ�
//����: �����ǽṹpara�ĳ�Ա
//      gkwin��Ŀ�괰��ָ��
//      x1��y1�������������
//      x2��y2�������յ�����(������)
//      color������ʹ�õ���ɫ
//      r2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void gk_lineto(struct gksc_para_lineto *para)//ȷ��
{
    struct st_rect limit;
    struct clip_rect *clip;
    struct gkwin_rsc *fb_gkwin;
    struct disp_draw *my_draw_fun;
    s32 offsetx,offsety;
    if((para->rop2_code == cn_r2_nop) || (para->rop2_code > cn_r2_last))
        return;                                         //��ִ�в���
    if((para->x1==para->x2) && (para->y1==para->y2))    //�������
        return;
    my_draw_fun = &para->gkwin->disp->draw;
    //˵����win buffer����ֱ��д������Ϊfalse
    if((para->gkwin->wm_bitmap.bm_bits != NULL)
        && (para->gkwin->direct_screen == false))     
    {   //������:��win buffer�л�ͼ����־changed_msk
        limit.left = 0;
        limit.top = 0;
        limit.right = para->gkwin->wm_bitmap.width;
        limit.bottom = para->gkwin->wm_bitmap.height;
        if(!my_draw_fun->line_bm(&para->gkwin->wm_bitmap,&limit,para->x1,
                    para->y1,para->x2,para->y2,para->color,para->rop2_code))
        {    //Ӳ�����ٲ�֧��ֱ�߻��ƣ��������ʵ�֣���������㷨��
            //ͬʱ������changed_msk
            __gk_lineto_bm(para->gkwin,&limit,para->x1,para->y1,para->x2,
                                    para->y2,para->color,para->rop2_code);
        }
        else    //Ӳ�����ٻ�ͼʱ��δ����changed_msk������֮
        {  
            __gk_shading_line(para->gkwin,&limit,
                                para->x1,para->y1,para->x2,para->y2);
        }
    }
    else
    {
        clip = para->gkwin->visible_clip;
        if(clip == NULL)
            return ;
        fb_gkwin = para->gkwin->disp->frame_buffer;
        offsetx = para->gkwin->absx0;
        offsety = para->gkwin->absy0;
        //��frame buffer,��ֱ��д������Ϊfalse
        if((fb_gkwin != NULL) && (para->gkwin->direct_screen == false))     
        {   //������:��frame buffer�л�ͼ����ֻ��gkwin�еĿ�������
            do{
                if(!my_draw_fun->line_bm(
                            &fb_gkwin->wm_bitmap,&clip->rect,
                            para->x1+offsetx,para->y1+offsety,para->x2+offsetx,
                            para->y2+offsety,para->color,para->rop2_code))
                {    //Ӳ�����ٲ�֧��ֱ�߻��ƣ��������ʵ�֣���������㷨��
                    //ͬʱ������changed_msk
                    __gk_lineto_bm(fb_gkwin,&clip->rect,
                                    para->x1+offsetx,para->y1+offsety,
                                    para->x2+offsetx,para->y2+offsety,
                                    para->color,para->rop2_code);
                }
                else    //Ӳ�����ٻ�ͼʱ��δ����changed_msk������֮
                { 
                    __gk_shading_line(fb_gkwin,&clip->rect,para->x1+offsetx,
                                      para->y1+offsety,para->x2+offsetx,
                                      para->y2+offsety);
                }
                clip = clip->next;
            }while(clip != para->gkwin->visible_clip);
        }
        else       
        //��win buffer��Ҳ��frame buffer��ֱ�ӻ���screen��
        //ֱ��д������Ϊtrue���������޻���������ֱ�ӻ���screen��
        {
            do{//��screen��ֱ�ӻ�ֱ�ߣ������յ�
            	if(!my_draw_fun->line_screen(&clip->rect,para->x1+offsetx,
                         	para->y1+offsety,para->x2+offsetx,para->y2+offsety,
                         	para->color,para->rop2_code))
                {
                	__gk_lineto_screen(para->gkwin->disp,&clip->rect,
                            para->x1+offsetx,para->y1+offsety,para->x2+offsetx,
                            para->y2+offsety,para->color,para->rop2_code);
                }
                clip = clip->next;
            }while(clip != para->gkwin->visible_clip);
        }
    }
}
//----��ֱ��-------------------------------------------------------------------
//����: �ڴ����ڻ�һ��ֱ�ߡ�
//����: gkwin��Ŀ�괰��ָ��
//      gkwin��Ŀ�괰��ָ��
//      x1��y1�������������
//      x2��y2�������յ�����(����)
//      color������ʹ�õ���ɫ
//      r2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void gk_lineto_ie(struct gksc_para_lineto *para)//ȷ��
{
	s32 dx,dy;
    struct st_rect limit;
    struct clip_rect *clip;
    struct gkwin_rsc *fb_gkwin;
    struct disp_draw *my_draw_fun;
    struct display_rsc *display;
    s32 offsetx,offsety;
    if((para->rop2_code == cn_r2_nop) || (para->rop2_code > cn_r2_last))
        return;                                         //��ִ�в���
    if((para->x1==para->x2) && (para->y1==para->y2))    //�������
        return;
    display = para->gkwin->disp;
    my_draw_fun = &display->draw;
    if(para->y1 == para->y2)    //����ˮƽ��
    {
        //˵����win buffer����ֱ��д������Ϊfalse
        if((para->gkwin->wm_bitmap.bm_bits != NULL)
            && (para->gkwin->direct_screen == false))    
        {   //������:��win buffer�л�ͼ����־changed_msk
            limit.left = 0;
            limit.top = 0;
            limit.right = para->gkwin->wm_bitmap.width;
            limit.bottom = para->gkwin->wm_bitmap.height;
            if(!my_draw_fun->line_bm_ie(&para->gkwin->wm_bitmap,&limit,para->x1,
                        para->y1,para->x2,para->y2,para->color,para->rop2_code))
            {    //Ӳ�����ٲ�֧��ֱ�߻��ƣ��������ʵ�֣���������㷨��
                //ͬʱ������changed_msk
            	__gk_hlineto_bm(para->gkwin,&limit,para->x1,para->y1,
                            	para->x2+1,para->color,para->rop2_code);
            }
            else
            {   //Ӳ������֧��ֱ�߻��ƣ�δ����changed_msk������֮
            	__gk_shading_line(para->gkwin,&limit,
                            	para->x1,para->y1,para->x2+1,para->y2);
            }
            
        }
        else
        {
            clip = para->gkwin->visible_clip;
            if(clip == NULL)
                return ;
            fb_gkwin = display->frame_buffer;
            offsetx = para->gkwin->absx0;
            offsety = para->gkwin->absy0;
            //��frame buffer����ֱ��д������Ϊfalse
            if((fb_gkwin != NULL) && (para->gkwin->direct_screen == false))     
            {   //������:��frame buffer�л�ͼ����ֻ��gkwin�еĿ�������
                do
                {
                    if(!my_draw_fun->line_bm_ie(
                                &fb_gkwin->wm_bitmap,&clip->rect,
                                para->x1+offsetx,para->y1+offsety,
                                para->x2+offsetx,para->y2+offsety,
                                para->color,para->rop2_code))
                    {    //Ӳ�����ٲ�֧��ֱ�߻��ƣ��������ʵ�֣���������㷨��
                        //ͬʱ������changed_msk
                    	__gk_hlineto_bm(fb_gkwin,&clip->rect,
                                	para->x1+offsetx,para->y1+offsety,
                                    para->x2+offsetx+1,
                                    para->color,para->rop2_code);
                    }
                	else
                    {   //Ӳ�����ٻ�ͼʱ��δ����changed_msk������֮
                    	__gk_shading_line(fb_gkwin,&clip->rect,
                                    	para->x1+offsetx,para->y1+offsety,
                                    	para->x2+offsetx+1,para->y2+offsety);
                    }
                    clip = clip->next;
                }while(clip != para->gkwin->visible_clip);
            }
            else       
            //��win buffer��Ҳ��frame buffer��ֱ�ӻ���screen��
            //ֱ��д������Ϊtrue���������޻���������ֱ�ӻ���screen��
            {
                do{//��screen��ֱ�ӻ�ֱ�ߣ����յ�
                	if(!my_draw_fun->line_screen_ie(&clip->rect,
                              	para->x1+offsetx,para->y1+offsety,
                              	para->x2+offsetx,para->y2+offsety,
                             	para->color,para->rop2_code))
                    {
                    	__gk_lineto_screen(display,&clip->rect,
                                    	para->x1+offsetx,para->y1+offsety,
                                    	para->x2+offsetx+1,para->y2+offsety,
                                    	para->color,para->rop2_code);
                    }           
                    clip = clip->next;
                }while(clip != para->gkwin->visible_clip);
            }
        }    
    }
	else if(para->x1 == para->x2)    //���ƴ�ֱ��
    {
        //˵����win buffer����ֱ��д������Ϊfalse
        if((para->gkwin->wm_bitmap.bm_bits != NULL)
            && (para->gkwin->direct_screen == false))    
        {   //������:��win buffer�л�ͼ����־changed_msk
            limit.left = 0;
            limit.top = 0;
            limit.right = para->gkwin->wm_bitmap.width;
            limit.bottom = para->gkwin->wm_bitmap.height;
            if(!my_draw_fun->line_bm_ie(&para->gkwin->wm_bitmap,&limit,para->x1,
                        para->y1,para->x2,para->y2,para->color,para->rop2_code))
            {    //Ӳ�����ٲ�֧��ֱ�߻��ƣ��������ʵ�֣���������㷨��
                //ͬʱ������changed_msk
            	__gk_vlineto_bm(para->gkwin,&limit,para->x1,
                        	para->y1,para->y2+1,para->color,para->rop2_code);
            }
            else
            {   //Ӳ������֧��ֱ�߻��ƣ�δ����changed_msk������֮
            	__gk_shading_line(para->gkwin,&limit,
                            	para->x1,para->y1,para->x2,para->y2+1);
            }            
        }
        else
        {
            clip = para->gkwin->visible_clip;
            if(clip == NULL)
                return ;
            fb_gkwin = display->frame_buffer;
            offsetx = para->gkwin->absx0;
            offsety = para->gkwin->absy0;
            //��frame buffer����ֱ��д������Ϊfalse
            if((fb_gkwin != NULL) && (para->gkwin->direct_screen == false))     
            {   //������:��frame buffer�л�ͼ����ֻ��gkwin�еĿ�������
                do
                {
                    if(!my_draw_fun->line_bm_ie(
                                &fb_gkwin->wm_bitmap,&clip->rect,
                                para->x1+offsetx,para->y1+offsety,
                                para->x2+offsetx,para->y2+offsety,
                                para->color,para->rop2_code))
                    {    //Ӳ�����ٲ�֧��ֱ�߻��ƣ��������ʵ�֣���������㷨��
                        //ͬʱ������changed_msk
                    	__gk_vlineto_bm(fb_gkwin,&clip->rect,
                                	para->x1+offsetx,para->y1+offsety,
                                    para->y2+offsety+1,
                                    para->color,para->rop2_code);
                    }
                	else
                    {   //Ӳ�����ٻ�ͼʱ��δ����changed_msk������֮
                    	__gk_shading_line(fb_gkwin,&clip->rect,
                                    	para->x1+offsetx,para->y1+offsety,
                                    	para->x2+offsetx,para->y2+offsety+1);
                    }
                    clip = clip->next;
                }while(clip != para->gkwin->visible_clip);
            }
            else       
            //��win buffer��Ҳ��frame buffer��ֱ�ӻ���screen��
            //ֱ��д������Ϊtrue���������޻���������ֱ�ӻ���screen��
            {
                do{//��screen��ֱ�ӻ�ֱ�ߣ����յ�
                	if(!my_draw_fun->line_screen_ie(&clip->rect,
                              	para->x1+offsetx,para->y1+offsety,
                              	para->x2+offsetx,para->y2+offsety,
                             	para->color,para->rop2_code))
                    {
                    	__gk_lineto_screen(display,&clip->rect,
                                    	para->x1+offsetx,para->y1+offsety,
                                    	para->x2+offsetx,para->y2+offsety+1,
                                    	para->color,para->rop2_code);
                    }           
                    clip = clip->next;
                }while(clip != para->gkwin->visible_clip);
            }
        }      
    }
    else	//����б��
    {
    	dx = abs(para->x2-para->x1);
    	dy = abs(para->y2-para->y1);
        //˵����win buffer����ֱ��д������Ϊfalse
        if((para->gkwin->wm_bitmap.bm_bits != NULL)
            && (para->gkwin->direct_screen == false))    
        {   //������:��win buffer�л�ͼ����־changed_msk
            limit.left = 0;
            limit.top = 0;
            limit.right = para->gkwin->wm_bitmap.width;
            limit.bottom = para->gkwin->wm_bitmap.height;
            if(!my_draw_fun->line_bm_ie(&para->gkwin->wm_bitmap,&limit,para->x1,
                        para->y1,para->x2,para->y2,para->color,para->rop2_code))
            {    //Ӳ�����ٲ�֧��ֱ�߻��ƣ��������ʵ�֣���������㷨��
                //ͬʱ������changed_msk
            	if(dy > dx)
                {
                	__gk_olineto_bm(para->gkwin,&limit,para->x1,para->y1,
                                	para->x2,para->y2+1,
                                	para->color,para->rop2_code);
                }
            	else
                {
                	__gk_olineto_bm(para->gkwin,&limit,para->x1,para->y1,
                                	para->x2+1,para->y2,
                                	para->color,para->rop2_code);
                }
            }
            else
            {   //Ӳ������֧��ֱ�߻��ƣ�δ����changed_msk������֮
            	__gk_shading_line(para->gkwin,&limit,
                            	para->x1,para->y1,para->x2,para->y2);
            }
            
        }
        else
        {
            clip = para->gkwin->visible_clip;
            if(clip == NULL)
                return ;
            fb_gkwin = display->frame_buffer;
            offsetx = para->gkwin->absx0;
            offsety = para->gkwin->absy0;
            //��frame buffer����ֱ��д������Ϊfalse
            if((fb_gkwin != NULL) && (para->gkwin->direct_screen == false))     
            {   //������:��frame buffer�л�ͼ����ֻ��gkwin�еĿ�������
                do
                {
                    if(!my_draw_fun->line_bm_ie(
                                &fb_gkwin->wm_bitmap,&clip->rect,
                                para->x1+offsetx,para->y1+offsety,
                                para->x2+offsetx,para->y2+offsety,
                                para->color,para->rop2_code))
                    {    //Ӳ�����ٲ�֧��ֱ�߻��ƣ��������ʵ�֣���������㷨��
                        //ͬʱ������changed_msk
                            __gk_olineto_bm(fb_gkwin,&clip->rect,
                                        	para->x1+offsetx,para->y1+offsety,
                                        	para->x2+offsetx,para->y2+offsety,
                                        	para->color,para->rop2_code);
                    }
                	else
                    {   //Ӳ�����ٻ�ͼʱ��δ����changed_msk������֮
                        	__gk_shading_line(fb_gkwin,&clip->rect,
                                        	para->x1+offsetx,para->y1+offsety,
                                        	para->x2+offsetx,para->y2+offsety);
                    }
                    clip = clip->next;
                }while(clip != para->gkwin->visible_clip);
            }
            else       
            //��win buffer��Ҳ��frame buffer��ֱ�ӻ���screen��
            //ֱ��д������Ϊtrue���������޻���������ֱ�ӻ���screen��
            {
                do{//��screen��ֱ�ӻ�ֱ�ߣ����յ�
                	if(!my_draw_fun->line_screen_ie(&clip->rect,para->x1+offsetx,
                             	para->y1+offsety,para->x2+offsetx,para->y2+offsety,
                             	para->color,para->rop2_code))
                    {
                        	__gk_lineto_screen(display,&clip->rect,
                                        	para->x1+offsetx,para->y1+offsety,
                                        	para->x2+offsetx,para->y2+offsety,
                                        	para->color,para->rop2_code);
                    }           
                    clip = clip->next;
                }while(clip != para->gkwin->visible_clip);
            }
        }  
    }
}
//----����λͼ-----------------------------------------------------------------
//����: �ڴ����ڻ�λͼ��λͼ�������ڵĲ��ֽ�������ơ�
//����: �����ǽṹpara�ĳ�Ա
//      gkwin��Ŀ�괰��ָ��
//      bitmap�������Ƶ�λͼ
//      x��y�����Ƶ�Ŀ��λ��
//����: ��
//-----------------------------------------------------------------------------
void gk_draw_bitmap(struct gksc_para_draw_bitmap *para)//ȷ��
{
    struct st_rect target,ins_rect;
    struct clip_rect *clip;
    struct gkwin_rsc *fb_gkwin;
    struct st_rect_bitmap *bitmap;
    struct st_rect_bitmap *src,*dst;    
    s32 x_src,y_src,x_dst,y_dst;
    
    bitmap = &para->gkwin->wm_bitmap;
    //�����Ƶ�λͼҪ���Ƶ�λ�������Ŀ�괰�ڵ�����
    target.left = para->x;
    target.top = para->y;
    target.right = para->x+para->bitmap.width;
    target.bottom = para->y+para->bitmap.height;
    //����������λͼҪ���Ƶ�λ�ã������ڴ�����
    if((target.right <= 0) || (target.left >= bitmap->width)
        ||(target.bottom <= 0) || (target.top >= bitmap->height))
        return;                             //��ͼ�����ڴ�������
    if(target.left < 0) target.left = 0;    //������߽��ڴ����ڲ�
    if(target.top < 0) target.top = 0;      //�����ϱ߽��ڴ����ڲ�
    if(target.right > bitmap->width)        //�����ұ߽��ڴ����ڲ�
        target.right = bitmap->width;
    if(target.bottom > bitmap->height)      //�����±߽��ڴ����ڲ�
        target.bottom = bitmap->height;
    //˵����win buffer����ֱ��д������Ϊfalse
    if((para->gkwin->wm_bitmap.bm_bits != NULL) 
        && (para->gkwin->direct_screen == false))     
    {   //������:��win buffer�л�ͼ����־changed_msk
        if(!para->gkwin->disp->draw.blt_bm_to_bm(bitmap,&target,
                            &para->bitmap,0,0,NULL,NULL,cn_r3_srccopy,0))
        {   
            //Ӳ�����ٲ�֧��λ�鴫�ͣ����ʵ��
            y_dst= target.top;
            for(y_src = 0;
                y_src < target.bottom-target.top;
                y_src++)
            {
                x_dst = target.left;
                for(x_src = 0;
                    x_src < target.right-target.left;
                    x_src++)
                {
                    __gk_set_pixel_rop3_bm(bitmap,&para->bitmap,NULL,
                        x_dst,y_dst,x_src,y_src,cn_r3_srccopy);
                    x_dst++;                                                                        
                }
                y_dst++;                                                                        
            }
        }
        __gk_shading_rect(para->gkwin,&target);    //���û��Ʋ��ֵ�changed_msk
    }
    else
    {
        clip = para->gkwin->visible_clip;
        if(clip == NULL)                            //���ڵĿ�����Ϊ�գ�ֱ�ӷ���
            return ;
        fb_gkwin = para->gkwin->disp->frame_buffer;
        target.left += para->gkwin->absx0;
        target.right += para->gkwin->absx0;
        target.top += para->gkwin->absy0;
        target.bottom += para->gkwin->absy0;
        //��frame buffer,��ֱ��д������Ϊfalse
        if((fb_gkwin != NULL) && (para->gkwin->direct_screen == false))     
        {//������:��frame_buffer�л�ͼ����ֻ��gkwin�еĿ�������
            //Ϊ����������걣��һ�£�����λͼҪ���Ƶ�λ�õ�
            //�����������screenԭ��Ϊԭ�������
            do{
                //Ҫ���Ƶ�λͼĿ��λ���ڴ��ڿ������ڲ�ִ�л��Ƴ���
                if(__gk_get_rect_ints(&clip->rect,&target,&ins_rect))
                {
                    if(!fb_gkwin->disp->draw.blt_bm_to_bm(
                                        &fb_gkwin->wm_bitmap,&ins_rect,
                                        &para->bitmap,ins_rect.left-target.left,
                                        ins_rect.top-target.top,NULL,NULL,
                                        cn_r3_srccopy,0))
                    {//Ӳ�����ٲ�֧��λ�鴫�ͣ����ʵ��
                        dst = &fb_gkwin->wm_bitmap;
                        src = &para->bitmap;
                        
                        y_dst= ins_rect.top;
                        for(y_src= ins_rect.top-target.top;
                            y_src < ins_rect.bottom-target.top;
                            y_src++)
                        {
                            x_dst = ins_rect.left;
                            for(x_src = ins_rect.left-target.left;
                                x_src < ins_rect.right-target.left;
                                x_src++)
                            {
                                __gk_set_pixel_rop3_bm(dst,src,NULL,
                                    x_dst,y_dst,x_src,y_src,cn_r3_srccopy);
                                x_dst++;                                                                        
                            }
                            y_dst++;                                                                        
                        }
                    } 
                    //���û��Ʋ��ֵ�changed_msk
                    __gk_shading_rect(fb_gkwin,&ins_rect);
                }
                clip = clip->next;
            }while(clip != para->gkwin->visible_clip);
        }
        else       
        //��win buffer��Ҳ��frame buffer��ֱ�ӻ���screen��
        //ֱ��д������Ϊtrue���������޻���������ֱ�ӻ���screen��
        {
            //Ϊ����������걣��һ�£�����λͼҪ���Ƶ�λ�õ�
            //�����������screenԭ��Ϊԭ�������
            do{
                //Ҫ���Ƶ�λͼĿ��λ���ڴ��ڿ������ڲ�ִ�л��Ƴ���
                if(__gk_get_rect_ints(&clip->rect,&target,&ins_rect))
                {
                    //����bitmap��screen
                    __gk_blt_bm_to_screen(para->gkwin->disp,&ins_rect,
                                        &para->bitmap,
                                        ins_rect.left-target.left,
                                        ins_rect.top-target.top);
                }                
                clip = clip->next;
            }while(clip != para->gkwin->visible_clip);
        }
    }
}

//----��ʾ�ı�-----------------------------------------------------------------
//����: ��Ĭ�������Ĭ���ַ�����ʾһ���ı�����
//����: gkwin����ʾ��Ŀ�괰��
//      x��y����ʾλ�ã������gkwin
//      text������ʾ���ַ���
//      count���ַ������ȣ�����������
//����: ��
//-----------------------------------------------------------------------------
void gk_draw_text(struct gksc_para_draw_text *para,const char *text)//ȷ��
{
    struct gksc_para_draw_bitmap bitmap_para;
    s32 len, src_len,size,size_bak;
    u32 wc;
    struct font_rsc* cur_font;
    u8 buf[128],*dbuf;  //����һ���㹻��32�����ֵĻ������������Ҫ��ʾ���ַ�
                        //�����˵��󣬾���Ҫ��̬����

    cur_font = font_get_cur_font_rsc();
	if(cur_font == NULL)
	    return;

    // �����ֽ���
    src_len = para->count;

    bitmap_para.x = para->x;
//    bitmap_para.command =cn_gksc_draw_bitmap;
    bitmap_para.gkwin = para->gkwin;
    bitmap_para.y = para->y;
    bitmap_para.bitmap.bm_bits = NULL;
    dbuf = NULL;
    size_bak = 0;
    for(; src_len > 0;)
    {
        len= mbtowc(&wc, text, src_len);
        if(len == -1)
        { // ��Ч�ַ�
            src_len--;
            text++;
        }else
        { // ��Ч�ַ�
            text += len;
            src_len -= len;

            // ȡ����
            bitmap_para.bitmap.bm_bits = NULL;      //NULL=��ѯbitmap.
            cur_font->get_char_bitmap(wc,0,0,&(bitmap_para.bitmap));
            size = bitmap_para.bitmap.linebytes * bitmap_para.bitmap.height;
            if(size < 128)
                bitmap_para.bitmap.bm_bits = buf;
            else
            {
                if(size > size_bak)
                {
                    m_free(dbuf);
                    dbuf = m_malloc(size,0);
                    if(dbuf == NULL)
                        return;
                    size_bak = size;
                }
                bitmap_para.bitmap.bm_bits = dbuf;
            }
            cur_font->get_char_bitmap(wc,0,0,&(bitmap_para.bitmap));
            gk_draw_bitmap(&bitmap_para);

            bitmap_para.x += bitmap_para.bitmap.width;
        }
    }
    m_free(dbuf);
}
//----���ƹ�դλͼ-------------------------------------------------------------
//����: �ڴ����ڻ���դλͼ��λͼ�������ڵĲ��ֽ�������ơ�
//����: �����ǽṹpara�ĳ�Ա
//      gkwin��Ŀ�괰��ָ��
//      bitmap�������Ƶ�λͼ
//      pattern��Ҫ���õ�ͼ��λͼ    
//      bitmsk��rop4��������Ҫ������λͼ��һ����ɫλͼ
//      transparentcolor��Ҫ���õ�͸��ɫ   
//      x��y�����Ƶ�Ŀ��λ��
//      rop_code����դ������
//����: ��
//-----------------------------------------------------------------------------
void gk_draw_bitmap_rop(struct gksc_para_draw_bitmap_rop *para)//ȷ��
{
    u32 color,transparentcolor;
    s32 x_src,y_src,x_dst,y_dst,x_temp,y_temp;    
    struct st_rect target,ins_rect;
    struct clip_rect *clip;
    struct gkwin_rsc *fb_gkwin;
    struct st_rect_bitmap *bitmap,pat;//patΪpattern������ɫ��ʽת���õ���λͼ
    struct st_rect_bitmap *src,*dst;
    struct display_rsc *display;

    display = para->gkwin->disp;

    //�����߸�����͸��ɫ����ɫ��ʽ�������ɫ
	transparentcolor = 
             	__gk_convert_color_to_pf(display->pixel_format,
                                             	para->transparentcolor);
    bitmap = &para->gkwin->wm_bitmap;
    //�����Ƶ�λͼҪ���Ƶ�λ�������Ŀ�괰�ڵ�����
    target.left = para->x;
    target.top = para->y;
    target.right = para->x+para->bitmap.width;
    target.bottom = para->y+para->bitmap.height;
    //����������λͼҪ���Ƶ�λ�ã������ڴ�����
    if((target.right <= 0) || (target.left >= bitmap->width)
        ||(target.bottom <= 0) || (target.top >= bitmap->height))
        return;                             //��ͼ�����ڴ�������
    if(target.left < 0) target.left = 0;    //������߽��ڴ����ڲ�
    if(target.top < 0) target.top = 0;      //�����ϱ߽��ڴ����ڲ�
    if(target.right > bitmap->width)        //�����ұ߽��ڴ����ڲ�
        target.right = bitmap->width;
    if(target.bottom > bitmap->height)      //�����±߽��ڴ����ڲ�
        target.bottom = bitmap->height;
    pat = para->pattern;

    //��Ҫpattern�������㣬�Ž��и�ʽ��
    if(gk_rop_needsno_pattern(para->rop_code) == true)
    {
        //�����߸�����pattern��ɫ��ʽ����ʾ����ɫ��ʽ��ͬ��ֱ��ʹ�ø�����pattern
        //�����轫pattern�е����ؽ�����ɫ��ʽת��
        if(para->pattern.pf_type != display->pixel_format)
        {
            //��pat��ֵ����ߴ�Ϊ8*8����pattern��ͬ����ɫ��ʽӦ����ʾ����ͬ
            pat.pf_type = display->pixel_format;
            pat.linebytes = pat.width * (pat.pf_type>>8)>>3;
            pat.bm_bits = m_malloc(pat.linebytes * pat.height,0);
            if(pat.bm_bits == NULL)     //�޷�����pattern����Ҫ�Ļ���
                return;
            //��ʽ��pattern
            if(display->format_pattern(pat.bm_bits,&para->pattern))
            {
            }else
            {//���ʵ�ָ�ʽ��pattern
                for(y_temp = 0;y_temp < pat.height;y_temp++)
                {
                    for(x_temp = 0;x_temp < pat.width;x_temp++)
                    {
                        color = __gk_get_pixel_bm(&para->pattern,
                                                     	x_temp,y_temp);
                        color = 
                         __gk_convert_part_pf_to_pf(
                                 	display->pixel_format,
                                 	para->pattern.pf_type,color);
                        __gk_set_pixel_rop2_bm(&pat,x_temp,y_temp,
                                                    color,cn_r2_copypen);
                    }
                }
            }
        }
    }
    //˵����win buffer����ֱ��д������Ϊfalse
    if((bitmap->bm_bits != NULL) && (para->gkwin->direct_screen == false))     
    {   //������:��win buffer�л�ͼ����־changed_msk
        if(!display->draw.blt_bm_to_bm(bitmap,&target,
            &para->bitmap,0,0,pat.bm_bits,&para->bitmsk,
            para->rop_code,para->transparentcolor))
        {
            dst = bitmap;
            src = &para->bitmap;

            y_dst= target.top;
            //�����Ĺ�դ������Ϊ��չ�Ĺ�դ������
            if((para->rop_code & cn_rop_ext_en) != 0)
            {
                //���ʹ����Ԫ��Ų��������]�и�����Ҫ������λͼ��ֱ�ӷ���
            	if(((para->rop_code & cn_rop_rop4_en) != 0) 
                    && (&para->bitmsk == NULL))
                {
                	return;
                }
                for(y_src= 0;
                    y_src < target.bottom-target.top;
                    y_src++)
                {
                    x_dst = target.left;
                    for(x_src = 0;
                        x_src < target.right-target.left;
                        x_src++)
                    {
                        __gk_set_pixel_rop_ext_bm(dst,src,pat.bm_bits,
                                        x_dst,y_dst,x_src,y_src,para->rop_code,
                                        &para->bitmsk,para->transparentcolor);
                        x_dst++;                                                                        
                    }
                    y_dst++;                                                                        
                }                                    
            }
            else//�����Ĺ�դ������Ϊ��Ԫ��դ������
            {
            	for(y_src= 0;
                    y_src < target.bottom-target.top;
                    y_src++)
                {
                    x_dst = target.left;
                    for(x_src = 0;
                        x_src < target.right-target.left;
                        x_src++)
                    {
                        __gk_set_pixel_rop3_bm(dst,src,pat.bm_bits,
                            x_dst,y_dst,x_src,y_src,para->rop_code);
                        x_dst++;                                                                        
                    }
                    y_dst++;                                                                        
                }                
            }
        }
        __gk_shading_rect(para->gkwin,&target);    //���û��Ʋ��ֵ�changed_msk
    }
    else
    {
        clip = para->gkwin->visible_clip;
        if(clip == NULL)                            //���ڿ�����Ϊ�գ�ֱ�ӷ���
            return ;
        fb_gkwin = display->frame_buffer;
        //��frame buffer����ֱ��д������Ϊfalse
        if((fb_gkwin != NULL) && (para->gkwin->direct_screen == false))     
        {   //������:��frame_buffer�л�ͼ����ֻ��gkwin�еĿ�������
            //Ϊ����������걣��һ�£�����λͼҪ���Ƶ�λ�õ�
            //�����������screenԭ��Ϊԭ�������
            target.left += para->gkwin->absx0;
            target.right += para->gkwin->absx0;
            target.top += para->gkwin->absy0;
            target.bottom += para->gkwin->absy0;
            do{
                if(__gk_get_rect_ints(&clip->rect,&target,&ins_rect))
                {
                    if(!fb_gkwin->disp->draw.blt_bm_to_bm(
                                        &fb_gkwin->wm_bitmap,&ins_rect,
                                        &para->bitmap,ins_rect.left-target.left,
                                        ins_rect.top-target.top,
                                        pat.bm_bits,&para->bitmsk,
                                        para->rop_code,para->transparentcolor))
                    {
                        dst = &fb_gkwin->wm_bitmap;
                        src = &para->bitmap;
                        
                        y_dst= ins_rect.top;
                        //�����Ĺ�դ������Ϊ��չ�Ĺ�դ������
                        if((para->rop_code & cn_rop_ext_en) != 0)
                        {
                            //���ʹ����Ԫ��Ų��������]�и�����Ҫ������λͼ��
                            //ֱ�ӷ���
                        	if(((para->rop_code & cn_rop_rop4_en) != 0) 
                                && (&para->bitmsk == NULL))
                           {
                            	return;
                            }
                            for(y_src= ins_rect.top-target.top;
                                y_src < ins_rect.bottom-target.top;
                                y_src++)
                            {
                                x_dst = ins_rect.left;
                                for(x_src = ins_rect.left-target.left;
                                    x_src < ins_rect.right-target.left;
                                    x_src++)
                                {
                                    __gk_set_pixel_rop_ext_bm(dst,src,
                                        pat.bm_bits,x_dst,y_dst,x_src,
                                        y_src,para->rop_code,&para->bitmsk,
                                        para->transparentcolor);
                                    x_dst++;                                                                        
                                }
                                y_dst++;                                                                        
                            }                                                                       
                        }
                        else//�����Ĺ�դ������Ϊ��Ԫ��դ������
                        {
                        	for(y_src= ins_rect.top-target.top;
                            	y_src < ins_rect.bottom-target.top;
                            	y_src++)
                            {
                            	x_dst = ins_rect.left;
                                for(x_src = ins_rect.left-target.left;
                                	x_src < ins_rect.right-target.left;
                                	x_src++)
                                {
                                	__gk_set_pixel_rop3_bm(dst,src,
                                        	pat.bm_bits,x_dst,y_dst,x_src,
                                            y_src,para->rop_code);
                                	x_dst++;                                                                        
                                }
                                y_dst++;                                                                        
                            }                                                                       
                        }
                    }
                    //���û��Ʋ��ֵ�changed_msk
                    __gk_shading_rect(fb_gkwin,&ins_rect);
                }
                clip = clip->next;
            }while(clip != para->gkwin->visible_clip);
        }
        else       
        //��դλͼ��֧��ֱ��д������win buffer��Ҳ��frame buffer��
        //����ֱ��д������Ϊtrue�������ֱ�ӷ���
        {
            return;
        }
    }
}
//----��䴰�ڵ�һ����---------------------------------------------------------
//����: ��ָ������ɫ��䴰�ڵ�һ���֡�
//����: �����ǽṹpara�ĳ�Ա
//      gkwin��Ŀ�괰��
//      rect�������ľ���
//      color�����ʹ�õ���ɫ
//����: ��
//-----------------------------------------------------------------------------
void gk_fill_part_win(struct gksc_para_fill_part_win *para)//ȷ��
{
    s32 x,y;
    struct st_rect_bitmap *bitmap;
    struct st_rect target,ins_rect;
    struct disp_draw *my_draw_fun;
    struct clip_rect *clip;
    struct gkwin_rsc *fb_gkwin;

    bitmap = &para->gkwin->wm_bitmap;
    //��������Ҫ���Ƶ�λ�õ����꣬�����Ŀ�괰��
    target = para->rect;
    if((target.right <= 0) || (target.left >= bitmap->width)
        ||(target.bottom <= 0) || (target.top >= bitmap->height))
        return;                             //��ͼ�����ڴ�������
        
    if(target.left < 0) target.left = 0;    //������߽��ڴ����ڲ�
    if(target.top < 0) target.top = 0;      //�����ϱ߽��ڴ����ڲ�
    if(target.right > bitmap->width)        //�����ұ߽��ڴ����ڲ�
        target.right = bitmap->width;
    if(target.bottom > bitmap->height)      //�����±߽��ڴ����ڲ�
        target.bottom = bitmap->height;

    my_draw_fun = &para->gkwin->disp->draw;
    //˵����win buffer����ֱ��д������Ϊfalse
    if((para->gkwin->wm_bitmap.bm_bits != NULL)
        && (para->gkwin->direct_screen == false))     
    {   
        //������:��win buffer�л�ͼ����ɫchanged_msk
        if(!my_draw_fun->fill_rect_bm(bitmap,&target,para->color))
        {
            //Ӳ�����ٲ�֧�����λͼ���������ʵ��
            __gk_fill_rect(bitmap,&target,para->color);
        }
        __gk_shading_rect(para->gkwin,&para->rect);//��ɫ��������changed_msk
    }else
    {
        clip = para->gkwin->visible_clip;
        if(clip == NULL)                //���ڿ�����Ϊ�գ�ֱ�ӷ���
            return ;
        fb_gkwin = para->gkwin->disp->frame_buffer;
        //�������Ļ��ƣ�����֡�������screen�ϻ��ƣ�ʹ�þ������꣬�任֮
        target.left += para->gkwin->absx0;
        target.right += para->gkwin->absx0;
        target.top += para->gkwin->absy0;
        target.bottom += para->gkwin->absy0;
        //��frame buffer,��ֱ��д������Ϊfalse
        if((fb_gkwin != NULL) && (para->gkwin->direct_screen == false))         
        {   //������:��frame buffer�л�ͼ����ֻ��gkwin�еĿ�������
            do{
                //ֻ��䴰�ڿ�������Ҫ���Ƶ�����Ľ���
                if(__gk_get_rect_ints(&clip->rect,&target,&ins_rect))
                {
                    //֡��������꼴�������꣬����������任
                    if(!my_draw_fun->fill_rect_bm(&fb_gkwin->wm_bitmap,
                                                        &ins_rect,para->color))
                    {
                        //Ӳ�����ٲ�֧�����λͼ���������ʵ��
                        __gk_fill_rect(&fb_gkwin->wm_bitmap,
                                            &ins_rect,para->color);
                    }
                    //��־��������changed_msk
                    __gk_shading_rect(fb_gkwin,&ins_rect);
                }
                clip = clip->next;
            }while(clip != para->gkwin->visible_clip);
        }
        else       
        //��win buffer��Ҳ��frame buffer��ֱ�ӻ���screen��
        //ֱ��д������Ϊtrue���������޻���������ֱ�ӻ���screen��
        {
            do{
                struct display_rsc *mirror,*current;
                //ֻ��䴰�ڿ�������Ҫ���Ƶ�����Ľ���
                if(__gk_get_rect_ints(&clip->rect,&target,&ins_rect))
                {
                    //Ӳ�����ٲ�֧�����λͼ���������ʵ��
                    if(!my_draw_fun->fill_rect_screen(&ins_rect,para->color))
                    {
                        for(y = ins_rect.top;y < ins_rect.bottom;y++)
                        {
                            for(x = ins_rect.left;x < ins_rect.right;x++)
                            {
                                my_draw_fun->set_pixel_screen(x,y,para->color,
                                                            cn_r2_copypen);
                            }
                        }
                    }
                    mirror = (struct display_rsc *)&para->gkwin->disp->node;
                    if(mirror == NULL)      //û�о�����ʾ����ֱ�ӷ���
                        return;
                    current = mirror;
                    while(current != NULL)
                    {
                        current->draw.fill_rect_screen(&ins_rect,para->color);
                        current = 
                            (struct display_rsc *)rsc_trave_scion(&mirror->node,
                                &current->node);
                    }
                }
                clip = clip->next;
            }while(clip != para->gkwin->visible_clip);
        }
    }
}

//----��䴰��-----------------------------------------------------------------
//����: ��ָ������ɫ��䴰�ڡ�
//����: �����ǽṹpara�ĳ�Ա
//      gkwin��Ŀ�괰��ָ��
//      color�����ʹ�õ���ɫ
//����: ��
//-----------------------------------------------------------------------------
void gk_fill_win(struct gksc_para_fill_win *para)//ȷ��
{
    s32 x,y;
    struct st_rect rect;
    struct clip_rect *clip;
    struct gkwin_rsc *fb_gkwin;
    struct st_rect_bitmap *bitmap;

    struct disp_draw *my_draw_fun;

    my_draw_fun = &para->gkwin->disp->draw;
    //˵����win buffer����ֱ��д������Ϊfalse
    if((para->gkwin->wm_bitmap.bm_bits != NULL)
        && (para->gkwin->direct_screen == false))     
    {   //������:��win buffer�л�ͼ����־changed_msk
        bitmap = &para->gkwin->wm_bitmap;
        rect.left = 0;
        rect.top = 0;
        rect.right = bitmap->width;
        rect.bottom = bitmap->height;
        if(!my_draw_fun->fill_rect_bm(bitmap,&rect,para->color))
        {//Ӳ�����ٲ�֧�����λͼ���������ʵ��
            __gk_fill_bm(bitmap,para->color);
        }
        para->gkwin->change_flag = cn_gkwin_change_all;
    }else
    {
        clip = para->gkwin->visible_clip;
        if(clip == NULL)
            return ;
        fb_gkwin = para->gkwin->disp->frame_buffer;
        //��frame buffer����ֱ��д������Ϊfalse
        if((fb_gkwin != NULL) && (para->gkwin->direct_screen == false))         
        {   //������:��frame buffer�л�ͼ����ֻ��gkwin�еĿ�������
            bitmap = &fb_gkwin->wm_bitmap;
            do{
                //֡����������clip�е����궼�Ǿ������꣬����任
                if(!my_draw_fun->fill_rect_bm(bitmap,&clip->rect,para->color))
                {//Ӳ�����ٲ�֧�����λͼ���������ʵ��
                    __gk_fill_rect(bitmap,&clip->rect,para->color);
                }
                //��־��������changed_msk
                __gk_shading_rect(fb_gkwin,&clip->rect);
                clip = clip->next;
            }while(clip != para->gkwin->visible_clip);
        }else       
        //��win buffer��Ҳ��frame buffer��ֱ�ӻ���screen��
        //ֱ��д������Ϊtrue���������޻���������ֱ�ӻ���screen��
        {
            do{
                struct display_rsc *mirror,*current;
                if(!my_draw_fun->fill_rect_screen(&clip->rect,para->color))
                {//Ӳ�����ٲ�֧�����screen�ϵľ������������ʵ��
                    for(y = clip->rect.top;y < clip->rect.bottom;y++)
                    {
                        for(x = clip->rect.left;x < clip->rect.right;x++)
                        {
                            my_draw_fun->set_pixel_screen(x,y,para->color,
                                                        cn_r2_copypen);
                        }
                    }
                }
                mirror = (struct display_rsc *)&para->gkwin->disp->node;
                if(mirror == NULL)      //û�о�����ʾ����ֱ�ӷ���
                    return;
                current = mirror;
                while(current != NULL)
                {
                    current->draw.fill_rect_screen(&clip->rect,para->color);
                    current = 
                            (struct display_rsc *)rsc_trave_scion(&mirror->node,
                            &current->node);
                }                
                clip = clip->next;
            }while(clip != para->gkwin->visible_clip);
        }
    }
}

#endif //   for #if (cfg_gui_kernel == 1)

