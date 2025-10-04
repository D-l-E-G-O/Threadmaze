#pragma once
#include <stdbool.h>

bool** init_bool_matrix(int width, int height);
void free_bool_matrix(bool** matrix, int height);