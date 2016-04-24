//----------------------------------------------------
/*
 *
 * PCI autoconfiguration library
 *
 * Author: Matt Porter <mporter@mvista.com>
 *
 * Copyright 2000 MontaVista Software Inc.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */
//------------------------------------------------------
#include "pci.h"
#include "stdio.h"


#define    PCIAUTO_IDE_MODE_MASK        0x05

/* the user can define CONFIG_SYS_PCI_CACHE_LINE_SIZE to avoid problems */
#ifndef CONFIG_SYS_PCI_CACHE_LINE_SIZE
#define CONFIG_SYS_PCI_CACHE_LINE_SIZE    8
#endif

//----pcie�Զ�ɨ��region��ʼ��-----------------------------------------------------------------
//���ܣ�pcie�Զ�ɨ��region��ʼ��
//������struct pci_region *res��pci���������IO�ռ䡢�洢�ռ䡢Ԥ���ռ�ȵĽṹ��
//���أ���
//-----------------------------------------------------------------------------
void pciauto_region_init(struct pci_region *res)
{
    /*
     * Avoid allocating PCI resources from address 0 -- this is illegal
     * according to PCI 2.1 and moreover, this is known to cause Linux IDE
     * drivers to fail. Use a reasonable starting value of 0x1000 instead.
     */
    res->bus_lower = res->bus_start ? res->bus_start : 0x1000;
}
//----pcie�Զ�ɨ��region����-----------------------------------------------------------------
//���ܣ�pcie�Զ�ɨ��region����
//������struct pci_region *res��pci���������IO�ռ䡢�洢�ռ䡢Ԥ���ռ�ȵĽṹ��
//pci_size_t size�������С��һ��1MB����
//���أ���
//-----------------------------------------------------------------------------
void pciauto_region_align(struct pci_region *res, pci_size_t size)
{
    res->bus_lower = ((res->bus_lower - 1) | (size - 1)) + 1;
}
//----pcie�Զ�ɨ���pcie�豸region�ռ����-----------------------------------------------------------------
//���ܣ�pcie�Զ�ɨ��region�ռ����
//������struct pci_region *res��pci���������IO�ռ䡢�洢�ռ䡢Ԥ���ռ�ȵĽṹ��
//pci_size_t size�������pcie�豸�Ŀռ��С��pci_addr_t *bar��������pcie�豸�ĵ�ַ
//���أ�0��ʾ����ɹ���-1��ʾ����ʧ��
//-----------------------------------------------------------------------------
s32 pciauto_region_allocate(struct pci_region *res, pci_size_t size,
    pci_addr_t *bar)
{
    pci_addr_t addr;

    if (!res) {
        pcie_debug("No resource");
        goto error;
    }

    addr = ((res->bus_lower - 1) | (size - 1)) + 1;

    if (addr - res->bus_start + size > res->size) {
        pcie_debug("No room in resource");
        goto error;
    }

    res->bus_lower = addr + size;

    pcie_debug("address=0x%llx bus_lower=0x%llx", (u64)addr, (u64)res->bus_lower);

    *bar = addr;
    return 0;

 error:
    *bar = (pci_addr_t)-1;
    return -1;
}

//----pcie�Զ�ɨ��֮ǰ�����豸������-----------------------------------------------------------------
//���ܣ�pcie�Զ�ɨ��֮ǰ�������豸����
//������struct pci_region *res��pci���������IO�ռ䡢�洢�ռ䡢Ԥ���ռ�ȵĽṹ��
//pci_dev_t dev,�����豸��BDF�ţ�int sub_bus�������bus��
//���أ���
//-----------------------------------------------------------------------------
void pciauto_prescan_setup_bridge(struct pci_controller *hose,
                     pci_dev_t dev, s32 sub_bus)
{
    struct pci_region *pci_mem = hose->pci_mem;
    struct pci_region *pci_prefetch = hose->pci_prefetch;
    struct pci_region *pci_io = hose->pci_io;
    u16 cmdstat;

    pci_hose_read_config_word(hose, dev, PCI_COMMAND, &cmdstat);

    /* Configure bus number registers */
    pci_hose_write_config_byte(hose, dev, PCI_PRIMARY_BUS,
                   PCI_BUS(dev) - hose->first_busno);

    pci_hose_write_config_byte(hose, dev, PCI_SECONDARY_BUS,
                   sub_bus - hose->first_busno);
    pci_hose_write_config_byte(hose, dev, PCI_SUBORDINATE_BUS, 0xff);
    if (pci_mem) {
        /* Round memory allocator to 1MB boundary */
        pciauto_region_align(pci_mem, 0x100000);
        /* Set up memory and I/O filter limits, assume 32-bit I/O space */
        pci_hose_write_config_word(hose, dev, PCI_MEMORY_BASE,
                    (pci_mem->bus_lower & 0xfff00000) >> 16);

        cmdstat |= PCI_COMMAND_MEMORY;
    }

    if (pci_prefetch) {
        /* Round memory allocator to 1MB boundary */
        pciauto_region_align(pci_prefetch, 0x100000);

        /* Set up memory and I/O filter limits, assume 32-bit I/O space */
        pci_hose_write_config_word(hose, dev, PCI_PREF_MEMORY_BASE,
                    (pci_prefetch->bus_lower & 0xfff00000) >> 16);

        cmdstat |= PCI_COMMAND_MEMORY;
    } else {
        /* We don't support prefetchable memory for now, so disable */
        pci_hose_write_config_word(hose, dev, PCI_PREF_MEMORY_BASE, 0x1000);
        pci_hose_write_config_word(hose, dev, PCI_PREF_MEMORY_LIMIT, 0x0);
    }

    if (pci_io) {
        /* Round I/O allocator to 4KB boundary */
        pciauto_region_align(pci_io, 0x1000);
        pci_hose_write_config_byte(hose, dev, PCI_IO_BASE,
                    (pci_io->bus_lower & 0x0000f000) >> 8);
        pci_hose_write_config_word(hose, dev, PCI_IO_BASE_UPPER16,
                    (pci_io->bus_lower & 0xffff0000) >> 16);

        cmdstat |= PCI_COMMAND_IO;
    }

    /* Enable memory and I/O accesses, enable bus master */
    pci_hose_write_config_word(hose, dev, PCI_COMMAND,
                    cmdstat | PCI_COMMAND_MASTER);
}
//----pcie�Զ�ɨ��֮������豸������-----------------------------------------------------------------
//���ܣ�pcie�Զ�ɨ��֮��������豸����
//������struct pci_region *res��pci���������IO�ռ䡢�洢�ռ䡢Ԥ���ռ�ȵĽṹ��
//pci_dev_t dev,�����豸��BDF�ţ�int sub_bus�������bus��
//���أ���
//-----------------------------------------------------------------------------
void pciauto_postscan_setup_bridge(struct pci_controller *hose,
                      pci_dev_t dev, s32 sub_bus)
{
    struct pci_region *pci_mem = hose->pci_mem;
    struct pci_region *pci_prefetch = hose->pci_prefetch;
    struct pci_region *pci_io = hose->pci_io;

    /* Configure bus number registers */
    pci_hose_write_config_byte(hose, dev, PCI_SUBORDINATE_BUS,
                   sub_bus - hose->first_busno);

    if (pci_mem) {
        /* Round memory allocator to 1MB boundary */
        pciauto_region_align(pci_mem, 0x100000);

        pci_hose_write_config_word(hose, dev, PCI_MEMORY_LIMIT,
                (pci_mem->bus_lower - 1) >> 16);
    }

    if (pci_prefetch) {
        /* Round memory allocator to 1MB boundary */
        pciauto_region_align(pci_prefetch, 0x100000);

        pci_hose_write_config_word(hose, dev, PCI_PREF_MEMORY_LIMIT,
                (pci_prefetch->bus_lower - 1) >> 16);
    }

    if (pci_io) {
        /* Round I/O allocator to 4KB boundary */
        pciauto_region_align(pci_io, 0x1000);

        pci_hose_write_config_byte(hose, dev, PCI_IO_LIMIT,
                ((pci_io->bus_lower - 1) & 0x0000f000) >> 8);
        pci_hose_write_config_word(hose, dev, PCI_IO_LIMIT_UPPER16,
                ((pci_io->bus_lower - 1) & 0xffff0000) >> 16);
    }
}
