;************************************
;**
;**
;**
;************************************

TWI_INT:
			reti


; необходимые константы
.equ	START = 0x08
.equ	RepeatSTART = 0x10
.equ	MT_SLA_ACK = 0x18
.equ	MT_DATA_ACK = 0x28
.equ	MR_SLA_ACK = 0x40
.equ	MR_DATA_ACK = 0x50
.equ	MR_DATA_NACK = 0x58


.equ	DS1307_R = 0xD1
.equ	DS1307_W = 0xD0
.equ	DS1307_CONTROL = 0x07
.equ	DS1307_OSCILLATOR = 0x00
.equ	DS1307_TIME = 0x00
.equ	DS1307_DATE = 0x03
.equ	ClockHalt = 0x80

.MACRO RTC_START
	rcall I2C_START

	ldi r16,DS1307_W
	sts DATA_I2C_T,r16
	rcall I2C_SEND

ldi r17,MT_SLA_ACK
rcall CHECK

	ldi r16,DS1307_OSCILLATOR
	sts DATA_I2C_T,r16
	rcall I2C_SEND

ldi r17,MT_DATA_ACK
rcall CHECK

	rcall I2C_START

	ldi r16,DS1307_R
	sts DATA_I2C_T,r16
	rcall I2C_SEND

ldi r17,MR_SLA_ACK
rcall CHECK

	rcall I2C_RECEIVE_LAST; OSCILLATOR BYTE READ
	
ldi r17,MR_DATA_NACK
rcall CHECK

	lds r16,DATA_I2C_R
	cbr r16,ClockHalt
	push r16	; Clear Clock Halt Bit

	rcall I2C_STOP

	rcall I2C_START

	ldi r16,DS1307_W
	sts DATA_I2C_T,r16
	rcall I2C_SEND

ldi r17,MT_SLA_ACK
rcall CHECK

	ldi r16,DS1307_OSCILLATOR
	sts DATA_I2C_T,r16
	rcall I2C_SEND

	pop r16
;ldi r16,0x80
	sts DATA_I2C_T,r16
	rcall I2C_SEND

ldi r17,MT_DATA_ACK
rcall CHECK

	rcall I2C_STOP

.ENDMACRO

.MACRO RTC_STOP

	rcall I2C_START

	ldi r16,DS1307_W
	sts DATA_I2C_T,r16
	rcall I2C_SEND

	ldi r16,DS1307_OCILLATOR
	sts DATA_I2C_T,r16
	rcall I2C_SEND

	rcall I2C_START

	ldi r16,DS1307_R
	sts DATA_I2C_T,r16
	rcall I2C_SEND

	rcall I2C_RECEIVE_LAST; OSCILLATOR BYTE READ
	
	lds r16,DATA_I2C_R
	sbr r16,ClockHalt
	push r16	; Set Clock Halt Bit
	
	rcall I2C_STOP

	rcall I2C_START

	ldi r16,DS1307_W
	sts DATA_I2C_T,r16
	rcall I2C_SEND

	ldi r16,DS1307_OSCILLATOR
	sts DATA_I2C_T,r16
	rcall I2C_SEND

	pop r16
	sts DATA_I2C_T,r16
	rcall I2C_SEND

;	rcall I2C_STOP

.ENDM

.MACRO RTC_ReadTime

	rcall I2C_START

	ldi r16,DS1307_W
	sts DATA_I2C_T,r16
	rcall I2C_SEND

ldi r17,MT_SLA_ACK
rcall CHECK

	ldi r16,DS1307_TIME
	sts DATA_I2C_T,r16
	rcall I2C_SEND

ldi r17,MT_DATA_ACK
rcall CHECK

	rcall I2C_START

	ldi r16,DS1307_R
	sts DATA_I2C_T,r16
	rcall I2C_SEND

ldi r17,MR_SLA_ACK
rcall CHECK

	rcall I2C_RECEIVE; BCD SECONDS BYTE READ

ldi r17,MR_DATA_ACK
rcall CHECK

	lds r16,DATA_I2C_R
	rcall BCDtoDec
	sts SECONDS, r16

	rcall I2C_RECEIVE; BCD MINUTES BYTE READ

ldi r17,MR_DATA_ACK
rcall CHECK

	lds r16,DATA_I2C_R
	rcall BCDtoDec
	sts MINUTES,r16

	rcall I2C_RECEIVE_LAST; BCD HOURS BYTE READ

ldi r17,MR_DATA_NACK
rcall CHECK

	lds r16,DATA_I2C_R
	rcall BCDtoDec
	sts HOURS,r16
		
	rcall I2C_STOP

.ENDMACRO

.MACRO RTC_ReadDate

	rcall I2C_START

	ldi r16,DS1307_W
	sts DATA_I2C_T,r16
	rcall I2C_SEND

	ldi r16,DS1307_DATE
	sts DATA_I2C_T,r16
	rcall I2C_SEND

	rcall I2C_START

	ldi r16,DS1307_R
	sts DATA_I2C_T,r16
	rcall I2C_SEND

	rcall I2C_RECEIVE; BCD DAY BYTE READ

	lds r16,DATA_I2C_R
	rcall BCDtoDec
	sts DAY, r16

	rcall I2C_RECEIVE; BCD DATE BYTE READ

	lds r16,DATA_I2C_R
	rcall BCDtoDec
	sts DATE,r16

	rcall I2C_RECEIVE; BCD MONTH BYTE READ

	lds r16,DATA_I2C_R
	rcall BCDtoDec
	sts MONTH,r16
		
	rcall I2C_RECEIVE; BCD YEAR BYTE READ

	lds r16,DATA_I2C_R
	rcall BCDtoDec
	sts YEAR,r16

	rcall I2C_STOP

.ENDM

.MACRO RTC_WriteTime

	rcall I2C_START

	ldi r16,DS1307_W
	sts DATA_I2C,r16
	rcall I2C_SEND

	ldi r16,DS1307_TIME
	sts DATA_I2C_T,r16
	rcall I2C_SEND

	lds r16,SECONDS
	rcall DecToBCD
	sts DATA_I2C_T, r16

	rcall I2C_SEND; BCD SECONDS BYTE WRITE

	lds r16,MINUTES
	rcall DecToBCD
	sts DATA_I2C_T, r16

	rcall I2C_SEND; BCD MINUTES BYTE WRITE

	lds r16,HOURS
	rcall DecToBCD
	sts DATA_I2C_T, r16

	rcall I2C_SEND; BCD HOURS BYTE WRITE

	rcall I2C_STOP

.ENDM


OutLCD_TIME:
push yl
push yh
push r16
push r17

	ldi yl,low(TmpVar_LCD)
	ldi yh,high(TmpVar_LCD)
	
	lds r16, HOURS
	clr r17
;ldi r16,55
	rcall Number99

	ldi r16,':'
	st y+,r16

	lds r16, MINUTES
	clr r17
;ldi r16,55
	rcall Number99

	ldi r16,':'
	st y+,r16

	lds r16, SECONDS
	clr r17
;ldi r16,55
	rcall Number99

	ldi		zl,Low(TmpVar_LCD)		;Укажем адрес начальной строки в памяти
	ldi		zh,High(TmpVar_LCD)	
	ldi		R16,$C0				;Укажем что выводим на 2 строку
	rcall	String_To_LCD		;Выводим на 2 строку начальную запись

pop r17
pop r16
pop yh
pop yl

ret




; резервирование нескольких переменных в ОЗУ
.dseg
SLA_W:		.BYTE 1
DATA_I2C_T:	.BYTE 1
DATA_I2C_R:	.BYTE 1




SECONDS:	.BYTE 1
MINUTES:	.BYTE 1
HOURS:		.BYTE 1

DAY:		.BYTE 1
DATE:		.BYTE 1
MONTH:		.BYTE 1
YEAR:		.BYTE 1


.cseg


; подпрограмма инициализации I2C
I2C_INIT:

;ldi r16,92
;sts TWBR,r16

	lds r16, SLA_W
	sts TWDR, r16 
	ldi r16, (1<<TWINT)|(1<<TWEN)
	sts TWCR, r16

	ret
		
I2C_START:

	ldi r16, (1<<TWINT)|(1<<TWSTA)|(1<<TWEN)
	sts TWCR, r16

 wait1:
	lds r16,TWCR
	sbrs r16,TWINT
	rjmp wait1

	ret

I2C_SEND:

	lds r16, DATA_I2C_T
	sts TWDR, r16       
	ldi r16, (1<<TWINT)|(1<<TWEN)
	sts TWCR, r16
	
	rjmp wait1

;wait2:
;	lds r16,TWCR
;	sbrs r16,TWINT
;	rjmp wait2
;
;	ret

I2C_RECEIVE:

	ldi r16, (1<<TWINT)|(1<<TWEN|1<<TWEA)
	sts TWCR, r16

wait3:
	lds r16,TWCR
	sbrs r16,TWINT
	rjmp wait3

 	lds r16,TWDR
	sts DATA_I2C_R,r16
	ret

I2C_RECEIVE_LAST:

	ldi r16, (1<<TWINT)|(1<<TWEN)
	sts TWCR, r16

	rjmp wait3

;wait4:
;	lds r16,TWCR
;	sbrs r16,TWINT
;	rjmp wait4
;
 ;	lds r16,TWDR
;	sts DATA_I2C_R,r16
;	ret

CHECK:
	lds r16,TWSR
	andi r16, 0xF8
;	cpi r16, MT_DATA_ACK
	cp r16,r17
	brne ERROR_I2C
	ret

ERROR_I2C:
I2C_STOP:

	ldi r16, (1<<TWINT)|(1<<TWEN)|(1<<TWSTO)
	sts TWCR, r16 

;wait5:
;	lds r16,TWCR
;	sbrs r16,TWINT
;	rjmp wait5

	ret
