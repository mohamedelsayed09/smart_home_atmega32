/*
 * LED.h
 *
 *  Created on: 16 Dec 2022
 *      Author: Aly Tarek
 */

#ifndef HAL_LEDDR_LED_H_
#define HAL_LEDDR_LED_H_

#include "gpio.h"
#include <stdint.h>

//#define LED_PORT  PORTC_ID
#define LED1      PIN2_ID
#define LED2      PIN6_ID
#define LED3      PIN7_ID
#define LED4      PIN3_ID
#define LED5      PIN4_ID

void LED_init(void);

void LED_OFF(uint8_t PortID,uint8_t PinID);


void LED_ON(uint8_t PortID,uint8_t PinID);

void LED_Toggle(uint8_t PortID,uint8_t PinID);






#endif /* HAL_LEDDR_LED_H_ */
