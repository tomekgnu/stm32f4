#include "SRAMDriver.h"
#include "spi.h"
#include "string.h"

static uint8_t SRAMBuf[SRAMPageSize];
static uint8_t currentSRAM = SRAM_0;
static union {
	uint32_t num;
	uint8_t arr;
} CurrentAddress;

void SRAM_WriteData16b(uint32_t StartAddress,uint16_t * pData,uint32_t DataSize){
	uint32_t currentByte;	// determine byte in SRAM chip
	uint32_t TotalBytes = DataSize * 2;
	uint32_t i = 0;	// sram chip counter
	uint32_t written = 0;
	if((TotalBytes / SRAMSize) > 7)
		return;

	while(TotalBytes > 0){
		currentSRAM = StartAddress / SRAMSize; 			// determine which SRAM chip
		CurrentAddress.num = StartAddress % SRAMSize;		// determine address in SRAM chip
		if((written = (TotalBytes / SRAMSize)) > 0)
			SRAMWriteSeq(CurrentAddress.arr[0],CurrentAddress.arr[1],CurrentAddress.arr[2],pData,written);

		StartAddress += written;
		pData += written;
		TotalBytes -= StartAddress;
	}

}

void SRAM_ReadData16b(uint32_t StartAddress,uint16_t * pData,uint32_t DataSize){
	currentSRAM = StartAddress / SRAMSize; 			// determine which SRAM chip
	uint32_t currentByte = StartAddress / SRAMSize;	// determine byte in SRAM chip

}

BOOL checkSRAM(){
	uint8_t ok = 0;
	uint8_t data = 0;
	for(currentSRAM = SRAM_0; currentSRAM <= SRAM_7; currentSRAM++){
		SRAMWriteByte(0,0,0,'c');
		data = SRAMReadByte(0,0,0);
		if(data == 'c')
			ok++;
	}

	if(ok != 8)
		return FALSE;
	ok = 0;
	for(currentSRAM = SRAM_0; currentSRAM <= SRAM_7; currentSRAM++){
			memset(SRAMBuf,0,SRAMPageSize);
			SRAMWritePage(0,0,0,(uint8_t *)"aaaabbbbccccddddeeeeffffgggghhhh");
			SRAMReadPage(0,0,0,SRAMBuf);
			if(strncmp((char *)"aaaabbbbccccddddeeeeffffgggghhhh",(char *)SRAMBuf,SRAMPageSize) == 0)
				ok++;
	}
	return ok == 8;
}

static uint16_t sramPins[8] = {
		SRAM_0_CS_Pin,
		SRAM_1_CS_Pin,
		SRAM_2_CS_Pin,
		SRAM_3_CS_Pin,
		SRAM_4_CS_Pin,
		SRAM_5_CS_Pin,
		SRAM_6_CS_Pin,
		SRAM_7_CS_Pin
};
static GPIO_TypeDef* sramPorts[8] = {
		SRAM_0_CS_GPIO_Port,
		SRAM_1_CS_GPIO_Port,
		SRAM_2_CS_GPIO_Port,
		SRAM_3_CS_GPIO_Port,
		SRAM_4_CS_GPIO_Port,
		SRAM_5_CS_GPIO_Port,
		SRAM_6_CS_GPIO_Port,
		SRAM_7_CS_GPIO_Port
};

static uint8_t ReadSPI(){
	uint8_t byte;
	HAL_SPI_Receive(&hspi5,&byte,1,100);
	return byte;
}

void WriteSPI(uint8_t byte){

	HAL_SPI_Transmit(&hspi5,&byte,1,100);
}

static void SRAMEnable(uint8_t n){
	HAL_GPIO_WritePin(sramPorts[n],sramPins[n],GPIO_PIN_RESET);
}

static void SRAMDisable(uint8_t n){
	HAL_GPIO_WritePin(sramPorts[n],sramPins[n],GPIO_PIN_SET);
}

void InitSRAM(void)
{
	for(currentSRAM = SRAM_0; currentSRAM <= SRAM_7; currentSRAM++)
		SRAMDisable(currentSRAM);
}

uint8_t SRAMWriteStatusReg(uint8_t WriteVal)
{
	SRAMEnable(currentSRAM);
	WriteSPI(CMD_SRAMWRSR);
	//while(!SPI_Rx_Buf_Full);
	//ReadData = ReadSPI();
	WriteSPI(WriteVal);
	//while(!SPI_Rx_Buf_Full);
	//ReadData = ReadSPI();
	SRAMDisable(currentSRAM);
	return 0;			//Return non -ve nuber indicating success
}

uint8_t SRAMReadStatusReg(void)
{
	uint8_t ReadData;
	SRAMEnable(currentSRAM);
	WriteSPI(CMD_SRAMRDSR);
	//while(!SPI_Rx_Buf_Full);
	//ReadData = ReadSPI();
	WriteSPI(DummyByte);
	//while(!SPI_Rx_Buf_Full);
	ReadData = ReadSPI();
	SRAMDisable(currentSRAM);
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
	SRAMEnable(currentSRAM);
	//Send Write command to SRAM along with address
	SRAMCommand(AddLB,AddMB,AddHB,CMD_SRAMWrite);
	//Send Data to be written to SRAM
	WriteSPI(WriteData);
	//while(!SPI_Rx_Buf_Full);
	//WriteData = ReadSPI();
	SRAMDisable(currentSRAM);
	return 0;			//Return non -ve number indicating success
}

uint8_t SRAMReadByte(uint8_t AddLB, uint8_t AddMB,uint8_t AddHB)
{
	uint8_t ReadData;
	SRAMWriteStatusReg(SRAMByteMode);
	SRAMEnable(currentSRAM);
	//Send Read command to SRAM along with address
	SRAMCommand(AddLB,AddMB,AddHB,CMD_SRAMRead);
	//Send dummy data so SRAM can put desired Data read from SRAM
	//WriteSPI(DummyByte);
	//while(!SPI_Rx_Buf_Full);
	ReadData = ReadSPI();
	SRAMDisable(currentSRAM);
	return ReadData;
}

uint32_t SRAMWritePage(uint8_t AddLB, uint8_t AddMB,uint8_t AddHB, uint8_t *WriteData)
{
	uint32_t WriteCnt;
	SRAMWriteStatusReg(SRAMPageMode);
	//Send Write command to SRAM along with address
	SRAMEnable(currentSRAM);
	SRAMCommand(AddLB,AddMB,AddHB,CMD_SRAMWrite);
	//Send Data to be written to SRAM
	for(WriteCnt = 0;WriteCnt < SRAMPageSize;WriteCnt++)
	{
		WriteSPI(*WriteData++);
		//while(!SPI_Rx_Buf_Full);
		//ReadData = ReadSPI();
	}
	SRAMDisable(currentSRAM);
	return WriteCnt;			//Return no# of bytes written to SRAM
}

uint32_t SRAMReadPage(uint8_t AddLB, uint8_t AddMB,uint8_t AddHB, uint8_t *ReadData)
{
	uint32_t ReadCnt;
	SRAMWriteStatusReg(SRAMPageMode);
	//Send Read command to SRAM along with address
	SRAMEnable(currentSRAM);
	SRAMCommand(AddLB,AddMB,AddHB,CMD_SRAMRead);
	//Send dummy data so SRAM can put desired Data read from SRAM
	for(ReadCnt = 0; ReadCnt < SRAMPageSize; ReadCnt++)
	{
		//while(!SPI_Rx_Buf_Full);
		*ReadData++ = ReadSPI();
	}
	SRAMDisable(currentSRAM);
	return ReadCnt;			//Return no# of bytes read from SRAM
}

uint8_t SRAMWriteSeq(uint8_t AddLB, uint8_t AddMB,uint8_t AddHB, uint8_t *WriteData,uint32_t WriteCnt)
{
	SRAMWriteStatusReg(SRAMSeqMode);
	//Send Write command to SRAM along with address
	SRAMEnable(currentSRAM);
	SRAMCommand(AddLB,AddMB,AddHB,CMD_SRAMWrite);
	//Send Data to be written to SRAM
	for(;WriteCnt > 0;WriteCnt--)
	{
		WriteSPI(*WriteData++);
		//while(!SPI_Rx_Buf_Full);
		//DummyRead =ReadSPI();
	}
	SRAMDisable(currentSRAM);
	return 0;			//Return non -ve nuber indicating success
}

uint8_t SRAMReadSeq(uint8_t AddLB, uint8_t AddMB,uint8_t AddHB, uint8_t *ReadData,uint32_t ReadCnt)
{
	SRAMWriteStatusReg(SRAMSeqMode);
	//Send Read command to SRAM along with address
	SRAMEnable(currentSRAM);
	SRAMCommand(AddLB,AddMB,AddHB,CMD_SRAMRead);
	//Send dummy data so SRAM can put desired Data read from SRAM
	for(; ReadCnt > 0; ReadCnt--)
	{
		//while(!SPI_Rx_Buf_Full);
		*ReadData++ = ReadSPI();
	}
	SRAMDisable(currentSRAM);
	return 0;			//Return non -ve nuber indicating success
}

