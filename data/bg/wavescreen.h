#ifndef WAVESCREEN_MAP_H
#define WAVESCREEN_MAP_H
#define wavescreen_data_length 2U
#define wavescreen_tiles_width 20U
#define wavescreen_tiles_height 24U
#define wavescreen_tiles_offset 0U
const unsigned char wavescreen_data[] = {
	255, 255,  16,   0,   0,  16
};
const unsigned char wavescreen_tiles[] = {
	  0,   0, 120,   1,   1, 255,   1,   1, 105
};
#define wavescreen_palette_data_length 1U
#define wavescreen_palette_offset 0U
const unsigned char wavescreen_palettes[] = {
      0,   0, 255,   0,   0, 225
};

const unsigned int wavescreen_palette_data[] = {
    32767,   0,   0,   0
};
#endif
