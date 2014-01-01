;--------------------------------------------------------
; Extract from PC 1500 Technical Reference.pdf
;--------------------------------------------------------
	.module reverse
	
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area	CODE (ABS)
	.org	0x3AF9
	.z80
binaryToHexa:
	ld h,0x77
	ld l, 0xe0
	ld a, b
	call binaryToHexa1
	ldi (hl),a
	ld a, b
	call binaryToHexa2
	ldi (hl),a
	ld a, c
	call binaryToHexa1
	ldi (hl),a
	ld a, c
	call binaryToHexa2
	ldi (hl),a
	and (hl), 0x00
	ret
binaryToHexa1:
	nex
binaryToHexa2:
	and a, 0x0f
	cp a, 0x0a
	jr c binaryToHexa3
	adc a, 0x30
	ret
binaryToHexa3:
	adc a, 0x30
	ret
