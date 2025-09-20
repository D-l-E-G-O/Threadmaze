#pragma once
#include <pthread.h>


void* timer_loop(void* arg);
int get_time_left(void);
void reset_timer(int seconds);