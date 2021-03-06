;************************************
;**
;**
;**
;************************************

TIM0_COMPA:

		reti

TIM0_COMPB:

		reti

TIM0_OVF:
	push zl
	push zh
	push R16
	push R17
	in r16,SREG
	push r16
		
; ����������������� �������
	ldi r16, -5;20;
	out TCNT0,r16
;�������� ��� �� ���������� �� ��������, ������ �� �����
; � ��� 7 �������� �� ������ �� 2 ������
	mov r16,cntLed
	cpi r16,7
	brne normCntLed

clearCntLed:
	clr cntLed

normCntLed:
;************
; �������� ������ �� LED
	ldi zl,low(TableDataLed)
	ldi zh,high(TableDataLed)
	clr r16
	add zl,cntLed
	adc zh,r16

; ������ �� ������� �����
	ld r17,Z
	in r16,PORTB
	andi r16,~(0x3F)
	andi r17,(0x3F)
	or r16,r17
	out PORTB,r16
; ������ �� ������� �����
	ld r17,Z
	in r16,PORTD
	andi r16,~(0xd0)
	andi r17,(0xd0)
	or r16,r17
	out PORTD,r16

;************
; �������� ������ �������
	ldi zl,low(2*TableLed)
	ldi zh,high(2*TableLed)
	clc
	mov r16,cntLed
	lsl r16
	add zl,r16
	clr r16
	adc zh,r16

; ������ �� ������� �������
	in r16,PORTC
	andi r16,~(0x30)
	lpm r17,Z+
	andi r17,(0x30)
	or r16,r17
	out PORTC,r16

; ������ �� ������� �������
	in r16,PORTD
	andi r16,~(0x1F)
	lpm r17,Z
	andi r17,(0x1F)
	or r16,r17
	out PORTD,r16
; �������������� ������� ����������� �� ��������� ������
	inc cntLed

extTIM0_OVF:


; ����� ������ ���
	ldi zl,low(2*TableADC)
	ldi zh,high(2*TableADC)
	add zl,cntADC
	clr r18
	adc zh,r18
	lpm r16,Z

	in r18,ADMUX
	andi r18,0xF0
	or r18,r16
	out ADMUX,r18	

; ������ ���, ���� �������� � ��������� ��������������
	in r16,ADCSRA
	ori r16,1<<ADSC
	out ADCSRA,r16
	
	
	pop r19
	pop r18
	pop r17
	pop r16
	out SREG,r16
	pop r16

	reti
;*******************************
;*******************************
