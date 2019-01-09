.DSEG                        ; Начало сегмента данных 
; Буфер часов 
Secund:	.BYTE 1	;Секунды
Minut:	.BYTE 1	;Минуты
Chas:	.BYTE 1	;Часы
Day:	.BYTE 1	;День недели
Data:	.BYTE 1	;Дата
Mon:	.BYTE 1	;Месяц
Year:	.BYTE 1	;Год
Status:	.BYTE 1	;Байт статуса
Secund2:.BYTE 1	;Секунды
Minut2:	.BYTE 1	;Минуты
Chas2:	.BYTE 1	;Часы
Day2:	.BYTE 1	;День недели
Data2:	.BYTE 1	;Дата
Mon2:	.BYTE 1	;Месяц
Year2:	.BYTE 1	;Год
Status2:.BYTE 1	;Байт статуса
AdrUstr: .BYTE 1	;Адрес памяти
ContrSum: .BYTE 1	;Контрольная сумма принятых данных  
NewTime:.BYTE 1	;
OldPortD:.BYTE 1;Прошлое состояние порта D
FlagNewS:.BYTE 1;Флаг новой секунды (1 - новая секунда)
NumADCkan:.BYTE 1;Номер канала АЦП, измеряемого в текущий момент
ADC0l:	.BYTE 1	;Текущее значение младшего байта АЦП 0-го канала	
ADC0h:	.BYTE 1	;Текущее значение старшего байта АЦП 0-го канала	  
ADC1l:	.BYTE 1	;Текущее значение младшего байта АЦП 1-го канала	
ADC1h:	.BYTE 1	;Текущее значение старшего байта АЦП 1-го канала	  
ADC2l:	.BYTE 1	;Текущее значение младшего байта АЦП 2-го канала	
ADC2h:	.BYTE 1	;Текущее значение старшего байта АЦП 2-го канала	  
ADC3l:	.BYTE 1	;Текущее значение младшего байта АЦП 3-го канала	
ADC3h:	.BYTE 1	;Текущее значение старшего байта АЦП 4-го канала	  
;C_Vib_P:.BYTE 1	;Выборок в периоде
CountADCvib:.BYTE 2;Число измерений с начала секунды
CountPer:.BYTE 1	;число периодов с начала секунды
SumADC0:.BYTE 4	;Сумма 0-го канала (ADC0)
SumADC1:.BYTE 4	;Сумма 1-го канала (ADC1)
SumADC2:.BYTE 4	;Сумма 2-го канала (ADC2)
SumADC3:.BYTE 4	;Сумма 3-го канала (ADC3)
Sum2ADC0:.BYTE 4	;Сумма квадратов 0-го канала (ADC0)**2
Sum2ADC1:.BYTE 4	;Сумма квадратов 1-го канала (ADC1)**2
Sum2ADC2:.BYTE 4	;Сумма квадратов 2-го канала (ADC2)**2
Sum2ADC3:.BYTE 4	;Сумма квадратов 3-го канала (ADC3)**2
ADC0min:.BYTE 2	;Минимальное значение 0-го канала
ADC0max:.BYTE 2	;Максимальное значение 0-го канала
ADC1min:.BYTE 2	;Минимальное значение 1-го канала
ADC1max:.BYTE 2	;Максимальное значение 1-го канала
ADC2min:.BYTE 2	;Минимальное значение 2-го канала
ADC2max:.BYTE 2	;Максимальное значение 2-го канала
ADC3min:.BYTE 2	;Минимальное значение 3-го канала
ADC3max:.BYTE 2	;Максимальное значение 3-го канала

_CountADCvib:.BYTE 2;Число измерений с начала секунды
_CountPer:.BYTE 1	;число периодов с начала секунды
_SumADC0:.BYTE 4	;Сумма 0-го канала (ADC0)
_SumADC1:.BYTE 4	;Сумма 1-го канала (ADC1)
_SumADC2:.BYTE 4	;Сумма 2-го канала (ADC2)
_SumADC3:.BYTE 4	;Сумма 3-го канала (ADC3)
_Sum2ADC0:.BYTE 4	;Сумма квадратов 0-го канала (ADC0)**2
_Sum2ADC1:.BYTE 4	;Сумма квадратов 1-го канала (ADC1)**2
_Sum2ADC2:.BYTE 4	;Сумма квадратов 2-го канала (ADC2)**2
_Sum2ADC3:.BYTE 4	;Сумма квадратов 3-го канала (ADC3)**2
_ADC0min:.BYTE 2	;Минимальное значение 0-го канала
_ADC0max:.BYTE 2	;Максимальное значение 0-го канала
_ADC1min:.BYTE 2	;Минимальное значение 1-го канала
_ADC1max:.BYTE 2	;Максимальное значение 1-го канала
_ADC2min:.BYTE 2	;Минимальное значение 2-го канала
_ADC2max:.BYTE 2	;Максимальное значение 2-го канала
_ADC3min:.BYTE 2	;Минимальное значение 3-го канала
_ADC3max:.BYTE 2	;Максимальное значение 3-го канала

_EndData:
AdressMMC:.BYTE 4	;Адрес в карте MMC
AdressMMCInd:.BYTE 4	;Номер сектора в карте MMC (Index)
BeginMMCInd:.BYTE 2	;Начало записи ADC в MMC
StatusWr:.BYTE 1	;Результат записи
CountSec:.BYTE 1	;Число секунд записанных в сектор
IndIn512:.BYTE 2	;Индекс для записи в буфер 512 байт
ON_WriteADC_MMC:.BYTE 1	;0 - Разрешена запись в MMC 
Err_MMC:.BYTE 1		;Не 0 - ошибка обращения к MMC 

.ORG $100
Buf512: .BYTE 512	;Буфер для записи и чтения сектора
SecPerClast:	.BYTE 1	;Секторов на кластер
SecRezPBR:	.BYTE 2	;Секторов резервированных под PBR
CountTabFAT:	.BYTE 1	;Число таблиц FAT
RootDitEntry:	.BYTE 2	;число записей в корневом каталоге
SecForFAT:	.BYTE 2	;Число секторов на FAT
AllSect:	.BYTE 4	;Всего секторов на MMC
Fat_base:	.BYTE 2	;Начало FAT = SecRezPBR
Root_base:	.BYTE 2	;Корневой каталог = Fat_base+CountTabFAT*SecForFAT
Claster_002:	.BYTE 2	;Начало файла = Root_base+RootDitEntry/16


AdressMMCInd2:.BYTE 4	;Номер сектора в карте MMC (Index2)
Buf_CID:	.BYTE 16;Индетификация карты
Buf_CSD:	.BYTE 16;Информация о карте


;Стартовый код: $A5 в контрольную сумму входит
;BufGet:   .BYTE 64	;Буфер принимаемых данных (страница для записи)
;AdrFlash: .BYTE 2	;Адрес записываемой страницы
;AdrUstr: .BYTE 1	;Адрес микросхемы
;ContrSum: .BYTE 1	;Контрольная сумма принятых данных  

.ESEG			; Начало сегмента EPROM
.ORG 0
;-------------- Данные в EPROM -----------------------
ReSerW:		.DB	$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
EpResursSYS:    .DW	0,0	;Общий ресурс системы в часах

.CSEG                   ; Начало сегмента кода программ 


