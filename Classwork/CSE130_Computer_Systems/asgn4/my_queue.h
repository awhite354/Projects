#include <stdio.h>
#include <stdbool.h>

struct Queue {
    int capacity;
    int size;
    int head;
    int tail;
    int *buffer;
};

struct Queue *queue_new(int capacity);

bool enqueue(struct Queue *q, int a);

int dequeue(struct Queue *q);

bool queue_full(struct Queue *q);

bool queue_empty(struct Queue *q);

void queue_free(struct Queue *q);

void queue_print(struct Queue *q);
