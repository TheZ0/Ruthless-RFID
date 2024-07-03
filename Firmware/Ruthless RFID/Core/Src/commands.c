/*
 * commands.c
 *
 *  Created on: 30 Jun. 2024
 *      Author: rhett
 */

#include "commands.h"
#include "terminal.h"
#include "ruthlessfs.h"
#include "W25N01GVZEIG.h"
#include "stm32f4xx.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * List all files currently stored on device
 * @return CMD_OK if command was successfully executed
 * */
CMD_StatusTypeDef cmd_ls () {
	uint32_t file_count = get_number_files_all();
	char** file_names = malloc(file_count * sizeof(char*));
	if (get_all_files(file_names) != RFS_OK) {
		return CMD_LS_ERROR;
	}

	for (int i = 0; i < file_count; i++) {
		printf("\n\r%s.rfid", file_names[i]);
	}
	free_filenames(file_names, file_count);
	return CMD_OK;
}

/**
 * Remove a file
 * @param arg - File name to remove
 * @return CMD_OK if file was successfully removed
 * */
CMD_StatusTypeDef cmd_rm(char* arg) {
	char** file_name_split = cmd_split(arg, '.'); //Split into name and extension

	if ((strcmp(file_name_split[1], "rfid") != 0)) {
		printf("\n\rfile not found: %s", arg);
		free_tokens(file_name_split, 2);
		return CMD_RM_ERROR;
	}
	if (remove_card_byname(file_name_split[0]) != RFS_OK) {
		printf("\n\rfile not found: %s", arg);
		free_tokens(file_name_split, 2);
		return CMD_RM_ERROR;
	}
	free_tokens(file_name_split, 2);
	return CMD_OK;
}

/**
 * Upload/program a file into the device
 * @param tokens - Arguments from user
 * @return CMD_OK if file was successfully uploaded
 * */
CMD_StatusTypeDef cmd_pg(char** args, uint8_t size) {
	Card to_program;
	char* name;

	if (strcmp(args[1], "--help") == 0) {
		pg_show_help();
		return CMD_OK;
	}

	for (int i = 1; i < size; i++) {

		if (strcmp(args[i], "-name") == 0) {
			name = args[i + 1];
		}

		if (strcmp(args[i], "-uid") == 0) {
			pg_parse_str(&to_program.uid, args[i + 1]);
			to_program.uidsize = get_token_count(args[i + 1], ',');
		}

		if (strcmp(args[i], "-mem") == 0) {
			pg_parse_str(&to_program.contents, args[i + 1]);
			to_program.contents_size = get_token_count(args[i + 1], ',');
		}

		if (strcmp(args[i], "-type") == 0) {
			to_program.type = args[i + 1];
		}

	}
	to_program.read_protected = 0;
	enter_card(&to_program, mem_find_free_block(), name);
	return CMD_OK;

}

/**
 * Parse a string representation of command
 * @param cmd - String representation of command e.g. ls
 * @return CMD_OK if command was successfully parsed and executed
 * */
CMD_StatusTypeDef cmd_parse(char* cmd) {
	char** tokens = cmd_split(cmd, ' ');
	uint32_t count = get_token_count(cmd, ' ');

	if (strcmp(tokens[0], "ls") == 0) {

		cmd_ls();

	} else if (strcmp(tokens[0], "clear") == 0) {

		clear_terminal();
		move_terminal_cursor(0, 0);

	} else if (strcmp(tokens[0], "rm") == 0) {

		cmd_rm(tokens[1]);

	} else if (strcmp(tokens[0], "pg") == 0) {
		cmd_pg(tokens, count);
	}
	else {
		printf("\n\rcommand not found: %s", cmd);
	}

	free_tokens(tokens, get_token_count(cmd, ' '));
	return CMD_OK;
}

/**
 * Build a string from user input characters
 * @param currnet - Current state of string
 * @param input - User input character to be concatenated
 * */
void cmd_build(char** current, char input) {
	uint8_t length;

	if (*current == NULL) {
		length = 0;
	} else {
		length = strlen(*current);
	}

	if ((((uint8_t) input == 0x7F)) && (length > 0)) { //Backspace
		*current = realloc(*current, length);
		(*current)[length - 1] = '\0';
	} else {
		*current = realloc(*current, length + 2);
		(*current)[length] = input;
		(*current)[length + 1] = '\0';
	}

}

/**
 * Split a command into tokens
 * @param cmd - Command to split
 * @param split - Character to split on
 * @return pointer to tokens
 * */
char** cmd_split(char* cmd, char split) {
	uint32_t token_count = get_token_count(cmd, split);
	uint32_t string_index = 0;

	char** tokens = calloc(token_count, sizeof(char*));

	for (int i = 0; i < token_count; i++) {
		while ((cmd[string_index] != split) && (string_index < strlen(cmd))) {
			cmd_build(&(tokens[i]), cmd[string_index]);
			string_index++;
		}
		string_index++;
	}

	return tokens;
}

/**
 * Get number of tokens in given command
 * @param cmd - Command
 * @param split - Character to split on
 * @return number of tokens within command
 * */
uint32_t get_token_count(char* cmd, char split) {
	uint32_t count = 0;
	char* cmd_stripped = cmd_strip(cmd);


	for(int i = 0; i <= strlen(cmd); i++) {
		if (cmd_stripped[i] == split) {
			count++;
		}
	}
	free(cmd_stripped);
	return count + 1;
}

/**
 * Strip command of leading and trailing whitespace
 * @param cmd - Command
 * @return stripped command
 * */
char* cmd_strip(char* cmd) {
	uint32_t start_index = 0;
	uint32_t end_index = strlen(cmd);

	while (cmd[start_index] == ' ') {
		start_index++;
	}

	while (cmd[end_index] == ' ') {
		end_index--;
	}

	char* result = malloc((end_index - start_index + 1) * sizeof(char));

	for (int i = start_index; i < end_index; i++) {
		result[i] = cmd[i];
	}

	result[end_index] = '\0';
	return result;
}

/**
 * Free token array
 * @param tokens - Tokens
 * @param size - Number of tokens
 * */
void free_tokens(char** tokens, uint32_t size) {
	for (int i = 0; i < size; i++) {
		free(tokens[i]);
	}
	free(tokens);
}

/**
 * Show help options for pg command (Shown using pg --help)
 * */
void pg_show_help() {
	printf("\n\r-name - Specifies file name");
	printf("\n\r-type - Specifies card IC type");
	printf("\n\r-mem - Specifies contents of card (comma separated, hexadecimal)");
	printf("\n\r-uid - Specifies card UID (comma separated, hexadecimal)");
}

/**
 * Parse a comma separated string of hex values to byte array
 * @param dest - Destination array
 * @param data_str - String input of data
 * @return CMD_OK if data was successfully parsed
 * */
CMD_StatusTypeDef pg_parse_str(uint8_t** dest, char* data_str) {
	char** nums = cmd_split(data_str, ',');
	uint32_t count = get_token_count(data_str, ',');
	*dest = malloc(count * sizeof(uint8_t));

	for (int i = 0; i < count; i++) {
		(*dest)[i] = strtol(nums[i], NULL, 16);
	}

	return CMD_OK;
}




