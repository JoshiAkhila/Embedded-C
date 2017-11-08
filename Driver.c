// Driver.c
// This software configures the driver input
// Runs on LM4F120 or TM4C123
// Program written by: Akhila Joshi
// Date Created: 11/22/15
// Last Modified: 12/01/15 
// Lab number: 6
// Hardware connections PE4-PE0 are connected to L293 motor driver


// connect the rest of the L293 Driver PINs as follows:
// PINs 4,5,12,13 Ground
// PIN 16 to 5V from your microcontroller
// PIN 8 outside voltage source for the motor. DON'T CONNECT THIS PIN TO YOUR MICROCONTROLLER
// DON'T SUPPLEY MORE THAN 5 V from the outside, make sure to use the same common ground
// PIN 3 RED wire on the Stepper motor
// PIN 6 BLUE wire on the stepper motor
// PIN 11 GREEN wire on the stepper motor
// PIN 14 BLACK wire on the stepper motor

// Define Ports registers locations here

#include "tm4c123gh6pm.h"
#include "Driver.h"

// **************Driver_Init*********************
// Initialize control driver, called once 
// Input: none 
// Output: none

 void Driver_Init(void){
	 volatile unsigned long delay;        // Initializing variable delay 
	 // initialize output driver pins 
	SYSCTL_RCGC2_R |= 0x00000010;         // 1) activate clock for Port E
  delay = SYSCTL_RCGC2_R;               //    allow time for clock to start
	GPIO_PORTE_DIR_R |= 0x1F;             // 2) PE4-PE1 used as out for motion sequence 
	GPIO_PORTE_AFSEL_R = 0x00;            // 3) Enable alt funct on PE2
  GPIO_PORTE_DEN_R |= 0XFF;             // 4) Disable digital I/o on PE2
  GPIO_PORTE_AMSEL_R =0X00;             // 5) Enable analog function on PE2
}

