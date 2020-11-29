#include "SystemTim.h"
#include "CommonFunc.h"


void Array_Copy(double* x, double* y, u8 len)
{
	for(u8 i = 0; i < len; i++)
	{
		y[i] = x[i];
	}
}

/****************************************************
函数名：Get_ArrayAvg(double* Hz, int start, int end)
参数：数组Hz、开始的下标位置start、结束位置end
功能：计算数组Hz下标从start到end的平均值
返回值：计算得到的平均值
****************************************************/
double Get_ArrayAvg(double* Hz, int start, int end)
{
	double Avg = 0;
	
	for(u8 i = start; i < end + 1; i++)
	{
		Avg = Avg + Hz[i];
	}
	
	Avg = Avg/(end - start + 1);
	return Avg;
}

/****************************************************
函数名：Selection_Sort(double* Hz, u8 N)
参数：数组Hz、常数N
功能：对数组Hz进行从大到小排序，N未数组的长度
返回值：返回排序状态
****************************************************/
u8 Selection_Sort(double* Hz, u8 N)
{
	u8 i,j,t;
	double temp;
	for(i = 0; i <N ; i++)
	{
		t = i;
		for(j = i+1; j < N; j++)
		{
			if(Hz[j]>Hz[t])
				t = j;
		}
		if(t != i)
		{
			temp = Hz[t];
			Hz[t] = Hz[i];
			Hz[i] = temp;
		}
	}
	return 1;
}
/*********************************************************************/

/****************************************************
函数名：WindowFilter(double* Hz,double Hz0)
参数：数组Hz、新的测量频率Hz0
功能：对数组Hz进行滤波，每次去掉最大或最小值
返回值：数组Hz的平均值AvgValue
****************************************************/
double WindowFilter(double* Hz,double Hz0,double AvgValue,u8 len)
{
	double temp1,temp2;
	
	Selection_Sort(Hz,len);	
				
	if(!((Hz0 < Hz[4])||(Hz0 > Hz[0])))
	{
		temp1 = Hz[0] - AvgValue;
		temp2 = AvgValue - Hz[4];
		if(temp1 > temp2)
		{
			AvgValue = AvgValue + (Hz0- Hz[0])/5; 
			Hz[0] = Hz0;
		}
		else
		{
			AvgValue = AvgValue + (Hz0- Hz[4])/5;
			Hz[4] = Hz0;	
		}
	}
	
	return AvgValue;
}
/*********************************************************************/


/**********************************************************************
函数名：ArrayTakeOut(double* Array,u8 N,double* NewArray,u8 start,u8 n)
参数：原始数组Array[]、原始数组长度N、
			新数组NewArray[]、提取数组的起始位置、提取数组的长度
功能：从一个数组中提取一组数据到另一个数组中去
返回值：无返回值、产生一个新的数组
**********************************************************************/
u8 ArrayTakeOut(double* Array,u8 N,double* NewArray,u8 start,u8 n)
{
	if(start + n <= N)
	{
		for(u8 i = start; i < start + n; i++)
		{
			NewArray[i-start] = Array[i];
		}
	}
	else
	{
		return 0;
	}
	
	return 1;
}
/***************************************************/

