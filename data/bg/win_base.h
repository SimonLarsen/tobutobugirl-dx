#ifndef WIN_BASE_MAP_H
#define WIN_BASE_MAP_H
#define win_base_data_length 11U
#define win_base_tiles_width 20U
#define win_base_tiles_height 18U
#define win_base_tiles_offset 40U
const unsigned char win_base_data[] = {
	255,   0, 231,   0, 195,   0, 129,   0, 129,   0, 195,   0, 231,   0, 255,   0,
    255,  63, 192,  64, 128, 128,  12, 255, 255,   2,   0,   0,  14, 255, 252,   3,
      2,   1,   1,  12, 128, 128,  16,   0,   0,  16,   1,   1,  16, 255, 128,   0,
      0,  14, 255,   0,   0,  15, 255,   1,   0,   0,  28, 255, 255,   2
};
const unsigned char win_base_tiles[] = {
	 40,  40,   3,  41,  42,  42,  12,  43,  40,  40,   6,  44,  45,  45,  12,  46,  40,  40,   6,  44,
     45,  45,  12,  46,  40,  40,   3,  42,  42,   3,  47,  48,  48,  12,  49,  42,  42,   3,  45,  45,
    240,  50,  50,  20,  40,  40,  20
};



#endif
