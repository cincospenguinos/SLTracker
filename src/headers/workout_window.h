#pragma once
#include <pebble.h>

/* Initializes workout window */
void workout_window_init();

/* Destroys the workout_window */
void workout_window_deinit();

/* Loads up the workout_window into memory */
void workout_window_load();

/* Unloads the workout window's elements from memory */
void workout_window_unload();
