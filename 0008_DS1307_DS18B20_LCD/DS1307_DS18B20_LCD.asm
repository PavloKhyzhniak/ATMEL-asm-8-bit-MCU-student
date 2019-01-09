;**************************************
;**	Programm Version 001
;**	26 March 2013
;** Lesson_005 DS1307_DS18B20_LCD
;**	Mega48	20MHz
;** Blackveolet
;** Ukraine, Donetsk region, Khartsyzsk
;** blackveolet@mail.ru
;**************************************

;**************************************
; Main system
;**************************************
.nolist
; ����������� ������������ ������
#include"m48def.inc"
.list

.device ATmega48

; �������� ������� ����������

; ����� �������� ������ ��������
.cseg
; Reset Handler
.org 0x000 rjmp RESET ; Reset Handler
; IRQ0 Handler
.org 0x001 reti;rjmp EXT_INT0 ; IRQ0 Handler
; IRQ1 Handler
.org 0x002 reti;rjmp EXT_INT1 ; IRQ1 Handler
; PCINT0 Handler
.org 0x003 reti;rjmp PCINT_0 ; PCINT0 Handler
; PCINT1 Handler
.org 0x004 reti;rjmp PCINT_1 ; PCINT1 Handler
; PCINT2 Handler
.org 0x005 reti;rjmp PCINT_2 ; PCINT2 Handler
; Watchdog Timer Handler
.org 0x006 reti;rjmp WDT ; Watchdog Timer Handler
; Timer2 Compare A Handler
.org 0x007 reti;rjmp TIM2_COMPA ; Timer2 Compare A Handler
; Timer2 Compare B Handler
.org 0x008 reti;rjmp TIM2_COMPB ; Timer2 Compare B Handler
; Timer2 Overflow Handler
.org 0x009 rjmp TIM2_OVF ; Timer2 Overflow Handler
; Timer1 Capture Handler
.org 0x00A reti;rjmp TIM1_CAPT ; Timer1 Capture Handler
; Timer1 Compare A Handler
.org 0x00B reti;rjmp TIM1_COMPA ; Timer1 Compare A Handler
; Timer1 Compare B Handler
.org 0x00C reti;rjmp TIM1_COMPB ; Timer1 Compare B Handler
; Timer1 Overflow Handler
.org 0x00D reti;rjmp TIM1_OVF ; Timer1 Overflow Handler
; Timer0 Compare A Handler
.org 0x00E reti;rjmp TIM0_COMPA ; Timer0 Compare A Handler
; Timer0 Compare B Handler
.org 0x00F reti;rjmp TIM0_COMPB ; Timer0 Compare B Handler
; Timer0 Overflow Handler
.org 0x010 reti;rjmp TIM0_OVF ; Timer0 Overflow Handler
; SPI Transfer Complete Handler
.org 0x011 rjmp SPI_STC ; SPI Transfer Complete Handler
; USART, RX Complete Handler
.org 0x012 reti;rjmp USART_RXC ; USART, RX Complete Handler
; USART, UDR Empty Handler
.org 0x013 reti;rjmp USART_UDRE ; USART, UDR Empty Handler
; USART, TX Complete Handler
.org 0x014 reti;rjmp USART_TXC ; USART, TX Complete Handler
; ADC Conversion Complete Handler
.org 0x015 reti;rjmp ADC_INT ; ADC Conversion Complete Handler
; EEPROM Ready Handler
.org 0x016 rjmp EE_RDY ; EEPROM Ready Handler
; Analog Comparator Handler
.org 0x017 reti;rjmp ANA_COMP ; Analog Comparator Handler
; 2-wire Serial Interface Handler
.org 0x018 rjmp TWI_INT ; 2-wire Serial Interface Handler
; Store Program Memory Ready Handler
.org 0x019 reti;rjmp SPM_RDY ; Store Program Memory Ready Handler

;�������� ���� �����
.def tmpL	= r16	;������� ������� �������(�������)
.def tmpH	= r17	;������� ������� �������(�������)
.def tmpL2	= r18	;��������������� ������� �������(�������)
.def tmpH2	= r19	;��������������� ������� �������(�������)
.def temp	= r20	;���������������� 1 �������
.def temp1	= r21	;���������������� 1 �������
.def error	= r22	;������� ������
.def cnt3	= r23	;������� ������(�������)
.def cnt2	= r24	;������� ������(�������)
.def cnt1	= r25	;������� ������� �������

// ���������� ����������� ���� ������ �������
;#include"SECURITY.inc";���� ��������� �� ������

#include"DSEG.inc"		;���� �������������� ���������� � ��� � ������������� �������� � ������
#include"EEPROM.inc"	;���� ������ � ����������������� �������
#include"RESET.inc" 	;���� �������������(������)
;#include"EXT_INT.inc"	;���� ��������� ������� ����������
;#include"PCINT.inc"		;���� ������ � ������������ �����/������
#include"SPI.inc"		;���� ������ � SPI
#include"TWI.inc"		;���� ������ � TWI
;#include"TIM0.inc"		;���� ������ � ��������0
;#include"TIM1.inc"		;���� ������ � ��������1
;#include"ANA_COMP.inc"	;���� ������ � ���������� ������������
#include"Delay.inc"		;���� ����������� ��������
#include"SubRouters.inc";���� ��������� ������� �����������
#include"LCD_KEY.inc"	;���� ������ � ��� � �������� ������� 74HC164
#include"1-wire.inc"	;���� ������ � 1-wire �����������
#include"MACRO.inc"		;���� �������� ��������
#include"TIM2.inc"		;���� ������ � ��������2


//���������� ����������� ����
main:
;*************
; ����������� �����
; ������� ������ �� ������ � �������� �������� 5���
;		sbi DDRB,PB0
;		in r16,PORTB
;		ldi r17,1<<PB0
;		ldi r18,21
;pulse1:
;		eor r16,r17
;		out PORTB,r16
;		nop
;		nop
;
;		dec r18
;		brne pulse1
/*sbi DDRB,PB0
main0:
sbi PORTB,PB0
cbi PORTB,PB0
sbi PORTB,PB0
cbi PORTB,PB0
sbi PORTB,PB0
cbi PORTB,PB0
sbi PORTB,PB0
cbi PORTB,PB0
sbi PORTB,PB0
cbi PORTB,PB0
sbi PORTB,PB0
cbi PORTB,PB0
sbi PORTB,PB0
cbi PORTB,PB0
sbi PORTB,PB0
cbi PORTB,PB0
sbi PORTB,PB0
cbi PORTB,PB0
sbi PORTB,PB0
cbi PORTB,PB0
sbi PORTB,PB0
cbi PORTB,PB0
sbi PORTB,PB0
cbi PORTB,PB0

rjmp main0
*/
;*************
; ���������� ����������
;	sei ; Enable interrupts
	
	nop
	rcall initLCD_KEY
	
	ldi r16,0b10110000
	sts SLA_W,r16
	rcall I2C_INIT

;	RTC_START

	ldi r16,1
	sts cntKey,r16
	rjmp no_keyb


;*********************
no_keyb:
sei
DS18B20
;ldi r16,78
;sts LCD_Temperature,r16
rcall OutLCD_Temperature

	ldi		R18,200
	rcall	delay_ms			;�������� � 200��
	rcall	Scan_keyb			;������ ������ ������
	cpi		R16,$00				;���� ��������� $00 ������ �� ���� ������� �� ������
	breq	no_keyb				;������� � ������������ ������ ���� �� ���� �������


;����� ��������� �������� �� ������� ������

;***********
;���������� ���������� �������
	sec
	sbis LED_LCD_PORT,LED
	clc

	brcs watchKey
; ������� ��������� ���
	sbi LED_LCD_DDR,LED
	sbi LED_LCD_PORT,LED

	clr r18
	sts TimeLed,R18
	brcc no_keyb
watchKey:
;***********

	sbrc r16,5
	rjmp Key_Up
	sbrc r16,6
	rjmp Key_Down
	sbrc r16,4
	rjmp Key_Left
	sbrc r16,7
	rjmp Key_Right
	sbrc r16,3
	rjmp Key_Enter
	sbrc r16,2
	rjmp Key_Escape
	sbrc r16,0
	rjmp Key_Start
	sbrc r16,1
	rjmp Key_Stop
	rjmp Key_Stop

	;****************


formMenu:

	rjmp no_keyb


Key_Up:
	
	rjmp formMenu


Key_Down:
	
	rjmp formMenu

Key_Left:

	rjmp formMenu	

Key_Right:
	
	rjmp formMenu

Key_Enter:
	
	rjmp formMenu

Key_Escape:
	
	rjmp formMenu

Key_Start:
	
	rjmp formMenu

Key_Stop:
	
	rjmp formMenu


;***********************

		rjmp main

