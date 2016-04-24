//----------------------------------------------------
//Copyright (C), 2013-2020,  luoshitian.
//��Ȩ���� (C), 2013-2020,   ������.
//����ģ��: pci�Ĵ�����д����
//����:  ������.
//�汾��V1.0.0
//�ļ�����: pci�Ĵ�����д����
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2013-11-14
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include <asm/io.h>
#include "pci.h"

int indirect_read_config_byte(struct pci_controller *hose,
                              pci_dev_t dev, int offset, u8 * val)
{
    u32 b, d,f;
    b = PCI_BUS(dev); d = PCI_DEV(dev); f = PCI_FUNC(dev);
    b = b - hose->first_busno;
    dev = PCI_BDF(b, d, f);
    *(hose->cfg_addr) = dev | (offset & 0xfc) | ((offset & 0xf00) << 16) | 0x80000000;
    sync();
    *val = in_8((u8*)&hose->cfg_data[offset&3]);
    return 0;
}

int indirect_read_config_word(struct pci_controller *hose,
          pci_dev_t dev, int offset, u16 * val)
{
    u32 b, d,f;
    b = PCI_BUS(dev); d = PCI_DEV(dev); f = PCI_FUNC(dev);
    b = b - hose->first_busno;
    dev = PCI_BDF(b, d, f);
    *(hose->cfg_addr) = dev | (offset & 0xfc) | ((offset & 0xf00) << 16) | 0x80000000;
    sync();
    *val = in_le16((u16*)&hose->cfg_data[offset&2]);
    return 0;
}

int indirect_read_config_dword(struct pci_controller *hose,
          pci_dev_t dev, int offset, u32 * val)
{
    u32 b, d,f;
    b = PCI_BUS(dev); d = PCI_DEV(dev); f = PCI_FUNC(dev);
    b = b - hose->first_busno;
    dev = PCI_BDF(b, d, f);
    *(hose->cfg_addr) = dev | (offset & 0xfc) | ((offset & 0xf00) << 16) | 0x80000000;
    sync();
    *val = in_le32((u32*)&hose->cfg_data[offset&0]);
    return 0;
}

int indirect_write_config_byte(struct pci_controller *hose,
        pci_dev_t dev, int offset, u8 val)
{
    u32 b, d,f;
    b = PCI_BUS(dev); d = PCI_DEV(dev); f = PCI_FUNC(dev);
    b = b - hose->first_busno;
    dev = PCI_BDF(b, d, f);
    *(hose->cfg_addr) = dev | (offset & 0xfc) | ((offset & 0xf00) << 16) | 0x80000000;
    sync();
    out_8((u8*)&hose->cfg_data[offset&3],val);
    return 0;
}

int indirect_write_config_word(struct pci_controller *hose,
        pci_dev_t dev, int offset, u16 val)
{
    u32 b, d,f;
    b = PCI_BUS(dev); d = PCI_DEV(dev); f = PCI_FUNC(dev);
    b = b - hose->first_busno;
    dev = PCI_BDF(b, d, f);
    *(hose->cfg_addr) = dev | (offset & 0xfc) | ((offset & 0xf00) << 16) | 0x80000000;
    sync();
    out_le16((u16*)&hose->cfg_data[offset&2],val);
    return 0;
}

int indirect_write_config_dword(struct pci_controller *hose,
        pci_dev_t dev, int offset, u32 val)
{
    u32 b, d,f;
    b = PCI_BUS(dev); d = PCI_DEV(dev); f = PCI_FUNC(dev);
    b = b - hose->first_busno;
    dev = PCI_BDF(b, d, f);
    *(hose->cfg_addr) = dev | (offset & 0xfc) | ((offset & 0xf00) << 16) | 0x80000000;
    sync();
    out_le32((u32*)&hose->cfg_data[offset&0],val);
    return 0;
}

void pci_setup_indirect(struct pci_controller* hose, u32 cfg_addr, u32 cfg_data)
{
    hose->cfg_addr = (unsigned int *) cfg_addr;
    hose->cfg_data = (unsigned char *) cfg_data;
}

