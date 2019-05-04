#include <gb/gb.h>
#include "defines.h"
#include "jukebox.h"
#include "fade.h"
#include "gamestate.h"
#include "set_data_rle.h"
#include "cos.h"
#include "sound.h"
#include "mmlgb/driver/music.h"
#include "sgb_send_packet.h"

#include "arrow.h"
#include "data/palettes/digital.h"
#include "data/palettes/jukebox_sprites.h"
#include "data/bg/jukebox.h"
#include "data/bg/jukebox_dx.h"
#include "data/sprite/digital.h"
#include "data/sprite/notes.h"
#include "data/sprite/bobblehead.h"
#include "data/sprite/bobblehead_dx.h"

UBYTE jukebox_active;
UBYTE jukebox_playing;
UBYTE jukebox_music_ticks;
UBYTE jukebox_bop;

extern UBYTE intro_song_data;
extern UBYTE title_song_data;
extern UBYTE mainmenu_song_data;
extern UBYTE plains_song_data;
extern UBYTE score_tally_song_data;
extern UBYTE highscore_song_data;
extern UBYTE clouds_song_data;
extern UBYTE space_song_data;
extern UBYTE ending_part1_song_data;
extern UBYTE ending_part2_song_data;
extern UBYTE dream_song_data;
extern UBYTE dream_score_song_data;

#define JUKEBOX_NUM_SONGS 12U

const UBYTE song_names[JUKEBOX_NUM_SONGS][8] = {
	{10U, 19U, 24U, 30U, 28U, 25U, 10U, 10U}, // " INTRO  "
	{10U, 30U, 19U, 30U, 22U, 15U, 10U, 10U}, // " TITLE  "
	{10U, 10U, 23U, 15U, 24U, 31U, 10U, 10U}, // "  MENU  "
	{10U, 26U, 22U, 11U, 19U, 24U, 29U, 10U}, // " PLAINS "

	{10U, 30U, 11U, 22U, 22U, 35U, 10U, 10U}, // " TALLY  "
	{10U, 29U, 13U, 25U, 28U, 15U, 29U, 10U}, // " SCORES "
	{10U, 13U, 22U, 25U, 31U, 14U, 29U, 10U}, // " CLOUDS "

	{10U, 29U, 26U, 11U, 13U, 15U, 10U, 10U}, // " SPACE  "
    {10U, 18U, 15U, 11U, 32U, 15U, 24U, 10U}, // " HEAVEN "

	{10U, 15U, 24U, 14U, 19U, 24U, 17U, 10U}, // " ENDING "
	{10U, 14U, 28U, 15U, 11U, 23U, 10U, 10U}, // " DREAM  "
	{16U, 11U, 28U, 15U, 33U, 15U, 22U, 22U}  // "FAREWELL"
};

const UBYTE jukebox_unlocked[5U] = { 4U, 7U, 9U, 10U, 12U }; 

const UBYTE SGB_JUKEBOX_PAL01[16U] = {
    1U,
    255, 127, 223,  37, 185,  20,   0,   0, 223,  37, 166,  20,   0,   0,
    0U
};

const UBYTE SGB_JUKEBOX_ATTRBLK[16] = {
    (4 << 3) + 1U,
    1U,
    7U, // change all
    1U,
    5U, 10U,
    14U, 14U,
    0U,
    0U, 0U, 0U, 0U, 0U, 0U, 0U
};

void initJukebox() {
    UBYTE i, j;

	disable_interrupts();
	DISPLAY_OFF;

	remove_TIM(updateMusic);
	add_TIM(jukeboxUpdateMusic);

	move_bkg(0U, 0U);

    set_sprite_data(0U, arrow_data_length, arrow_data);
    set_sprite_data(arrow_data_length, notes_data_length, notes_data);
    set_bkg_data(0U, 37U, digital_data);

    if(CGB_MODE) {
        set_sprite_data(arrow_data_length+notes_data_length, bobblehead_dx_data_length, bobblehead_dx_data);
        set_sprite_palette(0U, jukebox_sprites_palette_data_length, jukebox_sprites_palette_data);
        set_bkg_data_rle(jukebox_dx_tiles_offset, jukebox_dx_data_length, jukebox_dx_data);
        set_bkg_tiles_rle(0U, 0U, jukebox_dx_tiles_width, jukebox_dx_tiles_height, jukebox_dx_tiles);
        set_bkg_palette_buffer(0U, jukebox_dx_palette_data_length, jukebox_dx_palette_data);
        set_bkg_palette_buffer(7U, digital_palette_data_length, digital_palette_data);
        VBK_REG = 1U;
        set_bkg_tiles_rle(0U, 0U, jukebox_dx_tiles_width, jukebox_dx_tiles_height, jukebox_dx_palettes);
        j = 7U;
        for(i = 6U; i != 14U; ++i) set_bkg_tiles(i, 12U, 1U, 1U, &j);
        VBK_REG = 0U;
    } else {
        set_sprite_data(arrow_data_length+notes_data_length, bobblehead_data_length, bobblehead_data);

        set_bkg_data_rle(jukebox_tiles_offset, jukebox_data_length, jukebox_data);
        set_bkg_tiles_rle(0U, 0U, jukebox_tiles_width, jukebox_tiles_height, jukebox_tiles);
    }

	OBP0_REG = 0xD0U; // 11010000
	OBP1_REG = 0xE4; // 11100100
	BGP_REG = 0xE4U; // 11100100

	sub_selection = 0U;
	ticks = 0U;
	jukebox_active = 0U;
	sub_selection = 0U;
	jukebox_music_ticks = 0U;
	jukeboxUpdateTitle();

	clearSprites();

    if(sgb_mode) {
        sgb_send_packet(SGB_JUKEBOX_PAL01); delay(62U);
        sgb_send_packet(SGB_JUKEBOX_ATTRBLK);
    }

	HIDE_WIN;
	SHOW_SPRITES;
	SHOW_BKG;
	SPRITES_8x16;

	DISPLAY_ON;
	enable_interrupts();
}

void jukeboxUpdateMusic() {
	if(!jukebox_active) return;

	updateMusic();
	jukebox_music_ticks++;
	if(jukebox_music_ticks == 24U) {
		jukebox_music_ticks = 0U;
		jukebox_bop++;
	}
}

void jukeboxUpdateSprites() {
	UBYTE offset;
	offset = cos32_64[(ticks & 63U)] >> 3;

	// Left arrow
	setSprite(36U-offset, 108U, 0U, OBJ_PAL0 | 2U);
	setSprite(44U-offset, 108U, 2U, OBJ_PAL0 | 2U);

	// Right arrow
	setSprite(124U+offset, 108U, 2U, OBJ_PAL0 | FLIP_X | 2U);
	setSprite(132U+offset, 108U, 0U, OBJ_PAL0 | FLIP_X | 2U);

	if(jukebox_active) {
		// Small notes
		offset = jukebox_bop & 1U;
		setSprite(11U, 94U+offset, 4U, OBJ_PAL0 | 3U);
		setSprite(145U, 18U+offset, 4U, OBJ_PAL0 | 3U);

		// Double note
		offset = (jukebox_bop & 1U) ^ 1U;
		setSprite(12U, 19U+offset, 8U, OBJ_PAL0 | 3U);
		setSprite(20U, 19U+offset, 10U, OBJ_PAL0 | 3U);
	}

	// Bobble head figurine
	offset = jukebox_bop & 3U;
	if(offset == 1U) {
		offset = 12U;
	} else if(offset == 3U) {
		offset = 36U;
	} else {
		offset = 24U;
	}
	if(offset == 3U) offset = 0U;
	setSprite(135U, 34U, offset, OBJ_PAL1);
	setSprite(143U, 34U, offset+2U, OBJ_PAL1);
	setSprite(151U, 34U, offset+4U, OBJ_PAL1);
	setSprite(135U, 50U, offset+6U, OBJ_PAL1 | 1U);
	setSprite(143U, 50U, offset+8U, OBJ_PAL1 | 1U);
	setSprite(151U, 50U, offset+10U, OBJ_PAL1 | 1U);
}

void jukeboxUpdateTitle() {
	// 7,12
	set_bkg_tiles(6U, 12U, 8U, 1U, &song_names[sub_selection]);
}

void enterJukebox() {
	initJukebox();

	fadeFromWhite(4U);

	while(1U) {
		updateJoystate();

		ticks++;

		if(mus_is_done() && jukebox_playing == 8U) {
			jukebox_playing = 42U;
			disable_interrupts();
			STOP_MUSIC;
			setMusicBank(SONG_BANK_ENDING_PART2);
			playMusic(&ending_part2_song_data);
			enable_interrupts();
		}

		if(CLICKED(J_LEFT)) {
			if(sub_selection == 0U) {
				sub_selection = jukebox_unlocked[levels_completed] - 1U;
			} else {
				sub_selection--;
			}

			jukeboxUpdateTitle();
			playSound(SFX_MENU_CONFIRM);
		}
		if(CLICKED(J_RIGHT)) {
			sub_selection++;
			if(sub_selection >= jukebox_unlocked[levels_completed]) {
				sub_selection = 0U;
			}

			jukeboxUpdateTitle();
			playSound(SFX_MENU_CONFIRM);
		}
		if(CLICKED(J_A) || CLICKED(J_START)) {
			STOP_MUSIC;
			wait_sound_done();
			disable_interrupts();
			switch(sub_selection) {
				case 0U:
					setMusicBank(SONG_BANK_INTRO);
					playMusic(&intro_song_data);
					break;
				case 1U:
					setMusicBank(SONG_BANK_TITLE);
					playMusic(&title_song_data);
					break;
				case 2U:
					setMusicBank(SONG_BANK_MAINMENU);
					playMusic(&mainmenu_song_data);
					break;
				case 3U:
					setMusicBank(SONG_BANK_PLAINS);
					playMusic(&plains_song_data);
					break;
				case 4U:
					setMusicBank(SONG_BANK_SCORE_TALLY);
					playMusic(&score_tally_song_data);
					break;
				case 5U:
					setMusicBank(SONG_BANK_HIGHSCORE);
					playMusic(&highscore_song_data);
					break;
				case 6U:
					setMusicBank(SONG_BANK_CLOUDS);
					playMusic(&clouds_song_data);
					break;
				case 7U:
					setMusicBank(SONG_BANK_SPACE);
					playMusic(&space_song_data);
					break;
				case 8U:
					setMusicBank(SONG_BANK_SPACE);
					playMusic(&space_song_data);
					break;
				case 9U:
					setMusicBank(SONG_BANK_ENDING_PART1);
					playMusic(&ending_part1_song_data);
					break;
				case 10U:
					setMusicBank(SONG_BANK_DREAM);
					playMusic(&dream_song_data);
					break;
				case 11U:
					setMusicBank(SONG_BANK_DREAM_SCORE);
					playMusic(&dream_score_song_data);
					break;
			}
			enable_interrupts();
			jukebox_music_ticks = 0U;
			jukebox_bop = 1U;
			jukebox_active = 1U;
			jukebox_playing = sub_selection;
		}
		if(CLICKED(J_B)) {
			gamestate = GAMESTATE_SELECT;
			playSound(SFX_MENU_CANCEL);
			break;
		}

		jukeboxUpdateSprites();
		clearRemainingSprites();
		snd_update();
		wait_vbl_done();
	}

	STOP_MUSIC;
	clearRemainingSprites();
	fadeToWhite(4U);
	wait_sound_done();

	remove_TIM(jukeboxUpdateMusic);
	add_TIM(updateMusic);
}
