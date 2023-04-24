#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "data.h"

#define MAX_CAPACITY 2048


typedef struct Block {
    int start_location;
    int end_location;
    int isInitialized;
    char* allocatedTo;
} Block;

typedef struct Blocks {
    Block blocks[2048];
    int blockCount;
} Blocks;


int allocate_memory(int memory, Processes process_list, Blocks* 
    blocksContainer);
void deallocate_memory(Processes process, Blocks* blocksContainer);

#endif