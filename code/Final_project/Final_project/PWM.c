/*
 * PWM.c
 *
 *  Created on: Oct 5, 2022
 *      Author: magdy-pc
 */


#include <avr/io.h>
#include <stdint.h>
void TIMER0_INIT(uint8_t speed)
{
   
	TCCR0=(1<<WGM01)|(1<<WGM00)|(1<<CS01)|(1<<COM01);
	TCNT0=0;
	OCR0=((float)speed/100)*255;
	DDRB|=(1<<PB3);

}
