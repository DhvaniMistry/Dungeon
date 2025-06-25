

# COM S 3270 – Spring 2025

## Programming Project 1.04: Player Character and Monsters

### Author

Dhvani Mistry

---

### Description

This project extends the dungeon and pathfinding engine by introducing **game entities**: the **Player Character (PC)** and **Non-Player Characters (NPCs)**, referred to as **monsters**. Each character has distinct movement logic, speed, and characteristics that affect how they interact with the dungeon and each other.

The system uses a **discrete event simulator** driven by a **priority queue**, where characters act according to their speed-based schedule. The game continues until the PC is the last surviving character (win) or is killed (lose).

---

### Features

* **Player Character (@)**:

  * Represented by `'@'`.
  * Randomly placed in a room at the start.
  * Moves randomly, along a fixed vector, or not at all (user-defined behavior).
  * May optionally bore through walls (immutable walls are impassable).

* **Monster Generation**:

  * Configurable with the `--nummon` command-line switch.
  * Default number of monsters is 10 if switch is omitted.
  * Randomly scattered throughout dungeon floor tiles.
  * Each monster assigned one of 16 possible combinations of 4 binary characteristics:

    * **Intelligence**
    * **Telepathy**
    * **Tunneling**
    * **Erratic behavior**
  * Displayed using corresponding hexadecimal character (0–9, a–f).

* **Monster Characteristics**:

  * **Intelligent**: Uses pathfinding and remembers last known PC position.
  * **Telepathic**: Always knows PC's position.
  * **Tunneling**: Can dig through rock (hardness decreases by 85 per move).
  * **Erratic**: 50% chance to move randomly, otherwise moves normally.

* **Speed System**:

  * PC speed = 10.
  * Monster speed ∈ \[5, 20].
  * Move frequency = `floor(1000 / speed)` game turns.
  * Characters are scheduled and updated using a priority queue.

* **Combat**:

  * When a character moves into another's cell, the defender is killed.
  * PC death ends the game in a loss; all monsters dead results in a win.

* **Game Loop**:

  * Priority queue advances turn-by-turn.
  * Dungeon is redrawn after each PC move.
  * Includes visual delay (`usleep(250000)`) for observation.

---

### Compilation

```bash
make
./rlg327
```

Adjust source file names based on your implementation.

---

### Technical Notes

* Immutable walls (hardness 255) are always impassable.
* Tunneling monsters convert rock to corridor once hardness reaches 0.
* All characters move **one cell at a time** per action.
* Distance maps are recomputed if monsters change the dungeon structure (e.g., by tunneling).


