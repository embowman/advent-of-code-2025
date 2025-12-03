#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <math.h>

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
    getline(&line, &linecap, file);

    char *token, *endptr;
    u_int64_t total, start, stop, d, num_digits, left, right;
    total = 0;
    start = 0;
    while ((token = strsep(&line, "-,")) != NULL) {
        if (start == 0) {
            start = strtoull(token, &endptr, 10);
            continue;
        }
        stop = strtoull(token, &endptr, 10);

        for (u_int64_t i = start; i < stop; i++) {
            d = 10ULL;
            num_digits = 1ULL;
            while (d < i) {
                d *= 10ULL;
                num_digits++;
            }

            if (num_digits % 2 != 0) {
                d /= 10ULL;
            }
            d = (u_int64_t) sqrt(d);
            left = i / d;
            right = i - (left * d);
            if (left == right) {
                total += i;
            }
        }

        start = 0;
    }

    printf("%llu\n", total);
}
