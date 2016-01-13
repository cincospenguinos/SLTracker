#pragma once
/**
 * workout.h
 *
 * Functions and definitions for a workout object. The collection of files
 * here follow the factory design pattern. One workout, modifiable via methods.
 */

/* Workout struct. */
typedef struct Workout{
  uint16_t weight[3]; // Weight for each exercise

  uint8_t ex_sets1[5]; // exercise sets - number between 0 and 5 for each index
  uint8_t ex_sets2[5];
  uint8_t ex_sets3[5];

  int mday; // The day of the month
  int month; // The month of the year
  int year; // Number of years since 1900

  bool day_type; // False = B day, ALWAYS.
}__attribute__((__packed__)) Workout;

/*
 * Adapter functions for workout_window
 */

/* Creates a workout with today's date. */
void create_new_workout();

/* Returns the name of the exercise requested of the current workout*/
const char * get_current_exercise_name();

/* Returns the weight of the current exercise */
int get_current_exercise_weight();

/* Returns the current working set of the current exercise */
int get_current_set();

/* Get the rep count of the current exercise given some set number */
int get_rep_count(int);

/* Returns the rep count of the current set. */
int get_current_rep_count();

/* Moves to next set. Returns the number of the next set (i.e. 1-5), 6 for the next exercise and 7 if the exercise is over*/
int next_set();

/* Moves to the previous set. Returns the number of the set (i.e. 1-5), 0 for the previous exercise and -1 if the workout is quit instead. */
int previous_set();

/* Stores the workout at the next available space in persistent data. */
void store_workout();

/* Initializes the persistent data setup */
void initialize_persistent_data();

/* Deletes all persistent data associated with this app. */
void delete_all_data();

