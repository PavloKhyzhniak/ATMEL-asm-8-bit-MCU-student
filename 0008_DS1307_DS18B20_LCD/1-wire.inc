;-----------подпрограммы для датчика температуры
;организация задержки при 16МГц
;формула такая (число повторов) *1,88+0,44=(время задержки)
; определено экспериментально
; по трем уравнения при числе повторов= 1 2 и 10 :)
;Delay:
;		ldi temp3, 9
;del1:
;		dec temp3
;		brne del1
;	
;		dec cnt3
;		brne Delay
;		ret


; берем только целую часть температуры
; и компонуем ее в один байт
nibleRun:
		lds temp, Temperature
		swap temp
		andi temp,0x0F
		mov temp1,temp
		lds temp, Temperature+1
		swap temp
		andi temp, 0xF0
		add temp, temp1
		sts LCD_Temperature,temp
		ret


.MACRO DS18B20

		rcall setTemperature
		rcall readTemperature
		
		rcall nibleRun

.ENDMACRO


OutLCD_Temperature:

push yl
push yh
push r16
push r17

	ldi yl,low(TmpVar_LCD)
	ldi yh,high(TmpVar_LCD)
	
	lds r16, LCD_Temperature
	clr r17
	rcall Number99

	ldi r16,$20
	st y+,r16
	ldi r16,'C'
	st y+,r16
	ldi r16,$20
	st y+,r16
	st y+,r16
	st y+,r16
	st y+,r16


	ldi		zl,Low(TmpVar_LCD)		;Укажем адрес начальной строки в памяти
	ldi		zh,High(TmpVar_LCD)	
	ldi		R16,$80				;Укажем что выводим на 2 строку
	rcall	String_To_LCD		;Выводим на 2 строку начальную запись

pop r17
pop r16
pop yh
pop yl

	ret


;-------------------------- Подпрограммы 1-Wire интерфейса
.dseg

LCD_Temperature:	.BYTE 1
Temperature:	.BYTE 2


.cseg

.equ tempU = 0
.set tempDQ = PC0
.set DQ_PORT = PORTC
.set DQ_PIN = PINC
.set DQ_DDR = DDRC

;-------------------------- Система команд микросхемы DS18B20

;---------- Команды сетевого уровня

; 0F0H	поиск ПЗУ
; 033H	чтение ПЗУ
; 055H	совпадение ПЗУ
; 0CCH	пропуск ПЗУ
; 0ECH	поиск прерывания

;--------- чтение ПЗУ
/*
readROM:
		rcall resetLAN
		cpi error, 0
		brne rdROM2
		ldi temp, $33
		rcall writeByteLAN
		ldi zh, high(bufLAN*2)
		ldi zl, low(bufLAN*2)
		ldi temp1, 8
rdROM:	
		rcall readByteLAN
		st z, temp
		ldi temp,1
		add zl,temp
		clr temp
		adc zh, temp
		dec temp1
		brne rdROM
		ret
rdROM2:
		rjmp readROM
*/
;---------- Команды транспортного уровня

; 04EH	запись блокнотной памяти
; 0BEH	чтение блокнотной памяти
; 0B4H	чтение режима питания
; 0B8H	чтение из EEPROM в блокнотную память
; 048H	копирование блокнотной памяти в EEPROM
; 044H	запуск процесса преобразования

;----------- чтение температуры
readTemperature:
		rcall resetLAN
		cpi error,0
		brne rdT6
		ldi temp, $CC; пропуск ПЗУ
		rcall writeByteLAN
		ldi temp, $BE; чтение блокнотной памяти
		rcall writeByteLAN

		rcall readByteLAN
		sts Temperature,temp

		rcall readByteLAN
		sts Temperature+1,temp
		ret
rdT6:
ret
		cpi error, 1
		brne readTemperature
		rjmp readTemperature;ret

;----------- запуск процесса преобразования
setTemperature:
; запрос режима питания
		rcall resetLAN
		cpi error, 0
		brne setT4
		ldi temp, $CC; пропуск ПЗУ
		rcall writeByteLAN
		ldi temp, $B4; чтение режима питания
		rcall writeByteLAN
		rcall readBitLAN
		brcc setT1
		ldi cnt1,1
		rjmp setT2
setT1:
		ldi cnt1,0
setT2:
; запуск процесса преобразования
		rcall resetLAN
		cpi error,0
		brne setT2
		ldi temp, $CC; пропуск ПЗУ
		rcall writeByteLAN
		ldi temp, $44;запуск процесса преобразования
		rcall writeByteLAN
		cpi cnt1,0
		brne setT3
; ожидание в режиме питания по линии
		sbi DQ_PORT, tempDQ
		nop
		cbi DQ_PORT, tempU
		; задержка в 765 мксек.
	ldi		R18,255				;задержка в 255мкс
	rcall	delay_mcs
	ldi		R18,255				;задержка в 255мкс
	rcall	delay_mcs
	ldi		R18,255				;задержка в 255мкс
	rcall	delay_mcs

		sbi DQ_PORT, tempU
		ret
; ожидание в режиме внешнего питания
setT3:
		rcall readBitLAN
		brcc setT3
		ret		
setT4:
ret
		cpi error, 1
		brne setTemperature
		rjmp setTemperature;ret


;-------------------------- Подпрограммы 1-Wire интерфейса (общий пакет)

; подпрограмма чтения бита
readBitLAN:
;		cli
		rcall readSlot
;		sei
		ret

; подпрограмма чтения байта
readByteLAN:
		cli
		ldi temp1, 8
rd8LAN1:
		rcall readSlot
		ror temp
		dec temp1
		brne rd8LAN1
		sei
		ret

; подпрограмма записи бита
writeBitLAN:
;		cli
		brcs wr1LAN1
		rcall writeLow
		sei 
		ret
wr1LAN1:
		rcall writeHigh
;		sei
		ret

; подпрограмма записи байта
writeByteLAN:
		cli
		ldi temp1,8
wr8LAN1:
		ror temp
		brcs wr8LAN2
		rcall writeLow
		rjmp wr8LAN3
wr8LAN2:
		rcall writeHigh
wr8LAN3:
		dec temp1
		brne wr8LAN1
		sei
		ret

;-------------------------- Подпрограммы 1-Wire интерфейса (низкий уровень)

; подпрограмма записи 0 в слот
; выдача мастером 0 на линию
writeLow:
		sbi DQ_DDR, tempDQ
		cbi DQ_PORT, tempDQ

		; задержка в 90 мксек.
		ldi		R18,90				;задержка в 90мкс
		rcall	delay_mcs
	
		sbi DQ_PORT, tempDQ

		; задержка в 5 мксек.
		ldi		R18,5 			;задержка в 5мкс
		rcall	delay_mcs

		ret

; подпрограмма записи 1 в слот
; выдача мастером 1 на линию
writeHigh:
		sbi DQ_DDR, tempDQ
		cbi DQ_PORT, tempDQ

		; задержка в 5 мксек.
		ldi		R18,5				;задержка в 5мкс
		rcall	delay_mcs

		sbi DQ_PORT, tempDQ
		; задержка в 90 мксек.
		ldi		R18,90				;задержка в 90мкс
		rcall	delay_mcs
	
		ret

; подпрограмма чтения слота слэйвом
; результат чтения в признаке переноса С
readSlot:
		sbi DQ_DDR, tempDQ
		cbi DQ_PORT, tempDQ

		; задержка в 5 мксек.
		ldi		R18,5				;задержка в 5мкс
		rcall	delay_mcs

		cbi DQ_DDR, tempDQ
		
		; задержка в 20 мксек.
		ldi		R18,20				;задержка в 20мкс
		rcall	delay_mcs

		clc
		sbic DQ_PIN, tempDQ
		sec

		; задержка в 35 мксек.
		ldi		R18,35				;задержка в 35мкс
		rcall	delay_mcs
		ret

; подпрограмма сброса линии
; сброс - 0 в течении >500 мксек мастером
; потом через 60 мксек ответ 0 слэйвом 
; и через >500 мксек линия чиста - 1
resetLAN:
		; сброс линии мастером
		sbi DQ_DDR, tempDQ
		cbi DQ_PORT, tempDQ
		; задержка в 700 мксек.
		ldi		R18,250				;задержка в 250мкс
		rcall	delay_mcs
		ldi		R18,250				;задержка в 250мкс
		rcall	delay_mcs
		ldi		R18,200				;задержка в 200мкс
		rcall	delay_mcs

		; отпускаем линии
		sbi DQ_PORT, tempDQ
		cbi DQ_DDR, tempDQ
		; задержка в 60 мксек.
		ldi		R18,60				;задержка в 60мкс
		rcall	delay_mcs

		; ловим ответ от слэйвов
		;sbic PORTB, tempDQ2
		sbic DQ_PIN, tempDQ
		rjmp Er1
		; задержка в 750 мксек.
		ldi		R18,250				;задержка в 250мкс
		rcall	delay_mcs
		ldi		R18,250				;задержка в 250мкс
		rcall	delay_mcs
		ldi		R18,250				;задержка в 250мкс
		rcall	delay_mcs

		; все слэйвы должны окончить сброс линии
		; и отпустить линию
		sbis DQ_PORT, tempDQ
		rjmp Er2
		clr error	; нормальная работа
		ret
Er2:	; КЗ линии
		ldi error, 2
		ret
Er1:	; нет датчика
		ldi error, 1
		ret
