Ini_SPI:
	sbi	SPI_port,SPI_SS
	sbi	SPI_dir,SPI_SS
	ldi	temp,0b01010000
	out	SPCR,temp
	cbi	SPSR,0	;Работа в с одинарной скоростью
	;sbi	SPSR,0	;Работа в с удвоенной скоростью
	ret

PutSPIbyte:	;в temp - передаваемый байт (в temp возвращается принятый байт)
	out	SPDR,temp
WhPutSPIbyte:
	SBIS	SPSR,7
	rjmp	WhPutSPIbyte
	in	temp,SPDR
	ret
		
GetSPIbyte:	;в temp возвращается принятый байт
	ldi	temp,$FF
	out	SPDR,temp
WhGetSPIbyte:
	SBIS	SPSR,7
	rjmp	WhGetSPIbyte
	in	temp,SPDR
	ret
	
ON_SPI_Slave:
	cbi	SPI_port,SPI_SS	;Активируем MMC
	ret

OFF_SPI_Slave:
	sbi	SPI_port,SPI_SS	;Отключаем MMC
	ret

Ini_MMC:
	ldi	temp,0
	sts	Err_MMC,temp
	cbi	SPI_port,SPI_LED	;Включаем светодиод
	ldi	temp,0x3F
	;mov	r15,temp	;!!!!!
	sbi	SPI_port,SPI_SS	;Отключаем MMC
	;rcall	OFF_SPI_Slave
        Pause	5000
	ldi	temp2,74
loop1Ini_MMC:
	rcall	GetSPIbyte
	dec	temp2
	jnz	loop1Ini_MMC
        Pause	1000
	cbi	SPI_port,SPI_SS	;Активируем MMC
	;rcall	ON_SPI_Slave
	rcall	GetSPIbyte	;CMD0
	ldi	temp,0x40
	;mov	r15,temp	;!!!!!
	rcall	PutSPIbyte
	ldi	temp,0
	rcall	PutSPIbyte
	ldi	temp,0
	rcall	PutSPIbyte
	ldi	temp,0
	rcall	PutSPIbyte
	ldi	temp,0
	rcall	PutSPIbyte
	ldi	temp,0x95
	rcall	PutSPIbyte
	ldi	Wh,high(CountWhiteMMC)
	ldi	Wl,low(CountWhiteMMC)
Loop2Ini_MMC:
	sbiw	Wl,1	;Временной таймаут
	jnz	NErr001
	ldi	temp,1
	sts	Err_MMC,temp
	Pause10ms	20
	sbi	SPI_port,SPI_LED	;Выключаем светодиод
	Pause10ms	20
	cbi	SPI_port,SPI_LED	;Включаем светодиод
	Pause10ms	20
	sbi	SPI_port,SPI_LED	;Выключаем светодиод
	Pause10ms	20
	cbi	SPI_port,SPI_LED	;Включаем светодиод
	Pause10ms	20
	sbi	SPI_port,SPI_LED	;Выключаем светодиод
	Pause10ms	20
	;cbi	SPI_port,SPI_LED	;Включаем светодиод
	;Pause10ms	20
	;sbi	SPI_port,SPI_LED	;Выключаем светодиод
	rjmp	Ini_MMC
NErr001:
	rcall	GetSPIbyte	;Ожидание 0x01
	;mov	r14,temp	;!!!!!
	cpi	temp,0x01
	jnz	Loop2Ini_MMC
	;Pause10ms	250

Ini_MMC1:

	ldi	Wh,high(CountWhiteMMC)
	ldi	Wl,low(CountWhiteMMC)
Loop3Ini_MMC:
	rcall	GetSPIbyte	;CMD1
	ldi	temp,0x41
	;mov	r15,temp	;!!!!!
	rcall	PutSPIbyte
	ldi	temp,0
	rcall	PutSPIbyte
	ldi	temp,0
	rcall	PutSPIbyte
	ldi	temp,0
	rcall	PutSPIbyte
	ldi	temp,0
	rcall	PutSPIbyte
	ldi	temp,0xFF
	rcall	PutSPIbyte
Loop4Ini_MMC:
	sbiw	Wl,1	;Временной таймаут
	jnz	NErr002
	ldi	temp,2
	sts	Err_MMC,temp
	Pause10ms	40
	sbi	SPI_port,SPI_LED	;Выключаем светодиод
	Pause10ms	40
	cbi	SPI_port,SPI_LED	;Включаем светодиод
	Pause10ms	40
	sbi	SPI_port,SPI_LED	;Выключаем светодиод
	Pause10ms	40
	rjmp	Ini_MMC
NErr002:
	rcall	GetSPIbyte	;Ожидание 0x00
	;mov	r14,temp	;!!!!!
	cpi	temp,0xff
	jz	Loop4Ini_MMC
	cpi	temp,0x00
	jnz	Loop3Ini_MMC
	;Pause10ms	250
	;mov	r15,temp	;!!!!!
	;rcall	OFF_SPI_Slave
	sbi	SPI_port,SPI_LED	;Выключаем светодиод
	sbi	SPI_port,SPI_SS	;Отключаем MMC
	ret

Read512block:
	ldi	temp,0
	sts	Err_MMC,temp
	cbi	SPI_port,SPI_SS	;Активируем MMC
	cbi	SPI_port,SPI_LED	;Включаем светодиод
	ldi	xh,high(Buf512)
	ldi	xl,low(Buf512)
	rcall	GetSPIbyte	;CMD17
	ldi	temp,0x40+17
	;mov	r15,temp	;!!!!!
	rcall	PutSPIbyte
	lds	temp,AdressMMC+3
	rcall	PutSPIbyte
	lds	temp,AdressMMC+2
	rcall	PutSPIbyte
	lds	temp,AdressMMC+1
	rcall	PutSPIbyte
	lds	temp,AdressMMC
	rcall	PutSPIbyte
	ldi	temp,0xFF
	rcall	PutSPIbyte
	ldi	Wh,high(CountWhiteMMC)
	ldi	Wl,low(CountWhiteMMC)
Loop1Read512block:
	sbiw	Wl,1	;Временной таймаут
	jnz	NErr003
	ldi	temp,3
	sts	Err_MMC,temp
	rjmp	ErRead512block
NErr003:
	rcall	GetSPIbyte	;Ожидание ответа
	;mov	r14,temp	;!!!!!
	cpi	temp,0xff
	jz	Loop1Read512block
	;mov	r13,temp

Loop3Read512block:
	sbiw	Wl,1	;Временной таймаут
	jnz	NErr004
	ldi	temp,4
	sts	Err_MMC,temp
	rjmp	ErRead512block
NErr004:
	rcall	GetSPIbyte	;Ожидание 0xFE
	;mov	r14,temp	;!!!!!
	cpi	temp,0xff
	jz	Loop3Read512block
	;mov	r12,temp

	ldi	temp2,128
Loop2Read512block:
	rcall	GetSPIbyte
	st	X+,temp
	rcall	GetSPIbyte
	st	X+,temp
	rcall	GetSPIbyte
	st	X+,temp
	rcall	GetSPIbyte
	st	X+,temp
	dec	temp2
	jnz	Loop2Read512block	
ErRead512block:
	rcall	GetSPIbyte
	rcall	GetSPIbyte
	rcall	GetSPIbyte
	rcall	GetSPIbyte
	sbi	SPI_port,SPI_LED	;Выключаем светодиод
	sbi	SPI_port,SPI_SS	;Отключаем MMC
	ret

Write512block:
	ldi	temp,0
	sts	Err_MMC,temp
	cbi	SPI_port,SPI_SS	;Активируем MMC
	cbi	SPI_port,SPI_LED	;Включаем светодиод
	ldi	xh,high(Buf512)
	ldi	xl,low(Buf512)
	rcall	GetSPIbyte	;CMD24
	ldi	temp,0x40+24
	;mov	r15,temp	;!!!!!
	rcall	PutSPIbyte
	lds	temp,AdressMMC+3
	rcall	PutSPIbyte
	lds	temp,AdressMMC+2
	rcall	PutSPIbyte
	lds	temp,AdressMMC+1
	rcall	PutSPIbyte
	lds	temp,AdressMMC
	rcall	PutSPIbyte
	ldi	temp,0xFF
	rcall	PutSPIbyte

	ldi	temp,0xFF
	rcall	PutSPIbyte
	ldi	temp,0xFF
	rcall	PutSPIbyte
	ldi	temp,0xFE
	rcall	PutSPIbyte

	ldi	temp2,128
Loop1Write512block:
	ld	temp,X+
	rcall	PutSPIbyte
	ld	temp,X+
	rcall	PutSPIbyte
	ld	temp,X+
	rcall	PutSPIbyte
	ld	temp,X+
	rcall	PutSPIbyte
	dec	temp2
	jnz	Loop1Write512block
	ldi	temp,0xff	;Контрольная сумма	
	rcall	PutSPIbyte
	ldi	temp,0xff	
	rcall	PutSPIbyte

	rcall	GetSPIbyte	;Результат записи
	sts	StatusWr,temp
	ldi	Wh,high(CountWhiteMMC)
	ldi	Wl,low(CountWhiteMMC)
Loop2Write512block:
	sbiw	Wl,1	;Временной таймаут
	jnz	NErr005
	ldi	temp,5
	sts	Err_MMC,temp
	rjmp	ErWrite512block
NErr005:
	rcall	GetSPIbyte	;ожидание готовности MMC
	cpi	temp,0xff
	jnz	Loop2Write512block
ErWrite512block:
	sbi	SPI_port,SPI_LED	;Выключаем светодиод
	sbi	SPI_port,SPI_SS	;Отключаем MMC
	ret

CalcAdressMMC:
	lds	temp,AdressMMCInd	;AdressMMC=AdressMMCInd*512
	LSL	temp
	sts	AdressMMC+1,temp
	lds	temp,AdressMMCInd+1
	ROL	temp
	sts	AdressMMC+2,temp
	lds	temp,AdressMMCInd+2
	ROL	temp
	sts	AdressMMC+3,temp
	ldi	temp,0
	sts	AdressMMC,temp
	ret

CalcAdress2MMC:
	lds	temp,AdressMMCInd2	;AdressMMC=AdressMMCInd2*512
	LSL	temp
	sts	AdressMMC+1,temp
	lds	temp,AdressMMCInd2+1
	ROL	temp
	sts	AdressMMC+2,temp
	lds	temp,AdressMMCInd2+2
	ROL	temp
	sts	AdressMMC+3,temp
	ldi	temp,0
	sts	AdressMMC,temp
	ret

IncAdressMMCInd:
	lds	temp,AdressMMCInd
	inc	temp
	sts	AdressMMCInd,temp
	jnz	ExIncAdressMMCInd
	lds	temp,AdressMMCInd+1
	inc	temp
	sts	AdressMMCInd+1,temp
	jnz	ExIncAdressMMCInd
	lds	temp,AdressMMCInd+2
	inc	temp
	sts	AdressMMCInd+2,temp
ExIncAdressMMCInd:
	ret

DecAdressMMCInd:
	lds	temp,AdressMMCInd
	SUBI	temp,1
	sts	AdressMMCInd,temp
	jnc	ExIncAdressMMCInd
	lds	temp,AdressMMCInd+1
	SUBI	temp,1
	sts	AdressMMCInd+1,temp
	jnc	ExIncAdressMMCInd
	lds	temp,AdressMMCInd+2
	SUBI	temp,1
	sts	AdressMMCInd+2,temp
ExDecAdressMMCInd:
	ret

ReadPlusMMC:
	rcall	CalcAdressMMC
	rcall	Read512block
	lds	temp,Err_MMC
	cpi	temp,0
	jnz	ErReadPlusMMC
	rcall	IncAdressMMCInd
ErReadPlusMMC:
	ret

WritePlusMMC:
	lds	temp,AdressMMCInd+2
	ANDI	temp,$7F
	jnz	OkWritePlusMMC
	lds	temp,AdressMMCInd+1
	lds	temp2,BeginMMCInd+1
	cp	temp,temp2
	jc	NoWritePlusMMC
	jnz	OkWritePlusMMC
	lds	temp,AdressMMCInd
	lds	temp2,BeginMMCInd
	cp	temp,temp2
	jc	NoWritePlusMMC
OkWritePlusMMC:
	lds	temp,AdressMMCInd+2
	lds	temp2,AllSect+2
	cp	temp2,temp
	jc	NoWritePlusMMC
	jnz	Ok2WritePlusMMC
	lds	temp,AdressMMCInd+1
	lds	temp2,AllSect+1
	cp	temp2,temp
	jc	NoWritePlusMMC
	jnz	OkWritePlusMMC
	lds	temp,AdressMMCInd
	lds	temp2,AllSect
	cp	temp,temp2
	jnc	NoWritePlusMMC
Ok2WritePlusMMC:
	rcall	CalcAdressMMC
	rcall	Write512block
	lds	temp,Err_MMC
	cpi	temp,0
	jnz	ErWritePlusMMC
	rcall	IncAdressMMCInd
ErWritePlusMMC:
	ret
NoWritePlusMMC:
	ldi	temp,0 		;Устанавливаем на начало записи
	sts	AdressMMCInd+2,temp
	lds	temp,BeginMMCInd+1
	sts	AdressMMCInd+1,temp
	lds	temp,BeginMMCInd
	sts	AdressMMCInd,temp
	rjmp	OkWritePlusMMC

ReadInd2MMC:
	rcall	CalcAdress2MMC
	rcall	Read512block
	ret

WriteInd2MMC:
	rcall	CalcAdress2MMC
	rcall	Write512block
	ret

ErReadInfoMMC:
	rcall	Ini_SPI
	rcall	Ini_MMC
ReadInfoMMC:
	;rcall	GetCID
	;rcall	GetCSD
	ldi	temp,0
	sts	AdressMMCInd2+2,temp
	sts	AdressMMCInd2+1,temp
	sts	AdressMMCInd2,temp
	rcall	ReadInd2MMC
	lds	temp,Err_MMC
	cpi	temp,0
	jnz	ErReadInfoMMC
	lds	temp,Buf512	;Определяем - это сектор MBR или PBR
	cpi	temp,0
	jnz	No_MBR
	lds	temp,Buf512+1
	cpi	temp,0
	jnz	No_MBR
	lds	temp,Buf512+2
	cpi	temp,0
	jnz	No_MBR
	lds	temp,Buf512+3
	cpi	temp,0
	jnz	No_MBR
	lds	temp,Buf512+$1C6
	sts	AdressMMCInd2,temp
	lds	temp,Buf512+$1C6+1
	sts	AdressMMCInd2+1,temp
	lds	temp,Buf512+$1C6+2
	sts	AdressMMCInd2+2,temp
	rcall	ReadInd2MMC
	lds	temp,Err_MMC
	cpi	temp,0
	jnz	ErReadInfoMMC
No_MBR:
	lds	temp,Buf512+$D
	sts	SecPerClast,temp
	lds	temp,Buf512+$E
	sts	SecRezPBR,temp
	lds	temp,Buf512+$E+1
	sts	SecRezPBR+1,temp
	lds	temp,Buf512+$10
	sts	CountTabFAT,temp
	lds	temp,Buf512+$11
	sts	RootDitEntry,temp
	lds	temp,Buf512+$11+1
	sts	RootDitEntry+1,temp
	lds	temp,Buf512+$16
	sts	SecForFAT,temp
	lds	temp,Buf512+$16+1
	sts	SecForFAT+1,temp
	lds	temp,Buf512+$20
	sts	AllSect,temp
	lds	temp,Buf512+$20+1
	sts	AllSect+1,temp
	lds	temp,Buf512+$20+2
	sts	AllSect+2,temp
	lds	temp,Buf512+$20+3
	sts	AllSect+3,temp
	
	ldi	temp,0
	sts	Fat_base+1,temp
	lds	temp,SecRezPBR
	sts     Fat_base,temp
	lds	temp2,SecForFAT
	lds	temp3,SecForFAT+1
	lds	temp4,CountTabFAT
	cpi	temp4,1
	jz	CountTabFAT_EQ_1
	add	temp2,temp2
	adc	temp3,temp3
CountTabFAT_EQ_1:
	add	temp2,temp
	sts	Root_base,temp2
	ldi	temp,0
	adc	temp3,temp
	sts	Root_base+1,temp3

	lds	xl,RootDitEntry
	lds	xh,RootDitEntry+1
	LSR	xh	;RootDitEntry/16
	ROR	xl
	LSR	xh
	ROR	xl
	LSR	xh
	ROR	xl
	LSR	xh
	ROR	xl
	add	xl,temp2
	adc	xh,temp3
	sts	Claster_002,xl
	sts	Claster_002+1,xh
	ADIW	xl,1
	lds	temp,AdressMMCInd2	;Добавляем номер 1-го сектора раздела
	add	xl,temp		;
	lds	temp,AdressMMCInd2+1	;
	adc	xh,temp		;
	cpi	xh,2
	jz	OkClaster_002
	rjmp	ErReadInfoMMC
OkClaster_002:
	sts	BeginMMCInd,xl
	sts	BeginMMCInd+1,xh
	sts	AdressMMCInd2,xl
	sts	AdressMMCInd2+1,xh
	rcall	ReadInd2MMC
	lds	temp,Buf512
	cpi	temp,$FF
	jnz	ExReadInfoMMC
	lds	xl,BeginMMCInd
	lds	xh,BeginMMCInd+1
	sts	AdressMMCInd,xl	;Если в начале файла FF - пишем с начала файла
	sts	AdressMMCInd+1,xh
	ldi	temp,0
	sts	AdressMMCInd+2,temp
ExReadInfoMMC:
	ret

Read512block2COM:
	cbi	SPI_port,SPI_SS	;Активируем MMC
	cbi	SPI_port,SPI_LED	;Включаем светодиод
	ldi	temp4,$A5
	rcall	GetSPIbyte	;CMD17
	ldi	temp,0x40+17
	;mov	r15,temp	;!!!!!
	rcall	PutSPIbyte
	lds	temp,AdressMMC+3
	rcall	PutSPIbyte
	lds	temp,AdressMMC+2
	rcall	PutSPIbyte
	lds	temp,AdressMMC+1
	rcall	PutSPIbyte
	lds	temp,AdressMMC
	rcall	PutSPIbyte
	ldi	temp,0xFF
	rcall	PutSPIbyte
	ldi	Wh,high(CountWhiteMMC)
	ldi	Wl,low(CountWhiteMMC)
Loop1Read512block2COM:
	sbiw	Wl,1	;Временной таймаут
	jnz	NErr006
	ldi	temp,6
	sts	Err_MMC,temp
	sts	AdrUstr,temp
	rjmp	ErRead512block2COM
NErr006:
	rcall	GetSPIbyte	;Ожидание ответа
	;mov	r14,temp	;!!!!!
	cpi	temp,0xff
	jz	Loop1Read512block2COM
	;mov	r13,temp

Loop3Read512block2COM:
	sbiw	Wl,1	;Временной таймаут
	jnz	NErr007
	ldi	temp,7
	sts	Err_MMC,temp
	sts	AdrUstr,temp
	rjmp	ErRead512block2COM
NErr007:
	rcall	GetSPIbyte	;Ожидание 0xFE
	;mov	r14,temp	;!!!!!
	cpi	temp,0xff
	jz	Loop3Read512block2COM
	;mov	r12,temp
ErRead512block2COM:
	lds	temp,AdrUstr	;Подтверждаем ответ карточки
	rcall	PutChU
	ldi	temp2,128
Loop2Read512block2COM:
	rcall	GetSPIbyte
	add	temp4,temp
	rcall	PutChU
	rcall	GetSPIbyte
	add	temp4,temp
	rcall	PutChU
	rcall	GetSPIbyte
	add	temp4,temp
	rcall	PutChU
	rcall	GetSPIbyte
	add	temp4,temp
	rcall	PutChU
	dec	temp2
	jnz	Loop2Read512block2COM	
	mov	temp,temp4	;Послать контрольную сумму
	rcall	PutChU

	rcall	GetSPIbyte
	rcall	GetSPIbyte
	rcall	GetSPIbyte
	rcall	GetSPIbyte
	sbi	SPI_port,SPI_LED	;Выключаем светодиод
	sbi	SPI_port,SPI_SS	;Отключаем MMC
	ret

