//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//��Ȩ���� (C), 2004-2009,   ������.
//����ģ��:���豸����ģ��
//����:  ������.
//�汾��V1.0.0
//�ļ�����:�ṩ���豸������
//����˵��:
//�޶���ʷ:
//2. ����:2009-03-03
//   ����:  ������.
//   �°汾�ţ�1.0.1
//   �޸�˵��: ������dev_close_left������dev_close_right�����Ĵ��󣬸�bug��
//             ����sniper�ύ
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "os_inc.h"

#if ((cfg_pan_driver == 1) && (cn_run_mode != cn_mode_tiny))

#include <string.h>

static struct  pan_device *pg_device_rsc_tree;

static struct pan_device tg_mem_of_device[cn_device_limit];//���豸���ƿ��ڴ��
static struct mem_cell_pool *pg_pan_device_pool;     //�豸���ƿ��ڴ��ͷָ��
static struct mutex_LCB tg_dev_mutex;       //�����豸��Դ���Ĳ������ʰ�ȫ

//----��ʼ�����豸����---------------------------------------------------------
//���ܣ�1.����Դ��������������Ϊ"dev"�ĸ����.
//      2.��ʼ�����豸���ƿ��ڴ��
//      3.��ʼ�����豸����ڴ��
//��������
//���أ�1
//-----------------------------------------------------------------------------
ptu32_t module_init_driver(ptu32_t para)
{
    static struct  pan_device root;

    pg_device_rsc_tree = (struct  pan_device *)
            rsc_add_tree_root(&root.node,sizeof(struct  pan_device),"dev");
    //��ʼ�����豸���ƿ��ڴ��
    pg_pan_device_pool = mb_create_pool((void*)tg_mem_of_device,
                                    cn_device_limit,
                                    sizeof(struct  pan_device),
                                    0,0,
                                    "���豸���ƿ��");
    mutex_create_knl(&tg_dev_mutex,"device tree");
    return 1;
}

//----����豸-----------------------------------------------------------------
//����: ���豸���뵽�ֵ��豸��β��,
//����: parent_device,������豸�ĸ��豸
//      name�����豸�����֣���ָ����ַ����ڴ��������Ǿֲ�������Ҳ�����ǿ�
//      right_semp���豸���ֽӿ��ź���ָ��
//      left_semp���豸���ֽӿ��ź���ָ��
//      right_write������д����ָ��
//      right_read�����ֶ�����ָ��
//      right_ctrl�����ֿ��ƺ���ָ��
//      left_write������д����ָ��
//      left_read�����ֶ�����ָ��
//      left_ctrl�����ֿ��ƺ���ָ��
//      tag,��Ӧ�豸������������һ��������ָ����豸�������ݽṹ
//����:�¼�����豸����Դ���ָ��.��Դ�����򷵻�NULL
//�������ɷ��豸driver���ߵ��ã�������Ӧ�ó���ʹ�ã��ʲ�ʹ��djy_handle_t
//������struct  pan_device���Ͳ���
//-----------------------------------------------------------------------------
struct pan_device * dev_add_device(struct pan_device * parent_device,
                                   char               *name,
                                   struct semaphore_LCB   *right_semp,
                                   struct semaphore_LCB   *left_semp,
                                   dev_write_func     right_write ,
                                   dev_read_func      right_read,
                                   dev_ctrl_func      right_ctrl ,
                                   dev_write_func     left_write ,
                                   dev_read_func      left_read ,
                                   dev_ctrl_func      left_ctrl ,
                                   ptu32_t            tag)
{
    struct  pan_device  *new_device,*result;
    if((parent_device == NULL) || (name == NULL))
        return NULL;        //�豸����û������
    if(strchr(name,'\\'))   //�����в��ܰ����ַ� \.
        return NULL;
    mutex_pend(&tg_dev_mutex,cn_timeout_forever);
    if(rsc_search_son(&parent_device->node,name) != NULL)
    {
        djy_error_login(enum_drv_homonymy,"�豸����");
        result = NULL;
    }else
    {
        if(parent_device != NULL)
        {
            //���䷺�豸���ƿ�����豸
            new_device = mb_malloc(pg_pan_device_pool,0);
            if(new_device != NULL)
            {
                rsc_add_son(&parent_device->node,&new_device->node,
                                        sizeof(struct  pan_device),name);
                new_device->left_semp = left_semp;
                new_device->right_semp = right_semp;
                if(right_write != NULL)
                    new_device->right_io.io_write = right_write;
                else
                    new_device->right_io.io_write=(dev_write_func)NULL_func;
                if(right_read != NULL)
                    new_device->right_io.io_read = right_read;
                else
                    new_device->right_io.io_read = (dev_read_func)NULL_func;
                if(right_ctrl != NULL)
                    new_device->right_io.io_ctrl = right_ctrl;
                else
                    new_device->right_io.io_ctrl = (dev_ctrl_func)NULL_func;
                if(left_write != NULL)
                    new_device->left_io.io_write = left_write;
                else
                    new_device->left_io.io_write =(dev_write_func)NULL_func;
                if(left_read != NULL)
                    new_device->left_io.io_read = left_read;
                else
                    new_device->left_io.io_read = (dev_read_func)NULL_func;
                if(left_ctrl != NULL)
                    new_device->left_io.io_ctrl = left_ctrl;
                else
                    new_device->left_io.io_ctrl = (dev_ctrl_func)NULL_func;
                new_device->private_tag = tag;
                result = new_device;
            }else
            {
                djy_error_login(enum_mem_tried,"�ڴ治��");
                result = NULL;
            }
        }else
            result = NULL;
    }
    mutex_post(&tg_dev_mutex);
    return result;
}

//----ж���豸------------------------------------------------------------------
//����: ���豸���豸������ɾ��,���ͷ���ռ�õ��ڴ�.���豸�������������:
//      1.���豸û�����豸.
//      2.����ʹ�ø��豸���û���Ϊ0.
//����: device,���ͷŵ��豸
//����: �ɹ��ͷŷ���true,���򷵻�false
//��Գƺ���dev_add_deviceһ�����������ɷ��豸driver���ߵ��ã�������Ӧ�ó���
//ʹ�ã��ʲ�ʹ��struct  dev_handle������struct  pan_device���Ͳ���
//-----------------------------------------------------------------------------
bool_t dev_delete_device(struct pan_device * device)
{
    struct  rsc_node *node;
    bool_t en_del = true;
    mutex_pend(&tg_dev_mutex,cn_timeout_forever);
    if(device->left_semp != NULL)
        if(semp_query_used(device->left_semp) != 0)
            en_del = false;
    if(device->right_semp != NULL)
        if(semp_query_used(device->right_semp) != 0)
            en_del = false;
    if(en_del)
    {
        node = &device->node;
        if(rsc_del_node(node) != NULL)
        {
            mutex_post(&tg_dev_mutex);
            mb_free(pg_pan_device_pool,device);
            return(true);           //�ɹ�ж���豸
        }
    }
    mutex_post(&tg_dev_mutex);
    djy_error_login(enum_drv_dev_del,NULL);
    return false;
}

//----ȡ���豸ָ��-------------------------------------------------------------
//����: ��ȡ���豸ID��ʵ���Ͼ����豸��pan_deviceָ�룬�������򿪣���ʵ�ϸ��豸Ҳ
//      �޷��򿪡�
//����: ��
//����: ���豸�ķ��豸ָ��
//-----------------------------------------------------------------------------
struct pan_device *dev_get_root_device(void)
{
    return (djy_handle_t)pg_device_rsc_tree;
}

//----��ѯ�豸-----------------------------------------------------------------
//����: ��ѯ�豸�������ڷ����豸��ID�����������豸���������򷵻�0
//����: name���豸���֣�����·�����������ذ���'dev\'��4���ַ�
//����: �豸�����򷵻�ID���������򷵻�0
//-----------------------------------------------------------------------------
device_id_t dev_check_device(char * name)
{
    struct  pan_device     *pan = NULL;
    if( ! mutex_pend(&tg_dev_mutex,0))    //���Ǳ����豸���Ļ�����
        return NULL;
    //���豸���������豸
    pan = (struct  pan_device *)rsc_search(&pg_device_rsc_tree->node,name);
    mutex_post(&tg_dev_mutex);
    return (djy_handle_t)pan;
}

//----��ѯ����豸-------------------------------------------------------------
//����: ĳ�豸�ĺ���豸�У���ѯ����Ϊscion_name���豸�Ƿ���ڣ������򷵻�����
//      ��ID���������򿪣��������򷵻�0.
//����: ancestor������ѯ�豸�������豸��
//      scion_name,�豸���ַ���,����·����,
//����: �豸�����򷵻�ID���������򷵻�0
//-----------------------------------------------------------------------------
device_id_t dev_check_device_scion(djy_handle_t ancestor,
                                           char * scion_name)
{
    struct  pan_device *pan_scion;
    if(ancestor == NULL)
        return NULL;
    if( ! mutex_pend(&tg_dev_mutex,0))    //���Ǳ����豸���Ļ�����
        return NULL;
    //���豸���������豸
    pan_scion = (struct pan_device*)rsc_search(&ancestor->node,scion_name);

    mutex_post(&tg_dev_mutex);
    return (djy_handle_t)pan_scion;
}

//----���豸���ֽӿ�---------------------------------------------------------
//����: �����豸�����豸�����ֽӿڣ����������豸��Դ�����ҵ�������nameƥ���
//      ��Դ��㣬Ȼ��ӷ��豸������з�����ƿ�,�����ֽӿ�ָ�븳ֵ�������,
//      ���ظþ��ָ�롣
//����: name,�豸���ַ���,����·�����������ذ���'dev\'��4���ַ�
//      timeout����ʱ����,��λ��΢�룬cn_timeout_forever=���޵ȴ���0��������
//      ��ʱ���ء���0ֵ�������ϵ���Ϊcn_tick_us��������
//����: �ɹ����豸(�������ȴ����)�����豸���,���򷵻�NULL.
//-----------------------------------------------------------------------------
djy_handle_t dev_open_left(char *name,u32 timeout)
{
    struct  pan_device     *pan;
    if( ! mutex_pend(&tg_dev_mutex,timeout))    //���Ǳ����豸���Ļ�����
        return NULL;
    //���豸���������豸
    pan = (struct  pan_device *)rsc_search(&pg_device_rsc_tree->node,name);
    if(pan != NULL)     //���û���ҵ�name�豸,���ؿ�
    {
        if(semp_pend(pan->left_semp,timeout)==false)//��ȡ�ź���,���Ǳ����豸��
            pan = NULL;
    }

    mutex_post(&tg_dev_mutex);
    return (djy_handle_t)pan;
}

//----�򿪺���豸���ֽӿ�-----------------------------------------------------
//����: �򿪺���豸�����ֽӿڣ�����ancestor�豸���������豸�����ҵ�������
//      scion_nameƥ�����Դ��㣬Ȼ��ӷ��豸������з�����ƿ�,�����ֽӿ�ָ��
//      ��ֵ�������,���ظþ��ָ�롣
//����: ancestor�������豸�������豸��
//      scion_name,�豸���ַ���,����·����,
//      timeout����ʱ����,��λ��΢�룬cn_timeout_forever=���޵ȴ���0��������
//      ��ʱ���ء���0ֵ�������ϵ���Ϊcn_tick_us��������
//����: �ɹ����豸(�������������)�����豸���,���򷵻�NULL.
//-----------------------------------------------------------------------------
djy_handle_t dev_open_left_scion(djy_handle_t ancestor,
                                      char *scion_name, u32 timeout)
{
    struct  pan_device     *pan_scion;
    if(ancestor == NULL)
        return NULL;
    if( ! mutex_pend(&tg_dev_mutex,timeout))    //���Ǳ����豸���Ļ�����
        return NULL;
    //���豸���������豸
    pan_scion = (struct pan_device*)rsc_search(&ancestor->node,scion_name);

    if(pan_scion != NULL)     //���û���ҵ�name�豸,���ؿ�
    {
        if(semp_pend(pan_scion->left_semp,timeout)==false)//��ȡ�����豸���ź���
            pan_scion = NULL;
    }

    mutex_post(&tg_dev_mutex);
    return (djy_handle_t)pan_scion;
}

//----���ٴ��豸���ֽӿ�-----------------------------------------------------
//����: ���ٴ��豸���ֽӿ�,���豸IDΪ���������ð��豸�������豸,�ٶȺܿ졣����
//      ����dev_check_device��dev_check_device_scion�����Լ�dev_close_xxx������
//      ��ʹ�ã���ʵ���豸���ٴ򿪣���ʱ�رգ�������ռ���豸��
//����: device_id,���򿪵��豸id
//      timeout����ʱ����,��λ��΢�룬cn_timeout_forever=���޵ȴ���0��������
//      ��ʱ���ء���0ֵ�������ϵ���Ϊcn_tick_us��������
//����: �ɹ����豸�򷵻ؾ��,���򷵻�NULL.
//-----------------------------------------------------------------------------
djy_handle_t dev_open_left_fast(device_id_t device_id,u32 timeout)
{
    if (device_id == NULL)                                 //�����
        return NULL;
    if(semp_pend(device_id->left_semp,timeout)==false)     //��ȡ�ź���
    {
        return NULL;
    }else
    {
        return device_id;
    }
}

//----���豸���ֽӿ�---------------------------------------------------------
//����: �����豸�����豸�����ֽӿڣ����������豸��Դ�����ҵ�������nameƥ���
//      ��Դ��㣬Ȼ��ӷ��豸������з�����ƿ�,�����ֽӿ�ָ�븳ֵ�������,
//      ���ظþ��ָ�롣
//����: name,�豸���ַ���,����·��������������'dev\'��4���ַ�
//      timeout����ʱ����,��λ��΢�룬cn_timeout_forever=���޵ȴ���0��������
//      ��ʱ���ء���0ֵ�������ϵ���Ϊcn_tick_us��������
//����: �ɹ����豸�����豸���,���򷵻�NULL.
//-----------------------------------------------------------------------------
djy_handle_t dev_open_right(char *name,u32 timeout)
{
    struct  pan_device     *pan;
    if( ! mutex_pend(&tg_dev_mutex,timeout))    //���Ǳ����豸���Ļ�����
        return NULL;
    //���豸���������豸
    pan = (struct  pan_device *)rsc_search(&pg_device_rsc_tree->node,name);
    if(pan != NULL)     //���û���ҵ�name�豸,���ؿ�
    {
        if(semp_pend(pan->right_semp,timeout)==false)//��ȡ�ź���,���Ǳ����豸��
            pan = NULL;
    }

    mutex_post(&tg_dev_mutex);
    return (djy_handle_t)pan;
}

//----�򿪺���豸���ֽӿ�-----------------------------------------------------
//����: �򿪺���豸�����ֽӿڣ�����ancestor�豸���������豸�����ҵ�������
//      scion_nameƥ�����Դ��㣬Ȼ��ӷ��豸������з�����ƿ�,�����ֽӿ�ָ��
//      ��ֵ�������,���ظþ��ָ�롣
//����: ancestor�������豸�������豸��
//      scion_name,�豸���ַ���,����·����,
//      timeout����ʱ����,��λ��΢�룬cn_timeout_forever=���޵ȴ���0��������
//      ��ʱ���ء���0ֵ�������ϵ���Ϊcn_tick_us��������
//����: �ɹ����豸(�������������)�����豸���,���򷵻�NULL.
//-----------------------------------------------------------------------------
djy_handle_t dev_open_right_scion(djy_handle_t ancestor,
                                      char *scion_name, u32 timeout)
{
    struct  pan_device     *pan_scion;
    if(ancestor == NULL)
        return NULL;
    if( ! mutex_pend(&tg_dev_mutex,timeout))    //���Ǳ����豸���Ļ�����
        return NULL;
    //���豸���������豸
    pan_scion = (struct pan_device*)rsc_search(&ancestor->node,scion_name);

    if(pan_scion != NULL)     //���û���ҵ�name�豸,���ؿ�
    {
        if(semp_pend(pan_scion->right_semp,timeout)==false)//��ȡ�����豸���ź���
            pan_scion = NULL;
    }

    mutex_post(&tg_dev_mutex);
    return (djy_handle_t)pan_scion;
}

//----���ٴ��豸���ֽӿ�-----------------------------------------------------
//����: ���ٴ��豸���ֽӿ�,���豸IDΪ���������ð��豸�������豸,�ٶȺܿ졣����
//      ����dev_check_device��dev_check_device_scion�����Լ�dev_close_xxx������
//      ��ʹ�ã���ʵ���豸���ٴ򿪣���ʱ�رգ�������ռ���豸��
//����: device_id,���򿪵��豸id
//      timeout����ʱ����,��λ��΢�룬cn_timeout_forever=���޵ȴ���0��������
//      ��ʱ���ء���0ֵ�������ϵ���Ϊcn_tick_us��������
//����: �ɹ����豸�򷵻ؾ��,���򷵻�NULL.
//-----------------------------------------------------------------------------
djy_handle_t dev_open_right_fast(device_id_t device_id,u32 timeout)
{
    if (device_id == NULL)                                 //�����
        return NULL;
    if(semp_pend(device_id->right_semp,timeout)==false)     //��ȡ�ź���
    {
        return NULL;
    }else
    {
        return device_id;
    }
}

//----�ر��豸���ֽӿ�---------------------------------------------------------
//����: �ر��豸���ֽӿ�
//����: handle,���رյ��豸���
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t dev_close_left(djy_handle_t handle)
{
    if (handle == NULL)
        return false;
    semp_post(handle->left_semp);
    return true;
}

//----�ر��豸���ֽӿ�---------------------------------------------------------
//����: �ر��豸���ֽӿ�
//����: handle,���رյ��豸���
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t dev_close_right(djy_handle_t handle)
{
    if (handle == NULL)
        return false;
    semp_post(handle->right_semp);
    return true;
}

//----���ֶ��豸����-----------------------------------------------------------
//����: �����ֲ�����豸�Ķ�������ȡ����.
//����: left_handle,�豸���־��
//      ��������:���豸�����߶���
//����: ���豸�����߶��壬�Ƽ���ʵ�ʶ�ȡ�����ݳ���.
//-----------------------------------------------------------------------------
ptu32_t dev_read_left(djy_handle_t left_handle,ptu32_t src_buf,
                ptu32_t des_buf,ptu32_t len)
{
    if (left_handle == NULL)
        return 0;
    else
    {
        return (left_handle->left_io.io_read(left_handle,src_buf,des_buf,len));
    }
}

//----���ֶ��豸����-----------------------------------------------------------
//����: �����ֲ�����豸�Ķ�������ȡ����.
//����: right_handle,�豸���־��
//      ��������:���豸�����߶���
//����: ���豸�����߶��壬�Ƽ���ʵ�ʶ�ȡ�����ݳ���.
//-----------------------------------------------------------------------------
ptu32_t dev_read_right(djy_handle_t right_handle,ptu32_t src_buf,
                ptu32_t des_buf,ptu32_t len)
{
    if (right_handle == NULL)
        return 0;
    else
    {
        return (right_handle->right_io.io_read(right_handle,src_buf,des_buf,len));
    }
}

//----����д�豸����-----------------------------------------------------------
//����: �����ֲ�����豸��д����д������.
//����: left_handle,�豸���־��
//      ��������:���豸�����߶���
//����: ���豸�����߶��壬�Ƽ���ʵ��д������ݳ���.
//-----------------------------------------------------------------------------
ptu32_t dev_write_left(djy_handle_t left_handle,ptu32_t src_buf,
                ptu32_t des_buf,ptu32_t len)
{
    if (left_handle == NULL)
        return 0;
    else
    {
        return (left_handle->left_io.io_write(left_handle,src_buf,des_buf,len));
    }
}

//----����д�豸����-----------------------------------------------------------
//����: �����ֲ�����豸��д����д������.
//����: right_handle,�豸���־��
//      ��������:���豸�����߶���
//����: ���豸�����߶��壬�Ƽ���ʵ��д������ݳ���.
//-----------------------------------------------------------------------------
ptu32_t dev_write_right(djy_handle_t right_handle,ptu32_t src_buf,
                ptu32_t des_buf,ptu32_t len)
{
    if (right_handle == NULL)
        return 0;
    else
    {
        return (right_handle->right_io.io_write(right_handle,src_buf,des_buf,len));
    }
}

//----���ֿ����豸����---------------------------------------------------------
//����: �����ֲ�����豸�Ŀ��ƺ���.
//����: left_handle,�豸���
//      ��������:���豸�����߶���
//����: ���豸�����߶���
//-----------------------------------------------------------------------------
ptu32_t dev_ctrl_left(djy_handle_t left_handle,u32 cmd,
                    ptu32_t data1,ptu32_t data2)
{
    if (left_handle == NULL)
        return 0;
    else
    {
        return (left_handle->left_io.io_ctrl(left_handle,cmd,data1,data2));
    }
}

//----���ֿ����豸����---------------------------------------------------------
//����: �����ֲ�����豸�Ŀ��ƺ���.
//����: right_handle,�豸���־��
//      ��������:���豸�����߶���
//����: ���豸�����߶���
//-----------------------------------------------------------------------------
ptu32_t dev_ctrl_right(djy_handle_t right_handle,u32 cmd,
                    ptu32_t data1,ptu32_t data2)
{
    if (right_handle == NULL)
        return 0;
    else
    {
        return (right_handle->right_io.io_ctrl(right_handle,cmd,data1,data2));
    }
}

//----��ȡ�豸��ǩ-------------------------------------------------------------
//����: ÿ���豸����һ����ǩ���ñ�ǩһ����һ��ָ���������豸�����ݽṹָ�롣����
//      ��һ�����豸����������ã����豸������������ֻ֪���Լ��ķ��豸ָ�룬����
//      ֪���������˱�������struct pan_deviceΪ����
//����: pan,�豸ָ�룬ע�ⲻ�Ǿ��
//����: �豸��ǩ
//-----------------------------------------------------------------------------
ptu32_t dev_get_my_tag(struct pan_device * pan)
{
    if (pan == NULL)
        return 0;
    else
    {
        return pan->private_tag;
    }
}


//----��ȡ�豸ָ��-------------------------------------------------------------
//����: ͨ���豸handle��������豸�ķ��豸ָ�롣
//����: handle,�豸���
//����: ���豸�豸ָ��
//-----------------------------------------------------------------------------
struct pan_device *dev_get_panp(djy_handle_t handle)
{
    return (struct pan_device *)handle;
};

#endif  //#if ((cfg_pan_driver == 1) && (cn_run_mode != cn_mode_tiny))


