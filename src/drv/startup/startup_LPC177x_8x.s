;/*****************************************************************************
; * @file:    startup_LPC177x_8x.s
; * @purpose: CMSIS Cortex-M3 Core Device Startup File
; *           for the NXP LPC17xx Device Series
; * @version: V1.03
; * @date:    09. February 2010
; *----------------------------------------------------------------------------
; *
; * Copyright (C) 2010 ARM Limited. All rights reserved.
; *
; * ARM Limited (ARM) is supplying this software for use with Cortex-Mx
; * processor based microcontrollers.  This file can be freely distributed
; * within development tools that are supporting such ARM based processors.
; *
; * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
; * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
; * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
; * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
; * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
; *
; ******************************************************************************/


;
; The modules in this file are included in the libraries, and may be replaced
; by any user-defined modules that define the PUBLIC symbol _program_start or
; a user defined start symbol.
; To override the cstartup defined in the library, simply add your modified
; version to the workbench project.
;
; The vector table is normally located at address 0.
; When debugging in RAM, it can be located in RAM, aligned to at least 2^6.
; The name "__vector_table" has special meaning for C-SPY:
; it is where the SP start value is found, and the NVIC vector
; table register (VTOR) is initialized to this address if != 0.
;
; Cortex-M version
;

        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start
        EXTERN  SystemInit
;        EXTERN  system_manu_cp
        PUBLIC  __vector_table
        PUBLIC  __vector_table_0x1c
        PUBLIC  __Vectors
        PUBLIC  __Vectors_End
        PUBLIC  __Vectors_Size

        DATA

__vector_table
        DCD     sfe(CSTACK)
        DCD     Reset_Handler_ROM

        DCD     NMI_Handler_ROM
        DCD     HardFault_Handler_ROM
        DCD     MemManage_Handler_ROM
        DCD     BusFault_Handler_ROM
        DCD     UsageFault_Handler_ROM
__vector_table_0x1c
        DCD     0
        DCD     0
        DCD     0
        DCD     0
        DCD     SVC_Handler_ROM
        DCD     DebugMon_Handler_ROM
        DCD     0
        DCD     PendSV_Handler_ROM
        DCD     SysTick_Handler_ROM

        ; External Interrupts
        DCD     WDT_IRQHandler_ROM            ; 16: Watchdog Timer
        DCD     TIMER0_IRQHandler_ROM         ; 17: Timer0
        DCD     TIMER1_IRQHandler_ROM         ; 18: Timer1
        DCD     TIMER2_IRQHandler_ROM         ; 19: Timer2
        DCD     TIMER3_IRQHandler_ROM         ; 20: Timer3
        DCD     UART0_IRQHandler_ROM          ; 21: UART0
        DCD     UART1_IRQHandler_ROM          ; 22: UART1
        DCD     UART2_IRQHandler_ROM          ; 23: UART2
        DCD     UART3_IRQHandler_ROM          ; 24: UART3
        DCD     PWM1_IRQHandler_ROM           ; 25: PWM1
        DCD     I2C0_IRQHandler_ROM           ; 26: I2C0
        DCD     I2C1_IRQHandler_ROM           ; 27: I2C1
        DCD     I2C2_IRQHandler_ROM           ; 28: I2C2
        DCD     SPIFI_IRQHandler_ROM          ; 29: SPIFI
        DCD     SSP0_IRQHandler_ROM           ; 30: SSP0
        DCD     SSP1_IRQHandler_ROM           ; 31: SSP1
        DCD     PLL0_IRQHandler_ROM           ; 32: PLL0 Lock (Main PLL)
        DCD     RTC_IRQHandler_ROM            ; 33: Real Time Clock
        DCD     EINT0_IRQHandler_ROM          ; 34: External Interrupt 0
        DCD     EINT1_IRQHandler_ROM          ; 35: External Interrupt 1
        DCD     EINT2_IRQHandler_ROM          ; 36: External Interrupt 2
        DCD     EINT3_IRQHandler_ROM          ; 37: External Interrupt 3
        DCD     ADC_IRQHandler_ROM            ; 38: A/D Converter
        DCD     BOD_IRQHandler_ROM            ; 39: Brown-Out Detect
        DCD     USB_IRQHandler_ROM            ; 40: USB
        DCD     CAN_IRQHandler_ROM            ; 41: CAN
        DCD     DMA_IRQHandler_ROM            ; 42: General Purpose DMA
        DCD     I2S_IRQHandler_ROM            ; 43: I2S
        DCD     ENET_IRQHandler_ROM           ; 44: Ethernet
        DCD	MCI_IRQHandler_ROM		  ; 45: MCI Card
        DCD     MCPWM_IRQHandler_ROM          ; 46: Motor Control PWM
	DCD     QEI_IRQHandler_ROM            ; 47: Quadrature Encoder Interface
	DCD     PLL1_IRQHandler_ROM           ; 48: PLL1 Lock (USB PLL)
	DCD	USBActivity_IRQHandler_ROM	  ; 49: USB Activity Interrupt
	DCD	CANActivity_IRQHandler_ROM	  ; 50: CAN Activity Interrupt
	DCD	UART4_IRQHandler_ROM          ; 51: UART4
	DCD	SSP2_IRQHandler_ROM		  ; 52: SSP2
	DCD	LCD_IRQHandler_ROM		  ; 53: LCD
	DCD	GPIO_IRQHandler_ROM		  ; 54: GPIO
	DCD	PWM0_IRQHandler_ROM		  ; 55: PWM0
	DCD	EEPROM_IRQHandler_ROM	  ; 56: EEPROM




__Vectors_End

__Vectors       EQU   __vector_table
__Vectors_Size 	EQU 	__Vectors_End - __Vectors


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
        THUMB

        PUBWEAK Reset_Handler_ROM
        SECTION .text:CODE:REORDER(2)
Reset_Handler_ROM
        LDR     R0, =SystemInit
        BLX     R0
;        LDR     R0, =system_manu_cp
;        BLX     R0
        LDR     R0, =__iar_program_start
        BX      R0

        PUBWEAK NMI_Handler_ROM
        SECTION .text:CODE:REORDER(1)
NMI_Handler_ROM
        B NMI_Handler_ROM

        PUBWEAK HardFault_Handler_ROM
        SECTION .text:CODE:REORDER(1)
HardFault_Handler_ROM
        B HardFault_Handler_ROM

        PUBWEAK MemManage_Handler_ROM
        SECTION .text:CODE:REORDER(1)
MemManage_Handler_ROM
        B MemManage_Handler_ROM

        PUBWEAK BusFault_Handler_ROM
        SECTION .text:CODE:REORDER(1)
BusFault_Handler_ROM
        B BusFault_Handler_ROM

        PUBWEAK UsageFault_Handler_ROM
        SECTION .text:CODE:REORDER(1)
UsageFault_Handler_ROM
        B UsageFault_Handler_ROM

        PUBWEAK SVC_Handler_ROM
        SECTION .text:CODE:REORDER(1)
SVC_Handler_ROM
        B SVC_Handler_ROM

        PUBWEAK DebugMon_Handler_ROM
        SECTION .text:CODE:REORDER(1)
DebugMon_Handler_ROM
        B DebugMon_Handler_ROM

        PUBWEAK PendSV_Handler_ROM
        SECTION .text:CODE:REORDER(1)
PendSV_Handler_ROM
        B PendSV_Handler_ROM

        PUBWEAK SysTick_Handler_ROM
        SECTION .text:CODE:REORDER(1)
SysTick_Handler_ROM
        B SysTick_Handler_ROM

        PUBWEAK WDT_IRQHandler_ROM
        SECTION .text:CODE:REORDER(1)
WDT_IRQHandler_ROM
        B WDT_IRQHandler_ROM

        PUBWEAK TIMER0_IRQHandler_ROM
        SECTION .text:CODE:REORDER(1)
TIMER0_IRQHandler_ROM
        B TIMER0_IRQHandler_ROM

        PUBWEAK TIMER1_IRQHandler_ROM
        SECTION .text:CODE:REORDER(1)
TIMER1_IRQHandler_ROM
        B TIMER1_IRQHandler_ROM

        PUBWEAK TIMER2_IRQHandler_ROM
        SECTION .text:CODE:REORDER(1)
TIMER2_IRQHandler_ROM
        B TIMER2_IRQHandler_ROM

        PUBWEAK TIMER3_IRQHandler_ROM
        SECTION .text:CODE:REORDER(1)
TIMER3_IRQHandler_ROM
        B TIMER3_IRQHandler_ROM

        PUBWEAK UART0_IRQHandler_ROM
        SECTION .text:CODE:REORDER(1)
UART0_IRQHandler_ROM
        B UART0_IRQHandler_ROM

        PUBWEAK UART1_IRQHandler_ROM
        SECTION .text:CODE:REORDER(1)
UART1_IRQHandler_ROM
        B UART1_IRQHandler_ROM

        PUBWEAK UART2_IRQHandler_ROM
        SECTION .text:CODE:REORDER(1)
UART2_IRQHandler_ROM
        B UART2_IRQHandler_ROM

        PUBWEAK UART3_IRQHandler_ROM
        SECTION .text:CODE:REORDER(1)
UART3_IRQHandler_ROM
        B UART3_IRQHandler_ROM

        PUBWEAK PWM1_IRQHandler_ROM
        SECTION .text:CODE:REORDER(1)
PWM1_IRQHandler_ROM
        B PWM1_IRQHandler_ROM

        PUBWEAK I2C0_IRQHandler_ROM
        SECTION .text:CODE:REORDER(1)
I2C0_IRQHandler_ROM
        B I2C0_IRQHandler_ROM

        PUBWEAK I2C1_IRQHandler_ROM
        SECTION .text:CODE:REORDER(1)
I2C1_IRQHandler_ROM
        B I2C1_IRQHandler_ROM

        PUBWEAK I2C2_IRQHandler_ROM
        SECTION .text:CODE:REORDER(1)
I2C2_IRQHandler_ROM
        B I2C2_IRQHandler_ROM

        PUBWEAK SPIFI_IRQHandler_ROM
        SECTION .text:CODE:REORDER(1)
SPIFI_IRQHandler_ROM
        B SPIFI_IRQHandler_ROM

        PUBWEAK SSP0_IRQHandler_ROM
        SECTION .text:CODE:REORDER(1)
SSP0_IRQHandler_ROM
        B SSP0_IRQHandler_ROM

        PUBWEAK SSP1_IRQHandler_ROM
        SECTION .text:CODE:REORDER(1)
SSP1_IRQHandler_ROM
        B SSP1_IRQHandler_ROM

        PUBWEAK PLL0_IRQHandler_ROM
        SECTION .text:CODE:REORDER(1)
PLL0_IRQHandler_ROM
        B PLL0_IRQHandler_ROM

        PUBWEAK RTC_IRQHandler_ROM
        SECTION .text:CODE:REORDER(1)
RTC_IRQHandler_ROM
        B RTC_IRQHandler_ROM

        PUBWEAK EINT0_IRQHandler_ROM
        SECTION .text:CODE:REORDER(1)
EINT0_IRQHandler_ROM
        B EINT0_IRQHandler_ROM

        PUBWEAK EINT1_IRQHandler_ROM
        SECTION .text:CODE:REORDER(1)
EINT1_IRQHandler_ROM
        B EINT1_IRQHandler_ROM

        PUBWEAK EINT2_IRQHandler_ROM
        SECTION .text:CODE:REORDER(1)
EINT2_IRQHandler_ROM
        B EINT2_IRQHandler_ROM

        PUBWEAK EINT3_IRQHandler_ROM
        SECTION .text:CODE:REORDER(1)
EINT3_IRQHandler_ROM
        B EINT3_IRQHandler_ROM

        PUBWEAK ADC_IRQHandler_ROM
        SECTION .text:CODE:REORDER(1)
ADC_IRQHandler_ROM
        B ADC_IRQHandler_ROM

        PUBWEAK BOD_IRQHandler_ROM
        SECTION .text:CODE:REORDER(1)
BOD_IRQHandler_ROM
        B BOD_IRQHandler_ROM

        PUBWEAK USB_IRQHandler_ROM
        SECTION .text:CODE:REORDER(1)
USB_IRQHandler_ROM
        B USB_IRQHandler_ROM

        PUBWEAK CAN_IRQHandler_ROM
        SECTION .text:CODE:REORDER(1)
CAN_IRQHandler_ROM
        B CAN_IRQHandler_ROM

        PUBWEAK DMA_IRQHandler_ROM
        SECTION .text:CODE:REORDER(1)
DMA_IRQHandler_ROM
        B DMA_IRQHandler_ROM

        PUBWEAK I2S_IRQHandler_ROM
        SECTION .text:CODE:REORDER(1)
I2S_IRQHandler_ROM
        B I2S_IRQHandler_ROM

        PUBWEAK ENET_IRQHandler_ROM
        SECTION .text:CODE:REORDER(1)
ENET_IRQHandler_ROM
        B ENET_IRQHandler_ROM

        PUBWEAK MCI_IRQHandler_ROM
        SECTION .text:CODE:REORDER(1)
MCI_IRQHandler_ROM
        B MCI_IRQHandler_ROM

; PUBWEAK RIT_IRQHandler
        PUBWEAK MCPWM_IRQHandler_ROM
        SECTION .text:CODE:REORDER(1)
MCPWM_IRQHandler_ROM
        B MCPWM_IRQHandler_ROM

        PUBWEAK QEI_IRQHandler_ROM
        SECTION .text:CODE:REORDER(1)
QEI_IRQHandler_ROM
        B QEI_IRQHandler_ROM

        PUBWEAK PLL1_IRQHandler_ROM
        SECTION .text:CODE:REORDER(1)
PLL1_IRQHandler_ROM
        B PLL1_IRQHandler_ROM

        PUBWEAK USBActivity_IRQHandler_ROM
        SECTION .text:CODE:REORDER(1)
USBActivity_IRQHandler_ROM
        B USBActivity_IRQHandler_ROM

        PUBWEAK CANActivity_IRQHandler_ROM
        SECTION .text:CODE:REORDER(1)
CANActivity_IRQHandler_ROM
        B CANActivity_IRQHandler_ROM

        PUBWEAK UART4_IRQHandler_ROM
        SECTION .text:CODE:REORDER(1)
UART4_IRQHandler_ROM
        B UART4_IRQHandler_ROM

        PUBWEAK SSP2_IRQHandler_ROM
        SECTION .text:CODE:REORDER(1)
SSP2_IRQHandler_ROM
        B SSP2_IRQHandler_ROM

        PUBWEAK LCD_IRQHandler_ROM
        SECTION .text:CODE:REORDER(1)
LCD_IRQHandler_ROM
        B LCD_IRQHandler_ROM

        PUBWEAK GPIO_IRQHandler_ROM
        SECTION .text:CODE:REORDER(1)
GPIO_IRQHandler_ROM
        B GPIO_IRQHandler_ROM

        PUBWEAK PWM0_IRQHandler_ROM
        SECTION .text:CODE:REORDER(1)
PWM0_IRQHandler_ROM
        B PWM0_IRQHandler_ROM

		PUBWEAK EEPROM_IRQHandler_ROM
        SECTION .text:CODE:REORDER(1)
EEPROM_IRQHandler_ROM
        B EEPROM_IRQHandler_ROM

        END
