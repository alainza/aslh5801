;--------------------------------------------------------
; Extract from PC 1500 Technical Reference.pdf
;--------------------------------------------------------
	.module maths
	
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area	CODE (ABS)
maths:
	ldi ul, 0xf9
	ldi uh, 0x3a
	; Copy X value in 0x3af9-0x3afA
	lda xl
	sta (u)
	inc ul
	lda xh
	sta (u)
	; Increment X
	inc xl
	bcr fini
	inc xh
fini:
	sec	;set carry to return x register in callig variable
	rtn
