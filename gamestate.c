#include <gb/gb.h>
#include <gb/cgb.h>
#include "defines.h"
#include "gamestate.h"
#include "set_banner.h"
#include "set_data_rle.h"
#include "background1.h"
#include "background1_dx.h"
#include "background2.h"
#include "background3.h"
#include "background4.h"
#include "background5.h"
#include "pause_bg.h"
#include "win1.h"
#include "win1_dx.h"
#include "win2.h"
#include "win2_dx.h"
#include "win3.h"
#include "win3_dx.h"
#include "win4.h"
#include "win4_dx.h"
#include "win5.h"
#include "win5_dx.h"
#include "pause_cloud1.h"
#include "pause_cloud2.h"
#include "hud.h"
#include "hud_dx.h"
#include "hud_sgb.h"
#include "mmlgb/driver/music.h"

UBYTE sgb_mode;
UBYTE vbl_count;
UBYTE ticks, paused, timer;
UBYTE gamestate, scene_state;
UBYTE level, levels_unlocked, levels_completed;
UBYTE selection, sub_selection;
UBYTE joystate, oldjoystate;
UBYTE next_sprite, sprites_used;
UBYTE remaining_time, elapsed_minutes, elapsed_seconds;
UWORD elapsed_time, kills, wave;
UBYTE last_highscore_level, last_highscore_slot;
UBYTE last_progress;
UBYTE game_bank, music_bank;
UBYTE ending_flags;
UBYTE unlocked_bits;
UBYTE scroll_x, scroll_y;
UBYTE player_x, player_y;
UBYTE player_xdir, player_ydir;
UBYTE player_xspeed, player_yspeed, player_bounce;
UBYTE cat_x, cat_y, cat_frame;
UBYTE arrow_offset1, arrow_offset2;
UBYTE player_skin;
UBYTE show_dashcounter;
UBYTE circle_index;
UBYTE next_spawn, next_clock;
UBYTE next_entity;

UWORD palette_buffer[32];
UWORD palette_buffer2[32];

UBYTE entity_x[MAX_ENTITIES];
UBYTE entity_y[MAX_ENTITIES];
UBYTE entity_type[MAX_ENTITIES];
UBYTE entity_dir[MAX_ENTITIES];

const UBYTE fadePals[] = {
    0xE4U, // 11100100
    0x90U, // 10010000
    0x40U, // 01000000
    0x00U  // 00000000
};

const UBYTE level_names[8][6] = {
    {22U, 25U, 13U, 21U, 15U, 14U}, // "LOCKED"
    {26U, 22U, 11U, 19U, 24U, 29U}, // "PLAINS"
    {13U, 22U, 25U, 31U, 14U, 29U}, // "CLOUDS"
    {29U, 26U, 11U, 13U, 15U, 10U}, // "SPACE "
    {14U, 28U, 15U, 11U, 23U, 10U}, // "DREAM "
    {18U, 15U, 11U, 32U, 15U, 24U}, // "HEAVEN"
    {23U, 31U, 29U, 19U, 13U, 10U}, // "MUSIC "
    {29U, 13U, 25U, 28U, 15U, 29U}  // "SCORES"
};

const UBYTE level_max_time[4] = { 56U, 56U, 64U, 64U };

const UBYTE level_tiers[4][4] = {
    {125U, 115U, 90U, 0U},
    {110U,  95U, 80U, 0U},
    {100U,  90U, 80U, 0U},
    { 85U,  80U, 70U, 0U}
};

const UBYTE rank_letters[4] = {29U, 11U, 12U, 13U};

void _setSprite(UBYTE y, UBYTE x, UBYTE tile, UBYTE prop) {
    UBYTE* oam = (UBYTE*)0xC000UL + (UBYTE)(next_sprite << 2);
    *oam++ = y;
    *oam++ = x;
    *oam++ = tile;
    *oam = prop;

    sprites_used++;
    next_sprite++;
    if(next_sprite == 40U) next_sprite = 0U;
}

void clearRemainingSprites() {
    UBYTE* oam = (UBYTE*)0xC000UL + (next_sprite << 2);
    for(; next_sprite != 40U; ++next_sprite) {
        *oam = 0U;
        oam += 4UL;
    }
    next_sprite = 0U;
    sprites_used = 0U;
}

UWORD mydiv16(UWORD num, UWORD denom) {
    UWORD cnt = 0UL;
    while(num >= denom) {
        ++cnt;
        num -= denom;
    }
    return cnt;
}

UWORD mymod16(UWORD num, WORD denom) {
    while(num >= denom) {
        num -= denom;
    }
    return num;
}

/*
void drawNumber8(UBYTE x, UBYTE y, UBYTE value) {
    UBYTE tile;
    if(value) {
        while(value) {
            tile = mymod(value, 10U);
            set_bkg_tiles(x, y, 1U, 1U, &tile);
            value = mydiv(value, 10U);
            --x;
        }
    } else {
        tile = 0U;
        set_bkg_tiles(x, y, 1U, 1U, &tile);
    }
}
*/

void drawNumber16(UBYTE x, UBYTE y, UWORD value) {
    UBYTE tile;
    if(value) {
        while(value) {
            tile = (UBYTE)mymod16(value, 10U);
            set_bkg_tiles(x, y, 1U, 1U, &tile);
            value = mydiv16(value, 10U);
            --x;
        }
    } else {
        tile = 0U;
        set_bkg_tiles(x, y, 1U, 1U, &tile);
    }
}

void drawScore8(UBYTE x, UBYTE y, UBYTE value) {
	UBYTE tile;

	tile = 0U;
	set_bkg_tiles(x--, y, 1U, 1U, &tile);

    if(value) {
        set_bkg_tiles(x--, y, 1U, 1U, &tile);
        drawNumber8(x, y, value);
    }
}

void drawTime8(UBYTE x, UBYTE y, UBYTE secs) {
    UBYTE tile;

    x -= 3U;
    // minutes
	tile = mydiv(secs, 60U);
	set_bkg_tiles(x++, y, 1U, 1U, &tile);
    // colon
	tile = 37U;
	set_bkg_tiles(x++, y, 1U, 1U, &tile);
    // seconds
    secs = mymod(secs, 60U);
	tile = mydiv(secs, 10U);
	set_bkg_tiles(x++, y, 1U, 1U, &tile);
    tile = mymod(secs, 10U);
	set_bkg_tiles(x++, y, 1U, 1U, &tile);
}

void drawTime16(UBYTE x, UBYTE y, UWORD secs) {
    UBYTE tile, min;

    if(secs >= 35999UL) secs = 35999UL;

    x -= 6U;

    // hours (1 digit)
    tile = mydiv16(secs, 3600UL);
	set_bkg_tiles(x++, y, 1U, 1U, &tile);
    secs = mymod16(secs, 3600UL);

    // colon
    tile = 37U;
	set_bkg_tiles(x++, y, 1U, 1U, &tile);

    // minutes
    min = (UBYTE)mydiv16(secs, 60UL);
    secs = mymod16(secs, 60U);
    tile = mydiv(min, 10U);
	set_bkg_tiles(x++, y, 1U, 1U, &tile);
    tile = mymod(min, 10U);
	set_bkg_tiles(x++, y, 1U, 1U, &tile);

    // colon
    tile = 37U;
	set_bkg_tiles(x++, y, 1U, 1U, &tile);

    // seconds
    tile = mydiv(secs, 10U);
	set_bkg_tiles(x++, y, 1U, 1U, &tile);
    tile = mymod(secs, 10U);
	set_bkg_tiles(x++, y, 1U, 1U, &tile);
}

UBYTE getRank(UBYTE score, UBYTE level) {
    UBYTE i;
    UBYTE *data;
    data = level_tiers[level-1U];
    for(i = 0U; i != 4U; ++i) {
        if(score >= *(data++)) break;
    }
    return i;
}

void playMusic(UBYTE *data) {
    SWITCH_ROM_MBC1(music_bank);
    mus_init(data);
    SWITCH_ROM_MBC1(game_bank);
}

void updateMusic() {
    SWITCH_ROM_MBC1(music_bank);
    mus_update();
    SWITCH_ROM_MBC1(game_bank);
}


void setIngameBackground(UBYTE level, UBYTE first_load, UBYTE pal_buffer) {
    const UBYTE *tile_data, *tiles, *palette_data, *palettes;
    UBYTE data_length, palette_data_length;

    if(level == 255U) {
        SWITCH_ROM_MBC1(PAUSE_DATA_BANK);
    } else {
        SWITCH_ROM_MBC1(GAME_BACKGROUNDS_BANK);
    }

    switch(level) {
        case 1U:
            if(CGB_MODE) {
                tile_data = background1_dx_data;
                tiles = background1_dx_tiles;
                data_length = background1_dx_data_length;
                palette_data = background1_dx_palette_data;
                palettes = background1_dx_palettes;
                palette_data_length = background1_dx_palette_data_length;
            } else {
                tile_data = background1_data;
                tiles = background1_tiles;
                data_length = background1_data_length;
            }
            break;
        case 2U:
            tile_data = background2_data;
            tiles = background2_tiles;
            data_length = background2_data_length;
            if(CGB_MODE) {
                palette_data = background2_palette_data;
                palettes = background2_palettes;
                palette_data_length = background2_palette_data_length;
            }
            break;
        case 3U:
            tile_data = background3_data;
            tiles = background3_tiles;
            data_length = background3_data_length;
            if(CGB_MODE) {
                palette_data = background3_palette_data;
                palettes = background3_palettes;
                palette_data_length = background3_palette_data_length;
            }
            break;
        case 4U:
            tile_data = background4_data;
            tiles = background4_tiles;
            data_length = background4_data_length;
            if(CGB_MODE) {
                palette_data = background4_palette_data;
                palettes = background4_palettes;
                palette_data_length = background4_palette_data_length;
            }
            break;
        case 5U:
            tile_data = background5_data;
            tiles = background5_tiles;
            data_length = background5_data_length;
            if(CGB_MODE) {
                palette_data = background5_palette_data;
                palettes = background5_palettes;
                palette_data_length = background5_palette_data_length;
            }
            break;
        case 255U:
            tile_data = pause_bg_data;
            tiles = pause_bg_tiles;
            data_length = pause_bg_data_length;
            if(CGB_MODE) {
                palette_data = pause_bg_palette_data;
                palettes = pause_bg_palettes;
                palette_data_length = pause_bg_palette_data_length;
            }
            break;
    }

    if(first_load) {
        set_bkg_data_rle(background1_dx_tiles_offset, data_length, tile_data);
    }

    if(level == 255U) {
        set_bkg_tiles_rle(0U, 0U, 20U, 18U, tiles);
    } else {
        set_bkg_tiles_rle(0U, 0U, 18U, 32U, tiles);
    }

    if(CGB_MODE) {
        VBK_REG = 1U;
        if(level == 255U) {
            set_bkg_tiles_rle(0U, 0U, 20U, 18U, palettes);
        } else {
            set_bkg_tiles_rle(0U, 0U, 18U, 32U, palettes);
        }
        VBK_REG = 0U;

        if(pal_buffer) {
            set_bkg_palette_buffer(0U, palette_data_length, palette_data);
        } else {
            set_bkg_palette(0U, palette_data_length, palette_data);
        }
    }

    SWITCH_ROM_MBC1(game_bank);
}

void setIngameHUD(UBYTE first_load, UBYTE pal_buffer) {
    SWITCH_ROM_MBC1(GAME_BACKGROUNDS_BANK);

    // Load tile data
    if(first_load) {
        if(CGB_MODE) {
            set_bkg_data_rle(hud_dx_tiles_offset, hud_dx_data_length, hud_dx_data);
        } else if(sgb_mode) {
            set_bkg_data_rle(hud_sgb_tiles_offset, hud_sgb_data_length, hud_sgb_data);
        } else {
            set_bkg_data_rle(hud_tiles_offset, hud_data_length, hud_data);
        }
    }

    if(CGB_MODE) {
        if(pal_buffer) {
            set_bkg_palette_buffer(hud_dx_palette_offset, hud_dx_palette_data_length, hud_dx_palette_data);
        } else {
            set_bkg_palette(hud_dx_palette_offset, hud_dx_palette_data_length, hud_dx_palette_data);
        }
        set_win_tiles_rle(0U, 0U, hud_dx_tiles_width, hud_dx_tiles_height, hud_dx_tiles);

        VBK_REG = 1U;
        set_win_tiles_rle(0U, 0U, hud_dx_tiles_width, hud_dx_tiles_height, hud_dx_palettes);
        // fix clock palettes
        *((UBYTE*)0x9C20) = 7U;
        *((UBYTE*)0x9C21) = 7U;
        *((UBYTE*)0x9C40) = 7U;
        *((UBYTE*)0x9C41) = 7U;
        VBK_REG = 0U;
    } else if(sgb_mode) {
        set_win_tiles_rle(0U, 0U, hud_sgb_tiles_width, hud_sgb_tiles_height, hud_sgb_tiles);
    } else {
        set_win_tiles_rle(0U, 0U, hud_tiles_width, hud_tiles_height, hud_tiles);
    }

    SWITCH_ROM_MBC1(game_bank);
}

void setWinscreenBackground(UBYTE level) {
    const UBYTE *tile_data, *tiles, *palettes;
    const UWORD *palette_data;
    UBYTE data_length, palette_data_length;
    SWITCH_ROM_MBC1(WINSCREEN_BACKGROUNDS_BANK);

    switch(level) {
        case 1U:
            if(CGB_MODE) {
                tile_data = win1_dx_data;
                data_length = win1_dx_data_length;
                tiles = win1_dx_tiles;
                palette_data_length = win1_dx_palette_data_length;
                palette_data = win1_dx_palette_data;
                palettes = win1_dx_palettes;
            } else {
                tile_data = win1_data;
                data_length = win1_data_length;
                tiles = win1_tiles;
            }
            break;
        case 2U:
            if(CGB_MODE) {
                tile_data = win2_dx_data;
                data_length = win2_dx_data_length;
                tiles = win2_dx_tiles;
                palette_data_length = win2_dx_palette_data_length;
                palette_data = win2_dx_palette_data;
                palettes = win2_dx_palettes;
            } else {
                tile_data = win2_data;
                data_length = win2_data_length;
                tiles = win2_tiles;
            }
            break;
        case 3U:
            if(CGB_MODE) {
                tile_data = win3_dx_data;
                data_length = win3_dx_data_length;
                tiles = win3_dx_tiles;
                palette_data_length = win3_dx_palette_data_length;
                palette_data = win3_dx_palette_data;
                palettes = win3_dx_palettes;
            } else {
                tile_data = win3_data;
                data_length = win3_data_length;
                tiles = win3_tiles;
            }
            break;
        case 4U:
            if(CGB_MODE) {
                tile_data = win4_dx_data;
                data_length = win4_dx_data_length;
                tiles = win4_dx_tiles;
                palette_data_length = win4_dx_palette_data_length;
                palette_data = win4_dx_palette_data;
                palettes = win4_dx_palettes;
            } else {
                tile_data = win4_data;
                data_length = win4_data_length;
                tiles = win4_tiles;
            }
            break;
        case 5U:
            if(CGB_MODE) {
                tile_data = win5_dx_data;
                data_length = win5_dx_data_length;
                tiles = win5_dx_tiles;
                palette_data_length = win5_dx_palette_data_length;
                palette_data = win5_dx_palette_data;
                palettes = win5_dx_palettes;
            } else {
                tile_data = win5_data;
                data_length = win5_data_length;
                tiles = win5_tiles;
            }
    }

    set_bkg_data_rle(win1_tiles_offset, data_length, tile_data);
    set_bkg_tiles_rle(8U, 4U, 12U, 12U, tiles);

    if(CGB_MODE) {
        set_bkg_palette_buffer(win1_dx_palette_offset, palette_data_length, palette_data);

        VBK_REG = 1U;
        set_bkg_tiles_rle(8U, 4U, 12U, 12U, palettes);
        VBK_REG = 0U;
    }

    SWITCH_ROM_MBC1(game_bank);
}

void setCloudAnimation(UBYTE skin) {
    const UBYTE *data;

    SWITCH_ROM_MBC1(CLOUD_ANIMATIONS_BANK);

    switch(skin) {
        case 1U:
            data = pause_cloud1_data;
            break;
        case 2U:
            data = pause_cloud2_data;
            break;
    }

    set_sprite_data(0U, pause_cloud1_data_length, data);
    SWITCH_ROM_MBC1(game_bank);
}

void selectSetBannerData(UBYTE index, UBYTE part, UBYTE pal_buffer) {
    SWITCH_ROM_MBC1(SELECTION_DATA_BANK);
    _selectSetBannerData(index, part, pal_buffer);
    SWITCH_ROM_MBC1(game_bank);
}

void selectSetBannerTiles(UBYTE index, UBYTE x, UBYTE y) {
    SWITCH_ROM_MBC1(SELECTION_DATA_BANK);
    _selectSetBannerTiles(index, x, y);
    SWITCH_ROM_MBC1(game_bank);
}

void selectSetBannerColumn(UBYTE index, UBYTE x, UBYTE y) {
    SWITCH_ROM_MBC1(SELECTION_DATA_BANK);
    _selectSetBannerColumn(index, x, y);
    SWITCH_ROM_MBC1(game_bank);
}
