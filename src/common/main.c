#include <pebble.h>
#include "../headers/workout_window.h"
#include "../headers/workout.h"

void init(){
  initialize_persistent_data();
  workout_window_init();
}

void deinit(){
  //delete_all_data();
  workout_window_deinit();
}

int main(){
  init();
  app_event_loop();
  deinit();
}
