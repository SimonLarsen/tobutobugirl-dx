#include <gb/gb.h>
#include "defines.h"
#include "gamestate.h"
#include "fade.h"
#include "sound.h"
#include "ram.h"
#include "mmlgb/driver/music.h"
#include "sgb_send_packet.h"

#include "characters.h"

const UBYTE pause_text_on[] = { 10U, 25U, 24U };
const UBYTE pause_text_off[] = { 25U, 16U, 16U };

const UBYTE pause_marker_x1[] = { 50U, 18U, 58U };
const UBYTE pause_marker_x2[] = { 111U, 143U, 103U };
const UBYTE pause_marker_y[] = { 116U, 128U, 140U };

const UBYTE SGB_PAUSE_PAL01[16] = {
    1, 255, 127, 127, 26, 136, 56, 0, 0, 28, 49, 28, 49, 0, 0, 0
};

const UBYTE SGB_PAUSE_ATTRDIV[16] = {
    (0x06U << 3) + 1U,
    1U | (1U << 6),
    10U,
    0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U,
};

UBYTE pause_selection;
UBYTE pause_ticks;

void pauseUpdateDashCounter() {
	if(show_dashcounter) {
		set_bkg_tiles(14U, 14U, 3U, 1U, pause_text_on);
	} else {
		set_bkg_tiles(14U, 14U, 3U, 1U, pause_text_off);
	}
}

void initPause() {
    if(sgb_mode) {
        BGP_REG = 0x00U;
        delay(62U);
    }

	disable_interrupts();
	DISPLAY_OFF;

    if(sgb_mode) {
        sgb_send_packet(SGB_PAUSE_PAL01); delay(62U);
        sgb_send_packet(SGB_PAUSE_ATTRDIV); delay(62U);
    }

	set_bkg_data(0, 38U, characters_data);
	setIngameBackground(0U, 1U, 0U);
	setCloudAnimation(player_skin);

	pauseUpdateDashCounter();

    BGP_REG = 0xE4U;

	SHOW_BKG;
	SHOW_SPRITES;
	HIDE_WIN;

	move_bkg(0U, 0U);

	DISPLAY_ON;
	enable_interrupts();
}

UBYTE enterPause() {
	UBYTE i, j, frame;

	initPause();

	pause_selection = 0U;
	pause_ticks = 0U;

	mus_setPaused(0U);

	while(1U) {
		pause_ticks++;
		updateJoystate();
		
		if(CLICKED(J_START)) {
			return 0U;
		}

		if(CLICKED(J_UP)) {
			if(pause_selection != 0U) {
				pause_selection--;
				playSound(SFX_MENU_SWITCH);
			}
		}
		if(CLICKED(J_DOWN)) {
			if(pause_selection != 2U) {
				pause_selection++;
				playSound(SFX_MENU_SWITCH);
			}
		}
		if(CLICKED(J_A) || (pause_selection == 1U && (CLICKED(J_LEFT) || CLICKED(J_RIGHT)))) {
			playSound(SFX_MENU_CONFIRM);
			if(pause_selection == 0U) {
				wait_sound_done();
				return 0U;
			} else if(pause_selection == 1U) {
				ENABLE_RAM_MBC1;
				SWITCH_RAM_MBC1(0);
                show_dashcounter = !show_dashcounter;
                *(ram_data + RAM_DASHCOUNTER) = show_dashcounter;
				DISABLE_RAM_MBC1;

				disable_interrupts();
				pauseUpdateDashCounter();
				enable_interrupts();
			} else if(pause_selection == 2U) {
				wait_sound_done();
				return 3U;
			}
		}

		// Draw cloud animation
		frame = pause_ticks & 48U;
		for(j = 0U; j != 2U; ++j) {
			for(i = 0U; i != 4U; ++i) {
				setSprite(72U+(i << 3U), 48U+(j << 4U), frame, OBJ_PAL0);
				frame += 2U;
			}
		}

		// Draw selection markers
		setSprite(pause_marker_x1[pause_selection], pause_marker_y[pause_selection], 100U, OBJ_PAL0);
		setSprite(pause_marker_x1[pause_selection]+7U, pause_marker_y[pause_selection], 100U, OBJ_PAL0 | FLIP_X);

		setSprite(pause_marker_x2[pause_selection], pause_marker_y[pause_selection], 102U, OBJ_PAL0 | 1U);
		setSprite(pause_marker_x2[pause_selection]+7U, pause_marker_y[pause_selection], 102U, OBJ_PAL0 | FLIP_X | 1U);

		clearRemainingSprites();
		snd_update();
		wait_vbl_done();
	}
}
