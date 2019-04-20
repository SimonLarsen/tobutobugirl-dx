#include <gb/gb.h>
#include "music.h"
#include "data/background.h"
#include "data/sprites.h"

extern UBYTE song_data;
UBYTE joystate, oldjoystate;

#define CLICKED(x) ((joystate & x) && (joystate & x) != (oldjoystate & x))
#define RELEASED(x) (!(joystate & x) && (joystate & x) != (oldjoystate & x))
#define ISDOWN(x) (joystate & (x))

UBYTE selection, paused;

void togglePaused() {
	paused = !paused;
	mus_setPaused(paused);
	if(paused) {
		set_sprite_tile(2U, 12U);
		set_sprite_tile(3U, 14U);
	} else {
		set_sprite_tile(2U, 8U);
		set_sprite_tile(3U, 10U);
	}
}

void main() {
	disable_interrupts();

	DISPLAY_OFF;

	selection = 0U;
	paused = 0U;

	// Install music interrupt
	mus_init(&song_data);
	add_TIM(mus_update);

	// Load background
	set_bkg_data(0U, background_data_length, background_data);
	set_bkg_tiles(0U, 0U, background_tiles_width, background_tiles_height, background_tiles);

	set_sprite_data(0U, sprites_data_length, sprites_data);

	set_sprite_tile(0U, 4U);
	set_sprite_tile(1U, 6U);

	set_sprite_tile(2U, 8U);
	set_sprite_tile(3U, 10U);

	set_sprite_tile(4U, 16U);
	set_sprite_tile(5U, 18U);

	set_sprite_tile(6U, 0U);
	set_sprite_tile(7U, 2U);

	move_sprite(0U, 56U, 104U);
	move_sprite(1U, 64U, 104U);

	move_sprite(2U, 80U, 104U);
	move_sprite(3U, 88U, 104U);

	move_sprite(4U, 104U, 104U);
	move_sprite(5U, 112U, 104U);

	move_sprite(6U, 56U, 103U);
	move_sprite(7U, 64U, 103U);

	SHOW_BKG;
	SPRITES_8x16;
	SHOW_SPRITES;
	HIDE_WIN;
	DISPLAY_ON;

	enable_interrupts();
	set_interrupts(TIM_IFLAG | VBL_IFLAG);

	joystate = oldjoystate = 0U;

	while(1U) {
		oldjoystate = joystate;
		joystate = joypad();

		if(CLICKED(J_START)) {
			togglePaused();
		}

		if(CLICKED(J_LEFT)) {
			if(selection == 0U) {
				selection = 2U;
			} else {
				selection--;
			}
			move_sprite(6U, 56U + selection*24U, 103U);
			move_sprite(7U, 64U + selection*24U, 103U);
		}

		if(CLICKED(J_RIGHT)) {
			if(selection == 2U) {
				selection = 0U;
			} else {
				selection++;
			}
			move_sprite(6U, 56U + selection*24U, 103U);
			move_sprite(7U, 64U + selection*24U, 103U);
		}

		if(CLICKED(J_A)) {
			if(selection == 0U) {
				disable_interrupts();
				mus_init(&song_data);
				enable_interrupts();
				paused = 1U;
				togglePaused();
			}
			else if(selection == 1U) {
				togglePaused();
			}
		}

		if(ISDOWN(J_A) && selection == 2U) {
			disable_interrupts();
			if(paused) {
				togglePaused();
			}
			mus_update();
			enable_interrupts();
		}

		wait_vbl_done();
	}
}
