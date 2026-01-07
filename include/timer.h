#pragma once
#include <pthread.h>
#include <stdbool.h>

typedef struct {
    int time_left;
    int initial_time;
    bool active;
    bool tick_event;        // Flag set to true every second to notify the UI
    bool has_thread;
    pthread_t thread;
    pthread_mutex_t mutex;  // Protects access to time_left and active
    pthread_cond_t cond;    // To interrupt the sleep when stopped
} Timer;

/**
 * Initializes the timer structure and the mutex.
 * @param timer Pointer to the timer.
 * @param seconds The countdown duration in seconds.
 */
void timer_init(Timer *timer, int seconds);


/**
 * Starts the countdown thread.
 * @param timer Pointer to the timer.
 * @return true if the thread started successfully, false otherwise.
 */
bool timer_start(Timer *timer);

/**
 * Stops the timer thread safely.
 * @param timer Pointer to the timer.
 */
void timer_stop(Timer *timer);

/**
 * Thread-safe getter for the remaining time.
 * @param timer Pointer to the timer.
 * @return The number of seconds left.
 */
int timer_get_remaining(Timer *timer);

/**
 * Checks and consumes the tick event (resetting it to false).
 * Useful for the main loop to know when to redraw.
 * @param timer Pointer to the timer.
 * @return true if a second has passed since the last check.
 */
bool timer_consume_tick(Timer *timer);