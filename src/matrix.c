#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"

bool** init_bool_matrix(int width, int height) {
    if (width <= 0 || height <= 0) return NULL;
    bool** matrix = calloc(height, sizeof(*matrix));
    if (!matrix) {
        perror("calloc"); 
        exit(EXIT_FAILURE); 
    }

    for (int i = 0; i < height; i++) {
        matrix[i] = calloc(width, sizeof(**matrix));
        if (!matrix[i]) {
            perror("malloc");   
            // cleanup on allocation failure
            free_bool_matrix(matrix, i);
            exit(EXIT_FAILURE);
        }
    }
    return matrix;
}

void free_bool_matrix(bool** matrix, int height) {
    if (!matrix) return;
    for (int i = 0; i < height; i++) {
        free(matrix[i]);
    }
    free(matrix);
}