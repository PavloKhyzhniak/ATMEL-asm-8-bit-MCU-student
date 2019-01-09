;************************************
;**
;**
;**
;************************************

; первоначальный сброс
RESET:
; запрещаем прерывания
	cli

;************************************
;	инициализация стека
		ldi r16,high(RAMEND) ; Main program start
		out SPH,r16 ; Set stack pointer to top of RAM
		ldi r16,low(RAMEND)
		out SPL,r16
;************************************

;************************************

	;Port Pin Configurations
	;DDxn     PORTxn	PUD (in 
	;					MCUCR2)         I/O        Pull-up     	Comment
	;0 			0 		X 				Input      No        	Tri-state (Hi-Z)
	;0 			1 		0 				Input      Yes			Pxn will source current if ext. pulled low.
	;0 			1 		1 				Input      No        	Tri-state (Hi-Z)
	;1 			0 		X 				Output     No	        Output Low (Sink)
	;1 			1 		X 				Output     No        	Output High (Source)
	
;-------------------------- Инициализация Порта ввода/вывода B

		ldi		r16, 0x3F	; Записываем число $00 в регистр temp
		out		DDRB, r16	; Записываем это число в DDRС
		ldi		r16, 0x00	; Записываем число $00 в регистр temp
		out		PORTB, r16	; Записываем то  же число в PORTС

		ldi		r16, 0x30	; Записываем число $00 в регистр temp
		out		DDRC, r16	; Записываем это число в DDRС
		ldi		r16, 0x00	; Записываем число $00 в регистр temp
		out		PORTC, r16	; Записываем то  же число в PORTС

		ldi		r16, 0xFF	; Записываем число $00 в регистр temp
		out		DDRD, r16	; Записываем это число в DDRС
		ldi		r16, 0x00	; Записываем число $00 в регистр temp
		out		PORTD, r16	; Записываем то  же число в PORTС

;************************************

;-------------------------- Инициализация компаратора
; отключаем аналоговый компаратор для энерго сбережения
		ldi r16, 0x80		; выключение компаратора
		out ACSR, r16

;**********************************
;**********************************
	ldi r16, 1<<CS02|0<<CS01|1<<CS00
	out TCCR0,r16

	ldi r16, 1<<TOIE0
	out TIMSK,r16

	ldi r16, (-78)
	out TCNT0,r16

;**********************************
;опорное на 2,54В внешнее, подключаем АналогВход 0(РС0),нормирование на младший байт
	 ldi r16,0<<REFS1|0<<REFS0|0<<MUX2|0<<MUX1|0<<MUX0|0<<ADLAR
	 out ADMUX,r16
;разрешим работу АЦП, разрешим прерывание, делитель на 32
	 ldi r16, 1<<ADEN|1<<ADIE|0<<ADFR|1<<ADPS2|1<<ADPS1|1<<ADPS0
	 out ADCSRA,r16

;************************************
	;Watchdog Timer Control Register - WDTCSR
	;7		6		5		4		3		2		1		0
	;WDIF   WDIE    WDP3    WDCE    WDE     WDP2    WDP1    WDP0  
	; Bit 7 - WDIF: Watchdog Interrupt Flag
	; Bit 6 - WDIE: Watchdog Interrupt Enable
	; Bit 4 - WDCE: Watchdog Change Enable
	; Bit 3 - WDE: Watchdog System Reset Enable
	; Bit 5, 2..0 - WDP3..0: Watchdog Timer Prescaler 3, 2, 1 and 0
	;WDTON(1)	WDE WDIE    Mode 					Action on Time-out
	;	1 		0 	0 		Stopped 				None
	;	1 		0 	1 		Interrupt Mode 			Interrupt
	;	1 		1 	0 		System Reset Mode 		Reset
	;	1 		1 	1		Interrupt and System 	Interrupt, then go to
	;						Reset Mode				System Reset Mode
	;	0 		x 	x 		System Reset Mode 		Reset
	;Note:      1.  WDTON Fuse set to “0“ means programmed and “1” means unprogrammed.
	;WDP3     WDP2     WDP1     WDP0	Number of WDT Oscillator 	Typical Time-out at
	;									Cycles						VCC = 5.0V
	;	0 		0 		0 		0 		2K (2048) cycles 			16 ms
	;	0 		0 		0 		1 		4K (4096) cycles 			32 ms
	;	0 		0 		1 		0 		8K (8192) cycles 			64 ms
	;	0 		0 		1 		1 		16K (16384) cycles 			0.125 s
	;	0 		1 		0 		0 		32K (32768) cycles 			0.25 s
	;	0 		1 		0	 	1 		64K (65536) cycles 			0.5 s
	;	0 		1 		1 		0 		128K (131072) cycles 		1.0 s
	;	0 		1 		1 		1 		256K (262144) cycles 		2.0 s
	;	1 		0 		0 		0 		512K (524288) cycles 		4.0 s
	;	1 		0 		0 		1 		1024K (1048576) cycles 		8.0 s
;--------------------------- Выключение сторожевого таймера
WDT_off:
; Turn off global interrupt
cli
; Reset Watchdog Timer
wdr
; Clear WDRF in MCUSR
in    r16, MCUSR
andi  r16, (0xff - (1<<WDRF))
out   MCUSR, r16
; Write logical one to WDCE and WDE
; Keep old prescaler setting to prevent unintentional time-out
in    r16, WDTCR
ori   r16, (1<<WDCE) | (1<<WDE)
out   WDTCR, r16
; Turn off WDT
ldi   r16, (0<<WDE)
out   WDTCR, r16

;************************************

rjmp main
