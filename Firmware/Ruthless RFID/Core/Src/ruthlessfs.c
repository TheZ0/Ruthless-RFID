/*
 * ruthlessfs.c
 *
 *  Created on: 17 Jun. 2024
 *      Author: rhett
 */

/**
 * ******************************Ruthless File System***********************************
 *
 * File system used by Ruthless RFID to store data of read cards. Each entry occupies a 128kB block (smallest erasable block size).
 *
 * Page 1 - Metadata (Stored as: Type, card memory size, read protected, uid size)
 * Page 2 - Card name + uid
 * Page 3 - Contents
 * */
#include "stm32f4xx.h"
#include "W25N01GVZEIG.h"
#include "ruthlessfs.h"
#include <string.h>
#include <stdlib.h>


/**
 * Enter a card into the file system
 *
 * @param card - Card to store
 * @return RFS_OK if card was successfully stored
 * */
RFS_StatusTypeDef enter_card(Card* card) {
	uint16_t free_block = (uint16_t)mem_find_free_block();
	block_erase(free_block); //Erase entire block ready for new data
	enter_metadata(card, free_block);

	if (MEM_WRITE(free_block + NAMEPAGE_OFFSET, 0x0000, (uint8_t*)card->name, strlen(card->name)) != HAL_OK) {
		return RFS_WRITE_ERROR;
	}
	if (MEM_WRITE(free_block + NAMEPAGE_OFFSET, 0x0000 + strlen(card->name),card->uid ,card->uidsize) != HAL_OK) {
		return RFS_WRITE_ERROR;
	}
	if (MEM_WRITE(free_block + DATAPAGE_OFFSET, 0x0000, card->contents, card->contents_size) != HAL_OK) {
		return RFS_WRITE_ERROR;
	}

	return RFS_OK;

}

/**
 * Write the card metadata into a block
 *
 * @param card - Card to write
 * @param block_addr - Block start address
 * */
RFS_StatusTypeDef enter_metadata(Card* card, uint16_t block_addr) {
	uint8_t card_size = card->contents_size; //Card contents is uint8_t
	uint8_t read_protected = card->read_protected;
	uint8_t uid_size = card->uidsize;
	uint8_t metasize = sizeof(card_size) + sizeof(uid_size) + sizeof(read_protected) + strlen(card->type);
	uint8_t* metadata = malloc(metasize);

	memcpy(metadata, (uint8_t*) card->type, strlen(card->type));
	metadata[strlen(card->type) + 0] = card_size;
	metadata[strlen(card->type) + 1] = uid_size;
	metadata[strlen(card->type) + 2] = read_protected;

	if (MEM_WRITE(block_addr, 0x0000, metadata, metasize) != HAL_OK) {
		return RFS_WRITE_ERROR;
	}
	free(metadata);
	return RFS_OK;
}

/**
 * Read a card from memory
 *
 * @param Name - Card entry name
 * @return a new Card instance with read data. NULL if name is not in memory
 * */
Card* read_card_name (char* name) {
	uint16_t block_addr = 0;
	char* read_name = malloc(strlen(name) + 1);

	while ((strcmp(name, read_name) != 0) && (block_addr < BLOCK_COUNT)) {
		MEM_READPAGE(block_addr * BLOCK_PAGECOUNT, 0x0000, (uint8_t*)read_name, strlen(name));
		read_name[strlen(name)] = '\0';
		block_addr++;
	}
	if (block_addr == BLOCK_COUNT) {
		return NULL; //We couldn't find the name
	}
	return get_card_entry(block_addr);

}

/**
 * Get card from entry number
 *
 * @param entry - Entry number of card
 * @return a new Card instance with read data
 * */
Card* read_card_entry(uint16_t entry) {
	Card* result = malloc(sizeof(Card));
	uint16_t metadata_size = get_metasize(entry);
	char* type = malloc(metadata_size - 1); //-1 to account for data after type

	MEM_READPAGE(entry * BLOCK_PAGECOUNT, 0x0000, type, metadata_size - 1); //read card type
	type[metadata_size - 1] = '\0';
}

/**
 * Get the size of the metadata for a specific entry
 * @param entry - Entry to get size of
 * @return size of metadata in bytes
 * */
uint16_t get_metasize(uint16_t entry) {
	uint16_t size = 0;
	uint8_t byte_read = 0x00;

	while(byte_read != 0xFF) {
		if (MEM_READPAGE(entry * BLOCK_PAGECOUNT, size, &byte_read, 1) != HAL_OK) {
			return 0; //Error occured whilst reading
		}
		size++;
	}

	return size;
}
