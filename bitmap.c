//============================================================================
// Name        : bitmap演示程序
// Author      : @CodingGeek
// Version     : 1.0
// Copyright   : Your copyright notice
// Description :bitmap的UINT32代码示例
/*位图法即bitmap算法，实际是用每一位来存放某种状态。在项目中运用的比较多，常常用于位图法将某些运行过程中的关键步骤记录下来，便于后期运行过程定位跟踪。位图法也常常用于处理大数据，比如用1个int32类型的变量可以表示0～31,如果用常见的int32类型来表示这些数据，则需要32个int类型的变量表示，这极大地浪费了内存。一般来说0～A范围内的数据可以用A/32 int类型大小的数组表示*/
//============================================================================
#include <stdio.h>

#define bitmap_set(puiBitmap, A) puiBitmap[(A)/32] |= (1UL << ((A) & 31))
#define bitmap_unset(puiBitmap, A) puiBitmap[(A)/32] &= ~(1UL << ((A) & 31))

void testBitmap(void)
{
	unsigned int uiBitmap[32] = {0};
	for (int i = 32; i < 128; i++)
	{
		bitmap_set(uiBitmap, i);
	}

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			if (uiBitmap[i]>>(j) & 0x1)
			{
				printf("%d\n", i * 32 + j);
			}
		}
	}

	printf("****************************\n");

	for (int i = 64; i < 96; i++)
	{
		bitmap_unset(uiBitmap, i);
	}

	for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 32; j++)
			{
				if (uiBitmap[i]>>(j) & 0x1)
				{
					printf("%d\n", i * 32 + j);
				}
			}
		}
}




int main(void)
{
    testBitmap();
    return 0;
}

