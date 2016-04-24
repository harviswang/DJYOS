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
//����ģ��:��׼����--������ģ��
//����:  ������.
//�汾��V1.1.0
//�ļ�����: ������Ӳ����������
//����˵��:
//�޶���ʷ:
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------

#include "stdint.h"
#include "stdio.h"
#include "stddef.h"
#include "stm32f10x.h"
#include "Touch.h"
#include "string.h"
#include "stmpe811.h"
#include "I2C2.h"
#include "TS_I2C.h"
#include "gkernel.h"

struct tagST_TouchAdjust tg_touch_adjust;

void touch_ratio_adjust(char *display_name);
ufast_t read_touch_stmpe811(struct tagSingleTouchMsg *touch_data);
bool_t touch_hard_init(void);

//----��ʼ��������ģ��---------------------------------------------------------
//����:
//����: display_name,����������Ӧ����ʾ����(��Դ��)
//����: ��
//-----------------------------------------------------------------------------
ptu32_t ModuleInstall_Touch_Stmpe811(ptu32_t para)
{
    static struct tagSingleTouchPrivate stmpe811;
    if(!touch_hard_init( ))
        return -1;
    touch_ratio_adjust((char*)para);
    stmpe811.read_touch = read_touch_stmpe811;
    Touch_InstallDevice("stmpe811",&stmpe811);
    return 1;
}

//----������Ӳ����ʼ��---------------------------------------------------------
//����: ������Ӳ����ʼ��
//����: ��
//����: �����豸�ţ�-1��ʾ���ռ����豸ʧ��
//-----------------------------------------------------------------------------
bool_t touch_hard_init(void)
{
    u16 chipid;

    I2C2_Init();
    chipid = TS_Read (CHIP_ID, 2);                      //Read CHIP_ID

    if(chipid ==0x811)
    {
        TS_Write(SYS_CTRL2, 1, 0x0C);                   //��TSC��ADC��ʱ��
        TS_Write(INT_EN, 1, 0x07);                      //ʹ���ж�
        //ADC�ֱ��ʣ�12bit�����ο���ѹ���ڲ���������ʱ�䣨124��
        TS_Write(ADC_CTRL1 , 1, 0x68);
        TS_Write(ADC_CTRL2 , 1, 0x01);                  //ADC����Ƶ�ʣ�3.25Mhz��
        TS_Write(GPIO_AF , 1, 0x00);                    //�˿�ѡ��Ϊ������ʹ��
        TS_Write(TSC_CFG, 1, 0x92);                     //TSC_CFG
        TS_Write (FIFO_TH, 1, 16);          //�������Ĵ�������1���������ǵ�����
        //TS_Write (FIFO_TH, 1, 0x05);        //�������Ĵ�������5���������ǹ켣��
        TS_Write(FIFO_STA, 1, 0x01);                    //FIFO��λ
        TS_Write(FIFO_STA, 1, 0x00);
        TS_Write(TSC_FRACT_XYZ, 1, 0x07);
        TS_Write(TSC_I_DRIVE, 1, 0x01);

        TS_Write(TSC_CTRL, 1, 0x01 | stmpe811_opmode<<1); //ʹ��TSC
        TS_Write(INT_STA, 1, 0xFF);                       //��������ж�
        TS_Write(INT_CTRL, 1, 0x01);                      //��ʹ��TSC
        return true;
    }
    else
    {
        return false;
    }

}

//----��ȡ����������-----------------------------------------------------------
//����: ��ȡstmpe811�ɼ����Ĵ��������꣬����ж�㣬��ƽ��֮
//����: touch_data���ɼ���������
//����: 1=�����ʰ��£�0=����������
//-----------------------------------------------------------------------------
ufast_t read_touch_stmpe811(struct tagSingleTouchMsg *touch_data)
{
    s32 i, n, tch_data;
    s32 tch_x,tch_y,tch_z;
    u8 tmp,tch_int;

    tch_int = TS_Read (INT_STA, 1);     //Read Touch-screen interrupt status

    if ((tch_int & 0x02))                 //��������,��FIFO������FIFO���
    {
        tmp = TS_Read(INT_EN, 1);
        TS_Write(INT_EN, 1, tmp &( ~(tch_int & 0x0E)));  //��ȡ����ʱ��Ҫ��ֹ�ж�

        TS_Write(INT_STA, 1, 0xff);  //��ȡ����ʱ��Ҫ��ֹ�ж�

        i = TS_Read (FIFO_SIZE, 1);        //FIFO�еĲ�������
        n=i;
        tch_x=0;
        tch_y=0;
        tch_z=0;

#if (stmpe811_opmode == 0)

        i <<= 2;
        while (i--)                       //��Ҫ����Щ�ֽڶ�������ֱ��С������
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

        //���в�����δ�����FIFO����FIFO���
        if (TS_Read(FIFO_SIZE, 1) || (tch_int & 0x0C))
        {
            TS_Write(FIFO_STA, 1, 0x01);                //FIFO��λ
            TS_Write(FIFO_STA, 1, 0x00);
        }

        TS_Write(INT_EN, 1, tmp | (tch_int & 0x0E));    //����ʹ���ж�
        TS_Write(INT_STA, 1, 0x02);

        tch_x /=n;       //ƽ���������ֵ
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

//----������У׼---------------------------------------------------------------
//����: �������ĳߴ������Һ������ʾ������ȫһ�£���װҲ������ƫ���Ҫ����
//      У׼ϵ����ƫ������Ϊ��ø��߾��ȣ�ʹ�ö���С����
//����: display_name,����������Ӧ����ʾ����(��Դ��)
//����: ��
//-----------------------------------------------------------------------------
void touch_ratio_adjust(char *display_name)
{
    struct tagGkWinRsc *desktop;
    struct tagSingleTouchMsg touch_xyz0,touch_xyz1;
    FILE *touch_init;
    s32 limit_left,limit_top,limit_right,limit_bottom;

    if((touch_init = fopen("sys:\\touch_init.dat","r")) != NULL)
    {

        fread(&tg_touch_adjust,sizeof(struct tagST_TouchAdjust),1,touch_init);
    }
    else
    {
        desktop = GK_ApiGetDesktop(display_name);

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
        GK_ApiLineto(desktop,0,20,40,20,CN_COLOR_RED,CN_R2_COPYPEN,0);
        GK_ApiLineto(desktop,20,0,20,40,CN_COLOR_RED,CN_R2_COPYPEN,CN_TIMEOUT_FOREVER);

        while(!read_touch_stmpe811(&touch_xyz0));           //��¼��������һ��У��ֵ

        GK_ApiFillWin(desktop,CN_COLOR_WHITE,0);
        GK_ApiDrawText(desktop,NULL,NULL,limit_left+10,limit_top+50,
                            "����������",21,CN_COLOR_WHITE,CN_R2_COPYPEN,0);
        GK_ApiDrawText(desktop,NULL,NULL,limit_left+10,limit_top+70,
                            "�ٴ�׼ȷ���ʮ�ֽ����",21,CN_COLOR_WHITE,CN_R2_COPYPEN,0);
        GK_ApiLineto(desktop,limit_right-40,limit_bottom-20,
                      limit_right,limit_bottom-20,
                      CN_COLOR_RED,CN_R2_COPYPEN,0);
        GK_ApiLineto(desktop,limit_right-20,limit_bottom-40,
                      limit_right-20,limit_bottom,
                      CN_COLOR_RED,CN_R2_COPYPEN,0);
        GK_ApiSyncShow(CN_TIMEOUT_FOREVER);

        if (TS_Read(FIFO_SIZE, 1))
        {
            TS_Write(FIFO_STA, 1, 0x01);                //FIFO��λ
            TS_Write(FIFO_STA, 1, 0x00);
        }

        while(!read_touch_stmpe811(&touch_xyz1));           //��¼�������ڶ���У��ֵ

        tg_touch_adjust.ratio_x = ((touch_xyz1.x - touch_xyz0.x)<<16)
                        /(limit_right - limit_left -40);
        tg_touch_adjust.offset_x = (touch_xyz0.x<<16) - 20*tg_touch_adjust.ratio_x;
        tg_touch_adjust.ratio_y = ((touch_xyz1.y - touch_xyz0.y)<<16)
                        /(limit_bottom- limit_top-40);
        tg_touch_adjust.offset_y= (touch_xyz0.y<<16) - 20*tg_touch_adjust.ratio_y;
        GK_ApiFillWin(desktop,CN_COLOR_WHITE,0);
    //    GK_DestroyWin(desktop);
        touch_init = fopen("sys:\\touch_init.dat","w+");
        fwrite(&tg_touch_adjust,sizeof(struct tagST_TouchAdjust),1,touch_init);
    }
    fclose(touch_init);

}

