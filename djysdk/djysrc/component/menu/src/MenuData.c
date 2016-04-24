//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice, 
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice, 
//    this list of conditions and the following disclaimer in the documentation 
//    and/or other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
// 
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
// 
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
// ģ������: DJY�˵�ģ��֮�û��˵���Ը����
// ģ��汾: V1.00(��ʼ���汾)
// ������Ա: zqf
// ����ʱ��: 20121224
// =======================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =======================================================================


#include  "MenuData.h"
#include  "MenuFunction.h"


// =========================================================================
// ˵������Ŀǰ�׶Σ����õ��Ǿ�̬����ķ�ʽ���洢�û��Ĳ˵�������Ը�����ڲҵ���Ը�ĸ���
//      ������Բο���ؽṹ��Ķ��壬0,1,2,3������ǲ˵��Ĳ�νṹ�����һ������Ϊ��
//      �����Ž���
// =========================================================================
const  tagMenuitemModel g_tMenuitemModelTab[]=
{
    {   0,      "�鿴",         "",                 0,                      0x0000,         0,              "Menuitem_func"            },
    {   1,      "װ����Ϣ",     "�鿴װ����Ϣ",     0,                      0x0000,         0,              "CheckEquipment"      },
    {   1,      "������",       "�鿴������",       EN_MEA_RLY,             0x0000,         EN_ANA_0,      "Menuitem_func"       },
    {   1,      "������",       "�鿴������",       EN_MEA_INPUT1,          0x0000,         EN_INPUT_0,     "Menuitem_func"       },
    {   1,      "��ֵ",         "�鿴��ֵ",         0,                      0x0000,         0,              "Menuitem_func"            },
    {     2,    "�豸����",     "",                 EN_FIGURE_SET,          0x0000,         EN_SIDE_DEVINF, "Menuitem_func"          },
    {     2,    "�����",     "�鿴�������ֵ", 0,                      0x0000,         0,                  "Menuitem_func"         },
    {       3,    "��ֵ��ֵ",     "", EN_FIGURE_SET,          0x0000,  EN_SIDE_ALL, "Menuitem_func"         },
    {       3,    "������",       "", EN_SOFT_SET,            0x0000,  EN_SIDE_ALL, "Menuitem_func"         },
    {     2,    "��ѹ��󱸱���", "�鿴��ֵ",                           0,          0x0000,         0,                 "Menuitem_func"            },
    {       3,    "��ֵ��ֵ",     "", EN_FIGURE_SET,          0x0000,  EN_SIDE_HIGH, "Menuitem_func"        },
    {       3,    "������",       "", EN_SOFT_SET,            0x0000,  EN_SIDE_HIGH, "Menuitem_func"         },
    {     2,    "��ѹ��󱸱���", "�鿴��ֵ",                           0,          0x0000,         0,                  "Menuitem_func"            },
    {       3,    "��ֵ��ֵ",     "", EN_FIGURE_SET,          0x0000,  EN_SIDE_MED1, "Menuitem_func"         },
    {       3,    "������",       "", EN_SOFT_SET,            0x0000,  EN_SIDE_MED1, "Menuitem_func"         },
    {     2,    "��1��֧�󱸱���", "�鿴��ֵ",                           0,         0x0000,         0,                  "Menuitem_func"            },
    {       3,    "��ֵ��ֵ",     "", EN_FIGURE_SET,          0x0000,  EN_SIDE_LOW1, "Menuitem_func"         },
    {       3,    "������",       "", EN_SOFT_SET,            0x0000,  EN_SIDE_LOW1, "Menuitem_func"         },
    {     2,    "��2��֧�󱸱���", "�鿴��ֵ",                           0,         0x0000,         0,                 "Menuitem_func"          },
    {       3,    "��ֵ��ֵ",     "", EN_FIGURE_SET,          0x0000,  EN_SIDE_LOW2, "Menuitem_func"         },
    {       3,    "������",       "", EN_SOFT_SET,            0x0000,  EN_SIDE_LOW2, "Menuitem_func"         },
//    {     2,    "�翹���󱸱���",   "�鿴��ֵ",                          0,         0x0000,         0,                  "Menuitem_NonPfunc"            },
//    {       3,    "��ֵ��ֵ",     "", EN_FIGURE_SET,          0x0000,  EN_SIDE_LK,     "MenuitemProc_See_Set"         },
//    {       3,    "������",       "", EN_SOFT_SET,            0x0000,  EN_SIDE_LK,     "MenuitemProc_See_Set"         },
//    {     2,    "�Զ��嶨ֵ",     "�鿴��ֵ",                           0,          0x0000,         0,                  "Menuitem_NonPfunc"            },
//    {       3,    "��ֵ��ֵ",     "", EN_FIGURE_SET,          0x0000,  EN_SIDE_DEF, "MenuitemProc_See_Set"         },
//    {       3,    "������",       "", EN_SOFT_SET,            0x0000,  EN_SIDE_DEF, "MenuitemProc_See_Set"         },
    {     2,    "��բ����",       "�鿴�����ֶ�ֵ", EN_FIGURE_SET,          0x0000,  EN_SIDE_MATRIX,    "Menuitem_func"         },
//  {     2,    "���Ҷ�ֵ",       "�鿴�����ֶ�ֵ", EN_SOFT_SET,            0x0000,  EN_SIDE_COP,        "Menuitem_func"         }
    {   1,      "��ѹ��",       "",             0, 0x0000,         0,              "Menuitem_func"            },
    {   2,      "������ѹ��",   "�鿴����������ѹ��",   0,                  0x0000,         EN_SOFT_PRO,   "Menuitem_func"         },     
    {   2,      "GOOSE��ѹ��",  "�鿴GOOSE��ѹ��",      0,                  0x0000,         EN_SOFT_GOOSE,  "Menuitem_func"         },
    {   1,      "ң����Ϣ",     "�鿴ң����Ϣ",     0,                      0x0000,         0,              "Menuitem_func"             },
    {     2,    "������",       "�鿴������",       EN_MEA_AC,              0x0000,         EN_INPUT_0,     "Menuitem_func"           },
//    {     2,    "ֱ����",       "�鿴ֱ����",       EN_MEA_DC,              0x0000,         EN_INPUT_0,     "MenuitemProc_See_YC"           },

 
    {     2,    "�����",       "�鿴�����",       EN_MEA_POWER,           0x0000,         EN_INPUT_0,     "Menuitem_func"           },
     {     2,    "г����",       "�鿴г����",       EN_MEA_SYN,             0x0000,         EN_INPUT_0,     "Menuitem_func"       },
  
    {     2,    "��λ��",       "�鿴��λ��",       EN_MEA_GEAR,            0x0000,         EN_INPUT_0,     "Menuitem_func"           },
    {   1,      "ң����Ϣ",     "�鿴ң����Ϣ",     EN_MEA_YX,              0x0000,         EN_INPUT_0,    "Menuitem_func"        },
    {   1,      "������Ϣ",     "�鿴������Ϣ",     EN_MEA_LS,              0x0000,         0,             "Menuitem_func"           },
    {   1,      "˳����Ϣ",     "�鿴˳����Ϣ",     0,                      0x0000,         0,             "Menuitem_func"             },
    {     2,    "����Ʊ������Ϣ",   "�鿴����Ʊ������Ϣ",   EN_MEA_SCRLY,   0x0000,         0,              "Menuitem_func"     },
    {   1,  "װ�ü�¼",     "�鿴����װ�ü�¼",          0,                      0x0000,         0,                  "Menuitem_func"            },
    {     2,    "���б���",     "",                 0,                      0x0000,         0,              "Menuitem_func"    },
    {     2,    "��������",     "",                 0,                      0x0000,         0,              "Menuitem_func"       },
    {     2,    "�����澯",     "",                 0,                      0x0000,         0,              "Menuitem_func"      },
//    {     2,    "����Խ��",     "",                 0,                      0x0000,         0,            "Menuitem_func"     },
    {     2,    "��������",     "",                 0,                      0x0000,         0,             "Menuitem_func"    },
//  {     2,    "����Ͷ��",     "",                 0,                      0x0000,         0,              "MenuitemProc_See_RecOnoff"     },
    {     2,    "SOE��λ",      "",                 0,                      0x0000,         0,              "Menuitem_func"       },
    {     2,    "ң�ؼ�¼",     "�鿴ң�ز�����¼", 0,                      0x0000,         0,              "Menuitem_func"        },
    {     2,    "˳�ؼ�¼",     "�鿴˳��ִ�м�¼", 0,                      0x0000,         0,             "Menuitem_func"       },
    {     2,    "˳�ص���",     "�鿴˳�ص�����Ϣ", 0,                      0x0000,         0,             "Menuitem_func"},
    {     2,    "װ���Լ�",     "",                 0,                      0x0000,         0,              "Menuitem_func"       },
    {     2,    "װ������",     "",                 0,                      0x0000,         0,              "Menuitem_func"      },
//    {     2,    "˲ʱ����",     "",                 0,                      0x0000,         0,              "MenuitemProc_See_RecLock"      },
    {     2,    "װ�ò���",     "",                 0,                      0x0000,         0,              "Menuitem_func"       },
    {   1,      "¼����Ϣ",     "�鿴����¼����Ϣ", 0,                      0x0000,         0,              "Menuitem_func"            },
    {     2,    "����¼��",     "",                 EN_ACTWAVE_REC,         0x0000,         0,             "Menuitem_func"         },
    {     2,    "��¼��",     "",                 EN_STARTWAVE_REC,       0x0000,         0,              "Menuitem_func"         },
    {     2,    "�ֶ�¼��",     "",                 EN_HANDWAVE_REC,        0x0000,         0,              "Menuitem_func"        },
//    {  1,       "�Լ�״̬",     "",                 0,                      0x0000,         0,              "Menuitem_NonPfunc"             },
//    {     2,    "������",       "",                 0,                      0x0000,         0,              "MenuitemProc_See_ChkInfo"      },
//    {     2,    "�󱸱���",     "",                 1,                      0x0000,         0,              "MenuitemProc_See_ChkInfo"      },
    {0,       "����",     "",                               0,                      0x0000,         0            ,      "Menuitem_func"            },
    {   1,      "��ֵ����",     "",                 0,                      CN_USER_PWD,    0,              "Menuitem_func"  },
    {   1,      "�豸����",     "�����豸����",     EN_FIGURE_SET,          CN_USER_PWD,    EN_SIDE_DEVINF, "Menuitem_func"        },
    {   1,      "�����",     "�����������ֵ", 0,                      0x0000,         0,              "Menuitem_func"             },
    {     2,    "��ֵ��ֵ",     "",                 EN_FIGURE_SET,          CN_USER_PWD,    EN_SIDE_ALL,   "Menuitem_func"        },
    {     2,    "������",       "",                 EN_SOFT_SET,            CN_USER_PWD,    EN_SIDE_ALL,    "Menuitem_func"        },
    {   1,      "��ѹ��󱸱���",   "������ֵ",     0,                      0x0000,         0,              "Menuitem_func"            },
    {     2,    "��ֵ��ֵ",     "",                 EN_FIGURE_SET,          CN_USER_PWD,    EN_SIDE_HIGH,   "Menuitem_func"        },
    {     2,    "������",       "",                 EN_SOFT_SET,            CN_USER_PWD,    EN_SIDE_HIGH,   "Menuitem_func"       },
    {   1,      "��ѹ��󱸱���",   "������ֵ",     0,                      0x0000,         0,              "Menuitem_func"             },
    {     2,    "��ֵ��ֵ",     "",                 EN_FIGURE_SET,          CN_USER_PWD,    EN_SIDE_MED1,  "Menuitem_func"        },
    {     2,    "������",       "",                 EN_SOFT_SET,            CN_USER_PWD,    EN_SIDE_MED1,   "Menuitem_func"        },
    {   1,      "��1��֧�󱸱���",  "������ֵ",     0,                      0x0000,         0,              "Menuitem_func"             },
    {     2,    "��ֵ��ֵ",     "",                 EN_FIGURE_SET,          CN_USER_PWD,    EN_SIDE_LOW1,   "Menuitem_func"        },
    {     2,    "������",       "",                 EN_SOFT_SET,            CN_USER_PWD,    EN_SIDE_LOW1,  "Menuitem_func"       },
    {   1,      "��2��֧�󱸱���",  "������ֵ",     0,                      0x0000,         0,              "Menuitem_func"             },
    {     2,    "��ֵ��ֵ",     "",                 EN_FIGURE_SET,          CN_USER_PWD,    EN_SIDE_LOW2,   "Menuitem_func"       },
    {     2,    "������",       "",                 EN_SOFT_SET,            CN_USER_PWD,    EN_SIDE_LOW2,   "Menuitem_func"       },
//    {   1,      "�翹���󱸱���",   "������ֵ",     0,                      0x0000,         0,              "Menuitem_NonPfunc"             },
//    {     2,    "��ֵ��ֵ",     "",                 EN_FIGURE_SET,          CN_USER_PWD,    EN_SIDE_LK,     "MenuitemProc_Set_Value"        },
//    {     2,    "������",       "",                 EN_SOFT_SET,            CN_USER_PWD,    EN_SIDE_LK,     "MenuitemProc_Set_Value"        },
//    {   1,      "�Զ��嶨ֵ",   "������ֵ",         0,                      0x0000,         0,              "Menuitem_NonPfunc"             },
//    {     2,    "��ֵ��ֵ",     "",                 EN_FIGURE_SET,          CN_USER_PWD,    EN_SIDE_DEF,    "MenuitemProc_Set_Value"        },
//    {     2,    "������",       "",                 EN_SOFT_SET,            CN_USER_PWD,    EN_SIDE_DEF,    "MenuitemProc_Set_Value"        },
    {   1,      "��բ����",     "������բ����",     EN_FIGURE_SET,          CN_USER_PWD,    EN_SIDE_MATRIX, "Menuitem_func"        },
   
    {     2,    "������ѹ��",   "���ñ���������ѹ��",   0,                  CN_USER_PWD,    EN_SOFT_PRO,    "Menuitem_func"         },
    {     2,    "GOOSE��ѹ��",  "����GOOSE��ѹ��",  0,                      CN_USER_PWD,    EN_SOFT_GOOSE,  "Menuitem_func"        },
    {   1,      "��ֵ����",     "",                 0,                      CN_USER_PWD,    0,              "Menuitem_func"      },
    {   0,      "Ԥ��",         "",                         0,              0x0000,         0,             "Menuitem_func"            },
    {   1,      "ʱ������",     "����ϵͳʱ��",             0,              CN_USER_PWD,    0,              "Menuitem_func"         },
    {   1,  "MU��������",   "����MU����ֵ",                 0,                      0x0000,         0,                  "Menuitem_func"             },
//    {   2,  "ͬ����ʱ",     "����MUͬ����ʱ",               0,                      CN_USER_PWD,    0,                  "MenuitemProc_Cfg_MuDelay"},  //modified by zhanggl.110624
//    {   2,  "ͬ��ʱ��Դ",   "��������ͬ��ʱ��Դ",           0,                      CN_USER_PWD,    0,                  "MenuitemProc_Cfg_MuSyn"},  //modified by zhanggl.111027
    {   2,  "MUͨ������",   "����MU����ֵͨ���Ľ��뼫��",   0,                      CN_USER_PWD,    0,             "Menuitem_func"    },  //...liuyf:2010-9-3
//    {   1,      "��λ��׼",     "���ý�������ʾʱ��λ��׼", 0,              CN_USER_PWD,    0,              "MenuitemProc_Cfg_AngleBase"    },
    {   1,      "ң������",     "����ң�س�������",         0,              CN_USER_PWD,    0,              "Menuitem_func"      },
    {   1,      "ң������",     "ң���������",             0,              0x0000,         0,              "Menuitem_func"            },
    {     2,    "���ʼ��㷽ʽ", "���ù��ʼ��㷽ʽ",         0,              CN_USER_PWD,    0,              "Menuitem_func"      },
    {     2,    "ֱ������������",   "����ֱ���������Ĳ���", 0,              CN_USER_PWD,    0,              "Menuitem_func"     },
    {     2,    "��λ����������",   "���õ�λ�������Ĳ���", 0,              CN_USER_PWD,    0,              "Menuitem_func"     },
    {     2,    "PTCT����",     "����PTCT �ֵ",          0,              CN_USER_PWD,    0,              "Menuitem_func"      },
    {     2,    "ͨѶ�������", "����ң������ͨѶ�������", 0,              CN_USER_PWD,    0,              "Menuitem_func"   },
    {     2,    "ң���������ż�",   "����ң���������ż�",   0,              CN_USER_PWD,    0,              "Menuitem_func"   },
    {     2,    "ң����΢��ϵ��",   "����ң������΢��ϵ��", EN_MEA_ADJ,     CN_USER_PWD,    EN_ANA_0,       "Menuitem_func"     },
    {     1,    "ң������",     "ң�Ų�������",             0,              0x0000,         0,             "Menuitem_func"             },
    {     2,    "ң������",     "����ң������",             0,              CN_USER_PWD,    0,             "Menuitem_func"   },
    {     2,    "ң�ŷ���ʱ��", "����ң�ŷ���ʱ��",         0,              CN_USER_PWD,    0,             "Menuitem_func"     },
    {     2,    "˫��ң��",     "����˫��ң�������",       0,              CN_USER_PWD,    0,              "Menuitem_func"      },
    {   1,      "���⹦��",     "���ø�������ֵ",           0,              0x0000,         0,              "Menuitem_func"            },
    {     2,    "����ң��",     "��������ң��ֵ",           0,              0x0000,         0,             "Menuitem_func"            },
    {       3,  "������ң��",   "�������⽻��ң��ֵ",       EN_MEA_AC,      CN_USER_PWD,    EN_ANA_0,       "Menuitem_func"       },
    {       3,  "ֱ����ң��",   "��������ֱ��ң��ֵ",       EN_MEA_DC,      CN_USER_PWD,    EN_ANA_0,       "Menuitem_func"         },
    {       3,  "�����ң��",   "����������ң��ֵ",       EN_MEA_POWER,   CN_USER_PWD,    EN_ANA_0,       "Menuitem_func"        },
    {       3,  "��λ��ң��",   "�������⵵λң��ֵ",       EN_MEA_GEAR,    CN_USER_PWD,    EN_ANA_0,       "Menuitem_func"        },
    {     2,    "����ң��",     "��������ң��ֵ",           0,              CN_USER_PWD,    0,              "Menuitem_func"         },
    {     2,    "�����¼�",     "���������¼�",             0,              0x0000,         0,              "Menuitem_func"            },
    {       3,  "�����¼�",     "�������⶯���¼�",         EN_ACT_REC,     CN_USER_PWD,    0,              "Menuitem_func"      },
    {       3,  "�澯�¼�",     "��������澯�¼�",         EN_ALM_REC,     CN_USER_PWD,    0,              "Menuitem_func"      },
    {   1,      "���˳��",     "",                         0,              0x0000,         0,              "Menuitem_func"            },
    {     2,    "���Ͷ��",     "���Ͷ��������",           0,              CN_USER_PWD,    0,              "Menuitem_func"       },
    {     2,    "���������",   "���ø�ͨ�����Ͷ��",       0,              CN_USER_PWD,    0,             "Menuitem_func"    },
    {     2,    "˳��Ͷ��",     "˳��Ͷ��������",           0,              CN_USER_PWD,    0,              "Menuitem_func"       },
    {   1,      "GOCB������",  "����GOCB�������ʹ��",      0,              CN_USER_PWD,    0,             "Menuitem_func"  },  //����GOCB�������ʹ��
    {   1,      "��������",     "",                         0,              0x0000,         0,              "Menuitem_func"             },
//    {     2,    "�����¼�",     "���ɸ���δ�����¼�",       EN_REV_FREE,    CN_USER_PWD,    0,              "MenuitemProc_Cfg_RevEvent"     },
    {     2,    "ǿ�Ƹ���",     "�ɸ���δ�����¼�",         EN_REV_FORCE,   CN_USER_PWD,    0,              "Menuitem_func"    },
    {     2,    "�ֶ�¼��",     "�����ֶ�¼��",             0,              CN_USER_PWD,    0,              "Menuitem_func"  },
    {     2,    "�����¼",     "",                         0,              CN_USER_PWD,    0,              "Menuitem_func"       },
    {   1,      "ͨѶ����",     "",                         0,              0x0000,         0,              "Menuitem_func"           },
    {     2,    "ͨѶ����",     "�ⲿͨѶ����",             0,              CN_USER_PWD,    0,              "Menuitem_func"     },
    {     2,    "װ�ñ��",     "����װ�ú�",               0,              CN_USER_PWD,    0,              "Menuitem_func"   },
//    {     2,    "װ�ü���",     "Ͷ��ʱ���ϴ��¼�",         0,              CN_USER_PWD,    0,              "MenuitemProc_Cfg_DevRepair"    },
    {     2,    "��������",     "",                         0,              CN_USER_PWD,    0,              "Menuitem_func"       },
    {   0,      "����",         "",                         0,              0x0000,         0,             "Menuitem_func"             },
//    {   1,      "������",       "",                         0,              0x0000,         0,              "Menuitem_NonPfunc"             },
//    {   1,      "������",       "",                         EN_MEA_RLY,     0x0000,         EN_ANA_0,       "MenuitemProc_Dbg_AnaValue"     },
//    {     2,    "�󱸱���",     "",                         EN_MEA_RLY2,    0x0000,         EN_ANA_1,       "MenuitemProc_Dbg_AnaValue"     },
//    {   1,      "������",       "",                         0,              0x0000,         0,              "Menuitem_NonPfunc"             },
//    {   1,      "������",       "",                         0,              0x0000,         EN_INPUT_0,     "MenuitemProc_Dbg_InputValue"   },
//    {     2,    "�󱸱���",     "",                         0,              0x0000,         EN_INPUT_1,     "MenuitemProc_Dbg_InputValue"   },
    {   1,      "��������",     "����˲˵������˳�",       EN_OUTPUT_TRIP, CN_USER_PWD,    EN_INPUT_0,     "Menuitem_func"       },
    {   1,      "�źų���",     "����˲˵������˳�",       EN_OUTPUT_SIGN, CN_USER_PWD,    0,              "Menuitem_func"        },
    {   0,      "��ӡ",         "",                         0,                  0x0000,     0,             "Menuitem_func"           },
    {   1,      "װ����Ϣ",     "",                         0,                  0x0000,     0,             "Menuitem_func"         },
    {   1,      "������",       "",                         0,                  0x0000,     0,              "Menuitem_func"          },
//    {   1,      "����״̬",     "",                         0,                  0x0000,     0,              "Menuitem_Prn_InputInfo"        },
    {   1,      "��ֵ��Ϣ",     "",                         0,                  0x0000,     0,              "Menuitem_func"       },
    {   1,      "������¼",     "",                         0,                  0x0000,     0,              "Menuitem_func"       },
    {   1,      "������Ϣ",     "",                         0,                  0x0000,     0,              "Menuitem_func"      },
//    {   1,      "��������",     "",                         0,                  0x0000,     0,              "MenuitemProc_Prn_PrnAct"       },
    {   1,      "���б���",     "",                         EN_PRINT_RUN_RPT,   0x0000,     0,              "Menuitem_func"      },
    {   1,      "�¹ʱ���",     "",                         EN_PRINT_ACT_RPT,   0x0000,     0,             "Menuitem_func"      },
    {   1,      "ȡ����ӡ",     "",                         0,                  0x0000,     0,             "Menuitem_func"          },
    {   0,  "����",             "",                         0,                  0x0000,     0,             "Menuitem_func"             },
    {   1,  "������ʾ��ʽ",     "",                         0,                  0x0000,     0,              "Menuitem_func"  },
    {   1,  "������Ϣ",         "�鿴���ֵ�����Ϣ",         0,                  CN_COP_PWD, EN_NO_USER_PWD, "Menuitem_func"      },
    {   1,  "�ָ�Ĭ��",         "",                         0,                  0x0000,     0,              "Menuitem_func"             },
    {   2,  "�ָ�Ĭ������",     "",                         0,                  CN_COP_PWD, EN_NO_USER_PWD, "Menuitem_func"     },
    {   2,  "��ֵ",             "",                         0,                  CN_COP_PWD, EN_NO_USER_PWD, "Menuitem_func"      },
    {   2,  "��ѹ��",           "",                         0,                  CN_COP_PWD, EN_NO_USER_PWD, "Menuitem_func"     },
    {   1,  "˲ʱ����",     "",                             0,                 0x0000,         0,                  "Menuitem_func"      },
    {   1,  "����汾��Ϣ",  "�鿴����汾��Ϣ",         0,                    0x0000,         0,              "Menuitem_func"      },
    {   2,  "����汾��Ϣ",  "�鿴����汾��Ϣ",         0,                    0x0000,         0,              "Menuitem_func"      },
    {   3,  "����汾��Ϣ",  "�鿴����汾��Ϣ",         0,                    0x0000,         0,              "Menuitem_func"      },
    {   4,  "����汾��Ϣ",  "�鿴����汾��Ϣ",         0,                    0x0000,         0,              "Menuitem_func"      },
    {   0,  "��1",  "�鿴����汾��Ϣ",         0,                    0x0000,         0,              "Menuitem_func"      },
    {   0,  "��2",  "�鿴����汾��Ϣ",         0,                    0x0000,         0,              "Menuitem_func"      },
    {   0,  "��3",  "�鿴����汾��Ϣ",         0,                    0x0000,         0,              "Menuitem_func"      },
	//{   0,  "��4",  "�鿴����汾��Ϣ",         0,                    0x0000,         0,              "Menuitem_func"      },
    //{   0,  "��5",  "�鿴����汾��Ϣ",         0,                    0x0000,         0,              "Menuitem_func"      },
    //{   0,  "��6",  "�鿴����汾��Ϣ",         0,                    0x0000,         0,              "Menuitem_func"      },
	{   0,  "",  "",         0,                    0x0000,         0,              ""      },
};





