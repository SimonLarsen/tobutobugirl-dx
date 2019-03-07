songs: $(SONGS_BANK11) $(SONGS_BANK12) $(SONGS_BANK13) $(SONGS_BANK14) $(SONGS_BANK15)

SONGS_BANK11=data/songs/title_song.asm data/songs/mainmenu_song.asm data/songs/score_tally_song.asm data/songs/highscore_song.asm

SONGS_BANK12=data/songs/plains_song.asm data/songs/clouds_song.asm data/songs/space_song.asm

SONGS_BANK13=data/songs/intro_song.asm data/songs/dream_song.asm data/songs/dream_score_song.asm

SONGS_BANK14=data/songs/ending_part1_song.asm data/songs/ending_part2_song.asm data/songs/potato_jingle_song.asm data/songs/tangram_shine_song.asm data/songs/tangram_vox_song.asm data/songs/level_clear_song.asm data/songs/unlocked_song.asm

SONGS_BANK15=data/songs/minigame_song.asm data/sounds/potaka_song.asm

$(SONGS_BANK11): MMLBANK:=11
$(SONGS_BANK12): MMLBANK:=12
$(SONGS_BANK13): MMLBANK:=13
$(SONGS_BANK14): MMLBANK:=14
$(SONGS_BANK15): MMLBANK:=15

data/songs/%_song.asm: data/songs/%.mml
	java -jar $(MMLGB) $< $@ $(MMLBANK)
