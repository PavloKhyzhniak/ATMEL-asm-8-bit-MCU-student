;���. 270
;����� 1-�� ����� � ��� ��������� � ��������� Y
;����� 1-�� ����� � FLASH-������ ��������� � ��������� Z
.equ PAGESIZEB = PAGESIZE*2
.def spmcrval=r19 ;4 -� ������� ��� ��������. ��������	
.def looplo=r24;
.def loophi=r25;
.org SMALLBOOTSTART
Write_page:
;������� ��������
	ldi spmcrval,(1<<PGERS)|(1<<SPMEN)
	call Do_spm

;��������� ��������� ������� RWW
 	ldi spmcrval,(1<<RWWSRE)|(1<<SPMEN)
	call Do_spm

;�������� ������ �� ��� � ����� ��������
	ldi looplo,low(PAGESIZEB)	;���������������� ������� ������
	ldi loophi,high(PAGESIZEB)
Wrloop:
	ld r0,Y+
	ld r1,Y+
	ldi spmcrval,(1<<SPMEN)
	call Do_spm
	adiw ZH:ZL,2
	sbiw loophi:looplo,2
	brne Wrloop

;�������� ��������
	subi ZL,low(PAGESIZEB)	;������������ ���������
	sbci ZH,high(PAGESIZEB)
	ldi spmcrval,(1<<PGWRT)|(1<<SPMEN)
	call Do_spm

;��������� ��������� ������� RWW
	ldi spmcrval,(1<<RWWSRE)|(1<<SPMEN)
	call Do_spm

;����������������� ���������� ������
	ldi looplo,low(PAGESIZEB)
	ldi loophi,high(PAGESIZEB)
	subi YL,low(PAGESIZEB)
	sbci YH,high(PAGESIZEB)
Rdloop:
	lpm r0,Z+
	ld r1,Y+
	cpse r0,r1
	jmp Error
	sbiw loophi:looplo,1
	brne Rdloop

;������� � ������ ���������� ���������
	ldi	temp2,0
Return:
	lds temp,SPMCR
	sbrs temp,RWWSB

	ret

Error:	;????????????????????????
;��������� ��������� ������� RWW
	ldi spmcrval,(1<<RWWSRE)|(1<<SPMEN)
	call Do_spm
	ldi	temp2,$FF
	rjmp Return

Do_spm:
Wait_spm:
	in temp,SPMCR
	sbrc temp,SPMEN
	rjmp Wait_spm
;��������� ����������, ��������� ������� �������
	in temp2,SREG
	cli
;��������� � ���������� ������ � EEPROM
Wait_ee:
	sbic EECR,EEWE
	rjmp Wait_ee

	out SPMCR,spmcrval
	spm
;������������ ������� SREG (��� ���������� ���������� ����������)
	out SREG,temp2
	ret








