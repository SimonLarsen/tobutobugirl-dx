#include <gb/gb.h>
#include <gb/cgb.h>
#include "defines.h"
#include "gamestate.h"
#include "background1.h"
#include "background1_dx.h"
#include "background2.h"
#include "background3.h"
#include "background4.h"
#include "pause_bg.h"
#include "win1.h"
#include "win1_dx.h"
#include "win2.h"
#include "win2_dx.h"
#include "win3.h"
#include "win3_dx.h"
#include "win4.h"
#include "win4_dx.h"
#include "pause_cloud1.h"
#include "pause_cloud2.h"
#include "mmlgb/driver/music.h"

UBYTE vbl_count;
UBYTE ticks, paused, timer;
UBYTE gamestate, scene_state;
UBYTE level, levels_completed;
UBYTE selection, sub_selection;
UBYTE joystate, oldjoystate;
UBYTE next_sprite, sprites_used;
UBYTE elapsed_time, elapsed_minutes, remaining_time, kills;
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

const UBYTE level_names[7][6] = {
    {22U, 25U, 13U, 21U, 15U, 14U}, // "LOCKED"
    {26U, 22U, 11U, 19U, 24U, 29U}, // "PLAINS"
    {13U, 22U, 25U, 31U, 14U, 29U}, // "CLOUDS"
    {29U, 26U, 11U, 13U, 15U, 10U}, // "SPACE "
    {14U, 28U, 15U, 11U, 23U, 10U}, // "DREAM "
    //{18U, 15U, 11U, 32U, 15U, 24U}, // "HEAVEN"
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

UBYTE getRank(UBYTE score, UBYTE level) {
    UBYTE i;
    for(i = 0U; i != 4U; ++i) {
        if(score >= level_tiers[level-1U][i]) break;
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

void clearSprites() {
    UBYTE i;
    for(i = 0U; i < 40U; ++i) move_sprite(i, 0U, 0U);
    sprites_used = 0U;
    next_sprite = 0U;
}

void updateJoystate() {
    oldjoystate = joystate;
    joystate = joypad();
}

void setSprite(UBYTE x, UBYTE y, UBYTE tile, UBYTE prop) {
    UBYTE* oam = (UBYTE*)0xC000UL + (next_sprite << 2);
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

void setIngameBackground(UBYTE level) {
    UBYTE *tile_data, *tiles, *palette_data, *palettes;
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
            tile_data = background2_data;
            tiles = background2_tiles;
            data_length = background2_data_length;
            if(CGB_MODE) {
                palette_data = background2_palette_data;
                palettes = background2_palettes;
                palette_data_length = background2_palette_data_length;
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

    set_bkg_data_rle(background1_dx_tiles_offset, data_length, tile_data);
    if(level == 255U) {
        set_bkg_tiles_rle(0U, 0U, 20U, 18U, tiles);
    } else {
        set_bkg_tiles_rle(0U, 0U, 18U, 32U, tiles);
    }
    if(CGB_MODE) {
        VBK_REG = 1U;
        if(level == 255U) {
            set_bkg_palette(0U, palette_data_length, palette_data);
            set_bkg_tiles_rle(0U, 0U, 20U, 18U, palettes);
        } else {
            set_bkg_palette_buffer(0U, palette_data_length, palette_data);
            set_bkg_tiles_rle(0U, 0U, 18U, 32U, palettes);
        }
        VBK_REG = 0U;
    }

    SWITCH_ROM_MBC1(game_bank);
}

void setWinscreenBackground(UBYTE level) {
    SWITCH_ROM_MBC1(WINSCREEN_BACKGROUNDS_BANK);

    switch(level) {
        case 1:
            if(CGB_MODE) {
                set_bkg_data_rle(win1_dx_tiles_offset, win1_dx_data_length, win1_dx_data);
                set_bkg_tiles_rle(11U, 4U, win1_dx_tiles_width, win1_dx_tiles_height, win1_dx_tiles);
                set_bkg_palette_buffer(win1_dx_palette_offset, win1_dx_palette_data_length, win1_dx_palette_data);
                VBK_REG = 1U;
                set_bkg_tiles_rle(11U, 4U, win1_dx_tiles_width, win1_dx_tiles_height, win1_dx_palettes);
                VBK_REG = 0U;
            } else {
                set_bkg_data_rle(win1_tiles_offset, win1_data_length, win1_data);
                set_bkg_tiles_rle(11U, 4U, win1_tiles_width, win1_tiles_height, win1_tiles);
            }
            break;
        case 2:
            if(CGB_MODE) {
                set_bkg_data_rle(win2_dx_tiles_offset, win2_dx_data_length, win2_dx_data);
                set_bkg_tiles_rle(8U, 4U, win2_dx_tiles_width, win2_dx_tiles_height, win2_dx_tiles);
                set_bkg_palette_buffer(win2_dx_palette_offset, win2_dx_palette_data_length, win2_dx_palette_data);
                VBK_REG = 1U;
                set_bkg_tiles_rle(8U, 4U, win2_dx_tiles_width, win2_dx_tiles_height, win2_dx_palettes);
                VBK_REG = 0U;
            } else {
                set_bkg_data_rle(win2_tiles_offset, win2_data_length, win2_data);
                set_bkg_tiles_rle(8U, 4U, win2_tiles_width, win2_tiles_height, win2_tiles);
            }
            break;
        case 3:
            if(CGB_MODE) {
                set_bkg_data_rle(win3_dx_tiles_offset, win3_dx_data_length, win3_dx_data);
                set_bkg_tiles_rle(8U, 4U, win3_dx_tiles_width, win3_dx_tiles_height, win3_dx_tiles);
                set_bkg_palette_buffer(win3_dx_palette_offset, win3_dx_palette_data_length, win3_dx_palette_data);
                VBK_REG = 1U;
                set_bkg_tiles_rle(8U, 4U, win3_dx_tiles_width, win3_dx_tiles_height, win3_dx_palettes);
                VBK_REG = 0U;
            } else {
                set_bkg_data_rle(win3_tiles_offset, win3_data_length, win3_data);
                set_bkg_tiles_rle(8U, 4U, win3_tiles_width, win3_tiles_height, win3_tiles);
            }
            break;
        case 4:
            if(CGB_MODE) {
                set_bkg_data_rle(win4_dx_tiles_offset, win4_dx_data_length, win4_dx_data);
                set_bkg_tiles_rle(8U, 4U, win4_dx_tiles_width, win4_dx_tiles_height, win4_dx_tiles);
                set_bkg_palette_buffer(win4_dx_palette_offset, win4_dx_palette_data_length, win4_dx_palette_data);
                VBK_REG = 1U;
                set_bkg_tiles_rle(8U, 4U, win4_dx_tiles_width, win4_dx_tiles_height, win4_dx_palettes);
                VBK_REG = 0U;
            } else {
                set_bkg_data_rle(win4_tiles_offset, win4_data_length, win4_data);
                set_bkg_tiles_rle(8U, 4U, win4_tiles_width, win4_tiles_height, win4_tiles);
            }
            break;
    }


    SWITCH_ROM_MBC1(game_bank);
}

void setCloudAnimation(UBYTE skin) {
    UBYTE *data;

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

/*
void set_bkg_data_rle(UBYTE first, UBYTE n, UBYTE *data) {
    UBYTE i, j, run, tile;
    UBYTE block[16];

    run = 0U;
    for(i = first; i != first+n; ++i) {
        for(j = 0U; j != 16U; ++j) {
            if(run == 0U) {
                tile = data[0];
                if(data[0] == data[1]) {
                    run = data[2];
                    data += 3U;
                } else {
                    run = 1U;
                    data++;
                }
            }
            block[j] = tile;
            run--;
        }
        set_bkg_data(i, 1U, block);
    }
}
*/

/*
void set_bkg_data_rle(UBYTE first, UBYTE n, UBYTE *data) {
    UBYTE run, value;
    UWORD count;
    UBYTE *out;
    
    out = (UBYTE*)0x9000UL;
    out += (UWORD)first << 4UL;

    count = (UWORD)n << 4UL;
    run = 0U;

    while(count != 0UL) {
        if(out >= (UBYTE*)0x9800UL) out -= (UBYTE*)0x1000UL;

        if(run == 0U) {
            value = *data;
            if(value == data[1]) {
                run = data[2];
                data += 3U;
            } else {
                run = 1U;
                data++;
            }
        }
        run--;

        while(STAT_REG & 2U) {}
        *out = value;
        ++out;

        count--;
    }
}
*/

void set_bkg_tiles_rle(UBYTE x, UBYTE y, UBYTE width, UBYTE height, UBYTE *tiles) {
    UBYTE ix, iy, run, tile;
    run = 0U;
    for(iy = y; iy != y+height; ++iy) {
        for(ix = x; ix != x+width; ++ix) {
            if(!run) {
                tile = tiles[0];
                if(tile == tiles[1]) {
                    run = tiles[2];
                    tiles += 3U;
                } else {
                    run = 1U;
                    tiles++;
                }
            }
            set_bkg_tiles(ix, iy, 1U, 1U, &tile);
            run--;
        }
    }
}

/*
void sgb_send_packet(UBYTE* data) {
    UBYTE i, b, val;

    P1_REG = SGB_P1_NEUTRAL;

    P1_REG = 0U;
    P1_REG = SGB_P1_NEUTRAL;

    for(i = 0U; i != 16U; ++i) {
        val = data[i];
        for(b = 0U; b != 8U; ++b) {
            if(val & 1U) {
                P1_REG = SGB_P1_HIGH;
            } else {
                P1_REG = SGB_P1_LOW;
            }
            P1_REG = SGB_P1_NEUTRAL;
            val = val >> 1;
        }
    }

    P1_REG = SGB_P1_LOW;
    P1_REG = SGB_P1_NEUTRAL;
}
*/
