.LISTMAC 
.def temp=r16 ;регистр для промежут. операций	
.def temp2=r17 ;2 -й регистр для промежут. операций	
.def temp3=r18 ;3 -й регистр для промежут. операций	
.def temp4=r19 ;4 -й регистр для промежут. операций	
.def rCX1=r22 ;1 -й регистр счётчика	
.def rCX2=r23 ;2 -й регистр счётчика	
.def rW=r24; Для арифметических операций с INT
.def Wl=r24;
.def Wh=r25;
.def CountVibInPer=r8	;Число выборок в текущем периоде
.def CompStat=r9	;Текущее состояние компаратора

.EQU RegClock = 6	;Порт C, порт тактов 
.EQU RegData  = 7	;Порт C, порт данных 
;---------SPI интерфейс-----------
.EQU SPI_LED	= 1	;Индикация обращения к MMC
.EQU SPI_SS	= 2	;Бит для активации MMC
.EQU SPI_MOSI	= 3	;Выход SPI
.EQU SPI_MISO	= 4	;Вход SPI
.EQU SPI_SCK	= 5	;Синхроимпульсы SPI
.equ SPI_port = PORTB	;Порт работы с SPI
.equ SPI_dir  = DDRB	;Порт направления работы с SPI

.EQU BeginSecWr	= $211	;Начальный сектор для записи
.EQU CountWhiteMMC	= 60000	;Ожидание ответа карточки

