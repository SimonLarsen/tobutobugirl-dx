#include <gb/gb.h>
#include <gb/cgb.h>
#include <gb/rand.h>
#include "defines.h"
#include "game.h"
#include "fade.h"
#include "gamestate.h"
#include "cos.h"
#include "ram.h"
#include "highscore.h"
#include "sound.h"
#include "mmlgb/driver/music.h"
#include "pause.h"
#include "sgb_send_packet.h"
#include "set_data_rle.h"

#include "data/palettes/sprites.h"
#include "characters.h"
#include "data/bg/clock.h"
#include "data/sprite/sprites.h"
#include "data/sprite/portal.h"
#include "data/sprite/skin1.h"
#include "data/sprite/skin2.h"

UBYTE first_load;
UBYTE scrolled;
UBYTE last_spawn_x, last_spawn_index;

UBYTE progress, progressbar, portal_spawned, repeat_spikes;
UBYTE scrolled_length, allowed_spikes, clock_interval;
UBYTE blips, blip_bar;
UBYTE dashing, dashes, dash_xdir, dash_ydir;
UBYTE ghost_frame;
UBYTE *spawn_levels;
UBYTE spawn_level_gen[8];
UBYTE wave_bg, is_special_wave;

extern UBYTE plains_song_data;
extern UBYTE clouds_song_data;
extern UBYTE space_song_data;
extern UBYTE dream_song_data;
extern UBYTE heaven_song_data;
extern UBYTE level_clear_song_data;

const UBYTE scrolled_length_data[5] = {
    16U, // 16 * 111 / 36 = 49
    24U, // 24 * 111 / 36 = 74
    32U, // 32 * 111 / 36 = 98
    40U, // 40 * 111 / 36 = 123
    8U  // 16 * 111 / 36 = 49
};

const UBYTE clock_interval_data[5] = {
    8U, //  49/8 = 6.125
    10U, //  74/12 = 7.4
    11U, //  98/11 = 8.9
    14U, // 123/14 = 8.85
    255U
};

const UBYTE allowed_spikes_data[4] = { 1U, 1U, 1U, 3U };

const UBYTE spawn_level_data[96] = {
    // Plains
    E_BIRD, E_BIRD, E_BAT, E_BAT, E_BAT, E_BAT, E_BAT, E_BAT,
    E_SPIKES, E_SPIKES, E_BIRD, E_BIRD, E_BAT, E_BAT, E_BAT, E_BAT,
    E_SPIKES, E_SPIKES, E_SPIKES, E_BIRD, E_BIRD, E_BIRD, E_BAT, E_BAT,
    // Clouds
    E_SPIKES, E_SPIKES, E_SPIKES, E_BIRD, E_BIRD, E_BAT, E_BAT, E_BAT,
    E_SPIKES, E_SPIKES, E_ALIEN, E_ALIEN, E_BIRD, E_BIRD, E_BAT, E_BAT,
    E_FIREBALL, E_FIREBALL, E_ALIEN, E_ALIEN, E_BIRD, E_BIRD, E_BAT, E_BAT,
    // Space
    E_SPIKES, E_SPIKES, E_ALIEN, E_ALIEN, E_BIRD, E_BIRD, E_BAT, E_BAT,
    E_FIREBALL, E_FIREBALL, E_GHOST, E_GHOST, E_GHOST, E_BIRD, E_BIRD, E_ALIEN,
    E_FIREBALL, E_FIREBALL, E_FIREBALL, E_BIRD, E_GHOST, E_GHOST, E_GHOST, E_ALIEN,
    // Dream
    E_SPIKES, E_SPIKES, E_GHOST, E_GHOST, E_GHOST, E_ALIEN, E_BIRD, E_BIRD,
    E_FIREBALL, E_FIREBALL, E_SPIKES, E_GHOST, E_GHOST, E_GHOST, E_BIRD, E_ALIEN,
    E_FIREBALL, E_FIREBALL, E_SPIKES, E_ALIEN, E_GHOST, E_GHOST, E_BIRD, E_BIRD
};

const UBYTE spawn_level_heaven[32] = {
    E_FIREBALL, E_SPIKES, E_GHOST, E_ALIEN, E_BIRD, E_BIRD, E_BAT, E_BAT,
    E_FIREBALL, E_SPIKES, E_SPIKES, E_GHOST, E_ALIEN, E_BIRD, E_BIRD, E_BAT,
    E_FIREBALL, E_FIREBALL, E_SPIKES, E_GHOST, E_GHOST, E_ALIEN, E_BIRD, E_BAT,
    E_FIREBALL, E_FIREBALL, E_SPIKES, E_GHOST, E_GHOST, E_ALIEN, E_ALIEN, E_BIRD
};

#define PROGRESS_POS(x) mydiv(((x) << 1U), 3U)

const UBYTE wave_text[4] = { 33U, 11U, 32U, 15U };

const UWORD clock_palettes[4] = {
    32767UL, 18668UL, 18668UL, 0UL
};

const UBYTE entity_sprites[10] = {
    0,           // E_NONE
     // Hazards
    (9U << 2),   // E_SPIKES
    (19U << 2),  // E_FIREBALL
     // Enemies
    (17U << 2),  // E_ALIEN
    (13U << 2),  // E_BAT
    (11U << 2),  // E_BIRD
    (15U << 2),  // E_GHOST
    // Powerups
    (21U << 2),  // E_CLOCK
    // Special
    (29U << 2),  // E_PORTAL
    (27U << 2)   // E_CLOUD
};

const UBYTE entity_palettes[10U] = {
    0U, // none
    2U, // spikes
    2U, // fireball
    4U, // alien
    3U, // bat
    2U, // bird
    3U, // ghost
    0U // clock
};

const UBYTE SGB_GAME_STAGE_PAL01[16] = {
    1U, 255, 127,  92,  57, 203,  72,   0,   0, 59,  29, 233, 113,   0,   0, 0
};


const UBYTE SGB_GAME_ATTRDIV[16] = {
    (6U << 3) + 1U,
    1U | (1U << 4),
    18U,
    0U,
    0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U
};

const UBYTE SGB_WAVE_ATTRDIV[16] = {
    (6U << 3) + 1U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U
};

const UWORD wavescreen_palette_data[] = {
    32767,   0,   0,   0
};

const UBYTE retry_text[19] = {
    33U, 11U, 32U, 15U, 29U, // waves
    12U, 15U, 29U, 30U, // best
    28U, 15U, 30U, 28U, 35U, // retry
    35U, 15U, 29U, // yes
    24U, 25U // no
};

#define RETRY_NUM_CHARS 26U

const UBYTE retry_text_data[RETRY_NUM_CHARS * 4] = {
    // waves
    8U, 44U, 33U, OBJ_PAL0,
    8U, 52U, 11U, OBJ_PAL0,
    8U, 60U, 32U, OBJ_PAL0,
    8U, 68U, 15U, OBJ_PAL0,
    // best
    16U, 44U, 12U, OBJ_PAL0,
    16U, 52U, 15U, OBJ_PAL0,
    16U, 60U, 29U, OBJ_PAL0,
    16U, 68U, 30U, OBJ_PAL0,
    // retry
    32U, 60U, 28U, OBJ_PAL0,
    32U, 68U, 15U, OBJ_PAL0,
    32U, 76U, 30U, OBJ_PAL0,
    32U, 84U, 28U, OBJ_PAL0,
    32U, 92U, 35U, OBJ_PAL0,
    // yes
    40U, 72U, 35U, OBJ_PAL0,
    40U, 80U, 15U, OBJ_PAL0,
    40U, 88U, 29U, OBJ_PAL0,
    // no
    48U, 72U, 24U, OBJ_PAL0,
    48U, 80U, 25U, OBJ_PAL0,
    // wave count
    8U,  92U, 10U, OBJ_PAL0,
    8U, 100U, 10U, OBJ_PAL0,
    8U, 108U, 10U, OBJ_PAL0,
    // best count
    16U,  92U, 10U, OBJ_PAL0,
    16U, 100U, 10U, OBJ_PAL0,
    16U, 108U,  0U, OBJ_PAL0,
    // marker
    40U, 58U, 100U, OBJ_PAL0,
    40U, 66U, 102U, OBJ_PAL0
};

const UBYTE restart_window_tiles[9] = {
    3U, 3U, 18U,
    0xFFU, 0xFFU, 108U,
    5U, 5U, 18U
};

void initGame() {
    disable_interrupts();
    mus_setPaused(1U);
    DISPLAY_OFF;

    // Init variables
    timer = 0U;
    ticks = 0U;
    next_spawn = 0U;
    progress = 0U;
    progressbar = 121U;
    portal_spawned = 0U;
    repeat_spikes = 0U;
    ghost_frame = 0U;
    next_entity = 0U;
    remaining_time = MAX_TIME;
    scene_state = INGAME_ACTIVE;

    player_x = 88U;
    player_y = SCRLMGN;
    player_xdir = RIGHT;
    player_ydir = DOWN;
    player_yspeed = 0U;
    player_bounce = 0U;
    blip_bar = 0U;
    dashing = 0U;
    dashes = MAX_DASHES;
    dash_xdir = 0U;
    dash_ydir = 0U;
    paused = 0U;
    scrolled = 0U;
    scroll_y = 0U;
    clock_interval = clock_interval_data[level-1U];

    if (level == 5U) {
        generateSpawnData();
        is_special_wave = mymod(wave+1U, 5U) == 0U;
        wave_bg = wave - mydiv(wave+1U, 5U);
    } else {
        spawn_levels = (UBYTE*)&spawn_level_data[(level-1U) * 24];
        scrolled_length = scrolled_length_data[level-1U];
        allowed_spikes = allowed_spikes_data[level-1U];
    }

    if(level != 5U || wave == 0U) {
        // don't reset boost, time and stomps when progressing in level 5
        blips = MAX_BOOST;
        elapsed_time = 0UL;
        kills = 0UL;
    }

    next_clock = clock_interval;

    restoreGame(first_load, 0U);
    set_sprite_data(4U, portal_data_length, portal_data);

    if(first_load) {
        last_progress = 0U;

        switch(level) {
            case 1U:
                setMusicBank(SONG_BANK_PLAINS);
                playMusic(&plains_song_data);
                break;
            case 2U:
                setMusicBank(SONG_BANK_CLOUDS);
                playMusic(&clouds_song_data);
                break;
            case 3U:
                setMusicBank(SONG_BANK_SPACE);
                playMusic(&space_song_data);
                break;
            case 4U:
                setMusicBank(SONG_BANK_DREAM);
                playMusic(&dream_song_data);
                break;
            case 5U:
                setMusicBank(SONG_BANK_HEAVEN);
                playMusic(&heaven_song_data);
                break;
        }
        mus_setPaused(1U);
    }

    clearSprites();
    clearEntities();


    move_bkg(0U, 112U);
    move_win(151U, 0U);

    updateHUDTime();
    initSpawns();

    DISPLAY_ON;

    enable_interrupts();
}

void restoreGame(UBYTE update, UBYTE from_pause) {
    if(sgb_mode) {
        sgb_send_packet(SGB_GAME_STAGE_PAL01); delay(62U);
        sgb_send_packet(SGB_GAME_ATTRDIV);
    }


    set_sprite_data(0U, 24U, getSkinData());
    set_sprite_data(24U, sprites_data_length, sprites_data);
    set_sprite_palette(0U, sprites_palette_data_length, sprites_palette_data);

    setIngameBackground(is_special_wave ? 255U : level+wave_bg, update, !from_pause);
    setIngameHUD(update, !from_pause);
    if(update) {
        set_bkg_data_rle(clock_tiles_offset, clock_data_length, clock_data);
    }
    if(CGB_MODE) {
        set_bkg_palette_buffer(7U, 1U, clock_palettes);
    }

    updateHUDTime();

    SPRITES_8x16;
    SHOW_BKG;
    SHOW_WIN;
    SHOW_SPRITES;

    OBP0_REG = 0xD0U; // 11010000
    OBP1_REG = 0x40U; // 01010000
    BGP_REG = 0xE4U;  // 11100100

    move_bkg(0U, 112U-progress);
}

void updateInput() {
    updateJoystate();

    if(!dashing) {
        if(ISDOWN(J_LEFT)) {
            player_x -= MOVE_SPEED;
            player_xdir = LEFT;
        }
        else if(ISDOWN(J_RIGHT)) {
            player_x += MOVE_SPEED;
            player_xdir = RIGHT;
        }
    }

    if(CLICKED(KEY_DASH)) {
        if(dashes) {
            dash_xdir = dash_ydir = NONE;
            
            if(ISDOWN(J_LEFT)) dash_xdir = LEFT;
            else if(ISDOWN(J_RIGHT)) dash_xdir = RIGHT;

            if(ISDOWN(J_UP)) dash_ydir = UP;
            else if(ISDOWN(J_DOWN)) dash_ydir = DOWN;

            if(dash_xdir || dash_ydir) {
                dashing = DASH_TIME;
                dashes--;
                spawnEntity(E_CLOUD, player_x, player_y-6U, 0U);
                playSound(SFX_DASH);
            }
        }
    }
}

void updatePlayer() {
    UBYTE i, diff;
    UBYTE frame, palette, type;

    // Check entity collisions
    for(i = 0U; i != MAX_ENTITIES; ++i) {
        type = entity_type[i];

        if(type && type <= LAST_COLLIDABLE
        && (player_y - entity_y[i] + 11U) <= 22U
        && (player_x - entity_x[i] + 11U) <= 22U) {
            // Spikes and fireballs
            if(type <= E_FIREBALL) {
                killPlayer();
            // Enemies
            } else if(type <= LAST_ENEMY) {
                if(player_ydir == DOWN && player_y <= entity_y[i]+1U) {
                    if(dashing && dash_ydir == DOWN) {
                        entity_type[i] = E_NONE;
                        spawnEntity(E_CLOUD, player_x, player_y+5U, 0U);
                        blips += STOMP_BONUS;
                        if(blips > MAX_BOOST) blips = MAX_BOOST;
                        kills++;
                        playSound(type+SFX_STOMP_ALIEN-E_ALIEN);
                        bouncePlayer(i, BUMP_SPEED);
                    }
                    else if(type == E_GHOST) {
                        entity_type[i] = E_NONE;
                        spawnEntity(E_CLOUD, player_x, player_y+5U, 0U);
                        playSound(SFX_STOMP_GHOST);
                        bouncePlayer(i, BUMP_SPEED);
                    }
                    else if(type == E_ALIEN) {
                        playSound(SFX_BUMP_ALIEN);
                        bouncePlayer(i, ALIEN_BUMP_SPEED);
                    }
                    else {
                        playSound(SFX_BUMP);
                        bouncePlayer(i, BUMP_SPEED);
                    }
                } else {
                    if(player_x <= entity_x[i]) diff = entity_x[i] - player_x;
                    else diff = player_x - entity_x[i];
                    if(player_y <= entity_y[i]) diff += entity_y[i] - player_y;
                    else diff += player_y - entity_y[i];

                    if(diff <= 9U) killPlayer();
                }
            // Clock pickup
            } else if(type == E_CLOCK) {
                entity_type[i] = E_NONE;
                remaining_time += CLOCK_BONUS;
                if(remaining_time > MAX_TIME) remaining_time = MAX_TIME;
                updateHUDTime();
                playSound(SFX_TIME_PICKUP);
            // End level portal
            } else if(type == E_PORTAL) {
                scene_state = INGAME_COMPLETED;
                player_x = entity_x[i];
                player_y = entity_y[i];
                return;
            }
        }
    }

    // Dashing
    if(dashing) {
        dashing--;
        player_yspeed = 0U;
        player_ydir = DOWN;
        if(dash_xdir == LEFT) {
            player_x -= DASH_SPEED;
        }
        else if(dash_xdir == RIGHT) {
            player_x += DASH_SPEED;
        }
        if(dash_ydir == UP) {
            player_y -= DASH_SPEED-1U;
            player_yspeed = 7U;
            player_ydir = UP;
        }
        else if(dash_ydir == DOWN) {
            player_y += DASH_SPEED;
        }

        if(!ISDOWN(KEY_DASH)) {
            if(DASH_TIME - dashing >= 7U) {
                dashing = 0U;
            }
        }
    } 
    // Using boost
    else if(ISDOWN(KEY_USE) && blips) {
        blips--;
        if(player_ydir == UP && player_yspeed <= MAX_FLY_SPEED) {
            player_yspeed += 2U;
        }
        else {
            if(player_yspeed <= 2U) {
                player_yspeed = 2U;
                player_ydir = UP;
            } else {
                player_yspeed -= 2U;
            }
        }
        if((ticks & 15U) == 15U) {
            playSound(SFX_JETPACK);
            spawnEntity(E_CLOUD, player_x, player_y+5U, 0U);
        }
    }

    // Flying UP
    if(player_ydir == UP) {
        player_yspeed--;
        if(player_yspeed == 0U) {
            player_ydir = DOWN;
        }
        player_y -= mydiv(player_yspeed, 7U);
    }
    // Flying DOWN
    else {
        player_yspeed++;
        player_y += mydiv(player_yspeed, 7U);
        if(player_yspeed > MAX_YSPEED) {
            player_yspeed = MAX_YSPEED;
        }
    }

    // Left and right borders
    if(player_x <= 22U) player_x = 23U;
    else if(player_x >= 154U) player_x = 153U;

    // Don't underflow in auto scroll level
    if(player_y <= 11U) player_y = 12U;

    // Check bounds
    if(player_y > SCREENHEIGHT+5U) {
        player_y = SCREENHEIGHT;
        killPlayer();
    }

    // Update sprite
    frame = 0U;
    if(player_bounce) {
        frame = 8U;
        player_bounce--;
    }
    else if(player_ydir == DOWN) {
        frame = 4;
    }

    if(dashing) {
        frame = 20U;
    }
    else if(ISDOWN(KEY_USE) && blips) {
        if(ticks & 8U) frame = 12U;
        else frame = 16U;
    }

    // Blink
    palette = OBJ_PAL0;
    if(!dashes && (ticks & 4U)) palette = OBJ_PAL1 | 7U;

    // Dash marker
    if(show_dashcounter) {
        setSprite(player_x-12U, player_y-9U, 24U+(dashes << 1), palette | 5U);
    }

    if(player_xdir == LEFT) {
        setSprite(player_x-16U, player_y, frame, palette);
        setSprite(player_x-8U, player_y, frame+2U, palette);
    } else {
        setSprite(player_x-8U, player_y, frame, FLIP_X | palette);
        setSprite(player_x-16U, player_y, frame+2U, FLIP_X | palette);
    }

    // Update scroll
    if(wave == WAVE_SPC_AUTOSCROLL) {
        scroll_y = ticks & 1U;
        player_y += scroll_y;
    } else {
        scroll_y = 0U;
        if(player_y < SCRLMGN) {
            scroll_y = SCRLMGN - player_y;
            player_y = SCRLMGN;
        }
    }
}

void updateHUD() {
    UBYTE frame;

    if(blip_bar > blips) {
        blip_bar = blips;
    } else if(blip_bar < blips) {
        blip_bar += 2U;
    }

    // Blips
    setSprite(168U-(blip_bar >> 3), 136U, 96U, OBJ_PAL0 | 5U);
    setSprite(176U-(blip_bar >> 3), 136U, 98U, OBJ_PAL0 | 5U);

    // Progress bar
    frame = 100U + ((player_skin-1U) << 2U);
    setSprite(152U, progressbar, frame, OBJ_PAL0 | player_skin-1U);
    setSprite(160U, progressbar, frame+2U, OBJ_PAL0 | player_skin-1U);

    // Set last progress flag
    if(last_progress) {
        setSprite(153U, 119U - last_progress, 92U, OBJ_PAL0 | 5U);
    }

    // Low on time marker
    if(remaining_time <= LOW_TIME && ticks & 16U) {
        setSprite(136U, 24U, 32U, OBJ_PAL0 | 5U);
        setSprite(144U, 24U, 34U, OBJ_PAL0 | 5U);
    }
}

void updateHUDTime() {
    UBYTE index;

    index = (remaining_time >> 2) << 2;

    disable_interrupts();
    set_win_tiles(0U, 1U, 2U, 2U, &clock_tiles[index]);
    enable_interrupts();
}

void bouncePlayer(UBYTE entity, UBYTE str) {
    player_ydir = UP;
    player_bounce = 16U;
    dashes = MAX_DASHES;
    dashing = 0;
    player_y = entity_y[entity]-12U;
    player_yspeed = str;
}

void updateEntities() {
    UBYTE i, x, y, frame, pal, type, ghost_move;

    // Update last spawn position with last spawned
    // enemy if it still exists
    if(last_spawn_index != NO_LAST_SPAWN) {
        if(entity_type[last_spawn_index] != E_NONE) {
            last_spawn_x = entity_x[last_spawn_index]-24U;
        } else {
            last_spawn_index = NO_LAST_SPAWN;
        }
    }

    ghost_move = 0U;
    if(!(ticks & 1U) && !scene_state) {
        ghost_frame++;
        ghost_move = cos16_32[(ghost_frame+1U) & 31U] - cos16_32[ghost_frame & 31U];
    }

    for(i = 0U; i != MAX_ENTITIES; ++i) {
        type = entity_type[i];
        if(!type) continue;

        x = entity_x[i];
        y = entity_y[i];

        // Scroll entitites
        y += scroll_y;
        if(y >= SCREENHEIGHT+17U) {
            entity_type[i] = E_NONE;
            continue;
        }

        // Update entity
        switch(type) {
            case E_NONE: break;
            case E_SPIKES: break;

            case E_FIREBALL:
                if(!(ticks & 1U) && !scene_state) {
                    if(entity_dir[i] == RIGHT) {
                        x++;
                        if(x >= 152U) entity_dir[i] = LEFT;
                    }
                    else {
                        x--;
                        if(x <= 24U) entity_dir[i] = RIGHT;
                    }
                }
                break;

            case E_ALIEN: break;
            case E_BAT: break;

            case E_BIRD:
                if(ticks & 1U && !scene_state) {
                    if(entity_dir[i] == RIGHT) {
                        x++;
                        if(x >= 152U) entity_dir[i] = LEFT;
                    }
                    else {
                        x--;
                        if(x <= 24U) entity_dir[i] = RIGHT;
                    }
                }
                break;

            case E_GHOST:
                x += ghost_move;
                if(ghost_frame & 16U) entity_dir[i] = LEFT;
                else entity_dir[i] = RIGHT;
                break;

            case E_CLOCK: break;
            case E_PORTAL:
                if((ticks & 15U) == 15U) entity_dir[i]++;
                break;

            case E_CLOUD:
                if((ticks & 3U) == 3U) entity_dir[i]++;
                if(entity_dir[i] == 4U) {
                    entity_type[i] = E_NONE;
                    y = 0U;
                    continue;
                }
                break;
        }

        // Draw entities on screen
        frame = entity_sprites[type];

        entity_x[i] = x;
        entity_y[i] = y;

        switch(type) {
            case E_CLOUD:
                frame += entity_dir[i] << 1U;
                setSprite(x-16U, y, frame, OBJ_PAL0 | 3U);
                setSprite(x-8U,  y, frame, OBJ_PAL0 | FLIP_X | 3U);
                break;

            case E_PORTAL:
                if(level == 3U && player_skin == 1U) {
                    setSprite(x-16U, y-24U, 120U, OBJ_PAL0 | 1U);
                    setSprite(x-8U,  y-24U, 122U, OBJ_PAL0 | 1U);
                    setSprite(x-16U, y-8U, 124U, OBJ_PAL0 | 1U);
                    setSprite(x-8U,  y-8U, 126U, OBJ_PAL0 | 1U);
                } else {
                    if(entity_dir[i] & 1U) {
                        setSprite(x-16U, y, frame, OBJ_PAL0 | 6U);
                        setSprite(x-8U,  y, frame+2U, OBJ_PAL0 | 6U);
                    } else {
                        setSprite(x-8U,  y, frame, OBJ_PAL0 | FLIP_X | 6U);
                        setSprite(x-16U, y, frame+2U, OBJ_PAL0 | FLIP_X | 6U);
                    }
                }
                break;

            default:
                frame += ((ticks & 8U) >> 1);
                pal = OBJ_PAL0 | entity_palettes[type];
                if(entity_dir[i] == LEFT) {
                    setSprite(x-16U, y, frame, pal);
                    setSprite(x-8U,  y, frame+2U, pal);
                } else {
                    setSprite(x-8U,  y, frame, pal | FLIP_X);
                    setSprite(x-16U, y, frame+2U, pal | FLIP_X);
                }
                break;
        }
    }
}

UBYTE spawnEntity(UBYTE type, UBYTE x, UBYTE y, UBYTE dir) {
    UBYTE i;
    for(i = 0U; i != MAX_ENTITIES; ++i) {
        if(entity_type[next_entity] == E_NONE) break;
        ++next_entity;
        if(next_entity == MAX_ENTITIES) next_entity = 0U;
    }
    if(i == MAX_ENTITIES) return NO_LAST_SPAWN;

    entity_x[next_entity] = x;
    entity_y[next_entity] = y;
    entity_type[next_entity] = type;
    entity_dir[next_entity] = dir;

    return next_entity;
}

void initSpawns() {
    UBYTE i, x, y, type;

    if(wave == WAVE_SPC_SQUIDS) {
        type = E_ALIEN;
    } else {
        type = E_BAT;
    }

    last_spawn_x = 72U;
    x = last_spawn_x + 16U;
    y = 112U;
    spawnEntity(type, x, y, NONE);

    if(wave == WAVE_SPC_GHOSTS) type = E_GHOST;

    for(i = 0U; i != 3U; ++i) {
        last_spawn_x = (last_spawn_x + 32U + (rand() & 63U)) & 127U;
        x = last_spawn_x + 24U;
        y -= 36U;
        last_spawn_index = spawnEntity(type, x, y, RIGHT);
    }
}

void generateSpawnData() {
    UBYTE i;

    spawn_levels = (UBYTE*)spawn_level_gen;
    scrolled_length = 8U + (wave >> 1);
    if(scrolled_length >= 29U) scrolled_length = 28U;

    allowed_spikes = (wave >> 3) + 1U;
    if(allowed_spikes >= 4U) allowed_spikes = 3U;

    switch(wave) {
        case WAVE_SPC_SQUIDS:
            spawn_levels = spawn_level_gen;
            mymemset(spawn_levels, E_ALIEN, 8U);
            break;
        case WAVE_SPC_GHOSTS:
            spawn_levels = spawn_level_gen;
            mymemset(spawn_levels, E_GHOST, 8U);
            break;
        case WAVE_SPC_AUTOSCROLL:
            scrolled_length = 10U;
            clock_interval = 8U;
            spawn_levels = spawn_level_data + 32U;
            break;
        case 0U:
            spawn_levels = spawn_level_data + 8U;
            break;
        case 1U:
            spawn_levels = spawn_level_data + 48U;
            break;
        default:
            i = (wave >> 3);
            if(i >= 4U) i--;
            spawn_levels = spawn_level_heaven + (i << 3);
            break;
    }
}

void updateSpawns() {
    UBYTE x, dice, type, step;
    next_spawn += scroll_y;

    if(next_spawn < SPAWN_INTERVAL) return;

    if(progress <= 110U) {
        next_spawn -= SPAWN_INTERVAL;

        x = ((last_spawn_x + 32U + (rand() & 63U)) & 127U) + 24U;

        step = 0U;
        if(level != 5U) {
            step = mydiv(progress, 39U); // TODO: Optimize?
        }
        dice = rand() & 7U;

        while(dice != 8U) {
            type = spawn_levels[(step << 3) + dice];
            switch(type) {
                case E_FIREBALL:
                    if(repeat_spikes < allowed_spikes) {
                        last_spawn_index = spawnEntity(E_FIREBALL, x, 1U, 2U + ((rand() & 1U) << 1));
                        repeat_spikes++;
                        dice = 8U;
                        break;
                    }
                    dice++;
                    break;
                case E_SPIKES:
                    if(repeat_spikes < allowed_spikes) {
                        last_spawn_index = spawnEntity(E_SPIKES, x, 1U, NONE);
                        repeat_spikes++;
                        dice = 8U;
                        break;
                    }
                    dice++;
                    break;
                case E_GHOST:
                    if(x <= 39U) x = 40U;
                    if(x >= 137U) x = 136U;
                    last_spawn_index = spawnEntity(E_GHOST, x, 1U, NONE);
                    repeat_spikes = 0U;
                    dice = 8U;
                    break;
                case E_ALIEN:
                    if(x <= 39U) x = 40U;
                    if(x >= 137U) x = 136U;
                    last_spawn_index = spawnEntity(E_ALIEN, x, 1U, LEFT);
                    repeat_spikes = 0U;
                    dice = 8U;
                    break;
                case E_BIRD:
                    if(x <= last_spawn_x) {
                        last_spawn_index = spawnEntity(E_BIRD, x, 1U, LEFT);
                    } else {
                        last_spawn_index = spawnEntity(E_BIRD, x, 1U, RIGHT);
                    }
                    repeat_spikes = 0U;
                    dice = 8U;
                    break;
                case E_BAT:
                    last_spawn_index = spawnEntity(E_BAT, x, 1U, NONE);
                    repeat_spikes = 0U;
                    dice = 8U;
                    break;
            }
        }

        last_spawn_x = x - 24U;

        next_clock--;
        if(!next_clock) {
            next_clock = clock_interval;
            x = ((last_spawn_x + 32U + (rand() & 63U)) & 127U) + 24U;
            spawnEntity(E_CLOCK, x, 1U, NONE);
        }
    }
    else if(progress == 112U && !portal_spawned && next_spawn >= 56U) {
        spawnEntity(E_PORTAL, 88U, 1U, NONE);
        next_spawn = 0U;
        portal_spawned = 1U;
    }
}

void introAnimation() {
    UBYTE frame;

    playSound(SFX_WARP_START);

    for(ticks = 0U; ticks != 64U; ++ticks) {
        frame = 20U - ((ticks >> 4) << 2);
        if(ticks & 8U) {
            setSprite(player_x-16U, player_y, frame, OBJ_PAL0 | 6U);
            setSprite(player_x-8U, player_y, frame+2U, OBJ_PAL0 | 6U);
        } else {
            setSprite(player_x-8U, player_y, frame, FLIP_X | OBJ_PAL0 | 6U);
            setSprite(player_x-16U, player_y, frame+2U, FLIP_X | OBJ_PAL0 | 6U);
        }

        updateEntities();
        updateHUD();

        clearRemainingSprites();
        snd_update();
        wait_vbl_done();
    }

    for(ticks = 0U; ticks != 16U; ++ticks) {
        setSprite(player_x-8U, player_y, 0U, FLIP_X | OBJ_PAL0);
        setSprite(player_x-16U, player_y, 2U, FLIP_X | OBJ_PAL0);

        updateEntities();
        updateHUD();

        clearRemainingSprites();
        snd_update();
        wait_vbl_done();
    }

    disable_interrupts();
    set_sprite_data(0U, 24U, getSkinData());
    set_sprite_data(24U, 4U, sprites_data);
    enable_interrupts();
}

void intoPortalAnimation() {
    UBYTE frame;

    disable_interrupts();
    set_sprite_data(0U, portal_data_length, portal_data);
    enable_interrupts();

    playSound(SFX_WARP_START);

    for(ticks = 0U; ticks != 18U; ++ticks) {
        setSprite(player_x-16U, player_y, entity_sprites[E_PORTAL], OBJ_PAL0 | 6U);
        setSprite(player_x-8U, player_y, entity_sprites[E_PORTAL]+2U, OBJ_PAL0 | 6U);

        updateHUD();
        clearRemainingSprites();
        snd_update();
        wait_vbl_done();
    }

    for(ticks = 0U; ticks != 64U; ++ticks) {
        frame = 4U + ((ticks >> 4) << 2);
        if(ticks & 8U) {
            setSprite(player_x-16U, player_y, frame, OBJ_PAL0 | 6U);
            setSprite(player_x-8U, player_y, frame+2U, OBJ_PAL0 | 6U);
        } else {
            setSprite(player_x-8U, player_y, frame, FLIP_X | OBJ_PAL0 | 6U);
            setSprite(player_x-16U, player_y, frame+2U, FLIP_X | OBJ_PAL0 | 6U);
        }

        updateHUD();

        clearRemainingSprites();
        snd_update();
        wait_vbl_done();
    }

    updateHUD();
    clearRemainingSprites();
    wait_vbl_done();
    wait_sound_done();

    if(level != 5U) {
        STOP_MUSIC;
        disable_interrupts();
        setMusicBank(SONG_BANK_LEVEL_CLEAR);
        playMusic(&level_clear_song_data);
        enable_interrupts();

        while(!mus_is_done()) {
            wait_vbl_done();
        }
        STOP_MUSIC;
    }

    clearRemainingSprites();
    fadeToWhite(4U);
}

void saveCatAnimation() {
    UBYTE c;
    UBYTE r, g, b;
    UWORD v;

    disable_interrupts();

    STOP_MUSIC;
    setMusicBank(SONG_BANK_LEVEL_CLEAR);
    playMusic(&level_clear_song_data);

    get_bkg_palette(palette_buffer);

    enable_interrupts();

    player_y++;

    for(ticks = 0U; ticks != 180U; ++ticks) {
        if(CGB_MODE) {
            if((ticks & 3U) == 3U) {
                for(c = 0U; c != 32U; ++c) {
                    v = palette_buffer[c];
                    r = GET_RED(v);
                    g = GET_GREEN(v);
                    b = GET_BLUE(v);

                    if(r <= 29U) r += 1U; else r = 31U;
                    if(g <= 29U) g += 1U; else g = 31U;
                    if(b <= 29U) b += 1U; else b = 31U;
                    palette_buffer[c] = RGB(r, g, b);
                }
                set_bkg_palette(0U, 8U, palette_buffer);
            }
        } else {
            if(ticks == 24U) BGP_REG = 0x90U;
            else if(ticks == 48U) BGP_REG = 0x40U;
            else if(ticks == 72U) BGP_REG = 0x00U;
        }

        if((ticks & 15U) == 15U) player_y++;

        setSprite(player_x-16U, player_y-24U, 120U, OBJ_PAL0 | 1U);
        setSprite(player_x-8U,  player_y-24U, 122U, OBJ_PAL0 | 1U);
        setSprite(player_x-16U, player_y-8U, 124U, OBJ_PAL0 | 1U);
        setSprite(player_x-8U,  player_y-8U, 126U, OBJ_PAL0 | 1U);

        setSprite(player_x-16U, player_y, 4U, OBJ_PAL0);
        setSprite(player_x-8U, player_y, 6U, OBJ_PAL0);

        clearRemainingSprites();
        snd_update();
        wait_vbl_done();
    }

    while(!mus_is_done()) {
        wait_vbl_done();
    }
    STOP_MUSIC;

    fadeSpritesToWhite(4U);
}

void deathAnimation() {
    UBYTE offset, frame;
    set_sprite_data(0U, 8U, getSkinData()+384UL);
    set_sprite_data(8U, portal_data_length, portal_data);

    playSound(SFX_PLAYER_DIE);

    if(level == 5U) {
        for(offset = 0U; offset != MAX_ENTITIES; ++offset) {
            if(entity_type[offset]) {
                entity_type[offset] = E_CLOUD;
                entity_dir[offset] = 0U;
            }
        }
    }

    scroll_y = 0U;
    for(ticks = 0U; ticks != 48U; ++ticks) {
        if(ticks < 16U) {
            if(player_xdir == LEFT) {
                setSprite(player_x-16U, player_y, 0U, OBJ_PAL0);
                setSprite(player_x-8U, player_y, 2U, OBJ_PAL0);
            } else {
                setSprite(player_x-8U, player_y, 0U, FLIP_X | OBJ_PAL0);
                setSprite(player_x-16U, player_y, 2U, FLIP_X | OBJ_PAL0);
            }
        } else if(ticks < 20U) {
            if(player_xdir == LEFT) {
                setSprite(player_x-16U, player_y, 4U, OBJ_PAL0);
                setSprite(player_x-8U, player_y, 6U, OBJ_PAL0);
            } else {
                setSprite(player_x-8U, player_y, 4U, FLIP_X | OBJ_PAL0);
                setSprite(player_x-16U, player_y, 6U, FLIP_X | OBJ_PAL0);
            }
        } else if(ticks < 24U) {
            setSprite(player_x-16U, player_y, 8U, OBJ_PAL0);
            setSprite(player_x-8U, player_y, 10U, OBJ_PAL0);
        } else {
            offset = ((ticks-16U) >> 1);
            frame = 28U + ((ticks & 4U) >> 1);
            setSprite(player_x-12U-offset, player_y-offset, frame, OBJ_PAL0);
            setSprite(player_x-12U+offset, player_y-offset, frame, OBJ_PAL0);
            setSprite(player_x-12U-offset, player_y+offset, frame, OBJ_PAL0);
            setSprite(player_x-12U+offset, player_y+offset, frame, OBJ_PAL0);
        }

        updateEntities();
        updateHUD();

        clearRemainingSprites();
        snd_update();
        wait_vbl_done();
    }
}

void addScoreNormal() {
    UBYTE i, j, score;
    UBYTE *data;

    ENABLE_RAM_MBC1;
    SWITCH_RAM_MBC1(0U);

    score = TOTAL_SCORE;

    data = ram_data + ((level - 1U) << 4);
    for(i = 0U; i != 5U; ++i) {
        if(score > data[(i << 1) + 1U]
        || (score == data[(i << 1) + 1U] && (UBYTE)elapsed_time < data[i << 1])) {
            break;
        }
    }
    
    if(i <= 4U) {
        for(j = 4U; j != i; --j) {
            data[j << 1] = data[(j - 1U) << 1];
            data[(j << 1) + 1U] = data[((j - 1U) << 1) + 1U];
        }
        data[i << 1] = (UBYTE)elapsed_time;
        data[(i << 1) + 1U] = score;

        last_highscore_level = level;
        last_highscore_slot = i;
    }

    DISABLE_RAM_MBC1;
}

void addScoreInfinite() {
    UWORD i, j, score;
    UWORD *data;

    ENABLE_RAM_MBC1;
    SWITCH_RAM_MBC1(0U);

    data = (UWORD*)(ram_data + 64U);
    score = wave + 1UL;
    for(i = 0U; i != 5U; ++i) {
        if(score > data[i << 1]) {
            break;
        }
    }

    last_highscore_slot = 5U;
    if(i <= 4U) {
        for(j = 4U; j != i; --j) {
            data[j << 1] = data[(j - 1U) << 1];
            data[(j << 1) + 1U] = data[((j - 1U) << 1) + 1U];
        }

        data[i << 1] = score;
        data[(i << 1) + 1U] = elapsed_time;

        last_highscore_level = level;
        last_highscore_slot = i;
    }

    DISABLE_RAM_MBC1;
}

void fadeSpritesToWhite(UBYTE delay) {
    if(CGB_MODE) fadeSpritesToWhiteCGB(delay >> 2);
    else fadeSpritesToWhiteGB(delay);
}

void fadeSpritesToWhiteGB(UBYTE delay) {
    UINT8 i, j;
    for(i = 1U; i != 4U; ++i) {
        OBP0_REG = fadePals[i];
        for(j = 0U; j != delay; ++j) {
            snd_update();
            wait_vbl_done();
        }
    }
}

void fadeSpritesToWhiteCGB(UBYTE delay) {
    UBYTE i, c;
    UBYTE r, g, b;
    UWORD v;

    get_sprite_palette(palette_buffer);

    for(i = 0U; i != 15U; ++i) {
        for(c = 0U; c != 32U; ++c) {
            v = palette_buffer[c];
            r = GET_RED(v);
            g = GET_GREEN(v);
            b = GET_BLUE(v);

            if(r <= 29U) r += 2U; else r = 31U;
            if(g <= 29U) g += 2U; else g = 31U;
            if(b <= 29U) b += 2U; else b = 31U;
            palette_buffer[c] = RGB(r, g, b);
        }
        set_sprite_palette(0, 8U, palette_buffer);

        for(c = 0U; c != delay; ++c) {
            snd_update();
            wait_vbl_done();
        }
    }
}

void showWaveScreen() {
    UBYTE i, j;
    UWORD tmp;
    UBYTE text[3] = {10U, 10U, 10U};

    disable_interrupts();
    DISPLAY_OFF;

    SPRITES_8x8;
    OBP0_REG = 0xD0U; // 11010000
    OBP1_REG = 0x40U; // 01010000
    BGP_REG = 0xE4U;  // 11100100

    if(sgb_mode) {
        sgb_send_packet(SGB_WAVE_ATTRDIV);
    }

    set_sprite_data(0U, characters_data_length, characters_data);

    mymemset((UBYTE*)0x8FE0UL, 255U, 16U);
    mymemset((UBYTE*)0x8FF0UL, 0U, 16U);
    mymemset((UBYTE*)0x9800UL, 0xFEU, 192U);
    mymemset((UBYTE*)(0x9800UL + 192UL), 0xFFU, 576UL);
    mymemset((UBYTE*)0x9C00UL, 0xFEU, 192U);

    if(CGB_MODE) {
        set_bkg_palette(0U, 2U, wavescreen_palette_data);
        set_sprite_palette(0U, 1U, sprites_palette_data);

        VBK_REG = 1U;
        mymemset((UBYTE*)0x9800UL, 0U, 768UL);
        mymemset((UBYTE*)0x9C00UL, 0U, 192U);
        VBK_REG = 0U;
    }

    ticks = 0U;

    tmp = wave+1U;
    i = 0U;
    if(tmp >= 10U) i++;
    if(tmp >= 100U) i++;
    while(tmp) {
        text[i] = mymod16(tmp, 10U);
        tmp = mydiv16(tmp, 10U);
        --i;
    }

    j = 64U;
    if(wave >= 99U) j -= 8U;
    else if(wave >= 9U) j -= 4U;

    SHOW_WIN;
    SHOW_BKG;
    SHOW_SPRITES;

    DISPLAY_ON;
    enable_interrupts();

    if(!first_load) {
        mus_setPaused(0U);
    }

    for(i = 48U; i != 0U; i -= 4U) {
        move_bkg(0U, i);
        move_win(7U, 96+i);
        snd_update();
        wait_vbl_done();
    }
    move_bkg(0U, 0U);
    move_win(7U, 96U);

    while(1U) {
        updateJoystate();

        if(CLICKED(J_A) || CLICKED(J_START)) break;

        for(i = 0U; i != 4U; ++i) {
            setSprite(j+(i << 3), 80U+cos4_16[(i+(ticks >> 1)) & 15U], wave_text[i], OBJ_PAL0);
        }

        for(i = 5U; i != 8U; ++i) {
            if(text[i-5U] == 10U) continue;
            setSprite(j+(i << 3), 80U+cos4_16[(i+(ticks >> 1)) & 15U], text[i-5U], OBJ_PAL0);
        }

        clearRemainingSprites();
        ticks++;
        snd_update();
        wait_vbl_done();
    }

    clearSprites();
    fadeToWhite(4U);
}

void showInfiniteRestart() {
    UBYTE offset, i;
    UWORD tmp;
    UBYTE *data;

    disable_interrupts();
    set_sprite_data(0U, characters_data_length, characters_data);
    enable_interrupts();

    SPRITES_8x8;
    SHOW_WIN;
    SHOW_BKG;
    SHOW_SPRITES;

    sub_selection = 0U;

    offset = (156U - progress) >> 3;
    disable_interrupts();
    set_bkg_tiles_rle(0U, offset, 18U, 8U, restart_window_tiles);
    enable_interrupts();

    offset = 60U - mymod(116U - progress, 8U);

    // add sprite offset
    wait_vbl_done();
    memcpy((UBYTE*)0xC000UL, retry_text_data, RETRY_NUM_CHARS * 4);
    data = (UBYTE*)0xC000UL;
    for(i = 0U; i != RETRY_NUM_CHARS; ++i) {
        *data += offset;
        data += 4U;
    }

    // set wave counter
    data = (UBYTE*)(0xC000UL + 20 * 4 + 2);
    tmp = wave + 1U;
    while(tmp) {
        *data = mymod16(tmp, 10U);
        tmp = mydiv16(tmp, 10U);
        data -= 4U;
    }

    // set best counter
    ENABLE_RAM_MBC1;
    SWITCH_RAM_MBC1(0U);
    tmp = *((UWORD*)(ram_data + 64U));
    DISABLE_RAM_MBC1;
    data = (UBYTE*)(0xC000UL + 23 * 4 + 2);
    while(tmp) {
        *data = mymod16(tmp, 10U);
        tmp = mydiv16(tmp, 10U);
        data -= 4U;
    }

    while(1U) {
        updateJoystate();

        if(CLICKED(J_UP) && sub_selection) {
            sub_selection = 0U;
            playSound(SFX_MENU_SWITCH);
        }
        if(CLICKED(J_DOWN) && !sub_selection) {
            sub_selection = 1U;
            playSound(SFX_MENU_SWITCH);
        }
        if(CLICKED(J_A) || CLICKED(J_START)) {
            STOP_MUSIC;
            playSound(SFX_MENU_CONFIRM);
            if(sub_selection == 0U) {
                wave = 0U;
            } else {
                gamestate = GAMESTATE_WINSCREEN;
            }
            break;
        }

        data = (UBYTE*)(0xC000UL + (RETRY_NUM_CHARS-2) * 4);
        *data = offset + 40U + (sub_selection << 3);
        data += 4U;
        *data = offset + 40U + (sub_selection << 3);

        snd_update();
        wait_vbl_done();
    }

    clearSprites();
    fadeToWhite(4U);
}

void enterGame() {
    wave = 0U;
    first_load = 1U;

ingame_start:
    if(level == 5U) {
        showWaveScreen();
    }

    initGame();
    if(level == 5U && !first_load) {
        mus_setPaused(0U);
    }
    first_load = 0U;

    fadeFromWhite(8U);

    introAnimation();

    mus_setPaused(0U);

    vbl_count = 0U;
    while(scene_state == INGAME_ACTIVE) {
        updateInput();

        ticks++;
        timer++;

        // Update timing
        if(timer == 60U) {
            timer = 0U;
            elapsed_time++;
            remaining_time--;
            updateHUDTime();

            if(remaining_time == 11U) {
                playSound(SFX_TIME_LOW);
            } else if(remaining_time == 1U) {
                playSound(SFX_TIME_OUT);
            } else if(remaining_time == 0U) {
                scene_state = INGAME_DEAD;
            }
        }

        updatePlayer();
        updateHUD();

        updateEntities();
        updateSpawns();

        // Scroll screen
        scrolled += scroll_y;
        if(scrolled >= scrolled_length) {
            scrolled -= scrolled_length;
            if(progress < 112U) {
                progress++;
                progressbar = 121U - PROGRESS_POS(progress);
                move_bkg(0U, 112U-progress);
            }
        }

        if(CLICKED(J_START)) {
            mus_setPaused(1U);

            clearSprites();
            scene_state = enterPause();
            clearSprites();
            STOP_MUSIC;

            if(sgb_mode) {
                BGP_REG = 0x00U;
                delay(62U);
            }

            if(scene_state != INGAME_QUIT) {
                disable_interrupts();
                DISPLAY_OFF;
                restoreGame(1U, 1U);
                DISPLAY_ON;
                enable_interrupts();
                mus_setPaused(0U);
            }
        }

        clearRemainingSprites();
        snd_update();

        if(!vbl_count) wait_vbl_done();
        vbl_count = 0U;
    }

    if(scene_state == INGAME_DEAD) {
        if(level != 5U) STOP_MUSIC;
        deathAnimation();
        clearSprites();

        if(level == 5U) {
            stop_sound();
            addScoreInfinite();
            showInfiniteRestart();
            wait_sound_done();
        } else {
            fadeToWhite(8U);
            wait_sound_done();

            if(PROGRESS_POS(progress) > last_progress) {
                last_progress = PROGRESS_POS(progress);
            }
        }
    }
    else if(scene_state == INGAME_COMPLETED) {
        ending_flags = 0U;
        if(level == 4U) {
            ending_flags |= ENDING_FLAG_FROM_DREAM;
        }

        if(level == 5U) {
            if(wave != 998UL) wave++;
            intoPortalAnimation();
        } else {
            STOP_MUSIC;
            if(level == 3U && player_skin == 1U) {
                saveCatAnimation();
                gamestate = GAMESTATE_ENDING;
            } else {
                intoPortalAnimation();
                gamestate = GAMESTATE_WINSCREEN;
            }

            addScoreNormal();

            if(level > levels_completed) {
                levels_completed = level;
                levels_unlocked = level+1U;
                if(level == 3U) levels_unlocked += 2U;

                if(level == 1U) {
                    unlocked_bits = UNLOCKED_CLOUDS;
                }
                else if(level == 2U) {
                    unlocked_bits = UNLOCKED_SPACE | UNLOCKED_MUSIC;
                }
                else if(level == 3U) {
                    unlocked_bits = UNLOCKED_DREAM | UNLOCKED_HEAVEN;
                }
            }
        }
    }
    else if(scene_state == INGAME_QUIT) {
        gamestate = GAMESTATE_SELECT;
    }

    if(gamestate == GAMESTATE_INGAME) {
        goto ingame_start;
    }
}
