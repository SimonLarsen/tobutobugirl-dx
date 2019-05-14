; void mymemset(const UBYTE* dest, UBYTE value, UWORD count) {
_mymemset::
    ; copy dest to de
    ldhl sp,#2
    ld e, (hl)
    inc hl
    ld d, (hl)
    inc hl
    ; copy value to a
    ld a, (hl+)
    ; copy count to bc
    ld c, (hl)
    inc hl
    ld b, (hl)
    ; copy  de to hl
    ld h, d
    ld l, e
    ;ld hl, de
    ; copy bytes
    inc c
    inc b
    jr 00102$
00101$:
    ld (hl+), a
00102$:
    dec c
    jr nz, 00101$
    dec b
    jr nz, 00101$
    ret
