#include "queue.h"
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

// kolejka do przekazywania danych między wątkami (zawiera mutex i cond var)
// jest generyczna (trzeba podać rozmiar elementu), wykorzystuje FAM
struct Queue
{
    size_t max_elements;
    size_t element_size;
    size_t head;
    size_t tail;
    size_t size;
    pthread_mutex_t mutex;
    pthread_cond_t can_produce;
    pthread_cond_t can_consume;
    uint8_t buffer[];
};

 Queue *queueCreate(const size_t max_elements, const size_t element_size)
{
    if (max_elements == 0 || element_size == 0)
    {
        return NULL;
    }
    Queue *const q = calloc(1, sizeof(*q) + max_elements * element_size);
    if (q == NULL)
    {
        return NULL;
    }
    *q = (Queue){
        .max_elements = max_elements,
        .element_size = element_size,
        .mutex = PTHREAD_MUTEX_INITIALIZER,
        .can_produce = PTHREAD_COND_INITIALIZER,
        .can_consume = PTHREAD_COND_INITIALIZER};
    return q;
}

void queueDestroy(Queue *const q)
{
    if (q == NULL)
    {
        return;
    }
    free(q);
}

int queue_en(Queue *restrict q, const void *const restrict element)
{
    if (q == NULL || element == NULL)
    {
        return -1;
    }

    uint8_t *ptr = &q->buffer[q->head * q->element_size];
    memcpy(ptr, element, q->element_size);
    if (q->head >= q->max_elements)
    {
        q->head = 0;
    }
    q->size++;

    return 0;
}

int queue_de(Queue *restrict q, void *const restrict element)
{
    if (q == NULL || element == NULL)
    {
        return -1;
    }

    const uint8_t *const ptr = &q->buffer[q->tail * q->element_size];
    memcpy(element, ptr, q->element_size);
    q->tail++;
    if (q->tail >= q->max_elements)
    {
        q->tail = 0;
    }
    q->size--;
    return 0;
}