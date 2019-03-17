#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <string.h>

void clearSprites();
void updateJoystate();
void setSprite(UBYTE x, UBYTE y, UBYTE tile, UBYTE prop);
void clearRemainingSprites();
void setIngameBackground(UBYTE level);
void setWinscreenBackground(UBYTE level);
void setCloudAnimation(UBYTE skin);
void set_bkg_data_rle(UBYTE first, UBYTE n, UBYTE *data);
void set_bkg_tiles_rle(UBYTE x, UBYTE y, UBYTE width, UBYTE height, UBYTE *tiles);
void set_win_tiles_rle(UBYTE x, UBYTE y, UBYTE width, UBYTE height, UBYTE *tiles);
//void set_bkg_palette_buffer(UBYTE first_palette, UBYTE nbpalettes, UWORD *data);
void playMusic(UBYTE *data);
void updateMusic();
UBYTE getRank(UBYTE score, UBYTE level);

#define setGameBank(i) { game_bank = i; *((UBYTE*)0x2000) = (i); }
#define setMusicBank(i) (music_bank = (i))

#define STOP_MUSIC mus_setPaused(1U)

#define set_bkg_palette_buffer(first_palette, nbpalettes, data) (memcpy(&palette_buffer[(first_palette) << 2], (data), (nbpalettes) << 3))

extern UBYTE vbl_count;
extern UBYTE ticks, paused, timer;
extern UBYTE gamestate, scene_state;
extern UBYTE level, levels_completed;
extern UBYTE selection, sub_selection;
extern UBYTE joystate, oldjoystate;
extern UBYTE next_sprite, sprites_used;
extern UBYTE elapsed_time, elapsed_minutes, remaining_time, kills;
extern UBYTE last_highscore_level, last_highscore_slot;
extern UBYTE last_progress;
extern UBYTE game_bank, music_bank;
extern UBYTE ending_flags;
extern UBYTE unlocked_bits;
extern UBYTE scroll_x, scroll_y;
extern UBYTE player_x, player_y;
extern UBYTE player_xdir, player_ydir;
extern UBYTE player_xspeed, player_yspeed, player_bounce;
extern UBYTE cat_x, cat_y, cat_frame;
extern UBYTE arrow_offset1, arrow_offset2;
extern UBYTE player_skin;
extern UBYTE show_dashcounter;
extern UBYTE circle_index;
extern UBYTE next_spawn, next_clock;
extern UBYTE next_entity;

extern UWORD palette_buffer[32];
extern UWORD palette_buffer2[32];

#define MAX_ENTITIES 10U

extern UBYTE entity_x[MAX_ENTITIES];
extern UBYTE entity_y[MAX_ENTITIES];
extern UBYTE entity_type[MAX_ENTITIES];
extern UBYTE entity_dir[MAX_ENTITIES];

#define CLICKED(x) ((joystate & x) && (joystate & x) != (oldjoystate & x))
#define RELEASED(x) (!(joystate & x) && (joystate & x) != (oldjoystate & x))
#define ISDOWN(x) (joystate & (x))

#define CGB_MODE (_cpu == CGB_TYPE)

#define GAMESTATE_LOGOS           0U
#define GAMESTATE_INTRO           1U
#define GAMESTATE_TITLE           2U
#define GAMESTATE_SELECT          3U
#define GAMESTATE_INGAME          4U
#define GAMESTATE_PAUSE           5U
#define GAMESTATE_WINSCREEN       6U
#define GAMESTATE_HIGHSCORE       7U
#define GAMESTATE_JUKEBOX         8U
#define GAMESTATE_UNLOCKED        9U
#define GAMESTATE_ENDING         10U
#define GAMESTATE_WIPE           11U
#define GAMESTATE_MINIGAME_SCORE 12U

#define UNLOCKED_CLOUDS 1U
#define UNLOCKED_SPACE  2U
#define UNLOCKED_MUSIC  4U
#define UNLOCKED_DREAM  8U

#define TIME_BONUS (3U*(level_max_time[level-1U] - elapsed_time))
#define KILL_BONUS (kills)
#define TOTAL_SCORE (TIME_BONUS + KILL_BONUS)

#define ENDING_FLAG_ENDING_FINISHED 1U
#define ENDING_FLAG_FROM_DREAM      2U

#define GAME_BACKGROUNDS_BANK 7U
#define PAUSE_DATA_BANK 8U 
#define WINSCREEN_BACKGROUNDS_BANK 9U
#define CLOUD_ANIMATIONS_BANK 8U

#define SONG_BANK_TITLE 11U
#define SONG_BANK_MAINMENU 11U
#define SONG_BANK_SCORE_TALLY 11U
#define SONG_BANK_HIGHSCORE 11U
#define SONG_BANK_PLAINS 12U
#define SONG_BANK_CLOUDS 12U
#define SONG_BANK_SPACE 12U
#define SONG_BANK_INTRO 13U
#define SONG_BANK_DREAM 13U
#define SONG_BANK_DREAM_SCORE 13U
#define SONG_BANK_ENDING_PART1 14U
#define SONG_BANK_ENDING_PART2 14U
#define SONG_BANK_POTATO_JINGLE 14U
#define SONG_BANK_TANGRAM_SHINE 14U
#define SONG_BANK_TANGRAM_VOX 14U
#define SONG_BANK_LEVEL_CLEAR 14U
#define SONG_BANK_UNLOCKED 14U
#define SONG_BANK_MINIGAME 15U
#define SONG_BANK_POTAKA 15U

extern const UBYTE fadePals[4];
extern const UBYTE level_names[7][6];
extern const UBYTE level_max_time[4];
extern const UBYTE rank_letters[4];

extern const UWORD gs_palette[4];

#endif
