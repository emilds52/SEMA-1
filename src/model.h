#ifndef SRC_MODEL_H_
#define SRC_MODEL_H_

#include "fsm.h"

fsm_t* model_fsm_new();
void update_variables(int E_up, int Bp_up, int Bs_up); 

#endif
