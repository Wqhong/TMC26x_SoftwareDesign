#include "Oled.h"
#include "Delay.h"
#include "LED_Key.h"
#include "STMFlash.h"
#include "Probe_Cap.h"
#include "SystemTim.h"
#include "CommonFunc.h"
#include "CurveFitted.h"
#include "Probe_Calibrate.h"

u8 UpdateCoefficientFlag = 0;

static double Std_Hz[30] = {137.7445,123.077,114.99,109.116,105.3335,101.8805,99.237,96.988,95.39,93.704,
														92.348,91.0415,90.0555,89.073,88.291,87.5135,86.5735,85.9775,85.4685,84.9805,
														84.5635,84.13,83.781,83.423,83.142,82.785,82.536,82.286,82.0655,78.1686671349481};
double Std_X[30] = {137.7445,123.077,114.99,109.116,105.3335,101.8805,99.237,96.988,95.39,93.704,
										92.348,91.0415,90.0555,89.073,88.291,87.5135,86.5735,85.9775,85.4685,84.9805,
										84.5635,84.13,83.781,83.423,83.142,82.785,82.536,82.286,82.0655,78.1686671349481},		 
			 Std_Y[29] = {0,50,100,150,195,245,295,345,390,440,490,540,585,635,685,735,798,848,898,948,993,1043,
						1093,1143,1188,1238,1288,1338,1383};
			 
double 	Ca[4]={0},																						//0-150 59.763452    59.763452,-5.336241,0.120329,-0.000589
				Cb[7]={0},																						//150-735   345.7,-205.7,86.81,-48.6,11.53,7.387,-3.045
				Cc[7]={602,-223.8,68.26,-64.56,18.85,19.55,-8.281},		//735-993
				Cd[7]={1136,-159.4,40.98,13.48,-7.954,-7.091,2.779};	//993-1383
				
double	X[4]={119.722750,94.129857,88.113563,80.886},dt[3];
double 	Seg1 = 109.4063, Seg2 = 88.0909, Seg3 = 85.2617, EndHz = 78.9543 + 2;


void Parameter_WriteToFlash(u32 ReadAddr,double* X)
{
	STMFLASH_WriteDouble(ReadAddr,X,30);
	
//	printf("Ƶ������д��ɹ��� \r\n");
}

void Parameter_ReadFromFlash(u32 ReadAddr,double* X)
{
	STMFlash_ReadDoubleLast(ReadAddr,X,30);
	
//	printf("Ƶ�����ݶ�ȡ�ɹ��� \r\n");
}

				
	
/****************************************************
��������Get_Probe_Value(u8 flag)
������flag=0������������flag=1������У׼״̬�Ĳ���
���ܣ�ͨ�����ú������һ�β���
����ֵ����
****************************************************/
void Get_Probe_Value(void)
{
	double	Probe_Cap_Hz = 0, Value = -50;
	
	if(TIM5CH1_CAPTURE_VAL != 0)
		Probe_Cap_Hz = (double)1000000/TIM5CH1_CAPTURE_VAL;		
	if(InfValue < 75)
		InfValue = EndHz - 1;
	
	Probe_Cap_Hz = Probe_Cap_Hz*Std_X[29]/InfValue;
	
//		Probe_Cap_Hz = Get_Probe_Array_Val(Probe_Measure_Val_Arr,InfValue);
	if(Probe_Cap_Hz > 75)
		Value = Polynomial_GetThicknessValue(Probe_Cap_Hz);
	
	TIM5CH1_CAPTURE_VAL = 0;

	if(Value != -50)
	{
		if(OLED == 1)
			OLED_ShowValue(Value);
		
//		printf("%.3fHz - %.3fHz ",Probe_Cap_Hz,InfValue); 
		if(Value < 100)
			printf("%.1f  ",Value);
		else
			printf("%.0f  ",Value);
		
		if(Command != 0)
		{
			Command += 0x1000;
			printf("%x \r\n",Command);									//����ָ��			
		}
	}
	Command = 0;
}

double Get_Probe_Array_Val(double* Array,double Cali)
{
	double	Probe_Cap_Hz = 0, Value = -50, Probe_Cap, Val;
	
	Probe_Cap_Hz = Get_ArrayAvg(Array,0,PMVA_len-1);
	
	Probe_Cap = Probe_Cap_Hz*Std_X[29]/Cali;

	if(Probe_Cap_Hz > EndHz)
	{
		Value = Polynomial_GetThicknessValue(Probe_Cap_Hz);
		Val = Polynomial_GetThicknessValue(Probe_Cap);
	}

	if(Value != -50)
	{
		if(OLED == 1)
			OLED_ShowValue(Val);
		
		if(Value < 100)
			printf("%.1f ",Val);
		else
			printf("%.0f ",Val);
		
		if(Command != 0)
		{
			Command += 0x1000;
			printf("%x \r\n",Command);									//����ָ��			
		}
	}
	else
		Probe_Cap_Hz = 0;
	Command = 0;
	
	return Probe_Cap_Hz;
}

/***********************************************
��������Polynomial_GetThicknessValue
�������������Ƶ��ֵ
���ܣ�ͨ������Ƶ�ʼ�������
����ֵ��Ƶ�ʶ�Ӧ���
***********************************************/				
double Polynomial_GetThicknessValue(double Probe_Cap_Hz)
{
	double Value = -50;
	
	if(Probe_Cap_Hz > Seg1)
	{
		Value = Ca[0] + Ca[1]*(Probe_Cap_Hz-X[0]) + Ca[2]*pow(Probe_Cap_Hz-X[0],2) + Ca[3]*pow(Probe_Cap_Hz-X[0],3);
	}
	else if(Probe_Cap_Hz > Seg2)
	{
		Value = Cb[0] + Cb[1]*(Probe_Cap_Hz-X[1]) + Cb[2]*pow(Probe_Cap_Hz-X[1],2) + Cb[3]*pow(Probe_Cap_Hz-X[1],3) + 
		Cb[4]*pow(Probe_Cap_Hz-X[1],4) + Cb[5]*pow(Probe_Cap_Hz-X[1],5) + Cb[6]*pow(Probe_Cap_Hz-X[1],6);
	}
	else if(Probe_Cap_Hz > Seg3)
	{
		Value = Cc[0] + Cc[1]*(Probe_Cap_Hz-X[2]) + Cc[2]*pow(Probe_Cap_Hz-X[2],2) + Cc[3]*pow(Probe_Cap_Hz-X[2],3) + 
		Cc[4]*pow(Probe_Cap_Hz-X[2],4) + Cc[5]*pow(Probe_Cap_Hz-X[2],5) + Cc[6]*pow(Probe_Cap_Hz-X[2],6);
	}
	else if(Probe_Cap_Hz > EndHz)
	{
		Value = Cd[0] + Cd[1]*(Probe_Cap_Hz-X[3]) + Cd[2]*pow(Probe_Cap_Hz-X[3],2) + Cd[3]*pow(Probe_Cap_Hz-X[3],3) + 
		Cd[4]*pow(Probe_Cap_Hz-X[3],4) + Cd[5]*pow(Probe_Cap_Hz-X[3],5) + Cd[6]*pow(Probe_Cap_Hz-X[3],6);
	}

	return Value;
}
/**********************************************/


/***************************************************************
��������Interpolation_Polynomial_Calibrate(double* Arr, u8 ResetCali)
������Arr��ʾ���飬ResetCali��ʾУ׼������
���ܣ�����ResetCaliʵ������У׼ģʽ�����ⲿ����
����ֵ����
***************************************************************/
void Interpolation_Polynomial_Calibrate(double* Arr, u8 ResetCali)
{
	if(ResetCali == 1)
		Interpolation(Arr,Std_Hz,Std_X,2);
	else if(ResetCali == 8)
	{
		if(Selection_Sort(Arr,9))
			Interpolation(Arr,Std_Hz,Std_X,8);
		else	
			for(u8 i = 0; i < 30; i++)
				printf("ERROR X[%d] = %.3f \r\n",i,Std_X[i]);
	}
	else
		Selection_Sort(Arr,30);
	
	UpdateCoefficientFlag = 1;
	Parameter_WriteToFlash(FLASH_SAVE_ADDR,Std_X);
}
/*********************************************************************/



/**********************************************************************
��������AmendPolynomialCalibrate(void)
��������
���ܣ���У׼�������������У׼
����ֵ���޷���ֵ�������µ�У׼ϵ��
**********************************************************************/
void AmendPolynomialCalibrate(void)
{
	for(u8 i; i < 29 && (KEY_Scan(0)!=2); i++)	//����2��������У׼ģʽ
	{
//		Measure2Avg();

//		printf("Amend Std_X[%d]=%f  Std_Y[%d]=%f\r\n",i,Std_X[i],i,Std_Y[i]);
	}
	SubsectionPolynomial(Std_X,Std_Y,Ca,Cb,Cc,Cd);
}
/*********************************************************************/


/**********************************************************************
��������SubsectionPolynomial(double* Probe_Hz,double* a)
��������ֵ�������Probe_Hz[]��ϵ������a[]
���ܣ��ò�ֵ����������������ϵõ�ϵ������
����ֵ���޷���ֵ�������µ�У׼ϵ��
**********************************************************************/
void SubsectionPolynomial(double* Hz,double* Th,double* a,double* b,double* c,double* d)
{
	double Xa[4],Ya[4],Xb[14],Yb[14],Xc[20],Yc[20],Xd[9],Yd[9];
	
	Seg1 = Hz[3];
	Seg2 = Hz[15];
	Seg3 = Hz[20];
	EndHz = Hz[29] + 2;
	
	if((ArrayTakeOut(Hz,30,Xa,0,4))&&((ArrayTakeOut(Th,30,Ya,0,4))))
	{
		CurveFitted(Xa,Ya,4,a,4,dt);
		X[0] = Get_ArrayAvg(Xa,0,3);
	}
	if((ArrayTakeOut(Hz,30,Xb,2,14))&&((ArrayTakeOut(Th,30,Yb,2,14))))
	{
		CurveFitted(Xb,Yb,14,b,7,dt);
		X[1] = Get_ArrayAvg(Xb,0,13);
	}
	if((ArrayTakeOut(Hz,30,Xc,14,8))&&((ArrayTakeOut(Th,30,Yc,14,8))))
	{
		CurveFitted(Xc,Yc,8,c,7,dt);
		X[2] = Get_ArrayAvg(Xc,0,7);
	}
	if((ArrayTakeOut(Hz,30,Xd,20,8))&&((ArrayTakeOut(Th,30,Yd,20,8))))
	{
		CurveFitted(Xd,Yd,8,d,7,dt);
		X[3] = Get_ArrayAvg(Xd,0,7);
	}	
}
/*********************************************************************/


/****************************************************
��������KeyIncOrDec_Value(double Val)
��������Ȳ���ֵVal
���ܣ��Ժ�Ȳ���ֵ�ӻ������ʾ��OLED��
����ֵ��������ĺ��ֵVal
****************************************************/
double KeyIncOrDec_Value(double Val)
{
	double f;
	
  Val = fabs(Val);	
	f = (Val < 100)? 0.1 : 1;

	for(u8 i = 0,j= 1; KEY_Scan(1)==1 || KEY_Scan(1)==2; i++)
	{
		if(i > 9)
			j = (i > 19)? 100 : 10;

		if(KEY_Scan(1)==1)
			Val = Val + f * j;
		else if(KEY_Scan(1)==2)
		{
			if(Val >= 0.05)
				Val = Val - f * j;
			else
				Val = 0;
		}
		
		OLED_ShowValue(Val);

		Delay_ms(40);
	}
	
	return Val;
}

void KeyCalibrate_Value(double Hz,double Val)
{
	for(u8 i = 0; i < 29; i++)
	{
		if((Val - Std_Y[i-1] >= 0) && (Val - Std_Y[i] <= 0))
		{
			if(Val >= (Std_Y[i-1] + Std_Y[i])/2)
			{
				Std_Y[i] = Val;
				Std_X[i] = Hz;
			}
			else
			{
				Std_Y[i-1] = Val;
				Std_X[i-1] = Hz;
			}
			break;
		}
	}
}
/***************************************************/


/**********************************************************************
��������Interpolation(double* x,double* X0)
������ԭʼ����x[]����ֵ��������X0[]��
			EightOrTwo��ʾ�������鳤��8����2
���ܣ���һ���߸����ݵ�ʵ��ֵ��չ��һ��29������
����ֵ���޷���ֵ������һ���µĳ���Ϊ29������
**********************************************************************/
void Interpolation(double* x,double* std,double* X0,u8 EightOrTwo)	//�±�׼Ƭ7ƬУ׼
{
	if(EightOrTwo == 8)
	{
		for(u8 i = 0,j = 0; i < 8; i++)
		{
			if(i < 2)
			{
				X0[j] = x[i];
				j++;
			}
			else if(i == 2)
			{
				X0[j] = x[i];
				j++;
				X0[j] = x[i] - (x[i]-x[i+1])*(std[2]-std[j])/(std[2]-std[4]);
				j++;
			}
			else if(i == 3)
			{
				X0[j] = x[i];
				for(j = 5; j < 8; j++)
					X0[j] = x[i] - (x[i]-x[i+1])*(std[4]-std[j])/(std[4]-std[8]);
			}
			else if(i == 4)
			{
				X0[j] = x[i];
				for(j = 9; j < 16; j++)
					X0[j] = x[i] - (x[i]-x[i+1])*(std[8]-std[j])/(std[8]-std[16]);
			}
			else if(i == 5)
			{
				X0[j] = x[i];
				for(j = 17; j < 20; j++)
					X0[j] = x[i] - (x[i]-x[i+1])*(std[16]-std[j])/(std[16]-std[20]);
			}
			else if(i == 6)
			{
				X0[j] = x[i];
				for(j = 21; j < 25; j++)
					X0[j] = x[i] - (x[i]-x[i+1])*(std[20]-std[j])/(std[20]-std[25]);
			}
			else
			{
				X0[j] = x[i];
				for(j = 26; j < 29; j++)
				{
					X0[j] = x[i] * std[j]/std[25];
//					X0[j] = x[i] - (x[i]-x[i+1])*(std[25]-std[j])/(std[25]-std[29]);
				}
			}
		}
		X0[29] = x[8];
	}
	else if(EightOrTwo == 2)	
	{
		Selection_Sort(x,2);
		for(u8 i = 0; i < 30; i++)
			X0[i] = x[1] + (std[i] - std[29])/(std[0] - std[29])*(x[0] - x[1]);
	}
}
/*********************************************************************/




/****************************************************
��������FrequencyStd_WriteToFlash(void)
��������
���ܣ�����׼��ȶ�Ӧ��Ƶ��д��Flash����
����ֵ����
****************************************************/
void FrequencyStd_WriteToFlash(void)
{
	Parameter_WriteToFlash(FLASH_SAVE_ADDR,Std_X);

	Command = 0;
}
/***************************************************/

/****************************************************
��������FrequencyStd2Coefficient_ReadFromFlash(void)
��������
���ܣ���Flash�ж�����׼Ƭ��Ȳ��Զ�У׼
����ֵ����
****************************************************/
void FrequencyStd2Coefficient_ReadFromFlash(void)
{
	STMFlash_ReadDoubleLast(FLASH_SAVE_ADDR,Std_X,30);
	
	SubsectionPolynomial(Std_X,Std_Y,Ca,Cb,Cc,Cd);

	Command = 0;
	LED = 0;
}
/***************************************************/

//void CaliSendToUart(void)
//{
//	for(u8 i = 0; i < 30; i++)
//	{
//		printf("��������(Ƶ��)��X[%d] = %lf \r\n",i,Std_X[i]);
//	}
//}

