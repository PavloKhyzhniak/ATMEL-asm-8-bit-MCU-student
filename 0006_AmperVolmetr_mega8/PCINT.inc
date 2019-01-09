;************************************
;**
;**
;**
;************************************

PCINT:
	push zl
	push zh
	push xl
	push xh
	push r16
	push r17
	in r16,SREG
	push r16


extPCINT:

	pop r16
	out SREG,r16
	pop r17
	pop r16
	pop xh
	pop xl
	pop zh
	pop zl		

		reti






		
