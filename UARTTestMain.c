// UARTTestMain.c
// Runs on TM4C123
// UART Lab 7 
// Name: Akhila Joshi
// Last Modified: 12/06/2015

// Insert description of the program here.



#include "PLL.h"
#include "UART.h"
#include "tm4c123gh6pm.h"
#include "PortF.h"
#include "SysTick.h"
#include "Nokia5110.h"

char name[5]="BLUE";                  // String to display blue 
char name1[5]="RED";                  // String to display red
char name2[15]="NO DATA TO SEND";     // String to display no data to send

// Defining different variable
 unsigned long SW;    
 unsigned long LED; 
 unsigned long data;
 unsigned char output;
 
int main(void){
 // initialization function calls 
	
	PLL_Init();
	Nokia5110_Init();
	PortF_Init();
  UART_Init();	
  SysTick_Init();
  
	
  while(1){
		// insert your code here
		 
		SW   = (GPIO_PORTF_DATA_R & 0x11);        // Reading inputs from portF
	  data = UART_InChar_NB();                  // Checking for receiving data
		
  if(SW==0X01)                                // Checking for Switch1
		{
		  GPIO_PORTF_DATA_R = 0x04;               // Toggling Blue LED
		  UART_OutChar('b');                      // Transffering b data
		}	
  else if(SW==0x10)                           // Checking for Switch2
	  {
			GPIO_PORTF_DATA_R= 0x02;                // Toggling RED LED
		  UART_OutChar('r');
	  }
  else if(data=='b')                          // Checking for received data
		    {
			    Nokia5110_Clear();                  // Clearing the LCD
	        Nokia5110_SetCursor(1,0);           // To Set the cursor 
		      Nokia5110_OutString(name);          // Displaying recieved data on LCD
		    } 
	else if(data=='r')                          // Checking for received data
		    {
			     Nokia5110_Clear();                 // Clearing the LCD
	         Nokia5110_SetCursor(1,0);          // To Set the cursor 
		       Nokia5110_OutString(name1);        // Displaying recieved data on LCD
		    } 
     SysTick_Wait10ms(3);				 
     }
	}
