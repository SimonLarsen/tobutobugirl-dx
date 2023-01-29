#include <gb/gb.h>
#include <gb/cgb.h>
#include <rand.h>
#include "defines.h"
#include "ending.h"
#include "gamestate.h"
#include "fade.h"
#include "set_data_rle.h"
#include "sgb_send_packet.h"

#include "intro_bg.h"
#include "intro_bg_dx.h"
#include "data/palettes/ending_sprites.h"
#include "data/bg/ending_thanks.h"
#include "data/bg/ending_thanks_dx.h"
#include "data/sprite/ending_sprites1.h"
#include "data/sprite/ending_sprites2.h"
#include "mmlgb/driver/music.h"

#define ENDING_STATE_WAIT      1U
#define ENDING_STATE_FALL      2U
#define ENDING_STATE_POP       3U
#define ENDING_STATE_FALL2     4U
#define ENDING_STATE_SHAKE     5U
#define ENDING_STATE_PAN       6U
#define ENDING_STATE_STRUGGLE  7U
#define ENDING_STATE_GET_UP    8U
#define ENDING_STATE_END       9U

UBYTE ending_frame;

extern UBYTE ending_part1_song_data;
extern UBYTE ending_part2_song_data;

const UBYTE SGB_ENDING_PAL01[16] = {
    1, 255, 127, 27, 38, 209, 8, 34, 0, 0, 0, 0, 0, 0, 0, 0
};

const UBYTE SGB_ENDING_ATTRDIV[16] = {
    (6U << 3) + 1U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U
};

void initEnding() {
    disable_interrupts();
    DISPLAY_OFF;

    if(sgb_mode) {
        sgb_send_packet(SGB_ENDING_PAL01); delay(62U);
        sgb_send_packet(SGB_ENDING_ATTRDIV);
    }

    OBP0_REG = 0xD0U; // 11010000
    OBP1_REG = 0xE0U; // 11100000
    BGP_REG  = 0xE4U; // 11101000

    if(CGB_MODE) {
        set_bkg_data_rle(0U, intro_bg_dx_data_length, intro_bg_dx_data);
        set_bkg_data_rle(ending_thanks_dx_tiles_offset, ending_thanks_dx_data_length, ending_thanks_dx_data);
        set_bkg_tiles_rle(0U, 0U, intro_bg_dx_tiles_width, intro_bg_dx_tiles_height, intro_bg_dx_tiles);
        set_bkg_palette_buffer(0U, intro_bg_dx_palette_data_length, intro_bg_dx_palette_data);
        VBK_REG = 1U;
        set_bkg_tiles_rle(0U, 0U, intro_bg_dx_tiles_width, intro_bg_dx_tiles_height, intro_bg_dx_palettes);
        VBK_REG = 0U;
    } else {
        set_bkg_data_rle(0U, intro_bg_data_length, intro_bg_data);
        set_bkg_data_rle(ending_thanks_tiles_offset, ending_thanks_data_length, ending_thanks_data);
        set_bkg_tiles_rle(0U, 0U, intro_bg_tiles_width, intro_bg_tiles_height, intro_bg_tiles);
    }

    set_sprite_data(0U, ending_sprites1_data_length, ending_sprites1_data);
    set_sprite_palette(0U, ending_sprites_palette_data_length, ending_sprites_palette_data);

    clearSprites();

    setMusicBank(SONG_BANK_ENDING_PART1);
    playMusic(&ending_part1_song_data);

    HIDE_WIN;
    SHOW_BKG;
    SHOW_SPRITES;
    SPRITES_8x16;

    DISPLAY_ON;
    enable_interrupts();
}

void updateEnding() {
    UBYTE tmp, tmp2;

    switch(scene_state) {
        case ENDING_STATE_WAIT:
            if(ticks == 32U) {
                ticks = 0U;
                scene_state = ENDING_STATE_FALL;
            }
            break;
        case ENDING_STATE_FALL:
            if((ticks & 63U) == 63U) {
                player_x--;
            }
            if((ticks & 3U) == 3U) {
                player_y++;
            }

            if(player_y >= 135U) {
                tmp = ticks & 15U;
                if(tmp == 15U) scroll_y++;
            }
            else if(player_y >= 125U) {
                tmp = ticks & 7U;
                if(tmp == 7U) scroll_y++;
            }
            else if(player_y >= 90U) {
                tmp = ticks & 3U;
                if(tmp == 3U) scroll_y++;
            }

            if(player_y >= 142U) {
                tmp = 12U;
            }
            else if(player_y >= 140U) {
                tmp = 8U;
            }
            else if(ticks & 16U) {
                tmp = 0U;
            }
            else {
                tmp = 4U;
            }

            setSprite(player_x-4U, player_y-scroll_y-12U, tmp, OBJ_PAL0 | 1U);
            setSprite(player_x+4U, player_y-scroll_y-12U, tmp+2U, OBJ_PAL0 | 1U);

            if(ticks & 8U) tmp = 28U;
            else tmp = 20U;

            setSprite(player_x-4U, player_y-scroll_y+4U,  tmp+0U, OBJ_PAL0);
            setSprite(player_x+4U, player_y-scroll_y+4U,  tmp+2U, OBJ_PAL0);
            setSprite(player_x-4U, player_y-scroll_y+20U, tmp+4U, OBJ_PAL0 | 1U);
            setSprite(player_x+4U, player_y-scroll_y+20U, tmp+6U, OBJ_PAL0 | 1U);

            if(player_y >= 143U) {
                scene_state = ENDING_STATE_POP;
                ticks = 0U;
            }

            break;

        case ENDING_STATE_POP:
            // Shocked
            setSprite(player_x-4U, player_y-scroll_y+4U,  36U, OBJ_PAL0);
            setSprite(player_x+4U, player_y-scroll_y+4U,  38U, OBJ_PAL0);
            setSprite(player_x-4U, player_y-scroll_y+20U, 40U, OBJ_PAL0 | 1U);
            setSprite(player_x+4U, player_y-scroll_y+20U, 42U, OBJ_PAL0 | 1U);

            if(ticks & 16U) {
                setSprite(player_x-1U, player_y-scroll_y-10U, 16U, OBJ_PAL0 | 2U);
            }

            if(ticks >= 64U) {
                scene_state = ENDING_STATE_FALL2;
                player_x -= 3U;
                player_y += 6U;
                cat_x = player_x-2U;
                cat_y = player_y+8U;
            }
            break;

        case ENDING_STATE_FALL2:
            player_y++;
            cat_y++;
            if(ticks & 1U) {
                player_y++;
                cat_y++;
            }

            if((ticks & 3U) == 3U) {
                player_x++;
                cat_x--;
            }

            setSprite(player_x, player_y-scroll_y, 44U, OBJ_PAL0);
            setSprite(player_x+8U, player_y-scroll_y, 46U, OBJ_PAL0);

            setSprite(cat_x, cat_y-scroll_y, 48U, OBJ_PAL0 | 1U);
            setSprite(cat_x+8U, cat_y-scroll_y, 50U, OBJ_PAL0 | 1U);

            if(player_y >= 240U) {
                scene_state = ENDING_STATE_SHAKE;
                player_y = 240U;
                cat_y = player_y;
                ticks = 0U;
            }
            break;

        case ENDING_STATE_SHAKE:
            scroll_x = 0U;
            if(ticks <= 50U && (ticks & 3U) == 3U) {
                scroll_x = ((UBYTE)rand() & 4U) - 2U;
            }

            if(ticks >= 12U && ticks < 36U) {
                tmp = 52U + (((ticks-12U) >> 3) << 2);
                tmp2 = 144U; if(tmp != 52U) tmp2 -= 4U;

                setSprite(cat_x-8U, tmp2, tmp, OBJ_PAL0);
                setSprite(cat_x, tmp2, tmp+2U, OBJ_PAL0);
                setSprite(player_x+16U, tmp2, tmp, OBJ_PAL0 | FLIP_X);
                setSprite(player_x+8U, tmp2, tmp+2U, OBJ_PAL0 | FLIP_X);
            }

            if(ticks >= 225U) {
                scene_state = ENDING_STATE_PAN;
                ticks = 0U;
                disable_interrupts();
                set_sprite_data(0U, ending_sprites2_data_length, ending_sprites2_data);
                enable_interrupts();
            }
            break;

        case ENDING_STATE_PAN:
            if(scroll_y < 100U) {
                if((ticks & 3U) == 3U) {
                    scroll_y++;
                }
            } else {
                if((ticks & 7U) == 7U) {
                    scroll_y++;
                }
            }
            
            setSprite(cat_x, cat_y-scroll_y, 0U, OBJ_PAL0 | 1U);
            setSprite(cat_x+8U, cat_y-scroll_y, 2U, OBJ_PAL0 | 1U);

            setSprite(player_x, player_y-scroll_y, 28U, OBJ_PAL0);
            setSprite(player_x+8U, player_y-scroll_y, 30U, OBJ_PAL0);

            if(scroll_y == 112U) {
                scene_state = ENDING_STATE_STRUGGLE;
                ticks = 0U;
                ending_frame = 0U;
            }
            break;

        case ENDING_STATE_STRUGGLE:
            if(ticks == 32U) ending_frame = 1U;
            else if(ticks == 48U) ending_frame = 0U;
            else if(ticks == 96U) ending_frame = 1U;
            else if(ticks == 112U) ending_frame = 0U;

            if(ticks >= 170U) {
                scene_state = ENDING_STATE_GET_UP;
                ticks = 0U;
                ending_frame = 0U;

                disable_interrupts();
                setMusicBank(SONG_BANK_ENDING_PART2);
                playMusic(&ending_part2_song_data);
                enable_interrupts();
            }

            tmp = ending_frame << 2;
            setSprite(cat_x, cat_y-scroll_y, tmp, OBJ_PAL0 | 1U);
            setSprite(cat_x+8U, cat_y-scroll_y, tmp+2U, OBJ_PAL0 | 1U);

            tmp += 28U;
            setSprite(player_x, player_y-scroll_y, tmp, OBJ_PAL0);
            setSprite(player_x+8U, player_y-scroll_y, tmp+2U, OBJ_PAL0);
            break;

        case ENDING_STATE_GET_UP:
            if(ending_frame <= 2U) {
                if((ticks & 7U) == 7U) ending_frame++;
            } else {
                if((ticks & 15U) == 15U) {
                    ending_frame++;
                    if(ending_frame == 128U) ending_frame = 16U;
                }

                if(ending_frame == 12U) {
                    if(CGB_MODE) {
                        palette_buffer[0] = 0UL;
                        set_bkg_palette(0U, 1U, palette_buffer);
                    }
                    disable_interrupts();
                    if(CGB_MODE) {
                        set_bkg_tiles_rle(5U, 15U, ending_thanks_dx_tiles_width, ending_thanks_dx_tiles_height, ending_thanks_dx_tiles);
                    } else {
                        set_bkg_tiles_rle(5U, 15U, ending_thanks_tiles_width, ending_thanks_tiles_height, ending_thanks_tiles);
                    }
                    enable_interrupts();
                }
            }

            tmp = ending_frame;
            if(tmp <= 4U) tmp = tmp << 2;
            else if(tmp >= 5 && tmp <= 7) tmp = 20U;
            else tmp = (5U + (tmp & 1U)) << 2;
            setSprite(cat_x, cat_y-scroll_y, tmp, OBJ_PAL0 | 1U);
            setSprite(cat_x+8U, cat_y-scroll_y, tmp+2U, OBJ_PAL0 | 1U);

            tmp = ending_frame;
            if(tmp <= 4U) tmp = tmp << 2;
            else if(tmp >= 5U && tmp <= 7U) tmp = (4U - (tmp & 1U)) << 2;
            else tmp = (5U + (tmp & 1U)) << 2;
            setSprite(player_x, player_y-scroll_y, 28U+tmp, OBJ_PAL0);
            setSprite(player_x+8U, player_y-scroll_y, 30U+tmp, OBJ_PAL0);
            break;
    }
}

void enterEnding() {
    initEnding();

    scroll_x = 0U;
    scroll_y = 20U;
    player_x = 104U;
    player_y = 0U;
    scene_state = ENDING_STATE_FALL;

    move_bkg(0U, scroll_y);

    fadeFromWhite(8U);

    scroll_y = 20U;
    ticks = 0U;
    if(CGB_MODE) ticks = 6U;

    while(scene_state != ENDING_STATE_END) {
        updateJoystate();

        if((CLICKED(J_START) || CLICKED(J_A))
        && (scene_state == ENDING_STATE_GET_UP || !unlocked_bits)) {
            if(scene_state == ENDING_STATE_GET_UP) {
                ending_flags |= ENDING_FLAG_ENDING_FINISHED;
            }
            scene_state = ENDING_STATE_END;
        }

        updateEnding();
        ticks++;

        clearRemainingSprites();
        wait_vbl_done();
        move_bkg(scroll_x, scroll_y);
    }

    clearRemainingSprites();
    gamestate = GAMESTATE_WINSCREEN;

    mus_setPaused(1U);
    clearRemainingSprites();
    fadeToWhite(8U);
}
