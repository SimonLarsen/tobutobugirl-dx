#include <gb/gb.h>
#include <gb/cgb.h>
#include <string.h>
#include "defines.h"
#include "gamestate.h"
#include "set_data_rle.h"
#include "fade.h"
#include "winscreen.h"
#include "sound.h"
#include "sgb_send_packet.h"

#include "characters.h"
#include "circles.h"
#include "data/palettes/ranks.h"
#include "data/bg/win_base.h"
#include "data/bg/win_base_dx.h"
#include "data/bg/rank_banner.h"
#include "data/bg/rank_banner_dx.h"
#include "data/sprite/ranks.h"
#include "mmlgb/driver/music.h"
#include "mmlgb/driver/notes.h"
#include "mmlgb/driver/freq.h"

extern UBYTE score_tally_song_data;

const UBYTE winscreen_clear_text[5] = { 13U, 22U, 15U, 11U, 28U };
const UBYTE winscreen_time_text[4] = { 30U, 19U, 23U, 15U };
const UBYTE winscreen_stomps_text[6] = { 29U, 30U, 25U, 23U, 26U, 29U };
const UBYTE winscreen_total_text[5] = { 30U, 25U, 30U, 11U, 22U };
const UBYTE winscreen_wave_text[4] = { 33U, 11U, 32U, 15U };

const UBYTE sharkwave_data[16] = {
	1U, 35U, 69U, 103U, 138U, 166U, 205U, 239U, 255U, 134U, 67U, 50U, 162U, 17U, 16U, 0U
};

const UBYTE SGB_WINSCREEN_PAL01[16] = {
    1,
    255, 127, 233, 113, 92,  57,   0,   0, 220,  53, 146,  12,   0,   0,
    0
};

const UBYTE SGB_WINSCREEN_PAL23[16] = {
    (1 << 3) + 1,
    255, 127,  62,  53, 37,  89,   0,   0, 0,   0, 0,   0,   0,   0,
    0
};

const UBYTE SGB_WINSCREEN_ATTRBLK[16] = {
    (4 << 3) + 1,
    2,
    7,
    1 | (1 << 2),
    0, 3,
    19, 16,
    0, 0, 0, 0, 0, 0, 0, 0
};

const UBYTE SGB_WINSCREEN_ATTRBLK2[16] = {
    (4 << 3) + 1,
    1,
    // data set 1
    3, // inside and surrounding
    2 | (2 << 2),
    0, 6,
    19, 11,
    //
    0, 0, 0, 0, 0, 0, 0, 0
};

const UBYTE winscreen_shake[6] = {
    4U, 252U, 2U, 254U, 1U, 255U
};

void initWinscreen() {
	UBYTE tile;
	UBYTE *data;

	disable_interrupts();
	DISPLAY_OFF;

    if(sgb_mode) {
        sgb_send_packet(SGB_WINSCREEN_PAL01); delay(62U);
        sgb_send_packet(SGB_WINSCREEN_PAL23); delay(62U);
        sgb_send_packet(SGB_WINSCREEN_ATTRBLK);
    }

	BGP_REG = 0xE4U; // 11100100
	OBP0_REG = 0xD0U; // 11010000

	ticks = 0U;
	circle_index = 0U;
	move_bkg(0U, 0U);
	clearSprites();

	set_bkg_data(0U, 40U, characters_data);

    if(CGB_MODE) {
	    set_bkg_data_rle(win_base_dx_tiles_offset, win_base_dx_data_length, win_base_dx_data);
        set_bkg_data_rle(rank_banner_dx_tiles_offset, rank_banner_dx_data_length, rank_banner_dx_data);
	    set_bkg_tiles_rle(0U, 0U, win_base_dx_tiles_width, win_base_dx_tiles_height, win_base_dx_tiles);
        set_bkg_palette_buffer(0U, win_base_dx_palette_data_length, win_base_dx_palette_data);
        VBK_REG = 1U;
	    set_bkg_tiles_rle(0U, 0U, win_base_dx_tiles_width, win_base_dx_tiles_height, win_base_dx_palettes);
        VBK_REG = 0U;
        set_sprite_palette(0U, ranks_palette_data_length, ranks_palette_data);
    } else {
	    set_bkg_data_rle(win_base_tiles_offset, win_base_data_length, win_base_data);
        set_bkg_data_rle(rank_banner_tiles_offset, rank_banner_data_length, rank_banner_data);
	    set_bkg_tiles_rle(0U, 0U, win_base_tiles_width, win_base_tiles_height, win_base_tiles);
    }

	setWinscreenBackground(level);

	// Load rank sprite
	tile = getRank(TOTAL_SCORE, level);
	data = ranks_data + ((UWORD)tile << 8);
	set_sprite_data(0U, 16U, data);

	// Set level name
	data = level_names[level];
    set_bkg_tiles((level == 5U ? 7U : 4U), 1U, 6U, 1U, data);

    if(level != 5U) {
        set_bkg_tiles(11U, 1U, 5U, 1U, winscreen_clear_text);

        *((UBYTE*)0x98C2UL) = 39U; // (2,  6)
        *((UBYTE*)0x9962UL) = 39U; // (2, 11)

        mymemset((UBYTE*)0x9901UL, 0x30U, 7U); // (1,  8) -- (8,  8)
        mymemset((UBYTE*)0x99A1UL, 0x30U, 7U); // (1, 12) -- (8, 12)

        set_bkg_tiles(1U,  4U, 4U, 1U, winscreen_time_text);
        set_bkg_tiles(1U,  9U, 6U, 1U, winscreen_stomps_text);
        set_bkg_tiles(1U, 14U, 5U, 1U, winscreen_total_text);
    } else {
        mymemset((UBYTE*)0x9901UL, 0x30U, 7U); // (1,  8) -- (8,  8)
        mymemset((UBYTE*)0x9981UL, 0x30U, 7U); // (1, 11) -- (8, 11)

        set_bkg_tiles(1U,  5U, 4U, 1U, winscreen_wave_text);
        set_bkg_tiles(1U,  9U, 4U, 1U, winscreen_time_text);
        set_bkg_tiles(1U, 13U, 6U, 1U, winscreen_stomps_text);
    }

	HIDE_WIN;
	SHOW_BKG;
	SHOW_SPRITES;
	SPRITES_8x16;

	clearSprites();

	DISPLAY_ON;
	enable_interrupts();
}

void winscreenWait(UBYTE steps) {
	for(; steps != 0U; --steps) {
		snd_update();
		winscreenScrollCircles();
		wait_vbl_done();
	}
}

void winscreenScrollCircles() {
	ticks++;
	if((ticks & 3U) == 3U) {
		circle_index = (circle_index+1U) & 7U;
		set_bkg_data(40U, 1U, &circles_data[(circle_index << 4)]);
	}
}

void winscreenPlayNote(UBYTE note, UBYTE octave) {
	UWORD freq3;

	freq3 = freq[(octave << 4) + note - MUS_FIRST_NOTE];

	NR30_REG = 0x0U;
	NR30_REG = 0x80U;
	NR32_REG = 0xFFU;
	NR33_REG = (UBYTE)freq3;
	NR34_REG = 0x80U | (freq3 >> 8);
}

void winscreenJingle() {
	winscreenWait(2U);
	winscreenPlayNote(T_Fs, 6U); winscreenWait(3U);
	winscreenPlayNote(T_A,  6U); winscreenWait(3U);
	winscreenPlayNote(T_E,  7U); winscreenWait(3U);
	NR30_REG = 0x0U;
}

void winscreenTextJingle() {
	winscreenWait(2U);
	winscreenPlayNote(T_E,  6U); winscreenWait(4U);
	winscreenPlayNote(T_B,  6U); winscreenWait(4U);
	winscreenPlayNote(T_A,  7U); winscreenWait(4U);
	winscreenPlayNote(T_E,  7U); winscreenWait(4U);
	winscreenPlayNote(T_Cs, 8U); winscreenWait(5U);
	NR30_REG = 0x0U;
}

void winscreenShowRank() {
	UBYTE i, offset;
    UBYTE buf[3];

	disable_interrupts();

    sgb_send_packet(SGB_WINSCREEN_ATTRBLK2);

    if(CGB_MODE) {
	    set_bkg_tiles_rle(0U, 6U, 20U, 6U, rank_banner_dx_tiles);
        VBK_REG = 1U;
        buf[0] = 1U; buf[1] = 1U; buf[2] = 120U;
	    set_bkg_tiles_rle(0U, 6U, 20U, 6U, buf);
        VBK_REG = 0U;
    } else {
	    set_bkg_tiles_rle(0U, 6U, 20U, 6U, rank_banner_tiles);
    }
	enable_interrupts();

	winscreenWait(15U);

	for(offset = 64U; offset != 252U; offset -= 4U) {
		setSprite(100U-offset, 72U-offset, 0U, OBJ_PAL0);
		setSprite(108U-offset, 72U-offset, 2U, OBJ_PAL0);
		setSprite(116U+offset, 72U-offset, 4U, OBJ_PAL0);
		setSprite(124U+offset, 72U-offset, 6U, OBJ_PAL0);

		setSprite(100U-offset, 88U+offset,  8U, OBJ_PAL0);
		setSprite(108U-offset, 88U+offset, 10U, OBJ_PAL0);
		setSprite(116U+offset, 88U+offset, 12U, OBJ_PAL0);
		setSprite(124U+offset, 88U+offset, 14U, OBJ_PAL0);

		winscreenWait(1U);
	}

	clearRemainingSprites();

	setSprite(100U, 72U, 0U, OBJ_PAL0);
	setSprite(108U, 72U, 2U, OBJ_PAL0);
	setSprite(116U, 72U, 4U, OBJ_PAL0);
	setSprite(124U, 72U, 6U, OBJ_PAL0);

	setSprite(100U, 88U, 8U, OBJ_PAL0);
	setSprite(108U, 88U, 10U, OBJ_PAL0);
	setSprite(116U, 88U, 12U, OBJ_PAL0);
	setSprite(124U, 88U, 14U, OBJ_PAL0);

	playSound(SFX_RANK_CRASH);

	// shake screen
	for(i = 0; i != 6U; ++i) {
		move_bkg(0U, winscreen_shake[i]);
		winscreenWait(2U);
	}

	move_bkg(0U, 0U);

	winscreenWait(8U);
}

void countUpScore(UBYTE x, UBYTE y, UBYTE value) {
	UBYTE i, j;

	j = 0U;
	for(i = 0U; i != value+1U; ++i) {
		updateJoystate();
		if(CLICKED(J_A) || CLICKED(J_B) || CLICKED(J_START)) {
			i = value;
		}
		switch(j) {
			case 0U: winscreenPlayNote(T_C, 6U); break;
			case 1U: winscreenPlayNote(T_D, 7U); break;
			case 2U: winscreenPlayNote(T_A, 6U); break;
		}

		j++;
		if(j == 3U) j = 0U;

        disable_interrupts();
		drawScore8(x, y, i);
        enable_interrupts();
		winscreenWait(2U);
	}
}

void showWinscreenNormal() {
	UBYTE tmp;

	// Time
    disable_interrupts();
    drawTime8(4U, 5U, elapsed_time);
    enable_interrupts();

	winscreenTextJingle();
	winscreenWait(30U);

	// Count up time bonus
	countUpScore(7U, 6U, TIME_BONUS);

	winscreenJingle();
	winscreenWait(30U);

	// Kills
    tmp = 1U;
    if((UBYTE)kills >= 10U) tmp++;
    if((UBYTE)kills >= 100U) tmp++;
    disable_interrupts();
    drawNumber8(tmp, 10U, (UBYTE)kills);
    enable_interrupts();

	winscreenTextJingle();
	winscreenWait(30U);

	// Count up kill bonus
	countUpScore(7U, 11U, KILL_BONUS);
	winscreenJingle();

	winscreenWait(30U);

	// Count up total score
	countUpScore(7U, 15U, TOTAL_SCORE);
	winscreenJingle();

	while(1U) {
		updateJoystate();
		if(CLICKED(J_A) || CLICKED(J_B) || CLICKED(J_START)) {
			break;
		}
		winscreenWait(1U);
	}

	winscreenShowRank();
}

void showWinscreenInfinite() {
    disable_interrupts();
    drawNumber16(7U, 6U, wave+1UL);
    enable_interrupts();

    winscreenJingle();
    winscreenWait(30U);

    disable_interrupts();
    drawTime16(7U, 10U, elapsed_time);
    enable_interrupts();

    winscreenJingle();
    winscreenWait(30U);

    disable_interrupts();
    drawNumber16(7U, 14U, kills);
    enable_interrupts();

    winscreenJingle();
}

void enterWinscreen() {
	initWinscreen();

	fadeFromWhite(8U);

	disable_interrupts();
	// Did we come from end of ending sequence?
	if(ending_flags & ENDING_FLAG_ENDING_FINISHED) {
		mus_setPaused(0U);
	// Else just play regular music
	} else {
		setMusicBank(SONG_BANK_SCORE_TALLY);
		playMusic(&score_tally_song_data);

	    // Set wave channel data
	    NR30_REG = 0U;
	    memcpy((UBYTE*)0xFF30, sharkwave_data, 16U);
	}
	enable_interrupts(); 

	winscreenWait(15U);

    if(level != 5U) {
        showWinscreenNormal();
    } else {
        showWinscreenInfinite();
    }

	while(1U) {
		updateJoystate();
		if(CLICKED(J_A) || CLICKED(J_B) || CLICKED(J_START)) {
			if(unlocked_bits) {
				gamestate = GAMESTATE_UNLOCKED;
			} else {
				gamestate = GAMESTATE_HIGHSCORE;
			}
			break;
		}
		winscreenWait(1U);
	}

	clearSprites();
	fadeToWhite(12U);

	STOP_MUSIC;
}
