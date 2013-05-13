//----------------------------------------------------
//Copyright (C), 2005-2009,  lst.
//��Ȩ���� (C), 2005-2009,   lst.
//����ģ��:  ����
//���ߣ�     lst
//�汾��      V1.0.0
//��ʼ�汾������ڣ�2009-08-05
//�ļ�����:  cortex-m0�Ĵ�����ͷ�ļ�
//����˵��:  ��
//�޶���ʷ:
//2. ...
//1. ����: 2009-08-05
//   ����: lst
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------

#ifndef __CORTEXM0_H__
#define __CORTEXM0_H__

#ifdef __cplusplus
extern "C" {
#endif

//����cmsis�汾�ţ�cmsis��arm��˾�����һ�����ڷ���cpu��Դ�ı�׼��������
#define cn_cm0_cmsis_version_main  (0x01)
#define cn_cm0_cmsis_version_sub   (0x30)
#define cn_cm0_cmsis_version       ((cn_cm0_cmsis_version_main << 16) | cn_cm0_cmsis_version_sub) 

#define __cortex_m                (0x00)

#ifdef  __ICCARM__ 
  #include <intrinsics.h> 
#endif


#ifndef __NVIC_PRIO_BITS
  #define __NVIC_PRIO_BITS    2
#endif

typedef enum IRQn
{
  NonMaskableInt_IRQn           = -14,      /*!<  2 Non Maskable Interrupt                        */
  HardFault_IRQn                = -13,      /*!<  3 Hard Fault Interrupt                          */
  SVCall_IRQn                   = -5,       /*!< 11 SV Call Interrupt                             */
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
#define bo_systick_ctrl_tickint     1    //rw,1=SysTick ����0ʱ����SysTick �쳣����
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
#define bm_scb_icsr_nmipendset  (1<<31)     //rw д1����nmi,��1=nmi����
#define bm_scb_icsr_pendsvset   (1<<28)     //rw д1����pendsv�������ع���״̬
#define bm_scb_icsr_pendsvclr   (1<<27)     //w д1���pendsv����״̬
#define bm_scb_icsr_pendstset   (1<<26)     //rw д1����systick�������ع���״̬
#define bm_scb_icsr_pendstclr   (1<<25)     //w д1���systick����״̬
#define bm_scb_icsr_isrpending  (1<<22)     //r ���жϹ��𣨲���nmi��
#define bm_scb_icsr_vectpending (0x3f<<12)  //r ������ж������ȼ���ߵ��жϺ�
#define bm_scb_icsr_vectactive  (0x3f<<0)   //r ���ڷ�����쳣��
#define bo_scb_icsr_nmipendset  31    
#define bo_scb_icsr_pendsvset   28    
#define bo_scb_icsr_pendsvclr   27    
#define bo_scb_icsr_pendstset   26    
#define bo_scb_icsr_pendstclr   25    
#define bo_scb_icsr_isrpending  22    
#define bo_scb_icsr_vectpending 12
#define bo_scb_icsr_vectactive  0  

//AIRCR Ӧ�ó����жϼ���λ���ƼĴ�����0xE000_ED0c ,bm_Ϊλ���룬bo_Ϊλƫ��  
#define bm_scb_aircr_vectkey        (0xffff<<16)
#define bm_scb_aircr_endianess      (1<<15)     
#define bm_scb_aircr_sysresetreq    (1<<2)      
#define bm_scb_aircr_vectclractive  (1<<1)      
#define bo_scb_aircr_vectkey        16  //rw д���Ĵ���������ͬʱ�Դ���д0x05fa
#define bo_scb_aircr_endianess      15  //r 1=��ˣ�0=С��
#define bo_scb_aircr_sysresetreq    2   //w д1�߼���λоƬ
#define bo_scb_aircr_vectclractive  1   //w �������ڵ���

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
#define bm_scb_ccr_unaligntrp       (1<<3) 
#define bo_scb_ccr_stkalign         9   //rw �쳣��Ӧ�Զ���ջʱǿ��dp˫�ֶ���
#define bo_scb_ccr_unaligntrp       3   //rw 1=δ������ʽ������÷�fault

//�ر�ע��:m0��scb�Ĵ�����m3��ͬ��m3�����ֽڷ��ʣ���m0�����ֽڷ���
struct scb_reg
{
    vuc32 CPUID;
    vu32 ICSR;          //�жϿ��Ƽ�״̬�Ĵ�����0xE000_ED04
    vu32 res0;
    vu32 AIRCR;         //Ӧ�ó����жϼ���λ���ƼĴ�����0xE000_ED0c
    vu32 SCR;           //ϵͳ���ƼĴ���,0xE000_ED10
    vu32 CCR;           //��������ƼĴ���,0xE000_ED14
    vu32 res1;
    vu32 pri8_11;       //�쳣�����ȼ�,0xE000_ED1c,pri11=svc
    vu32 pri12_15;      //�쳣�����ȼ�,0xE000_ED20��pri14=pendsv,pri15=systick
    vu32 SHCSR;         //datasheet���ޣ���nxp�Ĵ�������?
    vu32 res4[2];
    vu32 DFSR;          //���� fault ״̬�Ĵ���(DFSR) 0xE000_ED30
                        //datasheet���ޣ���nxp�Ĵ�������?
};

#ifdef __cplusplus
}
#endif

#endif /*__CORTEXM0_H__*/


