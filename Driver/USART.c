#include "GPIO.h"
#include "USART.h"	


#if 1
#pragma import(__use_no_semihosting)                            
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
void _sys_exit(int x) 			
{ 
	x = x; 
} 

int fputc(int ch, FILE *f)
{ 	
	while((USART2->SR&0X40)==0); 
	USART2->DR = (u8) ch;      
	return ch;
}
#endif
/************printf函数自动调用USART2的发送函数，也可以调用USART2_SendData(BUF)*************/



u8 USART_LEN = 0;					//只接收两个字节的命令
u16 Command = 0;

//初始化USART2的IO口
void USART2_Init(u32 BaudRate)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
 
	//串口2对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); 
	
	//USART2端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(GPIOA,&GPIO_InitStructure); 

  //USART2 初始化设置
	USART_InitStructure.USART_BaudRate = BaudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	
  USART_Init(USART2, &USART_InitStructure); 
	
  USART_Cmd(USART2, ENABLE);  //使能串口2 
	//USART_ClearFlag(USART2, USART_FLAG_TC);
	
#if EN_USART2_RX	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart2 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//串口2中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
#endif
}

//发送的数据必须是0x0d 0x0a结尾
void USART2_SendData(char  BUF[USART_REC_LEN])
{
	unsigned int i =0;
	char SendFlag = 0;
	for(i = 0; i < USART_REC_LEN; i++)
	{
		while(!USART_GetFlagStatus(USART2,USART_FLAG_TXE));
		USART_SendData(USART2,BUF[i]);
		if(SendFlag == 1)
		{
			if(BUF[i] != 0x0A)SendFlag = 0;
			else 
			{
				SendFlag = 2;
				break;
			}
		}
		else
			if(BUF[i] == 0x0D)SendFlag = 1;
	}
}


u16 USART_SendBack(u16 Cmd)
{
	if(Cmd == 0)
	{
		if(USART2_RX_STA&0x80)
		{					   
			Command = USART2_RX_BUF[0];
			Command <<= 8;
			Command = Command + USART2_RX_BUF[1];
			USART2_RX_STA=0;
		}
	}
	else
	{
		if(USART2_RX_STA&0x80)
		{					   
			Cmd = USART2_RX_BUF[0];
			Cmd <<= 8;
			Cmd = Cmd + USART2_RX_BUF[1];
			USART2_RX_STA=0;
		}
	}
	return Cmd;
}


#if EN_USART2_RX   									//如果使能了USART2接收
 	
u8 USART2_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
u8 USART2_RX_STA=0;       //bit15--接收完成标志  bit14--接收到0x0d   bit13~0--接收到的有效字节数

void USART2_IRQHandler(void)                							//串口2中断服务程序
{
	u8 Res;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  	//接收中断
	{
		Res =USART_ReceiveData(USART2);	//(USART2->DR);				//读取接收到的数据
		if((USART2_RX_STA&0x80)==0)
		{
			USART2_RX_BUF[USART2_RX_STA&0X3F] = Res;
			USART2_RX_STA++;
			if(USART2_RX_STA == 2)
			{
				USART2_RX_STA = USART2_RX_STA | 0x80;
			}			
		}
	}
} 
#endif	
