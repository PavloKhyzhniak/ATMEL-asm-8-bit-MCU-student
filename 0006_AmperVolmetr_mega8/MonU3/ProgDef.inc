.LISTMAC 
.def temp=r16 ;������� ��� ��������. ��������	
.def temp2=r17 ;2 -� ������� ��� ��������. ��������	
.def temp3=r18 ;3 -� ������� ��� ��������. ��������	
.def temp4=r19 ;4 -� ������� ��� ��������. ��������	
.def rCX1=r22 ;1 -� ������� ��������	
.def rCX2=r23 ;2 -� ������� ��������	
.def rW=r24; ��� �������������� �������� � INT
.def Wl=r24;
.def Wh=r25;
.def CountVibInPer=r8	;����� ������� � ������� �������
.def CompStat=r9	;������� ��������� �����������

.EQU RegClock = 6	;���� C, ���� ������ 
.EQU RegData  = 7	;���� C, ���� ������ 
;---------SPI ���������-----------
.EQU SPI_LED	= 1	;��������� ��������� � MMC
.EQU SPI_SS	= 2	;��� ��� ��������� MMC
.EQU SPI_MOSI	= 3	;����� SPI
.EQU SPI_MISO	= 4	;���� SPI
.EQU SPI_SCK	= 5	;�������������� SPI
.equ SPI_port = PORTB	;���� ������ � SPI
.equ SPI_dir  = DDRB	;���� ����������� ������ � SPI

.EQU BeginSecWr	= $211	;��������� ������ ��� ������
.EQU CountWhiteMMC	= 60000	;�������� ������ ��������

