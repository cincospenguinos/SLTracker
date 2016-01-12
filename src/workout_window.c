#include "headers/workout_window.h"
#include "headers/workout.h"

static TextLayer *exercise_title; // What exercise the user is working on
static TextLayer *exercise_weight; // The amount of weight for the current exercise

// Should this be a pointer?
//static Workout current_workout;

void workout_window_init(){
  
}

void workout_window_deinit(){
  text_layer_destroy(exercise_title);
  text_layer_destroy(exercise_weight);
}
