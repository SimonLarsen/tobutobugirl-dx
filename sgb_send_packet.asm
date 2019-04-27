_sgb_send_packet::
	dec	sp
	ld	hl, #0xff00
	ld	(hl), #0x00
	ld	(hl), #0x30
	ldhl	sp,#0
	ld	(hl), #0x00
00108$:
	ldhl	sp,#(4 - 1)
	ld	e, (hl)
	inc	hl
	ld	d, (hl)
	ldhl	sp,#0
	ld	l, (hl)
	ld	h, #0x00
	add	hl, de
	ld	c, l
	ld	b, h
	ld	a, (bc)
	ld	c, a
	ld	b, #0x00
00106$:
	bit	0, c
	jr	Z,00102$
	ld	hl, #0xff00
	ld	(hl), #0x10
	jr	00103$
00102$:
	ld	hl, #0xff00
	ld	(hl), #0x20
00103$:
	ld	hl, #0xff00
	ld	(hl), #0x30
	srl	c
	inc	b
	ld	a, b
	sub	a, #0x08
	jr	NZ,00106$
	ldhl	sp,#0
	inc	(hl)
	ld	a, (hl)
	sub	a, #0x10
	jp	NZ,00108$
	ld	hl, #0xff00
	ld	(hl), #0x20
	ld	(hl), #0x30
	inc	sp
	ret
