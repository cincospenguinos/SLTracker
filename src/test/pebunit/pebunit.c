#include <pebble.h>
#include "pebunit.h"

static int total_tests = 0;
static int assertions = 0;
static int assertions_failed = 0;

static bool assert(bool value){
  if(!(value)){
    assertions_failed++;
  }

  assertions++;

  return value;
}

bool expect_true(bool value, char *description){
  if(!(assert(value))){
    APP_LOG(APP_LOG_LEVEL_ERROR, "ASSERTION FAILED: %s", description);
    APP_LOG(APP_LOG_LEVEL_ERROR, "Expected <true> but received <false>");
  } 

  // I've decided not to print anything when an assertion passes.
  //else
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "Assertion passed: %s", description);

  return value;
}

bool expect_equal(int expected, int actual, char *description){
  if(!(assert(expected == actual))){
    APP_LOG(APP_LOG_LEVEL_ERROR, "EXPECT FAILED: %s", description);
    APP_LOG(APP_LOG_LEVEL_ERROR, "Expected <%d> but received <%d>", expected, actual);
  } 

  // Let's not pass anything if an expectation passes.
  //else
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "Assertion passed: %s", description);

  return expected == actual;
}

void run_test(PEB_TEST test){
  test();
  total_tests++;
}

void report(){
  if(assertions_failed > 0){
    APP_LOG(APP_LOG_LEVEL_ERROR, "### RESULT: %i tests %i assertions %i failures", total_tests, assertions, assertions_failed);
  } else {
    APP_LOG(APP_LOG_LEVEL_INFO, "### RESULT: %i tests %i assertions %i failures", total_tests, assertions, assertions_failed);
  }
}

static void report_true_expect(bool result){
  
}
static void report_equal_expect(bool result){

}
