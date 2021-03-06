;------------ ������� ��������� �������� --------------------------------- 
.MACRO Pause                ; ����� � ������������� �������� 65535 (1���)
        ldi wl,low(@0*2)   ; ��������� ������� ���� ��������� 0
        ldi wh,high(@0*2)  ; ��������� ������� ���� ��������� 0
LoopPause:			; 8 ���: (@0*2) - ��������  32767 ���
	SBIW	rW,1		; 4 ���: (@0)   - ��������  65535 ���
	BRNE	LoopPause	; 2 ���: (@0/2) - �������� 131070 ���
.ENDMACRO                       ; ����� ����������������

.MACRO Pause10ms		; ����� � 10*x ������������ (8���)
	ldi temp,@0		; �� �������� �������� � �����
LoopP10:
        ldi wl,low(10000*2)   ; ��������� ������� ���� ��������� 0
        ldi wh,high(10000*2)  ; ��������� ������� ���� ��������� 0
LoopPau10:			; 8 ���: (@0*2) - ��������  32767 ���
	SBIW	rW,1		; 4 ���: (@0)   - ��������  65535 ���
	BRNE	LoopPau10	; 2 ���: (@0/2) - �������� 131070 ���
	dec	temp
	BRNE	LoopP10
.ENDMACRO                      
;-------------- ������� ��� ������ � ����������� ���� CHAR---------------------
;++++++++ ������������ �������� temp,temp2 ++++++++
.MACRO SET_B		;CHAR=Const	 
	ldi	temp,@1
	sts	@0,temp			 
.ENDMACRO                      

.MACRO CLR_B		;CHAR=0	 
	clr	temp
	sts	@0,temp			 
.ENDMACRO                      

.MACRO MOV_B		;CHAR1=CHAR2	 
	lds	temp,@1			 
	sts	@0,temp			 
.ENDMACRO                      

.MACRO INC_B		;++CHAR
	lds	temp,@0
	inc	temp
	sts	@0,temp			 
.ENDMACRO                      

.MACRO DEC_B		;--CHAR
	lds	temp,@0
	dec	temp
	sts	@0,temp			 
.ENDMACRO                      

.MACRO INC_BM		;++CHAR ���� CHAR<Const
	lds	temp,@0
	cpi	temp,@1
	BREQ	NO_INC_BM
	inc	temp
	sts	@0,temp	
NO_INC_BM:		 
.ENDMACRO                      

.MACRO DEC_BM		;--CHAR ���� CHAR>Const
	lds	temp,@0
	cpi	temp,@1
	BREQ	NO_DEC_BM
	dec	temp
	sts	@0,temp	
NO_DEC_BM:		 
.ENDMACRO                      

.MACRO INC_BL		;++CHAR ���� CHAR<Const1, �� CHAR=Const2
	lds	temp,@0
	inc	temp
	cpi	temp,@1+1
	BRNE	NO_INC_BL
	ldi	temp,@2
NO_INC_BL:		 
	sts	@0,temp	
.ENDMACRO                      

.MACRO DEC_BL		;--CHAR ���� CHAR>Const
	lds	temp,@0
	dec	temp
	cpi	temp,@1-1
	BRNE	NO_DEC_BL
	ldi	temp,@2
NO_DEC_BL:		 
	sts	@0,temp	
.ENDMACRO                      

.MACRO INC_BCDL		;++CHAR ���� CHAR<Const1, �� CHAR=Const2
	lds	temp,@0
	cpi	temp,@1
	BREQ	NO_INC_BCDL
	call	Inc_BCD
	rjmp	EX_INC_BCD
NO_INC_BCDL:
	ldi	temp,@2
EX_INC_BCD:		 
	sts	@0,temp	
.ENDMACRO                      

.MACRO DEC_BCDL		;--CHAR ���� CHAR>Const
	lds	temp,@0
	cpi	temp,@1
	BREQ	NO_DEC_BCDL
	call	Dec_BCD
	rjmp	EX_DEC_BCD
NO_DEC_BCDL:
	ldi	temp,@2
EX_DEC_BCD:		 
	sts	@0,temp	
.ENDMACRO                      

.MACRO CMP_B		;CHAR1-CHAR2 ������������� ����� ��� �������� ���������
	lds	temp,@0
	lds	temp2,@1
	sub	temp,temp2
.ENDMACRO       

.MACRO CPI_B		;CHAR1-Const ������������� ����� ��� �������� ���������
	lds	temp,@0
	subi	temp,@1
.ENDMACRO       

.MACRO SUB_B		;CHAR1-=CHAR2 ������������� ����� ��� �������� ���������
	lds	temp,@0
	lds	temp2,@1
	sub	temp,temp2
	sts	@0,temp
.ENDMACRO       

.MACRO ADD_B		;CHAR1+=CHAR2 ������������� ����� ��� �������� ���������
	lds	temp,@0
	lds	temp2,@1
	add	temp,temp2
	sts	@0,temp
.ENDMACRO       

.MACRO SUBI_B		;CHAR1-=const ������������� ����� ��� �������� ���������
	lds	temp,@0
	subi	temp,@1
	sts	@0,temp
.ENDMACRO       

.MACRO ADDI_B		;CHAR1+=const ������������� ����� ��� �������� ���������
	lds	temp,@0
	subi	temp,-@1
	sts	@0,temp
.ENDMACRO       

.MACRO	PUSH_B		; ��������� ���� � �����
	lds	temp,@0
	push	temp
.ENDMACRO                      

.MACRO	POP_B		; ������������ ���� �� �����
	pop	temp
	sts	@0,temp			 
.ENDMACRO                      
               
.MACRO	JZ		;������� ���� 0 �� �����
	BREQ	@0			 
.ENDMACRO                      

.MACRO	JNZ		;������� ���� �� 0 �� �����
	BRNE	@0			 
.ENDMACRO                      

.MACRO	JE		;������� ���� == �� �����
	BREQ	@0			 
.ENDMACRO                      

.MACRO	JNE		;������� ���� != �� �����
	BRNE	@0			 
.ENDMACRO                      

.MACRO	JC		;������� ���� C==1 �� �����
	BRCS	@0			 
.ENDMACRO                      

.MACRO	JNC		;������� ���� C!=1 �� �����
	BRCC	@0			 
.ENDMACRO                      

.MACRO	JLT		;������� ���� < �� �����
	BRLO	@0			 
.ENDMACRO                      

.MACRO	JLE		;������� ���� <= �� �����
	BRLO	@0			 
	BREQ	@0			 
.ENDMACRO                      

.MACRO	JGE		;������� ���� >= �� �����
	BRSH	@0			 
.ENDMACRO                      

.MACRO	JG		;������� ���� > �� �����
	BREQ	No_JG
	BRSH	@0
No_JG:			 
.ENDMACRO                      

.MACRO	JS		;������� ���� ������������ �� �����
	BRLT	@0			 
.ENDMACRO                      

.MACRO	JNS		;������� ���� ������������ �� �����
	BRGE	@0			 
.ENDMACRO                      

.MACRO CASE		;E��� CHAR1==Const ������� "������������"
	lds	temp,@0
	cpi	temp,@1
	BRNE	No_CASE_EQ
	call	@2
No_CASE_EQ:			 
.ENDMACRO                      

.MACRO  LOOPI_INC	; while(++CHAR != const goto �����)
	lds	temp,@0
	inc	temp
	sts	@0,temp			 
	subi	temp,@1
	BRNE	@2			 
.ENDMACRO                      

.MACRO  LOOPI_DEC	; while(--CHAR != const goto �����)
	lds	temp,@0
	dec	temp
	sts	@0,temp			 
	subi	temp,@1
	BRNE	@2			 
.ENDMACRO                      

.MACRO LOOPB		;���� --CHAR!=0 �� ������� �� "�����"
	lds	temp,@0		 
	dec	temp
	sts	@0,temp
	BRNE	@1
.ENDMACRO       
               
;-------������� ��� ������ � ���������� rW,X,Y,Z � ��������� � ����������� INT
.MACRO GET_X		;X=Int	 
	lds	xl,@0			 
	lds	xh,@0+1			 
.ENDMACRO                      

.MACRO PUT_X		;Int=X	 
	sts	@0,xl			 
	sts	@0+1,xh			 
.ENDMACRO                      

.MACRO	PUSH_X		; ��������� X � �����
	push	xh
	push	xl
.ENDMACRO                      

.MACRO	POP_X		; ������������ X �� �����
	pop	xl
	pop	xh
.ENDMACRO                      

.MACRO	PUSH_Y		; ��������� Y � �����
	push	yh
	push	yl
.ENDMACRO                      

.MACRO	POP_Y		; ������������ Y �� �����
	pop	yl
	pop	yh
.ENDMACRO                      

.MACRO	PUSH_Z		; ��������� Z � �����
	push	zh
	push	zl
.ENDMACRO                      

.MACRO	POP_Z		; ������������ Z �� �����
	pop	zl
	pop	zh
.ENDMACRO                      

.MACRO	PUSH_Wr		; ��������� Wr � �����
	push	wh
	push	wl
.ENDMACRO                      

.MACRO	POP_Wr		; ������������ Wr �� �����
	pop	wl
	pop	wh
.ENDMACRO                      

.MACRO GET_Y		;Y=Int	 
	lds	yl,@0			 
	lds	yh,@0+1			 
.ENDMACRO                      

.MACRO PUT_Y		;Int=Y	 
	sts	@0,yl			 
	sts	@0+1,yh			 
.ENDMACRO                      

.MACRO GET_Z		;Z=Int	 
	lds	zl,@0			 
	lds	zh,@0+1			 
.ENDMACRO                      

.MACRO PUT_Z		;Int=Z	 
	sts	@0,zl			 
	sts	@0+1,zh			 
.ENDMACRO                      

.MACRO SET_X		;X=Const	 
	ldi	xl,low(@0)
	ldi	xh,high(@0)
.ENDMACRO                      

.MACRO SET_Y		;Y=Const	 
	ldi	yl,low(@0)
	ldi	yh,high(@0)
.ENDMACRO                      

.MACRO SET_Z		;Z=Const	 
	ldi	zl,low(@0)
	ldi	zh,high(@0)
.ENDMACRO                      

.MACRO STincI_X		;(X++)=Const
	ldi	temp,@0
	st	X+,temp	 
.ENDMACRO                      

.MACRO STincI_Y		;(Y++)=Const
	ldi	temp,@0
	st	Y+,temp	 
.ENDMACRO                      

.MACRO STincI_Z		;(Z++)=Const
	ldi	temp,@0
	st	Z+,temp	 
.ENDMACRO                      

.MACRO GET_rW		;rW=Int	 
	lds	wl,@0			 
	lds	wh,@0+1			 
.ENDMACRO                      

.MACRO PUT_rW		;Int=rW	 
	sts	@0,wl			 
	sts	@0+1,wh			 
.ENDMACRO                      

.MACRO SET_rW		;rW=Const	 
	ldi	wl,low(@0)
	ldi	wh,high(@0)
.ENDMACRO       

.MACRO CLR_X		;X=0	 
	clr	xl
	clr	xh
.ENDMACRO                      

.MACRO CLR_Y		;Y=0	 
	clr	yl
	clr	yh
.ENDMACRO                      

.MACRO CLR_Z		;Z=0	 
	clr	zl
	clr	zh
.ENDMACRO                      

.MACRO CLR_rW		;rW=0	 
	clr	wl
	clr	wh
.ENDMACRO                      

.MACRO LOOPrW		;���� --rW!=0 �� ������� �� "�����"
	SBIW	rW,1
	BRNE	@1
.ENDMACRO       
               
;-------- ������� ��� ������ � ����������� ���� INT ---------------------
;++++++++ ��������� ������ � ����������� ���� INT ++++++++
;++++++++ ������������ �������� temp,temp2 ++++++++
.MACRO GET_W		;temp2:temp=Int	 
	lds	temp,@0			 
	lds	temp2,@0+1			 
.ENDMACRO                      

.MACRO PUT_W		;Int=temp2:temp	 
	sts	@0,temp			 
	sts	@0+1,temp2			 
.ENDMACRO                      

.MACRO SET_W		;Int=Const	 
	ldi	temp,low(@1)
	sts	@0,temp			 
	ldi	temp,high(@1)
	sts	@0+1,temp			 
.ENDMACRO                      

.MACRO CLR_W		;Int=0	 
	clr	temp
	sts	@0,temp			 
	sts	@0+1,temp			 
.ENDMACRO                      

.MACRO MOV_W		;Int1=Int2	 
	lds	temp,@1		 
	sts	@0,temp			 
	lds	temp,@1+1		 
	sts	@0+1,temp			 
.ENDMACRO                      

.MACRO XCH_W		;Int1<=>Int2	 
	lds	temp,@0		 
	lds	temp2,@1		 
	sts	@1,temp			 
	sts	@0,temp2			 
	lds	temp,@0+1		 
	lds	temp2,@1+1		 
	sts	@1+1,temp		 
	sts	@0+1,temp2			 
.ENDMACRO 

;++++++++ �������������� �������� � ����������� ���� INT ++++++++
;++++++++ ������������ �������� temp,temp2, wl � wh (rW) ++++++++
.MACRO INC_W		;++Int	 
	lds	wl,@0		 
	lds	wh,@0+1
	ADIW	rW,1		 
	sts	@0,wl
	sts	@0+1,wh
.ENDMACRO                      
                     
.MACRO DEC_W		;--Int	 
	lds	wl,@0		 
	lds	wh,@0+1
	SBIW	rW,1		 
	sts	@0,wl
	sts	@0+1,wh
.ENDMACRO                      

.MACRO ADI6_W		;Int+=const	 
	lds	wl,@0		 
	lds	wh,@0+1
	ADIW	rW,@1		 
	sts	@0,wl
	sts	@0+1,wh
.ENDMACRO                      

.MACRO ADI16_W		;Int+=const	 
	lds	wl,@0		 
	lds	wh,@0+1
	ldi	temp2,low(@1)
	add	wl,temp2
	ldi	temp2,high(@1)
	adc	wh,temp2		 
	sts	@0,wl
	sts	@0+1,wh
.ENDMACRO                      

.MACRO SUI6_W		;Int-=const	 
	lds	wl,@0		 
	lds	wh,@0+1
	SBIW	rW,@1		 
	sts	@0,wl
	sts	@0+1,wh
.ENDMACRO                      

.MACRO SUI16_W		;Int-=const	 
	lds	wl,@0		 
	lds	wh,@0+1
	subi	wl,low(@1)
	sbci	wh,high(@1)		 
	sts	@0,wl
	sts	@0+1,wh
.ENDMACRO                      

.MACRO CPI6_W		;rW=Int-const (������������� ����� ��� ���������)	 
	lds	wl,@0	;����� �������� �������� ���������� �� ������		 
	lds	wh,@0+1
	SBIW	rW,@1		 
.ENDMACRO                      

.MACRO ADD_W		;Int1+=Int2	 
	lds	temp,@0		 
	lds	temp2,@1		 
	add	temp,temp2
	sts	@0,temp
	lds	temp,@0+1
	lds	temp2,@1+1
	adc	temp,temp2
	sts	@0+1,temp
.ENDMACRO                      

.MACRO SUB_W		;Int1-=Int2	 
	lds	temp,@0		 
	lds	temp2,@1		 
	sub	temp,temp2
	sts	@0,temp
	lds	temp,@0+1
	lds	temp2,@1+1
	sbc	temp,temp2
	sts	@0+1,temp
.ENDMACRO                      

.MACRO CMP_W		;Int1-Int2	 
	lds	temp,@0		 
	lds	temp2,@1		 
	sub	temp,temp2
	lds	temp,@0+1
	lds	temp2,@1+1
	BRNE	NO_NULL_LOW_B
	sbc	temp,temp2
	rjmp	ExCMP_W
NO_NULL_LOW_B:
	sbc	temp,temp2
	CLZ
ExCMP_W:	
.ENDMACRO                      

.MACRO MUL_W2		;Int1*=Int2
	lds	r20,@1
	lds	r21,@1+1
	lds	r22,@0
	lds	r23,@0+1
	call	mul16x16_16
	sts	@0,r16
	sts	@0+1,r17
.ENDMACRO                      
                                                           
.MACRO MUL_WR2		;temp2:temp=Int1*Int2
	lds	r20,@1
	lds	r21,@1+1
	lds	r22,@0
	lds	r23,@0+1
	call	mul16x16_16
.ENDMACRO                      
                                                           
.MACRO MUL_WRI2		;temp2:temp=Int1*const
	ldi	r20,low(@1)
	ldi	r21,high(@1)
	lds	r22,@0
	lds	r23,@0+1
	call	mul16x16_16
.ENDMACRO                      
                                                           
.MACRO MUL_WI2		;Int1*=const
	ldi	r20,low(@1)
	ldi	r21,high(@1)
	lds	r22,@0
	lds	r23,@0+1
	call	mul16x16_16
	sts	@0,r16
	sts	@0+1,r17
.ENDMACRO                      
                                                           
.MACRO MUL_W		;Int1*=Int2
	lds	r20,@1
	lds	r21,@1+1
	lds	r22,@0
	lds	r23,@0+1
	call	mul16x16_32
	sts	@0,r16
	sts	@0+1,r17
.ENDMACRO                      
                                                           
.MACRO MUL_WI		;Int1*=const
	ldi	r20,low(@1)
	ldi	r21,high(@1)
	lds	r22,@0
	lds	r23,@0+1
	call	mul16x16_32
	sts	@0,r16
	sts	@0+1,r17
.ENDMACRO                      
                                                           
.MACRO MUL_WW		;Int2:Int1=Int3*Int4
	lds	r20,@3
	lds	r21,@3+1
	lds	r22,@2
	lds	r23,@2+1
	call	mul16x16_32
	sts	@1,r16
	sts	@1+1,r17
	sts	@0,r18
	sts	@0+1,r19
.ENDMACRO                      
                                                           
.MACRO MUL_WWI		;Int2:Int1=Int3*const
	lds	r20,low(@3)
	lds	r21,high(@3)
	lds	r22,@2
	lds	r23,@2+1
	call	mul16x16_32
	sts	@1,r16
	sts	@1+1,r17
	sts	@0,r18
	sts	@0+1,r19                
.ENDMACRO                      
                                                           
.MACRO MUL_WD		;long1=Int2*Int3
	lds	r20,@2
	lds	r21,@2+1
	lds	r22,@1
	lds	r23,@1+1
	call	mul16x16_32
	sts	@0,r16
	sts	@0+1,r17
	sts	@0+2,r18          
	sts	@0+3,r19
.ENDMACRO                      
                                                           
.MACRO MUL_WR		;r19:r18:r17:r16=Int1*Int2
	lds	r20,@1
	lds	r21,@1+1
	lds	r22,@0
	lds	r23,@0+1
	call	mul16x16_32
.ENDMACRO                      
                                                           
.MACRO MUL_WRI		;r19:r18:r17:r16=Int1*const
	ldi	r20,low(@1)
	ldi	r21,high(@1)
	lds	r22,@0
	lds	r23,@0+1
	call	mul16x16_32
.ENDMACRO                      
                                                           
.MACRO	DIV_W100 ;Int/=$100;
	lds	temp,@0+1
	sts	@0,temp
	clr	temp
	sts	@0+1,temp
.ENDMACRO                      

.MACRO	DIV_W10	;Int/=$10;
	lds	temp,@0
	lds	temp2,@0+1
	SWAP	temp
	SWAP	temp2
	mov	temp3,temp2
	andi	temp,$F
	andi	temp2,$F0
	or	temp,temp2
	sts	@0,temp
	mov	temp2,temp3
	andi	temp2,$F
	sts	@0+1,temp2
.ENDMACRO                      

.MACRO	ADD_W_d10	;Int1+=Int2/$10;
	lds	temp,@1
	lds	temp2,@1+1
	SWAP	temp
	SWAP	temp2
	mov	temp3,temp2
	andi	temp,$F
	andi	temp2,$F0
	or	temp,temp2
	mov	temp2,temp3
	andi	temp2,$F

	lds	temp3,@0		 
	add	temp,temp3
	sts	@0,temp
	lds	temp3,@0+1
	adc	temp2,temp3
	sts	@0+1,temp2
.ENDMACRO                      

.MACRO NEG_W		;Int=-Int	 
	lds	wl,@0		 
	lds	wh,@0+1
	com	wl
	com	wh
	SBIW	rW,-1		 
	sts	@0,wl
	sts	@0+1,wh
.ENDMACRO                      

;++++++++ �������� ��������� � ����������� ���� INT +++++++++++++
;++++++++ ������������ �������� temp,temp2, wl � wh (rW) ++++++++
.MACRO LOOPW		;���� --INT!=0 �� ������� �� "�����"
	lds	wl,@0		 
	lds	wh,@0+1
	SBIW	rW,1
	sts	@0,wl
	sts	@0+1,wh
	BRNE	@1
.ENDMACRO       
               
;++++++++ � ����������� ���� LONG +++++++++++++
.MACRO	SET_L	;Long=const
	ldi	temp,low(@1)
	sts	@0,temp
	ldi	temp,BYTE2(@1)
	sts	@0+1,temp
	ldi	temp,BYTE3(@1)
	sts	@0+2,temp
	ldi	temp,BYTE4(@1)
	sts	@0+3,temp
.ENDMACRO                      

.MACRO	ADD_L	;Long1+=Long2
	lds	temp,@0		 
	lds	temp2,@1		 
	add	temp,temp2
	sts	@0,temp
	lds	temp,@0+1
	lds	temp2,@1+1
	adc	temp,temp2
	sts	@0+1,temp
	lds	temp,@0+2
	lds	temp2,@1+2
	adc	temp,temp2
	sts	@0+2,temp
	lds	temp,@0+3
	lds	temp2,@1+3
	adc	temp,temp2
	sts	@0+3,temp
.ENDMACRO                      

.MACRO	ADDI_L	;Long1+=const
	lds	temp,@0		 
	subi	temp,low(-@1)
	sts	@0,temp
	lds	temp,@0+1
	sbci	temp,BYTE2(-@1)
	sts	@0+1,temp
	lds	temp,@0+2
	sbci	temp,BYTE3(-@1)
	sts	@0+2,temp
	lds	temp,@0+3
	sbci	temp,BYTE4(-@1)
	sts	@0+3,temp
.ENDMACRO                      

.MACRO	SUBI_L	;Long1-=const
	lds	temp,@0		 
	subi	temp,low(@1)
	sts	@0,temp
	lds	temp,@0+1
	sbci	temp,BYTE2(@1)
	sts	@0+1,temp
	lds	temp,@0+2
	sbci	temp,BYTE3(@1)
	sts	@0+2,temp
	lds	temp,@0+3
	sbci	temp,BYTE4(@1)
	sts	@0+3,temp
.ENDMACRO                      

.MACRO	SUB_L	;Long1-=Long2
	lds	temp,@0		 
	lds	temp2,@1		 
	sub	temp,temp2
	sts	@0,temp
	lds	temp,@0+1
	lds	temp2,@1+1
	sbc	temp,temp2
	sts	@0+1,temp
	lds	temp,@0+2
	lds	temp2,@1+2
	sbc	temp,temp2
	sts	@0+2,temp
	lds	temp,@0+3
	lds	temp2,@1+3
	sbc	temp,temp2
	sts	@0+3,temp
.ENDMACRO                      

.MACRO	CMP_L	;Long1-Long2
	lds	temp,@0		 
	lds	temp2,@1		 
	sub	temp,temp2
	mov	temp3,temp
	lds	temp,@0+1
	lds	temp2,@1+1
	sbc	temp,temp2
	or	temp3,temp
	lds	temp,@0+2
	lds	temp2,@1+2
	sbc	temp,temp2
	or	temp3,temp
	lds	temp,@0+3
	lds	temp2,@1+3
	sbc	temp,temp2
	or	temp3,temp
.ENDMACRO                      

.MACRO	ADD_LW	;Long1+=Int2
	lds	temp,@0		 
	lds	temp2,@1		 
	add	temp,temp2
	sts	@0,temp
	lds	temp,@0+1
	lds	temp2,@1+1
	adc	temp,temp2
	sts	@0+1,temp
	lds	temp,@0+2
	ldi	temp2,0
	adc	temp,temp2
	sts	@0+2,temp
	lds	temp,@0+3
	ldi	temp2,0
	adc	temp,temp2
	sts	@0+3,temp
.ENDMACRO                      

.MACRO	DIV_L100 ;Long1/=$100;
	lds	temp,@0+1
	sts	@0,temp
	lds	temp,@0+2
	sts	@0+1,temp
	lds	temp,@0+3
	sts	@0+2,temp
	clr	temp
	sts	@0+3,temp
.ENDMACRO                      

.MACRO	DIV_L10000 ;Long1/=$10000;
	lds	temp,@0+2
	sts	@0,temp
	lds	temp,@0+3
	sts	@0+1,temp
	clr	temp
	sts	@0+2,temp
	sts	@0+3,temp
.ENDMACRO                      

.MACRO	CLR_L	;Long1=0;
	clr	temp
	sts	@0,temp
	sts	@0+1,temp
	sts	@0+2,temp
	sts	@0+3,temp
.ENDMACRO                      

.MACRO	MOV_L	;Long1=Long2;
	lds	temp,@1
	sts	@0,temp
	lds	temp,@1+1
	sts	@0+1,temp
	lds	temp,@1+2
	sts	@0+2,temp
	lds	temp,@1+3
	sts	@0+3,temp
.ENDMACRO                      

