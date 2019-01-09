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
; подключение библиотечных файлов
#include"m8def.inc"
.list

.device ATmega8

; описание вектора прерываний

; выбор сегмента памяти программ
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


;описание всех РОНов
.def tmpL	= r16	;главный рабочий регистр(младший)
.def tmpH	= r17	;главный рабочий регистр(старший)
.def tmpL2	= r18	;вспомогательный рабочий регистр(младший)
.def tmpH2	= r19	;вспомогательный рабочий регистр(старший)
.def tmp1	= r20	;пользовательский 1 регистр
.def tmp2	= r21	;пользовательский 1 регистр
.def Flags	= r22	;регистр флагов
.def DataL	= r23	;регистр данных(младший)
.def DataH	= r24	;регистр данных(старший)
.def cnt1	= r25	;главный счетчик регистр

.def	cntADC = r15 ;какой замер проводим
.def	cntLED = r14 ;какой разряд выводим

// организуем подключение всех файлов проекта
#include"DSEG.inc"		;файл резервирования переменных в ОЗУ и инициализация констант и таблиц
#include"EEPROM.inc"	;файл работы с энергонезависимой памятью
#include"RESET.inc" 	;файл инициализации(сброса)
;#include"EXT_INT.inc"	;файл обработки внешних прерываний
;#include"PCINT.inc"		;файл работы с прерываниями ввода/вывода
#include"TIM0.inc"		;файл работы с Таймером0
#include"ADC_INT.inc"	;файл работы с АЦП
;#include"ANA_COMP.inc"	;файл работы с Аналоговым компоратором

#include"Delay.inc"		;файл организации задержек
#include"SubRouters.inc";файл некоторых ходовых подпрограмм


//организуем бесконечный цикл
main:
; запуск АЦП
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
