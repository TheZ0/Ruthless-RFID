/*
 * commands.h
 *
 *  Created on: 30 Jun. 2024
 *      Author: rhett
 */

#ifndef INC_COMMANDS_H_
#define INC_COMMANDS_H_

#include <stdint.h>

typedef enum {
	CMD_OK = 0x00,
	CMD_LS_ERROR = 0x01,
	CMD_RM_ERROR = 0x02,
	CMD_PG_ERROR = 0x03,
	CMD_CAT_ERROR = 0x04,
	CMD_MOD_ERROR = 0x05
}CMD_StatusTypeDef;

CMD_StatusTypeDef cmd_ls(void);
CMD_StatusTypeDef cmd_parse(char* cmd);
CMD_StatusTypeDef cmd_pg(char** args, uint8_t size);
CMD_StatusTypeDef cmd_mod(char** args, uint32_t size);
void cmd_build(char** current, char input);
char** cmd_split(char* cmd, char split);
uint32_t get_token_count(char* cmd, char split);
char* cmd_strip(char* cmd);
void free_tokens(char** tokens, uint32_t size);
void pg_show_help();
void mod_show_help();
CMD_StatusTypeDef pg_parse_str(uint8_t** dest, char* data_str);
void str_toupper(char* str);

#define PG_ARGCOUNT_MIN

#endif /* INC_COMMANDS_H_ */
