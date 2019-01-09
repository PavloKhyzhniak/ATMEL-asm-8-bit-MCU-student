;**************************************
;**	Test Programm
;**	19 Dec 2012
;** Test IR
;**	Mega8	1MHz
;**
;**************************************

.nolist
; подключение библиотечных файлов
#include"m8def.inc"

.list

; описание вектора прерываний
.cseg

.org 0x000 rjmp RESET ; Reset Handler
.org 0x001 reti;rjmp EXT_INT0 ; IRQ0 Handler
.org 0x002 reti;rjmp EXT_INT1 ; IRQ1 Handler
.org 0x003 reti;rjmp TIM2_COMP ; Timer2 Compare Handler
.org 0x004 reti;rjmp TIM2_OVF ; Timer2 Overflow Handler
.org 0x005 reti;rjmp TIM1_CAPT ; Timer1 Capture Handler
.org 0x006 reti;rjmp TIM1_COMPA ; Timer1 CompareA Handler
.org 0x007 reti;rjmp TIM1_COMPB ; Timer1 CompareB Handler
.org 0x008 reti;rjmp TIM1_OVF ; Timer1 Overflow Handler
.org 0x009 reti;rjmp TIM0_OVF ; Timer0 Overflow Handler
.org 0x00a reti;rjmp SPI_STC ; SPI Transfer Complete Handler
.org 0x00b reti;rjmp USART_RXC ; USART RX Complete Handler
.org 0x00c reti;rjmp USART_UDRE ; UDR Empty Handler
.org 0x00d reti;rjmp USART_TXC ; USART TX Complete Handler
.org 0x00e reti;rjmp ADC ; ADC Conversion Complete Handler
.org 0x00f reti;rjmp EE_RDY ; EEPROM Ready Handler
.org 0x010 reti;rjmp ANA_COMP ; Analog Comparator Handler
.org 0x011 reti;rjmp TWSI ; Two-wire Serial Interface Handler
.org 0x012 reti;rjmp SPM_RDY ; Store Program Memory Ready Handler
;
;******************************


;******************************
.cseg

/*	LDI ZH,High(2*TableTRIAC)
	LDI ZL,Low(2*TableTRIAC)
	lds r17, Power
	add ZL,r17
	clr r17
	adc ZH,r17
	lpm r16,Z
*/


; первоначальный сброс
RESET:
; запрещаем прерывания
	cli

;	инициализация стека
	ldi r16,high(RAMEND); Main program start
	out SPH,r16 ; Set Stack Pointer to top of RAM
	ldi r16,low(RAMEND)
	out SPL,r16

; отключаем аналоговый компаратор для энерго сбережения
	in r16,ACSR
	sbr r16, (1<<ACD);
	out ACSR, r16

; работа с внешними прерываниями
	in r16, GICR
	sbr r16, 1<<INT0
	out GICR, r16

	in r16, MCUCR
	sbr r16, (1<<ISC01); работа по спаду
	out MCUCR, r16

; работа с портами Ввода/Вывода
	ldi r16, 0<<PUD; отключаем подтягивающие резисторы
	out SFIOR, r16

	ldi r16,0xFF
	out DDRB, r16
	ldi r16, 0x00
	out PORTB, r16

	ldi r16,0b11111011
	out DDRD, r16
	ldi r16, 0x04
	out PORTD, r16

	ldi r16,0xFF
	out DDRC, r16
	ldi r16, 0x00
	out PORTC, r16

	
; выключение сторожевого таймера
WDT_off:
; reset WDT
WDR
; Write logical one to WDCE and WDE
in r16, WDTCR
ori r16, (1<<WDCE)|(1<<WDE)
out WDTCR, r16
; Turn off WDT
ldi r16, (0<<WDE)
out WDTCR, r16

;*******************
; очистка всех рабочих регистров

	clr R16
	clr R17
	clr R18
	clr R19
	clr R20
	clr ZL
	clr ZH

; разрешение прерываний
	sei ; Enable interrupts

; основной цикл
main0:

;	in r16, PINB
;	sbrc r16,0
	rjmp message_

	rjmp main0

;**********************
message_:

	;ldi r16, 0b10101110
	ldi r16,0b10000000
	inc r17
	add r16,r17
	clr r18

	rcall DelayLong

message_0:
	cpi r18,8
	breq message_
	
	inc r18
	rol r16
	brlo WR_1
	rjmp WR_0

;**********************
;1
WR_1:
	sbi PORTC,PC0
	sbi PORTD,PD2
	
	ldi r19,15
	rcall delay
	
	cbi PORTD,PD2
	cbi PORTC,PC0
	
	ldi r19,15
	rcall delay
	
	rjmp message_0
;**********************
;0
WR_0:
	cbi PORTC,PC0
	cbi PORTD,PD2
	
	ldi r19,15
	rcall delay
	
	sbi PORTD,PD2
	sbi PORTC,PC0
	
	ldi r19,15
	rcall delay
	
	rjmp message_0
;**********************


;**********************
; r19 =0 519 тактов
; r19 =1 519 тактов
; r19 =2 519+501 тактов
Delay:
	push r20
	push r19

	cpi r19,0
	brne loop0
	ldi r19,1
loop0:
	ldi r20,0xA6
loop1:
	dec r20
	brne loop1
	dec r19
	brne loop0

	pop r19
	pop r20
	ret
;**********************

;**********************
DelayLong:
	push r20
	push r21
	push r22

	ldi r22, 0xFF
loopL2:
	ldi r21, 0xFF
loopL1:
	ldi r20, 0xFF
loopL0:
	dec r20
	brne loopL0
	dec r21
	brne LoopL1
	dec r22
	brne LoopL2

	pop r22
	pop r21
	pop r20
	ret
;**********************



