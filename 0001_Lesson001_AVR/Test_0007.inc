;**************************************
; ������ � �����������
; ��������� ����� ������ ����������
;
work_Button:
;-------------------------- ������������� ����� �����/������ B

		ldi		r16, 0xFF	; ���������� ����� $FF � ������� r16
		out		DDRB, r16	; ���������� ��� ����� � DDRB
		ldi		r16, 0xF0	; ���������� ����� $F0 � ������� r16
		out		PORTB, r16	; ���������� ��  �� ����� � PORTB
; ������������������� ���� B �� ����� 

;-------------------------- ������������� ����� �����/������ D

		ldi		r16, 0x00	; ���������� ����� $00 � ������� r16
		out		DDRD, r16	; ���������� ��� ����� � DDRD
		ldi		r16, 0x00	; ���������� ����� $00 � ������� r16
		out		PORTD, r16	; ���������� ��  �� ����� � PORTD
; ������������������� ���� D �� ���� 

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

; ����������� ������� ��� ������
	ldi tmp3,0x0F
	eor tmp2,tmp3

	out PORTB,tmp2

	rjmp searchBut

