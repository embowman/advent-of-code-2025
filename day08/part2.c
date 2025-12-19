#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define INPUT_FILE_PATH "input.txt"
#define NUM_MERGES 1000
#define NUM_POINTS 1000
#define MAX_EDGES NUM_POINTS * (NUM_POINTS - 1) / 2

typedef struct {
    double x, y, z;
} Point;

typedef struct {
    int u, v;
    double length;
} Edge;

Point points[NUM_POINTS];
Edge edges[MAX_EDGES];

#pragma region INPUT
//////////////////////////////////////////////////////////////////////////////

FILE *file = NULL;

void close_file() {
    if (file != NULL) {
        fclose(file);
        file = NULL;
    }
}

void load_file() {
    atexit(close_file);

    file = fopen(INPUT_FILE_PATH, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char *line = NULL, *token = NULL, *endptr = NULL;
    size_t linecap = 0;
    double x = 0, y = 0, z = 0;
    int point = 0;
    while (getline(&line, &linecap, file) > 0) {
        while ((token = strsep(&line, ",")) != NULL) {
            if (x == 0) {
                x = strtod(token, &endptr);
                continue;
            } else if (y == 0) {
                y = strtod(token, &endptr);
                continue;
            }
            z = strtod(token, &endptr);

            points[point].x = x;
            points[point].y = y;
            points[point].z = z;
        }
        x = 0;
        y = 0;
        z = 0;
        point++;
    }

    fclose(file);
    file = NULL;
}

#pragma endregion

#pragma region PRIORITY QUEUE
//////////////////////////////////////////////////////////////////////////////

int compare_edges(const void *a, const void *b) {
    Edge *edge_a = (Edge *) a;
    Edge *edge_b = (Edge *) b;
    if (edge_a->length > edge_b->length) return 1;
    if (edge_a->length < edge_b->length) return -1;
    return 0;
}

void init_edges() {
    int num_edges = 0;
    for (int i = 0; i < NUM_POINTS; i++) {
        for (int j = i + 1; j < NUM_POINTS; j++) {
            edges[num_edges].u = i;
            edges[num_edges].v = j;
            edges[num_edges].length = sqrt(
                pow(points[i].x - points[j].x, 2) +
                pow(points[i].y - points[j].y, 2) +
                pow(points[i].z - points[j].z, 2)
            );
            num_edges++;
        }
    }
    qsort(edges, num_edges, sizeof(Edge), compare_edges);
}

#pragma endregion

#pragma region DISJOINT-SET
//////////////////////////////////////////////////////////////////////////////

int clusters[NUM_POINTS];

// make set
void init_clusters() {
    for (int i = 0; i < NUM_POINTS; i++) {
        clusters[i] = i;
    }
}

// find set
int find_cluster(int i) {
    if (i == clusters[i]) {
        return i;
    }
    return clusters[i] = find_cluster(clusters[i]);
}

// unite sets
bool unite_clusters(int edge_idx, double *answer) {
    Edge edge = edges[edge_idx];
    int root_u = find_cluster(edge.u);
    int root_v = find_cluster(edge.v);

    if (root_u == root_v) return false;

    clusters[root_v] = root_u;
    *answer = points[edge.u].x * points[edge.v].x;
    return true;
}

#pragma endregion

#pragma region MAIN
//////////////////////////////////////////////////////////////////////////////

int main() {
    load_file();
    init_edges();
    init_clusters();

    int num_merges = 1;
    int edge_idx = 0;
    double answer;
    while (num_merges < NUM_MERGES && edge_idx < MAX_EDGES) {
        if (unite_clusters(edge_idx, &answer)) {
            num_merges++;
        }
        edge_idx++;
    }

    printf("%.0f\n", answer);
}

#pragma endregion
