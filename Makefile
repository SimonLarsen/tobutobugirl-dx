GBDKDIR = ./gbdk
PYIMGTOGBDIR = ./pyimgtogb
CC=${GBDKDIR}/bin/lcc
CFLAGS=-Wl-m -Wl-j -msm83:gb -Wf--no-std-crt0
IMGTOGB=${PYIMGTOGBDIR}/imgtogb.py
IMGTOSGB=${PYIMGTOGBDIR}/imgtosgb.py
GETPALETTE=${PYIMGTOGBDIR}/imgtogbpal.py
MMLGB=mmlgb/parser/MMLGB.jar

define compile-source
	$(CC) $(CFLAGS) -c $<
endef

define compile-asm
	$(CC) $(CFLAGS) -c $< -o $@
endef

define make-palette
	$(GETPALETTE) $< $@
endef

default: tobudx.gb


clean: tidy
	find data/songs \( -iname '*.asm' -o -iname '*.h' \) -delete


cfiles = $(wildcard *.c)
tidy:
	rm -f tobudx.gb $(OBJ) $(OBJ_BANK1) $(OBJ_BANK2) $(OBJ_BANK3) $(OBJ_BANK4) $(OBJ_BANK5) $(OBJ_BANK6) \
	$(OBJ_BANK7) $(OBJ_BANK8) $(OBJ_BANK9) $(OBJ_BANK10) $(OBJ_BANK11) $(OBJ_SONGS) tobudx.map tobudx.sym tobudx.rel \
	tobudx.ihx tobudx.noi
	find ./ \( -iname '*.lst' -o -iname '*.sym' \) -delete
	rm -f $(cfiles:.c=.asm) $(cfiles:.c=.rel)


include backgrounds.mk
include sprites.mk
include sgb.mk
include songs.mk

OBJ=main.o fade.o gamestate.o set_data_rle.o cos.o circles.o zoom_circles.o characters.o arrow.o sound.o \
	mmlgb/driver/music.o mmlgb/driver/freq.o mmlgb/driver/noisefreq.o mmlgb/driver/vib.o \
	math.o getpal.o sgb_send_packet.o mymemset.o

OBJ_BANK1=game.o pause.o
OBJ_BANK2=select.o highscore.o unlocked.o
OBJ_BANK3=intro.o intro_bg.o intro_bg_dx.o ending.o
OBJ_BANK4=jukebox.o
OBJ_BANK5=winscreen.o logos.o
OBJ_BANK6=title.o
OBJ_BANK7=background1.o background1_dx.o background2.o background3.o background4.o
OBJ_BANK8=pause_bg.o pause_cloud1.o pause_cloud2.o pause_cloud3.o minigamescore.o \
		  background5.o background5_sgb.o background5_data_ptrs.o
OBJ_BANK9=win1.o win1_dx.o win2.o win2_dx.o win3.o win3_dx.o win4.o win4_dx.o win5.o win5_dx.o \
		  hud.o hud_dx.o hud_sgb.o
OBJ_BANK10=sound_data.o init.o wipe.o
OBJ_BANK11=set_banner.o banner_data_ptrs.o \
		   selection1.o selection2.o selection3.o selection4.o \
		   selection5.o selection5_dx.o \
		   selection_highscore.o selection_highscore_dx.o \
		   selection_jukebox.o selection_jukebox_dx.o \
		   selection_locked.o selection_locked_dx.o

OBJ_SONGS=title_song.o mainmenu_song.o score_tally_song.o highscore_song.o plains_song.o \
		clouds_song.o space_song.o dream_song.o dream_score_song.o intro_song.o \
		ending_part1_song.o ending_part2_song.o potato_jingle_song.o tangram_shine_song.o \
		tangram_vox_song.o level_clear_song.o unlocked_song.o minigame_song.o potaka_song.o \
		heaven_song.o

$(OBJ_BANK1): CFLAGS+=-Wf-bo1
$(OBJ_BANK2): CFLAGS+=-Wf-bo2
$(OBJ_BANK3): CFLAGS+=-Wf-bo3
$(OBJ_BANK4): CFLAGS+=-Wf-bo4
$(OBJ_BANK5): CFLAGS+=-Wf-bo5
$(OBJ_BANK6): CFLAGS+=-Wf-bo6
$(OBJ_BANK7): CFLAGS+=-Wf-bo7
$(OBJ_BANK8): CFLAGS+=-Wf-bo8
$(OBJ_BANK9): CFLAGS+=-Wf-bo9
$(OBJ_BANK10): CFLAGS+=-Wf-bo10
$(OBJ_BANK11): CFLAGS+=-Wf-bo11

$(RAM_BANK1): CFLAGS+=-ba0


ifeq (,$(filter clean tidy,$(MAKECMDGOALS)))
$(info $(shell make -C mmlgb/driver))
endif


arrow.asm: arrow.c arrow.h
	${compile-source}

background1.asm: background1.c background1.h
	${compile-source}

background1_dx.asm: background1_dx.c background1_dx.h
	${compile-source}

background2.asm: background2.c background2.h
	${compile-source}

background3.asm: background3.c background3.h
	${compile-source}

background4.asm: background4.c background4.h
	${compile-source}

background5.asm: background5.c background5.h
	${compile-source}

background5_sgb.asm: background5_sgb.c background5_sgb.h
	${compile-source}

background5_palettes.asm: background5_palettes.c background5_palettes.h
	${compile-source}

pause_bg.asm: pause_bg.c pause_bg.h
	${compile-source}

pause_cloud1.asm: pause_cloud1.c pause_cloud1.h
	${compile-source}

pause_cloud2.asm: pause_cloud2.c pause_cloud2.h
	${compile-source}

pause_cloud3.asm: pause_cloud3.c pause_cloud3.h
	${compile-source}

catface.asm: catface.c catface.h
	${compile-source}

catface_dx.asm: catface_dx.c catface_dx.h
	${compile-source}

characters.asm: characters.c characters.h
	${compile-source}

circles.asm: circles.c circles.h
	${compile-source}

cos.asm: cos.c cos.h
	${compile-source}

ending.asm: ending.c defines.h ending.h gamestate.h set_data_rle.h sgb_send_packet.h fade.h intro_bg.h intro_bg_dx.h data/palettes/ending_sprites.h data/bg/ending_thanks.h data/bg/ending_thanks_dx.h data/sprite/ending_sprites1.h data/sprite/ending_sprites2.h mmlgb/driver/music.h
	${compile-source}

fade.asm: fade.c gamestate.h sound.h fade.h
	${compile-source}

game.asm: game.c defines.h game.h fade.h gamestate.h cos.h ram.h highscore.h sound.h mmlgb/driver/music.h pause.h sgb_send_packet.h data/palettes/sprites.h data/bg/clock.h data/sprite/sprites.h data/sprite/portal.h data/sprite/skin1.h data/sprite/skin2.h data/sprite/skin3.h
	${compile-source}

gamestate.asm: gamestate.c defines.h gamestate.h set_data_rle.h background1.h background1_dx.h background2.h background3.h background4.h background5.h background5_sgb.h background5_palettes.h pause_bg.h win1.h win1_dx.h win2.h win2_dx.h win3.h win3_dx.h win4.h win4_dx.h win5_dx.h win5.h win5_dx.h pause_cloud1.h pause_cloud2.h pause_cloud3.h hud.h hud_dx.h hud_sgb.h mmlgb/driver/music.h
	${compile-source}

highscore.asm: highscore.c defines.h gamestate.h set_data_rle.h fade.h cos.h highscore.h ram.h sound.h sgb_send_packet.h characters.h arrow.h data/palettes/highscore_sprites.h data/sprite/empty.h data/bg/highscore.h data/bg/highscore_dx.h circles.h selection1.h selection2.h selection3.h selection4.h selection5.h selection_locked.h
	${compile-source}

intro_bg.asm: intro_bg.c intro_bg.h
	${compile-source}

intro_bg_dx.asm: intro_bg_dx.c intro_bg_dx.h
	${compile-source}

intro.asm: intro.c defines.h fade.h gamestate.h set_data_rle.h intro.h intro_bg_dx.h sgb_send_packet.h data/palettes/intro_sprites.h data/sprite/intro_sprites.h data/sprite/intro_flash.h
	${compile-source}

jukebox.asm: jukebox.c defines.h jukebox.h fade.h gamestate.h set_data_rle.h cos.h sound.h sgb_send_packet.h mmlgb/driver/music.h arrow.h data/palettes/digital.h data/palettes/jukebox_sprites.h data/bg/jukebox.h data/bg/jukebox_dx.h data/sprite/digital.h data/sprite/notes.h data/sprite/bobblehead.h data/sprite/bobblehead_dx.h
	${compile-source}

logos.asm: logos.c defines.h gamestate.h set_data_rle.h logos.h fade.h sound.h mmlgb/driver/music.h sgb_send_packet.h data/bg/tangram.h data/bg/potato.h data/bg/potato_dx.h data/sprite/shine.h
	${compile-source}

main.asm: main.c gamestate.h main.h init.h sound.h mmlgb/driver/music.h logos.h intro.h title.h select.h game.h winscreen.h highscore.h unlocked.h jukebox.h ending.h wipe.h minigamescore.h
	${compile-source}

minigamescore.asm: minigamescore.c defines.h fade.h gamestate.h set_data_rle.h sound.h ram.h sgb_send_packet.h data/palettes/minigame_score.h data/bg/minigame_score_bg.h characters.h zoom_circles.h
	${compile-source}

pause.asm: pause.c defines.h gamestate.h fade.h sound.h ram.h sgb_send_packet.h mmlgb/driver/music.h characters.h
	${compile-source}

select.asm: select.c defines.h select.h fade.h gamestate.h set_data_rle.h cos.h ram.h sound.h sgb_send_packet.h mmlgb/driver/music.h characters.h arrow.h circles.h data/palettes/select_sprites.h data/sprite/togglecat.h data/sprite/togglepanda.h data/bg/catface.h data/bg/catface_dx.h data/bg/select.h selection1.h selection2.h selection3.h selection4.h selection5.h selection5_dx.h selection_highscore.h selection_highscore_dx.h selection_jukebox.h selection_jukebox_dx.h selection_locked.h selection_locked_dx.h
	${compile-source}

selection1.asm: selection1.c selection1.h
	${compile-source}

selection2.asm: selection2.c selection2.h
	${compile-source}

selection3.asm: selection3.c selection3.h
	${compile-source}

selection4.asm: selection4.c selection4.h
	${compile-source}

selection5.asm: selection5.c selection5.h
	${compile-source}

selection5_dx.asm: selection5_dx.c selection5_dx.h
	${compile-source}

selection_highscore.asm: selection_highscore.c selection_highscore.h
	${compile-source}

selection_highscore_dx.asm: selection_highscore_dx.c selection_highscore_dx.h
	${compile-source}

selection_jukebox.asm: selection_jukebox.c selection_jukebox.h
	${compile-source}

selection_jukebox_dx.asm: selection_jukebox_dx.c selection_jukebox_dx.h
	${compile-source}

selection_locked.asm: selection_locked.c selection_locked.h
	${compile-source}

selection_locked_dx.asm: selection_locked_dx.c selection_locked_dx.h
	${compile-source}

set_banner.asm: set_banner.c set_banner.h set_data_rle.h gamestate.h selection1.h selection2.h selection3.h selection4.h selection5.h selection5_dx.h selection_highscore.h selection_highscore_dx.h selection_jukebox.h selection_jukebox_dx.h selection_locked.h selection_locked_dx.h
	${compile-source}

init.asm: init.c init.h data/sgb/border.h sgb_send_packet.h
	${compile-source}

sound.asm: sound.c sound.h gamestate.h mmlgb/driver/music.h mmlgb/driver/notes.h mmlgb/driver/freq.h mmlgb/driver/noisefreq.h mmlgb/driver/vib.h
	${compile-source}

sound_data.asm: sound_data.c data/sounds/sfx_bump.h data/sounds/sfx_bump_alien.h data/sounds/sfx_cat_disable.h data/sounds/sfx_cat_enable.h data/sounds/sfx_dash.h data/sounds/sfx_highscore_switch.h data/sounds/sfx_jetpack.h data/sounds/sfx_menu_cancel.h data/sounds/sfx_menu_confirm.h data/sounds/sfx_menu_locked.h data/sounds/sfx_menu_switch.h data/sounds/sfx_player_die.h data/sounds/sfx_stomp_alien.h data/sounds/sfx_stomp_bat.h data/sounds/sfx_stomp_bird.h data/sounds/sfx_stomp_ghost.h data/sounds/sfx_time_low.h data/sounds/sfx_time_out.h data/sounds/sfx_time_pickup.h data/sounds/sfx_warp_start.h data/sounds/sfx_rank_crash.h
	${compile-source}

title.asm: title.c defines.h title.h fade.h gamestate.h set_data_rle.h sound.h sgb_send_packet.h ram.h data/palettes/title_sprites.h data/palettes/minigame_fade.h data/bg/titlescreen.h data/bg/titlescreen_dx.h data/bg/titlescreen_bg.h data/bg/titlescreen_sgb.h characters.h data/sprite/title_cat.h
	${compile-source}

unlocked.asm: unlocked.c defines.h unlocked.h fade.h gamestate.h set_data_rle.h sgb_send_packet.h characters.h zoom_circles.h data/palettes/unlocked.h data/bg/unlocked.h selection2.h selection3.h selection4.h selection_jukebox.h
	${compile-source}

win1.asm: win1.c win1.h
	${compile-source}

win1_dx.asm: win1_dx.c win1_dx.h
	${compile-source}

win2.asm: win2.c win2.h
	${compile-source}

win2_dx.asm: win2_dx.c win2_dx.h
	${compile-source}

win3.asm: win3.c win3.h
	${compile-source}

win3_dx.asm: win3_dx.c win3_dx.h
	${compile-source}

win4.asm: win4.c win4.h
	${compile-source}

win4_dx.asm: win4_dx.c win4_dx.h
	${compile-source}

win5.asm: win5.c win5.h
	${compile-source}

win5_dx.asm: win5_dx.c win5_dx.h
	${compile-source}

hud.asm: hud.c hud.h
	${compile-source}

hud_dx.asm: hud_dx.c hud_dx.h

	${compile-source}

hud_sgb.asm: hud_sgb.c hud_sgb.h
	${compile-source}

winscreen.asm: winscreen.c defines.h gamestate.h set_data_rle.h fade.h winscreen.h sound.h sgb_send_packet.h characters.h data/palettes/ranks.h data/bg/win_base.h data/bg/win_base_dx.h data/bg/rank_banner.h data/bg/rank_banner_dx.h data/sprite/ranks.h circles.h mmlgb/driver/music.h mmlgb/driver/notes.h mmlgb/driver/freq.h
	${compile-source}

wipe.asm: wipe.c main.h wipe.h defines.h fade.h gamestate.h set_data_rle.h sound.h circles.h data/bg/wipe.h data/sprite/wipe_marker.h
	${compile-source}

zoom_circles.asm: zoom_circles.c zoom_circles.h
	${compile-source}

data/sounds/%.h: data/sounds/%.mml
	java -jar $(MMLGB) $< $@

data/palettes/%.h: data/palettes/%.png
	${make-palette}

%.o: data/songs/%.asm
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.asm
	$(CC) $(CFLAGS) -c $< -o $@

mmlgb/driver/music.o: mmlgb/driver/music.asm
	${compile-asm}

mmlgb/driver/freq.o: mmlgb/driver/freq.asm
	${compile-asm}

mmlgb/driver/noisefreq.o: mmlgb/driver/noisefreq.asm
	${compile-asm}

mmlgb/driver/vib.o: mmlgb/driver/vib.asm
	${compile-asm}

tobudx.gb: $(OBJ) $(OBJ_SONGS) \
	$(OBJ_BANK1) $(OBJ_BANK2) $(OBJ_BANK3) $(OBJ_BANK4) \
	$(OBJ_BANK5) $(OBJ_BANK6) $(OBJ_BANK7) $(OBJ_BANK8) \
	$(OBJ_BANK9) $(OBJ_BANK10) $(OBJ_BANK11)
	$(CC) $(CFLAGS) -Wm-yS -Wm-ynTOBUDX -Wm-yt03 -Wm-yo16 -Wm-ya1 -Wm-yc -Wm-ys -Wm-yj $^ -o $@
