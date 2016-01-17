/**
 * test_main.c
 *
 * Unit tests for the SL Pebble app. Main entry point and
 * main space for loading and running various test suites.
 * Built off of minunit, located on git hub at
 * https://github.com/siu/minunit
 */
#include <pebble.h>
#include "pebunit.h"

void some_test(){
	int bar = 0;
	int foo = 1;

	expect_true(bar == foo, "Bar does not equal foo!");
}

void run_all_tests(){
	run_test(some_test);
	report();
}

int main(int argc, char **argv) {
	run_all_tests();
	app_event_loop();
}









