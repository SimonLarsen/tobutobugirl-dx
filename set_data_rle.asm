_set_bkg_data_rle::
	add	sp, #-8
; count = n * 16
	ldhl	sp,#11
	ld	l, (hl)
	ld	h, #0x00
	add hl,hl
	add hl,hl
	add hl,hl
	add hl,hl
	inc	sp
	inc	sp
	push	hl
; out 0x9000 + (first  * 16)
	ldhl	sp,#10
    ld l,(hl)
    ld h,#0x00
    add hl,hl
    add hl,hl
    add hl,hl
    add hl,hl
    ld bc, #0x9000
    add hl, bc
	ld	a, l
	ld	d, h
	ldhl	sp,#4
	ld	(hl+), a
	ld	(hl), d
;gamestate.c:397: run = 0U;
	ldhl	sp,#7
	ld	(hl), #0x00
;gamestate.c:399: while(count != 0UL) {
00111$:
	ldhl	sp,#1
	ld	a, (hl-)
	or	a, (hl)
	jp	Z, 00114$
; check out >= 0x9800 ?
    ldhl sp,#5
    ld a, (hl)
    bit 3, a
    jr Z,00110$
    bit 4, a
    jr Z,00110$
    res 4, a
    ld (hl), a
00110$:
; if(run == 0U)
	ldhl	sp,#7
	ld	a, (hl)
	or	a, a
	jp	NZ, 00105$
; value = *data;
	ldhl	sp,#12
	ld	a, (hl+)
	ld	e, (hl)
	ldhl	sp,#2
	ld	(hl+), a
	ld	(hl), e
	dec	hl
	ld	e, (hl)
	inc	hl
	ld	d, (hl)
	ld	a,(de)
	ldhl	sp,#6
	ld	(hl), a
; if(value == data[1])
	ldhl	sp,#(3 - 1)
	ld	c, (hl)
	inc	hl
	ld	b, (hl)
	inc	bc
	ld	a, (bc)
	ld	e, a
	ldhl	sp,#6
	ld	a, (hl)
	sub	a, e
	jr	NZ,00102$
; run = data[2]
	ldhl	sp,#(3 - 1)
	ld	c, (hl)
	inc	hl
	ld	b, (hl)
	inc	bc
	inc	bc
	ld	a, (bc)
	ldhl	sp,#7
	ld	(hl), a
; data += 3U
	ldhl	sp,#(3 - 1)
	ld	e, (hl)
	inc	hl
	ld	d, (hl)
	ld	hl, #0x0003
	add	hl, de
	ld	a, l
	ld	d, h
	ldhl	sp,#12
	ld	(hl+), a
	ld	(hl), d
	jr	00105$
00102$:
;gamestate.c:406: run = 1U;
	ldhl	sp,#7
	ld	(hl), #0x01
;gamestate.c:407: data++;
	ldhl	sp,#12
	ld	(hl), c
	inc	hl
	ld	(hl), b
00105$:
;gamestate.c:410: run--;
	ldhl	sp,#7
	dec	(hl)
; put value in DE
	ldhl	sp,#(5 - 1)
	ld	e, (hl)
	inc	hl
	ld	d, (hl)
	inc	hl
; while(STAT & 2U)
00106$:
	ldh a,(#0xFF41)
	bit	1, a
	jr	NZ,00106$
; *out = value;
	ld	a, (hl)
	ld	(de), a
;gamestate.c:414: ++out;
	dec	hl
	dec	hl
	inc	(hl)
	jr	NZ,00160$
	inc	hl
	inc	(hl)
00160$:
; check out == 0x9800 ?
;    ldhl sp,#5
;    ld a, (hl)
;    sub a, #0x98
;	jr	NZ,00110$
;    ld (hl), #0x88
;    dec hl
;    ld (hl), #0x00
;00110$:
; count--
	pop	de
	push	de
	dec	de
	inc	sp
	inc	sp
	push	de
	jp	00111$
00114$:
	add	sp, #8
	ret
