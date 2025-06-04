#include "globals.h"


//Checks if the time hath come
//Also used for manually implementing timeout loops
bool is_time_out(uint64_t start_time, uint64_t timeout_ms) {
	uint64_t current_time = getSysTime();
	bool is_out = (WILL_LOOPS_TIMEOUT == true) && ((start_time + timeout_ms) <= current_time);
    return is_out;
}

//Continue repeating the do_work function as long as should_continue returns true. End if times out
void while_timeout(void (*do_work)(), bool (*should_continue)(), uint64_t timeout_ms) {
    uint64_t start_time = getSysTime(); //time in ms

    while (should_continue() == true) {
    	if (is_time_out(start_time, timeout_ms) == true) {
    		break;
    	}
        do_work();
    }
}


void empty_function()
{
    return;
}

void wait_with_timeout(bool (*continue_waiting)(), uint64_t timeout_ms) {
    while_timeout(empty_function, continue_waiting, timeout_ms);
}

