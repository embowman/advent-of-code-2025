#include <stdio.h>
#include <stdlib.h>

#define INPUT_FILE_PATH "input.txt"

FILE *file = NULL;

void close_file() {
    if (file != NULL) {
        fclose(file);
        file = NULL;
    }
}

int main() {
    atexit(close_file);

    file = fopen(INPUT_FILE_PATH, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char *line = NULL;
    size_t linecap = 0;
    ssize_t line_length;
    int curr_num, biggest_num;
    int total = 0;
    while ((line_length = getline(&line, &linecap, file)) > 0) {
        biggest_num = 0;
        for (int i = 0; i < line_length - 2; i++) {
            for (int j = i + 1; j < line_length - 1; j++) {
                curr_num = ((line[i] - '0') * 10) + (line[j] - '0');
                if (curr_num > biggest_num) {
                    biggest_num = curr_num;
                }
            }
        }
        total += biggest_num;
    }
    printf("%d\n", total);
}
