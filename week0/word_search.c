#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SIZE 20

// Directions: diagonal up, rightward, diagonal down, downward
const int directions[4][2] = {{-1, 1}, {0, 1}, {1, 1}, {1, 0}};

void fill_random_letters(char puzzle[MAX_SIZE][MAX_SIZE], int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (puzzle[i][j] == '.') {
                puzzle[i][j] = 'A' + rand() % 26;
            }
        }
    }
}

int can_place_word(char puzzle[MAX_SIZE][MAX_SIZE], int size, const char *word, int row, int col, int dir) {
    int len = strlen(word);
    int dr = directions[dir][0];
    int dc = directions[dir][1];

    for (int i = 0; i < len; i++) {
        int new_row = row + i * dr;
        int new_col = col + i * dc;

        if (new_row < 0 || new_row >= size || new_col < 0 || new_col >= size) {
            return 0;
        }
        if (puzzle[new_row][new_col] != '.' && puzzle[new_row][new_col] != word[i]) {
            return 0;
        }
    }

    return 1;
}

void place_word(char puzzle[MAX_SIZE][MAX_SIZE], const char *word, int row, int col, int dir) {
    int len = strlen(word);
    int dr = directions[dir][0];
    int dc = directions[dir][1];

    for (int i = 0; i < len; i++) {
        int new_row = row + i * dr;
        int new_col = col + i * dc;
        puzzle[new_row][new_col] = word[i];
    }
}

int insert_word(char puzzle[MAX_SIZE][MAX_SIZE], int size, const char *word) {
    int attempts = 100;
    int len = strlen(word);

    while (attempts--) {
        int row = rand() % size;
        int col = rand() % size;
        int dir = rand() % 4;

        if (can_place_word(puzzle, size, word, row, col, dir)) {
            place_word(puzzle, word, row, col, dir);
            return 1;
        }
    }

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <size> <word1> <word2> ...\n", argv[0]);
        return 1;
    }

    int size = atoi(argv[1]);
    if (size < 1 || size > MAX_SIZE) {
        printf("Size must be between 1 and %d.\n", MAX_SIZE);
        return 1;
    }

    char puzzle[MAX_SIZE][MAX_SIZE];

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            puzzle[i][j] = '.';
        }
    }

    srand(time(NULL));

    for (int i = 2; i < argc; i++) {
        if (!insert_word(puzzle, size, argv[i])) {
            printf("Failed to place word: %s\n", argv[i]);
            return 1;
        }
    }

    fill_random_letters(puzzle, size);

    printf("Generated Word Search:\n");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%c ", puzzle[i][j]);
        }
        printf("\n");
    }
    printf("\nWord List:\n");
    for (int i = 2; i < argc; i++) {
        printf("%s\n", argv[i]);
    }

    return 0;
}
