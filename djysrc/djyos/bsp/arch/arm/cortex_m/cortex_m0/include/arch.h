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
//1. ����: 2009-10-16
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __ARCH_H_
#define __ARCH_H_

#ifdef __cplusplus
extern "C" {
#endif

//�洢����С�˵�����
#define cn_little_endian       0
#define cn_big_endian          1
#define cn_cpu_endian          cn_little_endian

//��CPU�ֳ���ص�����
#define cn_cpu_bits     32  //�������ֳ�
#define cn_ptr_bits     32  //CPU��ַλ������ֲ����
#define cn_cpu_bits_suffix_zero 5
#define cn_byte_bits    8   //�ֽ�λ��

    #define cn_limit_ucpu   cn_limit_uint32
    #define cn_min_ucpu     (0)

    #define cn_limit_scpu   cn_limit_sint32
    #define cn_min_scpu     cn_min_sint32

    #define cn_limit_ufast  cn_limit_uint8
    #define cn_min_ufast    (0)

    #define cn_limit_sfast  cn_limit_sint8
    #define cn_min_sfast    cn_min_sint8

//���涨��cpuϵͳ��ջ����
#define cn_full_down_stack  0   //������������ջ
#define cn_empty_down_stack 1   //���������Ŀ�ջ
#define cn_full_up_stack    2   //������������ջ
#define cn_empty_up_stack   3   //���������Ŀ�ջ
#define cn_stack_type       cn_full_down_stack

//�����Ƿ�֧��cache��loader���ݱ����������Ƿ���cacheͬ��
#define cn_cache_used       0
#define cn_mmu_used         0   //�����Ƿ�֧��mmu
#define cortex_m0

//�����ں�ջ����ȼ��㷽��:ջ����������Ǹ�ϵͳ����
#define cn_kernel_stack     300     //�����ں�ջ���߳���ҪΪϵͳ����������ӵ�ջ
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


