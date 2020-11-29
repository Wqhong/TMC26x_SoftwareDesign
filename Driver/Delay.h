#ifndef __DELAY_H
#define __DELAY_H 			   
#include "GPIO.h"  
 
void Delay_init(u8 SYSCLK);
void Delay_us(u32 nus);
void Delay_ms(u16 nms);

#endif





























