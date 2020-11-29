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
��������Get_ArrayAvg(double* Hz, int start, int end)
����������Hz����ʼ���±�λ��start������λ��end
���ܣ���������Hz�±��start��end��ƽ��ֵ
����ֵ������õ���ƽ��ֵ
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
��������Selection_Sort(double* Hz, u8 N)
����������Hz������N
���ܣ�������Hz���дӴ�С����Nδ����ĳ���
����ֵ����������״̬
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
��������WindowFilter(double* Hz,double Hz0)
����������Hz���µĲ���Ƶ��Hz0
���ܣ�������Hz�����˲���ÿ��ȥ��������Сֵ
����ֵ������Hz��ƽ��ֵAvgValue
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
��������ArrayTakeOut(double* Array,u8 N,double* NewArray,u8 start,u8 n)
������ԭʼ����Array[]��ԭʼ���鳤��N��
			������NewArray[]����ȡ�������ʼλ�á���ȡ����ĳ���
���ܣ���һ����������ȡһ�����ݵ���һ��������ȥ
����ֵ���޷���ֵ������һ���µ�����
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

