; void mymemset(void* dest, UBYTE value, UWORD count) {
_mymemset::
    ldhl sp,#2
    ; copy count to bc
    ld c, (hl)
    inc hl
    ld b, (hl)
    ; copy  de to hl
    ld h, d
    ld l, e
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
    pop hl
    pop af
    jp (hl)
