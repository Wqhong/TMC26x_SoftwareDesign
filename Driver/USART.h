#ifndef __USART_H
#define __USART_H
#include "GPIO.h"
#include "stdio.h"	
#include "stm32f4xx_conf.h"

#define EN_USART2_RX 				1			//使能（1）/禁止（0）串口1接收
#define USART_REC_LEN  			100  	//定义最大接收字节数 100


extern u16 Command;         			//接收命令字

extern u8  USART2_RX_STA;         				//接收状态标记
extern u8  USART2_RX_BUF[USART_REC_LEN]; 	//接收缓冲,最大USART2_REC_LEN个字节.末字节为换行符 

void USART2_Init(u32 BaudRate);
void USART2_SendData(char  BUF[USART_REC_LEN]);

u16 USART_SendBack(u16 Cmd);								//检测、将USART2收到的数据回发
#endif


