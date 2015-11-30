// Name        :堆排序算法
// Author      : @CodingGeek
// Version     : 1.0
// Copyright   : Your copyright notice
// Description : 堆排序算法C代码实现
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PARENT(i) (i)/2
#define LEFT(i) 2 * (i) + 1
#define RIGHT(i) 2 * (i + 1)

void swap(int &a, int &b)
{
	int temp = a;
	 a = b;
	 b = temp;
}

void max_heapify(int arr[], int len, int index)
{
    int left = LEFT(index);
    int right = RIGHT(index);
    int largest = index;

    if (left < len && arr[left] > arr[index])
    {
    	largest = left;
    }

    if (right < len && arr[right] > arr[largest])
    {
    	largest = right;
    }

    if (largest != index)
    {
        swap(arr[largest], arr[index]);
        max_heapify(arr, len, largest);
    }
}


void build_max_heap(int arr[], int len)
{
	int i = 0;
	if (len <= 1)
	{
		return;
	}

	for (i = len/2 - 1; i >= 0; i--)
	{
		max_heapify(arr, len, i);
	}

}

void heap_sort(int arr[], int len)
{
	int i = 0;
	if (arr == NULL || len <= 1)
	{
		return;
	}
	build_max_heap(arr, len);
	for (i = len - 1; i >= 1; --i)
	{
		swap(arr[0], arr[i]);
		max_heapify(arr, --len, 0);
	}
}


int main(void)
{

    const int arr_num = 15;
    int *pstArr =  new int[arr_num];
	srand(time(NULL));

    for (int i = 0; i < arr_num; i++)
    {
    	pstArr[i] = rand() % 100;
    	printf("%d ", pstArr[i]);
    }
    printf("*************\n");
    heap_sort(pstArr, arr_num);
    for (int i = 0; i < arr_num; i++)
    {
       	printf("%d ", pstArr[i]);
    }

    delete []pstArr;


    return 0;
}
