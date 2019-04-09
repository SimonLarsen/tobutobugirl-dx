#include <string.h>
#include "defines.h"
#include "sgb.h"
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

const UBYTE SGB_FREEZE[16] = { (0x17U << 3) + 1U, 1U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U };
const UBYTE SGB_UNFREEZE[16] = { (0x17U << 3) + 1U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U };

const UBYTE SGB_BORDER_CHR_TRN1[16] = { (0x13U << 3) + 1U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U };
const UBYTE SGB_BORDER_CHR_TRN2[16] = { (0x13U << 3) + 1U, 1U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U };
const UBYTE SGB_BORDER_PCT_TRN[16] = { (0x14U << 3) + 1U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U };

const UBYTE SGB_MLT_REQ1[16] = { (0x11U << 3) + 1U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U };
const UBYTE SGB_MLT_REQ2[16] = { (0x11U << 3) + 1U, 1U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U };

const UBYTE SGB_PAL01[16] = { (0x00U << 3) + 1U, 255, 127, 60, 66, 144, 12, 0, 0, 60, 66, 144, 12, 0, 0, 0 };

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

    sgb_send_packet(SGB_MLT_REQ2); delay(65U);

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

    sgb_send_packet(SGB_MLT_REQ1); delay(65U);

    return a != b;
}

void sgb_init() {
    UBYTE x, y, i;
    UBYTE *dout;

    DISPLAY_OFF;

    sgb_send_packet(SGB_FREEZE); delay(65U);

    BGP_REG = 0xE4U;
    OBP0_REG = 0xE4U;
    OBP1_REG = 0xE4U;

    sgb_send_packet(SGB_INIT1); delay(65U);
    sgb_send_packet(SGB_INIT2); delay(65U);
    sgb_send_packet(SGB_INIT3); delay(65U);
    sgb_send_packet(SGB_INIT4); delay(65U);
    sgb_send_packet(SGB_INIT5); delay(65U);
    sgb_send_packet(SGB_INIT6); delay(65U);
    sgb_send_packet(SGB_INIT7); delay(65U);
    sgb_send_packet(SGB_INIT8); delay(65U);

    HIDE_SPRITES;
    HIDE_WIN;
    SHOW_BKG;

    SCX_REG = 0U;
    SCY_REG = 0U;

    LCDC_REG |= 16U;

    dout = (UBYTE*)0x9800UL;
    i = 0U;
    for(y = 0U; y != 18U; ++y) {
        for(x = 0U; x != 20U; ++x) {
            *dout = i;
            ++i;
            ++dout;
        }
        dout += 12U;
    }

    sgb_copy_rle(border_data1, (UBYTE*)0x8000UL, 0x1000UL, 1U);

    DISPLAY_ON;
    sgb_send_packet(SGB_BORDER_CHR_TRN1); delay(65U);
    DISPLAY_OFF;

    sgb_copy_rle(border_data2, (UBYTE*)0x8000UL, 0x1000UL, 1U);

    DISPLAY_ON;
    sgb_send_packet(SGB_BORDER_CHR_TRN2); delay(65U);
    DISPLAY_OFF;

    sgb_copy_rle(border_tiles, (UBYTE*)0x8000UL, 0x400UL, 2U);
    sgb_copy_rle(border_palettes, (UBYTE*)0x8001UL, 0x400UL, 2U);
    memcpy((UBYTE*)0x8800UL, border_palette_data, 0x80U);

    DISPLAY_ON;
    sgb_send_packet(SGB_BORDER_PCT_TRN); delay(65U);
    DISPLAY_OFF;

    sgb_send_packet(SGB_PAL01); delay(65U);

    sgb_send_packet(SGB_UNFREEZE); delay(65U);

    LCDC_REG ^= 16U;
    HIDE_BKG;
}
