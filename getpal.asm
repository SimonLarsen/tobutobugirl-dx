_get_bkg_palette::
	dec	sp
	ldhl	sp,#(4 - 1)
	ld	c, (hl)
	inc	hl
	ld	b, (hl)
	ldhl	sp,#0
	ld	(hl), #0x00
00105$:
	ld	de, #0xff68
	ldhl	sp,#0
	ld	a, (hl)
	ld	(de), a
00101$:
	ld	a, (#0xff41)
	bit	1, a
	jr	NZ,00101$
	ld	a, (#0xff69)
	ld	(bc), a
	inc	bc
	ldhl	sp,#0
	inc	(hl)
	ld	a, (hl)
	sub	a, #0x40
	jr	NZ,00105$
	inc	sp
	ret
