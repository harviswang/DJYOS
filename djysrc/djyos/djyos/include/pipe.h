//----------------------------------------------------
//Copyright (C), 2004-2010,  luoshitian.
//��Ȩ���� (C), 2004-2010,   ������.
//����ģ��: ��ģ��
//����:  ������.
//�汾��V1.0.0
//�ļ�����: �ܵ�ģ��
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2010-10-01
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __pipe_h__
#define __pipe_h__

#ifdef __cplusplus
extern "C" {
#endif

#if ((cfg_pan_driver == 1) && (cn_run_mode != cn_mode_tiny))

//�ܵ����ƿ�
//�����ܵ�ʱ������ܵ���û���㹻�������������������ź����У�д������ʱʹ�ܵ��е�
//����������level���������ͷ��ź�����
//д�ܵ�ʱ�����д���ˣ����������ź����У����ܵ�ʹ�ùܵ��е�����������levelʱ��
//���ͷ��ź�����
//�ܵ��ǵ��߳������

//�ر�ע��:�ܵ���������������ԱӦ���Լ�ע�ⲻҪ������������������Ϊ:
//1������ӹܵ����������������ܵ����ȣ��ܵ�д������������Ϊ��������һֱ�ò���
//   ���㣬��������������ȥ��
//2������ģʽ�£����������������(�ܵ�����-���һ������д��������)�������ֺ�1һ
//   ����Ч����
struct pipe_PCB
{
    struct ring_buf pipe_buf;               //���η��ͻ�����.
    struct semaphore_LCB *pipe_write_semp;  //д��������
    struct semaphore_LCB *pipe_read_semp;   //����������
    u32    write_level;                     //д����ˮƽ
    u32    read_level;                      //������ˮƽ
    bool_t whole_mode;          //����ģʽ��true=ȷ��ÿ��д��������¼������Ҫд
                                //��10���ֽڣ���pipe_buf��ֻ��9����λʱ��������
                                //false=��д��9����Ȼ������
};

#define pipe_open_write(name)       dev_open_left(name,cn_timeout_forever)
#define pipe_open_read(name)        dev_open_right(name,cn_timeout_forever)
#define pipe_open_write_fast(pipe)  dev_open_left_fast(pipe,cn_timeout_forever)
#define pipe_open_read_fast(pipe)   dev_open_right_fast(pipe,cn_timeout_forever)
#define pipe_close_write(pipe)      dev_close_left(pipe)
#define pipe_close_read(pipe)       dev_close_right(pipe)
#define pipe_write(pipe,buf,len)    dev_write_left(pipe,buf,0,len)
#define pipe_read(pipe,buf,len)     dev_read_right(pipe,0,buf,len)

ptu32_t module_init_pipe(ptu32_t para);
struct pipe_PCB *pipe_create(u8* buf,u32 size,bool_t whole,
                             u32 write_level,u32 read_level,char *name);
ptu32_t pipe_dev_read(struct  pan_device *pipe_dev,ptu32_t res,
                                        ptu32_t dst_buf,ptu32_t len);
ptu32_t pipe_dev_write(struct  pan_device *pipe_dev,ptu32_t src_buf,
                            ptu32_t res,ptu32_t len);
ptu32_t pipe_dev_ctrl(struct  pan_device *pipe_dev,u32 cmd,
                        u32 data1,u32 data2);
bool_t pipe_delete(struct  pan_device *pipe_dev);
u32 pipe_capacity(struct  pan_device *pipe_dev);
u32 pipe_check(struct  pan_device *pipe_dev);

#endif  //#if ((cfg_pan_driver == 1) && (cn_run_mode != cn_mode_tiny))

#ifdef __cplusplus
}
#endif

#endif //__pipe_h__

