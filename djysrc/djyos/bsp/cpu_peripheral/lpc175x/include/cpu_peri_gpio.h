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
#ifndef __CPU_PERI_GPIO_REG__
#define __CPU_PERI_GPIO_REG__

#ifdef __cplusplus
extern "C" {
#endif

//����IOģʽ
#define cn_gpio_mode_od_pullup          0x00    //��©������
#define cn_gpio_mode_od_repeater        0x01    //��©���м�(����Ӧ)
#define cn_gpio_mode_od_nopull          0x02    //��©����������
#define cn_gpio_mode_od_pulldown        0x03    //��©������
#define cn_gpio_mode_nood_pullup        0x04    //�޿�©������
#define cn_gpio_mode_nood_repeater      0x05    //�޿�©���м�(����Ӧ)
#define cn_gpio_mode_nood_nopull        0x06    //�޿�©����������
#define cn_gpio_mode_nood_pulldown      0x07    //�޿�©������

//���¶���ȫ��gpio�Ĺ���
#define	cn_p0_0_gpio             0x00      // gpio                         
#define cn_p0_0_rd1              0x01      // can-1��������������          
#define cn_p0_0_txd3             0x02      // uart3��������                
#define cn_p0_0_sda1             0x03      // i2c-1�����ߣ���©��          

#define cn_p0_1_gpio             0x00      // gpio                         
#define cn_p0_1_td1              0x01      // can-1��������������          
#define cn_p0_1_rxd3             0x02      // uart3��������                
#define cn_p0_1_scl1             0x03      // i2c-1ʱ���ߣ���©��          

#define cn_p0_2_gpio             0x00      // gpio                         
#define cn_p0_2_txd0             0x01      // uart0��������                
#define cn_p0_2_ad07             0x02      // ad0.7����                    
//...   reserved              0x03

#define cn_p0_3_gpio             0x00      // gpio                         
#define cn_p0_3_rxd0             0x01      // uart0��������                
#define cn_p0_3_ad06             0x02      // ad0.6����                    
//...   reserved              0x03 

#define cn_p0_4_gpio             0x00      // gpio                         
#define cn_p0_4_i2srx_clk        0x01      // i2s���߽���ʱ��sck           
#define cn_p0_4_rd2              0x02      // can-2������                  
#define cn_p0_4_cap20            0x03      // ��ʱ��2�������룬ͨ��0       
		
#define cn_p0_5_gpio             0x00      // gpio                         
#define cn_p0_5_i2srx_ws         0x01      // i2s���߽�����ѡ��ws          
#define cn_p0_5_td2              0x02      // can-2������                  
#define cn_p0_5_cap21            0x03      // ��ʱ��2�������룬ͨ��1       
		
#define cn_p0_6_gpio             0x00      // gpio                         
#define cn_p0_6_i2srx_sda        0x01      // i2s���߽���������sd          
#define cn_p0_6_ssel1            0x02      // ssp-1���ߴӻ�ѡ��            
#define cn_p0_6_mat20            0x03      // ��ʱ��2ƥ�������ͨ��0       
		
#define cn_p0_7_gpio             0x00      // gpio                         
#define cn_p0_7_i2stx_clk        0x01      // i2s���߷���ʱ����sck         
#define cn_p0_7_ssck1            0x02      // ssp-1ʱ����                  
#define cn_p0_7_mat21            0x03      // ��ʱ��2ƥ�������ͨ��1       
		
#define cn_p0_8_gpio             0x00      // gpio                         
#define cn_p0_8_i2stx_ws         0x01      // i2s���߷�����ѡ��ws          
#define cn_p0_8_smiso1           0x02      // ssp-1����������������      
#define cn_p0_8_mat22            0x03      // ��ʱ��2ƥ�������ͨ��2       

#define cn_p0_9_gpio             0x00      // gpio                         
#define cn_p0_9_i2stx_sda        0x01      // i2s���߷���������sd          
#define cn_p0_9_smosi1           0x02      // ssp-1�����������������      
#define cn_p0_9_mat23            0x03      // ��ʱ��2ƥ�������ͨ��3       

#define cn_p0_10_gpio            0x00      // gpio                         
#define cn_p0_10_txd2            0x01      // uart2��������                
#define cn_p0_10_sda2            0x02      // i2c-2������(�ǿ�©����)      
#define cn_p0_10_mat30           0x03      // ��ʱ��3ƥ�������ͨ��0       

#define cn_p0_11_gpio            0x00      // gpio                         
#define cn_p0_11_rxd2            0x01      // uart2��������                
#define cn_p0_11_scl2            0x02      // i2c-2ʱ����(�ǿ�©����)      
#define cn_p0_11_mat31           0x03      // ��ʱ��3ƥ�������ͨ��1       
 
#define cn_p0_15_gpio            0x00      // gpio                         
#define cn_p0_15_txd1            0x01      // uart1��������                
#define cn_p0_15_sck0            0x02      // ssp0ʱ����                   
#define cn_p0_15_sck             0x03      // spiʱ����                    
                                                   
#define cn_p0_16_gpio            0x00      // gpio                         
#define cn_p0_16_rxd1            0x01      // uart1��������                
#define cn_p0_16_ssel0           0x02      // ssp0�ӻ�ѡ��                 
#define cn_p0_16_ssel            0x03      // spi�ӻ�ѡ��                  
                                                
#define cn_p0_17_gpio            0x00      // gpio                         
#define cn_p0_17_cts1            0x01      // uart1���㷢���ź�cts         
#define cn_p0_17_miso0           0x02      // ssp0����������������       
#define cn_p0_17_miso            0x03      // spi����ӳ�������            
                                                
#define cn_p0_18_gpio            0x00      // gpio                         
#define cn_p0_18_dcd1            0x01      // uart1�����ز��������dcd     
#define cn_p0_18_mosi0           0x02      // ssp-0�����������������      
#define cn_p0_18_mosi            0x03      // spi��������������            
                                                
#define cn_p0_19_gpio            0x00      // gpio                         
#define cn_p0_19_dsr1            0x01      // uart1�������þ���dsr         
//...   reserved              0x02      // ����                         
#define cn_p0_19_sda1            0x03      // i2c-1�����ߣ��ǿ�©��        
                                                
#define cn_p0_20_gpio            0x00      // gpio                         
#define cn_p0_20_dtr1            0x01      // uart1�ն˾���dtr             
//...   reserved              0x02                                  	/* ����                         
#define cn_p0_20_scl1            0x03      // i2c-1ʱ���ߣ��ǿ�©��        
                                           
#define cn_p0_21_gpio            0x00      // gpio                         
#define cn_p0_21_ri1             0x01      // uart1�����ź�                
//...   reserved              0x02      // ����                         
#define cn_p0_21_rd1             0x03      // can-1��������                
                                      
#define cn_p0_22_gpio            0x00      // gpio                         
#define cn_p0_22_rts1            0x01      // uart1����������            
//...   reserved              0x02      // ����                         
#define cn_p0_22_td1             0x03      // can-1������                  
                                      
#define cn_p0_23_gpio            0x00      // gpio                         
#define cn_p0_23_ad00            0x01      // adc-0��ͨ��0                 
#define cn_p0_23_i2srx_clk       0x02      // i2s����ʱ��sck               
#define cn_p0_23_cap30           0x03      // ��ʱ��3�������룬ͨ��0       
                                      
#define cn_p0_24_gpio            0x00      // gpio                         
#define cn_p0_24_ad01            0x01      // adc-0��ͨ��1                 
#define cn_p0_24_i2srx_ws        0x02      // i2s������ѡ��ws              
#define cn_p0_24_cap31           0x03      // ��ʱ��3�������룬ͨ��1       
                                      
#define cn_p0_25_gpio            0x00      // gpio                         
#define cn_p0_25_ad02            0x01      // adc-0��ͨ��2                 
#define cn_p0_25_i2srx_sda       0x02      // i2s��������sd                
#define cn_p0_25_txd3            0x03      // uart3��������                
                                  
#define cn_p0_26_gpio            0x00      // gpio                         
#define cn_p0_26_ad03            0x01      // adc-0��ͨ��3                 
#define cn_p0_26_aout            0x02      // dac���                      
#define cn_p0_26_rxd3            0x03      // uart-3��������               
                                          
#define cn_p0_27_gpio            0x00      // gpio                         
#define cn_p0_27_sda0            0x01      // i2c0��������/�������©      
#define cn_p0_27_usb_sda         0x02      // usb�ӿ�i2c��������otg�շ���  
//..    reserved              0x03      // ����                         
                                          
#define cn_p0_28_gpio            0x00      // gpio                         
#define cn_p0_28_scl0            0x01      // i2c0ʱ�����룬��©           
#define cn_p0_28_usb_scl         0x02      // usb�ӿ�i2cʱ���ߣ�otg�շ���  
//..    reserved              0x03                                   /* ����                         
                                      
#define cn_p0_29_gpio            0x00      // gpio                         
#define cn_p0_29_usb_dd          0x01      // usb˫������d+��              
//..    reserved              0x02      // ����                         
//..    reserved              0x03      // ����                         
                                      
#define cn_p0_30_gpio            0x00      // gpio                         
#define cn_p0_30_usb_ds          0x01      // usb˫������d-��              
//..    reserved              0x02      // ����                         
//..    reserved              0x03      // ����                         
                                      							  
//p1�����Ź�������
#define cn_p1_0_gpio             0x00      // gpio                         
#define cn_p1_0_enet_txd0        0x01      // ��̫���������ݽ�0            
//..    reserved              0x02      // ����                         
//..    reserved              0x03      // ����                         

#define cn_p1_1_gpio             0x00      // gpio                         
#define cn_p1_1_enet_tx_en       0x01      // ��̫���������ݽ�1            
//..    reserved              0x02      // ����                         
//..    reserved              0x03      // ����                         

#define cn_p1_4_gpio             0x00      // gpio                         
#define cn_p1_4_enet_tx_en       0x01      // ��̫������ʹ��               
//..    reserved              0x02      // ����                         
//..    reserved              0x03      // ����                         

#define cn_p1_8_gpio             0x00      // gpio                         
#define cn_p1_8_enet_crs         0x01      // ��̫���ز�����             
//..    reserved              0x02      // ����                         
//..    reserved              0x03      // ����                         
										     
#define cn_p1_9_gpio             0x00      // gpio                         
#define cn_p1_9_enet_rxd0        0x01      // ��̫���������ݽ�0            
//..    reserved              0x02      // ����                         
//..    reserved              0x03      // ����                         
													  
#define cn_p1_10_gpio            0x00      // gpio                         
#define cn_p1_10_enet_rxd1       0x01      // ��̫���������ݽ�1            
//..    reserved              0x02      // ����                         
//..    reserved              0x03      // ����                         

#define cn_p1_14_gpio            0x00      //  gpio                        
#define cn_p1_14_enet_rx_er      0x01      // ��̫�����մ���               
//..    reserved              0x02      // ����                         
//..    reserved              0x03      // ����                         
													     
#define cn_p1_15_gpio            0x00      // gpio                         
#define cn_p1_15_enet_ref_clk    0x01      // ��̫���ο�ʱ������           
//..    reserved              0x02      // ����                         
//..    reserved              0x03      // ����                         
											   
#define cn_p1_16_gpio            0x00      // gpio                         
#define cn_p1_16_enet_mdc        0x01      // ��̫��miimʱ��               
//..    reserved              0x02      // ����                         
//..    reserved              0x03      // ����                         

#define cn_p1_17_gpio            0x00      // gpio                         
#define cn_p1_17_enet_mdio       0x01      // ��̫��mi��������\���        
//..    reserved              0x02      // ����                         
//..    reserved              0x03      // ����                         

#define cn_p1_18_gpio            0x00      //  gpio                        
#define cn_p1_18_usb_up_led      0x01      // usb goodlinkָʾ��           
#define cn_p1_18_pwm11           0x02      // pwm1�����ͨ��1              
#define cn_p1_18_cap10           0x03      // timer1�������룬ͨ��0        
                                              
#define cn_p1_19_gpio            0x00      //  gpio                        
#define cn_p1_19_mco0a           0x01      // �������pwmͨ��0�����a      
#define cn_p1_19_usb_ppwr        0x02      // usb�˿ڵ�Դʹ���ź�          
#define cn_p1_19_cap11           0x03      // timer1�������룬ͨ��1        
                                              
#define cn_p1_20_gpio            0x00      //  gpio                        
#define cn_p1_20_mcfb0           0x01      // �������pwmͨ��0���ػ�����   
#define cn_p1_20_pwm12           0x02      // pwm-2�����ͨ��2             
#define cn_p1_20_sck0            0x03      // ssp-0ʱ������                
                                              
#define cn_p1_21_gpio            0x00      // gpio                         
#define cn_p1_21_mcabort         0x01      // �������pwm��������ֹ        
#define cn_p1_21_pwm13           0x02      // pwm-1�����ͨ��3             
#define cn_p1_21_ssel0           0x03      // ssp-0�ӻ�ѡ��                
                                              
#define cn_p1_22_gpio            0x00      // gpio                         
#define cn_p1_22_mc0b            0x01      // �������pwmͨ��0�����b      
#define cn_p1_22_usb_pwrd        0x02      // usb�˿ڵĵ�Դ״̬            
#define cn_p1_22_mat10           0x03      // ��ʱ��1ƥ�������ͨ��0       
                                              
#define cn_p1_23_gpio            0x00      // gpio                         
#define cn_p1_23_mcfb1           0x01      // �������pwmͨ��1����������   
#define cn_p1_23_pwm14           0x02      // pwm-1�����ͨ��4             
#define cn_p1_23_miso0           0x03      // ssp-0�������룬�ӻ����      
                                              
#define cn_p1_24_gpio            0x00      // gpio                         
#define cn_p1_24_mcfb2           0x01      // �������pwmͨ��2����������   
#define cn_p1_24_pwm15           0x02      // pwm-1�����ͨ��5             
#define cn_p1_24_mosi0           0x03      // ssp-0����������ӻ�����      
                                              
#define cn_p1_25_gpio            0x00      // gpio                         
#define cn_p1_25_mc1a            0x01      // �������pwmͨ��1�����a      
#define cn_p1_25_clkout          0x02      // ʱ���������                 
#define cn_p1_25_mat11           0x03      // ��ʱ��1ƥ�������ͨ��1       
                                              
#define cn_p1_26_gpio            0x00      // gpio                         
#define cn_p1_26_mc1b            0x01      // �������pwmͨ��1�����b      
#define cn_p1_26_pwm16           0x02      // pwm-1�����ͨ��6             
#define cn_p1_26_cap00           0x03      // ��ʱ��0�������룬ͨ��0       
                                              
#define cn_p1_27_gpio            0x00      // gpio                         
#define cn_p1_27_clkout          0x01      // ʱ���������                 
#define cn_p1_27_usb_ovrcr       0x02      // usb�˿ڹ��ص���״̬          
#define cn_p1_27_cap01           0x03      // ��ʱ��0�������룬ͨ��1       
                                              
#define cn_p1_28_gpio            0x00      // gpio                         
#define cn_p1_28_mc2a            0x01      // �������pwmͨ��2�����a      
#define cn_p1_28_pcap10          0x02      // pwm-1�������룬ͨ��0         
#define cn_p1_28_mat00           0x03      // ��ʱ��0ƥ�������ͨ��0       
                                              
#define cn_p1_29_gpio            0x00      // gpio                         
#define cn_p1_29_mc2b            0x01      // �������pwmͨ��2�����b      
#define cn_p1_29_pcap11          0x02      // pwm-1�������룬ͨ��1         
#define cn_p1_29_mat01           0x03      // ��ʱ��0ƥ�������ͨ��1       
                                              
#define cn_p1_30_gpio            0x00      // gpio                         
//...   reserved              0x01      // ����                         
#define cn_p1_30_vbus            0x02      // ���ڵ�usb���ߵ�Դ            
#define cn_p1_30_ad04            0x03      // adcת����0.ͨ��4����         
                                              
#define cn_p1_31_gpio            0x00      // gpio                         
//...   reserved              0x01      // ����                         
#define cn_p1_31_ssck1           0x02      // ssp-1ʱ����                  
#define cn_p1_31_ad05            0x03      // adc-0���룬ͨ��5             
                                             
//p2�����Ź�������
#define cn_p2_0_gpio             0x00      // gpio                         
#define cn_p2_0_pwm11            0x01      // pwm-1��ͨ��1                 
#define cn_p2_0_txd1             0x02      // uart1���ݷ�������            
//...   reserved              0x03      // ����                         

#define cn_p2_1_gpio             0x00      // gpio                         
#define cn_p2_1_pwm12            0x01      // pwm-1��ͨ��2                 
#define cn_p2_1_rxd1             0x02      // uart1���ݽ�������            
//...   reserved              0x03      // ����                         

#define cn_p2_2_gpio             0x00      // gpio                         
#define cn_p2_2_pwm13            0x01      // pwm-1��ͨ��3                 
#define cn_p2_2_cts1             0x02      // uart1���㷢����              
//...   reserved              0x03      // ����                         

#define cn_p2_3_gpio             0x00      // gpio                         
#define cn_p2_3_pwm14            0x01      // pwm-1��ͨ��4                 
#define cn_p2_3_dcd1             0x02      // uart1�����ز������          
//...   reserved              0x03      // ����                         

#define cn_p2_4_gpio             0x00      // gpio                         
#define cn_p2_4_pwm15            0x01      // pwm-1��ͨ��5                 
#define cn_p2_4_dsr1             0x02      // uart1�������þ�����          
//...   reserved              0x03      // ����                         

#define cn_p2_5_gpio             0x00      // gpio                         
#define cn_p2_5_pwm16            0x01      // pwm-1��ͨ��6                 
#define cn_p2_5_dtr1             0x02      // uart1�����ն˾�����          
//...   reserved              0x03      // ����                         

#define cn_p2_6_gpio             0x00      // gpio                         
#define cn_p2_6_pcap10           0x01      // pwm-1�������룬ͨ��0         
#define cn_p2_6_ri1              0x02      // uart1��������                
//...   reserved              0x03      // ����                         

#define cn_p2_7_gpio             0x00      // gpio                         
#define cn_p2_7_rd2              0x01      // can-2������                  
#define cn_p2_7_rts1             0x02      // uart1���������            
                                        // rs-485/eia-485���ʹ���ź�   
//...   reserved              0x03      // ����                         

#define cn_p2_8_gpio             0x00      // gpio                         
#define cn_p2_8_td2              0x01      // can-2������                  
#define cn_p2_8_txd2             0x02      // uart2��������                
#define cn_p2_8_enet_mdc         0x03      // ��̫��miimʱ��               

#define cn_p2_9_gpio             0x00      // gpio                         
#define cn_p2_9_u1connect        0x01      // usb1�����ӿ���               
#define cn_p2_9_rxd2             0x02      // uart2��������                
#define cn_p2_9_enet_mdio        0x03      // ��̫��miim������������     

#define cn_p2_10_gpio            0x00      // gpio                         
#define cn_p2_10_eint0           0x01      // �ⲿ�ж�0����                
#define cn_p2_10_nmi             0x02      // �������ж�����               
//...   reserved              0x03      // ����                         
                                              
#define cn_p2_11_gpio            0x00      // gpio                         
#define cn_p2_11_eint1           0x01      // �ⲿ�ж�1����                
//...   reserved              0x02      // ����                         
#define cn_p2_11_i2stx_clk       0x03      // i2s����ʱ��sck               
                                              
#define cn_p2_12_gpio            0x00      // gpio                         
#define cn_p2_12_eint2           0x01      // �ⲿ�ж�2����                
//...   reserved              0x02      // ����                         
#define cn_p2_12_i2stx_ws        0x03      // i2s������ѡ��ws              
                                              
#define cn_p2_13_gpio            0x00      // gpio                         
#define cn_p2_13_eint3           0x01      // �ⲿ�ж�3����                
//...   reserved              0x02      // ����                         
#define cn_p2_13_i2stx_sda       0x03      // i2s��������sd                
                                              
//p3�����Ź�������
#define cn_p3_25_gpio            0x00      //  gpio                        
//...   reserved              0x01      //  ����                        
#define cn_p3_25_mat00           0x02      //  ��ʱ��0ƥ�������ͨ��0      
#define cn_p3_25_pwm12           0x03      //  pwm-1�����ͨ��2            
                                               
#define cn_p3_26_gpio            0x00      // gpio                         
#define cn_p3_26_stclk           0x01      // ϵͳ���Ķ�ʱ��ʱ������       
#define cn_p3_26_mat01           0x02      // ��ʱ��0ƥ�������ͨ��1       
#define cn_p3_26_pwm13           0x03      // pwm-1�����ͨ��3             
                                               
//p4�����Ź�������
#define cn_p4_28_gpio            0x00      // gpio                         
#define cn_p4_28_rx_mclk         0x01      // ��i2s��������ʱ��            
#define cn_p4_28_mat20           0x02      // ��ʱ��2ƥ�������ͨ��0       
#define cn_p4_28_txd3            0x03      // uart3��������                
                                               
#define cn_p4_29_gpio            0x00      // gpio                         
#define cn_p4_29_tx_mclk         0x01      // i2s��������ʱ��              
#define cn_p4_29_mat21           0x02      // ��ʱ��2ƥ�������ͨ��1       
#define cn_p4_29_rxd3            0x03      // uart3��������                
                                               
//p10  the trace port interface unit (tpiu)
#define cn_tpiu_disable          0x00      //tpiu interface ��ֹ          
#define cn_tpiu_enable           0x01      //tpiu interface ʹ��          

//�������üĴ���������ѡ�����Ź��ܡ���������ģʽ
//address=0x4002c000
struct st_pin_config_reg
{
    vu32 pinsel0;
    vu32 pinsel1;
    vu32 pinsel2;
    vu32 pinsel3;
    vu32 pinsel4;
    vu32 pinsel5;
    vu32 pinsel6;
    vu32 pinsel7;
    vu32 pinsel8;
    vu32 pinsel9;
    vu32 pinsel10;
    u32 res[5];
    vu32 pinmode0;
    vu32 pinmode1;
    vu32 pinmode2;
    vu32 pinmode3;
    vu32 pinmode4;
    vu32 pinmode5;
    vu32 pinmode6;
    vu32 pinmode7;
    vu32 pinmode8;
    vu32 pinmode9;
    vu32 pinmode_od0;
    vu32 pinmode_od1;
    vu32 pinmode_od2;
    vu32 pinmode_od3;
    vu32 pinmode_od4;
    vu32 i2cpadcfg;
};

//gpioģ����ƼĴ���
//address:  p0 = 0x2009c000
//          p1 = 0x2009c020
//          p2 = 0x2009c040
//          p3 = 0x2009c080
struct st_gpio_reg
{
    vu32    dir;
    u32     res[3];
    vu32    mask;
    vu32    pin;
    vu32    setpin;
    vu32    clrpin;
};

#ifdef __cplusplus
}
#endif

#endif /*__CPU_PERI_GPIO_REG__*/
