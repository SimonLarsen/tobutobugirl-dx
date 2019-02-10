#include <gb/gb.h>
#include <gb/cgb.h>
#include "gamestate.h"
#include "sound.h"
#include "fade.h"

void get_bkg_palette(UBYTE i, UBYTE *buffer);

const UBYTE fadePals[] = {
	0xE4U, // 11100100
	0x90U, // 10010000
	0x40U, // 01000000
	0x00U  // 00000000
};

const UBYTE spriteFadePals[] = {
	0xD0U, // 11010000
	0x80U, // 10000000
	0x40U, // 01000000
	0x00U  // 00000000
};

UWORD palette_buffer[32];

#define GET_RED(c) (c & 0x1FU)
#define GET_GREEN(c) ((c >> 5) & 0x1FU)
#define GET_BLUE(c) ((c >> 10) & 0x1FU)

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
	UBYTE i, c, p;
    UBYTE r, g, b;
    UWORD data[4];
    for(i = 0U; i != 15U; ++i) {
        for(p = 0U; p != 8U; ++p) {
            get_bkg_palette(p, data);
            for(c = 0U; c != 4U; ++c) {
                r = GET_RED(data[c]);
                g = GET_GREEN(data[c]);
                b = GET_BLUE(data[c]);

                if(r <= 29U) r += 2U; else r = 31U;
                if(g <= 29U) g += 2U; else g = 31U;
                if(b <= 29U) b += 2U; else b = 31U;
                data[c] = RGB(r, g, b);
            }
            set_bkg_palette(p, 1U, data);
        }

        for(p = 0U; p != delay; ++p) {
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
    UBYTE i, c, p;
    UBYTE r, g, b;
    UWORD v;
    UWORD data[4];

    for(p = 0U; p != 8U; ++p) {
        get_bkg_palette(p, &palette_buffer[p << 2]);
    }

    for(i = 31U; i != 1U; i -= 2U) {
        for(p = 0U; p != 8U; ++p) {
            for(c = 0U; c != 4U; ++c) {
                v = palette_buffer[(p << 2) + c];
                r = GET_RED(v);
                g = GET_GREEN(v);
                b = GET_BLUE(v);

                if(r <= 31U-i) r += i; else r = 31U;
                if(g <= 31U-i) g += i; else g = 31U;
                if(b <= 31U-i) b += i; else b = 31U;
                data[c] = RGB(r, g, b);
            }
            set_bkg_palette(p, 1U, data);
        }

        for(p = 0U; p != delay; ++p) {
            snd_update();
            wait_vbl_done();
        }
    }

    for(p = 0U; p != 8U; ++p) {
        set_bkg_palette(0, 8U, palette_buffer);
    }
}

void fadeSpritesToWhite(UBYTE delay) {
	UINT8 i, j;
	for(i = 1U; i != 4U; ++i) {
		OBP0_REG = fadePals[i];
		for(j = 0U; j != delay; ++j) {
			snd_update();
			wait_vbl_done();
		}
	}
}
