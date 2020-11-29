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


/**********************正常读写数据**************************/
void STMFLASH_Write(u32 WriteAddr,u32* pBuffer,u32 NumToWrite)	
{ 
	u32 addrx=0;
	u32 endaddr=0;	
	FLASH_Status status = FLASH_COMPLETE;

	if(WriteAddr<STM32_FLASH_BASE||WriteAddr%4)
	{
		printf("待写入的地址是非法地址");
		return;//非法地址
	}
	
	FLASH_Unlock();									//解锁 
	addrx=WriteAddr;
	if(addrx < ADDR_FLASH_SECTOR_8)
	{
		FLASH_DataCacheCmd(DISABLE);//FLASH擦除期间,必须禁止数据缓存
		
		while(STMFLASH_ReadWord(addrx)!=0XFFFFFFFF)		//Flash_MicroSize Byte分割一个小区间，查询可用区间
		{
			addrx = addrx + Flash_MicroSize;
			if(ADDR_FLASH_SECTOR_8 - addrx< Flash_MicroSize)		//第七扇区已满，进行擦除
			{			
				printf("Flash第七扇区已满，准备擦除...\r\n");
				addrx = WriteAddr;
				status=FLASH_EraseSector(STMFLASH_GetFlashSector(addrx),VoltageRange_3);//VCC=2.7~3.6V之间!!
				if(status!=FLASH_COMPLETE)
				{
					printf("Flash擦除失败\r\n");	
					break;	//发生错误了
				}
				printf("Flash擦除成功\r\n");
			}	
		}
		
		WriteAddr = addrx;
		endaddr=addrx+NumToWrite*4;
		
		if(status==FLASH_COMPLETE)		//写入数据
		{
			while(WriteAddr < endaddr)	//写数据
			{
				if(FLASH_ProgramWord(WriteAddr,*pBuffer)!=FLASH_COMPLETE)//写入数据
				{ 
					printf("Flash写入失败\r\n");
					break;	//写入异常
				}
				else 
				{
					printf("Flash成功写入一个u32类型\r\n");
				}
				WriteAddr+=4;
				pBuffer++;
			} 
		}
		
		FLASH_DataCacheCmd(ENABLE);	//FLASH擦除结束,开启数据缓存
		FLASH_Lock();//上锁
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
		pBuffer[i]=STMFLASH_ReadWord(ReadAddr);	//读取4个字节.
		ReadAddr+=4;														//偏移4个字节.	
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
//	printf("\r\n 读到的数据的地址为：%x \r\n",addrx);
}
/***************正常读写数据********************************/



/***************读取Flash中的double类型的数据***************/
/**************************************************************
函数名：FLASH_ProgramDouble(uint32_t Address, int64_t Data)
参数：待写入数据的起始地址Address、数据Data
功能：向Flash中写入一个Double类型数据
返回值：FLASH_Status状态值
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
函数名：STMFLASH_WriteDouble(u32 WriteAddr,double* pBuffer,u32 NumToWrite)
参数：待写入数据的起始地址Address、Double类型数据指针pBuffer
			写入数据的长度NumToWrite
功能：将Double类型数组全部写入到Flash中
返回值：无
**********************************************************************/
void STMFLASH_WriteDouble(u32 WriteAddr,double* pBuffer,u32 NumToWrite)	
{ 
	int64_t temp;
	u32 addrx=0;
	u32 endaddr=0;	
	FLASH_Status status = FLASH_COMPLETE;

	if(WriteAddr<STM32_FLASH_BASE||WriteAddr%8)
	{
		printf("待写入的地址是非法地址");
		return;//非法地址
	}

	FLASH_Unlock();									
	addrx=WriteAddr;
	if(addrx < ADDR_FLASH_SECTOR_8)
	{
		FLASH_DataCacheCmd(DISABLE);//FLASH擦除期间,必须禁止数据缓存
		
		while(STMFLASH_ReadDoubleWord(addrx)!=0XFFFFFFFFFFFFFFFF)	//Flash_MicroSize Byte分割一个小区间，查询可用区间
		{
			addrx = addrx + Flash_MicroSize;
			if(ADDR_FLASH_SECTOR_8 - addrx < Flash_MicroSize)				//第七扇区已满，进行擦除
			{			
				printf("\r\nFlash第七扇区已满，准备擦除\r\n");
				addrx = WriteAddr;
				status=FLASH_EraseSector(STMFLASH_GetFlashSector(addrx),VoltageRange_3);//VCC=2.7~3.6V之间!!
				if(status!=FLASH_COMPLETE)
				{
					printf("Flash擦除失败\r\n");	
					break;									//发生错误了
				}
				printf("Flash擦除成功\r\n");
			}	
		}
		
		WriteAddr = addrx;
		endaddr=addrx+NumToWrite*8;
		
		if(status==FLASH_COMPLETE)		//写入数据
		{
			while(WriteAddr < endaddr)	//写数据
			{
				temp = (int64_t)(*pBuffer * 100000000.0);
				if(FLASH_ProgramDouble(WriteAddr,temp)!=FLASH_COMPLETE)
				{ 
					printf("Flash写入失败\r\n");
					break;	//写入异常
				}
				WriteAddr+=8;
				pBuffer++;
			} 
		}
		
		FLASH_DataCacheCmd(ENABLE);	//FLASH擦除结束,开启数据缓存
		FLASH_Lock();//上锁
	}
} 

/*******************************************************************
函数名：STMFLASH_ReadDoubleWord(u32 faddr)
参数：准备读取数据的起始地址Address
功能：从Flash中读取一个Double类型数据
返回值：int64_t类型数据
********************************************************************/
int64_t STMFLASH_ReadDoubleWord(u32 faddr)
{
	return *(__IO int64_t*)faddr; 
} 

/*******************************************************************
函数名：STMFLASH_ReadDouble(u32 ReadAddr,double* pBuffer,u32 NumToRead)
参数：准备读取数据的起始地址Address、存放数据的Double类型数组pBuffer
			读取的长度NumToRead
功能：从Flash中读取一组Double类型数据
返回值：无
********************************************************************/
void STMFLASH_ReadDouble(u32 ReadAddr,double* pBuffer,u32 NumToRead)   	
{
	for(u32 i=0;i<NumToRead;i++)
	{
		pBuffer[i]=(double)(*(__IO int64_t*)ReadAddr)/100000000.0;//读取8个字节.
		ReadAddr+=8;//偏移8个字节.	
	}
}

/********************************************************************
函数名：STMFlash_ReadDoubleLast(u32 ReadAddr,double* pBuffer,u32 NumToRead)
参数：准备读取数据的起始地址Address、存放数据的Double类型数组pBuffer
			读取的长度NumToRead
功能：从Flash中读取最新存入的一组Double类型数据
返回值：读取是否成功
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
		printf("\r\n没有预置的标准样片值！\r\n");
		return 0;
	}
	else 
	{
		STMFLASH_ReadDouble(addrx,pBuffer,NumToRead);
	}
		
	return 1;	
}
