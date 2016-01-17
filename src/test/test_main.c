/**
 * test_main.c
 *
 * Unit tests for the SL Pebble app. Main entry point and
 * main space for loading and running various test suites.
 * Built off of minunit, located on git hub at
 * https://github.com/siu/minunit
 */
#include <pebble.h>
#include "pebunit/pebunit.h"
#include "../workout.c"

void startup(){
  initialize_persistent_data();
}

void shutdown(){
  delete_all_data();
}

void test_create_workout(){
  create_new_workout();

  expect_true(strcmp(get_current_exercise_name(), "Squat") == 0, "The first exercise is Squats.");
  expect_true(get_current_set() == 1, "The initial set amount is 1.");
  expect_equal(5, get_current_rep_count(), "The initial rep amount is 5.");
  expect_equal(45, get_current_exercise_weight(), "The initial exercise weight size is 45 lbs.");
}

void run_all_tests(){
  startup();
  run_test(test_create_workout);
  shutdown();
  report();
}

int main(int argc, char **argv) {
	run_all_tests();
	app_event_loop();
}









