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

  //先初始化ADC1通道9 IO口
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;							//PB1 通道9
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;					
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;			
  GPIO_Init(GPIOB, &GPIO_InitStructure);								
 
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//复位结束	 
	
	ADC_TempSensorVrefintCmd(ENABLE);											//使能内部温度传感器
 

  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA失能
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div8;//预分频8分频。ADCCLK=PCLK2/8=84/8=10.5Mhz,ADC时钟最好不要超过36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);
	
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;					//非扫描模式	
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;		//关闭连续转换
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
  ADC_InitStructure.ADC_NbrOfConversion = 1;						//1个转换在规则序列中 也就是只转换规则序列1 
  ADC_Init(ADC1, &ADC_InitStructure);
	
	
	ADC_Cmd(ADC1, ENABLE);
}		

/**************************************************************
//获得ADC值
//ch: @ref ADC_channels 
//通道值 0~16取值范围为：ADC_Channel_0~ADC_Channel_16
//返回值:转换结果
**************************************************************/
u16 Get_Adc(u8 ch)   
{
	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles );	//ADC1,ADC通道,480个周期,提高采样时间可以提高精确度			    
  
	ADC_SoftwareStartConv(ADC1);										//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));	//等待转换结束

	return ADC_GetConversionValue(ADC1);						//返回最近一次ADC1规则组的转换结果
}

/*************************************************************
//获取通道ch的转换值，取times次,然后平均 
//ch:通道编号
//times:获取次数
//返回值:通道ch的times次转换结果平均值
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
	Voltage=(float)adcx*(3.32/4096);		//电压值
	Voltage = Voltage * 11;
	return Voltage;	
}

//得到温度值
double Get_Temprate(void)
{
	u32 adcx;
 	double temperate;
	adcx=Get_Adc_Average(ADC_Channel_16,10);	//读取通道16内部温度传感器通道,10次取平均
	temperate=(float)adcx*(3.32/4096);		//电压值
	temperate=(temperate-0.76)/0.0025 + 25; //转换为温度值 
//	temperate*=100;					//扩大100倍.
	return temperate;
}


void Get_VoltageAndTemprate(void)
{
	printf("%x	",Command);
	Command = 0;
	
	float Voltage,Temprate;
	Voltage = Get_Voltage();
	printf("系统电压值：%.1f	",Voltage);
	Temprate = Get_Temprate();
	printf("单片机内部温度：%.1f\r\n",Temprate);
}

	 









