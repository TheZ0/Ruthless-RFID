/*
 * W25N01GVZEIG.c
 *
 *  Created on: Feb 1, 2024
 *      Author: rhett
 *
 *  This driver will be used in combination with FATFS to create a FAT filesystem on the SPI memory.
 *  Writing to the W25N01GVZEIG is done 2KiB at a time, so the memory should be formatted to FAT32 with
 *  sector size of 2KiB so that 1 sector in the filesystem is equal to 1 page in memory
 */


#include "W25N01GVZEIG.h"


extern SPI_HandleTypeDef hspi2;

/*
 * Function to read manufacturer and device ID's
 *
 * @param rec: Array to store received values (Note first two elements will be useless)
 *
 * */
void READ_ID(uint8_t* rec){
	uint8_t transaction[]={ID,0x00,0x00,0x00,0x00}; //ID opcode and four dummy bytes
	HAL_GPIO_WritePin(GPIOA, CS_MEM, 0);
	while(HAL_SPI_TransmitReceive(&hspi2, transaction,rec, 5, 100)!=HAL_OK); //Here the third to last byte will be MFR ID and last two will be Device ID
	HAL_GPIO_WritePin(GPIOA, CS_MEM, 1);
}

/*
 * Function to read one of three status registers. Registers in order are (Protection, config and status)
 *
 * @param addr: Address of status register we wish to read from
 * */

uint8_t STAT_READ(uint8_t addr){
	uint8_t transaction[]={READ_STAT,addr,0x00};
	uint8_t rec[3];
	HAL_GPIO_WritePin(GPIOA, CS_MEM, 0);
	while(HAL_SPI_TransmitReceive(&hspi2, transaction,rec, 3, 100)!=HAL_OK);
	HAL_GPIO_WritePin(GPIOA, CS_MEM, 1);
	return(rec[2]); //Status register value
}

/*
 * Function to write to one of three status registers.
 *
 * @param addr: Address of status register
 *
 * @param value: Value to write to register
 * */

HAL_StatusTypeDef STAT_WRITE(uint8_t addr,uint8_t value){
	uint8_t transaction[]={WRIT_STAT,addr,value};
	while(STAT_READ(addr)!=value){ //Loop here until we read the correct value in the register
		HAL_GPIO_WritePin(GPIOA, CS_MEM, 0);
		HAL_SPI_Transmit(&hspi2, transaction, 3, 100);
		HAL_GPIO_WritePin(GPIOA, CS_MEM, 1);
	}
	return(HAL_OK);
}

/*
 * Function to enable write permissions.
 *
 * !!Write enable instruction must preceed the following instructions:!!
 * 		Page program
 * 		Block erase
 * 		Bad block manage
 *
 * The Write Enable bit in status register 3 will be reset to 0 after any of these instructions
 * */

void WRIT_EN(void){ //Write enable instruction must pre-ceed the following: Page program, block erase, bad block manage
	uint8_t WRIT_EN=0x06;

	while(STAT_READ(STAT_REG3&0x02!=0x02)){ //Check that second to last bit (Write Enable bit) of status register is 1
		HAL_GPIO_WritePin(GPIOA, CS_MEM, 0);
		HAL_SPI_Transmit(&hspi2, &WRIT_EN, 1, 100); //load data to internal buffer
		HAL_GPIO_WritePin(GPIOA, CS_MEM, 1);
	}


}

/*
 * Function to initialise memory
 *
 * All we are doing here is removing write protection from entire array by setting BP bits
 * in status register 1 to zero
 *
 * */

HAL_StatusTypeDef MEM_INIT(void){
	HAL_GPIO_WritePin(GPIOA, CS_MEM, 1);
	STAT_WRITE(STAT_REG1, 0x02); //remove protection of entire memory array
}

/*
 * Function to write to 2KiB page in memory
 *
 * @param col_addr: Index of page to begin writing to (See datasheet)
 *
 * @param page_addr: Address of page to write to
 *
 * @param data: data to write
 *
 * @param bytes: Number of bytes to write to page
 * */

HAL_StatusTypeDef MEM_WRITE(uint16_t col_addr,uint16_t page_addr,uint8_t* data,uint16_t bytes){ //Writing is done in pages of size 2KiB + 64 optional bytes
	uint8_t setup[]={WRIT_LOAD,col_addr};
	uint8_t* transaction=malloc(bytes+2);
	uint8_t execute[]={WRIT_EXE,0x00,page_addr}; //0x00 is dummy (need 8 dummy clocks) Page

	memcpy(transaction,setup,2);
	memcpy(transaction,data,bytes);

	WRIT_EN();
	HAL_GPIO_WritePin(GPIOA, CS_MEM, 0);
	if(HAL_SPI_Transmit(&hspi2, transaction, bytes+2, 100)!=HAL_OK){ //load data to internal buffer
		HAL_GPIO_WritePin(GPIOA, CS_MEM, 1);
		return(HAL_ERROR);
	}

	while(STAT_READ(STAT_REG3)&0x02!=0x02); //Wait here until busy bit is clear
	WRIT_EN();
	HAL_GPIO_WritePin(GPIOA, CS_MEM, 0);
	if(HAL_SPI_Transmit(&hspi2, execute, 3, 100)!=HAL_OK){ //Send command to dump internal buffer data to memory array
		HAL_GPIO_WritePin(GPIOA, CS_MEM, 1);
		return(HAL_ERROR);
	}
	HAL_GPIO_WritePin(GPIOA, CS_MEM, 1);
	HAL_Delay(1);
	return(HAL_OK);

}

/*
 * Function to read a single 2KiB page from memory
 *
 * @param page_addr: Page address
 *
 * @param col_addr: Index of page to begin reading (0x00 to start from beginning of page)
 *
 * @param data: Data from page
 *
 * @param bytes: Number of bytes to read from data buffer
 *
 * !!Note data must first be shifted from memory array into internal data buffer and then can be read!!
 * */

HAL_StatusTypeDef MEM_READPAGE(uint16_t page_addr,uint16_t col_addr,uint8_t* data,uint16_t bytes){ //Read one 2KiB page. Data will be put into internal buffer which can then be read. Wait at least tDR or until busy bit is clear
	uint8_t transaction[]={READ_PAGE,0x00,(uint8_t)page_addr>>8,(uint8_t)page_addr};
	uint8_t read_data[bytes+5];
	uint8_t rec_data[bytes+5];

	memset(read_data,0,bytes+5);

	read_data[0]=READ_BUF;
	read_data[1]=col_addr;

	HAL_GPIO_WritePin(GPIOA, CS_MEM, 0);
	if(HAL_SPI_Transmit(&hspi2, transaction, 4, 100)!=HAL_OK){ //load data to internal buffer
		HAL_GPIO_WritePin(GPIOA, CS_MEM, 1);
		return(HAL_ERROR);
	}
	HAL_GPIO_WritePin(GPIOA, CS_MEM, 1);
	while(STAT_READ(STAT_REG3)&0x01!=0x01); //Wait here until BUSY bit is cleared
	HAL_GPIO_WritePin(GPIOA, CS_MEM, 0);
	if(HAL_SPI_TransmitReceive(&hspi2, read_data, rec_data, bytes+5, 100)!=HAL_OK){ //Ignore remaining 64 bytes so 2048 instead of 2112
		HAL_GPIO_WritePin(GPIOA, CS_MEM, 1);
		return(HAL_ERROR);
	}
	HAL_GPIO_WritePin(GPIOA, CS_MEM, 1);

	for(int i=0;i<bytes;i++){
		data[i]=rec_data[i+4]; //+4 as first four elements of rec_data are meaningless
	}
	return(HAL_OK);

}

/*
 * Function to find defective memory blocks (128kB) (There may be at most 20 defective blocks)
 *
 * !!Defective blocks are labelled by the manufacturer by the first page in the block beginning with
 * 0x00 instead of 0xFF!!
 *
 * Defective blocks can be placed in the BBM look up table in order to avoid them whilst writing
 *
 * @param defect: Array to store addresses of defective pages/blocks
 * */

HAL_StatusTypeDef MEM_SCAN(uint16_t* defect){
	int i=0; //Keep track of index in defect array. +1 when defective address is found


	for(int addr=0;addr<0x10000;addr++){ //We have 65536 pages to check (0x0000 to 0xFFFF) but we only need to check the first byte
		uint8_t BYTE1;
		if(MEM_READPAGE(addr, 0x00,&BYTE1,1)!=HAL_OK){ //Set col addr to 0x00 as we are only interested in first byte
			return(HAL_ERROR);
		}
		else{
			if(BYTE1==0x00){
				defect[i]=addr;
				i++;
			}
			else{

			}
		}


	}
	return(HAL_OK);
}

