//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//版权所有 (C), 2004-2009,   罗侍田.
//所属模块: gui
//作者:  罗侍田.
//版本：V1.0.0
//文件描述: 跟屏幕绘制相关的操作
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2010-03-30
//   作者:  罗侍田.
//   新版本号：V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#include "os_inc.h"

#if (cfg_gui_kernel == 1)

#include "gk_inc.h"
#include <stdlib.h>
#include <string.h>

#define __gk_swap(a, b){a^=b; b^=a; a^=b;}
//列出了三元光栅操作逆波兰式，每个元素在数组中的下标即与其对应的光栅操作码
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
//----光栅变量检查1------------------------------------------------------------
//功能: 检查指定的光栅操作码是否需要目标位图参加运算。
//参数: rop_code，三元/四元光栅操作码
//返回: true=需要，false=不需要
//注: 判断逻辑表达式(((fore_code & cn_rop_d)>>1) == (fore_code & ~cn_rop_d))的
//    含义，太长，注释无法写下，请参看代码配套的书。
//-----------------------------------------------------------------------------
bool_t gk_rop_needsno_destination(u32 rop_code)
{
    u8 fore_code,back_code;
    fore_code = (rop_code>>16) & 0xff;      //bit16~23为前景光栅操作码
    if(rop_code & cn_rop_ext_en)            //是扩展的rop码，判断扩展功能
    {   //是rop4，需要前景rop码和背景rop码共同参与运算
        //需要判断两种光栅操作，两种光栅操作都不需要才返回false
        if((rop_code & cn_rop_rop4_en) != 0)
        {
            back_code = (rop_code>>24) & 0xff;      //bit24~31为背景光栅操作码
            //满足((fore_code & cn_rop_d)>>1) == (fore_code & ~cn_rop_d)
            //前景光栅操作不需要目标位图，背景光栅操作与之类似
            //前景背景光栅操作均不需要目标位图参与，才返回false
            if((((fore_code & cn_rop_d)>>1) == (fore_code & ~cn_rop_d))
               && (((back_code & cn_rop_d)>>1) == (back_code & ~cn_rop_d)))
                return false;
            else
                return true; 
        }
        else
            return true;                    //非rop4，肯定需要目标像素参与运算        
    }else
    //不是扩展的光栅操作码，只有一种光栅操作
    {
        if(((fore_code & cn_rop_d)>>1) == (fore_code & ~cn_rop_d))
           return false;
        else
            return true;
    }
}

//----光栅变量检查2------------------------------------------------------------
//功能: 检查指定的光栅操作码是否需要源位图参加运算。
//参数: rop_code，三元/四元光栅操作码
//返回: true=需要，false=不需要
//注: 判断逻辑表达式(((fore_code & cn_rop_s)>>1) == (fore_code & ~cn_rop_s))的
//    含义，太长，注释无法写下，请参看代码配套的书。
//-----------------------------------------------------------------------------
bool_t gk_rop_needsno_source(u32 rop_code)
{
    u8 fore_code,back_code;
    fore_code = (rop_code>>16) & 0xff;      //bit16~23为前景光栅操作码
    if(rop_code & cn_rop_ext_en)            //是扩展的rop码，判断扩展功能
    {   //是rop4，需要前景rop码和背景rop码共同参与运算
        //需要判断两种光栅操作，两种光栅操作都不需要才返回false
        if((rop_code & cn_rop_rop4_en) != 0)
        {
            back_code = (rop_code>>24) & 0xff;      //bit24~31为背景光栅操作码
            //满足((fore_code & cn_rop_s)>>2) == (fore_code & ~cn_rop_s)
            //前景光栅操作不需要源位图，背景光栅操作与之类似
            //前景背景光栅操作均不需要源位图参与，才返回false
            if((((fore_code & cn_rop_s)>>2) == (fore_code & ~cn_rop_s))
               && (((back_code & cn_rop_s)>>2) == (back_code & ~cn_rop_s)))
                return false;
            else
                return true;
        }            
        else
            return true;                    //非rop4，肯定需要源像素参与运算            
    }else
    //不是扩展的光栅操作码，只有一种光栅操作
    {
        if(((fore_code & cn_rop_s)>>2) == (fore_code & ~cn_rop_s))
           return false;
        else
            return true;
    }
}

//----光栅变量检查3------------------------------------------------------------
//功能: 检查指定的光栅操作码是否需要pattern位图参加运算。
//参数: rop_code，三元/四元光栅操作码
//返回: true=需要，false=不需要
//注: 判断逻辑表达式(((fore_code & cn_rop_p)>>1) == (fore_code & ~cn_rop_p))的
//    含义，太长，注释无法写下，请参看代码配套的书。
//-----------------------------------------------------------------------------
bool_t gk_rop_needsno_pattern(u32 rop_code)
{
    u8 fore_code,back_code;
    fore_code = (rop_code>>16) & 0xff;      //bit16~23为前景光栅操作码
    if(rop_code & cn_rop_ext_en)            //是扩展的rop码，判断扩展功能
    {   //是rop4，需要前景rop码和背景rop码共同参与运算
        //需要判断两种光栅操作，两种光栅操作都不需要才返回false
        if((rop_code & cn_rop_rop4_en) != 0)
        {
            back_code = (rop_code>>24) & 0xff;      //bit24~31为背景光栅操作码
            //满足((fore_code & cn_rop_p)>>4) == (fore_code & ~cn_rop_p)
            //前景光栅操作不需要pattern位图，背景光栅操作与之类似
            //前景背景光栅操作均不需要pattern位图参与，才返回false
            if((((fore_code & cn_rop_p)>>4) == (fore_code & ~cn_rop_p))
               && (((back_code & cn_rop_p)>>4) == (back_code & ~cn_rop_p)))
                return false;
            else
                return true;
        }            
        else
            return false;           //非rop4，肯定不需要源pattern素参与运算        
    }else
    //不是扩展的光栅操作码，只有一种光栅操作
    {
        if(((fore_code & cn_rop_p)>>4) == (fore_code & ~cn_rop_p))
           return false;
        else
            return true;
    }
}

//----本地格式转换到rgb24------------------------------------------------------
//功能: 颜色格式转换，把一个本地格式颜色转换成真彩色，如果本地颜色含alpha信息，
//      则结果是cn_sys_pf_a8r8g8b8格式，否则是cn_sys_pf_e8r8g8b8格式。
//参数: pf_type，本地格式
//      color，源色彩
//返回: 真彩色格式
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

//----rgb24转换到本地----------------------------------------------------------
//功能: 把一个rgb24格式的颜色转换成本地格式，如果pf_type中含有alpha因素，则源
//      格式必须是cn_sys_pf_a8r8g8b8，否则必须是cn_sys_pf_e8r8g8b8。
//      本函数用于窗口显示，目标格式不支持调色板格式。
//参数: pf_type，目标格式
//      color，源色彩
//返回: pf_type格式的色彩
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
            //根据rgb值计算hls模型的亮度
            return (((MAX(MAX(r,g),b)+MIN(MIN(r,g),b))*1) + 255 )/(2*255); 
        }break;
        case cn_sys_pf_gray2:
        {
            //根据rgb值计算hls模型的亮度
            return (((MAX(MAX(r,g),b)+MIN(MIN(r,g),b))*3) + 255 )/(2*255); 
        }break;
        case cn_sys_pf_gray4:
        {
            //根据rgb值计算hls模型的亮度
            return (((MAX(MAX(r,g),b)+MIN(MIN(r,g),b))*15) + 255 )/(2*255); 
        }break;
        case cn_sys_pf_gray8:
        {
            //根据rgb值计算hls模型的亮度
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
        case cn_sys_pf_r5g5b5c1:    //实际上丢掉了最低位
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
//----各种颜色格式间的相互转换-------------------------------------------------
//功能: 各种颜色格式间的相互转换。
//参数: dpf_type，目标格式
//      spf_type，源格式
//      color，源色彩
//返回: dpf_type格式的色彩
//-----------------------------------------------------------------------------
u32 __gk_convert_part_pf_to_pf(u16 dpf_type,u16 spf_type,u32 color)
{
    u32 result;
    //源格式与目标格式相同，直接返回源色彩
    if(spf_type == dpf_type)
        return color;
    //将源色彩转换成真彩色
    result = __gk_convert_color_to_24(spf_type,color);
    //将真彩色转换成目标格式的颜色
    result = __gk_convert_color_to_pf(dpf_type,result);
    return result;
}
//----rop2混合-----------------------------------------------------------------
//功能: 计算rop2色彩混合。
//参数: dest，背景颜色
//      pen，画笔颜色
//      rop2_code，二元光栅操作码
//返回: 混合后的色彩
//-----------------------------------------------------------------------------
u32 __gk_blend_rop2(u32 dest,u32 pen,u32 rop2_code)
{
    switch(rop2_code)
    {
        case cn_r2_black:         // 混合后的颜色为黑色(black)  
        {
            return cn_color_black;
        }break;
        case cn_r2_notmergepen:   //混合后的颜色为~(dest | pen) 
        {
            return ~(dest | pen);
        }break;
        case cn_r2_masknotpen:    //混合后的颜色为~pen & dest  
        {
            return ~pen & dest;
        }break;
        case cn_r2_notcopypen:    //混合后的颜色为~pen           
        {
            return ~pen;
        }break;
        case cn_r2_maskpennot:    //混合后的颜色为~dest & pen  
        {
            return ~dest & pen;
        }break;
        case cn_r2_not:           //混合后的颜色为~dest          
        {
            return ~dest;
        }break;
        case cn_r2_xorpen:        //混合后的颜色为dest ^ pen   
        {
            return dest ^ pen;
        }break;
        case cn_r2_notmaskpen:    //混合后的颜色为~(dest & pen)
        {
            return ~(dest & pen);
        }break;
        case cn_r2_maskpen:       //混合后的颜色为dest & pen   
        {
            return dest & pen;
        }break;
        case cn_r2_notxorpen:     //混合后的颜色为~(dest ^ pen)
        {
            return ~(dest ^ pen);
        }break;
        case cn_r2_nop:           //混合后的颜色为dest（原色）   
        {
            return dest;
        }break;
        case cn_r2_mergenotpen:   //混合后的颜色为~pen | dest   
        {
            return ~pen | dest;
        }break;
        case cn_r2_copypen:       //混合后的颜色为pen（画笔色）  
        {
            return pen;
        }break;
        case cn_r2_mergepennot:   //混合后的颜色为~dest | pen   
        {
            return ~dest | pen;
        }break;
        case cn_r2_mergepen:      //混合后的颜色为dest | pen    
        {
            return dest | pen;
        }break;
        case cn_r2_white:         //混合后的颜色为白色(white)  
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
//----rop3混合-----------------------------------------------------------------
//功能: 计算rop3色彩混合。
//参数: dest，目标色彩
//      pat，图案色彩
//      src，源色彩
//      rop3_code，三元光栅操作码
//返回: 混合后的色彩
//注意: 详细的解释，djyos图形编程上有详细介绍
//-----------------------------------------------------------------------------
u32 __gk_blend_rop3(u32 dest,u32 src,u32 pat,u8 rop3_code)
{
    u32 j,m,k;
    u32 s[12];
    char *rpm;
    //根据rop3_table，由rop3_code对应出相应的逆波兰式，再求解混合后的色彩 
    rpm = rop3_table[rop3_code];
    
    if(rpm[0] == '0')           //目标位图颜色为黑色      
        return 0;
    else if(rpm[0] == '1')      //目标位图颜色为白色 
        return 0xffffff;
    else        //根据逆波兰式进行目标位图颜色、图案位图颜色、源位图颜色混合
    {
        m = 0;
        k = 0;
        //根据rop3_table，得到表示逆波兰式的数组中的元素中
        //最多有12个字符(操作数与运算符)
        //从数组的第一个成员扫描，根据后面的运算符依次进行运算得到混合后的颜色
        //如果运算符为单运算符，运算符前面一个字符(进行此运算前的数据)为操作数
        //如果运算符为双运算符，运算符前面两个字符(进行此运算前的数据)为操作数
        for(j = 0;j<12;j++)
        {   
            if(rpm[j] == '\0')      //逆波兰式结束，返回最终计算结果
                return s[j-1]; 
            else if(rpm[j] == 'S')  //操作数为S  
            {
                //S之前有运算符需要对中间参数做处理
                //所有的逆波兰式中存在运算符在中间的情况下
                //中间的运算符都是x
                if(rpm[j-1] == 'x') 
                {
                    m -= 2;
                    k++;
                    s[j] = src;
                }
                else
                    s[j] = src;
            }
            else if(rpm[j] == 'D')  //操作数为D 
            {
                if(rpm[j-1] == 'x') //D之前有运算符需要对中间参数做处理 
                {
                    m -= 2;
                    k++;
                    s[j] = dest;
                }
                else
                    s[j] = dest;
            }
            else if(rpm[j] == 'P')  //操作数为P
            {
                if(rpm[j-1] == 'x') //P之前有运算符需要对中间参数做处理
                {
                    m -= 2;
                    k++;
                    s[j] = pat;
                }
                else
                    s[j] = pat;
            }
            else if(rpm[j] == 'n')  //运算符为n，对运算符前面一个字符的数据取反
            {
                s[j] = ~s[j-1];
                m++;
                s[j-1] = s[j-1-m];
            }
            else if(rpm[j] == 'x')//运算符为x，对运算符前面两个字符的数据异或
            {
                s[j] = s[j-1]^s[j-2];
                m+=2;
                //对"操作数+运算符+操作数"类似的情况单独进行处理
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
            else if(rpm[j] == 'o')//运算符为o，对运算符前面两个字符的数据或运算
            {
                s[j] = s[j-1]|s[j-2];
                m+=2;
                //对"操作数+运算符+操作数"类似的情况单独进行处理
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
                if(rpm[j] == 'a')//运算符为a，对运算符前面两个字符的数据与运算
               {    
                    s[j] = s[j-1]&s[j-2];
                    m+=2;
                    //对"操作数+运算符+操作数"类似的情况单独进行处理
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
//----判断目标点所在的区域-----------------------------------------------------
//功能: 判断目标点所在的区域，分成矩形域的左、右、上、下四部分。
//参数: x、y，目标点,相对窗口的坐标
//      limit，矩形域,相对窗口的坐标
//返回: 标志量c，c = 0表示目标点在矩形域内，否则在矩形域外
//-----------------------------------------------------------------------------
u32 __gk_point_sect(s32 x,s32 y,struct st_rect *limit)  //确认
{
    s32 c;
    c = 0;
    
    if(x < limit->left)                 //判断目标点是否在矩形域左方
        c |= 0x01;
    else
    {
        if(x > limit->right)            //判断目标点是否在矩形域右方
        c |= 0x02;
    }
    if(y < limit->top)                  //判断目标点是否在矩形域上方
        c |= 0x04;
    else
    {
        if(y > limit->bottom)           //判断目标点是否在矩形域下方
        c |= 0x08;
    }
    return c;
}
//----求取直线段与矩形域的交点-------------------------------------------------
//功能: 求取直线段与矩形域的交点。
//参数: limit，矩形域,相对窗口的坐标
//      x1、y1、x2、y2，起、终点,相对窗口的坐标
//返回: 标志量flag，flag = 0表示斜线不在limit内，flag = 1表示斜线有点在limit内
//说明: 此处的直线段是针对斜线段的
//-----------------------------------------------------------------------------
u32 __gk_oline_sect_inter(struct st_rect *limit,s32 *x1,s32 *y1,s32 *x2,s32 *y2)//确认
{
    s32 c1,c2,i=0;
    s32 flag = 1;

    c1 = __gk_point_sect(*x1,*y1,limit);
    c2 = __gk_point_sect(*x2,*y2,limit);

    if((c1 == 0)&&(c2 == 0))        //斜线在limit内
        return flag;
    else if((c1&c2) != 0)           //斜线在limit外，整条线段在limit的一条边界外
    {
        flag = 0;
        return flag;
    }
    else                                                    
    {
        if(c1 == 0)                 //点(x1,y1)在limit内，点(x2,y2)肯定不在
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
        else                    //点(x1,y1)不在limit内，点(x2,y2)可能在limit内
        {                       //也可能在limit外
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
                //直线和limit有交点时，一个端点最多求两次交点
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

//----绘制斜线并标记掩码-------------------------------------------------------
//功能: 绘制斜线并标志位。
//参数: gkwin，目标窗口
//      x1、y1、x2、y2，起、终点
//      color，给定的颜色
//      rop2_code，二元光栅操作码
//返回: 无
//说明: 采用的是Bresenham算法
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
    //标志绘制斜线方向
    if(x2 > x1)   
        s1 = 1;
    else
        s1 = -1;
    if(y2 > y1)
        s2 = 1;
    else
        s2 = -1;
    //标志最大变化的变量，interchange为1，y变化最大，为0，x变化最大
    if(dy > dx)
    {
        __gk_swap(dx, dy);
        interchange = 1;
    }
    else
        interchange = 0;
    p = (dy<<1)-dx;            //设置初始误差判别值
    //颜色格式为显卡自定义格式
    if(bitmap->pf_type & cn_custom_pf)
    {
        for(i=1;i<=dx;i++)//hjj
        {
            gkwin->disp->draw.set_pixel_bm(bitmap,x,y,color,rop2_code);
            //取离数学点较近的像素
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
    {//颜色格式为其它格式
        pf_color = __gk_convert_color_to_pf(bitmap->pf_type,color);
        for(i=1;i<=dx;i++)
        {
            __gk_set_pixel_rop2_bm(bitmap,x,y,pf_color,rop2_code);
            //取离数学点较近的像素
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

//----读取位图中的像素---------------------------------------------------------
//功能: 在一个bitmap读取像素。
//参数: bitmap，目标位图
//      x、y，目标像素的坐标
//返回: 像素颜色值
//-----------------------------------------------------------------------------
u32 __gk_get_pixel_bm(struct st_rect_bitmap *bitmap, s32 x,s32 y)
{
    u8 color_bits,bit_offset;
    u32 color=0,byte_offset;
    color_bits = bitmap->pf_type>>8;
    //根据位图颜色格式读取像素
    switch(color_bits)
    {
        case 1:
        {
            //像素坐标对应的字节偏移量
            byte_offset = y*bitmap->linebytes + x/8;
            //像素坐标在相应字节中的位偏移
            bit_offset = 7-x%8;     //最高位显示在左边，最低位在右边
            //取像素颜色
            color = (u32)(bitmap->bm_bits[byte_offset]>>bit_offset)&0x1;
        }break;
        case 2:
        {
            byte_offset = y*bitmap->linebytes + x/4;
            bit_offset = (3-x%4)<<1;     //最高位显示在左边，最低位在右边
            color = (u32)(bitmap->bm_bits[byte_offset]>>bit_offset)&0x3;
        }break;
        case 4:
        {
            byte_offset = y*bitmap->linebytes + x/2;
            bit_offset = (1-x%2)<<2;     //最高位显示在左边，最低位在右边
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
            //注:因对齐问题，不可把目标地址强制转换成16位指针。
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
//----求取目标颜色的argb分量---------------------------------------------------
//功能: 求取目标颜色的argb分量。
//参数: pf_type，目标颜色颜色格式
//      color，目标颜色
//      a、r、g、b，目标颜色的a、r、g、b分量
//返回: 无
//-----------------------------------------------------------------------------
void __gk_get_rgb_bm(u16 pf_type,u32 color,u32 *a,u32 *r,
                                            u32 *g,u32 *b)
{
    //根据位图颜色格式分离目标颜色中的rgb分量
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
//----argb分量的求取组合-------------------------------------------------------
//功能: 对两个位图中指定坐标的像素求alpha混合值
//参数: dst_bitmap，目标位图，使用背景alpha
//      src_bitmap，源位图，使用前景alpha
//      x_dst、y_dst，目标位图中的目标坐标
//      x_src、y_src，源位图中的目标坐标
//      fore，前景alpha
//      back，背景alpha
//返回: 组合后的颜色
//说明: 进行alpha和双alpha均需要分离color,RGB分别计算
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
    //读取目标位图中的颜色
    color_dst = __gk_get_pixel_bm(dst_bitmap,x_dst,y_dst);
    //读取源位图中的颜色
    color_src = __gk_get_pixel_bm(src_bitmap,x_src,y_src);
    color_src = __gk_convert_part_pf_to_pf(dst_bitmap->pf_type, 
                                        	src_bitmap->pf_type,color_src);
    pf_type = dst_bitmap->pf_type;
    //求取目标位图颜色的rgb分量
    __gk_get_rgb_bm(pf_type,color_dst,&a_dst,&r_dst,&g_dst,&b_dst);
    //求取源位图颜色的rgb分量
    __gk_get_rgb_bm(pf_type,color_src,&a_src,&r_src,&g_src,&b_src);
    //根据目标位图颜色格式求取组合后的颜色            
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
//----在内存画点(扩展rop_code)-------------------------------------------------
//功能: 在一个bitmap上画点。
//参数: dst_bitmap，目标位图
//      src_bitmap，源位图
//      pat，pattern指针
//      x_dst、y_dst，目标位图中的目标坐标
//      x_src、y_src，源位图中的目标坐标
//      rop_code，光栅操作码，扩展的光栅操作码
//      bitmsk，掩码位图
//      transparentcolor，透明色
//返回: 无
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

    if((rop_code & cn_rop_transparentcolor) != 0)   //transparentcolor颜色透明
    {
        color = __gk_get_pixel_bm(src_bitmap,x_src,y_src);
    	color = __gk_convert_part_pf_to_pf(dst_bitmap->pf_type,
                                        src_bitmap->pf_type,color);
        if(color != transparentcolor)//color与 transparentcolor不同，输出前景色
            __gk_set_pixel_rop2_bm(dst_bitmap,x_dst,y_dst,
                                	color,cn_r2_copypen);                                                               
        else
            return;     //color与transparentcolor相同，透明，输出背景色
    }
    else if((rop_code & cn_rop_rop4_en) != 0)//rop4功能，bit24~31表示背景操作码
    {
        byte_offset_msk = y_src*bitmsk->linebytes+x_src/8;
        bit_offset = x_src%8;
        if(((bitmsk->bm_bits[byte_offset_msk]>>(7-bit_offset))&0x1) == 0)                    
        {//掩码位图掩码像素为0时，使用背景光栅操作，bit24~31表示背景光栅操作码
        	__gk_set_pixel_rop3_bm(dst_bitmap,src_bitmap,pat,
                    x_dst,y_dst,x_src,y_src,(rop_code>>24)<<16);
        }
        else
        {//掩码位图掩码像素为1时，使用前景光栅操作，bit16~23表示前景光栅操作码
        	__gk_set_pixel_rop3_bm(dst_bitmap,src_bitmap,pat,
                    x_dst,y_dst,x_src,y_src,rop_code);
        }                     
    } 
    //alpha混合,一个窗口只允许有一种alpha
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
        //允许alpha混合，bit0~7=alpha
        if((rop_code & cn_rop_alpha_en) != 0)
        {
            fore = rop_code & 0xff; //bit0~7为前景alpha值
            back = 255-fore;        //前景alpha+背景alpha=255
            //进行alpha混合后的颜色
            color = __gk_rop_alpha(dst_bitmap,src_bitmap,x_dst,y_dst,
                                	x_src,y_src,fore,back);
            //绘制像素
            __gk_set_pixel_rop2_bm(dst_bitmap,x_dst,y_dst,
                                	color,cn_r2_copypen);
        }
        //允许双alpha混合，bit0~7=前景alpha，bit16~23=背景alpha
        else if((rop_code & cn_rop_double_alpha) != 0)                                            
        {
            fore = rop_code & 0xff;         //bit0~7为前景alpha值
            back = (rop_code>>16) & 0xff;   //bit16~23为背景alpha值
            //进行alpha混合后的颜色
            color = __gk_rop_alpha(dst_bitmap,src_bitmap,x_dst,y_dst,
                                	x_src,y_src,fore,back);
            //绘制像素
            __gk_set_pixel_rop2_bm(dst_bitmap,x_dst,y_dst,
                                	color,cn_r2_copypen);
        }
        //逐点alpha混合，窗体像素格式只能是cn_sys_pf_a8r8g8b8
        else if((rop_code & cn_rop_pbp_alpha) != 0)     
        {
            if(src_bitmap->pf_type == cn_sys_pf_a8r8g8b8)      
            {
                fore = __gk_get_pixel_bm(src_bitmap,x_src,y_src);
                fore = fore>>24;
                back = 255-fore;        //前景alpha+背景alpha=255
                //进行alpha混合后的颜色
                color = __gk_rop_alpha(dst_bitmap,src_bitmap,x_dst,y_dst,
                                        x_src,y_src,fore,back);
                //绘制像素
                __gk_set_pixel_rop2_bm(dst_bitmap,x_dst,y_dst,
                                        color,cn_r2_copypen);
            }
            else
                return;  
        }
    }
}
//----在内存画点(执行二元光栅操作)---------------------------------------------
//功能: 在一个bitmap上画点，执行二元光栅操作，如果bitmap是逐点alpha的，对alpha
//      域不予理会。
//参数: bitmap，目标位图
//      x、y，画图坐标
//      color，颜色，bitmap的兼容格式
//      rop2_code，二元光栅操作码
//返回: 无
//-----------------------------------------------------------------------------
void __gk_set_pixel_rop2_bm(struct st_rect_bitmap *bitmap,
                        s32 x,s32 y,u32 color,u32 rop2_code)
{
    u8 bit_offset;
    u8 color_bits;
    u32 byte_offset;
    u32 pf,alpha;
    color_bits = bitmap->pf_type >>8;
    //根据位图颜色格式在内存上画点
    switch(color_bits)
    {
        case 1:
        {
            byte_offset = y*bitmap->linebytes + x/8;//点对应坐标的字节偏移量
            //点对应坐标在字节中的位偏移量高位在右边，低位在左
            bit_offset = 7-x%8; 
            //目标位图上对应坐标的颜色
            pf = (u32)bitmap->bm_bits[byte_offset];
            //进行二元光栅操作后的颜色
            pf = __gk_blend_rop2(pf,color<<bit_offset,rop2_code)
                    &(0x1<<bit_offset);
            //保留除目标位的，其它位颜色
            bitmap->bm_bits[byte_offset] &= ~(1<<bit_offset);
            //将混合后的颜色添加到相应位
            bitmap->bm_bits[byte_offset] |= (u8)pf;
        }break;
        case 2:
        {
            byte_offset = y*bitmap->linebytes + x/4;//点对应坐标的字节偏移量
            //点对应坐标在字节中的位偏移量高位在右边，低位在左
            bit_offset = (3-x%4)<<1;   
            //目标位图上对应坐标的颜色
            pf = (u32)bitmap->bm_bits[byte_offset];
            //进行二元光栅操作后的颜色
            pf = __gk_blend_rop2(pf,color<<bit_offset,rop2_code)
                    &(0x3<<bit_offset);
            //保留除目标位的，其它位颜色
            bitmap->bm_bits[byte_offset] &= ~(3<<bit_offset);
            //将混合后的颜色添加到相应位
            bitmap->bm_bits[byte_offset] |= (u8)pf;
        }break;
        case 4:
        {
            byte_offset = y*bitmap->linebytes + x/2;//点对应坐标的字节偏移量
            //点对应坐标在字节中的位偏移量高位在右边，低位在左
            bit_offset = (1-x%2)<<2;   
            //目标位图上对应坐标的颜色
            pf = (u32)bitmap->bm_bits[byte_offset];
            //进行二元光栅操作后的颜色
            pf = __gk_blend_rop2(pf,color<<bit_offset,rop2_code)
                    &(0xf<<bit_offset);
            //保留除目标位的，其它位颜色
            bitmap->bm_bits[byte_offset] &= ~(0xf<<bit_offset);
            //将混合后的颜色添加到相应位
            bitmap->bm_bits[byte_offset] |= (u8)pf;
        }break;
        case 8:            
        {
            byte_offset = y*bitmap->linebytes + x;//点对应坐标的字节偏移量
            //进行二元光栅操作后的颜色
            pf = __gk_blend_rop2((u32)bitmap->bm_bits[byte_offset],
                    color,rop2_code);
            bitmap->bm_bits[byte_offset] = (u8)pf;//将混合后的颜色添加到相应位
        }break;
        case 12:
        {
            byte_offset = y*bitmap->linebytes + x*3/2;//点对应坐标的字节偏移量
            bit_offset = (x*12)%8;              //点对应坐标在字节中的位偏移量
            //注:因对齐问题，不可把目标地址强制转换成16位指针。
            pf = (u32)bitmap->bm_bits[byte_offset] 
                  + (u32)(bitmap->bm_bits[byte_offset]<<8);
            pf = pf>>bit_offset;
            pf = __gk_blend_rop2(pf,color,rop2_code);//进行二元光栅操作后的颜色
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
            byte_offset = y*bitmap->linebytes + x*2;//点对应坐标的字节偏移量
            //目标位图上对应坐标的颜色
            pf = (u32)(*(u16*)(&bitmap->bm_bits[byte_offset]));
//            alpha = pf;
            //进行二元光栅操作后的颜色
            pf = __gk_blend_rop2(pf,color,rop2_code);
//            if(bitmap->pf_type == cn_sys_pf_a1r5g5b5)
//            {
//                pf &= 0x7fff;
//                pf |= alpha & 0x8000;
//            }
            //注:因对齐问题，不可把目标地址强制转换成16位指针。
            bitmap->bm_bits[byte_offset] = (u8)pf;
            bitmap->bm_bits[byte_offset+1] = (u8)(pf>>8);
        }break;
        case 24:
        {
            byte_offset = y*bitmap->linebytes + x*3;//点对应坐标的字节偏移量
            pf = (u32)bitmap->bm_bits[byte_offset];
            pf += (u32)bitmap->bm_bits[byte_offset+1]<<8;
            pf += (u32)bitmap->bm_bits[byte_offset+2]<<16;
            //进行二元光栅操作后的颜色
            pf = __gk_blend_rop2(pf,color,rop2_code);
            bitmap->bm_bits[byte_offset] = (u8)pf;
            bitmap->bm_bits[byte_offset+1] = (u8)(pf >> 8);
            bitmap->bm_bits[byte_offset+2] = (u8)(pf >> 16);
        }break;
        case 32:
        {
            byte_offset = y*bitmap->linebytes + x*4;//点对应坐标的字节偏移量
            pf = (u32)(*(u32*)(&bitmap->bm_bits[byte_offset]));
            alpha = pf;
            //进行二元光栅操作后的颜色
            pf = __gk_blend_rop2(pf,color,rop2_code);
            if(bitmap->pf_type == cn_sys_pf_a8r8g8b8)
            {
                pf &= 0x00ffffff;
                pf |= alpha & 0xff000000;
            }
            //注:因对齐问题，不可把目标地址强制转换成32位指针。
            bitmap->bm_bits[byte_offset] = (u8)pf;
            bitmap->bm_bits[byte_offset+1] = (u8)(pf >> 8);
            bitmap->bm_bits[byte_offset+2] = (u8)(pf >> 16);
            bitmap->bm_bits[byte_offset+3] = (u8)(pf >> 24);
        }break;
    }
}
//----在内存画点(执行三元光栅操作)---------------------------------------------
//功能: 在一个bitmap上画点，执行三元光栅操作，如果bitmap是逐点alpha的，对alpha
//      域不予理会。
//参数: dst_bitmap，目标位图
//      src_bitmap，源位图
//      pat，pattern指针
//      x_dst、y_dst，目标位图中的目标坐标
//      x_src、y_src，源位图中的目标坐标
//      rop3_code，三元光栅操作码
//返回: 无
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
    //目标位图颜色格式
    color_bits = dst_bitmap->pf_type >>8;
    //取源位图颜色，源位图颜色格式不固定
    pf_src = __gk_get_pixel_bm(src_bitmap,x_src,y_src);
    //将取出的源位图颜色进行颜色格式转换，得到与目标位图颜色格式一致的颜色
    pf_src = __gk_convert_part_pf_to_pf(dst_bitmap->pf_type,
                                        src_bitmap->pf_type,pf_src);
    switch(color_bits)
    {
        case 1:
        {
            //目标位图中坐标的字节偏移量
            byte_offset_dst= y_dst*dst_bitmap->linebytes + x_dst/8;
            //目标位图中坐标在字节中的位偏移量，高位在右边，低位在左
            dst_bit_offset = 7-x_dst%8;
            //图案位图中坐标在字节中的位偏移量，高位在右边，低位在左
            pat_bit_offset = 7-x_src%8;
            //图案位图中坐标的字节偏移量
            byte_offset_pat = y_src%8; 
            //目标位图中坐标所在字节的像素(低一位)
            temp = (u32)dst_bitmap->bm_bits[byte_offset_dst];
            pf = temp>>dst_bit_offset;    //目标位图中的目标像素
            //图案位图中的目标像素(低一位)
            pf_pat = (u32)pat[byte_offset_pat]>>pat_bit_offset;
            //rop3混合
            pf =__gk_blend_rop3(pf,pf_src,pf_pat,(u8)(rop3_code>>16));
            dst_bitmap->bm_bits[byte_offset_dst] =
                            ( temp
                                  & (~(1<<dst_bit_offset)))
                             | ( (u8)((pf&1)<<dst_bit_offset));//画像素
        }break;
        case 2:
        {
            //目标位图中坐标的字节偏移量
            byte_offset_dst= y_dst*dst_bitmap->linebytes + x_dst/4;
            //目标位图中坐标在字节中的位偏移量，高位在右边，低位在左
            dst_bit_offset = (3-x_dst%4)<<1;
            //图案位图中坐标在字节中的位偏移量，高位在右边，低位在左
            pat_bit_offset = (3-x_src%4)<<1;
            //图案位图中坐标的字节偏移量
            byte_offset_pat = (y_src%8)*2+(x_src%8)/4; 
            //目标位图中的目标像素(低两位)
            pf = (u32)dst_bitmap->bm_bits[byte_offset_dst]>>dst_bit_offset;
            //图案位图中的目标像素(低两位)
            pf_pat = (u32)pat[byte_offset_pat]>>pat_bit_offset;
            //rop3混合
            pf =__gk_blend_rop3(pf,pf_src,pf_pat,(u8)(rop3_code>>16));
            //画像素
            dst_bitmap->bm_bits[byte_offset_dst] &= ~(3<<dst_bit_offset);
            dst_bitmap->bm_bits[byte_offset_dst] |= (u8)(pf&3)<<dst_bit_offset;
        }break;
        case 4:
        {
            //目标位图中坐标的字节偏移量
            byte_offset_dst= y_dst*dst_bitmap->linebytes + x_dst/2;
            //目标位图中坐标在字节中的位偏移量，高位在右边，低位在左
            dst_bit_offset = (1-x_dst%2)<<2;
            //图案位图中坐标在字节中的位偏移量，高位在右边，低位在左
            pat_bit_offset = (1-x_src%2)<<2;
            //图案位图中坐标的字节偏移量
            byte_offset_pat = (y_src%8)*4+(x_src%8)/2; 
            //目标位图中的目标像素(低四位)
            pf = (u32)dst_bitmap->bm_bits[byte_offset_dst]>>dst_bit_offset;
            //图案位图中的目标像素(低四位)
            pf_pat = (u32)pat[byte_offset_pat]>>pat_bit_offset;
            //rop3混合
            pf =__gk_blend_rop3(pf,pf_src,pf_pat,(u8)(rop3_code>>16));
            //画像素
            dst_bitmap->bm_bits[byte_offset_dst] &= ~(0x0f<<dst_bit_offset);
            dst_bitmap->bm_bits[byte_offset_dst] |= (u8)(pf&0x0f)<<dst_bit_offset;
        }break;
        case 8:
        {
            //目标位图中坐标的字节偏移量
            byte_offset_dst= y_dst*dst_bitmap->linebytes + x_dst;
            //图案位图中坐标的字节偏移量
            byte_offset_pat = (y_src%8)*8+x_src%8; 
            //目标位图中的目标像素
            pf = (u32)dst_bitmap->bm_bits[byte_offset_dst];
            //图案位图中的目标像素
            pf_pat = (u32)pat[byte_offset_pat];
            //rop3混合
            pf=__gk_blend_rop3(pf,pf_src,pf_pat,(u8)(rop3_code>>16));
            //画像素
            dst_bitmap->bm_bits[byte_offset_dst] = (u8)pf;
        }break;
        case 12:
        {
            //目标位图中坐标的字节偏移量
            byte_offset_dst= y_dst*dst_bitmap->linebytes + x_dst*3/2;
            //目标位图中坐标在字节中的位偏移量，高位在右边，低位在左
            dst_bit_offset = (x_dst*12)%8;
            //图案位图中坐标在字节中的位偏移量，高位在右边，低位在左
            pat_bit_offset = (x_src*12)%8;
            //目标位图中坐标的字节偏移量
            byte_offset_pat = (y_src%8)*12 + ((x_src%8)*12)/8; 
            //注:因对齐问题，不可把目标地址强制转换成16位指针。
            //图案位图中的目标像素
            pf_pat = (u32)pat[byte_offset_pat]+(u32)(pat[byte_offset_pat]<<8);
            pf_pat = pf_pat>>pat_bit_offset;
            //目标位图中的目标像素
            pf = (u32)dst_bitmap->bm_bits[byte_offset_dst] 
                  + (u32)(dst_bitmap->bm_bits[byte_offset_dst]<<8);
            pf = pf>>dst_bit_offset;
            //rop3混合
            pf = __gk_blend_rop3(pf,pf_src,pf_pat,(u8)(rop3_code>>16));
            //画像素
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
            //目标位图中坐标的字节偏移量
            byte_offset_dst = y_dst*dst_bitmap->linebytes + x_dst*2;
            //图案位图中坐标的字节偏移量
            byte_offset_pat = ((y_src%8)*16) + (x_src%8)*2;
            //图案位图中的目标像素
            pf_pat = (u32)(*(u16*)(&pat[byte_offset_pat]));
            //目标位图中的目标像素
            pf = (u32)(*(u16*)(&dst_bitmap->bm_bits[byte_offset_dst]));            
//            alpha = pf;
            //rop3混合
            pf = __gk_blend_rop3(pf,pf_src,pf_pat,(u8)(rop3_code>>16));
//            if(dst_bitmap->pf_type == cn_sys_pf_a1r5g5b5)
//            {
//                pf &= 0x7fff;
//                pf |= alpha & 0x8000;
//            }
            //注:因对齐问题，不可把目标地址强制转换成16位指针。
            //画像素
            dst_bitmap->bm_bits[byte_offset_dst] = (u8)pf;
            dst_bitmap->bm_bits[byte_offset_dst+1] = (u8)(pf>>8);
        }break;
        case 24:
        {
            //目标位图中坐标的字节偏移量
            byte_offset_dst = y_dst*dst_bitmap->linebytes + x_dst*3;
            //图案位图中坐标的字节偏移量
            byte_offset_pat = ((y_src%8)*24) + (x_src%8)*3;
            //图案位图中的目标像素
            pf_pat = (u32)pat[byte_offset_pat];
            pf_pat += (u32)(pat[byte_offset_pat+1]<<8);
            pf_pat += (u32)(pat[byte_offset_pat+2]<<16);
            //目标位图中的目标像素
            pf = (u32)dst_bitmap->bm_bits[byte_offset_dst];
            pf += (u32)dst_bitmap->bm_bits[byte_offset_dst+1]<<8;
            pf += (u32)dst_bitmap->bm_bits[byte_offset_dst+2]<<16;
            //rop3混合
            pf = __gk_blend_rop3(pf,pf_src,pf_pat,(u8)(rop3_code>>16));
            //画像素
            dst_bitmap->bm_bits[byte_offset_dst] = (u8)pf;
            dst_bitmap->bm_bits[byte_offset_dst+1] = (u8)(pf >> 8);
            dst_bitmap->bm_bits[byte_offset_dst+2] = (u8)(pf >> 16);
        }break;
        case 32:
        {
            //目标位图中坐标的字节偏移量
            byte_offset_dst= y_dst*dst_bitmap->linebytes + x_dst*4;
            //图案位图中坐标的字节偏移量
            byte_offset_pat = ((y_src%8)*32) + (x_src%8)*4;
            //图案位图中的目标像素
            pf_pat = (u32)(*(u32*)(&pat[byte_offset_pat]));
            //目标位图中的目标像素
            pf = (u32)(*(u32*)(&dst_bitmap->bm_bits[byte_offset_dst]));
            alpha = pf;
            //rop3混合
            pf = __gk_blend_rop3(pf,pf_src,pf_pat,(u8)(rop3_code>>16));
            if(dst_bitmap->pf_type == cn_sys_pf_a8r8g8b8)
            {
                pf &= 0x00ffffff;
                pf |= alpha & 0xff000000;
            }
            //注:因对齐问题，不可把目标地址强制转换成32位指针。
            //画像素
            dst_bitmap->bm_bits[byte_offset_dst] = (u8)pf;
            dst_bitmap->bm_bits[byte_offset_dst+1] = (u8)(pf >> 8);
            dst_bitmap->bm_bits[byte_offset_dst+2] = (u8)(pf >> 16);
            dst_bitmap->bm_bits[byte_offset_dst+3] = (u8)(pf >> 24);
        }break;
    }
}
//----画点---------------------------------------------------------------------
//功能: 在窗口内画一个像素点。
//参数: 参数是结构para的成员
//      gkwin，目标窗口指针
//      x、y，画图坐标
//      color，画点使用的颜色
//      rop2_code，二元光栅操作码
//返回: 无
//-----------------------------------------------------------------------------
void gk_set_pixel(struct gksc_para_set_pixel *para)
{
    struct clip_rect *clip;
    struct gkwin_rsc *fb_gkwin;
    struct st_rect_bitmap *bitmap;
    u32 pf_color;
    s32 offsetx,offsety;
    if((para->rop2_code == cn_r2_nop) || (para->rop2_code > cn_r2_last))
        return;                                     //不执行操作
    //说明有win buffer，且直接写屏属性为false
    if((para->gkwin->wm_bitmap.bm_bits != NULL)
        && (para->gkwin->direct_screen == false))     
    {   //处理方法:在win buffer中绘图，标志changed_msk
        bitmap = &para->gkwin->wm_bitmap;
        //若显示器使用cn_custom_pf格式，应该由硬件实现绘制像素
        if(bitmap->pf_type & cn_custom_pf)
        {
            para->gkwin->disp->draw.set_pixel_bm(bitmap,para->x,para->y,
                                                 para->color,para->rop2_code);
        }else//由软件实现绘制像素
        {
            //规定调用者给定的颜色需是真彩色
            pf_color = __gk_convert_color_to_pf(bitmap->pf_type,para->color);
            __gk_set_pixel_rop2_bm(bitmap,para->x,para->y,
                                pf_color,para->rop2_code);      //绘制像素
        }
        __gk_shading_pixel(para->gkwin,para->x,para->y);//标志像素的changed_msk
    }else
    {
        clip = para->gkwin->visible_clip;
        if(clip == NULL)        //可视域为空，直接返回
            return ;
        fb_gkwin = para->gkwin->disp->frame_buffer;
        offsetx = para->gkwin->absx0 +para->x;
        offsety = para->gkwin->absy0 +para->y;
        //有frame buffer，且直接写屏属性为false
        if((fb_gkwin != NULL) && (para->gkwin->direct_screen == false))         
        {   //处理方法:在frame buffer中绘图，但只绘gkwin中的可视区域
            bitmap = &fb_gkwin->wm_bitmap;
            do{
                //像素对应的坐标在可视域内，则绘制像素
                if(!((offsetx<clip->rect.left) || (offsetx>=clip->rect.right)
                    || (offsety<clip->rect.top) ||(offsety>=clip->rect.bottom)))
                {   
                    //若显示器使用cn_custom_pf格式，应该由硬件driver实现绘制像素
                    if(bitmap->pf_type & cn_custom_pf)
                    {
                        fb_gkwin->disp->draw.set_pixel_bm(bitmap,offsetx,
                                           offsety,para->color,para->rop2_code);
                    }else//由软件实现绘制像素
                    {
                        //规定调用者给定的颜色颜色格式需是真彩色
                        pf_color = __gk_convert_color_to_pf(
                                                bitmap->pf_type,para->color);
                        __gk_set_pixel_rop2_bm(bitmap,offsetx,offsety,
                                                pf_color,para->rop2_code);
                    }
                    //标志像素的changed_msk
                    __gk_shading_pixel(fb_gkwin,offsetx,offsety);
                    break;      //像素已经绘制，退出循环
                }else//像素对应的坐标不在可视域内，指向下一个可视域
                    clip = clip->next;
            }while(clip != para->gkwin->visible_clip);
        }
        else
        //无win buffer，也无frame buffer，直接画在screen上
        //直接写屏属性为true，不管有无缓冲区，都直接画在screen上
        {
            do{
                //像素对应的坐标在可视域内，则绘制像素
                if(!((offsetx<clip->rect.left) || (offsetx>=clip->rect.right)
                    || (offsety<clip->rect.top) ||(offsety>=clip->rect.bottom)))
                {
                    __gk_set_pixel_screen(para->gkwin->disp,offsetx,offsety,
                                          para->color,para->rop2_code);
                    break;      //像素已经绘制，退出循环
                }else//像素对应的坐标不在可视域内，指向下一个可视域
                    clip = clip->next;
            }while(clip != para->gkwin->visible_clip);
        }
    }
}

//----设置changed_msk:像素-----------------------------------------------------
//功能: 绘制像素时，设置窗口的changed_msk，把改写过的区域着色，由调用者确保坐标
//      合法。
//参数: gkwin，目标窗口指针
//      x、y，目标像素对应的坐标
//返回: 无
//-----------------------------------------------------------------------------
void __gk_shading_pixel(struct gkwin_rsc *gkwin,s32 x,s32 y)
{
    s32 msk_x,msk_y;    
    if(gkwin->change_flag == cn_gkwin_change_all)   //已经全部着色
    {
        return;
    }
    gkwin->change_flag = cn_gkwin_change_part;
    msk_x = x/8;       //计算x在msk位图中第几列
    msk_y = y/8;       //计算y在msk位图中第几行
    //将像素所在的changed_msk中的bit置为1
    __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),msk_x,msk_y,
                                cn_color_white,cn_r2_copypen);
    return;
}

//----设置changed_msk----------------------------------------------------------
//功能: 把一个窗口的rect限定的区域设置为被改写
//参数: bitmap，目标bitmap
//      rect，改写区域
//返回: 无
//说明: 内部函数，不检查坐标合法性，由调用者保证坐标的合法性
//-----------------------------------------------------------------------------
void __gk_shading_rect(struct gkwin_rsc *gkwin,struct st_rect *rect)
{
    struct st_rect_bitmap *bitmap;
    s32 x,y;
    s32 msk_x1,msk_x2,msk_y1,msk_y2;

    if(gkwin->change_flag == cn_gkwin_change_all)   //已经全部着色
    {
        return;
    }
    bitmap = &gkwin->wm_bitmap;
    if((rect->left <= 0) && (rect->top <= 0) 
            && (rect->right >= bitmap->width)&&(rect->bottom >= bitmap->height))
    {
        gkwin->change_flag = cn_gkwin_change_all;   //被msk的区域覆盖整个窗口
        return;
    }
    
    gkwin->change_flag = cn_gkwin_change_part;
    msk_x1 = rect->left/8;          //计算x1在msk位图中第几列
    msk_x2 = (rect->right-1)/8;     //计算x2在msk位图中第几列，不包含终点，-1
    msk_y1 = rect->top/8;           //计算y1在msk位图中第几行
    msk_y2 = (rect->bottom-1)/8;    //计算y2在msk位图中第几行，不包含终点，-1

    for(y = msk_y1;y <= msk_y2;y++)
    {
        for(x = msk_x1;x <= msk_x2;x++)
            __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),x,y,
                                cn_color_white,cn_r2_copypen);
    }
}

//----设置changed_msk----------------------------------------------------------
//功能: 在窗口或frame buffer绘制直线时，设置窗口的changed_msk，
//      把改写过的区域着色。
//参数: gkwin，目标窗口
//      limit，允许绘图的限制区域，一个矩形的区域，超出此区域的直线不画
//      x1、y1、x2、y2， 起点、终点坐标
//返回: 无
//-----------------------------------------------------------------------------
void __gk_shading_line(struct gkwin_rsc *gkwin,struct st_rect *limit,
                     s32 x1,s32 y1,s32 x2,s32 y2)
{
    if(gkwin->change_flag == cn_gkwin_change_all)   //已经全部着色
    {
        return;
    }
    gkwin->change_flag = cn_gkwin_change_part;
    
    if(y1 == y2)        //水平直线着色
    {
        s32 x;
        s32 msk_x1,msk_x2,msk_y1;
        if(x1>x2)
            __gk_swap(x1,x2);
        if((y1 <= limit->top) || (y1 >= limit->bottom)
            ||(x2 <= limit->left) || (x1 >= limit->right))
            return;         //所绘直线在limit之外
        if(x1 < limit->top)
            x1 = limit->top;
        if(x2 > limit->bottom)
            x2 = limit->bottom;
        msk_x1 = x1/8;     //计算x1在msk位图中第几列
        msk_x2 = (x2-1)/8; //计算(x2-1)在msk位图中第几列，-1是因为终点不包含在内
        msk_y1 = y1/8;     //计算y1在msk位图中第几行
        for(x = msk_x1;x <= msk_x2;x++)
        {
            //将changed_msk相应的bit置1
            __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),x,msk_y1,
                                    cn_color_white,cn_r2_copypen);
        }

    }
    else if(x1 == x2)   //垂直线着色
    {
        s32 y;
        s32 msk_x1,msk_y1,msk_y2;
        if(y1>y2)
            __gk_swap(y1,y2);
        if((x1<limit->left) || (x1 >= limit->right)
            ||(y2 <= limit->top) || (y1 >= limit->bottom))
            return;         //所绘直线在limit之外
        if(y1 < limit->top)
            y1 = limit->top;
        if(y2 > limit->bottom)
            y2 = limit->bottom;
        msk_x1 = x1/8;     //计算x1在msk位图中第几列        
        msk_y1 = y1/8;     //计算y1在msk位图中第几行
        msk_y2 = (y2-1)/8; //计算(y2-1)在msk位图中第几行，-1是因为终点不包含在内
        for(y = msk_y1;y <= msk_y2;y++)
        {
            //将changed_msk相应的bit置1
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

        //x方向变化量大
        if(dx > dy)
        {            
            if(((x1>x2)&&(y1>y2)) || ((x1<x2)&&(y1<y2)))
            {//x、y增减方向相同，即同增或者同减
                //选取x1、x2中较小的数作为x1
                if(x1 > x2)
                {
                    __gk_swap(x1,x2);
                    __gk_swap(y1,y2);
                }
                //向下对齐，取8*8像素，x每变化8像素后y与直线的交点，
                //将斜线上所有的点所在的8*8像素区域对应的
                //在changed_msk中的bit置为1，具体说明请看djygui的书
                for(x = align_down(8,x1); x <= x2; x+=8)
                {
                    //对计算的交点进行四舍五入处理，x、y增减方向相同
                    //所以以0.5为基准量
                    y = ((((x-x1)<<1)*(y2-y1)+(x2-x1))/((x2-x1)<<1))+y1;
                    msk_x = x/8;    //计算x在msk位图中第几列
                    msk_y = y/8;    //计算y在msk位图中第几行
                    //将changed_msk相应的bit置1
                    __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),msk_x,
                                         msk_y,cn_color_white,cn_r2_copypen);
                    //标志(msk_x，msk_y)下面的一个点
                    msk_y += 1;
                    //将changed_msk相应的bit置1
                    __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),msk_x,
                                         msk_y,cn_color_white,cn_r2_copypen);
                }
            }
            else
            {//x、y增减方向相反，即x增、y减，x减、y增
                //选取x1、x2中较小的数作为x1
                if(x1 > x2)
                {
                    __gk_swap(x1,x2);
                    __gk_swap(y1,y2);
                }
                //向下对齐，取8*8像素，x每变化8像素后y与直线的交点，
                //将斜线上所有的点所在的8*8像素区域对应的
                //在changed_msk中的bit置为1，具体说明请看djygui的书
                for(x = align_down(8,x1); x <= x2; x+=8)
                {
                    //对所得交点进行四舍五入处理，y为减方向，所以以-0.5为基准量
                    y = ((((x-x1)<<1)*(y2-y1)-(x2-x1))/((x2-x1)<<1))+y1;
                    msk_x = x/8;    //计算x在msk位图中第几列
                    msk_y = y/8;    //计算y在msk位图中第几行
                    //将changed_msk相应的bit置1
                    __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),msk_x,
                                         msk_y,cn_color_white,cn_r2_copypen);
                    //标志(msk_x，msk_y)上面的一个点
                    msk_y -= 1;
                    //将changed_msk相应的bit置1
                    __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),msk_x,
                                         msk_y,cn_color_white,cn_r2_copypen);
                }
            }
        }else
        {//y方向变化量大，或者x，y变化量相等
            if(((x1>x2)&&(y1>y2)) || ((x1<x2)&&(y1<y2)))
            {//x、y增减方向相同，即同增或者同减   
                //选取y1、y2中较小的数作为y1
                if(y1 > y2)
                {
                    __gk_swap(x1,x2);
                    __gk_swap(y1,y2);
                }
                //向下对齐，取8*8像素，y每变化8像素后x与直线的交点，
                //将斜线上所有的点所在的8*8像素区域对应的
                //在changed_msk中的bit置为1，具体说明请看djygui的书
                for(y = align_down(8,y1); y <= y2; y+=8)
                {
                    x = ((((y-y1)<<1)*(x2-x1)+(y2-y1))/((y2-y1)<<1))+x1;
                    msk_x = x/8;    //计算x在msk位图中第几列
                    msk_y = y/8;    //计算y在msk位图中第几行
                    //将changed_msk相应的bit置1
                    __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),msk_x,
                                         msk_y,cn_color_white,cn_r2_copypen);
                    msk_x += 1;
                    //将changed_msk相应的bit置1
                    __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),msk_x,
                                         msk_y,cn_color_white,cn_r2_copypen);
                }
            }
            else
            {//x、y增减方向相反，即x增、y减，x减、y增
                //选取y1、y2中较小的数作为y1
                if(y1 > y2)
                {
                    __gk_swap(x1,x2);
                    __gk_swap(y1,y2);
                }
                //向下对齐，取8*8像素，y每变化8像素后x与直线的交点，
                //将斜线上所有的点所在的8*8像素区域对应的
                //在changed_msk中的bit置为1，具体说明请看djygui的书
                for(y = align_down(8,y1); y <= y2; y+=8)
                {
                    x = ((((y-y1)<<1)*(x2-x1)-(y2-y1))/((y2-y1)<<1))+x1;
                    msk_x = x/8;    //计算x在msk位图中第几列
                    msk_y = y/8;    //计算y在msk位图中第几行
                    //将changed_msk相应的bit置1
                    __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),msk_x,
                                         msk_y,cn_color_white,cn_r2_copypen);
                    msk_x -= 1;
                    //将changed_msk相应的bit置1
                    __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),msk_x,
                                             msk_y,cn_color_white,cn_r2_copypen);
                }
            }
        }
    }
}

//----清除changed_msk----------------------------------------------------------
//功能: 清除一个窗口的改写标志。
//参数: gkwin，目标窗口指针
//返回: 无
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

//----画垂直线(不含端点)-------------------------------------------------------
//功能: 在窗口内从当前光标处开始画一条垂直线。
//参数: gkwin，目标窗口指针
//      limit，绘制的限制区，只绘制直线在limit矩形内部的部分
//      x1、y1，起始点的坐标
//      y2，结束点的纵坐标
//      color，颜色，cn_sys_pf_e8r8g8b8格式
//      rop2_code，二元光栅操作码
//返回: 无
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
        return;         //所绘直线在limit之外
    bitmap = &gkwin->wm_bitmap;
    //垂直线超出limit的部分不绘制
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
        //用户给定颜色颜色格式为rgb24
        pf_color = __gk_convert_color_to_pf(bitmap->pf_type,color);
        for(y = y1;y < y2;y++)
        {
            __gk_set_pixel_rop2_bm(bitmap,x1,y,pf_color,rop2_code);
        }
    }

    //下面处理changed_msk标志位图，把窗口中因划线而修改的区域着色
    if(gkwin->change_flag != cn_gkwin_change_all)
    {
        gkwin->change_flag = cn_gkwin_change_part;  //标志窗口部分修改
        msk_x1 = x1/8;     //计算x1在msk位图中第几列        
        msk_y1 = y1/8;     //计算y1在msk位图中第几行
        msk_y2 = (y2-1)/8; //计算(y2-1)在msk位图中第几行，-1是因为终点不包含在内
        for(y = msk_y1;y <= msk_y2;y++)
        {   //将changed_msk相应的bit置1
            __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),msk_x1,y,
                                    cn_color_white,cn_r2_copypen);
        }
    }
}

//----画水平直线(不含端点)-------------------------------------------------------
//功能: 在窗口内从当前光标处开始画一条水平直线。
//参数: gkwin，目标窗口指针
//      limit，绘制的限制区，只绘制直线在limit矩形内部的部分
//      x1、y1，起始点的坐标
//      x2，结束点的横坐标
//      color，颜色，cn_sys_pf_e8r8g8b8格式
//      rop2_code，二元光栅操作码
//返回: 无
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
        return;         //所绘直线在limit之外
    bitmap = &gkwin->wm_bitmap;
    //水平线在limit外部分不绘制
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
        //用户给定颜色颜色格式为rgb24
        pf_color = __gk_convert_color_to_pf(bitmap->pf_type,color);
        for(x = x1;x < x2;x++)  
        {//对1、2、4位色screen，这个循环很耗cpu，考虑改进--db
            __gk_set_pixel_rop2_bm(bitmap,x,y1,pf_color,rop2_code);
        }
    }

    //下面处理changed_msk标志位图，把窗口中因划线而修改的区域着色
    if(gkwin->change_flag != cn_gkwin_change_all)
    {
        gkwin->change_flag = cn_gkwin_change_part;  //标志窗口部分修改
        msk_x1 = x1/8;     //计算x1在msk位图中第几列
        msk_x2 = (x2-1)/8; //计算(x2-1)在msk位图中第几列，-1是因为终点不包含在内
        msk_y1 = y1/8;     //计算y1在msk位图中第几行
        for(x = msk_x1;x <= msk_x2;x++)
        {   //将changed_msk相应的bit置1
            __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),x,msk_y1,
                                    cn_color_white,cn_r2_copypen);
        }
    }
}
//----画斜线(不含端点)---------------------------------------------------------
//功能: 在窗口内从当前光标处开始画一条斜线。
//参数: gkwin，目标窗口指针
//      limit，绘制的限制区，只绘制直线在limit矩形内部的部分
//      x1、y1，起始点的坐标
//      x2、y2，结束点的坐标
//      color，颜色，cn_sys_pf_e8r8g8b8格式
//      rop2_code，二元光栅操作码
//返回: 无
//-----------------------------------------------------------------------------
void __gk_olineto_bm(struct gkwin_rsc *gkwin,struct st_rect *limit,//确认
                       s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 rop2_code)
{
    s32 x,y;
    s32 msk_x,msk_y;
    s32 dx,dy;

    if(__gk_oline_sect_inter(limit,&x1,&y1,&x2,&y2))//取得斜线与limit的交点
    {
        __gk_draw_oline(gkwin,x1,y1,x2,y2,color,rop2_code);//绘制斜线

        //下面处理changed_msk标志位图，把窗口中因划线而修改的区域着色
        if(gkwin->change_flag != cn_gkwin_change_all)
        {        
            gkwin->change_flag = cn_gkwin_change_part;//标志窗口部分修改
            dx = abs(x2-x1);
            dy = abs(y2-y1);
            //x方向变化量大
            if(dx > dy)
            {            
                if(((x1>x2)&&(y1>y2)) || ((x1<x2)&&(y1<y2)))
                {//x、y增减方向相同，即同增或者同减
                    //选取x1、x2中较小的数作为x1
                    if(x1 > x2)
                    {
                        __gk_swap(x1,x2);
                        __gk_swap(y1,y2);
                    }
                    //向下对齐，取8*8像素，x每变化8像素后y与直线的交点，
                    //将斜线上所有的点所在的8*8像素区域对应的
                    //在changed_msk中的bit置为1，具体说明请看djygui的书
                    for(x = align_down(8,x1); x <= x2; x+=8)
                    {
                        //对计算的交点进行四舍五入处理，x、y增减方向相同
                        //所以以0.5为基准量
                        y = ((((x-x1)<<1)*(y2-y1)+(x2-x1))/((x2-x1)<<1))+y1;
                        msk_x = x/8;    //计算x在msk位图中第几列
                        msk_y = y/8;    //计算y在msk位图中第几行
                        //将changed_msk相应的bit置1
                        __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),msk_x,
                                             msk_y,cn_color_white,cn_r2_copypen);
                        //标志(msk_x，msk_y)下面的一个点
                        msk_y += 1;
                        //将changed_msk相应的bit置1
                        __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),msk_x,
                                             msk_y,cn_color_white,cn_r2_copypen);
                    }
                }
                else
                {//x、y增减方向相反，即x增、y减，x减、y增
                    //选取x1、x2中较小的数作为x1
                    if(x1 > x2)
                    {
                        __gk_swap(x1,x2);
                        __gk_swap(y1,y2);
                    }
                    //向下对齐，取8*8像素，x每变化8像素后y与直线的交点，
                    //将斜线上所有的点所在的8*8像素区域对应的
                    //在changed_msk中的bit置为1，具体说明请看djygui的书
                    for(x = align_down(8,x1); x <= x2; x+=8)
                    {
                        //对所得交点进行四舍五入处理，y为减方向，所以以-0.5为基准量
                        y = ((((x-x1)<<1)*(y2-y1)-(x2-x1))/((x2-x1)<<1))+y1;
                        msk_x = x/8;    //计算x在msk位图中第几列
                        msk_y = y/8;    //计算y在msk位图中第几行
                        //将changed_msk相应的bit置1
                        __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),msk_x,
                                             msk_y,cn_color_white,cn_r2_copypen);
                        //标志(msk_x，msk_y)上面的一个点
                        msk_y -= 1;
                        //将changed_msk相应的bit置1
                        __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),msk_x,
                                             msk_y,cn_color_white,cn_r2_copypen);
                    }
                }
            }else
            {//y方向变化量大，或者x，y变化量相等
                if(((x1>x2)&&(y1>y2)) || ((x1<x2)&&(y1<y2)))
                {//x、y增减方向相同，即同增或者同减   
                    //选取y1、y2中较小的数作为y1
                    if(y1 > y2)
                    {
                        __gk_swap(x1,x2);
                        __gk_swap(y1,y2);
                    }
                    //向下对齐，取8*8像素，y每变化8像素后x与直线的交点，
                    //将斜线上所有的点所在的8*8像素区域对应的
                    //在changed_msk中的bit置为1，具体说明请看djygui的书
                    for(y = align_down(8,y1); y <= y2; y+=8)
                    {
                        x = ((((y-y1)<<1)*(x2-x1)+(y2-y1))/((y2-y1)<<1))+x1;
                        msk_x = x/8;    //计算x在msk位图中第几列
                        msk_y = y/8;    //计算y在msk位图中第几行
                        //将changed_msk相应的bit置1
                        __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),msk_x,
                                             msk_y,cn_color_white,cn_r2_copypen);
                        msk_x += 1;
                        //将changed_msk相应的bit置1
                        __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),msk_x,
                                             msk_y,cn_color_white,cn_r2_copypen);
                    }
                }
                else
                {//x、y增减方向相反，即x增、y减，x减、y增
                    //选取y1、y2中较小的数作为y1
                    if(y1 > y2)
                    {
                        __gk_swap(x1,x2);
                        __gk_swap(y1,y2);
                    }
                    //向下对齐，取8*8像素，y每变化8像素后x与直线的交点，
                    //将斜线上所有的点所在的8*8像素区域对应的
                    //在changed_msk中的bit置为1，具体说明请看djygui的书
                    for(y = align_down(8,y1); y <= y2; y+=8)
                    {
                        x = ((((y-y1)<<1)*(x2-x1)-(y2-y1))/((y2-y1)<<1))+x1;
                        msk_x = x/8;    //计算x在msk位图中第几列
                        msk_y = y/8;    //计算y在msk位图中第几行
                        //将changed_msk相应的bit置1
                        __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),msk_x,
                                             msk_y,cn_color_white,cn_r2_copypen);
                        msk_x -= 1;
                        //将changed_msk相应的bit置1
                        __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),msk_x,
                                                 msk_y,cn_color_white,cn_r2_copypen);
                    }
                }
            }
        }
    }
}
//----画直线(不含端点)---------------------------------------------------------
//功能: 在窗口上画直线，端点不画，须处理changed_msk区。
//参数: gkwin，目标窗口指针
//      limit，允许绘图的限制区域，一个矩形的区域,超出此区域的直线不画
//      x1、y1、x2、y2，起点和终点坐标
//      color，绘图所用的颜色
//      rop2_code，二元光栅操作码
//返回: 无
//-----------------------------------------------------------------------------
void __gk_lineto_bm(struct gkwin_rsc *gkwin,struct st_rect *limit,
                    s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 rop2_code)
{
    if(y1 == y2)        //绘水平线
        __gk_hlineto_bm(gkwin,limit,x1,y1,x2,color,rop2_code);
    else if(x1 == x2)   //绘垂直线
        __gk_vlineto_bm(gkwin,limit,x1,y1,y2,color,rop2_code);
    else                //绘斜线
        __gk_olineto_bm(gkwin,limit,x1,y1,x2,y2,color,rop2_code);
}

//----在screen上画像素---------------------------------------------------------
//功能: 在screen上直接画点。
//参数: gkwin，绘制的目标窗口
//      x、y， 坐标
//      color，绘图所用的颜色，cn_sys_pf_e8r8g8b8格式
//      r2_code，二元光栅操作码
//返回: 无
//-----------------------------------------------------------------------------
void __gk_set_pixel_screen(struct display_rsc *display,s32 x,s32 y,
                           u32 color,u32 rop2_code)
{
    struct display_rsc *mirror,*current;

    display->draw.set_pixel_screen(x,y,color,rop2_code);//在screen上画点
    mirror = (struct display_rsc *)display->node.child;
    if(mirror == NULL)      //没有镜像显示器，直接返回
        return;
    current = mirror;
    while(current != NULL)
    {
        current->draw.set_pixel_screen(x,y,color,rop2_code);
        current = (struct display_rsc *)rsc_trave_scion(&mirror->node,
                &current->node);
    }
}

//----画直线(不含端点)---------------------------------------------------------
//功能: 在screen上直接画直线，端点不画。
//参数: display，绘制的目标显示器
//      limit，允许绘图的限制区域，一个矩形的区域，超出此区域的直线不画
//      x1、y1、x2、y2，起点和终点坐标
//      color，绘图所用的颜色，cn_sys_pf_e8r8g8b8格式
//      r2_code，二元光栅操作码
//返回: 无
//-----------------------------------------------------------------------------
void __gk_lineto_screen(struct display_rsc *display,struct st_rect *limit,
                   s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 rop2_code)
{
    struct display_rsc *mirror,*current;
    s32 x,y,ax,ay,s1,s2,p,interchange,i;

    if(display->draw.line_screen(limit, x1, y1, x2, y2, color,rop2_code))
    {
    }else   //硬件加速不支持直线绘制，软件实现
    {
        if(y1 == y2)    //绘水平线
        {
            if(x1 > x2)
                __gk_swap(x1,x2);
            if((y1 < limit->top) || (y1 >= limit->bottom)
                ||(x2 <= limit->left) || (x1 >= limit->right))
                return;         //所绘直线在limit之外
            if(x1 < limit->left)
                x1 = limit->left;
            if(x2 > limit->right)
                x2 = limit->right;
            for(x = x1; x < x2;x++)
            {
                display->draw.set_pixel_screen(x,y1,color,rop2_code);
            }
        }else if(x1 == x2)   //绘垂直线
        {
            if(y1 > y2)
                __gk_swap(y1,y2);          
            if((x1 < limit->left) || (x1 >= limit->right)
                ||(y2 <= limit->top) || (y1 >= limit->bottom))
                return;         //所绘直线在limit之外
            if(y1 < limit->top)
                y1 = limit->top;
            if(y2 > limit->bottom)
                y2 = limit->bottom;
            for(y = y1; y < y2;y++)
            {
                display->draw.set_pixel_screen(x1,y,color,rop2_code);
            }
        }else            //绘斜线
        {
            if( __gk_oline_sect_inter(limit,&x1,&y1,&x2,&y2))
            {
                x = x1;
                y = y1;
                ax = abs(x2-x1);
                ay = abs(y2-y1);
                if(x2 > x1)   //绘制斜线方向
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
                p = (ay<<1)-ax;   //设置初始误差判别值
                for(i = 1;i <= ax;i++)
                {
                    display->draw.set_pixel_screen(x,y,color,rop2_code);

                    if(p >= 0)     //取离数学点较近的像素
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
    if(mirror == NULL)      //没有镜像显示器，直接返回
        return;
    current = mirror;
    while(current != NULL)
    {
        current->draw.line_screen(limit, x1, y1, x2, y2, color,rop2_code);
        current = (struct display_rsc *)rsc_trave_scion(&mirror->node,
                &current->node);
    }    
}
//----传送bitmap到screen-------------------------------------------------------
//功能: 把bitmap的部分从内存传送到screen。
//参数: gkwin，绘制的目标窗口
//      rect，绘图区域，一个矩形的区域，超出此区域的部分不画
//      bitmap，绘制的位图
//      x、y，bitmap要传送部分左上角坐标
//返回: 无
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
    else//bitmap颜色格式与显示屏不一样或者硬件加速不支持由软件实现
    {
        //传送bitmap到screen
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
    if(mirror == NULL)      //没有镜像显示器，直接返回
        return;
    current = mirror;
    while(current != NULL)
    {
        current->draw.bm_to_screen(rect,bitmap,x,y);
        current = (struct display_rsc *)rsc_trave_scion(&mirror->node,
                                                        &current->node);
    }
}

//----填充矩形-----------------------------------------------------------------
//功能: 在dst_bitmap中dst_rect所占据的位置上，用color颜色填充。color的格式是
//      cn_sys_pf_e8r8g8b8，要转换成dst_bitmap->pf_type匹配的颜色填充。
//参数: dst_bitmap，被填充的位图
//      dst_rect，填充的位置
//      color，填充的颜色，格式是cn_sys_pf_e8r8g8b8
//返回: 无
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
    //color的格式是cn_sys_pf_e8r8g8b8，转换成dst_bitmap->pf_type匹配的格式
    pf_color = __gk_convert_color_to_pf(dst_bitmap->pf_type,color);
    color_bits = dst_bitmap->pf_type >>8;   //被填充的位图的颜色格式
    start_byte = 0;
    end_byte = 0;
    switch(color_bits)
    {
        case 1:
        {    
            pf = pf_color & 0x01;               //取给定的填充颜色
            byte_offset1 = y1*temp_bytes+x1/8;  //dst_rect首行左边界所在字节
            byte_offset2 = y1*temp_bytes+x2/8;  //dst_rect首行右边界所在字节
            bit_offset1 = 7-x1%8;   //dst_rect左边界在所在字节的位，x=0，bit=7
            bit_offset2 = x2%8;     //7减去dst_rect右边界在所在字节的位，7-bit
            //dst_rect首行的首字节与尾字节不在一个字节内
            if(byte_offset1 != byte_offset2)
            {
                //dst_rect每行的首字节各位的颜色
                //首字节取颜色的位数与bit相同，右边是高位
                for(i = 0;i <= bit_offset1;i++) 
                    start_byte |= pf<<i;
                for(i = 0;i <= bit_offset2;i++) //dst_rect每行的尾字节各位的颜色
                    end_byte |= pf<<(7-i);      //首字节取颜色的位数与7-bit相同
                //取每行中间字节的颜色，单色只有0、1两种情况
                if(pf == 1) 
                    middle_byte = 0xff;
                else
                    middle_byte = 0;
                for(y = y1;y < y2;y++)  //填充颜色
                {   
                    //填充dst_rect每行中间字节的颜色
                    for(x = byte_offset1+1; x < byte_offset2; x++)
                    {
                        dst_bitmap->bm_bits[x] = middle_byte;
                    }
                    dst_bitmap->bm_bits[byte_offset1] 
                                            &= (0xff>>bit_offset1)<<bit_offset1;
                    dst_bitmap->bm_bits[byte_offset1] 
                                            |= start_byte;  //字节头赋值 
                    dst_bitmap->bm_bits[byte_offset2] 
                                            &= 0xff>>(bit_offset2+1);   
                    dst_bitmap->bm_bits[byte_offset2] 
                                            |= end_byte;    //字节尾赋值 
                    //字节数换行
                    byte_offset1 += temp_bytes;
                    byte_offset2 += temp_bytes;
                }
            }
            else//dst_rect首行的首字节与尾字节在一个字节内
            {   //bit1在左，bit2在右，右边为高位，此处的bit_offset2=7-bit2
                for(i = 7-bit_offset2;i <= bit_offset1;i++)
                {
                    end_byte |= 1<<i;
                    start_byte |= pf<<i;
                }
                for(y = y1;y < y2;y++)
                {   //保留目标字节内不需要填充的位
                    dst_bitmap->bm_bits[byte_offset1] &= ~end_byte;
                    //填充相应的位
                    dst_bitmap->bm_bits[byte_offset1] |= start_byte;
                    byte_offset1 += temp_bytes; //字节数换行
                }
            }
        }break;
        case 2:
        {
            pf = pf_color & 0x03;               //取给定的填充颜色
            byte_offset1 = y1*temp_bytes+x1/4;  //dst_rect首行左边界所在字节
            byte_offset2 = y1*temp_bytes+x2/4;  //dst_rect首行右边界所在字节
            bit_offset1 = (3-x1%4)<<1;//dst_rect左边界在所在字节的位，x=0，bit=7
            bit_offset2 = (x2%4)<<1;//7减去dst_rect右边界在所在字节的位，7-bit
            middle_byte = (u8)((pf<<6)|(pf<<4)|(pf<<2)|pf);//每行中间字节的颜色
            //dst_rect首行的首字节与尾字节不在一个字节内
            if(byte_offset1 != byte_offset2)
            {   //dst_rect每行的首字节各位的颜色
                //首字节取颜色的位数与bit相同，右边是高位，位数是2的倍数
                for(i = 0;i <= bit_offset1;i+=2)
                    start_byte |= pf<<i;
                //dst_rect每行的尾字节各位的颜色，首字节取颜色的位数与7-bit相同
                for(i = 0;i <= bit_offset2;i+=2)
                    end_byte |= pf<<(6-i);
                for(y = y1;y < y2;y++)  //填充颜色
                {
                    //填充dst_rect每行中间字节的颜色
                    for(x = byte_offset1+1; x < byte_offset2; x++)
                    {
                        dst_bitmap->bm_bits[x] = middle_byte;
                    }
                    dst_bitmap->bm_bits[byte_offset1] 
                                            &= (0xff>>bit_offset1)<<bit_offset1;
                    dst_bitmap->bm_bits[byte_offset1] |= start_byte;//字节头赋值 
                    dst_bitmap->bm_bits[byte_offset2] &= 0xff>>(bit_offset2+1);
                    dst_bitmap->bm_bits[byte_offset2] |= end_byte;  //字节头赋值 
                    //字节数换行
                    byte_offset1 += temp_bytes;
                    byte_offset2 += temp_bytes;
                }
            }
            else//dst_rect首行的首字节与尾字节在一个字节内
            {   //bit1在左，bit2在右，右边为高位，此处的bit_offset2=7-bit2
                for(i = 6-bit_offset2;i <= bit_offset1;i = i+2)
                {
                    end_byte |= 0x3<<i;
                    start_byte |= pf<<i;
                }
                for(y = y1;y < y2;y++)
                {   //保留目标字节内不需要填充的位
                    dst_bitmap->bm_bits[byte_offset1] &= ~end_byte;
                    //填充相应的位
                    dst_bitmap->bm_bits[byte_offset1] |= start_byte;
                    byte_offset1 += temp_bytes;//字节数换行
                }
            }
        }break;
        case 4:
        {
            pf = pf_color & 0xf;                //取给定的填充颜色
            byte_offset1 = y1*temp_bytes+x1/2;  //dst_rect首行左边界所在字节
            byte_offset2 = y1*temp_bytes+x2/2;  //dst_rect首行右边界所在字节
            bit_offset1 = (1-x1%2)<<2;//dst_rect左边界在所在字节的位，x=0，bit=7
            bit_offset2 = (x2%2)<<2;//7减去dst_rect右边界在所在字节的位，7-bit
            middle_byte = (u8)((pf<<4)|pf);     //每行中间字节的颜色
            //dst_rect首行的首字节与尾字节不在一个字节内
            if(byte_offset1 != byte_offset2)
            {
                if(bit_offset1 == 4)            //左边界是所在字节左边的一个像素
                    start_byte = middle_byte;   //右边是高位，首字节是整字节填充
                else                            //左边界是所在字节右边的一个像素
                    start_byte = pf;            //右边是高位，首字节是半字节填充
                if(bit_offset2 == 4)            //右边界是所在字节右边的一个像素
                    start_byte = middle_byte;   //右边是高位，尾字节是整字节填充
                else                            //右边界是所在字节左边的一个像素
                    start_byte = pf<<4;         //右边是高位，尾字节是半字节填充
                for(y = y1;y < y2;y++)          //填充颜色
                {
                    //填充dst_rect每行中间字节的颜色
                    for(x = byte_offset1+1; x < byte_offset2; x++)
                    {
                        dst_bitmap->bm_bits[x] = middle_byte;
                    }
                    dst_bitmap->bm_bits[byte_offset1] &= 0xff<<(bit_offset1+4);
                    dst_bitmap->bm_bits[byte_offset1] |= start_byte;//字节头赋值 
                    dst_bitmap->bm_bits[byte_offset2] &= 0xff>>(bit_offset2+4);
                    dst_bitmap->bm_bits[byte_offset2] |= end_byte;  //字节尾赋值
                    //字节数换行
                    byte_offset1 += temp_bytes;
                    byte_offset2 += temp_bytes;
                }
            }
            else//dst_rect首行的首字节与尾字节在一个字节内
            {   //bit1在左，bit2在右，右边为高位，此处的bit_offset2=7-bit2
                for(i = 4-bit_offset2;i <= bit_offset1;i = i+4)
                {
                    end_byte |= 0xf<<i;
                    start_byte |= pf<<i;
                }
                for(y = y1;y < y2;y++)
                {   //保留目标字节内不需要填充的位
                    dst_bitmap->bm_bits[byte_offset1] &= ~end_byte;
                    //填充相应的位
                    dst_bitmap->bm_bits[byte_offset1] |= start_byte;
                    byte_offset1 += temp_bytes;     //字节数换行
                }
            }
        }break;
        case 8:
        {
            pf = pf_color;                      //取给定的填充颜色
            byte_offset1 = y1*temp_bytes+x1;    //dst_rect首行左边界所在字节
            byte_offset2 = y1*temp_bytes+x2;    //dst_rect首行右边界所在字节
            for(y = y1;y < y2;y++)      //填充颜色，每次填充一个像素
            {
                //填充dst_rect每行中间字节的颜色
                for(x = byte_offset1;x <= byte_offset2;x++)
                {                            
                    dst_bitmap->bm_bits[x] = (u8)pf;
                }
                //字节数换行
                byte_offset1 += temp_bytes;
                byte_offset2 += temp_bytes;
             }                                        
        }break;
        case 12:
        {
            pf = pf_color;           //取给定的填充颜色
            for(y = y1;y < y2;y++)  //填充颜色
            {
                temp_bytes = y*temp_bytes;              //y行的字节偏移量
                for(x = x1;x < x2;x++)
                {                                        
                    byte_offset = temp_bytes + x*3/2;   //目标像素所在字节
                    bit_offset = (x*12)%8;              //目标像素的位偏移量
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
            pf = pf_color;                       //取给定的填充颜色
            if((u32)dst_bitmap->bm_bits & 1 )      //非对齐地址
            {
                byte_offset1 = y1*temp_bytes+x1*2;  //dst_rect首行左边界所在字节
                byte_offset2 = y1*temp_bytes+x2*2;  //dst_rect首行右边界所在字节
                for(y = y1;y < y2;y++)              //填充颜色，每次填充两个像素
                {
                    for(x = byte_offset1;x <= byte_offset2;x = x+2)
                    {                                        
    //                    alpha = pf;
    //                    if(dst_bitmap->pf_type == cn_sys_pf_a1r5g5b5)
    //                    {
    //                        pf &= 0x7fff;
    //                        pf |= alpha & 0x8000;
    //                    }
                        //注:因对齐问题，不可把目标地址强制转换成16位指针。
                        dst_bitmap->bm_bits[x] = (u8)pf;
                        dst_bitmap->bm_bits[x+1] = (u8)(pf>>8);
                    }
                    //字节数换行
                    byte_offset1 += temp_bytes;
                    byte_offset2 += temp_bytes;
                }
            }
            else                        //对齐的地址
            {
                u16 *vm;
                vm = (u16*)((u32)dst_bitmap->bm_bits+y1*dst_bitmap->linebytes);
                for(y = y1;y < y2;y++)              //填充颜色，每次填充两个像素
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
            pf = pf_color;                      //取给定的填充颜色
            byte_offset1 = y1*temp_bytes+x1*3;  //dst_rect首行左边界所在字节
            byte_offset2 = y1*temp_bytes+x2*3;  //dst_rect首行右边界所在字节
            for(y = y1;y < y2;y++)              //填充颜色，每次填充一个像素
            {
                for(x = byte_offset1;x <= byte_offset2;x = x+3)
                {                                                    
                    dst_bitmap->bm_bits[x] = (u8)pf;
                    dst_bitmap->bm_bits[x+1] = (u8)(pf >> 8);
                    dst_bitmap->bm_bits[x+2] = (u8)(pf >> 16);
                }
                //字节数换行
                byte_offset1 += temp_bytes;
                byte_offset2 += temp_bytes;
             }                                                            
        }break;
        case 32:
        {
            pf = pf_color;                      //取给定的填充颜色
            byte_offset1 = y1*temp_bytes+x1*4;  //dst_rect首行左边界所在字节
            byte_offset2 = y1*temp_bytes+x2*4;  //dst_rect首行右边界所在字节
            for(y = y1;y < y2;y++)              //填充颜色，每次填充一个像素
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
                    //注:因对齐问题，不可把目标地址强制转换成32位指针。
                    dst_bitmap->bm_bits[x] = (u8)pf;
                    dst_bitmap->bm_bits[x+1] = (u8)(pf >> 8);
                    dst_bitmap->bm_bits[x+2] = (u8)(pf >> 16);
                    dst_bitmap->bm_bits[x+3] = (u8)(pf >> 24);
                }
                //字节数换行
                byte_offset1 += temp_bytes;
                byte_offset2 += temp_bytes;
             }                                                            
        }break; 
    }
}

//----填充位图-----------------------------------------------------------------
//功能: 本函数是__gk_fill_rect的特殊情况，即需要填充的矩形是整个bitmap。
//参数: dst_bitmap,被填充的位图
//      color，填充的颜色，格式是cn_sys_pf_e8r8g8b8
//返回: 无
//-----------------------------------------------------------------------------
void __gk_fill_bm(struct st_rect_bitmap *dst_bitmap,u32 color)
{
    u32 pf_color,color_bits,alpha;
    u32 byte_offset,total_bytes;

    //color的格式是cn_sys_pf_e8r8g8b8，转换成dst_bitmap->pf_type匹配的格式
    pf_color = __gk_convert_color_to_pf(dst_bitmap->pf_type,color);
    color_bits = dst_bitmap->pf_type >>8;//被填充位图的颜色格式
    switch(color_bits)
    {
        case 1:
        {//组合颜色，直接将相应的位置数
            if(1 == pf_color)
                memset( dst_bitmap->bm_bits,0xff,
                        dst_bitmap->linebytes*dst_bitmap->height);
            else
                memset( dst_bitmap->bm_bits,0,
                        dst_bitmap->linebytes*dst_bitmap->height);
        }break;
        case 2:
        {//组合颜色，直接将相应的位置数
            memset( dst_bitmap->bm_bits,
                    pf_color + (pf_color<<2) + (pf_color<<4) + (pf_color<<6),
                    dst_bitmap->linebytes*dst_bitmap->height);
        }break;
        case 4:
        {//组合颜色，直接将相应的位置数
            memset( dst_bitmap->bm_bits,(u8)(pf_color|(pf_color<<4)),
                    dst_bitmap->linebytes*dst_bitmap->height);
        }break;
        case 8:
        {//直接将相应的位置数
            memset( dst_bitmap->bm_bits,(u8)pf_color,
                    dst_bitmap->linebytes*dst_bitmap->height);
        }break;
        case 12:
        {   //总共需要填充的字节数
            total_bytes = dst_bitmap->linebytes*dst_bitmap->height;
            //12位的颜色格式，一次可填充3个字节，即两个像素
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
        {   //总共需要填充的字节数            
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
                    //注:因对齐问题，不可把目标地址强制转换成16位指针。
                    //一次写一个像素，两个字节
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
        {   //总共需要填充的字节数   
            total_bytes = dst_bitmap->linebytes*dst_bitmap->height;
            for(byte_offset = 0;byte_offset < total_bytes;)
            {
                dst_bitmap->bm_bits[byte_offset] = (u8)pf_color;
                dst_bitmap->bm_bits[byte_offset+1] = (u8)(pf_color >> 8);
                dst_bitmap->bm_bits[byte_offset+2] = (u8)(pf_color >> 16);
                byte_offset = +3;   //一次写一个像素，三个字节
             }                                                            
        }break;
        case 32:
        {   //总共需要填充的字节数
            total_bytes = dst_bitmap->linebytes*dst_bitmap->height;
            for(byte_offset = 0;byte_offset < total_bytes;)
            {
                alpha = pf_color;
                if(dst_bitmap->pf_type == cn_sys_pf_a8r8g8b8)
                {   
                    pf_color &= 0x00ffffff;
                    pf_color |= alpha & 0xff000000;
                }
                //注:因对齐问题，不可把目标地址强制转换成32位指针。
                dst_bitmap->bm_bits[byte_offset] = (u8)pf_color;
                dst_bitmap->bm_bits[byte_offset+1] = (u8)(pf_color >> 8);
                dst_bitmap->bm_bits[byte_offset+2] = (u8)(pf_color >> 16);
                dst_bitmap->bm_bits[byte_offset+3] = (u8)(pf_color >> 24);
                byte_offset = +4;   //一次写一个像素，四个字节
             }                                                            
        }break; 
    }
}
//----绘制8个像素点------------------------------------------------------------
//功能: 绘制8个像素点，在画圆的情况下调用，利用了圆的八分特性，
//      同时标志changed_msk。
//参数: gkwin，目标窗口
//      bitmap，目标位图
//      x0、y0，圆心坐标
//      x、y，要绘制的点的坐标，相对于圆心
//      color，给定的颜色
//      rop2_code，二元光栅操作码
//返回: 无
//-----------------------------------------------------------------------------
void __gk_set_all_pixels_circle(struct gkwin_rsc *gkwin,
                                struct st_rect_bitmap *bitmap,s32 x0,s32 y0,
                                s32 x,s32 y,u32 color,u32 rop2_code)
{
    //绘制8个像素点
    __gk_set_pixel_rop2_bm(bitmap,x0+x,y0+y,color,rop2_code);
    __gk_set_pixel_rop2_bm(bitmap,x0+x,y0-y,color,rop2_code);
    __gk_set_pixel_rop2_bm(bitmap,x0-x,y0+y,color,rop2_code);
    __gk_set_pixel_rop2_bm(bitmap,x0-x,y0-y,color,rop2_code);
    __gk_set_pixel_rop2_bm(bitmap,x0+y,y0+x,color,rop2_code);
    __gk_set_pixel_rop2_bm(bitmap,x0+y,y0-x,color,rop2_code);
    __gk_set_pixel_rop2_bm(bitmap,x0-y,y0+x,color,rop2_code);
    __gk_set_pixel_rop2_bm(bitmap,x0-y,y0-x,color,rop2_code);
    //对8个像素点分别进行changed_msk标志
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
//----绘制目标点并着色---------------------------------------------------------
//功能: 绘制目标点，同时标志changed_msk。
//参数: gkwin，目标窗口
//      limit，绘制的限制区，只绘制直线在limit矩形内部的部分
//      bitmap，目标位图
//      x、y，要绘制的点的坐标
//      color，给定的颜色
//      rop2_code，二元光栅操作码
//返回: 无
//-----------------------------------------------------------------------------
void __gk_pixel_sect_inter(struct gkwin_rsc *gkwin,struct st_rect *limit,
                                struct st_rect_bitmap *bitmap,s32 x,s32 y,
                                u32 color,u32 rop2_code)
{
    if((x >= limit->left)&&(x < limit->right)
        &&(y >= limit->top)&&(y < limit->bottom))
    {//待绘制的目标点在限制区内，画点并标志changed_msk
        __gk_set_pixel_rop2_bm(bitmap,x,y,color,rop2_code);
        __gk_set_pixel_rop2_bm(&(gkwin->changed_msk),x/8,y/8,
                                cn_color_white,cn_r2_copypen);
    }
    else    //待绘制的目标点在限制区外，直接返回
        return;
}
//----绘制多个目标点并标志-----------------------------------------------------
//功能: 绘制多个目标点，同时标志changed_msk。
//参数: gkwin，目标窗口
//      limit，绘制的限制区，只绘制直线在limit矩形内部的部分
//      bitmap，目标位图
//      x0、y0，圆心坐标
//      x、y，要绘制的点的坐标
//      color，给定的颜色
//      rop2_code，二元光栅操作码
//返回: 无
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
//----画圆---------------------------------------------------------------------
//功能: 在窗口内画一个圆。
//参数: gkwin，目标窗口
//      limit，绘制的限制区，只绘制直线在limit矩形内部的部分
//      x0、y0，圆心坐标
//      r，圆的半径
//      color，给定的颜色
//      rop2_code，二元光栅操作码
//返回: 无
//说明: 使用中点画圆算法
//-----------------------------------------------------------------------------
void __gk_draw_circle_bm(struct gkwin_rsc *gkwin,struct st_rect *limit,
                        s32 x0,s32 y0,s32 r,u32 color,u32 rop2_code)
{
    s32 x,y,e;
    s32 left,right,top,bottom;
    u32 pf_color,flag;
    struct st_rect_bitmap *bitmap;

    //选取(x,y)为(0,r)作为初始坐标(不是实际的圆，平移到原点的圆)，圆上的特殊点    
    x = 0;
    y = r;    
    e = 1-r;
    left = limit->left-x0;      //窗口左边界到圆心x方向的距离
    top = limit->top-y0;        //窗口左边界到圆心y方向的距离
    right = limit->right-x0;    //窗口右边界到圆心x方向的距离
    bottom = limit->bottom-y0;  //窗口右边界到圆心y方向的距离
    bitmap = &gkwin->wm_bitmap;
    //要求给定的颜色的颜色格式为24位
    //绘制像素前需将24位调整为与显示屏一致的颜色格式
    pf_color = __gk_convert_color_to_pf(gkwin->disp->pixel_format,color);

    //整个圆都在limit内
    if((x0-limit->left >= r)&&(limit->right-x0 > r)
        &&(y0-limit->top >= r)&&(limit->bottom-y0 > r))
        flag = 1;
    //圆上的点不都在limit内，分整个圆在窗口外和部分在窗口外两种情况
    else if((left*left+top*top < r*r)
        &&(right*right+top*top < r*r)
        &&(left*left+bottom*bottom < r*r)
        &&(right*right+bottom*bottom < r*r))
        return;
    //圆上的点部分在limit内
    else
        flag = 0;
    if(flag)
    {//整个圆都在limit内，计算出四分之一个圆的坐标
     //根据对称性，可得圆上其它所有点的坐标
        while(x <= y)
        {
            //不做判断，绘制8个像素，同时标志像素的changed_msk
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
    {//圆上部分点在limit内，需要对圆上各个点是否在limit内做出判断
        while(x <= y)
        {
            //判断要绘制点的位置后，决定是否绘制，绘制的像素同时标志changed_msk
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
        //画一个圆只改变了窗口的部分内容
        gkwin->change_flag = cn_gkwin_change_part;
}
//----目标点的绘制-------------------------------------------------------------
//功能: 据目标点所在的位置，得其是否需要绘制。
//参数: gkwin，目标窗口
//      limit，绘制的限制区，只绘制直线在limit矩形内部的部分
//      x、y，目标点的坐标
//      color，画圆使用的颜色
//      rop2_code，二元光栅操作码
//返回: 无
//-----------------------------------------------------------------------------
void __gk_pixel_sect_inter_screen(struct display_rsc *display,
                                    struct st_rect *limit,
                                    s32 x,s32 y,u32 color,u32 rop2_code)
{
    if((x >= limit->left)&&(x < limit->right)
        &&(y >= limit->top)&&(y < limit->bottom))
    //目标点在limit内，需要绘制
        display->draw.set_pixel_screen(x,y,color,rop2_code);
    else
        return;
}

//----在screen上画圆-----------------------------------------------------------
//功能: 在screen上画一个圆。
//参数: gkwin，目标窗口
//      limit，绘制的限制区，只绘制直线在limit矩形内部的部分
//      x0、y0，圆心坐标
//      r，圆的半径
//      color，画圆使用的颜色
//      rop2_code，二元光栅操作码
//返回: 无
//使用中点画圆算法
//-----------------------------------------------------------------------------
void __gk_draw_circle_screen(struct display_rsc *display,struct st_rect *limit,
                        s32 x0,s32 y0,s32 r,u32 color,u32 rop2_code)//确认
{
    s32 x,y,e;
    s32 left,right,top,bottom;
    u32 pf_color,flag;
    struct display_rsc *mirror,*current;
    struct disp_draw *mydraw;

    mydraw = &display->draw;
    //选取(x,y)为(0,r)作为初始坐标(不是实际的圆，平移到原点的圆)，圆上的特殊点        
    x = 0;
    y = r;
    e = 1-r;
    left = x0 - limit->left;
    top = y0 - limit->top;
    right = limit->right-x0;
    bottom = limit->bottom-y0;
    pf_color = __gk_convert_color_to_pf(display->pixel_format,color);

    //整个圆都在limit内
    if((x0-limit->left >= r)&&(limit->right-x0 > r)
        &&(y0-limit->top >= r)&&(limit->bottom-y0 > r))
        flag = 1;
    
    //圆上的点都不在limit内
    else if((left*left+top*top < r*r)
        &&(right*right+top*top < r*r)
        &&(left*left+bottom*bottom < r*r)
        &&(right*right+bottom*bottom < r*r))
        return;
    //圆上的点部分在limit内
    else
        flag = 0;

    if(flag) 
    {//整个圆都在limit内，利用圆的八分特性，只需要计算八分之一个圆的坐标
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
    {//圆上的点部分在limit内，需要考虑圆上各个点是否在limit内
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
    //镜像显示
    mirror = (struct display_rsc *)display->node.child;
    if(mirror == NULL)      //没有镜像显示器，直接返回
        return;
    current = mirror;
    while(current != NULL)
    {//存在镜像显示器
        if(flag) 
        {//整个圆都在limit内，利用圆的八分特性，只需要计算八分之一个圆的坐标
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
        {//圆上的点部分在limit内，需要考虑圆上各个点是否在limit内
            while(x <= y)
            {
                //只绘制在limit内的像素
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
//----画圆---------------------------------------------------------------------
//功能: 在窗口内画一个圆。
//参数: gkwin，目标窗口
//      x0、y0，圆心坐标，相对于窗口
//      r，圆的半径
//      color，画圆使用的颜色
//      rop2_code，二元光栅操作码
//返回: 无
//-----------------------------------------------------------------------------
void gk_draw_circle(struct gksc_para_draw_circle *para)//确认
{
    struct st_rect limit;
    struct clip_rect *clip;
    struct gkwin_rsc *fb_gkwin;
    struct disp_draw *my_draw_fun;
    s32 offsetx,offsety;
    if((para->rop2_code == cn_r2_nop) || (para->rop2_code > cn_r2_last))
        return;                                         //不执行操作
    if(para->r <= 0)                                         //无须绘制
        return;
    my_draw_fun = &para->gkwin->disp->draw;
    //说明有win buffer，且直接写屏属性为false
    if((para->gkwin->wm_bitmap.bm_bits != NULL)
        && (para->gkwin->direct_screen ==false))     
    {   //处理方法:在win buffer中绘图，标志changed_msk
        limit.left = 0;
        limit.top = 0;
        limit.right = para->gkwin->wm_bitmap.width;
        limit.bottom = para->gkwin->wm_bitmap.height;
        //以硬件加速不支持圆的绘制考虑，用软件实现
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
        //有frame buffer，且直接写屏属性为false
        if((fb_gkwin != NULL) && (para->gkwin->direct_screen == false))     
        {   //处理方法:在frame buffer中绘图，但只绘gkwin中的可视区域
            do{
                //以硬件加速不支持圆的绘制考虑，用软件实现
                __gk_draw_circle_bm(fb_gkwin,&clip->rect,
                                    para->x0+offsetx,para->y0+offsety,
                                    para->r,para->color,para->rop2_code);
                clip = clip->next;
            }while(clip != para->gkwin->visible_clip);
        }else       
        //无win buffer，也无frame buffer，直接画在screen上
        //直接写屏属性为true，不管有无缓冲区，都直接画在screen上
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

//----画贝塞尔曲线-------------------------------------------------------------
//功能: 在窗口上画贝塞尔曲线。
//参数: gkwin，目标窗口
//      limit，允许绘图的限制区域，一个矩形的区域，超出此区域的直线不画
//      x1、y1、x2、y2、x3、y3、x4、y4，端点坐标
//      color，绘图所用的颜色
//      rop2_code，二元光栅操作码
//返回: 无
//-----------------------------------------------------------------------------
void __gk_Bezier_bm(struct gkwin_rsc *gkwin,struct st_rect *limit,
                    float x1,float y1,float x2,float y2,float x3,float y3,
                    float x4,float y4,u32 color,u32 rop2_code)
{
    float a,b,c;
    float x12,y12,x23,y23,x34,y34;
    float x1223,y1223,x2334,y2334;
    float x,y,ab;

    //a、b、c是直线(x1,y1)、(x4,y4)以一般式ax+by+c=0时对应的各个参数
    a = y4-y1;
    b = x1-x4;
    c = y1*(x4-x1)-x1*(y4-y1);
    ab = a*a+b*b;
    
    //点(x2,y2)、(x3,y3)到直线(x1,y1)、(x4,y4)的距离小于1(个像素)，
    //则以直线代替曲线
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

    //不满足中间两个控制点到以端点为起、终点的直线的距离小于1(个像素)时，
    //在曲线的中点处分割曲线
    __gk_Bezier_bm(gkwin,limit,x1,y1,x12/2,y12/2,
                    x1223/4,y1223/4,x/8,y/8,color,rop2_code);
    __gk_Bezier_bm(gkwin,limit,x/8,y/8,x2334/4,y2334/4,
                    x34/2,y34/2,x4,y4,color,rop2_code);
}

//----在screen上画贝塞尔曲线---------------------------------------------------
//功能: 在screen上画贝塞尔曲线。
//参数: gkwin，目标窗口
//      limit，允许绘图的限制区域，一个矩形的区域，超出此区域的直线不画
//      x1、y1、x2、y2、x3、y3、x4、y4，端点坐标
//      color，绘图所用的颜色
//      rop2_code，二元光栅操作码
//返回: 无
//-----------------------------------------------------------------------------
void __gk_Bezier_screen(struct display_rsc *display,struct st_rect *limit,
                    float x1,float y1,float x2,float y2,float x3,float y3,
                    float x4,float y4,u32 color,u32 rop2_code)
{
    float a,b,c;
    float x12,y12,x23,y23,x34,y34;
    float x1223,y1223,x2334,y2334;
    float x,y,ab;

    //a、b、c是直线(x1,y1)、(x4,y4)以一般式ax+by+c=0时对应的各个参数    
    a = y4-y1;
    b = x1-x4;
    c = y1*(x4-x1)-x1*(y4-y1);
    ab = a*a+b*b;

    //点(x2,y2)、(x3,y3)到直线(x1,y1)、(x4,y4)的距离小于1(个像素)，
    //则以直线代替曲线
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

    //不满足中间两个控制点到以端点为起、终点的直线的距离小于1(个像素)时，
    //在曲线的中点处分割曲线
    __gk_Bezier_screen(display,limit,x1,y1,x12/2,y12/2,
                    x1223/4,y1223/4,x/8,y/8,color,rop2_code);
    __gk_Bezier_screen(display,limit,x/8,y/8,x2334/4,y2334/4,
                    x34/2,y34/2,x4,y4,color,rop2_code);
}
//----画贝塞尔曲线-------------------------------------------------------------
//功能: 在窗口内画一条贝塞尔曲线。
//参数: 参数是结构para的成员
//      gkwin，目标窗口指针
//      x1、y1、x2、y2、x3、y3、x4、y4，绘制Bezier曲线的四个控制点
//      color，画线使用的颜色
//      r2_code，二元光栅操作码
//返回: 无
//-----------------------------------------------------------------------------
void gk_Bezier(struct gksc_para_Bezier *para)//确认
{
    s32 offsetx,offsety;
    struct st_rect limit;
    struct clip_rect *clip;
    struct gkwin_rsc *fb_gkwin;
    struct disp_draw *my_draw_fun;
    if((para->rop2_code == cn_r2_nop) || (para->rop2_code > cn_r2_last))
        return;                                         //不执行操作
    my_draw_fun = &para->gkwin->disp->draw;
    //说明有win buffer，且直接写屏属性为false
    if((para->gkwin->wm_bitmap.bm_bits != NULL)
        && (para->gkwin->direct_screen == false))     
    {   //处理方法:在win buffer中绘图，标志changed_msk
        limit.left = 0;
        limit.top = 0;
        limit.right = para->gkwin->wm_bitmap.width;
        limit.bottom = para->gkwin->wm_bitmap.height;
        //以硬件加速不支持贝塞尔曲线的绘制考虑，用软件实现
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
        //有frame buffer，且直接写屏属性为false
        if((fb_gkwin != NULL) && (para->gkwin->direct_screen == false))     
        {   //处理方法:在frame buffer中绘图，但只绘gkwin中的可视区域
            do{
                //以硬件加速不支持贝塞尔曲线的绘制考虑，用软件实现
                __gk_Bezier_bm(fb_gkwin,&clip->rect,para->x1+offsetx,
                            para->y1+offsety,para->x2+offsetx,para->y2+offsety,
                            para->x3+offsetx,para->y3+offsety,para->x4+offsetx,
                            para->y4+offsety,para->color,para->rop2_code);
                clip = clip->next;
            }while(clip != para->gkwin->visible_clip);
        }else       
        //无win buffer，也无frame buffer，直接画在screen上
        //直接写屏属性为true，不管有无缓冲区，直接画在screen上
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
//----画直线-------------------------------------------------------------------
//功能: 在窗口内画一条直线。
//参数: 参数是结构para的成员
//      gkwin，目标窗口指针
//      x1、y1，画线起点坐标
//      x2、y2，画线终点坐标(不包含)
//      color，画线使用的颜色
//      r2_code，二元光栅操作码
//返回: 无
//-----------------------------------------------------------------------------
void gk_lineto(struct gksc_para_lineto *para)//确认
{
    struct st_rect limit;
    struct clip_rect *clip;
    struct gkwin_rsc *fb_gkwin;
    struct disp_draw *my_draw_fun;
    s32 offsetx,offsety;
    if((para->rop2_code == cn_r2_nop) || (para->rop2_code > cn_r2_last))
        return;                                         //不执行操作
    if((para->x1==para->x2) && (para->y1==para->y2))    //无须绘制
        return;
    my_draw_fun = &para->gkwin->disp->draw;
    //说明有win buffer，且直接写屏属性为false
    if((para->gkwin->wm_bitmap.bm_bits != NULL)
        && (para->gkwin->direct_screen == false))     
    {   //处理方法:在win buffer中绘图，标志changed_msk
        limit.left = 0;
        limit.top = 0;
        limit.right = para->gkwin->wm_bitmap.width;
        limit.bottom = para->gkwin->wm_bitmap.height;
        if(!my_draw_fun->line_bm(&para->gkwin->wm_bitmap,&limit,para->x1,
                    para->y1,para->x2,para->y2,para->color,para->rop2_code))
        {    //硬件加速不支持直线绘制，改用软件实现，软件画线算法中
            //同时处理了changed_msk
            __gk_lineto_bm(para->gkwin,&limit,para->x1,para->y1,para->x2,
                                    para->y2,para->color,para->rop2_code);
        }
        else    //硬件加速绘图时，未处理changed_msk，处理之
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
        //有frame buffer,且直接写屏属性为false
        if((fb_gkwin != NULL) && (para->gkwin->direct_screen == false))     
        {   //处理方法:在frame buffer中绘图，但只绘gkwin中的可视区域
            do{
                if(!my_draw_fun->line_bm(
                            &fb_gkwin->wm_bitmap,&clip->rect,
                            para->x1+offsetx,para->y1+offsety,para->x2+offsetx,
                            para->y2+offsety,para->color,para->rop2_code))
                {    //硬件加速不支持直线绘制，改用软件实现，软件画线算法中
                    //同时处理了changed_msk
                    __gk_lineto_bm(fb_gkwin,&clip->rect,
                                    para->x1+offsetx,para->y1+offsety,
                                    para->x2+offsetx,para->y2+offsety,
                                    para->color,para->rop2_code);
                }
                else    //硬件加速绘图时，未处理changed_msk，处理之
                { 
                    __gk_shading_line(fb_gkwin,&clip->rect,para->x1+offsetx,
                                      para->y1+offsety,para->x2+offsetx,
                                      para->y2+offsety);
                }
                clip = clip->next;
            }while(clip != para->gkwin->visible_clip);
        }
        else       
        //无win buffer，也无frame buffer，直接画在screen上
        //直接写屏属性为true，不管有无缓冲区，都直接画在screen上
        {
            do{//在screen上直接画直线，不画终点
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
//----画直线-------------------------------------------------------------------
//功能: 在窗口内画一条直线。
//参数: gkwin，目标窗口指针
//      gkwin，目标窗口指针
//      x1、y1，画线起点坐标
//      x2、y2，画线终点坐标(包含)
//      color，画线使用的颜色
//      r2_code，二元光栅操作码
//返回: 无
//-----------------------------------------------------------------------------
void gk_lineto_ie(struct gksc_para_lineto *para)//确认
{
	s32 dx,dy;
    struct st_rect limit;
    struct clip_rect *clip;
    struct gkwin_rsc *fb_gkwin;
    struct disp_draw *my_draw_fun;
    struct display_rsc *display;
    s32 offsetx,offsety;
    if((para->rop2_code == cn_r2_nop) || (para->rop2_code > cn_r2_last))
        return;                                         //不执行操作
    if((para->x1==para->x2) && (para->y1==para->y2))    //无须绘制
        return;
    display = para->gkwin->disp;
    my_draw_fun = &display->draw;
    if(para->y1 == para->y2)    //绘制水平线
    {
        //说明有win buffer，且直接写屏属性为false
        if((para->gkwin->wm_bitmap.bm_bits != NULL)
            && (para->gkwin->direct_screen == false))    
        {   //处理方法:在win buffer中绘图，标志changed_msk
            limit.left = 0;
            limit.top = 0;
            limit.right = para->gkwin->wm_bitmap.width;
            limit.bottom = para->gkwin->wm_bitmap.height;
            if(!my_draw_fun->line_bm_ie(&para->gkwin->wm_bitmap,&limit,para->x1,
                        para->y1,para->x2,para->y2,para->color,para->rop2_code))
            {    //硬件加速不支持直线绘制，改用软件实现，软件画线算法中
                //同时处理了changed_msk
            	__gk_hlineto_bm(para->gkwin,&limit,para->x1,para->y1,
                            	para->x2+1,para->color,para->rop2_code);
            }
            else
            {   //硬件加速支持直线绘制，未处理changed_msk，处理之
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
            //有frame buffer，且直接写屏属性为false
            if((fb_gkwin != NULL) && (para->gkwin->direct_screen == false))     
            {   //处理方法:在frame buffer中绘图，但只绘gkwin中的可视区域
                do
                {
                    if(!my_draw_fun->line_bm_ie(
                                &fb_gkwin->wm_bitmap,&clip->rect,
                                para->x1+offsetx,para->y1+offsety,
                                para->x2+offsetx,para->y2+offsety,
                                para->color,para->rop2_code))
                    {    //硬件加速不支持直线绘制，改用软件实现，软件画线算法中
                        //同时处理了changed_msk
                    	__gk_hlineto_bm(fb_gkwin,&clip->rect,
                                	para->x1+offsetx,para->y1+offsety,
                                    para->x2+offsetx+1,
                                    para->color,para->rop2_code);
                    }
                	else
                    {   //硬件加速绘图时，未处理changed_msk，处理之
                    	__gk_shading_line(fb_gkwin,&clip->rect,
                                    	para->x1+offsetx,para->y1+offsety,
                                    	para->x2+offsetx+1,para->y2+offsety);
                    }
                    clip = clip->next;
                }while(clip != para->gkwin->visible_clip);
            }
            else       
            //无win buffer，也无frame buffer，直接画在screen上
            //直接写屏属性为true，不管有无缓冲区，都直接画在screen上
            {
                do{//在screen上直接画直线，画终点
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
	else if(para->x1 == para->x2)    //绘制垂直线
    {
        //说明有win buffer，且直接写屏属性为false
        if((para->gkwin->wm_bitmap.bm_bits != NULL)
            && (para->gkwin->direct_screen == false))    
        {   //处理方法:在win buffer中绘图，标志changed_msk
            limit.left = 0;
            limit.top = 0;
            limit.right = para->gkwin->wm_bitmap.width;
            limit.bottom = para->gkwin->wm_bitmap.height;
            if(!my_draw_fun->line_bm_ie(&para->gkwin->wm_bitmap,&limit,para->x1,
                        para->y1,para->x2,para->y2,para->color,para->rop2_code))
            {    //硬件加速不支持直线绘制，改用软件实现，软件画线算法中
                //同时处理了changed_msk
            	__gk_vlineto_bm(para->gkwin,&limit,para->x1,
                        	para->y1,para->y2+1,para->color,para->rop2_code);
            }
            else
            {   //硬件加速支持直线绘制，未处理changed_msk，处理之
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
            //有frame buffer，且直接写屏属性为false
            if((fb_gkwin != NULL) && (para->gkwin->direct_screen == false))     
            {   //处理方法:在frame buffer中绘图，但只绘gkwin中的可视区域
                do
                {
                    if(!my_draw_fun->line_bm_ie(
                                &fb_gkwin->wm_bitmap,&clip->rect,
                                para->x1+offsetx,para->y1+offsety,
                                para->x2+offsetx,para->y2+offsety,
                                para->color,para->rop2_code))
                    {    //硬件加速不支持直线绘制，改用软件实现，软件画线算法中
                        //同时处理了changed_msk
                    	__gk_vlineto_bm(fb_gkwin,&clip->rect,
                                	para->x1+offsetx,para->y1+offsety,
                                    para->y2+offsety+1,
                                    para->color,para->rop2_code);
                    }
                	else
                    {   //硬件加速绘图时，未处理changed_msk，处理之
                    	__gk_shading_line(fb_gkwin,&clip->rect,
                                    	para->x1+offsetx,para->y1+offsety,
                                    	para->x2+offsetx,para->y2+offsety+1);
                    }
                    clip = clip->next;
                }while(clip != para->gkwin->visible_clip);
            }
            else       
            //无win buffer，也无frame buffer，直接画在screen上
            //直接写屏属性为true，不管有无缓冲区，都直接画在screen上
            {
                do{//在screen上直接画直线，画终点
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
    else	//绘制斜线
    {
    	dx = abs(para->x2-para->x1);
    	dy = abs(para->y2-para->y1);
        //说明有win buffer，且直接写屏属性为false
        if((para->gkwin->wm_bitmap.bm_bits != NULL)
            && (para->gkwin->direct_screen == false))    
        {   //处理方法:在win buffer中绘图，标志changed_msk
            limit.left = 0;
            limit.top = 0;
            limit.right = para->gkwin->wm_bitmap.width;
            limit.bottom = para->gkwin->wm_bitmap.height;
            if(!my_draw_fun->line_bm_ie(&para->gkwin->wm_bitmap,&limit,para->x1,
                        para->y1,para->x2,para->y2,para->color,para->rop2_code))
            {    //硬件加速不支持直线绘制，改用软件实现，软件画线算法中
                //同时处理了changed_msk
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
            {   //硬件加速支持直线绘制，未处理changed_msk，处理之
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
            //有frame buffer，且直接写屏属性为false
            if((fb_gkwin != NULL) && (para->gkwin->direct_screen == false))     
            {   //处理方法:在frame buffer中绘图，但只绘gkwin中的可视区域
                do
                {
                    if(!my_draw_fun->line_bm_ie(
                                &fb_gkwin->wm_bitmap,&clip->rect,
                                para->x1+offsetx,para->y1+offsety,
                                para->x2+offsetx,para->y2+offsety,
                                para->color,para->rop2_code))
                    {    //硬件加速不支持直线绘制，改用软件实现，软件画线算法中
                        //同时处理了changed_msk
                            __gk_olineto_bm(fb_gkwin,&clip->rect,
                                        	para->x1+offsetx,para->y1+offsety,
                                        	para->x2+offsetx,para->y2+offsety,
                                        	para->color,para->rop2_code);
                    }
                	else
                    {   //硬件加速绘图时，未处理changed_msk，处理之
                        	__gk_shading_line(fb_gkwin,&clip->rect,
                                        	para->x1+offsetx,para->y1+offsety,
                                        	para->x2+offsetx,para->y2+offsety);
                    }
                    clip = clip->next;
                }while(clip != para->gkwin->visible_clip);
            }
            else       
            //无win buffer，也无frame buffer，直接画在screen上
            //直接写屏属性为true，不管有无缓冲区，都直接画在screen上
            {
                do{//在screen上直接画直线，画终点
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
//----绘制位图-----------------------------------------------------------------
//功能: 在窗口内画位图，位图超出窗口的部分将不予绘制。
//参数: 参数是结构para的成员
//      gkwin，目标窗口指针
//      bitmap，待绘制的位图
//      x、y，绘制的目标位置
//返回: 无
//-----------------------------------------------------------------------------
void gk_draw_bitmap(struct gksc_para_draw_bitmap *para)//确认
{
    struct st_rect target,ins_rect;
    struct clip_rect *clip;
    struct gkwin_rsc *fb_gkwin;
    struct st_rect_bitmap *bitmap;
    struct st_rect_bitmap *src,*dst;    
    s32 x_src,y_src,x_dst,y_dst;
    
    bitmap = &para->gkwin->wm_bitmap;
    //待绘制的位图要绘制的位置相对于目标窗口的坐标
    target.left = para->x;
    target.top = para->y;
    target.right = para->x+para->bitmap.width;
    target.bottom = para->y+para->bitmap.height;
    //调整待绘制位图要绘制的位置，限制在窗口内
    if((target.right <= 0) || (target.left >= bitmap->width)
        ||(target.bottom <= 0) || (target.top >= bitmap->height))
        return;                             //绘图区域在窗口外面
    if(target.left < 0) target.left = 0;    //调整左边界在窗口内部
    if(target.top < 0) target.top = 0;      //调整上边界在窗口内部
    if(target.right > bitmap->width)        //调整右边界在窗口内部
        target.right = bitmap->width;
    if(target.bottom > bitmap->height)      //调整下边界在窗口内部
        target.bottom = bitmap->height;
    //说明有win buffer，且直接写屏属性为false
    if((para->gkwin->wm_bitmap.bm_bits != NULL) 
        && (para->gkwin->direct_screen == false))     
    {   //处理方法:在win buffer中绘图，标志changed_msk
        if(!para->gkwin->disp->draw.blt_bm_to_bm(bitmap,&target,
                            &para->bitmap,0,0,NULL,NULL,cn_r3_srccopy,0))
        {   
            //硬件加速不支持位块传送，软件实现
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
        __gk_shading_rect(para->gkwin,&target);    //设置绘制部分的changed_msk
    }
    else
    {
        clip = para->gkwin->visible_clip;
        if(clip == NULL)                            //窗口的可视域为空，直接返回
            return ;
        fb_gkwin = para->gkwin->disp->frame_buffer;
        target.left += para->gkwin->absx0;
        target.right += para->gkwin->absx0;
        target.top += para->gkwin->absy0;
        target.bottom += para->gkwin->absy0;
        //有frame buffer,且直接写屏属性为false
        if((fb_gkwin != NULL) && (para->gkwin->direct_screen == false))     
        {//处理方法:在frame_buffer中绘图，但只绘gkwin中的可视区域
            //为与可视域坐标保持一致，将待位图要绘制的位置的
            //坐标调整成以screen原点为原点的坐标
            do{
                //要绘制的位图目标位置在窗口可视域内才执行绘制程序
                if(__gk_get_rect_ints(&clip->rect,&target,&ins_rect))
                {
                    if(!fb_gkwin->disp->draw.blt_bm_to_bm(
                                        &fb_gkwin->wm_bitmap,&ins_rect,
                                        &para->bitmap,ins_rect.left-target.left,
                                        ins_rect.top-target.top,NULL,NULL,
                                        cn_r3_srccopy,0))
                    {//硬件加速不支持位块传送，软件实现
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
                    //设置绘制部分的changed_msk
                    __gk_shading_rect(fb_gkwin,&ins_rect);
                }
                clip = clip->next;
            }while(clip != para->gkwin->visible_clip);
        }
        else       
        //无win buffer，也无frame buffer，直接画在screen上
        //直接写屏属性为true，不管有无缓冲区，都直接画在screen上
        {
            //为与可视域坐标保持一致，将待位图要绘制的位置的
            //坐标调整成以screen原点为原点的坐标
            do{
                //要绘制的位图目标位置在窗口可视域内才执行绘制程序
                if(__gk_get_rect_ints(&clip->rect,&target,&ins_rect))
                {
                    //传送bitmap到screen
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

//----显示文本-----------------------------------------------------------------
//功能: 用默认字体和默认字符集显示一个文本串。
//参数: gkwin，显示的目标窗口
//      x、y，显示位置，相对于gkwin
//      text，待显示的字符串
//      count，字符串长度，含串结束符
//返回: 无
//-----------------------------------------------------------------------------
void gk_draw_text(struct gksc_para_draw_text *para,const char *text)//确认
{
    struct gksc_para_draw_bitmap bitmap_para;
    s32 len, src_len,size,size_bak;
    u32 wc;
    struct font_rsc* cur_font;
    u8 buf[128],*dbuf;  //定义一个足够存32点阵汉字的缓冲区，如果需要显示的字符
                        //超过此点阵，就需要动态分配

    cur_font = font_get_cur_font_rsc();
	if(cur_font == NULL)
	    return;

    // 计算字节数
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
        { // 无效字符
            src_len--;
            text++;
        }else
        { // 有效字符
            text += len;
            src_len -= len;

            // 取字形
            bitmap_para.bitmap.bm_bits = NULL;      //NULL=查询bitmap.
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
//----绘制光栅位图-------------------------------------------------------------
//功能: 在窗口内画光栅位图，位图超出窗口的部分将不予绘制。
//参数: 参数是结构para的成员
//      gkwin，目标窗口指针
//      bitmap，待绘制的位图
//      pattern，要设置的图案位图    
//      bitmsk，rop4操作才需要的掩码位图，一个单色位图
//      transparentcolor，要设置的透明色   
//      x、y，绘制的目标位置
//      rop_code，光栅操作码
//返回: 无
//-----------------------------------------------------------------------------
void gk_draw_bitmap_rop(struct gksc_para_draw_bitmap_rop *para)//确认
{
    u32 color,transparentcolor;
    s32 x_src,y_src,x_dst,y_dst,x_temp,y_temp;    
    struct st_rect target,ins_rect;
    struct clip_rect *clip;
    struct gkwin_rsc *fb_gkwin;
    struct st_rect_bitmap *bitmap,pat;//pat为pattern进行颜色格式转换得到的位图
    struct st_rect_bitmap *src,*dst;
    struct display_rsc *display;

    display = para->gkwin->disp;

    //调用者给定的透明色，颜色格式需是真彩色
	transparentcolor = 
             	__gk_convert_color_to_pf(display->pixel_format,
                                             	para->transparentcolor);
    bitmap = &para->gkwin->wm_bitmap;
    //待绘制的位图要绘制的位置相对于目标窗口的坐标
    target.left = para->x;
    target.top = para->y;
    target.right = para->x+para->bitmap.width;
    target.bottom = para->y+para->bitmap.height;
    //调整待绘制位图要绘制的位置，限制在窗口内
    if((target.right <= 0) || (target.left >= bitmap->width)
        ||(target.bottom <= 0) || (target.top >= bitmap->height))
        return;                             //绘图区域在窗口外面
    if(target.left < 0) target.left = 0;    //调整左边界在窗口内部
    if(target.top < 0) target.top = 0;      //调整上边界在窗口内部
    if(target.right > bitmap->width)        //调整右边界在窗口内部
        target.right = bitmap->width;
    if(target.bottom > bitmap->height)      //调整下边界在窗口内部
        target.bottom = bitmap->height;
    pat = para->pattern;

    //需要pattern参与运算，才进行格式化
    if(gk_rop_needsno_pattern(para->rop_code) == true)
    {
        //调用者给定的pattern颜色格式与显示屏颜色格式相同，直接使用给定的pattern
        //否则需将pattern中的像素进行颜色格式转换
        if(para->pattern.pf_type != display->pixel_format)
        {
            //给pat赋值，其尺寸为8*8，与pattern相同，颜色格式应与显示器相同
            pat.pf_type = display->pixel_format;
            pat.linebytes = pat.width * (pat.pf_type>>8)>>3;
            pat.bm_bits = m_malloc(pat.linebytes * pat.height,0);
            if(pat.bm_bits == NULL)     //无法申请pattern所需要的缓存
                return;
            //格式化pattern
            if(display->format_pattern(pat.bm_bits,&para->pattern))
            {
            }else
            {//软件实现格式化pattern
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
    //说明有win buffer，且直接写屏属性为false
    if((bitmap->bm_bits != NULL) && (para->gkwin->direct_screen == false))     
    {   //处理方法:在win buffer中绘图，标志changed_msk
        if(!display->draw.blt_bm_to_bm(bitmap,&target,
            &para->bitmap,0,0,pat.bm_bits,&para->bitmsk,
            para->rop_code,para->transparentcolor))
        {
            dst = bitmap;
            src = &para->bitmap;

            y_dst= target.top;
            //给定的光栅操作码为扩展的光栅操作码
            if((para->rop_code & cn_rop_ext_en) != 0)
            {
                //如果使用四元光柵操作，而沒有给定需要的掩码位图，直接返回
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
            else//给定的光栅操作码为三元光栅操作码
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
        __gk_shading_rect(para->gkwin,&target);    //设置绘制部分的changed_msk
    }
    else
    {
        clip = para->gkwin->visible_clip;
        if(clip == NULL)                            //窗口可视域为空，直接返回
            return ;
        fb_gkwin = display->frame_buffer;
        //有frame buffer，且直接写屏属性为false
        if((fb_gkwin != NULL) && (para->gkwin->direct_screen == false))     
        {   //处理方法:在frame_buffer中绘图，但只绘gkwin中的可视区域
            //为与可视域坐标保持一致，将待位图要绘制的位置的
            //坐标调整成以screen原点为原点的坐标
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
                        //给定的光栅操作码为扩展的光栅操作码
                        if((para->rop_code & cn_rop_ext_en) != 0)
                        {
                            //如果使用四元光柵操作，而沒有给定需要的掩码位图，
                            //直接返回
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
                        else//给定的光栅操作码为三元光栅操作码
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
                    //设置绘制部分的changed_msk
                    __gk_shading_rect(fb_gkwin,&ins_rect);
                }
                clip = clip->next;
            }while(clip != para->gkwin->visible_clip);
        }
        else       
        //光栅位图不支持直接写屏，无win buffer，也无frame buffer，
        //或者直接写屏属性为true的情况，直接返回
        {
            return;
        }
    }
}
//----填充窗口的一部分---------------------------------------------------------
//功能: 用指定的颜色填充窗口的一部分。
//参数: 参数是结构para的成员
//      gkwin，目标窗口
//      rect，待填充的矩形
//      color，填充使用的颜色
//返回: 无
//-----------------------------------------------------------------------------
void gk_fill_part_win(struct gksc_para_fill_part_win *para)//确认
{
    s32 x,y;
    struct st_rect_bitmap *bitmap;
    struct st_rect target,ins_rect;
    struct disp_draw *my_draw_fun;
    struct clip_rect *clip;
    struct gkwin_rsc *fb_gkwin;

    bitmap = &para->gkwin->wm_bitmap;
    //待填充矩形要绘制的位置的坐标，相对于目标窗口
    target = para->rect;
    if((target.right <= 0) || (target.left >= bitmap->width)
        ||(target.bottom <= 0) || (target.top >= bitmap->height))
        return;                             //绘图区域在窗口外面
        
    if(target.left < 0) target.left = 0;    //调整左边界在窗口内部
    if(target.top < 0) target.top = 0;      //调整上边界在窗口内部
    if(target.right > bitmap->width)        //调整右边界在窗口内部
        target.right = bitmap->width;
    if(target.bottom > bitmap->height)      //调整下边界在窗口内部
        target.bottom = bitmap->height;

    my_draw_fun = &para->gkwin->disp->draw;
    //说明有win buffer，且直接写屏属性为false
    if((para->gkwin->wm_bitmap.bm_bits != NULL)
        && (para->gkwin->direct_screen == false))     
    {   
        //处理方法:在win buffer中绘图，着色changed_msk
        if(!my_draw_fun->fill_rect_bm(bitmap,&target,para->color))
        {
            //硬件加速不支持填充位图，则用软件实现
            __gk_fill_rect(bitmap,&target,para->color);
        }
        __gk_shading_rect(para->gkwin,&para->rect);//着色填充区域的changed_msk
    }else
    {
        clip = para->gkwin->visible_clip;
        if(clip == NULL)                //窗口可视域为空，直接返回
            return ;
        fb_gkwin = para->gkwin->disp->frame_buffer;
        //接下来的绘制，是在帧缓冲或者screen上绘制，使用绝对坐标，变换之
        target.left += para->gkwin->absx0;
        target.right += para->gkwin->absx0;
        target.top += para->gkwin->absy0;
        target.bottom += para->gkwin->absy0;
        //有frame buffer,且直接写屏属性为false
        if((fb_gkwin != NULL) && (para->gkwin->direct_screen == false))         
        {   //处理方法:在frame buffer中绘图，但只绘gkwin中的可视区域
            do{
                //只填充窗口可视域与要绘制的区域的交集
                if(__gk_get_rect_ints(&clip->rect,&target,&ins_rect))
                {
                    //帧缓冲的坐标即绝对坐标，故无须坐标变换
                    if(!my_draw_fun->fill_rect_bm(&fb_gkwin->wm_bitmap,
                                                        &ins_rect,para->color))
                    {
                        //硬件加速不支持填充位图，则用软件实现
                        __gk_fill_rect(&fb_gkwin->wm_bitmap,
                                            &ins_rect,para->color);
                    }
                    //标志填充区域的changed_msk
                    __gk_shading_rect(fb_gkwin,&ins_rect);
                }
                clip = clip->next;
            }while(clip != para->gkwin->visible_clip);
        }
        else       
        //无win buffer，也无frame buffer，直接画在screen上
        //直接写屏属性为true，不管有无缓冲区，都直接画在screen上
        {
            do{
                struct display_rsc *mirror,*current;
                //只填充窗口可视域与要绘制的区域的交集
                if(__gk_get_rect_ints(&clip->rect,&target,&ins_rect))
                {
                    //硬件加速不支持填充位图，则用软件实现
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
                    if(mirror == NULL)      //没有镜像显示器，直接返回
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

//----填充窗口-----------------------------------------------------------------
//功能: 用指定的颜色填充窗口。
//参数: 参数是结构para的成员
//      gkwin，目标窗口指针
//      color，填充使用的颜色
//返回: 无
//-----------------------------------------------------------------------------
void gk_fill_win(struct gksc_para_fill_win *para)//确认
{
    s32 x,y;
    struct st_rect rect;
    struct clip_rect *clip;
    struct gkwin_rsc *fb_gkwin;
    struct st_rect_bitmap *bitmap;

    struct disp_draw *my_draw_fun;

    my_draw_fun = &para->gkwin->disp->draw;
    //说明有win buffer，且直接写屏属性为false
    if((para->gkwin->wm_bitmap.bm_bits != NULL)
        && (para->gkwin->direct_screen == false))     
    {   //处理方法:在win buffer中绘图，标志changed_msk
        bitmap = &para->gkwin->wm_bitmap;
        rect.left = 0;
        rect.top = 0;
        rect.right = bitmap->width;
        rect.bottom = bitmap->height;
        if(!my_draw_fun->fill_rect_bm(bitmap,&rect,para->color))
        {//硬件加速不支持填充位图，则用软件实现
            __gk_fill_bm(bitmap,para->color);
        }
        para->gkwin->change_flag = cn_gkwin_change_all;
    }else
    {
        clip = para->gkwin->visible_clip;
        if(clip == NULL)
            return ;
        fb_gkwin = para->gkwin->disp->frame_buffer;
        //有frame buffer，且直接写屏属性为false
        if((fb_gkwin != NULL) && (para->gkwin->direct_screen == false))         
        {   //处理方法:在frame buffer中绘图，但只绘gkwin中的可视区域
            bitmap = &fb_gkwin->wm_bitmap;
            do{
                //帧缓冲的坐标和clip中的坐标都是绝对坐标，无须变换
                if(!my_draw_fun->fill_rect_bm(bitmap,&clip->rect,para->color))
                {//硬件加速不支持填充位图，则用软件实现
                    __gk_fill_rect(bitmap,&clip->rect,para->color);
                }
                //标志填充区域的changed_msk
                __gk_shading_rect(fb_gkwin,&clip->rect);
                clip = clip->next;
            }while(clip != para->gkwin->visible_clip);
        }else       
        //无win buffer，也无frame buffer，直接画在screen上
        //直接写屏属性为true，不管有无缓冲区，都直接画在screen上
        {
            do{
                struct display_rsc *mirror,*current;
                if(!my_draw_fun->fill_rect_screen(&clip->rect,para->color))
                {//硬件加速不支持填充screen上的矩形域，则用软件实现
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
                if(mirror == NULL)      //没有镜像显示器，直接返回
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

