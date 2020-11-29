#include "Delay.h"
#include "USART.h"
#include "STMFlash.h"

uint16_t STMFLASH_GetFlashSector(u32 addr)
{
	if(addr<ADDR_FLASH_SECTOR_1)return FLASH_Sector_0;
	else if(addr<ADDR_FLASH_SECTOR_2)return FLASH_Sector_1;
	else if(addr<ADDR_FLASH_SECTOR_3)return FLASH_Sector_2;
	else if(addr<ADDR_FLASH_SECTOR_4)return FLASH_Sector_3;
	else if(addr<ADDR_FLASH_SECTOR_5)return FLASH_Sector_4;
	else if(addr<ADDR_FLASH_SECTOR_6)return FLASH_Sector_5;
	else if(addr<ADDR_FLASH_SECTOR_7)return FLASH_Sector_6;
	else if(addr<ADDR_FLASH_SECTOR_8)return FLASH_Sector_7;
	else if(addr<ADDR_FLASH_SECTOR_9)return FLASH_Sector_8;
	else if(addr<ADDR_FLASH_SECTOR_10)return FLASH_Sector_9;
	else if(addr<ADDR_FLASH_SECTOR_11)return FLASH_Sector_10; 
	return FLASH_Sector_11;	
}


/**********************������д����**************************/
void STMFLASH_Write(u32 WriteAddr,u32* pBuffer,u32 NumToWrite)	
{ 
	u32 addrx=0;
	u32 endaddr=0;	
	FLASH_Status status = FLASH_COMPLETE;

	if(WriteAddr<STM32_FLASH_BASE||WriteAddr%4)
	{
		printf("��д��ĵ�ַ�ǷǷ���ַ");
		return;//�Ƿ���ַ
	}
	
	FLASH_Unlock();									//���� 
	addrx=WriteAddr;
	if(addrx < ADDR_FLASH_SECTOR_8)
	{
		FLASH_DataCacheCmd(DISABLE);//FLASH�����ڼ�,�����ֹ���ݻ���
		
		while(STMFLASH_ReadWord(addrx)!=0XFFFFFFFF)		//Flash_MicroSize Byte�ָ�һ��С���䣬��ѯ��������
		{
			addrx = addrx + Flash_MicroSize;
			if(ADDR_FLASH_SECTOR_8 - addrx< Flash_MicroSize)		//�����������������в���
			{			
				printf("Flash��������������׼������...�\\r\n");
				addrx = WriteAddr;
				status=FLASH_EraseSector(STMFLASH_GetFlashSector(addrx),VoltageRange_3);//VCC=2.7~3.6V֮��!!
				if(status!=FLASH_COMPLETE)
				{
					printf("Flash����ʧ�ܣ\\r\n");	
					break;	//����������
				}
				printf("Flash�����ɹ��\\r\n");
			}	
		}
		
		WriteAddr = addrx;
		endaddr=addrx+NumToWrite*4;
		
		if(status==FLASH_COMPLETE)		//д������
		{
			while(WriteAddr < endaddr)	//д����
			{
				if(FLASH_ProgramWord(WriteAddr,*pBuffer)!=FLASH_COMPLETE)//д������
				{ 
					printf("Flashд��ʧ�ܣ\\r\n");
					break;	//д���쳣
				}
				else 
				{
					printf("Flash�ɹ�д��һ��u32���ͣ\\r\n");
				}
				WriteAddr+=4;
				pBuffer++;
			} 
		}
		
		FLASH_DataCacheCmd(ENABLE);	//FLASH��������,�������ݻ���
		FLASH_Lock();//����
	}
} 

u32 STMFLASH_ReadWord(u32 faddr)
{
	return *(vu32*)faddr; 
} 

void STMFLASH_Read(u32 ReadAddr,u32* pBuffer,u32 NumToRead)   	
{
	u32 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadWord(ReadAddr);	//��ȡ4���ֽ�.
		ReadAddr+=4;														//ƫ��4���ֽ�.	
	}
}

void STMFlash_ReadLast(u32 ReadAddr,u32* pBuffer, u32 NumToRead)
{
	u32 addrx;
	addrx = ReadAddr;
	while((addrx < ADDR_FLASH_SECTOR_8)&&(STMFLASH_ReadWord(addrx)!=0XFFFFFFFF))
	{
		addrx = addrx + Flash_MicroSize;
	}
	addrx = addrx - Flash_MicroSize;
	STMFLASH_Read(addrx,pBuffer,NumToRead);	
//	printf("\r\n ���������ݵĵ�ַΪ��%x \r\n",addrx);
}
/***************������д����********************************/



/***************��ȡFlash�е�double���͵�����***************/
/**************************************************************
��������FLASH_ProgramDouble(uint32_t Address, int64_t Data)
��������д�����ݵ���ʼ��ַAddress������Data
���ܣ���Flash��д��һ��Double��������
����ֵ��FLASH_Status״ֵ̬
***************************************************************/
FLASH_Status FLASH_ProgramDouble(uint32_t Address, int64_t Data)
{
  FLASH_Status status = FLASH_COMPLETE;

  assert_param(IS_FLASH_ADDRESS(Address));
  status = FLASH_WaitForLastOperation();
  
  if(status == FLASH_COMPLETE)
  {
    FLASH->CR &= CR_PSIZE_MASK;
    FLASH->CR |= FLASH_PSIZE_WORD;
    FLASH->CR |= FLASH_CR_PG;
  
    *(__IO int64_t*)Address = Data;
        
    status = FLASH_WaitForLastOperation();

    FLASH->CR &= (~FLASH_CR_PG);
  } 
  return status;
}

/*********************************************************************
��������STMFLASH_WriteDouble(u32 WriteAddr,double* pBuffer,u32 NumToWrite)
��������д�����ݵ���ʼ��ַAddress��Double��������ָ��pBuffer
			д�����ݵĳ���NumToWrite
���ܣ���Double��������ȫ��д�뵽Flash��
����ֵ����
**********************************************************************/
void STMFLASH_WriteDouble(u32 WriteAddr,double* pBuffer,u32 NumToWrite)	
{ 
	int64_t temp;
	u32 addrx=0;
	u32 endaddr=0;	
	FLASH_Status status = FLASH_COMPLETE;

	if(WriteAddr<STM32_FLASH_BASE||WriteAddr%8)
	{
		printf("��д��ĵ�ַ�ǷǷ���ַ");
		return;//�Ƿ���ַ
	}

	FLASH_Unlock();									
	addrx=WriteAddr;
	if(addrx < ADDR_FLASH_SECTOR_8)
	{
		FLASH_DataCacheCmd(DISABLE);//FLASH�����ڼ�,�����ֹ���ݻ���
		
		while(STMFLASH_ReadDoubleWord(addrx)!=0XFFFFFFFFFFFFFFFF)	//Flash_MicroSize Byte�ָ�һ��С���䣬��ѯ��������
		{
			addrx = addrx + Flash_MicroSize;
			if(ADDR_FLASH_SECTOR_8 - addrx < Flash_MicroSize)				//�����������������в���
			{			
				printf("\r\nFlash��������������׼�������\\r\n");
				addrx = WriteAddr;
				status=FLASH_EraseSector(STMFLASH_GetFlashSector(addrx),VoltageRange_3);//VCC=2.7~3.6V֮��!!
				if(status!=FLASH_COMPLETE)
				{
					printf("Flash����ʧ�ܣ\\r\n");	
					break;									//����������
				}
				printf("Flash�����ɹ��\\r\n");
			}	
		}
		
		WriteAddr = addrx;
		endaddr=addrx+NumToWrite*8;
		
		if(status==FLASH_COMPLETE)		//д������
		{
			while(WriteAddr < endaddr)	//д����
			{
				temp = (int64_t)(*pBuffer * 100000000.0);
				if(FLASH_ProgramDouble(WriteAddr,temp)!=FLASH_COMPLETE)
				{ 
					printf("Flashд��ʧ�ܣ\\r\n");
					break;	//д���쳣
				}
				WriteAddr+=8;
				pBuffer++;
			} 
		}
		
		FLASH_DataCacheCmd(ENABLE);	//FLASH��������,�������ݻ���
		FLASH_Lock();//����
	}
} 

/*******************************************************************
��������STMFLASH_ReadDoubleWord(u32 faddr)
������׼����ȡ���ݵ���ʼ��ַAddress
���ܣ���Flash�ж�ȡһ��Double��������
����ֵ��int64_t��������
********************************************************************/
int64_t STMFLASH_ReadDoubleWord(u32 faddr)
{
	return *(__IO int64_t*)faddr; 
} 

/*******************************************************************
��������STMFLASH_ReadDouble(u32 ReadAddr,double* pBuffer,u32 NumToRead)
������׼����ȡ���ݵ���ʼ��ַAddress��������ݵ�Double��������pBuffer
			��ȡ�ĳ���NumToRead
���ܣ���Flash�ж�ȡһ��Double��������
����ֵ����
********************************************************************/
void STMFLASH_ReadDouble(u32 ReadAddr,double* pBuffer,u32 NumToRead)   	
{
	for(u32 i=0;i<NumToRead;i++)
	{
		pBuffer[i]=(double)(*(__IO int64_t*)ReadAddr)/100000000.0;//��ȡ8���ֽ�.
		ReadAddr+=8;//ƫ��8���ֽ�.	
	}
}

/********************************************************************
��������STMFlash_ReadDoubleLast(u32 ReadAddr,double* pBuffer,u32 NumToRead)
������׼����ȡ���ݵ���ʼ��ַAddress��������ݵ�Double��������pBuffer
			��ȡ�ĳ���NumToRead
���ܣ���Flash�ж�ȡ���´����һ��Double��������
����ֵ����ȡ�Ƿ�ɹ�
*********************************************************************/
u8 STMFlash_ReadDoubleLast(u32 ReadAddr,double* pBuffer,u32 NumToRead)
{
	u32 addrx;
	addrx = ReadAddr;
	while((addrx < ADDR_FLASH_SECTOR_8)&&(STMFLASH_ReadDoubleWord(addrx)!=0XFFFFFFFFFFFFFFFF))
	{
		addrx = addrx + Flash_MicroSize;
	}
	addrx = addrx - Flash_MicroSize;
	
	if(addrx < ADDR_FLASH_SECTOR_7)
	{
		printf("\r\nû��Ԥ�õı�׼��Ƭֵ��\r\n");
		return 0;
	}
	else 
	{
		STMFLASH_ReadDouble(addrx,pBuffer,NumToRead);
	}
		
	return 1;	
}
