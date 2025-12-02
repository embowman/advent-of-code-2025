#include <stdio.h>
#include <stdlib.h>

#define INPUT_FILE_PATH "rotations.txt"

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
        perror("Error opening file\n");
        exit(EXIT_FAILURE);
    }

    char *line = NULL;
    size_t linecap = 0;
    int num;
    int position = 50;
    int password = 0;
    while (getline(&line, &linecap, file) > 0) {
        sscanf(line + 1, "%d", &num);

        if (line[0] == 'L') {
            position -= num;
        } else if (line[0] == 'R') {
            position += num;
        }

        if (position % 100 == 0) {
            password++;
        }
    }
    printf("%d\n", password);
}
