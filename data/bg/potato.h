#ifndef POTATO_MAP_H
#define POTATO_MAP_H
#define potato_data_length 125U
#define potato_tiles_width 20U
#define potato_tiles_height 18U
#define potato_tiles_offset 0U
const unsigned char potato_data[] = {
	  0,   0,  30,  30,  30,   2,   0,   0,  14,  51,  51,   2,   0,   0,  14,  62,
     62,   2,  51,  51,   2,  60,  60,   2,  15,  15,   2,  51,  51,   2,  30,  30,
      2,   0,   0,   6,  51,  51,   8,  30,  30,   2,   0,   0,   6,  59,  59,   2,
     63,  63,   4,  55,  55,   2,  51,  51,   2,   0,   0,   6,  51,  51,   8,  62,
     62,   2,   0,   0,   6,  51,  51,   2,  63,  63,   2,  51,  51,   4,  62,  62,
      2,   0,   0,   6,  59,  59,   2,  31,  31,   2,  14,  14,   2,  28,  28,   2,
     24,  24,   2,   0,   0,  14,   7,   0,  24,   0,  32,   0,  64,   0,   0,   9,
    252,   0,   3,   0,   0,   3,  64,   0,   0,  11, 128,   0,   0,   2,  96,  16,
     16,   2,   0,   0,   2,   1,   0,   1,   0,   0,   2,   2,   0,   2,   0,   0,
      2,   1,   1,   2,   3,   3,   2, 128,   0,   1,   0,   1,   0,   0,   7, 192,
    192,   4, 128,   0, 192,   0, 224,   0,   0,  11,  12,  12,   2,   7,   3,  15,
      0,  63,   0, 127,   0,  31,   0,   0,   9, 128, 128,   2, 240, 112, 252,  14,
    192,   1,  28,  28,   2,  60,  60,   2,   0,   0,  11, 128,  96,  96,   2,  48,
     16,   0,   0,  12,  56,  56,   2, 120, 120,   2,   0,   0,  12, 115, 115,   2,
    247, 247,   2,   0,   0,  12, 240, 240,   2, 252, 252,   2,   0,   0,  12,   1,
      1,   2,   7,   7,   2,   0,   0,  12, 248, 248,   2, 254, 254,   2,   7,   7,
     10,  15,  15,   2,  63,  63,   4, 192, 192,  10, 248, 248,   4, 249, 249,   2,
      0,   0,   2, 192,   0, 128,   0,   0,   7, 127, 127,   2, 255, 255,   2,   0,
      0,  12, 195, 195,   2, 227, 227,   2, 124, 124,  10, 255, 255,   6,  62,  14,
     48,   1,   0,   0,   6, 128, 128,   2, 129, 129,   2, 135, 135,   2,   0,   0,
      3, 128,   0,  64,   0,  32,   0,  16,   0,   0,   2, 248, 248,   2, 254, 254,
      2,   0,   0,  10,   1,   1,   2,   7,   7,   4, 248, 248,  10, 255, 255,   6,
      0,   0,  12,  15,  15,   2,  63,  63,   2,   0,   0,  12, 248, 248,   2, 252,
    252,   2,   0,   0,  12,  30,  30,   2,  63,  63,   2,   0,   0,  12,  62,  62,
      2, 127, 127,   2,   1,   1,  16, 255, 255,  10, 252, 252,   2, 248, 248,   4,
    254, 254,   2, 255, 255,   8,  31,  31,   2,  15,  15,   6,  31,  31,   4, 191,
    191,   6, 190, 190,   4, 255, 255,  10,  15,  15,   2,   7,   7,   4,  63,  63,
      2, 159, 159,   2, 143, 143,   2, 207, 207,  10, 241, 241,   2, 224, 224,   2,
    192, 192,   8, 216, 192, 217, 193, 255, 255,   4,   3,   3,   2,   1,   1,   4,
     63,  63,   2, 255, 255,   4, 243, 243,   2, 249, 249,   2, 248, 248,  12, 255,
    255,   2, 254, 254,   2, 252, 252,  12,  15,  15,   2,  31,  31,   4,  63,  63,
      6,  62,  62,   4,   0,   0,   2, 128, 128,   4, 192, 192,   4, 195, 195,   2,
    199, 199,   2, 223, 223,   2,   7,   7,   2,   3,   3,   2,   1,   1,   6, 241,
    241,   2, 249, 249,   4, 254, 254,   2, 252, 252,   2, 248, 248,  12,  63,  63,
      2,  31,  31,   2,   0,   0,   6,   7,   7,   2,  31,  31,   2,  63,  63,   2,
    254, 254,   2, 255, 255,   2, 127, 127,   2,  63,  63,   4, 255, 255,   6,  63,
     63,  16, 127, 127,   2, 255, 255,  12, 159, 159,   2, 128, 128,  16, 248, 248,
      8, 252, 252,   2, 255, 255,   6,  15,  15,   8,  31,  31,   2, 255, 255,   6,
    190, 190,   8, 191, 191,   6,  31,  31,   2,   7,   7,   8,  15,  15,   2, 255,
    255,   6, 207, 207,  14, 143, 143,   2, 219, 195, 219, 195, 219, 195, 219, 195,
    203, 195, 227, 227,   2, 251, 251,   4, 255, 255,   4, 225, 225,   2, 193, 193,
      6, 227, 227,   2, 255, 255,   2, 248, 248,  16, 252, 252,  10, 254, 254,   2,
    255, 255,   4,  62,  62,   8,  63,  63,   4, 191, 191,   2, 159, 159,   2, 223,
    223,   2, 207, 207,   2, 192, 192,   7, 208, 192, 208, 128, 144, 225, 225,   2,
    193, 193,   2,   1,   1,  12, 248, 248,  10, 252, 252,   2, 255, 255,   4, 127,
    127,   4, 124, 124,   2, 120, 120,   6, 124, 124,   2, 127, 127,   2, 255, 255,
      4,  63,  63,   8, 127, 127,   2, 255, 255,   2,  31,  31,  16, 255, 255,   6,
    251, 251,   2, 248, 248,   8, 255, 255,   2, 254, 254,   2, 252, 252,   2, 240,
    240,   2,   0,   0,   8,  31,  31,   2,  15,  15,   2,   7,   7,   2,   1,   1,
      2,   0,   0,   8, 255, 255,   4, 254, 254,   2, 248, 248,   2,   0,   0,   8,
    143, 143,   2,   7,   7,   4,   3,   3,   2,   0,   0,   8, 251, 251,   4, 241,
    241,   2, 228, 224,  14,   8,  15,   8,   7,   4,   3,   2, 255, 255,   6, 254,
    254,   2,   0,   0,   2, 255,   0, 255,   0, 255,   0, 248, 248,   2, 252, 252,
      2, 125, 124,  61,  60, 131,   0, 255,   0, 255,   0, 255,   0, 255, 255,   2,
    127, 127,   2,  63,  63,   2, 158,  30, 192,   0, 255,   0, 255,   0, 255,   0,
    159, 159,   2, 143, 143,   2,  39,   7, 113,   1, 248,   0, 255,   0, 255,   0,
    255,   0, 255, 255,   4, 254, 254,   2, 248, 248,   2,   7,   0, 255,   0, 255,
      0, 255,   0, 144, 144,   2,  48,  16, 112,  16, 240,  16, 224,  32, 224,  32,
    192,  64, 128, 128,   2,   1,   1,   2,   0,   0,  14, 255, 255,   4, 254, 254,
      2, 124, 124,   2,   0,   0,   8, 127, 127,   4,  63,  63,   2,  31,  31,   2,
      0,   0,   8, 255, 255,   4, 239, 239,   2, 199, 199,   2,   0,   0,   8,  63,
     63,   2, 191, 191,   4, 159, 159,   2,   0,   0,   8,  31,  31,   4,  15,  15,
      2,   7,   7,   2,   0,   0,   8, 128, 128,   4, 192, 192,   4,   0,   0,   8,
      1,   1,  12,   0,   0,   4, 248, 248,  10, 240, 240,   2, 224, 224,   2,   0,
      0,   2, 255,   0, 255, 128, 127,  96,  31,  24,   7,   6,   1,   1,   2,   0,
      0,   4, 255,   0, 255,   0, 255,   0, 255,   0, 255,   0, 255, 192,  63,  63,
      2,   0,   0,   2, 255,   0, 255,   0, 255,   0, 255,   0, 255,   0, 255,   0,
    255, 255,   2,   0,   0,   2, 255,   0, 255,   0, 255,   0, 255,   0, 255,   7,
    248, 120, 128, 128,   2,   0,   0,   2, 255,   1, 254,   6, 248,  24, 224, 224,
      2,   0,   0,  18,   1,   1,   2,  10,  11,  14,  15,   0,   0,   4,  48,  63,
     95,  96, 159, 224,  63, 192,  63, 192,  63, 128,   0,   0,   4,  32, 224, 168,
     88, 156, 100, 254,   2, 244,  10, 255,   1,   0,   0,  10,  48,  48,   2, 126,
     78, 127,  81,   0,   0,   4,   1,   1,   4,   0,   0,   8,  14,  11,  30,  25,
    128, 255, 127, 128, 189, 194, 115, 124,  38,  57,  60,  35,  95, 128, 103, 128,
    114, 141, 239, 153, 157, 251, 254, 251,  58, 207,  53, 222, 254,   1, 255,   0,
     62,   0, 104, 144, 239, 144,  54, 153,  78, 249, 168, 127,   0,   0,   2, 128,
    128,   2,   0, 128,   0, 128,   0, 128, 131, 131,   2, 135, 132, 135, 132, 127,
     93, 247, 149, 255, 153, 111, 105,   6,   6,   2, 128, 128,   2, 192,  64, 224,
     32,  32,   2,  63,  25,  31,   7,   7,   2,   5,   6,  15,   8,  31,  31,   2,
     63,  32,  71, 120,  87, 252, 249, 254, 190, 217, 247,  24, 215,  56, 251, 252,
    223, 111, 238, 242, 245,  31, 182,  94,  52, 220, 224,  16, 192,  32,  64, 192,
    128, 128,   2,   0,   0,   2,   7,   5,   7,   5,  15,  13,  31,  17,  55,  33,
     30,  18,  12,  12,   2,   0,   0,   2, 224,  32, 224, 160,  64,  64,   2,   0,
      0,  10,  56,  71, 124, 131, 121, 135, 124, 131, 124, 131, 125, 130, 240, 140,
    134, 249, 242,  30,  44, 220,  68, 188, 138, 254, 213, 255, 122, 231, 217,  35,
     61,  17,   0,   0,   4,  15,  15,   2,  31,  16,  63,  32, 159, 224,  63, 192,
    255, 255,   2,   0,   0,   4, 255, 255,   3,   0, 255,   0, 255,   0, 255,   1,
    254, 255,   0,   0,   4, 255, 255,   3,   0, 255,   0, 255,   0, 255, 255,   2,
      0, 255,   0,   0,   4, 248, 248,   2, 252,   4, 254,   2, 255,   3, 255, 255,
      2,  31, 255, 246, 240, 144, 138, 147, 193, 130, 146, 245, 228,  56,  63,  67,
    124, 127, 127,   2,  31, 159, 120,  63, 224, 255, 127, 127,   2, 158, 126, 220,
     62, 228,  38, 192, 192,   2,  49, 206,  50, 205,  98, 157, 255, 255,   2,  24,
     24,   4,   0,   0,   5, 255,   0, 255, 127, 255, 240, 240,   2,   0,   0,   9,
    255, 255,   3,   3,   3,   2,   2,   3,   2,   3,   0,   0,   6,  30, 254, 252,
    252,   2,   0,   0,  12
};
const unsigned char potato_tiles[] = {
	  0,   0,  26,   1,   1,   2,   2,   2,   2,   3,   0,   3,   2,   0,   0,  12,   4,   5,   5,   2,
      6,   7,   0,   8,   9,   0,   0,  12,  10,  11,  12,   0,   0,  16,  13,  14,  15,  16,  17,  18,
      0,   0,   3,  19,   0,   0,   6,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,   0,  31,
     32,  33,  34,  35,  36,   0,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  41,  48,  49,
     50,  51,  52,  53,  54,  55,  37,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  59,  66,  67,
     68,  69,  70,  53,  71,  55,  37,  72,  73,  74,  75,  76,  77,  78,  79,  80,  81,  82,  83,  84,
     85,  86,  87,  88,  89,  90,  91,  92,   0,   0,   4,  84,  93,  94,  95,  96,  97,   0,   0,  41,
     98,  99, 100,   0, 101,   0,   0,  14, 102, 103, 104, 105, 106, 107,   0,   0,  15, 108, 109, 110,
    111, 112,   0,   0,  15, 113, 114, 115, 116, 117, 118,   0,   0,  14, 119, 120, 121, 122, 123, 124,
      0,   0,  41
};



#endif
