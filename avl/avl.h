//============================================================================
// Name        : AVL.h
// Author      : @CodingGeek
// Version     : V1.0
// Copyright   :
// Description : AVL平衡搜索二叉树代码实现
//============================================================================

#ifndef AVL_H_
#define AVL_H_

typedef int KEY_TYPE;

/* AVL树结点数据结构 */
typedef struct AVL
{
    KEY_TYPE key;
    int height;
    struct AVL *lchild;
    struct AVL *rchild;
} AVL;

int getHeight(AVL* node); //获取结点深度

AVL* LL_Rotate(AVL* k2);  //LL类型
AVL* RR_Rotate(AVL* k2);  //RR类型
/*
 Return which the root pointer should point to
 */
AVL* LR_Rotate(AVL* k3); //LR类型
AVL* RL_Rotate(AVL* k3); //RL类型


AVL* insertNode(AVL* root, KEY_TYPE key); //插入结点
AVL* deleteNode(AVL* root, KEY_TYPE key); //删除结点
void print_tree(AVL *node); //打印AVL树结点

#endif /* AVL_H_ */
