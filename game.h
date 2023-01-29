#ifndef GAME_H
#define GAME_H

//#define getSkinData() (player_skin == 1U ? skin1_data : skin2_data)
#define clearEntities() (mymemset(entity_type, 0U, MAX_ENTITIES))
#define killPlayer() (scene_state = INGAME_DEAD)

UBYTE *getSkinData();
void initGame();
void restoreGame(UBYTE update, UBYTE from_pause);
void updateInput();
void updatePlayer();
void updateHUD();
void updateHUDTime();
void bouncePlayer(UBYTE entity, UBYTE str);
void updateEntities();
UBYTE spawnEntity(UBYTE type, UBYTE x, UBYTE y, UBYTE dir);
void initSpawns();
void generateSpawnData();
void updateSpawns();

void introAnimation();
void intoPortalAnimation();
void saveCatAnimation();
void deathAnimation();

void addScoreNormal();
void addScoreInfinite();

void fadeSpritesToWhite(UBYTE delay);
void fadeSpritesToWhiteGB(UBYTE delay);
void fadeSpritesToWhiteCGB(UBYTE delay);

void showWaveScreen();
void enterGame();

#define SCRLMGN 84U

#define NO_LAST_SPAWN 255U

#define MOVE_SPEED        2U
#define DASH_SPEED        4U
#define DASH_TIME        14U
#define SHORT_DASH_TIME   8U
#define BUMP_SPEED       28U
#define ALIEN_BUMP_SPEED 19U
#define MAX_YSPEED       24U
#define MAX_FLY_SPEED    23U
#define MAX_DASHES        3U

#define KEY_DASH J_A
#define KEY_USE  J_B

#define SPAWN_INTERVAL 36U
#define MAX_TIME 32U
#define LOW_TIME 11U
#define CLOCK_BONUS 4U
#define CLOCK_BONUS_LOWTIME 2U

#define MAX_BOOST 128U
#define STOMP_BONUS 32U

// Entities
#define E_NONE     0U

#define E_SPIKES   1U
#define E_FIREBALL 2U
#define E_ALIEN    3U
#define E_BAT      4U
#define E_BIRD     5U
#define E_GHOST    6U
#define E_CLOCK    7U
#define E_PORTAL   8U
#define E_CLOUD	   9U

#define FIRST_ENEMY E_SPIKES
#define LAST_ENEMY E_GHOST

#define LAST_COLLIDABLE E_PORTAL

// States
#define INGAME_ACTIVE    0U
#define INGAME_DEAD      1U
#define INGAME_COMPLETED 2U
#define INGAME_QUIT      3U

// Wave effects
#define WAVE_SPC_SQUIDS        1U
#define WAVE_SPC_AUTOSCROLL    2U
#define WAVE_SPC_SPIKEDASH     4U
#define WAVE_SPC_GHOSTS        8U
#define WAVE_SPC_LOWTIME      16U
#define WAVE_SPC_DOUBLESPIKES 32U

#define WAVE_SPC_LOOP 35U // 7 * 5

#endif
