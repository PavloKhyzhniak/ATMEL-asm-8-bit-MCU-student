;**************************************
; работа с клавиатурой
; реализуем опрос кнопок программно
;
work_Button:
;-------------------------- Инициализация Порта ввода/вывода B

		ldi		r16, 0xFF	; Записываем число $FF в регистр r16
		out		DDRB, r16	; Записываем это число в DDRB
		ldi		r16, 0xF0	; Записываем число $F0 в регистр r16
		out		PORTB, r16	; Записываем то  же число в PORTB
; проинициализировали порт B на вывод 

;-------------------------- Инициализация Порта ввода/вывода D

		ldi		r16, 0x00	; Записываем число $00 в регистр r16
		out		DDRD, r16	; Записываем это число в DDRD
		ldi		r16, 0x00	; Записываем число $00 в регистр r16
		out		PORTD, r16	; Записываем то  же число в PORTD
; проинициализировали порт D на ввод 

searchBut:
	in	tmp1,PIND 
	andi	tmp1,0x0F
	cpi tmp1,0x0F
	breq searchBut
	
	clr tmp2
	ldi cnt1,16
find_but:
	mov tmp2,tmp1

	in	tmp1,PIND 
	andi	tmp1,0x0F

	cp tmp2,tmp1
	brne find_but

	dec cnt1
	brne find_but

; преобразуем регистр для вывода
	ldi tmp3,0x0F
	eor tmp2,tmp3

	out PORTB,tmp2

	rjmp searchBut

