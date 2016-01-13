#include <pebble.h>
#include "headers/workout.h"

#define UNIT_OF_MEASUREMENT 7
#define NEXT_DAY_KEY 9 // what the next workout is going to be ---> false is always B
#define SQUAT_WEIGHT_KEY 10
#define BENCH_WEIGHT_KEY 11
#define BENT_WEIGHT_KEY 12
#define OVERHEAD_WEIGHT_KEY 13
#define DEADLIFT_WEIGHT_KEY 14
#define TOTAL_WORKOUTS 15
#define WORKOUT_START_INDEX 16

static Workout current_workout; // The model - what we're storing the data in
static int current_exercise;
static int current_set;



void create_new_workout(){
  current_workout.day_type = persist_read_bool(NEXT_DAY_KEY);
  current_workout.weight[0] = persist_read_int(SQUAT_WEIGHT_KEY);
  
  if(current_workout.day_type){ // If it's an A day
    current_workout.weight[1] = persist_read_int(BENCH_WEIGHT_KEY);
    current_workout.weight[2] = persist_read_int(BENT_WEIGHT_KEY);
  } else { // If it's a B day
    current_workout.weight[1] = persist_read_int(OVERHEAD_WEIGHT_KEY);
    current_workout.weight[2] = persist_read_int(DEADLIFT_WEIGHT_KEY);
  }

  time_t temp = time(NULL);
  struct tm *current_time = localtime(&temp);
  
  current_workout.mday = current_time->tm_mday;
  current_workout.month = current_time->tm_mon;
  current_workout.year = current_time->tm_year + 1900; // We need to add on the extra years we have
}

const char * get_current_exercise_name(){
  switch(current_exercise){
  case 0:
    return "Squat";
  case 1:
    if(current_workout.day_type)
      return "Bench Press";
    else
      return "Overhead Press";
  case 2:
    if(current_workout.day_type)
      return "Bent Rows";
    else
      return "Deadlift";
  default:
    return "ERROR!!!";
  }
}

int get_current_exercise_weight(){
  return current_workout.weight[current_exercise];
}

int get_current_set(){
  return current_set + 1;
}

int get_current_rep_count(){
  switch(current_exercise){
  case 0:
    return (int)current_workout.ex_sets1[current_set];
  case 1:
    return (int)current_workout.ex_sets2[current_set];
  case 2:
    return (int)current_workout.ex_sets3[current_set];
  default:
    APP_LOG(APP_LOG_LEVEL_ERROR, "This code should never run.");
    return -1;
  }
}

int next_set(){
  if(++current_set == 5){
    current_set = 0;

    if(++current_exercise == 3){
      return 7; // We have finished the workout
    }

    return 6; // We have moved on to the next exercise
  }

  return get_current_set();
}

int previous_set(){
  if(--current_set == -1){
    current_set = 4;

    if(--current_exercise == -1){
      return -1; // We quit the exercise
    }

    return 0; // We moved on to the previous exercise
  }

  return get_current_set(); // We are zero indexed, so we need to add one to make up
}

/*
static Workout get_workout_at(int index){
  Workout workout;
  persist_read_data(WORKOUT_START_INDEX + index, &workout, sizeof(workout));
  return workout;
}
*/

void store_workout(Workout workout){
  int index = persist_read_int(WORKOUT_START_INDEX);
  int total_workouts = persist_read_int(TOTAL_WORKOUTS);
  index += total_workouts;

  persist_write_data(index, &workout, sizeof(workout));
  persist_write_int(TOTAL_WORKOUTS, total_workouts);
}

