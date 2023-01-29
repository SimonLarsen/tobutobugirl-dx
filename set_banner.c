#include <gb/gb.h>
#include <gb/cgb.h>
#include "set_banner.h"
#include "set_data_rle.h"
#include "gamestate.h"

#include "selection1.h"
#include "selection2.h"
#include "selection3.h"
#include "selection4.h"
#include "selection5.h"
#include "selection5_dx.h"
#include "selection_highscore.h"
#include "selection_highscore_dx.h"
#include "selection_jukebox.h"
#include "selection_jukebox_dx.h"
#include "selection_locked.h"
#include "selection_locked_dx.h"

extern UBYTE *banner_data[];
extern UBYTE *banner_data2[];
extern UBYTE *banner_dx_data[];
extern UBYTE *banner_dx_data2[];
extern UWORD *banner_palette_data[];
extern UBYTE *banner_tiles[];
extern UBYTE *banner_dx_tiles[];
extern UBYTE *banner_palettes[];
extern UBYTE *banner_dx_palettes[];

const UBYTE banner_data_length[] = {
    selection_locked_data_length,
    selection1_data_length,
    selection2_data_length,
    selection3_data_length,
    selection4_data_length,
    selection5_data_length,
    selection_jukebox_data_length,
    selection_highscore_data_length
};

const UBYTE banner_data_length2[] = {
    selection_locked_data_length2,
    selection1_data_length2,
    selection2_data_length2,
    selection3_data_length2,
    selection4_data_length2,
    selection5_data_length2,
    selection_jukebox_data_length2,
    selection_highscore_data_length2
};

const UBYTE banner_dx_data_length[] = {
    selection_locked_dx_data_length,
    selection1_data_length,
    selection2_data_length,
    selection3_data_length,
    selection4_data_length,
    selection5_data_length,
    selection_jukebox_dx_data_length,
    selection_highscore_dx_data_length
};

const UBYTE banner_dx_data_length2[] = {
    selection_locked_dx_data_length2,
    selection1_data_length2,
    selection2_data_length2,
    selection3_data_length2,
    selection4_data_length2,
    selection5_data_length2,
    selection_jukebox_dx_data_length2,
    selection_highscore_dx_data_length2
};

void _selectSetBannerData(UBYTE index, UBYTE part, UBYTE pal_buffer) {
    const UBYTE *data;
    const UWORD *palette_data;
    UBYTE length, offset;

    offset = selection_locked_tiles_offset;

	if(index <= 5U && index > levels_unlocked || index == 6U && levels_completed < 2U) {
        index = 0U;
    }

    if(CGB_MODE) {
        if(part == 2U) {
            data = banner_dx_data2[index];
            length = banner_dx_data_length2[index];
            offset += banner_dx_data_length[index];
        } else {
            data = banner_dx_data[index];
            length = banner_dx_data_length[index];
        }
        palette_data = banner_palette_data[index];

        if(pal_buffer) {
            set_bkg_palette_buffer(selection_locked_dx_palette_offset, 5U, palette_data);
        } else {
            set_bkg_palette(selection_locked_dx_palette_offset, 5U, palette_data);
        }
    } else {
        if(part == 2U) {
            data = banner_data2[index];
            length = banner_data_length2[index];
            offset += banner_data_length[index];
        } else {
            data = banner_data[index];
            length = banner_data_length[index];
        }
    }

    set_bkg_data(offset, length, data);
}

void _selectSetBannerTiles(UBYTE index, UBYTE x, UBYTE y) {
    const UBYTE *tiles;

	if(index <= 5U && index > levels_unlocked || index == 6U && levels_completed < 2U) {
        index = 0U;
    }

    if(CGB_MODE) {
        VBK_REG = 1U;
        set_bkg_tiles(x, y, 16U, 6U, banner_palettes[index]);
        VBK_REG = 0U;

        tiles = banner_dx_tiles[index];
    } else {
        tiles = banner_tiles[index];
    }

    set_bkg_tiles(x, y, 16U, 6U, tiles);
}

void _selectSetBannerColumn(UBYTE index, UBYTE x, UBYTE y) {
    UBYTE i;
    const UBYTE *tiles;
    const UBYTE *palettes;
    UBYTE buf[6];

	if(index <= 5U && index > levels_unlocked || index == 6U && levels_completed < 2U) {
        index = 0U;
    }

    if(CGB_MODE) {
        tiles = banner_dx_tiles[index];
        palettes = banner_palettes[index];
    } else {
        tiles = banner_tiles[index];
    }

    tiles = tiles + x - 2U;
    for(i = 0U; i != 6U; ++i) {
        buf[i] = *tiles;
        tiles += 16U;
    }

    set_bkg_tiles(x, y, 1U, 6U, buf);

    if(CGB_MODE) {
        palettes = palettes + x - 2U;
        for(i = 0U; i != 6U; ++i) {
            buf[i] = *palettes;
            palettes += 16U;
        }
        VBK_REG = 1U;
        set_bkg_tiles(x, y, 1U, 6U, buf);
        VBK_REG = 0U;
    }
}
