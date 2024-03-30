#include <stdio.h>
#include <time.h>
#include "fsm.h"
#include "model.h"

#define NANO_SECOND_MULTIPLIER 1000000000L
const clock_t FSM_TICK_DELAY = 0.1 * NANO_SECOND_MULTIPLIER;


int main(){
    fsm_t* semaphore_fsm = model_fsm_new();
    struct timespec next;
    clock_gettime(CLOCK_MONOTONIC, &next);

    while(1){
        fsm_fire(semaphore_fsm);
        next.tv_nsec = next.tv_nsec + FSM_TICK_DELAY;
        if (next.tv_nsec > 1000000000){
            next.tv_nsec = next.tv_nsec - 1000000000;
            next.tv_sec = next.tv_sec + 1;
        }
        printf("next: %ld\n", next.tv_nsec);
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next, NULL);
    }
}
