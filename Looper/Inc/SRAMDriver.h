#ifndef _SRAM_H
#define _SRAM_H

#include "main.h"

#define		SRAM_0							0
#define		SRAM_1							1
#define		SRAM_2							2
#define		SRAM_3							3
#define		SRAM_4							4
#define		SRAM_5							5
#define		SRAM_6							6
#define		SRAM_7							7

#define  	CMD_SRAMRead   					0x03     	//Read Command for SRAM
#define  	CMD_SRAMWrite  					0x02     	//Write Command for SRAM
#define 	CMD_SRAMRDSR   					0x05     	//Read the status register
#define  	CMD_SRAMWRSR  					0x01     	//Write the status register
#define		SRAMByteMode					0x00
#define		SRAMPageMode					0x80
#define		SRAMSeqMode						0x40
#define		SRAMPageSize					32
#define		SRAMPageCount					4096
#define		SRAMSize						(SRAMPageSize * SRAMPageCount)
#define		DummyByte						0xFF


extern BOOL checkSRAM();
extern void InitSRAM(void);
extern uint8_t SRAMReadStatusReg(void);
extern uint8_t SRAMWriteStatusReg(uint8_t WriteVal);
extern void SRAMCommand(uint8_t AddLB, uint8_t AddMB,uint8_t AddHB, uint8_t RWCmd);
extern uint8_t SRAMWriteByte(uint8_t AddLB, uint8_t AddMB,uint8_t AddHB, uint8_t WriteData);
extern uint8_t SRAMReadByte(uint8_t AddLB, uint8_t AddMB,uint8_t AddHB);
extern uint32_t SRAMWritePage(uint8_t AddLB, uint8_t AddMB,uint8_t AddHB, uint8_t *WriteData);
extern uint32_t SRAMReadPage(uint8_t AddLB, uint8_t AddMB,uint8_t AddHB, uint8_t *ReadData);
extern uint8_t SRAMWriteSeq(uint8_t AddLB, uint8_t AddMB,uint8_t AddHB, uint8_t *WriteData,uint32_t WriteCnt);
extern uint8_t SRAMReadSeq(uint8_t AddLB, uint8_t AddMB,uint8_t AddHB, uint8_t *ReadData,uint32_t ReadCnt);
extern void SRAM_WriteData16b(uint32_t StartAddress,uint16_t * pData,uint32_t DataSize);
extern void SRAM_ReadData16b(uint32_t StartAddress,uint16_t * pData,uint32_t DataSize);
#endif
