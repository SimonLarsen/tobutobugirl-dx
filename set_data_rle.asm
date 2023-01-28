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
	ldhl	sp,#7
	ld	(hl), #0x00
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
; if run == 0U
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
; if value == data[1]
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
	ldhl	sp,#7
	ld	(hl), #0x01
; data++;
	ldhl	sp,#12
	ld	(hl), c
	inc	hl
	ld	(hl), b
00105$:
; run--;
	ldhl	sp,#7
	dec	(hl)
; put value in DE
	ldhl	sp,#(5 - 1)
	ld	e, (hl)
	inc	hl
	ld	d, (hl)
	inc	hl
; while STAT & 2U
00106$:
	ldh a,(#0xFF41)
	bit	1, a
	jr	NZ,00106$
; *out = value;
	ld	a, (hl)
	ld	(de), a
; ++out;
	dec	hl
	dec	hl
	inc	(hl)
	jr	NZ,00160$
	inc	hl
	inc	(hl)
00160$:
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

__set_tiles_rle::
	add	sp, #-9
;gamestate.c:359: if(is_bkg && (LCDC_REG & 8U) || !is_bkg && (LCDC_REG & 64U)) {
	ldhl	sp,#17
	ld	a, (hl)
	or	a, a
	jr	Z,00106$
	;ld	de, #0xff40
	;ld	a,(de)
    ld a, (#0xff40)
	bit	3, a
	jr	NZ,00101$
00106$:
	ldhl	sp,#17
	ld	a, (hl)
	or	a, a
	jr	NZ,00102$
	ld	de, #0xff40
	ld	a,(de)
	bit	6, a
	jr	Z,00102$
00101$:
;gamestate.c:360: out = (UBYTE*)0x9C00UL;
	ldhl	sp,#4
	ld	(hl), #0x00
	inc	hl
	ld	(hl), #0x9c
	jr	00103$
00102$:
;gamestate.c:362: out = (UBYTE*)0x9800UL;
	ldhl	sp,#4
	ld	(hl), #0x00
	inc	hl
	ld	(hl), #0x98
00103$:
;gamestate.c:365: out += (y << 5) + x;
	ldhl	sp,#12
; (y << 5) == ((y << 8) >> 3)
	ld	b, (hl)
	xor	a, a
.rept 3
	sra	b
	rra
.endm
	ld	c, a
	dec	hl
	ld	a, (hl)
	ldhl	sp,#2
	ld	(hl+), a
	ld	(hl), #0x00
	dec	hl
	ld	a, (hl+)
	ld	h, (hl)
	ld	l, a
	add	hl, bc
	ld	c, l
	ld	b, h
	ldhl	sp,#4
	ld	a, (hl+)
	ld	h, (hl)
	ld	l, a
	add	hl, bc
	ld	c, l
	ld	b, h
;gamestate.c:367: y = 32U - width;
	ld	a, #0x20
	ldhl	sp,#13
	sub	a, (hl)
	ldhl	sp,#2
	ld	(hl), a
;gamestate.c:368: run = 0U;
	ldhl	sp,#6
	ld	(hl), #0x00
;gamestate.c:369: for(j = 0U; j != height; ++j) {
	inc	hl
	ld	(hl), #0x00
00121$:
	ldhl	sp,#14
	ld	a, (hl)
	ldhl	sp,#7
	sub	a, (hl)
	jp	Z,00123$
;gamestate.c:370: for(i = 0U; i != width; ++i) {
	ldhl	sp,#4
	ld	(hl), c
	inc	hl
	ld	(hl), b
	ldhl	sp,#8
	ld	(hl), #0x00
00118$:
	ldhl	sp,#13
	ld	a, (hl)
	ldhl	sp,#8
	sub	a, (hl)
	jp	Z,00115$
;gamestate.c:371: if(!run) {
	ldhl	sp,#6
	ld	a, (hl)
	or	a, a
	jp	NZ, 00112$
;gamestate.c:372: x = *tiles;
	ldhl	sp,#15
	ld	a, (hl+)
	ld	e, (hl)
	ldhl	sp,#0
	ld	(hl+), a
	ld	(hl), e
	pop	de
	push	de
	ld	a,(de)
	ldhl	sp,#11
	ld	(hl), a
;gamestate.c:373: if(*tiles == tiles[1]) {
	pop	bc
	push	bc
	inc	bc
	push	af
	ld	a, (bc)
	ld	e, a
	pop	af
	sub	a, e
	jr	NZ,00108$
;gamestate.c:374: run = tiles[2];
	pop	bc
	push	bc
	inc	bc
	inc	bc
	ld	a, (bc)
	ldhl	sp,#6
	ld	(hl), a
;gamestate.c:375: tiles += 3U;
	pop	de
	push	de
	ld	hl, #0x0003
	add	hl, de
	ld	a, l
	ld	d, h
	ldhl	sp,#15
	ld	(hl+), a
	ld	(hl), d
	jr	00112$
00108$:
;gamestate.c:377: run = 1U;
	ldhl	sp,#6
	ld	(hl), #0x01
;gamestate.c:378: ++tiles;
	ldhl	sp,#15
	ld	(hl), c
	inc	hl
	ld	(hl), b
;gamestate.c:381: while(STAT_REG & 2U) { }
00112$:
	ld	a, (#0xff41)
	bit	1, a
	jr	NZ,00112$
;gamestate.c:382: *out = x;
	ldhl	sp,#(5 - 1)
	ld	e, (hl)
	inc	hl
	ld	d, (hl)
	ldhl	sp,#11
	ld	a, (hl)
	ld	(de), a
;gamestate.c:383: ++out;
	ldhl	sp,#4
	inc	(hl)
	jr	NZ,00188$
	inc	hl
	inc	(hl)
00188$:
;gamestate.c:384: --run;
	ldhl	sp,#6
	dec	(hl)
;gamestate.c:370: for(i = 0U; i != width; ++i) {
	inc	hl
	inc	hl
	inc	(hl)
	jp	00118$
00115$:
;gamestate.c:387: out += y;
	ldhl	sp,#(5 - 1)
	ld	e, (hl)
	inc	hl
	ld	d, (hl)
	ldhl	sp,#2
	ld	l, (hl)
	ld	h, #0x00
	add	hl, de
	ld	c, l
	ld	b, h
;gamestate.c:369: for(j = 0U; j != height; ++j) {
	ldhl	sp,#7
	inc	(hl)
	jp	00121$
00123$:
;gamestate.c:389: }
	add	sp, #9
	ret
