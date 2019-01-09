;************************************
;**
;**
;**
;************************************


ADC_INT:
push r16
in r16,SREG
push r16
push r17
push r18
push r19

; считываем полученные данные
	in r16,ADCL
	in r17,ADCH

	ldi zl,low(ADC_Conv)
	ldi zh,high(ADC_Conv)
	add zl,cntADC
	clr r18
	adc zh,r18
	ijmp

ADC_Conv:
	rjmp sAerror
	rjmp sVoltBig
	rjmp sVoltSmall
	rjmp sAtek
	
testAmper:
; организуем проверку мгновенных значений уставки тока и текущего тока
	lds r16,Atek
	lds r17,Aerror
	cp r16,r17
	brsh ErrorAmp
; ток не превышает уставки
	sbi DDRD,ErrorAmper
	cbi PORTD,ErrorAmper
	ret

ErrorAmp:
; ток превышает уставку
	sbi DDRD,ErrorAmper
	sbi PORTD,ErrorAmper
	ret

sAerror:
;выделяем мгновенное значение
	lsr r17
	ror r16
	lsr r17
	ror r16
	
	sts Aerror,r16

	rcall testAmper
rcall ChannelInc

	rjmp extADC_INT

testVolt:

lds r16,VoltSmallS
lds r17,VoltSmallS+1
; проверим не подошли ли мы к верхнему пределу по малому напряжению
ldi r18,low(VoltSmallMax)
ldi r19,high(VoltSmallMax)
cp	r18,r16
cpc	r19,r17
brsh normVolt
; подошли? значит выводим по большому напряжению
lds r16,VoltBigS
lds r17,VoltBigS+1
normVolt:

mov r18,r16
mov r19,r17


;усредним значения
lds r16,Aind
lds r17,Aind+1
add r18,r16
adc r19,r17

lsr r19
ror r18

	sts Vind,r18
	sts Vind+1,r19

;ldi r18,low(12345)
;ldi r19,high(12345)

rcall NumberV

	ret

sVoltBig:
; накапливаем значения по напряжению большому
	lds r18,VoltBig
	lds r19,VoltBig+1
	add r18,r16
	adc r19,r17

	sts VoltBig,r18
	sts VoltBig+1,r19

;проверим счетчик накопления
	lds r16,cntVoltBig
	inc r16
	sts cntVoltBig,r16

	cpi r16,cpiVoltBig
	brne extADC_INT1

;/8
lsr r19
ror r18
lsr r19
ror r18
lsr r19
ror r18


	sts VoltBigS,r18
	sts VoltBigS+1,r19

	clr r16
	sts cntVoltBig,r16

	rcall testVolt

	clr r16
	sts VoltBig,r16
	sts VoltBig+1,r16
rcall ChannelInc

extADC_INT1:
	rjmp extADC_INT


ChannelInc:
; указываем на следующий канал
	inc cntADC

; проверим все ли каналы опросили
; бегаем по кругу ,всего 4е канала
	mov r16,cntADC
	cpi r16,4
	brne normADC
clearCntADC:
	clr cntADC
normADC:

ret



sVoltSmall:
; накапливаем значения по напряжению малому
	lds r18,VoltSmall
	lds r19,VoltSmall+1
	add r18,r16
	adc r19,r17

	sts VoltSmall,r18
	sts VoltSmall+1,r19

;проверим счетчик накопления
	lds r16,cntVoltSmall
	inc r16
	sts cntVoltSmall,r16

	cpi r16,cpiVoltSmall
	brne extADC_INT1

	clr r16
	sts cntVoltSmall,r16

;/16
lsr r19
ror r18
lsr r19
ror r18
lsr r19
ror r18
lsr r19
ror r18

	sts VoltSmallS,r18
	sts VoltSmallS+1,r19

	rcall testVolt

	clr r16
	sts VoltSmall,r16
	sts VoltSmall+1,r16
rcall ChannelInc

	rjmp extADC_INT

sAtek:
; накапливаем значения по току
	lds r18,Amper
	lds r19,Amper+1
	add r18,r16
	adc r19,r17

	sts Amper,r18
	sts Amper+1,r19
;выделяем мгновенное значение
	lsr r17
	ror r16
	lsr r17
	ror r16
	
	sts Atek,r16

	rcall testAmper

;проверим счетчик накопления
	lds r16,cntAmper
	inc r16
	sts cntAmper,r16

	cpi r16,cpiAmper
	brne extADC_INT

;/4
lsr r19
ror r18
lsr r19
ror r18

;усредним значения
lds r16,Aind
lds r17,Aind+1
add r18,r16
adc r19,r17

lsr r19
ror r18

	sts Aind,r18
	sts Aind+1,r19

;ldi r18,low(12345)
;ldi r19,high(12345)

rcall NumberA

	clr r16
	sts cntAmper,r16
	sts Amper,r16
	sts Amper+1,r16
rcall ChannelInc

	rjmp extADC_INT


extADC_INT:



pop r19
pop r18
pop r17
pop r16
out SREG,r16
pop r16	

	reti
