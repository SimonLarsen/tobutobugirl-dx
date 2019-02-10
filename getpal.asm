_get_bkg_palette::
	add	sp, #-2
	ldhl	sp,#4
	ld	a, (hl)
	add	a, a
	add	a, a
	add	a, a
	ld	(hl+), a
	ld	c, (hl)
	inc	hl
	ld	b, (hl)
	push	hl
	dec	hl
	dec	hl
	ld	a, (hl)
	ldhl	sp,#2
	ld	(hl), a
	pop	hl
	ldhl	sp,#1
	ld	(hl), #0x00
00101$:
    ld a,(#0xff41)
	bit	1, a
	jr	NZ,00101$
	ld	de, #0xff68
	ldhl	sp,#0
	ld	a, (hl)
	ld	(de), a
	ld	de, #0xff69
	ld	a,(de)
	ld	(bc), a
	inc	bc
	inc	(hl)
	inc	hl
	inc	(hl)
	ld	a, (hl)
	sub	a, #0x08
	jr	NZ,00101$
	add	sp, #2
	ret
