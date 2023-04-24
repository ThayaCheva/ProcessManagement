#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "memory.h"

// Tassk 3: Function to allocate memory to Processes
int allocate_memory(int memory, Processes process, Blocks* blocksContainer) {
    int smallest_hole = 99999;
    int capacity;
    // Find the smallest hole that can fit the process
    for (int i = 0; i < blocksContainer->blockCount; i++) {
        capacity = blocksContainer->blocks[i].end_location - 
            blocksContainer->blocks[i].start_location;
        if (strcmp(blocksContainer->blocks[i].allocatedTo, "") == 0) {
            if (capacity < smallest_hole && capacity >= memory) {
                smallest_hole = i;
            } 
        }
    }

    // If there are no holes that can fit the process
    if (smallest_hole == 99999) {
        return 0;
    }

    // Allocate the memory to the process
    Block bestBlock = blocksContainer->blocks[smallest_hole];
    int memoryLeft = bestBlock.end_location;
    bestBlock.end_location = bestBlock.start_location + memory;
    bestBlock.allocatedTo = strdup(process.process);

    Block remainingBlock;
    int assigned = 0;
    capacity = blocksContainer->blocks[smallest_hole].end_location - 
        blocksContainer->blocks[smallest_hole].start_location;

    // Calculate the remaining memory left
    if (blocksContainer->blocks[smallest_hole].isInitialized == 0) {
        bestBlock.isInitialized = 1;
        remainingBlock.start_location = bestBlock.end_location;
        remainingBlock.end_location = memoryLeft;
    }
    else {
        // Calculate memory when the allocate memory is less than the block
        if (memory < capacity) {
            remainingBlock.start_location = bestBlock.end_location;
            remainingBlock.end_location = remainingBlock.start_location + 
                blocksContainer->blocks[smallest_hole].end_location - memory;
        }
        else if (memory == capacity) {
            assigned = 1;
        }
    }
    if (assigned == 0) {
        remainingBlock.isInitialized = 0;
        remainingBlock.allocatedTo = "";
        blocksContainer->blocks[blocksContainer->blockCount] = remainingBlock;
        blocksContainer->blockCount++;
    }

    blocksContainer->blocks[smallest_hole] = bestBlock;

    // Put all the blocks into there correct location using Bubble Sort
    Block temp;
    for (int i = 0; i < blocksContainer->blockCount; i++) {
        for (int j = 0; j < blocksContainer->blockCount - i - 1; j++) {
            if (blocksContainer->blocks[j].start_location > 
                blocksContainer->blocks[j + 1].start_location && 
                blocksContainer->blocks[j].end_location > 
                blocksContainer->blocks[j + 1].end_location) {

                temp = blocksContainer->blocks[j];
                blocksContainer->blocks[j] = blocksContainer->blocks[j + 1];
                blocksContainer->blocks[j + 1] = temp;
            }
        }
    }

    return 1;
}

// Function to deallocate memory from a process as well as join holes
void deallocate_memory(Processes process, Blocks* blocksContainer) {
    // Deallocate the memory
    for (int i = 0; i < blocksContainer->blockCount; i++) {
        if (strcmp(blocksContainer->blocks[i].allocatedTo, process.process) 
            == 0) {
            free(blocksContainer->blocks[i].allocatedTo);
            blocksContainer->blocks[i].allocatedTo = "";
        }
    }

    Block newBlock;
    // If there are holes after this hole, join them
    for (int i = 0; i < blocksContainer->blockCount - 1; i++) {
        if (strcmp(blocksContainer->blocks[i].allocatedTo, "") == 0 && 
            strcmp(blocksContainer->blocks[i + 1].allocatedTo, "") == 0) {
            // Initialize new hole after joining two holes
            newBlock.start_location = blocksContainer->blocks[i].start_location;
            newBlock.end_location = blocksContainer->blocks[i + 1].end_location;
            newBlock.isInitialized = 0;
            newBlock.allocatedTo = "";
            blocksContainer->blocks[i] = newBlock;
            // Shift array after removing element
            for (int j = i + 1; j < blocksContainer->blockCount - 1; j++) {
                blocksContainer->blocks[j] = blocksContainer->blocks[j + 1];
            }
            blocksContainer->blockCount -= 1;
        }
    }

    // If there are holes before this hole, join them
    for (int i = 0; i < blocksContainer->blockCount; i++) {
        if (i > 0) {
            if (strcmp(blocksContainer->blocks[i].allocatedTo, "") == 0 && 
                strcmp(blocksContainer->blocks[i - 1].allocatedTo, "") == 0) {
                // Initialize new hole after joining two holes
                newBlock.start_location = 
                    blocksContainer->blocks[i - 1].start_location;
                    
                newBlock.end_location = 
                    blocksContainer->blocks[i].end_location;

                newBlock.isInitialized = 0;
                newBlock.allocatedTo = "";
                blocksContainer->blocks[i] = newBlock;
                // Shift array after removing element
                for (int j = i - 1; j < blocksContainer->blockCount - 1; j++) {
                    blocksContainer->blocks[j] = blocksContainer->blocks[j + 1];
                }
                blocksContainer->blockCount -= 1;
            }
        }
    }
}