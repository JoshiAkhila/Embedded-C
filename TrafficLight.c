// TrafficLight.c
// Runs on LM4F120 or TM4C123
// Index implementation of a Moore finite state machine to operate
// a traffic light.
// Your Name: Akhila Joshi
// created: 
// last modified by Akhila Joshi: 10/14/15

// In this program we are using Clock register,Lock Registers, CR registers, AMSEL registers,DEN( digital enable),
// Registers,Data registers and AFSEL registers.
// In this lab we are using PortE, PortB and PortF ports.
// First we are initializing all the ports.
// PortE pins were used for input as well as for outputs.
// PE5 to PE3 used for main street trafic light. PE5, PE4 and PE3 used for red, yellow and green respectively.
// PE0 and PE1 were used for walk request on main street.
// PortB pins were also used for input as well outputs.
// PE5 to PE3 used for main street trafic light. PB5, PB4 and PB3 used for red, yellow and green respectively.
// PB0 were used for sensor request input on side street.
// PB2 were used for walk output.
// PortF was used for side street walk input.
// PF1 and PF0 were used for side street walk inputs.
// Here if any of the 4 walk button is pressed, both street trafic lights will be Red for 4sec.
// At that time walk light should be lit.
// The side street sensor will indicate if there are cars waiting on the side street.
// If there are cars waiting on the side street, the traffic light must alternate between Green on the side street and green on the main street.
// Here we are enforcing a wait time of 3sec for the red and green lights and 1sec for the yellow lights.
// If any changes from green to red have to go through the yellow state.
// Here we are providing more priority to main. 
// If there are no cars waiting on the side street, main street has the high priority and it will stay green.
// So here we comparing the sensor value(0x01) with walk value(w1,w2,w3,w4), if both the conditions are true then walk light shoul lit.
// Else it should read the value according sensors.
// When sensor is not active(0x10) and walk button is pressed then walk should read.
// Else it should remain in the initial state(GOM). Since GOM has highest priority.
// Here we are using systick clock to provide delay and set reload values.

/* This example accompanies the book
   "Embedded Systems: Introduction to ARM Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2013
   Volume 1 Program 6.8, Example 6.4
   "Embedded Systems: Real Time Interfacing to ARM Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2013
   Volume 2 Program 3.1, Example 3.1

 Copyright 2013 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */


#include "PLL.h"
#include "SysTick.h"
#define GPIO_PORTB_DATA_R       (*((volatile unsigned long *)0x400053FC)) // 
#define GPIO_PORTB_DIR_R        (*((volatile unsigned long *)0x40005400))
#define GPIO_PORTB_AFSEL_R      (*((volatile unsigned long *)0x40005420))
#define GPIO_PORTB_DEN_R        (*((volatile unsigned long *)0x4000551C))
#define GPIO_PORTB_AMSEL_R      (*((volatile unsigned long *)0x40005528))
#define GPIO_PORTB_PCTL_R       (*((volatile unsigned long *)0x4000552C))

#define GPIO_PORTE_DATA_R       (*((volatile unsigned long *)0x400243FC)) // 
#define GPIO_PORTE_DIR_R        (*((volatile unsigned long *)0x40024400))
#define GPIO_PORTE_AFSEL_R      (*((volatile unsigned long *)0x40024420))
#define GPIO_PORTE_DEN_R        (*((volatile unsigned long *)0x4002451C))
#define GPIO_PORTE_AMSEL_R      (*((volatile unsigned long *)0x40024528))
#define GPIO_PORTE_PCTL_R       (*((volatile unsigned long *)0x4002452C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC2_GPIOE      0x00000010  // port E Clock Gating Control
#define SYSCTL_RCGC2_GPIOB      0x00000002  // port B Clock Gating Control
// add any needed port definitions 
#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_LOCK_R       (*((volatile unsigned long *)0x40025520))
#define GPIO_PORTF_CR_R         (*((volatile unsigned long *)0x40025524))
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C))

// Linked data structure

struct State  {
// define your structure here
	
      unsigned long outM;                   // main street output
      unsigned long outS;                 	// Side street output
      unsigned long Time;                   // Delay 
 const struct State *Next[4];               // Size of the next array determined by the input	
};
typedef const struct State STyp;
//define your states here e.g. #define stateName 0, etc.

#define GOM &FSM[0]                          
#define GOS &FSM[1]
#define WaitM &FSM[2]
#define WaitS &FSM[3]
#define Walk &FSM[4]

volatile unsigned long input;
//Declare your states here 

  STyp FSM[5] = { // 00  01    10    11
 {0x08,0x20,300,   {GOM,WaitM,WaitM,WaitM}},              // Next states for GOM state
 {0x20,0x08,300,   {WaitS,WaitS,WaitS,WaitS}},            // Next states for GOS state
 {0x10,0x20,100,   {GOS,GOS,Walk,Walk}},                  // Next states for WaitM state
 {0x20,0x10,100,   {GOM,GOM,Walk,GOM}},                   // Next states for WaitS state
 {0x20,0x24,400,   {GOM,GOS,GOM,GOS}}                     // Next states for walk state
 };
	
void PortF_Init(void){ volatile unsigned long delay;   // Initializing PortF
  
	SYSCTL_RCGC2_R |= 0x00000020;            // 1) activate clock for Port F
  delay = SYSCTL_RCGC2_R;                  //    allow time for clock to start
  GPIO_PORTF_LOCK_R = 0x4C4F434B;          // 2) unlock GPIO Port F
  GPIO_PORTF_CR_R = 0x1F;                  //    allow changes to PF4-0
  // only PF0 needs to be unlocked, other bits can't be locked
  GPIO_PORTF_AMSEL_R = 0x00;               // 3) disable analog on PF
  GPIO_PORTF_PCTL_R = 0x00000000;          // 4) PCTL GPIO on PF4-0
  GPIO_PORTF_DIR_R &= ~0x03;               // 5) PF1,PF0 inputs for walk request on side street
  GPIO_PORTF_AFSEL_R = 0x00;               // 6) disable alt funct on PF7-0
  //  GPIO_PORTF_PUR_R = 0x11;             //    enable pull-up on PF0 and PF4
  GPIO_PORTF_DEN_R = 0x1F;                 // 7) enable digital I/O on PF4-0 
}

void PortE_Init(void){ volatile unsigned long delay;   // Initializing PortE
	
	SYSCTL_RCGC2_R |= 0x00000010;            // 1) activate clock for Port E
  delay = SYSCTL_RCGC2_R;                  //    allow time for clock to start
	GPIO_PORTE_DIR_R |= 0x38;                // 2) Set PE5,PE4 and PE3 as out pins for main street light
  GPIO_PORTE_DIR_R &= ~0x03;               // 3) Clear PE1 and PE0 as In pins for walk request on main street 
	GPIO_PORTE_AFSEL_R = 0x00;               // 4) disable alt funct on PF7-0
  GPIO_PORTE_DEN_R = 0xFF;                 // 5) enable digital I/O on PE7-0
}

void PortB_Init(void){volatile unsigned long delay;    // Initializing PortB

  SYSCTL_RCGC2_R |= 0x00000002;            // 1) activate clock for Port E
  delay = SYSCTL_RCGC2_R;                  //    allow time for clock to start
  GPIO_PORTB_DIR_R |= 0X3C;                // 2) Set PB5,PB4,PB3 and PB2 as out pins for side street light and sensor light respectively
	GPIO_PORTB_DIR_R &= ~0X01;               // 3) Clearing PB0 pin to request walk
	GPIO_PORTB_AFSEL_R = 0x00;               // 4) disable alt funct on PF7-0
  GPIO_PORTB_DEN_R = 0xFF;                 // 5) enable digital I/O on PB7-0
}

  unsigned long readsensors (void)         // Read values for inputs 
{

	 unsigned long  PE,PF,PB;
		 PE = GPIO_PORTE_DATA_R & 0X03;        // Read walk request for main
	   PF = GPIO_PORTF_DATA_R & 0X03;        // Read walk request for side
     PB = GPIO_PORTB_DATA_R & 0X01;        // Read sensor
   
	 if(PB==0x01)                            // If sensor is active (Switch is pressed)
	  { 
		  if (PE==0X01 || PE==0X02 || PE==0X03|| PF==0X01 || PF == 0X02 || PF==0X03) // Checking for walk button(W1 ,W2, W3, W4)Press
				{
				return(0x03);                      // Read input states from 11 condition 
	      }
      else 
		    {
			  return(0x01);		                   // Read input states from 01 condition
	      }
	  }
		  else if (PB==0x00)                   // Sensor is not active (Switch is not pressed)
			  { 
		     if (PE==0X01 || PE==0X02 || PE==0X03|| PF==0X01 || PF == 0X02 || PF==0X03) // Checking for walk button(W1 ,W2, W3, W4)Press
				   {
				   return(0x02);                   // Read input states from 10 condition
	         }
         else
		     {
			   return(0x00);		                 // Read input states from 00 condition
	       }
	      }
  else 
		{
		return(0x00);                         // Read input states from 00 condition
		}
 }

 int main(void)
	 {
	volatile unsigned long delay;
	
	STyp *Pt;                               //  Pointer to STyp
  PLL_Init();                             // 80 MHz, Program 10.1
  SysTick_Init();                         // Program 10.2
  SYSCTL_RCGC2_R |= 0x12;                 // 1) B E
  delay = SYSCTL_RCGC2_R;                 // 2) no need to unlock
  
	// initialize PortB and PortE here. 
	    PortF_Init();                       // Initializing PortF for side street input
	    PortE_Init();                       // Initializing PortE for main street input and output
	    PortB_Init();                       // Initializing PortB for side street output, sensor input and sensor output 
	
	    Pt = GOM;                           // Initial state
	  
  while(1)
		{
		 //your code goes here to move from one state to another. 
			
	 GPIO_PORTE_DATA_R = Pt -> outM;        // Writing the outputs to data register as it is connected to output LEDs
   GPIO_PORTB_DATA_R = Pt -> outS;        // Writing the outputs to data register 
      
	 SysTick_Wait10ms(Pt-> Time);     
   input=readsensors();                   // Taking inputs
   Pt=Pt->Next[input];		                // pointer is pointing to next state depending on the input provided
	  }
}
 
