#include <gb/gb.h>
#include <gb/cgb.h>
#include "defines.h"
#include "gamestate.h"
#include "set_data_rle.h"
#include "fade.h"
#include "cos.h"
#include "highscore.h"
#include "ram.h"
#include "sound.h"
#include "mmlgb/driver/music.h"
#include "sgb_send_packet.h"

#include "characters.h"
#include "arrow.h"
#include "data/palettes/highscore_sprites.h"
#include "data/sprite/empty.h"
#include "data/bg/highscore.h"
#include "data/bg/highscore_dx.h"
#include "circles.h"

extern UBYTE highscore_song_data;
extern UBYTE dream_score_song_data;

const UBYTE SGB_HIGHSCORE_PAL01[16] = {
    1, 255, 127, 124, 66, 176, 12, 0, 0, 233, 113, 62, 53, 0, 0, 0
};

const UBYTE SGB_HIGHSCORE_PAL23[16] = {
    (1 << 3) + 1, 255, 127, 28, 94, 112, 40, 0, 0, 126, 86, 62, 53, 0, 0, 0
};

const UBYTE SGB_HIGHSCORE_ATTRBLK[32] = {
    (0x04U << 3) + 2U,
    4U,
    // data set 1
    7U,
    3U | (3U << 2) | (1U << 4),
    0U, 1U, 19U, 3U,
    // data set 2
    3U,
    0U,
    2U, 4U, 17U, 9U,
    // data set 3
    3U,
    3U | (3U << 2),
    1U, 10U, 18U, 16U,
    // data set 4
    3U,
    3U | (3U << 2),
    5U, 8U, 14U, 10U,
    //
    0U, 0U, 0U, 0U, 0U, 0U
};

void initHighscore() {
    UBYTE buf[3];
    disable_interrupts();
    DISPLAY_OFF;

    if(sgb_mode) {
        sgb_send_packet(SGB_HIGHSCORE_PAL01); delay(65U);
        sgb_send_packet(SGB_HIGHSCORE_PAL23); delay(65U);
        sgb_send_packet(SGB_HIGHSCORE_ATTRBLK); delay(65U);
        sgb_send_packet(SGB_HIGHSCORE_ATTRBLK+16U);
    }

    move_bkg(0U, 0U);
    set_bkg_data(0U, 38U, characters_data);
    set_bkg_data(38U, circles_data_length, circles_data);

    if(CGB_MODE) {
        set_bkg_data_rle(highscore_dx_tiles_offset, highscore_dx_data_length, highscore_dx_data);
        set_bkg_tiles_rle(0U, 0U, highscore_dx_tiles_width, highscore_dx_tiles_height, highscore_dx_tiles);
        set_bkg_palette_buffer(highscore_dx_palette_offset, highscore_dx_palette_data_length, highscore_dx_palette_data);
        set_sprite_palette(0U, highscore_sprites_palette_data_length, highscore_sprites_palette_data);
        VBK_REG = 1U;
        set_bkg_tiles_rle(0U, 0U, highscore_dx_tiles_width, highscore_dx_tiles_height, highscore_dx_palettes);
        buf[0] = 0U; buf[1] = 0U; buf[2] = 18U;
        set_bkg_tiles_rle(2U, 4U, 3U, 6U, buf);
        set_bkg_tiles_rle(15U, 4U, 3U, 6U, buf);
        buf[2] = 40U;
        set_bkg_tiles_rle(5U, 4U, 10U, 4U, buf);
        VBK_REG = 0U;
    } else {
        set_bkg_data_rle(highscore_tiles_offset, highscore_data_length, highscore_data);
        set_bkg_tiles_rle(0U, 0U, highscore_tiles_width, highscore_tiles_height, highscore_tiles);
    }

    set_sprite_data(0U, arrow_data_length, arrow_data);
    set_sprite_data(arrow_data_length, 1U, empty_data);

    ticks = 0U;
    circle_index = 0U;
    sub_selection = level;

    OBP0_REG = 0xD0U; // 11010000
    OBP1_REG = 0xE0U; // 11100000
    BGP_REG = 0xE4U; // 11100100

    clearSprites();
    _highscoreUpdateScreen();

    SPRITES_8x8;
    SHOW_SPRITES;
    HIDE_WIN;
    SHOW_BKG;

    DISPLAY_ON;
    enable_interrupts();
}

void highscoreUpdateScreen() {
    clearRemainingSprites();
    fadeToWhite(4U);
    DISPLAY_OFF;
    disable_interrupts();

    _highscoreUpdateScreen();

    enable_interrupts();
    DISPLAY_ON;
    fadeFromWhite(4U);
}

void _highscoreUpdateScreen() {
    UBYTE i, j, tile;
    UBYTE *data;

    // Select level images
    tile = sub_selection;
    if(tile > levels_unlocked) {
        tile = 0U;
    }

    selectSetBannerData(sub_selection, 1U, 1U);
    selectSetBannerData(sub_selection, 2U, 1U);
    selectSetBannerTiles(sub_selection, 2U, 4U);

    if(CGB_MODE) {
        set_bkg_tiles_rle(5U, 8U, 10U, 1U, highscore_dx_tiles+55U);
        set_bkg_tiles_rle(5U, 9U, 10U, 1U, highscore_dx_tiles+69U);
        VBK_REG = 1U;
        set_bkg_tiles_rle(5U, 8U, 10U, 1U, highscore_dx_palettes+55U);
        set_bkg_tiles_rle(5U, 9U, 10U, 1U, highscore_dx_palettes+69U);
        VBK_REG = 0U;
        set_bkg_palette_buffer(highscore_dx_palette_offset, highscore_dx_palette_data_length, highscore_dx_palette_data);
    } else {
        set_bkg_tiles_rle(5U, 8U, 10U, 1U, highscore_tiles+55U);
        set_bkg_tiles_rle(5U, 9U, 10U, 1U, highscore_tiles+69U);
    }


    // Set level name
    data = level_names[tile];
    set_bkg_tiles(7U, 9U, 6U, 1U, data);

    ENABLE_RAM_MBC1;
    SWITCH_RAM_MBC1(0);

    // Set scores
    data = ram_data + ((sub_selection-1U) << 4);
    for(i = 0U; i != 5U; ++i) {
        tile = 10U;
        for(j = 3U; j != 8U; ++j) {
            set_bkg_tiles(j, i+11U, 1U, 1U, &tile);
        }
        for(j = 10U; j != 14U; ++j) {
            set_bkg_tiles(j, i+11U, 1U, 1U, &tile);
        }
        set_bkg_tiles(16U, i+11U, 1U, 1U, &tile);

        if(*data) {
            // Draw time
            drawTime8(13U, i+11U, data[0]);

            // Draw score
            drawScore8(7U, i+11U, data[1]);

            // Draw rank
            tile = rank_letters[getRank(data[1], sub_selection)];
            set_bkg_tiles(16U, i+11U, 1U, 1U, &tile);
        }
        data += 2U;
    }

    DISABLE_RAM_MBC1;
}

void enterHighscore() {
    UBYTE offset, j;

    initHighscore();

    fadeFromWhite(8U);

    disable_interrupts();
    if(ending_flags & ENDING_FLAG_FROM_DREAM) {
        setMusicBank(SONG_BANK_DREAM_SCORE);
        playMusic(&dream_score_song_data);
    } else {
        setMusicBank(SONG_BANK_HIGHSCORE);
        playMusic(&highscore_song_data);
    }
    ending_flags = 0U;
    enable_interrupts();

    while(1) {
        updateJoystate();

        ticks++;
        if((ticks & 3U) == 3U) {
            circle_index = (circle_index+1U) & 7U;
            set_bkg_data(47U, 1U, &circles_data[(circle_index << 4)]);
        }

        if(ISDOWN(J_LEFT)) {
            sub_selection--;
            if(sub_selection == 0U) sub_selection = 5U;
            playSound(SFX_HIGHSCORE_SWITCH);
            highscoreUpdateScreen();
        }
        if(ISDOWN(J_RIGHT)) {
            sub_selection++;
            if(sub_selection == 6U) sub_selection = 1U;
            playSound(SFX_HIGHSCORE_SWITCH);
            highscoreUpdateScreen();
        }

        if(CLICKED(J_START) || CLICKED(J_B)) {
            gamestate = GAMESTATE_SELECT;
            playSound(SFX_MENU_CANCEL);
            break;
        }

        offset = cos32_64[(ticks & 63U)] >> 3;

        // Draw arrows
        j = sgb_mode << 4;
        setSprite(12U-offset, 64U, 0U, j);
        setSprite(20U-offset, 64U, 2U, j);
        setSprite(12U-offset, 72U, 1U, j);
        setSprite(20U-offset, 72U, 3U, j);

        setSprite(148U+offset, 64U, 2U, j | FLIP_X);
        setSprite(156U+offset, 64U, 0U, j | FLIP_X);
        setSprite(148U+offset, 72U, 3U, j | FLIP_X);
        setSprite(156U+offset, 72U, 1U, j | FLIP_X);

        if((ticks & 63U) < 16U
        && last_highscore_level == sub_selection
        && last_highscore_slot < 5U) {
            offset = 104U + (last_highscore_slot << 3);

            for(j = 4U; j != 9U; ++j) {
                setSprite(j<<3, offset, 4U, OBJ_PAL0);
            }
            for(j = 11U; j != 15U; ++j) {
                setSprite(j<<3, offset, 4U, OBJ_PAL0);
            }
            setSprite(136U, offset, 4U, OBJ_PAL0);
        }

        clearRemainingSprites();
        snd_update();
        wait_vbl_done();
    }

    // Clear blinking after exit
    last_highscore_slot = 5U;

    STOP_MUSIC;
    clearRemainingSprites();
    fadeToWhite(4U);
    wait_sound_done();
}
