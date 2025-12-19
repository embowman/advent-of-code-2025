#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

#define INPUT_FILE_PATH "input.txt"

typedef struct _Node {
    u_int64_t left_cap, right_cap;
    struct _Node *prev, *next;
} Node;

Node *head = NULL;

Node* create_node(u_int64_t left_cap, u_int64_t right_cap) {
    Node *node = (Node*) malloc(sizeof(Node));
    if (node == NULL) {
        perror("Error allocating memory for node");
        exit(EXIT_FAILURE);
    }

    node->left_cap = left_cap;
    node->right_cap = right_cap;
    node->prev = NULL;
    node->next = NULL;

    return node;
}

void stitch_nodes(Node *left, Node *right) {
    Node *tmp = left->next;
    left->next = right;
    right->prev = left;
    while (tmp != right) {
        left = tmp;
        tmp = tmp->next;
        free(left);
    }
}

void exclude_fresh_range(u_int64_t left_cap, u_int64_t right_cap) {
    Node *left_node = NULL, *right_node = NULL;

    left_node = head;
    while (left_cap > left_node->right_cap) {
        left_node = left_node->next;
    }
    right_node = left_node;
    while (right_cap > right_node->right_cap) {
        right_node = right_node->next;
    }

    // same node
    if (left_node == right_node) {
        if (right_cap <= right_node->left_cap) {
            return;
        }

        if (left_cap < left_node->left_cap && right_cap <= right_node->right_cap) {
            right_node->left_cap = right_cap;
            return;
        }

        right_node = right_node->next;
        left_node->next = create_node(right_cap, left_node->right_cap);
        left_node->next->prev = left_node;
        if (right_node != NULL) {
            left_node->next->next = right_node;
            right_node->prev = left_node->next;
        }
        left_node->right_cap = left_cap;
        return;
    }

    // neighbor nodes
    if (left_node->next == right_node) {
        if (left_cap < left_node->left_cap) {
            stitch_nodes(left_node->prev, right_node);
        }
        else {
            left_node->right_cap = left_cap;
        }

        if (right_cap <= right_node->left_cap) {
            return;
        }

        right_node->left_cap = right_cap;
        return;
    }

    // separate nodes

    if (left_cap < left_node->left_cap) {
        stitch_nodes(left_node->prev, right_node);
    }
    else {
        left_node->right_cap = left_cap;
        stitch_nodes(left_node, right_node);
    }

    if (right_cap <= right_node->left_cap) {
        return;
    }

    right_node->left_cap = right_cap;
}

bool check_freshness(u_int64_t ingredient) {
    Node *curr_node = head;
    while (ingredient > curr_node->right_cap) {
        curr_node = curr_node->next;
    }
    return ingredient < curr_node->left_cap;
}

int main() {
    FILE *file = fopen(INPUT_FILE_PATH, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    head = create_node(0, UINT64_MAX);

    char *line = NULL, *token = NULL, *endptr = NULL;
    size_t linecap = 0;
    u_int64_t num_fresh_ingredients, left_cap, right_cap;
    num_fresh_ingredients = 0;
    left_cap = 0;
    while (getline(&line, &linecap, file) > 0) {
        while ((token = strsep(&line, "-")) != NULL) {
            if (left_cap == 0) {
                left_cap = strtoull(token, &endptr, 10);
                continue;
            }
            right_cap = strtoull(token, &endptr, 10);
        }

        if (right_cap != 0) {
            exclude_fresh_range(left_cap-1ULL, right_cap+1ULL);
        }
        else if (check_freshness(left_cap)) {
            num_fresh_ingredients++;
        }
        left_cap = 0;
        right_cap = 0;
    }
    printf("%llu\n", num_fresh_ingredients);
}
