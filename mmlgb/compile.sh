#/bin/sh

java -jar parser/MMLGB.jar "$1" driver/song.asm

../gbdk/bin/lcc -Wl-j driver/player.asm driver/music.asm driver/song.asm driver/freq.asm driver/noisefreq.asm driver/vib.asm -o rom.gb
