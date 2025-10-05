#pragma once
#include <stdbool.h>

typedef struct {
    int row, col;
    int g_cost; // cost from start up to this node
    int f_cost; // g_cost + h_cost
} HeapNode;

typedef struct {
    HeapNode* data;
    int size;
    int capacity;
} MinHeap;

void heap_init(MinHeap* h, int initial_capacity);
void heap_free(MinHeap* h);
bool heap_empty(const MinHeap* h);
void heap_push(MinHeap* h, HeapNode node);
HeapNode heap_pop(MinHeap* h);