_mydiv::
    ld c, #0
1$:
    sub a, e
    jr C, 2$
    inc c
    jr 1$
2$:
    ld a, c
    ret


_mymod::
    ld c, a
1$:
    sub a, e
    jr C, 2$
    ld c, a
    jr 1$
2$:
    ld a, c
    ret
