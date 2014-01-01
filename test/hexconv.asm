;--------------------------------------------------------
; Extract from PC 1500 Technical Reference.pdf
;--------------------------------------------------------
	.module reverse
	
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area	CODE (ABS)
	.org	0x3AF9
binaryToHexa:
	ldi uh,0x77
	ldi ul, 0xe0
	lda xh
	sjp binaryToHexa1
	sin u
	lda xh
	sjp binaryToHexa2
	sin u
	lda xl
	sjp binaryToHexa1
	sin u
	lda xl
	sjp binaryToHexa2
	sin u
	ani (u), 0x00
	rtn
binaryToHexa1:
	aex
binaryToHexa2:
	ani a, 0x0f
	cpi a, 0x0a
	bcs binaryToHexa3
	adi a, 0x30
	rtn
binaryToHexa3:
	adi a, 0x30
	rtn
