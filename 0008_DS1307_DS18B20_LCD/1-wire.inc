;-----------������������ ��� ������� �����������
;����������� �������� ��� 16���
;������� ����� (����� ��������) *1,88+0,44=(����� ��������)
; ���������� ����������������
; �� ���� ��������� ��� ����� ��������= 1 2 � 10 :)
;Delay:
;		ldi temp3, 9
;del1:
;		dec temp3
;		brne del1
;	
;		dec cnt3
;		brne Delay
;		ret


; ����� ������ ����� ����� �����������
; � ��������� �� � ���� ����
nibleRun:
		lds temp, Temperature
		swap temp
		andi temp,0x0F
		mov temp1,temp
		lds temp, Temperature+1
		swap temp
		andi temp, 0xF0
		add temp, temp1
		sts LCD_Temperature,temp
		ret


.MACRO DS18B20

		rcall setTemperature
		rcall readTemperature
		
		rcall nibleRun

.ENDMACRO


OutLCD_Temperature:

push yl
push yh
push r16
push r17

	ldi yl,low(TmpVar_LCD)
	ldi yh,high(TmpVar_LCD)
	
	lds r16, LCD_Temperature
	clr r17
	rcall Number99

	ldi r16,$20
	st y+,r16
	ldi r16,'C'
	st y+,r16
	ldi r16,$20
	st y+,r16
	st y+,r16
	st y+,r16
	st y+,r16


	ldi		zl,Low(TmpVar_LCD)		;������ ����� ��������� ������ � ������
	ldi		zh,High(TmpVar_LCD)	
	ldi		R16,$80				;������ ��� ������� �� 2 ������
	rcall	String_To_LCD		;������� �� 2 ������ ��������� ������

pop r17
pop r16
pop yh
pop yl

	ret


;-------------------------- ������������ 1-Wire ����������
.dseg

LCD_Temperature:	.BYTE 1
Temperature:	.BYTE 2


.cseg

.equ tempU = 0
.set tempDQ = PC0
.set DQ_PORT = PORTC
.set DQ_PIN = PINC
.set DQ_DDR = DDRC

;-------------------------- ������� ������ ���������� DS18B20

;---------- ������� �������� ������

; 0F0H	����� ���
; 033H	������ ���
; 055H	���������� ���
; 0CCH	������� ���
; 0ECH	����� ����������

;--------- ������ ���
/*
readROM:
		rcall resetLAN
		cpi error, 0
		brne rdROM2
		ldi temp, $33
		rcall writeByteLAN
		ldi zh, high(bufLAN*2)
		ldi zl, low(bufLAN*2)
		ldi temp1, 8
rdROM:	
		rcall readByteLAN
		st z, temp
		ldi temp,1
		add zl,temp
		clr temp
		adc zh, temp
		dec temp1
		brne rdROM
		ret
rdROM2:
		rjmp readROM
*/
;---------- ������� ������������� ������

; 04EH	������ ���������� ������
; 0BEH	������ ���������� ������
; 0B4H	������ ������ �������
; 0B8H	������ �� EEPROM � ���������� ������
; 048H	����������� ���������� ������ � EEPROM
; 044H	������ �������� ��������������

;----------- ������ �����������
readTemperature:
		rcall resetLAN
		cpi error,0
		brne rdT6
		ldi temp, $CC; ������� ���
		rcall writeByteLAN
		ldi temp, $BE; ������ ���������� ������
		rcall writeByteLAN

		rcall readByteLAN
		sts Temperature,temp

		rcall readByteLAN
		sts Temperature+1,temp
		ret
rdT6:
ret
		cpi error, 1
		brne readTemperature
		rjmp readTemperature;ret

;----------- ������ �������� ��������������
setTemperature:
; ������ ������ �������
		rcall resetLAN
		cpi error, 0
		brne setT4
		ldi temp, $CC; ������� ���
		rcall writeByteLAN
		ldi temp, $B4; ������ ������ �������
		rcall writeByteLAN
		rcall readBitLAN
		brcc setT1
		ldi cnt1,1
		rjmp setT2
setT1:
		ldi cnt1,0
setT2:
; ������ �������� ��������������
		rcall resetLAN
		cpi error,0
		brne setT2
		ldi temp, $CC; ������� ���
		rcall writeByteLAN
		ldi temp, $44;������ �������� ��������������
		rcall writeByteLAN
		cpi cnt1,0
		brne setT3
; �������� � ������ ������� �� �����
		sbi DQ_PORT, tempDQ
		nop
		cbi DQ_PORT, tempU
		; �������� � 765 �����.
	ldi		R18,255				;�������� � 255���
	rcall	delay_mcs
	ldi		R18,255				;�������� � 255���
	rcall	delay_mcs
	ldi		R18,255				;�������� � 255���
	rcall	delay_mcs

		sbi DQ_PORT, tempU
		ret
; �������� � ������ �������� �������
setT3:
		rcall readBitLAN
		brcc setT3
		ret		
setT4:
ret
		cpi error, 1
		brne setTemperature
		rjmp setTemperature;ret


;-------------------------- ������������ 1-Wire ���������� (����� �����)

; ������������ ������ ����
readBitLAN:
;		cli
		rcall readSlot
;		sei
		ret

; ������������ ������ �����
readByteLAN:
		cli
		ldi temp1, 8
rd8LAN1:
		rcall readSlot
		ror temp
		dec temp1
		brne rd8LAN1
		sei
		ret

; ������������ ������ ����
writeBitLAN:
;		cli
		brcs wr1LAN1
		rcall writeLow
		sei 
		ret
wr1LAN1:
		rcall writeHigh
;		sei
		ret

; ������������ ������ �����
writeByteLAN:
		cli
		ldi temp1,8
wr8LAN1:
		ror temp
		brcs wr8LAN2
		rcall writeLow
		rjmp wr8LAN3
wr8LAN2:
		rcall writeHigh
wr8LAN3:
		dec temp1
		brne wr8LAN1
		sei
		ret

;-------------------------- ������������ 1-Wire ���������� (������ �������)

; ������������ ������ 0 � ����
; ������ �������� 0 �� �����
writeLow:
		sbi DQ_DDR, tempDQ
		cbi DQ_PORT, tempDQ

		; �������� � 90 �����.
		ldi		R18,90				;�������� � 90���
		rcall	delay_mcs
	
		sbi DQ_PORT, tempDQ

		; �������� � 5 �����.
		ldi		R18,5 			;�������� � 5���
		rcall	delay_mcs

		ret

; ������������ ������ 1 � ����
; ������ �������� 1 �� �����
writeHigh:
		sbi DQ_DDR, tempDQ
		cbi DQ_PORT, tempDQ

		; �������� � 5 �����.
		ldi		R18,5				;�������� � 5���
		rcall	delay_mcs

		sbi DQ_PORT, tempDQ
		; �������� � 90 �����.
		ldi		R18,90				;�������� � 90���
		rcall	delay_mcs
	
		ret

; ������������ ������ ����� �������
; ��������� ������ � �������� �������� �
readSlot:
		sbi DQ_DDR, tempDQ
		cbi DQ_PORT, tempDQ

		; �������� � 5 �����.
		ldi		R18,5				;�������� � 5���
		rcall	delay_mcs

		cbi DQ_DDR, tempDQ
		
		; �������� � 20 �����.
		ldi		R18,20				;�������� � 20���
		rcall	delay_mcs

		clc
		sbic DQ_PIN, tempDQ
		sec

		; �������� � 35 �����.
		ldi		R18,35				;�������� � 35���
		rcall	delay_mcs
		ret

; ������������ ������ �����
; ����� - 0 � ������� >500 ����� ��������
; ����� ����� 60 ����� ����� 0 ������� 
; � ����� >500 ����� ����� ����� - 1
resetLAN:
		; ����� ����� ��������
		sbi DQ_DDR, tempDQ
		cbi DQ_PORT, tempDQ
		; �������� � 700 �����.
		ldi		R18,250				;�������� � 250���
		rcall	delay_mcs
		ldi		R18,250				;�������� � 250���
		rcall	delay_mcs
		ldi		R18,200				;�������� � 200���
		rcall	delay_mcs

		; ��������� �����
		sbi DQ_PORT, tempDQ
		cbi DQ_DDR, tempDQ
		; �������� � 60 �����.
		ldi		R18,60				;�������� � 60���
		rcall	delay_mcs

		; ����� ����� �� �������
		;sbic PORTB, tempDQ2
		sbic DQ_PIN, tempDQ
		rjmp Er1
		; �������� � 750 �����.
		ldi		R18,250				;�������� � 250���
		rcall	delay_mcs
		ldi		R18,250				;�������� � 250���
		rcall	delay_mcs
		ldi		R18,250				;�������� � 250���
		rcall	delay_mcs

		; ��� ������ ������ �������� ����� �����
		; � ��������� �����
		sbis DQ_PORT, tempDQ
		rjmp Er2
		clr error	; ���������� ������
		ret
Er2:	; �� �����
		ldi error, 2
		ret
Er1:	; ��� �������
		ldi error, 1
		ret
