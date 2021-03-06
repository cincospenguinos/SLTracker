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
#include "../common/workout.c"

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

  delete_all_data();
}

void test_next_set(){
  create_new_workout();

  for(int i = 0; i < 3; i++){
    for(int i = 1; i <= 5; i++){
      char buffer[24] = "Current set should be 1";
      snprintf(buffer, sizeof(buffer), "Current set should be %i", i);
      expect_equal(i, get_current_set(), buffer);
      next_set();
    }
  }

  delete_all_data();
}

void test_reps(){
  create_new_workout();

  if (!(expect_equal(5, get_current_rep_count(), "Current rep count is equal to 5.")))
    return;

  int reps = 5;

  for(int i = 0; i < 20; i++){
    switch(rand() % 2){
    case 0:
      add_rep();
      if(reps < 5)
	reps++;
      break;
    case 1:
      subtract_rep();
      if(reps > 0)
	reps--;
      break;
    default:
      break;
    }

    expect_equal(reps, get_current_rep_count(), "The amount of reps should equal each other.");
  }

  delete_all_data();
}

void test_store_workout_typeA(){
  initialize_persistent_data();
  create_new_workout();

  if(!expect_true(is_day_type_A(), "The current workout day is A."))
    return;

  for(int i = 0; i < 15; i++){
    next_set();
  }

  store_current_workout();

  // Now we should have workout type A stored away and we can assert that when we create
  // a new workout, various things should be different.

  create_new_workout();
  expect_true(!is_day_type_A(), "The current workout day is now B.");
  expect_equal(1, get_current_set(), "The current set is 1.");
  expect_equal(5, get_current_rep_count(), "The current rep count is 5.");
  expect_equal(50, get_current_exercise_weight(), "The current squat weight is 50 lbs");
  expect_true(strcmp(get_current_exercise_name(), "Squat") == 0, "The current exercise is Squats.");

  delete_all_data();
}

void test_wait_times(){
  initialize_persistent_data();
  create_new_workout();

  for(int i = 1; i <= 5; i++){
    switch(rand() % 2){
    case 0:
      // Set reps up to 5; wait time should be 90 seconds
      next_set();
      expect_equal(90, get_wait_time(), "Successful sets should have a wait time of 90 seconds.");
      break;
    case 1:
      // Set reps to < 5; wait time should be 300 seconds
      subtract_rep();
      next_set();
      expect_equal(300, get_wait_time(), "Failed sets should have a wait time of 300 seconds.");
      break;
    }
  }

  // Special case: when we change workouts, there should be no wait time.
  expect_equal(0, get_wait_time(), "Switching exercises should return no wait time.");
  delete_all_data();
}

void test_rep_count_under_five(){
  initialize_persistent_data();
  create_new_workout();

  for(int i = 0; i < 5; i++){
    next_set();
  }

  for(int i = 5; i >= 0; i--){
    int reps = get_current_rep_count();
    expect_equal(i, reps, "Rep counts should move downwards whenever subract_rep is called");

    subtract_rep();
  }

  for(int i = 0; i <= 5; i++){
    int reps = get_current_rep_count();
    expect_equal(i, reps, "Rep counts should move up whenever add_rep is called");

    add_rep();
  }
}

void test_deadlift_logic(){
  // TODO: This
}

void test_correct_weight_kilos(){
  create_new_workout();
  // TODO: This
}

void run_all_tests(){
  startup();
  run_test(test_create_workout);
  run_test(test_next_set);
  run_test(test_reps);
  run_test(test_store_workout_typeA);
  run_test(test_wait_times);
  run_test(test_deadlift_logic);
  run_test(test_correct_weight_kilos);
  run_test(test_rep_count_under_five);
  shutdown();
  report();
}

int main(int argc, char **argv) {
  run_all_tests();
  app_event_loop();
}
