// adept_unit_test.h - Adept unit test (aut_) macros.

#ifndef ADEPT_UNIT_TEST_H_INCLUDED
#define ADEPT_UNIT_TEST_H_INCLUDED

#include <stdio.h>
#include "astr.h"

/*
 * Based on minunit, with thanks!
 *
 * See: http://www.jera.com/techinfo/jtns/jtn002.html
 *
 * Changes:
 *
 *		Continues after test failure(s). The minunit stopped on the first failure.
 *		Counts test cases, passes, and failures.
 *		Uses an astr instance to store the failure message(s) for the report.
 *		Reports the result of the test suite.
 *
 * USAGE - 
 *
 * Put the following at global scope somewhere, accessible from each test 
 * function in the source file:
 * 
 *   int suite_runs;
 *   int suite_fails;
 *   int test_runs;
 *   int test_fails;
 *   astr *test_messages;
 * 
 * Put the following in a function that returns int:
 *
 *   aut_initialize_suite();
 *	  aut_run_test(test_func1); // Call the aut_run_test macro to run a function.
 *   aut_run_test(test_func2); //   In the test function, call the aut_assert
 *	  aut_run_test(test_func3); //   macro zero or more times as needed.
 *	  aut_report();
 *	  aut_terminate_suite();
 *	  aut_return();
 */

// Initialize a test suite.
#define aut_initialize_suite() do { suite_runs = 0; suite_fails = 0; test_messages = astr_create_empty(); } while (0)

// Run a test case and handle the result.
#define aut_run_test(test) do { test_runs = 0; test_fails = 0; \
	astr_set(test_messages, #test"()"); test(); test_runs++; suite_runs++; \
	if (test_fails > 0) { printf("FAIL: %s\n", test_messages->string); suite_fails++; } } while (0)

// Check a test result and save a failure message if it failed.
#define aut_assert(message, test) do { if (!(test)) astr_append(test_messages, ", "#message), test_fails++; } while (0)

// Report on a test suite.
#define aut_report() do { \
	if (suite_fails > 0) { printf("%40s: Pass: %d/%d %3.1f  Fail: %d/%d %3.1f\n", __FILE__, \
		suite_runs - suite_fails, suite_runs, ((suite_runs - suite_fails) / (float) suite_runs) * 100, \
		suite_fails, suite_runs, (suite_fails / (float) suite_runs) * 100); } \
	else { printf("%40s: All %d tests passed\n", __FILE__, suite_runs); } } while (0)

// Terminate a test suite.
#define aut_terminate_suite() do { test_messages = astr_free(test_messages); } while (0)

// Return 1 if there were failures in the suite, 0 if none.
#define aut_return() do { return(suite_fails > 0 ? 1 : 0); } while (0)

extern int test_runs;
extern int test_fails;
extern int suite_runs;
extern int suite_fails;
extern astr *test_messages;

#endif // ADEPT_UNIT_TEST_H_INCLUDED
