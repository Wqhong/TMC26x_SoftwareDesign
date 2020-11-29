#include "Driver_Control.h"

u8 SpeedUp, SpeedCut;
u8 ControlFlag;

void SpeedControl_TIM3(u8 Drivers, u16 Period)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE); 
	
  TIM_TimeBaseInitStructure.TIM_Period = Period; 								//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=84-1;  							//定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);		
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); 											//允许定时器3更新中断
	TIM_Cmd(TIM3,ENABLE); 																				//使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; 								//定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00;		//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00; 					//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	ControlFlag = Drivers;
	
	Command += 0x1000;
	printf("%x",Command);	
	
	Command = 0;
}
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //溢出中断
	{
		switch(ControlFlag)
		{
			case 1:	STEP_2A1 = !STEP_2A1;	break;
			case 2:	STEP_2A2 = !STEP_2A2;	break;
			case 4:	STEP_4A1 = !STEP_4A1, STEP_4A2 = !STEP_4A2;break;
			default:	break;
		}
	}	
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
}



/****************************************************
			电机正转和反转切换需不需要过渡?
****************************************************/
void Driver4A_GoForward(void)
{
	EN_4A1 = 1;
	EN_4A2 = 1;
	Delay_ms(10);
	
	Driver_4A_Init();
	
	DIR_4A1 = 1;
	DIR_4A2 = 1;
	
	SpeedUp = 3;
	SpeedCut = 3;
	
	SpeedControl_TIM3(Driver4A,100);
}
void Driver4A_GoBack(void)
{
	EN_4A1 = 1;
	EN_4A2 = 1;
	Delay_ms(10);
	
	Driver_4A_Init();
	
	DIR_4A1 = 0;
	DIR_4A2 = 0;

	SpeedUp = 3;
	SpeedCut = 3;	
	
	SpeedControl_TIM3(Driver4A,100);
}
void AllDriver_Stop(void)
{
	TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE); 		
	TIM_DeInit(TIM3);
	
	EN_2A1 = 1;
	EN_2A2 = 1;
	EN_4A1 = 1;
	EN_4A2 = 1;
	
	SpeedUp = 3;
	SpeedCut = 3;
	ControlFlag = 0;
	
	Command += 0x1000;
	printf("%x",Command);
	
	Command = 0;
}
void Driver4A_SpeedUp(void)
{
	if(SpeedCut != 3)
	{
		SpeedUp = SpeedCut;
		SpeedCut = 3;
	}
	if(SpeedUp < 7)
	{
		SpeedUp = SpeedUp + 1;
		switch(SpeedUp)
		{
			case 0:	SpeedControl_TIM3(Driver4A,200);break;
			case 1:	SpeedControl_TIM3(Driver4A,160);break;
			case 2:	SpeedControl_TIM3(Driver4A,125);break;
			case 3:	SpeedControl_TIM3(Driver4A,100);break;
			case 4:	SpeedControl_TIM3(Driver4A,80);	break;
			case 5:	SpeedControl_TIM3(Driver4A,64);	break;
			case 6:	SpeedControl_TIM3(Driver4A,40);	break;
			case 7:	SpeedControl_TIM3(Driver4A,20);	break;
			default:	SpeedUp = 7; break;
		}		
	}
		
	Command = 0;
}
void Driver4A_SpeedCut(void)
{
	if(SpeedUp != 3)
	{
		SpeedCut = SpeedUp;
		SpeedUp = 3;
	}
	if(SpeedCut > 0)
	{
		SpeedCut = SpeedCut - 1;
		switch(SpeedCut)
		{
			case 7:	SpeedControl_TIM3(Driver4A,20);	break;
			case 6:	SpeedControl_TIM3(Driver4A,40);	break;
			case 5:	SpeedControl_TIM3(Driver4A,64);	break;
			case 4:	SpeedControl_TIM3(Driver4A,80);	break;
			case 3:	SpeedControl_TIM3(Driver4A,100);break;
			case 2:	SpeedControl_TIM3(Driver4A,125);break;
			case 1:	SpeedControl_TIM3(Driver4A,160);break;
			case 0:	SpeedControl_TIM3(Driver4A,200);break;
			default:	SpeedCut = 0;	break;
		}
	}
	
	Command = 0;
}
///////////////////////////////////////////////////////


/**************暂时将2A1当做旋转探头电机**************/
void Driver2A1_Rolate(u8 Direct)
{
	Driver_2A1_Init();
	DIR_2A1 = Direct;
	
	SpeedControl_TIM3(Driver2A1,100);
}
void Driver2A1_ReSet(void)
{
	Driver_2A1_Init();
	DIR_2A1 = !DIR_2A1;
	
	SpeedControl_TIM3(Driver2A1,100);
}
void Driver2A1_Stop(void)
{
	TIM_DeInit(TIM3);
	EN_2A1 = 1;
}
////////////////////////////////////////////////////////


/*****************Driver2A2的推出和退回****************/
void Driver2A2_PushOut(void)
{
	Driver_2A2_Init();
	DIR_2A2 = 1;
	
	SpeedControl_TIM3(Driver2A2,100);
}
void Driver2A2_ReSet(void)
{
	Driver_2A2_Init();
	DIR_2A2 = 0;
	
	SpeedControl_TIM3(Driver2A2,100);
}
void Driver2A2_Stop(void)
{
	TIM_DeInit(TIM3);
	EN_2A2 = 1;
}
////////////////////////////////////////////////////////


