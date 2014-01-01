;--------------------------------------------------------
; Extract from PC 1500 Technical Reference.pdf
;--------------------------------------------------------
	.module shiftLeft
	
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area	CODE
	.z80
shiftLeft:
	PUSH BC
	PUSH DE
	PUSH HL
	ld a, (0x7600)
	nex
	and a,0x0f
	ld c,a
	ld a, (0x7601)
	nex
	and a, 0x0f
	ld b,a
	ld h, 0x78
shiftLeft1:
	dec h
	ld l, 0x4d
shiftLeft2:
	dec hl
	ldi a, (hl)
	ld e,a
	ld a, (hl)
	ld d,a
	ld a,b
	ldd (hl),a
	ld a,c
	ld (hl),a
	ld bc, de
	djl shiftLeft2
	cp h, 0x77
	jr c, shiftLeft1
	ld a,c
	nex
	ld (0x774e),a
	ld a,b
	nex
	ld (0x774f),a
	pop hl
	pop de
	pop bc
	rcf
	ret
