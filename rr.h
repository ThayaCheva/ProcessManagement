#ifndef _RR_H_
#define _RR_H_

#include "data.h"
#include "memory.h"


typedef struct Node {
    Processes process;
    struct Node* next;
} Node;

typedef struct Queue {
    Node* head;
    Node* foot;
} Queue;

Queue* createQueue();
void enqueue(Queue* process_queue, Processes process);
int dequeue(Queue* process_queue, Processes* process_list, int list_size);  

void rr(Processes* process_list, int list_size, int quantum, char* memory_strat, 
    Blocks* blocksContainer);

#endif