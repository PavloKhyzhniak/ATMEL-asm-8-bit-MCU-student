.DSEG                        ; ������ �������� ������ 
; ����� ����� 
Secund:	.BYTE 1	;�������
Minut:	.BYTE 1	;������
Chas:	.BYTE 1	;����
Day:	.BYTE 1	;���� ������
Data:	.BYTE 1	;����
Mon:	.BYTE 1	;�����
Year:	.BYTE 1	;���
Status:	.BYTE 1	;���� �������
Secund2:.BYTE 1	;�������
Minut2:	.BYTE 1	;������
Chas2:	.BYTE 1	;����
Day2:	.BYTE 1	;���� ������
Data2:	.BYTE 1	;����
Mon2:	.BYTE 1	;�����
Year2:	.BYTE 1	;���
Status2:.BYTE 1	;���� �������
AdrUstr: .BYTE 1	;����� ������
ContrSum: .BYTE 1	;����������� ����� �������� ������  
NewTime:.BYTE 1	;
OldPortD:.BYTE 1;������� ��������� ����� D
FlagNewS:.BYTE 1;���� ����� ������� (1 - ����� �������)
NumADCkan:.BYTE 1;����� ������ ���, ����������� � ������� ������
ADC0l:	.BYTE 1	;������� �������� �������� ����� ��� 0-�� ������	
ADC0h:	.BYTE 1	;������� �������� �������� ����� ��� 0-�� ������	  
ADC1l:	.BYTE 1	;������� �������� �������� ����� ��� 1-�� ������	
ADC1h:	.BYTE 1	;������� �������� �������� ����� ��� 1-�� ������	  
ADC2l:	.BYTE 1	;������� �������� �������� ����� ��� 2-�� ������	
ADC2h:	.BYTE 1	;������� �������� �������� ����� ��� 2-�� ������	  
ADC3l:	.BYTE 1	;������� �������� �������� ����� ��� 3-�� ������	
ADC3h:	.BYTE 1	;������� �������� �������� ����� ��� 4-�� ������	  
;C_Vib_P:.BYTE 1	;������� � �������
CountADCvib:.BYTE 2;����� ��������� � ������ �������
CountPer:.BYTE 1	;����� �������� � ������ �������
SumADC0:.BYTE 4	;����� 0-�� ������ (ADC0)
SumADC1:.BYTE 4	;����� 1-�� ������ (ADC1)
SumADC2:.BYTE 4	;����� 2-�� ������ (ADC2)
SumADC3:.BYTE 4	;����� 3-�� ������ (ADC3)
Sum2ADC0:.BYTE 4	;����� ��������� 0-�� ������ (ADC0)**2
Sum2ADC1:.BYTE 4	;����� ��������� 1-�� ������ (ADC1)**2
Sum2ADC2:.BYTE 4	;����� ��������� 2-�� ������ (ADC2)**2
Sum2ADC3:.BYTE 4	;����� ��������� 3-�� ������ (ADC3)**2
ADC0min:.BYTE 2	;����������� �������� 0-�� ������
ADC0max:.BYTE 2	;������������ �������� 0-�� ������
ADC1min:.BYTE 2	;����������� �������� 1-�� ������
ADC1max:.BYTE 2	;������������ �������� 1-�� ������
ADC2min:.BYTE 2	;����������� �������� 2-�� ������
ADC2max:.BYTE 2	;������������ �������� 2-�� ������
ADC3min:.BYTE 2	;����������� �������� 3-�� ������
ADC3max:.BYTE 2	;������������ �������� 3-�� ������

_CountADCvib:.BYTE 2;����� ��������� � ������ �������
_CountPer:.BYTE 1	;����� �������� � ������ �������
_SumADC0:.BYTE 4	;����� 0-�� ������ (ADC0)
_SumADC1:.BYTE 4	;����� 1-�� ������ (ADC1)
_SumADC2:.BYTE 4	;����� 2-�� ������ (ADC2)
_SumADC3:.BYTE 4	;����� 3-�� ������ (ADC3)
_Sum2ADC0:.BYTE 4	;����� ��������� 0-�� ������ (ADC0)**2
_Sum2ADC1:.BYTE 4	;����� ��������� 1-�� ������ (ADC1)**2
_Sum2ADC2:.BYTE 4	;����� ��������� 2-�� ������ (ADC2)**2
_Sum2ADC3:.BYTE 4	;����� ��������� 3-�� ������ (ADC3)**2
_ADC0min:.BYTE 2	;����������� �������� 0-�� ������
_ADC0max:.BYTE 2	;������������ �������� 0-�� ������
_ADC1min:.BYTE 2	;����������� �������� 1-�� ������
_ADC1max:.BYTE 2	;������������ �������� 1-�� ������
_ADC2min:.BYTE 2	;����������� �������� 2-�� ������
_ADC2max:.BYTE 2	;������������ �������� 2-�� ������
_ADC3min:.BYTE 2	;����������� �������� 3-�� ������
_ADC3max:.BYTE 2	;������������ �������� 3-�� ������

_EndData:
AdressMMC:.BYTE 4	;����� � ����� MMC
AdressMMCInd:.BYTE 4	;����� ������� � ����� MMC (Index)
BeginMMCInd:.BYTE 2	;������ ������ ADC � MMC
StatusWr:.BYTE 1	;��������� ������
CountSec:.BYTE 1	;����� ������ ���������� � ������
IndIn512:.BYTE 2	;������ ��� ������ � ����� 512 ����
ON_WriteADC_MMC:.BYTE 1	;0 - ��������� ������ � MMC 
Err_MMC:.BYTE 1		;�� 0 - ������ ��������� � MMC 

.ORG $100
Buf512: .BYTE 512	;����� ��� ������ � ������ �������
SecPerClast:	.BYTE 1	;�������� �� �������
SecRezPBR:	.BYTE 2	;�������� ��������������� ��� PBR
CountTabFAT:	.BYTE 1	;����� ������ FAT
RootDitEntry:	.BYTE 2	;����� ������� � �������� ��������
SecForFAT:	.BYTE 2	;����� �������� �� FAT
AllSect:	.BYTE 4	;����� �������� �� MMC
Fat_base:	.BYTE 2	;������ FAT = SecRezPBR
Root_base:	.BYTE 2	;�������� ������� = Fat_base+CountTabFAT*SecForFAT
Claster_002:	.BYTE 2	;������ ����� = Root_base+RootDitEntry/16


AdressMMCInd2:.BYTE 4	;����� ������� � ����� MMC (Index2)
Buf_CID:	.BYTE 16;������������� �����
Buf_CSD:	.BYTE 16;���������� � �����


;��������� ���: $A5 � ����������� ����� ������
;BufGet:   .BYTE 64	;����� ����������� ������ (�������� ��� ������)
;AdrFlash: .BYTE 2	;����� ������������ ��������
;AdrUstr: .BYTE 1	;����� ����������
;ContrSum: .BYTE 1	;����������� ����� �������� ������  

.ESEG			; ������ �������� EPROM
.ORG 0
;-------------- ������ � EPROM -----------------------
ReSerW:		.DB	$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
EpResursSYS:    .DW	0,0	;����� ������ ������� � �����

.CSEG                   ; ������ �������� ���� �������� 


