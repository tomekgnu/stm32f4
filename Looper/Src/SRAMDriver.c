#include "SRAMDriver.h"
#include "spi.h"

uint8_t SRAMBuf[SRAMPageSize];

static inline GPIO_TypeDef* getSRAMEnablePort(uint8_t n){

	switch(n){
		case SRAM_0: return SRAM_0_CS_GPIO_Port;
		case SRAM_1: return SRAM_1_CS_GPIO_Port;
		case SRAM_2: return SRAM_2_CS_GPIO_Port;
		case SRAM_3: return SRAM_3_CS_GPIO_Port;
		case SRAM_4: return SRAM_4_CS_GPIO_Port;
		case SRAM_5: return SRAM_5_CS_GPIO_Port;
		case SRAM_6: return SRAM_6_CS_GPIO_Port;
		case SRAM_7: return SRAM_7_CS_GPIO_Port;
		default: return SRAM_0_CS_GPIO_Port;
	}
}

static inline uint16_t getSRAMEnablePin(uint8_t n){

	switch(n){
		case SRAM_0: return SRAM_0_CS_Pin;
		case SRAM_1: return SRAM_1_CS_Pin;
		case SRAM_2: return SRAM_2_CS_Pin;
		case SRAM_3: return SRAM_3_CS_Pin;
		case SRAM_4: return SRAM_4_CS_Pin;
		case SRAM_5: return SRAM_5_CS_Pin;
		case SRAM_6: return SRAM_6_CS_Pin;
		case SRAM_7: return SRAM_7_CS_Pin;
		default: return SRAM_0_CS_Pin;
	}
}

static uint8_t ReadSPI(){
	uint8_t byte;
	HAL_SPI_Receive(&hspi5,&byte,1,100);
	return byte;
}

void WriteSPI(uint8_t byte){

	HAL_SPI_Transmit(&hspi5,&byte,1,100);
}

void SRAMEnable(uint8_t n){
	HAL_GPIO_WritePin(getSRAMEnablePort(n),getSRAMEnablePin(n),GPIO_PIN_RESET);
}

void SRAMDisable(uint8_t n){
	HAL_GPIO_WritePin(getSRAMEnablePort(n),getSRAMEnablePin(n),GPIO_PIN_SET);
}

void InitSRAM(void)
{
	SRAMDisable(SRAM_0);
	SRAMDisable(SRAM_1);
	SRAMDisable(SRAM_2);
	SRAMDisable(SRAM_3);
	SRAMDisable(SRAM_4);
	SRAMDisable(SRAM_5);
	SRAMDisable(SRAM_6);
	SRAMDisable(SRAM_7);
}

uint8_t SRAMWriteStatusReg(uint8_t WriteVal)
{
	SRAMEnable(SRAM_1);
	WriteSPI(CMD_SRAMWRSR);
	//while(!SPI_Rx_Buf_Full);
	//ReadData = ReadSPI();
	WriteSPI(WriteVal);
	//while(!SPI_Rx_Buf_Full);
	//ReadData = ReadSPI();
	SRAMDisable(SRAM_1);
	return 0;			//Return non -ve nuber indicating success
}

uint8_t SRAMReadStatusReg(void)
{
	uint8_t ReadData;
	SRAMEnable(SRAM_1);
	WriteSPI(CMD_SRAMRDSR);
	//while(!SPI_Rx_Buf_Full);
	//ReadData = ReadSPI();
	WriteSPI(DummyByte);
	//while(!SPI_Rx_Buf_Full);
	ReadData = ReadSPI();
	SRAMDisable(SRAM_1);
	return ReadData;
}
void SRAMCommand(uint8_t AddLB, uint8_t AddMB,uint8_t AddHB, uint8_t RWCmd)
{
	//Send Read or Write command to SRAM
	WriteSPI(RWCmd);
	//while(!SPI_Rx_Buf_Full);
	//ReadData = ReadSPI();
	//Send High byte of address to SRAM
	WriteSPI(AddHB);
	WriteSPI(AddMB);
	//while(!SPI_Rx_Buf_Full);
	//ReadData = ReadSPI();
	//Send Low byte of address to SRAM
	WriteSPI(AddLB);
	//while(!SPI_Rx_Buf_Full);
	//ReadData = ReadSPI();
}

uint8_t SRAMWriteByte(uint8_t AddLB, uint8_t AddMB,uint8_t AddHB, uint8_t WriteData)
{
	SRAMWriteStatusReg(SRAMByteMode);
	SRAMEnable(SRAM_1);
	//Send Write command to SRAM along with address
	SRAMCommand(AddLB,AddMB,AddHB,CMD_SRAMWrite);
	//Send Data to be written to SRAM
	WriteSPI(WriteData);
	//while(!SPI_Rx_Buf_Full);
	//WriteData = ReadSPI();
	SRAMDisable(SRAM_1);
	return 0;			//Return non -ve number indicating success
}

uint8_t SRAMReadByte(uint8_t AddLB, uint8_t AddMB,uint8_t AddHB)
{
	uint8_t ReadData;
	SRAMWriteStatusReg(SRAMByteMode);
	SRAMEnable(SRAM_1);
	//Send Read command to SRAM along with address
	SRAMCommand(AddLB,AddMB,AddHB,CMD_SRAMRead);
	//Send dummy data so SRAM can put desired Data read from SRAM
	//WriteSPI(DummyByte);
	//while(!SPI_Rx_Buf_Full);
	ReadData = ReadSPI();
	SRAMDisable(SRAM_1);
	return ReadData;
}

uint32_t SRAMWritePage(uint8_t AddLB, uint8_t AddMB,uint8_t AddHB, uint8_t RWCmd, uint8_t *WriteData)
{
	uint32_t WriteCnt;
	SRAMWriteStatusReg(SRAMPageMode);
	//Send Write command to SRAM along with address
	SRAMEnable(SRAM_1);
	SRAMCommand(AddLB,AddMB,AddHB,CMD_SRAMWrite);
	//Send Data to be written to SRAM
	for(WriteCnt = 0;WriteCnt < SRAMPageSize;WriteCnt++)
	{
		WriteSPI(*WriteData++);
		//while(!SPI_Rx_Buf_Full);
		//ReadData = ReadSPI();
	}
	SRAMDisable(SRAM_1);
	return WriteCnt;			//Return no# of bytes written to SRAM
}

uint32_t SRAMReadPage(uint8_t AddLB, uint8_t AddMB,uint8_t AddHB, uint8_t RWCmd,uint8_t *ReadData)
{
	uint32_t ReadCnt;
	SRAMWriteStatusReg(SRAMPageMode);
	//Send Read command to SRAM along with address
	SRAMEnable(SRAM_1);
	SRAMCommand(AddLB,AddMB,AddHB,CMD_SRAMRead);
	//Send dummy data so SRAM can put desired Data read from SRAM
	for(ReadCnt = 0; ReadCnt < SRAMPageSize; ReadCnt++)
	{
		//while(!SPI_Rx_Buf_Full);
		*ReadData++ = ReadSPI();
	}
	SRAMDisable(SRAM_1);
	return ReadCnt;			//Return no# of bytes read from SRAM
}

uint8_t SRAMWriteSeq(uint8_t AddLB, uint8_t AddMB,uint8_t AddHB, uint8_t RWCmd, uint8_t *WriteData,uint32_t WriteCnt)
{
	SRAMWriteStatusReg(SRAMSeqMode);
	//Send Write command to SRAM along with address
	SRAMEnable(SRAM_1);
	SRAMCommand(AddLB,AddMB,AddHB,CMD_SRAMWrite);
	//Send Data to be written to SRAM
	for(;WriteCnt > 0;WriteCnt--)
	{
		WriteSPI(*WriteData++);
		//while(!SPI_Rx_Buf_Full);
		//DummyRead =ReadSPI();
	}
	SRAMDisable(SRAM_1);
	return 0;			//Return non -ve nuber indicating success
}

uint8_t SRAMReadSeq(uint8_t AddLB, uint8_t AddMB,uint8_t AddHB, uint8_t RWCmd,uint8_t *ReadData,uint32_t ReadCnt)
{
	SRAMWriteStatusReg(SRAMSeqMode);
	//Send Read command to SRAM along with address
	SRAMEnable(SRAM_1);
	SRAMCommand(AddLB,AddMB,AddHB,CMD_SRAMRead);
	//Send dummy data so SRAM can put desired Data read from SRAM
	for(; ReadCnt > 0; ReadCnt--)
	{
		//while(!SPI_Rx_Buf_Full);
		*ReadData++ = ReadSPI();
	}
	SRAMDisable(SRAM_1);
	return 0;			//Return non -ve nuber indicating success
}

