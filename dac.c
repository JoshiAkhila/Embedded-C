// dac.c
// This software configures DAC output
// Runs on TM4C123
// Program written by: Akhila Joshi
// Date Created: 10/22/2015
// Last Modified: 10/25/2015
// Lab number: 4

// For DAC output we are using PortE.
// In portE we are using PE4-PE0 as DAC out.
// For bit0 we are using PE0 port.
// For bit1 we are using PE1 port.
// For bit2 we are using PE2 port.
// For bit3 we are using PE3 port.
// For bit4 we are using PE4 port.
// Here we are directely moving output data into portE ports.
// Hardware connections

#include "tm4c123gh6pm.h"
#include "dac.h"

// put code definitions for the software (actual C code)
// this file explains how the module works

// **************DAC_Init*********************
// Initialize 5-bit DAC, called once 
// Input: none
// Output: none

void DAC_Init(void){ volatile unsigned long delay;   
  SYSCTL_RCGC2_R |= 0x00000010;            // 1) activate clock for Port E
  delay = SYSCTL_RCGC2_R;                  //    allow time for clock to start
	GPIO_PORTE_DIR_R |= 0x1F;                // 2) Set PE0,PE1,PE2,PE3 and PE4 as out pins for main street light
  GPIO_PORTE_AFSEL_R = 0x00;               // 3) disable alt funct on PF7-0
  GPIO_PORTE_DEN_R = 0xFF;                 // 4) enable digital I/O on PE7-0
}

// **************DAC_Out*********************
// output to DAC
// Input: 5-bit data, 0 to 31 
// Output: none

void DAC_Out(unsigned long data)
	{
		GPIO_PORTE_DATA_R = data;              // Moving data to portE for Dac out.
  }
