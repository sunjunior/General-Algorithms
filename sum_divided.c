/*在算法设计中，常见的设计思想有分治法，分支定界，回溯，动态规划等等。其中分治法是算法设计中最为重要的思想之一。分治法思想的本质就是分而治之，将解决一个问题的解空间一分为二，每个子问题的解空间步骤与原问题解法上类似，分治法很多时候用递归这种方式来实现。算法设计相关教材一般将归并排序作为分治法的典型应用，归并排序思想是将一个待排序的序列一分为二，递归地对每个子序列排序，这样一直分解下去，直到不能分解为止，然后从最小子序列（不能再分割的序列)排序后合并，自底向上来对整个序列排序。其实分治法还有很多运用，比如二叉树查找，快速排序等，都有分治法思想的影子。下面就拿简单的从1到n累计求和程序设计来说明分治法的思想。从1到n最简单的就是累加法，然后也有利用等差数列公式求和之后，再计算值；当然我们也可以根据求和的递归关系来用递归求解。下面给出几个版本的代码：*/

//============================================================================
// Name        : 1～n累计和，不考虑结果溢出问题
// Author      : @CodingGeek
// Version     : 1.0
// Copyright   : Your copyright notice
// Description : 1到n求和C代码实现
//============================================================================

#include <assert.h>

unsigned int sum1(unsigned int n)
{
	if (n <= 1)
	{
		return n;
	}
	int sum = 0;
	for (unsigned int i = 1; i <= n; i++)
	{
		sum += i;
	}

	return sum;
}

unsigned int sum2(unsigned int n)
{
	if (n <= 1)
	{
		return n;
	}

	return n + sum2(n - 1);
}

unsigned int sum3(unsigned int n) //1到n的和为n*(n+1)/2;
{
        return (n*(n+1) >> 1);
}




int main(void)
{
	assert(0 == sum1(0));
	assert(1 == sum1(1));
	assert(55 == sum1(10));
	assert(5050 == sum1(100));

	assert(0 == sum2(0));
	assert(1 == sum2(1));
	assert(55 == sum2(10));
	assert(5050 == sum2(100));

	assert(0 == sum3(0));
	assert(1 == sum3(1));
	assert(55 == sum3(10));
	assert(5050 == sum3(100));

        return 0;
}

/*
下面考虑用分治法来实现，主要是体会分治法这种思想的运用。从1到n求和，我们可以这样将求和规模一分为二，前面一半序列和记为S1, 后面一半序列和记为S2，然后递归地进行下去，直到能求出最小子序列（不能再分解）的和，然后将两部分和合并，自底向上地计算出原序列的和，这样就可以得到整个序列的和了，下面是实现代码：*/

//============================================================================
// Name        : 1～n累计和，不考虑结果溢出问题
// Author      : @CodingGeek
// Version     : 1.0
// Copyright   : Your copyright notice
// Description : 1到n求和C代码实现,Conquer and Divide分治法实现
//============================================================================

#include <assert.h>

unsigned int sum(unsigned int begin, unsigned int end)
{
	if (begin == end) //递归结束条件，即分解到只有一个数求和为止
	{
		return begin;
	}

	unsigned int mid = begin + (end - begin) >> 1; //将解规模一分为二
        unsigned int s1 = sum(begin, mid); //计算前半部分和
	unsigned int s2 = sum(mid + 1, end); //计算后半部分和
        unsigned int sum = s1 + s2;   //将前后两部分的和合并
	return sum;
}


int main(void)
{
	assert(0 == sum(0, 0));
	assert(1 == sum(1, 1));
	assert(45 == sum(1, 9));
	assert(55 == sum(1, 10));
	assert(5050 == sum(1, 100));
	assert(12 == sum(3, 5));
        
        return 0;
}

/*
总的来说，分治法就是将一个大的问题分解为求解规模较小的子问题，每个子问题求解方法与原问题类似，这样求出子问题的解后，将子问题的解再合并，就可以得到原问题的解了。后面工作中若有类似的问题程序设计，不妨考虑此思想！
*/

