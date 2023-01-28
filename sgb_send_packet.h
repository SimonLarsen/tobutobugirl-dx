#ifndef SGB_SEND_PACKET_H
#define SGB_SEND_PACKET_H

void sgb_send_packet(UBYTE* data) OLDCALL;

/*
void sgb_send_packet(UBYTE* data) {
    UBYTE i, b, val;

    P1_REG = SGB_P1_NEUTRAL;

    P1_REG = 0U;
    P1_REG = SGB_P1_NEUTRAL;

    for(i = 0U; i != 16U; ++i) {
        val = data[i];
        for(b = 0U; b != 8U; ++b) {
            if(val & 1U) {
                P1_REG = SGB_P1_HIGH;
            } else {
                P1_REG = SGB_P1_LOW;
            }
            P1_REG = SGB_P1_NEUTRAL;
            val = val >> 1;
        }
    }

    P1_REG = SGB_P1_LOW;
    P1_REG = SGB_P1_NEUTRAL;
}
*/

#endif
