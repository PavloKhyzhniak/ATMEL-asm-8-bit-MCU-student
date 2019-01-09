.device mega8

#include "m8def.inc"
.cseg
.org 0x000 rjmp Reset
.org 0x001 rjmp EXT_INT0
.org 0x009 rjmp TIM0_OVF

TIM0_OVF:
	ldi r16,-99
	out TCNT0,r16

	in r16,PORTB
	ldi r17,1<<PB1
	eor r16,r17

	out DDRB,r16
	out PORTB,r16

		reti
EXT_INT0:

	in r16,PORTB
	ldi r17,1<<PB0
	eor r16,r17

	out DDRB,r16
	out PORTB,r16

		reti

Reset:
	ldi r16,high(RAMEND); Main program start
	out SPH,r16 ; Set Stack Pointer to top of RAM
	ldi r16,low(RAMEND)
	out SPL,r16
; INT0
	ldi r16,1<<ISC00|1<<ISC01
	out MCUCR,r16

	ldi r16,1<<INT0
	out GICR,r16
;TIMER 0

	ldi r16,1<<TOIE0
	out TIMSK,r16
	
	ldi r16,1<<CS01
	out TCCR0,r16

	ldi r16,-100
	out TCNT0,r16

sei 

main:
	
		rjmp main
