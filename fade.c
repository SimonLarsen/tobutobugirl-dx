#include <gb/gb.h>
#include <gb/cgb.h>
#include "defines.h"
#include "gamestate.h"
#include "sound.h"
#include "fade.h"

const UBYTE spriteFadePals[] = {
    0xD0U, // 11010000
    0x80U, // 10000000
    0x40U, // 01000000
    0x00U  // 00000000
};

void fadeToWhite(UBYTE delay) {
    if(CGB_MODE) fadeToWhiteCGB(delay >> 2);
    else fadeToWhiteGB(delay);
}

void fadeToWhiteGB(UBYTE delay) {
    UINT8 i, j;
    for(i = 1U; i != 4U; ++i) {
        BGP_REG = spriteFadePals[i];
        for(j = 0U; j != delay; ++j) {
            snd_update();
            wait_vbl_done();
        }
    }
}

void fadeToWhiteCGB(UBYTE delay) {
    UBYTE i, c;
    UBYTE r, g, b;
    UWORD v;

    get_bkg_palette(palette_buffer);

    for(i = 0U; i != 15U; ++i) {
        for(c = 0U; c != 32U; ++c) {
            v = palette_buffer[c];
            r = GET_RED(v);
            g = GET_GREEN(v);
            b = GET_BLUE(v);

            if(r <= 29U) r += 2U; else r = 31U;
            if(g <= 29U) g += 2U; else g = 31U;
            if(b <= 29U) b += 2U; else b = 31U;
            palette_buffer[c] = RGB(r, g, b);
        }
        set_bkg_palette(0, 8U, palette_buffer);

        for(c = 0U; c != delay; ++c) {
            snd_update();
            wait_vbl_done();
        }
    }
}

void fadeFromWhite(UBYTE delay) {
    if(CGB_MODE) fadeFromWhiteCGB(delay >> 2);
    else fadeFromWhiteGB(delay);
}

void fadeFromWhiteGB(UBYTE delay) {
    UINT8 i, j;

    for(i = 3U; i != 0U; --i) {
        BGP_REG = fadePals[i];
        for(j = 0U; j != delay; ++j) {
            snd_update();
            wait_vbl_done();
        }
    }

    BGP_REG = fadePals[0U];
}

void fadeFromWhiteCGB(UBYTE delay) {
    UBYTE i, c;
    UBYTE r, g, b;
    UWORD v;

    for(i = 31U; i != 1U; i -= 2U) {
        for(c = 0U; c != 32U; ++c) {
            v = palette_buffer[c];
            r = GET_RED(v);
            g = GET_GREEN(v);
            b = GET_BLUE(v);

            if(r <= 31U-i) r += i; else r = 31U;
            if(g <= 31U-i) g += i; else g = 31U;
            if(b <= 31U-i) b += i; else b = 31U;
            palette_buffer2[c] = RGB(r, g, b);
        }
        set_bkg_palette(0U, 8U, palette_buffer2);

        for(c = 0U; c != delay; ++c) {
            snd_update();
            wait_vbl_done();
        }
    }

    for(c = 0U; c != 8U; ++c) {
        set_bkg_palette(0, 8U, palette_buffer);
    }
}
