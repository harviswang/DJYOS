//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//��Ȩ���� (C), 2004-2009,   lst.
//����ģ��:�ѹ���ģ��
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����: �ں�����ͷ�ļ�
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-10-31
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __ARCH_H_
#define __ARCH_H_

#ifdef __cplusplus
extern "C" {
#endif

#define arm920t

//�洢����С�˵�����
#define cn_little_endian       0
#define cn_big_endian          1
#define cn_cpu_endian          cn_little_endian

//��CPU�ֳ���ص�����
#define cn_cpu_bits     32  //�������ֳ�
#define cn_ptr_bits     32  //ָ���ֳ�����ֲ����
#define cn_cpu_bits_suffix_zero 5
#define cn_byte_bits    8   //�ֽ�λ��



//������CPU������һ��ָ�����������ͣ���������Ҫ�������������ԵĲ������Լ�
//��Ҫ���ٵĲ�����
//���������Ժ�ԭ�Ӳ�������ͬ:
//������������ָ���κ�ʱ���ܶ�����ȷֵ�����ݣ������������:
//  uint32_t    b32;
//  b32++;  //b32��ԭֵ��0x0000ffff
//��32λrisc���ϣ�ִ�й���Ϊ��
//ȡb32��ַ���Ĵ���0-->ȡb32����ֵ���Ĵ���1-->�Ĵ���1��1-->�ѼĴ���1д��b32.
//�����������������ﱻ�жϻ��߱������ȼ��̴߳�ϣ����жϻ�����ȼ��߳��ж�b32��
//Ҫô�õ�0x0000ffff��Ҫô�õ�0x00010000,������������Ϊ����ȷ����.
//����16λ���ϣ�ִ�й�����
//1��ȡ0x0000���Ĵ���r0��ȡ0xffff���Ĵ���r1
//2��ִ�мӲ�����ʹr0=0x0001,r1=0x0000
//3����r0д��b32�ĸ�16λ��
//4����r1д��b32�ĵ�16λ��
//����ڵ�3�͵�4����֮�䱻�жϴ�ϣ����ж����b32�����õ������0x00001ffff��
//��ôԭ�Ӳ�����?��������ִ�й��̲����κ��жϻ��߸����ȼ��̴߳�ϣ�����������룬
//���b32������Ϊԭ�ӱ�������������16λ����32λ���ϣ�b32�����Եõ���ȷ��ֵ��Ŀ
//ǰ��djyos����֧��ԭ�ӱ�����ֻ��ʹ�û�����ʵ��ԭ�ӱ�����Ч����
    #define cn_limit_ucpu   cn_limit_uint32
    #define cn_min_ucpu     (0)
    #define cn_limit_scpu   cn_limit_sint32
    #define cn_min_scpu     cn_min_sint32
//�����ǿ����������ͣ�һ����˵��CPU�����ǿ����������ͣ�����Щϵͳ�洢�����С��
//CPU�ֳ������´Ӵ洢����һ��cpu�ֳ�������Ҫ����洢�����ڣ�����,16λ��32λcpu
//��,�ֽڲ���Ҳ����һ�����.������Щ���ֳ�û��Ҫ�������������˵��ʹ����洢��
//��ȵȳ��ı���Ҳ��������ٵġ�����16λSDRAM��44b0x����������16λ��.
    #define cn_limit_ufast  cn_limit_uint16
    #define cn_min_ufast    (0)
    #define cn_limit_sfast  cn_limit_sint16
    #define cn_min_sfast    cn_min_sint16

//���涨��cpuϵͳ��ջ����
#define cn_full_down_stack  0   //������������ջ
#define cn_empty_down_stack 1   //���������Ŀ�ջ
#define cn_full_up_stack    2   //������������ջ
#define cn_empty_up_stack   3   //���������Ŀ�ջ
#define cn_stack_type       cn_full_down_stack

//�����ں�ջ����ȼ��㷽��:ջ����������Ǹ�ϵͳ����
#define cn_kernel_stack     0x200   //�����ں�ջ���߳���ҪΪϵͳ����������ӵ�ջ
                                    //Ŀǰ���api����dev_add_root_device ��256bytes
                                    //----2009-10-11

//����һЩ�������صĺ�
#define mem_align           1           //���Ŀ��ϵͳû�ж���Ҫ�󣬸�Ϊ0
#define align_size          8           //arm(��cm3)Ҫ��8�ֽڶ���

#define align_down(bound,x) ((x)&(~((bound)-1)))    //��x��bound�߽����¶���
#define align_up(bound,x)   (((x)+((bound)-1))&(~((bound)-1)))  //���϶���,

//����������ϵͳ�����ݶ��뷽ʽ����ֲ����
#define align_down_sys(x)   align_down(8,x)   //armҪ��8�ֽڶ���
#define align_up_sys(x)     align_up(8,x)     //armҪ��8�ֽڶ���

#define define_align_buf(name,size)     \
                align_type name[align_up_sys(size)/sizeof(align_size)]

#ifdef __cplusplus
}
#endif
#endif // __ARCH_H_


