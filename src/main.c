#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include "fsm.h"
#include "model.h"

#define NANO_SECOND_MULTIPLIER 1000000000L
const clock_t FSM_TICK_DELAY = 0.1 * NANO_SECOND_MULTIPLIER;

void* scan_input_func(void *ptr);

int main(){
    fsm_t* semaphore_fsm = model_fsm_new();
    struct timespec next;
    clock_gettime(CLOCK_MONOTONIC, &next);
    pthread_t t1;
    pthread_create(&t1, NULL, scan_input_func, NULL);
    while(1){
        fsm_fire(semaphore_fsm);
        next.tv_nsec = next.tv_nsec + FSM_TICK_DELAY;
        if (next.tv_nsec > 1000000000){
            next.tv_nsec = next.tv_nsec - 1000000000;
            next.tv_sec = next.tv_sec + 1;
        }
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next, NULL);
    }
}

void* scan_input_func(void *ptr){
    int E, Bp, Bs;
    printf("Input format: E Bp Bs\n");
    while (1){
        scanf("%d %d %d", &E, &Bp, &Bs);
        update_variables(E, Bp, Bs); 
    }
}
