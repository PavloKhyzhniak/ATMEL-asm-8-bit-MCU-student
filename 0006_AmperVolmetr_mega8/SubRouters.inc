;************************************
;** ���� ��������������� �����������
;**
;**
;************************************

		;-------------------------- ������������

;������ ������������ ���������� ������ �� �����
; ���� �� �����������, ����������, �������� ���� ��� ������ �������� ����������(�� ���������)
; ��� ������������� ���������� ������� ����� �� ������ ���� �������� � ����������� ���������


NumberA:
; ������������ ����������� ���� ��� �����������
; ������� ���� ����������� �� 
;	tmpL2 ��� ������� ����
; 	tmpH2 ��� ������� ����
		
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
; ������������ ����������� ���� ��� �����������
; ������� ���� ����������� �� 
;	tmpL2 ��� ������� ����
; 	tmpH2 ��� ������� ����
		
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
; ������������ ���������� ����� �� �������
digit:	ldi r20, -1	; ����� �������������� -1
digit_1:inc r20		; ��������������, �.�. ������� ��� �������� � ����!!!
		sub r18, r16	; ��������� �������� �����
		sbc r19, r17	; ��������� � ��������� �������� �����
		brsh digit_1	; � ������ ���� �� ������������� ���������
		add r18, r16	; ����� ��������������� ��������� ��������
		adc r19, r17	; � ����� �������� ��� ��� ������ �����������
						; �.�. ������� 10 ������� �������!!!

		ldi zh, high(TableIndicator*2)
		ldi zl, low(TableIndicator*2)
		add zl, r20
		clr r20
		adc zh, r20
		lpm	; ��������� � R0
		ret

;*******************************
