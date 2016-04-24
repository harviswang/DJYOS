// =======================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// ģ������: VNCServer
// ģ��汾: V1.00(��ʼ���汾)
// �ļ���     ��clip_process.h
// ������Ա: Administrator
// ����ʱ��: 2013-6-9
// =======================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <V1.00> <2013-3-5>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =======================================================================

#ifndef __CLIP_PROCESS_H__
#define __CLIP_PROCESS_H__

#include "stdint.h"

void vnc_set_vncclip_pool(struct tagMemCellPool  *newpool);

struct tagClipRect *get_vncclip_space();

void free_vncclip_space(struct tagClipRect *clip);

void free_vnccliplink_space(struct tagClipRect *clip);


void connect_clip2cliplink(struct tagClipRect **mlink,struct tagClipRect *sub);

struct tagClipRect*  remove_clip_from_cliplink(struct tagClipRect **mlink);

void anastomose_vncclip(struct tagClipRect **head, struct tagClipRect *newclip);

struct tagClipRect * combine_vnccliplink(struct tagClipRect *clipq);

//����ʹ�ú���
bool_t check_clip_queque(struct tagClipRect *cliphead);

u16 count_clip_number(struct tagClipRect *cliphead);

void show_clip_queque(struct tagClipRect *clip);


#endif /* __CLIP_PROCESS_H__ */
