;--------------------------------------------------------
; Extract from PC 1500 Technical Reference.pdf
;--------------------------------------------------------
	.module using
	
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area	CODE
	.z80
	ld b, 0x79
	ld c, 0x00
	ld d, 0x7a
	call 0xf73f
	ld a, 0x01
	sbr 0x96
	dec a
	ld l,a
	ld d, 0x78
	ld e, 0xc0
	ldi (de),(bc)
	djl -0x03
	ld a,e
	and a, 0x0f
	jr z, -0x02
	and (de), 0x00
	ret
