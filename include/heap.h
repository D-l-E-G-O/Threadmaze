#pragma once
#include <stdbool.h>

typedef struct {
    int row, col;
    int g_cost; // cost from start up to this node
    int f_cost; // g_cost + h_cost
} HeapNode;

typedef struct {
    HeapNode *data;
    int size;
    int capacity;
} MinHeap;

/**
 * Initializes the min-heap.
 * @param h Pointer to the MinHeap structure.
 * @param initial_capacity Initial size of the heap storage.
 */
void heap_init(MinHeap *h, int initial_capacity);

/**
 * Frees the memory used by the heap.
 * @param h Pointer to the MinHeap structure.
 */
void heap_free(MinHeap *h);

/**
 * Checks if the heap is empty.
 * @param h Pointer to the MinHeap structure.
 * @return true if empty, false otherwise.
 */
bool heap_empty(const MinHeap *h);

/**
 * Pushes a new node onto the heap and maintains the heap property.
 * @param h Pointer to the MinHeap structure.
 * @param node The node to insert.
 */
void heap_push(MinHeap *h, HeapNode node);

/**
 * Removes and returns the node with the lowest f_cost from the heap.
 * @param h Pointer to the MinHeap structure.
 * @return The node with the minimum cost.
 */
HeapNode heap_pop(MinHeap *h);