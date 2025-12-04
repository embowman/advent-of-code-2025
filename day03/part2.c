#include <stdio.h>
#include <stdlib.h>

#define INPUT_FILE_PATH "input.txt"

FILE *file = NULL;
char *line = NULL;
char biggest_num[] = "000000000000";

void close_file() {
    if (file != NULL) {
        fclose(file);
        file = NULL;
    }
}

ssize_t get_big_digit(ssize_t start, ssize_t stop, int index) {
    char biggest_char = '0';
    ssize_t new_start;
    for (ssize_t i = start; i < stop; i++) {
        if (line[i] > biggest_char) {
            biggest_char = line[i];
            new_start = i + 1;
        }
    }
    biggest_num[index] = biggest_char;
    return new_start;
}

int main() {
    atexit(close_file);

    file = fopen(INPUT_FILE_PATH, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    size_t linecap = 0;
    ssize_t line_length, start, stop;
    int index;
    char *endptr;
    u_int64_t total = 0;
    while ((line_length = getline(&line, &linecap, file)) > 0) {
        start = 0;
        stop = line_length - 12L;
        index = 0;
        while (index < 12) {
            start = get_big_digit(start, stop, index);
            stop++;
            index++;
        }
        total += strtoull(biggest_num, &endptr, 10);
    }
    printf("%llu\n", total);
}
