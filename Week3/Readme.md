
# COM S 3270 – Spring 2025

## Programming Project 1.03: Path Finding

### Description

This project builds on the dungeon infrastructure developed in previous assignments by implementing **pathfinding algorithms** for dungeon navigation. Specifically, it calculates distance maps from the Player Character (PC) to every accessible dungeon cell using **Dijkstra’s Algorithm**, supporting two types of monsters:

* **Non-tunneling monsters**: Can only travel through open space (floor and stairs).
* **Tunneling monsters**: Can move through walls, with path costs based on cell hardness.

The calculated distance maps serve as terrain gradients that monsters will use to move "downhill" toward the PC.

---

### Features

* **Dijkstra's Algorithm** with 8-way connectivity (including diagonals).
* Two distinct distance maps:

  * **Non-tunneling**: Only passable through open floor tiles (walls are impassable).
  * **Tunneling**: Uses weights based on hardness; walls with hardness 255 are impassable.
* **Optimized Computation**: Distance is computed once from the PC outward rather than per-monster.
* **ASCII Visualization**:

  * Standard dungeon view.
  * Non-tunneling distance map (mod 10 representation).
  * Tunneling distance map (mod 10 representation).

---

### Movement Costs

* **Non-tunneling**:

  * Floor and stairs = cost 1.
  * Walls = ignored (not in queue).
* **Tunneling**:

  * Hardness = 0 → cost 1.
  * Hardness ∈ \[1, 254] → cost = `1 + (hardness / 85)`.
  * Hardness = 255 → impassable (not in queue).

---

### Visualization Example

Each distance map shows only the **last digit** of the computed shortest-path distance (`distance % 10`), with the PC marked as `'@'`. These maps visualize gradients for monster pathfinding.

Example snippet:

```
321098765432109876543
3210 32
665432109876543211 211
...
09888 21@12
```

---

### Compilation

```bash
make
./dungeon_generator
```

Note: Make sure to link all relevant source files, including any custom heap/priority queue code.

---

### Usage

Upon execution, the program will:

1. Generate a dungeon.
2. Randomly place the PC on a floor tile.
3. Compute and display:

   * Standard dungeon view.
   * Distance map for non-tunneling monsters.
   * Distance map for tunneling monsters.

---

### Algorithm Notes

* Priority queue implementation must support `decrease-key` operations.
* The implementation avoids recomputing distances per monster by solving from the PC outward (reverse Dijkstra).
* Monster can follow distance gradients for optimal pathing.



