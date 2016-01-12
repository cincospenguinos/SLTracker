#pragma once
/**
 * workout.h
 *
 * Functions and definitions for a workout object.
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

/* Creates a workout with today's date. */
Workout workout_create_today();

/* Returns the name of the exercise requested given the workout provided */
const char * get_exercise_name(Workout *, uint8_t);

/* Returns a workout at the index requested. */
Workout get_workout_at(int);

/* Stores the workout at the next available space in persistent data. */
void store_workout(Workout);

/* Initializes the persistent data setup */
void initialize_persistent_data();
