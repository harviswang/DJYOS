//----------------------------------------------------
//Copyright (C), 2004-2009,  djyos development team.
//��Ȩ���� (C), 2004-2009,   �����߲���ϵͳ�з��Ŷ�.
//����ģ��:cpu˫�˿���
//���ߣ�����
//�汾��V1.0.0
//�ļ�����:˫�˿��ƴ���
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------

#include "os_inc.h"
#include "cpu_peri.h"

// =========================================================================
// ��������: ���� Ti ��������ת����bin��ʽ�����ƴ��룬����ǰ�����Ȱ�bin�ļ�����
//      ���ڴ��С�bin�ļ��ı����ʽ:
// ��ڵ�ַ��4bytes
// section 1 size��4bytes
// section 1 dest��4bytes
// section 1 data����������

// section 2 size��4bytes
// section 2 dest��4bytes
// section 2 data����������

//����

// section n size��4bytes
// section n dest��4bytes
// section n data����������
// 00000000      ��4bytes
// ��������: code_buf��bin�ļ���ŵ�ַ
// ����ֵ  : ��
// =========================================================================
void (*Dcore_LoadTiBinCode(u8 *code_buf))(void)
{
    void (*result)(void);
    u8 *dst;
    u32 size;
    u8 *src = code_buf;
    u32 loop;
	result = (void (*)(void))(src[0] + (src[1]<<8) + (src[2]<<16) + (src[3]<<24));
    if((u32)result != 0xc0400000)
        return NULL;             //��ڵ�ַ����0xc0000000��ʼ��128M��Χ��
    src = code_buf + 4;     //������ڵ�ַ
    while(1)
    {
        size = src[0] + (src[1]<<8) + (src[2]<<16) + (src[3]<<24);
        if(size == 0)
            break;
        dst = (u8*)(src[4] + (src[5]<<8) + (src[6]<<16) + (src[7]<<24));
        src += 8;
        for(loop = 0; loop < size; loop++)
            *dst++ = *src++;
    }
	return result;
}


// =========================================================================
// ��������: ��arm���ã�����dsp��ʹ֮��start_address��ַ��ʼ���С����ñ�����ǰ��
//      �����ȰѴ�����ص�start_address���ڵĵ�ַ��dsp���ɵ��ñ�������
// ��������: start_address��dsp����ڵ�ַ
// ����ֵ  : ��
// =========================================================================
void Dcore_WakeupDspRun(void (*start_address)(void))
{
    g_ptSysconfig0Reg->KICKR[0] = KICK0R_UNLOCK;
    g_ptSysconfig0Reg->KICKR[1] = KICK1R_UNLOCK;
    g_ptLpsc0Reg->MDCTL[cn_PSC0_DSP] &= ~(u32)LRST;
    g_ptSysconfig0Reg->HOST1CFG = (u32)start_address;
    g_ptSysconfig0Reg->KICKR[0] = KICK0R_LOCK;
    g_ptSysconfig0Reg->KICKR[1] = KICK1R_LOCK;
    Cpucfg_EnableLPSC(cn_PSC0_DSP);
    g_ptSysconfig0Reg->KICKR[0] = KICK0R_UNLOCK;
    g_ptSysconfig0Reg->KICKR[1] = KICK1R_UNLOCK;
    g_ptLpsc0Reg->MDCTL[cn_PSC0_DSP] |= LRST;
    g_ptSysconfig0Reg->KICKR[0] = KICK0R_LOCK;
    g_ptSysconfig0Reg->KICKR[1] = KICK1R_LOCK;
}

// =========================================================================
// ��������: ��arm���ã�����dsp��ʹ֮���ڸ�λ̬��
// ��������: start_address��dsp����ڵ�ַ
// ����ֵ  : ��
// =========================================================================
void Dcore_WakeupDspReset(void)
{
    g_ptSysconfig0Reg->KICKR[0] = KICK0R_UNLOCK;
    g_ptSysconfig0Reg->KICKR[1] = KICK1R_UNLOCK;
    g_ptLpsc0Reg->MDCTL[cn_PSC0_DSP] &= ~(u32)LRST;
    g_ptSysconfig0Reg->KICKR[0] = KICK0R_LOCK;
    g_ptSysconfig0Reg->KICKR[1] = KICK1R_LOCK;
    Cpucfg_EnableLPSC(cn_PSC0_DSP);
    g_ptSysconfig0Reg->KICKR[0] = KICK0R_UNLOCK;
    g_ptSysconfig0Reg->KICKR[1] = KICK1R_UNLOCK;
    g_ptLpsc0Reg->MDCTL[cn_PSC0_DSP] |= LRST;
    g_ptSysconfig0Reg->KICKR[0] = KICK0R_LOCK;
    g_ptSysconfig0Reg->KICKR[1] = KICK1R_LOCK;
}

#define CN_AisMagicWord         0x41504954
#define CN_SectionLoad          0x58535901
#define CN_SectionFill          0x5853590A
#define CN_EnableCRC            0x58535903
#define CN_DisableCRC           0x58535904
#define CN_ValidateCRC          0x58535902
#define CN_Jump_Close           0x58535906
#define CN_Jump2App             0x58535905
#define CN_SequentialReadEnable 0x58535963
#define CN_FunctionExecute      0x5853590D
#define CN_BootTable            0x58535907
// =========================================================================
// ��������: ���� Ti AIS��ʽ�����ƴ��룬����ǰ�����Ȱ�AIS�ļ����뵽�ڴ��С�
//      ais�ļ��ı����ʽ��Ti�ĵ�:SPRAB41C UsingtheOMAP-L1x8Bootloader
// ��������: code_buf��ais�ļ���ŵ�ַ��Ҫ��4�ֽڶ���
// ����ֵ  : ��������ָ��
// =========================================================================
void (*Dcore_LoadAisCode(u8 *code_buf))(void)
{
    void (*result)(void);
    u32 *dst;
    s32 size;
    u32 *src = (u32*)code_buf;
    u32 loop,command,data;
    if(((u32)code_buf & 0x3) != 0)   //Ҫ�����뻺����4�ֽڶ���
        return NULL;
    if(*src != CN_AisMagicWord)         //��AIS�ļ�
        return NULL;
    src++;
    while(1)
    {
        switch(*src++)
        {
            case CN_SectionLoad:
            {
                dst = (u32*)(*src++);
                size = *src++; 
//                memcpy(dst,src,size);
//                src += size>>2;
                for(; size>0; size -=4)
                    *dst++ = *src++;
            }break;
            case CN_SectionFill:
            {
                dst = (u32*)(*src++);
                size = *src++; 
//                memset(dst,0,size);
//                src += size>>2;
                for(; size>0; size -=4)
                    *dst++ = 0;
            }break;
            case CN_Jump_Close:
            case CN_Jump2App:
            {
                result = (void (*)(void))(*src);
                return result;
            }break;
            default:break;
        }
    }
	return result;
}

// =========================================================================
// ��������: ��arm���ã�ÿ�ε��ý�����һ��dsp��nmi�жϣ����nmi�ж��Ѿ�������
//      ��ִ���κβ�����dsp���ɵ��ñ�������
// ��������: ��
// ����ֵ  : ��
// =========================================================================
void Dcore_AssertDspIntNmi(void)
{
    g_ptSysconfig0Reg->CHIPSIG = 0x10;
}

// =========================================================================
// ��������: ��arm���ã�ÿ�ε��ý�����һ��dsp��chipint2�ж�(5���ж�)�����
//      chipint2�ж��Ѿ�������ִ���κβ�����dsp���ɵ��ñ�������
// ��������: ��
// ����ֵ  : ��
// =========================================================================
void Dcore_AssertDspIntCi2(void)
{
    g_ptSysconfig0Reg->CHIPSIG = 0x04;
}

// =========================================================================
// ��������: ��arm���ã�ÿ�ε��ý�����һ��dsp��chipint3�ж�(67���ж�)�����
//      chipint3�ж��Ѿ�������ִ���κβ�����dsp���ɵ��ñ�������
// ��������: ��
// ����ֵ  : ��
// =========================================================================
void Dcore_AssertDspIntCi3(void)
{
    g_ptSysconfig0Reg->CHIPSIG = 0x08;
}

// =========================================================================
// ��������: ��dsp���ã�ÿ�ε��ý�����һ��arm��chipint0�ж�(28���ж�)�����
//      chipint0�ж��Ѿ�������ִ���κβ�����arm���ɵ��ñ�������
// �����������
// �����������
// ����ֵ  ����
// =========================================================================
void Dcore_AssertArmIntCi0(void)
{
    g_ptSysconfig0Reg->CHIPSIG = 0x1;
}

// =========================================================================
// ��������: ��dsp���ã�ÿ�ε��ý�����һ��arm��chipint1�ж�(29���ж�)�����
//      chipint1�ж��Ѿ�������ִ���κβ�����arm���ɵ��ñ�������
// �����������
// �����������
// ����ֵ  ����
// =========================================================================
void Dcore_AssertArmIntCi1(void)
{
    g_ptSysconfig0Reg->CHIPSIG = 0x2;
}

// =========================================================================
// ��������: ��dsp���ã�ÿ�ε��ý�����һ��arm��chipint2�ж�(30���ж�)�����
//      chipint2�ж��Ѿ�������ִ���κβ�����arm���ɵ��ñ�������
// �����������
// �����������
// ����ֵ  ����
// =========================================================================
void Dcore_AssertArmIntCi2(void)
{
    g_ptSysconfig0Reg->CHIPSIG = 0x4;
}

// =========================================================================
// ��������: ��dsp���ã�ÿ�ε��ý�����һ��arm��chipint3�ж�(28���ж�)�����
//      chipint3�ж��Ѿ�������ִ���κβ�����arm���ɵ��ñ�������
// �����������
// �����������
// ����ֵ  ����
// =========================================================================
void Dcore_AssertArmIntCi3(void)
{
    g_ptSysconfig0Reg->CHIPSIG = 0x8;
}

// =========================================================================
// ��������: ��dsp���ã�����ѹ����dsp��nmi�жϣ����nmi�ж�δ������ִ��
//      �κβ�����arm���ɵ��ñ�������
// �����������
// �����������
// ����ֵ  ����
// =========================================================================
void Dcore_ClearDspIntNmi(void)
{
    g_ptSysconfig0Reg->CHIPSIG_CLR = 0x10;
}

// =========================================================================
// ��������: ��dsp���ã�����ѹ����dsp��chipint2�ж�(5���ж�)�����chipint2��
//      ��δ������ִ���κβ�����arm���ɵ��ñ�������
// �����������
// �����������
// ����ֵ  ����
// =========================================================================
void Dcore_ClearDspIntCi2(void)
{
    g_ptSysconfig0Reg->CHIPSIG_CLR = 0x04;
}

// =========================================================================
// ��������: ��dsp���ã�����ѹ����dsp��chipint3�ж�(67���ж�)�����chipint3��
//      ��δ������ִ���κβ�����arm���ɵ��ñ�������
// �����������
// �����������
// ����ֵ  ����
// =========================================================================
void Dcore_ClearDspIntCi3(void)
{
    g_ptSysconfig0Reg->CHIPSIG_CLR = 0x08;
}

// =========================================================================
// ��������: ��arm���ã�����ѹ����arm��chipint0�ж�(28���ж�)�����chipint0��
//      ��δ������ִ���κβ�����dsp���ɵ��ñ�������
// ��������: ��
// ����ֵ  : ��
// =========================================================================
void Dcore_ClearArmIntCi0(void)
{
    g_ptSysconfig0Reg->CHIPSIG_CLR = 0x1;
}

// =========================================================================
// ��������: ��arm���ã�����ѹ����arm��chipint1�ж�(29���ж�)�����chipint1��
//      ��δ������ִ���κβ�����dsp���ɵ��ñ�������
// ��������: ��
// ����ֵ  : ��
// =========================================================================
void Dcore_ClearArmIntCi1(void)
{
    g_ptSysconfig0Reg->CHIPSIG_CLR = 0x2;
}

// =========================================================================
// ��������: ��dsp���ã�����ѹ����arm��chipint2�ж�(30���ж�)�����chipint2��
//      ��δ������ִ���κβ�����dsp���ɵ��ñ�������
// ��������: ��
// ����ֵ  : ��
// =========================================================================
void Dcore_ClearArmIntCi2(void)
{
    g_ptSysconfig0Reg->CHIPSIG_CLR = 0x4;
}

// =========================================================================
// ��������: ��arm���ã�����ѹ����arm��chipint3�ж�(31���ж�)�����chipint3��
//      ��δ������ִ���κβ�����dsp���ɵ��ñ�������
// ��������: ��
// ����ֵ  : ��
// =========================================================================
void Dcore_ClearArmIntCi3(void)
{
    g_ptSysconfig0Reg->CHIPSIG_CLR = 0x08;
}

