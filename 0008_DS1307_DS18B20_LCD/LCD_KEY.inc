;==============================================================================
;Начало основной программы
;==============================================================================
initLCD_KEY:
	
	in		R16,DATA_DDR
	ori		R16,(1<<CLK|1<<DATA)	
	out		DATA_DDR,R16				; Настроим порты как выход			
	
	in		R16,LCD_DDR
	ori		R16,(1<<A0|1<<EN)	
	out		LCD_DDR,R16				; Настроим порты как выход			
	
	in		R16,KEY_PORT
	ori		R16,(1<<B_RET1)
	out		KEY_PORT,R16				;Включим подтягивающий резистор к порту возврата кнопок

	rcall	initlcd				;Инициализируем LCD
	cbi		LCD_PORT,A0			;Установим передачу данных	
	ldi		R16,0b00000001			;Очистим дисплей
	rcall	load_555ir8
	ldi		R18,2				;задержка в 2мс
	rcall	delay_ms

	ldi		zl,Low(RusAbout*2)		;Укажем адрес начальной строки в памяти
	ldi		zh,High(RusAbout*2)	
	ldi		R16,$80					;Укажем что выводим на 1 строку
	rcall	String_To_LCD_cseg		;Выводим на 1 строку начальную запись

	ldi		zl,Low(RusAbout*2+8)	;Укажем адрес начальной строки в памяти
	ldi		zh,High(RusAbout*2+8)	
	ldi		R16,$C0					;Укажем что выводим на 2 строку
	rcall	String_To_LCD_cseg		;Выводим на 2 строку начальную запись

	ret
	
	;Завершение блока инициализации устройства

;***********************

;===========================================================
;Процедура сканирования клавиатуры
;Выход: a - код нажатых кнопки (a=0 - не было нажатий кнопок)
;===========================================================
Scan_Keyb:
	Push	R19				;Сохраняем задействованные регистры в стек
	Push	R18
	Push	R17

	ldi		R19,8				;Устанавливаем что сканируем 8 кнопок
	clr		R17				;Очищаем временный регистр для хранения кода нажатых кнопок
	ldi		R16,$FE				;Устанавливаем начальную позицию для сканирования кнопок
sc0:							;бегущим нулем
	Push	R16				;Сохраняем текущее состояние регистра в стек
							;Необходимо для расчета кода кнопки в случае ее нажатия
	ldi		R18,8				;Устанавливаем 8 бит для вывода
sc1:
	cbi		DATA_PORT,CLK			;CLK=0
	rol		R16				;Сдвигаем содержимое a влево
	brcs	sc3				;Если левый бит равен 1 то переход на sc3
	cbi		DATA_PORT,DATA			;DATA=0
	rjmp	sc4				;
sc3:
	sbi		DATA_PORT,DATA			;DATA=1
sc4:
	sbi		DATA_PORT,CLK			;CLK=1

	dec		R18				;Следующий бит
	brne	sc1				;Если не все вывели, то переход на sc1
	cbi		DATA_PORT,CLK			;CLK=0

	Pop		R16				;Востанавливаем содержимое а из стека
	sbic	KEY_PIN,B_RET1			;проверяем была ли нажата какая-нибудь кнопка
	rjmp	sc5				;если нет то переход по метке
	Push	R16				;Сохраняем а в стек
	com		R16				;a = a - $FF
	add		R17,R16				;складываем код кнопки с другими кодами нажатых кнопок
	Pop		R16				;Востанавливаем а из стека
sc5:
	sec						;Устанавливаем бит переноса в 1
	rol		R16				;cмещаем биты а влева с учетом бита переноса
	dec		R19				;Проверяем просканировали все 8 кнопок?
	brne	sc0				;если нет, то переход по метке
	mov		R16,R17				;Переносим код нажатых кнопок в выходной регистр
							;из временного регистра	
	Pop		R17			;Востанавливаем все задействованные регистры из стека
	Pop		R18
	Pop		R19
	ret

;===============================================
;Процедура выводит данные на дисплей из DSEG
;Вход - Z ссылка на начало строки
;Вход a - строка вывода ($80 - первая строка)
;			($C0 - вторая строка)
;================================================
String_to_LCD:
	Push		zl
	Push		zh
	Push		R18
	Push		R17
	Push		R16

	cbi		LCD_PORT,A0			;Установим передачу команд	
	rcall	load_555ir8			;Установим место вывода на LCD
	ldi		R18,39
	rcall	delay_mcs
	sbi		LCD_PORT,A0			;Установим передачу данных
							;Выведем на дисплей знак
	ldi		R17,8
Str1:
	ld		R16,Z+
	rcall	load_555ir8
	ldi		R18,39
	rcall	delay_mcs
	dec		R17
	brne	Str1				;Если не все вывели, то переход на Str1

	Pop		R16
	Pop		R17
	Pop		R18
	Pop		zh
	Pop		zl
	ret

;===============================================
;Процедура выводит данные на дисплей из CSEG
;Вход - Z ссылка на начало строки
;Вход a - строка вывода ($80 - первая строка)
;			($C0 - вторая строка)
;================================================
String_to_LCD_cseg:
	Push	zl
	Push	zh
	Push	R18
	Push	R17
	Push	R16

	cbi		LCD_PORT,A0			;Установим передачу команд	
	rcall	load_555ir8			;Установим место вывода на LCD
	ldi		R18,39
	rcall	delay_mcs
	sbi		LCD_PORT,A0			;Установим передачу данных
							;Выведем на дисплей знак
	ldi		R17,8
Str11:
	lpm		R16,Z+
	rcall	load_555ir8
	ldi		R18,39
	rcall	delay_mcs
	dec		R17
	brne	Str11				;Если не все вывели, то переход на Str1

	Pop		R16
	Pop		R17
	Pop		R18
	Pop		zh
	Pop		zl
	ret
;*****************

;lds r16,cntDriver
;lds r17,cntDriver+1
;ldi r18,$CF
;rcall OutLCD_VAR

OutLCD_VAR:
push yl
push yh
push r16
push r17

	ldi yl,low(TmpVar_LCD)
	ldi yh,high(TmpVar_LCD)
	
	rcall NumberTC

	ldi		zl,Low(TmpVar_LCD)		;Укажем адрес начальной строки в памяти
	ldi		zh,High(TmpVar_LCD)	
;	ldi		R16,$CF				;Укажем что выводим на 2 строку
	mov r16,r18
	rcall	String_To_LCD		;Выводим на 2 строку начальную запись

pop r17
pop r16
pop yh
pop yl

ret

;***********

;ldi r19,10
;rcall OutLCD_Metka

OutLCD_Metka:
push zl
push zh
	ldi zl,Low(KeyMetca*2)
	ldi zh,High(KeyMetca*2)
	ldi r18,4
; добавим смещение
	mul r18,r19
	
	add zl,r0
	adc zh,r1
	
	ldi		R16,$04;				;Укажем что выводим на 1 строку
	rcall	String_To_LCD_cseg		;Выводим на 1 строку начальную запись

pop zh
pop zl

ret

;==============================================================================
;Процедура инициализации ЖКИ
;==============================================================================
initlcd:
	cbi		LCD_PORT,A0			;Установка A0=0
	cbi		LCD_PORT,EN				;Установка E=0
	ldi		R18,20				;Ждать 20мс
	rcall	delay_ms			;
	
	ldi		R16,0b00110000			;
	rcall 	load_555ir8			;
	ldi		R18,5				;Ждать 5 ms
	rcall	delay_ms			;

	ldi		R16,0b00110000			;
	rcall 	load_555ir8			;
	ldi		R18,100				;Ждать 100 mcs
	rcall	delay_mcs
	
	ldi		R16,0b00110000			;
	rcall 	load_555ir8			;
	ldi		R18,200;40				;Ждать 40 mcs
	rcall	delay_mcs

	ldi		R16,0b00111000			;
	rcall 	load_555ir8			;
	ldi		R18,200;40				;Ждать 40 mcs
	rcall 	delay_mcs			;

	ldi		R16,0b00001000			;
	rcall 	load_555ir8			;
	ldi		R18,200;40				;Ждать 40 mcs
	rcall	delay_mcs

	ldi		R16,0b00000001			;
	rcall 	load_555ir8			;
	ldi		R18,2				;Ждать 2 ms
	rcall	delay_ms

	ldi		R16,0b00000110			;
	rcall	load_555ir8			;
	ldi		R18,200;40				;Ждать 40 mcs
	rcall	delay_mcs
;--------------------------------------------------------------------------------
	ldi		R16,0b00001100			;
	rcall 	load_555ir8			;
	ldi		R18,200;40				;Ждать 40 mcs
	rcall	delay_mcs

	ldi		R16,0b10000000		;Установка адреса для начала вывода (1сим.1стр.)
	rcall	load_555ir8			;
	ldi		R18,40				;Ждать 40 mcs
	rcall	delay_mcs

	ret

;==============================================================================
;Процедура выводит байт в 555ИР8(74HC164) Вход: A - байт для вывода
;==============================================================================
load_555ir8:
	push	R16
	push	R18
	push 	R19
	ldi		R19,8			;Устанавливаем 8 байт для вывода
ir8_1:
	cbi		DATA_PORT,CLK		;CLK=0
nop
nop
	rol		R16				;Сдвигаем содержимое a влево
	brcs	ir8_3			;Если левый бит равен 1 то переход на ir8_3
	cbi		DATA_PORT,DATA		;DATA=0
	rjmp	ir8_4				;
ir8_3:
	sbi		DATA_PORT,DATA		;DATA=1
ir8_4:
nop
nop
nop
nop
	sbi		DATA_PORT,CLK		;CLK=1
	dec		R19				;Следующий бит
	brne	ir8_1			;Если не все вывели, то переход на ir8_1
	cbi		DATA_PORT,CLK		;CLK=0
nop
nop
	sbi		LCD_PORT,EN		;Установка E=1
nop
nop
nop
nop
nop
nop
	cbi		LCD_PORT,EN		;Установка E=0

	pop 	R19
	pop		R18
	pop		R16
	ret
;====================================================
