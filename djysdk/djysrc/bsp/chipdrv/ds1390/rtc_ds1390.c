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
//����ģ�飺RTCʱ��ģ��
//���ߣ�hm
//�汾��V0.1.0
//�ļ�������Ӳ��RTC���Ӽ�RTC�ײ�����
//����˵����
//�޶���ʷ��
//1. ����: 2014-04-12
//   ���ߣ�
//   �°汾�ţ�V0.1.0
//   �޸�˵����ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "cpu_peri.h"
#include "stdio.h"
#include "rtc_ds1390.h"
#include "board_config.h"

#define HexToBcd(x) ((((x) / 10) <<4) + ((x) % 10))            //��16����ת����BCD��
#define BcdToHex(x) ((((x) & 0xF0) >>4) * 10 + ((x) & 0x0F))   //��BCD��ת����16����

#define RTC_CMD_SECONDS         (0x01)
#define RTC_CMD_MINUTES         (0x02)
#define RTC_CMD_HOURS           (0x03)
#define RTC_CMD_DAY_OF_WEEK     (0x04)
#define RTC_CMD_DATE_OF_MONTH   (0x05)
#define RTC_CMD_MONTH           (0x06)
#define RTC_CMD_YEAR            (0x07)

tagSpiConfig *rtc_spi_Config=NULL;

//-----��RTCʱ��---------------------------------------------------------------
//���ܣ���RTCʱ��ģ�����Ӧ�Ĵ���������롢�֡�ʱ���ꡢ�¡��յ�
//������reg,��Ӧ�ļĴ����ţ�����ds1390pdf�ĵ���֪����������д�������0x80������
//          ���磬��������Ϊ0x01ʱ��д������Ϊ0x81,�˲���ֻ�贫��0x01����
//���أ���ȡ����ֵ��ΪBCD��
//-----------------------------------------------------------------------------
static unsigned char __rtc_read (unsigned char reg)
{
    unsigned char ret;

    Spi_ActiveCS(CFG_RTC_SPI_BUS,CFG_RTC_SPI_CS);
    Spi_TxRx(CFG_RTC_SPI_BUS,&reg,1,&ret,1,1);
    Spi_InActiveCS(CFG_RTC_SPI_BUS,CFG_RTC_SPI_CS);
    return (int)ret< 0 ? 0 : ret;
}

//-----дRTCʱ��---------------------------------------------------------------
//���ܣ�дRTCʱ��ģ�����Ӧ�Ĵ���
//������reg,��Ӧ�ļĴ����ţ�����ds1390pdf�ĵ���֪����������д�������0x80������
//          ���磬��������Ϊ0x01ʱ��д������Ϊ0x81,�˲���ֻ�贫��0x01����
//      val,д���Ĵ����е���ֵ��д�������ΪBCD��
//���أ���
//-----------------------------------------------------------------------------
static void __rtc_write (unsigned char reg, unsigned char val)
{
    unsigned char dout[2];  /* SPI Output Data Bytes */

    dout[0] = 0x80 | reg;
    dout[1] = val;

    Spi_ActiveCS(CFG_RTC_SPI_BUS,CFG_RTC_SPI_CS);
    Spi_TxRx(CFG_RTC_SPI_BUS,dout,2,NULL,0,0);
    Spi_InActiveCS(CFG_RTC_SPI_BUS,CFG_RTC_SPI_CS);
}

//-----����RTCʱ��---------------------------------------------------------------
//���ܣ���������ʱ����д�뵽RTCʱ�ӣ�����RTC������RTCоƬ��ŵ�ʱ����Ϣ�ǻ���BCD
//      �����ʽ��������д��ʱ��ǰ����Ҫ��ʱ����Ϣת��Ϊ��Ӧ��BCD��ʽ
//������DateTime,rtc_tm�ṹ���͵ı������������������RTC��ʱ����Ϣ
//���أ���
//-----------------------------------------------------------------------------
uint32_t rtc_time_get(struct rtc_tm *DateTime)
{
    uint32_t sec, min, hour, mday, wday, mon, year;

    //�ж��Ƿ���Ҫ��ʼ��SPI���Է�module_init_rtcδ������
    if(!rtc_spi_Config)
    {
        rtc_spi_Config = &pg_spi_Config;
        rtc_spi_Config->freq=CFG_RTC_SPI_SPEED;
        Spi_Init(CFG_RTC_SPI_BUS,rtc_spi_Config);
    }

    //��RTC��ʱ��
    sec  = __rtc_read (RTC_CMD_SECONDS);
    min  = __rtc_read (RTC_CMD_MINUTES);
    hour = __rtc_read (RTC_CMD_HOURS);
    mday = __rtc_read (RTC_CMD_DATE_OF_MONTH);
    wday = __rtc_read (RTC_CMD_DAY_OF_WEEK);
    mon  = __rtc_read (RTC_CMD_MONTH);
    year = __rtc_read (RTC_CMD_YEAR);

    //��BCD��ʽת��Ϊ����ģʽ
    DateTime->tm_sec    = BcdToHex(sec & 0x7F);
    DateTime->tm_min    = BcdToHex(min & 0x7F);
    DateTime->tm_hour   = BcdToHex(hour);
    DateTime->tm_mday   = BcdToHex(mday & 0x3F);
    DateTime->tm_wday   = BcdToHex(wday & 0x07) - 1;
    DateTime->tm_mon    = BcdToHex(mon & 0x1F);
    DateTime->tm_year   = BcdToHex(year) + 2000;

/*---------------------test use only----------------------*/
    printf("Get RTC year: %04d mon: %02d mday: %02d wday: %02d "
           "hr: %02d min: %02d sec: %02d\r\n",
           DateTime->tm_year, DateTime->tm_mon, DateTime->tm_mday,
           DateTime->tm_wday, DateTime->tm_hour, DateTime->tm_min,
           DateTime->tm_sec);
/*---------------------test use only----------------------*/

    return 0;
}
//-----����RTCʱ��---------------------------------------------------------------
//���ܣ���������ʱ����д�뵽RTCʱ�ӣ�����RTC������RTCоƬ��ŵ�ʱ����Ϣ�ǻ���BCD
//      �����ʽ��������д��ʱ��ǰ����Ҫ��ʱ����Ϣת��Ϊ��Ӧ��BCD��ʽ
//������DateTime,rtc_tm�ṹ���͵ı������������������RTC��ʱ����Ϣ
//���أ���
//-----------------------------------------------------------------------------
bool_t rtc_update_time(struct rtc_tm *DateTime)
{
    //�ж��Ƿ���Ҫ��ʼ��SPI���Է�module_init_rtcδ������
    if(!rtc_spi_Config)
    {
        rtc_spi_Config = &pg_spi_Config;
        rtc_spi_Config->freq=CFG_RTC_SPI_SPEED;
        Spi_Init(CFG_RTC_SPI_BUS,rtc_spi_Config);
    }

    __rtc_write (RTC_CMD_SECONDS,     HexToBcd (DateTime->tm_sec));
    __rtc_write (RTC_CMD_MINUTES,     HexToBcd (DateTime->tm_min));
    __rtc_write (RTC_CMD_HOURS,       HexToBcd (DateTime->tm_hour));
    __rtc_write (RTC_CMD_DAY_OF_WEEK, HexToBcd (DateTime->tm_wday + 1));//���ڼ�
    __rtc_write (RTC_CMD_DATE_OF_MONTH, HexToBcd (DateTime->tm_mday));
    __rtc_write (RTC_CMD_MONTH,       HexToBcd (DateTime->tm_mon));
    __rtc_write (RTC_CMD_YEAR,        HexToBcd (DateTime->tm_year- 2000));

    return 1;
}

//----��ʼ��rtcʵʱʱ��ģ��------------------------------------------------------
//���ܣ���ʼ��RTCģ�飬��Ҫ�����ⲿ�ж�1�ĳ�ʼ��
//������ģ���ʼ������û�в���
//���أ�true = �ɹ���ʼ����false = ��ʼ��ʧ��
//-----------------------------------------------------------------------------
u32 rtc_init(void)
{
    if(!rtc_spi_Config)
    {
        rtc_spi_Config = &pg_spi_Config;
        rtc_spi_Config->freq=CFG_RTC_SPI_SPEED;
        Spi_Init(CFG_RTC_SPI_BUS,rtc_spi_Config);
    }
/*---------------------test use only----------------------*/
    struct rtc_tm time,gtime;
    time.tm_sec    = 00;
    time.tm_min    = 12;
    time.tm_hour   = 16;
    time.tm_mday   = 15;
    time.tm_wday   = 2;
    time.tm_mon    = 4;
    time.tm_year   = 2014;
    //rtc_update_time(&time);
    rtc_time_get(&gtime);
/*---------------------test use only----------------------*/
    return 1;
}

#ifdef  CN_CFG_DJYOS_USED
ptu32_t ModuleInstall_RTC(ptu32_t para)
{
    Tm_ConnectRtc(rtc_update_time,NULL);
    rtc_init();
    return 1;
}
#endif


