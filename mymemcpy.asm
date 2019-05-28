; void memcpy(void *dest, const void *source, int count)
_mymemcpy::
	ldhl	sp,#4
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	inc	hl
	ld	c,(hl)
	inc	hl
	ld	b,(hl)
	ldhl	sp,#2
	ld	a,(hl+)
	ld	h,(hl)
	ld	l,a
	inc	b
	inc	c
	jr	2$
1$:
	ld	a,(de)
	ld	(hl+),a
	inc	de
2$:
	dec	c
	jr	nz,1$
	dec	b
	jr	nz,1$	

	ret
