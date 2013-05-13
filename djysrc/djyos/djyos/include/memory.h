//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//��Ȩ���� (C), 2004-2009,   ������.
//����ģ��:�ڴ����ģ��
//����:  ������.
//�汾��V1.0.0
//�ļ�����:�Թ���͹̶����������ͷ�ļ�
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __memory_H__
#define __memory_H__

//���ó�������
//��nλΪ1�������λΪ0�ĳ���
#define cn_high_1bit_1      (0x80000000)
#define cn_high_2bit_1      (0xc0000000)
#define cn_high_3bit_1      (0xe0000000)
#define cn_high_4bit_1      (0xf0000000)
#define cn_high_5bit_1      (0xf8000000)
#define cn_high_6bit_1      (0xfc000000)
#define cn_high_7bit_1      (0xfe000000)
#define cn_high_8bit_1      (0xff000000)
#define cn_high_9bit_1      (0xff800000)
#define cn_high_10bit_1     (0xffc00000)
#define cn_high_11bit_1     (0xffe00000)
#define cn_high_12bit_1     (0xfff00000)
#define cn_high_13bit_1     (0xfff80000)
#define cn_high_14bit_1     (0xfffc0000)
#define cn_high_15bit_1     (0xfffe0000)
#define cn_high_16bit_1     (0xffff0000)
#define cn_high_17bit_1     (0xffff8000)
#define cn_high_18bit_1     (0xffffc000)
#define cn_high_19bit_1     (0xffffe000)
#define cn_high_20bit_1     (0xfffff000)
#define cn_high_21bit_1     (0xfffff800)
#define cn_high_22bit_1     (0xfffffc00)
#define cn_high_23bit_1     (0xfffffe00)
#define cn_high_24bit_1     (0xffffff00)
#define cn_high_25bit_1     (0xffffff80)
#define cn_high_26bit_1     (0xffffffc0)
#define cn_high_27bit_1     (0xffffffe0)
#define cn_high_28bit_1     (0xfffffff0)
#define cn_high_29bit_1     (0xfffffff8)
#define cn_high_30bit_1     (0xfffffffc)
#define cn_high_31bit_1     (0xfffffffe)
#define cn_allbit_1         (0xffffffff)

//��nλΪ1������λΪ0�ĳ���
#define cn_low_1bit_1      (0x00000001)
#define cn_low_2bit_1      (0x00000003)
#define cn_low_3bit_1      (0x00000007)
#define cn_low_4bit_1      (0x0000000f)
#define cn_low_5bit_1      (0x0000001f)
#define cn_low_6bit_1      (0x0000003f)
#define cn_low_7bit_1      (0x0000007f)
#define cn_low_8bit_1      (0x000000ff)
#define cn_low_9bit_1      (0x000001ff)
#define cn_low_10bit_1     (0x000003ff)
#define cn_low_11bit_1     (0x000007ff)
#define cn_low_12bit_1     (0x00000fff)
#define cn_low_13bit_1     (0x00001fff)
#define cn_low_14bit_1     (0x00003fff)
#define cn_low_15bit_1     (0x00007fff)
#define cn_low_16bit_1     (0x0000ffff)
#define cn_low_17bit_1     (0x0001ffff)
#define cn_low_18bit_1     (0x0003ffff)
#define cn_low_19bit_1     (0x0007ffff)
#define cn_low_20bit_1     (0x000fffff)
#define cn_low_21bit_1     (0x001fffff)
#define cn_low_22bit_1     (0x003fffff)
#define cn_low_23bit_1     (0x007fffff)
#define cn_low_24bit_1     (0x00ffffff)
#define cn_low_25bit_1     (0x01ffffff)
#define cn_low_26bit_1     (0x03ffffff)
#define cn_low_27bit_1     (0x07ffffff)
#define cn_low_28bit_1     (0x0fffffff)
#define cn_low_29bit_1     (0x1fffffff)
#define cn_low_30bit_1     (0x3fffffff)
#define cn_low_31bit_1     (0x7fffffff)

enum mem_error_code
{
    enum_mem_no_error = cn_mem_no_error,  //û�д���
    enum_mem_error,               //�洢������
    enum_mem_page_error,          //ҳ�ߴ����
    enum_mem_tried,               //�ڴ治��
    enum_mem_leak,                //�ڴ�й©
};

#if defined ( __CC_ARM   ) // RealView Compiler
#define heapsys__bottom     Image$$heap_bottom$$ZI$$Limit
#define heapsys__top        Image$$heap_top$$ZI$$Base
#endif

extern u32 sys_heap_top[]; //�ڽű��ж���
extern u32 sys_heap_bottom[];  //�ڽű��ж���

#endif //__memory_H___

