//----------------------------------------------------
//Copyright (C), 2005-2009,  lst.
//��Ȩ���� (C), 2005-2009,   lst.
//����ģ��:IO��ʼ��
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����:IO��ʼ��
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-03-10
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __gpio_api_h_
#define __gpio_api_h_

#ifdef __cplusplus
extern "C" {
#endif

#define cn_invalid_port     (-1)        //��gpio����
void gpio_cfg_pin_func(u32 port,u32 pinnum,u32 func_no);
void gpio_cfg_pin_mode(u32 port,u32 pinnum,u32 mode);
void gpio_setto_in(u32 port,u32 msk);
void gpio_setto_out(u32 port,u32 msk);
u32 gpio_get_data(u32 port);
void gpio_out_data(u32 port,u32 data);
void gpio_setto_high(u32 port,u32 msk);
void gpio_setto_low(u32 port,u32 msk);
void gpio_power_on(u32 port);
void gpio_power_off(u32 port);

#ifdef __cplusplus
}
#endif

#endif /*__gpio_api_h_*/
