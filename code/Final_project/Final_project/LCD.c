/**************************************************
 *  file name:LCD.c
 *  Created on: Oct 4, 2022
 *  Description : Source file for ATmega32 LCD DRIVER
 *  Author: Mohamed Magdy
 **************************************************/
#include "gpio.h"
#include "common_macros.h" /* To get macros like GET_BIT */
#include "lcd.h"
#include <util/delay.h> /* for delay function */
#include <stdint.h>

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/*
 * Description :
 * Initialize the LCD:
 * 1. Setup the LCD pins directions by use the GPIO driver.
 * 2. Setup the LCD Data Mode 4-bits or 8-bits.
 */
void LCD_init(void)
{
	/* configure direction of RS , RW, E pins as output pins*/
	GPIO_setupPinDirection(LCD_RS_PORT_ID, LCD_RS_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_E_PORT_ID, LCD_E_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_RW_PORT_ID, LCD_RW_PIN_ID, PIN_OUTPUT);
	_delay_ms(20);
#if(LCD_DATA_BITS_MODE==4)
	/* Configure 4 pins in the data port as output pins */

GPIO_setupPinDirection(LCD_DATA_PORT_ID, LCD_DB4_PIN_ID, PIN_OUTPUT);
GPIO_setupPinDirection(LCD_DATA_PORT_ID, LCD_DB5_PIN_ID, PIN_OUTPUT);
GPIO_setupPinDirection(LCD_DATA_PORT_ID, LCD_DB6_PIN_ID, PIN_OUTPUT);
GPIO_setupPinDirection(LCD_DATA_PORT_ID, LCD_DB7_PIN_ID, PIN_OUTPUT);

/* Send for 4 bit initialization of LCD  */

LCD_sendCommand(LCD_FOUR_BITS_TWO_LINES_MODE_INIT1);
LCD_sendCommand(LCD_FOUR_BITS_TWO_LINES_MODE_INIT2);
/* use 2-lines LCD + 4-bits Data Mode + 5*7 dot display Mode */

LCD_sendCommand(LCD_FOUR_BITS_TWO_LINES_MODE);

#elif (LCD_DATA_BITS_MODE==8)
/* Configure the data port as output port */
 	GPIO_setupPortDirection(LCD_DATA_PORT_ID, PORT_OUTPUT);

/* use 2-lines LCD + 8-bits Data Mode + 5*7 dot display Mode */
	LCD_sendCommand(LCD_EIGHT_BITS_TWO_LINES_MODE);
#endif

	LCD_sendCommand(LCD_CURSOR_OFF); /* cursor off */
	LCD_sendCommand(LCD_CLEAR_SCREEN);/* clear LCD at the beginning */
}
/*
 * Description :
 * Send the required command to the screen
 */
void LCD_sendCommand (uint8_t command){
	GPIO_writePin(LCD_RW_PORT_ID, LCD_RW_PIN_ID, LOGIC_LOW); /* WRITE 0 ON RW*/
	_delay_ms(1);
    GPIO_writePin(LCD_RS_PORT_ID, LCD_RS_PIN_ID, LOGIC_LOW);  /* Instruction Mode RS=0 */
	_delay_ms(1);
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_HIGH);  /* Enable LCD E=1 */
	_delay_ms(1);
#if(LCD_DATA_BITS_MODE==4)
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB4_PIN_ID, GET_BIT(command,4));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB5_PIN_ID, GET_BIT(command,5));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB6_PIN_ID, GET_BIT(command,6));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB7_PIN_ID, GET_BIT(command,7));
	_delay_ms(1);
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_LOW); /* Disable LCD E=0 */
	_delay_ms(1);
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_HIGH);/* Enable LCD E=0 */
	_delay_ms(1);
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB4_PIN_ID, GET_BIT(command,0));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB5_PIN_ID, GET_BIT(command,1));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB6_PIN_ID, GET_BIT(command,2));
	GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB7_PIN_ID, GET_BIT(command,3));
	_delay_ms(1);
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_LOW);
	_delay_ms(1);
	GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_HIGH);
	_delay_ms(1);

#elif(LCD_DATA_BITS_MODE==8)
	GPIO_writePort(LCD_DATA_PORT_ID, command);
		_delay_ms(1);
		GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_LOW);
		_delay_ms(1);
#endif
}
/*
 * Description :
 * Display the required character on the screen
 */
void LCD_displayCharacter(uint8_t character){

	GPIO_writePin(LCD_RS_PORT_ID, LCD_RS_PIN_ID, LOGIC_HIGH);
		_delay_ms(1);
		GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_HIGH);
		_delay_ms(1);
#if(LCD_DATA_BITS_MODE==4)
		GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB4_PIN_ID, GET_BIT(character,4));
			GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB5_PIN_ID, GET_BIT(character,5));
			GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB6_PIN_ID, GET_BIT(character,6));
			GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB7_PIN_ID, GET_BIT(character,7));
			_delay_ms(1);
			GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_LOW);
			_delay_ms(1);
			GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_HIGH);
			_delay_ms(1);
			GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB4_PIN_ID, GET_BIT(character,0));
			GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB5_PIN_ID, GET_BIT(character,1));
			GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB6_PIN_ID, GET_BIT(character,2));
			GPIO_writePin(LCD_DATA_PORT_ID,LCD_DB7_PIN_ID, GET_BIT(character,3));
			_delay_ms(1);
			GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_LOW);
			_delay_ms(1);
			GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_HIGH);
			_delay_ms(1);
#elif(LCD_DATA_BITS_MODE == 8)

		GPIO_writePort(LCD_DATA_PORT_ID, character);
		_delay_ms(1);
		GPIO_writePin(LCD_E_PORT_ID, LCD_E_PIN_ID, LOGIC_LOW);
		_delay_ms(1);
#endif
}
/*
 * Description :
 * Display the required string on the screen
 */

void LCD_displayString (const char *string){
	while((*string) !='\0'){
		LCD_displayCharacter(*string);
		string++;
	}
	/*
	 * Description :
	 * Move the cursor to a specified row and column index on the screen
	 */
}
void LCD_moveCursor(uint8_t row,uint8_t col){
	uint8_t LCD_memory_address;
	switch(row){
	case 0:
		LCD_memory_address=col;
		break;
	case 1:
		LCD_memory_address=col+0x40;
		break;
	case 2:
		LCD_memory_address=col+0x10;
		break;
	case 3:
		LCD_memory_address=col+0x50;
		break;
	}
	LCD_sendCommand(LCD_memory_address | LCD_SET_CURSOR_LOCATION);
/*
 * Description :
 * Display the required string in a specified row and column index on the screen
 */
}
void LCD_displayStringRowColumn(uint8_t row,uint8_t col, char*word){
LCD_moveCursor(row, col);
LCD_displayString(word);
}
/*
 * Description :
 * Send the clear screen command
 */
void LCD_clearScreen(void){
	LCD_sendCommand(LCD_CLEAR_SCREEN);
}
/*
 * Description :
 * Display the required decimal value on the screen
 */
void LCD_integerToString (int data){

	char buff[16];
	itoa (data,buff,10);
	LCD_displayString(buff);
}
