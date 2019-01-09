;**************************************
;** Компьютерный центр FLASH
;** Компьютерная грамота в Харцызске
;** г. Харцызск, пер. Шмидта, 2
;**
;**(здание ООО "Данко" - бывшая книжная база, рядом ГорГАИ, ОШ №7, "Танк")
;**
;** Менеджер по работе с клиентами Павел
;**
;** mob: 095-725-20-14
;**
;** mob: 099-490-69-45
;**
;** mail: info@imkoteh.com
;**************************************
;**************************************
;**	Интернет-магазин "ИМКОТЕХ"
;** Компьютерная техника в Харцызске
;** г. Харцызск, пер. Шмидта, 2
;**
;**(здание ООО "Данко" - бывшая книжная база, рядом ГорГАИ, ОШ №7, "Танк")
;**
;** Менеджер по работе с клиентами Александр
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
; подключение библиотечных файлов
#include"m8def.inc"
.list
; Директива DEVICE позволяет указать для какого устройства компилируется программа.
; При использовании данной директивы компилятор выдаст предупреждение, если будет 
; найдена инструкция, которую не поддерживает данный микроконтроллер. Также будет выдано
; предупреждение, если программный сегмент, либо сегмент EEPROM превысят размер допускаемый устройством. 
; Если же директива не используется то все инструкции считаются допустимыми, и отсутствуют ограничения на размер сегментов.
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
.org 0x009 reti;rjmp TIM0_OVF ; Timer0 Overflow Handler
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
.def tmp3	= r22	;пользовательский 1 регистр
.def DataL	= r23	;регистр данных(младший)
.def DataH	= r24	;регистр данных(старший)
.def cnt1	= r25	;главный счетчик регистр
.def Flags	= r26	;регистр флагов

// организуем подключение всех файлов проекта
#include"DSEG.inc"		;файл резервирования переменных в ОЗУ и инициализация констант и таблиц
#include"EEPROM.inc"	;файл работы с энергонезависимой памятью
#include"RESET.inc" 	;файл инициализации(сброса)
#include"EXT_INT.inc"	;файл обработки внешних прерываний
#include"TIM0.inc"		;файл работы с Таймером0
#include"ANA_COMP.inc"	;файл работы с Аналоговым компоратором
#include"Delay.inc"		;файл организации задержек
#include"SubRouters.inc"

//организуем бесконечный цикл
main:	
; тут распологаем все команды нашей программы	


;	rjmp test_main
	rjmp work_indicator
	rjmp main



;**************************************
; работа с семисегментным индикатором
; поочередно выводим все цифры с 0 по 9
; для работы с индикатором используем таблицу соответствия цифр-коду
;
;
;
work_indicator:
;-------------------------- Инициализация Порта ввода/вывода B

		ldi		r16, 0xFF	; Записываем число $FF в регистр r16
		out		DDRB, r16	; Записываем это число в DDRB
		ldi		r16, 0x00	; Записываем число $00 в регистр r16
		out		PORTB, r16	; Записываем то  же число в PORTB
; проинициализировали порт В на вывод и погасили все светики для индикатора с общим катодом

;-------------------------- Инициализация Порта ввода/вывода D

		ldi		r16, 0xC0	; Записываем число $С0 или 0b11000000 в регистр r16
		out		DDRD, r16	; Записываем это число в DDRD
		ldi		r16, 0x00	; Записываем число $00 в регистр r16
		out		PORTD, r16	; Записываем то  же число в PORTD
; проинициализировали порт D для контроля индикатор(когда гореть и какой цифре из двух)

; теперь организуем бесконечный цикл для вывода всех цифр
	ldi r17,-1
work_ind:
	inc r17	
	cpi r17,10
	brne normal
	clr r17
normal:
; проведем выбор кода для соответствующей цифры
	LDI ZH,High(2*TableIndicator)
	LDI ZL,Low(2*TableIndicator)
	add ZL,r17
	clr r18
	adc ZH, r18
	lpm r18,Z

	out PORTB, r18
; для того чтоб цифра отобразилась на индикаторе необходимо зажечь её
	sbi PORTD, PD7

	rcall DelayLong
	cbi PORTD, PD7

	rjmp work_ind


;**************************************
; тест МК для точной подстройки частоты
; организуем меандр в 20 тактов
; 10 тактов высокий уровень
; 10 тактов низкий уровень
test_main:
	cli
	ldi r16, 0xFF
	out DDRB, r16
	out PORTB, r16
	nop
	nop
	nop
	nop
	nop
	nop
	ldi r16, 0xFF
	out DDRB, r16
	clr r16
	out PORTB, r16
	nop
	nop
	nop
	nop
		rjmp test_main
