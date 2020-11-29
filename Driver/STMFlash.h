#ifndef __STMFLASH_H__
#define __STMFLASH_H__
#include "GPIO.h" 

//FLASH��ʼ��ַ
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH����ʼ��ַ

//FLASH ��������ʼ��ַ
#define ADDR_FLASH_SECTOR_0     ((u32)0x08000000) 	//����0��ʼ��ַ, 16 Kbytes  
#define ADDR_FLASH_SECTOR_1     ((u32)0x08004000) 	//����1��ʼ��ַ, 16 Kbytes  
#define ADDR_FLASH_SECTOR_2     ((u32)0x08008000) 	//����2��ʼ��ַ, 16 Kbytes  
#define ADDR_FLASH_SECTOR_3     ((u32)0x0800C000) 	//����3��ʼ��ַ, 16 Kbytes  
#define ADDR_FLASH_SECTOR_4     ((u32)0x08010000) 	//����4��ʼ��ַ, 64 Kbytes  
#define ADDR_FLASH_SECTOR_5     ((u32)0x08020000) 	//����5��ʼ��ַ, 128 Kbytes  
#define ADDR_FLASH_SECTOR_6     ((u32)0x08040000) 	//����6��ʼ��ַ, 128 Kbytes  
#define ADDR_FLASH_SECTOR_7     ((u32)0x08060000) 	//����7��ʼ��ַ, 128 Kbytes  
#define ADDR_FLASH_SECTOR_8     ((u32)0x08080000) 	//����8��ʼ��ַ, 128 Kbytes  
#define ADDR_FLASH_SECTOR_9     ((u32)0x080A0000) 	//����9��ʼ��ַ, 128 Kbytes  
#define ADDR_FLASH_SECTOR_10    ((u32)0x080C0000) 	//����10��ʼ��ַ,128 Kbytes  
#define ADDR_FLASH_SECTOR_11    ((u32)0x080E0000) 	//����11��ʼ��ַ,128 Kbytes  

#define Flash_MicroSize		256

void STMFLASH_Write(u32 WriteAddr,u32* pBuffer,u32 NumToWrite);		//��ָ����ַ��ʼд��ָ�����ȵ�����

u32 STMFLASH_ReadWord(u32 faddr);		  	//������  
void STMFLASH_Read(u32 ReadAddr,u32* pBuffer,u32 NumToRead);   		//��ָ����ַ��ʼ����ָ�����ȵ�����
void STMFlash_ReadLast(u32 ReadAddr,u32* pBuffer, u32 NumToRead);


/***************��дFlash�е�double���͵�����*************/

FLASH_Status FLASH_ProgramDouble(uint32_t Address, int64_t Data);
void STMFLASH_WriteDouble(u32 WriteAddr,double* pBuffer,u32 NumToWrite);		//��ָ����ַ��ʼд��ָ�����ȵ�����

int64_t STMFLASH_ReadDoubleWord(u32 faddr);		  	//������  
void STMFLASH_ReadDouble(u32 ReadAddr,double* pBuffer,u32 NumToRead);   		//��ָ����ַ��ʼ����ָ�����ȵ�����
u8 STMFlash_ReadDoubleLast(u32 ReadAddr,double* pBuffer,u32 NumToRead);
						   
#endif

















