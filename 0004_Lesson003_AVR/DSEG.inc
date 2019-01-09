;************************************
;** Файл резервирования переменных в ОЗУ и констант
;**
;**
;************************************

;************************************
;выбор сегмента EEPROM
.eseg
.org	0x10

eepromTu:	.BYTE 1

;************************************
.dseg
.org 0x060

Power:	.BYTE 1
Sinus:	.BYTE 1
SinFlags: .BYTE 1

dataRx_buf: .BYTE 32;данные на прием
dataTx_buf: .BYTE 32;данные на передачу

AD1H: .BYTE 1; сумма значений подсчитанного по каналу АЦП 1 (напряжение)
AD1L: .BYTE 1

AD3H: .BYTE 1; сумма значений подсчитанных по каналу АЦП 3 (ток)
AD3L: .BYTE 1

Ul:	.BYTE 1	; младший байт выведенного значения напряжения
Uh:	.BYTE 1	; старший байт выведенного значения напряжения
Il:	.BYTE 1	;
Ih:	.BYTE 1	;

U2: .BYTE 1; значение по напряжению старшая цифра
U1: .BYTE 1; значение по напряжению
U0: .BYTE 1; значение по напряжению младшая цифра

I1: .BYTE 1; значение по току старшая цифра
I0: .BYTE 1; значение по току младшая цифра

stolbik: .BYTE 1; значение для столбика тока

cntI: .BYTE 1; счетчик количество замеров по току
cntU: .BYTE 1; счетчик количества замеров по напряжению

T1l: .BYTE 1	;младший байт температуры считанной с датчика №1
T1h: .BYTE 1	;
T2l: .BYTE 1	;младший байт температуры считанной с датчика №2
T2h: .BYTE 1	;

TT2: .BYTE 1; значение температуры старшая цифра
TT1: .BYTE 1; значение температуры
TT0: .BYTE 1; значение температуры младшая цифра

bufLAN: .BYTE 8	; буфер принятых данных
;************************************


;************************************
.cseg
;запишем необходимые константы
.equ END_DATA = '+'
.equ START_DATA = ':'

//константы предустановок  МК
.equ T0PRE=-5	
.equ T2PRE=-150;-125;2	


.equ a = 1		; Bit 0 - a, вес разряда 1
.equ b = 2		; Bit 1 - b, вес разряда 2
.equ c = 4		; Bit 2 - c, вес разряда 4
.equ d = 8		; Bit 3 - d, вес разряда 8
.equ e = 16		; Bit 4 - e, вес разряда 16
.equ f = 32		; Bit 5 - f, вес разряда 32
.equ g = 64		; Bit 6 - g, вес разряда 64
.equ DP = 128	; Bit 7 - DP, вес разряда 128

.equ Num0 = a+b+c+d+e+f	; цифра 0
.equ Num1 = b+c			; цифра 1
.equ Num2 = a+b+d+e+g	; цифра 2
.equ Num3 = a+b+c+d+g	; цифра 3
.equ Num4 = b+c+f+g		; цифра 4
.equ Num5 = a+c+d+f+g	; цифра 5
.equ Num6 = a+c+d+e+f+g	; цифра 6
.equ Num7 = a+b+c		; цифра 7
.equ Num8 = a+b+c+d+e+f+g	; цифра 8
.equ Num9 = a+b+c+d+f+g; цифра 9

;************************************
; инициализируем необходимые названия для выводов

; канал Tx/Rx
.set PortX = PD2
.set KeyTRIAC = PD2


; кнопкa 
.set tempKey1 = PD3

;************************************
;Таблица констант для вывода цифр на семисегментный индикатор
; 0 1 2 3 4 5 6 7 8 9
TableIndicator:
.db Num0,Num1	;.db	0b00111111, 0b00000110
.db Num2,Num3	;.db	0b01011011, 0b01001111
.db Num4,Num5	;.db	0b01100110, 0b01101101
.db Num6,Num7	;.db	0b01111101, 0b00000111
.db Num8,Num9	;.db	0b01111111, 0b01101111

; Таблица углов открывания для тиристора(задержка)
; Предделитель Таймера0 выбран = 64
TableTRIAC:
.db 255-(0000/64), 255-(2587/64)
 ;полностью открыт ;90 % мощности
.db 255-(3362/64), 255-(3962/64) 
;80 % мощности	   ;70 % мощности
.db 255-(4494/64), 255-(4999/64) 
;60 % мощности	   ;50 % мощности
.db 255-(5503/64), 255-(6035/64)
;40 % мощности	   ;30 % мощности
.db 255-(6636/64), 255-(7410/64) 
;20 % мощности     ;10 % мощности


; Таблица синусов
; Содержит 90 отсчетов от 0 до 90 градусов
; Расчитан на 14 битный ЦАП
.equ Bits = 8
.equ Half = 1<<(Bits-1)
.equ HalfSinus = Half
.equ MaxNorm = 10000
TableSinus:
grad0:
.dw    0*Half/MaxNorm, 175*Half/MaxNorm, 349*Half/MaxNorm, 523*Half/MaxNorm, 698*Half/MaxNorm, 872*Half/MaxNorm
.dw 1045*Half/MaxNorm,1219*Half/MaxNorm,1392*Half/MaxNorm,1564*Half/MaxNorm,1736*Half/MaxNorm,1908*Half/MaxNorm
.dw 2079*Half/MaxNorm,2250*Half/MaxNorm,2419*Half/MaxNorm,2588*Half/MaxNorm,2756*Half/MaxNorm,2942*Half/MaxNorm
.dw 3090*Half/MaxNorm,3256*Half/MaxNorm,3420*Half/MaxNorm,3584*Half/MaxNorm,3746*Half/MaxNorm,3897*Half/MaxNorm
.dw 4067*Half/MaxNorm,4226*Half/MaxNorm,4384*Half/MaxNorm,4540*Half/MaxNorm,4695*Half/MaxNorm,4848*Half/MaxNorm
grad30:
.dw 5000*Half/MaxNorm,5150*Half/MaxNorm,5299*Half/MaxNorm,5446*Half/MaxNorm,5592*Half/MaxNorm,5736*Half/MaxNorm
.dw 5878*Half/MaxNorm,6018*Half/MaxNorm,6157*Half/MaxNorm,6293*Half/MaxNorm,6428*Half/MaxNorm,6561*Half/MaxNorm
.dw 6691*Half/MaxNorm,6820*Half/MaxNorm,6947*Half/MaxNorm,7071*Half/MaxNorm,7193*Half/MaxNorm,7314*Half/MaxNorm
.dw 7431*Half/MaxNorm,7547*Half/MaxNorm,7660*Half/MaxNorm,7771*Half/MaxNorm,7880*Half/MaxNorm,7986*Half/MaxNorm
.dw 8090*Half/MaxNorm,8192*Half/MaxNorm,8290*Half/MaxNorm,8387*Half/MaxNorm,8480*Half/MaxNorm,8572*Half/MaxNorm
grad60:
.dw 8660*Half/MaxNorm,8746*Half/MaxNorm,8829*Half/MaxNorm,8910*Half/MaxNorm,8988*Half/MaxNorm,9063*Half/MaxNorm
.dw 9135*Half/MaxNorm,9205*Half/MaxNorm,9272*Half/MaxNorm,9336*Half/MaxNorm,9397*Half/MaxNorm,9455*Half/MaxNorm
.dw 9511*Half/MaxNorm,9563*Half/MaxNorm,9613*Half/MaxNorm,9659*Half/MaxNorm,9703*Half/MaxNorm,9744*Half/MaxNorm
.dw 9781*Half/MaxNorm,9816*Half/MaxNorm,9848*Half/MaxNorm,9877*Half/MaxNorm,9903*Half/MaxNorm,9925*Half/MaxNorm
.dw 9945*Half/MaxNorm,9962*Half/MaxNorm,9976*Half/MaxNorm,9986*Half/MaxNorm,9994*Half/MaxNorm,9998*Half/MaxNorm
.dw 10000*Half/MaxNorm-1
;TableSinus:
;.dw    0, 175, 349, 523, 698, 872
;.dw 1045,1219,1392,1564,1736,1908
;.dw 2079,2250,2419,2588,2756,2942
;.dw 3090,3256,3420,3584,3746,3897
;.dw 4067,4226,4384,4540,4695,4848
;.dw 5000,5150,5299,5446,5592,5736
;.dw 5878,6018,6157,6293,6428,6561
;.dw 6691,6820,6947,7071,7193,7314
;.dw 7431,7547,7660,7771,7880,7986
;.dw 8090,8192,8290,8387,8480,8572
;.dw 8660,8746,8829,8910,8988,9063
;.dw 9135,9205,9272,9336,9397,9455
;.dw 9511,9563,9613,9659,9703,9744
;.dw 9781,9816,9848,9877,9903,9925
;.dw 9945,9962,9976,9986,9994,9998
;.dw 10000
;************************************

.cseg			; выбор сегмента памяти программ
dataRX: .db ':','R','S','-','4','8','5','+'
dataTX: .db ':','P','C','-','A','V','R','+'
; личные позывные каждого микроконтроллера
; 4 байта ID
dataID:	.db 'I','D','M','K','B','P','R','F','S','F'
; воспринимаемы команды
	; запрос на байт из памяти программ
;dataBP: 
	; запрос на байт из регистрового файла или I/O
;dataRF: 
	; специальный ответ
;dataSF: 
