;--------------------------------------------------------
; Extract from PC 1500 Technical Reference.pdf
;--------------------------------------------------------
	.module using
	
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area	CODE
	ldi xh, 0x79
	ldi xl, 0x00
	ldi yh, 0x7a
	sjp 0xf73f
	ldi a, 0x01
	vmj 0x96
	dec a
	sta ul
	ldi yh, 0x78
	ldi yl, 0xc0
	tin
	lop 0x03
	lda yl
	ani a, 0x0f
	bzs -0x02
	ani (y), 0x00
	rtn
