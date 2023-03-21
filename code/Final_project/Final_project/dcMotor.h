 /******************************************************************************
 *
 * Module: DC_Motor
 *
 * File Name: dcMotor.h
 *
 * Description: Header file for the DC Motor driver
 *
 * Author: Aly Tarek
 *
 *******************************************************************************/

#ifndef DCMOTOR_H_
#define DCMOTOR_H_

#include "gpio.h"
#include "common_macros.h"
#include <stdint.h>
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define DC_MOTOR_PORT_ID PORTD_ID
#define DC_MOTOR_PIN1_ID PIN6_ID
#define DC_MOTOR_PIN2_ID PIN7_ID


#define LOGIC_HIGH        (1u)
#define LOGIC_LOW         (0u)

/*******************************************************************************
 *                                Types                                 *
 *******************************************************************************/

typedef enum {
	Stop,CW,A_CW
}DcMotor_State;

/*******************************************************************************
 *                                Prototypes                                 *
 *******************************************************************************/
/*
 * Description:
 * Function to Initialize and Configure the DC Motor
 */
void DcMotor_Init(void);


/*
 * Description:
 * Function To Rotate the DC Motor and Control it's Speed
 */
void DcMotor_Rotate(DcMotor_State state,uint8_t speed);



#endif /* DCMOTOR_H_ */
