;==============================================================================
;������ �������� ���������
;==============================================================================
initLCD_KEY:
	
	in		R16,DATA_DDR
	ori		R16,(1<<CLK|1<<DATA)	
	out		DATA_DDR,R16				; �������� ����� ��� �����			
	
	in		R16,LCD_DDR
	ori		R16,(1<<A0|1<<EN)	
	out		LCD_DDR,R16				; �������� ����� ��� �����			
	
	in		R16,KEY_PORT
	ori		R16,(1<<B_RET1)
	out		KEY_PORT,R16				;������� ������������� �������� � ����� �������� ������

	rcall	initlcd				;�������������� LCD
	cbi		LCD_PORT,A0			;��������� �������� ������	
	ldi		R16,0b00000001			;������� �������
	rcall	load_555ir8
	ldi		R18,2				;�������� � 2��
	rcall	delay_ms

	ldi		zl,Low(RusAbout*2)		;������ ����� ��������� ������ � ������
	ldi		zh,High(RusAbout*2)	
	ldi		R16,$80					;������ ��� ������� �� 1 ������
	rcall	String_To_LCD_cseg		;������� �� 1 ������ ��������� ������

	ldi		zl,Low(RusAbout*2+8)	;������ ����� ��������� ������ � ������
	ldi		zh,High(RusAbout*2+8)	
	ldi		R16,$C0					;������ ��� ������� �� 2 ������
	rcall	String_To_LCD_cseg		;������� �� 2 ������ ��������� ������

	ret
	
	;���������� ����� ������������� ����������

;***********************

;===========================================================
;��������� ������������ ����������
;�����: a - ��� ������� ������ (a=0 - �� ���� ������� ������)
;===========================================================
Scan_Keyb:
	Push	R19				;��������� ��������������� �������� � ����
	Push	R18
	Push	R17

	ldi		R19,8				;������������� ��� ��������� 8 ������
	clr		R17				;������� ��������� ������� ��� �������� ���� ������� ������
	ldi		R16,$FE				;������������� ��������� ������� ��� ������������ ������
sc0:							;������� �����
	Push	R16				;��������� ������� ��������� �������� � ����
							;���������� ��� ������� ���� ������ � ������ �� �������
	ldi		R18,8				;������������� 8 ��� ��� ������
sc1:
	cbi		DATA_PORT,CLK			;CLK=0
	rol		R16				;�������� ���������� a �����
	brcs	sc3				;���� ����� ��� ����� 1 �� ������� �� sc3
	cbi		DATA_PORT,DATA			;DATA=0
	rjmp	sc4				;
sc3:
	sbi		DATA_PORT,DATA			;DATA=1
sc4:
	sbi		DATA_PORT,CLK			;CLK=1

	dec		R18				;��������� ���
	brne	sc1				;���� �� ��� ������, �� ������� �� sc1
	cbi		DATA_PORT,CLK			;CLK=0

	Pop		R16				;�������������� ���������� � �� �����
	sbic	KEY_PIN,B_RET1			;��������� ���� �� ������ �����-������ ������
	rjmp	sc5				;���� ��� �� ������� �� �����
	Push	R16				;��������� � � ����
	com		R16				;a = a - $FF
	add		R17,R16				;���������� ��� ������ � ������� ������ ������� ������
	Pop		R16				;�������������� � �� �����
sc5:
	sec						;������������� ��� �������� � 1
	rol		R16				;c������ ���� � ����� � ������ ���� ��������
	dec		R19				;��������� �������������� ��� 8 ������?
	brne	sc0				;���� ���, �� ������� �� �����
	mov		R16,R17				;��������� ��� ������� ������ � �������� �������
							;�� ���������� ��������	
	Pop		R17			;�������������� ��� ��������������� �������� �� �����
	Pop		R18
	Pop		R19
	ret

;===============================================
;��������� ������� ������ �� ������� �� DSEG
;���� - Z ������ �� ������ ������
;���� a - ������ ������ ($80 - ������ ������)
;			($C0 - ������ ������)
;================================================
String_to_LCD:
	Push		zl
	Push		zh
	Push		R18
	Push		R17
	Push		R16

	cbi		LCD_PORT,A0			;��������� �������� ������	
	rcall	load_555ir8			;��������� ����� ������ �� LCD
	ldi		R18,39
	rcall	delay_mcs
	sbi		LCD_PORT,A0			;��������� �������� ������
							;������� �� ������� ����
	ldi		R17,8
Str1:
	ld		R16,Z+
	rcall	load_555ir8
	ldi		R18,39
	rcall	delay_mcs
	dec		R17
	brne	Str1				;���� �� ��� ������, �� ������� �� Str1

	Pop		R16
	Pop		R17
	Pop		R18
	Pop		zh
	Pop		zl
	ret

;===============================================
;��������� ������� ������ �� ������� �� CSEG
;���� - Z ������ �� ������ ������
;���� a - ������ ������ ($80 - ������ ������)
;			($C0 - ������ ������)
;================================================
String_to_LCD_cseg:
	Push	zl
	Push	zh
	Push	R18
	Push	R17
	Push	R16

	cbi		LCD_PORT,A0			;��������� �������� ������	
	rcall	load_555ir8			;��������� ����� ������ �� LCD
	ldi		R18,39
	rcall	delay_mcs
	sbi		LCD_PORT,A0			;��������� �������� ������
							;������� �� ������� ����
	ldi		R17,8
Str11:
	lpm		R16,Z+
	rcall	load_555ir8
	ldi		R18,39
	rcall	delay_mcs
	dec		R17
	brne	Str11				;���� �� ��� ������, �� ������� �� Str1

	Pop		R16
	Pop		R17
	Pop		R18
	Pop		zh
	Pop		zl
	ret
;*****************

;lds r16,cntDriver
;lds r17,cntDriver+1
;ldi r18,$CF
;rcall OutLCD_VAR

OutLCD_VAR:
push yl
push yh
push r16
push r17

	ldi yl,low(TmpVar_LCD)
	ldi yh,high(TmpVar_LCD)
	
	rcall NumberTC

	ldi		zl,Low(TmpVar_LCD)		;������ ����� ��������� ������ � ������
	ldi		zh,High(TmpVar_LCD)	
;	ldi		R16,$CF				;������ ��� ������� �� 2 ������
	mov r16,r18
	rcall	String_To_LCD		;������� �� 2 ������ ��������� ������

pop r17
pop r16
pop yh
pop yl

ret

;***********

;ldi r19,10
;rcall OutLCD_Metka

OutLCD_Metka:
push zl
push zh
	ldi zl,Low(KeyMetca*2)
	ldi zh,High(KeyMetca*2)
	ldi r18,4
; ������� ��������
	mul r18,r19
	
	add zl,r0
	adc zh,r1
	
	ldi		R16,$04;				;������ ��� ������� �� 1 ������
	rcall	String_To_LCD_cseg		;������� �� 1 ������ ��������� ������

pop zh
pop zl

ret

;==============================================================================
;��������� ������������� ���
;==============================================================================
initlcd:
	cbi		LCD_PORT,A0			;��������� A0=0
	cbi		LCD_PORT,EN				;��������� E=0
	ldi		R18,20				;����� 20��
	rcall	delay_ms			;
	
	ldi		R16,0b00110000			;
	rcall 	load_555ir8			;
	ldi		R18,5				;����� 5 ms
	rcall	delay_ms			;

	ldi		R16,0b00110000			;
	rcall 	load_555ir8			;
	ldi		R18,100				;����� 100 mcs
	rcall	delay_mcs
	
	ldi		R16,0b00110000			;
	rcall 	load_555ir8			;
	ldi		R18,200;40				;����� 40 mcs
	rcall	delay_mcs

	ldi		R16,0b00111000			;
	rcall 	load_555ir8			;
	ldi		R18,200;40				;����� 40 mcs
	rcall 	delay_mcs			;

	ldi		R16,0b00001000			;
	rcall 	load_555ir8			;
	ldi		R18,200;40				;����� 40 mcs
	rcall	delay_mcs

	ldi		R16,0b00000001			;
	rcall 	load_555ir8			;
	ldi		R18,2				;����� 2 ms
	rcall	delay_ms

	ldi		R16,0b00000110			;
	rcall	load_555ir8			;
	ldi		R18,200;40				;����� 40 mcs
	rcall	delay_mcs
;--------------------------------------------------------------------------------
	ldi		R16,0b00001100			;
	rcall 	load_555ir8			;
	ldi		R18,200;40				;����� 40 mcs
	rcall	delay_mcs

	ldi		R16,0b10000000		;��������� ������ ��� ������ ������ (1���.1���.)
	rcall	load_555ir8			;
	ldi		R18,40				;����� 40 mcs
	rcall	delay_mcs

	ret

;==============================================================================
;��������� ������� ���� � 555��8(74HC164) ����: A - ���� ��� ������
;==============================================================================
load_555ir8:
	push	R16
	push	R18
	push 	R19
	ldi		R19,8			;������������� 8 ���� ��� ������
ir8_1:
	cbi		DATA_PORT,CLK		;CLK=0
nop
nop
	rol		R16				;�������� ���������� a �����
	brcs	ir8_3			;���� ����� ��� ����� 1 �� ������� �� ir8_3
	cbi		DATA_PORT,DATA		;DATA=0
	rjmp	ir8_4				;
ir8_3:
	sbi		DATA_PORT,DATA		;DATA=1
ir8_4:
nop
nop
nop
nop
	sbi		DATA_PORT,CLK		;CLK=1
	dec		R19				;��������� ���
	brne	ir8_1			;���� �� ��� ������, �� ������� �� ir8_1
	cbi		DATA_PORT,CLK		;CLK=0
nop
nop
	sbi		LCD_PORT,EN		;��������� E=1
nop
nop
nop
nop
nop
nop
	cbi		LCD_PORT,EN		;��������� E=0

	pop 	R19
	pop		R18
	pop		R16
	ret
;====================================================
