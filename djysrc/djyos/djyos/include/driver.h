//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//��Ȩ���� (C), 2004-2009,   ������.
//����ģ��:���豸����ģ��
//����:  ������.
//�汾��V1.0.0
//�ļ�����:�ṩ���豸������
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __driver_H__
#define __driver_H__

#ifdef __cplusplus
extern "C" {
#endif

#if ((cfg_pan_driver == 1) && (cn_run_mode != cn_mode_tiny))

#define cn_dev_name_limit   255     //�豸�����Ȳ��ܳ���255�ַ�
typedef struct pan_device * djy_handle_t;
typedef struct pan_device * device_id_t;
struct  pan_device;
struct  rsc_node;

//���豸ģ�������룬��enum������enum_drv_no_error��ʼ������1.
enum drv_error_code
{
    enum_drv_no_error = cn_drv_no_error,  //û�д���
    enum_drv_handle_error,                //�������
    enum_drv_homonymy,                    //�豸��������
    enum_drv_dev_del,                   //ɾ���豸ʧ��
};

typedef ptu32_t (*dev_write_func)(struct  pan_device *dev,ptu32_t src_buf,
                                 ptu32_t des_buf,ptu32_t len);
typedef ptu32_t (*dev_read_func) (struct  pan_device *dev,ptu32_t src_buf,
                                 ptu32_t des_buf,ptu32_t len);
typedef ptu32_t (*dev_ctrl_func) (struct  pan_device *dev,u32 cmd,
                                ptu32_t data1,ptu32_t data2);
//���豸����������ṹ,�����ָ�һ��
struct dev_io
{
    //����6���������ָ���������6����io������,����ϵͳ�����޶���������������
    //����,�������豸���ʾ���,���src_buf�ǽṹ����,len�����ǽṹ������,���
    //src_buf��ͨ�Ż�����,��len�������������ݰ�����.��֮,src_buf��len������Ӧ��
    //��driver���߸��ݱ�������Ծ���,����Ķ���ֻ���Ƽ����ʵ�.
    //΢��:src_buf��des_buf�������ptu32_t������ָ��,�Ǵ���ֲ�����Գ������ǵ�.
    //ָ���ǲ�ȷ�����ȵ���������,��������ֲ���������.�����ϵͳ,������16λϵͳ,
    //ָ��Ŀ����16λ��,������24λ��(keilC51�о��п���).��Ӧ�ó���Ҫ�Ѳ�������
    //����ʹ��ʱ,�����ٲ�֪�������ĳ��ȵ�����.������,��64λϵͳ��,���е�ϵͳָ��
    //���ȶ�С��32λ,������ת��û������.
    //����ֵ:���豸���Ծ���.
    dev_write_func io_write;
    dev_read_func  io_read;
    dev_ctrl_func  io_ctrl;
};

//�豸�˿ڷ�Ϊ��˿ں��Ҷ˿�,�û�ʹ���豸ʱ,���ϸ�����ʹ����˿ڻ����Ҷ˿�,
//һ��Ҫ������¼���������,��Ŀ��Ҫר�Ź���.
struct pan_device
{
    struct  rsc_node  node;
    struct  dev_io left_io;
    struct  dev_io right_io;
    struct semaphore_LCB *left_semp; //�ź���,�����豸��������ͬʱ�򿪵Ĵ�����
    struct semaphore_LCB *right_semp; //�ź���,�����豸��������ͬʱ�򿪵Ĵ�����
    ptu32_t private_tag;              //���豸���е����ݽṹ,
};

ptu32_t module_init_driver(ptu32_t para);
struct  pan_device *dev_add_device(struct  pan_device    *parent_device,
                                   char                  *name,
                                   struct semaphore_LCB  *right_semp,
                                   struct semaphore_LCB  *left_semp,
                                   dev_write_func        right_write ,
                                   dev_read_func         right_read,
                                   dev_ctrl_func         right_ctrl ,
                                   dev_write_func        left_write ,
                                   dev_read_func         left_read ,
                                   dev_ctrl_func         left_ctrl ,
                                   ptu32_t               tag);
bool_t dev_delete_device(struct  pan_device * device);
struct pan_device * dev_get_root_device(void);
struct pan_device * dev_check_device(char * name);
struct pan_device * dev_check_device_scion(djy_handle_t ancestor,char * scion_name);
djy_handle_t dev_open_left(char *name,u32 timeout);
djy_handle_t dev_open_left_scion(djy_handle_t ancestor,
                                      char *scion_name, u32 timeout);
djy_handle_t dev_open_left_fast(djy_handle_t handle,u32 timeout);
djy_handle_t dev_open_right(char *name,u32 timeout);
djy_handle_t dev_open_right_scion(djy_handle_t ancestor,
                                      char *scion_name, u32 timeout);
djy_handle_t dev_open_right_fast(djy_handle_t handle,u32 timeout);
bool_t dev_close_left(djy_handle_t handle);
bool_t dev_close_right(djy_handle_t handle);
ptu32_t dev_read_left(djy_handle_t handle,ptu32_t src_buf,
                ptu32_t des_buf,ptu32_t len);
ptu32_t dev_write_left(djy_handle_t handle,ptu32_t src_buf,
                ptu32_t des_buf,ptu32_t len);
ptu32_t dev_ctrl_left(djy_handle_t handle,u32 cmd,
                    ptu32_t data1,ptu32_t data2);
ptu32_t dev_read_right(djy_handle_t handle,ptu32_t src_buf,
                ptu32_t des_buf,ptu32_t len);
ptu32_t dev_write_right(djy_handle_t handle,ptu32_t src_buf,
                ptu32_t des_buf,ptu32_t len);
ptu32_t dev_ctrl_right(djy_handle_t handle,u32 cmd,
                    ptu32_t data1,ptu32_t data2);
struct pan_device *dev_get_panp(djy_handle_t handle);
ptu32_t dev_get_my_tag(struct pan_device * pan);

#endif  //#if ((cfg_pan_driver == 1) && (cn_run_mode != cn_mode_tiny))

#ifdef __cplusplus
}
#endif
#endif //__driver_H___
