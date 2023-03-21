 /******************************************************************************
 *
 * Module: External EEPROM
 *
 * File Name: external_eeprom.h
 *
 * Description: Header file for the External EEPROM Memory
 *
 * Author: Mohamed Tarek
 *
 *******************************************************************************/
#include <stdlib.h>
#include <stdint.h>

#ifndef EXTERNAL_EEPROM_H_
#define EXTERNAL_EEPROM_H_

//#include "std_types.h"

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/
#define ERROR 0
#define SUCCESS 1

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

uint8_t EEPROM_writeByte(uint16_t u16addr,uint8_t u8data);
uint8_t EEPROM_readByte(uint16_t u16addr,uint8_t *u8data);
 
#endif /* EXTERNAL_EEPROM_H_ */
