#ifndef _DATA_H_
#define _DATA_H_

#include <stdlib.h>
#define MAX_CHAR 10
#define LINE_LEN 100

typedef struct Processes {
    int start_time;
    char process[MAX_CHAR];
    int burst_time;
    int memory;
    int isReady;
    int finished;
} Processes;

#endif