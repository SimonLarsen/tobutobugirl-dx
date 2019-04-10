#include <gb/gb.h>
#include <string.h>
#include "defines.h"
#include "select.h"
#include "fade.h"
#include "gamestate.h"
#include "set_data_rle.h"
#include "cos.h"
#include "ram.h"
#include "sound.h"
#include "mmlgb/driver/music.h"

#include "characters.h"
#include "arrow.h"
#include "data/sprite/togglecat.h"

#include "circles.h"
#include "data/bg/catface.h"
#include "data/bg/catface_dx.h"
#include "data/bg/select.h"

UBYTE select_scroll_dir;
UBYTE select_cat_state;
UBYTE cat_frame_reverse;

#define CAT_ANIM_OFF 0U
#define CAT_ANIM_IN  1U
#define CAT_ANIM_ON  2U
#define CAT_ANIM_OUT 3U

extern UBYTE mainmenu_song_data;
extern UBYTE potaka_song_data;

const UWORD select_sprite_palettes[8] = {
	32767, 32767, 11516, 0,
	32767, 28638, 9695, 0
};

const UBYTE cat_even_tiles[6] = { 9U, 11U, 9U, 11U, 9U, 11U };
const UBYTE cat_odd_tiles[6]  = { 10U, 12U, 10U, 12U, 10U, 12U };
const UBYTE cat_palettes[6]  = { 0U, 0U, 0U, 0U, 0U, 0U };

void initSelect() {
	UBYTE buf[3];

	disable_interrupts();
	DISPLAY_OFF;

	move_bkg(0U, 0U);
	set_sprite_data(0U, 37U, characters_data);
	set_sprite_data(37U, arrow_data_length, arrow_data);
	set_sprite_data(41U, togglecat_data_length, togglecat_data);

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
		set_sprite_palette(0U, 2U, select_sprite_palettes);
	} else {
	    set_bkg_data(catface_tiles_offset, catface_data_length, catface_data);
    }

	ticks = 0U;
	timer = 0U;
	elapsed_time = 0U;
	circle_index = 0U;
	arrow_offset1 = 0U;
	arrow_offset2 = 0U;

	select_cat_state = CAT_ANIM_OFF;
	cat_frame = 0U;
	cat_frame_reverse = 0U;
	if(player_skin == 2U) {
		select_cat_state = CAT_ANIM_IN;
		cat_frame = 4U;
	}

	OBP0_REG = 0xD0U; // 11010000
	OBP1_REG = 0xB4U; // 11100100
	BGP_REG = 0xE4U; // 11100100

	clearSprites();
    selectSetBannerData(selection, 1U, 1U);
    selectSetBannerData(selection, 2U, 1U);
    selectSetBannerTiles(selection, 2U, 10U);

	setMusicBank(SONG_BANK_MAINMENU);
	playMusic(&mainmenu_song_data);

	HIDE_WIN;
	SHOW_SPRITES;
	SHOW_BKG;
	SPRITES_8x8;

	DISPLAY_ON;
	enable_interrupts();
}

void selectUpdateSprites() {
	UBYTE frame;

	setSprite(24U-arrow_offset1, 68U, 37U, OBJ_PAL0);
	setSprite(32U-arrow_offset1, 68U, 39U, OBJ_PAL0);
	setSprite(24U-arrow_offset1, 76U, 38U, OBJ_PAL0);
	setSprite(32U-arrow_offset1, 76U, 40U, OBJ_PAL0);

	setSprite(136U+arrow_offset2, 68U, 39U, OBJ_PAL0 | FLIP_X);
	setSprite(144U+arrow_offset2, 68U, 37U, OBJ_PAL0 | FLIP_X);
	setSprite(136U+arrow_offset2, 76U, 40U, OBJ_PAL0 | FLIP_X);
	setSprite(144U+arrow_offset2, 76U, 38U, OBJ_PAL0 | FLIP_X);

	if(levels_completed >= 3U) {
		switch(select_cat_state) {
			case CAT_ANIM_OFF:
				if((ticks & 15U) == 15U) {
					if(cat_frame_reverse) {
						cat_frame--;
					} else {
						cat_frame++;
					}
					if(cat_frame == 0U) cat_frame_reverse = 0U;
					else if(cat_frame == 4U) cat_frame_reverse = 1U;
				}
				if(CLICKED(J_SELECT)) {
					select_cat_state = CAT_ANIM_IN;
					player_skin = 2U;
					playSound(SFX_CAT_ENABLE);
				}
				break;
			case CAT_ANIM_IN:
				if((ticks & 7U) == 7U) cat_frame++;
				if(cat_frame == 8U) select_cat_state = CAT_ANIM_ON;
				break;
			case CAT_ANIM_ON:
				if((ticks & 15U) == 15U) cat_frame++;
				if(cat_frame == 10U) cat_frame = 8U;
				if(CLICKED(J_SELECT)) {
					select_cat_state = CAT_ANIM_OUT;
					player_skin = 1U;
					playSound(SFX_CAT_DISABLE);
				}
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
		setSprite(136U, 20U, frame++, OBJ_PAL0 | 1U);
		setSprite(144U, 20U, frame++, OBJ_PAL0 | 1U);
		setSprite(136U, 28U, frame++, OBJ_PAL0 | 1U);
		setSprite(144U, 28U, frame, OBJ_PAL0 | 1U);
	}
}

void selectScrollCircles() {
	circle_index = (circle_index+1U) & 7U;
	set_bkg_data(13U, 1U, &circles_data[(circle_index << 4)]);
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
			if((ticks & 3U) == 3U) selectScrollCircles();
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
			selection++;
			select_scroll_dir = RIGHT;
			if(selection == 5U && levels_completed < 2U) selection++;
			if(selection > 6U) selection = 1U;
			playSound(SFX_MENU_SWITCH);
			selectFadeOut();
			selectFadeIn();
			selectUpdateSprites();
		}
		if(ISDOWN(J_LEFT)) {
			selection--;
			select_scroll_dir = LEFT;
			if(selection == 5U && levels_completed < 2U) selection--;
			if(selection == 0U) selection = 6U;
			playSound(SFX_MENU_SWITCH);
			selectFadeOut();
			selectFadeIn();
			selectUpdateSprites();
		}
		if(CLICKED(J_START) || CLICKED(J_A)) {
			if(selection == 5U) {
				gamestate = GAMESTATE_JUKEBOX;
				playSound(SFX_MENU_CONFIRM);
			} else if(selection == 6U) {
				gamestate = GAMESTATE_HIGHSCORE;
				playSound(SFX_MENU_CONFIRM);
			} else {
				if(selection <= levels_completed+1U) {
					level = selection;
					if(ISDOWN(J_UP)) level = 5U;
					gamestate = GAMESTATE_INGAME;
					playSound(SFX_MENU_CONFIRM);
				} else {
					playSound(SFX_MENU_LOCKED);
				}
			}
		}
		if(CLICKED(J_B)) {
			gamestate = GAMESTATE_TITLE;
			playSound(SFX_MENU_CANCEL);
		}

		// Draw level name
		if(selection <= 4U && selection > levels_completed+1U) {
			name_index = 0U;
		} else {
			name_index = selection;
		}

		offset = 64U;
		if(level_names[name_index][5] == 10U) {
			offset += 4U;
		}
		for(i = 0U; i != 6; ++i) {
			setSprite(offset+(i << 3), 70U+cos4_16[(i+(ticks >> 1)) & 15U], level_names[name_index][i], OBJ_PAL0);
		}

		selectUpdateSprites();
		clearRemainingSprites();
		snd_update();
		wait_vbl_done();
	}

	STOP_MUSIC;
	clearRemainingSprites(); // Remove all sprites
	fadeToWhite(4U);
	wait_sound_done();
}
