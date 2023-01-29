GBDKDIR = ../../gbdk
CC=${GBDKDIR}/bin/lcc
CFLAGS=-Wl-m -Wl-j -msm83:gb -Wf--no-std-crt0

default: ASMFILES

ASMFILES: freq.asm music.asm noisefreq.asm player.asm vib.asm

%.asm: %.c
	$(CC) $(CFLAGS) -c $< ; perl -pi -e 's/\s+\.optsdcc.*//g' $@ && python ../../scripts/fixcode.py $@ $@
