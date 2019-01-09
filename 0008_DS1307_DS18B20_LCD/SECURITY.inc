;***********************************
;***********************************
;
;	SECURITY
;
;***********************************
;***********************************



;******** SECURITY *************
Security:
; выведем признак работы защиты!!!
	ldi r19,12
	rcall OutLCD_Metka

; организуем считывание 2х байт EEPROM с паролем
; пароль может быть 
;		на защите Password
;		на на тест драйве notPassword
;		на легальном режиме работы NormalWork
	ldi r17,low(SecurityH)	;SecurityH
	ldi r18,high(SecurityH)	;SecurityH
	rcall EEPROM_read
	mov r19,r16

	ldi r17,low(SecurityL)	;SecurityL
	ldi r18,high(SecurityL)	;SecurityL
	rcall EEPROM_read

; устроим проверку на каком этапе мы находимся
	ldi zl,low(Password)
	ldi zh,high(Password)
	cp r16,zl
	cpc r19,zh
	breq testSecurity; переходим на алгоритм ожидания разблокировки

; проверяем следующий этап работы
	ldi zl,low(NormalWork)
	ldi zh,high(NormalWork)
	cp r16,zl	
	cpc r19,zh
	brne PC+4
	; выведем метку что мы на этапе нормальной работы
	ldi r19,6
	rcall OutLCD_Metka
	ret

; проверка последнего этапа - тест драйв
	ldi zl,low(notPassword)
	ldi zh,high(notPassword)
	cp r16,zl
	cpc r19,zh
	brne endSecurity
	; выведем метку что мы на этапе тест драйва
	; иначе блокировка МК и стераем всю EEPROM
	ldi r19,13
	rcall OutLCD_Metka
	ret

testSecurity:
; этап проверка разблокировки МК

;sbis PIND,PD4
;rjmp testSecurity
sbic PIND,Sensor1
rjmp testSecurity
sbic PIND,Sensor2
rjmp Security
;sbis PIND,ChannelA
;rjmp testSecurity
;sbis PIND,ChannelB
;rjmp testSecurity
;sbis PINB,Sensor3
;rjmp testSecurity
;sbic PINB,Sensor4
;rjmp testSecurity

;при снятие блокировки сменим пароль в EEPROM
; теперь мы будем знать что разблокировали МК
	ldi r17,low(SecurityL)	;SecurityL
	ldi r18,high(SecurityL)	;SecurityL
	ldi r16,low(notPassword)
	rcall EEPROM_write

	ldi r17,low(SecurityH)	;SecurityH
	ldi r18,high(SecurityH)	;SecurityH
	ldi r16,high(notPassword)
	rcall EEPROM_write
	;установим метку тест драйва
	ldi r19,6
	rcall OutLCD_Metka
	ret

endSecurity:
; зависание МК и очистка EEPROM	
	ldi r19,14
	rcall OutLCD_Metka


	ldi r17,0xFF
	clr r16
	ldi zl,0x08
	ldi zh,0x00
clearEEPROM:
	adiw z,1
	mov r17,zl
	mov r18,zh
	rcall EEPROM_write

	adiw z,1
	mov r17,zl
	mov r18,zh
	rcall EEPROM_write

	dec r18
	brne clearEEPROM

	cli
endSecurityForever:
; бесконечный цикл
	rjmp endSecurityForever

;******** SECURITY *************















;*********** Test Drive *********
TestDrive:
; режим тест драйва - пробный период

; считаем наш пароль из EEPROM
	ldi r17,low(SecurityH)	;SecurityH
	ldi r18,high(SecurityH)	;SecurityH
	rcall EEPROM_read
	mov r19,r16

	ldi r17,low(SecurityL)	;SecurityL
	ldi r18,high(SecurityL)	;SecurityL
	rcall EEPROM_read

; сравним его с паролем тест драйва
	ldi zl,low(NormalWork)
	ldi zh,high(NormalWork)
	cp r16,zl
	cpc r19,zh
	breq exitTestDrive; при несовпадении зацикливание программы МК

	ldi zl,low(notPassword)
	ldi zh,high(notPassword)
	cp r16,zl
	cpc r19,zh
	brne exitTestDrive; при несовпадении зацикливание программы МК

; иначе уменьшаем счетчик запусков машины (от 999 до 0)
	ldi r17,low(SecurityCounter+1)	;SecurityCounter+1
	ldi r18,high(SecurityCounter+1)	;SecurityCounter+1
	rcall EEPROM_read
	mov r19,r16

	ldi r17,low(SecurityCounter)	;SecurityCounter
	ldi r18,high(SecurityCounter)	;SecurityCounter
	rcall EEPROM_read

	ldi r17,1
	ldi r18,0

	cp r16,r17
	cpc r19,r18
	breq endTestDrive

	sub r16,r17
	sbc r19,r18

; индикация тест драйва
ldi r18,13
sts key,r18

mov r17,r19
ldi r18,0x94
rcall OutLCD_VAR; r16,r17

; запись модернизированного счетчика
	ldi r17,low(SecurityCounter)	;SecurityH
	ldi r18,high(SecurityCounter)	;SecurityH
	rcall EEPROM_write

	ldi r17,low(SecurityCounter+1)	;SecurityH
	ldi r18,high(SecurityCounter+1)	;SecurityH
	mov r16,r19
	rcall EEPROM_write
	rjmp exitTestDrive
	
endTestDrive:
	ldi r19,14
	rcall OutLCD_Metka
		rjmp endTestDrive

exitTestDrive:

; продление тестового режима
	lds r18,cntKey
	dec r18
	cpi r18,0
	brne nextTest1

	lds xl,tmpVar
	lds xh,tmpVar+1

	cpi xl,5
	brne nextTest1

	ldi r17,low(SecurityL)	;SecurityL
	ldi r18,high(SecurityL)	;SecurityL
	ldi r16,low(notPassword)
	rcall EEPROM_write

	ldi r17,low(SecurityH)	;SecurityH
	ldi r18,high(SecurityH)	;SecurityH
	ldi r16,high(notPassword)
	rcall EEPROM_write

	ldi r16,low(999);(999)
	ldi r19,high(999);(999)
	
	rjmp saveNew

nextTest1:
	ldi r17,low(SecurityCounter+1)	;SecurityCounter+1
	ldi r18,high(SecurityCounter+1)	;SecurityCounter+1
	rcall EEPROM_read
	mov r19,r16

	ldi r17,low(SecurityCounter)	;SecurityCounter
	ldi r18,high(SecurityCounter)	;SecurityCounter
	rcall EEPROM_read

; проверка на актуальность тест драйва
	mov r18,r16
	or r18,r19
	brne nextTest2
	rjmp endTestDrive

saveNew:
; запись модернизированного счетчика
	ldi r17,low(SecurityCounter)	;SecurityH
	ldi r18,high(SecurityCounter)	;SecurityH
	rcall EEPROM_write

	ldi r17,low(SecurityCounter+1)	;SecurityH
	ldi r18,high(SecurityCounter+1)	;SecurityH
	mov r16,r19
	rcall EEPROM_write

; снятие тестового режима
	lds r18,cntKey
	dec r18
	cpi r18,1
	brne nextTest2

	lds xl,tmpVar
	lds xh,tmpVar+1

ldi r16,low(102)
ldi r17,high(102)
	cp xl,r16
	cpc xh,r17
	brne nextTest2

ldi r17,low(SecurityL)	;SecurityL
ldi r18,high(SecurityL)	;SecurityL
ldi r16,low(NormalWork)
rcall EEPROM_write

ldi r17,low(SecurityH)	;SecurityH
ldi r18,high(SecurityH)	;SecurityH
ldi r16,high(NormalWork)
rcall EEPROM_write

nextTest2:
ret

;*********** Test Drive *********








;***********************************
;***********************************
;
;	SECURITY
;
;***********************************
;***********************************
