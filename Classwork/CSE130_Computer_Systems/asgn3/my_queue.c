#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "my_queue.h"

// typedef struct {
// 	int capacity;
// 	int size;
// 	int head;
// 	int tail;
// 	int buffer[capacity]
// } queue;

struct Queue *queue_new(int capacity) {
    struct Queue *q = NULL;
    q = (struct Queue *) malloc(sizeof(struct Queue));
    q->capacity = capacity;
    q->size = 0;
    q->head = 0;
    q->tail = 0;
    q->buffer = (int *) malloc(capacity * sizeof(int));
    return q;
}

bool enqueue(struct Queue *q, int a) {
    if (q->capacity == q->size) {
        return false;
    }
    q->buffer[q->head] = a;
    q->head = (q->head + 1) % q->capacity;
    q->size++;
    return true;
}

int dequeue(struct Queue *q) {
    if (q->size == 0) {
        return -1;
    }
    int rtn = q->buffer[q->tail];
    q->tail = (q->tail + 1) % q->capacity;
    q->size--;
    return rtn;
}

bool queue_full(struct Queue *q) {
    return (q->capacity == q->size);
}

bool queue_empty(struct Queue *q) {
    return (q->size == 0);
}

void queue_free(struct Queue *q) {
    free(q->buffer);
    free(q);
    return;
}

void queue_print(struct Queue *q) {
    //printf("TEST");
    //return;
    printf("Capacity: %d\n", q->capacity);
    printf("Size: %d\n", q->size);
    printf("Tail: %d\n", q->tail);
    printf("Head: %d\n", q->head);
    printf("[");
    for (int i = 0; i < q->size; i++) {
        printf("%d,", q->buffer[(q->tail + i) % q->capacity]);
    }
    printf("]\n");
    return;
}
