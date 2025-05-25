# Word Search Generator

**Course**: COM S 327

**Semester**: Spring 2024

**Project**: Programming Project 0

## Overview

This project is a **Word Search Generator** written in C. The program takes command-line input to generate a customizable, square-shaped word search puzzle. The first argument specifies the puzzle size (number of rows and columns), and the remaining arguments are words to be hidden within the puzzle.

## Features

* Accepts a puzzle size up to **20x20**
* Accepts any number of words to hide
* Random placement of words in the following directions:

  * Rightward
  * Downward
  * Diagonal downward
  * Diagonal upward
* Fills unused cells with random letters
* Prints the completed puzzle along with the list of words
* Gracefully terminates if a word cannot be placed

## Assumptions

* All input is well-formed:

  * The size is a positive integer less than or equal to 20
  * All words contain only alphabetic characters
  * Words are short enough to fit within the puzzle bounds
* No dynamic memory allocation is required

## How It Works

1. **Static Allocation**: A 20×20 character matrix is statically allocated.
2. **Input Parsing**: The first command-line argument determines the puzzle size; all following arguments are words to hide.
3. **Word Insertion**: Words are inserted into the matrix in random directions and locations, checking for conflicts.
4. **Error Handling**: If a word cannot be inserted in any valid position, the program exits with an error message naming the word.
5. **Puzzle Completion**: Remaining empty cells are filled with random alphabetic characters.
6. **Output**: The complete puzzle and the list of inserted words are printed to standard output.

## Example Usage

```sh
./wordsearch 10 apple banana orange
```

Generates a 10×10 word search puzzle with the words "apple", "banana", and "orange" randomly placed within it.

## Compilation

Use `gcc` or another standard C compiler:

```sh
gcc -o wordsearch wordsearch.c
```

## Notes

* The program emphasizes clarity, correctness, and use of basic C concepts introduced in the first two lectures.
* The puzzle generation is randomized, so running the same input multiple times will typically produce different results.

---
