#!/bin/sh
echo -e "Compiling driver..."

cd driver
lcc -S freq.c
lcc -S player.c
lcc -S music.c
lcc -S noisefreq.c
lcc -S vib.c
cd ..

echo -e "\nCompiling parser..."
cd parser
ant
cd ..

echo -e "\nCreate archive"
mkdir -p dist
zip -r "dist/mmlgb-$(date '+%F-%H%M%S').zip" \
	compile.bat compile.sh \
	parser/MMLGB.jar \
	gbdk \
	driver/player.asm driver/music.asm driver/freq.asm driver/noisefreq.asm driver/vib.asm
