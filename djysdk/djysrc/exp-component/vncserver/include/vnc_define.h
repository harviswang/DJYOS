// =============================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ����ƣ�vnc_define.h
// ģ������: 
// ģ��汾: V1.00
// ����ʱ��: 2013-7-5
// ԭʼ����: Administrator
// ����˵����  
// =============================================================================

#ifndef VNC_DEFINE_H_
#define VNC_DEFINE_H_

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))
#define abs(a) ((a) > 0 ? (a) : -(a))

#define KeySym      u32
#define TRUE             1
#define FALSE            0
#define Swap16(s) ((((s) & 0xff) << 8) | (((s) >> 8) & 0xff))
#define Swap32(l) (((l) >> 24) | \
                   (((l) & 0x00ff0000) >> 8)  | \
                   (((l) & 0x0000ff00) << 8)  | \
                   ((l) << 24))

static const char rfbLittleEndian =TRUE;

#define Swap16IfLE(s) (rfbLittleEndian? Swap16(s) : (s))

#define Swap32IfLE(l) (rfbLittleEndian? Swap32(l) : (l))

#define swap16_if_need(value, need) (need? Swap16(value) : value)
#define swap32_if_need(value, need) (need? Swap32(value) : value)

#endif /* VNC_DEFINE_H_ */
