data/bg/tangram.h: data/bg/tangram.png data/bg/tangram_dx.png
	$(IMGTOGB) --map --rle $< --dx data/bg/tangram_dx.png $@

data/bg/potato.h: data/bg/potato.png
	$(IMGTOGB) --map --rle $< $@

data/bg/potato_dx.h: data/bg/potato_dx.png
	$(IMGTOGB) --map --rle -c $< --dx data/bg/potato_dx.png $@

data/bg/titlescreen.h: data/bg/titlescreen.png
	$(IMGTOGB) --map --rle $< $@

data/bg/titlescreen_sgb.h: data/bg/titlescreen_sgb.png
	$(IMGTOGB) --map --rle $< $@

data/bg/titlescreen_dx.h: data/bg/titlescreen_dx.png data/palettes/titlescreen_base.png
	$(IMGTOGB) --map --rle -c $< $@ -I data/palettes/titlescreen_base.png

data/bg/titlescreen_bg.h: data/bg/titlescreen_bg.png
	$(IMGTOGB) --map --rle -O 117 $< $@

select_backgrounds: data/bg/catface.h data/bg/catface_dx.h data/bg/select.h

data/bg/catface.h: data/bg/catface.png
	$(IMGTOGB) --map -O 9 $< $@

data/bg/catface_dx.h: data/bg/catface_dx.png
	$(IMGTOGB) --map -O 9 $< $@

data/bg/select.h: data/bg/select.png data/bg/select_dx.png
	$(IMGTOGB) --map --rle -O 13 $< --dx data/bg/select_dx.png $@

hud.h hud.c: data/bg/hud.png
	$(IMGTOGB) --map --rle $< -C hud.c hud.h

hud_sgb.h hud_sgb.c: data/bg/hud_sgb.png
	$(IMGTOGB) --map --rle $< -C hud_sgb.c hud_sgb.h

hud_dx.h hud_dx.c: data/bg/hud_dx.png
	$(IMGTOGB) --map --rle -c -P 5 $< -C hud_dx.c hud_dx.h

data/bg/clock.h: data/bg/clock.png
	$(IMGTOGB) --map -O 21 --rle_data $< $@

background1.h background1.c: data/bg/background1.png
	$(IMGTOGB) --map -O 35 --rle $< -C background1.c background1.h

background1_dx.h background1_dx.c: data/bg/background1_dx.png
	$(IMGTOGB) --map -O 35 --rle -c $< -C background1_dx.c background1_dx.h

background2.h background2.c: data/bg/background2.png data/bg/background2_dx.png
	$(IMGTOGB) --map -O 35 --rle $< --dx data/bg/background2_dx.png -C background2.c background2.h

background3.h background3.c: data/bg/background3.png data/bg/background3_dx.png
	$(IMGTOGB) --map -O 35 --rle $< --dx data/bg/background3_dx.png -C background3.c background3.h

background4.h background4.c: data/bg/background4.png data/bg/background4_dx.png
	$(IMGTOGB) --map -O 35 --rle $< --dx data/bg/background4_dx.png -C background4.c background4.h

background5.h background5.c: data/bg/background5.png data/bg/background5_dx.png
	$(IMGTOGB) --map -O 35 --rle --split_tiles 9 $< -C background5.c background5.h

pause_bg.h pause_bg.c: data/bg/pause_bg.png data/bg/pause_bg_dx.png
	$(IMGTOGB) --map -O 35 --rle $< --dx data/bg/pause_bg_dx.png -C pause_bg.c pause_bg.h

data/bg/win_base.h: data/bg/win_base.png
	$(IMGTOGB) --map -O 40 --rle $< $@

data/bg/win_base_dx.h: data/bg/win_base_dx.png data/palettes/win_base.png
	$(IMGTOGB) --map -O 40 --rle $< -c $@ -I data/palettes/win_base.png

data/bg/rank_banner.h: data/bg/rank_banner.png
	$(IMGTOGB) --map -O 64 --rle $< $@

data/bg/rank_banner_dx.h: data/bg/rank_banner_dx.png
	$(IMGTOGB) --map -O 64 --rle $< $@

win1.c win1.h: data/bg/win1.png
	$(IMGTOGB) --map -O 91 --rle $< -C win1.c win1.h

win1_dx.c win1_dx.h: data/bg/win1_dx.png
	$(IMGTOGB) --map -O 91 --rle $< -c -P 2 -C win1_dx.c win1_dx.h

win2.c win2.h: data/bg/win2.png
	$(IMGTOGB) --map -O 91 --rle $< -P 2 -C win2.c win2.h

win2_dx.c win2_dx.h: data/bg/win2_dx.png
	$(IMGTOGB) --map -O 91 --rle $< -P 2 -c -C win2_dx.c win2_dx.h

win3.c win3.h: data/bg/win3.png
	$(IMGTOGB) --map -O 91 --rle $< -P 2 -C win3.c win3.h

win3_dx.c win3_dx.h: data/bg/win3_dx.png
	$(IMGTOGB) --map -O 91 --rle $< -P 2 -c -C win3_dx.c win3_dx.h

win4.c win4.h: data/bg/win4.png
	$(IMGTOGB) --map -O 91 --rle $< -P 2 -C win4.c win4.h

win4_dx.c win4_dx.h: data/bg/win4_dx.png
	$(IMGTOGB) --map -O 91 --rle $< -P 2 -c -C win4_dx.c win4_dx.h

win5.c win5.h: data/bg/win5.png
	$(IMGTOGB) --map -O 91 --rle $< -P 2 -C win5.c win5.h

win5_dx.c win5_dx.h: data/bg/win5_dx.png
	$(IMGTOGB) --map -O 91 --rle $< -P 2 -c -C win5_dx.c win5_dx.h

data/bg/highscore.h: data/bg/highscore.png
	$(IMGTOGB) --map -O 47 --rle $< $@

data/bg/highscore_dx.h: data/bg/highscore_dx.png data/palettes/highscore.png
	$(IMGTOGB) --map -O 47 --rle $< -c -I data/palettes/highscore.png $@

data/sprite/empty.h: data/sprite/empty.png
	$(IMGTOGB) $< $@

data/bg/ending_thanks.h: data/bg/ending_thanks.png
	$(IMGTOGB) --map -O 174 --rle $< $@

data/bg/ending_thanks_dx.h: data/bg/ending_thanks_dx.png
	$(IMGTOGB) --map -O 174 $< --rle $@

unlocked_backgrounds: data/bg/unlocked.h

data/bg/unlocked.h: data/bg/unlocked.png
	$(IMGTOGB) --map --rle -O 47 $< $@

data/bg/jukebox.h: data/bg/jukebox.png
	$(IMGTOGB) --map --rle -O 37 $< $@

data/bg/jukebox_dx.h: data/bg/jukebox_dx.png
	$(IMGTOGB) --map --rle -c -O 37 $< $@

data/bg/wipe.h: data/bg/wipe.png data/bg/wipe_dx.png
	$(IMGTOGB) --map --rle -O 13 $< --dx data/bg/wipe_dx.png $@

data/bg/minigame_score_bg.h: data/bg/minigame_score_bg.png
	$(IMGTOGB) --map --rle -O 18 $< $@

selection1.c selection1.h: data/bg/selection1.png data/bg/selection1_dx.png
	$(IMGTOGB) --map -O 90 -P 3 -s 2 --rle_data $< --dx data/bg/selection1_dx.png -C selection1.c selection1.h

selection2.c selection2.h: data/bg/selection2.png data/bg/selection2_dx.png
	$(IMGTOGB) --map -O 90 -P 3 -s 2 --rle_data $< --dx data/bg/selection2_dx.png -C selection2.c selection2.h

selection3.c selection3.h: data/bg/selection3.png data/bg/selection3_dx.png
	$(IMGTOGB) --map -O 90 -P 3 -s 2 --rle_data $< --dx data/bg/selection3_dx.png -C selection3.c selection3.h

selection4.c selection4.h: data/bg/selection4.png data/bg/selection4_dx.png
	$(IMGTOGB) --map -O 90 -P 3 -s 2 --rle_data $< --dx data/bg/selection4_dx.png -C selection4.c selection4.h

selection5.c selection5.h: data/bg/selection5.png
	$(IMGTOGB) --map -O 90 -P 3 -s 2 --rle_data $< -C selection5.c selection5.h

selection5_dx.c selection5_dx.h: data/bg/selection5_dx.png
	$(IMGTOGB) --map -O 90 -P 3 -s 2 --rle_data -c $< -C selection5_dx.c selection5_dx.h

selection_highscore.c selection_highscore.h: data/bg/selection_highscore.png
	$(IMGTOGB) --map -O 90 -P 3 -s 2 --rle_data $< -C selection_highscore.c selection_highscore.h

selection_highscore_dx.c selection_highscore_dx.h: data/bg/selection_highscore_dx.png
	$(IMGTOGB) --map -O 90 -P 3 -s 2 -c --rle_data $< -C selection_highscore_dx.c selection_highscore_dx.h

selection_jukebox.c selection_jukebox.h: data/bg/selection_jukebox.png
	$(IMGTOGB) --map -O 90 -P 3 -s 2 --rle_data $< -C selection_jukebox.c selection_jukebox.h

selection_jukebox_dx.c selection_jukebox_dx.h: data/bg/selection_jukebox_dx.png
	$(IMGTOGB) --map -O 90 -P 3 -s 2 -c --rle_data $< -C selection_jukebox_dx.c selection_jukebox_dx.h

selection_locked.c selection_locked.h: data/bg/selection_locked.png
	$(IMGTOGB) --map -O 90 -P 3 -s 2 --rle_data $< -C selection_locked.c selection_locked.h

selection_locked_dx.c selection_locked_dx.h: data/bg/selection_locked_dx.png
	$(IMGTOGB) --map -O 90 -P 3 -s 2 -c --rle_data $< -C selection_locked_dx.c selection_locked_dx.h

circles.c circles.h: data/bg/circles.png
	$(IMGTOGB) --map $< -C circles.c circles.h

intro_bg.c intro_bg.h: data/bg/intro_bg.png
	$(IMGTOGB) --map $< --rle -C intro_bg.c intro_bg.h

intro_bg_dx.c intro_bg_dx.h: data/bg/intro_bg_dx.png
	$(IMGTOGB) --map -c $< --rle -C intro_bg_dx.c intro_bg_dx.h

zoom_circles.c zoom_circles.h: data/bg/zoom_circles.png
	$(IMGTOGB) --map $< -C zoom_circles.c zoom_circles.h

characters.c characters.h: data/sprite/characters.png
	$(IMGTOGB) $< -C characters.c characters.h
