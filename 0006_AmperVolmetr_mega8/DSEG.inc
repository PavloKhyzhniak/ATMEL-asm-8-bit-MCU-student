;************************************
;** ���� �������������� ���������� � ��� � ��������
;**
;**
;************************************
;************************************
; �������������� ����������� �������� ��� �������

; ������ ��������������� ����������
.set Seg_A = PB5
.set Seg_B = PB4
.set Seg_C = PB3
.set Seg_D = PD6
.set Seg_E = PB1
.set Seg_F = PB2
.set Seg_G = PB0
.set Seg_DP = PD7
; ���������� ����������������
.equ v1 = PC4
.equ v2 = PC5
.equ v3 = PD0
.equ a1 = PD1
.equ a2 = PD2
.equ a3 = PD3
.equ a4 = PD4

.equ ErrorAmper = PD5

;************************************
;����� �������� EEPROM
.eseg
.org	0x08


;************************************
;����� �������� ���
.dseg
.org 0x060


Atek:	.BYTE 1
Aerror:	.BYTE 1
VoltBig:	.BYTE 2
VoltSmall:	.BYTE 2
VoltBigS:	.BYTE 2
VoltSmallS:	.BYTE 2

Amper:	.BYTE 2
Vind: .BYTE 2
Aind: .BYTE 2


.equ cpiVoltBig = 40;/8
.equ cpiVoltSmall = 32;/16
.equ cpiAmper = 32;/4
.equ VoltSmallMax = 1000;2816/100 = 28 �����, 10 ��� ������ �� ��������

cntVoltBig:	.BYTE 1
cntVoltSmall:	.BYTE 1
cntAmper:	.BYTE 1


TableDataLed:
sV1:	.BYTE 1
sV2:	.BYTE 1
sV3:	.BYTE 1
sA1:	.BYTE 1
sA2:	.BYTE 1
sA3:	.BYTE 1
sA4:	.BYTE 1


;************************************


;************************************
; ����� �������� ������ ��������
.cseg		


;************************************
; ��������� ��������������� ����������
.equ g = 1		; Bit 0 - a, ��� ������� 1
.equ e = 2		; Bit 1 - b, ��� ������� 2
.equ f = 4		; Bit 2 - c, ��� ������� 4
.equ c = 8		; Bit 3 - d, ��� ������� 8
.equ b = 16		; Bit 4 - e, ��� ������� 16
.equ a = 32		; Bit 5 - f, ��� ������� 32
.equ d = 64		; Bit 6 - g, ��� ������� 64
.equ DP = 128	; Bit 7 - DP, ��� ������� 128

.equ Num0 = a+b+c+d+e+f	; ����� 0
.equ Num1 = b+c			; ����� 1
.equ Num2 = a+b+d+e+g	; ����� 2
.equ Num3 = a+b+c+d+g	; ����� 3
.equ Num4 = b+c+f+g		; ����� 4
.equ Num5 = a+c+d+f+g	; ����� 5
.equ Num6 = a+c+d+e+f+g	; ����� 6
.equ Num7 = a+b+c		; ����� 7
.equ Num8 = a+b+c+d+e+f+g	; ����� 8
.equ Num9 = a+b+c+d+f+g; ����� 9

;������� �������� ��� ������ ���� �� �������������� ���������
; 0 1 2 3 4 5 6 7 8 9
TableIndicator:
.db Num0,Num1	;.db	0b00111111, 0b00000110
.db Num2,Num3	;.db	0b01011011, 0b01001111
.db Num4,Num5	;.db	0b01100110, 0b01101101
.db Num6,Num7	;.db	0b01111101, 0b00000111
.db Num8,Num9	;.db	0b01111111, 0b01101111


.equ AddrV11 = 1<<PC4
.equ AddrV12 = 0
.equ AddrV21 = 1<<PC5
.equ AddrV22 = 0
.equ AddrV31 = 0
.equ AddrV32 = 1<<PD0
.equ AddrA11 = 0
.equ AddrA12 = 1<<PD1
.equ AddrA21 = 0
.equ AddrA22 = 1<<PD2
.equ AddrA31 = 0
.equ AddrA32 = 1<<PD3
.equ AddrA41 = 0
.equ AddrA42 = 1<<PD4

; ������� ������� ��������� �������� LED
TableLed:
.db AddrV11,AddrV12
.db AddrV21,AddrV22
.db AddrV31,AddrV32
.db AddrA11,AddrA12
.db AddrA21,AddrA22
.db AddrA31,AddrA32
.db AddrA41,AddrA42


; ������� �������� ��� ��������� ���������� ��� ��� ������ ��������������� ���� ��������
TableADC:
.db 0x00,0x01,0x02,0x03






