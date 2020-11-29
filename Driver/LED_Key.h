#ifndef __LED_KEY_H
#define __LED_KEY_H
#include "GPIO.h"
#include "Delay.h"

#define LED 	PEout(7)

#define KEY0 	GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2) //PE2
#define KEY1 	GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)	//PE3 

void LED_KEY_Init(void);
u8 KEY_Scan(u8); 

#endif
