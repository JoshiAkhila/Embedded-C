// 0.Documentation Section 
// Oscilator, main.c
// First we are initializing PortF and PortE.
// Runs on LM4F120 or TM4C123 LaunchPad
// In this program we are using Clock register,Lock Registers, CR registers, AMSEL registers,DEN( digital enable),
// Registers,Data registers and AFSEL registers.
// Here we are using PortF and PortE for input and ouput respectively.
// The system has two digital inputs and two digital outputs.  
// In PortF we are using PF1 and PF2 for input.
// In PortE we are using PE1 and PE2 for output1 and output0 respectively.
// Here we are using two delay functions to get two different frequency values.
// Here we are providing 262Hz and 392Hz frequency values.
// If input1 is (ture) equal to 0x02 then digital output,"output1" oscillates at 262Hz.
// If input1 is (true) equal to 0X04 then digital output,"output0" oscillates at 392Hz.
// If input1 is false then output remains low.
// If both the inputs are true (i.e input1= 0X06) then both the outputs remain low.
// When we connect each outputs to 10 OHM registers we are able to hear tone as middleC and middleG.
// When we press both the switches then we are not able to hear any tone.


// Author: Akhila Joshi 
// Date:10/03/2015

// 1. Pre-processor Directives Section
// Constant declarations to access port registers using 
// symbolic names instead of addresses

#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_LOCK_R       (*((volatile unsigned long *)0x40025520))
#define GPIO_PORTF_CR_R         (*((volatile unsigned long *)0x40025524))
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define GPIO_PORTE_DATA_R       (*((volatile unsigned long *)0x400243FC))
#define GPIO_PORTE_DIR_R        (*((volatile unsigned long *)0x40024400))
#define GPIO_PORTE_AFSEL_R      (*((volatile unsigned long *)0x40024420))
#define GPIO_PORTE_DEN_R        (*((volatile unsigned long *)0x4002451C))
#define GPIO_PORTE_LOCK_R       (*((volatile unsigned long *)0x40024520))
#define GPIO_PORTE_CR_R         (*((volatile unsigned long *)0x40024524))
#define GPIO_PORTE_AMSEL_R      (*((volatile unsigned long *)0x40024528))
#define GPIO_PORTE_PCTL_R       (*((volatile unsigned long *)0x4002452C))

// 2. Declarations Section
//    Global Variables
unsigned long input1;                        // Input1 for comparing input values
unsigned long Output_1, Output_0, Output;    // Output variables

//   Insert Function Prototypes here

void Delay1(unsigned long frequency);        
void Delay2(unsigned long frequency);
void Delay(unsigned long frequency);
void PortF_Init(void);
void PortE_Init(void);
unsigned long  PortF_input1(void);
unsigned long  output_1(void);
unsigned long  output_0(void);
unsigned long  output(void);

// 3. Subroutines Section
// MAIN: Mandatory for a C Program to be executable
int main(void)
	{
      PortF_Init();                         // Initializing PortF for input 
	    PortE_Init();                         // Initializing PortE for output
	   
  while(1){
	
		//		Insert your code here
	input1= PortF_input1();                   // Calling PortF for input
	
		if(input1==0x02)                        // Comparing for input1 value(PF1= 0X02). Switch(0x01)    
		 { 
			 output_1();                          // Calling output_1 for PE1 when input is true
			 Delay1(262);                         // Providing delay function for frequecy 262Hz
			 output();                            // Calling output for zero value
			 Delay1(262);                         // Providing delay function for frequecy 262Hz
		 }
  else if(input1==0x04) 		                // Comparing for input1 value(PF1= 0X02). Switch(0x10)
	   {
		    output_0();                         // Calling output_0 for PE2 when input is true
		    Delay2(392);                        // Providing delay function for frequecy 392Hz
		    output();                           // Calling output for zero value 
		    Delay2(392);                        // Providing delay function for frequecy 392Hz
  	  }
	else if(input1==0x06)                     // When both inputs are true(0x06). Switch(0x00) 
	    {
		    output();                           // Calling output for zero value
	    }
else 
{ 	
	 output();                                // For other condtion output will be zero
	}
}
	}

unsigned long PortF_input1(void)           
{
   return(GPIO_PORTF_DATA_R &=0x06);        // Read input value from PortF
}

	// Insert a subroutine to initialize ports for input and output
  // 
  // Inputs: None
  // Outputs: None
  // Notes: ...
	
	void PortF_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000020;     // 1) activate clock for Port F
  delay = SYSCTL_RCGC2_R;           //    allow time for clock to start
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F
  GPIO_PORTF_CR_R = 0x1F;           //    allow changes to PF4-0
  // only PF0 needs to be unlocked, other bits can't be locked
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog on PF
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) PCTL GPIO on PF4-0
  GPIO_PORTF_DIR_R &= ~0x06;        // 5) PF2,PF1 in
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) disable alt funct on PF7-0
  //  GPIO_PORTF_PUR_R = 0x11;      //    enable pull-up on PF0 and PF4
  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital I/O on PF4-0
}
	
	void PortE_Init(void){ volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x00000010;     // 1) activate clock for Port E
  delay = SYSCTL_RCGC2_R;           // allow time for clock to start
	GPIO_PORTE_DIR_R |= 0x06;          // 5) PE1,PE2 out pins
  GPIO_PORTE_AFSEL_R = 0x00;        // 6) disable alt funct on PF7-0
  GPIO_PORTE_DEN_R = 0xFF;          // 7) enable digital I/O on PF4-0
}
	// Insert a subroutine to initialize port E here 
  // MODIFY and use the following delay function to create delay periods
  // We will make a precise estimate later: 
  // For now we assume it takes 0.1 sec to count down
  //    from 145448 down to zero
  // Inputs: Modify as needed
  // Outputs: None
  // Notes: ...
unsigned long  output_1(void)                   // Pointer to output
{
	return(GPIO_PORTE_DATA_R |=0X02);             // write 1 to output pin PE1 
}
unsigned long  output_0(void)
{
	return (GPIO_PORTE_DATA_R |= 0X04);           // wrire 1 to output pin PE2
}
unsigned long  output(void)
{ 
	return (GPIO_PORTE_DATA_R = 0X00);            // write 0 to output pins 
}
void Delay1(unsigned long frequency)            // Pointer to delay function
	{
	unsigned long volatile time;
	   //time for 0.1 second is 145448
	    time = 2776;                              // For 0.003816793893sec
      while(time!=0)
				{
	    time--;
        }
  }
void Delay2(unsigned long frequency)           // Pointer to delay function
	{
	unsigned long volatile time;
        //time for 0.1 second is 145448
	      time = 1855;                           // For 0.002551020408sec
    while(time!=0)
			{
		  time--;
      }
  }
	