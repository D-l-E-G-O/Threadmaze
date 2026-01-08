#include "heap.h"
#include <stdlib.h>
#include <stdio.h>


static void heap_swap(HeapNode *a, HeapNode *b) {
    HeapNode tmp = *a; 
    *a = *b; 
    *b = tmp;
}

void heap_init(MinHeap *h, int initial_capacity) {
    if (initial_capacity <= 0) initial_capacity = 64;
    h->data = malloc(sizeof(HeapNode) * initial_capacity);
    if (!h->data) { 
        perror("malloc"); 
        exit(EXIT_FAILURE); 
    }
    h->size = 0;
    h->capacity = initial_capacity;
}

void heap_free(MinHeap *h) {
    free(h->data);
    h->data = NULL;
    h->size = 0;
    h->capacity = 0;
}

bool heap_empty(const MinHeap *h) {
    return h->size == 0;
}

void heap_push(MinHeap *h, HeapNode node) {
    if (h->size >= h->capacity) {
        h->capacity *= 2;
        h->data = realloc(h->data, sizeof(HeapNode) * h->capacity);
        if (!h->data) { 
            perror("realloc"); 
            exit(EXIT_FAILURE); 
        }
    }

    h->data[h->size] = node;
    int i = h->size++;

    while (i > 0) {
        int parent = (i - 1) / 2;
        if (h->data[parent].f_cost <= h->data[i].f_cost) break;
        heap_swap(&h->data[parent], &h->data[i]);
        i = parent;
    }
}

HeapNode heap_pop(MinHeap *h) {
    HeapNode top = h->data[0];
    h->data[0] = h->data[--h->size];

    int i = 0;
    while (1) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;

        if (left < h->size && h->data[left].f_cost < h->data[smallest].f_cost)
            smallest = left;
        if (right < h->size && h->data[right].f_cost < h->data[smallest].f_cost)
            smallest = right;
        if (smallest == i) break;

        heap_swap(&h->data[i], &h->data[smallest]);
        i = smallest;
    }

    return top;
}