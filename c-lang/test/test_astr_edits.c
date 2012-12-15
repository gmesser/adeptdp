// test_astr_edits.c - test astr edit functions

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

char *upper =                        "A BCDE FGHI JKLM []{}|\\;:,.<>/?`-=~!@#$%^&*()_+ NOPQ RSTU VWXY Z";
char *lower =                        "a bcde fghi jklm []{}|\\;:,.<>/?`-=~!@#$%^&*()_+ nopq rstu vwxy z";
char *mixed =                        "A Bcde Fghi Jklm []{}|\\;:,.<>/?`-=~!@#$%^&*()_+ Nopq Rstu Vwxy Z";
char *leading =          " \t\n\v\f\r A BCDE FGHI JKLM []{}|\\;:,.<>/?`-=~!@#$%^&*()_+ NOPQ RSTU VWXY Z";
char *trailing =                     "A BCDE FGHI JKLM []{}|\\;:,.<>/?`-=~!@#$%^&*()_+ NOPQ RSTU VWXY Z \t\n\v\f\r ";
char *leading_trailing = " \t\n\v\f\r A BCDE FGHI JKLM []{}|\\;:,.<>/?`-=~!@#$%^&*()_+ NOPQ RSTU VWXY Z \t\n\v\f\r ";
char *dirty_string =         "  dirty   string  ";
char *packed_dirty_string =  " dirty string ";
char *cleaned_dirty_string = "dirty string";
char *forward_string = "A B C D E F";
char *reverse_string = "F E D C B A";

// ----------

void test_to_upper_case(void) {
	astr *aslower;
	astr *asupper;

	aslower = astr_create(lower);
	aut_assert("1 test creation", strcmp(aslower->string, lower) == 0);

	asupper = astr_create(upper);
	aut_assert("2 test creation", strcmp(asupper->string, upper) == 0);

	aslower = astr_to_upper_case(aslower);
	aut_assert("3 test to_upper", astr_equals(aslower, asupper) == 1);

	astr_free(aslower);
	astr_free(asupper);
}

void test_to_lower_case(void) {
	astr *aslower;
	astr *asupper;

	aslower = astr_create(lower);
	aut_assert("1 test creation", strcmp(aslower->string, lower) == 0);

	asupper = astr_create(upper);
	aut_assert("2 test creation", strcmp(asupper->string, upper) == 0);

	asupper = astr_to_lower_case(asupper);
	aut_assert("3 test to_lower", astr_equals(aslower, asupper) == 1);

	astr_free(aslower);
	astr_free(asupper);
}

void test_to_mixed_case(void) {
	astr *aslower;
	astr *asupper;
	astr *asmixed;

	aslower = astr_create(lower);
	aut_assert("1 test creation", strcmp(aslower->string, lower) == 0);

	asupper = astr_create(upper);
	aut_assert("2 test creation", strcmp(asupper->string, upper) == 0);

	asmixed = astr_create(mixed);
	aut_assert("3 test creation", strcmp(asmixed->string, mixed) == 0);

	asupper = astr_to_mixed_case(asupper);
	aut_assert("3 test to_mixed", astr_equals(asmixed, asupper) == 1);
	
	aslower = astr_to_mixed_case(aslower);
	aut_assert("4 test to_mixed", astr_equals(asmixed, aslower) == 1);

	astr_free(aslower);
	astr_free(asupper);
	astr_free(asmixed);
}

void test_left_trim(void) {
	astr *asleading;
	astr *astrailing;
	astr *asleading_trailing;
	astr *asupper;

	asleading = astr_create(leading);
	aut_assert("1 test creation", strcmp(asleading->string, leading) == 0);

	astrailing = astr_create(trailing);
	aut_assert("2 test creation", strcmp(astrailing->string, trailing) == 0);

	asleading_trailing = astr_create(leading_trailing);
	aut_assert("3 test creation", strcmp(asleading_trailing->string, leading_trailing) == 0);

	asupper = astr_create(upper);
	
	asleading = astr_left_trim(asleading);
	aut_assert("4 test left_trim", astr_equals(asleading, asupper) == 1);

	astr_free(asleading);
	astr_free(astrailing);
	astr_free(asleading_trailing);
	astr_free(asupper);
}

void test_right_trim(void) {
	astr *asleading;
	astr *astrailing;
	astr *asleading_trailing;
	astr *asupper;

	asleading = astr_create(leading);
	aut_assert("1 test creation", strcmp(asleading->string, leading) == 0);

	astrailing = astr_create(trailing);
	aut_assert("2 test creation", strcmp(astrailing->string, trailing) == 0);

	asleading_trailing = astr_create(leading_trailing);
	aut_assert("3 test creation", strcmp(asleading_trailing->string, leading_trailing) == 0);

	asupper = astr_create(upper);
	
	astrailing = astr_right_trim(astrailing);
	aut_assert("4 test right_trim", astr_equals(astrailing, asupper) == 1);

	astr_free(asleading);
	astr_free(astrailing);
	astr_free(asleading_trailing);
	astr_free(asupper);
}

void test_trim(void) {
	astr *asleading;
	astr *astrailing;
	astr *asleading_trailing;
	astr *asupper;

	asleading = astr_create(leading);
	aut_assert("1 test creation", strcmp(asleading->string, leading) == 0);

	astrailing = astr_create(trailing);
	aut_assert("2 test creation", strcmp(astrailing->string, trailing) == 0);

	asleading_trailing = astr_create(leading_trailing);
	aut_assert("3 test creation", strcmp(asleading_trailing->string, leading_trailing) == 0);
	
	asupper = astr_create(upper);

	asleading = astr_trim(asleading);
	aut_assert("4 test trim", astr_equals(asleading, asupper) == 1);

	astrailing = astr_trim(astrailing);
	aut_assert("5 test trim", astr_equals(astrailing, asupper) == 1);

	asleading_trailing = astr_trim(asleading_trailing);
	aut_assert("6 test trim", astr_equals(asleading_trailing, asupper) == 1);

	astr_free(asleading);
	astr_free(astrailing);
	astr_free(asleading_trailing);
	astr_free(asupper);
}

void test_not_empty(void) {
	astr *as;
	char *empty = "";
	char *space_str = " ";

	as = astr_create(lower);
	aut_assert("1 test creation", strcmp(as->string, lower) == 0);

	as = astr_not_empty(as);
	aut_assert("2 test not empty", strcmp(as->string, lower) == 0);

	as = astr_set(as, empty);
	as = astr_not_empty(as);
	aut_assert("3 test empty", strcmp(as->string, space_str) == 0);
}

void test_not_empty_char(void) {
	astr *as;
	char *empty = "";
	char space = ' ';
	char bang = '!';
	char *space_str = " ";
	char *bang_str = "!";

	as = astr_create(lower);
	aut_assert("1 test creation", strcmp(as->string, lower) == 0);

	as = astr_not_empty_char(as, bang);
	aut_assert("2 test not empty", strcmp(as->string, lower) == 0);

	as = astr_set(as, empty);
	as = astr_not_empty_char(as, space);
	aut_assert("3 test empty", strcmp(as->string, space_str) == 0);

	as = astr_set(as, empty);
	as = astr_not_empty_char(as, bang);
	aut_assert("4 test empty", strcmp(as->string, bang_str) == 0);
}

void test_pack_dirty_string(void) {
	astr *as;

	as = astr_create(dirty_string);
	aut_assert("1 test creation", strcmp(as->string, dirty_string) == 0);

	as = astr_pack(as);
	aut_assert("2 test pack", strcmp(as->string, packed_dirty_string) == 0);
}

void test_pack_packed_string(void) {
	astr *as;

	as = astr_create(packed_dirty_string);
	aut_assert("1 test creation", strcmp(as->string, packed_dirty_string) == 0);

	as = astr_pack(as);
	aut_assert("2 test pack", strcmp(as->string, packed_dirty_string) == 0);
}

void test_clean_dirty_string(void) {
	astr *as;

	as = astr_create(dirty_string);
	aut_assert("1 test creation", strcmp(as->string, dirty_string) == 0);

	as = astr_clean(as);
	aut_assert("2 test clean", strcmp(as->string, cleaned_dirty_string) == 0);
}

void test_clean_packed_string(void) {
	astr *as;

	as = astr_create(packed_dirty_string);
	aut_assert("1 test creation", strcmp(as->string, packed_dirty_string) == 0);

	as = astr_clean(as);
	aut_assert("2 test clean", strcmp(as->string, cleaned_dirty_string) == 0);
}

void test_clean_cleaned_string(void) {
	astr *as;

	as = astr_create(cleaned_dirty_string);
	aut_assert("1 test creation", strcmp(as->string, cleaned_dirty_string) == 0);

	as = astr_clean(as);
	aut_assert("2 test clean", strcmp(as->string, cleaned_dirty_string) == 0);
}

void test_reverse_string(void) {
	astr *as;

	as = astr_create(forward_string);
	aut_assert("1 test reverse", strcmp(as->string, forward_string) == 0);

	as = astr_reverse(as);
	aut_assert("2 test reverse", strcmp(as->string, reverse_string) == 0);

	as = astr_reverse(as);
	aut_assert("3 test reverse", strcmp(as->string, forward_string) == 0);
}

// ----------

int main(int argc, char *argv[]) {
	aut_initialize_suite();
	aut_run_test(test_to_upper_case);
	aut_run_test(test_to_lower_case);
	aut_run_test(test_to_mixed_case);
	aut_run_test(test_left_trim);
	aut_run_test(test_right_trim);
	aut_run_test(test_trim);
	aut_run_test(test_not_empty);
	aut_run_test(test_not_empty_char);
	aut_run_test(test_pack_dirty_string);
	aut_run_test(test_pack_packed_string);
	aut_run_test(test_clean_dirty_string);
	aut_run_test(test_clean_packed_string);
	aut_run_test(test_clean_cleaned_string);
	aut_run_test(test_reverse_string);
	aut_report();
	aut_terminate_suite();
	aut_return();
}
