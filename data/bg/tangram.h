#ifndef TANGRAM_MAP_H
#define TANGRAM_MAP_H

#define tangram_data_length 46
const unsigned char tangram_data[] = {
	  0,   0,  38,  24,   0,  24,  60,  62, 124, 124,   2, 126, 236, 126,   0,   0, 
	 32,   7,  15,  15,   3,   8,   7,   0,   0,  10, 128, 255, 255,   5,   1,   0, 
	  1,   0,   1,   0,   0,   2,   1,   0,   1, 238, 255, 255,   5, 205, 254, 236, 
	220, 252, 220, 220,   2, 248, 248,   3,   1, 255, 255,   5, 192,   0,   0,   9, 
	128, 192, 128, 192,   0, 128,   0,   0,  24,  56,  28,   0,   0,  14,  64,  48, 
	  0,   0,  14,   3,   1,   0,   0,  14, 128, 192,   0,   0,  14,   8,  48,   0, 
	  1,   0,   1,   0,   1,   0,   1,   1,   2,   0,   1,   0,   0,   5, 240, 248, 
	240, 240,   3, 224, 192, 224, 224,   2, 192, 192,   2, 224, 232, 240, 255, 127, 
	  7,   0,  15,  31,  31,   2,  63, 127,  56,  56,   2, 112, 240, 112, 112,   2, 
	248, 253, 255,   0,   0,   2, 160, 192, 240, 224, 224,   2, 240, 112, 112,   2, 
	 96, 113, 115, 241, 255, 255,   2, 124,  62, 127, 126, 231, 127, 127,   2, 231, 
	239, 231, 231,   2, 199, 199,   4, 135, 112, 248, 248,   3, 252, 248, 248,   2, 
	220, 248, 156, 152, 156,  28, 156, 143,  31,   7,   0,  31,  15,  31,  63, 123, 
	 60,  48, 120, 120,   2, 112, 252, 120, 255, 255,   2, 128,   0, 224, 192, 224, 
	240, 113, 240,  48, 113,  49, 113, 241, 115, 255, 255,   2, 144,  96, 127, 255, 
	255,   3, 241, 207, 193, 193,   2, 129, 193, 192, 129, 129,   2, 128,   0,   0, 
	  3, 129, 195, 131, 195, 135, 131, 199, 195, 199, 239, 199, 255, 255,   2, 112, 
	  0, 248, 252, 255, 254, 254,   2, 143, 134,   7,   6,   7,   7,   2, 143, 223, 
	255,   7,   3,   7,   7,   2,  14,   7,   7,   2,  14,  12,  14,  14,   2,  28, 
	 60,  28, 252, 248, 233, 198, 255, 239, 127, 255, 255,   2, 127, 255, 123, 123, 
	  3, 115, 123, 123,   2, 115, 124, 120, 120,   2, 252, 252,   3, 236, 220, 204, 
	220, 140, 220, 142, 156,  15, 143,   0,   0,   9,  48, 120,  48, 120, 112, 224, 
	240, 127,  63,  23,  15,   0,   0,  12, 255, 223,   7, 143,   0,   0,  12, 255, 
	255,   2,  79, 158,   0,   0,  12,   3, 135,   3,   7,   7,   2,   2,   0,   0, 
	 10,  15,  15,   2,   3,   7,   0,   0,  12, 207, 255,  79, 135,   3,   3,   3, 
	  7,   7,   3,  14,   7,   6,  14,   7,  14, 255, 255,   2, 253, 254, 224, 240, 
	224, 240,  96, 240, 240,   2,  96, 224, 224,   2, 192, 224, 128,   0,   0,  15, 
	127, 255,  80,  60,   0,   0,  12, 255, 255,   2, 116, 249,   0,   0,  12, 240, 
	248, 240, 224,   0,   0,  12, 115, 112, 112,   3,  80,  32,   0,   0,  10,  15, 
	  7,   4,   3,   0,   0,  12, 224, 224,   2,   0, 192,   0,   0,  12,   7,  15, 
	 15,   2,   7,   7,   2,   3,   0,   0,  10, 224, 192, 128, 192,   0, 128,   0, 
	  0,  10
};

#define tangram_tiles_width 20
#define tangram_tiles_height 18
#define tangram_offset 0
const unsigned char tangram_tiles[] = {
	  0,   0, 142,   1,   0,   2,   3,   4,   0,   0,  15,   5,   6,   7,   8,   9, 
	 10,  11,   0,   0,   5,  12,  13,  14,   0,   0,   6,  15,  16,  17,  18,  19, 
	 20,  21,  22,  23,  24,  25,  26,  27,  28,  29,   0,   0,   6,  30,  31,  32, 
	 33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,   0,   0,  11,  44,  45, 
	  0,   0, 129
};

#endif