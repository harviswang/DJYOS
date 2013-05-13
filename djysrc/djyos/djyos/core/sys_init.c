//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//��Ȩ���� (C), 2004-2009,   ������.
//����ģ��: Ӳ����ʱ��
//����:  ������.
//�汾��V1.0.0
//�ļ�����: ϵͳ��ʼ���ļ�
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-04-24
//   ����:  ������.
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾�����ļ�����ԭ���ͼ���������һ��
//------------------------------------------------------
#include "os_inc.h"
#include "exception.h"

ptu32_t init_debug_info(ptu32_t para);
extern void app_init(void);
ptu32_t module_init_shell(ptu32_t para);
ptu32_t module_init_fs_nandflash(ptu32_t para);

//----����ϵͳ�ں������ʼ��---------------------------------------------------
//���ܣ���ʼ������ϵͳ�����������Щ�����OS�������бر��������
//��������
//���أ���
//---------------------------------------------------------------------------
void os_knl_module_init(void)
{
    module_init_rsc1(0);
    module_init_lock1(0);
    module_init_rsc2(0);
    module_init_memb(0);
    module_init_lock2(0);
}



//----����ϵͳ��Χ�����ʼ��---------------------------------------------------
//���ܣ���Χ������ڿɲü��������������������������module_init_modulename����
//      ��ʼ������ģ�飬siģʽ�ڴ˳�ʼ��������Χ�����dlspģʽ��mpģʽ����Χ���
//      ���������Ҳ���ܴ��ļ�����ͨ�ſڼ��غͳ�ʼ����
//��������
//���أ���
//-----------------------------------------------------------------------------
void os_ext_module_init(void)
{
#if (cfg_wdt == 1)
    module_init_wdt(0);
#endif

#if (cfg_pan_driver == 1)
    module_init_driver(0);
#endif

#if (cfg_time_module == 1)
	module_init_datatime(0);
#endif

#if (cfg_djyfs == 1)
    module_init_djyfs(0);
#if (cfg_djyfs_flash == 1)
    module_init_DFFSD(0);
    module_init_fs_nandflash(0);
#endif
#endif

//��ʼ��gui kernelģ��
#if (cfg_gui_kernel == 1)
    module_init_gk(0);
#endif

#if (cfg_stdin_device_limit != 0)
    module_init_stdin( 0 );
#endif

#if (cfg_keyboard == 1)
    module_init_keyboard(0);
#endif

#if(cfg_debug_info == 1)
    init_debug_info(0);
#endif

}

//----dlspģʽ���س���--------------------------------------------------------
//���ܣ���autoexec.bat�ļ�����ִ�У���������ļ�����main��������Ӧ�ó���ִ��
//      ֮�����û��������module_init_modulename������ִ��֮������������û����
//      ֻ���أ���ִ������Ĵ��롣
//����: �ޡ�
//����: �ޡ�
//----------------------------------------------------------------------------
void dlsp_module_loader(void)
{
}
//----ϵͳ��������-----------------------------------------------------
//���ܣ�ִ�в���ϵͳ���أ�ģ��(�豸)��ʼ�����û���ʼ���Լ���������ϵͳ
//��������
//���أ���
//---------------------------------------------------------------------------
void start_sys(void)
{
#if cfg_nude_terminal == 1
    nude_terminal_init();   //�㱼ʽ��������ն˳�ʼ��
    printf_str_nude("\r\n��������...\r\n");
#endif
    //�Դ˿��Ե���malloc�庯������׼��̬�ѷ����㷨����Ҫ�ͷ��ڴ棬Ҫ���ϸ����
    //����malloc-free���������߲�����
    exp_init();
    module_init_heap_static(0);
    __djy_set_delay();
    __djy_init_sys();
    os_knl_module_init();
    //�����￪ʼ��tiny��si��dlsp��mpģʽ������ͬ��ͨ�����ó�������
    os_ext_module_init();
    app_init();

    //�Բ���ϵͳ��˵��shell�Ѿ�����Ӧ�ó��򷶳��ˣ���Ӧ����ʼ��
#if (cfg_shell == 1)
    module_init_shell(0);
    module_init_char_term(0);
#endif
#if (cfg_dynamic_mem == 1)
    module_init_heap_dynamic(0);  //�Դ�malloc����ִ�п������㷨
#endif
    __djy_start_os();
}

