#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "rr.h"
#include "sjf.h"

// Queue implementation from Foundation of Algorithm Lecture
// Creates a queue
Queue* createQueue() {
    Queue* new_queue = (Queue*)malloc(sizeof(Queue));
    assert(new_queue);
    new_queue->head = NULL;
    new_queue->foot = NULL;
    return new_queue;
}

// Enqueue process to queue
void enqueue(Queue* process_queue, Processes process) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    assert(new_node);
    new_node->process = process;
    new_node->next = NULL;
    if (process_queue->foot == NULL) {
        process_queue->head = new_node;
        process_queue->foot = new_node;
        return;
    }

    Node* current_process = process_queue->head;

    while (current_process != NULL) {
        if(strcmp(current_process->process.process, process.process) == 0) {
            return;
        }
        current_process = current_process -> next;
    }
    process_queue->foot->next = new_node;
    process_queue->foot = new_node;
}

// Dequeue process from queue
int dequeue(Queue* process_queue, Processes* process_list, int list_size) {
    if (process_queue->head == NULL) {
        return -1;
    }
    Node* temp = process_queue->head;
    process_queue->head = process_queue->head->next;

    if (process_queue->head == NULL) {
        process_queue->foot = NULL;
    }

    int removed_index;
    for (int i = 0; i < list_size; i++) {
        if (strcmp(temp->process.process, process_list[i].process) == 0) {
            removed_index = i;
        }
    }
    free(temp);
    return removed_index;
}

// Function to free memory from queue
void freeQueue(Queue* process_queue) {
    Node* curr = process_queue->head;
    while (curr != NULL) {
        Node* next = curr->next;
        free(curr);
        curr = next;
    }
}

// Task 2: Function to perform round robin scheduling
void rr(Processes* process_list, int list_size, int quantum, char* memory_strat,
     Blocks* blocksContainer) {
    Processes temp;
    // Bubble sort for shortest start time first
    for (int i = 0; i < list_size; i++) {
        for (int j = 0; j < list_size - i - 1; j++) {
            if (process_list[j].start_time > process_list[j + 1].start_time) {
                temp = process_list[j];
                process_list[j] = process_list[j + 1];
                process_list[j + 1] = temp;
            }
        }
    }

    Queue* process_queue = createQueue();
    int cycle = 0;
    int sim_time = cycle * quantum;
    int is_running = 1;
    int time_ran = -1;
    int running_index = -1;
    int isLast = 0;
    int proc_remaining = list_size;
    float turn_time = 0;
    float max_overhead = 0;
    float avg_overhead = 0;
    int service_time[list_size];
    int prev_value = -1;
    // Store all the service time for calculating turn time
    for (int i = 0; i < list_size; i++) {
        service_time[i] = process_list[i].burst_time;
    }

    // Run round robin
    while (allFinished(process_list, list_size) != 0) {
        Processes running_process;

        // Find the Process that is ready
        for (int i = 0; i < list_size; i++) {
            if (process_list[i].start_time <= sim_time && 
                process_list[i].isReady == 1 && process_list[i].finished == 1) {
                if (strcmp(memory_strat, "best-fit") == 0) {
                    if (allocate_memory(process_list[i].memory, process_list[i],
                         blocksContainer) == 1) {
                        for (int j = 0; j < blocksContainer->blockCount; j++) {
                            if (strcmp(process_list[i].process, 
                                blocksContainer->blocks[j].allocatedTo) == 0) {
                                printf("%d,READY,process_name=%s,assigned_at=%d\n", 
                                    sim_time, process_list[i].process, 
                                    blocksContainer->blocks[j].start_location);
                            }
                        }
                        process_list[i].isReady = 0;
                        enqueue(process_queue, process_list[i]);
                    }
                }
                else {
                    process_list[i].isReady = 0;
                    enqueue(process_queue, process_list[i]);
                }
            }
        }

        cycle += 1;
        time_ran += quantum;
        sim_time = cycle * quantum; 

        // Decrement the time remaining of each process
        if (running_index != - 1) {
             if (process_list[running_index].burst_time >= 0) {
                process_list[running_index].burst_time -= quantum;
            }
        }
        // Checks the process has run its quantum
        if (time_ran >= quantum) {
            if (process_list[running_index].finished == 1) {
                // Check if the process has finished otherwise enqueue again
                if (process_list[running_index].burst_time < 1) {
                    proc_remaining -= 1;
                    printf("%d,FINISHED,process_name=%s,proc_remaining=%d\n", 
                        sim_time - quantum, running_process.process, 
                        proc_remaining);

                    if (strcmp(memory_strat, "best-fit") == 0) {
                        deallocate_memory(process_list[running_index], blocksContainer);
                        for (int i = 0; i < list_size; i++) {
                            if (process_list[i].start_time <= sim_time && 
                                process_list[i].isReady == 1 && 
                                process_list[i].finished == 1) {
                                if (allocate_memory(process_list[i].memory, 
                                    process_list[i], blocksContainer) == 1) {
                                    for (int j = 0; j < blocksContainer->blockCount; 
                                        j++) {
                                        if (strcmp(process_list[i].process, 
                                            blocksContainer->blocks[j].allocatedTo) == 0) {
                                            printf("%d,READY,process_name=%s,assigned_at=%d\n", 
                                                sim_time - quantum, process_list[i].process, 
                                                blocksContainer->blocks[j].start_location);
                                        }
                                    }
                                    process_list[i].isReady = 0;
                                    enqueue(process_queue, process_list[i]);
                                }
                            }
                        }
                    }

                    process_list[running_index].finished = 0;
                    float cur_turn = sim_time - running_process.start_time - quantum;
                    float overhead = cur_turn / service_time[running_index];
                    turn_time += cur_turn;
                    avg_overhead += overhead;
                    if (max_overhead < overhead) {
                        max_overhead = overhead;
                    }
                }
                else {
                    enqueue(process_queue, running_process);
                }
                is_running = 1;
            }
        }
        // Starts running the process
        if (is_running == 1) {
            // Initilize the process
            int removed_index;
            if (isLast == 0 && proc_remaining > 0) {
                removed_index = dequeue(process_queue, process_list, list_size);
            }

            running_process = process_list[removed_index];
            running_index = removed_index;

            is_running = 0;
            time_ran = 0;
            // Print the running process if its not the previous process
            if (process_list[running_index].burst_time >= 1 && isLast == 0) {
                if (prev_value == -1) {
                    printf("%d,RUNNING,process_name=%s,remaining_time=%d\n", 
                        sim_time - quantum, running_process.process, 
                        running_process.burst_time);
                    prev_value = running_index;
                }
                else if (strcmp(process_list[prev_value].process, 
                    process_list[running_index].process) != 0) {
                    printf("%d,RUNNING,process_name=%s,remaining_time=%d\n", 
                        sim_time - quantum, running_process.process, 
                        running_process.burst_time);
                    prev_value = running_index;
                }

                if (proc_remaining == 1) {
                    isLast = 1;
                }
            }
        }

    }
    printf("Turnaround time %.0f\n", ceil(turn_time / list_size));
    printf("Time overhead %.2f %.2f\n", max_overhead, avg_overhead/list_size);
    printf("Makespan %d\n", sim_time - quantum);

    if (strcmp(memory_strat, "best-fit") == 0) {
        freeQueue(process_queue);
    }
    free(process_queue);
}