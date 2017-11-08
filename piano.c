// Piano.c
// This software configures the off-board piano keys
// Runs on TM4C123
// Program written by: Akhila Joshi
// Date Created: 10/21/2015
// Last Modified: 10/25/2015
// In this part we are writting code for piano keys. 
// For that we are using portA.
// In PortA we are using PA2-PA5 bits.
// PA5 pin was used for SW3.
// PA4 pin was used for SW2.
// PA3 pin was used for SW1.
// PA2 pin was used for SW0.
// Lab number: 4
// Hardware connections

#include "tm4c123gh6pm.h"
#include "piano.h"
// put code definitions for the software (actual C code)
// this file explains how the module works

// **************Piano_Init*********************
// Initialize piano key inputs, called once 
// Input: none 
// Output: none
void Piano_Init(void){ volatile unsigned long delay;  
	
	SYSCTL_RCGC2_R |= 0x00000001;             // 1) activate clock for Port A
  delay = SYSCTL_RCGC2_R;                   //    allow time for clock to start
	GPIO_PORTA_DIR_R &= ~0x3C;                // 2) Clear PA5,PA4,PA3 and PA2 as In pins for walk request on main street 
	GPIO_PORTA_AFSEL_R = 0x00;                // 3) disable alt funct on PF7-0
  GPIO_PORTA_DEN_R = 0xFF;                	// 4) enable digital I/O on PE7-0
	GPIO_PORTA_PCTL_R = 0x00;                 // 5) Regular GPIO
  GPIO_PORTA_IS_R &= ~0x3C;                 // 6) Edge sensitive
  GPIO_PORTA_IBE_R &= ~0x3C;                // 7) For both edges
  GPIO_PORTA_IEV_R |= 0x3C;                 // 8) For falling edge event
  GPIO_PORTA_IM_R  |= 0x3C;                 // 9) Arm interrupt
	GPIO_PORTA_DIR_R |= 0x40;                 // 10) Setting PA6 for toggling
}
// **************Piano_In*********************
// Input from piano key inputs 
// Input: none 
// Output: 0 to 15 depending on keys
// 0x01 is just Key0, 0x02 is just Key1, 0x04 is just Key2 and 0x08 is just key3

unsigned long Piano_In(void){ 
  int input_switch;                         // Initializing Switches for piano sw3,sw2,sw1,sw0
	input_switch = GPIO_PORTA_DATA_R;         // Read input from data register
   return (input_switch & 0x3c);            // replace this line with actual code
}
