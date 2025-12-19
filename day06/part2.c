#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INPUT_FILE_PATH "input.txt"
#define NUM_OPERATIONS 1000
#define NUM_OPERANDS 4

u_int64_t operands[NUM_OPERATIONS][NUM_OPERANDS];
char operators[NUM_OPERATIONS];
char *rows[NUM_OPERANDS];

int main() {
    FILE *file = fopen(INPUT_FILE_PATH, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char *line = NULL;
    size_t linecap = 0;
    ssize_t line_length;
    for (int i = 0; i < NUM_OPERANDS; i++) {
        line_length = getline(&line, &linecap, file);
    
        // copy string
        rows[i] = line;

        // reset before next getline
        line = NULL;
        linecap = 0;
    }

    int operation_num = NUM_OPERATIONS, operand_num = 0;
    int num, offset;
    char operator;

    getline(&line, &linecap, file);
    while (sscanf(line, "%c %n", &operator, &offset) == 1) {
        operators[--operation_num] = operator;
        line += offset;
    }

    char operand[NUM_OPERANDS + 1];
    operand[NUM_OPERANDS] = '\0';
    int col = (int) line_length - 1;
    operation_num = 0;
    while (col > 0) {
        col--;

        for (int row = 0; row < NUM_OPERANDS; row++) {
            operand[row] = rows[row][col];
        }

        if (sscanf(operand, "%d", &num) == 1) {
            operands[operation_num][operand_num] = (u_int64_t) num;
            operand_num++;
            continue;
        }
        operand_num = 0;
        operation_num++;
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
                if (!operands[i][j]) continue;
                result *= operands[i][j];
            }
        }
        total += result;
    }
    printf("%llu\n", total);
}
