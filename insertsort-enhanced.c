/*排序算法按照其稳定性来说，分为非稳定性排序与稳定性排序算法。判断稳定性与非稳定性算法的依据一般在是否为相邻元素之间的数据交换，不然一般为非稳定排序。比如快速排序，Shell排序等都不是稳定排序，而像冒泡排序，插入排序，合并排序等都是稳定排序。C语言库函数中的qsort函数是基于快速排序的，因此排序之后，未参加排序的数据之间的顺序可能被打乱。有时我们又需要在某一类场景下利用到稳定排序算法。而C++ STL算法库中也对应着这两种算法接口函数: sort与stable_sort，而stable_sort就是稳定排序算法。当然稳定与否还是与具体实现相关，如果算法在交换两个元素时，用的是严格意义上的比较运算，则可能就是稳定的，如果不是严格的(比如>=)，那即使用稳定的算法，也可能排序后的相对元素位置也发生改变了。下面我们研究下稳定的插入排序算法实现，原理就不用多说了，算法流程类似与我们玩纸牌手法，插入新元素之前，原来的序列就是已经排序好的，这样从时间复杂度来说，和冒泡排序一样，都是O(n^2)。以下是简单的实现源码:*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void insertSort(int a[], int len) //插入排序
{
    if (len < 2) // 如果元素个数小于2,直接返回
    {
        return;
    }
    for (int i = 1; i < len; i++)
    {
        int temp = a[i]; //先将待交换的元素保存起来
        int j = i - 1;
        while (j >= 0 && a[j] > temp) //按照升序排列，如果指定排序元素之前的元素比待排序元素大
        {	                          //，那么从此元素开始到开头，都向后移动一位
            a[j +1] = a[j];
            j--;
        }
        a[j+1] = temp;  //将交换的数据插入到合适位置
    }
}

#define SIZE  sizeof(a)/sizeof(a[0])

int main()
{
    int a[10] = {0};

    srand(time(NULL));

    for (int i = 0; i < SIZE; i++)//随机生成10个数字
    {
        a[i] = rand() % 100;
    }

    for (int i = 0; i < SIZE; i++)//打印排序之前的数字
    {
        printf("%d ", a[i]);
    }
    printf("\n");
    insertSort(a, SIZE);

    for (int i = 0; i < SIZE; i++)//打印排序之后的数字
    {
        printf("%d ", a[i]);
    }

    return 0;
}

/*
上面只是对整数进行排序，那么能不能对一般数据类型(主要指的是结构体类型)进行排序呢? 答案是肯定的。我们先看看库函数中qsort的函数接口。实际上qsort一般对连续内存的数组进行排序的，qsort一共有4个参数，第1个参数是数组基址，第2个参数是元素个数，第3个参数是每个元素大小，第4个参数是提供给调用者的回调函数，用来改变排序规则用的。我们也可以提供类似的接口，我们这里交换元素的概念变为交换两块内存的内容，本质上和一般的插入排序算法一样，一般插入排序本质上也是交换4个字节的内存数据。于是我们得到如下增强版的插入排序算法：*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void insertSort(void *base , size_t num, size_t size, int(*cmp_func)(const void*, const void *) ) //插入排序
{
    if (num < 2 || size < 1) // 如果元素个数小于2,直接返回
    {
        return;
    }
    void *pTemp = (void*)malloc(size); //不判定为空指针
    memset(pTemp, 0x0, size);
    for (size_t i = 1; i < num; i++)
    {
        memcpy(pTemp, (char*)base + i * size, size);//先将待交换的元素保存起来
        int j = i - 1; //防止无符号溢出,此处用有符号数
        while (j >= 0 && cmp_func((char*)base + j * size, pTemp) > 0) //按照升序排列，如果指定排序元素之前的元素比待排序元素大
        {	                          //，那么从此元素开始到开头，都向后移动一位
            memcpy((char*)base + (j+1)*size, (char*)base + j * size, size);
            j--;
        }
        memcpy((char*)base + (j+1)*size, pTemp, size);  //将交换的数据插入到合适位置
    }

    free(pTemp);

}

#define SIZE  sizeof(a)/sizeof(a[0])

int cmp_func(const void *a, const void *b)
{
    return *(int*)a - *(int*)b;
}

int main()
{
    int a[10] = {0};

    srand(time(NULL));

    for (unsigned int i = 0; i < SIZE; i++)//随机生成10个数字
    {
        a[i] = rand() % 100;
    }

    for (unsigned int i = 0; i < SIZE; i++)//打印排序之前的数字
    {
        printf("%d ", a[i]);
    }
    printf("\n");

    insertSort(a, SIZE, sizeof(int), cmp_func);

    for (unsigned int i = 0; i < SIZE; i++)//打印排序之后的数字
    {
        printf("%d ", a[i]);
    }

    return 0;
}

/*假设现在有一个需求，班级一共有10名学生，学生信息包括学生id号，学生姓名及考试总成绩(100分制算)。现在需要对学生信息进行排名：学生成绩按照从高分往低分排序，如果两人成绩一样，则按照学生的姓名字典序排序。*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct
{
	int id; //学生ID
	char *name; //学生姓名
	int score; //分数
}student_t;

void insertSort(void *base , size_t num, size_t size, int(*cmp_func)(const void*, const void *) ) //插入排序
{
    if (num < 2 || size < 1) // 如果元素个数小于2,直接返回
    {
        return;
    }
    void *pTemp = (void*)malloc(size); //不判定为空指针
    memset(pTemp, 0x0, size);
    for (size_t i = 1; i < num; i++)
    {
        memcpy(pTemp, (char*)base + i * size, size);//先将待交换的元素保存起来
        int j = i - 1;
        while (j >= 0 && cmp_func((char*)base + j * size, pTemp) > 0) //按照升序排列，如果指定排序元素之前的元素比待排序元素大
        {	                          //，那么从此元素开始到开头，都向后移动一位
            memcpy((char*)base + (j+1)*size, (char*)base + j * size, size);
            j--;
        }
        memcpy((char*)base + (j+1)*size, pTemp, size);  //将交换的数据插入到合适位置
    }

    free(pTemp);

}

student_t student[] =
{
		{320991, "zhangsan", 65},
		{543321, "lisi", 78},
		{333453, "xiaoqi", 79},
		{456566, "hangang", 77},
		{354444, "zhaolei", 91},
		{456566, "hangang", 87},
		{242333, "sunjunior", 99},
		{424444, "wangyong", 88},
		{445444, "santian", 73},
		{445444, "qidong", 91}
};

#define g_student_num sizeof(student)/sizeof(student[0])

int cmp_func(const void *a, const void *b)
{
	student_t *pa = (student_t*)a;
	student_t *pb = (student_t*)b;
	if (pa->score != pb->score)
	{
		return pb->score - pa->score;
	}
	else
	{
		return strcmp(pa->name, pb->name);
	}
}

int main()
{
    for (unsigned int i = 0; i < g_student_num; i++)//打印排序之前的数字
    {
        printf("%d:%s:%d\n", student[i].id, student[i].name, student[i].score);
    }
    printf("*********************************\n");

    insertSort(student, g_student_num, sizeof(student_t), cmp_func);

    for (unsigned int i = 0; i < g_student_num; i++)//打印排序之后的数字
    {
        printf("%d:%s:%d\n", student[i].id, student[i].name, student[i].score);
    }

    return 0;
}

/*
最后再验证下此增强版排序算法的稳定性，即如果数据元素不发生交换的话，排序之前与之后的相对位置不变。现在对上面学生学号分成10类，以学号最后一位作为分类号，如果学号最后一位数字相同，则归为一类，那排序后的情况如何呢？我们只需在上面源码下修改下比较函数即可，其他不变 */
int cmp_func(const void *a, const void *b)
{
	student_t *pa = (student_t*)a;
	student_t *pb = (student_t*)b;
	return pa->id % 10 - pb->id % 10;
}

/*同样，我们将此排序算法运用链表来实现，也可以同样实现其要求，下面是用单链表来实现排序的功能：*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct ListNode
{
    void *data;
    struct ListNode *next;

}SList;

SList *insertSort(SList *head, int (*cmp_func)(const void *a, const void *b))
{
    if (head == NULL || head->next == NULL)
    {
        return head;
    }

    SList *newHead = NULL; //原链表剩下的用于直接插入的结点头指针
    SList *pNode = NULL; //待插入结点
    SList *pre = NULL; //前驱指针变量
    SList *cur = NULL; //指向排序好的链表当前指针变量

    newHead = head->next;
    head->next = NULL;
    while (newHead != NULL)
    {
        pNode = newHead;   //待插入结点
        cur = head;   //已经排序好的结点
        while (cur != NULL && (cmp_func(cur->data, pNode->data) < 0))
        {
            pre = cur;
            cur = cur->next;
        }
        //退出循环，找到插入位置
        newHead = newHead->next;
        if (cur == head)
        {
            head = pNode;
        }
        else
        {
            pre->next = pNode;
        }

        pNode->next = cur; //完成插入动作
    }

    return head;
}


typedef struct
{
    int id; //student id
    const char *name; //student name
    int score; //student score
}student_t;

student_t student[] =
{
        {320991, "zhangsan", 65},
        {543321, "lisi", 78},
        {333453, "xiaoqi", 79},
        {456566, "hangang", 77},
        {354444, "zhaolei", 91},
        {456566, "hangang", 87},
        {242333, "sunjunior", 99},
        {424444, "wangyong", 88},
        {445444, "santian", 73},
        {445447, "qidong", 91}
};

int g_student_num = sizeof(student) / sizeof(student[0]);

int cmp_func(const void *a, const void *b)
{
    student_t *pa = (student_t*)a;
    student_t *pb = (student_t*)b;
    if (pa->score != pb->score)
    {
        return pb->score - pa->score;
    }
    else
    {
        return strcmp(pa->name, pb->name);
    }
}


SList *createList(void)
{
    SList *pHead = NULL;
    for (int i = 0; i < g_student_num; i++)
    {
        SList *pTemp = (SList*)malloc(sizeof(SList));
        student_t *pt = (student_t*)malloc(sizeof(student_t));
        pt->id = student[i].id;
        pt->name = student[i].name;
        pt->score = student[i].score;
        pTemp->data = (void*)pt;
        pTemp->next = pHead;
        pHead = pTemp;
    }

    return pHead;
}



void traverseList(SList *pList)
{
    while (pList != NULL)
    {
        student_t *pt = (student_t*)pList->data;
        printf("%d:%s:%d\n", pt->id, pt->name, pt->score);
        pList = pList->next;
    }
    printf("****************\n");
}

int main()
{
    SList *pHead = createList();
    traverseList(pHead);
    pHead = insertSort(pHead, cmp_func);
    traverseList(pHead);



    return 0;
}

