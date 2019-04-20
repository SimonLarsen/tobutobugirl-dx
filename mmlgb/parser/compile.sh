#/bin/sh

java -jar parser/MMLGB.jar "$1" driver/song.asm

lcc -c driver/song.asm -o driver/song.o
lcc -Wl-j driver/player.o driver/music.o driver/song.o driver/freq.o driver/noisefreq.o driver/vib.o -o rom.gb
