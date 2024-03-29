/*
 * OLED.h
 *
 *  Created on: Jan 29, 2024
 *      Author: rhett
 */

#ifndef INC_OLED_H_
#define INC_OLED_H_

#include "stm32f4xx.h"

#define OLED_CS GPIO_PIN_4
#define OLED_RST GPIO_PIN_3
#define OLED_DC GPIO_PIN_1 //Port B

#define NORMAL 0
#define INVERT 1
#define START_COL 2
#define MID_COL 64
#define END_COL 128
#define OLED_LEFT 0
#define OLED_RIGHT 1
#define OLED_NORESTORE 0
#define OLED_RESTORE 1

#define LOWER_COL 0x02 //Lower nibble op code for column start (0x02)
#define UPPER_COL 0x10 //Upper nibble op code of column start (0x02)
#define CHRG_PMP 0x33 //Set Charge pump output to 9V
#define LINE_STRT 0x40 //Start at line 0x00
#define CNTRST_SET 0x81 //Op code for changing contrast
#define CNTRST 0xFF //Max constrast
#define SEG_MAP 0xA1 //Left rotate
#define DISP_NORM 0xA6
#define DISP_INV 0xA7
#define MUX_SET 0xA8
#define MUX 0x3F
#define DCDC_SET 0xAD
#define DCDC 0x8B
#define DISP_ON 0xAF
#define DISP_OFF 0xAE
#define PAGE_ADDR 0xB0
#define SCAN_DIR 0xC8 //COM[n-1] to COM0
#define DISP_OFFSET_SET 0xD3
#define DISP_OFFSET 0x00
#define OSC_SET 0xD5
#define OSC 0x80 //+15% on top of default internal clock speed
#define PRE_CHRG_SET 0xD9
#define PRE_CHRG 0x1F
#define COM_SET 0xDA
#define COM 0x12
#define VCOM_SET 0xDB
#define VCOM 0x40
#define NOP 0xE3

static const char ASCII[96][5] = {
 {0x00, 0x00, 0x00, 0x00, 0x00} // 20  (space)
,{0x00, 0x00, 0x5f, 0x00, 0x00} // 21 !
,{0x00, 0x07, 0x00, 0x07, 0x00} // 22 "
,{0x14, 0x7f, 0x14, 0x7f, 0x14} // 23 #
,{0x24, 0x2a, 0x7f, 0x2a, 0x12} // 24 $
,{0x23, 0x13, 0x08, 0x64, 0x62} // 25 %
,{0x36, 0x49, 0x55, 0x22, 0x50} // 26 &
,{0x00, 0x05, 0x03, 0x00, 0x00} // 27 '
,{0x00, 0x1c, 0x22, 0x41, 0x00} // 28 (
,{0x00, 0x41, 0x22, 0x1c, 0x00} // 29 )
,{0x14, 0x08, 0x3e, 0x08, 0x14} // 2a *
,{0x08, 0x08, 0x3e, 0x08, 0x08} // 2b +
,{0x00, 0x50, 0x30, 0x00, 0x00} // 2c ,
,{0x08, 0x08, 0x08, 0x08, 0x08} // 2d -
,{0x00, 0x60, 0x60, 0x00, 0x00} // 2e .
,{0x20, 0x10, 0x08, 0x04, 0x02} // 2f /
,{0x3e, 0x51, 0x49, 0x45, 0x3e} // 30 0
,{0x00, 0x42, 0x7f, 0x40, 0x00} // 31 1
,{0x42, 0x61, 0x51, 0x49, 0x46} // 32 2
,{0x21, 0x41, 0x45, 0x4b, 0x31} // 33 3
,{0x18, 0x14, 0x12, 0x7f, 0x10} // 34 4
,{0x27, 0x45, 0x45, 0x45, 0x39} // 35 5
,{0x3c, 0x4a, 0x49, 0x49, 0x30} // 36 6
,{0x01, 0x71, 0x09, 0x05, 0x03} // 37 7
,{0x36, 0x49, 0x49, 0x49, 0x36} // 38 8
,{0x06, 0x49, 0x49, 0x29, 0x1e} // 39 9
,{0x00, 0x36, 0x36, 0x00, 0x00} // 3a :
,{0x00, 0x56, 0x36, 0x00, 0x00} // 3b ;
,{0x08, 0x14, 0x22, 0x41, 0x00} // 3c <
,{0x14, 0x14, 0x14, 0x14, 0x14} // 3d =
,{0x00, 0x41, 0x22, 0x14, 0x08} // 3e >
,{0x02, 0x01, 0x51, 0x09, 0x06} // 3f ?
,{0x32, 0x49, 0x79, 0x41, 0x3e} // 40 @
,{0x7e, 0x11, 0x11, 0x11, 0x7e} // 41 A
,{0x7f, 0x49, 0x49, 0x49, 0x36} // 42 B
,{0x3e, 0x41, 0x41, 0x41, 0x22} // 43 C
,{0x7f, 0x41, 0x41, 0x22, 0x1c} // 44 D
,{0x7f, 0x49, 0x49, 0x49, 0x41} // 45 E
,{0x7f, 0x09, 0x09, 0x09, 0x01} // 46 F
,{0x3e, 0x41, 0x49, 0x49, 0x7a} // 47 G
,{0x7f, 0x08, 0x08, 0x08, 0x7f} // 48 H
,{0x00, 0x41, 0x7f, 0x41, 0x00} // 49 I
,{0x20, 0x40, 0x41, 0x3f, 0x01} // 4a J
,{0x7f, 0x08, 0x14, 0x22, 0x41} // 4b K
,{0x7f, 0x40, 0x40, 0x40, 0x40} // 4c L
,{0x7f, 0x02, 0x0c, 0x02, 0x7f} // 4d M
,{0x7f, 0x04, 0x08, 0x10, 0x7f} // 4e N
,{0x3e, 0x41, 0x41, 0x41, 0x3e} // 4f O
,{0x7f, 0x09, 0x09, 0x09, 0x06} // 50 P
,{0x3e, 0x41, 0x51, 0x21, 0x5e} // 51 Q
,{0x7f, 0x09, 0x19, 0x29, 0x46} // 52 R
,{0x46, 0x49, 0x49, 0x49, 0x31} // 53 S
,{0x01, 0x01, 0x7f, 0x01, 0x01} // 54 T
,{0x3f, 0x40, 0x40, 0x40, 0x3f} // 55 U
,{0x1f, 0x20, 0x40, 0x20, 0x1f} // 56 V
,{0x3f, 0x40, 0x38, 0x40, 0x3f} // 57 W
,{0x63, 0x14, 0x08, 0x14, 0x63} // 58 X
,{0x07, 0x08, 0x70, 0x08, 0x07} // 59 Y
,{0x61, 0x51, 0x49, 0x45, 0x43} // 5a Z
,{0x00, 0x7f, 0x41, 0x41, 0x00} // 5b [
,{0x02, 0x04, 0x08, 0x10, 0x20} // 5c �
,{0x00, 0x41, 0x41, 0x7f, 0x00} // 5d ]
,{0x04, 0x02, 0x01, 0x02, 0x04} // 5e ^
,{0x40, 0x40, 0x40, 0x40, 0x40} // 5f _
,{0x00, 0x01, 0x02, 0x04, 0x00} // 60 `
,{0x20, 0x54, 0x54, 0x54, 0x78} // 61 a
,{0x7f, 0x48, 0x44, 0x44, 0x38} // 62 b
,{0x38, 0x44, 0x44, 0x44, 0x20} // 63 c
,{0x38, 0x44, 0x44, 0x48, 0x7f} // 64 d
,{0x38, 0x54, 0x54, 0x54, 0x18} // 65 e
,{0x08, 0x7e, 0x09, 0x01, 0x02} // 66 f
,{0x0c, 0x52, 0x52, 0x52, 0x3e} // 67 g
,{0x7f, 0x08, 0x04, 0x04, 0x78} // 68 h
,{0x00, 0x44, 0x7d, 0x40, 0x00} // 69 i
,{0x20, 0x40, 0x44, 0x3d, 0x00} // 6a j
,{0x7f, 0x10, 0x28, 0x44, 0x00} // 6b k
,{0x00, 0x41, 0x7f, 0x40, 0x00} // 6c l
,{0x7c, 0x04, 0x18, 0x04, 0x78} // 6d m
,{0x7c, 0x08, 0x04, 0x04, 0x78} // 6e n
,{0x38, 0x44, 0x44, 0x44, 0x38} // 6f o
,{0x7c, 0x14, 0x14, 0x14, 0x08} // 70 p
,{0x08, 0x14, 0x14, 0x18, 0x7c} // 71 q
,{0x7c, 0x08, 0x04, 0x04, 0x08} // 72 r
,{0x48, 0x54, 0x54, 0x54, 0x20} // 73 s
,{0x04, 0x3f, 0x44, 0x40, 0x20} // 74 t
,{0x3c, 0x40, 0x40, 0x20, 0x7c} // 75 u
,{0x1c, 0x20, 0x40, 0x20, 0x1c} // 76 v
,{0x3c, 0x40, 0x30, 0x40, 0x3c} // 77 w
,{0x44, 0x28, 0x10, 0x28, 0x44} // 78 x
,{0x0c, 0x50, 0x50, 0x50, 0x3c} // 79 y
,{0x44, 0x64, 0x54, 0x4c, 0x44} // 7a z
,{0x00, 0x08, 0x36, 0x41, 0x00} // 7b {
,{0x00, 0x00, 0x7f, 0x00, 0x00} // 7c |
,{0x00, 0x41, 0x36, 0x08, 0x00} // 7d }
,{0x10, 0x08, 0x08, 0x10, 0x08} // 7e ?
,{0x00, 0x06, 0x09, 0x09, 0x06} // 7f ?
}; // end char ASCII[96][5]

static const char* HOME_SCREEN[7]={"Ruthless RFID","1.READ CARD","2.WRITE CARD","3.SHOW FILES","4.DISP","5.STATS","6.EXIT"};
static const uint8_t HOME_DATLOC[7][2]={{0,0},{2,START_COL},{4,START_COL},{6,START_COL},{2,MID_COL+15},{4,MID_COL+15},{6,MID_COL+15}};
static const uint8_t HOME_SEL[6][2]={{2,START_COL},{4,START_COL},{6,START_COL},{2,MID_COL+15},{4,MID_COL+15},{6,MID_COL+15}}; //Select arrow info for HOME screen format {Page,column,replacesize}}

static const char* READ_SCREEN[3]={"READ CARD","STANDARD: ISO 14443-3","STATUS: NO CARD"};
static const uint8_t READ_DATLOC[3][2]={{0,0},{2,START_COL},{4,START_COL}};
static const uint8_t READ_SEL[1][2]={{0,0}};

static const char* CARD_FOUNDSCREEN[5]={"CARD FOUND","UID:","IC:","SAVE","EXIT"};
static const uint8_t CARD_FOUNDATLOC[5][2]={{0,0},{2,START_COL},{4,START_COL},{7,START_COL+12},{7,END_COL-24}};
static const uint8_t CARD_FOUNDSEL[2][2]={{7,START_COL},{7,END_COL-36}};

static const char* WRITE_SCREEN[4]={"WRITE CARD","STANDARD: ISO 14443-3","SRC FILE: NONE","STATUS: NO CARD"};
static const uint8_t WRITE_SEL[3][2]={{2,START_COL},{4,START_COL},{6,START_COL}};

/*STRUCT INFO
 *
 * Screen struct is used to define a set of strings to display to the OLED display as well as locations for the select
 * arrow "->" to be placed. The first string in data must always be a header. If a screen has no select options on it
 * the seldata must still be defined in order to know where each string in data should go.
 * */

typedef struct{
    int datsize;
    int selsize;
    char** data;
    uint8_t (*dataloc)[2];
    uint8_t (*seldata)[2];
}Screen;





void OLED_OFF(void);
HAL_StatusTypeDef OLED_cmd(uint8_t data);
HAL_StatusTypeDef OLED_INIT(void);
HAL_StatusTypeDef OLED_data(uint8_t* data,uint8_t size);
void OLED_PWRDWN(void);
void OLED_Test(void);
void OLED_FLUSH(uint8_t* mem);
void SCREEN_INIT(Screen* screen,int datasize,int selsize,char** data,uint8_t (*dataloc)[2],uint8_t (*seldata)[2]);
void OLED_InvChar(char character,uint8_t* result);
void OLED_drawChar(uint8_t page,uint8_t col, char character,uint8_t invert);
void OLED_Printlin(uint8_t page,uint8_t col,char* string,uint8_t invert);
void OLED_Shift(uint8_t page,uint8_t start_col,uint8_t spaces, uint8_t dir, char* string);
void OLED_PrintCent(uint8_t page, char* string, uint8_t invert);
void OLED_Print(char* string);
void OLED_SCREEN(Screen* screen,uint8_t invert);
void OLED_SCRNREF(Screen* screen,uint8_t dataindx,char* data);
void OLED_FILL(uint8_t* data,uint8_t page,uint8_t start_col, uint8_t length);
void OLED_SELECT(Screen* screen,uint8_t selopt,int restore);

#endif /* INC_OLED_H_ */
