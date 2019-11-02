# Tobu Tobu Girl Deluxe ![MIT License](https://img.shields.io/badge/license-MIT%20License-blue.svg) ![CC BY 4.0](https://img.shields.io/badge/license-CC%20BY%204.0-blue.svg) ![Game Boy](https://img.shields.io/badge/platform-Game%20Boy-blue.svg)

A dual GB/GBC remaster of Tobu Tobu Girl.

More info at: http://tangramgames.dk/tobutobugirldx.

## Playing the game

In order to play the game you will need to either flash the game to a Game Boy flash cart or use a Game Boy emulator. The binaries are provided through [itch.io](https://tangramgames.itch.io/tobutobugirldx).

## Compilation

The build chain is currently a huge mess involving an old version of GBDK/LCC, a more recent version of SDCC and some custom tools. I plan on cleaning things up at some point. In the meantime, if you still want try and compile this you should install the following tools and then change the paths in the Makefile accordingly:

* [GBDK 2.96](http://gbdk.sourceforge.net)
* [GBDK-n](https://github.com/andreasjhkarlsson/gbdk-n)
* [SDCC 3.8.0](http://sdcc.sourceforge.net)
* [pyimgtogb](https://github.com/SimonLarsen/pyimgtogb)
* [mmlgb](https://github.com/SimonLarsen/mmlgb)

## License

The source code for Tobu Tobu Girl is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

All assets (images, text, sound and music) are licensed under the [Creative Commons Attribution 4.0 International License](http://creativecommons.org/licenses/by/4.0/).
