;--------------------------------------------------------
; Extract from Programming the Z80
;--------------------------------------------------------
	.module zaks8x8
	
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area	CODE
	ld bc, (0x8000)
	ld b,8
	ld de, (0x8001)
	ld d,0
	ld hl, 0
mult:
	srl c
	jr nc, noadd
	add hl, de
noadd:
	sla e
	rl d
	dec b
	jp nz, mult
	ld (0x8002),hl
	ret

