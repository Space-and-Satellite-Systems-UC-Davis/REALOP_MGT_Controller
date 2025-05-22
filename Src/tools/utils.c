#include "globals.h"

void empty_function()
{
    return;
}

void wait_with_timeout(bool (*continue_waiting)(), uint64_t timeout_ms) {
    while_timeout(empty_function, continue_waiting, timeout_ms);
}