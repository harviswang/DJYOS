//----------------------------------------------------
//Copyright (C), 2013-2020,  luoshitian.
//��Ȩ���� (C), 2013-2020,   ������.
//����ģ��: 88E1512 phyоƬ����
//����:  ������.
//�汾��V1.0.0
//�ļ�����: 88E1512 phyоƬ��������
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2013-11-14
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------

#ifndef __PHY_88E1512_H__
#define __PHY_88E1512_H__



struct phy_parameter
{
    u32 speed;
    u32 duplex;
};

s32 genphy_update_link_88E1512(u32 addr);
struct phy_parameter genphy_parse_link__88E1512(u32 addr);
s32 phy_config_88e1512(s32 addr,s32 supported);
void phy_reset_88e1512(s32 addr);

#endif









