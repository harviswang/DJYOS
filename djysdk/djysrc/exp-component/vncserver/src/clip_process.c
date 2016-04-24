// =======================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// ģ������: VNCServer
// ģ��汾: V1.00(��ʼ���汾)
// �ļ���  ��clip_process.c
// ������Ա: Administrator
// ����ʱ��: 2013-6-9
// ˵���ĵ������ļ���VNCclip��clip�����㷨��ʵ�֣�����ǽ����ڲ�ʹ�õĺ�����
//           �ܶ಻������ڲ��������ģ���ϣ����ʱ���˷��ڴˣ���Ϊʹ�û���
//           ������
// =======================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <V1.00> <2013-3-5>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =======================================================================
#include "clip_process.h"
#include "debug.h"

struct tagMemCellPool               *pg_vncclip_pool;
// the vnc clip pool,û�а취��Ϊ�˱���mirrordisplay���Ϲ�ϵ���ڴ�����ȫ�ֱ���

// =============================================================================
// ��������:����VNC���ڴ��
// �������:
// ���������
// ����ֵ  :
// =============================================================================

void vnc_set_vncclip_pool(struct tagMemCellPool  *newpool)
{
    pg_vncclip_pool = newpool;
}

// =============================================================================
// ��������:ΪVNCclip����洢�ռ䣬
// �������:
// ���������
// ����ֵ  :NULL��ʧ�ܣ�����ɹ�
// ˵��    :���Ǵ�VNC���ڴ�ط���Ŀռ䣬����һ�������£��벻Ҫ����ɾ����ʹ��API����
//          ��Ӧ�ķ����ɾ��
// =============================================================================
struct tagClipRect *get_vncclip_space()
{
    struct tagClipRect *clip = (struct tagClipRect *)Mb_Malloc(pg_vncclip_pool, 0);
    return clip;
}

// =============================================================================
// ��������:�ͷ�vncclip�Ĵ洢�ռ䣬
// �������:
// ���������
// ����ֵ  :NULL��ʧ�ܣ�����ɹ�
// ˵��    :���Ǵ�VNC���ڴ�ط���Ŀռ䣬����һ�������£��벻Ҫ����ɾ����ʹ��API����
//          ��Ӧ�ķ����ɾ��
// =============================================================================
void free_vncclip_space(struct tagClipRect *clip)
{
     Mb_Free(pg_vncclip_pool,clip);
}

// =============================================================================
// ��������:�ͷ�vncclip���еĴ洢�ռ䣬
// �������:
// ���������
// ����ֵ  :NULL��ʧ�ܣ�����ɹ�
// ˵��    :���Ǵ�VNC���ڴ�ط���Ŀռ䣬����һ�������£��벻Ҫ����ɾ����ʹ��API����
//          ��Ӧ�ķ����ɾ��
// =============================================================================
void free_vnccliplink_space(struct tagClipRect *clip)
{

    struct tagClipRect *temp,*temp1;
    if(clip != NULL)
    {
        temp = clip;
        do
        {
            temp1 = temp->next;
            free_vncclip_space(temp);
            temp = temp1;
        }while(temp != clip);
    }
}

// =============================================================================
// ��������:����vncclip���뵽clip������
// �������:
// ���������
// ����ֵ  :
// ˵��    :
// =============================================================================
void connect_clip2cliplink(struct tagClipRect **mlink,struct tagClipRect *sub)
{
    struct tagClipRect *temp;

    if(*mlink == NULL)
        *mlink = sub;
    else if(sub != NULL)
    {
        temp = sub->previous;
        sub->previous = (*mlink)->previous;
        (*mlink)->previous->next = sub;
        (*mlink)->previous = temp;
        temp->next = (*mlink);
    }
}
// =============================================================================
// ��������:��clip��clip�������Ƴ�
// �������:
// ���������
// ����ֵ  :
// ˵��    :��*mlink�ܶ������Ƴ���ͬʱ�����Ƴ���Ķ��ж��ף���ǰclip����һ������
//          ������򷵻�NULL
// =============================================================================
struct tagClipRect*  remove_clip_from_cliplink(struct tagClipRect **mlink)
{
   struct  tagClipRect  *clip = NULL;
   if(NULL == *mlink)
   {
    clip = NULL;
   }
   else if((*mlink)->next == (*mlink))//����һ��clip
   {
    clip = NULL;
   }
   else if((*mlink)->next == (*mlink)->previous)//����clip
   {
    clip=(*mlink)->next;
    clip->next = clip;
    clip->previous = clip;
   }
   else
   {
        clip=(*mlink)->next;
        clip->previous = (*mlink)->previous;
        (*mlink)->previous->next = clip;
   }
   return clip;
}
// =============================================================================
// ��������:��ӡclip���е���������Ժ����������ã�
// �������:
// ���������
// ����ֵ  :
// ˵��    :
// =============================================================================-
void show_clip_queque(struct tagClipRect *clip)
{

        int o,p,q;
        struct tagClipRect *var1,*var2;
        if(clip==NULL)
         {
            return;
         }
        var1=clip;
         var2=var1;
         do{
             o=(int)var1;
             p=(int)var1->previous;
             q=(int)var1->next;

            printf(" connected:cur=%08x pre=%08x  next=%08x  top=%d left=%d\n",o,p,q,var1->rect.top,var1->rect.left);
            var1=var1->next;
         }while(var1!=var2);
         debug_printf("\n");
}
// =============================================================================
// ��������:���clip���е���ȷ�ԣ����Ժ����������ã�
// �������:
// ���������
// ����ֵ  :
// ˵��    :ͨ�����������ͷ����������ж϶��е���ȷ�ԣ����ܺ����ǣ��������ڴ˴��㹻
//          ���ų������ε�����
// =============================================================================-
bool_t check_clip_queque(struct tagClipRect *cliphead)
{

    int backward=0;
    int forward=0;
    struct tagClipRect *pclip;
    pclip=cliphead;

    if(NULL==pclip)
    {
       return true;

    }
    else
    {
        do{
          backward++;
          pclip=pclip->next;
        }while(pclip!=cliphead);

         do{
          forward++;
          pclip=pclip->previous;
        }while(pclip!=cliphead);

        if(backward!=forward)
         {
            printf("some fatal error has happend in the clip queque!for=%d back=%d\n",forward,backward);
            return false;
        }

    }
        return true;
}


// =========================================================================
// ��������:���clip�ĸ���(���ڵ����ã����������ǲ�ʹ�õ�,Ӱ���ٶ�)
// �������:clip����ͷ
// ���������
// ����ֵ  :clip�ĸ�����0xFFFF�ǳ����ˣ����ص�bug
// ˵��    :���뱣֤clip��˫��ѭ���б�
// =========================================================================
u16 count_clip_number(struct tagClipRect *cliphead)
{
    int ret=0;
    struct tagClipRect *pclip;
    pclip=cliphead;

    if(NULL==pclip)
    {
        return ret;
    }
    else
    {
        do{
          ret++;
          pclip=pclip->next;
        }while(pclip!=cliphead);
    }
    return ret;
}

//���뷨�����½���clip���뵽ԭ����clip������
enum twoclipstate
{
    INCLUDING=0, //A��B��
    INCLUDED,  //B��A��
    CROSSING, //�ཻ
    ISLAND,   //���뽻
    FAILED,   //something too bad
    SUCCESS,  //do something to the clip success
    ANYOPER,  //��clip�������κβ���
};
// =============================================================================
// ��������:�жϸ�clip�����ĵ������߻��ǵ�
// �������:���ж���clip
// ���������
// ����ֵ  :
// ˵��    :1 �Ǿ���  0 �߻��ߵ�
// =============================================================================
static IsRealRec(struct tagRectangle rec)//���ǲ����ߵ㻹�������ľ���
{
    if((rec.top == rec.bottom)||(rec.left == rec.right))
    {
        return 0;
    }
    else
    {
        return 1;
    }

}
// =============================================================================
// ��������:�ж�clip_compared�����CLIPA�Ĺ�ϵ��
// �������:
// ���������
// ����ֵ  :INCLUDED A����B��INCLUDING B����A ��CROSSING ��ͨ�ཻ ��ISLAND,���ཻ
// ˵��    :
// =============================================================================
static int  judge_position_B2A(struct tagClipRect *A, struct tagClipRect *B)//�ж�Bclip�����Aclip��״̬
{
    if(NULL==A)//AΪ��
    {
        return INCLUDING;
    }
    else if(NULL == B)// BΪ��
    {
        return INCLUDED;
    }
    else//A B�����գ�һ������������
    {
        if((A->rect.top >= B->rect.bottom)||(A->rect.left>=B->rect.right)||\
        (A->rect.right <= B->rect.left)||(A->rect.bottom <= B->rect.top))//���ཻ�����
        {
            return ISLAND;
        }
        else if((A->rect.top <= B->rect.top)&&(A->rect.left <= B->rect.left)\
        &&(A->rect.right >= B->rect.right)&&(A->rect.bottom >= B->rect.bottom))//A����B
        {
            return  INCLUDED;
        }
        else if((A->rect.top >= B->rect.top)&&(A->rect.left >= B->rect.left)\
        &&(A->rect.right <= B->rect.right)&&(A->rect.bottom <= B->rect.bottom))//A����B
        {
            return  INCLUDING;
        }
        else//��ͨ���ཻ���� ,different���Է�Ϊ�Ĳ���
        {
            return  CROSSING;
        }//��ͨ�ཻ

    } //��������

}//��������


// =============================================================================
// ��������:��clipA������CLIPB
// �������:
// �������:different ��B�����A��ͬ�ĵط���ints B�����A��ͬ�ĵط�
// ����ֵ  :�ɹ���ʧ��
// ˵��    :Ŀǰ����ints�����㷨û���ã�����û����������ǧ�����ints��������⾪ϲ��
// =============================================================================

static int  break_vncclip_BbyA(struct tagClipRect *A, struct tagClipRect *B,\
    struct tagClipRect **different, struct tagClipRect **ints)
{
    int  ret = 0;
    int  intsTop = 0, intsLeft = 0, intsRight=0, intsBottom=0;
    struct tagClipRect  *temp = NULL, *curque = NULL;
    struct tagRectangle    rec;

    intsTop = (A->rect.top > B->rect.top? A->rect.top : B->rect.top);
    intsLeft = (A->rect.left > B->rect.left? A->rect.left : B->rect.left);
    intsRight = (A->rect.right < B->rect.right? A->rect.right : B->rect.right);
    intsBottom = (A->rect.bottom < B->rect.bottom? A->rect.bottom : B->rect.bottom);
            //�������
    rec.top = B->rect.top;
    rec.bottom = intsTop;
    rec.left = B->rect.left;
    rec.right = B->rect.right;
    if(IsRealRec(rec))
    {
        temp =  get_vncclip_space();
        if(temp)
        {
            if(curque)
            {
                temp->rect = rec;
                temp->previous = curque;
                temp->next = curque->next;
                curque->next->previous = temp;
                curque->next = temp;
                curque = temp;
            }
            else
            {
                temp->rect = rec;
                temp->previous = temp;
                temp->next = temp;
                curque = temp;
            }
        }
    }
            //�������
    rec.top = intsBottom;
    rec.bottom = B->rect.bottom;
    rec.left = B->rect.left;
    rec.right = B->rect.right;
    if(IsRealRec(rec))
    {
        temp =  get_vncclip_space();
        if(temp)
        {
            if(curque)
            {
                temp->rect = rec;
                temp->previous = curque;
                temp->next = curque->next;
                curque->next->previous = temp;
                curque->next = temp;
                curque = temp;
            }
            else
            {
                temp->rect = rec;
                temp->previous = temp;
                temp->next = temp;
                curque = temp;
            }
        }
    }
            //��߾���
    rec.top = intsTop;
    rec.bottom = intsBottom;
    rec.left = B->rect.left;
    rec.right = intsLeft;
    if(IsRealRec(rec))
    {
        temp =  get_vncclip_space();
        if(temp)
        {
            if(curque)
            {
                temp->rect = rec;
                temp->previous = curque;
                temp->next = curque->next;
                curque->next->previous = temp;
                curque->next = temp;
                curque = temp;
            }
            else
            {
                temp->rect = rec;
                temp->previous = temp;
                temp->next = temp;
                curque = temp;
            }
        }
    }
            //�ұ߾���
    rec.top = intsTop;
    rec.bottom = intsBottom;
    rec.left = intsRight;
    rec.right = B->rect.right;
    if(IsRealRec(rec))
    {
        temp =  get_vncclip_space();
        if(temp)
        {
            if(curque)
            {
                temp->rect = rec;
                temp->previous = curque;
                temp->next = curque->next;
                curque->next->previous = temp;
                curque->next = temp;
                curque = temp;
            }
            else
            {
                temp->rect = rec;
                temp->previous = temp;
                temp->next = temp;
                curque = temp;
            }
        }
    }
    if(curque)
    {
        *different = curque;
        return SUCCESS ;
    }
    else
    {
        return FAILED;
    }

}

// =============================================================================
// ��������:�ж�A��B˭���ʺ���������clip2break
// �������:
// �������:
// ����ֵ  :1 A���ʺϣ� 0 B���ʺ�
// ˵��    :�����ǿ�A��B����˭��clipbreaked�Ĺ����������������Ȼ����ʣ�������ƬС���������գ�
//          ��Ҫ�Ľ����ǣ����������ڶ�����࣬������rect�洢������ÿ�μ���Ҫ�����
// =============================================================================
//--TODO
static int judge_proper_clipbreaker(struct tagClipRect *A,\
             struct tagClipRect *B, struct tagClipRect  *clip2bebroken)
{
    struct tagRectangle  ints1;//A�ཻ�����
    struct tagRectangle  ints2;//B�ཻ�����

    ints1.top =(A->rect.top > clip2bebroken->rect.top? A->rect.top:clip2bebroken->rect.top);
    ints1.left = (A->rect.left > clip2bebroken->rect.left? A->rect.left:clip2bebroken->rect.left);
    ints1.right = (A->rect.right < clip2bebroken->rect.right?A->rect.right:clip2bebroken->rect.right);
    ints1.bottom = (A->rect.bottom < clip2bebroken->rect.bottom?A->rect.bottom:clip2bebroken->rect.bottom);

    ints2.top =(B->rect.top > clip2bebroken->rect.top? B->rect.top:clip2bebroken->rect.top);
    ints2.left = (B->rect.left > clip2bebroken->rect.left? B->rect.left:clip2bebroken->rect.left);
    ints2.right = (B->rect.right < clip2bebroken->rect.right?B->rect.right:clip2bebroken->rect.right);
    ints2.bottom = (B->rect.bottom < clip2bebroken->rect.bottom?B->rect.bottom:clip2bebroken->rect.bottom);

    if(((ints1.bottom-ints1.top)*(ints1.right-ints1.left)) > ((ints2.bottom-ints2.top)*(ints2.right-ints2.left)))
    {
        return 1;
    }
    else
    {
        return 0;
    }

}



// ============================================================================
// ��������:��newclip�鲢��head������
// �������:head��clip���С�newclip�����鲢��clip��
// �������:
// ����ֵ  :
// ˵��    :С��newclip��ֱ�ӱ����գ�boss����Ļ�initԭclipȻ��free����cross�Ļ���飬
//          Ȼ��ԭ�������յ�
// ============================================================================
//��������clip���뵽clip�����У���ʵ�ϲ�����ÿ��clip����Ҫ���룬���߼��������ġ�
//
//��stateΪincluded��ʱ��ֱ�ӽ���ǰclip�ӱȽ϶���ɾ��
//��stateΪincluding��ʱ�򣬸�clip�������Ա����飬����ѭ������ʱ��ӱȽ϶���ɾ��
//��stateΪISLAND��ʱ�򣬱�ʾclip���Լ����´αȽϣ�����ڶ�β����ISLANGD״̬��
//��ӱȽ϶����Ƴ�������ʣ����У������������ⲿ�ֶ��м��뵽�ܶ���
//��stateΪCROSSING��ʱ�򣬿��Լ������±Ƚϣ��������ȻΪCROSING����������ʵ�clip
//�������
//�ڲ�ʹ�ã����ٽ��в�����飬����ⲿʹ�ã��뱣֤clip���е���ȷ��
//newclipҪô��ת�ƣ�Ҫô�����ɾ�����벻Ҫ��free����ʹ��
void anastomose_vncclip(struct tagClipRect **head, struct tagClipRect *newclip)
{
    int clipstate = ANYOPER,clipstatebak = ANYOPER;
    struct tagClipRect *startCompare = NULL;
    struct tagClipRect *properBreakclip = NULL;//������������鵱ǰclip�Ŀ�
    struct tagClipRect *islandClip = NULL;//����������Щ�������ǹ����clip
    struct tagClipRect *clipCompared = NULL;
    struct tagClipRect *temp;//

    struct tagClipRect *different = NULL, *ints=NULL;
    //ĿǰΪֹintsû��ʹ�ã�����Ҫʹ�õĻ��������ڴ��й¶��������

    clipCompared = newclip;

    if(NULL == *head )
    {
        *head = newclip;
        return;
    }
    else if(NULL==newclip)
    {
        return;
    }
    else
    {
      //��������
    }

    while(clipCompared != NULL)//һֱ�Ƚϣ�ֱ����newcip��
    {
        startCompare = *head;
        //Compared = clipCompared;
        do
        {

            clipstate=judge_position_B2A(startCompare,clipCompared);
            if(clipstate == INCLUDING) //���Ƚϵ�clip����ԭ����clip��
            {
                clipstatebak = clipstate;
                startCompare->rect = clipCompared->rect;
                startCompare = startCompare->next;
            }
            else if(clipstate == INCLUDED) //������
            {
                temp = clipCompared;
                clipCompared = remove_clip_from_cliplink(&clipCompared);
                free_vncclip_space(temp);
                clipstatebak = ANYOPER;
                break;
            }
            else if(clipstate == CROSSING) //�ཻ
            {
                if(clipstatebak != INCLUDING)
                {
                    if(NULL == properBreakclip)
                    {
                        properBreakclip = startCompare;

                    }
                    else
                    {
                        if(judge_proper_clipbreaker(startCompare, properBreakclip,clipCompared))
                        {
                            properBreakclip = startCompare;
                        }
                        else
                        {
                            // �����κθı�
                        }
                    }
                    clipstatebak = clipstate;
                }
                else
                {
                    //�����κθı�
                }
                startCompare = startCompare->next;
            }
            else//����
            {
                if( (clipstatebak== ISLAND)||(clipstatebak == ANYOPER))
                {
                    clipstatebak = ISLAND;
                }
                startCompare = startCompare->next;
            }
        }while(*head != startCompare); //Сѭ������
        if(clipstatebak == INCLUDING)//��clip�Ѿ������ô�������ɾ��
        {
           temp = clipCompared;
           clipCompared = remove_clip_from_cliplink(&clipCompared);
           free_vncclip_space(temp);
         //  properBreakclip = NULL;
          // clipstatebak = ANYOPER;
        }
        else if(clipstatebak == CROSSING) //��Ҫ����
        {
            //����clip
             break_vncclip_BbyA(properBreakclip, clipCompared, &different, &ints);
             connect_clip2cliplink(&clipCompared, different);
            //ɾ��clip
             temp= clipCompared;
             clipCompared = remove_clip_from_cliplink(&clipCompared);
             free_vncclip_space(temp);
        }
        else if(clipstatebak == ISLAND)//����clip�ƶ���island������
        {
            temp = clipCompared;
            clipCompared = remove_clip_from_cliplink(&clipCompared);
            temp->previous = temp;
            temp->next = temp;
            connect_clip2cliplink(&islandClip, temp);
        }
        else//�϶���INCLUDED����ģ�������
        {
           //clipcompared ��ԭ�����̵��ˣ���õ�����
        }
        clipstatebak = ANYOPER;
        properBreakclip = NULL;
        startCompare = *head;
    }//��ѭ������

    //��island�Ķ��м��뵽 ԭ��������
    connect_clip2cliplink(head, islandClip);
}
//----�ϲ�����򻯰�-----------------------------------------------------------
//����: �Ѹ����ļ���������пɺϲ�������ϲ����򻯰����˼�ǣ������е������Ѿ���
//      ��->�ң�����->�µ�˳�����У��ϲ��󣬶����clip�����ͷš�
//����: clipq: ָ����ϲ��Ķ�����
//����: ��
//-----------------------------------------------------------------------------
struct tagClipRect *combine_vncclip_s(struct tagClipRect *clipq)
{
    struct tagClipRect *clip,*clip1,*tempclip;
    if(clipq == NULL)
        return NULL;
    //�ϲ�������
    clip = clipq;
    clip1 = clipq->next;
    //ִ��ˮƽ�ϲ�,����clip�ǰ����Ⱥ���ݵķ�ʽ���ֵģ����ˮƽ�ϲ�ֻ��Ҫ��
    //�����ھ����Ƿ���Ժϲ�����
    while(clip1 != clipq)
    {
        if((clip1->rect.top == clip->rect.top)
            &&(clip1->rect.bottom == clip->rect.bottom)
            &&(clip1->rect.left == clip->rect.right)) //�ɺϲ�
        {
            clip->rect.right = clip1->rect.right;
            clip->next = clip1->next;
            clip1->next->previous = clip;
            free_vncclip_space(clip1);
            clip1 = clip->next;
        }
        else if((clip1->rect.top == clip->rect.top) //���ڼ�������ͬ�����
            &&(clip1->rect.bottom == clip->rect.bottom)
            &&(clip1->rect.left == clip->rect.left)
            &&(clip1->rect.right== clip->rect.right))
        {
            clip->next = clip1->next;
            clip1->next->previous = clip;
            free_vncclip_space(clip1);
            clip1 = clip->next;
        }else
        {
            clip = clip1;
            clip1 = clip1->next;
        }
    }
    //ִ�д�ֱ�ϲ����ɺϲ��ľ��ο��ܲ����ڣ���������ѭ��������ɡ�
    clip = clipq;
    do{
        clip1 = clip->next;
        while(clip1 != clip)
        {
            if((clip1->rect.left == clip->rect.left)
                &&(clip1->rect.right == clip->rect.right)
                &&(clip->rect.bottom == clip1->rect.top)) //�ɺϲ�
            {
                clip->rect.bottom = clip1->rect.bottom;
                clip1->previous->next = clip1->next;
                clip1->next->previous = clip1->previous;
                tempclip = clip1;
                clip1 = clip1->next;
                free_vncclip_space(tempclip);
            }else
                clip1 = clip1->next;
        }
        clip = clip->next;
    }while(clip != clipq);
    return clipq;
}

// =============================================================================
// ��������:�ж�clipA�ǲ�����clipB�����·�
// �������:
// ���������
// ����ֵ  :1 �ǵģ�0 ����
// ˵��    :ˮƽ��Ȩ�ظ���
// =============================================================================
static int AIsBR2B_vnc(struct tagClipRect *A, struct tagClipRect *B)
{
    if((A->rect.top > B->rect.top) || \
    ((A->rect.top == B->rect.top) && (A->rect.left > B->rect.left)))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//----�ϲ�����ͨ�ð�-----------------------------------------------------------
//����: �Ѹ����ļ���������пɺϲ�������ϲ�����򻯰治ͬ���ǣ������е�clip��
//      ������еġ�������:
//      1��������clip����y��x��������ʹ���б������ң����ϡ�������ġ�
//      2������__gk_combine_clip_s������������
//����: clipq: ָ����ϲ��Ķ�����
//����: ���ص����ź�˳��Ķ���
//-----------------------------------------------------------------------------

struct tagClipRect * combine_vnccliplink(struct tagClipRect *clipq)
{

    struct tagClipRect  *startclip, *endclip,  *max;
    struct tagClipRect  *A, *B, *C, *D;
    int    firstTime=0;

    if(clipq == NULL)       //�����ǿյ�
        return NULL;
    else if(clipq->next == clipq)   //����ֻ��һ��clip
        return clipq;
    //������ð�����򷨰�clipq�еļ���������ң����ϡ�������
    //������ܶ�ʱ��ð�ݷ���Ƚ������Ľ�֮��--db
    //startclipΪÿ��ð�ݿ�ʼ�ĵط�
    //endclipΪÿ��ð�ݽ����ĵط�
    //����ķ����ǣ���һ�ν�����max��������棬Ȼ��ÿ�ζ���max->next��ʼ��
    //��һ���δ�ģ�ֱ����start��end��ͬ
    startclip = clipq;
    endclip = startclip->previous;

    do
    {
        A = startclip;
        firstTime++;
        do
        {
            B = A->next;
            if(AIsBR2B_vnc(A, B))//A��B�����£�������Ҫ����AB��λ��
            {
                if(A == B->next)//�����н���������clip
                {
                    //֮��������������ϣ��Aָ���������max��
                }
                else//��A��B��λ�õ�����ע��A��B�����ڵ�
                {
                    C = A->previous;
                    D = B->next;
                    C->next = B;
                    D->previous = A;

                    A->previous = B;
                    A->next = D;
                    B->next = A;
                    B->previous = C;
                }
            }
            else
            {
                A = B;
            }
        }while(B != endclip);
        if(firstTime ==1)//��ѭ����һ�ξͿ���֪���������ĸ���
        {
            max=A;
        }
        endclip = A;
        startclip = max->next;
        endclip = endclip->previous;
    }while(startclip!= endclip);

    return  combine_vncclip_s(startclip);
}
