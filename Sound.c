// Sound.c, 
// This module contains the SysTick ISR that plays sound
// Runs on LM4F120 or TM4C123
// Program written by: Akhila Joshi
// Date Created: 10/21/2015 
// Last Modified: 10/25/2015 

// In this section we are writting program for sound wave.
// First we are creating 64 different voltage points to get a clearer sine waveform.
// Here we are comparing the input values depending on that we are loading different reload value.
// Reload values were determined using note frequency and bus frequency.
// In SysTeick handler we are initializing sine variable and starting wave from zero.
// Comparing sine value with wave size if it is more then 63, wave should reaches to zero.
// If it is not then increment sine.
// After that we are calling Dac output and toggling the output bit.
// For toggling we are using PortA.
// in PortA we are using PA6 bit.
// For this we are setting PA6 bit.

// Lab number: 4
// Hardware connections
// Initializing SysTick timer.

#include "tm4c123gh6pm.h"
#include "dac.h"
#include "Sound.h"

// put code definitions for the software (actual C code)
// this file explains how the module works

const unsigned char sinewave[64]= {          // Creating 64 different voltage points to get a clearer sine waveform
0x8,0x8,0x9,0xa,0xa,0xb,0xc,0xc,
0xd,0xd,0xe,0xe,0xe,0xf,0xf,0xf,
0xf,0xf,0xf,0xf,0xe,0xe,0xe,0xd,
0xd,0xc,0xc,0xb,0xa,0xa,0x9,0x8,
0x8,0x7,0x6,0x5,0x5,0x4,0x3,0x3,
0x2,0x2,0x1,0x1,0x1,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x1,0x1,0x1,0x2,
0x2,0x3,0x3,0x4,0x5,0x5,0x6,0x7,
};
// FUNCTION DECLARATION
void DisableInterrupts(void);             // Disable interrupts
void EnableInterrupts(void);              // Enable interrupts

// **************Sound_Init*********************
// Initialize Systick periodic interrupts
// Input: Initial interrupt period
//           Units to be determined by YOU
//           Maximum to be determined by YOU
//           Minimum to be determined by YOU
// Output: none
void Sound_Init(void){
	unsigned long period;                 // Initialize period variable 
	DAC_Init();                           // Initialize DAC out
	                                      // Initialize SisTick timer 
 
	NVIC_ST_CTRL_R=0;                     // Disabling SysTick during setup (clear counter) 
	NVIC_ST_RELOAD_R=period-1;            // Specifing reload value ( number of counts)
	NVIC_ST_CURRENT_R=0;                  // values writ
	NVIC_SYS_PRI3_R =(NVIC_SYS_PRI3_R & 0X00FFFFFF)|0x20000000;  // Setting highest priority   
	NVIC_ST_CTRL_R=0X07;                  // setting counter and interrupt
//	DisableInterrupts();                  // Disabling interrupt 
}

// **************Sound_Play*********************
// Start sound output, and set Systick interrupt period 
// Input: interrupt period
//           Units to be determined by YOU
//           Maximum to be determined by YOU
//           Minimum to be determined by YOU
//           input of zero disables sound output
// Output: none

void Sound_Play(unsigned long period){
	
	if(period==0x20)                     // If sw3 on PA5 is pressed
 NVIC_ST_RELOAD_R= 2841;   		         // Period by pressing sw3. For frequency 440HZ
	
	if(period==0x10)                     // If sw2 on PA4 is pressed
 NVIC_ST_RELOAD_R= 2530;   		         // Period by pressing sw2. For frequency 494HZ
	
	if(period==0x08)                     // If sw1 on PA3 is pressed
 NVIC_ST_RELOAD_R= 4770;   		         // Period by pressing sw1. For frequency 262HZ
	
	if(period==0x04)                     // If sw0 on PA2 is pressed
 NVIC_ST_RELOAD_R= 3582;   		         // Period by pressing sw0. For frequency 349HZ
 EnableInterrupts();
}

// Interrupt service routine
// Executed periodically, the actual period
// determined by the current Reload.
unsigned long sine=0;                  // Initializing  sine variable
void SysTick_Handler(void)
	{
		//GPIO_PORTA_DATA_R ^= 0X40;         // Toggling PA6 to indecate heart beat. 
if(sine >= 63)
  { 
	sine=0;                              // To get sine wave, if sine reaches to 64 then make it zero

   }
else
	 {
		sine++;                            // else keep on increamenting the sine 
	 }
DAC_Out(sinewave[sine]);               // DAC output for sine wave
 GPIO_PORTA_DATA_R ^= 0X40;            // Toggling PA6 to indecate heart beat.
	}
