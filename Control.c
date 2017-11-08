// Control.c
// This software configures the off-board control switches
// Runs on LM4F120 or TM4C123
// Program written by: Akhila Joshi
// Date Created: 11/22/15
// Last Modified: 12/01/15 
// Lab number: 6
// Hardware connections PB4-PB2 are connected to off-board switches
// PB4 is used to turn the motor on/off, if PB2 is 1 the motor will be on waiting for commands
// PB3 makes the motor turn clockwise
// PB2 makes the motor turn counter clockwise
// PB4 tirggers edge-trigger interrupts at both edges


// defing registers here
#include "tm4c123gh6pm.h"
#include "Control.h"

unsigned short delay;
void EnableInterrupts(void);  // Enable interrupts
void DisableInterrupts(void); // Disable interrupts

// **************Control_Init*********************
// Initialize control switches inputs, called once 
// Input: none 
// Output: none

void Control_Init(void){
volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x00000002;             // 1) activate clock for Port B
  delay = SYSCTL_RCGC2_R;                   //    allow time for clock to start
	GPIO_PORTB_DIR_R &= ~0x1C;                // 2) Clear PB2,PB3 and PB4 as In pins as switch 
	GPIO_PORTB_AFSEL_R = 0x00;                // 3) disable alt funct on PB7-0
  GPIO_PORTB_DEN_R = 0xFF;                	// 4) enable digital I/O on PB7-0
	GPIO_PORTB_PCTL_R = 0x00;                 // 5) Regular GPIO
  GPIO_PORTB_AMSEL_R = 0x00;                // 6) Disabling analog mode
	GPIO_PORTB_IS_R &= ~0x10;                 // 7) Edge sensitive
  GPIO_PORTB_IBE_R |= 0x10;                 // 8) For both edges
  GPIO_PORTB_IM_R  |= 0x10;                 // 9) Arm interrupt
	NVIC_PRI0_R=(NVIC_PRI0_R & 0X00FFFFFF)|0x00004000;   // 10) Setting highest priority 
	NVIC_EN0_R=0X00000002;                     // 11)  Setting interrupt for portB  
	EnableInterrupts();                        // 12) Enabling global interrupt
}	 

// **************Control_In*********************
// Input from switches  
// Input: none 
// Output: 0 to 7 depending on keys
// PB4 is on/off, PB3 CW, PB2 CCW
// 

unsigned long Control_In(void){                // Reading input sswitches
	unsigned long data;                          // Initializing veriale data to read input value
	data = GPIO_PORTB_DATA_R;                    // Reading switch value from PortB
 	return (data & 0X1C);                        // Return to the data value
 }

