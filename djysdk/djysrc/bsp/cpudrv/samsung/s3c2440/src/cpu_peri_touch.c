//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

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
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------

#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include "stdint.h"

#include "int_hard.h"
#include "int.h"
#include "cpu_peri.h"
#include "Touch.h"
#include "gkernel.h"
#include "systime.h"

/*============================================================================*/
#define ADC_REG_BASE       (ADC_REG*)0x58000000

typedef struct tagADC_REG{
	vu32 ADCCON;
	vu32 ADCTSC;
	vu32 ADCDLY;
	vu32 ADCDAT0;
	vu32 ADCDAT1;
	vu32 ADCUPDN;
}ADC_REG ;

#define ADC_MCLK    (50*1000*1000)
#define ADC_CLK     2000000 /*2MHZ*/

/*============================================================================*/

#define	TS_XY_SWAP
#define	TS_ORG_RD

#define TS_CAL_XOFFSET      (40)
#define TS_CAL_YOFFSET      (40)

#define TS_CAL_LU_XPOS      (TS_CAL_XOFFSET)
#define TS_CAL_LU_YPOS      (TS_CAL_YOFFSET)

#define TS_CAL_RU_XPOS      (LCD_XSIZE-TS_CAL_XOFFSET)
#define TS_CAL_RU_YPOS      (TS_CAL_YOFFSET)

#define TS_CAL_LD_XPOS      (TS_CAL_XOFFSET)
#define TS_CAL_LD_YPOS      (LCD_YSIZE-TS_CAL_YOFFSET)

#define TS_CAL_RD_XPOS      (LCD_XSIZE-TS_CAL_XOFFSET)
#define TS_CAL_RD_YPOS      (LCD_YSIZE-TS_CAL_YOFFSET)

#define TS_CAL_XSIZE        (TS_CAL_RU_XPOS-TS_CAL_LU_XPOS)
#define TS_CAL_YSIZE        (TS_CAL_RD_YPOS-TS_CAL_RU_YPOS)

typedef struct  tagTS_CFG_DATA
{
    u16 LUAdx,LUAdy;
    u16 RUAdx,RUAdy;
    u16 RDAdx,RDAdy;
    u16 LDAdx,LDAdy;
}TS_CFG_DATA;

///////////////////

static	int ts_xsize,ts_ysize;
static	int	ts_cal_ref_pos[4][2];
static  TS_CFG_DATA ts_cfg_data;   //��������

#define	LCD_XSIZE	ts_xsize
#define	LCD_YSIZE	ts_ysize

/*============================================================================*/
/*============================================================================*/

static bool_t ts_translate_data(s32 *x,s32 *y)
{
    int x_coor,y_coor,adx,ady;
    int ts_adx_min,ts_adx_max,ts_ady_min,ts_ady_max;

    if((x!=NULL)&&(y!=NULL))
    {
        adx =*x;
        ady =*y;

#ifdef	TS_ORG_LU
        ts_adx_min  =ts_cfg_data.LUAdx;
        ts_ady_min  =ts_cfg_data.LUAdy;

        ts_adx_max  =ts_cfg_data.RDAdx;
        ts_ady_max  =ts_cfg_data.RDAdy;

        if((ts_adx_min==ts_adx_max) || (ts_ady_min==ts_ady_max))
        {
            *x  =-1;
            *y  =-1;
            return  false;
        }

#ifdef	TS_XY_SWAP
        x_coor = TS_CAL_XOFFSET+(LCD_XSIZE-(TS_CAL_XOFFSET<<1))*(ady-ts_ady_min)/ (ts_ady_max-ts_ady_min);
        y_coor = TS_CAL_YOFFSET+(LCD_YSIZE-(TS_CAL_YOFFSET<<1))*(adx-ts_adx_min)/ (ts_adx_max-ts_adx_min);
#else
        x_coor = TS_CAL_XOFFSET+(LCD_XSIZE-(TS_CAL_XOFFSET<<1))*(adx-ts_adx_min)/ (ts_adx_max-ts_adx_min);
        y_coor = TS_CAL_YOFFSET+(LCD_YSIZE-(TS_CAL_YOFFSET<<1))*(ady-ts_ady_min)/ (ts_ady_max-ts_ady_min);
#endif
        *x =x_coor;
        *y =y_coor;
#endif

#ifdef	TS_ORG_RU
        ts_adx_min  =ts_cfg_data.RUAdx;
        ts_ady_min  =ts_cfg_data.RUAdy;

        ts_adx_max  =ts_cfg_data.LDAdx;
        ts_ady_max  =ts_cfg_data.LDAdy;

        if((ts_adx_min==ts_adx_max) || (ts_ady_min==ts_ady_max))
        {
            *x  =-1;
            *y  =-1;
            return  false;
        }

#ifdef	TS_XY_SWAP
        x_coor = TS_CAL_XOFFSET+(LCD_XSIZE-(TS_CAL_XOFFSET<<1))*(ady-ts_ady_min)/ (ts_ady_max-ts_ady_min);
        y_coor = TS_CAL_YOFFSET+(LCD_YSIZE-(TS_CAL_YOFFSET<<1))*(adx-ts_adx_min)/ (ts_adx_max-ts_adx_min);
#else
        x_coor = TS_CAL_XOFFSET+(LCD_XSIZE-(TS_CAL_XOFFSET<<1))*(adx-ts_adx_min)/ (ts_adx_max-ts_adx_min);
        y_coor = TS_CAL_YOFFSET+(LCD_YSIZE-(TS_CAL_YOFFSET<<1))*(ady-ts_ady_min)/ (ts_ady_max-ts_ady_min);
#endif
        *x  =LCD_XSIZE-x_coor;
        *y  =y_coor;
#endif

#ifdef	TS_ORG_RD
        ts_adx_min  =ts_cfg_data.RDAdx;
        ts_ady_min  =ts_cfg_data.RDAdy;

        ts_adx_max  =ts_cfg_data.LUAdx;
        ts_ady_max  =ts_cfg_data.LUAdy;

        if((ts_adx_min==ts_adx_max) || (ts_ady_min==ts_ady_max))
        {
            *x  =-1;
            *y  =-1;
            return  false;
        }

#ifdef	TS_XY_SWAP
        x_coor = TS_CAL_XOFFSET+(LCD_XSIZE-(TS_CAL_XOFFSET<<1))*(ady-ts_ady_min)/ (ts_ady_max-ts_ady_min);
        y_coor = TS_CAL_YOFFSET+(LCD_YSIZE-(TS_CAL_YOFFSET<<1))*(adx-ts_adx_min)/ (ts_adx_max-ts_adx_min);
#else
        x_coor = TS_CAL_XOFFSET+(LCD_XSIZE-(TS_CAL_XOFFSET<<1))*(adx-ts_adx_min)/ (ts_adx_max-ts_adx_min);
        y_coor = TS_CAL_YOFFSET+(LCD_YSIZE-(TS_CAL_YOFFSET<<1))*(ady-ts_ady_min)/ (ts_ady_max-ts_ady_min);
#endif
        *x  =LCD_XSIZE-x_coor;
        *y  =LCD_YSIZE-y_coor;
#endif

#ifdef	TS_ORG_LD
        ts_adx_min  =ts_cfg_data.LDAdx;
        ts_ady_min  =ts_cfg_data.LDAdy;

        ts_adx_max  =ts_cfg_data.RUAdx;
        ts_ady_max  =ts_cfg_data.RUAdy;

        if((ts_adx_min==ts_adx_max) || (ts_ady_min==ts_ady_max))
        {
            *x  =-1;
            *y  =-1;
            return  false;
        }

#ifdef	TS_XY_SWAP
        x_coor = TS_CAL_XOFFSET+(LCD_XSIZE-(TS_CAL_XOFFSET<<1))*(ady-ts_ady_min)/ (ts_ady_max-ts_ady_min);
        y_coor = TS_CAL_YOFFSET+(LCD_YSIZE-(TS_CAL_YOFFSET<<1))*(adx-ts_adx_min)/ (ts_adx_max-ts_adx_min);
#else
        x_coor = TS_CAL_XOFFSET+(LCD_XSIZE-(TS_CAL_XOFFSET<<1))*(adx-ts_adx_min)/ (ts_adx_max-ts_adx_min);
        y_coor = TS_CAL_YOFFSET+(LCD_YSIZE-(TS_CAL_YOFFSET<<1))*(ady-ts_ady_min)/ (ts_ady_max-ts_ady_min);
#endif
        *x  =x_coor;
        *y  =LCD_YSIZE-y_coor;

#endif
        return true;
    }
    return  false;
}

/*============================================================================*/


void touch_ratio_adjust(struct tagGkWinRsc *desktop);
static ufast_t read_touch_data(struct tagSingleTouchMsg *touch_data);
static bool_t touch_hard_init(void);

//----��ʼ��������ģ��---------------------------------------------------------
//����:
//����: display_dev_name:����������Ӧ����ʾ����(��Դ��)
//		touch_dev_name:�������豸��.
//����: ��
//-----------------------------------------------------------------------------
bool_t ModuleInstall_TOUCH_S3C2440(struct tagGkWinRsc *desktop,const char *touch_dev_name)
{
    static struct tagSingleTouchPrivate touch_dev;

    ts_xsize =desktop->right-desktop->left;
    ts_ysize =desktop->bottom-desktop->top;

    ts_cal_ref_pos[0][0] =TS_CAL_LU_XPOS;
    ts_cal_ref_pos[0][1] =TS_CAL_LU_YPOS;
    ts_cal_ref_pos[1][0] =TS_CAL_RU_XPOS;
    ts_cal_ref_pos[1][1] =TS_CAL_RU_YPOS;
    ts_cal_ref_pos[2][0] =TS_CAL_RD_XPOS;
    ts_cal_ref_pos[2][1] =TS_CAL_RD_YPOS;
    ts_cal_ref_pos[3][0] =TS_CAL_LD_XPOS;
    ts_cal_ref_pos[3][1] =TS_CAL_LD_YPOS;

    if(!touch_hard_init( ))
    {
        return false;
    }
    touch_ratio_adjust(desktop);
    touch_dev.read_touch = read_touch_data;
    Touch_InstallDevice(touch_dev_name,&touch_dev);
    return true;
}

//----������Ӳ����ʼ��---------------------------------------------------------
//����: ������Ӳ����ʼ��
//����: ��
//����: �����豸�ţ�-1��ʾ���ռ����豸ʧ��
//-----------------------------------------------------------------------------
static bool_t touch_hard_init(void)
{
	int prs;

	ADC_REG *adc=ADC_REG_BASE;

	prs = (ADC_MCLK/ADC_CLK)-1;

	adc->ADCDLY = 4000;                  //Normal conversion mode delay about (1/3.6864M)*50000=13.56ms
	adc->ADCCON = (1<<14) | (prs<<6) | (1<<3)| (0<<2)  ;       //read start
	adc->ADCTSC = 0xd3;  //Wfait,XP_PU,XP_Dis,XM_Dis,YP_Dis,YM_En
    return true;
}

static  bool_t ts_is_down(void)
{
	ADC_REG *adc=ADC_REG_BASE;
    int i,j;
    ///

    for(i=0;i<4;i++)    //���4��
    {
        if((adc->ADCDAT0&(1<<15)))  return  false;
        if((adc->ADCDAT1&(1<<15)))  return  false;
        for(j=0;j<50;j++);  //Delay
    }
    //DebugPrintf("ts_down\r\n");
    return  true;
}

static  void InsertSort(int *A, int p, int r)
{
    int i,j;
    int key;
    for(i=p+1; i<=r; i++)
    {
        key = A[i];
        j = i-1;
        while (j >= 0 && A[j] > key)
        {
            A[j+1] = A[j];
            j--;
        }
        A[j+1] = key;
    }
}

#define TS_SAMPLE_COUNT 8
static  int ad_buf[TS_SAMPLE_COUNT];

static  int ts_get_x_data_raw(void)
{
    int i,j,prs;
    int dat;
    ADC_REG *adc=ADC_REG_BASE;
    /////

    prs  = (ADC_MCLK/ADC_CLK)-1;

    adc->ADCDLY = 1000;
    adc->ADCCON = (1<<14) | (prs<<6)|(1<<3)  ;     //read start
    adc->ADCTSC = (0xF<<4)|(1<<3)|(0<<2)|(1);// read x
    for(j=200;j>0;j--);

    for(i=0;i<TS_SAMPLE_COUNT;i++)
    {
        adc->ADCCON |=0x1;                  //start ADC
        for(j=200;j>0;j--);
        while(adc->ADCCON & 0x1);           //check if Enable_start is low
        while(!(adc->ADCCON & (1<<15)));    //check if EC(End of Conversion) flag is high, This line is necessary~!!

        ad_buf[i]=adc->ADCDAT0&0x3FF;

    }

    adc->ADCTSC = 0xd3;

    InsertSort(ad_buf,0,TS_SAMPLE_COUNT-1);
    i = TS_SAMPLE_COUNT>>1;
    dat = 0;
    dat += ad_buf[i++];
    dat += ad_buf[i++];
    dat = dat>>1;

//  DebugPrintf("ts_adx =%04XH\r\n",dat);

    return dat;
    ////
}

static  int ts_get_y_data_raw(void)
{
    int i,j,prs;
    int dat;
    ADC_REG *adc=ADC_REG_BASE;
    /////

    prs  = (ADC_MCLK/ADC_CLK)-1;

    adc->ADCDLY = 1000;
    adc->ADCCON = (1<<14) | (prs<<6)|(1<<3)  ;     //read start
    adc->ADCTSC = (0xF<<4)|(1<<3)|(0<<2)|(2);// read y
    for(j=200;j>0;j--);

    for(i=0;i<TS_SAMPLE_COUNT;i++)
    {
        adc->ADCCON |=0x1;                  //start ADC
        for(j=200;j>0;j--);
        while(adc->ADCCON & 0x1);           //check if Enable_start is low
        while(!(adc->ADCCON & (1<<15)));    //check if EC(End of Conversion) flag is high, This line is necessary~!!

        ad_buf[i] =adc->ADCDAT1&0x3FF;


    }

    adc->ADCTSC = 0xd3;

    InsertSort(ad_buf,0,TS_SAMPLE_COUNT-1);
    i = TS_SAMPLE_COUNT>>1;
    dat = 0;
    dat += ad_buf[i++];
    dat += ad_buf[i++];
    dat = dat>>1;

//  DebugPrintf("ts_ady =%04XH\r\n",dat);

    return dat;
    ////

}

static bool_t	ts_get_data_raw(s32 *x,s32 *y)
{
	if(ts_is_down())
	{
		x=ts_get_x_data_raw();
		y=ts_get_y_data_raw();
		return true;
	}
	return false;
}

//----��ȡ����������-----------------------------------------------------------
//����: ��ȡstmpe811�ɼ����Ĵ��������꣬����ж�㣬��ƽ��֮
//����: touch_data���ɼ���������
//����: 1=�����ʰ��£�0=����������
//-----------------------------------------------------------------------------
static ufast_t read_touch_data(struct tagSingleTouchMsg *touch_data)
{
	 static s32 x=0,y=0,z=0;

	if(ts_is_down())
	{

		x =ts_get_x_data_raw();
		y =ts_get_y_data_raw();
		z =1;
		ts_translate_data(&x,&y);
		touch_data->x =x;
		touch_data->y =y;
        touch_data->z = z;

        return 1;

	}
	MouseInput(x,y,0);
	return 0;

}


static	void draw_cursor(struct tagGkWinRsc *desktop,int x,int y)
{
	GK_ApiLineto(desktop,x,y,x,y-20+1,CN_COLOR_RED,CN_R2_COPYPEN,0); //��
	GK_ApiLineto(desktop,x,y,x,y+20-1,CN_COLOR_RED,CN_R2_COPYPEN,0); //��
	GK_ApiLineto(desktop,x,y,x+20-1,y,CN_COLOR_RED,CN_R2_COPYPEN,0);//��
	GK_ApiLineto(desktop,x,y,x-20+1,y,CN_COLOR_RED,CN_R2_COPYPEN,0); //��
	GK_ApiSyncShow(1000*mS);
}

static	void clr_cursor(struct tagGkWinRsc *desktop,int x,int y)
{
	GK_ApiLineto(desktop,x,y,x,y-20+1,CN_COLOR_WHITE,CN_R2_COPYPEN,0); //��
	GK_ApiLineto(desktop,x,y,x,y+20-1,CN_COLOR_WHITE,CN_R2_COPYPEN,0); //��
	GK_ApiLineto(desktop,x,y,x+20-1,y,CN_COLOR_WHITE,CN_R2_COPYPEN,0);//��
	GK_ApiLineto(desktop,x,y,x-20+1,y,CN_COLOR_WHITE,CN_R2_COPYPEN,0); //��
	GK_ApiSyncShow(1000*mS);

}

//----������У׼---------------------------------------------------------------
//����: �������ĳߴ������Һ������ʾ������ȫһ�£���װҲ������ƫ���Ҫ����
//      У׼ϵ����ƫ������Ϊ��ø��߾��ȣ�ʹ�ö���С����
//����: display_name,����������Ӧ����ʾ����(��Դ��)
//����: ��
//-----------------------------------------------------------------------------
void touch_ratio_adjust(struct tagGkWinRsc *desktop)
{

    struct tagSingleTouchMsg touch_xyz0,touch_xyz1;
    FILE *touch_init;
    s32 limit_left,limit_top,limit_right,limit_bottom,pen_down_time,step;

    if((touch_init = fopen("sys:\\touch_init.dat","r")) != NULL)
    {

        fread(&ts_cfg_data,sizeof(TS_CFG_DATA),1,touch_init);
    }
    else
    {
        limit_left = desktop->limit_left;
        limit_top = desktop->limit_top;
        limit_right = desktop->limit_right;
        limit_bottom = desktop->limit_bottom;
    //    GK_ApiCreateGkwin(desktop,desktop,limit_left,limit_top,limit_right,limit_bottom,
    //                      CN_COLOR_WHITE,CN_WINBUF_BUF,"&tg_touch_adjust",CN_R3_SRCCOPY,0);
    //    GK_ApiSetPrio(desktop,-1,CN_GK_SYNC);
        GK_ApiFillWin(desktop,CN_COLOR_WHITE,0);
        GK_ApiDrawText(desktop,NULL,NULL,limit_left+10,limit_top+50,
                            "����������",21,CN_COLOR_WHITE,CN_R2_COPYPEN,0);
        GK_ApiDrawText(desktop,NULL,NULL,limit_left+10,limit_top+70,
                            "��׼ȷ���ʮ�ֽ����",21,CN_COLOR_WHITE,CN_R2_COPYPEN,0);

        step=0;
        while(step<4)
        {
        	s32 adx,ady;

        	if(step>0)
        	{
        		clr_cursor(desktop,ts_cal_ref_pos[step-1][0],ts_cal_ref_pos[step-1][1]);
        	}
        	draw_cursor(desktop,ts_cal_ref_pos[step][0],ts_cal_ref_pos[step][1]);

        	while(ts_is_down())
			{
        		Djy_DelayUs(100*mS);
			}
			pen_down_time=0;
			while(1)
			{
				if(ts_is_down())
				{

					adx=ts_get_x_data_raw();
					ady=ts_get_y_data_raw();
					if(pen_down_time++>5)
					{
						break;
					}
				}
				else
				{
					pen_down_time=0;
				}
				Djy_DelayUs(100*mS);
			}

			switch(step)
			{
			case 0:
				ts_cfg_data.LUAdx =adx;
				ts_cfg_data.LUAdy =ady;
				break;

			case 1:
				ts_cfg_data.RUAdx =adx;
				ts_cfg_data.RUAdy =ady;
				break;

			case 2:
				ts_cfg_data.RDAdx =adx;
				ts_cfg_data.RDAdy =ady;
				break;

			case 3:
				ts_cfg_data.LDAdx =adx;
				ts_cfg_data.LDAdy =ady;
				break;

			}
			printf("ts_cal[%d]: %04X,%04X\r\n",step,adx,ady);
			step++;
        }

        GK_ApiFillWin(desktop,CN_COLOR_WHITE,0);
        GK_ApiSyncShow(1000*mS);
    	while(ts_is_down())
		{
    		Djy_DelayUs(100*mS);
		}
    	Djy_DelayUs(500*mS);

    //    GK_DestroyWin(desktop);
        touch_init = fopen("sys:\\touch_init.dat","w+");
        fwrite(&ts_cfg_data,sizeof(TS_CFG_DATA),1,touch_init);
    }
    fclose(touch_init);

}
