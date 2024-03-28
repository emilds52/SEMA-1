#FSM_TICK_DELAY = 100

fsm_t* semaphore_fsm = fsm_new (Semaphore);
while(1){
	
	fsm_fire(semaphore_fsm);
    HAL_Delay(FSM_TICK_DELAY);
	
}