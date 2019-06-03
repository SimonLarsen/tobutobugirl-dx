#ifndef HIGHSCORE_MAP_H
#define HIGHSCORE_MAP_H
#define highscore_data_length 42U
#define highscore_tiles_width 20U
#define highscore_tiles_height 18U
#define highscore_tiles_offset 47U
const unsigned char highscore_data[] = {
	255,   0, 231,   0, 195,   0, 129,   0, 129,   0, 195,   0, 231,   0, 255,   0,
    255, 255,   2,   0, 255,   0, 255,   0, 255,   0, 255,   0, 255,   0, 255,   0,
    255,   0, 255,   0, 255,   0, 255,   0, 255,   0, 255,   0, 255,   0, 255,   0,
    255,   0, 230,   0, 230,   0, 230,   0, 224,   6, 230,   0, 230,   0, 230,  25,
    255,   0,  67,  36, 103,   0, 103,   0, 103,   0, 103,   0, 103,   0,  67, 188,
    255,   0, 135,  56,  59,   4,  63,   0,  35,  16,  51,   0,  51, 132, 135, 120,
    255,   0,  51,   0,  51,   0,  51,   0,   3,  48,  51,   0,  51,   0,  51, 204,
    255,   0, 135,  56,  59,   4,  63, 128, 135, 112, 243,   0, 115, 132, 135, 120,
    255,   0, 135,  56,  59,   4,  63,   0,  63,   0,  63,   0,  59, 132, 135, 120,
    255,   0, 135,  48,  51,   0,  51,   0,  51,   0,  51,   0,  51, 132, 135, 120,
    255,   0,   7,  48,  51,   0,  51,   4,   7,  48,  51,   0,  51,   0,  51, 204,
    255,   0,   3,  60,  63,   0,  63,   0,   7,  56,  63,   0,  63,   0,   3, 252,
    255,   0, 255,   0, 255,   0, 255,   0, 255,   0, 255,   0, 255,   0, 255, 255,
      3,   0,   0,  16, 255, 255,   2, 192, 128, 128,  13, 255, 255,   2,   0,   0,
     14, 255, 255,   2,   3,   1,   1,  13, 128, 128,  16,   1,   1,  16, 255, 255,
      2, 248, 128, 224, 128, 192, 128, 192, 128, 128,   3, 176, 128, 176, 128, 192,
    128,   0,   0,  14,   3,   1,   0,   0,  14, 255, 255,   2,  31,   1,   7,   1,
      3,   1,   3,   1,   1,   3,  13,   1,  13,   1, 128, 128,   2, 192, 128, 224,
    128, 224, 128, 224, 128, 224, 128, 224, 128, 224, 128,   1,   1,   2,   3,   1,
      7,   1,   7,   1,   7,   1,   7,   1,   7,   1,   7,   1, 224, 128, 224, 128,
    240, 128, 248, 128, 248, 128, 248, 128, 240, 128, 228, 128,   7,   1,   7,   1,
     15,   1,  31,   1,  31,   1,  31,   1,  15,   1,  39,   1, 238, 128, 228, 128,
    240, 128, 248, 128, 252, 128, 252, 128, 252, 128, 252, 128, 119,   1,  39,   1,
     15,   1,  31,   1,  63,   1,  63,   1,  63,   1,  63,   1, 248, 128, 243, 128,
    247, 128, 247, 128, 243, 128, 248, 128, 252, 128, 252, 128,   0,   0,   4, 128,
      0, 128,   0,   0,  13,   1,   0,   1,   0,   0,   9,  31,   1, 207,   1, 239,
      1, 239,   1, 207,   1,  31,   1,  63,   1,  63,   1, 252, 128, 252, 128, 252,
    128, 254, 128, 255, 128, 255, 128, 255, 128, 255, 128,   0,   0,  10, 128,   0,
    128,   0, 128,   0,   0,  11,   1,   0,   1,   0,   1,   0,  63,   1,  63,   1,
     63,   1, 127,   1, 255,   1, 255,   1, 255,   1, 255,   1, 255, 128, 254, 128,
    254, 128, 255, 128, 255, 128, 255, 128, 255, 128, 255, 255,   2,   0,   0,   2,
     96,   0,  96,   0,   0,   3, 192,   0, 224,   0, 248,   0, 255, 255,   2,   0,
      0,  14, 255, 255,   2,   0,   0,   2,   6,   0,   6,   0,   0,   3,   3,   0,
      7,   0,  31,   0, 255, 255,   3,   1, 127,   1, 127,   1, 255,   1, 255,   1,
    255,   1, 255,   1, 255, 255,   2
};
const unsigned char highscore_tiles[] = {
	 47,  47,  20,  48,  48,  20,  49,  49,   5,  50,  51,  52,  53,  54,  55,  56,  57,  58,  54,  49,
     49,   5,  59,  59,  20,  47,  47,   2,  60,  60,  16,  47,  47,   4,  60,  60,  16,  47,  47,   4,
     60,  60,  16,  47,  47,   4,  60,  60,  16,  47,  47,   4,  60,  60,   3,  61,  62,  62,   8,  63,
     60,  60,   3,  47,  47,   4,  60,  60,   3,  64,  60,  60,   8,  65,  60,  60,   3,  47,  47,   3,
     66,  67,  60,  60,  14,  68,  69,  47,  47,   2,  70,  60,  60,  16,  71,  47,  47,   2,  72,  60,
     60,  16,  73,  47,  47,   2,  74,  60,  60,  16,  75,  47,  47,   2,  76,  77,  60,  60,  14,  78,
     79,  47,  47,   2,  80,  81,  60,  60,  14,  82,  83,  47,  47,   2,  84,  85,  86,  86,  14,  87,
     88,  47,  47,  21
};


#endif
