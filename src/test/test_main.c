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

void test_create_workout_1(){
	create_new_workout();

	expect_true(get_current_set() == 1, "The initial set amount is 1.");
	expect_true(get_current_rep_count() == 5, "The initial rep amount is 5.");
}

void run_all_tests(){
	run_test(test_create_workout_1);
	report();
}

int main(int argc, char **argv) {
	run_all_tests();
	app_event_loop();
}









