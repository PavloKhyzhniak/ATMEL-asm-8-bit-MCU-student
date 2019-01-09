;************************************
;**
;**
;**
;************************************

TIM2_COMPA:

		reti

TIM2_COMPB:

		reti

TIM2_OVF:
	push xh
	push xl
	push r16
	in r16,SREG
	push r16


;**************************
;**************************

RTC_ReadTime
rcall OutLCD_TIME

;**************************
;**************************

; организуем программный подсчет прерываний		
		lds xl,(cntTimer)
		lds xh,(cntTimer+1)
		
		sbiw x,1
; проверим накопили ли мы прерываний на 1 секунду		
		mov r16,xl
		or r16,xh
		brne ExitTimer2
; прошла ровно секунда!!!
		ldi xl,low(50)
		ldi xh,high(50)

; проверим подсветку ЖКИ
		lds R16,TimeLED
		inc R16
		sts TimeLed,R16
		cpi R16,20; горит 20 сек
		brsh OffLED
		rjmp ExitTimer2

OffLED:
;тушим подсветку ЖКИ
		cbi LED_LCD_DDR,LED
		cbi LED_LCD_PORT,LED
		clr R16
		sts TimeLED,R16

ExitTimer2:		
; завершение работы подпрограммы обработки прерывания от Таймера 2
		sts (cntTimer),xl
		sts (cntTimer+1),xh

	pop r16
	out SREG,r16
	pop r16
	pop xl
	pop xh
	
		reti
