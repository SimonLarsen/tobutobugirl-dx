#ifndef WINSCREEN_H
#define WINSCREEN_H

void initWinscreen();
void winscreenWait(UBYTE steps);
void winscreenScrollCircles();
void drawScore(UBYTE x, UBYTE y, UBYTE value);
void winscreenPlayNote(UBYTE note, UBYTE octave);
void winscreenJingle();
void winscreenTextJingle();
void winscreenShowRank();
void countUpScore(UBYTE x, UBYTE y, UBYTE value);
void drawTime16(UBYTE x, UBYTE y, UWORD secs);
void showWinscreenNormal();
void showWinscreenInfinite();
void enterWinscreen();

#endif
