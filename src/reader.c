#include "queue.h"
#include "data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STAT "/proc/stat"
#define BUFFER_SIZE 4096

void reader_t(Queue *q)
{
    FILE *f = fopen(STAT, "r");
    char usage[BUFFER_SIZE+1];

    if(f != NULL)
    {
        size_t len = fread(usage, sizeof(char), BUFFER_SIZE,f);
        usage[len] = '\0';
        fclose(f);
    }

    Data data = {.dataSize = strlen(usage),.dataInfo = usage};
    queue_en(q, &data);
}