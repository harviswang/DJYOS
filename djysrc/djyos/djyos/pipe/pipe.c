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
#include "os_inc.h"

#if ((cfg_pan_driver == 1) && (cn_run_mode != cn_mode_tiny))

static struct  pan_device *pg_pipe_parent_device;
#if(cn_pipes_limit != 0)
static struct pipe_PCB tg_pipe_mem_block[cn_pipes_limit];
#endif
static struct mem_cell_pool *pg_pipe_pool;  //�ź����ṹ�ڴ��ͷָ��

//----��ʼ��pipeģ��ģ��-------------------------------------------------------
//���ܣ���"dev"�豸�´���"pipe"�豸����Ϊ��������pipe�豸�ĸ��豸������һ����
//      �豸�����κξ���Ĺ��ܡ�
//������para�������ߴ���Ĳ�������ģ��δʹ��
//���أ�1=�ɹ���0=ʧ��
//-----------------------------------------------------------------------------
ptu32_t module_init_pipe(ptu32_t para)
{
    static struct mem_cell_pool pipe_pool;
    
    pg_pipe_parent_device = dev_add_device( dev_get_root_device(),"pipe",
                                 NULL,NULL,      //���ź�������
                                 (dev_write_func) NULL_func ,
                                 (dev_read_func ) NULL_func,
                                 (dev_ctrl_func ) NULL_func ,
                                 (dev_write_func ) NULL_func ,
                                 (dev_read_func  ) NULL_func ,
                                 (dev_ctrl_func  ) NULL_func,
                                 (ptu32_t)NULL   //key�豸˽������
                                 ); 
    if(pg_pipe_parent_device == NULL)
        return 0;
    
    //��ʼ��pipe���ƿ��ڴ��
#if(cn_pipes_limit != 0)
    pg_pipe_pool = mb_create_pool_knl( &pipe_pool,
                                    (void*)tg_pipe_mem_block,
                                    cn_pipes_limit,
                                    sizeof(struct pipe_PCB),
                                    0,0,
                                    "pipe���ƿ��");
#else
    pg_pipe_pool = mb_create_pool_knl( &pipe_pool,
                                    NULL,
                                    cn_pipes_limit,
                                    sizeof(struct pipe_PCB),
                                    0,0,
                                    "pipe���ƿ��");
#endif
    return 1;
}


//----����һ���ܵ�-----------------------------------------------------------
//���ܣ�����һ���ܵ�
//������buf,�ܵ�ʹ�õĻ�����������ʹ���߷����ṩ��
//      size����������С
//      whole��true=һ���Բ�������Ҫ���д����һ������ɣ�false=��һ���Բ�����
//          д��ʱ���ж��ٿ�λд����٣���ȡʱ���ܵ����ж������ݶ�ȡ����
//      name���ܵ������֣���ָ����ַ����ڴ��������Ǿֲ������������ǿ�
//���أ��½������ź���ָ��
//-----------------------------------------------------------------------------
struct pipe_PCB *pipe_create(u8* buf,u32 size,bool_t whole,
                             u32 write_level,u32 read_level,char *name)
{
    struct pipe_PCB *pipe;
    struct semaphore_LCB *read_dev_semp,*write_dev_semp;
    struct pan_device *pipe_device;
    
    pipe = (struct pipe_PCB *)mb_malloc(pg_pipe_pool, 0);
    if(pipe == NULL)
        goto exit_from_malloc_PCB;
    //��ʼ�������ź�״̬����������ͷŲŴ������ź�״̬
    pipe->pipe_write_semp = semp_create(1,0,name);
    if(pipe->pipe_write_semp == NULL)
        goto exit_from_pipe_write_semp;
    //��ʼ�������ź�״̬����д�����ͷŲŴ������ź�״̬
    pipe->pipe_read_semp = semp_create(1,0,name);
    if(pipe->pipe_read_semp == NULL)
        goto exit_from_pipe_read_semp;
    read_dev_semp = semp_create(1,1,NULL);
    if(read_dev_semp == NULL)
        goto exit_from_dev_read_semp;
    write_dev_semp = semp_create(1,1,NULL);
    if(write_dev_semp == NULL)
        goto exit_from_dev_write_semp;
    pipe_device = dev_add_device(pg_pipe_parent_device,
                                 name,
                                 read_dev_semp,write_dev_semp,
                                 NULL,
                                 pipe_dev_read,
                                 pipe_dev_ctrl,
                                 pipe_dev_write,
                                 NULL,
                                 pipe_dev_ctrl,
                                 (ptu32_t)pipe       );
    if(pipe_device == NULL)
        goto exit_from_create_device;
                                 
    ring_init(&pipe->pipe_buf,buf,size);
    pipe->write_level = write_level;
    pipe->read_level = read_level;
    pipe->whole_mode = whole;
    return pipe;

exit_from_create_device:
    semp_delete(write_dev_semp);
exit_from_dev_write_semp:
    semp_delete(read_dev_semp);
exit_from_dev_read_semp:
    semp_delete(pipe->pipe_read_semp);
exit_from_pipe_read_semp:
    semp_delete(pipe->pipe_write_semp);
exit_from_pipe_write_semp:
    mb_free(pg_pipe_pool,pipe);
exit_from_malloc_PCB:
    return NULL;
}

//----���ܵ�-------------------------------------------------------------------
//����: �ӹܵ��ж�����
//����: pipe_dev��Ŀ��ܵ��豸
//      res����������
//      dst_buf���������ݴ�ŵ�ַ
//      len����������
//����: ���Ƿ���len
//-----------------------------------------------------------------------------
ptu32_t pipe_dev_read(struct  pan_device *pipe_dev,ptu32_t res,
                                        ptu32_t dst_buf,ptu32_t len)
{
    struct pipe_PCB *pipe;
    u32 completed=0,temp;
    pipe = (struct pipe_PCB *)dev_get_my_tag(pipe_dev);
    if(len > ring_capacity(&pipe->pipe_buf))    //һ�ζ�ȡ���ݲ��ܳ����ܵ�����
        return 0;
    temp = ring_check(&pipe->pipe_buf);
    while(1)
    {
        completed += ring_read(&pipe->pipe_buf,(u8*)(dst_buf+completed),
                               (u32)len-completed);
        if(completed < (u32)len)    //�ܵ��е�����������
        {
            semp_pend(pipe->pipe_read_semp,cn_timeout_forever);
        }else
        {
            break;
        }
    } 
    //�����֮ǰ����д���ż��Ҷ�֮�����д���ż���post�ź���
    if( (temp >= pipe->write_level)
          &&((temp - completed) < pipe->write_level)) 
    {
        semp_post(pipe->pipe_write_semp);
    }
    return len;
}

//----д�ܵ�-------------------------------------------------------------------
//����: ������д��ܵ���
//����: pipe_dev��Ŀ��ܵ��豸
//      src_buf��д�����ݴ�ŵ�ַ
//      res����������
//      len��д�볤��
//����: ʵ��д��������
//-----------------------------------------------------------------------------
u32 pipe_dev_write(struct  pan_device *pipe_dev,ptu32_t src_buf,
                            ptu32_t res,ptu32_t len)
{
    struct pipe_PCB *pipe;
    u32 completed = 0, temp;
    pipe = (struct pipe_PCB *)dev_get_my_tag(pipe_dev);
    temp = ring_check(&pipe->pipe_buf);
    while(1)
    {
        if(pipe->whole_mode == true)        //����ģʽ
        {
            //�ܵ��е�ʣ��ռ䲻��
            if((ring_capacity(&pipe->pipe_buf)-ring_check(&pipe->pipe_buf))
                        < (u32)len)
            {
                semp_pend(pipe->pipe_write_semp,cn_timeout_forever);
            }else
            {
                ring_write(&pipe->pipe_buf,(u8*)src_buf,(u32)len);
                break;
            }
        }else
        {
            completed +=ring_write(&pipe->pipe_buf,(u8*)src_buf,len -completed);
            if(completed >= len)
                break;
        }
    }

    //���д֮ǰ������ȡ�ż��Ҷ�֮�󳬹�д���ż���post�ź���
    if( (temp < pipe->read_level)
          &&((temp + completed) >= pipe->read_level)) 
    {
        semp_post(pipe->pipe_read_semp);
    }
    return completed;
}

//----���ƹܵ�-----------------------------------------------------------------
//����: �պ���
//-----------------------------------------------------------------------------
ptu32_t pipe_dev_ctrl(struct  pan_device *pipe_dev,u32 cmd,
                        u32 data1,u32 data2)
{
    return 0;
}
//----ɾ��һ���ܵ�-----------------------------------------------------------
//���ܣ�ɾ��һ���ܵ�
//������mutex����ɾ���Ĺܵ��豸
//���أ���
//-----------------------------------------------------------------------------
bool_t pipe_delete(struct  pan_device *pipe_dev)
{
    struct pipe_PCB *pipe;
    struct semaphore_LCB *pipe_left_semp,*pipe_right_semp;
    pipe = (struct pipe_PCB *)dev_get_my_tag(pipe_dev);
    pipe_left_semp = pipe_dev->left_semp;
    pipe_right_semp = pipe_dev->right_semp;
    if(dev_delete_device(pipe_dev))
    {
        semp_delete(pipe_left_semp);
        semp_delete(pipe_right_semp);
        semp_delete(pipe->pipe_read_semp);
        semp_delete(pipe->pipe_write_semp);
        mb_free(pg_pipe_pool,pipe);
        return true;
    }else
        return false;
}

//----��ѯ�ܵ�����-----------------------------------------------------------
//���ܣ���ѯһ���ܵ��豸��������
//������pipe_dev������ѯ�Ĺܵ����
//���أ��ܵ�����
//-----------------------------------------------------------------------------
u32 pipe_capacity(struct  pan_device *pipe_dev)
{
    struct pipe_PCB *pipe;
    pipe = (struct pipe_PCB *)dev_get_my_tag(pipe_dev);
    return ring_capacity(&pipe->pipe_buf);
}

//----��ѯ�ܵ���������---------------------------------------------------------
//���ܣ���ѯһ���ܵ��豸�е���������
//������pipe_dev������ѯ�Ĺܵ����
//���أ��ܵ��е�������
//-----------------------------------------------------------------------------
u32 pipe_check(struct  pan_device *pipe_dev)
{
    struct pipe_PCB *pipe;
    pipe = (struct pipe_PCB *)dev_get_my_tag(pipe_dev);
    return ring_check(&pipe->pipe_buf);
}

#endif  //#if ((cfg_pan_driver == 1) && (cn_run_mode != cn_mode_tiny))


