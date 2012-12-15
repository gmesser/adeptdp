// test_astr_comparisons.c - test the comparison functions

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "astr.h"
#include "aclock.h"
#include "adept_unit_test.h"

int suite_runs;
int suite_fails;
aclock *suite_clock;
int test_runs;
int test_fails;
astr *suite_messages;
char error[256];

// ----------

void test_equals(void) {
	char *str = "ABC";
	char *buf = "DEF   ";
	astr *as1;
	astr *as2;

	as1 = astr_create(str);
	as2 = astr_create(str);
	aut_assert("1 test_equal", astr_equals(as1, as2) == 1);

	as2 = astr_set_from_buffer(as2, buf, strlen(buf));
	aut_assert("1 test_equal", astr_equals(as1, as2) == 0);
	
	astr_free(as1);
	astr_free(as2);
}

void test_prefix_equals(void) {
	char *str =     "ABCDEF";
	char *strsame = "ABCXYZ   ";
	char *strdiff = "ABZZZZZZZ";
	astr *as1;
	astr *as2;

	as1 = astr_create(str);
	as2 = astr_create(strsame);
	aut_assert("1 same", astr_prefix_equals(as1, as2, 3) == 1);

	as2 = astr_set(as2, strdiff);
	aut_assert("1 different", astr_prefix_equals(as1, as2, 3) == 0);
	
	astr_free(as1);
	astr_free(as2);
}

void test_compare(void) {
	char *abc = "ABC";
	char *xyz = "XYZ";
	astr *as1 = NULL;
	astr *as2 = NULL;

	as1 = astr_set(as1, abc);
	as2 = astr_set(as2, xyz);
	aut_assert("1 test_compare", astr_compare(as1, as2) < 0);
	as1 = astr_append(as1, abc);
	as2 = astr_append(as2, xyz);
	aut_assert("2 test_compare", astr_compare(as1, as2) < 0);

	as1 = astr_set(as1, abc);
	as2 = astr_set(as2, abc);
	aut_assert("3 test_compare", astr_compare(as1, as2) == 0);
	as1 = astr_append(as1, abc);
	as2 = astr_append(as2, abc);
	aut_assert("4 test_compare", astr_compare(as1, as2) == 0);

	as1 = astr_set(as1, xyz);
	as2 = astr_set(as2, abc);
	aut_assert("5 test_compare", astr_compare(as1, as2) > 0);
	as1 = astr_append(as1, abc);
	as2 = astr_append(as2, xyz);
	aut_assert("6 test_compare", astr_compare(as1, as2) > 0);

	astr_free(as1);
	astr_free(as2);
}

void test_prefix_compare(void) {
	char *abcabc = "ABCABC";
	char *abcxyz = "ABCXYZ";
	char *xyzxyz = "XYZXYZ";
	astr *as1 = NULL;
	astr *as2 = NULL;

	as1 = astr_set(as1, abcabc);
	as2 = astr_set(as2, xyzxyz);
	aut_assert("1 less than", astr_prefix_compare(as1, as2, 3) < 0);

	as1 = astr_set(as1, abcabc);
	as2 = astr_set(as2, abcxyz);
	aut_assert("3 equal", astr_prefix_compare(as1, as2, 3) == 0);

	as1 = astr_set(as1, xyzxyz);
	as2 = astr_set(as2, abcxyz);
	aut_assert("5 greater than", astr_prefix_compare(as1, as2, 3) > 0);

	astr_free(as1);
	astr_free(as2);
}

void test_less_than(void) {
	char *abc = "ABC";
	char *xyz = "XYZ";
	astr *as1 = NULL;
	astr *as2 = NULL;

	as1 = astr_set(as1, abc);
	as2 = astr_set(as2, xyz);
	aut_assert("1 test_less_than", astr_less_than(as1, as2) == 1);

	as1 = astr_set(as1, abc);
	as2 = astr_set(as2, abc);
	aut_assert("2 test_less_than", astr_less_than(as1, as2) == 0);

	as1 = astr_set(as1, xyz);
	as2 = astr_set(as2, abc);
	aut_assert("3 test_less_than", astr_less_than(as1, as2) == 0);

	astr_free(as1);
	astr_free(as2);
}

void test_greater_than(void) {
	char *abc = "ABC";
	char *xyz = "XYZ";
	astr *as1 = NULL;
	astr *as2 = NULL;

	as1 = astr_set(as1, abc);
	as2 = astr_set(as2, xyz);
	aut_assert("1 test_greater_than", astr_greater_than(as1, as2) == 0);

	as1 = astr_set(as1, abc);
	as2 = astr_set(as2, abc);
	aut_assert("2 test_greater_than", astr_greater_than(as1, as2) == 0);

	as1 = astr_set(as1, xyz);
	as2 = astr_set(as2, abc);
	aut_assert("3 test_greater_than", astr_greater_than(as1, as2) == 1);

	astr_free(as1);
	astr_free(as2);
}

// ----------

int main(int argc, char *argv[]) {
	aut_initialize_suite();
	aut_run_test(test_equals);
	aut_run_test(test_prefix_equals);
	aut_run_test(test_compare);
	aut_run_test(test_prefix_compare);
	aut_run_test(test_less_than);
	aut_run_test(test_greater_than);
	aut_report();
	aut_terminate_suite();
	aut_return();
}
