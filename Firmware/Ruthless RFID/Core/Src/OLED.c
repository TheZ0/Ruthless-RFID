/*
 * OLED.c
 *
 *  Created on: Jan 29, 2024
 *      Author: rhett
 */

#include "OLED.h"
#include "MFRC.h"
#include "stm32f4xx.h"
#include "usbd_cdc_if.h"
#include "ruthlessfs.h"
#include "screen.h"

// 'Capture1', 128x64px HVE logo
uint8_t HVE[] = {
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f,
		0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x80, 0xe0, 0xf0, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0x07, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
		0xe0, 0xf0, 0xfc, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01, 0x01, 0x01,
		0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01, 0x01, 0x01,
		0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe1, 0x81, 0x01, 0x01, 0x07, 0x3f, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x0f, 0x03, 0x01, 0x01, 0xe1, 0xf9, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xe1, 0xe1, 0xe1, 0xe1, 0xe1, 0xe1, 0xe1, 0xe1, 0xe1, 0xe1, 0xe1, 0xe1, 0xe1, 0xe1,
		0xe1, 0xe1, 0xe1, 0xf3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x0f, 0x01, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xe0, 0xf8, 0xfc, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00,
		0x00, 0xff, 0xff, 0xff, 0xff, 0x8f, 0x87, 0x87, 0x87, 0x87, 0x87, 0x87, 0x07, 0x00, 0x00, 0x00,
		0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0xe0, 0x00, 0x00, 0x01, 0x07, 0x3f, 0xff,
		0xff, 0xff, 0xff, 0x0f, 0x01, 0x00, 0x80, 0xe0, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0x87, 0x87, 0x87, 0x87, 0x87, 0x87, 0x87, 0x87, 0x87, 0x87, 0x87, 0x87, 0x87, 0x87,
		0x87, 0x8f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0x1e,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x81, 0xc1, 0xe1, 0xf1, 0xf9,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00,
		0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00,
		0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0xc0, 0x00, 0x00, 0x01,
		0x0f, 0x7f, 0xff, 0xfe, 0xf8, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f,
		0x3f, 0x3f, 0x3f, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x0f, 0x01, 0x00,
		0x00, 0x00, 0x00, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xfc, 0xfc,
		0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0xfc, 0xfc,
		0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xfc, 0xfc,
		0xfc, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc,
		0xfc, 0xfc, 0xfc, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0x83, 0xc0, 0xe0, 0xf0, 0xf8,
		0xfc, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
	};

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 1040)



/* Function to turn off OLED (sends 0xAE)
 *
 * */
void OLED_OFF(void){
	uint8_t off_op=0xAE;
	HAL_GPIO_WritePin(GPIOA, OLED_CS, 0);
	HAL_SPI_Transmit(&hspi1,&off_op , 1, 100);
	HAL_GPIO_WritePin(GPIOA, OLED_CS, 1);
}
/*Function to clear OLED (write all zero to display memory)
 *
 * */
void OLED_Clear(void){
	uint8_t zeros[1024];
	memset(zeros,0x00,1024);
	OLED_FLUSH(zeros);

}

/*Function to send single byte command to display (ENSURE DC is low)
 *
 * @param data: Command to send to display
 * */
HAL_StatusTypeDef OLED_cmd(uint8_t data){
	uint8_t value=data;
	HAL_GPIO_WritePin(GPIOA, OLED_CS, 0);
	HAL_GPIO_WritePin(GPIOB, OLED_DC, 0);
	if(HAL_SPI_Transmit(&hspi1, &value, 1, HAL_MAX_DELAY)!=HAL_OK){
		HAL_GPIO_WritePin(GPIOA, OLED_CS, 1);
		HAL_Delay(1);
		return(HAL_ERROR);
	}
	else{
		HAL_GPIO_WritePin(GPIOA, OLED_CS, 1);
		HAL_Delay(1);
		return(HAL_OK);
	}

}

/*Function to send display data/write to memory in display (ENSURE DC is high)
 *
 * @param data: Pointer to array of data to send
 *
 * @param size: Size of array (maximum is 1024)
 * */
HAL_StatusTypeDef OLED_data(uint8_t* data,uint8_t size){
	HAL_GPIO_WritePin(GPIOA, OLED_CS, 0);
	HAL_GPIO_WritePin(GPIOB, OLED_DC, 1);
	if(HAL_SPI_Transmit(&hspi1, data, size, HAL_MAX_DELAY)!=HAL_OK){
		HAL_GPIO_WritePin(GPIOB, OLED_DC, 0);
		HAL_GPIO_WritePin(GPIOA, OLED_CS, 1);
		return(HAL_ERROR);
	}
	else{
		HAL_GPIO_WritePin(GPIOB, OLED_DC, 0);
		HAL_GPIO_WritePin(GPIOA, OLED_CS, 1);
		return(HAL_OK);
	}



}

/*Function to send a whole page of display data (1024 bytes)
 *
 * @param mem: Pointer to array of data
 * */
void OLED_FLUSH(uint8_t* mem){
	for(int page=0;page<8;page++){
		OLED_cmd(PAGE_ADDR+page);
		OLED_cmd(LOWER_COL);
		OLED_cmd(UPPER_COL);
		for(int i=0;i<128;i++){
			OLED_data(&mem[(page*128)+i],1);
		}


	}
}


/*Function to initialise OLED display
 *
 * */

HAL_StatusTypeDef OLED_INIT(void){
	HAL_GPIO_WritePin(GPIOA, OLED_CS, 1);
	HAL_GPIO_WritePin(GPIOA, OLED_RST, 1);
	HAL_GPIO_WritePin(GPIOB, OLED_DC, 0);
	HAL_Delay(100);
	HAL_GPIO_WritePin(GPIOA, OLED_RST, 0);
	HAL_Delay(100);
	HAL_GPIO_WritePin(GPIOA, OLED_RST, 1);
	HAL_Delay(100);
	uint8_t config_data[25]={DISP_OFF , LOWER_COL , UPPER_COL , LINE_STRT , PAGE_ADDR , CNTRST_SET , CNTRST , SEG_MAP ,
							DISP_NORM , MUX_SET , MUX , DCDC_SET , DCDC , CHRG_PMP , SCAN_DIR , DISP_OFFSET_SET ,
							DISP_OFFSET , OSC_SET , OSC , PRE_CHRG_SET , PRE_CHRG , COM_SET , COM , VCOM_SET, VCOM};


	for(int i = 0; i < 25; i++){
		OLED_cmd(config_data[i]);
		HAL_Delay(1);
	}

	OLED_cmd(DISP_INV);
	OLED_FLUSH(HVE);
	OLED_cmd(DISP_ON);
	HAL_Delay(1000);
	OLED_cmd(DISP_OFF);
	HAL_Delay(10);
	OLED_cmd(DISP_NORM);
	HAL_Delay(10);
	OLED_cmd(DISP_ON);
	OLED_Clear();
	return HAL_OK;

}


/*Function to invert the characters from ASCII in OLED.h (Default is 1 is on 0 is off)
 *
 * @param character: Character to invert (Must be an ASCII character)
 *
 * @param result: Pointer to array to store the 5 inverted bytes that make up the character
 * */
void OLED_InvChar(char character,uint8_t* result){
	uint8_t temp[OLED_CHARSIZE];

	for(int i=0;i<=OLED_CHARSIZE;i++){
			temp[i]=~(ASCII[(uint8_t)character-ASCII_START][i]);
	}
	memcpy(result,temp,OLED_CHARSIZE);
}

/*Function to draw a single character on the display
 *
 * @param page: Row to write on (Display has 8 pages/rows)
 *
 * @param col: Column to begin writing (Display has 128 columns)
 *
 * @param character: Character to draw
 *
 * @param invert: Option to invert the character (Use INVERT to invert defined in OLED.h)
 * */

void OLED_drawChar(uint8_t page,uint8_t col, char character, uint8_t invert){
	OLED_cmd(PAGE_ADDR+page);
	OLED_cmd(col&0x0F);
	OLED_cmd(0x10|(col>>4));
	uint8_t data[6];

	if(invert==NORMAL){
		memcpy(data,ASCII[(uint8_t)character-0x20],5);
		data[5]=0x00;
	}
	else{
		data[0]=0xFF;
		OLED_InvChar(character,data + 1);

	}

	OLED_data(data, 6);

	}

/*Function to write string on a certain line of display
 *
 * @param page: Row to write on (Display has 8 pages/rows)
 *
 * @param col: Column to begin writing (Display has 128 columns)
 *
 * @param string: String to print (MUST BE LESS THAN 21)
 *
 * @param invert: Option to invert the entire string
 * */

void OLED_Printlin(uint8_t page,uint8_t col,char* string,uint8_t invert){
	for(int i=0;i<strlen(string);i++){
		OLED_drawChar(page, col+(i*6), string[i],invert);
	}
}

/*Function to shift a string over a certain number of spaces
 *
 * @param page: OLED page where string is
 *
 * @param start_col: Column location of first character in string
 *
 * @param spaces: Number of spaces to shift string
 *
 * @param dir: Direction to shift string (OLED_LEFT or OLED_RIGHT)
 *
 * @param string: String to shift
 * */

void OLED_Shift(uint8_t page,uint8_t start_col,uint8_t spaces, uint8_t dir, char* string){

	char data[strlen(string)+spaces];

	memset(data,' ',spaces); //Fill start of array with whitespace
	strcpy(data+spaces,string); //Fill resting of data with string we want to shift

	OLED_Printlin(page, start_col, data, NORMAL);
}

/*Function to print a single line center justified
 *
 * @param page: Page to print string to
 *
 * @param string: String to print
 *
 * @param invert: Option to invert text
 * */

void OLED_PrintCent(uint8_t page, char* string, uint8_t invert){
	int len=strlen(string);

	int start_col=(MID_COL)-((len/2)*6);

	OLED_Printlin(page, start_col, string, invert);
}

/*General purpose print function with wrap around ability. (Can take string of abitrary size and fit onto display)
 *
 * @param string: String to print (This can be longer than the max 21 character per line this function will chop it up)
 * */

void OLED_Print(char* string){
	char fill = ' ';
	int i=0;
	int last_ind=0;
	int line=0;
	int linend=0;

	    while(1){

	        char thisline[22];
	        char whitespaces[20];

	        for(int charac=0;charac<22;charac++){
	            if(string[last_ind+charac]==' '){
	                whitespaces[i]=charac; //keep track of our whitespaces
	                i++;
	            }
	            else{
	                continue;
	            }


	        }

	        if(string[last_ind]==' '){
	            last_ind++;
	        }

	        else{

	        }

	        if(strlen(string+last_ind)<22){ //special routine for end of line (since string[last_ind+20] may not exist)
	            linend=strlen(string+last_ind);
	            strncpy(thisline,string+last_ind,linend); //i-1 since the last loop of whitespaces will increment i i.e. if 4 whitespaces in first 21 characters whitespaces[3] will be last index
	            memset(thisline+linend,fill,21-linend);
	            thisline[21]=' ';
	            OLED_Printlin(line, 0x02,thisline,NORMAL);
	            break;
	        }

	        else{
	        if((string[(last_ind)+20]!=' ') && (string[(last_ind)+21]!=' ')){
	            strncpy(thisline,string+last_ind,whitespaces[i-1]); //i-1 since the last loop of whitespaces will increment i i.e. if 4 whitespaces in first 21 characters whitespaces[3] will be last index
	            memset(thisline+whitespaces[i-1],fill,21-whitespaces[i-1]);
	            thisline[21]=' ';
	            last_ind=last_ind+whitespaces[i-1];

	            }

	        else{
	            strncpy(thisline,string+last_ind,21);
	            thisline[21]=' ';
	            last_ind=last_ind+21;

	        }
	        }
	        i=0;
	        OLED_Printlin(line, 0x02, thisline,NORMAL);
	        line++;





	    }
}

/*Function to display a Screen e.g. HOME, Read card etc
 *
 * @param screen: Screen to display
 *
 * @param invert: Option to invert entire screen
 * */

void OLED_SCREEN(const Screen* screen,uint8_t invert) {
	OLED_Clear();
	OLED_PrintCent(0, (char*)screen->data[0], invert);

	for(int i = 0; i < screen->datsize - 1; i++) {
		OLED_Printlin((uint8_t)screen->dataloc[i+1][0],(uint8_t) screen->dataloc[i+1][1],(char*) screen->data[i+1], invert);
	}
}

/*Function to add dynamic data to base screen e.g. adding UID read from card to display
 *
 * @param screen: Base screen to add data to
 *
 * @param dataindx: Index of screen->data to modify
 *
 * @param data: Data to add to the screen
 * */

void OLED_SCRNREF(const Screen* screen,uint8_t dataindx,char* data){
	int len=strlen(screen->data[dataindx]);
	int start_col=screen->dataloc[dataindx][1]+((len)*6);

	OLED_Printlin(screen->dataloc[dataindx][0], start_col, data, NORMAL);
}

/*Function to fill area of display with certain character
 *
 * @param data: Single byte of data to fill screen with
 *
 * @param page: Row to begin writing
 *
 * @param col: Column to begin writing (Display has 128 columns)
 *
 * @param length: Length of fill
 * */

void OLED_FILL(uint8_t* data,uint8_t page,uint8_t start_col, uint8_t length){
	OLED_cmd(PAGE_ADDR+page);
	OLED_cmd(start_col&0x0F);
	OLED_cmd(0x10|(start_col>>4));
	for(int i=0;i<length*6;i++){
		OLED_data(data, 1);

	}
}

/*Function to place the select arrow at certain position of display
 *
 * @param page: Row to place arrow
 *
 * @param col: Column to place arrow
 * */

void OLED_SELECT(const Screen* screen,uint8_t selopt, int restore){

	char arrow[]="->";

	uint8_t prevpage,prevcol,thispage,thiscol,index;
	char* rest;

	if(selopt == 0){
	    index = screen->selsize - 1;
	} else {
		index = selopt - 1;
	}

	prevpage=screen->seldata[index][0];
	prevcol=screen->seldata[index][1];
	thispage=screen->seldata[selopt][0];
	thiscol=screen->seldata[selopt][1];

	if(restore == OLED_RESTORE){
	    if (selopt == 0) {
	    	rest=(char*)screen->data[find_restore_string(screen, screen->selsize - 1)]; //Here we may be wrapping around so we must restore the last select option
	    } else {
	    	rest=(char*)screen->data[find_restore_string(screen, selopt - 1)];
	    }
	} else {
	    rest = "  ";
	}

	    OLED_Printlin(prevpage, prevcol, rest, NORMAL); //This restores the line of the previous (now deselected) select option
	    OLED_Printlin(thispage, thiscol, arrow, NORMAL);

}

/**
 * Select option by inverting the character which is selected
 * @param screen - Screen currently displayed
 * @param selopt - Selected select option
 * */
void OLED_select_inv(const Screen* screen, uint8_t selopt) {
	uint8_t index, prevpage, prevcol, thispage, thiscol;
	char* restore;

	if(selopt == 0){
		index = screen->selsize - 1;
	} else {
		index = selopt - 1;
	}

	prevpage=screen->seldata[index][0];
	prevcol=screen->seldata[index][1];
	thispage=screen->seldata[selopt][0];
	thiscol=screen->seldata[selopt][1];

	if (selopt == 0) {
		restore=(char*)screen->data[find_restore_string(screen, screen->selsize - 1)]; //Here we may be wrapping around so we must restore the last select option
	} else {
		restore=(char*)screen->data[find_restore_string(screen, selopt - 1)];
	}

	OLED_Printlin(prevpage, prevcol, restore, NORMAL);
	OLED_Printlin(thispage, thiscol, screen->data[selopt + 1], INVERT);
}

/*Function to perform the display's power down sequence
 * */

void OLED_PWRDWN(void){
	OLED_OFF();
	uint8_t data[]={0x8D,0x10};
	OLED_cmd(data[0]);
	HAL_Delay(1);
	OLED_cmd(data[1]);
}

/**
 * Display filenames to OLED display (used in show files task)
 *
 * @param page - Page of files to display (page count is increased as user scrolls through files)
 * @return HAL_OK if files were successfully displayed
 * */
HAL_StatusTypeDef OLED_display_files(const Screen* screen, uint8_t page) {
	if (get_number_files() == 0) {
		return HAL_ERROR;
	}
	uint16_t file_count = get_number_files();

	char** file_names = malloc(file_count * sizeof(char*));
	char used[5];
	char free[5];

	get_free_size_str(free);
	get_used_size_str(used);

	OLED_SCRNREF(&SCRN_ShowFiles, 4, free);
	OLED_SCRNREF(&SCRN_ShowFiles, 5, used);

	if (get_all_files(file_names) != RFS_OK) {
		free_filenames(file_names, file_count);
		return HAL_ERROR;
	}

	for (int i = 0; i < file_count; i++) {
		OLED_SCRNREF(&SCRN_ShowFiles, i + 1, file_names[i]);
	}

	free_filenames(file_names, file_count);
	return HAL_OK;
}

/**
 * Free list of file names
 * @param file_names - File names
 * @param size -  Number of file names
 * */
void free_filenames(char** file_names, int size) {
	for (int i = 0; i < size; i++) {
		free(file_names[i]);
	}
	free(file_names);
}

/**
 * Find the string to restore for given previous string selected
 *
 * @param screen - Screen to update
 * @param prev_sel_opt - Previous selected option
 * @return index of string in screen->data to be used as replacement
 * */
uint8_t find_restore_string(const Screen* screen, uint8_t prev_sel_opt) {
	uint8_t index_of_string;

	for (int i = 1; i < screen->datsize; i++) {
		if((screen->dataloc[i][0] == screen->seldata[prev_sel_opt][0]) && (screen->dataloc[i][1] == screen->seldata[prev_sel_opt][1])) {
			index_of_string = i;
			break;
		}
	}

	return index_of_string;
}

/**
 * Show the file data of given file entry number
 * @param entry - Entry to show data of
 * */
void oled_show_file(uint16_t entry) {
	Card* work;

	if (entry_present(entry) != RFS_OK) {
		return; //No card entry present
	}

	work = read_card_entry(entry);

	OLED_SCREEN(&SCRN_FileData, NORMAL);
	OLED_SCRNREF(&SCRN_FileData, 0, work->name);
	OLED_SCRNREF(&SCRN_FileData, SHOWFILE_TYPE_LOC, work->type);
	OLED_SCRNREF(&SCRN_FileData, SHOWFILE_UID_LOC, uid_tostring(work->uid, work->uidsize));
	OLED_SELECT(&SCRN_FileData, 0, OLED_NORESTORE);

	free(work);
}

/**
 * Move selection arrow for given screen
 * @param screen - Screen to be modified
 * @param arrow_index - Pointer to variable which keeps track of arrow index
 * @param restore - Option to restore previous text or not
 * */
void oled_move_selection(const Screen* screen, uint8_t* arrow_index, uint8_t restore) {
	uint8_t max_index = screen->selsize;

	if (*arrow_index >= max_index - 1) {
		*arrow_index = 0;
	} else {
		*arrow_index += 1;

	}
	OLED_SELECT(screen, *arrow_index, restore);
}

/**
 * Select by inverting the text on the screen
 * @param screen - Screen to modify
 * @param select_index - Pointer to variable that keeps track of selection index
 * */
void oled_move_selection_inv(const Screen* screen, uint8_t* select_index) {
	uint8_t max_index = screen->selsize;

	if (*select_index >= max_index - 1) {
		*select_index = 0;
	} else {
		*select_index += 1;
	}

	OLED_select_inv(screen, *select_index);
}
