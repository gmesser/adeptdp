// test_astr_conversions.c - test conversions

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "astr.h"
#include "adept_unit_test.h"

int suite_runs;
int suite_fails;
int test_runs;
int test_fails;
astr *test_messages;
char error[256];

// ----------

void test_conversion_to_long(void) {
   char *good_integral = "123";
   char *good_integral_leading_pos = "+123";
   char *good_integral_leading_neg = "-123";
   char *bad_integral_trailing_pos = "123+";
   char *bad_integral_trailing_neg = "-123-";
   char *bad_integral = "12X";
	char *empty = "";
	long longvalue;
	int intvalue;
	astr *as;
	
	as = astr_create(good_integral);
	aut_assert("1 good_integral", strcmp(as->string, good_integral) == 0);
	longvalue = astr_to_long(as);
	aut_assert("2 good_integral", errno == 0);
	aut_assert("3 good_integral", longvalue == 123L);
	intvalue = (int) astr_to_long(as);
	aut_assert("4 good_integral", errno == 0);
	aut_assert("5 good_integral", intvalue == 123);

	as = astr_create(good_integral_leading_pos);
	aut_assert("1 good_integral_leading_pos", strcmp(as->string, good_integral_leading_pos) == 0);
	longvalue = astr_to_long(as);
	aut_assert("2 good_integral_leading_pos", errno == 0);
	aut_assert("3 good_integral_leading_pos", longvalue == 123L);
	intvalue = (int) astr_to_long(as);
	aut_assert("4 good_integral_leading_pos", errno == 0);
	aut_assert("5 good_integral_leading_pos", intvalue == 123);

	as = astr_create(good_integral_leading_neg);
	aut_assert("1 good_integral_leading_neg", strcmp(as->string, good_integral_leading_neg) == 0);
	longvalue = astr_to_long(as);
	aut_assert("2 good_integral_leading_neg", errno == 0);
	aut_assert("3 good_integral_leading_neg", longvalue == -123L);
	intvalue = (int) astr_to_long(as);
	aut_assert("4 good_integral_leading_neg", errno == 0);
	aut_assert("5 good_integral_leading_neg", intvalue == -123);

	as = astr_create(bad_integral_trailing_pos);
	aut_assert("1 good_integral_trailing_pos", strcmp(as->string, bad_integral_trailing_pos) == 0);
	longvalue = astr_to_long(as);
	aut_assert("2 good_integral_trailing_pos", errno == EINVAL);
	aut_assert("3 good_integral_trailing_pos", longvalue == 0L);
	intvalue = (int) astr_to_long(as);
	aut_assert("4 good_integral_trailing_pos", errno == EINVAL);
	aut_assert("5 good_integral_trailing_pos", intvalue == 0);

	as = astr_create(bad_integral_trailing_neg);
	aut_assert("1 good_integral_trailing_neg", strcmp(as->string, bad_integral_trailing_neg) == 0);
	longvalue = astr_to_long(as);
	aut_assert("2 good_integral_trailing_neg", errno == EINVAL);
	aut_assert("3 good_integral_trailing_neg", longvalue == 0L);
	intvalue = (int) astr_to_long(as);
	aut_assert("4 good_integral_trailing_neg", errno == EINVAL);
	aut_assert("5 good_integral_trailing_neg", intvalue == 0);

	as = astr_create(bad_integral);
	aut_assert("1 bad_integral", strcmp(as->string, bad_integral) == 0);
	longvalue = astr_to_long(as);
	aut_assert("2 bad_integral", errno == EINVAL);
	aut_assert("3 bad_integral", longvalue == 0);
	intvalue = (int) astr_to_long(as);
	aut_assert("4 bad_integral", errno == EINVAL);
	aut_assert("5 bad_integral", intvalue == 0);

	as = astr_create(empty);
	aut_assert("1 empty", strcmp(as->string, empty) == 0);
	longvalue = astr_to_long(as);
	aut_assert("2 empty", errno == EDOM);
	aut_assert("3 empty", longvalue == 0);
	intvalue = (int) astr_to_long(as);
	aut_assert("4 empty", errno == EDOM);
	aut_assert("5 empty", intvalue == 0);

	astr_free(as);
}

void test_conversion_to_double(void) {
   char *good_float = "123.45";
   char *good_float_leading_pos = "+123.45";
   char *good_float_leading_neg = "-123.45";
   char *bad_float_trailing_pos = "123.45+";
   char *bad_float_trailing_neg = "123.45-";
   char *bad_float = "12.45X";
	char *empty = "";
	double doublevalue;
	float floatvalue;
	astr *as;
	
	as = astr_create(good_float);
	aut_assert("1 good_float", strcmp(as->string, good_float) == 0);
	doublevalue = astr_to_double(as);
	aut_assert("2 good_float", errno == 0);
	aut_assert("3 cogood_floatnversion", doublevalue == 123.45);
	floatvalue = (float) astr_to_double(as);
	aut_assert("4 good_float", errno == 0);
	aut_assert("5 good_float", floatvalue == 123.45F);

	as = astr_create(good_float_leading_pos);
	aut_assert("1 good_float_leading_pos", strcmp(as->string, good_float_leading_pos) == 0);
	doublevalue = astr_to_double(as);
	aut_assert("2 good_float_leading_pos", errno == 0);
	aut_assert("3 good_float_leading_pos", doublevalue == 123.45);
	floatvalue = (float) astr_to_double(as);
	aut_assert("4 good_float_leading_pos", errno == 0);
	aut_assert("5 good_float_leading_pos", floatvalue == 123.45F);

	as = astr_create(good_float_leading_neg);
	aut_assert("1 good_float_leading_neg", strcmp(as->string, good_float_leading_neg) == 0);
	doublevalue = astr_to_double(as);
	aut_assert("2 good_float_leading_neg", errno == 0);
	aut_assert("3 good_float_leading_neg", doublevalue == -123.45);
	floatvalue = (float) astr_to_double(as);
	aut_assert("4 good_float_leading_neg", errno == 0);
	aut_assert("5 good_float_leading_neg", floatvalue == -123.45F);

	as = astr_create(bad_float_trailing_pos);
	aut_assert("1 good_float_trailing_pos", strcmp(as->string, bad_float_trailing_pos) == 0);
	doublevalue = astr_to_double(as);
	aut_assert("2 good_float_trailing_pos", errno == EINVAL);
	aut_assert("3 good_float_trailing_pos", doublevalue == 0.0);
	floatvalue = (float) astr_to_double(as);
	aut_assert("4 good_float_trailing_pos", errno == EINVAL);
	aut_assert("5 good_float_trailing_pos", floatvalue == 0.0F);

	as = astr_create(bad_float_trailing_neg);
	aut_assert("1 good_float_trailing_neg", strcmp(as->string, bad_float_trailing_neg) == 0);
	doublevalue = astr_to_double(as);
	aut_assert("2 good_float_trailing_neg", errno == EINVAL);
	aut_assert("3 good_float_trailing_neg", doublevalue == 0.0);
	floatvalue = (float) astr_to_double(as);
	aut_assert("4 good_float_trailing_neg", errno == EINVAL);
	aut_assert("5 good_float_trailing_neg", floatvalue == 0.0F);

	as = astr_create(bad_float);
	aut_assert("1 bad_float", strcmp(as->string, bad_float) == 0);
	doublevalue = astr_to_double(as);
	aut_assert("2 bad_float", errno == EINVAL);
	aut_assert("3 bad_float", doublevalue == 0.0);
	floatvalue = (float) astr_to_double(as);
	aut_assert("4 bad_float", errno == EINVAL);
	aut_assert("5 bad_float", floatvalue == 0.0F);

	as = astr_create(empty);
	aut_assert("1 empty", strcmp(as->string, empty) == 0);
	doublevalue = astr_to_double(as);
	aut_assert("2 empty", errno == EDOM);
	aut_assert("3 empty", doublevalue == 0.0);
	floatvalue = (float) astr_to_double(as);
	aut_assert("4 empty", errno == EDOM);
	aut_assert("5 empty", floatvalue == 0.0F);

	astr_free(as);
}

// ----------

int main(int argc, char *argv[]) {
	aut_initialize_suite();
	aut_run_test(test_conversion_to_long);
	aut_run_test(test_conversion_to_double);
	aut_report();
	aut_terminate_suite();
	aut_return();
}
