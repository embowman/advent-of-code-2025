#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INPUT_FILE_PATH "input.txt"
#define NUM_OPERATIONS 1000
#define NUM_OPERANDS 4

u_int64_t operands[NUM_OPERATIONS][NUM_OPERANDS];
char operators[NUM_OPERATIONS];

int main() {
    FILE *file = fopen(INPUT_FILE_PATH, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char *line = NULL;
    size_t linecap = 0;
    ssize_t line_length;
    int operation_num = 0, operand_num = 0;
    int num, offset;
    char operator;
    while((line_length = getline(&line, &linecap, file)) > 0) {
        while (operand_num < NUM_OPERANDS && sscanf(line, "%d %n", &num, &offset) == 1) {
            operands[operation_num][operand_num] = (u_int64_t) num;
            line += offset;
            operation_num++;
        }
        while (operand_num == NUM_OPERANDS && sscanf(line, "%c %n", &operator, &offset) == 1) {
            operators[operation_num] = operator;
            line += offset;
            operation_num++;
        }
        operation_num = 0;
        operand_num++;
    }

    u_int64_t result, total = 0;
    for (int i = 0; i < NUM_OPERATIONS; i++) {
        if (operators[i] == '+') {
            result = 0;
            for (int j = 0; j < NUM_OPERANDS; j++) {
                result += operands[i][j];
            }
        } else {
            result = 1;
            for (int j = 0; j < NUM_OPERANDS; j++) {
                result *= operands[i][j];
            }
        }
        total += result;
    }
    printf("%llu\n", total);
}
