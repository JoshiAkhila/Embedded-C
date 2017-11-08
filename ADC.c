// ADC.c
// Runs on LM4F120/TM4C123
// Provide functions that initialize ADC0 SS3 to be triggered by
// software and trigger a conversion, wait for it to finish,
// and return the result.
// AKHILA JOSHI 
// 11/7/2015
// For ADC we are using PortE.
// In PortE we are using PE2

#include "tm4c123gh6pm.h"

// This initialization function 
// Max sample rate: <=125,000 samples/second
// Sequencer 0 priority: 1st (highest)
// Sequencer 1 priority: 2nd
// Sequencer 2 priority: 3rd
// Sequencer 3 priority: 4th (lowest)
// SS3 triggering event: software trigger
// SS3 1st sample source: Ain1 (PE2)
// SS3 interrupts: flag set on completion but no interrupt requested
void ADC_Init(void){ volatile unsigned long delay;

	SYSCTL_RCGC2_R |= 0x00000010;         // 1) activate clock for Port E
  delay = SYSCTL_RCGC2_R;               //    allow time for clock to start
	GPIO_PORTE_DIR_R &= ~0x04;            // 2) Clear PE2 for input 
	GPIO_PORTE_AFSEL_R |= 0x04;           // 3) Enable alt funct on PE2
  GPIO_PORTE_DEN_R &= ~0X04;            // 4) Disable digital I/o on PE2
  GPIO_PORTE_AMSEL_R |=0X04;            // 5) Enable analog function on PE2
  SYSCTL_RCGC0_R |=0X00010000;          // 6) Activate ADC0
	delay= SYSCTL_RCGC2_R;                //    allow time for clock to start
	SYSCTL_RCGC0_R &= ~0X00000300;        // 7) Configure for 125K 
  ADC0_SSPRI_R = 0x0123;                // 8) Sequency 3 is highest priority
	ADC0_ACTSS_R &= ~0X0008;              // 9) Disable sample sequece 3 
	ADC0_EMUX_R &= ~0XF000;								// 10) Sequece 3 is software trigger 
  ADC0_SSMUX3_R &= ~0X000F;	            // 11) Clear SS3 Field
  ADC0_SSMUX3_R += 1;                   // Set channel Ain1 (PE2)
	ADC0_SSCTL3_R = 0X0006;               // 12)No Temp sensor and Differential sampling,enabling only Interrupt END0
	ADC0_ACTSS_R |= 0X0008;               // 13) Enable sample sequence3 
}

//------------ADC_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
unsigned long ADC_In(void){ 
	unsigned long bit;
	
	 ADC0_PSSI_R =0X0008;                 // Initiating ADC or SS3
	 while ((ADC0_RIS_R & 0X08)==0){};    // Wait for conversion
	 bit= (ADC0_SSFIFO3_R & 0XFFF);         // Read the value 
	 ADC0_ISC_R =0X0008;                  // Acknowledge the flag
	 return bit;                          // Returning value
 }


