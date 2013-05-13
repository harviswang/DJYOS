//----------------------------------------------------
//Copyright (C), 2011,  .
//��Ȩ���� (C), 2011,   djy.
//����ģ��: GB2312-80 16*16������������
//���ߣ�mjh
//�汾��V1.0.0
//�ļ�����: GB2312-80�������
//����˵��:
//�޶���ʷ:
//    2. ...
//    1. ����:2011-05-17
//       ����:lst
//       �°汾�ţ�
//       �޸�˵��:��ʼ�汾
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
#define FONT_HZ16_GLYPH_BYTES   32              // �ַ���ģ���ֽ���
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
        //���ֿ��м��غ��ֵ���
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

//----������ȡ-----------------------------------------------------------------
//����: ��ȡ16�������庺�ֺ�ascii 8*16�������壬���ֽ�����gb2312�ַ����ڣ�������
//      ��ʾgb2312����Ϊ0xa1a1���ַ���ascii�����ڻ���ascii�룬������չ��ascii
//      ������ʾ
//      ���bitmap�����е�bm_bits����ΪNULL����copy����
//����: charcode��ucs4�ַ����룬������gb2312�ַ����ڵĺ��֣�Ҳ������ascii��
//      size����Ч
//      resv����Ч
//      bitmap����������ȡ�ĵ����λͼ���������ɵ������ṩ
//����: true=������false=charcode���Ǹ�������֧�ֵ��ַ�����Χ������ʱ��Ȼ����
//      Ĭ���ַ��ĵ���
//-----------------------------------------------------------------------------
bool_t __gb2312_8x16_1616_get_char_bitmap(u32 charcode, u32 size,u32 resv,
                                    struct st_rect_bitmap *bitmap)
{
    u8 i;
    u8 gbcode[2];  // GB2312����
    u32 offset;
    bool_t result = true;


    // �õ��ַ���ģ�������е�ƫ����
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

//----��װgb2312 16��������----------------------------------------------------
//����: ��װgb2312 16�������塣
//����: ������
//����: 1=�ɹ���0=ʧ��
//-----------------------------------------------------------------------------
ptu32_t module_init_font_gb2312_1616(ptu32_t para)
{
    static struct font_rsc font_gb2312_8x16_1616;
    __gb2312_816_1616_load_font((u8*)para);  //�����ֿ�
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

