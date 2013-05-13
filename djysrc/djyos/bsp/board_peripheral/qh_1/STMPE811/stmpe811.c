//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//版权所有 (C), 2004-2009,   罗侍田.
//所属模块:标准输入--触摸屏模块
//作者:  罗侍田.
//版本：V1.1.0
//文件描述: 触摸屏硬件驱动程序
//其他说明:
//修订历史:
//1. 日期: 2009-01-04
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------

#include "os_inc.h"

#if (cfg_single_touch == 1)

#include "stm32f10x.h"
#include "Touch.h"
#include "string.h"
#include "stmpe811.h"
#include "I2C2.h"
#include "TS_I2C.h"

extern struct st_touch_adjust tg_touch_adjust;

void touch_ratio_adjust(char *display_name,struct st_touch_adjust *adjust);
//----触摸屏硬件初始化---------------------------------------------------------
//功能: 触摸屏硬件初始化
//参数: 无
//返回: 键盘设备号，-1表示按照键盘设备失败
//----------------------------------------------------------------------------
bool_t touch_hard_init(void)
{
    u16 chipid;

    I2C2_Init();
    chipid = TS_Read (CHIP_ID, 2);                      //Read CHIP_ID
                                                      
    if(chipid ==0x811)                                  
    {                                                   
        TS_Write(SYS_CTRL2, 1, 0x0C);                   //打开TSC及ADC的时钟
        TS_Write(INT_EN, 1, 0x07);                      //使能中断
        //ADC分辨率（12bit），参考电压（内部）及采样时间（124）
        TS_Write(ADC_CTRL1 , 1, 0x68);
        TS_Write(ADC_CTRL2 , 1, 0x01);                  //ADC采样频率（3.25Mhz）
        TS_Write(GPIO_AF , 1, 0x00);                    //端口选择为触摸屏使用
        TS_Write(TSC_CFG, 1, 0x92);                     //TSC_CFG
        TS_Write (FIFO_TH, 1, 16);          //触摸屏的触发门限1（测量的是单击）
        //TS_Write (FIFO_TH, 1, 0x05);        //触摸屏的触发门限5（测量的是轨迹）
        TS_Write(FIFO_STA, 1, 0x01);                    //FIFO复位
        TS_Write(FIFO_STA, 1, 0x00);                      
        TS_Write(TSC_FRACT_XYZ, 1, 0x07);                 
        TS_Write(TSC_I_DRIVE, 1, 0x01);                   
                                                          
        TS_Write(TSC_CTRL, 1, 0x01 | stmpe811_opmode<<1); //使能TSC
        TS_Write(INT_STA, 1, 0xFF);                       //清除所有中断
        TS_Write(INT_CTRL, 1, 0x01);                      //不使能TSC
        return true;
    }
    else
    {
        return false;
    }

}

//----读取触摸点坐标-----------------------------------------------------------
//功能: 读取stmpe811采集到的触摸点坐标，如果有多点，则平均之
//参数: 无
//返回: 采集到的坐标
//-----------------------------------------------------------------------------
ufast_t touch_scan_hard(struct single_touch_msg *touch_data)
{
    s32 i, n, tch_data;
    s32 tch_x,tch_y,tch_z;
    u8 tmp,tch_int;
    
    tch_int = TS_Read (INT_STA, 1);     //Read Touch-screen interrupt status
                                                      
    if ((tch_int & 0x02))                 //过了门限,或FIFO满，或FIFO溢出
    {                                                 
        tmp = TS_Read(INT_EN, 1);                       
        TS_Write(INT_EN, 1, tmp &( ~(tch_int & 0x0E)));  //读取数据时需要禁止中断

        TS_Write(INT_STA, 1, 0xff);  //读取数据时需要禁止中断

        i = TS_Read (FIFO_SIZE, 1);        //FIFO中的采样次数
        n=i;                                                
        tch_x=0;                                            
        tch_y=0;                                            
        tch_z=0;                                            
                                                          
#if (stmpe811_opmode == 0)
                                                          
        i <<= 2;                                            
        while (i--)                       //需要将这些字节读出来，直到小于门限
        {
            tch_data = TS_Read(TSC_DATA, 1);
            touch_data->x = tch_data;
            tch_data |= TS_Read(TSC_DATA, 1);
            touch_data->x <<= 4;
            touch_data->x |= tch_data>>4;
            
            touch_data->y = tch_data & 0x0F;
            tch_data = TS_Read(TSC_DATA, 1);
            touch_data->y <<= 4;
            touch_data->y |= tch_data;
            touch_data->z = TS_Read(TSC_DATA, 1);
    
            tch_x +=touch_data->x;
            tch_y +=touch_data->y;
            tch_z +=touch_data->z;
        }
    
#elif (stmpe811_opmode == 1)
        while (i--)
        {
            tch_data = TS_Read(TSC_DATA, 1);
            touch_data->x = tch_data<<4;
            tch_data = TS_Read(TSC_DATA, 1);
            touch_data->x |= tch_data>>4;
            touch_data->y = (tch_data & 0x0F)<<8;
            tch_data = TS_Read(TSC_DATA, 1);
            touch_data->y |= tch_data;
    
            tch_x +=touch_data->x;
            tch_y +=touch_data->y;
            tch_z++;
        }
#endif

        //还有采样点未读完或FIFO满或FIFO溢出
        if (TS_Read(FIFO_SIZE, 1) || (tch_int & 0x0C))  
        {                                               
            TS_Write(FIFO_STA, 1, 0x01);                //FIFO复位
            TS_Write(FIFO_STA, 1, 0x00);                  
        }                                               
                                                      
        TS_Write(INT_EN, 1, tmp | (tch_int & 0x0E));    //重新使能中断
        TS_Write(INT_STA, 1, 0x02);        
        
        tch_x /=n;       //平均多次座标值
        tch_y /=n;
        tch_z /=n;

        if(tg_touch_adjust.ratio_x != 0)
        {
            touch_data->x = ((tch_x << 16)- tg_touch_adjust.offset_x)
                                / tg_touch_adjust.ratio_x;
        }
        
        if(tg_touch_adjust.ratio_y != 0)
        {
            touch_data->y = ((tch_y << 16)- tg_touch_adjust.offset_y)
                                / tg_touch_adjust.ratio_y;                      
        }
        touch_data->z = tch_z;
        return 1;                                  
    }
    else
    {
        return 0;                                  
    }
}                                                       
                                                        
//----触摸屏校准---------------------------------------------------------------
//功能: 触摸屏的尺寸可能与液晶可显示区域不完全一致，安装也可能有偏差，需要计算
//      校准系数和偏移量。为获得更高精度，使用定点小数。
//参数: display_name,本触摸屏对应的显示器名(资源名)
//      adjust,用于返回校准结果。
//返回: 无
//-----------------------------------------------------------------------------
void touch_ratio_adjust(char *display_name,struct st_touch_adjust *adjust)
{
    struct gkwin_rsc *desktop;
    struct single_touch_msg touch_xyz0,touch_xyz1;
    djyfs_file *touch_init;
    s32 limit_left,limit_top,limit_right,limit_bottom;

    if(djyfs_fsearch("sys:\\touch_init.dat"))
    {
        touch_init = djyfs_fopen("sys:\\touch_init.dat","r");
        djyfs_fread(&tg_touch_adjust,sizeof(tg_touch_adjust),1,touch_init);
    }
    else
    {
		desktop = gk_api_get_desktop(display_name);

		limit_left = desktop->limit_left;
		limit_top = desktop->limit_top;
		limit_right = desktop->limit_right;
		limit_bottom = desktop->limit_bottom;
	//    gk_api_create_gkwin(desktop,desktop,limit_left,limit_top,limit_right,limit_bottom,
	//    					cn_color_white,cn_winbuf_buf,"adjust",cn_r3_srccopy,0);
	//    gk_api_set_prio(desktop,-1,cn_gk_sync);
		gk_api_fill_win(desktop,cn_color_white,0);
		gk_api_draw_text(desktop,limit_left+10,limit_top+50,
							"触摸屏矫正",21,0);
		gk_api_draw_text(desktop,limit_left+10,limit_top+70,
							"请准确点击十字交叉点",21,0);
		gk_api_lineto(desktop,0,20,40,20,cn_color_red,cn_r2_copypen,0);
		gk_api_lineto(desktop,20,0,20,40,cn_color_red,cn_r2_copypen,cn_timeout_forever);

		while(!touch_scan_hard(&touch_xyz0));           //记录触摸屏第一点校正值

		gk_api_fill_win(desktop,cn_color_white,0);
		gk_api_draw_text(desktop,limit_left+10,limit_top+50,
							"触摸屏矫正",21,0);
		gk_api_draw_text(desktop,limit_left+10,limit_top+70,
							"再次准确点击十字交叉点",21,0);
		gk_api_lineto(desktop,limit_right-40,limit_bottom-20,
					  limit_right,limit_bottom-20,
					  cn_color_red,cn_r2_copypen,0);
		gk_api_lineto(desktop,limit_right-20,limit_bottom-40,
					  limit_right-20,limit_bottom,
					  cn_color_red,cn_r2_copypen,cn_timeout_forever);

		if (TS_Read(FIFO_SIZE, 1))
		{
			TS_Write(FIFO_STA, 1, 0x01);                //FIFO复位
			TS_Write(FIFO_STA, 1, 0x00);
		}

		while(!touch_scan_hard(&touch_xyz1));           //记录触摸屏第二点校正值

		adjust->ratio_x = ((touch_xyz1.x - touch_xyz0.x)<<16)
						/(limit_right - limit_left -40);
		adjust->offset_x = (touch_xyz0.x<<16) - 20*adjust->ratio_x;
		adjust->ratio_y = ((touch_xyz1.y - touch_xyz0.y)<<16)
						/(limit_bottom- limit_top-40);
		adjust->offset_y= (touch_xyz0.y<<16) - 20*adjust->ratio_y;
		gk_api_fill_win(desktop,cn_color_white,0);
	//    gk_destroy_win(desktop);
        touch_init = djyfs_fopen("sys:\\touch_init.dat","w+");
        djyfs_fwrite(&tg_touch_adjust,sizeof(tg_touch_adjust),1,touch_init);
    }
    djyfs_fclose(touch_init);

}

#endif  //for #if (cfg_single_touch == 1)
