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
// �����Ȩ�����ʹ���߷������¶������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�����������У���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�����������У��Լ�����
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
//����ģ��:s29glxxx����
//���ߣ�hm
//�汾��V1.0.1
//�ļ�����:���������ص�����
//����˵��:
//�޶���ʷ:
//1. ����: 2014-04-25
//   ����: hm
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------

#ifndef _BOARD_CONFIG_H
#define _BOARD_CONFIG_H


//CPU�ܹ��������,��ѡ������������,�����С��,�ǿ�ѡ��,����������,
//��CPU�ֳ��̶�,���ֳ���BSP��arch.h�ļ��ж���
//�洢����С�˵�����
#define CN_CFG_LITTLE_ENDIAN        0
#define CN_CFG_BIG_ENDIAN           1
#define CN_CFG_BYTE_ORDER           CN_CFG_BIG_ENDIAN

/*____��Ӳ����ص�����____*/
#define CN_CFG_TICK_US 1000  //����ϵͳ�ں�ʱ�����峤�ȣ���usΪ��λ��*
#define CN_CFG_TICK_HZ 1000  //�ں�ʱ��Ƶ�ʣ���λΪhz��
#define Mhz 1000000
#define CN_CFG_MCLK      (800*Mhz)  //��Ƶ
#define cfg_core_ccb_clk (400*Mhz)
#define cfg_core_tb_clk  (cfg_core_ccb_clk/8)  //tbclk
#define cfg_core_tb_clk_time   20              //ns
#define cfg_rtc_clk 800*Mhz

//�����ļ�ϵͳ��ʼ�ͽ������
#define CN_CFG_EASYNOR_START_SECTOR       0   //��������ļ�ϵͳ��ʼ��
#define CN_CFG_EASYNOR_LENGTH_SECTOR      50  //�����ļ�ϵͳռ��sector����
//flashоƬ�궨��
#define cn_norflash_bus_width		   16 	//����flash���߿��
#define cn_norflash_buf_size           64	//����norflash��������С
//RTC_DS1390
#define CFG_RTC_SPI_BUS			g_ptSpi_Reg //RTCʹ�õ�SPI
#define CFG_RTC_SPI_CS			0			//CS0
#define CFG_RTC_SPI_SPEED		600000      //RTC���ݵ�SPI�ٶ� 600kHz

/*____��������____*/
#define CN_CFG_UART0_USED 1
#define CN_CFG_UART1_USED 0


#include "SysSpace.h"

#endif









