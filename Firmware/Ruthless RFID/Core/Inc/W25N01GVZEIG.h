/*
 * W25N01GVZEIG.h
 *
 *  Created on: Feb 1, 2024
 *      Author: rhett
 */

#ifndef INC_W25N01GVZEIG_H_
#define INC_W25N01GVZEIG_H_

#include "stm32f4xx.h"
#include <integer.h>
#include <fatfs.h>
#include <integer.h>
#include <diskio.h>
//PINS
#define CS_MEM GPIO_PIN_8 //Port A

//Register Addresses

#define STAT_REG1 0xA0 //Protection settings
#define STAT_REG2 0xB0 //Configuration settings
#define STAT_REG3 0xC0 //Status

//OPCODES

#define ERASE 0xFF
#define ID 0x9F
#define READ_STAT 0x0F
#define WRIT_STAT 0x1F
#define W_EN 0x06
#define W_DIS 0x04
#define BLOCK_ERS 0xD8
#define READ_PAGE 0x13
#define FAST_READ 0x0B
#define READ_BUF 0x03
#define WRIT_LOAD 0x02 //Loads data we wish to write into an internal buffer
#define WRIT_EXE 0x10 //Takes internal buffer data and writes to memory array
#define BBM 0xA1 //Bad Block Management

//Flag values
#define MEM_OK 0x00 //Used for FATFS

//Constants
#define DUMMY 0x00
#define SECTOR_SIZE 2048

//Functions
void READ_ID(uint8_t* rec);
uint8_t STAT_READ(uint8_t addr);
HAL_StatusTypeDef STAT_WRITE(uint8_t addr,uint8_t value);
void WRIT_EN(void);
void WRITE_DIS(void);
void block_erase(uint16_t page_addr);
HAL_StatusTypeDef MEM_INIT(void);
HAL_StatusTypeDef MEM_WRITE(uint16_t page_addr,uint16_t col_addr,uint8_t* data,uint16_t bytes);
HAL_StatusTypeDef MEM_READPAGE(uint16_t addr,uint16_t col_addr,uint8_t* data,uint16_t bytes);
HAL_StatusTypeDef MEM_SCAN(uint16_t* defect);
void findfreeaddr (uint32_t* result);
DSTATUS mem_init (BYTE pdrv);
DSTATUS mem_getstatus (BYTE prdv);
DRESULT mem_read(BYTE pdrv, BYTE* buff, DWORD sector, UINT count);
#endif /* INC_W25N01GVZEIG_H_ */
