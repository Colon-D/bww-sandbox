Proof of concept mod for Balan Wonderworld.
- Enables infinite costume uses:
  - costume uses never decreases
  - costume uses only increases from 0 to 1
- Enables a buggy Force Jump key on "Space Bar", "Caps Lock" and "Right Click":
  - ⚠️ Please manually rebind "Jump/Costume Ability" from "Space Bar":
    - Settings -> Controls -> Keyboard Configuration
  - Can be configured to different keys in `Sandbox\Scripts\main.lua`
  - If you want Gamepad binds, use Steam Input's Controller Configurator to bind a Gamepad button to a bound Keyboard key, like "Space Bar".
    Note this will affect other uses of that button, like the UI.
  - 🐞 Bugs:
    - Only full jumps
    - Can jump when you shouldn't be able too:
      - In Cutscenes
      - Using certain abilities (small list of examples):
        - Dusk Butterfly: Can jump forever by spamming force jump.
        - Pounding Pig: Can jump forever by force jumping as soon as ground pounding.
        - Jumping Jack: Can double jump by jumping whilst fluttering.

Installation Instructions
---
Unzip [UE4SS](https://github.com/UE4SS-RE/RE-UE4SS/releases/download/experimental-latest/UE4SS_v3.0.1-394-g437a8ff.zip) into `BALAN WONDERWORLD/Happiness/Binaries/Win64/`.
Unzip [Sandbox](https://github.com/Colon-D/bww-sandbox/releases/latest/download/Sandbox.zip) into `ue4ss/Mods/`.

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
    - 2 (`main.dll` in `Binaries\<Configuration>\Sandbox`)
    - 3 (`mods\Sandbox\dlls\main.dll`)
    - 4 (`Sandbox : 1`)
- Copy `main.lua` to `mods\Sandbox\Scripts\main.lua`
