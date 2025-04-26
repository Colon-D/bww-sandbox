Proof of concept mod for Balan Wonderworld.
- Enables infinite costume uses:
  - costume uses never decreases
  - costume uses only increases from 0 to 1
- Enables a buggy Force Jump key on "Space Bar", "Caps Lock" and "Right Click":
  - 🐞 Bugs:
    - Only full jumps
    - Can jump when you shouldn't be able too:
      - In Cutscenes
      - Using certain abilities (small list of examples):
        - Dusk Butterfly: Can jump forever by spamming force jump.
        - Pounding Pig: Can jump forever by force jumping as soon as ground pounding.
        - Jumping Jack: Can double jump by jumping whilst fluttering.
- 🆕 Volume multipliers:
  - Can now 100% mute Music, Sound Effects, and/or Voices
  - Subcategories of Music, Sound Effects, and Voices can be changed too

Installation Instructions
---
Unzip [UE4SS](https://github.com/UE4SS-RE/RE-UE4SS/releases/download/experimental-latest/UE4SS_v3.0.1-394-g437a8ff.zip) into `BALAN WONDERWORLD/Happiness/Binaries/Win64/`.
Unzip [Sandbox](https://github.com/Colon-D/bww-sandbox/releases/latest/download/Sandbox.zip) into `ue4ss/Mods/`.
Ingame, manually rebind "Jump/Costume Ability" from "Space Bar":
- Settings -> Controls -> Keyboard Configuration

Configuration Instructions
---
Force Jump can be configured to different keys in `Sandbox\config.lua`.
Infinite costume uses can be disabled in `Sandbox\config.lua`.
- Set `infinite_costumes` to `false`.
If you want Gamepad binds, use Steam Input's Controller Configurator to bind a Gamepad button to a bound Keyboard key, like "Space Bar".
- Note this will affect other uses of that button, like the UI.
Volume Multipliers can be set between [0.0, 1.0].
- Volume Multipliers multiply together. Setting ingame Music to 5 (50% volume), and the config's Music to 0.5 (a further 50% of that volume) will result in 25% volume total.

---

Compilation Instructions
---
- Run `git submodule update --init --recursive`
- Follow:
  - https://docs.ue4ss.com/dev/guides/creating-a-c++-mod.html:
    - Part 1 (build requirements, 1, 2, 3)
    - Part 2 (3A) or (3B, 4, 5, 6 (Sandbox))
  - https://docs.ue4ss.com/dev/guides/installing-a-c++-mod.html:
    - 1
    - 2 (`main.dll` in `.\Binaries\<Configuration>\Sandbox`)
    - 3 (`Mods\Sandbox\dlls\main.dll`). Alternatively, a Symbolic Link negates the need to copy after every build.
- Copy the contents of `.\Lua\` to `Mods\Sandbox\`, or create Symbolic Links.
- Create an empty file named `enabled.txt` in `Mods\Sandbox\`.
