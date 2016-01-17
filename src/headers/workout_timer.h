/**
 * workout_timer.h
 *
 * A simple wrapper of the AppTimer to better match the type of timer
 * needed for a single workout.
 */
#pragma once
#include <pebble.h>

/* Callback to be provided by the user that will be called over and over */
typedef void (*WorkoutTimerCallback)(int elapsed_seconds);

/* Register a callback to be run once per second*/
void workout_timer_start(WorkoutTimerCallback callback);

/* Starts the timer without resetting the seconds */
void workout_timer_restart();

/* Stops the workout timer without resetting the seconds*/
void workout_timer_stop();

/* Stops and resets the timer */
void workout_timer_cancel();

/* Returns the elapsed seconds for this workout timer */
int workout_timer_elapsed_seconds();

/* Returns true if the timer is running */
bool workout_timer_is_running();
