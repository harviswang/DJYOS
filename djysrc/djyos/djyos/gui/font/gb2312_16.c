//----------------------------------------------------
//Copyright (C), 2011,  .
//版权所有 (C), 2011,   djy.
//所属模块: GB2312-80 16*16点阵宋体驱动
//作者：mjh
//版本：V1.0.0
//文件描述: GB2312-80字体解析
//其他说明:
//修订历史:
//    2. ...
//    1. 日期:2011-05-17
//       作者:lst
//       新版本号：
//       修改说明:初始版本
//------------------------------------------------------
#include "os_inc.h"

#if (cfg_gui_kernel == 1)
#if (cfg_gb2312_8x16_1616 == 1)

#include "gk_inc.h"

#include "gb2312_16.h"

extern const u8 cs_ascii_8x16[];
bool_t __gb2312_816_1616_load_font(u8 *zk_addr);
void __gb2312_816_1616_unload_font(void);
bool_t __gb2312_8x16_1616_get_char_bitmap(u32 charcode, u32 size,u32 resv,
                                    struct st_rect_bitmap *bitmap);

u8 *pg_gb2312_song_dot16;
u8 *pg_ascii_dot816;

#define FONT_HZ16_DEF_CHAR      0xa1a1
#define FONT_HZ16_W             16              // Width
#define FONT_HZ16_H             16              // Height
#define FONT_HZ16_GLYPH_BYTES   32              // 字符字模的字节数
s32 __gb2312_ucs4_to_mb (u8* mb, u32 wc);


bool_t __gb2312_816_1616_load_font(u8* zk_addr)
{
    pg_ascii_dot816 = (u8*)cs_ascii_8x16;

    if(zk_addr != 0)
    {
        pg_gb2312_song_dot16 = (u8*)zk_addr;
        return true;
    }
    else
    {
#if ((cfg_djyfs == 1) && (cn_run_mode != cn_mode_tiny))
    	djyfs_file *zk_gb2312;
        //从字库中加载汉字点阵
        zk_gb2312 = djyfs_fopen("sys:\\gb2312_1616","rb");
        if(zk_gb2312 == NULL)
            return false;
        pg_gb2312_song_dot16 = m_malloc(32*8813,0);
        if(pg_gb2312_song_dot16 != NULL)
        {
            djyfs_fread(pg_gb2312_song_dot16,32*8813,1,zk_gb2312);
        }
        djyfs_fclose(zk_gb2312);
        return true;
#else
        return false;
#endif
    }
}


void __gb2312_816_1616_unload_font(void)
{
    // Nothing ...
}

//----点阵提取-----------------------------------------------------------------
//功能: 提取16点阵宋体汉字和ascii 8*16点阵字体，汉字仅限于gb2312字符集内，超出的
//      显示gb2312编码为0xa1a1的字符，ascii仅限于基本ascii码，对于扩展的ascii
//      不予显示
//      如果bitmap参数中的bm_bits参数为NULL，则不copy点阵
//参数: charcode，ucs4字符编码，可能是gb2312字符集内的汉字，也可能是ascii码
//      size，无效
//      resv，无效
//      bitmap，保存所提取的点阵的位图，缓冲区由调用者提供
//返回: true=正常，false=charcode不是该字体所支持的字符集范围，但此时仍然返回
//      默认字符的点阵
//-----------------------------------------------------------------------------
bool_t __gb2312_8x16_1616_get_char_bitmap(u32 charcode, u32 size,u32 resv,
                                    struct st_rect_bitmap *bitmap)
{
    u8 i;
    u8 gbcode[2];  // GB2312内码
    u32 offset;
    bool_t result = true;


    // 得到字符字模在数组中的偏移量
    if(__gb2312_ucs4_to_mb(gbcode, charcode) == -1)
    {
        gbcode[0] = FONT_HZ16_DEF_CHAR &0xff;
        gbcode[1] = (FONT_HZ16_DEF_CHAR>>8) &0xff;
        result = false;
    }
    bitmap->pf_type = cn_sys_pf_gray1;
    if(charcode < 0x80)
    {
        offset = charcode * 16;
        bitmap->width = 8;
        bitmap->height = 16;
        bitmap->linebytes = 1;
        bitmap->pf_type = cn_sys_pf_gray1;
        if(bitmap->bm_bits != NULL)
        {
            for(i=0; i<16; i++)
            {
                bitmap->bm_bits[i] = pg_ascii_dot816[offset + i]; 
            }
        }
    }else
    {
        offset = (94*(gbcode[1]-0xa1) + gbcode[0] - 0xa1)*32;
        bitmap->width = FONT_HZ16_W;
        bitmap->height = FONT_HZ16_H;
        bitmap->linebytes = 2;
        bitmap->pf_type = cn_sys_pf_gray1;
        if(bitmap->bm_bits != NULL)
        {
            for(i=0; i<FONT_HZ16_GLYPH_BYTES; i++)
            {
                bitmap->bm_bits[i] = pg_gb2312_song_dot16[offset + i]; 
            }
        }
    }

    return result;
}

//----安装gb2312 16点阵字体----------------------------------------------------
//功能: 安装gb2312 16点阵字体。
//参数: 无意义
//返回: 1=成功，0=失败
//-----------------------------------------------------------------------------
ptu32_t module_init_font_gb2312_1616(ptu32_t para)
{
    static struct font_rsc font_gb2312_8x16_1616;
    __gb2312_816_1616_load_font((u8*)para);  //加载字库
    font_gb2312_8x16_1616.fi.type = cn_font_type_dot;
    font_gb2312_8x16_1616.load_font = __gb2312_816_1616_load_font;
    font_gb2312_8x16_1616.unload_font = __gb2312_816_1616_unload_font;
    font_gb2312_8x16_1616.get_char_bitmap = __gb2312_8x16_1616_get_char_bitmap;
    
    if(font_install_font(&font_gb2312_8x16_1616, cn_font_gb2312_8x16_1616))
    {
    	printf_ng("gb2312 16x16 font install sucess\n\r");
        return 1;
    }else
    {
        djy_error_login(enum_gk_font_install_error,
                        "gb2312 16x16 font install fail");
        return 0;
    }
}
#endif      //#if (cfg_gb2312_8x16_1616 == 1)
#endif //   for #if (cfg_gui_kernel == 1)

