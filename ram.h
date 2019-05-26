#ifndef RAM_H
#define RAM_H

#define RAM_SIG_ADDR     120U
#define RAM_MINIGAME_MIN 100U
#define RAM_MINIGAME_SEC 101U
#define RAM_DASHCOUNTER  102U

//extern UBYTE ram_data[];
#define ram_data ((UBYTE*)0xA000UL)

#endif
