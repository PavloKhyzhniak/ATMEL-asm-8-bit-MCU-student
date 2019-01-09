;************************************
;**
;**
;**
;************************************

PCINT_0:

		reti

PCINT_1:

		reti

PCINT_2:

	push zl
	push zh
	push yl
	push yh
	push xl
	push xh
	push r16
	push r17
	in r16,SREG
	push r16

; �������� ����� ����������� �� �����
		sbi DDRD,CW
; ������� ����� ������ ������ ����������
	
	;********** ������ �� ������� PNP
		sbis PIND,Sensor1
		rjmp SensorLeft

		sbis PIND,Sensor2
		rjmp SensorRight
		
	;********** ������ �� ������� NPN
		;sbic PIND,Sensor1
		;rjmp SensorLeft

		;sbic PIND,Sensor2
		;rjmp SensorRight
		
		rjmp extPCINT

SensorLeft:
; ��������� ��� ���������� ������ ����
; ������ �����������
		cbi PORTD,CW
		rjmp ExitPCINT

SensorRight:
; ��������� ��� ���������� ������� ����
; ������ �����������
		sbi PORTD,CW
		rjmp ExitPCINT
ExitPCINT:
; ����� �� ������������ ��������� ����������
; ��������� ������� ��������� � ��������
		lds xl,cntEncoderR
		lds xh,cntEncoderR+1
	
; ��������� ������� ��������� � ��������
		lds yl,cntEncoderL
		lds yh,cntEncoderL+1
	
		lds zl,ImpPause
		lds zh,ImpPause+1

		add xl,zl
		adc xh,zh

		add yl,zl
		adc yh,zh

		sts cntEncoderR,xl
		sts cntEncoderR+1,xh

		sts cntEncoderL,yl
		sts cntEncoderL+1,yh

; �������� ��������� ��������� �����	
		ldi r16,1
		ldi r17,0

		sts cntDriver,r16
		sts cntDriver+1,r17	

		ldi r16,low(1)
		sts Speed,r16
		sts ActSpeed,r16

		ldi r17,high(1)
		sts Speed+1,r16		
		sts ActSpeed+1,r16		

extPCINT:

	pop r16
	out SREG,r16
	pop r17
	pop r16
	pop xh
	pop xl
	pop yh
	pop yl
	pop zh
	pop zl		

		reti






		