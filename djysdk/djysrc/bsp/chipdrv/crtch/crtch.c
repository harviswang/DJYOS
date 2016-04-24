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
// =============================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ���     ��crtouch.c
// ģ������: ����оƬCRTOUCH����
// ģ��汾: V1.10
// ������Ա: hm
// ����ʱ��: 15/10.2014
// =============================================================================

#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include "stdint.h"

#include "int_hard.h"
#include "int.h"
#include "cpu_peri.h"
#include "Touch.h"
#include "iicbus.h"
#include "gkernel.h"
#include "systime.h"

#define CRT_XY_AD    0x03
#define CRT_STATUS   0X01

//����IICBUS�ܹ��µ�IIC�豸�ṹ
static struct tagIIC_Device *ps_CRT_Dev = NULL;
static u32 s_CRT_Timeout = CN_TIMEOUT_FOREVER;

#define CRT_ADDRESS			0x49			//�豸��ַ
#define CRT_CLK_FRE			(100*1000)		//�����ٶȣ���λHz

#define CRT_SR_RTST_MASK       (1<<7)

// =============================================================================
// ���ܣ���ȡ������AD����ֵ���ֱ�ΪXֵ��Yֵ��δ��У��������
// ������X,Y������������洢��ȡ�������ݵ�ַ
// ���أ�true,��ȡ�ɹ�;false,��ȡʧ��
// =============================================================================
static bool_t CRT_ReadXY(s32 *X, s32 *Y)
{
    u8 buf[4],RegAddr,wBytesNum;
    bool_t result = false;

    RegAddr = CRT_XY_AD;
    wBytesNum = 4;
    wBytesNum = IIC_Read(ps_CRT_Dev,RegAddr,buf,wBytesNum,s_CRT_Timeout);
    if(4 == wBytesNum)
    {
        *X = (buf[0]<<8) + buf[1];
        *Y = (buf[2]<<8) + buf[3];
        result = true;
    }

    return result;
}

// =============================================================================
// ���ܣ���ȡ��ʱ�Ƿ��������д���
// ��������
// ���أ�1,����;0,δ����
// =============================================================================
static bool_t CRT_Touched(void)
{
    u8 buf,RegAddr,wBytesNum;
    bool_t result = false;

    RegAddr = CRT_STATUS;
    wBytesNum = 1;
    wBytesNum = IIC_Read(ps_CRT_Dev,RegAddr,&buf,wBytesNum,s_CRT_Timeout);
    if(1 == wBytesNum)
    {
        if(buf & CRT_SR_RTST_MASK)
            result = true;
    }
    return result;
}

// =============================================================================
// ���ܣ�CRTCHоƬ��ʼ��������WPд�����������ų�ʼ���ͼ����豸����Ӧ��IIC����
// ��������
// ���أ�true,�ɹ�;false,ʧ��
// =============================================================================
bool_t CRT_Init(void)
{

	static struct tagIIC_Device s_CRT_Dev;

	//��ʼ��IIC�豸�ṹ��
	s_CRT_Dev.DevAddr 					= CRT_ADDRESS;
	s_CRT_Dev.BitOfMemAddr 				= 8;
	s_CRT_Dev.BitOfMemAddrInDevAddr 	= 0;

	//���CRTCH��IIC0����
	if(NULL != IIC_DevAdd_s("IIC0","IIC_Dev_CRTCH",&s_CRT_Dev))
	{
		ps_CRT_Dev = &s_CRT_Dev;
		IIC_BusCtrl(ps_CRT_Dev,CN_IIC_SET_CLK,CRT_CLK_FRE,0);
		IIC_BusCtrl(ps_CRT_Dev,CN_IIC_SET_POLL,0,0);
		return true;
	}

	return false;
}

/*============================================================================*/

#define	TS_ORG_LD

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
bool_t ModuleInstall_TOUCH_TWR_K70(struct tagGkWinRsc *desktop,const char *touch_dev_name)
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

    if(!touch_hard_init())
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
	return CRT_Init();
}

static  bool_t ts_is_down(void)
{
	return CRT_Touched();
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

static bool_t	ts_get_data_raw(s32 *x,s32 *y)
{
	if(ts_is_down())
	{
		CRT_ReadXY(x,y);
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
		ts_get_data_raw(&x,&y);
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
					ts_get_data_raw(&adx,&ady);
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
