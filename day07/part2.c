#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INPUT_FILE_PATH "input.txt"
#define MAP_SIZE 142
#define HASH_MAP_SIZE 262144

typedef struct {
    int row;
    int col;
    bool occupied;
} HashMapKey;

typedef struct {
    HashMapKey key;
    u_int64_t count;
} HashMapEntry;

int map[MAP_SIZE][MAP_SIZE];

#pragma region MEMO
//////////////////////////////////////////////////////////////////////////////

HashMapEntry hash_map[HASH_MAP_SIZE];

u_int32_t hash(int row, int col) {
    u_int32_t combined = (row * 1000) + col;
    return combined % HASH_MAP_SIZE;
}

void hash_map_store(int row, int col, u_int64_t count) {
    unsigned int index = hash(row, col);
    unsigned int original_index = index;
    int i = 0;

    while (hash_map[index].key.occupied) {
        if (++i > HASH_MAP_SIZE) {
            perror("Hash map full\n");
            exit(EXIT_FAILURE);
        }
        index = (original_index + i * i) % HASH_MAP_SIZE;
    }

    hash_map[index].count = count;
    hash_map[index].key.row = row;
    hash_map[index].key.col = col;
    hash_map[index].key.occupied = true;
}

bool hash_map_lookup(int row, int col, u_int64_t *count) {
    unsigned int index = hash(row, col);
    unsigned int original_index = index;
    int i = 0;

    while (hash_map[index].key.occupied) {
        if (
            hash_map[index].key.row == row &&
            hash_map[index].key.col == col
        ) {
            *count = hash_map[index].count;
            return true;
        }

        if (++i > HASH_MAP_SIZE) {
            return false;
        }
        index = (original_index + i * i) % HASH_MAP_SIZE;
    }

    return false;
}

#pragma endregion

#pragma region RECURSIVE COUNT
//////////////////////////////////////////////////////////////////////////////

u_int64_t count_beam_split(int row, int col) {
    if (
        row >= MAP_SIZE ||
        col >= MAP_SIZE ||
        col < 0
    ) {
        return 0;
    }

    u_int64_t count;
    if (hash_map_lookup(row, col, &count)) {
        return count;
    }

    count = 0;
    row++;
    if (map[row][col] == '^') {
        count = 1ULL;
        count += count_beam_split(row, col - 1);
        count += count_beam_split(row, col + 1);
    } else {
        count = count_beam_split(row, col);
    }

    hash_map_store(row, col, count);
    return count;
}

#pragma endregion

#pragma region MAIN
//////////////////////////////////////////////////////////////////////////////

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

    fclose(file);
    file = NULL;

    int init_col;
    for (int i = 0; i < MAP_SIZE; i++) {
        if (map[0][i] == 'S') {
            init_col = i;
        }
    }

    printf("%llu\n", 1ULL + count_beam_split(0, init_col));
}

#pragma endregion
