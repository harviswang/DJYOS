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
#include "os_inc.h"

#if (cfg_stdin_device_limit != 0)

//�¼����ͣ�����δ���ý���������豸��Ĭ���Դ�Ϊ����
u16 u16g_default_focus = cn_invalid_evtt_id;
struct stdin_device_rsc stdin_device[cfg_stdin_device_limit];
struct mem_cell_pool *pg_stdin_device_pool;
s32 s32g_next_id = 0;       //ÿ�ΰ�װ�����豸ʱ���Ա�����Ϊ�豸ID��Ȼ�󱾱���++
                            //ɾ���豸��ID���ջء�
static struct  stdin_device_rsc *pg_stdin_device_rsc_tree;

//----��ʼ����׼����ģ��-------------------------------------------------------
//����: ��ʼ����׼����ģ�飬ִ�г�ʼ���󣬿��Դ��������豸��
//����: ��
//����: ���Ƿ���1
//-----------------------------------------------------------------------------
ptu32_t module_init_stdin(ptu32_t para)
{
    //��������ɵĹ���:
    //����"input device"��Դ
    //����pg_stdin_device_pool�ڴ��
    static struct  stdin_device_rsc root;

    pg_stdin_device_rsc_tree = (struct  stdin_device_rsc *)
                        rsc_add_tree_root(&root.stdin_device_node,
                        sizeof(struct  stdin_device_rsc),"stdin input device");
    //��ʼ�����豸���ƿ��ڴ��
    pg_stdin_device_pool = mb_create_pool((void*)stdin_device,
                                    cfg_stdin_device_limit,
                                    sizeof(struct  stdin_device_rsc),
                                    0,0,
                                    "�����豸���ƿ��ڴ��");

    return 1;
}

//----��װ�����豸-------------------------------------------------------------
//����: �������豸�Ǽǵ���Դ�����
//����: device_name��Ŀ���豸����Դ��
//����: �����豸ID,-1��ʾʧ��
//-----------------------------------------------------------------------------
s32 stdin_install_device(char *device_name,enum stdin_input_type stdin_type)
{
    struct stdin_device_rsc *stdin;

    if(rsc_search_son(&pg_stdin_device_rsc_tree->stdin_device_node,device_name))
    {
        return -1;
    }
    else
    {
        stdin = mb_malloc(pg_stdin_device_pool,0);
        if(stdin != NULL)
        {
            rsc_add_son(&pg_stdin_device_rsc_tree->stdin_device_node,
                    &stdin->stdin_device_node,sizeof(struct stdin_device_rsc),
                    device_name);
            stdin->input_type = stdin_type;
            stdin->input_counter = 0;
            stdin->focus_evtt = u16g_default_focus;
            s32g_next_id += 1;
            stdin->device_id = s32g_next_id;
            return stdin->device_id;
        }
        else
        {
            djy_error_login(enum_mem_tried,"�ڴ治��");
            return -1;
        }

    }
}

//----�������뽹��---------------------------------------------------------
//����: ���������豸�����뽹�㣬����������Ϣ(������̻����������)����˭
//����: device_name��Ŀ���豸����Դ��
//      focus_evtt���µĽ���
//����: true =�ɹ���false=�����豸�Ҳ���
//-----------------------------------------------------------------------------
bool_t stdin_set_focus(char *device_name,u16 focus_evtt)
{
    struct stdin_device_rsc *result;

    result =
        (struct stdin_device_rsc *)rsc_search_son
        (&pg_stdin_device_rsc_tree->stdin_device_node,device_name);
    if(result != NULL)
    {
        result->focus_evtt = focus_evtt;
        return true;
    }
    else
    {
        return false;
    }
}

//----����Ĭ�����뽹��---------------------------------------------------------
//����: ���������豸�����н��㣬���豸����������ʱ�����ĸ��¼����ͷ���Ϣ��δ�趨
//      ������豸��ʹ��Ĭ�Ͻ��㣬���Ĭ�Ͻ�����cn_invalid_evtt_id����δ���ý���
//      ���豸��û�н���
//����: focus_evtt��Ĭ�Ͻ��㣬
//����: ��
//-----------------------------------------------------------------------------
void stdin_set_focus_default(u16 focus_evtt)
{
    u16g_default_focus = focus_evtt;
}

//----���������豸���뽹��-----------------------------------------------------
//����: �����������豸�Ľ�����Ϊָ��ֵ
//����: focus_evtt���µĽ��㽹��
//����: ��
//-----------------------------------------------------------------------------
void stdin_set_focus_all(u16 focus_evtt)
{
    //������������������豸����"input device"��Դ��������Դ���������ǵ�focus_evtt
    struct  rsc_node  *current,*start,*target;
    start = &pg_stdin_device_rsc_tree->stdin_device_node;
    current = &pg_stdin_device_rsc_tree->stdin_device_node;
    while((target =
        rsc_trave_scion(start,current))!=NULL)
    {
        ((struct stdin_device_rsc *)target)->focus_evtt = focus_evtt;
        current = target;
    }
}

//----������Ϣ-----------------------------------------------------------------
//����: �����豸����µ������(�����µļ��̻������������)�����ñ����������µ�
//      ��Ϣ����ϵͳ�������¼������in_device->focus_evtt�ѳ�ʼ�������¼�������
//      in_device->focus_evtt�����δ��ʼ�������¼�������u16g_default_focus��
//      ���u16g_default_focusҲû�г�ʼ�����򲻵����¼�
//����: in_device��Ŀ�������豸����Դ�ӵ�
//      msg_data���������������
//      msg_size��������Ϣ�ĳ��ȣ��ֽ���
//����: true=�ɹ���false=ʧ�ܣ�һ������Ϊ�����豸δ��װ��
//-----------------------------------------------------------------------------
bool_t stdin_input_msg(s32 stdin_id,u8 *msg_data, u32 msg_size)
{
    struct  rsc_node  *current,*start,*target;
    struct input_device_msg input_msg;
    u16 evtt;

    start = &pg_stdin_device_rsc_tree->stdin_device_node;
    current = &pg_stdin_device_rsc_tree->stdin_device_node;

    //����Դ�����в���stdin_id��Ӧ�������豸
    while((target = rsc_trave_scion(start,current))!=NULL)
    {
        if(((struct stdin_device_rsc *)target)->device_id == stdin_id)
            break;
        current = target;
    }
    if(target == NULL)        //δ�ҵ������豸
        return false;

    evtt = ((struct stdin_device_rsc *)target)->focus_evtt;
    if(evtt == cn_invalid_evtt_id)      //���豸�����뽹��δ��ʼ��
    {
        evtt = u16g_default_focus;      //ȡĬ�Ͻ���(����Ҳδ��ʼ��)
    }

    if(evtt != cn_invalid_evtt_id)
    {
        input_msg.input_msg = *(union input_device_mem *)msg_data;
        input_msg.input_type = ((struct stdin_device_rsc *)target)->input_type;
        input_msg.device_id = ((struct stdin_device_rsc *)target)->device_id;
        djy_event_pop(evtt,enum_para_async,NULL,0,
                        &input_msg,sizeof(input_msg),0,0);
    }
	return true;
}

//----ж�������豸-------------------------------------------------------------
//����: �������豸����Դ�ӵ�ɾ������"input device"��Դ������Դ�У��ҵ�
//      "device_name"��Դ�ӵ㣬ɾ��֮�������ڴ��ͷŵ�pg_stdin_device_pool
//      �ڴ����
//����: device_name��Ŀ���豸����Դ��
//����: true = �ɹ�ɾ����0=ʧ�ܡ�
//-----------------------------------------------------------------------------
bool_t stdin_uninstall_device(char *device_name)
{
	struct  rsc_node *temp;
    struct stdin_device_rsc *stdin;
	stdin = (struct stdin_device_rsc *)rsc_search_son(
                 &pg_stdin_device_rsc_tree->stdin_device_node,device_name);
    if(stdin == NULL)
        return false;

    temp = &stdin->stdin_device_node;
    if(rsc_del_node(temp) != NULL)
    {
        mb_free(pg_stdin_device_pool,stdin);
        return true;
    }
    else
    	return false;
}

#endif      //for #if (cfg_stdin_device_limit != 0)

