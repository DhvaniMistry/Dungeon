# COM S 3270 – Spring 2025

## Programming Project 1.06: Fog of War and Porting to C++

### Overview

This project transitions the dungeon game from C to **C++**, introducing **object-oriented design**, a **“fog of war” visibility system**, and **developer debug commands**. Gameplay is further enhanced with limited player vision and support for teleportation and fog toggle.

---

### Major Updates

* ✅ Full conversion from C to **C++**
* ✅ Inheritance: `PC` and `NPC` inherit from `Character`
* ✅ **Fog of war**: Player can only see terrain within a light radius of 3
* ✅ **Remembered terrain**: Seen terrain is cached even after leaving view
* ✅ Rendering system updated to display only visible monsters and known terrain

---

### Gameplay Controls

| Key(s)            | Action                                    |
| ----------------- | ----------------------------------------- |
| `y`, `7`          | Move up-left                              |
| `k`, `8`          | Move up                                   |
| `u`, `9`          | Move up-right                             |
| `l`, `6`          | Move right                                |
| `n`, `3`          | Move down-right                           |
| `j`, `2`          | Move down                                 |
| `b`, `1`          | Move down-left                            |
| `h`, `4`          | Move left                                 |
| `.`, `5`, `space` | Rest for a turn                           |
| `<`               | Go up stairs (if on up staircase)         |
| `>`               | Go down stairs (if on down staircase)     |
| `m`               | Display monster list                      |
| `↑ / ↓`           | Scroll monster list                       |
| `Esc`             | Exit monster list                         |
| `Q`               | Quit game                                 |
| `f`               | Toggle fog of war (developer command)     |
| `g`               | Enter teleport targeting mode (developer) |
| `g` (again)       | Teleport to targeted location             |
| `g r`             | Random teleport                           |

---

### C++ Specific Features

* Class hierarchy:

  * `Character` (base class)

    * `PC`
    * `NPC`
* Clean encapsulation: use access specifiers where it makes sense.
* Modular design for dungeon, rendering, visibility, and control handling.
* All newly added code must be in C++ (`.cpp`).

---

### Fog of War

* PC has **light radius = 3**, making a **5x5 visible region**.
* Terrain is remembered once seen.
* Monsters are **only visible** when within light radius.
* Remembered terrain does **not update** if PC doesn’t witness changes.

---

### Teleportation Mode

* Triggered with `g`.
* Use movement keys to move a `*` targeting cursor.
* Press `g` again to teleport, or `r` to teleport randomly.
* You **can teleport into rock**, but not immutable rock.

---

### Building the Project

Make sure to:

1. Rename all source files from `.c` → `.cpp`
2. Run `make`
3. Update the `Makefile`:

   * Use `g++` for linking instead of `gcc`

Example:

```bash
make
```

---

### Run the Game

```bash
./rlg327 [--nummon <count>]
```

* `--nummon <count>`: Number of monsters to spawn (optional)
---

### Dependencies

* C++17 compatible compiler (`g++`)
* `ncurses` library

---

### Author

**Dhvani Mistry**
COM S 3270 – Spring 2025
Iowa State University
