#pragma once
#include <stdbool.h>

/**
 * Allocates a 2D boolean matrix initialized to false.
 * @param width Width of the matrix.
 * @param height Height of the matrix.
 * @return Pointer to the allocated 2D array, or NULL on failure.
 */
bool **bool_matrix_init(int width, int height);

/**
 * Frees a 2D boolean matrix.
 * @param matrix Pointer to the matrix to free.
 * @param height Height of the matrix (number of rows).
 */
void bool_matrix_free(bool **matrix, int height);