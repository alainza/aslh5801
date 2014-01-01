;--------------------------------------------------------
; Extract from PC 1500 Technical Reference.pdf
;--------------------------------------------------------
	.module shiftRight
	
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area	CODE
	.z80
shiftRight:
	push bc
	push de
	push hl
	ld a, (0x774c)
	nex
	and a,0xf0
	ld c,a
	ld a, (0x774d)
	nex
	and a, 0xf0
	ld b,a
	ld h, 0x75
	ld l, 0xff
	inc hl
	ldi a,(hl)
	ld e,a
	ld a, (hl)
	ld d,a
	ld a,b
	ldd (hl),a
	ld a,c
	ldi (hl),a
	ld bc, de
	cp l, 0x4d
	jr nc, -0x0f
	cp h, 0x77
	jr nc, -0x15
	inc hl
	ld a,c
	nex
	ldi (hl),a
	ld a,b
	nex
	ld (hl),a
	pop hl
	pop de
	pop bc
	rcf
	ret
