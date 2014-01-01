;--------------------------------------------------------
; Extract from PC 1500 Technical Reference.pdf
;--------------------------------------------------------
	.module shiftLeft
	
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area	CODE
shiftLeft:
	psh x
	psh y
	psh u
	lda 0x7600
	aex
	ani a,0x0f
	sta xl
	lda 0x7601
	aex
	ani a, 0x0f
	sta xh
	ldi uh, 0x78
shiftLeft1:
	dec uh
	ldi ul, 0x4d
shiftLeft2:
	dec u
	lin u
	sta yl
	lda (u)
	sta yh
	lda xh
	sde u
	lda xl
	sta (u)
	ldx y
	lop shiftLeft2
	cpi uh, 0x77
	bcs shiftLeft1
	lda xl
	aex
	sta 0x774e
	lda xh
	aex
	sta 0x774f
	pop u
	pop y
	pop x
	rec
	rtn
