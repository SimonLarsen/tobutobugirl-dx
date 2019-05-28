#include <gb/gb.h>
#include <gb/cgb.h>
#include "defines.h"
#include "gamestate.h"
#include "main.h"
#include "init.h"
#include "sound.h"
#include "mmlgb/driver/music.h"

#include "logos.h"
#include "intro.h"
#include "title.h"
#include "select.h"
#include "game.h"
#include "winscreen.h"
#include "highscore.h"
#include "unlocked.h"
#include "jukebox.h"
#include "ending.h"
#include "wipe.h"
#include "minigamescore.h"

void vbl_update() {
	++vbl_count;
}

void main() {
	DISPLAY_OFF;
	disable_interrupts();

	SWITCH_ROM_MBC1(INIT_BANK);
    sgb_mode = sgb_check2();
    if(sgb_mode) {
		sgb_init();
	}

	initRAM(0U);
	snd_init();

	vbl_count = 0U;
	joystate = oldjoystate = 0U;
	unlocked_bits = 0U;
	player_skin = 1U;
	ending_flags = 0U;

	last_highscore_level = 0U;
	last_highscore_slot = 5U;
	last_progress = 0U;
	selection = level = 1U;
	
	gamestate = GAMESTATE_LOGOS;

	SWITCH_16_8_MODE_MBC1;
	add_TIM(updateMusic);
	add_VBL(vbl_update);
	set_interrupts(TIM_IFLAG | VBL_IFLAG);

	enable_interrupts();

	while(1U) {
		switch(gamestate) {
			case GAMESTATE_LOGOS:
				setGameBank(5U);
				enterLogos();
				break;
			case GAMESTATE_INGAME:
				setGameBank(1U);
				enterGame();
				break;
			case GAMESTATE_TITLE:
				setGameBank(6U);
				enterTitle();
				break;
			case GAMESTATE_SELECT:
				setGameBank(2U);
				enterSelect();
				break;
			case GAMESTATE_HIGHSCORE:
				setGameBank(2U);
				enterHighscore();
				break;
			case GAMESTATE_UNLOCKED:
				setGameBank(2U);
				enterUnlocked();
				break;
			case GAMESTATE_INTRO:
				setGameBank(3U);
				enterIntro();
				break;
			case GAMESTATE_ENDING:
				setGameBank(3U);
				enterEnding();
				break;
			case GAMESTATE_JUKEBOX:
				setGameBank(4U);
				enterJukebox();
				break;
			case GAMESTATE_WINSCREEN:
				setGameBank(5U);
				enterWinscreen();
				break;
			case GAMESTATE_WIPE:
				setGameBank(10U);
				enterWipe();
				break;
			case GAMESTATE_MINIGAME_SCORE:
				setGameBank(8U);
				enterMinigamescore();
				break;
		}
	}
}
