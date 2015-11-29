/*************************************************************************
	> File Name: sky.c
	> Author: 
	> Mail: 
	> Created Time: 2015年11月29日 星期日 23时07分39秒
数字滑雪问题实际上是递归（动态规划）的典型问题，和走迷宫或者网络爬虫搜索策略类似。将二维数组转化为一个树的遍历算法(广度优先或者深度优先，也称作队列优先或者堆栈优先）。下面是源码，代码的核心在于此时状态到下一状态需要利用状态转移方程来获取。而问题难点在于如何利用规则设计出状态方程。然后利用递归求解（因为每个结点的遍历方式都一样）
 ************************************************************************/

#include <stdio.h>

#define MAX 100
int skiMap[MAX][MAX]; //地图存储
int skiLength[MAX][MAX];//滑行最长距离存储
int direction[4][2]= {{1,0},{0,1},{-1,0},{0,-1}};    //四个方向的坐标变化

 //判断当前要访问节点（x,y）是否在地图内
int isInSkiMap(int x, int y, int N)
{
    if (x >= 0 && x < N
        && y >= 0 && y < N)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


int DepthFirstSearch(int x, int y, int N)
{
    if (skiLength[x][y] > 1)
    {
        return skiLength[x][y];//要是该点已经处理过了，取值不是初值，直接返回该点的值即可
    }

    int maxLen = 1;

    //对四个可能能滑行的方向分别搜索
    for (int i = 0; i < 4; i++)
    {
        int nextX = x + direction[i][0];
        int nextY = y + direction[i][1];
        if (isInSkiMap(nextX, nextY, N) //该点在地图内，该点值比上一点的值小
            && skiMap[nextX][nextY] < skiMap[x][y])
        {
            //上一点的最大滑行距离=max{四个方向相邻点的最大滑行距离}+1
            skiLength[x][y] = DepthFirstSearch(nextX, nextY, N) + 1;
            if (skiLength[x][y] > maxLen)
            {
                maxLen = skiLength[x][y];
            }
        }
    }

     skiLength[x][y] = maxLen;//记录最大值，便于下次使用

     return maxLen;
}

int ski(const int *arr, int N)
{
    if ( arr == NULL || N <= 0)
    {
        return -1;
    }

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
    {
        skiLength[i][j] = 1;//所有点的滑行最长长度初始化为1，只有自身
        skiMap[i][j] = arr[N * i + j];
    }

    int longest = DepthFirstSearch(0, 0, N); //从skimap[0][0]开始搜索

    return (longest > 1)? longest:1;
}


int main(void)
{
    int a[] = {24,23,22,21,20,13,14,17,18,19,12,15,16,1,0,11,8,7,4,3,10,9,6,5,2};

    printf("%d\n", ski(a, 5));

    return 0;
}

