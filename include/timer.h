#pragma once
#include <pthread.h>
#include <stdbool.h>


typedef struct {
    int time_left;
    bool initialized;
    bool active;
    pthread_t thread;
    pid_t pid_caller;
    int callback_signal;
} Timer;

bool init_timer(Timer* timer, int seconds, pid_t pid_caller, int callback_signal);
bool start_timer(Timer* timer);
bool stop_timer(Timer* timer);
int  get_time_left(Timer timer);