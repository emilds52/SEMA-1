#include <stddef.h>
#include "fsm.h"

#define TimeV 100
#define TimeA 50
#define TimeR 75
//TODO: como calculamos now? Como generamos entradas? Desde barra comandos o probamos en uc?

/* USER CODE BEGIN PV */
enum semaphoreFSM {
  s_Vp,
  s_Ap,
  s_Rp,
  s_Vs,
  s_As,
  s_Rs
};

//Variables declaration
static int  Ap, Rp, SPp = 0;
static int Vs, As, SPs = 0;
static int Vp, Rs = 1;
static int E, Bs, Bp = 0; //inputs, modelamos?
static int next = ;// now?

// Guard functions
static int evaluate_to_Ap (fsm_t* this){return (now > next) && (E == 1 || Bp == 1);}
static int evaluate_next  (fsm_t* this){return (now > next);}
static int evaluate_to_As (fsm_t* this){return (now > next) && (E == 0 || Bs == 1);}

// Output functions
static void to_Vp(fsm_t* this)
{
  Vp = 1; Rp = 0;
  next += TimeV; 
}

static void to_Ap(fsm_t* this)
{
  Ap = 1; Vp = 0;
  next += TimeA; 
}

static void to_Rp(fsm_t* this)
{
  Ap = 0; Rp = 1;
  next += TimeR; 
}

static void to_Vs(fsm_t* this)
{
  Vs = 1; Rs = 0;
  next += TimeV; 
}

static void to_As(fsm_t* this)
{
  As = 1; Vs = 0;
  next += TimeA; 
}

static void to_Rs(fsm_t* this)
{
  As = 0; Rs = 1;
  next += TimeR; 
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

