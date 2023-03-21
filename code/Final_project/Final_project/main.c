/*
 * Final_project.c
 *
 * Created: 27/02/2023 07:01:56 م
 * Author : Dell
 */ 

#include <avr/io.h>
#include <stdlib.h>
#include <stdint.h>
#include <util/delay.h>
#include <string.h>

#include "adc.h"
#include "common_macros.h"
#include "dcMotor.h"
#include "gpio.h"
#include "uart.h"
#include "PWM.h"
#include "LED.h"
#include "lm35_sensor.h"
#include "lcd.h"
#include "keypad.h"
#include "external_eeprom.h"
#include "twi.h"
#include "avr/interrupt.h"


#define address   0x05
#define bit_rate   2
#define admin_name_address    0x00
#define admin_pass_address    0x10
#define user_name_address     0x20
#define user_pass_address     0x80  


uint8_t user_number = 0;
uint8_t user_count = '0';
uint8_t user_counter = 0;
uint8_t type;
uint8_t lcd_user_counter = 0;

char admin_name[4];
char admin_pass[4];
char receved_name[4];
char receved_pass[4];
char user_name[4];
char user_pass[4];
char user_receved_name[4];
char user_receved_pass[4];
char user_comp_name[4];
char user_comp_pass[4];
char lcd_user_name[4];
char lcd_user_pass[4];
char lcd_user_comp_name[4];
char lcd_user_comp_pass[4];


void smart_home(void);
uint8_t program_init(void);
void admin_prog_init(void);
void admin_info(void);
uint8_t compare_info(void);
void alarm_check(void);
void main_opt(void);
void aircond_uart(void);
void air_conditioner_on(void);
void air_conditioner_off(void);
void choose_led_uart(void);
void open_led (uint8_t led_num);
void close_led (uint8_t led_num);
void dimmer_uart(void);
void door_control(void);
void user_control(void);
void add_user(void);
void remove_user(void);
void user_prog_init(void);
void user_info(void);
uint8_t user_compare_info(void);
uint8_t user_check(void);
void user_alarm_check(void);
void user_main_opt(void);
void interrupt_init(void);
void lcd_program_init(void);
void lcd_user_info(void);
uint8_t lcd_user_compare_info(void);
uint8_t lcd_user_check(void);
void lcd_user_alarm_check(void);
void lcd_main_opt(void);
void aircond_lcd(void);
void lcd_air_conditioner_on(void);
void lcd_air_conditioner_off(void);
void choose_led_lcd(void);
void open_led_lcd (uint8_t led_num);
void close_led_lcd (uint8_t led_num);
void dimmer_lcd(void);


int main(void)
{
	uint8_t device_num;
	TWI_ConfigType twi_conf={address,bit_rate};
	TWI_init(&twi_conf);
	UART_init(9600);
	ADC_init();
	DcMotor_Init();
	LCD_init();
	DcMotor_Init();
	ADC_init();
	LED_init();
	interrupt_init();
	smart_home();
}

void smart_home(void)
{
	uint8_t state;
	state = program_init();
	switch (state)
	{
		case '1':
		admin_prog_init();
		alarm_check();
		main_opt();
		break;
		case '2':
		user_alarm_check();
		user_main_opt();
		break;
	}
}

uint8_t program_init(void)
{
	uint8_t user_type;
	UART_sendString("welcome...!\r");
	_delay_ms(1000);
	UART_sendString("************************************* \r");
	UART_sendString("who are you? \r");
	UART_sendString("1:admin \r");
	UART_sendString("2:user \r");
	user_type = UART_recieveByte();
	type = user_type;
	return user_type;
}

void admin_prog_init(void)
{
	uint8_t i=0,j=0;
	while (i<4)
	{
		while(!EEPROM_writeByte(admin_name_address+i,'1'));
		i++;
		_delay_ms(10);
	}
	while (j<4)
	{
		while(!EEPROM_writeByte(admin_pass_address+j,'0'));
		j++;
		_delay_ms(10);
	}
	UART_sendString("you have 3 trials \r");
}

void admin_info(void)
{
	uint8_t i=0,j=0;
	UART_sendString("enter admin name \r");
	UART_sendString("admin name: ");
	while (i<4)
	{
		admin_name[i]=UART_recieveByte();
		UART_sendByte(admin_name[i]);
		i++;
	}
	UART_sendString("\r");
	UART_sendString("enter admin password \r");
	UART_sendString("password: ");
	while (j<4)
	{
		admin_pass[j]=UART_recieveByte();
		UART_sendByte(admin_pass[j]);
		j++;
	}
	UART_sendString("\r");
}

uint8_t compare_info(void)
{
	uint8_t i=0,j=0,k=0;
	while(j<4)
	{
		while(!EEPROM_readByte(admin_name_address+j,receved_name+j));
		j++;
		_delay_ms(10);
	}
	while (k<4)
	{
		while(!EEPROM_readByte(admin_pass_address+k,receved_pass+k));
		k++;
		_delay_ms(10);
	}
	while(i<4)
	{
		if ((receved_name[i]==admin_name[i])&&(receved_pass[i]==admin_pass[i]))
		{
			return 1;
		}
		else
		{
			return 0;
			break;
		}
		i++;
	}
}

void alarm_check(void)
{
	uint8_t check,counter=0;
	while(counter < 3)
		{
			admin_info();
			check = compare_info();
			if (check ==1)
			{
				UART_sendString(" \r");
				UART_sendString("welcome back... :) \r");
				UART_sendString(" \r");
				_delay_ms(1000);
				counter=0;
				break;
			}
			else
			{
				UART_sendString("false \r");
				counter++;
				if (counter == 3)
				{
					UART_sendString("security mode... \r");
					GPIO_setupPinDirection(PORTA_ID,0,PIN_OUTPUT);
					UART_sendString("error :(");
					LCD_clearScreen();
					LCD_displayString("security mode...");
					while(1)
					{
						GPIO_writePin(PORTA_ID,0,LOGIC_HIGH);
					}
				}
			}
	}
}

void main_opt(void)
{
	uint8_t device;
	UART_sendString("**********choose the device********** \r");
	LCD_clearScreen();
	LCD_moveCursor(0,1);
	LCD_displayString("main options");
	UART_sendString("1:air conditioner \r");
	UART_sendString("2:leds control\r");
	UART_sendString("3:dimmer circuit \r");
	UART_sendString("4:door control\r");
	UART_sendString("5:adding or removing users \r");
	UART_sendString("6:return to the first page \r");
	UART_sendString("************************************* \r");
	UART_sendString(" \r");
	device = UART_recieveByte();
	switch (device)
	{
		case '1':aircond_uart();break;
		case '2':choose_led_uart();break;
		case '3':dimmer_uart();break;
		case '4':door_control();break;
		case '5':user_control();break;
		case '6':smart_home();break;
	}
}

void aircond_uart(void)
{
	uint8_t state;
	UART_sendString("********air conditioner menu****** \r");
	_delay_ms(1000);
	UART_sendString("1:air conditioner on \r");
	UART_sendString("2:air conditioner off \r");
	UART_sendString("3:Main \r");
	UART_sendString("************************************* \r");
	UART_sendString(" \r");
	LCD_clearScreen();
	LCD_moveCursor(0,1);
	LCD_displayString("air conditioner");
	LCD_moveCursor(1,3);
	LCD_displayString("RUNING...");
	state = UART_recieveByte();
	switch (state)
	{
		case '1':air_conditioner_on();break;
		case '2':air_conditioner_off();break;
		case '3':
		if (type == '1')
		{
		    main_opt();break;	
		}
		if (type == '2')
		{
			user_main_opt();break;
		}
	}
}

void air_conditioner_on(void)
{
	uint8_t temp,button=1;
	UART_sendString("      air conditioner runinng...    \r");
	UART_sendString(" \r");
	_delay_ms(1000);
	while (button == 1){
		button = GPIO_readPin(PORTB_ID,PIN0_ID);
		temp= LM35_getTemperature();
		if (temp >= 28)
		{
			DcMotor_Rotate(CW,50);
		}
		else if (temp < 28)
		{
			DcMotor_Rotate(Stop,0);
		}
	}
	DcMotor_Rotate(Stop,0);
	aircond_uart();
}

void air_conditioner_off(void)
{
	DcMotor_Rotate(Stop,0);
	UART_sendString("      air conditioner closed...    \r");
	UART_sendString(" \r");
	_delay_ms(1000);
	aircond_uart();
}

void choose_led_uart(void)
{
	uint8_t state,led;
	UART_sendString("***************led menu************** \r");
	UART_sendString("1:open led \r");
	UART_sendString("2:close led \r");
	UART_sendString("3:Main \r");
	LCD_clearScreen();
	LCD_moveCursor(4,1);
	LCD_displayString("led");
	LCD_moveCursor(1,3);
	LCD_displayString("RUNING...");
	state = UART_recieveByte();
	switch (state)
	{
		case '1':
		UART_sendString("press the led number \r");
		led = UART_recieveByte();
		open_led(led);
		break;
		
		case'2':
		UART_sendString("press the led number \r");
		led = UART_recieveByte();
		close_led(led);
		break;
		
		case '3':
		if (type == '1')
		{
			main_opt();break;
		}
		if (type == '2')
		{
			user_main_opt();break;
		}
		
	}
}

void open_led (uint8_t led_num)
{
	switch (led_num)
	{
		case '1':LED_ON(PORTA_ID,LED1);break;
		case '2':LED_ON(PORTC_ID,LED2);break;
		case '3':LED_ON(PORTC_ID,LED3);break;
		case '4':LED_ON(PORTD_ID,LED4);break;
		case '5':LED_ON(PORTD_ID,LED5);break;

	}
	choose_led_uart();
}

void close_led (uint8_t led_num)
{
	switch (led_num)
	{
		case '1':LED_OFF(PORTA_ID,LED1);break;
		case '2':LED_OFF(PORTC_ID,LED2);break;
		case '3':LED_OFF(PORTC_ID,LED3);break;
		case '4':LED_OFF(PORTD_ID,LED4);break;
		case '5':LED_OFF(PORTD_ID,LED5);break;

	}
	choose_led_uart();
}

void dimmer_uart(void)
{
	uint8_t percentage;
	UART_sendString("choose lamp percentage \r");
	_delay_ms(1000);
	UART_sendString("1: 0% \r");
	UART_sendString("2: 25% \r");
	UART_sendString("3: 50% \r");
	UART_sendString("4: 75% \r");
	UART_sendString("5: 100% \r");
	LCD_clearScreen();
	LCD_moveCursor(0,1);
	LCD_displayString("dimmer lamp");
	LCD_moveCursor(1,3);
	LCD_displayString("RUNING...");
	percentage = UART_recieveByte();
	switch (percentage)
	{
		case '1':TIMER0_INIT(0);break;
		case '2':TIMER0_INIT(25);break;
		case '3':TIMER0_INIT(50);break;
		case '4':TIMER0_INIT(75);break;
		case '5':TIMER0_INIT(100);break;
	}
	if (type == '1')
	{
		main_opt();
	}
	if (type == '2')
	{
		user_main_opt();
	}
}

void user_control(void)
{
    uint8_t state;
	UART_sendString("choose add or remove user \r");
	UART_sendString("users now equal ");
	UART_sendByte(user_count);
	UART_sendString("\r");
	_delay_ms(1000);
	UART_sendString("1:add user \r");
	UART_sendString("2:remove user \r");
	UART_sendString("3:Main \r");
	state = UART_recieveByte();
	switch (state)
	{
		case '1':add_user();break;
		case '2':remove_user();break;
		case '3':
		if (type == '1')
		{
			main_opt();break;
		}
		if (type == '2')
		{
			user_main_opt();break;
		}
	}
}

void add_user(void)
{
	uint8_t i=0,j=0,k=0,s=0;
	UART_sendString("enter user name \r");
	UART_sendString("user name: ");
	while (i<4)
	{
		user_name[i]=UART_recieveByte();
		UART_sendByte(user_name[i]);
		i++;
	}
	UART_sendString("\r");
	UART_sendString("enter user password \r");
	UART_sendString("password: ");
	while (j<4)
	{
		user_pass[j]=UART_recieveByte();
		UART_sendByte(user_pass[j]);
		j++;
	}
	UART_sendString("\r");
	while (k<4)
	{
		while(!EEPROM_writeByte(user_name_address+user_number+k,user_name[k]));
		k++;
		_delay_ms(10);
	}
	while (s<4)
	{
		while(!EEPROM_writeByte(user_pass_address+user_number+s,user_pass[s]));
		s++;
		_delay_ms(10);
	}
	user_number+=4;
	user_count++;
	if (type == '1')
	{
		main_opt();
	}
	if (type == '2')
	{
		user_main_opt();
	}
}

void remove_user(void)
{
	uint8_t i=0,k=0,x;
	UART_sendString("write the user number \r");
	UART_sendString("the number should be greater than ");                      
	UART_sendByte(user_count);
	UART_sendString("\r");
	x = UART_recieveByte();
	UART_sendByte(x);
	UART_sendString("\r");
	
	while (k<4)
	{
		while(!EEPROM_writeByte(user_name_address+((x-'0')-1)*4+k,'0'));
		k++;
		_delay_ms(10);
	}
	while (i<4)
	{
		while(!EEPROM_writeByte(user_pass_address+((x-'0')-1)*4+i,'0'));
		i++;
		_delay_ms(10);
	}
	user_number-=4;
	user_count--;
	UART_sendString("done... \r");
	_delay_ms(500);
	if (type == '1')
	{
		main_opt();
	}
	if (type == '2')
	{
		user_main_opt();
	}
}

void user_prog_init(void)
{
	uint8_t i=0,j=0;
	UART_sendString("hello! \r");
	UART_sendString("users now equal ");
	UART_sendByte(user_count);
	UART_sendString("\r");
	UART_sendString("enter user name \r");
	UART_sendString("user name: ");
	while (i<4)
	{
		user_receved_name[i]=UART_recieveByte();
		UART_sendByte(user_receved_name[i]);
		i++;
	}
	UART_sendString("\r");
	UART_sendString("enter user password \r");
	UART_sendString("user password: ");
	while (j<4)
	{
		user_receved_pass[j]=UART_recieveByte();
		UART_sendByte(user_receved_pass[j]);
		j++;
	}
	UART_sendString("\r");
}

void user_info(void)
{
	uint8_t i=0,j=0,k=0;
	while(j<4)
	{
		while(!EEPROM_readByte(user_name_address+user_counter+j,user_comp_name+j));
		j++;
		_delay_ms(10);
	}
	while (k<4)
	{
		while(!EEPROM_readByte(user_pass_address+user_counter+k,user_comp_pass+k));
		k++;
		_delay_ms(10);
	}
	
}

uint8_t user_compare_info(void)
{
	uint8_t i=0;
	while(i<4)
	{
		if ((user_receved_name[i]==user_comp_name[i])&&(user_receved_pass[i]==user_comp_pass[i]))
		{
			return 1;
		}
		else
		{
			return 0;
			break;
		}
		i++;
	}
}

uint8_t user_check(void)
{
	uint8_t check,i='0',trial=0;
	while(1)
	{
	user_info();
	check = user_compare_info();
	if(check == 1)
	{
		return 1;
		break;
	}
	else
	{
		if (i==user_count)
		{
			return 0;
			break;
		}
		
		user_counter+=4;
		i++;
	}
	}
	
}

void user_alarm_check(void)
{
	uint8_t counter=0,check;
	while(counter<3)
	{
		user_prog_init();
		check = user_check();
		if(check == 1)
		{
			UART_sendString(" \r");
			UART_sendString("welcome back... :) \r");
			UART_sendString(" \r");
			_delay_ms(1000);
			break;
		}
		else
		{
			UART_sendString("false \r");
			counter++;
			if (counter == 3)
			{
				UART_sendString("security mode... \r");
				GPIO_setupPinDirection(PORTA_ID,0,PIN_OUTPUT);
				UART_sendString("error :(");
				LCD_clearScreen();
				LCD_displayString("security mode...");
				while(1)
				{
					GPIO_writePin(PORTA_ID,0,LOGIC_HIGH);
				}
			}
		}
	}
}

void user_main_opt(void)
{
	uint8_t device;
	UART_sendString("**********choose the device********** \r");
	UART_sendString("1:air conditioner \r");
	UART_sendString("2:leds control\r");
	UART_sendString("3:dimmer circuit \r");
	UART_sendString("4:return to the first page \r");
	UART_sendString("************************************* \r");
	UART_sendString(" \r");
	device = UART_recieveByte();
	switch (device)
	{
		case '1':aircond_uart();break;
		case '2':choose_led_uart();break;
		case '3':dimmer_uart();break;
		case '4':smart_home();break;
	}
}

void interrupt_init(void)
{
	GPIO_setupPinDirection(PORTD_ID,2,PIN_INPUT);
	GPIO_writePin(PORTD_ID,2,LOGIC_HIGH);
	GICR |= (1<<7)|(1<<6)|(1<<5);
	MCUCR |= (1<<ISC00)|(1<<ISC01);
	sei();
}

ISR (INT0_vect)
{
	lcd_user_alarm_check();
	lcd_main_opt();
}

void lcd_program_init(void)
{
	uint8_t i,j,name,pass;
	LCD_clearScreen();
	LCD_displayString("hello...!");
	LCD_moveCursor(1,0);
	LCD_displayString("users= ");
	LCD_displayCharacter(user_count);
	_delay_ms(1000);
	LCD_clearScreen();
	LCD_displayString("enter user name");
	LCD_moveCursor(1,0);
	LCD_displayString("and password");
	_delay_ms(1000);
	LCD_clearScreen();
	LCD_displayString("username:");
	for (i=0;i<4;i++)
	{
		name = KEYPAD_getPressedKey();
		_delay_ms(200);
		LCD_displayCharacter(name);
		_delay_ms(200);
		lcd_user_name[i] = name;
	}
	LCD_moveCursor(1,0);
	LCD_displayString("password:");
	for (j=0;j<4;j++)
	{
		pass = KEYPAD_getPressedKey();
		_delay_ms(200);
		LCD_displayCharacter('*');
		_delay_ms(200);
		lcd_user_pass[j] = pass;
	}
}

void lcd_user_info(void)
{
	uint8_t i=0,j=0,k=0;
	while(j<4)
	{
		while(!EEPROM_readByte(user_name_address+lcd_user_counter+j,lcd_user_comp_name+j));
		j++;
		_delay_ms(10);
	}
	while (k<4)
	{
		while(!EEPROM_readByte(user_pass_address+lcd_user_counter+k,lcd_user_comp_pass+k));
		k++;
		_delay_ms(10);
	}
}

uint8_t lcd_user_compare_info(void)
{
	uint8_t i=0;
	while(i<4)
	{
		if ((lcd_user_name[i]==(lcd_user_comp_name[i]))&&(lcd_user_pass[i]==(lcd_user_comp_pass[i])))
		{
			return 1;
		}
		else
		{
			return 0;
			break;
		}
		i++;
	}
}

uint8_t lcd_user_check(void)
{
	uint8_t check,i='0',trial=0;
	while(1)
	{
		lcd_user_info();
		check = lcd_user_compare_info();
		if(check == 1)
		{
			return 1;
			break;
		}
		else
		{
			lcd_user_counter+=4;
			i++;
			if (i==user_count)
			{
				return 0;
				break;
			}
		}
	}
	
}

void lcd_user_alarm_check(void)
{
	uint8_t counter=0,check;
	while(counter<3)
	{
		lcd_program_init();
		check = lcd_user_check();
		if(check == 1)
		{
			LCD_clearScreen();
			LCD_displayString("welcome back :)");
			_delay_ms(1000);
			break;
		}
		else
		{
			LCD_clearScreen();
			LCD_moveCursor(0,0);
			LCD_displayString("false :(");
			_delay_ms(500);
			counter++;
			if (counter == 3)
			{
				LCD_clearScreen();
				LCD_displayString("security mode...");
				_delay_ms(1000);
				GPIO_setupPinDirection(PORTA_ID,0,PIN_OUTPUT);
				while(1)
				{
					GPIO_writePin(PORTA_ID,0,LOGIC_HIGH);
				}
			}
		}
	}
}

void lcd_main_opt(void)
{
	uint8_t device_num;
	LCD_clearScreen();
	LCD_displayString("press the device");
	LCD_moveCursor(1,0);
	LCD_displayString("number");
	_delay_ms(1000);
	LCD_clearScreen();
	LCD_displayString("1:A/c");
	LCD_moveCursor(0,8);
	LCD_displayString("2:leds");
	LCD_moveCursor(1,0);
	LCD_displayString("3:dimmer");
	LCD_moveCursor(1,8);
	LCD_displayString("4:uart");
	device_num = KEYPAD_getPressedKey();
	switch (device_num)
	{
		case '1':aircond_lcd();break;
		case '2':choose_led_lcd();break;
		case '3':dimmer_lcd();break;
		case '4':break;
	}
}

void aircond_lcd(void)
{
	uint8_t state;
	LCD_clearScreen();
	LCD_displayString("1:onA/C ");
	LCD_displayString("2:offA/C");
	LCD_moveCursor(1,0);
	LCD_displayString("3:Main");
	state = KEYPAD_getPressedKey();
	switch (state)
	{
		case '1':lcd_air_conditioner_on();break;
		case '2': lcd_air_conditioner_off();break;
		case '3':lcd_main_opt();break;
	}
}

lcd_air_conditioner_on(void)
{
	uint8_t temp,button=1;
	LCD_clearScreen();
	LCD_moveCursor(0,1);
	LCD_displayString("air conditioner");
	LCD_moveCursor(1,3);
	LCD_displayString("RUNING...");
	while (button == 1){
		button = GPIO_readPin(PORTB_ID,PIN0_ID);
		temp= LM35_getTemperature();
		if (temp >= 28)
		{
			DcMotor_Rotate(CW,50);
		}
		else if (temp < 28)
		{
			DcMotor_Rotate(Stop,0);
		}
	}
	DcMotor_Rotate(Stop,0);
	aircond_lcd();
}

lcd_air_conditioner_off(void)
{
	DcMotor_Rotate(Stop,0);
	LCD_clearScreen();
	LCD_moveCursor(0,1);
	LCD_displayString("air conditioner");
	LCD_moveCursor(1,3);
	LCD_displayString("closed...");
	_delay_ms(1000);
	aircond_lcd();
}

choose_led_lcd(void)
{
	uint8_t state,led;
	LCD_clearScreen();
	LCD_moveCursor(0,0);
	LCD_displayString("1:led_on/2:ledoff");
	LCD_moveCursor(1,0);
	LCD_displayString("3:main");
	state =  KEYPAD_getPressedKey();
	_delay_ms(200);
	switch(state)
	{
	case '1':
	LCD_clearScreen();
	LCD_displayString("open led");
	_delay_ms(200);
	led = KEYPAD_getPressedKey();
	open_led_lcd(led);
	break;
	
	case '2':
	LCD_clearScreen();
	LCD_displayString("close led");
	_delay_ms(200);
	led = KEYPAD_getPressedKey();
	close_led_lcd(led);
	break;
	case '3':lcd_main_opt();break;
	}
}

void open_led_lcd (uint8_t led_num)
{
	_delay_ms(200);
	switch (led_num)
	{
		case '1':LED_ON(PORTA_ID,LED1);break;
		case '2':LED_ON(PORTC_ID,LED2);break;
		case '3':LED_ON(PORTC_ID,LED3);break;
		case '4':LED_ON(PORTD_ID,LED4);break;
		case '5':LED_ON(PORTD_ID,LED5);break;

	}
	choose_led_lcd();
}

void close_led_lcd (uint8_t led_num)
{
	_delay_ms(200);
	switch (led_num)
	{
		case '1':LED_OFF(PORTA_ID,LED1);break;
		case '2':LED_OFF(PORTC_ID,LED2);break;
		case '3':LED_OFF(PORTC_ID,LED3);break;
		case '4':LED_OFF(PORTD_ID,LED4);break;
		case '5':LED_OFF(PORTD_ID,LED5);break;

	}
	choose_led_lcd();
}

void dimmer_lcd(void)
{
	uint8_t percentage;
	LCD_clearScreen();
	LCD_moveCursor(0,0);
	LCD_displayString("choose lamp");
	LCD_moveCursor(1,0);
	LCD_displayString("percentage");
	_delay_ms(1000);
	LCD_clearScreen();
	LCD_moveCursor(0,0);
	LCD_displayString("1:0%");
	LCD_moveCursor(0,5);
	LCD_displayString("2:25%");
	LCD_moveCursor(0,11);
	LCD_displayString("3:50%");
	LCD_moveCursor(1,2);
	LCD_displayString("4:75%");
	LCD_moveCursor(1,8);
	LCD_displayString("5:100%");
	percentage = KEYPAD_getPressedKey();
	switch (percentage)
	{
		case '1':TIMER0_INIT(0);break;
		case '2':TIMER0_INIT(25);break;
		case '3':TIMER0_INIT(50);break;
		case '4':TIMER0_INIT(75);break;
		case '5':TIMER0_INIT(100);break;
	}
	lcd_main_opt();
}

void door_control(void)
{
	uint8_t state;
	DDRD |= (1<<PD5);	/* Make OC1A pin as output */
	TCNT1 = 0;			/* Set timer1 count zero */
	ICR1 = 3124;		/* Set TOP count for timer1 in ICR1 register */

	/* Set Fast PWM, TOP in ICR1, Clear OC1A on compare match, clk/64 */
	TCCR1A = (1<<WGM11)|(1<<COM1A1);
	TCCR1B = (1<<WGM12)|(1<<WGM13)|(1<<CS10)|(1<<CS11);
	UART_sendString("1:open the door \r");
	UART_sendString("2:close the door \r");
	UART_sendString("3:Main \r");
	LCD_clearScreen();
	LCD_moveCursor(0,1);
	LCD_displayString("door control");
	while(1)
	{
		state=UART_recieveByte();
		switch (state)
		{
			case '1':
			LCD_clearScreen();
			LCD_moveCursor(0,6);
			LCD_displayString("door");
			LCD_moveCursor(1,3);
			LCD_displayString("is opening...");
			OCR1A = 37.5;	/* Set servo at +90° position */
			_delay_ms(1500);
			break;
			case '2':
			LCD_clearScreen();
			LCD_moveCursor(0,6);
			LCD_displayString("door");
			LCD_moveCursor(1,3);
			LCD_displayString("is closing...");
			OCR1A = 23.4375;	/* Set servo shaft at 0° position */
			_delay_ms(1500);
			break;
			case '3':main_opt();break;
		}
	}
}