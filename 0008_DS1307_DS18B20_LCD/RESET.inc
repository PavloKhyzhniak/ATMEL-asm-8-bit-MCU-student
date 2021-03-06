;************************************
;**
;**
;**
;************************************

; �������������� �����
RESET:
; ��������� ����������
	cli
	
;************************************
;---����������� ���������� ���������
;		ldi r16, 0x48
;		sts OSCCAL, r16
;************************************

;************************************
;	������������� �����
		ldi r16,high(RAMEND)	
		out SPH,r16			 	; Set Stack Pointer to top of RAM
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
	
;-------------------------- ����������� ������������� ����������
;		lds		r16, SFIOR
;		ori		r16, 0<<PUD	
;		out		SFIOR, r16	
;-------------------------- ������������� ����� �����/������ B

		ldi		r16, 0x00	; ���������� ����� $00 � ������� temp
		out		DDRB, r16	; ���������� ��� ����� � DDR�
		ldi		r16, 0x00	; ���������� ����� $00 � ������� temp
		out		PORTB, r16	; ���������� ��  �� ����� � PORT�

;-------------------------- ������������� ����� �����/������ �

		ldi		r16, 0x00	; ���������� ����� $00 � ������� temp
		out		DDRC, r16	; ���������� ��� ����� � DDR�
		ldi		r16, 0x00	; ���������� ����� $00 � ������� temp
		out		PORTC, r16	; ���������� ��  �� ����� � PORT�

;-------------------------- ������������� ����� �����/������ D

		ldi		r16, 0x00	; ���������� ����� $00 � ������� temp
		out		DDRD, r16	; ���������� ��� ����� � DDR�
		ldi		r16, 0x00	; ���������� ����� $00 � ������� temp
		out		PORTD, r16	; ���������� ��  �� ����� � PORT�

;************************************
;-------------------------- ������������� ������� 2	

		ldi		r16, 1<<CS22|1<<CS21|1<<CS20
		sts		TCCR2B, r16	; �������� �� 1024

;		; ��� ���������� ����� � 8���, ������� ����� 0,0078 ���!!!
;		ldi		r16, T2PRE	; �������� ��������� �������� �������0
;		sts		TCNT2, r16	; ������ ���� 8-�� ������� ������� � ����� 255-32
;							; ���������� ���������� ��� ������������, �.�. ���������� ����� 255
;
		ldi		r16, 1<<TOIE2
		sts		TIMSK2, r16	; 
;************************************
; �������� ������������� ������������� ���� ����������� �� ������ ����
;������� ADCSRB
;Bit 	7	 6 	5 	4 	3	 	2 			1 			0
;		� ACME	 �	 �	 �	 ADTS2        ADTS1        ADTS0        
;ldi temp, (1<<ACME)
;sts ADCSRB, temp 
; ������ ������� ������� ���� �������������
; �� ���� ������� ��� �� ���� REF ����� �����������
;������� ADMUX
;Bit 	7		 6			 5 		4 		3 			2 			1 			0
;	REFS1       REFS0      ADLAR	 �	 MUX3        MUX2        MUX1        MUX0        
;ldi temp, 0xC7
;sts ADMUX, temp

	; ������� ACSR
	;7		6		5		4		3		2		1		0
	;ACD    ACBG    ACO 	ACI 	ACIE    ACIC    ACIS1   ACIS0  
	;7-ACD: ��������� (1) ���������(0) ����������
	;6-ACBG:(1) ����������� � ���������������� ����� ����������� ����������� ���, (0) �� ���������
	;5-ACO:��������� ���������
	;4-ACI:���� ����������
	;3-ACIE:���������� ���������� (1)
	;2-ACIC:(1) ����������� ����������� � ����� ������� ������� �1
	;1-ACIS1:������� ������������ ����������
	;0-ACIS0:
	;00 ����� ��������� 
	;10 � 1 �� 0
	;11 � 0 �� 1
	;ldi temp, (1<<ACD)		; ���������� �����������
;-------------------------- ������������� �����������
; ��������� ���������� ���������� ��� ������ ����������
		ldi r16, 0x80		; ���������� �����������
		out ACSR, r16

	
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
	;Note:      1.  WDTON Fuse set to �0� means programmed and �1� means unprogrammed.
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
;--------------------------- ���������� ����������� �������
WDT_off:
; Turn off global interrupt
cli
; Reset Watchdog Timer
wdr
; Clear WDRF in MCUSR
in    r16, MCUSR
andi  r16, (0xff & (0<<WDRF))
out   MCUSR, r16
; Write logical one to WDCE and WDE
; Keep old prescaler setting to prevent unintentional time-out
lds r16, WDTCSR
ori   r16, (1<<WDCE) | (1<<WDE)
sts WDTCSR, r16
; Turn off WDT
ldi   r16, (0<<WDE)
sts WDTCSR, r16
;************************************

;************************************
; ������� ���� ������� ���������

; ������� ��������� ������� ���� ����������������� �������



; ���������� ����������
;	sei ; Enable interrupts

// ������� � ������� ������� ���������
	rjmp main
;************************************
