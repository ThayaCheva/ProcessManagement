#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "data.h"
#include "sjf.h"
#include "rr.h"
#include "memory.h"

int main(int argc, char *argv[]) {
    char *filename;
    char *scheduler;
    char *memory_strat;
    int quantum;

    Processes *process_list = NULL;
    int list_size = 0;
    
    // Read in the input
    for (int i = 1; i < argc; i++) {
        // Read in the input file
        if (strcmp(argv[i], "-f") == 0) {
            filename = argv[i + 1];
            FILE *file = fopen(filename, "r");
            assert(file);
            Processes new_process;
            while (fscanf(file, "%d %s %d %d", &new_process.start_time, 
                new_process.process, &new_process.burst_time, 
                &new_process.memory) == 4) {   

                new_process.isReady = 1;
                new_process.finished = 1;
                process_list = realloc(process_list, (list_size + 1) * 
                    sizeof(Processes));
                process_list[list_size++] = new_process;
                
            }
            fclose(file);
        }
        else if (strcmp(argv[i], "-s") == 0) {
            scheduler = argv[i + 1];
        }
        else if (strcmp(argv[i], "-m") == 0) {
            memory_strat = argv[i + 1];
        }
        else if (strcmp(argv[i], "-q") == 0) {
            quantum = atoi(argv[i + 1]);
        }
    }

    // Generate Initial Block
    Blocks* blocksContainer = malloc(sizeof(Blocks));
    blocksContainer->blockCount = 0;
    Block block;
    block.start_location = 0;
    block.end_location = 2048;
    block.isInitialized = 0;
    block.allocatedTo = "";
    blocksContainer->blocks[0] = block;
    blocksContainer->blockCount++;

    if (strcmp(scheduler, "SJF") == 0) {
        sjf(process_list, list_size, quantum, memory_strat, blocksContainer);
    }

    if (strcmp(scheduler, "RR") == 0) {
        rr(process_list, list_size, quantum, memory_strat, blocksContainer);
    }

    free(blocksContainer);
    free(process_list);
}   