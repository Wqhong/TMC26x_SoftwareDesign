#ifndef __GPIO_INIT_H
#define __GPIO_INIT_H	
#include "GPIO.h"
#include "Delay.h"


#define ProbeRst		PEin(15)
#define RolateRst		PAin(11)

#define Webcam1			PEout(12)
#define Webcam2			PEout(13)
#define Webcam3			PEout(14)

#define Transmitter	PBout(12)

//void Webcam_Init(void);
//void ProbeRst_Init(void);
//void RolateRst_Init(void);
//void Transmitter_Init(void);

void GPIO_InitAll(void);


//void test(void);
//#define Test	PAout(15)

#endif
