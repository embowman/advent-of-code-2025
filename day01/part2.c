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
    int pwd_change;
    while (getline(&line, &linecap, file) > 0) {
        sscanf(line + 1, "%d", &num);

        password += num / 100;
        num %= 100;
        pwd_change = 0;
        
        if (line[0] == 'L') {
            num *= -1;

            // start from zero
            // cancel crossover
            if (position == 0) {
                pwd_change--;
            }
        }

        position += num;

        // crossover
        if (position > 99) {
            position -= 100;
            pwd_change++;
        } else if (position < 0) {
            position += 100;
            pwd_change++;
        }

        // end at zero
        if (position == 0) {
            pwd_change = 1;
        }

        password += pwd_change;
    }
    printf("%d\n", password);
}
