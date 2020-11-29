#ifndef __Encode_H
#define __Encode_H	

#include "GPIO.h" 
#include "Delay.h" 
#include "LED_Key.h" 

#define Encoder1		PAout(5)
#define Encoder2		PAout(6)
#define Encoder3		PAout(7)

void Encoder_Init(void);				//编码器外部中断初始化	

#endif

