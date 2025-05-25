Author: Dhvani Mistry

# Dungeon Generation

**Course**: COM S 3270 – Spring 2025

**Project**: Programming Project 1.01


## Overview

This project is the first step in developing a Roguelike game. It implements a dungeon generator in **C**, producing a fully connected, playable, and visually appealing dungeon that adheres to realism and game design principles. The generated dungeon is printed to standard output using a symbolic tile-based map.

## Features

* Fixed-size dungeon: **80 columns × 21 rows**
* **At least 6 non-overlapping rooms**, each at least **4×3**
* Rooms separated by at least **1 cell of rock**
* Fully connected dungeon using **corridors (`#`)**
* Realistic corridor generation with optional bends and twists
* **At least one upward (`<`) and one downward (`>`) staircase**, placed on floor tiles
* Floor tiles (`.`) and corridor tiles (`#`) are clearly distinguished
* Outermost border cells are immutable rock (`' '`)
* Uses **static allocation** for dungeon data
* Structures and code designed with future extensibility in mind (e.g., hardness field)

## Compilation & Execution

```bash
make
./dungeon_generator
```

## Code Organization

* `dungeon_generator.c`: Main source file implementing dungeon generation logic
* `room_t` and `cell_t` structures used for clarity and extensibility
* Global `dungeon[21][80]` matrix represents the dungeon
* Room placement and corridor carving functions are modular

## Assumptions

* No user input is required; dungeon generation is fully randomized
* All rooms fit within the constraints due to placement checks
* Future assignments will add gameplay features like characters, items, and visibility

