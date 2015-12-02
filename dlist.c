#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*Redis源码Adlist中实现了一个双向链表，该链表在设计数据结构及其操作上有许多不同的技巧！实现了数据与算法框架相分离，特此记录下，以便为后面参考*/
/* Node, List, and Iterator are the only data structures used currently. */

typedef struct listNode
{
    struct listNode *prev;
    struct listNode *next;
    void *value;
} listNode;

typedef struct listIter
{
    listNode *next;
    int direction;
} listIter;

typedef struct list
{
    listNode *head;
    listNode *tail;
    void *(*dup)(void *ptr);
    void (*free)(void *ptr);
    int (*match)(void *ptr, void *key);
    unsigned long len;
} list;

/* Directions for iterators */
#define DL_START_HEAD 0
#define DL_START_TAIL 1

list *listCreate(void)
{
    list *pList = (list *)malloc(sizeof(list));
    if (pList == NULL)
    {
        return NULL;
    }
    pList->head = pList->tail = NULL;
    pList->len = 0;
    pList->dup = NULL;
    pList->free = NULL;
    pList->match = NULL;
    return pList;
}

/* Free the whole list.
 *
 * This function can't fail. */
void listRelease(list *pList)
{
    listNode *current = pList->head;
    unsigned long len = pList->len;
    while(len--)
    {
        listNode *next = current->next;
        if (pList->free)
        {
            pList->free(current->value);
        }
        free(current);
        current = next;
    }
    free(pList);
}

/* Add a new node to the list, to head, containing the specified 'value'
 * pointer as value.
 *
 * On error, NULL is returned and no operation is performed (i.e. the
 * list remains unaltered).
 * On success the 'list' pointer you pass to the function is returned. */
list *listAddNodeHead(list *pList, void *value)
{
    listNode *node = (listNode *)malloc(sizeof(listNode));
    if (node == NULL)
    {
        return NULL;
    }
    node->value = value;
    if (pList->len == 0) //¿ÕÁ´±í
    {
        pList->head = pList->tail = node;
        node->prev = node->next = NULL;
    }
    else
    {
        node->prev = NULL;
        node->next = pList->head;
        pList->head->prev = node;
        pList->head = node;
    }
    pList->len++;
    return pList;
}

/* Add a new node to the list, to tail, containing the specified 'value'
 * pointer as value.
 *
 * On error, NULL is returned and no operation is performed (i.e. the
 * list remains unaltered).
 * On success the 'list' pointer you pass to the function is returned. */
list *listAddNodeTail(list *pList, void *value)
{
    listNode *node = (listNode *)malloc(sizeof(listNode));
    if (node == NULL)
    {
        return NULL;
    }

    node->value = value;
    if (pList->len == 0)
    {
        pList->head = pList->tail = node;
        node->prev = node->next = NULL;
    }
    else
    {
        node->prev = pList->tail;
        node->next = NULL;
        pList->tail->next = node;
        pList->tail = node;
    }

    pList->len++;
    return pList;
}

list *listInsertNode(list *pList, listNode *old_node, void *value, int after)
{
    listNode *node = (listNode *)malloc(sizeof(listNode));

    if (node == NULL)
    {
        return NULL;
    }
    node->value = value;

    if (after)
    {
        node->prev = old_node;
        node->next = old_node->next;
        if (pList->tail == old_node)
        {
            pList->tail = node;
        }
    }
    else
    {
        node->next = old_node;
        node->prev = old_node->prev;
        if (pList->head == old_node)
        {
            pList->head = node;
        }
    }
    if (node->prev != NULL)
    {
        node->prev->next = node;
    }
    if (node->next != NULL)
    {
        node->next->prev = node;
    }
    pList->len++;
    return pList;
}

/* Remove the specified node from the specified list.
 * It's up to the caller to free the private value of the node.
 *
 * This function can't fail. */
void listDelNode(list *pList, listNode *node)
{
    if (node->prev)
    {
        node->prev->next = node->next;
    }
    else
    {
        pList->head = node->next;
    }

    if (node->next)
    {
        node->next->prev = node->prev;
    }
    else
    {
        pList->tail = node->prev;
    }

    if (pList->free)
    {
        pList->free(node->value);
    }

    free(node);
    pList->len--;
}

/* Returns a list iterator 'iter'. After the initialization every
 * call to listNext() will return the next element of the list.
 *
 * This function can't fail. */
listIter *listGetIterator(list *pList, int direction)
{
    listIter *iter = (listIter *)malloc(sizeof(listIter));

    if (pList == NULL)
    {
        return NULL;
    }

    if (direction == DL_START_HEAD)
    {
        iter->next = pList->head;
    }
    else
    {
        iter->next = pList->tail;
    }

    iter->direction = direction;
    return iter;
}

/* Release the iterator memory */
void listReleaseIterator(listIter *iter)
{
    free(iter);
}

/* Create an iterator in the list private iterator structure */
void listRewind(list *pList, listIter *li)
{
    li->next = pList->head;
    li->direction = DL_START_HEAD;
}

void listRewindTail(list *pList, listIter *liter)
{
    liter->next = pList->tail;
    liter->direction = DL_START_TAIL;
}

/* Return the next element of an iterator.
 * It's valid to remove the currently returned element using
 * listDelNode(), but not to remove other elements.
 *
 * The function returns a pointer to the next element of the list,
 * or NULL if there are no more elements, so the classical usage patter
 * is:
 *
 * iter = listGetIterator(list,<direction>);
 * while ((node = listNext(iter)) != NULL)
 * {
 *     doSomethingWith(listNodeValue(node));
 * }
 *
 * */
listNode *listNext(listIter *iter)
{
    listNode *current = iter->next;

    if (current != NULL)
    {
        if (iter->direction == DL_START_HEAD)
        {
            iter->next = current->next;
        }
        else
        {
            iter->next = current->prev;
        }
    }

    return current;
}

/* Duplicate the whole list. On out of memory NULL is returned.
 * On success a copy of the original list is returned.
 *
 * The 'Dup' method set with listSetDupMethod() function is used
 * to copy the node value. Otherwise the same pointer value of
 * the original node is used as value of the copied node.
 *
 * The original list both on success or error is never modified. */
list *listDup(list *orig)
{


    list *copy = listCreate();
    if (copy == NULL)
    {
        return NULL;
    }
    copy->dup = orig->dup;
    copy->free = orig->free;
    copy->match = orig->match;
    listIter *iter = listGetIterator(orig, DL_START_HEAD);
    listNode *node = listNext(iter);
    while(node != NULL)
    {
        void *value = NULL;
        if (copy->dup)
        {
            value = copy->dup(node->value);
            if (value == NULL)
            {
                listRelease(copy);
                listReleaseIterator(iter);
                return NULL;
            }
        }
        else
        {
            value = node->value;
        }

        if (listAddNodeTail(copy, value) == NULL)
        {
            listRelease(copy);
            listReleaseIterator(iter);
            return NULL;
        }
    }
    listReleaseIterator(iter);
    return copy;
}

/* Search the list for a node matching a given key.
 * The match is performed using the 'match' method
 * set with listSetMatchMethod(). If no 'match' method
 * is set, the 'value' pointer of every node is directly
 * compared with the 'key' pointer.
 *
 * On success the first matching node pointer is returned
 * (search starts from head). If no matching node exists
 * NULL is returned. */
listNode *listSearchKey(list *pList, void *key)
{
    listIter *iter = listGetIterator(pList, DL_START_HEAD);
    listNode *node = listNext(iter);
    while(node != NULL)
    {
        if (pList->match)
        {
            if (pList->match(node->value, key))
            {
                listReleaseIterator(iter);
                return node;
            }
        }
        else
        {
            if (key == node->value)
            {
                listReleaseIterator(iter);
                return node;
            }
        }
    }
    listReleaseIterator(iter);
    return NULL;
}

/* Return the element at the specified zero-based index
 * where 0 is the head, 1 is the element next to head
 * and so on. Negative integers are used in order to count
 * from the tail, -1 is the last element, -2 the penultimate
 * and so on. If the index is out of range NULL is returned. */
listNode *listIndex(list *pList, long index)
{
    listNode *node;

    if (index < 0)
    {
        index = (-index) - 1;
        node = pList->tail;
        while(index-- && node)
        {
            node = node->prev;
        }
    }
    else
    {
        node = pList->head;
        while(index-- && node)
        {
            node = node->next;
        }
    }
    return node;
}

/* Rotate the list removing the tail node and inserting it to the head. */
void listRotate(list *pList)
{

    listNode *tail = pList->tail;

    if (pList->len <= 1)
    {
        return;
    }
    /* Detach current tail */
    pList->tail = tail->prev;
    pList->tail->next = NULL;
    /* Move it as head */
    pList->head->prev = tail;
    tail->prev = NULL;
    tail->next = pList->head;
    pList->head = tail;
}
typedef struct tagMsgNode
{
    unsigned int uiMsgType;
    unsigned int uiTid;
    void *pSendMsg;
}T_msgNode;

list *pList = NULL;

void createList()
{

    pList = listCreate();
    T_msgNode *pMsgNode1 = (T_msgNode*)malloc(sizeof(T_msgNode));
    pMsgNode1->pSendMsg = (char*)malloc(16);
    strcpy((char*)pMsgNode1->pSendMsg, "send msg hello");
    pMsgNode1->uiMsgType = 0x8d48;
    pMsgNode1->uiTid = 0x701;

    pList = listAddNodeTail(pList, (void*)pMsgNode1);

    T_msgNode *pMsgNode2 = (T_msgNode*)malloc(sizeof(T_msgNode));
    pMsgNode2->pSendMsg = (char*)malloc(16);
    strcpy((char*)pMsgNode2->pSendMsg, "send msg world");
    pMsgNode2->uiMsgType = 0x8d49;
    pMsgNode2->uiTid = 0x702;

    pList = listAddNodeTail(pList, (void*)pMsgNode2);

    T_msgNode *pMsgNode3 = (T_msgNode*)malloc(sizeof(T_msgNode));
    pMsgNode3->pSendMsg = (char*)malloc(16);
    strcpy((char*)pMsgNode3->pSendMsg, "send msg ack");
    pMsgNode3->uiMsgType = 0x8d50;
    pMsgNode3->uiTid = 0x703;

    pList = listAddNodeTail(pList, (void*)pMsgNode3);

    T_msgNode *pMsgNode4 = (T_msgNode*)malloc(sizeof(T_msgNode));
    pMsgNode4->pSendMsg = (char*)malloc(16);
    strcpy((char*)pMsgNode4->pSendMsg, "send msg end");
    pMsgNode4->uiMsgType = 0x8d51;
    pMsgNode4->uiTid = 0x704;

    pList = listAddNodeTail(pList, (void*)pMsgNode4);

    listIter *iter = listGetIterator(pList, DL_START_TAIL);
    listNode *node = NULL;
    while ((node = listNext(iter)) != NULL)
    {
        T_msgNode *pNode = (T_msgNode *)node->value;
        printf("msg type: 0x%x\n", pNode->uiMsgType);
        printf("msg tid : 0x%x\n", pNode->uiTid);
        printf("msg body: %s\n", (char*)pNode->pSendMsg);

        printf("***********************\n");
    }
    printf("*********************************\n");
    iter = listGetIterator(pList, DL_START_HEAD);
    while ((node = listNext(iter)) != NULL)
    {
        T_msgNode *pNode = (T_msgNode *)node->value;
        printf("msg type: 0x%x\n", pNode->uiMsgType);
        printf("msg tid : 0x%x\n", pNode->uiTid);
        printf("msg body: %s\n", (char*)pNode->pSendMsg);

        printf("***********************\n");
    }
}



int main()
{
    createList();

    return 0;
}

