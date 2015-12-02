#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define HASHLEN 2807303
typedef struct node
{
    char *word; //单词
    int count; //单词统计词频
    struct node *next;
} T_Node;

T_Node *head[HASHLEN] = {NULL}; //hash链表法表示 

// 最简单hash函数
int hash_function(char const *p)
{
    int value = 0;
    while (*p != '\0')
    {
        value = value * 31 + *p++;
        if (value > HASHLEN)
            value = value % HASHLEN;
    }
    return value;
}

// 添加单词到hash表
void appendWord(char const *str)
{
    int index = hash_function(str);
    T_Node *pNode = head[index];
    while (pNode != NULL)
    {
        if (strcmp(str, pNode->word) == 0) //找到对应的值，则词频加1,单词去重
        {
            (pNode->count)++;
            return;
        }
        pNode = pNode->next;
    }

    // 新建一个结点
    T_Node *q = (T_Node*)malloc(sizeof(T_Node));
    q->count = 1;
    q->word =  (char*)malloc(strlen(str)+1);
    strcpy(q->word, str);
    q->next = head[index];
    head[index] = q;
}

int deleteWord(char const *str)
{
    int index = hash_function(str);
    T_Node *pHead = head[index];
    T_Node *preHead = NULL;
    while (pHead != NULL)
    {
        if (strcmp(str, pHead->word) == 0)
        {
            T_Node *deNode = pHead;
            pHead = pHead->next;
            if (NULL != preHead)
            {
                preHead->next = pHead;
            }
            else
            {
                head[index] = pHead;
            }
            free(deNode->word);
            deNode->word = NULL;
            free(deNode);
            deNode = NULL;
            return 0;
        }
        else
        {
            preHead = pHead;
            pHead = pHead->next;
        }
    }

    return -1;
}


void printWords() //打印hash表中的所有单词，及统计词频
{
    int i = 0;
    while (i < HASHLEN)
    {
        for (T_Node *p = head[i]; p != NULL; p = p->next)
        {
            fprintf(stdout, "%s:%d\n", p->word, p->count);
        }

        i++;
    }

    printf("**************\n");
}




int main()
{
    char words[][16] = {"Remember", "what", "should", "be", "remembered", "and",
                        "forget", "what", "should", "be", "forgotten", "Alter", "what", "is",
                        "changeable", "and", "accept", "what", "is", "unchangeable"
                       };
    for(unsigned i = 0; i < sizeof(words)/16; i++)
    {
        appendWord(words[i]);
    }
    printWords();
    deleteWord("be");

    printWords();
    return 0;
}

