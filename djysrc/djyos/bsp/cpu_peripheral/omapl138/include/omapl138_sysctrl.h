//========================================================  
// �ļ�ģ��˵��: ϵͳ���ƼĴ�������
// �ļ��汾: v1.00                                          
// ������Ա: lst                                            
// ����ʱ��: 2011.06.01                                     
// Copyright(c) 2011-2011  ��������������޹�˾             
//========================================================  
// �����޸ļ�¼(���µķ�����ǰ��):                            
//  <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>            
//========================================================  
//���磺
// V1.02 2004-07-14, ����: ���պ������ӶԱ��ĳ�����Ч��У��

#ifndef __CPU_PERI_SYSCTRL_H__
#define __CPU_PERI_SYSCTRL_H__

#ifdef __cplusplus
extern "C" {
#endif

struct st_sysctrl_reg
{
    vu32 flashcfg;               // flash accelerator module           
    u32 reserved0[31];
    vu32 pll0con;                // clocking and power control         
    vu32 pll0cfg;
    vu32 pll0stat;
    vu32 pll0feed;
    u32 reserved1[4];
    vu32 pll1con;
    vu32 pll1cfg;
    vu32 pll1stat;
    vu32 pll1feed;
    u32 reserved2[4];
    vu32 pcon;
    vu32 pconp;
    u32 reserved3[15];
    vu32 cclkcfg;
    vu32 usbclkcfg;
    vu32 clksrcsel;
    u32 reserved4[12];
    vu32 extint;                 // external interrupts                
    u32 reserved5;
    vu32 extmode;
    vu32 extpolar;
    u32 reserved6[12];
    vu32 rsid;                   // reset                              
    u32 reserved7[7];
    vu32 scs;                    // syscon miscellaneous registers     
    vu32 irctrim;                // clock dividers                     
    vu32 pclksel0;
    vu32 pclksel1;
    u32 reserved8[4];
    vu32 usbintst;               // usb device/otg interrupt register  
    u32 reserved9;
    vu32 clkoutcfg;              // clock output configuration         
 };

#ifdef __cplusplus
}
#endif

#endif //__CPU_PERI_SYSCTRL_H__

