;************************************
;**
;**
;**
;************************************
EXT_INT0:

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

; ��������� ������� ��������� � ��������
		lds xl,cntEncoderR
		lds xh,cntEncoderR+1

; ��������� ������� ��������� � ��������
		lds yl,cntEncoderL
		lds yh,cntEncoderL+1

; ���������� ����������� �������� ��������
		sbis PIND,ChannelB
		rjmp LeftWay
		rjmp RightWay
LeftWay:
; ������ ����������� ��������
		ldi r16,1
; ���������� ������� ���������
		sbiw Y,1

;mov r16,yl
;mov r17,yh
;		rcall OutLCD_VAR; r16,r17

; ������� �� ����
		mov r17,yh
		or r17,yl
		breq DoStep
		rjmp ExitINT
RightWay:
; ������ ����������� ��������
		clr r16
; ���������� ������� ���������
		sbiw X,1
;mov r16,xl
;mov r17,xh
;		rcall OutLCD_VAR; r16,r17
	
; ������� �� ����
		mov r17,xh
		or r17,xl
		breq DoStep
		rjmp ExitINT
DoStep:
; ������ ���
;��� ����� ������������������ ���������� ��������� �� ���������� ����

		lds yl,EncoderImp
		lds yh,EncoderImp+1

;������� �� 10 ��� ������ � 100 ������� 1000��������� �� ������!!!
		rol yl
		rol yh
	
		mov xl,yl
		mov xh,yh
		
		rol yl
		rol yh
		rol yl
		rol yh
	
		add yl,xl
		adc yh,xh
		;*2
		rol yl
		rol yh
	
		mov xl,yl
		mov xh,yh

;��������� �������� ��� ��������� ����
		lds 	r16,SetSpeed
		sts 	Speed,r16
		lds 	r16,SetSpeed+1
		sts 	Speed+1,r16	
; �������� ��������� ��������� �����	
; � �������� �� �� ��������� ���,
; � ������ ���� ��������� �� ����� ���������� ���������� �����������
		lds r16,cntDriver
		lds r17,cntDriver+1

		lds zl,ImpStep
		lds zh,ImpStep+1
		add r16,zl
		adc r17,zh
;******************************************
; ������� ��������� ��� ������, 10 ���������
;		ldi zl,Low(10)
;		ldi zh,high(10)
;		add r16,zl
;		adc r17,zh
;******************************************

; �������� ����� �������� �����
		sts cntDriver,r16
		sts cntDriver+1,r17

ExitINT:
; �������� ���������� ������
		sts Way,r16

		sts cntEncoderR,XL
		sts cntEncoderR+1,XH

		sts cntEncoderL,YL
		sts cntEncoderL+1,YH

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


EXT_INT1:

		reti
