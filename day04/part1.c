#include <stdio.h>
#include <stdlib.h>

#define INPUT_FILE_PATH "input.txt"
#define MAP_WIDTH 137

int map[MAP_WIDTH][MAP_WIDTH];

int check_shelves(int row, int col) {
    int paper = 0;
    if (row > 0 && col > 0 && map[row-1][col-1]) paper++;
    if (row > 0 && map[row-1][col]) paper++;
    if (row > 0 && col < MAP_WIDTH-1 && map[row-1][col+1]) paper++;
    if (col > 0 && map[row][col-1]) paper++;
    if (col < MAP_WIDTH-1 && map[row][col+1]) paper++;
    if (row < MAP_WIDTH-1 && col > 0 && map[row+1][col-1]) paper++;
    if (row < MAP_WIDTH-1 && map[row+1][col]) paper++;
    if (row < MAP_WIDTH-1 && col < MAP_WIDTH-1 && map[row+1][col+1]) paper++;
    return paper;
}

void load_file() {
    FILE *file = fopen(INPUT_FILE_PATH, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char *line = NULL;
    size_t linecap = 0;
    ssize_t line_length;
    int row = 0;
    while ((line_length = getline(&line, &linecap, file)) > 0) {
        for (int col = 0; col < line_length; col++) {
            if (line[col] == '@') {
                map[row][col] = 1;
            }
        }
        row++;
    }

    fclose(file);
    file = NULL;
}

int main() {
    load_file();

    int accessible = 0;
    for (int i = 0; i < MAP_WIDTH; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            if (map[i][j] == 1) {
                if (check_shelves(i, j) < 4) {
                    accessible++;
                }
            }
        }
    }

    printf("%d\n", accessible);
}
