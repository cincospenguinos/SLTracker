#include "headers/workout_timer.h"

static AppTimer *timer = NULL;
static int seconds;
static WorkoutTimerCallback external_callback;
static bool run_timer = false;

static void workout_timer_internal_callback(){
  external_callback(++seconds);

  if(run_timer)
    timer = app_timer_register(1000, workout_timer_internal_callback, NULL);
}

void workout_timer_start(WorkoutTimerCallback callback){
  run_timer = true;
  timer = app_timer_register(1000, workout_timer_internal_callback, NULL);
  external_callback = callback;
}

void workout_timer_restart(){
  run_timer = false;
  timer = app_timer_register(1000, workout_timer_internal_callback, NULL);
}

void workout_timer_stop(){
  APP_LOG(APP_LOG_LEVEL_INFO, "Cancelling timer!");
  run_timer = false;
}

void workout_timer_cancel(){
  workout_timer_stop();
  seconds = 0;
}

int workout_timer_elapsed_seconds(){
  return seconds;
}

bool workout_timer_is_running(){
  return run_timer;
}
