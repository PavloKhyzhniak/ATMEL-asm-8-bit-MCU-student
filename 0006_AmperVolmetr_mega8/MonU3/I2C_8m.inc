;����� ���������� � ������ � ���������� (��� ������ ����� ���������� ������)
SetAdI2Cwr:	;� temp2 - ����� ����������, � temp3 - ����� � ����������
	ldi	temp,0b10100100	;�����
	out	TWCR,temp
WhiteStartI2C:               ;�������� ������������ ������ �����
	in	temp,TWCR
	SBRS	temp,TWINT
	rjmp	WhiteStartI2C
	in	temp,TWSR
	mov	temp,temp2	;����� ����������
	ANDI	temp,$FE     	;���������� ��� - ������ ������
	out	TWDR,temp
	ldi	temp,0b10000100
	out	TWCR,temp
WhiteAdrUsI2C:               	;�������� �������� ������ ����������
	in	temp,TWCR
	SBRS	temp,TWINT
	rjmp	WhiteAdrUsI2C
	in	temp,TWSR
	
	mov	temp,temp3	;����� ������ � ����������
	out	TWDR,temp
	ldi	temp,0b10000100
	out	TWCR,temp
WhiteAdr1I2C:               ;�������� �������� ������ ������� ����
	in	temp,TWCR
	SBRS	temp,TWINT
	rjmp	WhiteAdr1I2C
	in	temp,TWSR
	
	ret	

;����� �������� ������ � ���������� - ������������ �� ������
StartI2Crd:	;� temp2 - ����� ����������
	ldi	temp,0b10100100
	out	TWCR,temp
WhiteStartI2Crd:               ;�������� ������������ ������ �����
	in	temp,TWCR
	SBRS	temp,TWINT
	rjmp	WhiteStartI2Crd
	in	temp,TWSR
	mov	temp,temp2
	ORI	temp,1     ;������������� ��� - ������ ������
	out	TWDR,temp
	ldi	temp,0b10000100
	out	TWCR,temp
WhiteAdrUsI2Crd:               ;�������� �������� ������ ����������
	in	temp,TWCR
	SBRS	temp,TWINT
	rjmp	WhiteAdrUsI2Crd
	in	temp,TWSR
	ret

StopI2C:
	ldi	temp,0b10010100
	out	TWCR,temp
WhiteStopI2Crd:               ;�������� Stop
	in	temp,TWCR
	SBRC	temp,TWSTO
	rjmp	WhiteStopI2Crd
	ret

GetByteI2C:	;� temp - ������������ ���������
	;ldi	temp,$FF
	;out	TWDR,temp
	ldi	temp,0b10000100
	out	TWCR,temp
WhiteByRI2C:               ;�������� ����� �����
	in	temp,TWCR
	SBRS	temp,TWINT
	rjmp	WhiteByRI2C
;	in	r0,TWSR
	in	temp,TWDR
	ret

PutByteI2C:	;� temp - ���� ��� ��������
	out	TWDR,temp
	ldi	temp,0b10000100
	out	TWCR,temp
WhiteByWI2C:               ;�������� �������� �����
	in	temp,TWCR
	SBRS	temp,TWINT
	rjmp	WhiteByWI2C
	in	temp,TWSR
	ret

Ini_I2C:
	cbi	DDRC,4
	cbi	DDRC,5
	cbi	PORTC,4
	cbi	PORTC,5
	ldi	temp,32	;100���
	out	TWBR,temp
	cbi	TWSR,0
	cbi	TWSR,1
	ret

ReadClock:

	ldi	temp2,$D0

	ldi	temp3,0
	rcall	SetAdI2Cwr
	rcall	StartI2Crd
	rcall	GetByteI2C
	sts	Secund,temp

	inc	temp3
	rcall	SetAdI2Cwr
	rcall	StartI2Crd
	rcall	GetByteI2C
	sts	Minut,temp

	inc	temp3
	rcall	SetAdI2Cwr
	rcall	StartI2Crd
	rcall	GetByteI2C
	sts	Chas,temp

	inc	temp3
	rcall	SetAdI2Cwr
	rcall	StartI2Crd
	rcall	GetByteI2C
	sts	Day,temp

	inc	temp3
	rcall	SetAdI2Cwr
	rcall	StartI2Crd
	rcall	GetByteI2C
	sts	Data,temp

	inc	temp3
	rcall	SetAdI2Cwr
	rcall	StartI2Crd
	rcall	GetByteI2C
	sts	Mon,temp

	inc	temp3
	rcall	SetAdI2Cwr
	rcall	StartI2Crd
	rcall	GetByteI2C
	sts	Year,temp

	inc	temp3
	rcall	SetAdI2Cwr
	rcall	StartI2Crd
	rcall	GetByteI2C
	sts	Status,temp

	in	temp,TWSR
	rcall	StopI2C
	ret

SetClock:
	ldi	temp2,$D0

	ldi	temp3,0
	rcall	SetAdI2Cwr

	lds	temp,Secund2
	rcall	PutByteI2C

	lds	temp,Minut2
	rcall	PutByteI2C

	lds	temp,Chas2
	rcall	PutByteI2C

	lds	temp,Day2
	rcall	PutByteI2C

	lds	temp,Data2
	rcall	PutByteI2C

	lds	temp,Mon2
	rcall	PutByteI2C

	lds	temp,Year2
	rcall	PutByteI2C

	lds	temp,Status2
	rcall	PutByteI2C

	rcall	StopI2C
	ret

Ini_Clock:
	rcall	ReadClock
	lds	temp,Status
	cpi	temp,$10
	jnz	Ok_Status

	ldi	temp2,$D0
	ldi	temp3,7
	rcall	SetAdI2Cwr
	ldi	temp,0b00010000
	rcall	PutByteI2C
	rcall	StopI2C
Ok_Status:
	lds	temp,Secund
	andi	temp,$80
	jz	Ok_Secund

	ldi	temp2,$D0
	ldi	temp3,0
	rcall	SetAdI2Cwr
	ldi	temp,0
	rcall	PutByteI2C
	rcall	StopI2C
Ok_Secund:
	ret

ReadClockRAM:	;� temp3 - ����� RAM, � temp ������������ ��������� ����
	ldi	temp2,$D0
	rcall	SetAdI2Cwr
	rcall	StartI2Crd
	rcall	GetByteI2C
	rcall	StopI2C
	ret

WriteClockRAM:	;� temp3 - ����� RAM, � temp ���� ��� ������
	ldi	temp2,$D0
	push	temp
	rcall	SetAdI2Cwr
	pop	temp
	rcall	PutByteI2C
	rcall	StopI2C
	ret

ReadIndexClockRAM:
	ldi	temp2,$D0

	ldi	temp3,8
	rcall	SetAdI2Cwr
	rcall	StartI2Crd
	rcall	GetByteI2C
	sts	AdressMMCInd,temp

	inc	temp3
	rcall	SetAdI2Cwr
	rcall	StartI2Crd
	rcall	GetByteI2C
	sts	AdressMMCInd+1,temp

	inc	temp3
	rcall	SetAdI2Cwr
	rcall	StartI2Crd
	rcall	GetByteI2C
	sts	AdressMMCInd+2,temp

	inc	temp3
	rcall	SetAdI2Cwr
	rcall	StartI2Crd
	rcall	GetByteI2C
	sts	AdressMMCInd+3,temp

	in	temp,TWSR
	rcall	StopI2C
	ret

WriteIndexClockRAM:
	ldi	temp2,$D0

	ldi	temp3,8
	rcall	SetAdI2Cwr

	lds	temp,AdressMMCInd
	rcall	PutByteI2C

	lds	temp,AdressMMCInd+1
	rcall	PutByteI2C

	lds	temp,AdressMMCInd+2
	rcall	PutByteI2C

	lds	temp,AdressMMCInd+3
	rcall	PutByteI2C

	rcall	StopI2C
	ret

ClearIndexClockRAM:
	ldi	temp2,$D0

	ldi	temp3,8
	rcall	SetAdI2Cwr

	lds	temp,BeginMMCInd
	rcall	PutByteI2C

	lds	temp,BeginMMCInd+1
	rcall	PutByteI2C

	ldi	temp,0
	rcall	PutByteI2C

	ldi	temp,0
	rcall	PutByteI2C

	rcall	StopI2C
	ret

