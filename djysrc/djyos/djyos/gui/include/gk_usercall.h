//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//��Ȩ���� (C), 2004-2009,   ������.
//����ģ��: gui
//����:  ������.
//�汾��V1.0.0
//�ļ�����: gui kernel��user(ͨ����gui window)���Ͳ�������
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-11-21
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#if (cfg_gui_kernel == 1)

#ifndef __GK_USERCALL_H__
#define __GK_USERCALL_H__

#ifdef __cplusplus
extern "C" {
#endif

//gui kernel��user call����,���ϲ�ͨ���ܵ����ݸ�gui kernel
#define cn_gkuc_repaint         0x000   //�ػ洰��

struct gkuc_para_repaint
{
    void *win;               //���Ƶ�Ŀ�괰��
    struct clip_rect *redraw_clip;  //���ػ�ļ���������
};

#ifdef __cplusplus
}
#endif

#endif //__GK_USERCALL_H__

#endif //   for #if (cfg_gui_kernel == 1)

