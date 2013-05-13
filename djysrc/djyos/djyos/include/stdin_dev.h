//----------------------------------------------------
//Copyright (C), 2004-2011,  �����߲���ϵͳ�����Ŷ�.
//��Ȩ���� (C), 2004-2011,   �����߲���ϵͳ�����Ŷ�.
//����ģ��:�û�����ģ��
//����:  ������.
//�汾��V1.0.0
//�ļ�����: ��ꡢ���̵������豸����
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2011-01-24
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: �����ļ�
//------------------------------------------------------
#if (cfg_stdin_device_limit != 0)
#ifndef __STDIN_DEV_H__
#define __STDIN_DEV_H__

#ifdef __cplusplus
extern "C" {
#endif

enum stdin_input_type
{
    enum_char_termineral,
    enum_stdin_keyboard,            //����
    enum_stdin_mouse_2d,            //2d���
    enum_stdin_mouse_3d,            //3d���
    enum_stdin_single_touch,        //���㴥����
    enum_stdin_muti_touch,          //��㴥����
    enum_stdin_area_touch,          //��������
};

//�ַ��ն��豸���ݽṹ
struct char_termineral_msg
{
    u32 input_char;         //������ַ�
    u32 time;               //�����ַ���ʱ��,ticks��
};

//������Ϣ���ݽṹ
struct key_board_msg
{
    u8 key_value[3];       //��ֵ,ÿ������3�������
    u32 time;              //�����¼�����ʱ��,ticks��
};

//2ά������ݽṹ����Ŀǰ��õ����
struct mouse_2D_msg
{
    u16 key_no;             //������������
    u32 time;               //����¼�����ʱ��,ticks��
};

//3ά������ݽṹ
struct mouse_3D_msg
{
    u32 time;              //�����¼�����ʱ��,ticks��
};

//���㴥����������õĴ�����
struct single_touch_msg
{
    s32 x,y,z;  //x,y��ʾ����λ�ã�z>0��־����ѹ����0��־δ����
    u32 time;              //�����¼�����ʱ��,ticks��
};

//��㴥����
struct multi_touch_msg
{
    u32 time;              //�����¼�����ʱ��,ticks��
};

//������������ʾ����������һ������
struct area_touch_msg
{
    u32 time;              //�����¼�����ʱ��,ticks��
};

union input_device_mem
{
    struct char_termineral_msg char_termineral;
    struct key_board_msg key_board;
    struct mouse_2D_msg  mouse_2d;
    struct mouse_3D_msg  mouse_3d;
    struct single_touch_msg  single_touch_msg;
    struct multi_touch_msg   muti_touch;
    struct area_touch_msg    area_touch;
};

struct input_device_msg
{
    enum stdin_input_type input_type;         //������Ϣ���ͣ�
    u32 device_id;          //�����豸id
    union input_device_mem input_msg;
};

struct stdin_device_rsc
{
    struct rsc_node stdin_device_node;
    enum stdin_input_type input_type;   //�����豸����
    u32 device_id;                      //�����豸id
    u32 input_counter;                  //�������豸�ۼ������˶�����Ϣ
    u16 focus_evtt;                     //���ձ���Ϣ���¼�����
};

ptu32_t module_init_stdin(ptu32_t para);
s32 stdin_install_device(char *device_name,enum stdin_input_type stdin_type);
bool_t stdin_uninstall_device(char *device_name);
bool_t stdin_set_focus(char *device_name,u16 focus_evtt);
void stdin_set_focus_default(u16 focus_evtt);
void stdin_set_focus_all(u16 focus_evtt);
bool_t stdin_input_msg(s32 stdin_id,u8 *msg_data, u32 msg_size);

#ifdef __cplusplus
}
#endif

#endif //__STDIN_DEV_H__

#endif      //for #if (cfg_stdin_device_limit != 0)

