#include "SRAMDriver.h"
#include "spi.h"
#include "string.h"
#include "ff.h"

static uint8_t SRAMBuf[SRAMPageSize];
static uint8_t currentSRAM = SRAM_0;
static sramAddress currentRW = {0};
static uint32_t sram_read = 0;
static uint32_t sram_written = 0;
static void incrementBytes(int size);
static void SRAM_addressReset();



static void SRAM_addressReset(){
	currentRW.currentByte.value = 0;
	currentRW.currentPage.value = 0;
	currentRW.totalBytes.value = 0;
	currentRW.currentPage.value = 0;
	currentRW.currentByte.value = 0;
	currentSRAM = 0;
}


void SRAM_seek(unsigned int size,unsigned int whence){
	switch(whence){
		case SRAM_SET: SRAM_addressReset();
					   incrementBytes(size);
					   break;
		case SRAM_CUR: incrementBytes(size);
					   break;
		case SRAM_END: SRAM_addressReset();
					   incrementBytes(SRAMTotalSize - size);
					   break;
		default:	   break;

	}

}
uint32_t SRAM_written(){

}

uint32_t SRAM_read(){

}

static void incrementBytes(int size){
	currentRW.totalBytes.value += size;
	if(currentRW.totalBytes.value > SRAMTotalSize)
		currentRW.totalBytes.value %= SRAMTotalSize;
	currentRW.currentByte.value = currentRW.totalBytes.value % SRAMChipSize;
	currentRW.currentPage.value = (currentRW.totalBytes.value / SRAMPageSize) % SRAMPageCount;
	currentRW.currentSram = currentRW.totalBytes.value / SRAMChipSize;
	currentSRAM = currentRW.currentSram;
}

void writeSRAM(unsigned char *buf,unsigned int size){
	unsigned int unaligned = currentRW.currentByte.value % SRAMPageSize; // byte between start and end of page
	unsigned int remainder = (unaligned > 0?(SRAMPageSize - unaligned):0); // bytes remaining to end of page
	if(size == 0)
		return;

	if(size >= SRAMPageSize){
		if(unaligned == 0){
			// write page only, pass remaining size to next call
			SRAMWriteSeq(currentRW.currentByte.bytes[0],currentRW.currentByte.bytes[1],currentRW.currentByte.bytes[2],buf,SRAMPageSize);
			incrementBytes(SRAMPageSize);
			writeSRAM(buf + SRAMPageSize,size - SRAMPageSize);
		}
		else{
			// write remainder, pass remaining size to next call
			SRAMWriteSeq(currentRW.currentByte.bytes[0],currentRW.currentByte.bytes[1],currentRW.currentByte.bytes[2],buf,remainder);
			incrementBytes(remainder);
			writeSRAM(buf + remainder,size - remainder);
		}


	}
	else{

		if(unaligned > 0){
			if(size > remainder){
				size -= remainder;
				SRAMWriteSeq(currentRW.currentByte.bytes[0],currentRW.currentByte.bytes[1],currentRW.currentByte.bytes[2],buf,remainder);
				incrementBytes(remainder);
				writeSRAM(buf + remainder,size);
			}
			else{
				SRAMWriteSeq(currentRW.currentByte.bytes[0],currentRW.currentByte.bytes[1],currentRW.currentByte.bytes[2],buf,size);
				incrementBytes(size);
			}

		}
		else{
			SRAMWriteSeq(currentRW.currentByte.bytes[0],currentRW.currentByte.bytes[1],currentRW.currentByte.bytes[2],buf,size);
			incrementBytes(size);
		}
	}
}


void readSRAM(unsigned char *buf,unsigned int size,UINT *bytes_read){
	unsigned int unaligned = currentRW.currentByte.value % SRAMPageSize; // byte between start and end of page
	unsigned int remainder = (unaligned > 0?(SRAMPageSize - unaligned):0); // bytes remaining to end of page
	UINT rbytes;	// local bytes read

	if(size == 0)
		return;

	if(size >= SRAMPageSize){
		if(unaligned == 0){
			// read page only, pass remaining size to next call
			SRAMReadSeq(currentRW.currentByte.bytes[0],currentRW.currentByte.bytes[1],currentRW.currentByte.bytes[2],buf,SRAMPageSize);
			incrementBytes(SRAMPageSize);
			readSRAM(buf + SRAMPageSize,size - SRAMPageSize,&rbytes);
		}
		else{
			// read remainder, pass remaining size to next call
			SRAMReadSeq(currentRW.currentByte.bytes[0],currentRW.currentByte.bytes[1],currentRW.currentByte.bytes[2],buf,remainder);
			incrementBytes(remainder);
			readSRAM(buf + remainder,size - remainder,&rbytes);
		}


	}
	else{

		if(unaligned > 0){
			if(size > remainder){
				size -= remainder;
				SRAMReadSeq(currentRW.currentByte.bytes[0],currentRW.currentByte.bytes[1],currentRW.currentByte.bytes[2],buf,remainder);
				incrementBytes(remainder);
				readSRAM(buf + remainder,size,&rbytes);
			}
			else{
				SRAMReadSeq(currentRW.currentByte.bytes[0],currentRW.currentByte.bytes[1],currentRW.currentByte.bytes[2],buf,size);
				incrementBytes(size);
			}

		}
		else{
			SRAMReadSeq(currentRW.currentByte.bytes[0],currentRW.currentByte.bytes[1],currentRW.currentByte.bytes[2],buf,size);
			incrementBytes(size);
		}
	}

	*bytes_read += rbytes;
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

