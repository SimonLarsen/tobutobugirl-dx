; void mymemset(void* dest, UBYTE value, UWORD count) {
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
    jr 2$
1$:
    ld (hl+), a
2$:
    dec c
    jr nz, 1$
    dec b
    jr nz, 1$
    ret
