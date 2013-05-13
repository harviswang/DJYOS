//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//��Ȩ���� (C), 2004-2009,   lst.
//����ģ��: ���Ź�ģ��
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����: ���Ź�ģ��
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __wdt_h__
#define __wdt_h__

#ifdef __cplusplus
extern "C" {
#endif

#define cn_wdt_action_none          0 //û���ж�(һ����yip_remedy�����Ѿ��㹻�����)
#define cn_wdt_action_reboot        1 //��λ�����
#define cn_wdt_action_hard_reset    2


//���Ź��ṹ
struct wdt_rsc
{
	struct rsc_node wdt_node;
    struct semaphore_LCB wdt_semp;
    bool_t (* judge)(void);         //�û��ṩ���жϹ��еĺ���
    u32 (* yip_remedy)(void);  //���к�Ĳ��ȴ�ʩ���û��ṩ
    u32 timeout;               //����judge��ʱ����
};


//������Ϣ����
enum wdt_error_code
{
    enum_wdt_no_error = cn_wdt_no_error,  //û�д���
    enum_wdt_yip,                         //����
};

ptu32_t module_init_wdt(ptu32_t para);
struct wdt_rsc * wdt_create(bool_t (*judge)(void),
                  u32 (*yip_remedy)(void),
                  u32 timeout,char *wdt_name);
void wdt_check(void);

#ifdef __cplusplus
}
#endif

#endif //__wdt_h__

