#include "Oled.h"
#include "USART.h"
#include "LED_Key.h"
#include "SystemTim.h"
#include "CommonFunc.h"
#include "Probe_Calibrate.h"

extern double Std_X[30];

u16 CALI_STA = 0, CALI_Avg_Cnt = 0;		//校准测量的状态,校准测量时计数取平均
double HzFilter[5] = {0},CALI_Hz_Array[30] = {0},DatumPointFilter[5] = {0},InfPoint = 0,DatumPoint = 0;

u8 OneTimeMeasure = 0;	//判断是都完成一次测量	
double Probe_Measure_Val_Arr[PMVA_len] = {0};
double ThicknessValue,lastHzValue,InfValue = 0,AvgHzValue = 0;				
/**********************************测量使用的变量*************************************/

void SystemTim_Init(u32 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  	//TIM2时钟使能  
  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  						//定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;   							//自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
		
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1; //抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;				//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);													//根据指定的参数初始化VIC寄存器
	
	TIM_Cmd(TIM2,ENABLE); 																					//使能定时器5
	TIM_SetCounter(TIM2,0);
	TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC1,ENABLE);							//允许更新中断 ,允许CC1IE捕获中断
}

void TIM2_IRQHandler(void)						//每10ms进入一次中断。作为系统时钟使用
{
	if(TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
	{
		if(!CALI_STA)
			Measure_One_Thickness();
		else if(CALI_STA & 0x4000)
			Polynomial_Calibrate(1);
		else if(CALI_STA & 0x0080)
			Polynomial_Calibrate(8);
		else
			Polynomial_Calibrate(29);
	}
	TIM_ClearITPendingBit(TIM2, TIM_IT_CC1|TIM_IT_Update);
}




/************************************校准测量******************************************/
void Polynomial_Calibrate(u8 ResetCali)
{
	Calibrate_Measure2Avg();
	if(ResetCali == 1)
	{
		if((CALI_STA & 0x0100) && (!(CALI_STA & 0x0F)))
		{
			CALI_Hz_Array[0] = AvgHzValue;
			CALI_Hz_Array[1] = InfPoint;
			
			CALI_STA = 0;
			AvgHzValue = 0;
			Interpolation_Polynomial_Calibrate(CALI_Hz_Array,ResetCali);
		}
	}
	else if(ResetCali == 8)
	{
		if((CALI_STA & 0x0100) && ((CALI_STA & 0x0F) < 8))
		{
			CALI_Hz_Array[CALI_STA & 0x0F] = AvgHzValue;
			if(!(CALI_STA & 0x0F))
				CALI_Hz_Array[8] = DatumPoint = InfPoint;
			else
				CALI_Hz_Array[8] = (CALI_Hz_Array[8] + InfPoint)/2;
			
			printf("InfPoint=%f CALI_Hz_Array[%d]=%f	\r\n",InfPoint,CALI_STA&0x0F,CALI_Hz_Array[CALI_STA & 0x0F]);
			
			CALI_STA &= 0xEF;
			CALI_STA += 1;
			AvgHzValue = 0;
		}
		if((CALI_STA & 0x0F) == 0x08)
		{
			Interpolation_Polynomial_Calibrate(CALI_Hz_Array,ResetCali);
			CALI_STA = 0;
			AvgHzValue = 0;	
		}
	}
	else
	{
		if((CALI_STA & 0x0100) && ((CALI_STA & 0x1F) < 29))
		{
			CALI_Hz_Array[CALI_STA & 0x1F] = AvgHzValue;
			if(!(CALI_STA & 0x1F))
			{
				CALI_Hz_Array[29] = InfPoint;
				DatumPoint = InfPoint;
			}
			printf("InfPoint=%f CALI_Hz_Array[%d]=%f	\r\n",InfPoint,CALI_STA&0x1F,CALI_Hz_Array[CALI_STA & 0x1F]);
			
			CALI_STA &= 0xEF;
			CALI_STA += 1;
			AvgHzValue = 0;
		}
		if((CALI_STA & 0x1F) == 29)
		{
			Interpolation_Polynomial_Calibrate(CALI_Hz_Array,ResetCali);
			CALI_STA = 0;
			AvgHzValue = 0;
		}		
	}
}

double Calibrate_Measure2Avg(void)
{
	double Hz_Value;

	Hz_Value = Measure_One_Thickness();
	if(Hz_Value != 0)
	{
		if(CALI_Avg_Cnt < 5)
		{
			HzFilter[CALI_Avg_Cnt] = Hz_Value;
			DatumPointFilter[CALI_Avg_Cnt] = InfValue;
			AvgHzValue = AvgHzValue*CALI_Avg_Cnt/(CALI_Avg_Cnt+1)+Hz_Value/(CALI_Avg_Cnt+1);
			InfPoint = InfPoint*CALI_Avg_Cnt/(CALI_Avg_Cnt+1)+InfValue/(CALI_Avg_Cnt+1);
			CALI_Avg_Cnt += 1;
		}
		else
		{
			AvgHzValue = WindowFilter(HzFilter,Hz_Value,AvgHzValue,5);
			InfPoint = WindowFilter(DatumPointFilter,InfValue,InfPoint,5);
		}
	
		ThicknessValue = Polynomial_GetThicknessValue(AvgHzValue);
		
		if(CALI_STA == 0)
		{
			AvgHzValue = 0;
			CALI_Avg_Cnt = 0;
		}
		printf(" AvgVal=%.3f ",AvgHzValue);
		
		if(OLED == 1)
			OLED_ShowValue(ThicknessValue);			
	}

	if((AvgHzValue != 0) && ((CALI_STA & 0x20)||(KEY_Scan(1)==1)))
	{
		lastHzValue = AvgHzValue;
		
		CALI_Avg_Cnt = 0;
		CALI_STA &= ~0x20;
		CALI_STA |= 0x0100;			//0x0100标记得到一个平均后的校准值						
	}		
	 
	return AvgHzValue;
}
/************************************校准测量******************************************/





/************************************正常测量******************************************/
double Measure_One_Thickness(void)
{
	double Ret_Hz = 0;
	
	if(TIM5CH1_CAPTURE_VAL != 0)
	{
		Save_Latest_Val(TIM5CH1_CAPTURE_VAL,Probe_Measure_Val_Arr);
		
		if(Judge_Measure_isVal(Probe_Measure_Val_Arr))
		{
			if(!(OneTimeMeasure & 0x80) && (InfValue > 75))
			{
				OneTimeMeasure |= 0x80;
				Ret_Hz = Get_Probe_Array_Val(Probe_Measure_Val_Arr,InfValue);
			}
		}
	}
	return Ret_Hz;
}

u8 Judge_Measure_isVal(double* Probe_Measure_Val_Arr)
{
	u8 ret = 0;
	double temp[PMVA_len]= {0};
	
	for(u8 i = 0; i < PMVA_len; i++)
		temp[i] = Probe_Measure_Val_Arr[i];
	
	Selection_Sort(temp,PMVA_len);
	if((Probe_Measure_Val_Arr[PMVA_len-1] > Seg1) && (temp[0] - temp[PMVA_len-1] < temp[0]/512))
			ret = 1;
	else if((Probe_Measure_Val_Arr[PMVA_len-1] > Seg2) && (temp[0] - temp[PMVA_len-1] < temp[0]/832))
			ret = 1;
	else if((Probe_Measure_Val_Arr[PMVA_len-1] > EndHz) && (temp[0] - temp[PMVA_len-1] < temp[0]/1024))
			ret = 1;
	
	if((temp[0] < EndHz) && (temp[0] - temp[PMVA_len-1] < temp[0]/1250))
	{
		if(InfValue > 0)
			InfValue = (Get_ArrayAvg(temp,0,PMVA_len-1) + InfValue)/2;
		else
			InfValue = Get_ArrayAvg(temp,0,PMVA_len-1);
		
		if(OneTimeMeasure & 0x80)
		{
			printf(" %x\r\n",0x9008);
			OneTimeMeasure &= ~0x80;
		}
	}
		
	return ret;
}

void Save_Latest_Val(u32 TIM5CH1_CAPTURE_VAL, double* Probe_Measure_Val_Arr)
{
	for(u8 i = 0; i < PMVA_len - 1; i++)
	{
		Probe_Measure_Val_Arr[i] = Probe_Measure_Val_Arr[i+1];
	}
	Probe_Measure_Val_Arr[PMVA_len - 1] = (double)1000000/TIM5CH1_CAPTURE_VAL;

	TIM5CH1_CAPTURE_VAL = 0;
}
/************************************正常测量******************************************/




