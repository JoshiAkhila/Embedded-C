// Stepper.c
// Runs on LM4F120 or TM4C123
// Index implementation of a Moore finite state machine to operate
// A Stepper Motor
// Your Name: Akhila Joshi
// created: 11/22/2015
// last modified:12/01/2015 
// In this lab we are using FSM to rotate motor in different directions.
// If PB4 and PB3 are pressed motor will rotate in clockwise direction.
// If PB4 and PB2 are pressed motor will rotate in counter clockwise direction.
// If PB4, PB3 and PB2 all are pressed motor will rotate in half clockwise direction.

// define your registers here
#include "tm4c123gh6pm.h"
#include "PLL.h"
#include "SysTick.h"
#include "Control.h"
#include "Driver.h" 
#include "Nokia5110.h"

// Defining veriables
unsigned long input;
unsigned long data;
unsigned long check;
unsigned long count;

// Defining strings for LCD display
char name[25]="TURN CW";                  // String to display clockwise rotation. 
char name1[25]="TURN CCW";                // String to display counter clockwise rotation.
char name2[25]="TURN HALF CW";            // String to display half clockwise rotation
char name3[10]="STOPED";                  // String to display stop condition


struct State  {
// define your structure here
	    unsigned long out;                         // Pointing to motor output
	    unsigned long time;                        // Pointing to speed value
      const struct State *Next[4];               // Size of the next array determined by the input	
};
typedef const struct State STyp;

unsigned long CW =0;                            // Initializing clockwise value as zero 
unsigned long CCW =0;                           // Initializing counterclock value as zero
unsigned long halfCW =0;                        // Initializing half clockwise rotation value as zero

//Declaring states here

STyp FSM[13] = {   // 00      01      10      11
       {0x00,1,  {&FSM[0],&FSM[9],&FSM[1],&FSM[5]}},      // S[0]
       {0x0B,1,  {&FSM[2],&FSM[2],&FSM[2],&FSM[2]}},      // S[1] // CW (Clockwise)
       {0x0D,1,  {&FSM[3],&FSM[3],&FSM[3],&FSM[3]}},      // S[2]
       {0x15,1,  {&FSM[4],&FSM[4],&FSM[4],&FSM[4]}},      // S[3]
       {0x13,1,  {&FSM[0],&FSM[0],&FSM[0],&FSM[0]}},      // S[4]
       {0x0B,6,  {&FSM[6],&FSM[6],&FSM[6],&FSM[6]}},      // S[5] //  HalfCW(half rotate clock)
       {0x0D,6,  {&FSM[7],&FSM[7],&FSM[7],&FSM[7]}},      // S[6]
       {0x15,6,  {&FSM[8],&FSM[8],&FSM[8],&FSM[8]}},      // S[7]
			 {0x13,6,  {&FSM[0],&FSM[0],&FSM[0],&FSM[0]}},      // S[8]
			 {0x13,1,  {&FSM[10],&FSM[10],&FSM[10],&FSM[10]}}, 	// S[9] // CCW (counter clockwise)
			 {0x15,1,  {&FSM[11],&FSM[11],&FSM[11],&FSM[11]}},  // S[10]
			 {0x0D,1,  {&FSM[12],&FSM[12],&FSM[12],&FSM[12]}},  // S[11]
			 {0x0B,1,  {&FSM[0],&FSM[0],&FSM[0],&FSM[0]}}       // S[12]
			 
 };
	 
#define stepper (*((volatile unsigned long *)0x400243FC))   // Intializing DATA register for portE

 // move 1.8 degree clockwise
unsigned long stepper_CW (void)
	{
  data=Control_In();            // Read input from switch
    if(data==0x18)              // Check for SW4 and Sw3 are pressed
		{
			return(0x02);             // Rotate Clockwise
		}
		else if(data==0x14)         // Check for SW4 and Sw2 are pressed
		{
			return(0x01);             // Rotate Counter Clockwise
		}
	  else if(data==0x1C)         // Check for SW4,Sw3 and SW2 are pressed
		{
			return(0x03);             // Rotate half Clockwise
		}
		else
		{
		return(0x00);               // Stop rotation
		}
	}

	void GPIOPortB_Handler(void)
	{
		GPIO_PORTB_ICR_R=0x10;      // Acknowledging PortB
		check=1;                    // Set flag
  }
	
	int i;  	
  int main(void){ volatile unsigned long delay;
	 
	STyp *Pt;                // Pointing to FSM
	PLL_Init();              // 80 MHz, bus frequency
  SysTick_Init();          // Initializing Systick 
  Control_Init();          // Initializing PortB for input 
	Driver_Init();           // Initializing PortE for output(motor)
  Nokia5110_Init();        // Initializing Nokia5110
	                        
	Pt= &FSM[0];             // Initial state
	 
  // turn stepper motor to required position
	  
  while(1){
  //your code goes here
	    if(check==1)
				{
					input= stepper_CW();   // Read output values
					  if(input==0x02)      
						  {
                stepper = Pt->out;            // Rotate in clockwise
				        SysTick_Wait1ms(Pt->time);    // providing wait time
                input= stepper_CW();          // check for output
	              Pt=Pt->Next[input];           // Point to the next state
								Nokia5110_Clear();            // Clearing the LCD
	              Nokia5110_SetCursor(1,0);     // To Set the cursor 	      
		            Nokia5110_OutString(name);    // Displaying rotation direction as CW 		 
	            }
				    else if(input==0x01)
						  {
                stepper = Pt->out;            // Rotate in counter clockwise
				        SysTick_Wait1ms(Pt->time);    // providing wait time
				        input= stepper_CW();          // check for output
	              Pt=Pt->Next[input];           // Point to the next state
                Nokia5110_Clear();            // Clearing the LCD
	              Nokia5110_SetCursor(1,0);     // To Set the cursor 	      
		     		    Nokia5110_OutString(name1);		// Displaying rotation direction as CCW						
				      }
					  else if(input==0x03)
						  {
                 stepper = Pt->out;                  // Steper motor output for half clockwise
	                
 								
                     while(count<99)								// Check 100 steps half spin 
	                   	{			
	                      SysTick_Wait1ms(Pt->time);   // providing wait time
	                      input= stepper_CW();         // Check for output
	                      Pt=Pt->Next[input];          // Point to the next state 
	                      count++;                   // Increament counter
                       	Nokia5110_Clear();           // Clearing the LCD
	                      Nokia5110_SetCursor(1,0);    // To Set the cursor
	                      Nokia5110_OutString(name2);  // Displaying rotation direction as halfCW	
	                      //Nokia5110_SetCursor(1,2);    // To Set the cursor 	      
	                      //Nokia5110_OutString(name3);	 // Displaying rotation direction as stop	
		                  }  
											count=0;
											stepper=count;
							}
	                                      // Set Conter value as zero
				     	
						else
						  {
							  	stepper= 0x00;                // Stop rotation
						      Nokia5110_Clear();            // Clearing the LCD
	                Nokia5110_SetCursor(1,0);     // To Set the cursor
	                Nokia5110_OutString(name3);   // Displaying rotation direction as stop	
						  }
          }
       }
}
 
