/*
 * screen.h
 *
 *  Created on: Jun 11, 2024
 *      Author: rhett
 */

#ifndef INC_SCREEN_H_
#define INC_SCREEN_H_

#include "stdint.h"

#define START_COL 2
#define MID_COL 64
#define END_COL 128

/*STRUCT INFO
 *
 * Screen struct is used to define a set of strings to display to the OLED display as well as locations for the select
 * arrow "->" to be placed. The first string in data must always be a header. If a screen has no select options on it
 * the seldata must still be defined in order to know where each string in data should go.
 * */

typedef struct{
    int datsize;
    int selsize;
    uint8_t restore;
    const char** data;
    const uint8_t (*dataloc)[2];
    const uint8_t (*seldata)[2];
}Screen;

extern const Screen SCRN_Home, SCRN_ReadCard, SCRN_CardFound, SCRN_WriteCard, SCRN_ShowFiles, SCRN_FileData; //Screens

//Card found data locations
#define FOUND_UID_LOC 1 //Location index for UID when calling OLED_SCRNREF
#define FOUND_CARDTYPE_LOC 2 //Location index for PICC type


//Show File data locations
#define SHOWFILE_TYPE_LOC 1
#define SHOWFILE_UID_LOC 2
#endif /* INC_SCREEN_H_ */
