#include <bits/time.h>
#include <stddef.h>
#include <stdio.h>
#include <time.h>
#include "fsm.h"

#define NANO_SECOND 1000000000L
const long TIMEV = 3*NANO_SECOND; 
const long TIMEA = 1*NANO_SECOND; 
const long TIMER = 2*NANO_SECOND; 

enum semaphoreFSM {
    s_Vp,
    s_Ap,
    s_Rp,
    s_Vs,
    s_As,
    s_Rs
};

void add_time_ns(struct timespec* time, long addition_ns){
    time->tv_nsec += addition_ns;
    while(time->tv_nsec > NANO_SECOND){
        time->tv_nsec -= NANO_SECOND;
        time->tv_sec++;
    }
}

int cmpg_time(struct timespec t1, struct timespec t2){
    return (t1.tv_sec > t2.tv_sec || (t1.tv_sec == t2.tv_sec && t1.tv_nsec > t2.tv_nsec)); 
}

//Variables declaration
static int  Ap, Rp, SPp = 0;
static int Vs, As, SPs = 0;
static int Vp, Rs = 1;
static int E, Bs, Bp = 0;
static struct timespec next;

// Guard functions
static int evaluate_to_Ap (fsm_t* this){
    struct timespec now; clock_gettime(CLOCK_MONOTONIC, &now);
    return cmpg_time(now, next) && (E || Bp);
}
static int evaluate_next  (fsm_t* this){
    struct timespec now; clock_gettime(CLOCK_MONOTONIC, &now);
    return cmpg_time(now, next);
}
static int evaluate_to_As (fsm_t* this){
    struct timespec now; clock_gettime(CLOCK_MONOTONIC, &now);
    return cmpg_time(now, next) && (!E || Bs);
}

// Output functions
static void to_Vp(fsm_t* this)
{
    Vp = 1; Rp = 0;
    clock_gettime(CLOCK_MONOTONIC, &next);
    add_time_ns(&next, TIMEV);
    printf("state = Vp\n");
}

static void to_Ap(fsm_t* this)
{
    Ap = 1; Vp = 0; Bp = 0;
    clock_gettime(CLOCK_MONOTONIC, &next);
    add_time_ns(&next, TIMEA);
    printf("state = Ap\n");
}

static void to_Rp(fsm_t* this)
{
    Ap = 0; Rp = 1;
    clock_gettime(CLOCK_MONOTONIC, &next);
    add_time_ns(&next, TIMER);
    printf("state = Rp\n");
}

static void to_Vs(fsm_t* this)
{
    Vs = 1; Rs = 0;
    clock_gettime(CLOCK_MONOTONIC, &next);
    add_time_ns(&next, TIMEV);
    printf("state = Vs\n");
}

static void to_As(fsm_t* this)
{
    As = 1; Vs = 0; Bs = 0;
    clock_gettime(CLOCK_MONOTONIC, &next);
    add_time_ns(&next, TIMEA);
    printf("state = As\n");
}

static void to_Rs(fsm_t* this)
{
    As = 0; Rs = 1;
    clock_gettime(CLOCK_MONOTONIC, &next);
    add_time_ns(&next, TIMER);
    printf("state = Rs\n");
}


//Explicit FSM description

static fsm_trans_t Semaphore[] = {
    {s_Vp , evaluate_to_Ap , s_Ap , to_Ap },
    {s_Ap , evaluate_next  , s_Rp , to_Rp },
    {s_Rp , evaluate_next  , s_Vs , to_Vs },
    {s_Vs , evaluate_to_As , s_As , to_As },
    {s_As , evaluate_next  , s_Rs , to_Rs },
    {s_Rs , evaluate_next  , s_Vp , to_Vp },
    {-1, NULL, -1, NULL},
};


fsm_t* model_fsm_new(){
    return fsm_new(Semaphore);
}


void update_variables(int E_up, int Bp_up, int Bs_up){
   E = E_up;
   if (Bp_up){Bp = 1;}
   if (Bs_up){Bs = 1;}
}
