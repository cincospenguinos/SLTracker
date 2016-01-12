#include <pebble.h>
#include "headers/workout_window.h"

void init(){
  workout_window_init();
}

void deinit(){
  workout_window_deinit();


}

int main(){
  init();
  app_event_loop();
  deinit();
}
