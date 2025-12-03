#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define INPUT_FILE_PATH "input.txt"

FILE *file = NULL;

void close_file() {
    if (file != NULL) {
        fclose(file);
        file = NULL;
    }
}

u_int64_t powull(u_int64_t base, u_int32_t exponent) {
    u_int64_t result = 1;
    while (exponent > 0) {
        result *= base;
        exponent--;
    }
    return result;
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
    u_int64_t total, start, stop;
    u_int64_t d, num_digits;
    u_int64_t base, r, last_invalid;
    total = 0;
    start = 0;
    while ((token = strsep(&line, "-,")) != NULL) {
        if (start == 0) {
            start = strtoull(token, &endptr, 10);
            continue;
        }
        stop = strtoull(token, &endptr, 10);

        for (u_int64_t i = start; i <= stop; i++) {
            d = 10ULL;
            num_digits = 1ULL;
            while (d < i) {
                d *= 10ULL;
                num_digits++;
            }

            // reconstruct from pattern
            base = 1ULL;
            for (int j = 1; j < num_digits; j++) {
                d /= 10ULL;
                base *= 10ULL;
                if (num_digits % j != 0) continue;
                r = i / d;
                for (u_int32_t exp = 1; exp < (num_digits / j); exp++) {
                    r += (i / d) * powull(base, exp);
                }
                if (r == i && r != last_invalid) {
                    total += r;
                    last_invalid = r;
                }
            }
        }

        start = 0;
    }

    printf("%llu\n", total);
}
