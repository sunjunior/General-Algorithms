/*
排序常见的有十几种排序算法，然而对于单链表来说，链表缺乏随机访问特点，导致一些排序算法在时间和空间复杂度上都比较复杂，一般的冒泡排序与插入排序，其时间复杂度都是O(N^2)。链表中最优的排序算法应该为归并排序，归并排序是分治法的典型应用，主要思想是将一个大问题划分为N（一般取2）个等价的子问题，通过对子问题的解答来达到对原有问题的解决。下面对链表的归并排序用代码实现。归并排序思想可以参考数组归并排序思想，其时间复杂度为O（NlogN)，需要注意的是，对于链表的排序，需要维系一些辅助链表指针，特别注意指针的取值范围。在链表数据量较大时，应该优先考虑此算法排序：
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 20

typedef struct tagListNode
{
    int data;
    struct tagListNode *next;
}LinkList;

LinkList *getMiddleNode(LinkList *pList)
{
    if (pList == NULL)
    {
        return NULL;
    }
    LinkList *pAhead = pList->next;
    LinkList *pBehind = pList;
    while (pAhead != NULL)
    {
         pAhead = pAhead->next;
         if (pAhead != NULL)
         {
             pAhead = pAhead->next;
             pBehind = pBehind->next;
         }

    }

    return pBehind;
}

LinkList *MergeList(LinkList *p1, LinkList *p2) //合并有序链表，合并之后升序排列
{
    if (NULL == p1)
    {
        return p2;
    }
    if (NULL == p2)
    {
        return p1;
    }

    LinkList *pLinkA = p1;
    LinkList *pLinkB = p2;
    LinkList *pTemp = NULL;
    if (pLinkA->data <= pLinkB->data)
    {
        pTemp = pLinkA;
        pLinkA = pLinkA->next;
    }
    else
    {
        pTemp = pLinkB;
        pLinkB = pLinkB->next;
    }

    LinkList *pHead = pTemp; //初始化头结点，即头结点指向不为空的结点
    while (pLinkA && pLinkB)
    {
        if (pLinkA->data <= pLinkB->data)
        {
            pTemp->next = pLinkA;
            pTemp = pLinkA;
            pLinkA = pLinkA->next;
        }
        else
        {
            pTemp->next = pLinkB;
            pTemp = pLinkB;
            pLinkB = pLinkB->next;
        }

    }

    pTemp->next = pLinkA ? pLinkA:pLinkB; //插入多余的链表部分

    return pHead;
}

LinkList *MergeSort(LinkList *pList)
{
    if (pList == NULL || pList->next == NULL)
    {
        return pList;
    }

    LinkList *pMiddle = getMiddleNode(pList); //获取中间结点
    LinkList *pBegin = pList; //链表前半部分，包括中间结点
    LinkList *pEnd = pMiddle->next;//链表后半部分
    pMiddle->next = NULL; //必须赋值为空
    pBegin = MergeSort(pBegin); //排序前半部分数据
    pEnd = MergeSort(pEnd); //排序后半部分数据
    return MergeList(pBegin, pEnd); //合并有序链表
}


LinkList *CreateList(void)
{
    LinkList *pHead = NULL;
    srand(time(NULL));
    int i;
    for (i = 0; i < MAX; i++)
    {
        LinkList *pNode = (LinkList*)malloc(sizeof(*pNode));
        if (pNode == NULL)
        {
            return 0;
        }
        pNode->data = rand() % MAX; //随机产生MAX个数据
        pNode->next = pHead; //采用前插法生成链表
        pHead = pNode;
    }

    return pHead;
}

void traverse(LinkList *pList) //遍历整个链表
{
    while (pList != NULL)
    {
        fprintf(stdout, "%d\n", pList->data);
        pList = pList->next;
    }
    printf("***********************\n");
}


int main()
{
    LinkList *pList = CreateList();
    traverse(pList);
    pList = MergeSort(pList);
    traverse(pList);

    return 0;
}

/*
上面链表合并排序算法只是对整数进行排序，那么能不能对一般类型的数据进行排序呢，答案是肯定的！ 我们可以仿照库函数中的qsort函数接口，提供一个回调函数来指定排序规则。注意qsort一般只能对连续内存进行排序，这里我们可以将上面的函数接口改为：

    LinkList *MergeSort(LinkList *pList, int(*cmp_func)(const void *a, const void *b))

上面的int(*cmp_func)(const void *a, const void *b)为回调函数，实质上指的是排序比较规则由调用者指定。

假设现在有一个需求，班级一共有20名学生，学生信息包括学生id号，学生姓名及考试总成绩(100分制算)。现在需要对学生信息进行排名：学生成绩按照从高分往低分排序，如果两人成绩一样，则按照学生的姓名字典序排序。显然需要修改下排序规则，那么我们对链表数据域类型泛化，只提供一个void *类型的数据，具体由实际类型指定。基于上述排序算法的改进排序算法如下：
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define MAX 20

typedef struct tagListNode
{
    void *data;
    struct tagListNode *next;
}LinkList;

LinkList *getMiddleNode(LinkList *pList)
{
    if (pList == NULL)
    {
        return NULL;
    }
    LinkList *pAhead = pList->next;
    LinkList *pBehind = pList;
    while (pAhead != NULL)
    {
         pAhead = pAhead->next;
         if (pAhead != NULL)
         {
             pAhead = pAhead->next;
             pBehind = pBehind->next;
         }

    }

    return pBehind;
}

LinkList *MergeList(LinkList *p1, LinkList *p2, int(*cmp_func)(const void *a, const void *b)) //合并有序链表，合并之后升序排列
{
    if (NULL == p1)
    {
        return p2;
    }
    if (NULL == p2)
    {
        return p1;
    }

    LinkList *pLinkA = p1;
    LinkList *pLinkB = p2;
    LinkList *pTemp = NULL;

    if(cmp_func(pLinkA->data, pLinkB->data) <= 0)
    {
        pTemp = pLinkA;
        pLinkA = pLinkA->next;
    }
    else
    {
        pTemp = pLinkB;
        pLinkB = pLinkB->next;
    }

    LinkList *pHead = pTemp; //初始化头结点，即头结点指向不为空的结点
    while (pLinkA && pLinkB)
    {
        if (cmp_func(pLinkA->data, pLinkB->data) <= 0)
        {
            pTemp->next = pLinkA;
            pTemp = pLinkA;
            pLinkA = pLinkA->next;
        }
        else
        {
            pTemp->next = pLinkB;
            pTemp = pLinkB;
            pLinkB = pLinkB->next;
        }

    }

    pTemp->next = pLinkA ? pLinkA:pLinkB; //插入多余的链表部分

    return pHead;
}

LinkList *MergeSort(LinkList *pList, int(*cmp_func)(const void *a, const void *b))
{
    if (pList == NULL || pList->next == NULL)
    {
        return pList;
    }

    LinkList *pMiddle = getMiddleNode(pList); //获取中间结点
    LinkList *pBegin = pList; //链表前半部分，包括中间结点
    LinkList *pEnd = pMiddle->next;//链表后半部分
    pMiddle->next = NULL; //必须赋值为空
    pBegin = MergeSort(pBegin, cmp_func); //排序前半部分数据
    pEnd = MergeSort(pEnd, cmp_func); //排序后半部分数据
    return MergeList(pBegin, pEnd, cmp_func); //合并有序链表
}

typedef struct
{
    int id;
    char name[20];
    int score;
}student_t;

int cmp(const void *a, const void *b)
{
    student_t *pa = (student_t *)a;
    student_t *pb = (student_t *)b;
    if(pa->score != pb->score)
    {
        return pb->score - pa->score;
    }
    else
    {
        return strcmp(pa->name, pb->name) > 0;
    }

}

LinkList *CreateDataList(void)
{
    LinkList *pHead = NULL;
    srand(time(NULL));

    for (int i = 0; i < MAX; i++)
    {
        LinkList *pNode = (LinkList*)malloc(sizeof(*pNode));
        if (pNode == NULL)
        {
            return 0;
        }
        //随机产生MAX个数据
        student_t *pt = (student_t*)malloc(sizeof(student_t));
        pt->id = i + 1;
        int len = 0;
        for (int k = 0; k < 10; k++)
        {
            len += sprintf(pt->name + len, "%c", rand() % 25 + 'a');
        }
        pt->score = rand() % 40 + 60;
        pNode->data = (void*)pt;
        pNode->next = pHead; //采用前插法生成链表
        pHead = pNode;
    }
    return pHead;
}

void traverseList(LinkList *pList) //遍历整个链表
{
    while (pList != NULL)
    {
        student_t *pt = (student_t *)pList->data;
        fprintf(stdout, "%d:%s:%d\n", pt->id, pt->name, pt->score);
        pList = pList->next;
    }
    printf("***********************\n");
}




int main()
{
    LinkList *pList = CreateDataList();
    traverseList(pList);
    pList = MergeSort(pList, cmp);
    traverseList(pList);

    return 0;
}

