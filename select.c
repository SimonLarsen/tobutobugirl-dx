#include <gb/gb.h>
#include "defines.h"
#include "select.h"
#include "fade.h"
#include "gamestate.h"
#include "set_data_rle.h"
#include "cos.h"
#include "ram.h"
#include "sound.h"
#include "mmlgb/driver/music.h"
#include "sgb_send_packet.h"

#include "characters.h"
#include "arrow.h"
#include "circles.h"
#include "data/palettes/select_sprites.h"
#include "data/sprite/togglecat.h"
#include "data/sprite/togglepanda.h"
#include "data/bg/catface.h"
#include "data/bg/catface_dx.h"
#include "data/bg/select.h"

UBYTE select_scroll_dir;
UBYTE select_cat_state, select_panda_state;
UBYTE cat_frame_reverse, panda_frame_reverse;
UBYTE panda_frame;

#define CAT_ANIM_OFF 0U
#define CAT_ANIM_IN  1U
#define CAT_ANIM_ON  2U
#define CAT_ANIM_OUT 3U

extern UBYTE mainmenu_song_data;
extern UBYTE potaka_song_data;

const UBYTE cat_even_tiles[6] = { 9U, 11U, 9U, 11U, 9U, 11U };
const UBYTE cat_odd_tiles[6]  = { 10U, 12U, 10U, 12U, 10U, 12U };
const UBYTE cat_palettes[6]  = { 0U, 0U, 0U, 0U, 0U, 0U };

const UBYTE SGB_SELECT_PAL01[16] = {
    (0U << 3) + 1U, 255U, 127U, 124U, 66U, 176U, 12U, 0U, 0U, 233U, 113U, 62U, 53U, 0U, 0U, 0U
};

const UBYTE SGB_SELECT_PAL23[16] = {
    (1U << 3) + 1U, 255U, 127U, 28U, 94U, 112U, 40U, 0U, 0U, 254U, 98U, 62U, 53U, 0U, 0U, 0U
};

const UBYTE SGB_SELECT_ATTRBLK[16] = {
    (4U << 3) + 1U,
    2U,
    // data set 1
    7U,
    3U | (3U << 2) | (1U << 4),
    0U, 2U, 19U, 4U,
    // data set 2
    3U,
    0U,
    2U, 10U, 17U, 15U,
    //
    0U, 0U
};

void initSelect() {
    UBYTE buf[3];

    disable_interrupts();
    DISPLAY_OFF;

    if(sgb_mode) {
        sgb_send_packet(SGB_SELECT_PAL01); delay(65U);
        sgb_send_packet(SGB_SELECT_PAL23); delay(65U);
        sgb_send_packet(SGB_SELECT_ATTRBLK);
    }

    move_bkg(0U, 0U);
    set_sprite_data(0U, 37U, characters_data);
    set_sprite_data(37U, arrow_data_length, arrow_data);
    set_sprite_data(41U, togglecat_data_length, togglecat_data);
    set_sprite_data(41U+togglecat_data_length, togglepanda_data_length, togglepanda_data);

    set_bkg_data(0U, circles_data_length, circles_data);
    set_bkg_data_rle(select_tiles_offset, select_data_length, select_data);
    set_bkg_tiles_rle(0U, 0U, select_tiles_width, select_tiles_height, select_tiles);

    if(CGB_MODE) {
        set_bkg_data(catface_dx_tiles_offset, catface_dx_data_length, catface_dx_data);
        set_bkg_palette_buffer(0U, select_palette_data_length, select_palette_data);
        VBK_REG = 1U;
        set_bkg_tiles_rle(0U, 0U, select_tiles_width, select_tiles_height, select_palettes);
        buf[0] = 3U; buf[1] = 3U; buf[2] = 96U;
        set_bkg_tiles_rle(2U, 10U, 16U, 6U, buf);
        VBK_REG = 0U;
        set_sprite_palette(0U, select_sprites_palette_data_length, select_sprites_palette_data);
    } else {
        set_bkg_data(catface_tiles_offset, catface_data_length, catface_data);
    }

    ticks = 0U;
    timer = 0U;
    elapsed_time = 0U;
    circle_index = 0U;
    arrow_offset1 = 0U;
    arrow_offset2 = 0U;

    select_cat_state = select_panda_state = CAT_ANIM_OFF;
    cat_frame = cat_frame_reverse = 0U;
    panda_frame = panda_frame_reverse = 0U;
    if(player_skin == 2U) {
        select_cat_state = CAT_ANIM_IN;
        cat_frame = 4U;
    }
    else if(player_skin == 3U) {
        select_panda_state = CAT_ANIM_IN;
        panda_frame = 4U;
    }

    OBP0_REG = 0xD0U; // 11010000
    OBP1_REG = 0xE0U; // 11100000
    BGP_REG = 0xE4U; // 11100100

    clearSprites();
    selectSetBannerData(selection, 1U, 1U);
    selectSetBannerData(selection, 2U, 1U);
    selectSetBannerTiles(selection, 2U, 10U);

    HIDE_WIN;
    SHOW_SPRITES;
    SHOW_BKG;
    SPRITES_8x8;

    DISPLAY_ON;
    enable_interrupts();
}

void selectUpdateSprites() {
    UBYTE frame, pal;

    setSprite(24U-arrow_offset1, 68U, 37U, OBJ_PAL1);
    setSprite(32U-arrow_offset1, 68U, 39U, OBJ_PAL1);
    setSprite(24U-arrow_offset1, 76U, 38U, OBJ_PAL1);
    setSprite(32U-arrow_offset1, 76U, 40U, OBJ_PAL1);

    setSprite(136U+arrow_offset2, 68U, 39U, OBJ_PAL1 | FLIP_X);
    setSprite(144U+arrow_offset2, 68U, 37U, OBJ_PAL1 | FLIP_X);
    setSprite(136U+arrow_offset2, 76U, 40U, OBJ_PAL1 | FLIP_X);
    setSprite(144U+arrow_offset2, 76U, 38U, OBJ_PAL1 | FLIP_X);
    
    if(levels_completed >= 4U) {
        switch(select_panda_state) {
            case CAT_ANIM_OFF:
                if((ticks & 15U) == 15U) {
                    if(panda_frame_reverse) panda_frame--;
                    else panda_frame++;
                    if(panda_frame == 0U) panda_frame_reverse = 0U;
                    else if(panda_frame == 4U) panda_frame_reverse = 1U;
                }
                break;
            case CAT_ANIM_IN:
                if((ticks & 7U) == 7U) panda_frame++;
                if(panda_frame == 8U) select_panda_state = CAT_ANIM_ON;
                break;
            case CAT_ANIM_ON:
                if((ticks & 15U) == 15U) panda_frame++;
                if(panda_frame == 10U) panda_frame = 8U;
                break;
            case CAT_ANIM_OUT:
                if((ticks & 7U) == 7U) panda_frame--;
                if(panda_frame == 4U) {
                    select_panda_state = CAT_ANIM_OFF;
                    panda_frame_reverse = 1U;
                }
                break;
        }

        frame = 81U + (panda_frame << 2);
        pal = sgb_mode << 4 | 2U;
        setSprite(148U, 20U, frame++, pal);
        setSprite(156U, 20U, frame++, pal);
        setSprite(148U, 28U, frame++, pal);
        setSprite(156U, 28U,   frame, pal);
    }

    if(levels_completed >= 3U) {
        switch(select_cat_state) {
            case CAT_ANIM_OFF:
                if((ticks & 15U) == 15U) {
                    if(cat_frame_reverse) cat_frame--;
                    else cat_frame++;
                    if(cat_frame == 0U) cat_frame_reverse = 0U;
                    else if(cat_frame == 4U) cat_frame_reverse = 1U;
                }
                break;
            case CAT_ANIM_IN:
                if((ticks & 7U) == 7U) cat_frame++;
                if(cat_frame == 8U) select_cat_state = CAT_ANIM_ON;
                break;
            case CAT_ANIM_ON:
                if((ticks & 15U) == 15U) cat_frame++;
                if(cat_frame == 10U) cat_frame = 8U;
                break;
            case CAT_ANIM_OUT:
                if((ticks & 7U) == 7U) cat_frame--;
                if(cat_frame == 4U) {
                    select_cat_state = CAT_ANIM_OFF;
                    cat_frame_reverse = 1U;
                }
                break;
        }

        frame = 41U + (cat_frame << 2);
        pal = sgb_mode << 4 | 1U;
        setSprite(128U, 20U, frame++, pal);
        setSprite(136U, 20U, frame++, pal);
        setSprite(128U, 28U, frame++, pal);
        setSprite(136U, 28U,   frame, pal);

        if(CLICKED(J_SELECT)) {
            if(player_skin == 1U && select_cat_state == CAT_ANIM_OFF && select_panda_state == CAT_ANIM_OFF) {
                player_skin = 2U;
                select_cat_state = CAT_ANIM_IN;
                playSound(SFX_CAT_ENABLE);
            }
            else if(player_skin == 2U && select_cat_state == CAT_ANIM_ON) {
                select_cat_state = CAT_ANIM_OUT;
                if(levels_completed >= 4U) {
                    player_skin = 3U;
                    select_panda_state = CAT_ANIM_IN;
                    playSound(SFX_CAT_ENABLE);
                } else {
                    player_skin = 1U;
                    playSound(SFX_CAT_DISABLE);
                }
            }
            else if(player_skin == 3U && select_panda_state == CAT_ANIM_ON) {
                player_skin = 1U;
                select_panda_state = CAT_ANIM_OUT;
                playSound(SFX_CAT_DISABLE);
            }
        }
    }
}

void selectScrollCircles() {
    circle_index = (circle_index+1U) & 7U;
    disable_interrupts();
    set_bkg_data(13U, 1U, &circles_data[(circle_index << 4)]);
    enable_interrupts();
}

void selectFadeOut() {
    UBYTE i, x;

    for(i = 0U; i != 16U; ++i) {
        if(select_scroll_dir == LEFT) x = i+2U;
        else x = 17U - i;
        disable_interrupts();
        if(x & 1U) {
            set_bkg_tiles(x, 10U, 1U, 6U, cat_odd_tiles);
        } else {
            set_bkg_tiles(x, 10U, 1U, 6U, cat_even_tiles);
        }
        if(CGB_MODE) {
            VBK_REG = 1U;
            set_bkg_tiles(x, 10U, 1U, 6U, cat_palettes);
            VBK_REG = 0U;
        }
        enable_interrupts();
        if(!(i & 1U)) {
            ticks++;
            if((ticks & 3U) == 3U) selectScrollCircles();
            if(select_scroll_dir == LEFT) arrow_offset1 = cos32_64[i >> 1];
            else arrow_offset2 = cos32_64[i >> 1];
            selectUpdateSprites();
            clearRemainingSprites();
            snd_update();
            wait_vbl_done();
        }
    }
}

void selectFadeIn() {
    UBYTE i, x;

    snd_update();

    disable_interrupts();
    selectSetBannerData(selection, 1U, 0U);
    enable_interrupts();

    selectScrollCircles();
    snd_update();

    disable_interrupts();
    selectSetBannerData(selection, 2U, 0U);
    enable_interrupts();

    selectScrollCircles();
    snd_update();

    for(i = 0U; i != 16U; ++i) {
        if(select_scroll_dir == LEFT) x = i+2U;
        else x = 17U - i;
        disable_interrupts();
        selectSetBannerColumn(selection, x, 10U);
        enable_interrupts();

        if(i & 1U) {
            ticks++;
            if((ticks & 3U) == 3U) {
                selectScrollCircles();
            }
            if(select_scroll_dir == LEFT) arrow_offset1 = cos32_64[7U - (i >> 1)];
            else arrow_offset2 = cos32_64[7U - (i >> 1)];
            selectUpdateSprites();
            clearRemainingSprites();
            snd_update();
            wait_vbl_done();
        }
    }

    arrow_offset1 = 0U;
    arrow_offset2 = 0U;
}

void enterSelect() {
    UBYTE i, offset, name_index;
    initSelect();

    fadeFromWhite(4U);

    disable_interrupts();
    setMusicBank(SONG_BANK_MAINMENU);
    playMusic(&mainmenu_song_data);
    enable_interrupts();

    while(gamestate == GAMESTATE_SELECT) {
        updateJoystate();

        ticks++;
        if((ticks & 3U) == 3U) {
            selectScrollCircles();
        }

        timer++;
        if(timer == 60U) {
            timer = 0U;
            elapsed_time++;

            if(elapsed_time == 110U) {
                disable_interrupts();
                STOP_MUSIC;
                enable_interrupts();
            }

            else if(elapsed_time == 120U) {
                disable_interrupts();
                setMusicBank(SONG_BANK_MAINMENU);
                playMusic(&mainmenu_song_data);
                enable_interrupts();

                elapsed_time = 0U;
            }
        }

        if(elapsed_time == 110U && timer == 32) {
            disable_interrupts();
            setMusicBank(SONG_BANK_POTAKA);
            playMusic(&potaka_song_data);
            enable_interrupts();
        }

        if(ISDOWN(J_RIGHT)) {
            select_scroll_dir = RIGHT;
            selection++;
            if(selection == 8U) selection = 1U;
            playSound(SFX_MENU_SWITCH);
            selectFadeOut();
            selectFadeIn();
            selectUpdateSprites();
        }
        if(ISDOWN(J_LEFT)) {
            select_scroll_dir = LEFT;
            selection--;
            if(selection == 0U) selection = 7U;
            playSound(SFX_MENU_SWITCH);
            selectFadeOut();
            selectFadeIn();
            selectUpdateSprites();
        }
        if(CLICKED(J_START) || CLICKED(J_A)) {
            if(selection == 7U) {
                gamestate = GAMESTATE_HIGHSCORE;
                playSound(SFX_MENU_CONFIRM);
            } else if(selection == 6U && levels_completed >= 2U) {
                gamestate = GAMESTATE_JUKEBOX;
                playSound(SFX_MENU_CONFIRM);
            } else if(selection <= levels_unlocked) {
                level = selection;
                gamestate = GAMESTATE_INGAME;
                playSound(SFX_MENU_CONFIRM);
            } else {
                playSound(SFX_MENU_LOCKED);
            }
        }
        if(CLICKED(J_B)) {
            gamestate = GAMESTATE_TITLE;
            playSound(SFX_MENU_CANCEL);
        }

        // Draw level name
        if(selection <= 5U && selection > levels_unlocked || selection == 6U && levels_completed < 2U) {
            name_index = 0U;
        } else {
            name_index = selection;
        }

        offset = 64U;
        if(level_names[name_index][5] == 10U) {
            offset += 4U;
        }
        for(i = 0U; i != 6U; ++i) {
            setSprite(offset, 70U+cos4_16[(i+(ticks >> 1)) & 15U], level_names[name_index][i], OBJ_PAL0);
            offset += 8U;
        }

        selectUpdateSprites();
        clearRemainingSprites();
        snd_update();
        wait_vbl_done();
    }

    disable_interrupts();
    STOP_MUSIC;
    enable_interrupts();

    clearRemainingSprites(); // Remove all sprites
    fadeToWhite(4U);
    wait_sound_done();
}
