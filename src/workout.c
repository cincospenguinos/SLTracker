#include <pebble.h>
#include "headers/workout.h"

#define UNIT_OF_MEASUREMENT 21213
#define SQUAT_DELOAD_KEY 21214
#define BENCH_DELOAD_KEY 21215
#define BENT_DELOAD_KEY 21216
#define OVERHEAD_DELOAD_KEY 21217
#define DEADLIFT_DELOAD_KEY 21218
#define NEXT_DAY_KEY 21219 // what the next workout is going to be ---> false is always B
#define SQUAT_WEIGHT_KEY 212110
#define BENCH_WEIGHT_KEY 212111
#define BENT_WEIGHT_KEY 212112
#define OVERHEAD_WEIGHT_KEY 212113
#define DEADLIFT_WEIGHT_KEY 212114
#define TOTAL_WORKOUTS 212115
#define WORKOUT_START_INDEX 212116

static Workout current_workout; // The model - what we're storing the data in
static int current_exercise;
static int current_set;

void create_new_workout(){
  current_set = 0;
  current_exercise = 0;

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

  for(int i = 0; i < 5; i++){
    current_workout.ex_sets1[i] = 5;
    current_workout.ex_sets2[i] = 5;
    current_workout.ex_sets3[i] = 5;
  }
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

bool is_day_type_A(){
  return current_workout.day_type;
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
    APP_LOG(APP_LOG_LEVEL_ERROR, "This code should never run. ---> get_current_rep_count()");
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

int add_rep(){
  int reps = -1;

  switch(current_exercise){
  case 0:
    if(current_workout.ex_sets1[current_set] < 5)
      current_workout.ex_sets1[current_set]++;
    
    reps = current_workout.ex_sets1[current_set];
    break;
  case 1:
    if(current_workout.ex_sets2[current_set] < 5)
      current_workout.ex_sets2[current_set]++;

    reps = current_workout.ex_sets2[current_set];
    break;
  case 2:
    if(current_workout.ex_sets3[current_set] < 5)
      current_workout.ex_sets3[current_set]++;

    reps = current_workout.ex_sets3[current_set];
    break;
  default:
    APP_LOG(APP_LOG_LEVEL_ERROR, "This code should never run. --> add_rep");
  }

  return reps;
}

int subtract_rep(){
  int reps = -1;

  switch(current_exercise){
  case 0:
    if(current_workout.ex_sets1[current_set] > 0)
      current_workout.ex_sets1[current_set]--;
    
    reps = current_workout.ex_sets1[current_set];
    break;
  case 1:
    if(current_workout.ex_sets2[current_set] > 0)
      current_workout.ex_sets2[current_set]++;

    reps = current_workout.ex_sets2[current_set];
    break;
  case 2:
    if(current_workout.ex_sets3[current_set] > 0)
      current_workout.ex_sets3[current_set]++;

    reps = current_workout.ex_sets3[current_set];
    break;
  default:
    APP_LOG(APP_LOG_LEVEL_ERROR, "This code should never run. --> subtract_rep");
  }

  return reps;
}

void store_current_workout(){
  store_workout(current_workout);
}

void store_workout(Workout workout){
  // Store the workout
  int index = persist_read_int(WORKOUT_START_INDEX);
  int total_workouts = persist_read_int(TOTAL_WORKOUTS);
  index += total_workouts;

  if(current_workout.day_type){ // If it's an A day, then it should be a B day next time
    persist_write_bool(NEXT_DAY_KEY, false);
  }
  else{
    persist_write_bool(NEXT_DAY_KEY, true);
  }

  persist_write_data(index, &workout, sizeof(workout));
  persist_write_int(TOTAL_WORKOUTS, total_workouts);

  // Store the weights as needed
  int ex_sum1 = 0;
  int ex_sum2 = 0;
  int ex_sum3 = 0;

  for(int i = 0; i < 5; i++){
    ex_sum1 += current_workout.ex_sets1[i];
    ex_sum2 += current_workout.ex_sets2[i];
    ex_sum3 += current_workout.ex_sets3[i];
  }

  if(ex_sum1 == 25){
    persist_write_int(SQUAT_WEIGHT_KEY, persist_read_int(SQUAT_WEIGHT_KEY) + 5);
  }

  if(ex_sum2 == 25){
    if(current_workout.day_type)
      persist_write_int(BENCH_WEIGHT_KEY, persist_read_int(BENCH_WEIGHT_KEY) + 5);
    else
      persist_write_int(OVERHEAD_WEIGHT_KEY, persist_read_int(OVERHEAD_WEIGHT_KEY) + 5);
  }

  if(ex_sum3 == 25 && current_workout.day_type){
    persist_write_int(BENT_WEIGHT_KEY, persist_read_int(BENT_WEIGHT_KEY) + 5);
  } else if(current_workout.ex_sets3[0] == 5 && !current_workout.day_type){
    persist_write_int(DEADLIFT_WEIGHT_KEY, persist_read_int(DEADLIFT_WEIGHT_KEY) + 10);
  }
}

void initialize_persistent_data(){
  if(!persist_exists(TOTAL_WORKOUTS)){
    persist_write_int(TOTAL_WORKOUTS, 0);
    persist_write_bool(NEXT_DAY_KEY, true);
    persist_write_int(SQUAT_WEIGHT_KEY, 45);
    persist_write_int(BENCH_WEIGHT_KEY, 45);
    persist_write_int(BENT_WEIGHT_KEY, 45);
    persist_write_int(OVERHEAD_WEIGHT_KEY, 45);
    persist_write_int(DEADLIFT_WEIGHT_KEY, 45);
  }
}

void delete_all_data(){
  int start = persist_read_int(WORKOUT_START_INDEX);
  for(int i = 0; i < persist_read_int(TOTAL_WORKOUTS); i++){
    persist_delete(start + i);
  }

  persist_delete(WORKOUT_START_INDEX);
  persist_delete(TOTAL_WORKOUTS);
  persist_delete(NEXT_DAY_KEY);
  persist_delete(SQUAT_WEIGHT_KEY);
  persist_delete(BENCH_WEIGHT_KEY);
  persist_delete(BENT_WEIGHT_KEY);
  persist_delete(OVERHEAD_WEIGHT_KEY);
  persist_delete(DEADLIFT_WEIGHT_KEY);
}
