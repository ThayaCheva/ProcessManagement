#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "sjf.h"


// Checks if all processes are finished
int allFinished(Processes* process_list, int list_size) {
    int count = 0;
    for (int i = 0; i < list_size; i++) {
        if (process_list[i].finished == 0) {
            count += 1;
        }
    }
    if (count == list_size) {
        return 0;
    }
    else {
        return 1;
    }
}

// Task 1: Function to perform Shortest Job First Scheduling
void sjf(Processes* process_list, int list_size, int quantum, 
    char* memory_strat, Blocks* blocksContainer) {

    int processes_left = list_size;
    int cycle = 0;
    int is_running = 1;
    int sim_time = cycle * quantum;
    float turn_time = 0;
    float max_overhead = 0;
    float avg_overhead = 0;
    float service_time;
    float time_ran;
    
    while (allFinished(process_list, list_size) != 0) {
        // Find the Process that is ready
        for (int i = 0; i < list_size; i++) {
            if (process_list[i].start_time <= sim_time && 
                process_list[i].isReady == 1 && process_list[i].finished == 1) {
                if (strcmp(memory_strat, "best-fit") == 0) {
                    if (allocate_memory(process_list[i].memory, 
                        process_list[i], blocksContainer) == 1) {
                        for (int j = 0; j < blocksContainer->blockCount; j++) {
                            if (strcmp(process_list[i].process, 
                                blocksContainer->blocks[j].allocatedTo) == 0) {
                                printf(
                                    "%d,READY,process_name=%s,assigned_at=%d\n", 
                                    sim_time, process_list[i].process,
                                    blocksContainer->blocks[j].start_location);
                            }
                        }
                        process_list[i].isReady = 0;
                    }
                }
                else {
                    process_list[i].isReady = 0;
                }
            }
        }

        int min_index = -1;
        int min_burst_time = 999;
        int min_start_time;

        // Find the shortest job first
        for (int i = 0; i < list_size; i++) {
            if (process_list[i].isReady == 0 && process_list[i].finished == 1) {
                if (process_list[i].burst_time < min_burst_time) {
                    min_index = i;
                    min_burst_time = process_list[i].burst_time;
                    min_start_time = process_list[i].start_time;
                }
                else if (process_list[i].burst_time == min_burst_time) {
                    if (process_list[i].start_time < min_start_time) {
                        min_index = i;
                        min_start_time = process_list[i].start_time;
                    }
                    else if(process_list[i].start_time == 
                        process_list[min_index].start_time) {
                        if (strcmp(process_list[i].process, 
                            process_list[min_index].process) < 0) {
                            min_index = i;
                        }
                    }
                }
            }
        }

        cycle += 1;
        sim_time = cycle * quantum; 
        time_ran += 1;

        // Start running the shortest process
        Processes shortest_process;
        if (is_running == 1 && process_list[min_index].finished == 1) {
            // check if there is only one process left to be run
            if (processes_left <= 0) {
                for (int i = 0; i < list_size; i++) {
                    if (process_list[i].isReady == 0) {
                        shortest_process = process_list[i];
                    }
                }
            }
            else {
                shortest_process = process_list[min_index];
            }

            printf("%d,RUNNING,process_name=%s,remaining_time=%d\n", 
                sim_time - quantum, shortest_process.process, 
                shortest_process.burst_time);
            is_running = 0;
            service_time = shortest_process.burst_time / (float)quantum;
            time_ran = 1;
        }
        
        // Check if the process is finished
        if (time_ran >= service_time && shortest_process.finished == 1) {
            for (int i = 0; i < list_size; i++) {
                if (strcmp(shortest_process.process, 
                    process_list[i].process) == 0 && shortest_process.start_time 
                    == process_list[i].start_time) {

                    int count = 0;
                    for (int i = 0; i < list_size; i++) {
                        if (process_list[i].isReady == 0) {
                            count += 1;
                        }
                    }

                    if (process_list[i].finished == 1) {
                        printf(
                            "%d,FINISHED,process_name=%s,proc_remaining=%d\n", 
                            sim_time, shortest_process.process, count - 1);
                        if (strcmp(memory_strat, "best-fit") == 0) {
                            deallocate_memory(shortest_process, 
                            blocksContainer);
                        }
                        processes_left -= 1;

                        // Calculate Turnaround time and Overhead
                        float cur_turn = sim_time - shortest_process.start_time;
                        float overhead = cur_turn / shortest_process.burst_time;
                        turn_time += cur_turn;
                        avg_overhead += overhead;

                        if (max_overhead < overhead) {
                            max_overhead = overhead;
                        }

                        process_list[i].finished = 0;
                        process_list[i].isReady = 1;
                        is_running = 1;
                    }
                }
            }
        }
    }

    printf("Turnaround time %.0f\n", ceil(turn_time / list_size));
    printf("Time overhead %.2f %.2f\n", max_overhead, avg_overhead/list_size);
    printf("Makespan %d\n", sim_time);
}
