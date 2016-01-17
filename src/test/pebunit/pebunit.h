/**
* pebbunit.h
*
* Simple test framework for pebble. I'm sick of not being able to test my model code,
* so making my own is my solution.
*/
#pragma once
// #include <pebble.h>

/* Definition of a pebble test */
typedef void (*PEB_TEST)();

/* Returns the result of value, asserting that value is true */
bool expect_true(bool value, char *description);

/* Returns the result of expected == actual, asserting that it is true */
bool expect_equal(int expected, int actual, char *description);

/* Runs the test provided */
void run_test(PEB_TEST test);

/* Provides report on all of the data gathered during testing */
void report();
