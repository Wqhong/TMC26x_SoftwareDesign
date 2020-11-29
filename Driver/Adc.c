#include "Adc.h"
#include "Delay.h"
#include "USART.h"	


void  Adc_Init(void)
{    
  GPIO_InitTypeDef  		GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 

  //�ȳ�ʼ��ADC1ͨ��9 IO��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;							//PB1 ͨ��9
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;					
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;			
  GPIO_Init(GPIOB, &GPIO_InitStructure);								
 
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//��λ����	 
	
	ADC_TempSensorVrefintCmd(ENABLE);											//ʹ���ڲ��¶ȴ�����
 

  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���������׶�֮����ӳ�5��ʱ��
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMAʧ��
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div8;//Ԥ��Ƶ8��Ƶ��ADCCLK=PCLK2/8=84/8=10.5Mhz,ADCʱ����ò�Ҫ����36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);
	
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;					//��ɨ��ģʽ	
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;		//�ر�����ת��
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
  ADC_InitStructure.ADC_NbrOfConversion = 1;						//1��ת���ڹ��������� Ҳ����ֻת����������1 
  ADC_Init(ADC1, &ADC_InitStructure);
	
	
	ADC_Cmd(ADC1, ENABLE);
}		

/**************************************************************
//���ADCֵ
//ch: @ref ADC_channels 
//ͨ��ֵ 0~16ȡֵ��ΧΪ��ADC_Channel_0~ADC_Channel_16
//����ֵ:ת�����
**************************************************************/
u16 Get_Adc(u8 ch)   
{
	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles );	//ADC1,ADCͨ��,480������,��߲���ʱ�������߾�ȷ��			    
  
	ADC_SoftwareStartConv(ADC1);										//ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));	//�ȴ�ת������

	return ADC_GetConversionValue(ADC1);						//�������һ��ADC1�������ת�����
}

/*************************************************************
//��ȡͨ��ch��ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
//ch:ͨ�����
//times:��ȡ����
//����ֵ:ͨ��ch��times��ת�����ƽ��ֵ
*************************************************************/
u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		Delay_ms(1);
	}
	return temp_val/times;
} 



double Get_Voltage(void)
{
	u32 adcx;
 	double Voltage;
	adcx=Get_Adc_Average(ADC_Channel_9,20);	
	Voltage=(float)adcx*(3.32/4096);		//��ѹֵ
	Voltage = Voltage * 11;
	return Voltage;	
}

//�õ��¶�ֵ
double Get_Temprate(void)
{
	u32 adcx;
 	double temperate;
	adcx=Get_Adc_Average(ADC_Channel_16,10);	//��ȡͨ��16�ڲ��¶ȴ�����ͨ��,10��ȡƽ��
	temperate=(float)adcx*(3.32/4096);		//��ѹֵ
	temperate=(temperate-0.76)/0.0025 + 25; //ת��Ϊ�¶�ֵ 
//	temperate*=100;					//����100��.
	return temperate;
}


void Get_VoltageAndTemprate(void)
{
	printf("%x	",Command);
	Command = 0;
	
	float Voltage,Temprate;
	Voltage = Get_Voltage();
	printf("ϵͳ��ѹֵ��%.1f	",Voltage);
	Temprate = Get_Temprate();
	printf("��Ƭ���ڲ��¶ȣ�%.1f\r\n",Temprate);
}

	 









