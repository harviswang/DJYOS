//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//��Ȩ���� (C), 2004-2009,   lst.
//����ģ��: ϵͳ��ʼ��
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����: ʱ�Ӻ͵�Դ���ƼĴ����ṹ����
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-08-21
//   ����: lst
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __RCC_H__
#define __RCC_H__

#ifdef __cplusplus
extern "C" {
#endif

//------------------------ Reset and Clock Control ---------------------------
//����ʱ�����ó���
#define cn_pll          (cn_mclk/cn_extclk-2)   //pll����
#define cn_AHB_scaler   0           //����Ƶ
#define cn_pclk2_scaler 0           //����Ƶ
#define cn_pclk1_scaler 4           //����Ƶ

//����RCC_TypeDef.CRλ���룬ֻ��1λ����λ����ַ������bitmask---------------
#define bb_rcc_cr_base   (0x42000000 + 0x21000*32)  //λ����ַ
//��ʼ�����ֵ��������(0x030b0000)��ʾʱ��δ��ʼ��
#define cn_cr_check ((1<<25)+(1<<24)+(1<<19)+(0<<18)+(1<<17)+(1<<16)+(0<<0))
//��ʼ�����ֵ������ 0x030f0001
#define cn_cr_check_mask ((1<<25)+(1<<24)+(1<<19)+(1<<18)+(1<<17)+(1<<16)+(1<<0))
//��ʼ������ֵ
#define cn_cr_set cn_cr_check

//rw,hsion,�ڲ�����ʱ��ʹ��,1=ʹ��
#define bb_rcc_cr_hsion    (*(vu32 *)(bb_rcc_cr_base+0*4))
//r,hsirdy,�ڲ�����ʱ�Ӿ�����־��1=����
#define bb_rcc_cr_hsirdy   (*(vu32 *)(bb_rcc_cr_base+1*4))
#define bm_rcc_hsitrim     (0x1f<<3)  //rw,�ڲ�����ʱ�ӵ���
#define bm_rcc_hsical      (0xff<<8)  //r,�ڲ�����ʱ��У׼�Ĵ���
//rw,�ⲿ����ʱ��ʹ�ܣ����������ֹͣģʽʱ����λ��Ӳ�����㡣 0=HSE�����رգ�1=ʹ��
#define bb_rcc_cr_hseon    (*(vu32 *)(bb_rcc_cr_base+16*4))
//r,�ⲿ1-25MHzʱ�Ӿ�����־,0=��������1=����
#define bb_rcc_cr_hserdy   (*(vu32 *)(bb_rcc_cr_base+17*4))
//rw,�ⲿ����ʱ����·���ڵ���ģʽ���������1����������·�ⲿ����������ֻ�����ⲿ
//1-25MHz�����رյ�����£���λ�ſ���д�롣0=���룬1=��·
#define bb_rcc_cr_hsebyp   (*(vu32 *)(bb_rcc_cr_base+18*4))
//rw,ʱ�Ӱ�ȫϵͳʹ�ܣ�0=ʱ�Ӽ�����رգ�1=����ⲿ1-25MHzʱ�Ӿ�����ʱ�Ӽ����������
#define bb_rcc_cr_csson    (*(vu32 *)(bb_rcc_cr_base+19*4))
//rw,PLLʹ���������1�����㡣�����������ֹͣģʽʱ����λ��Ӳ�����㡣 0=�رգ�1=ʹ��
#define bb_rcc_cr_pllon    (*(vu32 *)(bb_rcc_cr_base+24*4))
//r,0=PLLδ������1=PLL����
#define bb_rcc_cr_pllrdy   (*(vu32 *)(bb_rcc_cr_base+25*4))

//����RCC_TypeDef.CFGRλ���룬ֻ��1λ����λ����ַ������bitmask------------
#define bb_rcc_cfgr_base  (0x42000000 + 0x21004*32)  //λ����ַ
//��ʼ�����ֵ�������ڴ�ֵ��ʾʱ��δ��ʼ��
#define cn_cfgr_check   ((2<<2)+(cn_AHB_scaler<<4)+(cn_pclk1_scaler<<8) \
                        +(cn_pclk2_scaler<<11)+(1<<16)+(1<<17)+(cn_pll<<18))
//��ʼ�����ֵ������ 0x003f3ff3
#define cn_cfgr_check_mask    ((3<<2)+(0xf<<4)+(0x7<<8)+(0x7<<11)+(1<<16) \
                        +(1<<17)+(0xf<<18))
//��ʼ������ֵ
#define cn_cfgr_set ((2<<0)+(cn_AHB_scaler<<4)+(cn_pclk1_scaler<<8) \
                    +(cn_pclk2_scaler<<11)+(3<<14)+(1<<16)+(0<<17)+(cn_pll<<18))
#define bm_rcc_mco      (7<<24)  //rw,΢������ʱ�������
                                 //0xx=��ʱ�������
                                 //100=ϵͳʱ�������
                                 //101=�ڲ�8MHz��RC����ʱ�������
                                 //110=�ⲿ1-25MHz����ʱ�����
                                 //111: PLLʱ��2��Ƶ�������
                                 //ע�⣺- ��ʱ��������������л�MCOʱ��Դʱ
                                 //���ܻᱻ�ضϡ�- ��ϵͳʱ����Ϊ���ʱ��ʱ��
                                 //�뱣֤���ʱ��Ƶ�ʲ�����50MHz (IO�����Ƶ��)
//rw,USBԤ��Ƶ,���������������48MHz��USBʱ�ӡ���RCC_APB1ENR�Ĵ�����ʹ��USBʱ��֮ǰ�����뱣֤��λ�Ѿ���Ч��
//���USBʱ�ӱ�ʹ�ܣ���λ���Ա����㡣 0 = PLLʱ��1.5����Ƶ��ΪUSBʱ�� 1 = PLLʱ��ֱ����ΪUSBʱ�� .
#define bb_rcc_cfgr_usbpre  (*(vu32 *)(bb_rcc_cfgr_base+22*4))

#define bm_rcc_cfgr_pllmul       (0xf<<18)//rw,PLL��Ƶ����0~14��Ӧ2~16����15=16��
//rw,�������������ƵHSE����ΪPLL����ʱ�ӡ���λֻ��
//   ��PLL�ر�ʱ�ſ��Ա�д�롣0 = ����Ƶ��1 = 2��Ƶ
#define bb_rcc_cfgr_pllxtpre  (*(vu32 *)(bb_rcc_cfgr_base+17*4))
//rw,PLL����ʱ��Դ,�����������ѡ��PLL����ʱ��Դ����λֻ����PLL�ر�ʱ�ſ��Ա�д��.
//   0 = HSIʱ��2��Ƶ����ΪPLL����ʱ��
//   1 = HSEʱ����ΪPLL����ʱ�ӡ�
#define bb_rcc_cfgr_pllsrc    (*(vu32 *)(bb_rcc_cfgr_base+16*4))
//rw,ADCPRE,b15~14,ADCԤ��Ƶ,����PLCLK2��Ƶ����ΪADCʱ��Ƶ�ʵ�
//   ��Ƶ����00~11��Ӧ2/4/6/8��Ƶ
#define bm_rcc_cfgr_adcpre     (3<<14)
//rw,PPRE2,b13~11,������HCLK����APB2ʱ�ӵ�Ԥ��Ƶ����0xx=����Ƶ��
//   100~111�ֱ��Ӧ2/4/8/16��Ƶ
#define bm_rcc_cfgr_ppre2    (7<<11)
//rw,PPRE1,b10~8,������HCLK����APB1ʱ�ӵ�Ԥ��Ƶ�������뱣֤APB1
//   ʱ��Ƶ�ʲ�����36MHz��0xx=����Ƶ��100~111��Ӧ2/4/8/16��Ƶ
#define bm_rcc_cfgr_ppre1    (7<<8)
//rw,HPRE,b7~4��������SYSCLK����AHBʱ�ӵ�Ԥ��Ƶ����
//   0xxx=����Ƶ��1000~1111��ʾ2/4/8/16/64/128/256/512
#define bm_rcc_cfgr_hpre     (0xf<<4)
//r,SWS,b3~2����ȡӲ�����õ�ϵͳʱ�ӣ�SYSCLK��ѡ��״̬��
//   00=HSI��01=HSE��10=pll�����11=������
#define bm_rcc_cfgr_sws     (3<<2)
//rw,SW,b1~0,����ϵͳʱ�ӣ�SYSCLK��Դ��ע���ڴ�ֹͣ�����ģʽ
//   �з���ʱ��ֱ�ӻ�����Ϊϵͳʱ�ӵ�HSE���ֹ���ʱ������Ӳ
//   ��ǿ��ѡ��HSI��Ϊϵͳʱ�ӣ����ʱ�Ӱ�ȫϵͳ�Ѿ�������
//   00=HSI��01=HSE��10=pll�����11=������
#define bm_rcc_cfgr_sw     (3<<0)

//����RCC_TypeDef.CIRλ���룬ֻ��1λ����λ����ַ������bitmask------------
#define bb_rcc_cir_base     (0x42000000 + 0x21008*32)         //λ����ַ
#define bb_rcc_cir_cssc     (*(vu32 *)(bb_rcc_cir_base+23*4)) //w,д1�����CSSF��ȫϵͳ�жϱ�־λCSSF��
#define bb_rcc_cir_pllrdyc  (*(vu32 *)(bb_rcc_cir_base+20*4)) //w,д1�����PLL�����жϱ�־λPLLRDYF��
#define bb_rcc_cir_hserdyc  (*(vu32 *)(bb_rcc_cir_base+19*4)) //w,д1������HSE�����жϱ�־λHSERDYF��
#define bb_rcc_cir_hsirdyc  (*(vu32 *)(bb_rcc_cir_base+18*4)) //w,д1������HSI�����жϱ�־λHSIRDYF��
#define bb_rcc_cir_lserdyc  (*(vu32 *)(bb_rcc_cir_base+17*4)) //w,д1������LSE�����жϱ�־λLSERDYF��
#define bb_rcc_cir_lsirdyc  (*(vu32 *)(bb_rcc_cir_base+16*4)) //w,д1������LSI�����жϱ�־λLSIRDYF��
#define bb_rcc_cir_pllrdyie (*(vu32 *)(bb_rcc_cir_base+12*4)) //rw,PLL�����ж�ʹ�ܣ�0 = ��ֹ��1 = ʹ��
#define bb_rcc_cir_hsercyie (*(vu32 *)(bb_rcc_cir_base+11*4)) //rw,HSE�����ж�ʹ�ܣ�0 = ��ֹ��1 = ʹ��
#define bb_rcc_cir_hsirdyie (*(vu32 *)(bb_rcc_cir_base+10*4)) //rw,HSI�����ж�ʹ�ܣ�0 = ��ֹ��1 = ʹ��
#define bb_rcc_cir_lserdyie (*(vu32 *)(bb_rcc_cir_base+9*4))  //rw,LSE�����ж�ʹ�ܣ�0 = ��ֹ��1 = ʹ��
#define bb_rcc_cir_lsirdyie (*(vu32 *)(bb_rcc_cir_base+8*4))  //rw,LSI�����ж�ʹ�ܣ�0 = ��ֹ��1 = ʹ��
#define bb_rcc_cir_cssf     (*(vu32 *)(bb_rcc_cir_base+7*4))  //r,ʱ�Ӱ�ȫϵͳ�жϱ�־,����1-25M��������ʱ��Ӳ����λ��
                                                              //    ���д1��CSSCλ���㣬0 = ���жϣ�1 = ���жϣ�
#define bb_rcc_cir_pllrdyf  (*(vu32 *)(bb_rcc_cir_base+4*4))  //r,PLL�����жϱ�־,PLL������PLLRDYIEλ����1ʱ��Ӳ��
                                                              //    ��λ�����д1�� PLLRDYCλ���㣬0 = ���жϣ�1 = ���жϣ�
#define bb_rcc_cir_hserdyf  (*(vu32 *)(bb_rcc_cir_base+3*4))  //r,HSE�����жϱ�־ ,�ⲿ����ʱ�Ӿ�����HSERDYIEλ����1ʱ��
                                                              //    Ӳ����λ,���д1��HSERDYCλ���㣬0 = ���жϣ�1 = ���ж�
#define bb_rcc_cir_hsirdyf  (*(vu32 *)(bb_rcc_cir_base+2*4))  //r,HSI�����жϱ�־ ,�ⲿ����ʱ�Ӿ�����HSIRDYIEλ����1ʱ��
                                                              //    Ӳ����λ,���д1��HSIRDYCλ���㣬0 = ���жϣ�1 = ���ж�
#define bb_rcc_cir_lserdyf  (*(vu32 *)(bb_rcc_cir_base+1*4))  //r,LSE�����жϱ�־ ,�ⲿ����ʱ�Ӿ�����LSERDYIEλ����1ʱ��
                                                              //    Ӳ����λ,���д1��LSERDYCλ���㣬0 = ���жϣ�1 = ���ж�
#define bb_rcc_cir_lsirdyf  (*(vu32 *)(bb_rcc_cir_base+0*4))  //r,LSI�����жϱ�־ ,�ⲿ����ʱ�Ӿ�����LSIRDYIEλ����1ʱ��
                                                              //    Ӳ����λ,���д1��LSIRDYCλ���㣬0 = ���жϣ�1 = ���ж�
//����RCC_TypeDef.APB2RSTRλ���룬ֻ��1λ����λ����ַ������bitmask------------
#define bb_rcc_apb2rstr_base        (0x42000000 + 0x2100c*32)  //λ����ַ
#define bb_rcc_apb2rstr_uart1rst    (*(vu32*)(bb_rcc_apb2rstr_base+14*4)) //rw,д0��Ч��д1=��λUSART1
#define bb_rcc_apb2rstr_spi1rst     (*(vu32*)(bb_rcc_apb2rstr_base+12*4)) //rw,д0��Ч��д1=��λSPI1
#define bb_rcc_apb2rstr_tim1rst     (*(vu32*)(bb_rcc_apb2rstr_base+11*4)) //rw,д0��Ч��д1=��λTIM1
#define bb_rcc_apb2rstr_adc2rst     (*(vu32*)(bb_rcc_apb2rstr_base+10*4)) //rw,д0��Ч��д1=��λADC2
#define bb_rcc_apb2rstr_adc1rst     (*(vu32*)(bb_rcc_apb2rstr_base+9*4))  //rw,д0��Ч��д1=��λADC1
#define bb_rcc_apb2rstr_ioperst     (*(vu32*)(bb_rcc_apb2rstr_base+6*4))  //rw,д0��Ч��д1=��λIO��E
#define bb_rcc_apb2rstr_iopdrst     (*(vu32*)(bb_rcc_apb2rstr_base+5*4))  //rw,д0��Ч��д1=��λIO��D
#define bb_rcc_apb2rstr_iopcrst     (*(vu32*)(bb_rcc_apb2rstr_base+4*4))  //rw,д0��Ч��д1=��λIO��C
#define bb_rcc_apb2rstr_iopbrst     (*(vu32*)(bb_rcc_apb2rstr_base+3*4))  //rw,д0��Ч��д1=��λIO��B
#define bb_rcc_apb2rstr_ioparst     (*(vu32*)(bb_rcc_apb2rstr_base+2*4))  //rw,д0��Ч��д1=��λIO��A
#define bb_rcc_apb2rstr_afiorst     (*(vu32*)(bb_rcc_apb2rstr_base+0*4))  //rw,д0��Ч��д1=��λ����IO����
//����RCC_TypeDef.APB1RSTRλ���룬ֻ��1λ����λ����ַ������bitmask------------
#define bb_rcc_apb1rstr_base        (0x42000000 + 0x21010*32)  //λ����ַ
#define bb_rcc_apb1rstr_pwrrst      (*(vu32*)(bb_rcc_apb1rstr_base+28*4)) //rw,д0��Ч��д1=��λ��Դ��·
#define bb_rcc_apb1rstr_bkprst      (*(vu32*)(bb_rcc_apb1rstr_base+27*4)) //rw,д0��Ч��д1=��λ���ݵ�·
#define bb_rcc_apb1rstr_canrst      (*(vu32*)(bb_rcc_apb1rstr_base+25*4)) //rw,д0��Ч��д1=��λCAN
#define bb_rcc_apb1rstr_usbrst      (*(vu32*)(bb_rcc_apb1rstr_base+23*4)) //rw,д0��Ч��д1=��λUSB
#define bb_rcc_apb1rstr_i2c2rst     (*(vu32*)(bb_rcc_apb1rstr_base+22*4)) //rw,д0��Ч��д1=��λI2C2
#define bb_rcc_apb1rstr_i2c1rst     (*(vu32*)(bb_rcc_apb1rstr_base+21*4)) //rw,д0��Ч��д1=��λI2C1
#define bb_rcc_apb1rstr_uart3rst    (*(vu32*)(bb_rcc_apb1rstr_base+18*4)) //rw,д0��Ч��д1=��λUSART3
#define bb_rcc_apb1rstr_uart2rst    (*(vu32*)(bb_rcc_apb1rstr_base+17*4)) //rw,д0��Ч��д1=��λUSART2
#define bb_rcc_apb1rstr_spi2rst     (*(vu32*)(bb_rcc_apb1rstr_base+14*4)) //rw,д0��Ч��д1=��λSPI2
#define bb_rcc_apb1rstr_wwdgrst     (*(vu32*)(bb_rcc_apb1rstr_base+11*4)) //rw,д0��Ч��д1=��λ���ڿ��Ź�
#define bb_rcc_apb1rstr_tim4rst     (*(vu32*)(bb_rcc_apb1rstr_base+2*4))  //rw,д0��Ч��д1=��λtimer4
#define bb_rcc_apb1rstr_tim3rst     (*(vu32*)(bb_rcc_apb1rstr_base+1*4))  //rw,д0��Ч��д1=��λtimer3
#define bb_rcc_apb1rstr_tim2rst     (*(vu32*)(bb_rcc_apb1rstr_base+0*4))  //rw,д0��Ч��д1=��λtimer2
//����RCC_TypeDef.AHBENRλ���룬ֻ��1λ����λ����ַ������bitmask------------
#define bb_rcc_ahbenr_base     (0x42000000 + 0x21014*32)  //λ����ַ
#define bb_rcc_ahbenr_flitfen   (*(vu32*)(bb_rcc_ahbenr_base+4*4))//rw,b4,����ӿڵ�·ʱ��ʹ��,
                                                                //0��˯��ģʽʱ����ӿڵ�·ʱ�ӹر�
                                                                //1��˯��ģʽʱ����ӿڵ�·ʱ�ӿ���
#define bb_rcc_ahbenr_sramen    (*(vu32*)(bb_rcc_ahbenr_base+2*4))//rw,b2,SRAMʱ��ʹ��,
                                                                //0��˯��ģʽʱSRAMʱ�ӹر�
                                                                //1��˯��ģʽʱSRAMʱ�ӿ���
#define bb_rcc_ahbenr_dma1en    (*(vu32*)(bb_rcc_ahbenr_base+0*4))//rw,b0, 0��DMA1ʱ�ӹرգ�1��DMAʱ�ӿ���
#define bb_rcc_ahbenr_dma2en    (*(vu32*)(bb_rcc_ahbenr_base+1*4))//rw,b0, 0��DMA1ʱ�ӹرգ�1��DMAʱ�ӿ���
//����RCC_TypeDef.APB2ENRλ���룬ֻ��1λ����λ����ַ������bitmask------------
#define bb_rcc_apb2enr_base     (0x42000000 + 0x21018*32)  //λ����ַ
#define bb_rcc_apb2enr_uart1en  (*(vu32*)(bb_rcc_apb2enr_base+14*4))  //rw,0=USART1ʱ�ӹر�,1=USART1ʱ�ӿ���
#define bb_rcc_apb2enr_spi1en   (*(vu32*)(bb_rcc_apb2enr_base+12*4))  //rw,0=SPI1ʱ�ӹر�,  1=SPI1ʱ�ӿ���
#define bb_rcc_apb2enr_tim1en   (*(vu32*)(bb_rcc_apb2enr_base+11*4))  //rw,0=TIM1ʱ�ӹر�,  1=TIM1ʱ�ӿ���
#define bb_rcc_apb2enr_adc2en   (*(vu32*)(bb_rcc_apb2enr_base+10*4))  //rw,0=ADC2ʱ�ӹر�,  1=ADC2ʱ�ӿ���
#define bb_rcc_apb2enr_adc1en   (*(vu32*)(bb_rcc_apb2enr_base+9*4))   //rw,0=ADC1ʱ�ӹر�,  1=ADC1ʱ�ӿ���
#define bb_rcc_apb2enr_iopgen   (*(vu32*)(bb_rcc_apb2enr_base+8*4))   //rw,0=IO��Eʱ�ӹر�, 1=IO��Eʱ�ӿ���
#define bb_rcc_apb2enr_iopfen   (*(vu32*)(bb_rcc_apb2enr_base+7*4))   //rw,0=IO��Eʱ�ӹر�, 1=IO��Eʱ�ӿ���
#define bb_rcc_apb2enr_iopeen   (*(vu32*)(bb_rcc_apb2enr_base+6*4))   //rw,0=IO��Eʱ�ӹر�, 1=IO��Eʱ�ӿ���
#define bb_rcc_apb2enr_iopden   (*(vu32*)(bb_rcc_apb2enr_base+5*4))   //rw,0=IO��Dʱ�ӹر�, 1=IO��Dʱ�ӿ���
#define bb_rcc_apb2enr_iopcen   (*(vu32*)(bb_rcc_apb2enr_base+4*4))   //rw,0=IO��Cʱ�ӹر�, 1=IO��Cʱ�ӿ���
#define bb_rcc_apb2enr_iopben   (*(vu32*)(bb_rcc_apb2enr_base+3*4))   //rw,0=IO��Bʱ�ӹر�, 1=IO��Bʱ�ӿ���
#define bb_rcc_apb2enr_iopaen   (*(vu32*)(bb_rcc_apb2enr_base+2*4))   //rw,0=IO��Aʱ�ӹر�, 1=IO��Aʱ�ӿ���
#define bb_rcc_apb2enr_afioen   (*(vu32*)(bb_rcc_apb2enr_base+0*4))   //rw,0=���ù���IOʱ�ӹر�, 1=���ù���IOʱ�ӿ���
//����RCC_TypeDef.APB1ENRλ���룬ֻ��1λ����λ����ַ������bitmask------------
#define bb_rcc_apb1enr_base     (0x42000000 + 0x2101c*32)  //λ����ַ
#define bb_rcc_apb1enr_pwren    (*(vu32*)(bb_rcc_apb1enr_base+28*4))  //rw,0=�رգ�1=������Դʱ��
#define bb_rcc_apb1enr_bkpen    (*(vu32*)(bb_rcc_apb1enr_base+27*4))  //rw,0=�رգ�1=��������ʱ��
#define bb_rcc_apb1enr_canen    (*(vu32*)(bb_rcc_apb1enr_base+25*4))  //rw,0=�رգ�1=����CANʱ��
#define bb_rcc_apb1enr_usben    (*(vu32*)(bb_rcc_apb1enr_base+23*4))  //rw,0=�رգ�1=����USBʱ��
#define bb_rcc_apb1enr_i2c2en   (*(vu32*)(bb_rcc_apb1enr_base+22*4))  //rw,0=�رգ�1=����I2C2ʱ��
#define bb_rcc_apb1enr_i2c1en   (*(vu32*)(bb_rcc_apb1enr_base+21*4))  //rw,0=�رգ�1=����I2C1ʱ��
#define bb_rcc_apb1enr_uart5en  (*(vu32*)(bb_rcc_apb1enr_base+20*4))  //rw,0=�رգ�1=����USART5ʱ��
#define bb_rcc_apb1enr_uart4en  (*(vu32*)(bb_rcc_apb1enr_base+19*4))  //rw,0=�رգ�1=����USART4ʱ��
#define bb_rcc_apb1enr_uart3en  (*(vu32*)(bb_rcc_apb1enr_base+18*4))  //rw,0=�رգ�1=����USART3ʱ��
#define bb_rcc_apb1enr_uart2en  (*(vu32*)(bb_rcc_apb1enr_base+17*4))  //rw,0=�رգ�1=����USART2ʱ��
#define bb_rcc_apb1enr_spi2en   (*(vu32*)(bb_rcc_apb1enr_base+14*4))  //rw,0=�رգ�1=����SPI2ʱ��
#define bb_rcc_apb1enr_wwdgen   (*(vu32*)(bb_rcc_apb1enr_base+11*4))  //rw,0=�رգ�1=�������ڿ��Ź�ʱ��
#define bb_rcc_apb1enr_tim4en   (*(vu32*)(bb_rcc_apb1enr_base+2*4))   //rw,0=�رգ�1=����timer4ʱ��
#define bb_rcc_apb1enr_tim3en   (*(vu32*)(bb_rcc_apb1enr_base+1*4))   //rw,0=�رգ�1=����timer3ʱ��
#define bb_rcc_apb1enr_tim2en   (*(vu32*)(bb_rcc_apb1enr_base+0*4))   //rw,0=�رգ�1=����timer2ʱ��
//����RCC_TypeDef.BDCRλ���룬ֻ��1λ����λ����ַ������bitmask------------
#define bb_rcc_bdcr_base     (0x42000000 + 0x21020*32)  //λ����ַ
#define bb_rcc_bdcr_bdrst   (*(vu32*)(bb_rcc_bdcr_base+16*4)) //rw,д1����λ����������
#define bb_rcc_bdcr_rtcen   (*(vu32*)(bb_rcc_bdcr_base+15*4)) //rw,RTCʱ��ʹ�� ��0=�رգ�1=����
#define bb_rcc_bdcr_lsebyp  (*(vu32*)(bb_rcc_bdcr_base+2*4))  //rw,1=�ⲿ����ʱ����·��0=δ��·
#define bb_rcc_bdcr_lserdy  (*(vu32*)(bb_rcc_bdcr_base+1*4))  //r ,0=32kʱ��δ������1=����
#define bb_rcc_bdcr_lseon   (*(vu32*)(bb_rcc_bdcr_base+0*4))  //rw,0=32k�����رգ�1=����
#define bm_rcc_bdcr_rtcsel  (3<<8)  //RTCʱ��ѡ��00=��ʱ�ӡ�01=lse��10=lsi��11=hse��128��Ƶ

//����RCC_TypeDef.CSRλ���룬ֻ��1λ����λ����ַ������bitmask------------
#define bb_rcc_csr_base     (0x42000000 + 0x21024*32)  //λ����ַ
#define bb_rcc_csr_lpwrrstf (*(vu32*)(bb_rcc_csr_base+31*4))  //rw,b31,0���޵͹��Ĺ���λ����
                                                            //1�������͹��Ĺ���λ ,��ͨ��д1��RMVF�����
#define bb_rcc_csr_wwdgrstf (*(vu32*)(bb_rcc_csr_base+30*4))  //rw,b30,0���޴��ڿ��Ź���λ����
                                                            //1���������ڿ��Ź���λ ,��ͨ��д1��RMVF�����
#define bb_rcc_csr_iwdgrstf (*(vu32*)(bb_rcc_csr_base+29*4))  //rw,b29,0���޶������Ź���λ����
                                                            //1�������������Ź���λ ,��ͨ��д1��RMVF�����
#define bb_rcc_csr_sftrstf  (*(vu32*)(bb_rcc_csr_base+28*4))  //rw,b28,0���������λ����
                                                            //1�����������λ ,��ͨ��д1��RMVF�����
#define bb_rcc_csr_porrstf  (*(vu32*)(bb_rcc_csr_base+27*4))  //rw,b27,0�����ϵ�/���縴λ����
                                                            //1�������ϵ�/���縴λ ,��ͨ��д1��RMVF�����
#define bb_rcc_csr_pinrstf  (*(vu32*)(bb_rcc_csr_base+26*4))  //rw,b26,0����NRST�ܽŸ�λ����
                                                            //1������NRST�ܽŸ�λ,��ͨ��д1��RMVF�����
#define bb_rcc_csr_rmvf     (*(vu32*)(bb_rcc_csr_base+24*4))  //rw,b24,0�����ָ�λ��־  1�����㸴λ��־
#define bb_rcc_csr_lsirdy   (*(vu32*)(bb_rcc_csr_base+1*4))   //r,b1,0��LSI(�ڲ�40kHz RC����)ʱ��δ����
                                                            //     1��LSI(�ڲ�40kHz RC����)ʱ�Ӿ���
#define bb_rcc_csr_lsion    (*(vu32*)bb_rcc_csr_base+0*4)   //rw,b0,0���ڲ�40kHz RC�����ر�,1���ڲ�40kHz RC��������
struct st_rcc_reg     //��λ��ʱ�ӿ��ƼĴ��� at 0x40021000
{
    vu32 CR;      //ʱ�ӿ��ƼĴ���
    vu32 CFGR;    //ʱ�����üĴ���
    vu32 CIR;     //ʱ���жϼĴ��� (RCC_CIR)
    vu32 APB2RSTR;//APB2���踴λ�Ĵ���
    vu32 APB1RSTR;//APB1���踴λ�Ĵ���
    vu32 AHBENR;  //AHB����ʱ��ʹ�ܼĴ���
    vu32 APB2ENR; //APB2����ʱ��ʹ�ܼĴ���
    vu32 APB1ENR; //APB1����ʱ��ʹ�ܼĴ���
    vu32 BDCR;    //��������ƼĴ���
    vu32 CSR;     //����/״̬�Ĵ���
} ;

extern struct st_rcc_reg volatile * const pg_rcc_reg;

#ifdef __cplusplus
}
#endif

#endif /*__RCC_H__*/



