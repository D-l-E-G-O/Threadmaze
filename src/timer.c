#include "timer.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>


static void* timer_thread(void* arg) {
    Timer* timer = (Timer*)arg;
    while (timer->active && timer->time_left > 0) {
        sleep(1);
        timer->time_left--;
        if (!timer->active) break;
        if(kill(timer->pid_caller, timer->callback_signal)){
            perror("kill");
            break;
        }
    }
    timer->active = false;
    return NULL;
}

bool init_timer(Timer* timer, int seconds, pid_t pid_caller, int callback_signal) {
    if (!timer) return false;
    timer->time_left = seconds;
    timer->pid_caller = pid_caller;
    timer->callback_signal = callback_signal;
    timer->active = false;
    timer->initialized = true;
    return true;
}


bool start_timer(Timer* timer) {
    if (!timer) return false;
    if (timer->active) {
        stop_timer(timer);
    }
    if (!timer->initialized) {
        fprintf(stderr, "[ERROR] Timer must be initialized before start.\n");
        return false;
    }
    timer->active = true;
    if (pthread_create(&timer->thread, NULL, timer_thread, timer)) {
        perror("pthread_create");
        timer->active = false;
        return false;
    }
    return true;
}

bool stop_timer(Timer* timer) {
    if (!timer || !timer->active) return false;
    timer->active = false;
    //if (pthread_cancel(timer->thread) != 0) return false;
    if (timer->initialized) {
        if (pthread_join(timer->thread, NULL)) {
            perror("pthread_join");
            return false;
        }
    }
    return true;
}

int get_time_left(Timer timer) {
    return timer.initialized ? timer.time_left : -1;
}