GetCID:
	cbi	SPI_port,SPI_SS	;Активируем MMC
	cbi	SPI_port,SPI_LED	;Включаем светодиод
	ldi	xh,high(Buf_CID)
	ldi	xl,low(Buf_CID)
	rcall	GetSPIbyte	;CMD10
	ldi	temp,0x40+10
	mov	r15,temp	;!!!!!
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
Loop1GetCID:
	rcall	GetSPIbyte	;Ожидание ответа
	mov	r14,temp	;!!!!!
	cpi	temp,0xff
	jz	Loop1GetCID
	mov	r13,temp
Loop2GetCID:
	rcall	GetSPIbyte	;Ожидание 0xFE
	mov	r14,temp	;!!!!!
	cpi	temp,0xff
	jz	Loop2GetCID
	mov	r12,temp
	ldi	temp2,16
Loop3GetCID:
	rcall	GetSPIbyte
	st	X+,temp
	dec	temp2
	jnz	Loop3GetCID	
	rcall	GetSPIbyte
	rcall	GetSPIbyte
	rcall	GetSPIbyte
	rcall	GetSPIbyte
	sbi	SPI_port,SPI_LED	;Выключаем светодиод
	sbi	SPI_port,SPI_SS	;Отключаем MMC
	ret

GetCSD:
	cbi	SPI_port,SPI_SS	;Активируем MMC
	cbi	SPI_port,SPI_LED	;Включаем светодиод
	ldi	xh,high(Buf_CSD)
	ldi	xl,low(Buf_CSD)
	rcall	GetSPIbyte	;CMD9
	ldi	temp,0x40+9
	mov	r15,temp	;!!!!!
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
Loop1GetCSD:
	rcall	GetSPIbyte	;Ожидание ответа
	mov	r14,temp	;!!!!!
	cpi	temp,0xff
	jz	Loop1GetCSD
	mov	r13,temp
Loop2GetCSD:
	rcall	GetSPIbyte	;Ожидание 0xFE
	mov	r14,temp	;!!!!!
	cpi	temp,0xff
	jz	Loop2GetCSD
	mov	r12,temp
	ldi	temp2,16
Loop3GetCSD:
	rcall	GetSPIbyte
	st	X+,temp
	dec	temp2
	jnz	Loop3GetCSD	
	rcall	GetSPIbyte
	rcall	GetSPIbyte
	rcall	GetSPIbyte
	rcall	GetSPIbyte
	sbi	SPI_port,SPI_LED	;Выключаем светодиод
	sbi	SPI_port,SPI_SS	;Отключаем MMC
	ret

