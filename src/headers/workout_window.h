#pragma once
#include <pebble.h>

extern Window *workout_window;

/* Initializes workout window */
void workout_window_init();

/* Frees up the workout window */
void workout_window_deinit();

/* Loads up the workout_window into memory */
void workout_window_load();

/* Unloads the workout window's elements from memory */
void workout_window_unload();
