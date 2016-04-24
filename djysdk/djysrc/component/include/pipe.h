//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
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
#ifndef __PIPE_H__
#define __PIPE_H__

#ifdef __cplusplus
extern "C" {
#endif

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
 struct tagPipePCB
{
    struct tagRingBuf pipe_buf;               //���η��ͻ�����.
    struct tagSemaphoreLCB *pipe_write_semp;  //д��������
    struct tagSemaphoreLCB *pipe_read_semp;   //����������
    u32    write_level;                     //д����ˮƽ
    u32    read_level;                      //������ˮƽ
    bool_t whole_mode;          //����ģʽ��true=ȷ��ÿ��д��������¼������Ҫд
                                //��10���ֽڣ���pipe_buf��ֻ��9����λʱ��������
                                //false=��д��9����Ȼ������
};

#define pipe_open_write(name,timeout)       Driver_DevOpenLeft(name,timeout)
#define pipe_open_read(name,timeout)        Driver_DevOpenRight(name,timeout)
#define pipe_open_write_fast(pipe,timeout)  Driver_DevOpenLeftFast(pipe,timeout)
#define pipe_open_read_fast(pipe,timeout)   Driver_DevOpenRightFast(pipe,timeout)
#define pipe_close_write(pipe)              Driver_DevCloseLeft(pipe)
#define pipe_close_read(pipe)               Driver_DevCloseRight(pipe)
#define pipe_write(pipe,buf,len,timeout)    Driver_DevWriteLeft(pipe,buf,len,timeout)
#define pipe_read(pipe,buf,len,timeout)     Driver_DevReadRight(pipe,buf,len,timeout)

ptu32_t Pipe_ModuleInit(ptu32_t para);
tagDevHandle Pipe_Create(u8* buf,u32 size,bool_t whole,
                             u32 write_level,u32 read_level,char *name);
ptu32_t Pipe_DevRead(tagDevHandle pipe_dev,ptu32_t res,
                                        ptu32_t dst_buf,ptu32_t len);
ptu32_t Pipe_DevWrite(tagDevHandle pipe_dev,ptu32_t src_buf,
                            ptu32_t res,ptu32_t len);
ptu32_t Pipe_DevCtrl(tagDevHandle pipe_dev,u32 cmd,
                        u32 data1,u32 data2);
bool_t Pipe_Delete(tagDevHandle pipe_dev);
u32 Pipe_Capacity(tagDevHandle pipe_dev);
u32 Pipe_Check(tagDevHandle pipe_dev);

#ifdef __cplusplus
}
#endif

#endif //__PIPE_H__

