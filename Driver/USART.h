#ifndef __USART_H
#define __USART_H
#include "GPIO.h"
#include "stdio.h"	
#include "stm32f4xx_conf.h"

#define EN_USART2_RX 				1			//ʹ�ܣ�1��/��ֹ��0������1����
#define USART_REC_LEN  			100  	//�����������ֽ��� 100


extern u16 Command;         			//����������

extern u8  USART2_RX_STA;         				//����״̬���
extern u8  USART2_RX_BUF[USART_REC_LEN]; 	//���ջ���,���USART2_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 

void USART2_Init(u32 BaudRate);
void USART2_SendData(char  BUF[USART_REC_LEN]);

u16 USART_SendBack(u16 Cmd);								//��⡢��USART2�յ������ݻط�
#endif


