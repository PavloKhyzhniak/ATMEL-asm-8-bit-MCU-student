
Ini_MonUart:
;----------------------------------------------
	cbi	DDRD,0	;����������� ����� ��� COM
	sbi	PORTD,0	;����������� ���� ���������� � +5� - ������ ������ ����������
	sbi	DDRD,1
	sbi	PORTD,1

	;ldi	temp,0b00000010	;������� ��������, ��� �������� �������
	ldi	temp,0b00000000	;��������� ��������, ��� �������� �������
	out	UCSRA,temp
	;sbi	UCSRB,RXEN
	;sbi	UCSRB,TXEN
	;ldi	temp,0b00011000	;�������� ���� � �������� ��� ����������
	ldi	temp,0b10011000	;�������� ���� � �������� � ����������� ��� �����
	out	UCSRB,temp
	ldi	temp,0b10000110	;8 ��� ������ 1 ���� ��� ��� �������� ��������
	out	UCSRC,temp
	ldi	temp,0	;������� ���� �������� �������� ����� 0
	out	UBRRH,temp
	;ldi	temp,12	; ������� ���� �������� �������� ����� 12 (2��� - 19200)
	ldi	temp,12	; ������� ���� �������� �������� ����� 12 (4��� - 38400)
	out	UBRRL,temp
;----------------------------------------------
	ret

GetRam:
	SWAP	temp
	mov		zl,temp
	mov		zh,temp
	ANDI	zl,$F0
	ANDI	zh,$F
	ld		temp,Z+
	rcall	PutChU
	ld		temp,Z+
	rcall	PutChU
	ld		temp,Z+
	rcall	PutChU
	ld		temp,Z+
	rcall	PutChU
	ld		temp,Z+
	rcall	PutChU
	ld		temp,Z+
	rcall	PutChU
	ld		temp,Z+
	rcall	PutChU
	ld		temp,Z+
	rcall	PutChU
	ld		temp,Z+
	rcall	PutChU
	ld		temp,Z+
	rcall	PutChU
	ld		temp,Z+
	rcall	PutChU
	ld		temp,Z+
	rcall	PutChU
	ld		temp,Z+
	rcall	PutChU
	ld		temp,Z+
	rcall	PutChU
	ld		temp,Z+
	rcall	PutChU
	ld		temp,Z+
	rcall	PutChU
	ret

GetPort:
	in	temp,ACSR	;��� ATmega8 - ����������
	rcall	PutChU
	in	temp,PINB
	rcall	PutChU
	in	temp,PINC
	rcall	PutChU
	in	temp,PIND
	rcall	PutChU

	in	temp,PORTB
	rcall	PutChU
	in	temp,PORTB
	rcall	PutChU
	in	temp,PORTC
	rcall	PutChU
	in	temp,PORTD
	rcall	PutChU

	in	temp,DDRB
	rcall	PutChU
	in	temp,DDRB
	rcall	PutChU
	in	temp,DDRC
	rcall	PutChU
	in	temp,DDRD
	rcall	PutChU
	ret

GetPortI:
	in	temp,ACSR	;��� ATmega8 - ����������
	rcall	PutChU
	in	temp,PINB
	rcall	PutChU
	in	temp,PINC
	rcall	PutChU
	in	temp,PIND
	rcall	PutChU
	ret

PutChU:		;� temp - ������������ ����
	sbis	UCSRA,UDRE	;�������� ���������� �������� ������
	rjmp	PutChU
	out	UDR,temp
	ret

ReadPort:
	rcall	GetPort
	rjmp	ExIntRx

ReadPortI:
	rcall	GetPortI
	rjmp	ExIntRx

ReadU:
	push	temp                          
	in	temp,SREG
	push	temp
	push	zh
	push	zl
	in	temp,UCSRA	;������ ����� ���������
	in	temp,UCSRB	;9-� ������ (&0x01)
	in	temp,UDR		;������ ���� ������
	cpi	temp,$46
	BREQ	ReadPort
	cpi	temp,$47
	BREQ	ReadPortI
	cpi	temp,$A5
	BREQ	ProgROM
	cpi	temp,$AF
	BREQ	ReadROM0

	rcall	GetRam
ExIntRx:
	pop		zl
	pop		zh
	pop	temp
	out	SREG,temp	
	pop	temp
	reti

ReadROM0: rjmp	ReadROM

ErrProgROM:
	ldi	temp,0xFF
	rcall	PutChU
	;sbi	PORTB,1
	;sbi	PORTB,0
	;rcall	Pause05s
	;cbi	PORTB,1
	;rcall	Pause05s
	;rjmp	ErrProgROM
	rjmp	ExProgROM

ProgROM:
	push	temp2
	push	temp3
	push	temp4
	push	yh
	push	yl
	push	xh
	push	xl
	push	wh
	push	wl
	push	rCX1
	push	rCX2
	
	;cli	;��������� ����������
	rcall	GetBlok2Mem
	cpi	temp,0
	jnz	ErrProgROM
	lds	temp,AdrUstr
	cpi	temp,$FE
	jnz	NoSaveClock

	lds	temp,NewTime
	cpi	temp,0
	jnz	WhiteSetClock
        rcall	SetClock
	lds	temp,AdrUstr
	rcall	PutChU
	rjmp	ExProgROM
WhiteSetClock:
	ldi	temp,2	;����� ��������
	sts	NewTime,temp
	rjmp	ExProgROM1
NoSaveClock:	
	cpi	temp,64
	jnc	NoRamSect512Wr
	ldi	xl,low(Secund2)
	ldi	xh,high(Secund2)
	mov	yl,temp
	ldi	yh,0
	LSL	yl	;Y*=8;
	ROL	yh
	LSL	yl
	ROL	yh
	LSL	yl
	ROL	yh
	inc	yh	;Y+=0x100
	ldi	temp2,8
Loop1ProgROM:
	ld	temp,X+
	st	Y+,temp
	dec	temp2
	jnz	Loop1ProgROM
	rjmp	ExProgROM1
NoRamSect512Wr:
	rjmp	UprComm
ExProgROM1:
	lds	temp,AdrUstr
	rcall	PutChU
ExProgROM:
	pop	rCX2
	pop	rCX1
	pop	wl
	pop	wh
	pop	xl
	pop	xh
	pop	yl
	pop	yh
	pop	temp4
	pop	temp3
	pop	temp2
	sei	;���������� ��������� ����������
;Loop3243:	rjmp	Loop3243
	rjmp	ExIntRx

UprComm:
	cpi	temp,0xF0	;������ �������
	jnz	NoUprComm_F0
	lds	temp,Secund2
	sts	AdressMMC,temp
	lds	temp,Secund2+1
	sts	AdressMMC+1,temp
	lds	temp,Secund2+2
	sts	AdressMMC+2,temp
	lds	temp,Secund2+3
	sts	AdressMMC+3,temp
	rcall	ON_SPI_Slave
	rcall	Read512block2COM
	rcall	OFF_SPI_Slave
	rjmp	ExProgROM	;����� ��� �������
NoUprComm_F0:
	cpi	temp,0xF1	;������ �������
	jnz	NoUprComm_F1
	lds	temp,Secund2
	sts	AdressMMC,temp
	lds	temp,Secund2+1
	sts	AdressMMC+1,temp
	lds	temp,Secund2+2
	sts	AdressMMC+2,temp
	lds	temp,Secund2+3
	sts	AdressMMC+3,temp
	rcall	ON_SPI_Slave
	rcall	Write512block
	rcall	OFF_SPI_Slave
	rjmp	ExProgROM1
NoUprComm_F1:
	cpi	temp,0xF2	;������ ������ ADC � MMC
	jnz	NoUprComm_F2
	ldi	temp,1
	sts	ON_WriteADC_MMC,temp
	rjmp	ExProgROM1
NoUprComm_F2:
	cpi	temp,0xF3	;���������� ������ ADC � MMC
	jnz	NoUprComm_F3
	ldi	temp,0
	sts	ON_WriteADC_MMC,temp
	rjmp	ExProgROM1
NoUprComm_F3:
	rjmp	ExProgROM1
	

GetChU:	sbis	UCSRA,RXC
	rjmp	GetChU
	in	r18,UCSRA	;������ ����� ���������
	in	r17,UCSRB	;9-� ������ (&0x01)
	in	r16,UDR		;������ ���� ������
	ret

GetChUW1:
        ldi wl,low(60000)   ; ��������� ������� ���� ��������� 0
        ldi wh,high(60000)  ; ��������� ������� ���� ��������� 0
LoopPauseGetChUW1:			; 8 ���: (@0*2) - ��������  32767 ���
	sbic	UCSRA,RXC
	rjmp	GetChUWN
;------------------
	ldi	temp,100	;����� ��������
LoopGCW1t:
	dec	temp
	jnz	LoopGCW1t
;------------------
	SBIW	rW,1		
	BRNE	LoopPauseGetChUW1;
	ldi	temp4,$ff	;������ ��������
	ret
GetChUWN:
	ldi	temp4,0
	in	r18,UCSRA	;������ ����� ���������
	in	r17,UCSRB	;9-� ������ (&0x01)
	in	r16,UDR		;������ ���� ������
	ret

GetBlok2Mem:
	ldi	yh,high(Secund2)
	ldi	yl,low(Secund2)
	ldi	rCX1,8+2
	ldi	rCX2,$A5
LoopGetBlok2Mem:
	rcall	GetChUW1
	;rcall	GetChU
	st	Y+,temp
	add	rCX2,temp
	cpi	temp4,0
	jnz	ErrGetBlok2Mem
	dec	rCX1
	jnz	LoopGetBlok2Mem
	sub	rCX2,temp	;��������� ���� �������� �� �����
	cp	rCX2,temp	;�������� ����������� �����
	jnz	ErrGetBlok2Mem
	ldi	temp,0	;������ �������
	ret
ErrGetBlok2Mem:
	ldi	temp,$FF
	ret

ErrReadROM:
	ldi	temp,0xFF
	rcall	PutChU
	rjmp	ExProgROM

ReadROM:
	push	temp2
	push	temp3
	push	temp4
	push	yh
	push	yl
	push	xh
	push	xl
	push	wh
	push	wl
	push	rCX1
	push	rCX2
	
	;cli	;��������� ����������
	;sbi	PORTB,1
	;cbi	PORTB,0
	;rcall	GetAdrBlok2Mem
	;cpi	temp,0
	;jnz	ErrReadROM
	;call	ReadBlokI2c
	;rcall	PutChU
	;call	StopI2C

;	ldi	yh,high(BufGet)
;	ldi	yl,low(BufGet)
;	ldi	rCX1,64+3
;	ldi	rCX2,$AF
;LoopPutBlok2Mem:
;	ld	temp,Y+
;	add	rCX2,temp
;	rcall	PutChU
;	dec	rCX1
;	jnz	LoopPutBlok2Mem
;	mov	temp,rCX2
;	st	Y,temp
;	rcall	PutChU
	rjmp	ExProgROM

GetAdrBlok2Mem:
;	ldi	yh,high(BufGet+64)
;	ldi	yl,low(BufGet+64)
;	ldi	rCX1,4
;	ldi	rCX2,$AF
;LoopGetAdrBlok2Mem:
;	rcall	GetChUW1
;	st	Y+,temp
;	add	rCX2,temp
;	cpi	temp4,0
;	jnz	ErrGetAdrBlok2Mem
;	dec	rCX1
;	jnz	LoopGetAdrBlok2Mem
;	sub	rCX2,temp	;��������� ���� �������� �� �����
;	cp	rCX2,temp	;�������� ����������� �����
;	jnz	ErrGetAdrBlok2Mem
;	ldi	temp,0	;������ �������
;	ret
;ErrGetAdrBlok2Mem:
;	ldi	temp,$FF
	ret

