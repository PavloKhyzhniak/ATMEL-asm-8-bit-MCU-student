;стр. 270
;Адрес 1-го байта в ОЗУ передаётся в указателе Y
;Адрес 1-го байта в FLASH-памяти передаётся в указателе Z
.equ PAGESIZEB = PAGESIZE*2
.def spmcrval=r19 ;4 -й регистр для промежут. операций	
.def looplo=r24;
.def loophi=r25;
.org SMALLBOOTSTART
Write_page:
;Стереть страницу
	ldi spmcrval,(1<<PGERS)|(1<<SPMEN)
	call Do_spm

;Разрешить адресацию области RWW
 	ldi spmcrval,(1<<RWWSRE)|(1<<SPMEN)
	call Do_spm

;Передать данные из ОЗУ в буфер страницы
	ldi looplo,low(PAGESIZEB)	;Инициализировали счётчик байтов
	ldi loophi,high(PAGESIZEB)
Wrloop:
	ld r0,Y+
	ld r1,Y+
	ldi spmcrval,(1<<SPMEN)
	call Do_spm
	adiw ZH:ZL,2
	sbiw loophi:looplo,2
	brne Wrloop

;Записать страницу
	subi ZL,low(PAGESIZEB)	;Восстановили указатель
	sbci ZH,high(PAGESIZEB)
	ldi spmcrval,(1<<PGWRT)|(1<<SPMEN)
	call Do_spm

;Разрешить адресацию области RWW
	ldi spmcrval,(1<<RWWSRE)|(1<<SPMEN)
	call Do_spm

;Проконтролировать записанные данные
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

;Возврат в секцию прикладной программы
	ldi	temp2,0
Return:
	lds temp,SPMCR
	sbrs temp,RWWSB

	ret

Error:	;????????????????????????
;Разрешить адресацию области RWW
	ldi spmcrval,(1<<RWWSRE)|(1<<SPMEN)
	call Do_spm
	ldi	temp2,$FF
	rjmp Return

Do_spm:
Wait_spm:
	in temp,SPMCR
	sbrc temp,SPMEN
	rjmp Wait_spm
;Запретить прерывания, сохранить регистр статуса
	in temp2,SREG
	cli
;Убедиться в отсутствии записи в EEPROM
Wait_ee:
	sbic EECR,EEWE
	rjmp Wait_ee

	out SPMCR,spmcrval
	spm
;Восстановить регистр SREG (для повторного разрешения прерываний)
	out SREG,temp2
	ret








