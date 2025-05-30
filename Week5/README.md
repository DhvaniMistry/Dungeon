# Dungeon Game with Ncurses

# COM S 3270 – Spring 2025

## Programming Project 1.05: User Interface with Ncurses

### Overview

This project upgrades the dungeon crawler game with a full terminal-based user interface using the `ncurses` library. The player character (PC), represented by `@`, is now manually controlled through keyboard input. The game includes stair functionality for moving between levels, dynamic dungeon generation, and a real-time monster list interface.

### Features

* Manual movement of the PC using `vi`-style keys and arrows.
* Unbuffered keyboard input with `ncurses`.
* Dynamic dungeon generation when stairs are used (`<`, `>`).
* Monster list view with relative positions.
* Clean terminal interface with message and status lines.
* Game ends when the PC dies or all NPCs are defeated.

### Controls

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
| `<`               | Ascend staircase (if on up stair)         |
| `>`               | Descend staircase (if on down stair)      |
| `m`               | Display monster list                      |
| `↑ / ↓`           | Scroll monster list (if list is too long) |
| `Esc`             | Exit monster list view                    |
| `Q`               | Quit game                                 |

### Build Instructions

Ensure that you have the `ncurses` library installed. Then compile using:

```bash
make
```

### Running the Game

```bash
./rlg327
```

* `--nummon <count>`: Optional argument to specify the number of monsters (default: 10).

### Notes

* Dungeon map is drawn on lines 1–21 of the terminal.
* Line 0 is reserved for messages (e.g., "There’s a wall in the way!").
* Lines 22–23 are reserved for future status displays.
* PC and NPCs move in turns, controlled via a discrete event simulator using a priority queue.
* Stairs can generate persistent or non-persistent levels, as designed.

### Dependencies

* C Standard Library
* `ncurses` (`libncurses-dev` on most systems)

### Known Limitations

* PC cannot tunnel through walls anymore.
* Monsters do not use stairs.
* Levels may not persist unless implemented.



| Key(s)    | Action                                        |
|-----------|-----------------------------------------------|
| `7` / `y` | Move up-left                                  |
| `8` / `k` | Move up                                       |
| `9` / `u` | Move up-right                                 |
| `6` / `l` | Move right                                    |
| `3` / `n` | Move down-right                               |
| `2` / `j` | Move down                                     |
| `1` / `b` | Move down-left                                |
| `4` / `h` | Move left                                     |
| `>`       | Go down stairs (if on down staircase)         |
| `<`       | Go up stairs (if on up staircase)             |
| `5` / `Space` / `.` | Rest for a turn                     |
| `m`       | Display a list of monsters                    |
| `Q`       | Quit the game                                 |



This assignment was too confusing for me because i initially forgot to add the LDFLAGS = -lncurses to makefile.