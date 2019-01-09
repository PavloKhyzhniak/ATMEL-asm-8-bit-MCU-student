;**************************************
;**	Programm Version 001
;**	15 March 2013
;** AmperVoltMetr
;**	ATmega8 	8MHz crystal
;** Blackveolet
;** Ukraine, Donetsk region, Khartsyzsk
;** +38(095)725-20-14
;** blackveolet@mail.ru
;**************************************

;**************************************
; Main system
;**************************************
.nolist
; ����������� ������������ ������
#include"m8def.inc"
.list

.device ATmega8

; �������� ������� ����������

; ����� �������� ������ ��������
.cseg
; Reset Handler
.org 0x000 rjmp RESET ; Reset Handler
; IRQ0 Handler
.org 0x001 reti;rjmp EXT_INT0 ; IRQ0 Handler
; IRQ1 Handler
.org 0x002 reti;rjmp EXT_INT1 ; IRQ1 Handler
; Timer2 Compare Handler
.org 0x003 reti;rjmp TIM2_COMP ; Timer2 Compare Handler
; Timer2 Overflow Handler
.org 0x004 reti;rjmp TIM2_OVF ; Timer2 Overflow Handler
; Timer1 Capture Handler
.org 0x005 reti;rjmp TIM1_CAPT ; Timer1 Capture Handler
; Timer1 CompareA Handler
.org 0x006 reti;rjmp TIM1_COMPA ; Timer1 CompareA Handler
; Timer1 CompareB Handler
.org 0x007 reti;rjmp TIM1_COMPB ; Timer1 CompareB Handler
; Timer1 Overflow Handler
.org 0x008 reti;rjmp TIM1_OVF ; Timer1 Overflow Handler
; Timer0 Overflow Handler
.org 0x009 rjmp TIM0_OVF ; Timer0 Overflow Handler
; SPI Transfer Complete Handler
.org 0x00a reti;rjmp SPI_STC ; SPI Transfer Complete Handler
; USART RX Complete Handler
.org 0x00b reti;rjmp USART_RXC ; USART RX Complete Handler
; UDR Empty Handler
.org 0x00c reti;rjmp USART_UDRE ; UDR Empty Handler
; USART TX Complete Handler
.org 0x00d reti;rjmp USART_TXC ; USART TX Complete Handler
; ADC Conversion Complete Handler
.org 0x00e rjmp ADC_INT ; ADC Conversion Complete Handler
; EEPROM Ready Handler
.org 0x00f rjmp EE_RDY ; EEPROM Ready Handler
; Analog Comparator Handler
.org 0x010 reti;rjmp ANA_COMP ; Analog Comparator Handler
; Two-wire Serial Interface Handler
.org 0x011 reti;rjmp TWSI ; Two-wire Serial Interface Handler
; Store Program Memory Ready Handler
.org 0x012 reti;rjmp SPM_RDY ; Store Program Memory Ready Handler


;�������� ���� �����
.def tmpL	= r16	;������� ������� �������(�������)
.def tmpH	= r17	;������� ������� �������(�������)
.def tmpL2	= r18	;��������������� ������� �������(�������)
.def tmpH2	= r19	;��������������� ������� �������(�������)
.def tmp1	= r20	;���������������� 1 �������
.def tmp2	= r21	;���������������� 1 �������
.def Flags	= r22	;������� ������
.def DataL	= r23	;������� ������(�������)
.def DataH	= r24	;������� ������(�������)
.def cnt1	= r25	;������� ������� �������

.def	cntADC = r15 ;����� ����� ��������
.def	cntLED = r14 ;����� ������ �������

// ���������� ����������� ���� ������ �������
#include"DSEG.inc"		;���� �������������� ���������� � ��� � ������������� �������� � ������
#include"EEPROM.inc"	;���� ������ � ����������������� �������
#include"RESET.inc" 	;���� �������������(������)
;#include"EXT_INT.inc"	;���� ��������� ������� ����������
;#include"PCINT.inc"		;���� ������ � ������������ �����/������
#include"TIM0.inc"		;���� ������ � ��������0
#include"ADC_INT.inc"	;���� ������ � ���
;#include"ANA_COMP.inc"	;���� ������ � ���������� ������������

#include"Delay.inc"		;���� ����������� ��������
#include"SubRouters.inc";���� ��������� ������� �����������


//���������� ����������� ����
main:
; ������ ���
	in r16,ADCSRA
	ori r16,1<<ADSC
	out ADCSRA,r16


	clr r16
	sts VoltSmall,r16
	sts VoltSmall+1,r16
	sts VoltBig,r16
	sts VoltBig+1,r16
	sts Amper,r16
	sts Amper+1,r16
	sts cntVoltBig,r16
	sts cntVoltSmall,r16
	sts cntAmper,r16
	sts Atek,r16
	sts Aerror,r16


sei	; enable global interrupt
mainWork:
		nop

		rjmp mainWork
