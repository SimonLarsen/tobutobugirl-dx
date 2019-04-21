@echo off
PATH=%PATH%;%~dp0\gbdk\bin

java.exe -jar parser\MMLGB.jar music\%1 driver\song.asm
if %errorlevel% neq 0 (
	pause
	exit /b %errorlevel%
)

lcc.exe driver\player.asm driver\music.asm driver\song.asm driver\freq.asm driver\noisefreq.asm driver\vib.asm -o rom.gb

if %errorlevel% neq 0 (
	echo Error compiling rom!
	pause
	exit /b %errorlevel%
)

echo %~nx0 compiled to rom.gb successfully!
pause
