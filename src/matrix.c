#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"

bool **bool_matrix_init(int width, int height) {
    if (width <= 0 || height <= 0) return NULL;
    bool **matrix = calloc(height, sizeof(*matrix));
    if (!matrix) {
        perror("calloc"); 
        exit(EXIT_FAILURE); 
    }

    for (int i = 0; i < height; i++) {
        matrix[i] = calloc(width, sizeof(**matrix));
        if (!matrix[i]) {
            perror("malloc");   
            // cleanup on allocation failure
            bool_matrix_free(matrix, i);
            exit(EXIT_FAILURE);
        }
    }
    return matrix;
}

void bool_matrix_free(bool **matrix, int height) {
    if (!matrix) return;
    for (int i = 0; i < height; i++) {
        free(matrix[i]);
    }
    free(matrix);
}