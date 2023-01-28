#ifndef SET_DATA_RLE_H
#define SET_DATA_RLE_H

void set_bkg_data_rle(UBYTE first, UBYTE n, const UBYTE *data) OLDCALL;
void _set_tiles_rle(UBYTE x, UBYTE y, UBYTE width, UBYTE height, const UBYTE *tiles, UBYTE is_bkg) OLDCALL;

#define set_bkg_tiles_rle(x, y, width, height, tiles) (_set_tiles_rle((x), (y), (width), (height), (tiles), 1U))
#define set_win_tiles_rle(x, y, width, height, tiles) (_set_tiles_rle((x), (y), (width), (height), (tiles), 0U))

#endif
