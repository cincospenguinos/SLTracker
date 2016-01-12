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

static Workout current_workout;

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

const char * get_exercise_name(uint8_t exercise){
  switch(exercise){
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
