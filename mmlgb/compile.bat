@echo off
PATH=%PATH%;%~dp0\gbdk\bin

java.exe -jar parser\MMLGB.jar music\%1 driver\song.asm
if %errorlevel% neq 0 (
	pause
	exit /b %errorlevel%
)

lcc.exe -c driver\song.asm -o driver\song.o
lcc.exe driver\player.o driver\music.o driver\song.o driver\freq.o driver\noisefreq.o driver\vib.o -o rom.gb

if %errorlevel% neq 0 (
	echo Error compiling rom!
	pause
	exit /b %errorlevel%
)

echo %~nx0 compiled to rom.gb successfully!
pause
