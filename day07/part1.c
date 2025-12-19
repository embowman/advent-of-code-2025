#include <stdio.h>
#include <stdlib.h>

#define INPUT_FILE_PATH "input.txt"
#define MAP_SIZE 142

int map[MAP_SIZE][MAP_SIZE];

int count_beam_split(int row, int col) {
    if (
        row >= MAP_SIZE ||
        col >= MAP_SIZE ||
        col < 0 ||
        map[row][col] == 'X'
    ) {
        return 0;
    }

    row++;
    if (map[row][col] == '^') {
        map[row][col] = 'X';
        int left = count_beam_split(row, col - 1);
        int right = count_beam_split(row, col + 1);
        return 1 + left + right;
    } else {
        return count_beam_split(row, col);
    }
}

int main() {
    FILE *file = fopen(INPUT_FILE_PATH, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char *line = NULL;
    size_t linecap = 0;
    int row = 0;
    while (getline(&line, &linecap, file) > 0) {
        for (int col = 0; col < MAP_SIZE; col++) {
            map[row][col] = line[col];
        }
        row++;
    }

    int init_col;
    for (int i = 0; i < MAP_SIZE; i++) {
        if (map[0][i] == 'S') {
            init_col = i;
        }
    }

    printf("%d\n", count_beam_split(0, init_col));
}
