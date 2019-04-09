#include <gb/gb.h>
#include <gb/cgb.h>
#include "set_banner.h"
#include "set_data_rle.h"
#include "gamestate.h"

#include "selection1.h"
#include "selection2.h"
#include "selection3.h"
#include "selection4.h"
#include "selection_highscore.h"
#include "selection_highscore_dx.h"
#include "selection_jukebox.h"
#include "selection_jukebox_dx.h"
#include "selection_locked.h"
#include "selection_locked_dx.h"

void _selectSetBannerData(UBYTE index, UBYTE pal_buffer) {
    UBYTE *data;
    UWORD *palette_data;
    UBYTE length;

	if(index <= 4U && index > levels_completed+1U) {
        if(CGB_MODE) {
            length = selection_locked_dx_data_length;
            data = selection_locked_dx_data;
            palette_data = selection_locked_dx_palette_data;
        } else {
            length = selection_locked_data_length;
            data = selection_locked_data;
        }
	} else if(index == 1U) {
		length = selection1_data_length;
        data = selection1_data;
        if(CGB_MODE) palette_data = selection1_palette_data;
	} else if(index == 2U) {
		length = selection2_data_length;
        data = selection2_data;
        if(CGB_MODE) palette_data = selection2_palette_data;
	} else if(index == 3U) {
		length = selection3_data_length;
        data = selection3_data;
        if(CGB_MODE) palette_data = selection3_palette_data;
	} else if(index == 4U) {
		length = selection4_data_length;
        data = selection4_data;
        if(CGB_MODE) palette_data = selection4_palette_data;
	} else if(index == 5U) {
        if(CGB_MODE) {
            length = selection_jukebox_dx_data_length;
            data = selection_jukebox_dx_data;
            palette_data = selection_jukebox_dx_palette_data;
        } else {
            length = selection_jukebox_data_length;
            data = selection_jukebox_data;
        }
	} else if(index == 6U) {
        if(CGB_MODE) {
            length = selection_highscore_dx_data_length;
            data = selection_highscore_dx_data;
            palette_data = selection_highscore_dx_palette_data;
        } else {
            length = selection_highscore_data_length;
            data = selection_highscore_data;
        }
	}

    set_bkg_data_rle(selection_locked_tiles_offset, length, data);
    if(CGB_MODE) {
        if(pal_buffer) {
            set_bkg_palette_buffer(selection_locked_dx_palette_offset, 5U, palette_data);
        } else {
            set_bkg_palette(selection_locked_dx_palette_offset, 5U, palette_data);
        }
    }
}

void _selectSetBannerTiles(UBYTE index, UBYTE x, UBYTE y) {
    UBYTE *tiles;
    UBYTE *palettes;
	if(index <= 4U && index > levels_completed+1U) {
        if(CGB_MODE) {
		    tiles = selection_locked_dx_tiles;
            palettes = selection_locked_dx_palettes;
        } else {
		    tiles = selection_locked_tiles;
        }
	} else if(index == 1U) {
		tiles = selection1_tiles;
        if(CGB_MODE) palettes = selection1_palettes;
	} else if(index == 2U) {
		tiles = selection2_tiles;
		if(CGB_MODE) palettes = selection2_palettes;
	} else if(index == 3U) {
		tiles = selection3_tiles;
		if(CGB_MODE) palettes = selection3_palettes;
	} else if(index == 4U) {
		tiles = selection4_tiles;
		if(CGB_MODE) palettes = selection4_palettes;
	} else if(index == 5U) {
        if(CGB_MODE) {
		    tiles = selection_jukebox_dx_tiles;
		    palettes = selection_jukebox_dx_palettes;
        } else {
		    tiles = selection_jukebox_tiles;
        }
	} else if(index == 6U) {
        if(CGB_MODE) {
		    tiles = selection_highscore_dx_tiles;
		    palettes = selection_highscore_dx_palettes;
        } else {
		    tiles = selection_highscore_tiles;
        }
	}

    set_bkg_tiles(x, y, 16U, 6U, tiles);
    if(CGB_MODE) {
        VBK_REG = 1U;
        set_bkg_tiles(x, y, 16U, 6U, palettes);
        VBK_REG = 0U;
    }
}

void _selectSetBannerColumn(UBYTE index, UBYTE x, UBYTE y) {
    UBYTE i;
    UBYTE *ptr;
    UBYTE *palette_ptr;
    UBYTE tiles[6];

	if(index <= 4U && index > levels_completed+1U) {
        if(CGB_MODE) {
            ptr = selection_locked_dx_tiles;
            palette_ptr = selection_locked_dx_palettes;
        } else {
            ptr = selection_locked_tiles;
        }
	} else if(index == 1U) {
		ptr = selection1_tiles;
        if(CGB_MODE) palette_ptr = selection1_palettes;
	} else if(index == 2U) {
		ptr = selection2_tiles;
        if(CGB_MODE) palette_ptr = selection2_palettes;
	} else if(index == 3U) {
		ptr = selection3_tiles;
        if(CGB_MODE) palette_ptr = selection3_palettes;
	} else if(index == 4U) {
		ptr = selection4_tiles;
        if(CGB_MODE) palette_ptr = selection4_palettes;
	} else if(index == 5U) {
        if(CGB_MODE) {
            ptr = selection_jukebox_dx_tiles;
            palette_ptr = selection_jukebox_dx_palettes;
        } else {
            ptr = selection_jukebox_tiles;
        }
	} else if(index == 6U) {
		if(CGB_MODE) {
            ptr = selection_highscore_dx_tiles;
            palette_ptr = selection_highscore_dx_palettes;
        } else {
            ptr = selection_highscore_tiles;
        }
	}

    ptr = ptr + x - 2U;
    for(i = 0U; i != 6U; ++i) {
        tiles[i] = *ptr;
        ptr += 16U;
    }

    set_bkg_tiles(x, y, 1U, 6U, tiles);

    if(CGB_MODE) {
        palette_ptr = palette_ptr + x - 2U;
        for(i = 0U; i != 6U; ++i) {
            tiles[i] = *palette_ptr;
            palette_ptr += 16U;
        }
        VBK_REG = 1U;
        set_bkg_tiles(x, y, 1U, 6U, tiles);
        VBK_REG = 0U;
    }
}
