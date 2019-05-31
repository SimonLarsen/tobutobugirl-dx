_mydiv::
    ld	e, #0
	ldhl	sp,#2
    ld	a, (hl+)
    ld	c, (hl)
1$:
    sub a, c
    jr C, 2$
    inc e
    jr 1$
2$:
    ret


_mymod::
	ldhl	sp,#2
    ld	a, (hl+)
    ld	c, (hl)
    ld	e, a
1$:
    sub a, c
    jr C, 2$
    ld e, a
    jr 1$
2$:
    ret
