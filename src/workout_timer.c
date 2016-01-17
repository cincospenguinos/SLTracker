#include "headers/workout_timer.h"

static AppTimer *timer = NULL;
static int seconds;
static WorkoutTimerCallback external_callback = NULL;

static void workout_timer_internal_callback(){
  external_callback(++seconds);
  timer = app_timer_register(1000, workout_timer_internal_callback, NULL);
}

void workout_timer_create(WorkoutTimerCallback callback){
  timer = app_timer_register(1000, workout_timer_internal_callback, NULL);
  external_callback = callback;
}

void workout_timer_cancel(){
  app_timer_cancel(timer);
}

int elapsed_seconds(){
  return seconds;
}
