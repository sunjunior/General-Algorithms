//============================================================================
// Name        : AVLTree.cpp
// Author      : @CodingGeek
// Version     :
// Copyright   : V1.0
// Description : AVL平衡搜索二叉树代码实现
//============================================================================
#include <stdio.h>
#include "AVL.h"

int main(int argc, char* argv[])
{
    srand(time(NULL));
    const int num = 20;
    int *pArr = (int*)malloc(sizeof(int) * num);

    for (int i = 0; i < num; i++)
    {
        pArr[i] = rand() % 100;
        printf("%d ", pArr[i]);
    }
    printf("\n=======================\n");
    AVL *root = NULL;
    for (int i = 0; i < num; i++)
    {
        root = insertNode(root, pArr[i]);
    }
    printf("AVL Tree:\n"); 
    print_tree(root); 
    int input = 0x0; 
    do 
    { 
        printf("\nPlease input the value you want to delete:\n"); 
        scanf("%d",&input);
        root = deleteNode(root, input); 
        printf("\nAfter delete %d:\n",input); 
        print_tree(root); 
    }while (getHeight(root)); 
    printf("\n"); 
    return 0;
}
