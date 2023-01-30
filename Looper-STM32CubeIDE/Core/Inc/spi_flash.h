/**
  ******************************************************************************
  * @file    spi_flash.h
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    18-January-2013
  * @brief   This file contains all the functions prototypes for the spi_flash
  *          firmware driver.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPI_FLASH_H
#define __SPI_FLASH_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "main.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Size-related constants ----------------------------------------------------*/
#define N25Q256A_FLASH_SIZE                  0x2000000 /* 256 MBits => 32MBytes */
#define N25Q256A_SECTOR_SIZE                 0x10000   /* 512 sectors of 64KBytes */
#define N25Q256A_SUBSECTOR_SIZE              0x1000    /* 8192 subsectors of 4kBytes */
#define N25Q256A_PAGE_SIZE                   0x100     /* 131072 pages of 256 bytes */

#define N25Q256A_DUMMY_CYCLES_READ           8
#define N25Q256A_DUMMY_CYCLES_READ_QUAD      10
#define N25Q256A_DUMMY_CYCLES_READ_DTR       6
#define N25Q256A_DUMMY_CYCLES_READ_QUAD_DTR  8

#define N25Q256A_BULK_ERASE_MAX_TIME         480000
#define N25Q256A_SECTOR_ERASE_MAX_TIME       3000
#define N25Q256A_SUBSECTOR_ERASE_MAX_TIME    800

/* M25P SPI Flash supported commands */  
#define sFLASH_CMD_WRITE          0x02  /* Write to Memory instruction */
#define sFLASH_CMD_WRSR           0x01  /* Write Status Register instruction */
#define sFLASH_CMD_WREN           0x06  /* Write enable instruction */
#define sFLASH_CMD_READ           0x03  /* Read from Memory instruction */
#define sFLASH_CMD_RDSR           0x05  /* Read Status Register instruction  */
#define sFLASH_CMD_RDID           0x9E  /* Read identification */
#define sFLASH_CMD_SE             0xD8  /* Sector Erase instruction */
#define sFLASH_CMD_SUBSE          0x20  /* Subsector Erase instruction */
#define sFLASH_CMD_BE             0xC7  /* Bulk Erase instruction */

#define sFLASH_WIP_FLAG           0x01  /* Write In Progress (WIP) flag */

#define sFLASH_DUMMY_BYTE         0xFF
#define sFLASH_SPI_PAGESIZE       0x100

#define sFLASH_N25Q256A_ID        0x20BA19


/* Exported macro ------------------------------------------------------------*/
/* Select sFLASH: Chip Select pin low */
#define sFLASH_CS_LOW()       HAL_GPIO_WritePin(SF3_CS_GPIO_Port,SF3_CS_Pin,GPIO_PIN_RESET);
/* Deselect sFLASH: Chip Select pin high */
#define sFLASH_CS_HIGH()      HAL_GPIO_WritePin(SF3_CS_GPIO_Port,SF3_CS_Pin,GPIO_PIN_SET);

/* Exported functions ------------------------------------------------------- */

/* High layer functions  */
void sFLASH_DeInit(void);
void sFLASH_Init(void);
uint32_t sFLASH_Erase(uint32_t EraseAddr, uint32_t Size);
void sFLASH_EraseSector(uint32_t SectorAddr);
void sFLASH_EraseSubSector(uint32_t SubSectorAddr);
void sFLASH_EraseBulk(void);
void sFLASH_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void sFLASH_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void sFLASH_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
uint32_t sFLASH_ReadID(void);
void sFLASH_StartReadSequence(uint32_t ReadAddr);

/* Low layer functions */
uint8_t sFLASH_ReadByte(void);
uint8_t sFLASH_SendByte(uint8_t byte);
uint16_t sFLASH_SendHalfWord(uint16_t HalfWord);
void sFLASH_WriteEnable(void);
void sFLASH_WaitForWriteEnd(void);

#ifdef __cplusplus
}
#endif

#endif /* __SPI_FLASH_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
