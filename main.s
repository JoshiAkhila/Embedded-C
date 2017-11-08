;****************** main.s ***************
; Program written by: Akhila Joshi 
; Date Created:  09/11/2015
; Last Modified: 09/22/2015
; Lab number: 1
; Brief description of the program
; The overall objective of this system is a digital lock
; Color of the LED changes according to the Inputs 
; Hardware connections
; PORTE is switch input (1 means switch is pressed, 0 means switch is not pressed)
; PORTA is LED output (0 means door is locked, 1 means door is unlocked)
; In PORTE PE0 to PE4 are used as input pins.
; PE0 to PE4 are connected to lounchpad and dipswiches to provide inputs.
; In PORTA PA2 and PA3 are used as output pins.
; Color of the LED will change according to the inputs.
; If input is 0X00 then both the remains OFF.
; If input is incorrect then RED LED will toggle once and then remains ON.
; RED LED connected to PA2.
; If input is correct then Green LED will toggle twice and then turned OFF. 
; Green LED connected to PA3.
; A short delay is inserted.Here onesecond delay is used to observe LED toggling
; Direction register, data registers, analog mode selection registers,alternative
; function disabling register,Digital enabling registers are used.   
; Test GPIO initialization function by setting the LED
; PORTA memory locations is provided in the register definitions file on blackboard

GPIO_PORTE_DATA_R       EQU   0x400243FC              ;PortE data register
GPIO_PORTE_DIR_R        EQU   0x40024400              ;PortE direction regiser
GPIO_PORTE_LOCK_R       EQU   0x40024520
GPIO_PORTE_AFSEL_R      EQU   0x40024420              ;PortE alternative register
GPIO_PORTE_DEN_R        EQU   0x4002451C              ;PortE digital enabling
GPIO_PORTE_AMSEL_R      EQU   0x40024528
GPIO_PORTE_PCTL_R       EQU   0x4002452C
SYSCTL_RCGC2_R          EQU   0x400FE108              ; Clock
SYSCTL_RCGCGPIO_R4		EQU	  0x00000010
SYSCTL_RCGC2_GPIOE     	EQU   0x00000010              ;PortE clock gating control
GPIO_PORTE_CR_R         EQU   0x40024524
GPIO_PORTE_PUR_R        EQU   0x40024510
GPIO_PORTA_DATA_R       EQU   0x400043FC              ;PortA data register
GPIO_PORTA_DIR_R        EQU   0x40004400              ;PortA direction register
SYSCTL_RCGCGPIO_R0		EQU	  0x00000001
GPIO_PORTA_AFSEL_R      EQU   0x40004420              ;PortA alternative register
GPIO_PORTA_DEN_R        EQU   0x4000451C              ;PortA digital enabling
GPIO_PORTA_CR_R         EQU   0x40004524
GPIO_PORTA_AMSEL_R      EQU   0x40004528
GPIO_PORTA_PCTL_R       EQU   0x4000452C
GPIO_PORTA_PUR_R        EQU   0x40004510	
	
RED     EQU  0X04   
GREEN   EQU  0X08
NOLED   EQU	 0X00
	
	
      AREA    |.text|, CODE, READONLY, ALIGN=2
      THUMB
      EXPORT  Start
Start

    BL PortE_Init                   ;Initialize PortE
	BL PortA_Init                   ;Initialize PortA
loop
    LDR R0, =FIFTHSEC               ; R0 = FIFTHSEC (delay 0.2 second)
    BL  delay                       
	BL  PortE_Input                 ; read all of the inputs on Port E
    MOV R2,R0   
    CMP R0, #0x00                   ; R0 == 0x00?
    BEQ NOPRESSED                   ; if so, go to No pressed
    CMP R0, #0x01                   ; R0 == 0x01?
    BEQ UNLOCKED                    ; if so,   go to Unlocked
    CMP R0, #0x01                   ; R0 != 0x01?
    BNE LOCKED                      ; if so,  go to locked
	
    MOV R1, #(RED)                  ; R0 = Red(Red LED ON)
    BL  PortA_Output1               ; Go to PortA_Output1
    B   loop
	
	MOV R1, #(GREEN)                ; R1 = (GREEN) 
    BL  PortA_Output2               ; Go to PortA_Output2
    B   loop
	 
NOPRESSED	
    MOV R1, #NOLED                  ; R1 = NOLED
    BL  PortA_Output                ; Go to PortA_Output
    B   loop
	
	
UNLOCKED
    MOV R1, #GREEN                  ; R1 = Green(Green LED on)
    BL  PortA_Output2            	; Go to PortA_Output2
KEEPON
    BL PortE_Input                  ; Read the inputs again from PortE
	CMP R0,R2                       ; Compare input values with input value
	BNE loop                        ; If R0!= R2 back to loop
	CMP R0,R2                       ; If R0 = R2 
	BEQ KEEPON                      ; Green should keep on
	
	
LOCKED
    MOV R1, #RED                    ; R1 = Red (Red LED on)
    BL  PortA_Output1               ; Go to PortA_Output1
BACK 
    BL PortE_Input                  ; Read the inputs again from PortE
	CMP R0,R2                       ; Compare input values with input value
	BEQ BACK                        ; If R0= R2 go to back(Red off)
	BNE loop                        ; If R0!= R2 back to loop
;-----------------------------------------------------------------------
; delay function

ONESECOND	EQU		5333333         
QUARTERSEC	EQU		1333333         
FIFTHSEC	EQU		1066666          

delay

	SUBS	R0, R0, #1             
	BNE		delay                   
	BX		LR                      
;--------------------------------------------------------------------------
;-------------------PortE_Init----------------------------------------------
PortE_Init
    LDR R1, =SYSCTL_RCGC2_R         ;Initializing Clock
    LDR R0, [R1]                 
    ORR R0, R0, #0x11               ;Set bit5 and bit1 for PortE and PortA 
    STR R0, [R1]                  
    NOP
    NOP
    LDR R1, =GPIO_PORTE_AFSEL_R     ; Disable alternative function on PortE
    MOV R0, #0                      
    STR R0, [R1]                    
    LDR R1, =GPIO_PORTE_DEN_R       ; Digital enabling portE
    MOV R0, #0XFF                   
    STR R0, [R1]  
    LDR R1, =GPIO_PORTE_DIR_R      ; direction register
    LDR R0, [R1]
	AND R0,#0xE0                    ; Clearing bit4 to bit0
    STR R0, [R1]	                ; Store the R0 value in R1
    BX  LR  
	
PortA_Init
    LDR R1, =SYSCTL_RCGC2_R         ; Initializing clock 
    LDR R0, [R1]                 
    ORR R0, R0, #0x11               ; Set bit5 and bit1 for PortE and PortA 
    STR R0, [R1]                    
    NOP
    NOP
    LDR R1, =GPIO_PORTA_AMSEL_R     ; Disable analog mode on PortE
    MOV R0, #0                      
    STR R0, [R1]                                      
    LDR R1, =GPIO_PORTA_AFSEL_R     ; Disabling alternative function on portA
    MOV R0, #0                      
    STR R0, [R1]                    
    LDR R1, =GPIO_PORTA_DEN_R       ; Digital enabling portA
    MOV R0, #0xFF                  
    STR R0, [R1] 
    LDR R1, =GPIO_PORTA_DIR_R       ; Set direction register 
    LDR R0,[R1]
	ORR R0,#0x0C                    ; Set PA2 and PA3 for output 
    STR R0, [R1]	
    BX  LR  

PortE_Input
    LDR R1, =GPIO_PORTE_DATA_R      ; Pointer to Port E data
    LDR R0, [R1]                    
    AND R0,R0,#0x1F                 ; Read the inputs from PE0 to PE1
    BX  LR                          
	
PortA_Output
    LDR R0, =GPIO_PORTA_DATA_R      ; pointer to PortA data
    STR R1, [R0]                    ; Store NOLED value in R0(0x00)  
    BX  LR  
	
PortA_Output1
    LDR R0,=GPIO_PORTA_DATA_R       ; Pointer to portA data 
	STR R1,[R0]                     ; Store red value to R0(0x04)
	LDR R0,=ONESECOND               ; Give one second delay  
	BL delay
	EOR R1,R1,#0X04                 ; Toggle bit3(PA2) on portA 
    LDR R0,=GPIO_PORTA_DATA_R       ; Pointer to portA data 
	STR R1,[R0]                     ; Store value of R1 to R0(0x00)
	LDR R0,=ONESECOND               ; Give one second delay 
	BL delay
	EOR R1,R1,#0X04                 ; Toggle bit3(PA2) on portA
    LDR R0,=GPIO_PORTA_DATA_R        
	STR R1,[R0]                     ; Store value of R1 to R0(0x04)
	LDR R0,=ONESECOND
	BL delay
    EOR R1,R1,#0X04                 ; Toggle bit3(PA2) on portA
    LDR R0,=GPIO_PORTA_DATA_R       ; Pointer to portA data 
	STR R1,[R0]	                    ; Store value of R1 to R0(0x00)
	B   BACK	                    ; Keep RED LED Off
	
PortA_Output2
    LDR R0,=GPIO_PORTA_DATA_R       ; Pointer to portA data
    STR R1,[R0]                     ; Store red value to R0(0x08)
    LDR R0,=ONESECOND               ; Give one second delay
	BL delay
	EOR R1,R1,#0X08                 ; Toggle bit4(PA3) on portA
    LDR R0,=GPIO_PORTA_DATA_R       ; Pointer to portA data
    STR R1,[R0]                     ; Store red value to R0(0x00)
    LDR R0,=ONESECOND
	BL delay
	EOR R1,R1,#0X08                 ; Toggle bit4(PA3) on portA             	
	LDR R0,=GPIO_PORTA_DATA_R       ; Pointer to portA data
    STR R1,[R0]                     ; Store red value to R0(0x08)
    B   KEEPON                      ; Keep Green LED On
                        
	
	ALIGN                           ; make sure the end of this section is aligned
	END                             ; end of file
