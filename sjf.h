#ifndef _SFJ_H_
#define _SFJ_H_

#include "data.h"
#include "memory.h"

void sjf(Processes* process_list, int list_size, int quantum, 
    char* memory_strat, Blocks* blocksContainer);
int allFinished(Processes* process_list, int list_size);

#endif