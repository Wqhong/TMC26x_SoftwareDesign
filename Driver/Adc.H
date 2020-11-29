#ifndef __ADC_H
#define __ADC_H	
#include "GPIO.h" 
 							   
void Adc_Init(void); 				//ADCͨ����ʼ��
u16  Get_Adc(u8 ch); 				//���ĳ��ͨ��ֵ 
u16 Get_Adc_Average(u8 ch,u8 times);//�õ�ĳ��ͨ����������������ƽ��ֵ  

double Get_Voltage(void);		//��ȡϵͳ��ѹֵ
double Get_Temprate(void);	//��ȡ�ڲ��¶�ֵ
void Get_VoltageAndTemprate(void);	//��USART2����ϵͳ��ѹ���ڲ��¶�
#endif 


/**************************************************************************
	���������еĵ���
	
	Adc_Init();         //��ʼ��ADC  
	
	adcx=Get_Adc_Average(ADC_Channel_9,20);//��ȡͨ��5��ת��ֵ��20��ȡƽ��
	printf("��������ֵ��%d\r\n",adcx);
	temp=(float)adcx*(3.3/4096);          //��ȡ�����Ĵ�С����ʵ�ʵ�ѹֵ������3.1111
	OLED_ShowString(0,0,"ADC Test");
	OLED_FloatNumber(0,2,temp);
***************************************************************************/











