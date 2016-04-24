// =======================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// ģ������: VNCServer
// ģ��汾: V1.00(��ʼ���汾)
// �ļ���     ��random.c
// ������Ա: Administrator
// ����ʱ��: 2013-3-7
// =======================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <V1.00> <2013-3-7>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =======================================================================
#include <stdlib.h>


#include "authencode.h"
#include "D3des.h"

void RandomData(rfbClientPtr cl)
{
  int i=0;
  static unsigned  int j=0;
  srand(j);
  j++;
  for(i=0;i<CHALLENGESIZE;i++)
  {
	  cl->authChallenge[i]= (u8)((rand())%256);
  }
}

/*
 * Encrypt CHALLENGESIZE bytes in memory using a password.
 */

void rfbEncryptBytes(unsigned char *bytes, char *passwd)
{
    unsigned char key[8];
    unsigned int i;

    /* key is simply password padded with nulls */

    for (i = 0; i < 8; i++) {
	if (i < strlen(passwd)) {
	    key[i] = passwd[i];
	} else {
	    key[i] = 0;
	}
    }

    rfbDesKey(key, EN0);

    for (i = 0; i < CHALLENGESIZE; i += 8) {
	rfbDes(bytes+i, bytes+i);
    }
}
