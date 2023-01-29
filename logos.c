#include <gb/gb.h>
#include <gb/cgb.h>
#include <string.h>
#include "defines.h"
#include "gamestate.h"
#include "set_data_rle.h"
#include "fade.h"
#include "logos.h"
#include "sound.h"
#include "mmlgb/driver/music.h"
#include "sgb_send_packet.h"

#include "data/bg/tangram.h"
#include "data/bg/potato.h"
#include "data/bg/potato_dx.h"
#include "data/sprite/shine.h"

extern UBYTE tangram_shine_song_data;
extern UBYTE tangram_vox_song_data;
extern UBYTE potato_jingle_song_data;

const UWORD tangram_flash_palette[4] = {
	32767UL, 32767UL, 32767UL, 32767UL // pure white
};

const UBYTE SGB_LOGOS_PAL01[16] = {
	1U, 255U, 127U, 216U, 45U, 11U, 29U, 0U, 0U, 92U, 54U, 214U, 40U, 0U, 0U, 0U
};

const UBYTE SGB_LOGOS_ATTRDIV1[16] = {
	(6U << 3) + 1U,
	1U | (1U << 2) | (1U << 4),
	0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U
};

void initLogos() {
	disable_interrupts();
	DISPLAY_OFF;

	if(sgb_mode) {
		sgb_send_packet(SGB_LOGOS_PAL01);
	}

	OBP0_REG = 0xD0U; // 11010000
	BGP_REG = 0xE4U; // 11100100

	HIDE_WIN;
	SHOW_SPRITES;
	SHOW_BKG;
	SPRITES_8x16;

	set_bkg_data_rle(0U, tangram_data_length, tangram_data);
	set_bkg_tiles_rle(0U, 0U, tangram_tiles_width, tangram_tiles_height, tangram_tiles);

	if(CGB_MODE) {
		set_sprite_palette(0U, 1U, tangram_flash_palette);
		set_bkg_palette_buffer(0U, tangram_palette_data_length, tangram_palette_data);
		VBK_REG = 1U;
		set_bkg_tiles_rle(0U, 0U, tangram_tiles_width, tangram_tiles_height, tangram_palettes);
		VBK_REG = 0U;
	}

	set_sprite_data(0U, shine_data_length, shine_data);

	DISPLAY_ON;
	enable_interrupts();
}

void enterLogos() {
	UBYTE i;

	initLogos();

	fadeFromWhite(8U);

	for(i = 0U; i != 8U; ++i) wait_vbl_done();

	setMusicBank(SONG_BANK_TANGRAM_VOX);
	disable_interrupts();
	playMusic(&tangram_vox_song_data);
	enable_interrupts();

	for(i = 0U; i != 60U; ++i) wait_vbl_done();
	STOP_MUSIC;

	setMusicBank(SONG_BANK_TANGRAM_SHINE);
	disable_interrupts();
	playMusic(&tangram_shine_song_data);
	enable_interrupts();

	for(i = 10U; i != 142U; i += 6U) {
		if(i < 64U) {
			setSprite(i+8U, 80U, 0U, OBJ_PAL0);
			setSprite(i+16U, 80U, 2U, OBJ_PAL0);
			setSprite(i+24U, 80U, 4U, OBJ_PAL0);
		} else {
			setSprite(i+4U, 88U, 0U, OBJ_PAL0);
			setSprite(i+12U, 88U, 2U, OBJ_PAL0);
			setSprite(i+20U, 88U, 4U, OBJ_PAL0);
		}

		setSprite(i, 96U, 0U, OBJ_PAL0);
		setSprite(i+8U, 96U, 2U, OBJ_PAL0);
		setSprite(i+16U, 96U, 4U, OBJ_PAL0);

		clearRemainingSprites();
		snd_update();
		wait_vbl_done();
	}

	clearRemainingSprites();

	while(!mus_is_done()) {
		wait_vbl_done();
	}

	STOP_MUSIC;
	fadeToWhite(8U);

	disable_interrupts();
	DISPLAY_OFF;

	if(sgb_mode) {
		sgb_send_packet(SGB_LOGOS_ATTRDIV1);
	}

	if(CGB_MODE) {
		set_bkg_data_rle(0U, potato_dx_data_length, potato_dx_data);
		set_bkg_tiles_rle(0U, 0U, potato_dx_tiles_width, potato_dx_tiles_height, potato_dx_tiles);
		set_bkg_palette_buffer(0U, potato_dx_palette_data_length, potato_dx_palette_data);
		VBK_REG = 1U;
		set_bkg_tiles_rle(0U, 0U, potato_dx_tiles_width, potato_dx_tiles_height, potato_dx_palettes);
		VBK_REG = 0U;
	} else {
		set_bkg_data_rle(0U, potato_data_length, potato_data);
		set_bkg_tiles_rle(0U, 0U, potato_tiles_width, potato_tiles_height, potato_tiles);
	}

	DISPLAY_ON;
	enable_interrupts();

	BGP_REG = 0xE4U;
	fadeFromWhite(8U);

	for(i = 0U; i != 10U; ++i) wait_vbl_done();

	disable_interrupts();
	setMusicBank(SONG_BANK_POTATO_JINGLE);
	playMusic(&potato_jingle_song_data);
	enable_interrupts();

	while(!mus_is_done()) {
		wait_vbl_done();
	}

	STOP_MUSIC;
	fadeToWhite(8U);

	gamestate = GAMESTATE_INTRO;
}
