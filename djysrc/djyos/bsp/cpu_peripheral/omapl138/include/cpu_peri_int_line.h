//----------------------------------------------------
//Copyright (C), 2004-2009,  都江堰操作系统开发团队
//版权所有 (C), 2004-2009,   都江堰操作系统开发团队
//所属模块:中断模块
//作者：lst
//版本：V1.0.0
//文件描述: 定义中断号
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009--9-28
//   作者: lst
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifndef __CPU_PERI_INT_LINE__
#define __CPU_PERI_INT_LINE__

#ifdef __cplusplus
extern "C" {
#endif

#define cn_int_line_commtx          (0)
#define cn_int_line_commrx          (1)
#define cn_int_line_nint            (2)
#define cn_int_line_pru_ev0         (3)
#define cn_int_line_pru_ev1         (4)
#define cn_int_line_pru_ev2         (5)
#define cn_int_line_pru_ev3         (6)
#define cn_int_line_pru_ev4         (7)
#define cn_int_line_pru_ev5         (8)
#define cn_int_line_pru_ev6         (9)
#define cn_int_line_pru_ev7         (10)
#define cn_int_line_edma3_0_cc0     (11)
#define cn_int_line_edma3_0_cc0_err (12)
#define cn_int_line_edma3_0_tc0_err (13)
#define cn_int_line_emifa           (14)
#define cn_int_line_iic0            (15)
#define cn_int_line_mmcsd0_int0     (16)
#define cn_int_line_mmcsd0_int1     (17)
#define cn_int_line_psc0_allint     (18)
#define cn_int_line_rtc_irqs10      (19)
#define cn_int_line_spi0            (20)
#define cn_int_line_t64p0_tint12    (21)
#define cn_int_line_t64p0_tint34    (22)
#define cn_int_line_t64p1_tint12    (23)
#define cn_int_line_t64p1_tint34    (24)
#define cn_int_line_uart0           (25)
#define cn_int_line_reserved1       (26)
#define cn_int_line_proterr         (27)
#define cn_int_line_sfg_chip0       (28)
#define cn_int_line_sfg_chip1       (29)
#define cn_int_line_sfg_chip2       (30)
#define cn_int_line_sfg_chip3       (31)
#define cn_int_line_edma3_0_tc1_err (32)
#define cn_int_line_emac_c0rxthresh (33)
#define cn_int_line_emac_c0rx       (34) 
#define cn_int_line_emac_c0tx       (35)
#define cn_int_line_emac_c0misc     (36)
#define cn_int_line_emac_c1rxthresh (37)
#define cn_int_line_emac_c1rx       (38)
#define cn_int_line_emac_c1tx       (39)
#define cn_int_line_c1misc          (40)
#define cn_int_line_memerr          (41)
#define cn_int_line_gpiob0          (42)
#define cn_int_line_gpiob1          (43)
#define cn_int_line_gpiob2          (44)
#define cn_int_line_gpiob3          (45)
#define cn_int_line_gpiob4          (46)
#define cn_int_line_gpiob5          (47)
#define cn_int_line_gpiob6          (48)
#define cn_int_line_gpiob7          (49)
#define cn_int_line_gpiob8          (50)
#define cn_int_line_iic1            (51)
#define cn_int_line_lcdc            (52)
#define cn_int_line_uart1           (53)
#define cn_int_line_mcasp           (54)
#define cn_int_line_psc1_allint     (55)
#define cn_int_line_spi1            (56)
#define cn_int_line_uhpi_arm        (57)
#define cn_int_line_usb0            (58)
#define cn_int_line_usb1_hc         (59)
#define cn_int_line_usb1_r_wakeup   (60)
#define cn_int_line_uart2           (61)
#define cn_int_line_reserved2       (62)
#define cn_int_line_ehrpwm0         (63)
#define cn_int_line_ehrpwm0tz       (64)
#define cn_int_line_ehrpwm1         (65)
#define cn_int_line_ehrpwm1tz       (66)
#define cn_int_line_sata            (67)
#define cn_int_line_t64p2_all       (68)
#define cn_int_line_ecap0           (69)
#define cn_int_line_ecap1           (70)
#define cn_int_line_ecap2           (71)
#define cn_int_line_mmcad1_int0     (72)
#define cn_int_line_mmcsd1_int1     (73)
#define cn_int_line_t64p0_cmp0      (74)
#define cn_int_line_t64p0_cmp1      (75)
#define cn_int_line_t64p0_cmp2      (76)
#define cn_int_line_t64p0_cmp3      (77)
#define cn_int_line_t64p0_cmp4      (78)
#define cn_int_line_t64p0_cmp5      (79)
#define cn_int_line_t64p0_cmp6      (80)
#define cn_int_line_t64p0_cmp7      (81)
#define cn_int_line_t64p1_cmp0      (82)
#define cn_int_line_t64p1_cmp1      (83)
#define cn_int_line_t64p1_cmp2      (84)
#define cn_int_line_t64p1_cmp3      (85)
#define cn_int_line_t64p1_cmp4      (86)
#define cn_int_line_t64p1_cmp5      (87)
#define cn_int_line_t64p1_cmp6      (88)
#define cn_int_line_t64p1_cmp7      (89)
#define cn_int_line_armclkstopreq   (90)
#define cn_int_line_upp_all         (91)
#define cn_int_line_vpif_all        (92)
#define cn_int_line_edma3_1_cc0     (93)
#define cn_int_line_edma3_1_cc0_err (94)
#define cn_int_line_edma3_1_tc0_err (95)
#define cn_int_line_t64p3_all       (96)
#define cn_int_line_mcbsp0_r        (97)
#define cn_int_line_mcbsp0_x        (98)
#define cn_int_line_mcbsp1_r        (99)
#define cn_int_line_mcbsp1_x        (100)
#define cn_int_line_last            (100)

#define cn_int_msk_all_line         (0xffffffff)

#ifdef __cplusplus
}
#endif

#endif //__CPU_PERI_INT_LINE__

