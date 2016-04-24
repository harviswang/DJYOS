//----------------------------------------------------
//Copyright (C), 2013-2020,  luoshitian.
//��Ȩ���� (C), 2013-2020,   ������.
//����ģ��: AR8021 phyоƬ����
//����:  ������.
//�汾��V1.0.0
//�ļ�����: AR8021 phyоƬ��������
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2013-11-14
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
extern s32 tsec_phy_write(s32 addr, s32 dev_addr, s32 regnum,
                   s16 value);
/**
 * genphy_config_aneg - restart auto-negotiation or write BMCR
 * @phydev: target phy_device struct
 *
 * Description: If auto-negotiation is enabled, we configure the
 *   advertising, and then restart auto-negotiation.  If it is not
 *   enabled, then we write the BMCR.
 */
//----phy��ʼ������-----------------------------------------------------------------
//���ܣ�����Atheros PHY�Ĵ�����ֻ���p1020rdb��etsec3�ڵ�ar8201 phyоƬ
//������int addr��phyоƬ��ַ��int supported��phyоƬ֧�ֵ����ݴ���ģʽ����
//���أ�����0
//-----------------------------------------------------------------------------
s32 phy_config_ar8021(s32 addr,s32 supported)
{
    tsec_phy_write(addr, -1, 0x1d, 0x05);
    tsec_phy_write(addr, -1, 0x1e, 0x3D47);

    return 0;
}
