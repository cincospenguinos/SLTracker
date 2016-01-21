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

/* Defines the clicks for the workout window */
void workout_window_click_config_provider();

/* Adds one to the current set of reps */
void add_one_current_reps(ClickRecognizerRef recognizer, void *context);

/* Subtracts one from the current set of reps */
void subtract_one_current_reps(ClickRecognizerRef recognizer, void *context);

/* Move on to the next set */
void go_to_next_set(ClickRecognizerRef recognizer, void *context);

/* Move back to the previous set */
void go_to_previous_set(ClickRecognizerRef recognizer, void *context);

/* The amount of time to wait before the next set. */
int wait_time();
