.include "m8def.inc"
;--------------------------------------------- 
.include "ProgVar.inc"
.CSEG                   ; Начало сегмента кода программ 
.include "ProgDef.inc"
.include "macro.inc"
.org 0
	jmp	reset

;.org	INT0addr
;	jmp	Int0Ext; Прерывание от Int0
.org	OC1Aaddr
;	jmp	IntTimer1; Прерывание от сравнения таймера 1
.org	ADCCaddr;
	jmp	IntADC; Прерывание от АЦП
.org	TWIaddr
	reti
	
;--------MonUart-------------------------------
	.org	URXCaddr
	jmp	ReadU
;----------------------------------------------
.org	$30	;Начало области программ
.include "PrUart.inc"
.include "I2C_8m.inc"
.include "ADC.inc"
.include "SPI.inc"

reset:
	ldi	temp,high(RAMEND) ;Установить указатель стека на верх ОЗУ 
	out	SPH,temp	         ;
	ldi	temp,low(RAMEND)	 
	out	SPL,temp
;;;;	jmp	ReadADC3;!!!!!!!!!!!!!!!!!!!!!!!
	rcall	Ini_Ports
	rcall	ClearRAM
	rcall	Ini_Prog

	in	temp,MCUCR
	cbr	temp,(1<<PUD) ;Включаем поддтягивающие резисторы
	sbr	temp,(1<<SE)  ;Разрешаем использовать SLEEP режим
	cbr	temp,(1<<SM2) ;Уменьшение шума АЦП
	cbr	temp,(1<<SM1) ;
	cbr	temp,(1<<SM0) ;-------------------
	out	MCUCR,temp

;------- MonUart ------------------------------
	rcall	Ini_MonUart
;----------------------------------------------
	;call	InitTimer1
	sei	;Глобальное рарешение прерываний

	rcall	Ini_I2C
	rcall	Ini_Clock
	rcall	InitADC
	rcall	StartADC
	;ldi	temp,0x30
	;mov	r15,temp	;!!!!!
	Pause10ms	1
	rcall	ReadIndexClockRAM
	rcall	Ini_SPI
	rcall	Ini_MMC
	rcall	ReadInfoMMC
	;rcall	ClearIndexClockRAM;!!!!
	rcall	WhiteStartOk1Upit;При отсутсвии напряжения с транса-сервис режим
;-----------------------------------------
;	Pause10ms	250
;	ldi	temp4,250
;LoopRead200:
;	push	temp4
;	rcall	ReadPlusMMC
;	pop	temp4
;	dec	temp4
;	jnz	LoopRead200	
;	Pause10ms	250
;	Pause10ms	250
;
;	ldi	temp4,250
;LoopWrite200:
;	push	temp4
;	rcall	WritePlusMMC
;	pop	temp4
;	dec	temp4
;	jnz	LoopWrite200	
;	Pause10ms	250
;	Pause10ms	250
;
;	Pause10ms	250
;	Pause10ms	250
;	Pause10ms	250
;	Pause10ms	250
;	Pause10ms	250
;-----------------------------------------

MainLoop:
	;inc	r15		;!!!!!!
	sleep          ;Переходим в спящий режим
	rcall	TestNewCloc
	lds	temp,FlagNewS
	cpi	temp,1
	jnz	MainLoop
MainLoop1:
	lds	temp,NumADCkan
	cpi	temp,0
	jnz	MainLoop1
	mov	temp,CountVibInPer
	cpi	temp,0
	jnz	MainLoop1	;ожидание конца периода

	cbi	SPI_port,SPI_LED	;Включаем светодиод
	rcall	GetMemADC
	sbi	SPI_port,SPI_LED	;Выключаем светодиод
	lds	temp,AdrUstr
	;cpi	temp,$FD	;FD - включение непрерывной посылки данных
	;jnz	NoPutRAWSbuf
	SBIS	PIND,4
	rcall	PutSbuf
NoPutRAWSbuf:
	;lds	temp,ON_WriteADC_MMC
	;cpi	temp,0
	;jz	OK_WriteADC_MMC
	SBIC	PIND,3
	jmp	OK_WriteADC_MMC
	ldi	temp,0
	sts	CountSec,temp	;Обнуляем буфер, если запрещена запись ADC в MMC
	rjmp	Not15Sec
OK_WriteADC_MMC:
	lds	temp,CountSec
	cpi	temp,0
	jne	NotNewBlok
	ldi	yl,low(Buf512)
	ldi	yh,high(Buf512)
	sts	IndIn512,yl
	sts	IndIn512+1,yh
	ldi	temp,$FF
	ldi	temp2,128
LoopSetFF512:
	st	Y+,temp
	st	Y+,temp
	st	Y+,temp
	st	Y+,temp
	dec	temp2
	jnz	LoopSetFF512
NotNewBlok:

	rcall	PutTime512
	rcall	PutADC512
	lds	temp,CountSec
	inc	temp
	sts	CountSec,temp
	cpi	temp,8	;8 секунд в блоке
	jc	Not15Sec
	rcall	WhiteOk1Upit
	rcall	WritePlusMMC
	lds	temp,Err_MMC
	cpi	temp,0
	jz	Ok8WritePlusMMC
	rcall	Ini_SPI		;Заново инициализируем MMC
	rcall	Ini_MMC		;
	rcall	ReadInfoMMC	;
Ok8WritePlusMMC:
	;rcall	WhiteOk1Upit
	rcall	WriteIndexClockRAM	;Сохраняем индекс на случай отключения
	ldi	temp,0
	sts	CountSec,temp
Not15Sec:
	rcall	WhiteOk1Upit
	rcall	ReadClocks
	rjmp	MainLoop

WhiteOk1Upit:		;Ожидание нормального напряжения
	SBIS	PINB,0
	rjmp	WhiteOk1Upit	
	ret

WhiteStartOk1Upit:		;Ожидание нормального напряжения при старте
	SBIC	PINB,0
	rjmp	WhiteStartOk1UpitEx
	Pause10ms	10	;Пауза 0.1 секунду
	rcall	ReadClocks
	rjmp	WhiteStartOk1Upit
WhiteStartOk1UpitEx:	
	ret



PutTime512:
	ldi	xl,low(Secund)
	ldi	xh,high(Secund)
	lds	yl,IndIn512
	lds	yh,IndIn512+1
	ld	temp,X+
	st	Y+,temp	
	ld	temp,X+
	st	Y+,temp	
	ld	temp,X+
	st	Y+,temp	
	ld	temp,X+
	ld	temp,X+
	st	Y+,temp	
	ld	temp,X+
	st	Y+,temp	
	ld	temp,X+
	st	Y+,temp	
	sts	IndIn512,yl
	sts	IndIn512+1,yh
	ret

PutADC512:
	ldi	xl,low(_CountADCvib)
	ldi	xh,high(_CountADCvib)
	lds	yl,IndIn512
	lds	yh,IndIn512+1
	ldi	temp3,_EndData-_CountADCvib
LoopCopy512:
	ld	temp,X+
	st	Y+,temp
	dec	temp3
	jnz	LoopCopy512
	ldi	temp2,0
	sub	temp2,yl
	ANDI	temp2,63
	jz	NoNulPutADC512
	ldi	temp,0
LoopNulPutADC512:
	st	Y+,temp
	dec	temp2
	jnz	LoopNulPutADC512
NoNulPutADC512:
	sts	IndIn512,yl
	sts	IndIn512+1,yh
	ret

PutSbuf:
	ldi	temp,$A5
	rcall	PutChU
	ldi	temp,_EndData-_CountADCvib+1+8
	ldi	temp4,$A5
	add	temp4,temp
	rcall	PutChU

	ldi	temp3,8
	ldi	xl,low(Secund)
	ldi	xh,high(Secund)
LoopPutSbuf0:
	ld	temp,X+
	add	temp4,temp
	rcall	PutChU
	dec	temp3
	jnz	LoopPutSbuf0

	ldi	temp3,_EndData-_CountADCvib
	ldi	xl,low(_CountADCvib)
	ldi	xh,high(_CountADCvib)
LoopPutSbuf:
	ld	temp,X+
	add	temp4,temp
	rcall	PutChU
	dec	temp3
	jnz	LoopPutSbuf
	mov	temp,temp4
	rcall	PutChU
	ret

GetMemADC:
	;sts	C_Vib_P,CountVibInPer
	ldi	xl,low(CountADCvib)
	ldi	xh,high(CountADCvib)
	ldi	yl,low(_CountADCvib)
	ldi	yh,high(_CountADCvib)
	ldi	temp2,0
	ldi	temp3,_EndData-_CountADCvib
LoopClear:
	ld	temp,X
	st	Y+,temp
	st	X+,temp2
	dec	temp3
	jnz	LoopClear
	ret

TestNewCloc:
	in	temp,PIND
	lds	temp2,OldPortD
	sts	OldPortD,temp
	SBRC	temp,2
	rjmp	NoReadClock
	SBRS	temp2,2
	rjmp	NoReadClock
	ldi	temp,1	;Сменилась секунда
	sts	FlagNewS,temp
	ret
NoReadClock:
	ldi	temp,0	;Нет новой секунды
	ret

ReadClocks:
	ldi	temp,1
	sts	NewTime,temp
	ldi	temp,0
	sts	FlagNewS,temp
	;inc	r14	;!!!!!!!!!!!!!
	rcall	ReadClock
	ldi	temp2,0
	cli
	lds	temp,NewTime
	sts	NewTime,temp2
	sei
	cpi	temp,2
	jnz	NoNewTimeSet
	rcall	SetClock
NoNewTimeSet:
	ret

ClearRAM:	;Для памяти 1Кбайт
	ldi	xl,$60
	ldi	xh,0
	ldi	temp,0
	ldi	temp2,$ff
LoopClearRAM:
	st	X+,temp
	st	X+,temp
	st	X+,temp
	st	X+,temp
	dec	temp2
	jnz	LoopClearRAM
	ldi	zl,0
	ldi	zh,0
	ldi	yh,16+13
LoopClearRAM2:
	st	Z+,temp
	dec	yh
	jnz	LoopClearRAM2
	ldi	yh,0
	ldi	zl,0
	ldi	zh,0
	ret

Ini_Ports:	;Инициализация портов при старте
	ldi	temp,0	;Порт на ввод	
	out	DDRC,temp
	out	PORTC,temp

	ldi	temp,0	
	out	DDRB,temp
	out	PORTB,temp

	ldi	temp,0	
	out	DDRD,temp
	ldi	temp,$3F	
	out	PORTD,temp

	cbi	DDRD,0	;Настраеваем порты для COM
	sbi	PORTD,0	;Подтягиваем порт резистором к +5В - убрать ложные прерывания
	sbi	DDRD,1
	sbi	PORTD,1

	sbi	SPI_port,SPI_SS
	sbi	SPI_dir,SPI_SS
	sbi	SPI_port,SPI_MOSI
	sbi	SPI_dir,SPI_MOSI
	cbi	SPI_port,SPI_SCK
	sbi	SPI_dir,SPI_SCK
	sbi	SPI_port,SPI_LED	;Настраиваем светодиод
	sbi	SPI_dir,SPI_LED

	ret

Ini_Prog:	;Настройка контроллера при запуске
	ldi	temp,0
	sts	NewTime,temp
	sts	OldPortD,temp
	sts	FlagNewS,temp
	ldi	temp,low(BeginSecWr)
	sts	AdressMMCInd,temp
	ldi	temp,high(BeginSecWr)
	sts	AdressMMCInd+1,temp
	ldi	temp,$0
	sts	AdressMMCInd+2,temp
	ret

EEPROM_read:	;temp2:temp - адрес ( на выходе temp - данные)
; Wait for completion of previous write
	sbic	EECR,EEWE
	rjmp	EEPROM_read
; Set up address (r17:r16) in address register
	out	EEARH,temp2
	out	EEARL,temp
	sbi	EECR,EERE
	in	temp,EEDR
	ret

EEPROM_write:	;temp - данные, temp3:temp2 - адрес 
; Wait for completion of previous write
	sbic	EECR,EEWE
	rjmp	EEPROM_write
; Set up address (r18:r17) in address register
	out	EEARH,r18
	out	EEARL,r17
; Write data (r16) to data register
	out	EEDR,r16
; Write logical one to EEMWE
	sbi	EECR,EEMWE
; Start eeprom write by setting EEWE
	sbi	EECR,EEWE
	ret


.include "RomWr.asm"


