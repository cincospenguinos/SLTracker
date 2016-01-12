#include "headers/workout_window.h"

static TextLayer *exercise_title; // What exercise the user is working on
static TextLayer *exercise_weight; // The amount of weight for the current exercise

void workout_window_init(){
  
}

void workout_window_deinit(){
  text_layer_destroy(exercise_title);
}
