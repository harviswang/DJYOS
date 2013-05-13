/*
 * arm_mode_s_file.h
 *
 *  Created on: 2011-12-4
 *      Author: dmg
 */

#ifndef ARM_MODE_S_FILE_H_
#define ARM_MODE_S_FILE_H_

#define     SF_MODEMASK		0x1f

#define		SF_IRQ_BIT		0x80
#define		SF_FIQ_BIT		0x40

#define     SF_USERMODE		0x10
#define     SF_SYSMODE		0x1f
#define     SF_FIQMODE		0x11
#define     SF_IRQMODE		0x12
#define     SF_SVCMODE		0x13
#define     SF_ABORTMODE	0x17
#define     SF_UNDEFMODE	0x1b

#endif /* ARM_MODE_S_FILE_H_ */
