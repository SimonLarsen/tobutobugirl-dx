#include <gb/gb.h>
#include <gb/cgb.h>
#include "defines.h"
#include "unlocked.h"
#include "fade.h"
#include "gamestate.h"
#include "set_data_rle.h"
#include "mmlgb/driver/music.h"

#include "characters.h"
#include "zoom_circles.h"
#include "data/palettes/unlocked.h"
#include "data/bg/unlocked.h"
#include "selection2.h"
#include "selection3.h"
#include "selection4.h"
#include "selection_jukebox.h"

extern UBYTE unlocked_song_data;

const UBYTE unlocked_messages[3][24] = {
	{
		10U, 24U, 15U, 34U, 30U, 10U, 22U, 15U, 32U, 15U, 22U, 10U, // " NEXT LEVEL "
		10U, 10U, 31U, 24U, 22U, 25U, 13U, 21U, 15U, 14U, 10U, 10U  // "  UNLOCKED  "
	},
	{
		23U, 31U, 29U, 19U, 13U, 10U, 26U, 22U, 11U, 35U, 15U, 28U, // "MUSIC PLAYER"
		10U, 10U, 31U, 24U, 22U, 25U, 13U, 21U, 15U, 14U, 10U, 10U  // "  UNLOCKED  "
	},
	{
		10U, 10U, 10U, 14U, 28U, 15U, 11U, 23U, 10U, 10U, 10U, 10U, // "   DREAM    "
		10U, 10U, 31U, 24U, 22U, 25U, 13U, 21U, 15U, 14U, 10U, 10U  // "  UNLOCKED  "
	}
};

const UBYTE unlocked_bkg_palettes[45] = {
    0U, 0U, 80U,
    1U, 1U, 80U,
    0U, 0U, 2U, 1U, 1U, 16U,
    0U, 0U, 4U, 1U, 1U, 16U,
    0U, 0U, 4U, 1U, 1U, 16U,
    0U, 0U, 4U, 1U, 1U, 16U,
    0U, 0U, 4U, 1U, 1U, 16U,
    0U, 0U, 4U, 1U, 1U, 16U,
    0U, 0U, 82U,
};

void initUnlocked() {
    UBYTE index, msg;
	disable_interrupts();
	DISPLAY_OFF;

	move_bkg(0U, 0U);

	set_bkg_data(0U, 38U, characters_data);
	set_bkg_data(38U, zoom_circles_data_length, zoom_circles_data);
	set_bkg_data_rle(unlocked_tiles_offset, unlocked_data_length, unlocked_data);
	set_bkg_data(47U, 8U, zoom_circles_data);

	set_bkg_tiles_rle(0U, 0U, unlocked_tiles_width, unlocked_tiles_height, unlocked_tiles);

    if(CGB_MODE) {
        set_bkg_palette_buffer(0U, unlocked_palette_data_length, unlocked_palette_data);
        VBK_REG = 1U;
        set_bkg_tiles_rle(0U, 0U, 20U, 18U, unlocked_bkg_palettes);
        VBK_REG = 0U;
    }

	if(unlocked_bits & UNLOCKED_CLOUDS) {
		unlocked_bits ^= UNLOCKED_CLOUDS;
        index = 2U;
        msg = 0U;
	}
	else if(unlocked_bits & UNLOCKED_SPACE) {
		unlocked_bits ^= UNLOCKED_SPACE;
        index = 3U;
        msg = 0U;
	}
	else if(unlocked_bits & UNLOCKED_MUSIC) {
		unlocked_bits ^= UNLOCKED_MUSIC;
        index = 5U;
        msg = 1U;
	}
	else if(unlocked_bits & UNLOCKED_DREAM) {
		unlocked_bits ^= UNLOCKED_DREAM;
        index = 4U;
        msg = 2U;
	}

    selectSetBannerData(index, 1U, 1U);
    selectSetBannerData(index, 2U, 1U);
    selectSetBannerTiles(index, 2U, 8U);

    set_bkg_tiles(4U, 5U, 12U, 2U, unlocked_messages[msg]);

	BGP_REG = 0xE4U; // 11100100

	circle_index = 0U;

	clearSprites();

	SHOW_SPRITES;
	HIDE_WIN;
	SHOW_BKG;

	DISPLAY_ON;
	enable_interrupts();
}

void enterUnlocked() {
	initUnlocked();

	fadeFromWhite(8U);

	setMusicBank(SONG_BANK_UNLOCKED);
	disable_interrupts();
	playMusic(&unlocked_song_data);
	enable_interrupts();

	ticks = 0U;
	while(1U) {
		updateJoystate();

		ticks++;
		if((ticks & 3U) == 3U) {
			circle_index = (circle_index+1U) & 7U;
			set_bkg_data(47U+circle_index, 8U-circle_index, zoom_circles_data);
			if(circle_index) {
				set_bkg_data(47U, circle_index, &zoom_circles_data[(8U-circle_index) << 4]);
			}
		}

		if(CLICKED(J_START) || CLICKED(J_A)) {
			if(unlocked_bits == 0U) {
				gamestate = GAMESTATE_HIGHSCORE;
			}
			break;
		}

		clearRemainingSprites();
		wait_vbl_done();
	}

	clearRemainingSprites();
	fadeToWhite(8U);

	STOP_MUSIC;
}
