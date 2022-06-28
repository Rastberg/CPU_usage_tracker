#ifndef QUEUE_H

#define QUEUE_H
typedef struct Queue Queue;

Queue *queueCreate();
void queueDestroy(Queue *q);

int queue_en(Queue *q,const void  *element);
int queue_de(Queue *q,void *element);

#endif