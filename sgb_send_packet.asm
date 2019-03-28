_sgb_send_packet::
	dec	sp
; P1_REG = 0U;
	ld	hl, #0xff00
	ld	(hl), #0x00
; P1_REG = SGB_P1_NEUTRAL;
	ld	(hl), #0x30
;gamestate.c:419: for(i = 0U; i != 16U; ++i) {
	ldhl	sp,#0
	ld	(hl), #0x00
00108$:
;gamestate.c:420: val = data[i];
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
;gamestate.c:421: for(b = 0U; b != 8U; ++b) {
	ld	b, #0x00
00106$:
;gamestate.c:422: if(val & 1U) {
	bit	0, c
	jr	Z,00102$
;gamestate.c:423: P1_REG = SGB_P1_HIGH;
	ld	hl, #0xff00
	ld	(hl), #0x10
	jr	00103$
00102$:
;gamestate.c:425: P1_REG = SGB_P1_LOW;
	ld	hl, #0xff00
	ld	(hl), #0x20
00103$:
;gamestate.c:427: P1_REG = SGB_P1_NEUTRAL;
	ld	hl, #0xff00
	ld	(hl), #0x30
;gamestate.c:428: val = val >> 1;
	srl	c
;gamestate.c:421: for(b = 0U; b != 8U; ++b) {
	inc	b
	ld	a, b
	sub	a, #0x08
	jr	NZ,00106$
;gamestate.c:419: for(i = 0U; i != 16U; ++i) {
	ldhl	sp,#0
	inc	(hl)
	ld	a, (hl)
	sub	a, #0x10
	jp	NZ,00108$
; P1_REG = SGB_P1_LOW;
	ld	hl, #0xff00
	ld	(hl), #0x20
; P1_REG = SGB_P1_NEUTRAL;
	ld	(hl), #0x30
	inc	sp
	ret

;_sgb_send_packet::
;; P1_REG = 0U;
;	ld	hl, #0xff00
;	ld	(hl), #0x00
;; P1_REG = SGB_P1_NEUTRAL;
;	ld	(hl), #0x30
;; de = &data
;    ldhl sp,#2
;    ld e,(hl)
;    inc hl
;    ld d,(hl)
;    ld h, d
;    ld l, e
;    ld b, #16
;.sgb_send_packet2_loop1:
;    ld a, (hl+)
;    ld d, a
;    ld c, #8
;.sgb_send_packet2_loop2:
;    ld a, #0x20
;    bit 0, d
;    jr z,.sgb_send_packet2_low
;    ld a, #0x10
;.sgb_send_packet2_low:
;    ld (#0xff00), a
;    rr d ; d = d >> 1
;    dec c
;    jr nz,.sgb_send_packet2_loop2
;    dec b
;    jr nz,.sgb_send_packet2_loop1
;    ld hl, #0xff00
;    ld (hl), #0x020
;    ld (hl), #0x030
;    ret
