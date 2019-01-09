;**************************************
;** ������������ ����� FLASH
;** ������������ ������� � ���������
;** �. ��������, ���. ������, 2
;**
;**(������ ��� "�����" - ������ ������� ����, ����� ������, �� �7, "����")
;**
;** �������� �� ������ � ��������� �����
;**
;** mob: 095-725-20-14
;**
;** mob: 099-490-69-45
;**
;** mail: info@imkoteh.com
;**************************************
;**************************************
;**	��������-������� "�������"
;** ������������ ������� � ���������
;** �. ��������, ���. ������, 2
;**
;**(������ ��� "�����" - ������ ������� ����, ����� ������, �� �7, "����")
;**
;** �������� �� ������ � ��������� ���������
;**
;** mob: 066-817-76-78
;**
;** mob: 050-044-79-69
;**
;** icq: 419-543-015
;**
;** mail: info@imkoteh.com
;**************************************


;**************************************
;**	Test Programm Version 001
;**	12 Jan 2013
;** Test List for Study
;**	Mega8	1MHz
;** Blackveolet
;** Ukraine, Donetsk region, Khartsyzsk
;** blackveolet@mail.ru
;**************************************

;**************************************
; Lesson 001 IO system
;**************************************
.nolist
; ����������� ������������ ������
#include"m8def.inc"
.list
; ��������� DEVICE ��������� ������� ��� ������ ���������� ������������� ���������.
; ��� ������������� ������ ��������� ���������� ������ ��������������, ���� ����� 
; ������� ����������, ������� �� ������������ ������ ���������������. ����� ����� ������
; ��������������, ���� ����������� �������, ���� ������� EEPROM �������� ������ ����������� �����������. 
; ���� �� ��������� �� ������������ �� ��� ���������� ��������� �����������, � ����������� ����������� �� ������ ���������.
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
.org 0x00e reti;rjmp ADC ; ADC Conversion Complete Handler
; EEPROM Ready Handler
.org 0x00f reti;rjmp EE_RDY ; EEPROM Ready Handler
; Analog Comparator Handler
.org 0x010 rjmp ANA_COMP_TRIAC ; Analog Comparator Handler
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
.def tmp3	= r22	;���������������� 1 �������
.def DataL	= r23	;������� ������(�������)
.def DataH	= r24	;������� ������(�������)
.def cnt1	= r25	;������� ������� �������
.def Flags	= r26	;������� ������

// ���������� ����������� ���� ������ �������
#include"DSEG.inc"		;���� �������������� ���������� � ��� � ������������� �������� � ������
#include"EEPROM.inc"	;���� ������ � ����������������� �������
#include"RESET.inc" 	;���� �������������(������)
#include"EXT_INT.inc"	;���� ��������� ������� ����������
#include"TIM0.inc"		;���� ������ � ��������0
#include"ANA_COMP.inc"	;���� ������ � ���������� ������������
#include"Delay.inc"		;���� ����������� ��������
#include"SubRouters.inc"

; ����� ����������� ����������� ������
#include"Lesson001_IO.inc"
#include"Lesson002_AnaComp.inc"
#include"Lesson003_TIM0.inc"


//���������� ����������� ����
main:
; ��� ����������� ��� ������� ����� ���������	
;	sleep
;	rjmp PC-1


;****************************************
;**
;**			START
;**		LESSON_003_TIM0
;**
;**
;****************************************
	rcall TIM0_Init
	sleep
	rjmp PC-1


;***********************
;** ������ ����������� ������ ���������
;***********************

; �������� ���������� ����������
; �������� ��� ������ � �������� ���������� �� ����
	ldi r16, (0<<ACD)|(1<<ACIE)
	out ACSR, r16
; �������� ������ ����������� ��� ������ ��������
	cbi DDRD,PD6
	cbi PORTD,PD6

	cbi DDRD,PD7
	cbi PORTD,PD7
	
	rjmp PC-1

TIM0_OVF:
rjmp DAC_Sinus
; ���������� ����������������� ������ �������
; ����� �������� ���������� ����� 100 ����� �� 1024 ����
	ldi r16,-2
	out TCNT0,r16

	ldi r16,0xFF
	out DDRD,r16
	inc r17
	out PORTD,r17
; ��� ������ ����� ���������� ����� 100 ����� �� 1024???
; ���������� ��� ������� � 1 ��� ������ ��� 1 �����
; ����� 100*1024*1��� = 102,4 ����
; �� ���� ~10 ��� � �������

; ��� ����� ���������� ����������� ���

	reti

; enable Ava_Comp
		in r16, ACSR
		andi r16,~(1<<ACD)
		out ACSR, r16
	
; �������� �� ��� ��������� ���� ����������
	lds r16,Power
	inc r16
; ���������� ��������� �� 160
; ��������� ���������� 160 ���
; ���� ��������� ��� ����� ������  �� 0 �� 10
; �� ��� ������� � ��������� ����� ������������ ������ ��������� �� 16 ���
; �� ���� ��� 8 �������� �������� ����������
	cpi r16,160
	brlo PC+2
	clr r16
	sts Power,r16

; disable Timer0
; �������� ������, ���� ���������� ������������ �����������
	in r16,TIMSK
	cbr r16,0<<TOIE0
	out TIMSK, r16
	clr r16
	out TCCR0, r16

; impuls
; ������� ������� �� ��������� ��� �������� ������������ �����	
	sbi DDRD,PD5
	sbi PORTD,PD5
	ldi r19,1
	rcall Delay
	cbi PORTD,PD5

	reti

;������������ ������������� �������0
TIM0_Init:
; �������� ������������ ������� 0
	ldi r16,(1<<CS00)|(0<<CS01);|(1<<CS02)
	out TCCR0,r16
; ������� ��������� �������� � ������0 
; ����� �������� ���������� ����� 100 ����� �� 1024 ����
	ldi r16,-100
	out TCNT0,r16
; �������� ���������� �� ������� 0 �� ������������
	ldi r16,(1<<TOIE0)
	out TIMSK,r16

clr r16
sts SinFlags,r16

	ret


DAC_Sinus:
	ldi r16,-1
	out TCNT0,r16

	LDI ZH,High(2*TableSinus)
	LDI ZL,Low(2*TableSinus)

lds r16,Sinus

lds r20,SinFlags
andi r20,0x03
;���� 1 ��������� �� ����������� ��� �������� ������
sbrs r20,0
inc r16
sbrc r20,0
dec r16

cpi r16,90
brne PC+2
inc r20

cpi r16,0
brne PC+2
inc r20


;���������� ��������� ������� � ������
sts SinFlags,r20
sts Sinus,r16

;���������� 
lsl r16	
	add ZL,r16
	clr r16
	adc ZH,r16
	lpm r16,Z+
	lpm r17,Z

	ldi r18,Low(HalfSinus)
	ldi r19,High(HalfSinus)
sbrs r20,1
rjmp PC+4
	sub r18,r16
	sbc r19,r17
rjmp PC+3
	add r18,r16
	adc r19,r17

	out PORTD,r18
	out PORTB,r19

	ldi r16,0xFF
	out DDRD,r16
	out DDRB,r16

	reti


;****************************************
;**
;**			END
;**		LESSON_003_TIM0
;**
;**
;****************************************



;********-----------------------*********



;****************************************
;**
;**			START
;**		LESSON_002_AnaComp
;**
;**
;****************************************
;������������ ������������� ����������� �����������
;
;������������ ������ �� ����������� �����������
;����� ������������� ����� ������ ��� ��� ������ ������������ ���������� �� �����������
;	rcall AnaComp_Init
;	sleep
;	rjmp PC-1
;
;****************************************
;**
;**			END
;**		LESSON_002_AnaComp
;**
;**
;****************************************



;********-----------------------*********



;****************************************
;**
;**			START
;**		LESSON_001_IO
;**
;**
;****************************************
;	rjmp test_main
;********
;	rjmp work_indicator
;********
;	ldi r19,0
;	rcall delay
;********
;	rcall DelaySmall
;********
;	rjmp work_indicator2
;********
;	rjmp work_indBCD
;********
;	rjmp work_Color
;********
;	rjmp work_Keyboard
;********
;	rjmp work_Button
;********
;	rjmp work_ButINT
;********
;	rjmp work_ButINT01
;********
;****************************************
;**		
;**			END
;**		LESSON_001_IO
;**
;**
;****************************************

	rjmp main


