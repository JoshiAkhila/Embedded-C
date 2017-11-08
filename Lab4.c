// Lab4.c
// Runs on TM4C123
// Program written by: Akhila Joshi
// Date Created: 10/22/2015 
// Last Modified: 10/25/2015 
// Lab number: 4

// Brief description of the program
// In this project we are converting digital signals to analog by building a DAC circuit. 
// And we are using digital signal to represents sound and music.
// Here we are using 4 different switches as  piano keys.
// For Piano keys we are uisng PortA.
// In PortA we are using PA2-PA5.
// For DAC output we are using PortE.
// In PortE we are PE0-PE4 for DAC output.
// In this program we are using DATA, DEN, AFSEL, DIR and PCTL registors.
// We are using SysTick handler and interrupt.
// Hardware connections


#include "tm4c123gh6pm.h"
#include "PLL.h"
#include "Sound.h"
#include "Piano.h"
#include "dac.h"

// basic functions defined at end of startup.s
void DisableInterrupts(void);             // Disable interrupts
void EnableInterrupts(void);              // Enable interrupts

int main(void){                          
	unsigned long input;                    // Initializing input variable
//unsigned long Data;	
 PLL_Init();                              // bus clock at 80 MHz
// all initializations go here
	
	DAC_Init();                             // Initializing DAC output
	Sound_Init();                           // Initializing sound wave
	Piano_Init();                           // Initializing Piano input
  
  while(1)
		  { 
// main loop, read from switchs change sounds	
				
				input= Piano_In();                // Taking inputs
        if(input==0x00)                   // if none of the input pressed
				{
					DisableInterrupts();            // Disabling interrupt
				}
				else
				{
					EnableInterrupts();            // enabling interrupt
					Sound_Play(input);             // calling Sound play for different frequency
				}		
				//  DAC_Out(Data);
		    //Data = 0x1F &(Data+1);
      }             
   }


