CC=lcc
CFLAGS=-Wa-l -Wl-m -Wl-j
SDCCN=/home/simon/share/sdcc-3.8.0/bin/sdcc -mgbz80 --no-std-crt0 -I "${GBDKNDIR}/include" -I "${GBDKNDIR}/include/asm"

default: ASMFILES

ASMFILES: freq.asm music.asm noisefreq.asm player.asm vib.asm

%.asm: %.c
	$(SDCCN) -c $< ; perl -pi -e 's/\s+\.optsdcc.*//g' $@ && python ../../scripts/fixcode.py $@ $@
