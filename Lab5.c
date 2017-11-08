// Lab5.c
// this lab creates a position measurment software that outputs the result on LCD
// The software samples analog input signal and use calibration to output the position
// Runs on LM4F120 or TM4C123
// Use the SysTick timer to request interrupts at a particular period.
// Akhila Joshi 
// 11/09/2015
// In this project we are using PortF for ADC output.
// We are using PF1 for toggling.
// We are using PF2 for Heart beat.
// In this project we are using,

// Blue Nokia 5110 for LCD display
// ---------------
// Signal        (Nokia 5110) LaunchPad pin
// Reset         (RST, pin 1) connected to PA7
// SSI0Fss       (CE,  pin 2) connected to PA3
// Data/Command  (DC,  pin 3) connected to PA6
// SSI0Tx        (Din, pin 4) connected to PA5
// SSI0Clk       (Clk, pin 5) connected to PA2
// 3.3V          (Vcc, pin 6) power
// back light    (BL,  pin 7) not connected
// Ground        (Gnd, pin 8) ground

#include "Nokia5110.h"
#include "pll.h"
#include "ADC.h"
#include "tm4c123gh6pm.h"

char str[10];                  // Output string display as POSITION 
char point[10];                // Output string display for length
char st[20];                   // Dispaly function
unsigned long Data;           // 12-bit ADC
unsigned long Position;       // 32-bit fixed-point 0.001 cm
unsigned long ADCMail;        // Initializing mailbox variale ADCMail
unsigned long ADCstatus;      // Initializing mailbox variale ADCstatus

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode
void SysTick_Init(void);      // Initializing SysTick
void charecter_display(unsigned long value);  // Output display function
unsigned long Convert(unsigned long input);   // For output Convertion

#define PF1       (*((volatile unsigned long *)0x40025008))
#define PF2       (*((volatile unsigned long *)0x40025010))
#define PF3       (*((volatile unsigned long *)0x40025020))
	
void PortF_Init(void){unsigned long volatile delay;
	SYSCTL_RCGC2_R |= 0x00000020;          // 1) activate clock for Port F
  delay = SYSCTL_RCGC2_R;                //    allow time for clock to start
  GPIO_PORTF_LOCK_R = 0x4C4F434B;        // 2) unlock GPIO Port F
  GPIO_PORTF_CR_R = 0x1F;                //    allow changes to PF4-0
  // only PF0 needs to be unlocked, other bits can't be locked
  GPIO_PORTF_AMSEL_R = 0;                // 3) disable analog on PF
  GPIO_PORTF_PCTL_R = 0x00000000;        // 4) PCTL GPIO on PF4-0
  GPIO_PORTF_DIR_R |= 0x06;              // 5) Setting PF1,PF2 for outputs
  GPIO_PORTF_AFSEL_R = 0;                // 6) Disable alt funct on PF7-0
  //  GPIO_PORTF_PUR_R = 0x11;           //    enable pull-up on PF0 and PF4
  GPIO_PORTF_DEN_R = 0xFF;               // 7) enable digital I/O on PF4-0 
}

void SysTick_Init(void){                   // Initialize SisTick Timer 
	
  NVIC_ST_CTRL_R=0;                        // Disabling SysTick during setup(Clear Counter)
	NVIC_ST_RELOAD_R=1999999;                // Specifying Reload value for 40Hz
	NVIC_ST_CURRENT_R=0;                     // Current value written as Zero
	NVIC_SYS_PRI3_R =(NVIC_SYS_PRI3_R & 0X00FFFFFF)|0x20000000;   // Setting highest priority 1
	NVIC_ST_CTRL_R=0X07;                     // Setting counter and interrupt 
	EnableInterrupts();                      // Enabling global interrupt
}

void SysTick_Handler(void){
	
  GPIO_PORTF_DATA_R ^=0x02;     // Toggle LED using PF1
	//Data= ADC_In();             // Saving the 12bit ADC sample data into data
	ADCMail= ADC_In();//Data;     // Saving the 12bit ADC sample data into ADC mail
	ADCstatus=1;                  // Setting a mail flag ADC status to new value
//GPIO_PORTF_DATA_R ^= 0x02;    // Toggle LED using PF1
  
}

void charecter_display(unsigned long value){      // Dispaly function
 int k;                                           // Initializing int variable
	for (k=4;k>0;k--){  
	point[k]= value % 10 + 0x30;                    // Converting charecter into string value
	value= value/10;} 
	// Displaying WELCOME TO ADC on LCD
	  st[0]= 'W';
		st[1]= 'E';
		st[2]= 'L';
		st[3]= 'C';
		st[4]= 'O';
		st[5]= 'M';
		st[6]= 'E';
	  st[7]= ' ';
	  st[8]= 'T';
		st[9]= 'O';
		st[10]= ' ';
		st[11]= ' ';
		st[12]= ' ';
		st[13]= ' ';
	  st[14]= ' ';
	  st[15]= 'A';
	  st[16]= 'D';
	  st[17]= 'C';
	
	// Displaying POSITION on LCD
		str[0]= 'P';
		str[1]= 'O';
		str[2]= 'S';
		str[3]= 'I';
		str[4]= 'T';
		str[5]= 'I';
		str[6]= 'O';
	  str[7]= 'N';
	
	// Displaying value of position on LCD	
		point[0]= point[1];
		point[1]='.';
    point[5]=' ';
    point[6]='c';
    point[7]='m';
    
	}

unsigned long Convert(unsigned long input){   // To get Fixed Point numbers
	  Position= (int)input*500 /1000;         // we are taking delta = 10^(-3) 
	return Position;
}

int main(void){               // single step this program and look at Data
  PLL_Init();                 // Bus clock is 80 MHz 
  ADC_Init();         	      // turn on ADC, set channel to 1
  PortF_Init();               // Initializing PortF
	SysTick_Init();             // Initializing SysTick
	Nokia5110_Init();           // Initializing Nokia5110
	  
	while(1){
		  
		 if(ADCstatus==1)                 // Check for mailbox flag to be set
		  {
			  Position = Convert(ADCMail);  // Converting sample into fixed point number
        charecter_display(Position);  // Display function to read position
				ADCstatus=0; 		              // Clearing mailbox ADCstatus
				Nokia5110_Clear();            // Clearing the LCD
	      Nokia5110_SetCursor(1,0);     // To Set the cursor 
	      Nokia5110_OutString(st); 
				Nokia5110_SetCursor(1,3);     // To Set the cursor 
	      Nokia5110_OutString(str);     // Read outstring function to display POSITION
			  Nokia5110_SetCursor(1,5);     // To Set the cursor
			  Nokia5110_OutString(point);   // Read outstring function to display length
				//Nokia5110_SetCursor(1,2);
				//Nokia5110_OutUDec(Position);//  output charecter
				}
    	// Data = ADC_In();             // sample 12-bit channel 1
    }
 }

int main2(void){
 
	PLL_Init();         // Bus clock is 80 MHz 
  ADC_Init();         // turn on ADC, set channel to 1
  PortF_Init();				//initializes port F for debugging
  
	while(1){           // use scope to measure execution time for ADC_In and LCD_OutDec           
    PF2 = 0x04;       // Profile
    Data = ADC_In();  // sample 12-bit channel 1
    PF2 = 0x00;       // Profile
	}
}
 int main3(void){ 
   PLL_Init();         // Bus clock is 80 MHz 
   PortF_Init();
   ADC_Init();         // turn on ADC, set channel to 1
  while(1){  
    PF2 ^= 0x04;      // Heartbeat
    Data = ADC_In();  // sample 12-bit channel 1
    Position = Convert(Data);  
    }
 }
