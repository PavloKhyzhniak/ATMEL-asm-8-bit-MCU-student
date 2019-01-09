;************************************
;** ‘айл работы с энергонезависимой пам€тью
;**
;**
;************************************

;************************************
; запись данных в EEPROM
EEPROM_write:
; Wait for completion of previous write
		sbic EECR,EEPE; ждать завершени€ предыдущей записи
		rjmp EEPROM_write    
; Set up address (r18:r17) in address register
		out EEARH, r18; занести адресс в регистр адресса
		out EEARL, r17
; Write data (r16) to Data Register
		out EEDR,r16; записать данные в регистр данных
; Write logical one to EEMPE
		sbi EECR,EEMPE; установить флаг EEMWE
; Start eeprom write by setting EEPE
		sbi EECR,EEPE;начать запись в EEPROM
		ret
;************************************
;чтение данных из EEPROM
EEPROM_read:
; Wait for completion of previous write
		sbic EECR,EEPE;ждать завершени€ предыдущей записи
		rjmp EEPROM_read
; Set up address (r18:r17) in address register
		out EEARH, r18
		out EEARL, r17
; Start eeprom read by writing EERE
		sbi EECR,EERE; начать чтение
; Read data from Data Register
		in  r16,EEDR;сохранить данные
		ret		
;************************************

EE_RDY:

		reti
