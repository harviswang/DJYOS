//----------------------------------------------------
//Copyright (C), 2005-2009,  lst.
//��Ȩ���� (C), 2005-2009,   lst.
//����ģ��:  ����
//���ߣ�     lst
//�汾��      V1.0.0
//��ʼ�汾������ڣ�2009-08-05
//�ļ�����:  cortex-m3�Ĵ�����ͷ�ļ�
//����˵��:  ��
//�޶���ʷ:
//2. ...
//1. ����: 2009-08-05
//   ����: lst
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------

#ifndef __CM3_H__
#define __CM3_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef enum IRQn
{
  NonMaskableInt_IRQn           = -14,      /*!<  2 Non Maskable Interrupt                        */
  MemoryManagement_IRQn         = -12,      /*!<  4 Memory Management Interrupt                   */
  BusFault_IRQn                 = -11,      /*!<  5 Bus Fault Interrupt                           */
  UsageFault_IRQn               = -10,      /*!<  6 Usage Fault Interrupt                         */
  SVCall_IRQn                   = -5,       /*!< 11 SV Call Interrupt                             */
  DebugMonitor_IRQn             = -4,       /*!< 12 Debug Monitor Interrupt                       */
  PendSV_IRQn                   = -2,       /*!< 14 Pend SV Interrupt                             */
  SysTick_IRQn                  = -1,       /*!< 15 System Tick Interrupt                         */
} IRQn_Type;

//------------------------ SystemTick ----------------------------------------
//CTRL �жϿ��Ƽ�״̬�Ĵ�����0xE000_E010,bm_Ϊλ���룬bo_Ϊλƫ��
#define bm_systick_ctrl_countflag   (1<<16) //rc,����ֵ��Ϊ0ʱ�Զ���1��������
#define bm_systick_ctrl_clksource   (1<<2)  //rw,ʱ��ѡ��0=�ⲿʱ��stclk��1=�ں�fclk
#define bm_systick_ctrl_tickint     (1<<1)  //rw,1=SysTick ����0 ʱ����SysTick �쳣����
#define bm_systick_ctrl_enable      (1<<0)  //rw,SysTick ��ʱ����ʹ��λ
#define bo_systick_ctrl_countflag   16   //rc,����ֵ��Ϊ0ʱ�Զ���1��������
#define bo_systick_ctrl_clksource   2    //rw,ʱ��ѡ��0=�ⲿʱ��stclk��1=�ں�fclk
#define bo_systick_ctrl_tickint     1    //rw,1=SysTick ����0 ʱ����SysTick �쳣����
#define bo_systick_ctrl_enable      0    //rw,SysTick ��ʱ����ʹ��λ

//CTRL SysTickУ׼��ֵ�Ĵ��� 0xE000_E01c,bm_Ϊλ���룬bo_Ϊλƫ��
#define bm_systick_calib_NOREF  (1<<31)  //r,1=STCLK ������,0=����
#define bm_systick_calib_SKEW   (1<<30)  //r,1=У׼ֵ����׼ȷ��10ms��0=��
#define bo_systick_calib_NOREF  31  //r,1=STCLK ������,0=����
#define bo_systick_calib_SKEW   30  //r,1=У׼ֵ����׼ȷ��10ms��0=��
//rw,10ms ��ʱ���ڵ������ĸ�����оƬ�����Ӧ��ͨ��Cortex�\M3 �������ź��ṩ����ֵ��
//����ֵ�����㣬���ʾ�޷�ʹ��У׼����
#define bm_systick_calib_TENMS  (0xffffff<<0)
#define bo_systick_calib_TENMS  0

struct systick_reg      //0xe000e010
{
    vu32 ctrl;
    vu32 reload;    //����������ʱ��������װ�ص�ֵ
    vu32 current;   //��ȡʱ���ص�ǰ��������ֵ��д�����㣬����CTRL�е�COUNTFLAG ��־
    vuc32 verify;   //SysTickУ׼��ֵ�Ĵ���
};
//
//cpu�汾��
#define cn_revision_r0p0   0x410fc230
#define cn_revision_r1p0   0x410fc231
#define cn_revision_r1p1   0x411fc231
#define cn_revision_r2p0   0x412fc320

//����scb_reg.ICSR �жϿ��Ƽ�״̬�Ĵ�����0xE000_ED04 ,bm_Ϊλ���룬bo_Ϊλƫ��  
#define bm_scb_icsr_nmipendset  (1<<31)     //rw д1����nmi
#define bm_scb_icsr_pendsvset   (1<<28)     //rw д1����pendsv�������ع���״̬
#define bm_scb_icsr_pendsvclr   (1<<27)     //w д1���pendsv����״̬
#define bm_scb_icsr_pendstset   (1<<26)     //rw д1����systick�������ع���״̬
#define bm_scb_icsr_pendstclr   (1<<25)     //w д1���systick����״̬
#define bm_scb_icsr_isrpreempt  (1<<23)     //r ������
#define bm_scb_icsr_isrpending  (1<<22)     //r ���жϹ��𣨲���nmi��
#define bm_scb_icsr_vectpending (0x3ff<<12) //r ������ж������ȼ���ߵ��жϺ�
#define bm_scb_icsr_rettobase   (1<<11)     //r 1=ִ���쳣���������û��Ƕ�ס�
                                            //  0=�������л�Ƕ���쳣������
#define bm_scb_icsr_vectactive  (0xff<<0)   //r ���ڷ�����쳣��
#define bo_scb_icsr_nmipendset  31    
#define bo_scb_icsr_pendsvset   28    
#define bo_scb_icsr_pendsvclr   27    
#define bo_scb_icsr_pendstset   26    
#define bo_scb_icsr_pendstclr   25    
#define bo_scb_icsr_isrpreempt  23    
#define bo_scb_icsr_isrpending  22    
#define bo_scb_icsr_vectpending 12
#define bo_scb_icsr_rettobase   11    
#define bo_scb_icsr_vectactive  0  

//AIRCR Ӧ�ó����жϼ���λ���ƼĴ�����0xE000_ED0c ,bm_Ϊλ���룬bo_Ϊλƫ��  
#define bm_scb_aircr_vectkey        (0xffff<<16)
#define bm_scb_aircr_endianess      (1<<15)     
#define bm_scb_aircr_prigroup       (7<<8)      
#define bm_scb_aircr_sysresetreq    (1<<2)      
#define bm_scb_aircr_vectclractive  (1<<1)      
#define bm_scb_aircr_vectreset      (1<<0)      
#define bo_scb_aircr_vectkey        16  //rw д���Ĵ���������ͬʱ�Դ���д0x05fa
#define bo_scb_aircr_endianess      15  //r 1=��ˣ�0=С��
#define bo_scb_aircr_prigroup       8   //rw ����+1=�����ȼ�ռ��λ��
#define bo_scb_aircr_sysresetreq    2   //w д1�߼���λоƬ
#define bo_scb_aircr_vectclractive  1   //w д1��������쳣�״̬��Ϣ��
#define bo_scb_aircr_vectreset      0   //w д1��λcm3�ںˣ���Ӱ���ں������·

//����scb_reg.SCR ϵͳ���ƼĴ���,0xE000_ED10 ,bm_Ϊλ���룬bo_Ϊλƫ��  
#define bm_scb_scr_sevonpend    (1<<4)      
#define bm_scb_scr_sleepdeep    (1<<2)      
#define bm_scb_scr_sleeponexit  (1<<1)      
#define bo_scb_scr_sevonpend    4      //rw 1=�κ��жϺ��¼������Ի��Ѵ�����
                                       //   0=ֻ��ʹ�ܵ��жϺ��¼����Ի��Ѵ�����
#define bo_scb_scr_sleepdeep    2      //rw 0=��ͨ˯�ߣ�1=���˯��
#define bo_scb_scr_sleeponexit  1      //rw 1=���жϷ���ʱ����sleep

//����scb_reg.CCR ��������ƼĴ���,0xE000_ED14 ,bm_Ϊλ���룬bo_Ϊλƫ��  
#define bm_scb_ccr_stkalign         (1<<9) 
#define bm_scb_ccr_bfhfnmign        (1<<8) 
#define bm_scb_ccr_div0trp          (1<<4) 
#define bm_scb_ccr_unaligntrp       (1<<3) 
#define bm_scb_ccr_usersetmpend     (1<<1) 
#define bm_scb_ccr_nonbasethrdena   (1<<0) 
#define bo_scb_ccr_stkalign         9   //rw �쳣��Ӧ�Զ���ջʱǿ��dp˫�ֶ���
#define bo_scb_ccr_bfhfnmign        8   //rw 1=Ӳfault��nmi�����к�����������fault
#define bo_scb_ccr_div0trp          4   //rw 1=ʹ�ܳ�o�÷�fault
#define bo_scb_ccr_unaligntrp       3   //rw 1=δ������ʽ������÷�fault
#define bo_scb_ccr_usersetmpend     1   //rw 1=�����û���������stir
#define bo_scb_ccr_nonbasethrdena   0   //rw,д1������������޸�exc_return�����߳�ģʽ

//����scb_reg.SHCSRλ���룬0xE000ED24,bm_Ϊλ���룬bo_Ϊλƫ��  
#define bm_scb_shcsr_usgfaultena    (1<<18)  
#define bm_scb_shcsr_busfaultena    (1<<17)  
#define bm_scb_shcsr_memfaultena    (1<<16)  
#define bm_scb_shcsr_svcallpended   (1<<15)  
#define bm_scb_shcsr_busfaultpended (1<<14)  
#define bm_scb_shcsr_memfaultpended (1<<13)  
#define bm_scb_shcsr_usgfaultpended (1<<12)  
#define bm_scb_shcsr_systickact     (1<<11)  
#define bm_scb_shcsr_pendsvact      (1<<10)  
#define bm_scb_shcsr_monitoract     (1<<8)   
#define bm_scb_shcsr_svcallact      (1<<7)   
#define bm_scb_shcsr_usgfaultact    (1<<3)   
#define bm_scb_shcsr_busfaultact    (1<<1)   
#define bm_scb_shcsr_memfaultact    (1<<0)   
#define bo_scb_shcsr_usgfaultena    18  //rw,b,1 = ʹ���÷� fault��
#define bo_scb_shcsr_busfaultena    17  //rw,b,1 = ʹ������ fault��
#define bo_scb_shcsr_memfaultena    16  //rw,b,1 = ʹ�ܴ洢������ fault��
#define bo_scb_shcsr_svcallpended   15  //rw,b,1 = SVC �����С�
#define bo_scb_shcsr_busfaultpended 14  //rw,b,1 = ���� fault ������
#define bo_scb_shcsr_memfaultpended 13  //rw,b,1 = �洢������ fault ������
#define bo_scb_shcsr_usgfaultpended 12  //rw,b,1 = �÷� fault ������
#define bo_scb_shcsr_systickact     11  //rw,b,1 = SysTick �쳣���
#define bo_scb_shcsr_pendsvact      10  //rw,b,1 = PendSV �쳣���
#define bo_scb_shcsr_monitoract     8   //rw,b,1 = Devug Monitor �쳣���
#define bo_scb_shcsr_svcallact      7   //rw,b,1= SVC �쳣���
#define bo_scb_shcsr_usgfaultact    3   //rw,b,1= �÷� fault �쳣���
#define bo_scb_shcsr_busfaultact    1   //rw,b,1 = ���� fault �쳣���
#define bo_scb_shcsr_memfaultact    0   //rw,b,1 = �洢������ fault �쳣���

//MFSR �洢������ fault ״̬�Ĵ���,0xE000_ED28 ,bm_Ϊλ���룬bo_Ϊλƫ��  
#define bm_scb_mfsr_MMARVALID   (1<<7)   
#define bm_scb_mfsr_STKERR      (1<<4)   
#define bm_scb_mfsr_UNSTKERR    (1<<3)   
#define bm_scb_mfsr_DACCVIOL    (1<<1)   
#define bm_scb_mfsr_IACCVIOL    (1<<0)   
#define bo_scb_mfsr_MMARVALID   7      //r,�洢������fault��ַ�Ĵ�����MMAR����Ч
#define bo_scb_mfsr_STKERR      4      //r/Wc,��ջʱ��������
#define bo_scb_mfsr_UNSTKERR    3      //r/Wc,��ջʱ��������
#define bo_scb_mfsr_DACCVIOL    1      //r/Wc,���ݷ���Υ��
#define bo_scb_mfsr_IACCVIOL    0      //r/Wc,ȡַ����Υ��

//����scb_reg.BFSR ���� fault ״̬�Ĵ���,0xE000_ED29 ,bm_Ϊλ���룬bo_Ϊλƫ��  
#define bm_scb_bfsr_BFARVALID   (1<<7)   
#define bm_scb_bfsr_STKERR      (1<<4)   
#define bm_scb_bfsr_UNSTKERR    (1<<3)   
#define bm_scb_bfsr_IMPREISERR  (1<<2)   
#define bm_scb_bfsr_PRECISERR   (1<<1)   
#define bm_scb_bfsr_IACCVIOL    (1<<0)   
#define bo_scb_bfsr_BFARVALID   7    //r,����fault��ַ�Ĵ�����BFAR����Ч
#define bo_scb_bfsr_STKERR      4    //r/Wc,��ջʱ��������
#define bo_scb_bfsr_UNSTKERR    3    //r/Wc,��ջʱ��������
#define bo_scb_bfsr_IMPREISERR  2    //r/Wc,����ȷ�����ݷ���Υ����violation��
#define bo_scb_bfsr_PRECISERR   1    //r/Wc,��ȷ�����ݷ���Υ����violation��
#define bo_scb_bfsr_IACCVIOL    0    //r/Wc,ȡַ����Υ��

//UFSR �÷� fault ״̬�Ĵ�������ַ��0xE000_ED2A ,bm_Ϊλ���룬bo_Ϊλƫ��  
#define bm_scb_ufsr_DIVBYZERO   (1<<9) 
#define bm_scb_ufsr_UNALIGNED   (1<<8) 
#define bm_scb_ufsr_NOCP        (1<<3) 
#define bm_scb_ufsr_INVPC       (1<<2) 
#define bm_scb_ufsr_INVSTATE    (1<<1) 
#define bm_scb_ufsr_UNDEFINSTR  (1<<0) 
#define bo_scb_ufsr_DIVBYZERO   9   //r/Wc,��ʾ��������ʱ����Ϊ��
#define bo_scb_ufsr_UNALIGNED   8   //r/Wc,δ������ʵ��µ� fault?
#define bo_scb_ufsr_NOCP        3   //r/Wc,��ͼִ��Э���������ָ��?
#define bo_scb_ufsr_INVPC       2   //r/Wc,���쳣����ʱEXC_RETURN�Ƿ�
#define bo_scb_ufsr_INVSTATE    1   //r/Wc,��ͼ���� ARM ״̬
#define bo_scb_ufsr_UNDEFINSTR  0   //r/Wc,ִ�е�ָ���������δ�����

//����scb_reg.HFSR Ӳ fault ״̬�Ĵ���  0xE000_ED2C ,bm_Ϊλ���룬bo_Ϊλƫ��  
#define bm_scb_hfsr_DEBUGEVT    (1<<31)
#define bm_scb_hfsr_FORCED      (1<<30)
#define bm_scb_hfsr_VECTBL      (1<<1) 
#define bo_scb_hfsr_DEBUGEVT    31  //r/Wc,������¼�������
#define bo_scb_hfsr_FORCED      30  //r/Wc,������ fault���洢������
                                    //    fault �����÷� fault �ϷõĽ��
#define bo_scb_hfsr_VECTBL      1    //r/Wc,��ȡ����ʱ������
struct scb_reg
{
    vuc32 CPUID;
    vu32 ICSR;          //�жϿ��Ƽ�״̬�Ĵ�����0xE000_ED04
    vu32 VTOR;          //������ƫ�����Ĵ�����0xE000_ED08
    vu32 AIRCR;         //Ӧ�ó����жϼ���λ���ƼĴ�����0xE000_ED0c
    vu32 SCR;           //ϵͳ���ƼĴ���,0xE000_ED10
    vu32 CCR;           //��������ƼĴ���,0xE000_ED14
    vu8 mmf_pri4;       //�洢������ fault �����ȼ�,0xE000_ED18
    vu8 bus_pri5;       //���� fault �����ȼ�,0xE000_ED19
    vu8 usr_pri6;       //�÷� fault �����ȼ�,0xE000_ED1a
    vu8 res1[4];
    vu8 svc_pri11;      //svc �쳣�����ȼ�,0xE000_ED1f
    vu8 dbg_pri12;      //���Լ����������ȼ�,0xE000_ED20
    vu8 res2[1];
    vu8 pendsv_pri14;   //pendsv �쳣�����ȼ�,0xE000_ED22
    vu8 systick_pri15;  //systick�����ȼ�,0xE000_ED23
    vu32 SHCSR;
    vu8 MFSR;           //�洢������ fault ״̬�Ĵ���(MFSR),0xE000_ED28
    vu8 BFSR;           //���� fault ״̬�Ĵ���(MFSR),0xE000_ED29
    vu16 UFSR;          //�÷� fault ״̬�Ĵ���(UFSR)����ַ��0xE000_ED2A
    vu32 HFSR;          //Ӳ fault ״̬�Ĵ���      0xE000_ED2C
    vu32 DFSR;          //���� fault ״̬�Ĵ���(DFSR)    0xE000_ED30
    vu32 MMFAR;         //�����洢����fault�ĵ�ַ�Ĵ���(MMAR)    0xE000_ED34
    vu32 BFAR;          //��������fault�ĵ�ַ�Ĵ���(MMAR)    0xE000_ED38
    vu32 AFAR;          //��оƬ�����̾�������ѡ��    0xE000_ED3C
};

#define STIR_reg_addr   0xe000ef00

extern struct scb_reg volatile * const pg_scb_reg;

#ifdef __cplusplus
}
#endif

#endif /*__CM3_H__*/


