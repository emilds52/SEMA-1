#include <bits/time.h>
#include <stddef.h>
#include <time.h>
#include "fsm.h"

#define NANO_SECOND_MULTIPLIER 1000000000L
const long TIMEV = 0.1*NANO_SECOND_MULTIPLIER; 
const long TIMEA = 0.05*NANO_SECOND_MULTIPLIER; 
const long TIMER = 0.075*NANO_SECOND_MULTIPLIER; 
//TODO: como calculamos now? Como generamos entradas? Desde barra comandos o probamos en uc?

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
    if (time->tv_nsec > 1000000000){
        time->tv_nsec -= 1000000000;
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
static int E, Bs, Bp = 0; //inputs, modelamos?
static struct timespec next;// now?

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
}

static void to_Ap(fsm_t* this)
{
    Ap = 1; Vp = 0;
    clock_gettime(CLOCK_MONOTONIC, &next);
    add_time_ns(&next, TIMEA);
}

static void to_Rp(fsm_t* this)
{
    Ap = 0; Rp = 1;
    clock_gettime(CLOCK_MONOTONIC, &next);
    add_time_ns(&next, TIMER);
}

static void to_Vs(fsm_t* this)
{
    Vs = 1; Rs = 0;
    clock_gettime(CLOCK_MONOTONIC, &next);
    add_time_ns(&next, TIMEV);
}

static void to_As(fsm_t* this)
{
    As = 1; Vs = 0;
    clock_gettime(CLOCK_MONOTONIC, &next);
    add_time_ns(&next, TIMEA);
}

static void to_Rs(fsm_t* this)
{
    As = 0; Rs = 1;
    clock_gettime(CLOCK_MONOTONIC, &next);
    add_time_ns(&next, TIMER);
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


