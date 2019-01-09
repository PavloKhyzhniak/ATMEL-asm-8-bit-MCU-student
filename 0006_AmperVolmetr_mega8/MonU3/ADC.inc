IntADC:
	push	temp
	in	temp,SREG
	push	temp
	lds	temp,NumADCkan
	SBRC	temp,0
	rjmp	ReadADC1	;x1
	SBRC	temp,1
	rjmp	ReadADC2	;10
	
	in	temp,ADCL
	sts	ADC0l,temp
	in	temp,ADCH
	sts	ADC0h,temp
	ldi	temp,1
	sts	NumADCkan,temp
		;Задержка в 6+32 тактов, всего 64+2 такта с окончания преобразования
	ldi	temp,13
Loop0IntADC:
	dec	temp
	BRNE	Loop0IntADC
		;0-й уже считан, 1-й начал считаться, 2-й в очередь.
	ldi	temp,0b11000000+2; Выбрать 2-й канал АЦП
	out	ADMUX,temp
	;sbi	ADCSR,ADSC ;Старт нового преобразования
	pop	temp
	out	SREG,temp	
	pop	temp
	reti

ReadADC1:
	SBRC	temp,1
	rjmp	ReadADC3	;11
	in	temp,ACSR	;Сохранить состояние компаратора при изм. A
	mov	CompStat,temp
	in	temp,ADCL
	sts	ADC1l,temp
	in	temp,ADCH
	sts	ADC1h,temp
	ldi	temp,2
	sts	NumADCkan,temp
		;Задержка в 3+32 тактов, всего 64+2 такта с окончания преобразования
	ldi	temp,12
Loop1IntADC:
	dec	temp
	BRNE	Loop1IntADC
	ldi	temp,0b11000000+3; Выбрать 3-й канал АЦП
	;ori	temp,0b11000000
	out	ADMUX,temp
	;sbi	ADCSR,ADSC ;Старт нового преобразования
	pop	temp
	out	SREG,temp	
	pop	temp
	reti

ReadADC2:
	in	temp,ADCL
	sts	ADC2l,temp
	in	temp,ADCH
	sts	ADC2h,temp
	ldi	temp,3
	sts	NumADCkan,temp
		;Задержка в 6+32 тактов, всего 64+2 такта с окончания преобразования
	ldi	temp,13
Loop2IntADC:
	dec	temp
	BRNE	Loop2IntADC
	ldi	temp,0b11000000+0; Выбрать 0-й канал АЦП
	;ori	temp,0b11000000
	out	ADMUX,temp
	;sbi	ADCSR,ADSC ;Старт нового преобразования
	pop	temp
	out	SREG,temp	
	pop	temp
	reti

ReadADC3:
	in	temp,ADCL
	sts	ADC3l,temp
	in	temp,ADCH
	sts	ADC3h,temp
	ldi	temp,0
	sts	NumADCkan,temp
	;sbi	ADCSR,ADSC ;Старт нового преобразования
;-----------Прочитаны все 4 канала----------------------
	push	temp2
	push	temp3
	push	temp4
	push	r2
	push	r1
	push	r0
	push	xh
	push	xl
	lds	xl,CountADCvib
	lds	xh,CountADCvib+1
	cpi	xl,0
	jnz	No0ADCstart
	cpi	xh,0
	jnz	No0ADCstart
	rcall	Set0MinMax	;Установка min/max в начале секунды
	rjmp	NoCalcMinMax	;В первый раз не нужно находить min/max
No0ADCstart:			;
	rcall	CalcMinMax
NoCalcMinMax:
	adiw	xl,1
	sts	CountADCvib,xl
	sts	CountADCvib+1,xh
	inc	CountVibInPer	;Определение конца периода
	mov	temp,CountVibInPer
	cpi	temp,33
	jc	SumMed0C
	mov	temp,CompStat
	;in	temp,ACSR
	andi	temp,$20
	jnz	SumMed0C
	lds	temp,CountPer
	inc	temp
	sts	CountPer,temp
	ldi	temp,0
	mov	CountVibInPer,temp

SumMed0C:
	rcall	SumAllVib
	ldi	temp,0b11000000+1; Выбрать 1-й канал АЦП
	;ori	temp,0b11000000
	out	ADMUX,temp
	rcall	Sum2AllVib
;-------------------------------------------------------------------
ExitSumADC:
	pop	xl
	pop	xh
	pop	r0
	pop	r1
	pop	r2
	pop	temp4
	pop	temp3
	pop	temp2
;-----------------------------------------------------
	pop	temp
	out	SREG,temp	
	pop	temp
	reti

;<><><><><><><><><><><><>Минимальное и максимальное значение<><><><>
CalcMinMax:
	lds	temp4,ADC0h	;текущее
	lds	temp3,ADC0l
	lds	temp,ADC0min+1
	cp	temp,temp4	;минимальное<текущего - пропускаем
	jc	NoADC0minSet
	jnz	GoADC0minSet
	lds	temp,ADC0min
	cp	temp3,temp	;текущее<минимального - ok!
	jnc	NoADC0minSet
GoADC0minSet:
	sts	ADC0min,temp3
	sts	ADC0min+1,temp4
NoADC0minSet:
	;lds	temp4,ADC0h	;текущее
	;lds	temp3,ADC0l
	lds	temp,ADC0max+1
	cp	temp4,temp	;текущее<максимального - пропускаем
	jc	NoADC0maxSet
	jnz	GoADC0maxSet
	lds	temp,ADC0max
	cp	temp,temp3	;текущее>максимального - ok!
	jnc	NoADC0maxSet
GoADC0maxSet:
	sts	ADC0max,temp3
	sts	ADC0max+1,temp4
NoADC0maxSet:
	
	lds	temp4,ADC1h	;текущее
	lds	temp3,ADC1l
	lds	temp,ADC1min+1
	cp	temp,temp4	;минимальное<текущего - пропускаем
	jc	NoADC1minSet
	jnz	GoADC1minSet
	lds	temp,ADC1min
	cp	temp3,temp	;текущее<минимального - ok!
	jnc	NoADC1minSet
GoADC1minSet:
	sts	ADC1min,temp3
	sts	ADC1min+1,temp4
NoADC1minSet:
	lds	temp,ADC1max+1
	cp	temp4,temp	;текущее<максимального - пропускаем
	jc	NoADC1maxSet
	jnz	GoADC1maxSet
	lds	temp,ADC1max
	cp	temp,temp3	;текущее>максимального - ok!
	jnc	NoADC1maxSet
GoADC1maxSet:
	sts	ADC1max,temp3
	sts	ADC1max+1,temp4
NoADC1maxSet:
	
	lds	temp4,ADC2h	;текущее
	lds	temp3,ADC2l
	lds	temp,ADC2min+1
	cp	temp,temp4	;минимальное<текущего - пропускаем
	jc	NoADC2minSet
	jnz	GoADC2minSet
	lds	temp,ADC2min
	cp	temp3,temp	;текущее<минимального - ok!
	jnc	NoADC2minSet
GoADC2minSet:
	sts	ADC2min,temp3
	sts	ADC2min+1,temp4
NoADC2minSet:
	lds	temp,ADC2max+1
	cp	temp4,temp	;текущее<максимального - пропускаем
	jc	NoADC2maxSet
	jnz	GoADC2maxSet
	lds	temp,ADC2max
	cp	temp,temp3	;текущее>максимального - ok!
	jnc	NoADC2maxSet
GoADC2maxSet:
	sts	ADC2max,temp3
	sts	ADC2max+1,temp4
NoADC2maxSet:
	
	lds	temp4,ADC3h	;текущее
	lds	temp3,ADC3l
	lds	temp,ADC3min+1
	cp	temp,temp4	;минимальное<текущего - пропускаем
	jc	NoADC3minSet
	jnz	GoADC3minSet
	lds	temp,ADC3min
	cp	temp3,temp	;текущее<минимального - ok!
	jnc	NoADC3minSet
GoADC3minSet:
	sts	ADC3min,temp3
	sts	ADC3min+1,temp4
NoADC3minSet:
	lds	temp,ADC3max+1
	cp	temp4,temp	;текущее<максимального - пропускаем
	jc	NoADC3maxSet
	jnz	GoADC3maxSet
	lds	temp,ADC3max
	cp	temp,temp3	;текущее>максимального - ok!
	jnc	NoADC3maxSet
GoADC3maxSet:
	sts	ADC3max,temp3
	sts	ADC3max+1,temp4
NoADC3maxSet:
	ret

;***************************Сумма квадратов***********
Sum2AllVib:
	lds	temp,ADC0l
	lds	temp2,ADC0h
	mul	temp,temp	;temp3:temp4  
	mov	temp3,r0	;      temp : r2
	mov	temp4,r1	;      temp : r2 
	mul	temp,temp2	;             r0 : r1
	mov	temp,r0
	mov	r2,r1
	mul	temp2,temp2	;r1=0
	LSL	temp	;temp:r2+temp:r2
	ROL	r2	;
	add	temp4,temp
	adc	r0,r2	;temp3:temp4:r0:r1
	ld	temp,X
	add	temp,temp3
	st	X+,temp
	ld	temp,X
	adc	temp,temp4
	st	X+,temp
	ld	temp,X
	adc	temp,r0
	st	X+,temp
	ld	temp,X
	adc	temp,r1
	st	X+,temp
	
	lds	temp,ADC1l
	lds	temp2,ADC1h
	mul	temp,temp	;temp3:temp4  
	mov	temp3,r0	;      temp : r2
	mov	temp4,r1	;      temp : r2 
	mul	temp,temp2	;             r0 : r1
	mov	temp,r0
	mov	r2,r1
	mul	temp2,temp2	;r1=0
	LSL	temp	;temp:r2+temp:r2
	ROL	r2	;
	add	temp4,temp
	adc	r0,r2	;temp3:temp4:r0:r1
	ld	temp,X
	add	temp,temp3
	st	X+,temp
	ld	temp,X
	adc	temp,temp4
	st	X+,temp
	ld	temp,X
	adc	temp,r0
	st	X+,temp
	ld	temp,X
	adc	temp,r1
	st	X+,temp
	
	lds	temp,ADC2l
	lds	temp2,ADC2h
	mul	temp,temp	;temp3:temp4  
	mov	temp3,r0	;      temp : r2
	mov	temp4,r1	;      temp : r2 
	mul	temp,temp2	;             r0 : r1
	mov	temp,r0
	mov	r2,r1
	mul	temp2,temp2	;r1=0
	LSL	temp	;temp:r2+temp:r2
	ROL	r2	;
	add	temp4,temp
	adc	r0,r2	;temp3:temp4:r0:r1
	ld	temp,X
	add	temp,temp3
	st	X+,temp
	ld	temp,X
	adc	temp,temp4
	st	X+,temp
	ld	temp,X
	adc	temp,r0
	st	X+,temp
	ld	temp,X
	adc	temp,r1
	st	X+,temp
	
	lds	temp,ADC3l
	lds	temp2,ADC3h
	mul	temp,temp	;temp3:temp4  
	mov	temp3,r0	;      temp : r2
	mov	temp4,r1	;      temp : r2 
	mul	temp,temp2	;             r0 : r1
	mov	temp,r0
	mov	r2,r1
	mul	temp2,temp2	;r1=0
	LSL	temp	;temp:r2+temp:r2
	ROL	r2	;
	add	temp4,temp
	adc	r0,r2	;temp3:temp4:r0:r1
	ld	temp,X
	add	temp,temp3
	st	X+,temp
	ld	temp,X
	adc	temp,temp4
	st	X+,temp
	ld	temp,X
	adc	temp,r0
	st	X+,temp
	ld	temp,X
	adc	temp,r1
	st	X+,temp
	ret

;++++++++++++++++++++++++++++++++Суммирование++++++++++++++++++++++++
SumAllVib:
	ldi	temp4,0
	ldi	xl,low(SumADC0)
	ldi	xh,high(SumADC0)

	lds	temp,ADC0l
	lds	temp2,ADC0h
	ld	temp3,X
	add	temp3,temp
	st	X+,temp3
	ld	temp3,X
	adc	temp3,temp2
	st	X+,temp3
	ld	temp3,X
	adc	temp3,temp4
	st	X+,temp3
	ld	temp3,X
	adc	temp3,temp4
	st	X+,temp3

	lds	temp,ADC1l
	lds	temp2,ADC1h
	ld	temp3,X
	add	temp3,temp
	st	X+,temp3
	ld	temp3,X
	adc	temp3,temp2
	st	X+,temp3
	ld	temp3,X
	adc	temp3,temp4
	st	X+,temp3
	ld	temp3,X
	adc	temp3,temp4
	st	X+,temp3

	lds	temp,ADC2l
	lds	temp2,ADC2h
	ld	temp3,X
	add	temp3,temp
	st	X+,temp3
	ld	temp3,X
	adc	temp3,temp2
	st	X+,temp3
	ld	temp3,X
	adc	temp3,temp4
	st	X+,temp3
	ld	temp3,X
	adc	temp3,temp4
	st	X+,temp3

	lds	temp,ADC3l
	lds	temp2,ADC3h
	ld	temp3,X
	add	temp3,temp
	st	X+,temp3
	ld	temp3,X
	adc	temp3,temp2
	st	X+,temp3
	ld	temp3,X
	adc	temp3,temp4
	st	X+,temp3
	ld	temp3,X
	adc	temp3,temp4
	st	X+,temp3
	ret

Set0MinMax:
	lds	temp,ADC0l
	sts	ADC0min,temp
	sts	ADC0max,temp
	lds	temp,ADC0h
	sts	ADC0min+1,temp
	sts	ADC0max+1,temp
	lds	temp,ADC1l
	sts	ADC1min,temp
	sts	ADC1max,temp
	lds	temp,ADC1h
	sts	ADC1min+1,temp
	sts	ADC1max+1,temp
	lds	temp,ADC2l
	sts	ADC2min,temp
	sts	ADC2max,temp
	lds	temp,ADC2h
	sts	ADC2min+1,temp
	sts	ADC2max+1,temp
	lds	temp,ADC3l
	sts	ADC3min,temp
	sts	ADC3max,temp
	lds	temp,ADC3h
	sts	ADC3min+1,temp
	sts	ADC3max+1,temp
	ret

InitADC:	;      +   +===  +             +                    ===
	ldi	temp,0b10001110; Включить АЦП, Разрешить прерывания,f=CLOCK/32
	;ldi	temp,0b10001001; Включить АЦП, Разрешить прерывания,f=CLOCK/2
	out	ADCSR,temp
	sbi	ADCSR,ADFR	;Непрерывное преобразование АЦП
	ldi	temp,0b11000000; Выбрать 0-й канал АЦП
	out	ADMUX,temp
	;ldi	temp,0
	;sts	NumADCkan,temp	;Начинаем с нулевого канала
	ret

StartADC:
	ldi	temp,0
	sts	NumADCkan,temp	;Начинаем с нулевого канала
	ldi	temp,0b11000000; Выбрать 0-й канал АЦП
	out	ADMUX,temp
	sbi	ADCSR,ADSC	;Запуск измерения 0-го канала
	ldi	temp,64
Loop1StartADC:			;Выжидаем 3 такта АЦП
	dec	temp
	BRNE	Loop1StartADC
	ldi	temp,0b11000000+1; Выбрать 1-й канал АЦП для следующего преобразования
	out	ADMUX,temp
	ret

