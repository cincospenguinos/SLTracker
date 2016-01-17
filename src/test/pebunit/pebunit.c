#include <pebble.h>
#include "pebunit.h"

static int total_tests = 0;
static int assertions = 0;
static int assertions_failed = 0;

bool expect_true(bool value, char *description){
	APP_LOG(APP_LOG_LEVEL_INFO, "%s", description);
	if(!(value)) {
		APP_LOG(APP_LOG_LEVEL_ERROR, "Assertion failed! Expected <true> but was <false>");

		assertions_failed++;
	}

	assertions++;

	return value;
}

void run_test(PEB_TEST test){
	test();
	total_tests++;
}

void report(){
	APP_LOG(APP_LOG_LEVEL_INFO, "%i tests %i assertions %i failures", total_tests, assertions, assertions_failed);
}