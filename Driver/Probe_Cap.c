#include "USART.h"
#include "LED_Key.h"
#include "Probe_Cap.h"
#include "CommonFunc.h"
#include "Probe_Calibrate.h"


u32	TIM5CH1_CAPTURE_VAL = 0;	//输入捕获值(TIM2/TIM5是32位)	
u8  TIM5CH1_CAPTURE_STA = 0;	//输入捕获状态		    				

/****************************************************
函数名：Probe_Cap_Init(u32 arr,u16 psc)
参数：arr：自动重装值		psc：时钟预分频数
功能：定时器5通道1输入捕获配置，测厚模块启动或者重新启动
返回值：无
****************************************************/
void Probe_Cap_Init(u32 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  	//TIM5时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //使能PORTA时钟	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 					//GPIOA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;				//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 			//推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; 			//下拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);								//初始化PA0
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5); //PA0复用位定时器5
   
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  						//定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;   							//自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);
	
	//初始化TIM5输入捕获参数
	TIM_ICInitTypeDef  TIM5_ICInitStructure;
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1; 							
	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;		
	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 				
	TIM5_ICInitStructure.TIM_ICFilter = 0x00;			  								
	TIM_ICInit(TIM5, &TIM5_ICInitStructure);
		
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0; //抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;				//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);													//根据指定的参数初始化VIC寄存器

	TIM5CH1_CAPTURE_VAL = 0;
	TIM5CH1_CAPTURE_STA = 0;
	
	if(Command != 0)
	{
		Command += 0x1000;
		printf("%x \r\n",Command);																		//返回命令值
		Command = 0;
	}
}


/****************************************************
函数名：TIM5_IRQHandler(void)
参数：无
功能：定时器5的中断服务函数
返回值：无
****************************************************/
void TIM5_IRQHandler(void)
{ 		    	
/***************************每次采样一次就发送出去************************/	
	if(TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)		//
	{
		if(TIM5CH1_CAPTURE_STA == 0)
		{
			TIM5CH1_CAPTURE_STA |= 0x80;	//避免第一个上升沿采样的数据
			TIM_Cmd(TIM5,DISABLE); 		//关闭定时器5
			TIM_SetCounter(TIM5,0);
			TIM_Cmd(TIM5,ENABLE); 		//使能定时器5
		}
		else
		{
			TIM5CH1_CAPTURE_VAL = TIM_GetCapture1(TIM5);
			TIM_Cmd(TIM5,DISABLE); 		//关闭定时器5
			TIM_SetCounter(TIM5,0);
			TIM_Cmd(TIM5,ENABLE); 		//使能定时器5
		}
	}
	TIM_ClearITPendingBit(TIM5, TIM_IT_CC1|TIM_IT_Update); 	//清除中断标志位
}


/****************************************************
函数名：Probe_Cap_DeInit(void)
参数：无
功能：断开测量状态
返回值：无
****************************************************/
void Probe_Cap_DeInit(void)
{
	TIM_ITConfig(TIM5,TIM_IT_Update|TIM_IT_CC1,DISABLE);
	TIM_Cmd(TIM5,DISABLE);
	TIM_DeInit(TIM5);
	
	TIM5CH1_CAPTURE_VAL = 0;
	TIM5CH1_CAPTURE_STA = 0;
	
	Command += 0x1000;
	printf("%x",Command);
	Command = 0;
}


void StartMeasure(u8 flag)
{
//	TIM5CH1_CAPTURE_STA = (flag == 0? 0 : Filter_N + 1);
	
	TIM_Cmd(TIM5,ENABLE); 																					//使能定时器5
	TIM_SetCounter(TIM5,0);
	TIM_ITConfig(TIM5,TIM_IT_Update|TIM_IT_CC1,ENABLE);							//允许更新中断 ,允许CC1IE捕获中断
	
//	if(!flag)
//		while(!(TIM5CH1_CAPTURE_STA == Filter_N + 1));
}





