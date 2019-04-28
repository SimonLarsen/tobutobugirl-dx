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

void _selectSetBannerData(UBYTE index, UBYTE part, UBYTE pal_buffer) {
    UBYTE *data;
    UWORD *palette_data;
    UBYTE length, offset;

    offset = selection_locked_tiles_offset;

	if(index <= 5U && index > levels_unlocked || index == 6U && levels_completed < 2U) {
        if(CGB_MODE) {
            if(part == 2) {
                length = selection_locked_dx_data_length2;
                data = selection_locked_dx_data2;
                offset += selection_locked_dx_data_length;
            } else {
                length = selection_locked_dx_data_length;
                data = selection_locked_dx_data;
            }
            palette_data = selection_locked_dx_palette_data;
        } else {
            if(part == 2) {
                length = selection_locked_data_length2;
                data = selection_locked_data2;
                offset += selection_locked_data_length;
            } else {
                length = selection_locked_data_length;
                data = selection_locked_data;
            }
        }
	} else if(index == 1U) {
        if(part == 2) {
            length = selection1_data_length2;
            data = selection1_data2;
            offset += selection1_data_length;
        } else {
            length = selection1_data_length;
            data = selection1_data;
        }
        if(CGB_MODE) palette_data = selection1_palette_data;
	} else if(index == 2U) {
        if(part == 2) {
            length = selection2_data_length2;
            data = selection2_data2;
            offset += selection2_data_length;
        } else {
            length = selection2_data_length;
            data = selection2_data;
        }
        if(CGB_MODE) palette_data = selection2_palette_data;
	} else if(index == 3U) {
        if(part == 2) {
            length = selection3_data_length2;
            data = selection3_data2;
            offset += selection3_data_length;
        } else {
            length = selection3_data_length;
            data = selection3_data;
        }
        if(CGB_MODE) palette_data = selection3_palette_data;
	} else if(index == 4U) {
        if(part == 2) {
		    length = selection4_data_length2;
            data = selection4_data2;
		    offset += selection4_data_length;
        } else {
		    length = selection4_data_length;
            data = selection4_data;
        }
        if(CGB_MODE) palette_data = selection4_palette_data;
	} else if(index == 5U) {
        if(CGB_MODE) {
            if(part == 2) {
                length = selection5_data_length2;
                data = selection5_dx_data2;
                offset += selection5_dx_data_length;
            } else {
                length = selection5_dx_data_length;
                data = selection5_dx_data;
            }
            palette_data = selection5_dx_palette_data;
        } else {
            if(part == 2) {
                length = selection5_data_length2;
                data = selection5_data2;
                offset += selection5_data_length;
            } else {
                length = selection5_data_length;
                data = selection5_data;
            }
        }
	} else if(index == 6U) {
        if(CGB_MODE) {
            if(part == 2) {
                length = selection_jukebox_dx_data_length2;
                data = selection_jukebox_dx_data2;
                offset += selection_jukebox_dx_data_length;
            } else {
                length = selection_jukebox_dx_data_length;
                data = selection_jukebox_dx_data;
            }
            palette_data = selection_jukebox_dx_palette_data;
        } else {
            if(part == 2) {
                length = selection_jukebox_data_length2;
                data = selection_jukebox_data2;
                offset += selection_jukebox_data_length;
            } else {
                length = selection_jukebox_data_length;
                data = selection_jukebox_data;
            }
        }
	} else if(index == 7U) {
        if(CGB_MODE) {
            if(part == 2) {
                length = selection_highscore_dx_data_length2;
                data = selection_highscore_dx_data2;
                offset += selection_highscore_dx_data_length;
            } else {
                length = selection_highscore_dx_data_length;
                data = selection_highscore_dx_data;
            }
            palette_data = selection_highscore_dx_palette_data;
        } else {
            if(part == 2) {
                length = selection_highscore_data_length2;
                data = selection_highscore_data2;
                offset += selection_highscore_data_length;
            } else {
                length = selection_highscore_data_length;
                data = selection_highscore_data;
            }
        }
	}

    set_bkg_data_rle(offset, length, data);

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
	if(index <= 5U && index > levels_unlocked || index == 6U && levels_completed < 2U) {
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
            tiles = selection5_dx_tiles;
            palettes = selection5_dx_palettes;
        } else {
            tiles = selection5_tiles;
        }
	} else if(index == 6U) {
        if(CGB_MODE) {
		    tiles = selection_jukebox_dx_tiles;
		    palettes = selection_jukebox_dx_palettes;
        } else {
		    tiles = selection_jukebox_tiles;
        }
	} else if(index == 7U) {
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

	if(index <= 5U && index > levels_unlocked || index == 6U && levels_completed < 2U) {
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
            ptr = selection5_dx_tiles;
            palette_ptr = selection5_dx_palettes;
        } else {
            ptr = selection5_tiles;
        }
	} else if(index == 6U) {
        if(CGB_MODE) {
            ptr = selection_jukebox_dx_tiles;
            palette_ptr = selection_jukebox_dx_palettes;
        } else {
            ptr = selection_jukebox_tiles;
        }
	} else if(index == 7U) {
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
