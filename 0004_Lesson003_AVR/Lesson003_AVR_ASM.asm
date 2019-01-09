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

; Файлы подключения подпрограмм уроков
#include"Lesson001_IO.inc"
#include"Lesson002_AnaComp.inc"
#include"Lesson003_TIM0.inc"


//организуем бесконечный цикл
main:
; тут распологаем все команды нашей программы	
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
;** Пример управлением мощной нагрузкой
;***********************

; настроим аналоговый компаратор
; разрешим его работы и разрешим прерывания от него
	ldi r16, (0<<ACD)|(1<<ACIE)
	out ACSR, r16
; настроим выводы компаратора для приема сигналов
	cbi DDRD,PD6
	cbi PORTD,PD6

	cbi DDRD,PD7
	cbi PORTD,PD7
	
	rjmp PC-1

TIM0_OVF:
rjmp DAC_Sinus
; организуем переинициализацию нашего таймера
; будем вызывать прерывание через 100 тиков по 1024 тика
	ldi r16,-2
	out TCNT0,r16

	ldi r16,0xFF
	out DDRD,r16
	inc r17
	out PORTD,r17
; что значит такое прерывание через 100 тиков по 1024???
; подсчитаем при частоте в 1 МГц каждый тик 1 мксек
; тогда 100*1024*1мкс = 102,4 мсек
; то есть ~10 раз в секунду

; тут можно разместить необходимый код

	reti

; enable Ava_Comp
		in r16, ACSR
		andi r16,~(1<<ACD)
		out ACSR, r16
	
; загрузим из ОЗУ константу угла открывания
	lds r16,Power
	inc r16
; организуем сравнение со 160
; инкремент производим 160 раз
; сама константа нам нужна только  от 0 до 10
; но для красоты и плавности будем использовать каждую константу по 16 раз
; то есть для 8 периодов сетевого напряжения
	cpi r16,160
	brlo PC+2
	clr r16
	sts Power,r16

; disable Timer0
; выключим таймер, ждем следующего срабатывания компоратора
	in r16,TIMSK
	cbr r16,0<<TOIE0
	out TIMSK, r16
	clr r16
	out TCCR0, r16

; impuls
; выведем импульс на включение или открытие симисторного ключа	
	sbi DDRD,PD5
	sbi PORTD,PD5
	ldi r19,1
	rcall Delay
	cbi PORTD,PD5

	reti

;Подпрограмма инициализации Таймера0
TIM0_Init:
; настроим предделитель таймера 0
	ldi r16,(1<<CS00)|(0<<CS01);|(1<<CS02)
	out TCCR0,r16
; запишем начальное значение в таймер0 
; будем вызывать прерывание через 100 тиков по 1024 тика
	ldi r16,-100
	out TCNT0,r16
; разрешим прерывание от Таймера 0 по переполнению
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
;флаг 1 указывает на возростание или убывание синуса
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


;сохранение указателя градуса и флагов
sts SinFlags,r20
sts Sinus,r16

;считывание 
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
;Подпрограмма инициализации аналогового компоратора
;
;Подпрограмма работы по аналоговому компоратору
;Будем инвертировать выход каждый раз при вызове подпрограммы прерывания от компоратора
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


