GBDKDIR = ../../gbdk
CC=${GBDKDIR}/bin/lcc
CFLAGS=-Wa-l -Wl-m -Wl-j
SDCCN=${GBDKDIR}/bin/sdcc -mgbz80 --no-std-crt0 -I "${GBDKDIR}/include" -I "${GBDKDIR}/include/asm"

default: ASMFILES

ASMFILES: freq.asm music.asm noisefreq.asm player.asm vib.asm

%.asm: %.c
	$(SDCCN) -c $< ; perl -pi -e 's/\s+\.optsdcc.*//g' $@ && python ../../scripts/fixcode.py $@ $@
