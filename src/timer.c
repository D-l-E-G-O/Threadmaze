#define _DEFAULT_SOURCE // For clock_gettime
#include "timer.h"
#include <stdio.h>
#include <time.h>       // For timespec
#include <errno.h>      // For ETIMEDOUT

static void *timer_thread_routine(void *arg) {
    Timer *timer = (Timer*)arg;
    
    pthread_mutex_lock(&timer->mutex);

    while (timer->active && timer->time_left > 0) {
        // Target time (now + 1 second)
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec += 1;

        // We wait until timeout or signal (stop)
        int rc = pthread_cond_timedwait(&timer->cond, &timer->mutex, &ts);

        if (rc == ETIMEDOUT) {
            // One second passed
            timer->time_left--;
            timer->tick_event = true;
        } 
        else {
            // We were awakened by timer_stop (cond_signal)
            // The while(timer->active) loop will end
        }
    }

    timer->active = false;
    // Case where the time passed naturally : we inform the UI
    if (timer->time_left <= 0) timer->tick_event = true;
    
    pthread_mutex_unlock(&timer->mutex);
    return NULL;
}

void timer_init(Timer *timer, int seconds) {
    if (!timer) return;
    timer->time_left = seconds;
    timer->initial_time = seconds;
    timer->active = false;
    timer->tick_event = false;
    timer->has_thread = false;

    pthread_mutex_init(&timer->mutex, NULL);
    pthread_cond_init(&timer->cond, NULL);
}

bool timer_start(Timer *timer) {
    if (!timer) return false;
    
    timer_stop(timer); // Ensure it's not already running

    pthread_mutex_lock(&timer->mutex);
    timer->active = true;
    timer->time_left = timer->initial_time;
    pthread_mutex_unlock(&timer->mutex);

    if (pthread_create(&timer->thread, NULL, timer_thread_routine, timer) != 0) {
        perror("pthread_create");
        pthread_mutex_lock(&timer->mutex);
        timer->active = false;
        pthread_mutex_unlock(&timer->mutex);
        return false;
    }

    timer->has_thread = true;
    return true;
}

void timer_stop(Timer *timer) {
    if (!timer) return;

    pthread_mutex_lock(&timer->mutex);
    // If the timer is active, we deactivate it and awaken the thread
    if (timer->active) {
        timer->active = false;
        pthread_cond_signal(&timer->cond); // Awakening
    }
    pthread_mutex_unlock(&timer->mutex);

    // Wait for termination
    if (timer->has_thread) {
        pthread_join(timer->thread, NULL);
        timer->has_thread = false;
    }
}

int timer_get_remaining(Timer *timer) {
    if (!timer) return 0;
    int remaining;
    pthread_mutex_lock(&timer->mutex);
    remaining = timer->time_left;
    pthread_mutex_unlock(&timer->mutex);
    return remaining;
}

bool timer_consume_tick(Timer *timer) {
    if (!timer) return false;
    bool tick = false;
    pthread_mutex_lock(&timer->mutex);
    if (timer->tick_event) {
        tick = true;
        timer->tick_event = false; // Reset flag
    }
    pthread_mutex_unlock(&timer->mutex);
    return tick;
}