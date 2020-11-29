#ifndef __ADC_H
#define __ADC_H	
#include "GPIO.h" 
 							   
void Adc_Init(void); 				//ADC通道初始化
u16  Get_Adc(u8 ch); 				//获得某个通道值 
u16 Get_Adc_Average(u8 ch,u8 times);//得到某个通道给定次数采样的平均值  

double Get_Voltage(void);		//获取系统电压值
double Get_Temprate(void);	//获取内部温度值
void Get_VoltageAndTemprate(void);	//向USART2发送系统电压和内部温度
#endif 


/**************************************************************************
	在主函数中的调用
	
	Adc_Init();         //初始化ADC  
	
	adcx=Get_Adc_Average(ADC_Channel_9,20);//获取通道5的转换值，20次取平均
	printf("采样量化值：%d\r\n",adcx);
	temp=(float)adcx*(3.3/4096);          //获取计算后的带小数的实际电压值，比如3.1111
	OLED_ShowString(0,0,"ADC Test");
	OLED_FloatNumber(0,2,temp);
***************************************************************************/












