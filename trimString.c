/*
在字符串中删除特定的字符（字符串）。输入两个字符串，从第一字符串中删除第二个字符串中所有字符
例如，输入"They are students." 和 "aeiou"。则删除之后的第一个字符串变成 "Thy r stdnts." 
其他类似的问题，处理类似上面流程，比如判断两个字符串字符是否相互包含，查找字符串中第一个只出现一次的字符等。
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


//trimString函数
//inputString：输入字符串
//pattern：待删除的字符串中所有字符
//outputString：输出已删除字符后的字符串
//返回值： 0表示成功，-1表示失败


int trimString(const char *inputString, const char *pattern, char *outputString)
{
    if (inputString == NULL || pattern == NULL)
    {
    	*outputString = '\0';
    	return -1;
    }

    int hash[256] = {0}; //字符映射为数组下标，hash简单表示

    while (*pattern != '\0')
    {
    	hash[*pattern++] = 1;
    }

    while (*inputString != '\0')
    {
    	if (hash[*inputString] == 1) //如果能找到的话，跳过
    	{
    		inputString++;
    	}
    	else //否则存入outputString中
    	{
            *outputString++ = *inputString++;
    	}
    }

    *outputString = '\0';
    return 0;

}
int main()
{
    char *src = "They are students.";
    char *pattern = "aeiou";
    char output[32];
    trimString(src, pattern, output);

    printf("%s\n", output);


    return 0;
}

