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
void workout_timer_create(WorkoutTimerCallback callback);

/* Cancels the timer's repeated callback */
void workout_timer_cancel();

/* Returns the elapsed seconds for this workout timer */
int elapsed_seconds();
