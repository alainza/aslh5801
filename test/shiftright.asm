;--------------------------------------------------------
; Extract from PC 1500 Technical Reference.pdf
;--------------------------------------------------------
	.module shiftRight
	
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area	CODE
shiftRight:
	psh x
	psh y
	psh u
	lda 0x774c
	aex
	ani a,0xf0
	sta xl
	lda 0x774d
	aex
	ani a, 0xf0
	sta xh
	ldi uh, 0x75
	ldi ul, 0xff
	inc u
	lin u
	sta yl
	lda (u)
	sta yh
	lda xh
	sde u
	lda xl
	sin u
	ldx y
	cpi ul, 0x4d
	bcr -0x0f
	cpi uh, 0x77
	bcr -0x15
	inc u
	lda xl
	aex
	sin u
	lda xh
	aex
	sta (u)
	pop u
	pop y
	pop x
	rec
	rtn
