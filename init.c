#include "defines.h"
#include "init.h"
#include "ram.h"
#include "gamestate.h"
#include "data/sgb/border.h"
#include "sgb_send_packet.h"

const UBYTE SGB_INIT1[16] = { 0x79U, 0x5DU, 0x08U, 0x00U, 0x0BU, 0x8CU, 0xD0U, 0xF4U, 0x60U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U };
const UBYTE SGB_INIT2[16] = { 0x79U, 0x52U, 0x08U, 0x00U, 0x0BU, 0xA9U, 0xE7U, 0x9FU, 0x01U, 0xC0U, 0x7EU, 0xE8U, 0xE8U, 0xE8U, 0xE8U, 0xE0U };
const UBYTE SGB_INIT3[16] = { 0x79U, 0x47U, 0x08U, 0x00U, 0x0BU, 0xC4U, 0xD0U, 0x16U, 0xA5U, 0xCBU, 0xC9U, 0x05U, 0xD0U, 0x10U, 0xA2U, 0x28U };
const UBYTE SGB_INIT4[16] = { 0x79U, 0x3CU, 0x08U, 0x00U, 0x0BU, 0xF0U, 0x12U, 0xA5U, 0xC9U, 0xC9U, 0xC8U, 0xD0U, 0x1CU, 0xA5U, 0xCAU, 0xC9U };
const UBYTE SGB_INIT5[16] = { 0x79U, 0x31U, 0x08U, 0x00U, 0x0BU, 0x0CU, 0xA5U, 0xCAU, 0xC9U, 0x7EU, 0xD0U, 0x06U, 0xA5U, 0xCBU, 0xC9U, 0x7EU };
const UBYTE SGB_INIT6[16] = { 0x79U, 0x26U, 0x08U, 0x00U, 0x0BU, 0x39U, 0xCDU, 0x48U, 0x0CU, 0xD0U, 0x34U, 0xA5U, 0xC9U, 0xC9U, 0x80U, 0xD0U };
const UBYTE SGB_INIT7[16] = { 0x79U, 0x1BU, 0x08U, 0x00U, 0x0BU, 0xEAU, 0xEAU, 0xEAU, 0xEAU, 0xEAU, 0xA9U, 0x01U, 0xCDU, 0x4FU, 0x0CU, 0xD0U };
const UBYTE SGB_INIT8[16] = { 0x79U, 0x10U, 0x08U, 0x00U, 0x0BU, 0x4CU, 0x20U, 0x08U, 0xEAU, 0xEAU, 0xEAU, 0xEAU, 0xEAU, 0x60U, 0xEAU, 0xEAU };

const UBYTE SGB_FREEZE[16] = { (0x17U << 3) + 1U, 2U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U };
const UBYTE SGB_UNFREEZE[16] = { (0x17U << 3) + 1U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U };

const UBYTE SGB_BORDER_CHR_TRN1[16] = { (0x13U << 3) + 1U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U };
const UBYTE SGB_BORDER_CHR_TRN2[16] = { (0x13U << 3) + 1U, 1U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U };
const UBYTE SGB_BORDER_PCT_TRN[16] = { (0x14U << 3) + 1U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U };

const UBYTE SGB_MLT_REQ1[16] = { (0x11U << 3) + 1U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U };
const UBYTE SGB_MLT_REQ2[16] = { (0x11U << 3) + 1U, 1U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U };

const UBYTE RAM_SIG[8] = {'T','O','B','U','T','O','B','U'};

void initRAM(UBYTE force_clear) {
    UBYTE *d1, *d2;
	UBYTE initialized;

	ENABLE_RAM_MBC1;
	SWITCH_RAM_MBC1(0);

	// Check for signature
	initialized = 1U;
    d1 = ram_data+RAM_SIG_ADDR;
    d2 = RAM_SIG;
    while(d2 != RAM_SIG+8) {
        if(*d1 != *d2) {
			initialized = 0U;
			break;
		}
        d1++;
        d2++;
	}

	// Initialize memory
	if(initialized == 0U || force_clear) {
        mymemset(ram_data, 0U, 128U);
        memcpy(ram_data+RAM_SIG_ADDR, RAM_SIG, 8U);
		*(ram_data + RAM_DASHCOUNTER) = 1U;
	}

	// Load values from ram
    d1 = ram_data;
	for(levels_completed = 0U; levels_completed != 4U; ++levels_completed) {
        if(*d1 == 0U) break;
        d1 += 16U;
	}
    levels_unlocked = levels_completed + 1U;
    if(levels_completed == 3U) levels_unlocked++;

	show_dashcounter = *(ram_data + RAM_DASHCOUNTER);

	DISABLE_RAM_MBC1;
}

void sgb_copy_rle(UBYTE *data, UBYTE *dest, UWORD n, UBYTE step) {
    UBYTE run, value;
    run = 0U;
    while(n) {
        if(!run) {
            value = data[0];
            if(value == data[1]) {
                run = data[2];
                data += 3U;
            } else {
                run = 1U;
                data++;
            }
        }
        *dest = value;
        dest += step;
        run--;
        n--;
    }
}

UBYTE sgb_check2() {
    UBYTE a, b;

    sgb_send_packet(SGB_MLT_REQ2); delay(62U);

    P1_REG = SGB_P1_NEUTRAL;

    a = P1_REG;
    a = P1_REG;
    a = P1_REG;
    a = P1_REG;

    P1_REG = SGB_P1_LOW;
    P1_REG = SGB_P1_HIGH;

    P1_REG = SGB_P1_NEUTRAL;
    b = P1_REG;
    b = P1_REG;
    b = P1_REG;
    b = P1_REG;

    sgb_send_packet(SGB_MLT_REQ1);

    return a != b;
}

void sgb_init() {
    UBYTE x, y, i;
    UBYTE *dout;

    BGP_REG = 0xE4U;
    OBP0_REG = 0xE4U;
    OBP1_REG = 0xE4U;

    delay(62U);
    sgb_send_packet(SGB_INIT1); delay(62U);
    sgb_send_packet(SGB_INIT2); delay(62U);
    sgb_send_packet(SGB_INIT3); delay(62U);
    sgb_send_packet(SGB_INIT4); delay(62U);
    sgb_send_packet(SGB_INIT5); delay(62U);
    sgb_send_packet(SGB_INIT6); delay(62U);
    sgb_send_packet(SGB_INIT7); delay(62U);
    sgb_send_packet(SGB_INIT8); delay(62U);

    sgb_send_packet(SGB_FREEZE); delay(62U);

    HIDE_SPRITES;
    HIDE_WIN;
    SHOW_BKG;

    SCX_REG = 0U;
    SCY_REG = 0U;

    delay(62U);

    LCDC_REG |= 16U;

    dout = (UBYTE*)0x9800UL;
    i = 0U;
    for(y = 0U; y != 18U; ++y) {
        for(x = 0U; x != 20U; ++x) {
            *dout = i;
            ++i;
            ++dout;
        }
        dout += 12UL;
    }

    sgb_copy_rle(border_data1, (UBYTE*)0x8000UL, 0x1000UL, 1U);

    DISPLAY_ON;
    delay(62U);
    sgb_send_packet(SGB_BORDER_CHR_TRN1); delay(62U);
    DISPLAY_OFF;

    sgb_copy_rle(border_data2, (UBYTE*)0x8000UL, 0x1000UL, 1U);

    DISPLAY_ON;
    delay(62U);
    sgb_send_packet(SGB_BORDER_CHR_TRN2); delay(62U);
    DISPLAY_OFF;

    sgb_copy_rle(border_tiles, (UBYTE*)0x8000UL, 0x400UL, 2U);
    sgb_copy_rle(border_palettes, (UBYTE*)0x8001UL, 0x400UL, 2U);
    memcpy((UBYTE*)0x8800UL, border_palette_data, 0x80U);

    DISPLAY_ON;
    delay(62U);
    sgb_send_packet(SGB_BORDER_PCT_TRN); delay(62U);
    DISPLAY_OFF;

    LCDC_REG &= 0xEFU; // 0b11101111

    dout = (UBYTE*)0x9000UL;
    for(i = 0U; i != 16U; ++i) {
        *dout = 0U;
        ++dout;
    }

    dout = (UBYTE*)0x9800UL;
    for(y = 0U; y != 18U; ++y) {
        for(x = 0U; x != 20U; ++x) {
            *dout = 0U;
            ++dout;
        }
        dout += 12UL;
    }

    DISPLAY_ON;
    delay(62U);
    sgb_send_packet(SGB_UNFREEZE); delay(62U);
}
