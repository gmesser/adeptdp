// adept_unit_test.h - Adept unit test (aut_) macros.

#ifndef ADEPT_UNIT_TEST_H
#define ADEPT_UNIT_TEST_H

#include <stdio.h>
#include "astr.h"
#include "aclock.h"

/*
 * Based on minunit, with thanks!
 *
 * See: http://www.jera.com/techinfo/jtns/jtn002.html
 *
 * Changes:
 *
 *		Continues after test failure(s). The minunit stopped on the first failure.
 *		Counts test cases, passes, and failures.
 *		Uses an astr instance to store the failure message(s) for the suite.
 *		Uses an aclock instance to time the suite.
 *		Reports the result of the test suite.
 *
 * USAGE - 
 *
 * Put the following variables at global scope somewhere, accessible from each
 * test function in the source file:
 * 
 *		aclock *suite_clock;
 *		astr *suite_messages;
 *		int suite_runs;
 *		int suite_fails;
 *		int test_runs;
 *		int test_fails;
 * 
 * This example main function will initialize the test suite, run three test
 * functions, report the results, then clean up the suite.  It will return 1
 * if there were any failures in any of the the test functions, or 0 if all the
 * tests passed.
 *
 * int main(int argc, char *argv[]) {
 *   aut_initialize_suite();
 *	  aut_run_test(test_func1); // Call the aut_run_test macro to run a function.
 *   aut_run_test(test_func2); //   In the test function, call the aut_assert
 *	  aut_run_test(test_func3); //   macro zero or more times as needed.
 *	  aut_report();
 *	  aut_terminate_suite();
 *	  return(aut_return());
 * }
 *
 * This example test function has two calls to aut_assert to specify
 * the conditions to be evaluated and to specify the messages to be saved in
 * the case where the condition is false.
 *
 * void test_func1(void) {
 *		int result = 0;
 *
 *		// ...do something that updates result
 *		aut_assert("1st result was not zero!", result == 0);
 *
 *		// ...do something else that updates result
 *		aut_assert("2nd result was not zero!", result == 0);
 * }
 *
 * See the unit tests for aclock, afile, and astr for more examples.
 */

// Initialize a test suite.
#define aut_initialize_suite() do { suite_runs = 0; suite_fails = 0; suite_clock = aclock_create(); suite_messages = astr_create_empty(); } while (0)

// Run a test case and handle the result.
#define aut_run_test(test) do { test_runs = 0; test_fails = 0; \
	astr_set(suite_messages, #test"()"); test(); test_runs++; suite_runs++; \
	if (test_fails > 0) { printf("FAIL: %s\n", suite_messages->string); suite_fails++; } } while (0)

// Check a test result and save a failure message if it failed.
#define aut_assert(message, test) do { if (!(test)) astr_append(suite_messages, ", "#message), test_fails++; } while (0)

// Report on a test suite.
#define aut_report() do { \
	if (suite_fails > 0) { printf("%40s:  Pass: %d/%d %3.1f%%  Fail: %d/%d %3.1f%%  Elapsed: %g\n", __FILE__, \
		suite_runs - suite_fails, suite_runs, ((suite_runs - suite_fails) / (float) suite_runs) * 100, \
		suite_fails, suite_runs, (suite_fails / (float) suite_runs) * 100, aclock_elapsed(suite_clock)); } \
	else { printf("%40s:  All %2d tests passed  Elapsed: %g\n", __FILE__, suite_runs, aclock_elapsed(suite_clock)); } } while (0)

// Terminate a test suite.
#define aut_terminate_suite() do { suite_clock = aclock_free(suite_clock); suite_messages = astr_free(suite_messages); } while (0)

// Return 1 if there were failures in the suite, 0 if none.
#define aut_return() do { return(suite_fails > 0 ? 1 : 0); } while (0)

extern aclock *suite_clock;
extern astr *suite_messages;
extern int suite_runs;
extern int suite_fails;
extern int test_runs;
extern int test_fails;

#endif // ADEPT_UNIT_TEST_H
