#include <gb/gb.h>

/*
Memory:
  - Level 1-4
	2 bytes per record. (seconds, score)
	10 bytes pr. level

	00-09	Level 1
	16-25	Level 2
	32-41	Level 3
	48-57   Level 4

  - Level 5
    6 bytes (3 words) per record. (seconds, stomps, waves)
    64-94

  -- Mini game
	100U Minigame minutes
	101U Minigame seconds

  - Options
	102U Show dash counter

  - Signature
	120-127 Ram signature "TOBUTOBU"
 */

UBYTE ram_data[128];
