#ifndef _MIRROR_DISPLAY_H
#define _MIRROR_DISPLAY_H

#include "rfb.h"


#ifdef  __cplusplus
extern "C"{
#endif 

//��װ������ʾ��
bool_t install_vncserver_mirrordisplay(rfbScreenInfoPtr rfbserver);

void  uninstall_vncserver_mirrrordisplay(rfbScreenInfoPtr rfbserver);


#ifdef __cplusplus
}
#endif


#endif
