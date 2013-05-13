#include "os_inc.h"
#include "cpu_peri.h"
#include "LCD_9325.h"

void djy_main(void);

void __djy_main(void)
{
    djy_main();
}

int app_init(void)
{
    static struct gkwin_rsc desktop;
    struct display_rsc *ili9325;
    uint16_t evtt_main;
    
    ili9325 = (struct display_rsc*)module_init_lcd((ptu32_t)"ili9325");
    
    gk_api_create_desktop(ili9325,&desktop,0,0,
                            cn_color_red+cn_color_green,cn_winbuf_buf);
#if (cfg_single_touch == 1)
#endif

#if (cfg_gui_kernel == 1)
    //以下初始化字符集驱动程序
#if (cfg_charset_gb2312 == 1)       //初始化gb2312字符集
extern ptu32_t module_init_encode_gb2312(ptu32_t para);
    module_init_encode_gb2312(0);
#endif

#if (cfg_charset_ascii == 1)       //初始化ascii字符集
    module_init_encode_ascii(0);
#endif

#if (cfg_charset_utf8 == 1)       //初始化utf8字符集
    module_init_encode_utf8(0);
#endif

//以下初始化字体驱动程序
#if (cfg_gb2312_8x16_1616 == 1)     //初始化gb2312点阵字库
extern ptu32_t module_init_font_gb2312_1616(ptu32_t para);
	module_init_font_gb2312_1616(0);
#endif

#if (cfg_ascii_8x16 == 1)           //初始化8*16 ascii点阵字库
	module_init_font_ascii_8x16(0);
#endif

#if (cfg_ascii_8x8 == 1)           //初始化8*8 ascii点阵字库
extern ptu32_t module_init_font_ascii_8x8(ptu32_t para);
	module_init_font_ascii_8x8(0);
#endif
extern ptu32_t module_init_nls(ptu32_t para);
	module_init_nls(0);
#endif		//for #if (cfg_gui_kernel == 1)

#if((cfg_time_module == 1) && (cfg_time_src == cn_time_src_rtc))
	module_init_rtc(0);
#endif

#if(cfg_uart1_used == 1)
    module_init_uart1(0);
#endif
#if(cfg_uart2_used == 1)
    module_init_uart2(0);
#endif
#if(cfg_uart3_used == 1)
    module_init_uart3(0);
#endif
#if(cfg_uart4_used == 1)
    module_init_uart4(0);
#endif
#if(cfg_uart5_used == 1)
    module_init_uart5(0);
#endif
    evtt_main = djy_evtt_regist(enum_correlative,cn_prio_RRS,0,1, __djy_main,0x1000,"main function");
    //事件的两个参数暂设为0，如果用shell启动，可用来采集shell命令行参数
    djy_event_pop(evtt_main,enum_para_async,NULL,0,NULL,0,0,0);
    return 1;
}

