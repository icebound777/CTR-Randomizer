# CTR-Randomizer

This is the source code for a randomizer base-mod for the videogame Crash Team Racing, originally released for the Sony Playstation in 1999.

This mod requires the NTSC-U version of CTR.

The resulting ROM is a base for randomization, but no randomization is done by this mod itself.

## Playing the randomizer

If you've found yourself here just wanting to play the randomizer, then please head over to the repository for the [CTR-Randomizer Standalone](https://github.com/icebound777/CTR-Randomizer-Standalone) tool. There you can grab a release of that tool, which will patch and randomize the game for you.
This repository is only for the source code of the base-mod for that randomizer.

## Requirements

This mod is to be used with the [CTR-ModSDK](https://github.com/CTR-tools/ctr-modsdk), and its foundation the [PSX Modding Toolchain](https://github.com/mateusfavarin/psx-modding-toolchain).
You will also need a ROM of the game's NTSC-U version in `.bin` format.

## Setup

Please follow the respective setup guides of PSX Modding Toolchain and CTR-ModSDK, then clone this repository into the `psx-modding-toolchain/games/CTR-ModSDK/mods/MyMods/` directory (create that directory first if it does not exist).
Then follow the setup.md file within the `/docs` directory.
Place the NTSC-U ROM of CTR in `.bin` format into the `psx-modding-toolchain/games/CTR-ModSDK/build/` directory, then rename that file to `ctr-u.bin`.
Finally, launch `build.bat` to compile the mod itself ("1 then 1"), then patch the original ROM ("4 then 1").
This should result in `ctr-u_CTR-Randomizer.bin` getting placed into the `psx-modding-toolchain/games/CTR-ModSDK/build/` directory, ready to be randomized.
