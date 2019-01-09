;************************************
;** Файл вспомогательных подпрограмм
;**
;**
;************************************

		;-------------------------- Подпрограммы

;пример подпрограммы подготовки данных на вывод
; будь то температура, напряжение, значение тока или другая цифровая информация(не буквенная)
; при необходимости отображать признак точки он должен быть добавлен к извлеченной константе


NumberA:
; подпрограмма определения цифр для отображения
; рабочий байт передаеться из 
;	tmpL2 это младший байт
; 	tmpH2 это старший байт
		
		ldi tmpL, low(10000)
		ldi tmpH, high(10000)
		rcall digit
		mov r16,r0
		ori r16,DP
		sts sA1, r16

		ldi tmpL, low(1000)
		ldi tmpH, high(1000)
		rcall digit
		sts sA2, r0

		ldi tmpL, low(100)
		ldi tmpH, high(100)
		rcall digit
		sts sA3, r0
		
		ldi tmpL, low(10)
		ldi tmpH, high(10)
		rcall digit
;		mov cnt1, tmpL2
;		rcall number_init
		sts sA4, r0
		
		ret;

NumberV:
; подпрограмма определения цифр для отображения
; рабочий байт передаеться из 
;	tmpL2 это младший байт
; 	tmpH2 это старший байт
		
		ldi tmpL, low(10000)
		ldi tmpH, high(10000)
		rcall digit
;		sts sV1, r0

		ldi tmpL, low(1000)
		ldi tmpH, high(1000)
		rcall digit
		sts sV1, r0

		ldi tmpL, low(100)
		ldi tmpH, high(100)
		rcall digit
		mov r16,r0
		ori r16,DP
		sts sV2, r16

		ldi tmpL, low(10)
		ldi tmpH, high(10)
		rcall digit
		sts sV3, r0
		
		ret;
;
; подпрограмма разложения числа на разряды
digit:	ldi r20, -1	; сразу инициализируем -1
digit_1:inc r20		; инкрементируем, т.е. впервый раз начинаем с нуля!!!
		sub r18, r16	; вычитание младшего байта
		sbc r19, r17	; вычитание с переносом старшего байта
		brsh digit_1	; в случае коль не отрицательное повторить
		add r18, r16	; иначе восстанавливаем последнее значение
		adc r19, r17	; в итоге остается все что меньше вычитаемого
						; т.е. вычитая 10 остаток единицы!!!

		ldi zh, high(TableIndicator*2)
		ldi zl, low(TableIndicator*2)
		add zl, r20
		clr r20
		adc zh, r20
		lpm	; результат в R0
		ret

;*******************************
