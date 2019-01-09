;************************************
;** ‘айл вспомогательных подпрограмм
;**
;**
;************************************

		;-------------------------- ѕодпрограммы

;пример подпрограммы подготовки данных на вывод
; будь то температура, напр€жение, значение тока или друга€ цифрова€ информаци€(не буквенна€)
; при необходимости отображать признак точки он должен быть добавлен к извлеченной константе

NumberTC:
; подпрограмма определени€ цифр дл€ отображени€
; рабочий байт передаетьс€ из 
;	tmpL2 это младший байт
; 	tmpH2 это старший байт
push r16
push r17
push r18
push r19
push r20

; запись дес€тков тыс€ч если есть
		ldi r18, low(10000)
		ldi r19, high(10000)
		rcall digit
		cpi r20,0
		breq zeroFound1

		ldi	R19,$30
		add	R20,R19
		st	y+,R20
		rjmp weiterNotZero

zeroFound1:

		ldi	R20,$20
		st	y+,R20
; запись тыс€ч если небыло дес€тков тыс€ч
		ldi r18, low(1000)
		ldi r19, high(1000)
		rcall digit
		cpi r20,0
		breq zeroFound2
	
		ldi	R19,$30
		add	R20,R19
		st	y+,R20
		rjmp weiter
weiterNotZero:
; запись тыс€ч если были дес€тки тыс€ч
		ldi r18, low(1000)
		ldi r19, high(1000)
		rcall digit
		ldi	R19,$30
		add	R20,R19
		st	y+,R20
		rjmp weiter
zeroFound2:
		ldi	R20,$20
		st	y+,R20
weiter:
; запись сотен
		ldi r18, low(100)
		ldi r19, high(100)
		rcall digit
		ldi	R19,$30
		add	R20,R19
		st	y+,R20

; запись дес€тков
		ldi tmpL2, low(10)
		ldi tmpH2, high(10)
		rcall digit
		ldi	R19,$30
		add	R20,R19
		st	y+,R20
; запись единиц
		ldi	R19,$30
		add	R16,R19
		st	y+,R16
		
pop r20
pop r19
pop r18
pop r17
pop r16
		ret


Number99:
; подпрограмма определени€ цифр дл€ отображени€
; рабочий байт передаетьс€ из 
;	tmpL2 это младший байт
; 	tmpH2 это старший байт
push r16
push r17
push r18
push r19
push r20

; запись сотен
		ldi r18, low(100)
		ldi r19, high(100)
		rcall digit

; запись дес€тков
		ldi tmpL2, low(10)
		ldi tmpH2, high(10)
		rcall digit
		ldi	R19,$30
		add	R20,R19
		st	y+,R20
; запись единиц
		ldi	R19,$30
		add	R16,R19
		st	y+,R16
		
pop r20
pop r19
pop r18
pop r17
pop r16
		ret

; подпрограмма разложени€ числа на разр€ды
digit:	ldi r20, -1	; сразу инициализируем -1
digit_1:inc r20		; инкрементируем, т.е. впервый раз начинаем с нул€!!!
		sub r16, r18	; вычитание младшего байта
		sbc r17, r19	; вычитание с переносом старшего байта
		brsh digit_1	; в случае коль не отрицательное повторить
		add r16, r18	; иначе восстанавливаем последнее значение
		adc r17, r19	; в итоге остаетс€ все что меньше вычитаемого
						; т.е. вычита€ 10 остаток единицы!!!
		ret

; подпрограмма извлечени€ константы дл€ индикатора из таблицы
;number_init:
;		ldi zh, high(TableIndicator*2)
;		ldi zl, low(TableIndicator*2)
;		add zl, cnt1
;		clr cnt1
;		adc zh, cnt1
;		lpm	; результат в R0
;		ret


;-----------------------------------------------------------------------------:
; 16bit/16bit Unsigned Division
;
; Register Variables
;  Call:  var1[1:0] = dividend (0x0000..0xffff)
;         var2[1:0] = divisor (0x0001..0x7fff)
;         mod[1:0]  = <don't care>
;         lc        = <don't care> (high register must be allocated)
;
;  Result:var1[1:0] = var1[1:0] / var2[1:0]
;         var2[1:0] = <not changed>
;         mod[1:0]  = var1[1:0] % var2[1:0]
;         lc        = 0
;
; Size  = 16 words
; Clock = 179..211 cycles (+ret)
; Stack = 0 byte


div16u:

		clr	R20		;initialize variables
		clr	R21		;  mod = 0;
		ldi	R22,16		;  lc = 16;
					;---- calcurating loop
		lsl	R16		;var1 = var1 << 1;
		rol	R17		;/
		rol	R20		;mod = mod << 1 + carry;
		rol	R21		;/
		cp	R20,R18	;if (mod => var2) {
		cpc	R21,R19	; mod -= var2; var1++;
		brcs	PC+4		; }
		inc	R16		;
		sub	R20,R18	;
		sbc	R21,R19	;/
		dec	R22		;if (--lc > 0)
		brne	PC-11		; continue loop;

		ret 
;-----------------------------------------------------------------------------:
; 16bit x 16bit unsigned multiply
;
; Register Variables
;  Call:  var1[1:0] = 16bit multiplicand
;         var1[3:2] = <don't care>
;         var2[1:0] = 16bit multiplier
;         lc        = <don't care> (high register must be allocated)
;
;  Result:var1[3:0] = 32bit result of var1[1:0] * var2[1:0]
;         var2[1:0] = <not changed>
;         lc        = 0
;
; Size  = 13 words
; Clock = 157..172 cycles (+ret)
; Stack = 0 byte


;mul16u:
;
;		sub	var13,var13	;initialize variables
;		sub	var12,var12	;
;		ldi	lc,17		; lc = 17;
;		brcc	PC+3		;---- calcurating loop
;		add	var12,var20	;
;		adc	var13,var21	;
;		ror	var13		;
;		ror	var12		;
;		ror	var11		;
;		ror	var10		;
;		dec	lc		;if (--lc > 0)
;		brne	PC-8		; continue loop;
;		ret 














BCDtoDEC:
	
	push r16
	swap r16
	clr r17
	cbr r16,0xF0
	mov r17,r16
	clr r18

mult:
	add r16,r17
	inc r18
	cpi r18,9
	brne mult

	mov r17,r16
	pop r16
	cbr r16,0xF0
	add r16,r17

	ret

DecToBCD:

	cpi r16,10
	brlo exit
	
	push r16
	clr r18
	ldi r17,10

round1:
	sub r16,r17
	inc r18
	cpi r18,10
	brge round1

	clr r19
	clr r16

round2:
	add r19,r17
	inc r16
	cp r16,r18
	brne round2

	pop r16
	sub r16,r19
	swap r18
	add r16,r19
exit:
	ret
