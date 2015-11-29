//============================================================================
// Name        : AVL.cpp
// Author      : @CodingGeek
// Version     : V1.0
// Copyright   :
// Description : AVL平衡搜索二叉树C代码实现
//============================================================================

#include "AVL.h"
#include <stdio.h>
#include <stdlib.h>

AVL* makeNode(KEY_TYPE key)
{
    AVL* p_avl = (AVL*)malloc(sizeof(AVL));
    p_avl->key = key;
    p_avl->lchild = NULL;
    p_avl->rchild = NULL;
    p_avl->height = 1;
    return p_avl;
}

int getHeight(AVL* node)
{
    return (node == NULL)? 0: node->height;
}

inline int max(int a, int b)
{
    return a > b ? a : b;
}

/* LL(Y rotates to the right):

        k2                   k1
       /  \                 /  \
      k1   Z     ==>       X   k2
     / \                       /  \
    X   Y                    Y    Z
*/

AVL* LL_Rotate(AVL* k2)
{
    AVL* k1 = k2->lchild;
    k2->lchild = k1->rchild;
    k1->rchild = k2;
    k2->height = max(getHeight(k2->lchild), getHeight(k2->rchild)) + 1;
    k1->height = max(getHeight(k1->lchild), k2->height) + 1;
    return k1;
}

/* RR(Y rotates to the left):

        k2                           k1
       /  \                         /  \
      X    k1         ==>          k2   Z
          /  \                    /  \
         Y    Z                 X    Y
 */
AVL* RR_Rotate(AVL* k2)
{
    AVL* k1 = k2->rchild;
    k2->rchild = k1->lchild;
    k1->lchild = k2;
    k2->height = max(getHeight(k2->lchild), getHeight(k2->rchild)) + 1;
    k1->height = max(getHeight(k1->rchild), k2->height) + 1;
    return k1;
}

/* LR(B rotates to the left, then C rotates to the right):

      k3                       k3                       k2
     /  \                     /  \                     /  \
    k1   D                   k2   D                  k1   k3
   /  \         ==>         /  \        ==>          / \   / \
  A    k2                  k1   C                  A  B  C   D
       /  \               /  \
      B    C            A    B

*/

AVL* LR_Rotate(AVL* k3)
{
    k3->lchild = RR_Rotate(k3->lchild);
    return LL_Rotate(k3);
}


/* RL(D rotates to the right, then C rotates to the left):

       k3                           k3                         k2
      /  \                         /  \                       /  \
     A    k1                      A    k2                   k3   k1
          /  \       ==>              /  \         ==>      / \   / \
         k2   B                     C    k1                A  C  D   B
        /  \                             /  \
       C    D                           D    B

 */

AVL* RL_Rotate(AVL* k3)
{
    k3->rchild = LL_Rotate(k3->rchild);
    return RR_Rotate(k3);
}

AVL* insertNode(AVL* root, KEY_TYPE key)
{
    if (root == NULL)
    {
        return makeNode(key);
    }

    if (root->key == key)
    {
        return root;
    }
    else if (key < root->key)
    {
        root->lchild = insertNode(root->lchild, key);
    }
    else //key >= root->key
    {
        root->rchild = insertNode(root->rchild, key);
    }

    root->height = max(getHeight(root->lchild), getHeight(root->rchild)) + 1;

    if (getHeight(root->lchild) - getHeight(root->rchild) == 2)
    {
        if(key < root->lchild->key)
        {
            root = LL_Rotate(root);
        }
        else
        {
            root = LR_Rotate(root);
        }
    }
    else if (getHeight(root->rchild) - getHeight(root->lchild) == 2)
    {
        if(key > root->rchild->key)
        {
            root = RR_Rotate(root);
        }
        else
        {
            root = RL_Rotate(root);
        }
    }
    return root;
}


AVL* deleteNode(AVL* root, KEY_TYPE key)
{
    if (root == NULL)
    {
        return NULL;
    }

    if (key == root->key)
    {
        if(root->rchild == NULL)
        {
            AVL* temp = root;
            root = root->lchild;
            free(temp);
            return root;
        }
        else
        {
            AVL* temp = root->rchild;
            while(temp->lchild)
            {
                temp = temp->lchild;
            }
            root->key = temp->key;
            root->rchild = deleteNode(root->rchild, temp->key);
        }
    }
    else if (key < root->key)
    {
        root->lchild = deleteNode(root->lchild, key);
    }
    else
    {
        root->rchild = deleteNode(root->rchild, key);
    }

    root->height = max(getHeight(root->lchild), getHeight(root->rchild)) + 1;

    if (getHeight(root->rchild) - getHeight(root->lchild) == 2)
    {
        if (getHeight(root->rchild->rchild) >= getHeight(root->rchild->lchild))
        {
            root = RR_Rotate(root);
        }
        else
        {
            root = RL_Rotate(root);
        }
    }
    else if (getHeight(root->lchild) - getHeight(root->rchild) == 2)
    {
        if (getHeight(root->lchild->lchild) >= getHeight(root->lchild->rchild))
        {
            root = LL_Rotate(root);
        }
        else
        {
            root = LR_Rotate(root);
        }
    }
    return root;
}

void print_tree_indent(AVL *node, int indent)
{
    for (int ix = 0; ix < indent; ix++)
    {
        printf(" ");
    }
    if (node == NULL)
    {
        printf("Empty Child\n");
    }
    else
    {
        printf("Node: %d; Height: %d\n", node->key, node->height);
        print_tree_indent(node->lchild, indent + 4);
        print_tree_indent(node->rchild, indent + 4);
    }
}

void print_tree(AVL *node)
{
    print_tree_indent(node, 0);
}
