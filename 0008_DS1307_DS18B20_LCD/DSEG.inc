;************************************
;** Файл резервирования переменных в ОЗУ и констант
;**
;**
;************************************
;************************************
; инициализируем необходимые названия для выводов

; контроль температуры по 1-Wire
.set DQ_PORT = PORTB; порт датчика температуры или программной 1-Wire
.set DQ_DDR = DDRB
.set DQ = PB3
.set tempDQ = PB3

; контроль времени по SPI
.set SCL = PC5
.set SDA = PC4


/*
; управление ЖКИ и кнопки
.set LCD_PORT = PORTB ; порт управления ЖКИ
.set LCD_DDR = DDRB
.set LCD_PIN = PINB
.set A0 = PB5;Выход выбора передачи данных или команд управления на ЖКИ (A0) (RS)
.set EN = PB4;Выход разрешения обращения к ЖКИ (а также строб данных)(E)


.set DATA_PORT = PORTC ; порт передачи данных
.set DATA_DDR = DDRC
.set DATA_PIN = PINC
.set DATA = PC0;Выход data (загрузка 555ИР8 - 74HC164 данными)
.set CLK = PC2;Выход clock(синхроимпульсы для 555ИР8 - 74HC164)


.set KEY_PORT = PORTC ; порт кнопок
.set KEY_DDR = DDRC
.set KEY_PIN = PINC
.set B_RET1 = PC3;Вход опроса кнопок управления 1


.set LED_LCD_PORT = PORTC ;порт подсветки
.set LED_LCD_DDR = DDRC
.set LED_LCD_PIN = PINC
.set LED = PC1
*/
; управление ЖКИ и кнопки
.set LCD_PORT = PORTB ; порт управления ЖКИ
.set LCD_DDR = DDRB
.set LCD_PIN = PINB
.set A0 = PB2;Выход выбора передачи данных или команд управления на ЖКИ (A0) (RS)
.set EN = PB3;Выход разрешения обращения к ЖКИ (а также строб данных)(E)


.set DATA_PORT = PORTB ; порт передачи данных
.set DATA_DDR = DDRB
.set DATA_PIN = PINB
.set DATA = PB1;Выход data (загрузка 555ИР8 - 74HC164 данными)
.set CLK = PB0;Выход clock(синхроимпульсы для 555ИР8 - 74HC164)


.set KEY_PORT = PORTB ; порт кнопок
.set KEY_DDR = DDRB
.set KEY_PIN = PINB
.set B_RET1 = PB5;Вход опроса кнопок управления 1


.set LED_LCD_PORT = PORTB ;порт подсветки
.set LED_LCD_DDR = DDRB
.set LED_LCD_PIN = PINB
.set LED = PB4
;************************************
;выбор сегмента EEPROM
.eseg
.org	0x08




;************************************
;выбор сегмента ОЗУ
.dseg
.org 0x100

TmpVar_LCD: .BYTE 8

cntTimer:	.BYTE 2; счетчик времени

;Temperature:	.BYTE 2; 

cntKey:	.BYTE 1
TimeLED:	.BYTE 1

;************************************


;************************************
; выбор сегмента памяти программ
.cseg		
;RusAlp:
;.db 0x41,0xA0,0x42,0xA1,0xE0,0x45,0xA2,0xA3,0xA4,0xA5
;.db 0xA6,0x4B,0xA7,0x4D,0x48,0x4F,0xA8,0x50,0x43,0x54
;.db 0xA9,0xAA,0x58,0xE1,0xAB,0xAC,0xE2,0xAD,0xAE,0xC4
;.db 0xAF,0xB0,0xB1,$20,$20,$20,$20,$20,$20,$20
;прописные
;.db 0x61,0xB2,0xB3,0xB4,0xE3,0x65,0xB5,0xB6,0xB7,0xB8
;.db 0xB9,0xBA,0xBB,0xBC,0xBD,0x6F,0xBE,0x70,0x63,0xBF
;.db 0x79,0xE4,0xD5,0xE5,0xC0,0xC1,0xE6,0xC2,0xC3,0xC4
;.db 0xC5,0xC6,0xC7,$20,$20,$20,$20,$20,$20,$20

/*; Константы русского алфавита
;строчные
;А
.equ Ab =	0x41         
;Б
.equ Bb =	0xA0
;В
.equ Vb =	0x42
;Г
.equ Gb =	0xA1
;Д
.equ Db =	0xE0
;Е
.equ Eb =	0x45        
;Ё
.equ E2b =	0xA2
;Ж
.equ JEb =	0xA3       
;З
.equ Zb =	0xA4
;И
.equ Ib =	0xA5
;Й
.equ I2b =	0xA6
;К
.equ Kb =	0x4B          
;Л
.equ Lb =	0xA7
;М
.equ Mb =	0x4D
;Н
.equ Nb =	0x48
;О
.equ Ob =	0x4F
;П
.equ Pb =	0xA8
;Р
.equ Rb =	0x50
;С
.equ Cb =	0x43
;Т
.equ Tb =	0x54
;У
.equ Ub =	0xA9
;Ф
.equ Fb =	0xAA
;Х
.equ Hb =	0x58
;Ц
.equ Cmb =	0xE1
;Ч
.equ CHb =	0xAB
;Ш
.equ SHb =	0xAC
;Щ
.equ SHmb =	0xE2
;Ъ
.equ Tzb = 	0xAD
;Ы
.equ SSb =	0xAE
;Ь
.equ Mzb =	0xC4
;Э
.equ Einvb =	0xAF
;Ю
.equ Udb =	0xB0
;Я
.equ Iab = 	0xB1
;прописные
;а
.equ Ap =	0x61
;б
.equ Bp =	0xB2
;в
.equ Vp =	0xB3
;г
.equ Gp =	0xB4
;д
.equ Dp =	0xE3
;е
.equ Ep =	0x65
;ё
.equ E2p =	0xB5
;ж
.equ JEp =	0xB6
;з
.equ Zp =	0xB7
;и
.equ Ip =	0xB8        
;й
.equ I2p =	0xB9
;к
.equ Kp =	0xBA
;л
.equ Lp =	0xBB
;м
.equ Mp =	0xBC
;н
.equ Np =	0xBD
;о
.equ Op =	0x6F       
;п
.equ Pp =	0xBE
;р
.equ Rp =	0x70
;с
.equ Sp = 	0x63
;т
.equ Tp =	0xBF       
;у
.equ Up =	0x79
;ф
.equ Fp =	0xE4
;х
.equ Hp =	0xD5
;ц
.equ Cmp =	0xE5
;ч
.equ CHp =	0xC0
;ш
.equ SHp =	0xC1       
;щ
.equ SHmp =	0xE6
;ъ
.equ Tzp =	0xC2
;ы
.equ SSp =	0xC3
;ь
.equ Mzp =	0xC4
;э
.equ Einvp =	0xC5     
;ю
.equ Udp =	0xC6
;я
.equ Iap =	0xC7
*/
;
;************************************

RusAbout:
NameTIME: .db	'T','I','M','E',$20,$20,$20,$20
.db	'D','i','a','m','e','t','e','r'
.db	$20,$20,$20,$20,$20,$20,$20,$20,$20,$20,$20,$20,$20,$20,$20,$20,$20,$20,$20,$20
.db	$20,$20,$20,$20,$20,$20,$20,$20,$20,$20,$20,$20,$20,$20,'V','e','r','.','0','1'
;EmptyLine:	.db	$20,$20,$20,$20,$20,$20,$20,$20,$20,$20,$20,$20,$20,$20,$20,$20,$20,$20,$20,$20
;EmptyLine:	.db	$20,$20,$20,$20,$20,$20,$20,$20,$20,$20,$20,$20,$20,$20,$20,$20,$20,$20,$20,$20

Rus: .db $4f,$4f,$4f,$cb,$58,$4b,$a4,$ca,$af,$48,$45,$50,$a1,$4f,$cb,$20,'v','.','0','1'

;Var_Number: .dw SetType,D,H,wireD,L,Lk,impN,Pause,deltaH,EncoderImp,SetSpeed
Var_LCD:

Name01: .db	$20,'D','i','a','m','e','t','e','r',$20
Var01:	.db	'D',$20,$20,$20,$20,$20,$20,'m','m',$20
Min01:	.db	'M','i','n',$20,'9','9','m','m',$20,$20
Max01:	.db	'M','a','x',$20,'9','9','0','m','m',$20

Name02: .db $20,'H','e','i','g','h','t',$20,$20,$20
Var02:	.db	'H',$20,$20,$20,$20,$20,$20,'m','m',$20
Min02:	.db	'M','i','n',$20,'9','9','m','m',$20,$20
Max02:	.db	'M','a','x',$20,'9','9','0','m','m',$20

Name03: .db	$20,'W','i','r','e',$20,$20,$20,$20,$20
Var03:	.db	'd',$20,$20,$20,$20,$20,$20,'m','i','l'
Min03:	.db	'M','i','n',$20,'0','5','m','i','l',$20
Max03:	.db	'M','a','x',$20,'2','5','0','m','i','l'

Name04: .db $20,'L','e','n','g','h','t',$20,$20,$20
Var04:	.db	'L',$20,$20,$20,$20,$20,$20,'m',$20,$20
Min04:	.db	'M','i','n',$20,'1','0','m',$20,$20,$20
Max04:	.db	'M','a','x',$20,'5','0','0','m',$20,$20


KeyMetca:
KeyUp:		.db	'U','p',$20,$20,$20,$20,$20,$20,$20,$20
KeyDown:	.db	'D','o','w','n',$20,$20,$20,$20,$20,$20
KeyDec:		.db	'D','e','c',$20,$20,$20,$20,$20,$20,$20
KeyInc:		.db	'I','n','c',$20,$20,$20,$20,$20,$20,$20
KeyEnt:		.db	'S','a','v','e',$20,$20,$20,$20,$20,$20
KeyEsc:		.db	'L','o','a','d',$20,$20,$20,$20,$20,$20
KeyStart:	.db	'R','u','n',$20,$20,$20,$20,$20,$20,$20
KeyStop:	.db	'S','t','o','p',$20,$20,$20,$20,$20,$20

;************************************
; Константы семисегментного индикатора
;.equ a = 1		; Bit 0 - a, вес разряда 1
;.equ b = 2		; Bit 1 - b, вес разряда 2
;.equ c = 4		; Bit 2 - c, вес разряда 4
;.equ d = 8		; Bit 3 - d, вес разряда 8
;.equ e = 16		; Bit 4 - e, вес разряда 16
;.equ f = 32		; Bit 5 - f, вес разряда 32
;.equ g = 64		; Bit 6 - g, вес разряда 64
;.equ DP = 128	; Bit 7 - DP, вес разряда 128
;
;.equ Num0 = a+b+c+d+e+f	; цифра 0
;.equ Num1 = b+c			; цифра 1
;.equ Num2 = a+b+d+e+g	; цифра 2
;.equ Num3 = a+b+c+d+g	; цифра 3
;.equ Num4 = b+c+f+g		; цифра 4
;.equ Num5 = a+c+d+f+g	; цифра 5
;.equ Num6 = a+c+d+e+f+g	; цифра 6
;.equ Num7 = a+b+c		; цифра 7
;.equ Num8 = a+b+c+d+e+f+g	; цифра 8
;.equ Num9 = a+b+c+d+f+g; цифра 9
;
;Таблица констант для вывода цифр на семисегментный индикатор
; 0 1 2 3 4 5 6 7 8 9
;TableIndicator:
;.db Num0,Num1	;.db	0b00111111, 0b00000110
;.db Num2,Num3	;.db	0b01011011, 0b01001111
;.db Num4,Num5	;.db	0b01100110, 0b01101101
;.db Num6,Num7	;.db	0b01111101, 0b00000111
;.db Num8,Num9	;.db	0b01111111, 0b01101111
;
; Таблица углов открывания для тиристора(задержка)
; Предделитель Таймера0 выбран = 64
;TableTRIAC:
;.db 255-(0000/64), 255-(2587/64)
; ;полностью открыт ;90 % мощности
;.db 255-(3362/64), 255-(3962/64) 
;80 % мощности	   ;70 % мощности
;.db 255-(4494/64), 255-(4999/64) 
;60 % мощности	   ;50 % мощности
;.db 255-(5503/64), 255-(6035/64)
;40 % мощности	   ;30 % мощности
;.db 255-(6636/64), 255-(7410/64) 
;20 % мощности     ;10 % мощности


; Таблица синусов
; Содержит 90 отсчетов от 0 до 90 градусов
; Расчитан на 14 битный ЦАП
;.equ Bits = 8
;.equ Half = 1<<(Bits-1)
;.equ HalfSinus = Half
;.equ MaxNorm = 10000
;TableSinus:
;grad0:
;.dw    0*Half/MaxNorm, 175*Half/MaxNorm, 349*Half/MaxNorm, 523*Half/MaxNorm, 698*Half/MaxNorm, 872*Half/MaxNorm
;.dw 1045*Half/MaxNorm,1219*Half/MaxNorm,1392*Half/MaxNorm,1564*Half/MaxNorm,1736*Half/MaxNorm,1908*Half/MaxNorm
;.dw 2079*Half/MaxNorm,2250*Half/MaxNorm,2419*Half/MaxNorm,2588*Half/MaxNorm,2756*Half/MaxNorm,2942*Half/MaxNorm
;.dw 3090*Half/MaxNorm,3256*Half/MaxNorm,3420*Half/MaxNorm,3584*Half/MaxNorm,3746*Half/MaxNorm,3897*Half/MaxNorm
;.dw 4067*Half/MaxNorm,4226*Half/MaxNorm,4384*Half/MaxNorm,4540*Half/MaxNorm,4695*Half/MaxNorm,4848*Half/MaxNorm
;grad30:
;.dw 5000*Half/MaxNorm,5150*Half/MaxNorm,5299*Half/MaxNorm,5446*Half/MaxNorm,5592*Half/MaxNorm,5736*Half/MaxNorm
;.dw 5878*Half/MaxNorm,6018*Half/MaxNorm,6157*Half/MaxNorm,6293*Half/MaxNorm,6428*Half/MaxNorm,6561*Half/MaxNorm
;.dw 6691*Half/MaxNorm,6820*Half/MaxNorm,6947*Half/MaxNorm,7071*Half/MaxNorm,7193*Half/MaxNorm,7314*Half/MaxNorm
;.dw 7431*Half/MaxNorm,7547*Half/MaxNorm,7660*Half/MaxNorm,7771*Half/MaxNorm,7880*Half/MaxNorm,7986*Half/MaxNorm
;.dw 8090*Half/MaxNorm,8192*Half/MaxNorm,8290*Half/MaxNorm,8387*Half/MaxNorm,8480*Half/MaxNorm,8572*Half/MaxNorm
;grad60:
;.dw 8660*Half/MaxNorm,8746*Half/MaxNorm,8829*Half/MaxNorm,8910*Half/MaxNorm,8988*Half/MaxNorm,9063*Half/MaxNorm
;.dw 9135*Half/MaxNorm,9205*Half/MaxNorm,9272*Half/MaxNorm,9336*Half/MaxNorm,9397*Half/MaxNorm,9455*Half/MaxNorm
;.dw 9511*Half/MaxNorm,9563*Half/MaxNorm,9613*Half/MaxNorm,9659*Half/MaxNorm,9703*Half/MaxNorm,9744*Half/MaxNorm
;.dw 9781*Half/MaxNorm,9816*Half/MaxNorm,9848*Half/MaxNorm,9877*Half/MaxNorm,9903*Half/MaxNorm,9925*Half/MaxNorm
;.dw 9945*Half/MaxNorm,9962*Half/MaxNorm,9976*Half/MaxNorm,9986*Half/MaxNorm,9994*Half/MaxNorm,9998*Half/MaxNorm
;.dw 10000*Half/MaxNorm-1
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


