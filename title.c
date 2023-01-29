#include <gb/gb.h>
#include <gb/cgb.h>
#include <rand.h>
#include "defines.h"
#include "title.h"
#include "fade.h"
#include "gamestate.h"
#include "set_data_rle.h"
#include "sound.h"
#include "ram.h"
#include "mmlgb/driver/music.h"
#include "sgb_send_packet.h"

#include "data/palettes/title_sprites.h"
#include "data/palettes/minigame_fade.h"
#include "data/bg/titlescreen.h"
#include "data/bg/titlescreen_sgb.h"
#include "data/bg/titlescreen_dx.h"
#include "data/bg/titlescreen_bg.h"
#include "characters.h"
#include "data/sprite/title_cat.h"

extern UBYTE title_song_data;
extern UBYTE minigame_song_data;

const UBYTE title_message[11] = {
	26U, 28U, 15U, 29U, 29U, 10U, 29U, 30U, 11U, 28U, 30U
};

const UBYTE minigame_bkg_palettes[6] = {
	0U, 0U, 180U, 0U, 0U, 180U
};

const UBYTE SGB_TITLE_PAL01[16] = {
	1U, 255U, 127U, 60U, 90U, 168U, 101U, 0U, 0U, 233U, 113U, 173U, 53U, 0U, 0U, 0U
};

const UBYTE SGB_TITLE_ATTRDIV[16] = {
	(6U << 3) + 1U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U
};

UBYTE next_enemy;

void initTitle() {
	UBYTE i;

	disable_interrupts();
	DISPLAY_OFF;

	if(sgb_mode) {
		sgb_send_packet(SGB_TITLE_PAL01); delay(62U);
		sgb_send_packet(SGB_TITLE_ATTRDIV);
	}

	move_win(7U, 0U);
	move_bkg(0U, 72U);

	if(CGB_MODE) {
		set_bkg_data_rle(0U, titlescreen_dx_data_length, titlescreen_dx_data);
		set_win_tiles_rle(0U, 0U, titlescreen_dx_tiles_width, titlescreen_dx_tiles_height, titlescreen_dx_tiles);
		set_bkg_palette_buffer(0U, titlescreen_dx_palette_data_length, titlescreen_dx_palette_data);
		VBK_REG = 1U;
		set_win_tiles_rle(0U, 0U, titlescreen_dx_tiles_width, titlescreen_dx_tiles_height, titlescreen_dx_palettes);
		set_bkg_tiles_rle(0U, 0U, 20U, 20U, minigame_bkg_palettes);
		VBK_REG = 0U;
	} else if(sgb_mode) {
		set_bkg_data_rle(0U, titlescreen_sgb_data_length, titlescreen_sgb_data);
		set_win_tiles_rle(0U, 0U, titlescreen_sgb_tiles_width, titlescreen_sgb_tiles_height, titlescreen_sgb_tiles);
	} else {
		set_bkg_data_rle(0U, titlescreen_data_length, titlescreen_data);
		set_win_tiles_rle(0U, 0U, titlescreen_tiles_width, titlescreen_tiles_height, titlescreen_tiles);
	}

	set_bkg_data_rle(titlescreen_bg_tiles_offset, titlescreen_bg_data_length, titlescreen_bg_data);
	set_bkg_tiles_rle(0U, 0U, titlescreen_bg_tiles_width, titlescreen_bg_tiles_height, titlescreen_bg_tiles);

	set_sprite_data(0U, 37U, characters_data);
	set_sprite_data(38U, title_cat_data_length, title_cat_data);
	set_sprite_palette(0U, title_sprites_palette_data_length, title_sprites_palette_data);

	OBP0_REG = 0xD0U; // 11010000
	BGP_REG = 0xE4U;  // 11100100
	SPRITES_8x8;

	clearSprites();

	SHOW_WIN;
	SHOW_SPRITES;
	SHOW_BKG;

	ticks = 0U;
	timer = 0U;
	next_spawn = 0U;
	elapsed_time = 0UL;

	player_x = 220U;
	player_y = 40U;
	player_xdir = LEFT;
	scene_state = TITLE_SCROLL;
	player_xspeed = 128U;
	player_yspeed = 128U;
	cat_x = 0U;
	cat_y = 0U;
	scroll_y = 0U;
	scroll_x = 72U;
	next_enemy = 0U;

	for(i = 0U; i != MAX_ENTITIES; ++i) {
		entity_type[i] = 0U;
	}

	DISPLAY_ON;
	enable_interrupts();
}

void updateTitleEnemies() {
	UBYTE i;

	if(next_enemy < 8U && !entity_type[next_enemy]) {
		i = next_enemy;
		entity_type[i] = 1U;
		entity_y[i] = player_y;
		if(i & 2U) {
			entity_x[i] = 248U;
			entity_dir[i] = RIGHT;
		} else {
			entity_x[i] = 168U;
			entity_dir[i] = LEFT;
		}
	}

	next_spawn++;
	if(next_spawn == 213U && next_enemy < 8U) {
		next_spawn = 0U;
		next_enemy++;
	}

	i = 0U;
	while(entity_type[i]) {
        if(entity_type[i] == 1U) {
            if(entity_dir[i] == RIGHT) entity_x[i]++;
            else entity_x[i]--;
        } else {
            if(entity_dir[i] == RIGHT) entity_y[i]++;
            else entity_y[i]--;
        }

		if(entity_x[i] <= 240U && entity_x[i] >= 172U) {
			if(!(i & 1U)) {
				entity_type[i] = 2U;
			}

			if(entity_type[i] == 1U) {
				entity_y[i] = player_y + ((UBYTE)rand() & 31U) - 16U;

				if(entity_dir[i] == RIGHT) entity_x[i] = 248U;
				else entity_x[i] = 168U;
			} else {
				entity_x[i] = player_x + ((UBYTE)rand() & 31U) - 16U;

				if(entity_dir[i] == RIGHT) entity_y[i] = 248U;
				else entity_y[i] = 152U;
			}
		}

		if((player_x - entity_x[i] + 11U) <= 22U
		&& (player_y - entity_y[i] + 14U) <= 22U) {
			scene_state = TITLE_DEAD;
			STOP_MUSIC;
			playSound(SFX_PLAYER_DIE);
			timer = 0U;
			player_xspeed = 128U;
			player_yspeed = 128U;
			saveMinigameTime();
		}

		++i;
	}
}

void drawTitleSprites(UBYTE triggered) {
	UBYTE i, j, frame;

	if(scene_state <= TITLE_MOVE) {
		// Draw PUSH START
		if(scroll_y < 48U && (!triggered && (ticks & 63U) < 48U) || (triggered && ticks & 4U)) {
			for(i = 0U; i != 11U; ++i) {
				if(i != 5U) {
					setSprite(48U + (i << 3), 140U+scroll_y, title_message[i], OBJ_PAL0);
				}
			}
		}

		// Draw balloon
		setSprite(player_x, player_y, 39U, OBJ_PAL0 | 1U);
		setSprite(player_x+8, player_y, 41U, OBJ_PAL0 | 1U);

		// Draw cat
		frame = 42U;
		if(ticks & 16U) frame += 4U;

		for(i = 0U; i != 2U; ++i) {
			for(j = 1U; j != 3U; ++j) {
				if(player_xdir == LEFT) {
					setSprite(player_x+(i<<3U), player_y+(j<<3U), frame, OBJ_PAL0 | 1U);
				} else {
					setSprite(player_x+8U-(i<<3U), player_y+(j<<3U), frame, FLIP_X | OBJ_PAL0 | 1U);
				}
				++frame;
			}
		}
	} else {
		if(scene_state == TITLE_MINIGAME) {
			setSprite(player_x, player_y-8U, 38U, OBJ_PAL0 | 1U);
			setSprite(player_x+8, player_y-8U, 40U, OBJ_PAL0 | 1U);

			frame = 42U;
			if(ticks & 16U) frame += 4U;

			if(player_xdir == LEFT) {
				setSprite(player_x, player_y+8, frame, OBJ_PAL0 | 1U);
				setSprite(player_x+8U, player_y+8, frame+2U, OBJ_PAL0 | 1U);
			} else {
				setSprite(player_x+8U, player_y+8, frame, FLIP_X | OBJ_PAL0 | 1U);
				setSprite(player_x, player_y+8, frame+2U, FLIP_X | OBJ_PAL0 | 1U);
			}
		} else {
			if(player_xdir == LEFT) {
				setSprite(player_x, player_y+8, 50U, OBJ_PAL0 | 1U);
				setSprite(player_x+8U, player_y+8, 52U, OBJ_PAL0 | 1U);
			} else {
				setSprite(player_x+8U, player_y+8, 50U, FLIP_X | OBJ_PAL0 | 1U);
				setSprite(player_x, player_y+8, 52U, FLIP_X | OBJ_PAL0 | 1U);
			}

			if(timer <= 12U) {
				setSprite(player_x, player_y-6U, 54U, OBJ_PAL0 | 2U);
				setSprite(player_x+8U, player_y-6U, 56U, OBJ_PAL0 | 2U);
			}
		}

		i = 0U;
		while(entity_type[i]) {
			frame = 50U + (entity_type[i] << 3) + ((ticks & 8U) >> 1);
			if(entity_dir[i] == LEFT) {
				setSprite(entity_x[i], entity_y[i], frame, OBJ_PAL0);
				setSprite(entity_x[i]+8U, entity_y[i], frame+2U, OBJ_PAL0);
			} else {
				setSprite(entity_x[i], entity_y[i], frame+2U, FLIP_X | OBJ_PAL0);
				setSprite(entity_x[i]+8U, entity_y[i], frame, FLIP_X | OBJ_PAL0);
			}

			++i;
		}
	}
}

void saveMinigameTime() {
    UBYTE *d;
	ENABLE_RAM_MBC1;
	SWITCH_RAM_MBC1(0U);

    elapsed_minutes = 0U;
    while(elapsed_time >= 60U) {
        elapsed_minutes++;
        elapsed_time -= 60U;
    }
    elapsed_seconds = elapsed_time;

    if(elapsed_minutes >= 100U) {
        elapsed_minutes = 99U;
        elapsed_seconds = 59U;
    }

    d = ram_data + RAM_MINIGAME_MIN;
	if (elapsed_minutes > d[0] || (elapsed_minutes == d[0] && elapsed_seconds > d[1])) {
		d[0] = elapsed_minutes;
		d[1] = elapsed_seconds;
	}

	DISABLE_RAM_MBC1;
}

void enterTitle() {
	initTitle();

	fadeFromWhite(8U);

	disable_interrupts();
	setMusicBank(SONG_BANK_TITLE);
	playMusic(&title_song_data);
	enable_interrupts();

	while(1U) {
		updateJoystate();

		if(CLICKED(J_START)) {
			initrand(DIV_REG);
			STOP_MUSIC;
			playSound(SFX_MENU_CONFIRM);
			
			for(ticks = 0U; ticks != 32U; ++ticks) {
				drawTitleSprites(1U);
				clearRemainingSprites();
				snd_update();
				wait_vbl_done();
			}

			gamestate = GAMESTATE_SELECT;
			break;
		}
		else if(ISDOWN(J_DOWN) && ISDOWN(J_SELECT) && ISDOWN(J_B)) {
			gamestate = GAMESTATE_WIPE;
			STOP_MUSIC;
			break;
		}

		ticks++;
		timer++;
		if(timer == 60U) {
			timer = 0U;
			elapsed_time++;
		}

		if(elapsed_time == 33U && scene_state <= TITLE_MOVE) {
			gamestate = GAMESTATE_INTRO;
			STOP_MUSIC;
			break;
		}

		// Update cat physics
		if(player_xspeed < 128U) {
			cat_x += (128U - player_xspeed) >> 4U;
		} else {
			cat_x += (player_xspeed - 128U) >> 4U;
		}
		if(cat_x >> 3U) {
			if(player_xspeed < 128U) player_x--;
			else player_x++;
			cat_x &= 7U;
		}

		if(player_yspeed < 128U) {
			cat_y += (128U - player_yspeed) >> 4U;
		} else {
			cat_y += (player_yspeed - 128U) >> 4U;
		}
		if(cat_y >> 3U) {
			if(player_yspeed < 128U) player_y--;
			else player_y++;
			cat_y &= 7U;
		}

		if(scene_state == TITLE_SCROLL) {
			player_xspeed = 96U;
			if(player_x <= 152U && ISDOWN(J_LEFT | J_RIGHT | J_UP | J_DOWN)) {
				scene_state = TITLE_MOVE;
			}
		} else if (scene_state == TITLE_DEAD) {
			if(timer == 59U) {
				gamestate = GAMESTATE_MINIGAME_SCORE;
				break;
			}
		} else {
			if(ISDOWN(J_LEFT)) { player_xspeed -= 3U; player_xdir = LEFT; }
			else if(ISDOWN(J_RIGHT)) { player_xspeed += 3U; player_xdir = RIGHT; }
			else {
				if(player_xspeed < 128U) player_xspeed++;
				else if(player_xspeed > 128U) player_xspeed--;
			}

			if(ISDOWN(J_UP)) player_yspeed -= 3U;
			else if(ISDOWN(J_DOWN)) player_yspeed += 3U;
			else {
				if(player_yspeed < 128U) player_yspeed ++;
				else if(player_yspeed > 128U) player_yspeed--;
			}

			if(player_x < 7U) {
				player_x = 7U;
				player_xspeed = 128U + ((128U - player_xspeed) >> 1);
			}
			else if(player_x > 153U) {
				player_x = 153U;
				player_xspeed = 128U - ((player_xspeed - 128U) >> 1);
			}

			if(player_y > 140U) {
				player_y = 140U;
				player_yspeed = 128U - ((player_yspeed - 128U) >> 1);
			}

			// bottom
			if(scene_state == TITLE_MOVE) {
				if(player_y > 113U && scroll_y != 0U) {
					player_y--;
					scroll_y--;
					if(!(scroll_y & 1U)) {
						scroll_x++;
					}
				}

				if(player_y < 30U) {
					player_y++;
					scroll_y++;
					if(!(scroll_y & 1U)) {
						scroll_x--;
					}
				}

				if(scroll_y >= 72U) {
					scene_state = TITLE_MINIGAME;
					ticks = 40U;
					elapsed_time = 0UL;
					player_yspeed = 128U;

					disable_interrupts();
					setMusicBank(SONG_BANK_MINIGAME);
					playMusic(&minigame_song_data);
					enable_interrupts();
				}
			}
			// top
			else if(scene_state == TITLE_MINIGAME) {
				if(scroll_y != 144U) {
					if(ticks & 1U) {
						scroll_y++;
						player_y++;
					}
					if((ticks & 3U) == 3U) {
						scroll_x--;
					}
				} else {
					scroll_y = 144U;
					scroll_x = 0U;
					updateTitleEnemies();
				}

				if(CLICKED(J_A) && !snd_isActive1()) {
					playSound(SFX_CAT_ENABLE);
				}

				if(player_y <= 6U) {
					player_y = 7U;
					player_yspeed = 128U + ((128U - player_yspeed) >> 1);
				}
			}
		}

		if(player_xspeed <= 15U) player_xspeed = 16U;
		else if(player_xspeed >= 241U) player_xspeed = 240U;
		if(player_yspeed <= 15U) player_yspeed = 16U;
		else if(player_yspeed >= 241U) player_yspeed = 240U;

		if(scroll_y < 0x40U) {
			palette_buffer[2] = minigame_fade_palette_data[scroll_y >> 1];
			set_bkg_palette(0U, 1U, palette_buffer);
		}

		move_win(7U, scroll_y);
		move_bkg(0U, scroll_x);
		drawTitleSprites(0U);
		clearRemainingSprites();
		snd_update();
		wait_vbl_done();

		if(scene_state == TITLE_MINIGAME) {
			SPRITES_8x16;
		}
	}

	clearRemainingSprites();
	fadeToWhite(4U);
	wait_sound_done();
}
